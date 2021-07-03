#include "iesmotors.h"
#include <util/delay.h>

#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"

#define THRESHOLD 512
#define SAMPLE_SIZE 20

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

int main() {
    init_ADC();
    init_motors();

    // both sides forward
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);

    while(1) {

        short sensor_left = ADC_read_avg(LEFT_SENSOR, SAMPLE_SIZE);
        short sensor_mid = ADC_read_avg(MID_SENSOR, SAMPLE_SIZE);
        short sensor_right = ADC_read_avg(RIGHT_SENSOR, SAMPLE_SIZE);

        // NO Sensor detects path
        if (sensor_left < THRESHOLD && sensor_mid < THRESHOLD && sensor_right <THRESHOLD) {
            drive_forward();
        } else if (sensor_right >= THRESHOLD) {
            drive_left();
        } else if (sensor_left >= THRESHOLD) {
            drive_right();
        } else {
            drive_forward();
        }
    }
}