#include "ADC.h"

void ADC_Init() {
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN) | (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Aktiver ADC og sæt prescaler//
}

uint16_t Read_ADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    ADCSRA |= (1<<ADSC); // Start konvertering//
    while(ADCSRA & (1<<ADSC)); // Vent på konverteringens afslutning//
    return (ADC);
}
