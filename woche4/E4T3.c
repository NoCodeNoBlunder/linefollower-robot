#include "iesmotors.h"
#include <util/delay.h>

#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"

#define THRESHOLD 512

typedef enum {
    LEFT_ENG = PD5,
    RIGHT_ENG = PD6
} Motor ;

typedef enum {
    STILL = 0,
    SLOW = 50,
    MID = 155,
    FAST = 200
} Speed;


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


int main() {
    init_motors();
    init_ADC();

    set_duty_cycle(LEFT_ENG, MID);
    set_duty_cycle(RIGHT_ENG, MID);

    // both sides forward
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);
}