#ifndef SERVO_H
#define SERVO_H


#include "stm32f4xx_hal.h"

void servo_init(TIM_HandleTypeDef* htim, uint32_t channel);

void set_servo_pulse(TIM_HandleTypeDef* htim, uint32_t channel, float degrees);



#endif