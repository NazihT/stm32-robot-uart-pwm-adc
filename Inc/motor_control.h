#ifndef PWM_H
#define PWM_H

#include "stm32f407xx.h"


void PWM_Init(void);
void SetMotors(uint8_t motor, uint8_t dir, uint16_t speed);
void control_car(const char* cmd);

#endif
