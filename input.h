#ifndef INPUT_H
#define INPUT_H

typedef enum {
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_PAUSE,
    INPUT_QUIT,
} e_input;

e_input read_input();

#endif
