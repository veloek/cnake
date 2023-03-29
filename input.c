#include "game.h"
#include "input.h"

#include <unistd.h> // read

e_input read_input()
{
    char buf[3]; int n;
    if ((n = read(0, buf, 3)) <= 0)
        return -1;

    char input = buf[0];
    if (input == '\33') // escape character (i.e. arrow keys)
        input = buf[2]; // format: [\33, '[', 'A'|'B'|'C'|'D']

    switch (input)
    {
        case NUM_1: return INPUT_1;
        case NUM_2: return INPUT_2;
        case NUM_3: return INPUT_3;
        case NUM_4: return INPUT_4;
        case NUM_5: return INPUT_5;
        case NUM_6: return INPUT_6;
        case NUM_7: return INPUT_7;
        case NUM_8: return INPUT_8;
        case NUM_9: return INPUT_9;
        case NUM_0: return INPUT_0;

        case ARROW_DOWN: case 'j': return INPUT_DOWN;
        case ARROW_UP: case 'k': return INPUT_UP;
        case ARROW_LEFT: case 'h': return INPUT_LEFT;
        case ARROW_RIGHT: case 'l': return INPUT_RIGHT;

        case SPACE: return INPUT_PAUSE;

        case ESC: case 'q': return INPUT_QUIT;
    }

    return -1;
}
