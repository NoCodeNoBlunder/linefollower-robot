#ifndef IESMOTORS
#define IESMOTORS
#include <avr/io.h>
#include <avr/interrupt.h>
#include "typedefs.h"

void setup_timer0(void);
void set_duty_cycle(uint8_t pin, uint8_t value);
void set_polarity_forward();
void set_polarity_backward();
void set_polarity_left_rot();
void set_polarity_right_rot();
void motors_Init();
void drive_straight(RoboterData *data);
void turn_left(RoboterData *data);
void turn_right(RoboterData *data);
void accelerate_straight(RoboterData *data, int to_value);
void deaccelerate_straight(RoboterData *data, int to_value);
#endif