#include <avr/io.h> 	   // microcontroller
#include <stdio.h> 		   // sprintf()
#include <string.h> 	   // memset();
#include <avr/interrupt.h> // cli(), sli();
#include "iesusart.h"      // USART functionality

#define LED_BLUE PC0
#define LED_GREEN PC1
#define LED_YELLOW PC2
#define ADC0 PC0
#define ADC1 PC1
#define ADC2 PC2
#define ECHO PB4
#define TRIGGER PB5
#define TRANSISTOR PC3

#define MIN_VOLT (5.0f / 1023)
#define DEC_PLACES 1000
#define STR_BUF2_SIZE 30
#define CH0 PC0
#define CH1 PC1
#define CH2 PC2
#define SAMPLESIZE 20

//  CHANGES:
// -Fixed USART_Init problem.
// -name changes to snake_case.
// -Einschiebung und Whitespaces korrigiert.
// -added funcionality to transmit voltage as a rep of float
// -added Macros.

unsigned int cnt = 0;
unsigned int cnt2 = 0;

ISR(TIMER2_COMPA_vect) {
    cnt += 1;
    cnt2 += 1;
}

void setup_timer() {
    cli();
    TCCR2B = (1 << CS00); // Prescaler: 1
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void ADC_init(void) {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
    ADCSRA |= (1 << ADSC); // Single conversion
    while (ADCSRA & (1 << ADSC));
    ADCW; // Read once to "warm up" ADC.
}


uint16_t ADC_read(uint8_t channel) {
    // 0x1F durch EECR ersetzt.
    ADMUX = (ADMUX & ~(EECR)) | (channel & EECR);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADCW;
}

// reads multiple samples and returns average!
uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples) {
    uint32_t sum = 0;

    for (uint8_t i = 0; i < nsamples; ++i) {
        sum += ADC_read(channel);
    }
    return (uint16_t)(sum / nsamples);
}

float convert_dig_value(int dig_value) {
    return dig_value * MIN_VOLT;
}

void f_to_str(char *str_buf, float f_to_convert) {
    char pre_comma = (char)f_to_convert;
    float post_comma = (f_to_convert - pre_comma);
    short post_comma_int = (short)((f_to_convert - pre_comma) * DEC_PLACES);

    short zero_count = -1;
    while(post_comma < 1) {
        post_comma *= 10;
        zero_count++;
    }

    static char zero_str[STR_BUF2_SIZE];
    zero_str[zero_count] = '\0';
    memset(zero_str, '0', zero_count);

    // sprintf does not work with floats
    sprintf(str_buf, "ADCVAL: %hu.%s%hu\n", pre_comma, zero_str, post_comma_int);
}

void init() {
    DDRC &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2));

    DDRC &= ~(1 << TRANSISTOR);
    PORTC &= ~(1 << TRANSISTOR);

    USART_init(UBRR_SETTING);
    setup_timer();
    ADC_init();
}

int main(void) {

    init();
    char str_buf[STR_BUF2_SIZE];
    int prev_value = ADC_read_avg(CH0, SAMPLESIZE);

    f_to_str(str_buf, convert_dig_value(prev_value));
    USART_print(str_buf);

    while(1) {
        // Only print when there was change.
        int adc_val0 = ADC_read_avg(CH0, SAMPLESIZE);
        if (prev_value != adc_val0) {
            f_to_str(str_buf, convert_dig_value(adc_val0));
            USART_print(str_buf);
            prev_value = adc_val0;
        }
    }
    return 0;
}
