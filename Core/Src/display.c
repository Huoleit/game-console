#include "display.h"

uint16_t lastFrame[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {WHITE};
uint16_t curFrame[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {WHITE};

void DISPLAY_draw_ball(int x, int y, int radius, int color) {
  int i, j;
  for (i = x - radius; i < x + radius; i++) {
    for (j = y - radius; j < y + radius; j++) {
      if (i >= 0 && i < DISPLAY_WIDTH && j >= 0 && j < DISPLAY_HEIGHT) {
        curFrame[j][i] = color;
      }
    }
  }
}

void DISPLAY_update() {
  int i, j;
  for (i = 0; i < DISPLAY_WIDTH; i++) {
    for (j = 0; j < DISPLAY_HEIGHT; j++) {
      if (curFrame[j][i] != lastFrame[j][i]) {
        LCD_DrawDot(i, j, curFrame[j][i]);
        lastFrame[j][i] = curFrame[j][i];
      }
      curFrame[j][i] = WHITE;
    }
  }
}