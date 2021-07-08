#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include "iesusart.h"
#define ADC0 PC0

// TODO globale Variablen werden die gebraucht kommentieren!
unsigned int cnt = 0;
unsigned int cnt2 = 0;

ISR (TIMER2_COMPA_vect) {
    cnt+=1;
    cnt2+=1;
}

// TODO do i need this and if yes what for?
// das wird beim Ultraschall benutzt. Gehört hier nicht hin.
void setup_timer2() {
    cli();
    TCCR2B = (1 << CS00); // Prescaler: 1
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void ADC_Init() {
    // added myself!
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2));

    ADMUX = (1 << REFS0); // Why not explicitely set bit REFS1 to 0?
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set clock (Takt)?
    ADCSRA |= (1 << ADEN); // Before you can start the conversion by setting the ADSC bit, you have to "allow" it first by setting the ADEN-bit.
    ADCSRA |= (1 << ADSC); // Single conversion
    while (ADCSRA & (1 << ADSC) );
    ADCW; // Read once to "warm up" ADC.

    // TODO Potential danger! Why is this needed?
    // setup_timer2();
}

uint16_t ADC_Read(uint8_t channel) {
    // Returns the ADC-value of the tension. !Not in Volt! - Has to be converted into Volt
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F); // No idea what happens here.
    ADCSRA |= (1 << ADSC); // Set the start bit
    while (ADCSRA & (1 << ADSC) ); // Wait till MCU is finished with the calculation
    return ADCW; // Now that ADSC bit is low again, MCU finished the conversion and we are ready to return the calculated ADC-value.
}

uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples) {
    // Returns a more precise ADC-value of the tension. Use this over ADC_Read for more precision!
    // More precision is guaranteed by taking the arithmetic mean of multiple calculations.
    uint32_t sum = 0;
    for (uint8_t i = 0; i < nsamples; ++i) {
        sum += ADC_Read(channel);
    }

    return (uint16_t)(sum / nsamples);
}
