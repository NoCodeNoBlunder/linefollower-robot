/**
 * @file adc.c
 * @brief Provides basic functionality to read the volatege from a pin and convert it
 * to a digital signal
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../inc/usart.h"
#include "../inc/adc.h"
#include "../inc/main.h"

void ADC_Init() {
    DDRD = 0;
    DDRB = 0;

    DDRC &= ~((1 << LEFT_LF) | (1 << MID_LF) | (1 << RIGHT_LF));

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Set clock (Takt)?
    ADCSRA |= (1 << ADEN);                                // Before you can start the conversion by setting the ADSC bit, you have to "allow" it first by setting the ADEN-bit.
    ADCSRA |= (1 << ADSC);                                // Single conversion
    while (ADCSRA & (1 << ADSC)) { }
    ADCW;                                                 // Read once to "warm up" ADC.
}

/**
 * Function to read the voltage and convert it to digital
 * @param channel pin to read from
 * @return digital uint16_6 value ranging from 0-1023
 */
uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
    ADCSRA |= (1 << ADSC);                          // Set the start bit
    while (ADCSRA & (1 << ADSC)) { }                // Wait till MCU is finished with the calculation
    return ADCW;                                   // Now that ADSC bit is low again, MCU finished the conversion and we are ready to return the calculated ADC-value.
}

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
