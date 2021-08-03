
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"
#include "main.h"

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

    if(data->sensor_right >= THRESHOLD_R && data->sensor_left >= THRESHOLD_L) {
        light_led(LEFT_AND_RIGHT);
    }
    else {
        light_led(MID_LF);
    }
}

void update_forward(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);
    data->sensor_mid = ADC_read_avg(MID_LF, SAMPLE_SIZE);

    if (data->sensor_mid < THRESHOLD_M) {
        // MID OFF TRACK
        if (data->sensor_left >= THRESHOLD_L && data->sensor_right < THRESHOLD_R) {
            // LEFT ON TRACK AND RIGHT OFF TRACK
            transition_to_state(fsm, data, LEFT);
        }
        else if(data->sensor_right >= THRESHOLD_R){
            // LEFT OFF TRACK AND RIGHT ON TRACK
            transition_to_state(fsm, data, RIGHT);
        }
    }

    // TODO müssen die entsprechenden leds angemacht werden.
    transmit_debug_msg(fsm, data);
}

void enter_left(RoboterData *data) {
    set_direction(data, LEFT);
    light_led(LEFT_LF);
}

void update_left(FSM *fsm, RoboterData *data)
{
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
//    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);
    data->sensor_mid = ADC_read_avg(MID_LF, SAMPLE_SIZE);

    if (data->sensor_left < THRESHOLD_L || data ->sensor_mid >= THRESHOLD_R) {
        // LEFT IS OFF TRACK OR MID IS ON TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    transmit_debug_msg(fsm, data);
}

void enter_right(RoboterData *data) {
    set_direction(data, RIGHT);
    light_led(RIGHT_LF);
}

void update_right(FSM *fsm, RoboterData *data) {
//    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);
    data->sensor_mid = ADC_read_avg(MID_LF, SAMPLE_SIZE);

    if (data->sensor_mid >= THRESHOLD_L || data->sensor_right < THRESHOLD_R) {
        // MID IS ON TRACK OR RIGHT IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    transmit_debug_msg(fsm, data);
}