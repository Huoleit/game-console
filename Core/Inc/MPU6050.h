#ifndef __INC_MPU6050_H_
#define __INC_MPU6050_H_

#ifdef __cplusplus
extern "C" {
#endif

void IMU_init();
float IMU_readX();
void IMU_Start_reading();

#ifdef __cplusplus
}
#endif

#endif /* __INC_MPU6050_H_ */
