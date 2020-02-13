#ifndef DRAWING_H
#define DRAWING_H

#include "game.h"

void clear_screen();

void draw_frame(int rows, int cols);

void draw_snake(t_snake *snake);

void clear_snake(t_snake *snake);

void draw_candy(t_pos **candy);

void clear_candy(t_pos *candy);

void draw_count_down(t_pos *pos, int n);

#endif
