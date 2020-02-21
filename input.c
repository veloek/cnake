#include "game.h"
#include "input.h"

#include <unistd.h> // read

e_dir read_input() {
    char buf[3]; int n;
    if (!(n = read(0, buf, 3)))
        return -1;

    char input = buf[0];
    if (input == '\33') // escape character (i.e. arrow keys)
        input = buf[2]; // format: [\33, '[', 'A'|'B'|'C'|'D']

    switch (input)
    {
        case 'j':
        case 'B':
            return DOWN;
        case 'k':
        case 'A':
            return UP;
        case 'h':
        case 'D':
            return LEFT;
        case 'l':
        case 'C':
            return RIGHT;
    }
    return -1;
}
