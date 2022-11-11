#include "game.h"
#include "term.h"

#include <signal.h> // signal
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // STDOUT_FILENO
#include <stdio.h> // fprintf
#include <stdlib.h> // exit

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
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, size) == -1)
    {
        fprintf(stderr, "error: failed to get term size\n");
        exit(EXIT_FAILURE);
    }
}

static void on_resize()
{
    struct winsize size;
    get_winsize(&size);
    game->resize(size.ws_row, size.ws_col);
}
