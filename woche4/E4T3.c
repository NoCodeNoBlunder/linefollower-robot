#include "iesmotors.h"
#include <util/delay.h>

#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"

#define THRESHOLD 512
#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 50

// TODO not all have to be enum or typedefs!
typedef enum {
    IDLE,
    FORWARD,
    BACKWARD,
    LEFT_TURN,
    RIGHT_TURN
} Mode;

typedef enum {
    LEFT_ENG = PD5,
    RIGHT_ENG = PD6
} Motor ;

typedef enum {
    ENG_STILL = 0,
    ENG_SLOW = 50,
    ENG_MID = 155,
    ENG_FAST = 200
} Speed;


typedef enum {
    LEFT_SENSOR = PC0,
    MID_SENSOR = PC1,
    RIGHT_SENSOR = PC2
} Sensor;

void init_motors() {
    // Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;

    // TODO alle pins müssen auf output gestellt sein.
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);

    // Set PB0, PB1, and PB3 as output (IN[1|2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3) | (1 << DD7);

    // Make PWM work on PD[5|6]
    setup_timer0();
}

void drive_forward() {
    set_duty_cycle(LEFT_ENG, ENG_MID);
    set_duty_cycle(RIGHT_ENG, ENG_MID);
}

void drive_left() {
    set_duty_cycle(LEFT_ENG, ENG_SLOW);
    set_duty_cycle(RIGHT_ENG, ENG_FAST);
}

void drive_right() {
    set_duty_cycle(LEFT_ENG, ENG_FAST);
    set_duty_cycle(RIGHT_ENG, ENG_SLOW);
}

// TODO create struct with Data?
typedef struct {
    Mode mode;
    short sensor_left;
    short sensor_mid;
    short sensor_right;

    char left_eng_rot;
    char right_eng_rot;
    char left_eng_dir;
    char right_eng_dir;

} RoboterData;

// TODO lohnt sich das?
void fire_mode(Mode mode) {
    static char str_buf[STR_BUF_SIZE];

    switch (mode) {
        case IDLE:
            break;
        case FORWARD:
            drive_forward();
            break;
        case BACKWARD:
            break;
        case LEFT_TURN:
            drive_left();
            break;
        case RIGHT_TURN:
            drive_right();
            break;
    }
}

void trigger_mode(RoboterData *data) {
    // NO Sensor detects path
    if (data->sensor_left < THRESHOLD && data->sensor_mid < THRESHOLD && data->sensor_right < THRESHOLD) {
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
            USART_print("There is no such Mode");
            break;
    }

    sprintf(str_buf, "%s Left:%d Mid:%d Right:%d\n ", mode_str, data->sensor_left, data->sensor_mid, data->sensor_right);
    USART_print(str_buf);
}

int main() {
    init_ADC();
    init_motors();

    // both sides forward
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);

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