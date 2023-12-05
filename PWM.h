#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

void PWM_Init();
void Set_PWM(uint8_t dutyCycle);

#endif
