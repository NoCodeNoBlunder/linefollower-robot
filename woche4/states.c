
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"
#include "main.h"

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
    transition_to_state(fsm, data, FORWARD);
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

void enter_check_starpos(RoboterData *data) {

}

void update_check_startpos(FSM *fsm, RoboterData *data) {

}

void enter_countdown(RoboterData *data) {

}

void update_countdown(FSM *fsm, RoboterData *data) {

}

void enter_check_lap(RoboterData *data) {

}

void update_check_lap(FSM *fsm, RoboterData *data) {

}




