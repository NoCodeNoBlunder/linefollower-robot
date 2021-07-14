
#include "states.h"

void enter_forward(RoboterData *data) {
    // TODO should this be made into a Method and be the API of iesmotors?
    set_direction(data, FORWARD);
}

void update_forward(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);

    if (data->sensor_left >= THRESHOLD_L) {
        // LEFT ON TRACK
        if (data->sensor_right < THRESHOLD_R) {
            // LEFT ON TRACK AND RIGHT OFF TRACK
            transition_to_state(fsm, data, LEFT);
        }
    }
    else {
        // LEFT OFF TRACK
        if (data->sensor_right >= THRESHOLD_R) {
            // LEFT OFF TRACK AND RIGHT ON TRACK
            transition_to_state(fsm, data, RIGHT);
        }
    }

    if (data->debug_mode) {
        transmit_sensor_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}
