
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"
#include "main.h"

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
    set_direction(data, FORWARD);
    light_led(MID_LF);
}

void update_forward(FSM *fsm, RoboterData *data) {

    take_measurement(data);

    // TODO müssen die entsprechenden leds angemacht werden.
    transmit_debug_msg(fsm, data);

    if(!mid_on_line(data)) {
        if(left_on_line(data)) {
            if(!right_on_line(data)) {
                // LEFT ON TRACK AND RIGHT OFF TRACK
                transition_to_state(fsm, data, LEFT);
            }
        }
        else if(right_on_line(data)) {
            // LEFT OFF TRACK AND RIGHT ON TRACK
            transition_to_state(fsm, data, RIGHT);
        }
        else {
            // TODO alles weiss?
        }
    }
    else if(left_on_line(data) && right_on_line(data)) {
        // TODO CHeck for staring field
    }
}

void enter_left(RoboterData *data) {
    set_direction(data, LEFT);
    light_led(LEFT_LF);
}

void update_left(FSM *fsm, RoboterData *data) {

    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (!left_on_line(data) && mid_on_line(data)) {
        // LEFT IS OFF TRACK OR MID IS ON TRACK
        transition_to_state(fsm, data, FORWARD);
    }
}

void enter_right(RoboterData *data) {
    set_direction(data, RIGHT);
    light_led(RIGHT_LF);
}

void update_right(FSM *fsm, RoboterData *data) {

    take_measurement(data);
    transmit_debug_msg(fsm, data);

    if (left_on_line(data) && !right_on_line(data)) {
        // MID IS ON TRACK OR RIGHT IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }
}