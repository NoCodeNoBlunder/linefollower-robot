
#ifndef UTIL_h
#define UTIL_H

void set_pin_high(volatile char *reg, char pin);
void set_pin_low(volatile char *reg, char pin);
void toggle_pin(volatile char *reg, char pin);

#endif //UTIL_h
