#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include "iesusart.h"

#include "iesmotors.h"

#define ADC0 PC0
#define MAX_ADC 1023.0
#define MAX_VOLT 5.0
#define SAMPLE_SIZE 20

unsigned int cnt = 0;
unsigned int cnt2 = 0;

typedef enum {
    LEFT_SENSOR,
    MID_SENSOR,
    RIGHT_SENSOR
} Sensor;

typedef enum {
    IDLE,
    FORWARD,
    BACKWARD,
    LEFT_TURN,
    RIGHT_TURN,
    LEFT_ROTATION,
    RIGHT_ROTATION
} State ;

// TODO steht für für ein Interrupt siehe Mehdi E3T3
ISR (TIMER2_COMPA_vect) {
    cnt+=1;
    cnt2+=1;
}

void setup_timer2() {
    cli();
    TCCR2B = (1 << CS00); // Prescaler: 1
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void ADC_Init(void) {
    ADMUX = (1 << REFS0); //Why not explicitely set bit REFS1 to 0?
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Set clock (Takt)?
    ADCSRA |= (1 << ADEN); //Before you can start the conversion by setting the ADSC bit, you have to "allow" it first by setting the ADEN-bit.
    ADCSRA |= (1 << ADSC); // Single conversion
    while (ADCSRA & (1 << ADSC) );
    ADCW; // Read once to "warm up" ADC.
}

uint16_t ADC_Read(uint8_t channel) {
    //Returns the ADC-value of the tension. !Not in Volt! - Has to be converted into Volt
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F); //No idea what happens here.
    ADCSRA |= (1 << ADSC); //Set the start bit
    while (ADCSRA & (1 << ADSC) ); //Wait till MCU is finished with the calculation
    return ADCW; //Now that ADSC bit is low again, MCU finished the conversion and we are ready to return the calculated ADC-value.
}

uint16_t adc_read_avg(uint8_t channel, uint8_t nsamples) {
    //Returns a more precise ADC-value of the tension. Use this over ADC_Read for more precision!
    //More precision is guaranteed by taking the arithmetic mean of multiple calculations.
    uint32_t sum = 0;
    for (uint8_t i = 0; i < nsamples; ++i ) {
        sum += ADC_Read( channel );
    }

    return (uint16_t)( sum / nsamples );
}



void init_adc() {
    // Set pins as input.
    DDRC &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2));
    // Init USART to be able to send data to serial Port
    USART_init(UBRR_SETTING);
    setup_timer2();
    ADC_Init();
}
// -------------------------  MOTORS  -----------------------




void init_motors() {
// Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;

    // TODO alle pins müssen auf output gestellt sein.
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);

    // Set PB0, PB1, and PB3 as output (IN[1|2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3) | (1 << DD7);

    // Make PWM work on PD[5|6]
    setup_timer0();

    // TODO states für verschiedene Speedsettings!
    set_duty_cycle(PD5, 155);
    set_duty_cycle(PD6, 155);
}

int main(void) {
    init_adc();
    init_motors();

    while(1) {

        short adc_left = adc_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
        short adc_mid = adc_read_avg(MID_SENSOR, SAMPLE_SIZE);
        short adc_right = adc_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);


        /*USART_print("ADCVAL: ");
        sprintf(strbuff, "%u", adcval);
        USART_print(strbuff);
        USART_print("\n");
        USART_print("Volt: ");
        sprintf(strbuff, "%u", voltval_int);
        USART_print(strbuff);
        USART_print(".");
        sprintf(strbuff, "%u", voltval_frac);
        USART_print(strbuff);
        USART_print("\n\n\n");
        _delay_ms(500);*/
    }
    return 0;
}
