#include "game.h"
#include "term.h"

#include <signal.h> // signal
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // STDOUT_FILENO

int main()
{
    term_setup();

    // Get terminal window size
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    t_game *game = new_game(size.ws_row, size.ws_col);

    // Catch sigterm and stop loop
    signal(SIGINT, game->stop);

    game->start();

    term_cleanup();
}
