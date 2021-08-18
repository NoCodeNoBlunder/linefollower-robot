
#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "iesleds.h"
//#include "iesusart.h"
#include "util.h"
#include <stdio.h>
#include "linefollower.h"

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

void light_led(LineFollower next_led_mode) {

    // TODO muss ich immer resetten?
    reset();

    switch (next_led_mode) {
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
        default:
            break;
    }
}

void select_and_light_led(FSM *fsm, RoboterData *data) {

    if (mid_on_line(data)) {
        if (left_on_line(data) && right_on_line(data)) {
            light_led(ALL);
            transition_to_state(fsm, data, COUNTDOWN);
        }
        else if (!left_on_line(data) && right_on_line(data)) {
            light_led(RIGHT_AND_MID);
        }
        else if (left_on_line(data) && !right_on_line(data)) {
            light_led(LEFT_AND_MID);
        }
        else {
            light_led(MID_LF);
        }
    }
    else {
        if (!left_on_line(data) && right_on_line(data)) {
            light_led(RIGHT_LF);
        }
        else if (left_on_line(data) && !right_on_line(data)) {
            light_led(LEFT_LF);
        }
        else if (left_on_line(data) && right_on_line(data)) {
            light_led(LEFT_AND_RIGHT);
        }
        else {
            light_led(NONE);
        }
    }
}


void leds_Init() {
    // Set both pins as output.
    set_pin_high(&DDRB, DDB2);
    set_pin_high(&DDRD, DDD4);
}



