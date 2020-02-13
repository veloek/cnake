#include "term.h"
#include "draw.h"

#include <termios.h> // tcgetattr/tcsetattr
#include <fcntl.h> // fcntl
#include <stdio.h> // setbuf
#include <unistd.h> // STDIN_FILENO

static int flags;
static struct termios ctrl;

void term_setup()
{
    clear_screen();

    // Turn off output buffer
    setbuf(stdout, NULL);

    // Turn off input buffer and echo
    tcgetattr(STDIN_FILENO, &ctrl);
    ctrl.c_lflag &= ~ICANON;
    ctrl.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);

    // Make stdin non-blocking
    flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Hide cursor
    printf("\033[?25l");
}

void term_cleanup()
{
    // Reset cursor hidden state
    printf("\033[?25h");

    // Reset terminal controls
    printf("\033c");
    fcntl(STDIN_FILENO, F_SETFL, flags);
    ctrl.c_lflag |= ICANON;
    ctrl.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);

    clear_screen();
}
