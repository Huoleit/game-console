#include "MPU6050.h"

#include "i2c.h"
#include "lcd.h"
#include "stm32f1xx_hal.h"

#include <main.h>
#include <stdio.h>

void mpu_init() {
  HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(&hi2c2, (0x68 << 1), 1, 100);
  if (ret == HAL_OK) {
    LCD_DrawString(60, 160, "HAL_OK");
    LCD_DrawString(60, 180, "            ");
  } else {
    LCD_DrawString(60, 160, "          ");
    LCD_DrawString(60, 180, "HAL_NOT_OK");
  }
  uint8_t gyrosetting = 0b00001000; // accelerometer setting
  ret = HAL_I2C_Mem_Write(&hi2c2, (0x68 << 1), 0x1C, 1, &gyrosetting, 1, 100);
  if (ret == HAL_OK) {
    LCD_DrawString(60, 200, "WRITTEN");
  } else {
    LCD_DrawString(60, 220, "HAL_NOT_WRITTEN");
  }
  gyrosetting = 0; // exit from sleep mode
  HAL_I2C_Mem_Write(&hi2c2, (0x68 << 1), 0x6B, 1, &gyrosetting, 1, 100);
}

float mpu_readX() {
  uint8_t data[2];
  uint16_t xaxis;
  HAL_I2C_Mem_Read(&hi2c2, (0x68 << 1), 0x3D, 1, data, 2, 100);
  xaxis = ((int16_t)data[0] << 8) + data[1];
  float range = 0.0;
  if (xaxis > 18000.0 && xaxis < 58000) {
    range = 0.0;
  } else if (xaxis > 8000 && xaxis < 18000.0) {
    range = 1.0;
  } else if (xaxis >= 0 && xaxis < 8000) {
    range = 0.5 + (xaxis / 8000.0) / 2;
  } else if (xaxis > 58000 && xaxis < 65536) {
    range = (xaxis - 58000.0) / 7500.0 / 2;
  }
  return range;
}
