
#include "states.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "iesusart.h"
#include "iesleds.h"

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
        light_led(NONE);
    }
}

// TODO wie vermeide ich Codewiederholung unterhalbt der states? Inheritance?
void update_forward(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);

    if (data->sensor_left >= THRESHOLD_L) {
        // LEFT ON TRACK
        if (data->sensor_right < THRESHOLD_R) {
            // LEFT ON TRACK AND RIGHT OFF TRACK
            transition_to_state(fsm, data, LEFT);
//            light_led(LEFT_LF);
        }

    }
    else {
        // LEFT OFF TRACK
        if (data->sensor_right >= THRESHOLD_R) {
            // LEFT OFF TRACK AND RIGHT ON TRACK
            transition_to_state(fsm, data, RIGHT);
        }
    }

    transmit_debug_msg(fsm, data);
}

void enter_left(RoboterData *data) {
    set_direction(data, LEFT);
    light_led(LEFT_LF);
}

void update_left(FSM *fsm, RoboterData *data)
{
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);

    if (data->sensor_left < THRESHOLD_L || data ->sensor_right >= THRESHOLD_R) {
        // LEFT IS OFF TRACK OR RIGHT IS ON TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    transmit_debug_msg(fsm, data);
}

void enter_right(RoboterData *data) {
    set_direction(data, RIGHT);
    light_led(RIGHT_LF);
}

void update_right(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);

    if (data->sensor_left >= THRESHOLD_L || data->sensor_right < THRESHOLD_R) {
        // LEFT IS ON TRACK OR RIGHT IS OFF TRACK
        transition_to_state(fsm, data, FORWARD);
    }

    transmit_debug_msg(fsm, data);
}