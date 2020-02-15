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
    int row, col;
} t_pos;

typedef struct t_snake {
    t_pos *pos;
    e_dir dir;
    struct t_snake *next;
} t_snake;

typedef struct {
    t_snake *snake;
    t_pos **candy;
    int w_width;
    int w_height;
    int is_running;
    int speed;

    void (*start)();
    void (*stop)();
} t_game;

t_game *new_game(int rows, int cols);

#endif
