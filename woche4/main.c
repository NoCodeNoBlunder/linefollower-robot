#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"
#include "typedefs.h"
#include "iesmotors.h"
#include "fsm.h"

#define THRESHOLD 512
#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 40

enum Threshold{
    THRESHOLD_L = 512,
    THRESHOLD_M = 512,
    THRESHOLD_R = 512,
};

// TODO lohnt sich das?
void fire_mode(RoboterData *data) {
    switch (data -> mode) {
        case IDLE:
            break;
        case FORWARD:
            // TODO Polarity sollte gesetzt werden indirekt, wenn die
            // driveMod sich ändert!
            // Set polarity sollte indirekt über die drive Mod gesteurt werden!
            set_polarity_forward();
            drive_straight(data);
            break;
        case BACKWARD:
            set_polarity_backward();
            drive_straight(data);
            break;
        case LEFT_TURN:
            set_polarity_forward();
            turn_left(data);
            break;
        case RIGHT_TURN:
            set_polarity_forward();
            turn_right(data);
            break;
    }
}

void set_mode(RoboterData *data) {
    // NO Sensor detects path
    // TODO was sollte priorisiert werden?
    if (data->sensor_left < THRESHOLD_L && data->sensor_mid < THRESHOLD_M && data->sensor_right < THRESHOLD_R) {
        data->mode = FORWARD;
    }
    // Left Sensor detects path
    else if (data->sensor_left >= THRESHOLD_L) {
        data->mode = LEFT_TURN;
    }
    // Right Sensor detects path
    else if (data->sensor_right >= THRESHOLD_R) {
        data->mode = RIGHT_TURN;
    }
    else {
        data->mode = FORWARD;
    }

    event_handler(data);
}

void send_data(RoboterData *data) {
    static char str_buf[STR_BUF_SIZE];
    static char *mode_str;

    switch (data -> mode) {
        case IDLE:
            mode_str = "IDLE: ";
            break;
        case FORWARD:
            mode_str = "FORWARD: ";
            break;
        case BACKWARD:
            mode_str = "BACKWARD: ";
            break;
        case LEFT_TURN:
            mode_str = "LEFT_TURN: ";
            break;
        case RIGHT_TURN:
            mode_str = "RIGHT_TURN: ";
            break;
        default:
            USART_print("There is no such DriveMode");
            break;
    }

    sprintf(str_buf,
            "%s L:%d | M:%d | R:%d\n %d | %d\n\n",
            mode_str,data->sensor_left, data->sensor_mid, data->sensor_right,
            data->left_eng_speed ,data->right_eng_speed);
    USART_print(str_buf);
}

void enter_start(FSM *fsm, RoboterData *data) {
    ADC_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);

    switchState(fsm, data, STEADY);
}

// TODO argument change!
void enter_steady() {

}

void update_steady(FSM *fsm, RoboterData *data) {
    // accelerate_straight(data, ENG_FAST);
    data->sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
    data->sensor_mid = ADC_read_avg(MID_SENSOR, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

    set_mode(data);
    fire_mode(data);
    send_data(data);
    // _delay_ms( 250);

    if (data ->sensor_right > THRESHOLD_R) {
        switchState(fsm, data, SHUTDOWN);
    }
}

// TODO reduce header ammount and file ammount and change names
int main() {
    FSM fsm;
    RoboterData data;

    addState(&fsm, INIT, "Start", enter_start, NULL);
    addState(&fsm, STEADY, "Steady", enter_steady, update_steady);

    start_fsm_cycle(&fsm, &data);

    USART_print("Terminated?\n");
}