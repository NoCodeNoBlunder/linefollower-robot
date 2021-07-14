/**
 * @file iesadc.c
 * @brief Provides basic functionality to read the volatege from a pin and convert it
 * to a digital signal
 * @version 0.1
 * @date 2021-06-08
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"
#include "main.h"

// TODO globale Variablen werden die gebraucht kommentieren!?
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
    TCCR2B = (1 << CS00);     // Prescaler: 1
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void ADC_Init() {
    // added myself!
    DDRC &= ~((1 << LEFT_LF) | (1 << MID_LF) | (1 << RIGHT_LF));

    ADMUX = (1 << REFS0);                                 // Why not explicitely set bit REFS1 to 0?
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Set clock (Takt)?
    ADCSRA |= (1 << ADEN);                                // Before you can start the conversion by setting the ADSC bit, you have to "allow" it first by setting the ADEN-bit.
    ADCSRA |= (1 << ADSC);                                // Single conversion
    while (ADCSRA & (1 << ADSC));
    ADCW;                                                 // Read once to "warm up" ADC.

    // TODO Remove only needed for Ultrasonic.
    // setup_timer2();
}

/* Reads once from channel and converts analog value to digital from 0-1023 */
/**
 * Function to read the voltage and convert it to digital
 * @param channel pin to read from
 * @return digital uint16_6 value ranging from 0-1023
 */
uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);   // TODO was passiert hier?
    ADCSRA |= (1 << ADSC);                          // Set the start bit
    while (ADCSRA & (1 << ADSC) );                  // Wait till MCU is finished with the calculation
    return ADCW;                                    // Now that ADSC bit is low again, MCU finished the conversion and we are ready to return the calculated ADC-value.
}

/* Combines multiple readings from channel to return more accurate values. */

/**
 * Combines multiple readings to return more accurate values
 * @param channel pin to read from
 * @param nsamples number of samples
 * @return average value of readings
 */
uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < nsamples; ++i) {
        sum += ADC_Read(channel);
    }

    return (uint16_t)(sum / nsamples);
}
