#include "draw.h"
#include "game.h"

#include <stdio.h> // printf
#include <string.h> // strlen

#ifndef VERSION
#define VERSION "N/A"
#endif

void clear_screen()
{
    printf("\033[2J");
}

void draw_frame(int width, int height)
{
    printf("\033[100m");

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
    printf("\033[90;40m\033[0;%dH %s%s ", header_pos, header, VERSION);

    printf("\033[0m");
}

void draw_snake(const t_snake *snake)
{
    printf("\033[%d;%dH\033[42m \033[0m", snake[1].pos.row, snake[1].pos.col);
    printf("\033[%d;%dH\033[102;30m \033[0m", snake[0].pos.row, snake[0].pos.col);
}

void clear_snake(const t_snake *snake, int snake_length)
{
    // Only necessary to clear tail
    const t_snake *s = &snake[snake_length-1];
    printf("\033[%d;%dH ", s->pos.row, s->pos.col);
}

void draw_candy(t_pos *candy)
{
    for (int i = 0; i < N_CANDY; i++)
    {
        if (candy[i].row > 0)
        {
            printf("\033[%d;%dH\033[41m \033[0m", candy[i].row, candy[i].col);
        }
    }
}

void clear_candy(const t_pos *candy)
{
    printf("\033[%d;%dH ", candy->row, candy->col);
}

void draw_count_down(const t_pos *pos, int n)
{
    int color = n == 3 ? 91 : n == 2 ? 93 : 92;
    printf("\033[%d;%dH\033[%d;1m%d\033[0m", pos->row, pos->col, color, n);
}

void draw_game_over(const t_pos *pos)
{
    printf("\033[%d;%dH\033[31;1m%s\033[0m", pos->row, pos->col - 4, "GAME OVER");
}

void draw_statusbar(int row, char speed, int points, int highscore, t_bool is_paused)
{
    printf("\033[%d;1H", row);
    printf(" \033[4mSPEED\033[0m: %d", speed);
    printf(" \033[4mPOINTS\033[0m: %d", points);
    printf(" \033[4mHIGHSCORE\033[0m: %d", highscore);

    if (is_paused)
        printf(" \033[101m*** PAUSED ***\033[0m");
}

void clear_statusbar(int row)
{
    printf("\033[%d;0H", row);
    printf("\033[2K");
}
