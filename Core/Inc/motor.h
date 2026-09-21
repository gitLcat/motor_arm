#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx.h"

void print_encoder_count(void);
void pos_pid(float dt);
void motor_forward(uint8_t forward);
void updated_ramp_target(void);
void set_home(void);
void motor_move_to(float revolutions);
void update_target_from_pot(void);

#endif