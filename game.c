#include "game.h"
#include "draw.h"

#include <unistd.h> // usleep
#include <stdlib.h> // malloc

#define SECOND_IN_MIKROS 1000000

static t_game *game;

static void initialize(t_pos *start)
{
    game->snake = (t_snake*)malloc(sizeof(t_snake));
    game->snake->pos = start;
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

    free(game);
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

static void update()
{
    handle_input();
    clear_snake(game->snake);
    move_snake(game->snake, NULL);
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
    game->running = 1;

    count_down();

    while (game->running)
    {
        update();
        usleep((2 * SECOND_IN_MIKROS) / (game->speed / 2));
    }
}

static void stop()
{
    game->running = 0;
    destroy();
}

t_game* new_game(int rows, int cols)
{
    game = (t_game*)malloc(sizeof(t_game));
    game->start = &start;
    game->stop = &stop;
    game->speed = rows;

    // Create snake at the center
    t_pos *pos = (t_pos*)malloc(sizeof(t_pos));
    pos->row = rows / 2;
    pos->col = cols / 2;

    initialize(pos);

    draw_frame(rows, cols);

    return game;
}
