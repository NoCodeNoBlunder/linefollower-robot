
#include <stdint.h>

#ifndef UTIL_h
#define UTIL_H

void set_pin_high(volatile uint8_t *reg, char pin);
void set_pin_low(volatile uint8_t *reg, char pin);
void toggle_pin(volatile uint8_t *reg, char pin);

#endif //UTIL_h
