#include "game.h"

#include "lcd.h"
#include "stddef.h"
#include "stdlib.h"

struct GAME_Board _game;
struct GAME_Boundary _boundaries[4];

static int in_bound(float x, float y, struct GAME_Boundary *b) {
  return (b->u * x) + (b->v * y) >= b->c;
}

static void normalize(float *x, float *y) {
  float sumOfSquares = *x * *x + *y * *y;
  if (sumOfSquares > 1e-6) {
    *x = *x / sumOfSquares;
    *y = *y / sumOfSquares;
  } else {
    *x = 0;
    *y = 0;
  }
}

static struct GAME_Boundary init_boundary(float u, float v, float x, float y) {
  struct GAME_Boundary b;

  b.u = u;
  b.v = v;
  normalize(&b.u, &b.v);

  b.x = x;
  b.y = y;

  b.c = (u * x) + (v * y);

  return b;
}

static void game_loop(struct GAME_Board *board, float dt) {
  // Ball
  float x = board->ball.x + board->ball.dx * dt; // intermediate pos
  float y = board->ball.y + board->ball.dy * dt;
  float dx = board->ball.dx;
  float dy = board->ball.dy;

  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < board->boundaryCount; i++) {
      struct GAME_Boundary *b = &board->boundaries[i];
      if (!in_bound(x, y, b)) {
        float u = b->u;
        float v = b->v;
        float c = b->c;

        float d = (u * board->ball.dx) + (v * board->ball.dy);
        float e = (u * board->ball.x) + (v * board->ball.y) - c;

        float t = -e / d;

        if (t > dt) {
          continue;
        }

        x = board->ball.x + board->ball.dx * t; // Collision point
        y = board->ball.y + board->ball.dy * t;

        dx = board->ball.dx - 2 * (u * d); // Reflect
        dy = board->ball.dy - 2 * (v * d);

        x += dx * (dt - t); // Move after collision
        y += dy * (dt - t);

        break;
      }
    }
  }
  board->ball.x = x;
  board->ball.y = y;
  board->ball.dx = dx;
  board->ball.dy = dy;

  if (board->ball.x < 0) {
    board->ball.x = 0;
  }
  if (board->ball.x > board->width) {
    board->ball.x = board->width;
  }
  if (board->ball.y < 0) {
    board->ball.y = 0;
  }

  // Paddle
  // board->paddle.x += board->paddle.speed * dt;
  // if (board->paddle.x < 0) {
  //   board->paddle.x = 0;
  // }
  // if (board->paddle.x > board->width) {
  //   board->paddle.x = board->width;
  // }

  // Ball hit paddle
  if (board->ball.y + board->ball.radius >= board->height - board->paddle.hight) {
    if (board->ball.x >= board->paddle.x - board->paddle.width / 2 &&
        board->ball.x <= board->paddle.x + board->paddle.width / 2) {
      board->ball.y = board->height - board->paddle.hight - board->ball.radius - 2;
      board->ball.dy = -board->ball.dy;
      board->ball.color = rand() % 0xFFFF;
    }
  }
}

void GAME_init(struct GAME_Board *board, int width, int height) {
  board->width = width;
  board->height = height;

  board->ball.x = width / 2;
  board->ball.y = height / 2;
  board->ball.radius = 5;
  board->ball.color = WHITE;

  board->ball.dx = 200;
  board->ball.dy = 150;

  board->paddle.x = width / 2;
  board->paddle.hight = 10;
  board->paddle.width = 50;

  board->boundaries = _boundaries;
  board->boundaryCount = 3;
  board->boundaries[0] = init_boundary(1, 0, 0, 0);      // Left
  board->boundaries[1] = init_boundary(0, 1, 0, 0);      // Top
  board->boundaries[2] = init_boundary(-1, 0, width, 0); // Right
  // board->boundaries[3] = init_boundary(0, -1, 0, height);  // Bottom

  board->state = GAME_STATE_PLAY;
}

void GAME_update(struct GAME_Board *board, float dt) {

  switch (board->state) {
  case GAME_STATE_INIT:
    break;
  case GAME_STATE_CONNECTING:
    break;
  case GAME_STATE_PLAY:
    game_loop(board, dt);
    break;
  case GAME_STATE_OVER:
    break;
  case GAME_STATE_EXIT:
    break;
  default:
    break;
  }
}

void GAME_set_id(struct GAME_Board *board, int id) { board->id = id; }
void GAME_set_state(struct GAME_Board *board, int state) { board->state = state; }
void GAME_set_paddle_speed(struct GAME_Board *board, float speed) {
  board->paddle.speed = speed;
}
void GAME_set_paddle_pos(struct GAME_Board *board, float x_rel) {
  if (x_rel < 0) {
    x_rel = 0;
  } else if (x_rel > 1) {
    x_rel = 1;
  }
  board->paddle.x = board->width * x_rel;
}

// get the position of ball
void GAME_get_ball(struct GAME_Board *board, int *x, int *y, int *radius,
                   uint32_t *color) {
  *x = board->ball.x;
  *y = board->ball.y;
  *radius = board->ball.radius;
  *color = board->ball.color;
}

// get the position of paddle
void GAME_get_paddle(struct GAME_Board *board, int *x, int *y, int *width, int *height) {
  *x = board->paddle.x;
  *y = board->height - board->paddle.hight / 2;
  *width = board->paddle.width;
  *height = board->paddle.hight;
}

// get the state of game
int GAME_get_state(struct GAME_Board *board) { return board->state; }