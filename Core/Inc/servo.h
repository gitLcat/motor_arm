#ifndef __SERVO_H__
#define __SERVO_H__

#include "stm32f4xx.h"

void update_servo_from_pot(void);
void update_joint_servo_from_pot(void);
void update_claw_servo_from_pot(void);
#endif