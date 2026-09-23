#ifndef __SERVO_H
#define __SERVO_H
#include "stm32f4xx_hal.h"


void Servo_SetAngle (TIM_HandleTypeDef *htim,uint32_t Channel,float angle);
void Servo_test1(void);
void Servo_test2(void);
void Servo_test3(void);
void Servo_test4(void);
void Servo_Init(void);
void Servo_Init2(void);
void Servo_Init_tremble(void);
void Servo_Init_tremble2(void);
void hand(void);
float xyset(float x,float y);
void perform(void);

#endif