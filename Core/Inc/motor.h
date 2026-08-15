#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx.h"

typedef struct {
    TIM_HandleTypeDef *htim_encoder;
    TIM_HandleTypeDef *htim_pwm;
    uint32_t pwm_channel;
    GPIO_TypeDef *dir_port;
    uint16_t dir_pin1;
    uint16_t dir_pin2;
    int32_t target_pos;
    float ramped_target, final_target, ramp_step;
    float integral_pos, prev_pos_error;
    float Kp, Ki, Kd;
    float last_pwm_output;
} MotorController;

void print_encoder_count(MotorController *motor1 , UART_HandleTypeDef *huart);
void pos_pid(float dt);
void motor_forward(uint8_t forward);
void upated_ramp_target(void);
void set_home(void);
void motor_move_to(float revolutions);

#endif

