#ifndef GAME_H
#define GAME_H

#define N_CANDY 5

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} e_dir;

typedef struct {
    unsigned short row, col;
} t_pos;

typedef struct t_snake {
    t_pos pos;
    e_dir dir;
} t_snake;

typedef unsigned char t_bool;

typedef struct {
    t_snake *snake;
    unsigned int snake_length;
    unsigned int snake_capacity;
    t_pos candy[N_CANDY];
    unsigned short w_width;
    unsigned short w_height;
    t_bool is_running;
    t_bool is_paused;
    unsigned char speed;
    unsigned int points;
    t_bool should_restart;

    void (*start)();
    void (*stop)();
} t_game;

t_game *new_game(unsigned short rows, unsigned short cols);

#endif
