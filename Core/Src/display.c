#include "display.h"

struct Ball {
  int x, y;
  int radius;
  int color;
  int valid;
};

struct Paddle {
  int x, y;
  int width, height;
  int color;
  int valid;
};

struct Line {
  int x1, y1;
  int x2, y2;
  int valid;
};

struct Ball currentBall, lastBall;
struct Paddle currentPaddle, lastPaddle;

void DISPLAY_draw_ball(int x, int y, int radius, int color) {
  currentBall.x = x;
  currentBall.y = y;
  currentBall.radius = radius;
  currentBall.color = color;
  currentBall.valid = 1;
}

void DISPLAY_draw_paddle(int x, int y, int width, int height, int color) {
  currentPaddle.x = x;
  currentPaddle.y = y;
  currentPaddle.width = width;
  currentPaddle.height = height;
  currentPaddle.color = color;
  currentPaddle.valid = 1;
}

void DISPLAY_update() {
  if (currentBall.valid) {
    if (lastBall.valid) {
      LCD_DrawCircle(lastBall.x, lastBall.y, lastBall.radius, BACKGROUND);
    }
    LCD_DrawCircle(currentBall.x, currentBall.y, currentBall.radius,
                   currentBall.color);
    lastBall = currentBall;
  }
  if (currentPaddle.valid) {
    if (lastPaddle.valid) {
      if (lastPaddle.x < currentPaddle.x) {
        LCD_DrawRectangle(
            (lastPaddle.x + currentPaddle.x - currentPaddle.width) / 2,
            lastPaddle.y, currentPaddle.x - lastPaddle.x + 1, lastPaddle.height,
            BACKGROUND);
      } else if (lastPaddle.x > currentPaddle.x) {
        LCD_DrawRectangle(
            (lastPaddle.x + currentPaddle.x + currentPaddle.width) / 2,
            lastPaddle.y, lastPaddle.x - currentPaddle.x + 1, lastPaddle.height,
            BACKGROUND);
      }
    }
    LCD_DrawRectangle(currentPaddle.x, currentPaddle.y, currentPaddle.width,
                      currentPaddle.height, currentPaddle.color);

    lastPaddle = currentPaddle;
  }
}