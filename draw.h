#ifndef DRAW_H
#define DRAW_H

#include "game.h"

void clear_screen();

void draw_frame(int width, int height);

void draw_snake(const t_snake *snake);

void clear_snake(const t_snake *snake);

void draw_candy(t_pos **candy);

void clear_candy(const t_pos *candy);

void draw_count_down(const t_pos *pos, int n);

void draw_game_over(const t_pos *pos);

void draw_statusbar(int row, char speed, int points, int highscore);

void clear_statusbar(int row);

#endif
