#include "game.h"
#include "input.h"

#include <unistd.h> // read

#define ARROW_UP 'A'
#define ARROW_DOWN 'B'
#define ARROW_RIGHT 'C'
#define ARROW_LEFT 'D'
#define ESC 0

e_input read_input() {
    char buf[3]; int n;
    if ((n = read(0, buf, 3)) <= 0)
        return -1;

    char input = buf[0];
    if (input == '\33') // escape character (i.e. arrow keys)
        input = buf[2]; // format: [\33, '[', 'A'|'B'|'C'|'D']

    switch (input)
    {
        case ARROW_DOWN:
        case 'j':
            return INPUT_DOWN;

        case ARROW_UP:
        case 'k':
            return INPUT_UP;

        case ARROW_LEFT:
        case 'h':
            return INPUT_LEFT;

        case ARROW_RIGHT:
        case 'l':
            return INPUT_RIGHT;

        case ESC:
        case 'q':
            return INPUT_QUIT;
    }

    return -1;
}
