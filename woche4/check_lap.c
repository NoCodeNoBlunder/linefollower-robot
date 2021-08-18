
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "check_lap.h"
#include "linefollower.h"
//#include "states.h"
//#include "iesadc.h"
#include "iesmotors.h"
#include "iesusart.h"
#include "iesleds.h"
#include "main.h"

#define CHECKDURATION 0.17 // 0.15 and 3 worked
#define ERROR_TOLERANCE 0 // worked with 1

#define TIMER_SIZE 256 // in bit as float
#define OF_FREQUENCY (F_CPU / TIMER_SIZE)
#define OVERFLOWS_FOR_CHECK ((unsigned int)(OF_FREQUENCY * CHECKDURATION))

volatile unsigned short cnt2 = 0;
volatile unsigned char check_passed = false;

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

    if (check_passed) {
        // USART_print("Ist die Runde abgeschlossen?");
        disable_isr_checklap();
        data->laps_to_go--;
        // USART_print("Laps++");

        if(data->laps_to_go <= 0) {
            transition_to_state(fsm, data, GOAL_REACHED);
        }
        else {
            transition_to_state(fsm, data, LEAVE_START);
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
