#ifndef __INPUT_DEVICE_H__
#define __INPUT_DEVICE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

enum INPUT_DeviceType {
  INPUT_DEVICE_NONE,
  INPUT_DEVICE_JOYSTICK,
  INPUT_DEVICE_BUTTON,
  INPUT_DEVICE_SLIDER,
  INPUT_DEVICE_KNOB,
  INPUT_DEVICE_IMU,
  INPUT_DEVICE_MAX
};

extern uint32_t INPUT_adc_read[3];

void INPUT_init();
void INPUT_loop();
float INPUT_get_x(float dt); // x \in [0,1]
int INPUT_get_device_type();

struct AverageFilter;
extern struct AverageFilter sliderFilter;
extern struct AverageFilter joystickFilter;
extern struct AverageFilter knobFilter;
void AverageFilter_init(struct AverageFilter *filter);
void AverageFilter_add(struct AverageFilter *filter, uint32_t value);
uint32_t AverageFilter_sumOfDifference(struct AverageFilter *filter);
uint32_t AverageFilter_get(struct AverageFilter *filter);

#ifdef __cplusplus
}
#endif
#endif