
#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "iesleds.h"
#include "iesusart.h"
#include "util.h"
#include <stdio.h>

#define SHORT_WTIME 250
#define MID_WTIME 500
#define SR_LED_COUNT 3
#define SR_CLK PD4
#define SR_DATA PB2

void toggle_DI() {
    toggle_pin(&PORTB, SR_DATA);
}

void send_flanks(char flank_count) {
    for (char i = 0; i < flank_count; i++) {
        toggle_pin(&PORTD, SR_CLK);
        toggle_pin(&PORTD, SR_CLK);
    }
}

void reset() {
    set_pin_low(&PORTB, SR_DATA);
    send_flanks(SR_LED_COUNT);
}

void light_led(LineFollower diode) {
	
    // TODO muss ich immer resetten?
    reset();
  
    switch (diode) {
        case RIGHT_LF:
			//USART_print("\nlight led mode LEFT_HARD LF\n");
            toggle_DI();
            send_flanks(1);
            break;
        case MID_LF:
			//USART_print("\nlight led mode MID LF\n");
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            break;
        case LEFT_LF:
			//USART_print("\nlight led mode RIGHT_HARD LF\n");
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(2);
            break;
        case LEFT_AND_RIGHT:
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            break;
        case RIGHT_AND_MID:
            toggle_DI();
            send_flanks(2);
            break;
        case LEFT_AND_MID:
            toggle_DI();
            send_flanks(2);
            toggle_DI();
            send_flanks(1);
            break;
        case ALL:
            toggle_DI();
            send_flanks(3);
            break;
        case NONE:
            break;
    }
    
    //_delay_ms(1000);
}

void leds_Init() {
    // Set both pins as output.
    set_pin_high(&DDRB, DDB2);
    set_pin_high(&DDRD, DDD4);
}



