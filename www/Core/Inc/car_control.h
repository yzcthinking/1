#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <stdint.h>

void Car_Control_Init(void);
void Car_Control_SetVelocity(float linear_x, float linear_y, float angular_z);
void Car_Control_Update(void);
void Car_Control_Stop(void);
void Car_Control_TimerIrq(void);

#endif