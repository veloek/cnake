#ifndef INPUT_H
#define INPUT_H

#define NUM_1 '1'
#define NUM_2 '2'
#define NUM_3 '3'
#define NUM_4 '4'
#define NUM_5 '5'
#define NUM_6 '6'
#define NUM_7 '7'
#define NUM_8 '8'
#define NUM_9 '9'
#define NUM_0 '0'
#define ARROW_UP 'A'
#define ARROW_DOWN 'B'
#define ARROW_RIGHT 'C'
#define ARROW_LEFT 'D'
#define SPACE ' '
#define ESC 0

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

e_input read_input();

#endif
