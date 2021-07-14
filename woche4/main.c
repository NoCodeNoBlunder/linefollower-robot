/**
 * @file main.c
 * @author Fabian Indrunas
 * @brief Function implementation of all ConcreteStates
 * @version 0.1
 * @date 2021-06-08
 *
 * @details ?
 */

#include <util/delay.h>
#include <stdlib.h>
#include "stdio.h"
#include "main.h"
#include "iesusart.h"
#include "iesadc.h"
#include "iesmotors.h"
#include "fsm.h"

/**
 * @def SAMPLE_SIZE determines the
 */
#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 40
#define SHORT_wTIME 350


/**
 *  @enum Threshold
 *  @brief enums values determine the threshold of each sensor
 *  @details due to inaccuracy a sensor might fire early or late
 */
enum Threshold{
    THRESHOLD_L = 512,
    THRESHOLD_M = 512,
    THRESHOLD_R = 512,
};

/**
 * @enum Sensor
 * @brief enum stores the corrosponding pins for the sensors
 */
enum Sensor {
    LEFT_SENSOR = PC2,
    MID_SENSOR = PC1,
    RIGHT_SENSOR = PC0
};

/**
 * Function to transmit data via Serial Port.
 * @param fsm pointer to instance of FSM
 * @param data pointer to instance of RoboterData
 * @details this function is only called when the var debug_mode is enabled in data
 */
void transmit_data(FSM *fsm, RoboterData *data) {
    static char str_buf[STR_BUF_SIZE];

    sprintf(str_buf,
            "%s L:%d | M:%d | R:%d\n %d | %d\n\n",
            fsm->current_state->state_name, data->sensor_left, data->sensor_mid, data->sensor_right,
            data->left_eng_speed, data->right_eng_speed);
    USART_print(str_buf);
}

// region ConcreteStates Enter() and Update() implementations.
// TODO eventuell seperate Files erstelle für die State functionen.


void enter_init() {
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

    if (data->sensor_left >= THRESHOLD_L) {
        // LEFT ON TRACK
        if (data->sensor_right < THRESHOLD_R) {
            // LEFT ON TRACK AND RIGHT OFF TRACK
            switch_state(fsm, data, LEFT);
        }
    }
    else {
        // LEFT OFF TRACK
        if (data->sensor_right >= THRESHOLD_R) {
            // LEFT OFF TRACK AND RIGHT ON TRACK
            switch_state(fsm, data, RIGHT);
        }
    }

    if (data->debug_mode) {
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

    if (data->sensor_left < THRESHOLD_L || data ->sensor_right >= THRESHOLD_R) {
        // LEFT IS OFF TRACK OR RIGHT IS ON TRACK
        switch_state(fsm, data, FORWARD);
    }

    if (data->debug_mode) {
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

    if (data->sensor_left >= THRESHOLD_L || data->sensor_right < THRESHOLD_R) {
        // LEFT IS ON TRACK OR RIGHT IS OFF TRACK
        switch_state(fsm, data, FORWARD);
    }

    if (data->debug_mode) {
        transmit_data(fsm, data);
        _delay_ms(SHORT_wTIME);
    }
}

void enter_goal_reached(RoboterData *data) {

}
// endregion

int main() {
    FSM fsm;
    RoboterData data = {data.debug_mode = 1 }; // if this is set to 0 Debug mode is disabled. No data will be sent to Serial PORT.

    add_state(&fsm, INIT, "Init", enter_init, update_init);
    add_state(&fsm, FORWARD, "Forward", enter_forward, update_forward);
    add_state(&fsm, LEFT, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT, "Right", enter_right, update_right);

    start_fsm_cycle(&fsm, &data);

    return EXIT_SUCCESS;
}