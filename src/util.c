
/**
 * @file
 * @brief
 * Implementation of helper funcitons.
 */

#include "../inc/util.h"

void set_pin_high(volatile uint8_t *reg, char bit) {
    reg[0] |= (1 << bit);
}


void set_pin_low(volatile uint8_t *reg, char bit) {
    reg[0] &= ~(1 << bit);
}


void toggle_pin(volatile uint8_t *reg, char bit) {
    reg[0] ^= (1 << bit);
}

