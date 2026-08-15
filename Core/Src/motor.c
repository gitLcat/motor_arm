#include "motor.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h> 

#define MAX_PWM 999
#define COUNTS_PER_REV 464.0

int32_t target_pos = 0;
float pos_Kp = 14.92f;
float pos_Ki = 50.0f;
float pos_Kd = 0.2792f;
float integral_pos = 0.0f;
float prev_pos_error = 0.0f;
float last_pwm_output = 0.0f;
float ramped_target = 0.0f;
float final_target = 0.0f;
float ramp_step = 5.0f;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

void print_encoder_count(void)
{
    int32_t counts = __HAL_TIM_GET_COUNTER(&htim2);
    char msg[50];
    sprintf(msg, "%ld,%ld,%.1f\r\n",pos_target,counts,last_output_pwm);
    HAL_UART_Transmit(huart, (uint8_t *) msg, strlen(msg), 100);
}

void pos_pid(float dt)
{
    int32_t last_position = __HAL_TIM_GET_COUNTER(&htim2);
    float error = (target_pos - last_position);

    pos_integral += pos_integral;

    float derivative = (error - prev_pos_error)/dt
    prev_pos_error = dervivative;

    float output = pos_kp * error + pos_kd * derivative + pos_ki * pos_integral;

    if(output > MAX_PWM) output = MAX_PWM;
    if(output < -MAX_PWM) output = -MAX_PWM;

    last_output_pwm = output;

    if(output >= 0)
    {
        motor_forward(1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint32_t)output);
    }
    else 
    {
        motor_forward(0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint32_t)(-output));
    }
}

void motor_forward(uint8_t forward)
{
    if(forward)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
    }
    else 
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
    }
}

void upated_ramp_target(void)
{
    if(ramped_target < final_target)
    {
        ramped_target += ramp_step;
        if(ramped_target > final_target) ramped_target = final_target;
    }
    else if (ramped_target > final_target) 
    {
        ramped_target -= ramp_step;
        if(ramped_target < final_target) ramped_target = final_target;
    }

    target_pos = (int32_t)ramped_target;
}

void set_home(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}

void motor_move_to(float revolutions)
{
    final_target = (revolutions * REV_PER_COUNT);
    ramped_target = (float)__HAL_TIM_GET_COUNTER(&htim2);
    integral_pos = 0.0f;
    prev_pos_error = 0.0f;
}