#include "motor.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include <stdint.h>
#include <stdio.h>




void print_encoder_count(MotorController *motor1 , UART_HandleTypeDef *huart)
{
    int32_t counts = __HAL_TIM_GET_COUNTER(motor -> htim_encoder);
    char msg[50];
    sprintf(msg, "%ld,%ld,%.1f\r\n",pos_target,counts,last_output_pwm);
    HAL_UART_Transmit(huart, (uint8_t *) msg, strlen(msg), 100);
}

void pos_pid(float dt)
{
    int32_t last_position = __HAL_TIM_GET_COUNTER(__HANDLE__);
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
        __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__);
    }
    else 
    {
        motor_forward(0);
        __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__);
    }
}

void motor_forward(uint8_t forward)
{
    if(forward)
    {
        HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
        HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
    }
    else 
    {
        HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
        HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
    }
}

void upated_ramp_target(void)
{
    if(ramped_target < final_target)
    {
        ramped_target += ramped_step;
        if(ramped_target > final_target) ramped_target = final_target;
    }
    else if (ramped_target > final_target) 
    {
        ramped_target -= ramped_step;
        if(ramped_target < final_target) ramped_target = final_target;
    }

    target_pos = ramped_target;
}

void set_home(void)
{
    __HAL_TIM_SET_COUNTER(__HANDLE__, __COUNTER__);
}

void motor_move_to(float revolutions)
{
    final_target = (revolutions * REV_PER_COUNT);
    ramped_target = (float)__HAL_TIM_GET_COUNTER(__HANDLE__);
    integral_pos = 0.0f;
    prev_pos_error = 0.0f;
}