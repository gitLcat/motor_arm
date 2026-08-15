#include "servo.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>


void servo_init(TIM_HandleTypeDef* htim, uint32_t channel)
{
    HAL_TIM_PWM_Start(htim,  channel);
}

void set_servo_pulse(TIM_HandleTypeDef* htim, uint32_t channel, float degrees)
{
    if(degrees < 0) degrees = 0;
    if(degrees > 180) degrees = 180;

    uint16_t pulse = (500 + (uint16_t) (degrees / 180.0f) * (25000 - 500));
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}

