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
osThreadId HeartbeatHandle;
osThreadId displayHandle;
osThreadId ReaderHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void heartBeat(void const *argument);
void displayFunc(void const *argument);
void uartRead(void const *argument);

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

  /* definition and creation of Heartbeat */
  osThreadDef(Heartbeat, heartBeat, osPriorityLow, 0, 128);
  HeartbeatHandle = osThreadCreate(osThread(Heartbeat), NULL);

  /* definition and creation of display */
  osThreadDef(display, displayFunc, osPriorityNormal, 0, 128);
  displayHandle = osThreadCreate(osThread(display), NULL);

  /* definition and creation of Reader */
  osThreadDef(Reader, uartRead, osPriorityAboveNormal, 0, 128);
  ReaderHandle = osThreadCreate(osThread(Reader), NULL);

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
  /* Infinite loop */
  // char str[20];
  // TickType_t lastTick = 0;
  for (;;) {
    // TickType_t curTick = xTaskGetTickCount();

    // sprintf(str, "%10lu", curTick);
    // LCD_DrawString(20, 20, str);

    // sprintf(str, "%10lu", curTick - lastTick);
    // LCD_DrawString(20, 50, str);
    // lastTick = xTaskGetTickCount();

    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    osDelay(200);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_heartBeat */
/**
 * @brief Function implementing the Heartbeat thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_heartBeat */
void heartBeat(void const *argument) {
  /* USER CODE BEGIN heartBeat */
  /* Infinite loop */
  for (;;) {
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);

    osDelay(500);
  }
  /* USER CODE END heartBeat */
}

/* USER CODE BEGIN Header_displayFunc */
/**
 * @brief Function implementing the display thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_displayFunc */
void displayFunc(void const *argument) {
  /* USER CODE BEGIN displayFunc */
  /* Infinite loop */
  int ball_x, ball_y, ball_radius;
  uint32_t ball_color;
  struct UART_GameStatusMsg txBuffer;

  int paddle_x, paddle_y, paddle_width, paddle_height;
  TickType_t lastTick = xTaskGetTickCount();
  while (1) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
      GAME_set_paddle_speed(&_game, -80);
    } else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
      GAME_set_paddle_speed(&_game, 80);
    } else {
      GAME_set_paddle_speed(&_game, 0);
    }

    GAME_update(&_game, (float)xTaskGetTickCount() / 1000.0f); // s

    switch (GAME_get_state(&_game)) {
    case GAME_STATE_CONNECTING:
      break;
    case GAME_STATE_PLAY:
      GAME_get_ball(&_game, &ball_x, &ball_y, &ball_radius, &ball_color);
      GAME_get_paddle(&_game, &paddle_x, &paddle_y, &paddle_width, &paddle_height);
      DISPLAY_draw_ball(ball_x, ball_y, ball_radius, ball_color);
      DISPLAY_draw_paddle(paddle_x, paddle_y, paddle_width, paddle_height, WHITE);

      DISPLAY_update();
      break;
    case GAME_STATE_OVER:
      DISPLAY_game_over();
      break;
    default:
      break;
    }

    txBuffer.id = 1;
    txBuffer.state = GAME_get_state(&_game);
    txBuffer.ball.x = ball_x;
    txBuffer.ball.y = ball_y;
    txBuffer.ball.radius = ball_radius;
    txBuffer.ball.color = ball_color;
    txBuffer.tickCount = xTaskGetTickCount();
    (void)txBuffer;
    // HAL_UART_Transmit(&huart2, (uint8_t *)&txBuffer, sizeof(struct
    // UART_GameStatusMsg),10);

    osDelayUntil(&lastTick, GAME_UPDATE_RATE_MS);
  }
  /* USER CODE END displayFunc */
}

/* USER CODE BEGIN Header_uartRead */
/**
 * @brief Function implementing the Reader thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_uartRead */
void uartRead(void const *argument) {
  /* USER CODE BEGIN uartRead */
  /* Infinite loop */
  // struct UART_GameStatusMsg rxBuffer;
  // int failedCount = 0;

  while (1) {

    if (_other_isConnected == 1 &&
        HAL_GetTick() - _other_lastReceiveTick > GAME_STATUS_TIMEOUT_MS) {
      taskENTER_CRITICAL();
      _other_isConnected = 0;
      taskEXIT_CRITICAL();
    }

    osDelay(GAME_STATUS_WAIT_MS);
  }
  /* USER CODE END uartRead */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
