#include "input_device.h"

#include "stm32f1xx_hal.h"

float _last_x = 0.5;

static float button(float dt) {
  float speed = 1.0f;
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
    _last_x -= speed * dt;
  }
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
    _last_x += speed * dt;
  }

  if (_last_x < 0.0f) {
    _last_x = 0.0f;
  } else if (_last_x > 1.0f) {
    _last_x = 1.0f;
  }

  return _last_x;
}

float INPUT_get_x(int deviceID, float dt) {
  switch (deviceID) {
  case INPUT_DEVICE_BUTTON:
    return button(dt);
  case INPUT_DEVICE_JOYSTICK:
    break;
  case INPUT_DEVICE_SLIDER:
    break;
  case INPUT_DEVICE_IMU:
    break;
  default:
    break;
  }

  return 0.5f;
}