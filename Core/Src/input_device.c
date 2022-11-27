#include "input_device.h"

#include "stm32f1xx_hal.h"

#include <stdlib.h>

#define ADC_SLIDER_INDEX 0
#define ADC_KNOB_INDEX 1
#define ADC_JOYSTICK_INDEX 2

#define AVERAGE_COUNT 6

uint32_t INPUT_adc_read[3];

// Global Input Device State
static float _last_x = 0.5;
enum INPUT_DeviceType INPUT_device_type = INPUT_DEVICE_NONE;

struct AverageFilter {
  uint32_t samples[AVERAGE_COUNT];
  uint32_t sum;
  uint8_t index;
};

struct AverageFilter sliderFilter;
struct AverageFilter knobFilter;
struct AverageFilter joystickFilter;

void AverageFilter_init(struct AverageFilter *filter) {
  filter->sum = 0;
  filter->index = 0;
  for (int i = 0; i < AVERAGE_COUNT; i++) {
    filter->samples[i] = 0;
  }
}

void AverageFilter_add(struct AverageFilter *filter, uint32_t value) {
  filter->sum -= filter->samples[filter->index];
  filter->samples[filter->index] = value;
  filter->sum += value;
  filter->index = (filter->index + 1) % AVERAGE_COUNT;
}

uint32_t AverageFilter_sumOfDifference(struct AverageFilter *filter) {
  uint32_t difference = 0;
  for (int i = 0; i < AVERAGE_COUNT - 1; i++) {
    difference += labs((int32_t)filter->samples[i + 1] - (int32_t)filter->samples[i]);
  }
  return difference;
}

uint32_t AverageFilter_get(struct AverageFilter *filter) {
  return filter->sum / AVERAGE_COUNT;
}

static float slider(float dt) {
  float x = (float)AverageFilter_get(&sliderFilter) / 4095.0;

  if (x < 0) {
    x = 0.0;
  } else if (x > 1) {
    x = 1.0;
  }
  return 1.0f - x;
}

static float joystick(float dt) {
  float x = (float)AverageFilter_get(&joystickFilter) / 4095.0;

  if (x < 0) {
    x = 0.0;
  } else if (x > 1) {
    x = 1.0;
  }
  return 1.0f - x;
}

static float knob(float dt) {
  float x = (float)AverageFilter_get(&knobFilter) / 4095.0;

  if (x < 0) {
    x = 0.0;
  } else if (x > 1) {
    x = 1.0;
  }
  return x;
}

static float button(float dt) {
  float speed = 1.0f;
  float x = _last_x;
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
    x -= speed * dt;
  }
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
    x += speed * dt;
  }

  if (x < 0.0f) {
    x = 0.0f;
  } else if (x > 1.0f) {
    x = 1.0f;
  }

  return x;
}

void INPUT_init() {
  AverageFilter_init(&sliderFilter);
  AverageFilter_init(&joystickFilter);
  AverageFilter_init(&knobFilter);
}

void INPUT_loop() {
  AverageFilter_add(&sliderFilter, INPUT_adc_read[ADC_SLIDER_INDEX]);
  AverageFilter_add(&knobFilter, INPUT_adc_read[ADC_KNOB_INDEX]);
  AverageFilter_add(&joystickFilter, INPUT_adc_read[ADC_JOYSTICK_INDEX]);

  if (AverageFilter_sumOfDifference(&sliderFilter) > 800) {
    INPUT_device_type = INPUT_DEVICE_SLIDER;
  } else if (AverageFilter_sumOfDifference(&knobFilter) > 800) {
    INPUT_device_type = INPUT_DEVICE_KNOB;
  } else if (AverageFilter_sumOfDifference(&joystickFilter) > 800) {
    INPUT_device_type = INPUT_DEVICE_JOYSTICK;
  } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ||
             HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
    INPUT_device_type = INPUT_DEVICE_BUTTON;
  }
}

float INPUT_get_x(float dt) {
  switch (INPUT_device_type) {
  case INPUT_DEVICE_BUTTON:
    return _last_x = button(dt);
  case INPUT_DEVICE_SLIDER:
    return _last_x = slider(dt);
  case INPUT_DEVICE_KNOB:
    return _last_x = knob(dt);
  case INPUT_DEVICE_JOYSTICK:
    return _last_x = joystick(dt);
  default:
    break;
  }

  return _last_x;
}

int INPUT_get_device_type() { return INPUT_device_type; }