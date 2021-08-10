
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

#include "main.h"
#include "iescountdown.h"
#include "iesleds.h"
#include "iesusart.h"

#include "iesmotors.h"

// TODO warum ist das ein Float? Zahl zu gross und man muss sowieso durch 10 teilen.
// TODO muss das hier volatile sein?
volatile unsigned int cnt = 0;
volatile unsigned int cnt_countdown = 0;
volatile unsigned char toggle_led = false;
volatile unsigned char countdown_finished = false;

volatile LineFollower current_led_state = ALL;

#define F_LEDS 5 // in hz
#define STAR_C_DUR 15 // in Sekunden
#define END_C_DUR 60

#define TIMER_SIZE 65536.0 // in bit as float
#define OF_FREQUENCY (F_CPU / TIMER_SIZE)
#define LED_CYCLE_TIME ((1.0 / F_LEDS) / 2.0) // == 0.1  100ms

#define OVERFLOWS_PER_CYCLE ((unsigned int)(OF_FREQUENCY * LED_CYCLE_TIME))
#define TOTAL_OVERFLOWS ((unsigned int)(OF_FREQUENCY * STAR_C_DUR))

ISR (TIMER1_OVF_vect) {
    cnt += 1;
    cnt_countdown += 1;

    if(cnt == OVERFLOWS_PER_CYCLE) {
        cnt = 0;
        toggle_led = true;
    }

    if(cnt_countdown == TOTAL_OVERFLOWS) {
        countdown_finished = true;
    }
}

// TODO this has to be done only once when initilizing the Timer!
// TODO can u disable the timer?
void Timer1_init() {
    cli();
    TCCR1B = (1 << CS10);  // Prescaler: 1
    // set normal mode
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B &= ~((1 << WGM12) | (1 << WGM13));
    sei();
}

void enable_isr_countdown() {
    cnt = 0;
    cnt_countdown = 0;

    toggle_led = false;
    countdown_finished = false;

    cli();
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0; // set counter value to 0
    sei();
}

void disable_isr_countdown() {
    // Modus zurücksetzten bringt nicht da einfach auf random Mode gestellt werden
    // würde.
    cli();
    TIMSK1 &= ~(1 << TOIE1); // disable compare match interrupt.
    sei();
}

void enter_countdown(void) {
    light_led(ALL);
    enable_isr_countdown();
}

void update_countdown(FSM *fsm, RoboterData *data) {

    take_measurement(data);
    //transmit_debug_msg(fsm, data); // TODO üeso geht das hier nicht?

    // WIESO muss hier ein trü hin?
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

    USART_print("\n3 Laps completed!s\n");
    enable_isr_countdown();
    data->lapcounter = 0;
}

void update_goal_reached(FSM *fsm, RoboterData *data) {

//    transmit_debug_msg(fsm, data);

    if (countdown_finished) {
        // USART_print("Counter finished.");
        disable_isr_countdown();
        transition_to_state(fsm, data, LEAVE_START);
    }
    else if (toggle_led) {
        toggle_led = false;

        if (current_led_state == ALL) {
            light_led(NONE);
            current_led_state = NONE;
        } else {
            light_led(ALL);
            current_led_state = ALL;
        }
    }
}



