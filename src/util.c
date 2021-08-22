
#include "../inc/util.h"

void set_pin_high(volatile uint8_t *reg, char pin) {
    reg[0] |= (1 << pin);
}


void set_pin_low(volatile uint8_t *reg, char pin) {
    reg[0] &= ~(1 << pin);
}


void toggle_pin(volatile uint8_t *reg, char pin) {
    reg[0] ^= (1 << pin);
}

