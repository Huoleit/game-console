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
  GAME_STATE_CONNECTING,
  GAME_STATE_PLAY,
  GAME_STATE_OVER,
  GAME_STATE_EXIT
};

struct GameBoard;

void GAME_init(struct GameBoard *board, int width, int height);
void GAME_set_paddle_speed(struct GameBoard *board, float speed);
void GAME_update(struct GameBoard *board, float curTime);
void GAME_get_ball(struct GameBoard *board, int *x, int *y, int *radius, uint32_t *color);
void GAME_get_paddle(struct GameBoard *board, int *x, int *y, int *width, int *height);
int GAME_get_state(struct GameBoard *board);

// Global variables
extern struct GameBoard _game;

#ifdef __cplusplus
}
#endif

#endif