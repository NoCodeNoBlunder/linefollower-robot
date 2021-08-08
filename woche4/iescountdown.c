
#include <avr/interrupt.h>
#include "main.h"
#include "iescountdown.h"
#include "iesleds.h"

#include "iesusart.h" // TODO F_CPU is defined in iesusart!

// TODO warum ist das ein Float? Zahl zu gross und man muss sowieso durch 10 teilen.
#define F_LEDS 5 // in hz
#define COUNTDOWN_DURATION 15 // in Sekunden
#define TIMER_SIZE 256.0 // in bit as float
#define OF_FREQUENCY (F_CPU / TIMER_SIZE)
#define SECONDS_PER_OF (1 / OF_FREQUENCY) // Einheiten das ist in Sekunden!
#define LED_CYCLE_TIME ((1.0 / F_LEDS) / 2.0) // == 0.1  100ms
#define OVERFLOWS_PER_CYCLE ((unsigned int)(LED_CYCLE_TIME / SECONDS_PER_OF))
#define CYCLE_AMMOUNT ((short)(COUNTDOWN_DURATION * F_LEDS))

// Wie viel overflows = 200ms?
unsigned short cnt = 0;
unsigned char turned_on_leds = 0;

ISR (TIMER2_COMPA_vect) {
    cnt += 1;

    if (cnt == OVERFLOWS_PER_CYCLE) {
        light_led(NONE);
    }
    else if(cnt == OVERFLOWS_PER_CYCLE * 2) {
        light_led(ALL);
        cnt = 0;
        turned_on_leds++;
    }
}

void setupTimer2() {
    cli();
    TCCR2B = (1 << CS00);  // Prescaler: 1
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

// TODO write enable Timer function?

void disable_Timer2() {
    cli();
    // TCCR2B &= ~(1 << CS00);  // Prescaler: 1
    TIMSK2 &= ~(1 << OCIE2A);
    TCCR2A &= ~(1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void enter_countdown(void) {
    light_led(ALL);
    setupTimer2();
    cnt = 0;
    turned_on_leds = 0;
}

void update_countdown(FSM *fsm, RoboterData *data) {

    take_measurement(data);

    if (turned_on_leds == CYCLE_AMMOUNT - 1) {
        disable_Timer2();
        transition_to_state(fsm, data, LEAVE_START);
    }
    else if(!left_on_line(data) || !mid_on_line(data) || !right_on_line(data)) {
        disable_Timer2();
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }
}
