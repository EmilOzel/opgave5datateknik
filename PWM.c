#include "PWM.h"

void PWM_Init() {
    // Sæt pin 4 som output for PWM//
    DDRG |= (1<<DDG5); // Sæt pin 4 (PORTG5) som output//

    // Konfigurer Timer 0 (OC0B) til Fast PWM//
    TCCR0A |= (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
    TCCR0B |= (1<<CS00); // Ingen prescaler
}

void Set_PWM(uint8_t dutyCycle) {
    // Sæt PWM duty cycle for pin 4 (OC0B)//
    OCR0B = dutyCycle;
}
