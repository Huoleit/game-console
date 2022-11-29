#include "LED.h"

#include "stm32f1xx_hal.h"
#include "tim.h"

#define PI 3.14159265

#define MAX_LED 256
#define USE_BRIGHTNESS 0

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4]; // for brightness

uint8_t chararray[17]
                 [40] = //[no of char][bits for the led]                   note: figure
                        // out the odd row inverted issue
    {
        {0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1,
         1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0}, // 0
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
        {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1,
         0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0}, // 2
        {0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0,
         1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0}, // 3
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0,
         0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0}, // 4
        {0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0,
         1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1}, // 5
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0,
         1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0}, // 6
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
         0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}, // 7
        {0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1,
         0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0}, // 8
        {0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1,
         0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0}, // 9
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
         0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //:
        {0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
         0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // P (player)
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
         1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}, // W (WIN)
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // I
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
         1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // N
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, // C (as oC)
        {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
         1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0}, //%
};

// BEGINNING OF WS2812 CODE
int datasentflag = 0;

void Set_LED(int LEDnum, int Red, int Green, int Blue) {
  LED_Data[LEDnum][0] = LEDnum;
  LED_Data[LEDnum][1] = Green;
  LED_Data[LEDnum][2] = Red;
  LED_Data[LEDnum][3] = Blue;
}

void Set_Brightness(int brightness) // 0-45
{
#if USE_BRIGHTNESS

  if (brightness > 45)
    brightness = 45;
  for (int i = 0; i < MAX_LED; i++) {
    LED_Mod[i][0] = LED_Data[i][0];
    for (int j = 1; j < 4; j++) {
      float angle = 90 - brightness; // in degrees
      angle = angle * PI / 180;      // in rad
      LED_Mod[i][j] = (LED_Data[i][j]) / (tan(angle));
    }
  }

#endif
}

uint32_t pwmData[(24 * MAX_LED) + 50];

void WS2812_Send(void) {
  uint32_t indx;
  uint32_t color;

  indx = 0;

  for (int i = 0; i < MAX_LED; i++) {
#if USE_BRIGHTNESS
    color = ((LED_Mod[i][1] << 16) | (LED_Mod[i][2] << 8) | (LED_Mod[i][3]));
#else
    color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | (LED_Data[i][3]));
#endif

    for (int i = 23; i >= 0; i--) {
      if (color & (1 << i)) {
        pwmData[indx] = 60; // 2/3 of 90
      } else {
        pwmData[indx] = 30; // 1/3 of 90
      }

      indx++;
    }
  }

  for (int i = 0; i < 50; i++) {
    pwmData[indx] = 0;
    indx++;
  }

  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t *)pwmData, indx);
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_4);
}

void LED_DrawChar(uint16_t usP, const char charIndex, int r, int g, int b) {
  int pos = usP * 8;
  //	int r = 10, g = 10, b = 10;
  int tmp = 0;
  switch (charIndex) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    tmp = (int)charIndex - 48;
    break;
  case ':':
    tmp = 10;
    break;
  case 'P':
    tmp = 11;
    break;
  case 'W':
    tmp = 12;
    break;
  case 'I':
    tmp = 13;
    break;
  case 'N':
    tmp = 14;
    break;
  case 'C':
    tmp = 14;
    break;
  case '%':
    tmp = 14;
    break;
  }
  if (usP % 2 == 0) {
    for (int i = 0; i < 40; i++) {
      if (chararray[tmp][i] == 1)
        Set_LED(pos++, r, g, b);
      else {
        pos++;
        //				Set_LED(pos++, r, g, b);
      }
    }
    WS2812_Send();
  } else {
    for (int i = 0; i < 5; i++) {
      for (int j = 7; j > -1; j--) {
        if (chararray[tmp][i * 8 + j] == 1)
          Set_LED(pos++, r, g, b);
        else {
          pos++;
          //					Set_LED(pos++, r, g, b);
        }
      } // end of j for loop
    }   // end of i for loop
    //		WS2812_Send();
  } // end of else
}

void WinShow(char Winner) {
  int r = 10, g = 10, b = 10; // color
  LED_DrawChar(0, 'N', r, g, b);
  LED_DrawChar(5, 'I', r, g, b);
  LED_DrawChar(10, 'W', r, g, b);
  LED_DrawChar(18, Winner, r, g, b);
  LED_DrawChar(23, 'P', r, g, b);
  WS2812_Send();
}
// END OF WS2812 CODE