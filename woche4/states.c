
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"

#include "main.h"
#include <avr/interrupt.h>
#include <stdio.h>

#include "iescountdown.h"

void enter_init(void) {
    ADC_Init();
    leds_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);
    Timer2_init();
}

void update_init(FSM *fsm, RoboterData *data) {
    if(data->start_counter_mode) {
        transition_to_state(fsm, data, CHECK_STARTPOS);
    }
    else {
        // hier muss ich in leave_start transition!
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
    else if (left_on_line(data) && mid_on_line(data) && right_on_line(data)) {
		transition_to_state(fsm, data, CHECK_LAP);
	}

	/*if (left_on_line(data)) {
		transition_to_state(fsm, data, LEFT_SOFT);
	}
	else if (right_on_line(data)) {
		transition_to_state(fsm, data, RIGHT_SOFT);
	}*/
}

void enter_soft_left(RoboterData *data) {
    set_direction(data, LEFT_SOFT);
    light_led(LEFT_AND_MID);
}

void update_soft_left(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!left_on_line(data) && !mid_on_line(data) && !right_on_line(data)) {
        transition_to_state(fsm, data, LEFT_HARD);
    }

    else if (left_on_line(data) && !mid_on_line(data)) {
        transition_to_state(fsm, data, LEFT_HARD);
    }

    else if (!left_on_line(data) && mid_on_line(data) || right_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }

    /*// hier ein or? und Reihenfolge tauschen?
    if (mid_on_line(data) && !right_on_line(data)) {
        // MID IS ON TRACK OR RIGHT_HARD IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    if (!mid_on_line(data) && !right_on_line(data) && !left_on_line(data)) {
        transition_to_state(fsm, data, LEFT_HARD);
    }*/
}

void enter_soft_right(RoboterData *data) {
    set_direction(data, RIGHT_SOFT);
    light_led(RIGHT_AND_MID);
}

void update_soft_right(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!left_on_line(data) && !mid_on_line(data) && !right_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_HARD);
    }

    else if (right_on_line(data) && !mid_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_HARD);
    }

    else if (!right_on_line(data) && mid_on_line(data) || left_on_line(data)) {
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

//    if (mid_on_line(data) /* && !left_on_line(data)*/) {
//        // LEFT_HARD IS OFF TRACK OR MID IS ON TRACK
//        transition_to_state(fsm, data, FORWARD);
//    }
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
    light_led(LEFT_AND_RIGHT);
}

void update_leave_start(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!left_on_line(data) || !mid_on_line(data) || !right_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }
}

void update_check_startpos(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    select_and_light_led(fsm, data);
}

volatile unsigned short cnt2 = 0;
volatile unsigned char check_passed = 0;

#define CHECKDURATION 0.2
#define ERROR_TOLERANCE 2
#define OVERFLOWS_FOR_CHECK ((unsigned int)(CHECKDURATION / SECONDS_PER_OF))

ISR (TIMER2_OVF_vect) {
	
    cnt2 += 1;
	
	if(cnt2 == OVERFLOWS_FOR_CHECK) {
        check_passed = 1;
		cnt2 = 0; // brauche ich das oder im enable
	}
}
void enable_isr_checklap() {
    cli();
    TIMSK2 |= (1 << TOIE2);
    TCCR2A &= ~((1 << WGM20) | (1 << WGM21));
    TCCR2B &= ~(1 << WGM22);
    TCNT2 = 0; // counter value.
    sei();
}

void disable_isr_checklap() {
    // Modus zurücksetzten bringt nicht da einfach auf random Mode gestellt werden
    // würde.
    cli();
    TIMSK2 &= ~(1 << TOIE2); // disable compare match interrupt.
    sei();
}

void enter_check_lap(RoboterData *data) {
	
	USART_print("check lap");
    light_led(CHECK_LAP);
    check_passed = 0;
	cnt2 = 0;
	
    cli(); 				    // disables interrupts globally
    TCCR2B = (1 << CS00);   // Prescaler: 1
    TIMSK2 |= (1 << TOIE2);  // enables Timer2 OVERFLOW interrupt
    TCCR2A &= ~((1 << WGM20) | (1 << WGM21));   // Waveform generation form unter features im Datenblatt muss hier auf Normal mode sein.
    TCNT2 = 0;				// Setzen von Timer counter
    sei(); 				    // enables interrupts globally
	
}

void update_check_lap(FSM *fsm, RoboterData *data) {
	unsigned static char lapcounter = 0;
	unsigned static char error = 0;
	
	take_measurement(data);
	
	if(check_passed) {
		USART_print("Ist die Runde abgeschlossen?");
        disable_isr_checklap();
		lapcounter++;
		transition_to_state(fsm, data, LEAVE_START);
		
		if(lapcounter >= 2) {
            transition_to_state(fsm, data, GOAL_REACHED);
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

void enter_goal_reached(RoboterData *data) {
    USART_print("3 Laps completed!s");
    set_direction(data, STILL);
}

void update_goal_reached(FSM *fsm, RoboterData *data) {

}




