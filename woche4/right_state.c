
#include "states.h"

void enter_right(RoboterData *data) {
    set_direction(data, RIGHT);
}

void update_right(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);

    if (data->sensor_left >= THRESHOLD_L || data->sensor_right < THRESHOLD_R) {
        // LEFT IS ON TRACK OR RIGHT IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    if (data->debug_mode) {
        transmit_sensor_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}