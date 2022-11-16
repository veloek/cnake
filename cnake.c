#include "game.h"
#include "term.h"

#include <errno.h> // errno
#include <fcntl.h> // open, O_NOCTTY, O_NONBLOCK
#include <signal.h> // signal
#include <stdio.h> // fprintf
#include <stdlib.h> // exit
#include <string.h> // strerror
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // close

static t_game *game;

static void get_winsize(struct winsize *size);

static void on_resize();

int main()
{
    // Get terminal window size
    struct winsize size;
    get_winsize(&size);

    // Hide cursor and configure input
    term_setup();

    game = new_game(size.ws_row, size.ws_col);

    // Catch sigterm and stop loop
    signal(SIGINT, game->stop);

    signal(SIGWINCH, on_resize);

    game->start();

    free_game(game);

    term_cleanup();
}

static void get_winsize(struct winsize *size)
{
    int tty_fd = open("/dev/tty", O_NOCTTY | O_NONBLOCK);
    if (tty_fd == -1)
    {
        fprintf(stderr, "error: opening `/dev/tty` failed (%d): %s\n",
                errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (ioctl(tty_fd, TIOCGWINSZ, size) == -1)
    {
        fprintf(stderr, "error: getting term size failed (%d): %s\n",
                errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static void on_resize()
{
    struct winsize size;
    get_winsize(&size);
    game->resize(size.ws_row, size.ws_col);
}
