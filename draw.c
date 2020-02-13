#include "draw.h"
#include "game.h"

#include <stdio.h>

void clear_screen()
{
    printf("\033[2J");
}

void draw_frame(int rows, int cols)
{
    int i;

    for (i = 1; i <= cols; i++)
    {
        // Top row
        printf("\033[0;%dH-", i);
        // Bottom row
        printf("\033[%d;%dH-", rows, i);
    }

    for (i = 1; i <= rows; i++)
    {
        // Left column
        printf("\033[%d;0H|", i);
        // Right column
        printf("\033[%d;%dH|", i, cols);
    }
}

void draw_snake(t_snake *snake)
{
    if (snake->next)
    {
        printf("\033[%d;%dH+", snake->next->pos->row, snake->next->pos->col);
    }

    // Move and print head
    char head;
    switch (snake->dir)
    {
        case LEFT:
            snake->pos->col--;
            head = '<';
            break;
        case RIGHT:
            snake->pos->col++;
            head = '>';
            break;
        case UP:
            snake->pos->row--;
            head = 'A';
            break;
        case DOWN:
            snake->pos->row++;
            head = 'V';
            break;
    }
    printf("\033[%d;%dH%c", snake->pos->row, snake->pos->col, head);
}

void clear_snake(t_snake *snake)
{
    if (snake->next != NULL)
    {
        while (snake->next != NULL)
            snake = snake->next;
    }
    printf("\033[%d;%dH ", snake->pos->row, snake->pos->col);
}

void draw_candy(t_pos **candy)
{
    for (int i = 0; i < N_CANDY; i++)
    {
        if (candy[i]->row > 0)
            printf("\033[%d;%dHX", candy[i]->row, candy[i]->col);
    }
}

void clear_candy(t_pos *candy)
{
    printf("\033[%d;%dH ", candy->row, candy->col);
}

void draw_count_down(t_pos *pos, int n)
{
    printf("\033[%d;%dH%d", pos->row, pos->col, n);
}