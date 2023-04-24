#include "draw.h"
#include "game.h"

#include <stdbool.h> // bool
#include <stdio.h> // printf
#include <string.h> // strlen

#ifndef VERSION
#define VERSION "N/A"
#endif

static int colors[][2] =
{
    [BLACK]   = { 30, 40 },
    [RED]     = { 31, 41 },
    [GREEN]   = { 32, 42 },
    [YELLOW]  = { 33, 43 },
    [BLUE]    = { 34, 44 },
    [MAGENTA] = { 35, 45 },
    [CYAN]    = { 36, 46 },
    [WHITE]   = { 37, 47 },
    [DEFAULT] = { 39, 49 },

    [BRIGHT_BLACK]   = { 90, 100 },
    [BRIGHT_RED]     = { 91, 101 },
    [BRIGHT_GREEN]   = { 92, 102 },
    [BRIGHT_YELLOW]  = { 93, 103 },
    [BRIGHT_BLUE]    = { 94, 104 },
    [BRIGHT_MAGENTA] = { 95, 105 },
    [BRIGHT_CYAN]    = { 96, 106 },
    [BRIGHT_WHITE]   = { 97, 107 },
};

static void set_color(enum color fg, enum color bg)
{
    printf("\033[%d;%dm", colors[fg][0], colors[bg][1]);
}

static void set_fg_color(enum color c)
{
    printf("\033[%dm", colors[c][0]);
}

static void set_bg_color(enum color c)
{
    printf("\033[%dm", colors[c][1]);
}

static void set_bold()
{
    printf("\033[1m");
}

static void reset_color()
{
    printf("\033[0m");
}

void clear_screen()
{
    printf("\033[2J");
}

void draw_frame(int width, int height)
{
    set_bg_color(BRIGHT_BLACK);

    int i;

    for (i = 1; i <= width; i++)
    {
        // Top row
        printf("\033[0;%dH ", i);
        // Bottom row
        printf("\033[%d;%dH ", height, i);
    }

    for (i = 1; i <= height; i++)
    {
        // Left column
        printf("\033[%d;0H ", i);
        // Right column
        printf("\033[%d;%dH ", i, width);
    }

    // Print version in header
    char header[] = "CNAKE VERSION ";
    int header_pos = width / 2 - (strlen(header) + strlen(VERSION) + 2) / 2;
    set_color(BLACK, BRIGHT_BLACK);
    printf("\033[0;%dH %s%s ", header_pos, header, VERSION);

    reset_color();
}

void draw_snake(const t_snake *snake)
{
    set_bg_color(GREEN);
    printf("\033[%d;%dH ", snake[1].pos.row, snake[1].pos.col);

    set_bg_color(BRIGHT_GREEN);
    printf("\033[%d;%dH ", snake[0].pos.row, snake[0].pos.col);

    reset_color();
}

void clear_snake(const t_snake *snake, int snake_length)
{
    // Only necessary to clear tail
    const t_snake *s = &snake[snake_length-1];
    printf("\033[%d;%dH ", s->pos.row, s->pos.col);
}

void draw_candy(t_pos *candy)
{
    set_bg_color(RED);
    for (int i = 0; i < N_CANDY; i++)
    {
        if (candy[i].row > 0)
        {
            printf("\033[%d;%dH ", candy[i].row, candy[i].col);
        }
    }
    reset_color();
}

void clear_candy(const t_pos *candy)
{
    printf("\033[%d;%dH ", candy->row, candy->col);
}

void draw_count_down(const t_pos *pos, int n)
{
    int color = n == 3 ? BRIGHT_RED : n == 2 ? BRIGHT_YELLOW : BRIGHT_GREEN;
    set_fg_color(color);
    set_bold();
    printf("\033[%d;%dH%d", pos->row, pos->col, n);
    reset_color();
}

void draw_game_over(const t_pos *pos)
{
    set_fg_color(RED);
    set_bold();
    printf("\033[%d;%dH%s", pos->row, pos->col - 4, "GAME OVER");
    reset_color();
}

void draw_statusbar(int row, char speed, int points, int highscore, bool is_paused)
{
    printf("\033[%d;1H", row);
    printf(" \033[4mSPEED\033[0m: %d", speed);
    printf(" \033[4mPOINTS\033[0m: %d", points);
    printf(" \033[4mHIGHSCORE\033[0m: %d", highscore);

    if (is_paused)
    {
        printf(" ");
        set_bg_color(BRIGHT_RED);
        printf("*** PAUSED ***");
        reset_color();
    }
}

void clear_statusbar(int row)
{
    printf("\033[%d;0H", row);
    printf("\033[2K");
}
