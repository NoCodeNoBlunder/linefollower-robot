#ifndef IESMOTORS
#define IESMOTORS
#include <avr/io.h>
#include <avr/interrupt.h>

void setup_timer0(void);
void set_duty_cycle(uint8_t pin, uint8_t value);
void set_polarity_forward();
void set_polarity_backward();
void set_polarity_left_rot();
void set_polarity_right_rot();
void motors_Init();
void drive_straight();
void turn_left();
void turn_right();
#endif