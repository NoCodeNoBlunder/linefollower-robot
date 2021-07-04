#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"
#include "typedefs.h"
#include "iesmotors.h"

#define THRESHOLD 512
#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 50

// TODO lohnt sich das?
void fire_mode(DriveMode mode) {
    switch (mode) {
        case IDLE:
            break;
        case FORWARD:
            set_polarity_forward();
            drive_forward();
            break;
        case BACKWARD:
            set_polarity_backward();
            break;
        case LEFT_TURN:
            set_polarity_forward();
            drive_left();
            break;
        case RIGHT_TURN:
            set_polarity_forward();
            drive_right();
            break;
    }
}

void trigger_mode(RoboterData *data) {
    // NO Sensor detects path
    // TODO was sollte priorisiert werden?
    if (data->sensor_left < THRESHOLD && data->sensor_mid < THRESHOLD && data->sensor_right < THRESHOLD) {
        data->mode = FORWARD;
    }
    else if(data->sensor_mid >= THRESHOLD) {
        data->mode = FORWARD;
    }
    // Left Sensor detects path
    else if (data->sensor_left >= THRESHOLD) {
        data->mode = LEFT_TURN;
    }
    // Right Sensor detects path
    else if (data->sensor_right >= THRESHOLD) {
        data->mode = RIGHT_TURN;
    }
    else {
        data->mode = FORWARD;
    }
}

void send_data(RoboterData *data) {
    static char str_buf[STR_BUF_SIZE];
    static char *mode_str;

    switch (data->mode) {
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

    sprintf(str_buf, "%s Left:%d Mid:%d Right:%d\n ", mode_str,
            data->sensor_left, data->sensor_mid, data->sensor_right);
    USART_print(str_buf);
}

// TODO reduce header ammount and file ammount and change names
int main() {
    ADC_Init();
    motors_Init();
    USART_Init(UBRR_SETTING);

    RoboterData data;

    while(1) {
        data.sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
        data.sensor_mid = ADC_read_avg(MID_SENSOR, SAMPLE_SIZE);
        data.sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

        trigger_mode(&data);
        fire_mode(data.mode);
        send_data(&data);
        _delay_ms(250);
    }
}