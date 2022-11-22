#ifndef __GAME_H
#define __GAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define GAME_UPDATE_RATE_MS 34
#define GAME_STATUS_RECEIVE_TIMEOUT_MS 25
#define GAME_STATUS_WAIT_MS (GAME_UPDATE_RATE_MS - GAME_STATUS_RECEIVE_TIMEOUT_MS)
#define GAME_STATUS_TIMEOUT_MS 1500
#define GAME_STATUS_TIMEOUT_FAIL_COUNT (GAME_STATUS_TIMEOUT_MS / GAME_UPDATE_RATE_MS)

enum GAME_State {
  GAME_STATE_INIT,
  GAME_STATE_CONNECTING,
  GAME_STATE_PLAY,
  GAME_STATE_OVER,
  GAME_STATE_EXIT
};

struct GAME_Boundary {
  float u, v; // Norm vector
  float x, y;
  float c;
};

struct GAME_Board {
  int width, height;
  struct GAME_Boundary *boundaries;
  int boundaryCount;

  struct {
    uint16_t x, y;
    float dx, dy; // Norm vector
    float radius;
    uint16_t color;
  } ball;

  struct {
    float x;
    int hight, width;
    float speed;
  } paddle;

  enum GAME_State state;
  uint8_t id;
};

void GAME_init(struct GAME_Board *board, int width, int height);
void GAME_update(struct GAME_Board *board, float curTime);

void GAME_get_ball(struct GAME_Board *board, int *x, int *y, int *radius,
                   uint32_t *color);
void GAME_get_paddle(struct GAME_Board *board, int *x, int *y, int *width, int *height);
int GAME_get_state(struct GAME_Board *board);

void GAME_set_id(struct GAME_Board *board, int id);
void GAME_set_state(struct GAME_Board *board, int state);
void GAME_set_paddle_speed(struct GAME_Board *board, float speed);
void GAME_set_paddle_pos(struct GAME_Board *board, float x_rel);

// Global variables
extern struct GAME_Board _game;

#ifdef __cplusplus
}
#endif

#endif