
#include "states.h"

void enter_left(RoboterData *data) {
    set_direction(data, LEFT);
}

void update_left(FSM *fsm, RoboterData *data)
{
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    if (data->sensor_left < THRESHOLD_L || data ->sensor_right >= THRESHOLD_R) {
        // LEFT IS OFF TRACK OR RIGHT IS ON TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    if (data->debug_mode) {
        transmit_sensor_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}