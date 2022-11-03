#include "game.h"
#include "term.h"

#include <signal.h> // signal
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // STDOUT_FILENO
#include <stdio.h> // fprintf
#include <stdlib.h> // exit

void get_winsize(struct winsize *size)
{
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, size) == -1)
    {
        fprintf(stderr, "error: failed to get term size\n");
        exit(-1);
    }
}

void on_resize()
{
    struct winsize size;
    get_winsize(&size);
    fprintf(stderr, "RESIZE: %d x %d\n", size.ws_col, size.ws_row);
}

int main()
{
    // Get terminal window size
    struct winsize size;
    get_winsize(&size);

    term_setup();

    t_game *game = new_game(size.ws_row, size.ws_col);

    // Catch sigterm and stop loop
    signal(SIGINT, game->stop);

    signal(SIGWINCH, on_resize);

    game->start();

    term_cleanup();
}
