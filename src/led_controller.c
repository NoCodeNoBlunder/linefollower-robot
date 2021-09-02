
/**
 * @file
 * @brief
 * implementation of the leds controller.
 */

#include <avr/io.h>
#include <stdio.h>
#include "../inc/main.h"
#include "../inc/led_controller.h"
#include "../inc/usart.h"
#include "../inc/util.h"
#include "../inc/linefollower.h"

/**
 * @def
 * @brief The amount of leds mounted on the robot.
 */
#define SR_LED_COUNT 3

/**
 * @def
 * @brief
 * Which port the SR_CLK in connected to the microcontroller.
 */
#define SR_CLK PD4

/**
 * @def
 * @brief
 * Which port the SR_DATA is connected to the microcontroller.
 */
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

    reset();

    switch (next_led_mode) {
        case RIGHT_LF:
            toggle_DI();
            send_flanks(1);
            break;
        case MID_LF:
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            break;
        case LEFT_LF:
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
    /** Set both pins as output. */
    set_pin_high(&DDRB, DDB2);
    set_pin_high(&DDRD, DDD4);
}



