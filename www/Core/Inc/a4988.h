/*
  a4988.cpp - - Arduino library for using the a4988 stepper driver
  William Smith, 2014
  
  The A4988 stepper driver is for Pololu stepper driver boards
  and compatible clones. These boards use the Allegro a4988
  stepper motor driver IC. (see Allegro website for datasheet)

  This library diverges from others that are around, in that it
  assumes that the MS1, MS2, and MS3 pins are connected to gpio
  pins on the Arduino, allowing control over the microstepping
  modes.
  
  The A4988 is capable of microstepping down to 1/16 of a step,
  enabling fine control over the stepper motor. This fine control
  can be used in, among other things, 3D printers.

  This library provides an interface for setting the different
  step modes, going from full step down to 1/16 step, using a
  simple setter function, where the argument is 1,2,4,8, or 16.

  
     MS1   MS2   MS3
    -----------------
     low   low   low   Full step
     high  low   low   Half step
     low   high  low   1/4 step
     high  high  low   1/8 step
     high  high  high  1/16 step

  Note:
  Lower delay values can be used in the microstepping mode.
  Values as low as 25 usec can be used in the 1/16 mode
  with some motors. 

*/

#ifndef A4988_H
#define A4988_H

#include "main.h"

typedef struct
{
    GPIO_TypeDef *ms1_port;
    uint16_t ms1_pin;
    GPIO_TypeDef *ms2_port;
    uint16_t ms2_pin;
    GPIO_TypeDef *ms3_port;
    uint16_t ms3_pin;
    GPIO_TypeDef *dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef *enable_port;
    uint16_t enable_pin;
    GPIO_TypeDef *step_port;
    uint16_t step_pin;
} A4988_HandleTypeDef;

void A4988_Init(A4988_HandleTypeDef *driver);
void A4988_SetDelayUs(uint32_t delay_us);
void A4988_Enable(A4988_HandleTypeDef *driver, uint8_t enable);
void A4988_SetDirection(A4988_HandleTypeDef *driver, uint8_t direction);
HAL_StatusTypeDef A4988_SetStepMode(A4988_HandleTypeDef *driver, uint8_t step_mode);
void A4988_StepOnce(A4988_HandleTypeDef *driver);
void A4988_Step(A4988_HandleTypeDef *driver, uint32_t steps);

#endif
