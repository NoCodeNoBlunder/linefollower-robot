
/**
 * @file
 * @brief
 * Implements both the start and end Countdown using Timer1 with Overflow
 * interrupt.
 */

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/wdt.h>

#include "../inc/main.h"
#include "../inc/countdown_states.h"
#include "../inc/led_controller.h"
#include "../inc/usart.h"

#include "../inc/motor_controller.h"
#include "../inc/linefollower.h"

#define F_LEDS 5
#define STAR_C_DUR 15
#define END_C_DUR 60

#define TIMER_SIZE 65536.0
#define OF_FREQUENCY (F_CPU / TIMER_SIZE)
#define LED_CYCLE_TIME ((1.0 / F_LEDS) / 2.0)

#define OF_PER_CYCLE ((unsigned int)(OF_FREQUENCY * LED_CYCLE_TIME))
#define TOTAL_OF_START ((unsigned int)(OF_FREQUENCY * STAR_C_DUR))
#define TOTAL_OF_END ((unsigned int)(OF_FREQUENCY * END_C_DUR))

typedef enum {
    START_C,
    END_C,
} COUNTDOWNMODE;

volatile unsigned int cnt = 0;
volatile unsigned int cnt_countdown = 0;
volatile unsigned char toggle_led = false;
volatile unsigned char countdown_finished = false;
volatile LineFollower current_led_state = ALL;
volatile int of_for_countdown;

ISR (TIMER1_OVF_vect) {
    cnt += 1;
    cnt_countdown += 1;

    if(cnt == OF_PER_CYCLE) {
        cnt = 0;
        toggle_led = true;
    }


    if(cnt_countdown == of_for_countdown) {
        countdown_finished = true;
    }
}

void Timer1_init() {
    cli();
    TCCR1B = (1 << CS10);  /** Prescaler: 1 */
    /** set normal mode */
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B &= ~((1 << WGM12) | (1 << WGM13));
    sei();
}

void enable_isr_countdown(COUNTDOWNMODE mode) {

    switch (mode) {
        case START_C:
            of_for_countdown = TOTAL_OF_START;
            break;
        case END_C:
            of_for_countdown = TOTAL_OF_END;
            break;
    }

    cnt = 0;
    cnt_countdown = 0;

    toggle_led = false;
    countdown_finished = false;

    cli();
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0; /** set counter value to 0 */
    sei();
}

void disable_isr_countdown() {
    cli();
    TIMSK1 &= ~(1 << TOIE1); /** disable compare match interrupt. */
    sei();
}

void enter_countdown(RoboterData *data) {
    light_led(ALL);
    enable_isr_countdown(START_C);
}

void update_countdown(FSM *fsm, RoboterData *data) {

    take_measurement(data);

    if (countdown_finished) {
        disable_isr_countdown();
        transition_to_state(fsm, data, LEAVE_START);
    }
    else if (!all_on_line(data)){
        disable_isr_countdown();
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }

    if (toggle_led) {
        toggle_led = false;

        if (current_led_state == ALL) {
            light_led(NONE);
            current_led_state = NONE;
        }
        else {
            light_led(ALL);
            current_led_state = ALL;}
    }
}

void enter_goal_reached(RoboterData *data) {
    set_direction(data, STILL);

    USART_print("\nYAY, FINISHED!\n");
    enable_isr_countdown(END_C);
    data->laps_to_go = LAPS;
}

void update_goal_reached(FSM *fsm, RoboterData *data) {

    if (countdown_finished) {
        disable_isr_countdown();
        transition_to_state(fsm, data, LEAVE_START);

    }
    else if (toggle_led) {
        toggle_led = false;

        if (current_led_state == ALL) {
            light_led(NONE);
            current_led_state = NONE;
        }
        else {
            light_led(ALL);
            current_led_state = ALL;
        }
    }
}



