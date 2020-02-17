#include "game.h"
#include "draw.h"
#include "debug.h"

#include <unistd.h> // usleep
#include <stdlib.h> // malloc

#define SECOND_IN_MIKROS 1000000

static t_game *game;

static void initialize()
{
    // Create snake at the center
    t_pos *center = (t_pos*)malloc(sizeof(t_pos));
    center->col = game->w_width / 2;
    center->row = game->w_height/ 2;

    game->snake = (t_snake*)malloc(sizeof(t_snake));
    game->snake->pos = center;
    game->snake->dir = LEFT;

    // Bucket of candy
    game->candy = (t_pos**)malloc(sizeof(t_pos*) * N_CANDY);
    for (int i = 0; i < N_CANDY; i++)
    {
        game->candy[i] = (t_pos*)malloc(sizeof(t_pos));
        game->candy[i]->row = 0;
    }

    // TEMP START
    game->candy[0]->row = 3;
    game->candy[0]->col = 20;

    t_pos *rrr = (t_pos*)malloc(sizeof(t_pos));
    rrr->row = game->snake->pos->row;
    rrr->col = game->snake->pos->col + 1;
    t_snake *rrrs = (t_snake*)malloc(sizeof(t_snake));
    rrrs->pos = rrr;

    t_pos *rrr2 = (t_pos*)malloc(sizeof(t_pos));
    rrr2->row = game->snake->pos->row;
    rrr2->col = game->snake->pos->col + 2;
    t_snake *rrr2s = (t_snake*)malloc(sizeof(t_snake));
    rrr2s->pos = rrr2;

    rrrs->next = rrr2s;
    game->snake->next = rrrs;
    // TEMP END
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
}

static void handle_input()
{
    char buf[1]; int n;
    if (!(n = read(0, buf, 1)))
        return;

    char input = buf[0];
    switch (input)
    {
        case 'j':
            if (game->snake->dir != UP)
                game->snake->dir = DOWN;
            break;
        case 'k':
            if (game->snake->dir != DOWN)
                game->snake->dir = UP;
            break;
        case 'h':
            if (game->snake->dir != RIGHT)
                game->snake->dir = LEFT;
            break;
        case 'l':
            if (game->snake->dir != LEFT)
                game->snake->dir = RIGHT;
            break;
    }
}

static void move_snake(t_snake *snake, t_snake *prev)
{
    // Recursive move to start with tail
    if (snake->next != NULL)
        move_snake(snake->next, snake);

    if (prev != NULL) {
        snake->pos->row = prev->pos->row;
        snake->pos->col = prev->pos->col;
        return;
    }

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


    // Check if snake is eating any candy
    for (int i=0; i < N_CANDY; i++)
    {
        if (game->snake->pos->col == game->candy[i]->col &&
            game->snake->pos->row == game->candy[i]->row)
        {
            debug("eat candy\n");
            game->candy[0]->row = 0;
            // TODO: Add points
        }
    }
}

static void update()
{
    debug("snake pos: %d, %d\n", game->snake->pos->col, game->snake->pos->row);
    handle_input();

    clear_snake(game->snake);
    move_snake(game->snake, NULL);
    handle_collision();

    draw_candy(game->candy);
    draw_snake(game->snake);
}

static void count_down()
{
    for (int i = 3; i > 0; i--)
    {
        draw_count_down(game->snake->pos, i);
        usleep(SECOND_IN_MIKROS);
    }
}

static void start()
{
    game->is_running = 1;
    initialize();
    clear_screen();
    draw_frame(game->w_width, game->w_height);
    count_down();

    while (game->is_running && !game->should_restart)
    {
        update();
        usleep((SECOND_IN_MIKROS) / (game->speed * 2));
    }

    if (game->should_restart)
    {
        game->should_restart = 0;
        start();
    }
}

static void stop()
{
    game->is_running = 0;
    destroy();
}

t_game* new_game(unsigned short rows, unsigned short cols)
{
    game = (t_game*)malloc(sizeof(t_game));
    game->w_width = cols;
    game->w_height = rows;
    game->speed = rows/5;
    if (game->speed > 10) game->speed = 10;
    game->start = &start;
    game->stop = &stop;

    return game;
}
