#ifndef INPUT_H
#define INPUT_H

typedef enum {
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7,
    INPUT_8,
    INPUT_9,
    INPUT_0,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_PAUSE,
    INPUT_QUIT,
} e_input;

e_input input_read();

#endif
