
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"
#include "main.h"

#include <avr/interrupt.h>
#include <stdio.h>

// TODO auslagern?
void take_measurement(RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);
    data->sensor_mid = ADC_read_avg(MID_LF, SAMPLE_SIZE);
}

char left_on_line(RoboterData *data) {
    return data->sensor_left >= THRESHOLD_L;
}

char mid_on_line(RoboterData *data) {
    return data->sensor_mid >= THRESHOLD_M;
}

char right_on_line(RoboterData *data) {
    return data->sensor_right >= THRESHOLD_R;
}

void enter_init(void) {
    ADC_Init();
    leds_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);
}

void update_init(FSM *fsm, RoboterData *data) {
    if(data->start_counter_mode) {
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }
    else {
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_forward(RoboterData *data) {
	// USART_print("\nenter_forward was called\n");
    set_direction(data, FORWARD);
    light_led(MID_LF);
}

void update_forward(FSM *fsm, RoboterData *data) {

    take_measurement(data);

    // TODO müssen die entsprechenden leds angemacht werden.
    transmit_debug_msg(fsm, data);
	
	if(left_on_line(data)) {
		transition_to_state(fsm, data, LEFT_SOFT);
	}
	else if(right_on_line(data)) {
		transition_to_state(fsm, data, RIGHT_SOFT);
	}
	
	/*if(mid_on_line(data)) {
		if(left_on_line(data)) {
			transition_to_state(fsm, data, LEFT_HARD);
		}
		else if(right_on_line(data)) {
			transition_to_state(fsm, data, LEFT_HARD);
		}
	}
	else {
		if(left_on_line(data)) {
			transition_to_state(fsm, data, LEFT_SOFT);
		}
		else if(right_on_line(data)) {
			transition_to_state(fsm, data, RIGHT_SOFT);
		}
	}*/
}

void enter_left(RoboterData *data) {
    set_direction(data, LEFT_HARD);
    light_led(LEFT_LF);
}

void update_left(FSM *fsm, RoboterData *data) {

    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (mid_on_line(data) /* && !left_on_line(data)*/) {
        // LEFT_HARD IS OFF TRACK OR MID IS ON TRACK
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_right(RoboterData *data) {
    set_direction(data, RIGHT_HARD);
    light_led(RIGHT_LF);
}

void update_right(FSM *fsm, RoboterData *data) {

    take_measurement(data);
    transmit_debug_msg(fsm, data);
	
    if (mid_on_line(data) /*&& !right_on_line(data)*/) {
        // MID IS ON TRACK OR RIGHT_HARD IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_soft_left(RoboterData *data) {
	set_direction(data, LEFT_SOFT);
	light_led(LEFT_AND_MID);
}

void update_soft_left(FSM *fsm, RoboterData *data) {
	take_measurement(data);
	transmit_debug_msg(fsm, data);

	// hier ein or? und Reihenfolge tauschen?
	if (mid_on_line(data) && !right_on_line(data)) {
        // MID IS ON TRACK OR RIGHT_HARD IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }
    
    if (!mid_on_line(data) && !right_on_line(data) && !left_on_line(data)) {
		transition_to_state(fsm, data, LEFT_HARD);
	}
}

void enter_soft_right(RoboterData *data) {
	set_direction(data, RIGHT_SOFT);
	light_led(RIGHT_AND_MID);
}

void update_soft_right(FSM *fsm, RoboterData *data) {
	take_measurement(data);
	transmit_debug_msg(fsm, data);
	
	if (mid_on_line(data) && !right_on_line(data)) {
        // MID IS ON TRACK OR RIGHT_HARD IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }
    
	if (!mid_on_line(data) && !right_on_line(data) && !left_on_line(data)) {
		transition_to_state(fsm, data, RIGHT_HARD);
	}
}

// *******************************************************************
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
    cnt+=1;

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

void disable_Timer2() {
    cli();
    TCCR2B &= ~(1 << CS00);  // Prescaler: 1
    TIMSK2 &= ~(1 << OCIE2A);
    TCCR2A &= ~(1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void enter_check_starpos(RoboterData *data) {

}

void update_check_startpos(FSM *fsm, RoboterData *data) {
    take_measurement(data);

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

void enter_countdown(RoboterData *data) {
    light_led(ALL);
    setupTimer2();
    cnt = 0;
    turned_on_leds = 0;
}

void update_countdown(FSM *fsm, RoboterData *data) {

    take_measurement(data);

    if (turned_on_leds == CYCLE_AMMOUNT - 1) {
        disable_Timer2();
        transition_to_state(fsm, data, FORWARD);
    }
    else if(!left_on_line(data) || !mid_on_line(data) || !right_on_line(data)) {
        disable_Timer2();
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }
}

void enter_check_lap(RoboterData *data) {

}

void update_check_lap(FSM *fsm, RoboterData *data) {

}




