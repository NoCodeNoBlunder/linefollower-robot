

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "iesleds.h"
#include "iesusart.h"
#include "util.h"

#define SHORT_WTIME 250
#define MID_WTIME 500
#define SR_LED_COUNT 3

void toggle_DI() {
    toggle_pin(&PORTB, PB2);
}

void send_flanks(char flank_count) {
    for (char i = 0; i < flank_count; i++) {
        toggle_pin(&PORTD, PD4);
        toggle_pin(&PORTD, PD4);
    }
}

void reset() {
    set_pin_low(&PORTB, PB2);
    send_flanks(SR_LED_COUNT);
}

void light_led(LineFollower diode) {

    // TODO muss ich immer resetten?
    reset();

    switch (diode) {
        case LEFT_LF:
            toggle_DI();
            send_flanks(1);
            break;
        case MID_LF:
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            break;
        case RIGHT_LF:
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
        case LEFT_AND_MID:
            toggle_DI();
            send_flanks(2);
            break;
        case RIGHT_AND_MID:
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
}

void leds_Init() {
    set_pin_high(&DDRB, DDB2);
    set_pin_high(&DDRD, DDD4);
}



