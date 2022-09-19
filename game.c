#include "game.h"
#include "draw.h"
#include "input.h"
#include "debug.h"

#include <unistd.h> // usleep
#include <stdlib.h> // malloc, rand, etc.
#include <time.h> // time
#include <stdio.h> // putchar

#define SECOND_IN_MIKROS 1000000

// Get random number between [min, max)
static int rand_int(int min, int max)
{
    double r = (double)rand() / RAND_MAX;
    return (int)(r * max) + min;
}

static t_game *game;

static int highscore;

static void initialize()
{
    // Create snake at the center
    t_pos *center = (t_pos*)malloc(sizeof(t_pos));
    center->col = game->w_width / 2;
    center->row = game->w_height / 2;
    game->snake = (t_snake*)malloc(sizeof(t_snake));
    game->snake->pos = center;
    game->snake->dir = LEFT;

    t_pos *body_pos = (t_pos*)malloc(sizeof(t_pos));
    body_pos->row = game->snake->pos->row;
    body_pos->col = game->snake->pos->col + 1;
    t_snake *body = (t_snake*)malloc(sizeof(t_snake));
    body->pos = body_pos;
    body->next = NULL;
    game->snake->next = body;

    // Bucket of candy
    game->candy = (t_pos**)malloc(sizeof(t_pos*) * N_CANDY);
    for (int i = 0; i < N_CANDY; i++)
    {
        game->candy[i] = (t_pos*)malloc(sizeof(t_pos));
        game->candy[i]->row = 0;
    }

    game->points = 0;
}

static void free_snake(t_snake *snake)
{
    if (snake->next != NULL)
        free_snake(snake->next);

    free(snake->pos);
    free(snake);
}

static void destroy()
{
    free_snake(game->snake);

    for (int i = 0; i < N_CANDY; i++)
        free(game->candy[i]);

    free(game->candy);
}

static void handle_input()
{
    e_dir input = read_input();

    switch (input)
    {
        case DOWN:
            if (game->snake->dir != UP)
                game->snake->dir = DOWN;
            break;
        case UP:
            if (game->snake->dir != DOWN)
                game->snake->dir = UP;
            break;
        case LEFT:
            if (game->snake->dir != RIGHT)
                game->snake->dir = LEFT;
            break;
        case RIGHT:
            if (game->snake->dir != LEFT)
                game->snake->dir = RIGHT;
            break;
    }
}

static void move_snake(t_snake *snake, const t_snake *prev)
{
    // Recursive move to start with tail
    if (snake->next != NULL)
        move_snake(snake->next, snake);

    // Move part to position of prev
    if (prev != NULL) {
        snake->pos->row = prev->pos->row;
        snake->pos->col = prev->pos->col;
        return;
    }

    // Move head in set direction
    switch (snake->dir)
    {
        case LEFT:
            snake->pos->col--;
            break;
        case RIGHT:
            snake->pos->col++;
            break;
        case UP:
            snake->pos->row--;
            break;
        case DOWN:
            snake->pos->row++;
            break;
    }
}

static void grow() {
    t_snake *tail = game->snake->next;
    while (tail->next != NULL)
        tail = tail->next;

    t_pos *new_pos = (t_pos*)malloc(sizeof(t_pos));
    new_pos->row = tail->pos->row;
    new_pos->col = tail->pos->col;
    t_snake *new_tail = (t_snake*)malloc(sizeof(t_snake));
    new_tail->pos = new_pos;
    new_tail->next = NULL;
    tail->next = new_tail;
}

static void handle_collision()
{
    // Check if snake is hitting any walls
    if (game->snake->pos->col <= 1 ||
        game->snake->pos->col >= game->w_width ||
        game->snake->pos->row <= 1 ||
        game->snake->pos->row >= game->w_height)
    {
        debug("hit wall\n");
        game->should_restart = 1;
    }

    // Check if snake it hitting itself
    t_snake *body = game->snake->next;
    do
    {
        if (game->snake->pos->col == body->pos->col &&
            game->snake->pos->row == body->pos->row)
        {
            debug("hit body\n");
            game->should_restart = 1;
            break;
        }
    }
    while ((body = body->next) != NULL);

    // Check if snake is eating any candy
    for (int i=0; i < N_CANDY; i++)
    {
        if (game->snake->pos->col == game->candy[i]->col &&
            game->snake->pos->row == game->candy[i]->row)
        {
            debug("eat candy\n");
            game->candy[i]->row = 0;
            grow();
            game->points += game->speed * 10;
        }
    }
}

static void create_candy()
{
    int activeCandy = 0;
    for (int i = 0; i < N_CANDY; i++)
    {
        if (game->candy[i]->row > 0)
            activeCandy++;
    }

    // Create candy randomly, less often the more candy is available.
    int r = rand_int(0, 20 * activeCandy);
    if (r != 0 || activeCandy == N_CANDY) return;

    for (int i = 0; i < N_CANDY; i++)
    {
        // Find a free "spot"
        if (game->candy[i]->row == 0)
        {
            // Find a random position within the bounds of the frame
            int col = rand_int(2, game->w_width - 2);
            int row = rand_int(2, game->w_height - 2);
            debug("adding candy[%d] at %d,%d\n", i, col, row);
            game->candy[i]->col = col;
            game->candy[i]->row = row;

            break;
        }
    }
}

static void update()
{
    debug("snake pos: %d, %d\n", game->snake->pos->col, game->snake->pos->row);
    handle_input();

    clear_snake(game->snake);
    clear_statusbar(game->w_height + 1);
    move_snake(game->snake, NULL);
    handle_collision();
    create_candy();

    draw_candy(game->candy);
    draw_snake(game->snake);
    draw_statusbar(game->w_height + 1, game->speed, game->points, highscore);
}

static void clear_and_count_down()
{
    clear_screen();
    draw_frame(game->w_width, game->w_height);
    for (int i = 3; i > 0 && game->is_running; i--)
    {
        draw_count_down(game->snake->pos, i);
        usleep(SECOND_IN_MIKROS);
    }
}

static void beep()
{
    putchar('\a');
}

static void start()
{
    game->is_running = 1;

    initialize();
    clear_and_count_down();

    while (game->is_running)
    {
        if (game->should_restart)
        {
            game->should_restart = 0;
            beep();

            if (game->points > highscore)
            {
                highscore = game->points;
                draw_statusbar(game->w_height + 1, game->speed,
                               game->points, highscore);
            }

            destroy();

            t_pos center = {.col = game->w_width / 2, .row = game->w_height / 2};
            draw_game_over(&center);
            usleep(SECOND_IN_MIKROS);

            initialize();
            clear_and_count_down();
        }
        update();

        // Adjust the speed based on direction (since cells are rectangular and
        // the snake appears to move faster in vertical direction).
        float speed_factor =
            game->snake->dir == UP || game->snake->dir == DOWN ? 1.5 : 2;
        usleep((SECOND_IN_MIKROS) / (game->speed * speed_factor));
    }
}

static void stop()
{
    game->is_running = 0;
}

t_game* new_game(unsigned short rows, unsigned short cols)
{
    // Initialize random number generator with a unique seed
    srand(time(NULL));

    highscore = 0;

    game = (t_game*)malloc(sizeof(t_game));
    game->w_width = cols;
    game->w_height = rows - 1;
    game->speed = rows/5;
    if (game->speed > 10) game->speed = 10;
    game->start = &start;
    game->stop = &stop;

    return game;
}
