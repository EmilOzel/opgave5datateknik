#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void ADC_Init();
uint16_t Read_ADC(uint8_t channel);

#endif
