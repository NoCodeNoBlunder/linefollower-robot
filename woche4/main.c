#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"
#include "iesmotors.h"
#include "fsm.h"

#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 40
#define SHORT_wTIME 350

// TODO eventuell seperate Files erstelle für die States

enum Threshold{
    THRESHOLD_L = 512,
    THRESHOLD_M = 512,
    THRESHOLD_R = 512,
};

enum Sensor {
    LEFT_SENSOR = PC0,
    MID_SENSOR = PC1,
    RIGHT_SENSOR = PC2
};

void transmit_data(FSM *fsm, RoboterData *data) {
    static char str_buf[STR_BUF_SIZE];

    sprintf(str_buf,
            "%s L:%d | M:%d | R:%d\n %d | %d\n\n",
            fsm->current_state->state_name, data->sensor_left, data->sensor_mid, data->sensor_right,
            data->left_eng_speed, data->right_eng_speed);
    USART_print(str_buf);
}

// TODO Kann ich auch weniger Argumente übergeben hier?
void enter_init(RoboterData *data) {
    ADC_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);
}

void update_init(FSM *fsm, RoboterData *data) {
    switch_state(fsm, data, FORWARD);
}

void enter_forward(RoboterData *data) {
    // TODO should this be made into a Method and be the API of iesmotors?
    set_direction(data, FORWARD);
}

void update_forward(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    if (data ->sensor_left >= THRESHOLD_L) {
        // LEFT ON TRACK
        if (data ->sensor_right < THRESHOLD_R) {
            // LEFT ON TRACK AND RIGHT OFF TRACK
            switch_state(fsm, data, LEFT);
        }
    }
    else {
        // LEFT OFF TRACK
        if (data ->sensor_right >= THRESHOLD_R) {
            // LEFT OFF TRACK AND RIGHT ON TRACK
            switch_state(fsm, data, RIGHT);
        }
    }

    if (data ->debug_mode) {
        transmit_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}

void enter_left(RoboterData *data) {
    set_direction(data, LEFT);
}

void update_left(FSM *fsm, RoboterData *data)
{
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    if (data ->sensor_left < THRESHOLD_L || data ->sensor_right >= THRESHOLD_R) {
        // LEFT IS OFF TRACK OR RIGHT IS ON TRACK
        switch_state(fsm, data, FORWARD);
    }

    if (data ->debug_mode) {
        transmit_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}

void enter_right(RoboterData *data) {
    set_direction(data, RIGHT);
}

void update_right(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    if (data ->sensor_left >= THRESHOLD_L || data ->sensor_right < THRESHOLD_R) {
        // LEFT IS ON TRACK OR RIGHT IS OFF TRACK
        switch_state(fsm, data, FORWARD);
    }

    if (data ->debug_mode) {
        transmit_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}

void enter_goal_reached(RoboterData *data) {

}

// TODO reduce header ammount and file ammount and change names
int main() {
    FSM fsm;
    RoboterData data = {data.debug_mode = 1}; // if this is set to 0 Debug mode is disabled. No data will be sent to Serial PORT.

    add_state(&fsm, INIT, "Init", enter_init, update_init);
    add_state(&fsm, FORWARD, "Forward", enter_forward, update_forward);
    add_state(&fsm, LEFT, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT, "Right", enter_right, update_right);

    start_fsm_cycle(&fsm, &data);

    USART_print("Programm terminated\n");
}