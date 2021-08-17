
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"

#include "main.h"
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "iescountdown.h"

void Timer2_init();

void enter_init(void) {
    ADC_Init();
    leds_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);
    Timer1_init();
    Timer2_init();
}

void update_init(FSM *fsm, RoboterData *data) {
    if(data->start_counter_mode) {
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }
    else {
        transition_to_state(fsm, data, LEAVE_START);
    }
}

void enter_forward(RoboterData *data) {
    set_direction(data, FORWARD);
    light_led(MID_LF);
}

void update_forward(FSM *fsm, RoboterData *data) {

    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (right_on_line(data) && !left_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_SOFT);
    }
    else if (left_on_line(data) && !right_on_line(data)) {
        transition_to_state(fsm, data, LEFT_SOFT);
    }
    else if(data->lapcounter_mode) {
        if (all_on_line(data)) {
            transition_to_state(fsm, data, CHECK_LAP);
        }
    }
}

void enter_soft_left(RoboterData *data) {
    set_direction(data, LEFT_SOFT);
    light_led(LEFT_AND_MID);
}

void update_soft_left(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!left_on_line(data)) {
        transition_to_state(fsm, data, LEFT_HARD);
    }

    else if(mid_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_soft_right(RoboterData *data) {
    set_direction(data, RIGHT_SOFT);
    light_led(RIGHT_AND_MID);
}

void update_soft_right(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!right_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_HARD);
    }

    else if(mid_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_left_hard(RoboterData *data) {
    set_direction(data, LEFT_HARD);
    light_led(LEFT_LF);
}

void update_left_hard(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    // hoechste prio ganz oben
    if (!left_on_line(data) && !mid_on_line(data) && !right_on_line(data)) {
        return;
    }

    else if (left_on_line(data) && mid_on_line(data)) {
        transition_to_state(fsm, data, LEFT_SOFT);
    }

        // evt hier !left_on_line(data) && mid_on_line(data)
    else if (!left_on_line(data) || mid_on_line(data) || right_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_SOFT);
    }
}

void enter_right_hard(RoboterData *data) {
    set_direction(data, RIGHT_HARD);
    light_led(RIGHT_LF);
}

void update_right_hard(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    // hoechste prio ganz oben
    if (!left_on_line(data) && !mid_on_line(data) && !right_on_line(data)) {
        return;
    }

    else if (right_on_line(data) && mid_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_SOFT);
    }

    else if (left_on_line(data) || mid_on_line(data) || !right_on_line(data)) {
        transition_to_state(fsm, data, LEFT_SOFT);
    }
}

// ---------------------------------------------------

void enter_leave_start(RoboterData *data) {
    set_direction(data, FORWARD);
    // TODO welche LEDS mache ich hier an.
    light_led(ALL);
    // USART_print("\nLeave Start is entered");
}

void update_leave_start(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!all_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }
}

void update_check_startpos(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);
    select_and_light_led(fsm, data);
}

volatile unsigned short cnt2 = 0;
volatile unsigned char check_passed = false;

#define CHECKDURATION 0.17 // 0.15 and 3 worked
#define ERROR_TOLERANCE 0 // worked with 1

#define TIMER_SIZE 256 // in bit as float
#define OF_FREQUENCY (F_CPU / TIMER_SIZE)
#define OVERFLOWS_FOR_CHECK ((unsigned int)(OF_FREQUENCY * CHECKDURATION))

ISR (TIMER2_OVF_vect) {
    cnt2 += 1;

    if(cnt2 == OVERFLOWS_FOR_CHECK) {
        check_passed = 1;
        cnt2 = 0; // brauche ich das oder im enable
    }
}

void Timer2_init() {
    cli();
    TCCR2B = (1 << CS20); // Prescaler 1:
    // set normal mode
    TCCR2A &= ~((1 << WGM20) | (1 << WGM21));
    TCCR2B &= ~(1 << WGM22);
    sei();
}

void enable_isr_checklap() {
    cnt2 = 0;
    check_passed = false;

    cli();                  // disables interrupts globally
    TIMSK2 |= (1 << TOIE2); // enables Timer2 OVERFLOW interrupt
    TCNT2 = 0;              // set counter value.
    sei();                  // enables interrupts globally
}

void disable_isr_checklap() {
    // Modus zurücksetzten bringt nicht da einfach auf random Mode gestellt werden
    // würde.
    cli();
    TIMSK2 &= ~(1 << TOIE2); // disable compare match interrupt.
    sei();
}

void enter_check_lap(RoboterData *data) {

    // USART_print("\nEntered CHeck lap!\n");
    set_direction(data, CHECK_LAP);
    enable_isr_checklap();
}

void update_check_lap(FSM *fsm, RoboterData *data) {
    static unsigned char error = 0;

    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if(check_passed) {
        // USART_print("Ist die Runde abgeschlossen?");
        disable_isr_checklap();
        data->lapcounter++;
        // USART_print("Laps++");

        if(data->lapcounter >= 3) {
            transition_to_state(fsm, data, GOAL_REACHED);
            return;
        }
        else {
            transition_to_state(fsm, data, LEAVE_START);
            return;
        }
    }

    else if (!all_on_line(data)) {
        error++;

        if (error > ERROR_TOLERANCE) {
            disable_isr_checklap();
            transition_to_state(fsm, data, FORWARD);
        }
    }
}





