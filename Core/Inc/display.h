#ifndef __DISPLAY_H
#define __DISPLAY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "game.h"
#include "lcd.h"
#include "stdint.h"

#define DISPLAY_WIDTH LCD_Default_Max_COLUMN
#define DISPLAY_HEIGHT LCD_Default_Max_PAGE

void DISPLAY_draw_ball(int x, int y, int radius, int color);
void DISPLAY_draw_ball_from_game(struct GAME_Board *board);

void DISPLAY_draw_paddle(int x, int y, int width, int height, int color);
void DISPLAY_draw_paddle_from_game(struct GAME_Board *board);

void DISPLAY_display();

void DISPLAY_game_over();
void DISPLAY_connecting(float dt);
void DISPLAY_clear();

void DISPLAY_update(struct GAME_Board *board);

#ifdef __cplusplus
}
#endif

#endif