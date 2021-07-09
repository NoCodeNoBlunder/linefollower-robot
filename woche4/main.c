#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"
#include "typedefs.h"
#include "iesmotors.h"
#include "fsm.h"

#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 40

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

void transmit_data(RoboterData *data) {
    static char str_buf[STR_BUF_SIZE];
    static char *mode_str;

    sprintf(str_buf,
            "%s L:%d | M:%d | R:%d\n %d | %d\n\n",
            mode_str,data->sensor_left, data->sensor_mid, data->sensor_right,
            data->left_eng_speed ,data->right_eng_speed);
    USART_print(str_buf);
}

void enter_init(FSM *fsm, RoboterData *data) {
    ADC_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);

    switch_state(fsm, data, STRAIGHT);
}

// TODO ich sollte keine driveMod states mehr haben sondern alles über die fsm regulieren.
// das macht aber das RoboterData struct etwas unnötig.
void enter_straight(FSM *fsm, RoboterData *data) {
    set_polarity_forward();
    drive_straight(data);
}

void update_straight(FSM *fsm, RoboterData *data) {
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
            // RIGHT ON TRACK
            switch_state(fsm, data, RIGHT);
        }
    }
}

void enter_left(FSM *fsm, RoboterData *data) {
    USART_print("POlarity left rot");
    set_polarity_left_rot();
    turn_left(data);
}

void update_left(FSM *fsm, RoboterData *data)
{
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    if (data ->sensor_left < THRESHOLD_L || data ->sensor_right >= THRESHOLD_R) {
        // LEFT IS OFF TRACK OR RIGH IS ON TRACK
        switch_state(fsm, data, STRAIGHT);
    }
}

void enter_right(FSM *fsm, RoboterData *data) {
    set_polarity_right_rot();
    turn_right(data);
}

void update_right(FSM *fsm, RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    if (data ->sensor_left >= THRESHOLD_L || data ->sensor_right < THRESHOLD_R) {
        // LEFT IS ON TRACK OR RIGHT IS OFF TRACK
        switch_state(fsm, data, STRAIGHT);
    }
}

void enter_goal_reached(FSM *fsm, RoboterData *data) {

}

// TODO reduce header ammount and file ammount and change names
int main() {
    FSM fsm;
    RoboterData data;

    add_state(&fsm, INIT, "Init", enter_init, NULL);
    add_state(&fsm, STRAIGHT, "Straight", enter_straight, update_straight);
    add_state(&fsm, LEFT, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT, "Right", enter_right, update_right);

    start_fsm_cycle(&fsm, &data);

    USART_print("Programm terminated\n");
}