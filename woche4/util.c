//
// Created by Fabian on 14.07.2021.
//

#include "util.h"


// TODO create util file with helper methods?
void set_pin_high(volatile char *reg, char pin) {
    reg[0] |= (1 << pin);
}

void set_pin_low(volatile char *reg, char pin) {
    reg[0] &= ~(1 << pin);
}

void toggle_pin(volatile char *reg, char pin) {
    reg[0] ^= (1 << pin);
}

