#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include "iesusart.h"
#define ADC0 PC0
#define MAX_ADC 1023.0
#define MAX_VOLT 5.0
//Removed some leftover constants (no leds and transistors anymore)
//Added constant for max ADC-Value and max Volt-Value


unsigned int cnt = 0;
unsigned int cnt2 = 0;

ISR (TIMER2_COMPA_vect) {
  cnt+=1;
  cnt2+=1;
}

void setup_timer2() {
  cli();
  TCCR2B = (1<<CS00); // Prescaler: 1
  TIMSK2 |= (1<<OCIE2A);
  TCCR2A = (1<<WGM01);
  TCNT2 = 0;  
  OCR2A = 255; 
  sei();
}

void ADC_Init(void) {
  ADMUX = (1<<REFS0); //Why not explicitely set bit REFS1 to 0?  
  ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //Set clock (Takt)?
  ADCSRA |= (1<<ADEN); //Before you can start the conversion by setting the ADSC bit, you have to "allow" it first by setting the ADEN-bit.
  ADCSRA |= (1<<ADSC); // Single conversion
  while (ADCSRA & (1<<ADSC) );
  ADCW; // Read once to "warm up" ADC.
}

uint16_t ADC_Read(uint8_t channel) {
  //Returns the ADC-value of the tension. !Not in Volt! - Has to be converted into Volt
  ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F); //No idea what happens here.
  ADCSRA |= (1<<ADSC); //Set the start bit
  while (ADCSRA & (1<<ADSC) ); //Wait till MCU is finished with the calculation
  return ADCW; //Now that ADSC bit is low again, MCU finished the conversion and we are ready to return the calculated ADC-value.                   
}

uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples) {
  //Returns a more precise ADC-value of the tension. Use this over ADC_Read for more precision! 
  //More precision is guaranteed by taking the arithmetic mean of multiple calculations.
  uint32_t sum = 0;
  for (uint8_t i = 0; i < nsamples; ++i ) {
    sum += ADC_Read( channel );
  }

  return (uint16_t)( sum / nsamples );
}


void init_ADC() {
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2));
    USART_init(UBRR_SETTING);
    ADC_Init();
    setupTimer2();
}

/*
int main(void) {

    short adcval = -1;
    while(1) {
	if ( adcval == ADC_Read_Avg(0, 20) ) { 
		continue; //waiting till change detected
	}
        adcval = ADC_Read_Avg(0, 20); //Changed int to short. We will never surpass 10-bit-values. (Max-Value is 1023 = 0b11111111)
	float voltval = adcval * ( MAX_VOLT / MAX_ADC );
	unsigned char voltval_int = (char) voltval; 
	unsigned char voltval_frac = (char) trunc((voltval_int - voltval)*1000);
        
        char strbuff[17];
        
        USART_print("ADCVAL: ");
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
	_delay_ms(500);

    }
    return 0;
}*/
