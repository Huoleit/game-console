/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId GameTaskHandle;
osThreadId UartTaskHandle;
osThreadId InputTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void gameTaskFunc(void const *argument);
void uartTaskFunc(void const *argument);
void InputTaskFunc(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of GameTask */
  osThreadDef(GameTask, gameTaskFunc, osPriorityNormal, 0, 128);
  GameTaskHandle = osThreadCreate(osThread(GameTask), NULL);

  /* definition and creation of UartTask */
  osThreadDef(UartTask, uartTaskFunc, osPriorityAboveNormal, 0, 128);
  UartTaskHandle = osThreadCreate(osThread(UartTask), NULL);

  /* definition and creation of InputTask */
  osThreadDef(InputTask, InputTaskFunc, osPriorityHigh, 0, 128);
  InputTaskHandle = osThreadCreate(osThread(InputTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument) {
  /* USER CODE BEGIN StartDefaultTask */
  for (;;) {
    if (GAME_get_id(&_game) == 1) {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    } else if (GAME_get_id(&_game) == 2) {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    }

    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_gameTaskFunc */
/**
 * @brief Function implementing the GameTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_gameTaskFunc */
void gameTaskFunc(void const *argument) {
  /* USER CODE BEGIN gameTaskFunc */
  /* Infinite loop */
  struct UART_GameStatusMsg txBuffer;
  int startHaptic = 0;
  uint32_t startHapticTick = 0;
  TickType_t lastTick = xTaskGetTickCount();
  int32_t diff;
  // int txFailCount = 0;
  // Game loop
  const float dt = (float)(GAME_UPDATE_RATE_MS) / 1000.0f; // sec
  for (;;) {
    switch (GAME_get_state(&_game)) {
    case GAME_STATE_INIT:
      DISPLAY_clear();
      GAME_set_state(&_game, GAME_STATE_CONNECTING);
      break;
    case GAME_STATE_CONNECTING:
      DISPLAY_connecting(dt);
      // id == 0 means just power up
      if (_other_isConnected) {
        if (GAME_get_id(&_game) == 0) {
          if (_other_gameStatus.id == 1) {
            GAME_set_id(&_game, 2);
          } else if (_other_gameStatus.id == 2) {
            GAME_set_id(&_game, 1);
          } else { // Both no id
            if (_other_gameStatus.tickCount < xTaskGetTickCount()) {
              GAME_set_id(&_game, 1);
            } else {
              GAME_set_id(&_game, 2);
            }
          }
        }
        if (GAME_get_prvState(&_game) == GAME_STATE_INIT) {
          if (GAME_get_id(&_game) == 1) {
            GAME_set_state(&_game, GAME_STATE_MY_TURN);
          } else if (GAME_get_id(&_game) == 2) {
            GAME_set_state(&_game, GAME_STATE_OTHERS_TURN);
          }
        } else {
          GAME_set_state(&_game, GAME_get_prvState(&_game));
        }
        DISPLAY_clear();
      }
      break;
    case GAME_STATE_MY_TURN:
      GAME_set_paddle_pos(&_game, INPUT_get_x(dt));
      if (GAME_loop(&_game, dt) && INPUT_get_device_type() == INPUT_DEVICE_SLIDER &&
          startHaptic == 0) {
        startHaptic = 1;
      }
      DISPLAY_draw_ball_from_game(&_game);
      DISPLAY_draw_paddle_from_game(&_game);
      DISPLAY_display();

#ifdef GAME_SINGLE_PLAYER
      if (_game.ball.y > _game.height) {
        DISPLAY_game_over();
        GAME_set_state(&_game, GAME_STATE_OVER);
      }
#else
      if (!_other_isConnected) {
        DISPLAY_clear();
        GAME_set_state(&_game, GAME_STATE_CONNECTING);
      } else if (_game.ball.y > _game.height) {
        DISPLAY_game_over();
        GAME_set_state(&_game, GAME_STATE_OVER);
      } else if (_game.ball.y < 0) {
        GAME_set_state(&_game, GAME_STATE_TRANSITION);
      }
#endif

      break;
    case GAME_STATE_TRANSITION:
      GAME_set_paddle_pos(&_game, INPUT_get_x(dt));
      DISPLAY_draw_paddle_from_game(&_game);
      DISPLAY_display();

      if (!_other_isConnected) {
        DISPLAY_clear();
        GAME_set_state(&_game, GAME_STATE_CONNECTING);
      } else if (_other_gameStatus.state == GAME_STATE_MY_TURN) {
        GAME_set_state(&_game, GAME_STATE_OTHERS_TURN);
      }
      break;
    case GAME_STATE_OTHERS_TURN:
      GAME_set_paddle_pos(&_game, INPUT_get_x(dt));
      DISPLAY_draw_paddle_from_game(&_game);
      DISPLAY_display();

      if (!_other_isConnected) {
        DISPLAY_clear();
        GAME_set_state(&_game, GAME_STATE_CONNECTING);
      } else if (_other_gameStatus.state == GAME_STATE_TRANSITION) {
        UART_msg_to_game(&_other_gameStatus, &_game);
        GAME_set_state(&_game, GAME_STATE_MY_TURN);
      } else if (_other_gameStatus.state == GAME_STATE_OVER) {
        DISPLAY_game_over();
        GAME_set_state(&_game, GAME_STATE_OVER);
      }

      break;
    case GAME_STATE_OVER:
      break;
    default:
      break;
    }

    UART_game_to_msg(&_game, &txBuffer);

    huart3.TxXferCount = 0;
    while (huart3.TxXferCount < sizeof(struct UART_GameStatusMsg)) {
      if ((__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) != RESET)) {
        huart3.Instance->DR = ((uint8_t *)&txBuffer)[huart3.TxXferCount] & 0xFFU;
        ++huart3.TxXferCount;
      }
    }

    if (startHaptic == 1) {
      startHaptic = 2;
      startHapticTick = xTaskGetTickCount();
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
    } else if (startHaptic == 2 && xTaskGetTickCount() - startHapticTick > 200) {
      startHaptic = 0;
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
    }

    // osDelayUntil(&lastTick, GAME_UPDATE_RATE_MS);
    lastTick += pdMS_TO_TICKS(GAME_UPDATE_RATE_MS);
    if ((diff = lastTick - xTaskGetTickCount()) > 0) {
      osDelay(diff);
    } else {
      lastTick = xTaskGetTickCount();
    }
  }
  /* USER CODE END gameTaskFunc */
}

/* USER CODE BEGIN Header_uartTaskFunc */
/**
 * @brief Function implementing the UartTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_uartTaskFunc */
void uartTaskFunc(void const *argument) {
  /* USER CODE BEGIN uartTaskFunc */
  /* Infinite loop */
  const TickType_t xDisconnectTimeout = pdMS_TO_TICKS(GAME_STATUS_TIMEOUT_MS);
  uint32_t ulNotifiedValue;
  TickType_t lastTick = xTaskGetTickCount();

  for (;;) {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)&rxBuffer,
                                 sizeof(struct UART_GameStatusMsg));

    ulNotifiedValue = ulTaskNotifyTake(pdFALSE, xDisconnectTimeout);
    if (ulNotifiedValue > 0) {
      /* Perform any processing necessitated by the interrupt. */
      if (rxBuffer.FIXED_HEADER == USART_FIXED_HEADER &&
          rxBuffer.FIXED_FOOTER == USART_FIXED_FOOTER) {
        taskENTER_CRITICAL();
        _other_isConnected = 1;
        _other_gameStatus = rxBuffer;
        taskEXIT_CRITICAL();

        lastTick = xTaskGetTickCount();
      } else {
        if (xTaskGetTickCount() - lastTick > xDisconnectTimeout) {
          taskENTER_CRITICAL();
          _other_isConnected = 0;
          taskEXIT_CRITICAL();
        }

        osDelay(GAME_UPDATE_RATE_MS / 3);
      }

    } else {
      /* Did not receive a notification within the expected time. */
      taskENTER_CRITICAL();
      _other_isConnected = 0;
      taskEXIT_CRITICAL();
    }
  }
  /* USER CODE END uartTaskFunc */
}

/* USER CODE BEGIN Header_InputTaskFunc */
/**
 * @brief Function implementing the InputTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_InputTaskFunc */
void InputTaskFunc(void const *argument) {
  /* USER CODE BEGIN InputTaskFunc */
  /* Infinite loop */
  for (;;) {
    INPUT_loop();

    osDelay(10);
  }
  /* USER CODE END InputTaskFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
