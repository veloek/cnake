#include "game.h"
#include "draw.h"
#include "input.h"
#include "debug.h"

#include <unistd.h> // usleep
#include <stdlib.h> // malloc, rand, etc.
#include <time.h> // time
#include <stdio.h> // putchar
#include <assert.h> // assert

#define SECOND_IN_MICROS 1000000
#define MILLI_IN_MICROS 1000

// Get random number between [min, max)
static int rand_int(int min, int max)
{
    double r = (double)rand() / RAND_MAX;
    return (int)(r * (max-min)) + min;
}

static t_game *game;

static unsigned int highscore;

static void initialize()
{
    game->snake_length = 2;

    // Create snake at the center
    t_pos center;
    center.col = game->w_width / 2;
    center.row = game->w_height / 2;
    game->snake[0].pos = center;
    game->snake[0].dir = LEFT;

    game->snake[1].pos.row = game->snake[0].pos.row;
    game->snake[1].pos.col = game->snake[0].pos.col + 1;
    game->snake[1].dir = LEFT;

    // Bucket of candy
    for (int i = 0; i < N_CANDY; i++)
    {
        game->candy[i].row = 0;
    }

    game->speed = 1;
    game->points = 0;
    game->is_paused = 0;
}

static void handle_input()
{
    e_input input = read_input();

    switch (input)
    {
        case INPUT_1: game->speed = 1; break;
        case INPUT_2: game->speed = 2; break;
        case INPUT_3: game->speed = 3; break;
        case INPUT_4: game->speed = 4; break;
        case INPUT_5: game->speed = 5; break;
        case INPUT_6: game->speed = 6; break;
        case INPUT_7: game->speed = 7; break;
        case INPUT_8: game->speed = 8; break;
        case INPUT_9: game->speed = 9; break;
        case INPUT_0: game->speed = 10; break;

        case INPUT_DOWN:
            if (game->snake->dir != UP && !game->is_paused)
                game->snake->dir = DOWN;
            break;
        case INPUT_UP:
            if (game->snake->dir != DOWN && !game->is_paused)
                game->snake->dir = UP;
            break;
        case INPUT_LEFT:
            if (game->snake->dir != RIGHT && !game->is_paused)
                game->snake->dir = LEFT;
            break;
        case INPUT_RIGHT:
            if (game->snake->dir != LEFT && !game->is_paused)
                game->snake->dir = RIGHT;
            break;
        case INPUT_PAUSE:
            game->is_paused = !game->is_paused;
            break;
        case INPUT_QUIT:
            game->is_running = 0;
            break;
    }
}

static void move_snake(t_snake *snake, int snake_length)
{
    for(int i=snake_length-1; i>0; --i)
    {
        snake[i].pos = snake[i-1].pos;
    }

    // Move head in set direction
    switch (snake[0].dir)
    {
        case LEFT:
            snake[0].pos.col--;
            break;
        case RIGHT:
            snake[0].pos.col++;
            break;
        case UP:
            snake[0].pos.row--;
            break;
        case DOWN:
            snake[0].pos.row++;
            break;
    }
}

static void grow() {
    // Ensure we have capacity to grow
    assert(game->snake_length < game->snake_capacity);

    t_snake *tail = &game->snake[game->snake_length];

    t_pos new_pos;
    new_pos.row = tail->pos.row;
    new_pos.col = tail->pos.col;
    tail->pos = new_pos;

    game->snake_length++;
}

static inline t_bool t_pos_equals(const t_pos *a, const t_pos *b)
{
    return (a->row == b->row && a->col == b->col);
}

static void handle_collision()
{
    // Check if snake is hitting any walls
    if (game->snake[0].pos.col <= 1 ||
        game->snake[0].pos.col >= game->w_width ||
        game->snake[0].pos.row <= 1 ||
        game->snake[0].pos.row >= game->w_height)
    {
        debug("hit wall\n");
        game->should_restart = 1;
    }

    // Check if snake it hitting itself
    for(unsigned int i = 1; i < game->snake_length; i++)
    {
        if (t_pos_equals(&game->snake[0].pos, &game->snake[i].pos))
        {
            debug("hit body\n");
            game->should_restart = 1;
            break;
        }
    }

    // Check if snake is eating any candy
    for (int i = 0; i < N_CANDY; i++)
    {
        if (t_pos_equals(&game->snake[0].pos, &game->candy[i]))
        {
            debug("eat candy\n");
            game->candy[i].row = 0;
            grow();
            game->points += game->speed * 10;
        }
    }
}

static t_bool t_pos_is_taken(const t_pos *pos)
{
    // Snake
    for (unsigned int i = 0; i < game->snake_length; i++)
    {
        if (t_pos_equals(pos, &game->snake[i].pos))
            return 1;
    }

    // Candy
    for (int i = 0; i < N_CANDY; i++)
    {
        if (t_pos_equals(pos, &game->candy[i]))
            return 1;
    }

    return 0;
}

static void create_candy()
{
    int activeCandy = 0;
    for (int i = 0; i < N_CANDY; i++)
    {
        if (game->candy[i].row > 0)
            activeCandy++;
    }

    // Return early if there's no room for more candy
    if (activeCandy == N_CANDY)
        return;

    // Create candy randomly, less often the more candy is available.
    int r = rand_int(0, 20 * activeCandy);
    if (r != 0)
        return;


    // Find available space for the new candy
    t_pos pos;
    do
    {
        // Find a random position within the bounds of the frame
        pos.col = rand_int(2, game->w_width - 2);
        pos.row = rand_int(2, game->w_height - 2);
    }
    while (t_pos_is_taken(&pos));

    for (int i = 0; i < N_CANDY; i++)
    {
        // Find a free "spot"
        if (game->candy[i].row == 0)
        {
            debug("adding candy[%d] at %d,%d\n", i, pos.col, pos.row);
            game->candy[i].col = pos.col;
            game->candy[i].row = pos.row;

            break;
        }
    }
}

static void update()
{
    handle_input();

    if (game->is_paused)
    {
        draw_statusbar(game->w_height + 1, game->speed, game->points, highscore, game->is_paused);
        return;
    }

    debug("snake pos: %d, %d\n", game->snake->pos.col, game->snake->pos.row);

    clear_snake(game->snake, game->snake_length);
    clear_statusbar(game->w_height + 1);
    move_snake(game->snake, game->snake_length);
    handle_collision();
    create_candy();

    draw_candy(game->candy);
    draw_snake(game->snake);
    draw_statusbar(game->w_height + 1, game->speed, game->points, highscore, game->is_paused);
}

static void clear_and_count_down()
{
    clear_screen();
    draw_frame(game->w_width, game->w_height);
    for (int i = 3; i > 0 && game->is_running; i--)
    {
        for (int j = 0; j < 5; j++)
        {
            handle_input();
            if (!game->is_running)
                break;

            usleep(200 * MILLI_IN_MICROS);
        }
        draw_count_down(&game->snake->pos, i);
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
                               game->points, highscore, game->is_paused);
            }

            t_pos center = {.col = game->w_width / 2, .row = game->w_height / 2};
            draw_game_over(&center);
            usleep(SECOND_IN_MICROS);

            initialize();
            clear_and_count_down();
        }

        update();

        // Adjust the speed based on direction (since cells are rectangular and
        // the snake appears to move faster in vertical direction).
        float speed_factor =
            game->snake->dir == UP || game->snake->dir == DOWN ? 1.5 : 2;
        usleep(200 * (MILLI_IN_MICROS) / (game->speed * speed_factor));
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

    /*
     * In order to get only a single memory allocation, we
     * compute the amount of memory needed here, and do a
     * single call to malloc. The memory needed is the size
     * of the t_game struct, as well as memory for all the
     * snake body positions.
     * Reducing the calls to (m/c/re)alloc reduces the number
     * of syscalls we produce, the number of possible error
     * locations in our code, simplifies freeing and cleaning
     * up, as well as packing all of our data tightly in memory,
     * which is good for cache locality and the memory bus.
     */
    int max_snake_length = (rows-2) * (cols-2);
    unsigned int game_memory_size = sizeof(t_game) +
        max_snake_length * sizeof(t_snake);
    unsigned char *game_memory = (unsigned char *)malloc(game_memory_size);
    assert(game_memory);
    debug("Allocated %u kB of memory for the game.\n", game_memory_size >> 10);

    game = (t_game*)game_memory;
    game->snake = (t_snake *)(game_memory + sizeof(t_game));
    game->snake_capacity = max_snake_length;
    game->w_width = cols;
    game->w_height = rows - 1;
    game->start = &start;
    game->stop = &stop;

    return game;
}
