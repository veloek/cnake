#ifndef DRAW_H
#define DRAW_H

#include "game.h"

void clear_screen();

void draw_frame(int width, int height);

void draw_snake(t_snake *snake);

void clear_snake(t_snake *snake);

void draw_candy(t_pos **candy);

void clear_candy(t_pos *candy);

void draw_count_down(t_pos *pos, int n);

void draw_game_over(t_pos *pos);

#endif
