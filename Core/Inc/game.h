#ifndef __GAME_H
#define __GAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

struct GameBoard;

void GAME_init(struct GameBoard *board, int width, int height);
void GAME_update(struct GameBoard *board, float curTime);
void GAME_get_ball(struct GameBoard *board, int *x, int *y, int *radius);

extern struct GameBoard _game;

#ifdef __cplusplus
}
#endif

#endif