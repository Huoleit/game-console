#ifndef __INPUT_DEVICE_H__
#define __INPUT_DEVICE_H__
#ifdef __cplusplus
extern "C" {
#endif

enum INPUT_DeviceType {
  INPUT_DEVICE_NONE,
  INPUT_DEVICE_JOYSTICK,
  INPUT_DEVICE_BUTTON,
  INPUT_DEVICE_SLIDER,
  INPUT_DEVICE_IMU,
  INPUT_DEVICE_MAX
};

float INPUT_get_x(int deviceID, float dt); // x \in [0,1]

#ifdef __cplusplus
}
#endif
#endif