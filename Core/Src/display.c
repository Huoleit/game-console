#include "display.h"

struct DISPLAY_Ball {
  int x, y;
  int radius;
  int color;
  int valid;
};

struct DISPLAY_Paddle {
  int x, y;
  int width, height;
  int color;
  int valid;
};

struct DISPLAY_Line {
  int x1, y1;
  int x2, y2;
  int valid;
};

struct DISPLAY_Ball currentBall, lastBall;
struct DISPLAY_Paddle currentPaddle, lastPaddle;

void DISPLAY_draw_ball(int x, int y, int radius, int color) {
  currentBall.x = x;
  currentBall.y = y;
  currentBall.radius = radius;
  currentBall.color = color;
  currentBall.valid = 1;
}

void DISPLAY_draw_ball_from_game(struct GAME_Board *board) {
  DISPLAY_draw_ball(board->ball.x, board->ball.y, board->ball.radius, board->ball.color);
}

void DISPLAY_draw_paddle(int x, int y, int width, int height, int color) {
  currentPaddle.x = x;
  currentPaddle.y = y;
  currentPaddle.width = width;
  currentPaddle.height = height;
  currentPaddle.color = color;
  currentPaddle.valid = 1;
}

void DISPLAY_draw_paddle_from_game(struct GAME_Board *board) {
  DISPLAY_draw_paddle(board->paddle.x, board->height - board->paddle.hight,
                      board->paddle.width, board->paddle.hight, WHITE);
}

void DISPLAY_game_over() {
  LCD_Clear(0, 0, 240, 320, BACKGROUND);
  LCD_DrawString(80, 160, "Game Over");
}

void DISPLAY_connecting(float dt) {
  static float t;
  static int mode;

  t += dt;
  if (t > 1) {
    t = 0;
    mode++;
    mode %= 4;
  }
  if (mode == 0) {
    LCD_DrawString(80, 160, "Connecting   ");
  } else if (mode == 1) {
    LCD_DrawString(80, 160, "Connecting.  ");
  } else if (mode == 2) {
    LCD_DrawString(80, 160, "Connecting.. ");
  } else if (mode == 3) {
    LCD_DrawString(80, 160, "Connecting...");
  }
}

void DISPLAY_clear() { LCD_Clear(0, 0, 240, 320, BACKGROUND); }

void DISPLAY_display() {
  if (lastBall.valid) {
    LCD_DrawCircle(lastBall.x, lastBall.y, lastBall.radius, BACKGROUND);
    lastBall.valid = 0;
  }
  if (currentBall.valid) {
    LCD_DrawCircle(currentBall.x, currentBall.y, currentBall.radius, currentBall.color);
    lastBall = currentBall;
    currentBall.valid = 0;
  }
  if (currentPaddle.valid) {
    if (lastPaddle.valid) {
      if (lastPaddle.x < currentPaddle.x) {
        LCD_DrawRectangle((lastPaddle.x + currentPaddle.x - currentPaddle.width) / 2,
                          lastPaddle.y, currentPaddle.x - lastPaddle.x + 1,
                          lastPaddle.height, BACKGROUND);
      } else if (lastPaddle.x > currentPaddle.x) {
        LCD_DrawRectangle((lastPaddle.x + currentPaddle.x + currentPaddle.width) / 2,
                          lastPaddle.y, lastPaddle.x - currentPaddle.x + 1,
                          lastPaddle.height, BACKGROUND);
      }
    }
    LCD_DrawRectangle(currentPaddle.x, currentPaddle.y, currentPaddle.width,
                      currentPaddle.height, currentPaddle.color);

    lastPaddle = currentPaddle;
  }
}

void DISPLAY_update(struct GAME_Board *board) {}