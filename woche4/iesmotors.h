#ifndef IESMOTORS
#define IESMOTORS
#include <avr/io.h>
#include <avr/interrupt.h>

void setup_timer0(void);
void set_duty_cycle(uint8_t pin, uint8_t value);
#endif