/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.h
 * @brief   This file contains all the function prototypes for
 *          the usart.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "game.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
struct __attribute__((__packed__)) UART_GameStatusMsg {
  uint8_t id;
  uint8_t state;

  struct __attribute__((__packed__)) {
    uint16_t x, y;
    float dx, dy; // Norm vector
    float radius;
    uint16_t color;
  } ball;

  uint32_t tickCount;
};

extern uint8_t _other_isConnected;
extern struct UART_GameStatusMsg _other_gameStatus;
extern uint32_t _other_lastReceiveTick;
extern uint8_t _other_gameStatus_isUpdated;
/* USER CODE END Private defines */

void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void UART_game_to_msg(struct GAME_Board *game, struct UART_GameStatusMsg *msg);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
