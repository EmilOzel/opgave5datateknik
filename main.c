//Emil Ozel og Heini Poulsen
//02/12-2023
//Datateknik og Programmering
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "ssd1306.h"
#include "ADC.h"
#include "PWM.h"

#define BAUD 9600
#define UBRR_VALUE ((F_CPU / 16 / BAUD) - 1)

void USART_Init() {
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Aktiverer receiver og transmitter//
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Sæt frame format 8data 1 stop bit/7
}
int UART_Data_Available() {
    return (UCSR0A & (1<<RXC0));
}
unsigned char USART_Receive() {
    while (!(UCSR0A & (1<<RXC0))); // Vent på data at blive modtaget//
    return UDR0; // Hent og returner modtaget data fra bufferen//
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1<<UDRE0))); // Vent på tom transmit buffer//
    UDR0 = data; // Sæt data i bufferen, sender data//
}

uint16_t Read_UART_Value() {
    char buffer[10];
    uint8_t i = 0;
    char received;

    // Læs karakterer indtil ny linje eller buffer er fuld//
    while (i < sizeof(buffer) - 1) {
        received = USART_Receive();
        if (received == '\n') break;
        buffer[i++] = received;
    }
    buffer[i] = '\0'; // Null-terminate strengen//

    // Konverter strengen til et tal//
    return (uint16_t)atoi(buffer);
}

void UpdateDisplay(uint16_t adcValue, uint8_t pwmValue) {
    char buffer[20];
    clear_display();
    snprintf(buffer, sizeof(buffer), "ADC: %u", adcValue);
    sendStrXY(buffer, 0, 0);
    snprintf(buffer, sizeof(buffer), "PWM: %u%%", (uint16_t)(pwmValue * 100 / 255));
    sendStrXY(buffer, 0, 1);
}

void setup() {
    I2C_Init();
    InitializeDisplay();
    ADC_Init();
    PWM_Init();
    USART_Init(); // Initialiser UART//
}

void loop() {
    static uint16_t minPWM = 0;
    static uint16_t maxPWM = 255;

    // Læs minimum og maksimum PWM-grænser fra UART//
    if (UART_Data_Available()) { // Antager denne funktion eksisterer//
        minPWM = Read_UART_Value();
        maxPWM = Read_UART_Value();
        printf("Min PWM: %u, Max PWM: %u\n", minPWM, maxPWM); // Send tilbage til UART
    }

    uint16_t adcValue = Read_ADC(0);
    uint16_t pwmValue = (uint16_t)((adcValue * (maxPWM - minPWM)) / 1023 + minPWM);
    Set_PWM(pwmValue);
    UpdateDisplay(adcValue, pwmValue);
    _delay_ms(500);
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
