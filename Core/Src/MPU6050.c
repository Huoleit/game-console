#include "MPU6050.h"

#include "i2c.h"
#include "lcd.h"
#include "stm32f1xx_hal.h"

#include <main.h>
#include <stdio.h>

static uint8_t IMU_Y_data[2];
uint8_t hasIMU;

void IMU_init() {
  HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(&hi2c2, (0x68 << 1), 5, 100);
  if (ret == HAL_OK) {
    hasIMU = 1;
    uint8_t gyroSetting = 0b00001000; // accelerometer setting
    ret = HAL_I2C_Mem_Write(&hi2c2, (0x68 << 1), 0x1C, 1, &gyroSetting, 1, 100);
    gyroSetting = 0; // exit from sleep mode
    HAL_I2C_Mem_Write(&hi2c2, (0x68 << 1), 0x6B, 1, &gyroSetting, 1, 100);
  } else {
    hasIMU = 0;
  }
}

void IMU_Start_reading() {
  HAL_I2C_Mem_Read_IT(&hi2c2, (0x68 << 1), 0x3D, 1, IMU_Y_data, 2);
}

float IMU_readX() {
  uint16_t y_axis = ((int16_t)IMU_Y_data[0] << 8) + IMU_Y_data[1];
  float range = 0.0;
  if (y_axis > 18000.0 && y_axis < 58000) {
    range = 0.0;
  } else if (y_axis > 8000 && y_axis < 18000.0) {
    range = 1.0;
  } else if (y_axis >= 0 && y_axis < 8000) {
    range = 0.5 + (y_axis / 8000.0) / 2;
  } else if (y_axis > 58000 && y_axis < 65536) {
    range = (y_axis - 58000.0) / 7500.0 / 2;
  }
  return range;
}

int IMU_isReady() { return hasIMU; }
