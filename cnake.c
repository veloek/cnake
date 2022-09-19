#include "game.h"
#include "term.h"

#include <signal.h> // signal
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // STDOUT_FILENO
#include <stdio.h> // fprintf

int main()
{
    // Get terminal window size
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
    {
        fprintf(stderr, "error: failed to get term size\n");
        return -1;
    }

    term_setup();

    t_game *game = new_game(size.ws_row, size.ws_col);

    // Catch sigterm and stop loop
    signal(SIGINT, game->stop);

    game->start();

    term_cleanup();
}
