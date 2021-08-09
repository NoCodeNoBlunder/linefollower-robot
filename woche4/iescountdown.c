
#include <avr/interrupt.h>
#include "main.h"
#include "iescountdown.h"
#include "iesleds.h"

#include "iesusart.h" // TODO F_CPU is defined in iesusart!

// TODO warum ist das ein Float? Zahl zu gross und man muss sowieso durch 10 teilen.
// TODO muss das hier volatile sein?
volatile unsigned short cnt = 0;
volatile unsigned char toggled_led_count = 0; // TODO kann ich schlaür die ganze zeit
volatile unsigned char toggle_led = 0; // TODO boolean importieren?

ISR (TIMER2_COMPA_vect) {
    cnt += 1;

    if(cnt == OVERFLOWS_PER_CYCLE) {
        toggle_led = 1;
    }

//    if (cnt == OVERFLOWS_PER_CYCLE) {
//        turn_off_leds = 1;
//    }
//    else if (cnt == OVERFLOWS_PER_CYCLE * 2) {
//        turn_on_leds = 1;
//    }
}

// TODO this has to be done only once when initilizing the Timer!
// TODO can u disable the timer?
void Timer2_init() {
    cli();
    TCCR2B = (1 << CS00);  // Prescaler: 1
    OCR2A = 255;           // Compare match interrupt
    sei();
}

void enable_isr_countdown() {
    cnt = 0;
    toggled_led_count = 0; // TODO geht es irgendwie ohne diese Variable

    cli();
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM21);
    TCNT2 = 0; // counter value.
    sei();
}

void disable_isr_countdown() {
    // Modus zurücksetzten bringt nicht da einfach auf random Mode gestellt werden
    // würde.
    cli();
    TIMSK2 &= ~(1 << OCIE2A); // disable compare match interrupt.
    sei();
}

void enter_countdown(void) {
    light_led(ALL);
    // TODO rein moven in enable timer?
    enable_isr_countdown();
}

void update_countdown(FSM *fsm, RoboterData *data) {
    static LineFollower current_led_state = ALL;

    if (toggle_led) {
        toggle_led = 0;
        cnt = 0;

        if (current_led_state == ALL) {
            light_led(NONE);
            current_led_state = NONE;
        }
        else {
            light_led(ALL);
            current_led_state = ALL;
            toggled_led_count++; // TODO use Prescaler instead??
        }
    }

    take_measurement(data);

    if (toggled_led_count == CYCLE_AMMOUNT - 1) {
        disable_isr_countdown();
        transition_to_state(fsm, data, LEAVE_START);
    }
    else if(!left_on_line(data) || !mid_on_line(data) || !right_on_line(data)) {
        disable_isr_countdown();
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }
}
