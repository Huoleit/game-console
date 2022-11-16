#include "game.h"

#include "stddef.h"

struct Boundary {
  float u, v;  // Norm vector
  float x, y;
  float c;
};

struct GameBoard {
  int width, height;
  struct Boundary* boundaries;
  int boundaryCount;

  struct {
    int x, y;
    float dx, dy;  // Norm vector
    float radius;
  } ball;

  float lastTime;
} GameBoard;

struct GameBoard _game;
struct Boundary _boundaries[4];

static in_bound(float x, float y, struct Boundary* b) {
  return (b->u * x) + (b->v * y) >= b->c;
}

static normalize(float* x, float* y) {
  float sumOfSquares = *x * *x + *y * *y;
  if (sumOfSquares > 1e-6) {
    *x = *x / sumOfSquares;
    *y = *y / sumOfSquares;
  } else {
    *x = 0;
    *y = 0;
  }
}

static struct Boundary init_boundary(float u, float v, float x, float y) {
  struct Boundary b;

  b.u = u;
  b.v = v;
  normalize(&b.u, &b.v);

  b.x = x;
  b.y = y;

  b.c = (u * x) + (v * y);

  return b;
}

void GAME_init(struct GameBoard* board, int width, int height) {
  board->width = width;
  board->height = height;

  board->ball.x = width / 2;
  board->ball.y = height / 2;
  board->ball.radius = 5;

  board->ball.dx = 200;
  board->ball.dy = 150;

  board->lastTime = 0;

  board->boundaries = _boundaries;
  board->boundaryCount = 4;
  board->boundaries[0] = init_boundary(1, 0, 0, 0);
  board->boundaries[1] = init_boundary(0, 1, 0, 0);
  board->boundaries[2] = init_boundary(-1, 0, width, 0);
  board->boundaries[3] = init_boundary(0, -1, 0, height);
}

void GAME_update(struct GameBoard* board, float curTime) {
  if (board->lastTime == 0) {
    board->lastTime = curTime;
    return;
  }

  float dt = curTime - board->lastTime;
  board->lastTime = curTime;

  float x = board->ball.x + board->ball.dx * dt;  // intermediate pos
  float y = board->ball.y + board->ball.dy * dt;
  float dx = board->ball.dx;
  float dy = board->ball.dy;

  int allPass;
  while (1) {
    allPass = 1;
    for (int i = 0; i < board->boundaryCount; i++) {
      struct Boundary* b = &board->boundaries[i];
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

        x = board->ball.x + board->ball.dx * t;  // Collision point
        y = board->ball.y + board->ball.dy * t;

        dx = board->ball.dx - 2 * (u * d);  // Reflect
        dy = board->ball.dy - 2 * (v * d);

        x += dx * (dt - t);  // Move after collision
        y += dy * (dt - t);

        allPass = 0;
        break;
      }
    }
    if (allPass) {
      board->ball.x = x;
      board->ball.y = y;
      board->ball.dx = dx;
      board->ball.dy = dy;

      break;
    }
  }

  if (board->ball.x < 0) {
    board->ball.x = 0;
  }
  if (board->ball.x > board->width) {
    board->ball.x = board->width;
  }
  if (board->ball.y < 0) {
    board->ball.y = 0;
  }
  if (board->ball.y > board->height) {
    board->ball.y = board->height;
  }
}

// get the position of ball
void GAME_get_ball(struct GameBoard* board, int* x, int* y, int* radius) {
  *x = board->ball.x;
  *y = board->ball.y;
  *radius = board->ball.radius;
}