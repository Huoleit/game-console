#ifndef __DISPLAY_H
#define __DISPLAY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lcd.h"
#include "stdint.h"

#define DISPLAY_WIDTH LCD_Default_Max_COLUMN
#define DISPLAY_HEIGHT LCD_Default_Max_PAGE

void DISPLAY_draw_ball(int x, int y, int radius, int color);
void DISPLAY_draw_paddle(int x, int y, int width, int height, int color);
void DISPLAY_update();

void DISPLAY_game_over();

#ifdef __cplusplus
}
#endif

#endif