
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"

#include "main.h"
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "check_lap.h"

#include "iescountdown.h"

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


    if (left_on_line(data) && !right_on_line(data)) {
        transition_to_state(fsm, data, LEFT_SOFT);
    }
    else if (right_on_line(data) && !left_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_SOFT);
    }
    else if(data->lapcounter_mode && all_on_line(data)) {
        transition_to_state(fsm, data, CHECK_LAP);
    }
}

void enter_soft_left(RoboterData *data) {
    set_direction(data, LEFT_SOFT);
    light_led(LEFT_AND_MID);
}

void update_soft_left(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if(mid_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }
    // Reicht das um in Hard turn zu gehen? Nur wenn mitter nicht on line ist.
    // alternatiib if (!left_left_on_line(data) && !mid_on_line(data) && !right_on_line)
    else if (!left_on_line(data)) {
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

    if(mid_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }

    else if (!right_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_HARD);
    }
}

void enter_left_hard(RoboterData *data) {
    set_direction(data, LEFT_HARD);
    light_led(LEFT_LF);
}

void update_left_hard(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (left_on_line(data)) {
        transition_to_state(fsm, data, LEFT_SOFT);
    }

    else if (right_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_SOFT);
    }

    else if(mid_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_right_hard(RoboterData *data) {
    set_direction(data, RIGHT_HARD);
    light_led(RIGHT_LF);
}

void update_right_hard(FSM *fsm, RoboterData *data) {
    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (right_on_line(data)) {
        transition_to_state(fsm, data, RIGHT_SOFT);
    }

    else if (left_on_line(data)) {
        transition_to_state(fsm, data, LEFT_SOFT);
    }

    else if(mid_on_line(data)) {
        transition_to_state(fsm, data, FORWARD);
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






