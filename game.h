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
    t_pos *pos;
    e_dir dir;
    struct t_snake *next;
} t_snake;

typedef struct {
    t_snake *snake;
    t_pos **candy;
    unsigned short w_width;
    unsigned short w_height;
    unsigned char is_running;
    unsigned char speed;
    unsigned int points;
    unsigned char should_restart;

    void (*start)();
    void (*stop)();
} t_game;

t_game *new_game(unsigned short rows, unsigned short cols);

#endif
