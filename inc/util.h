/**
 * @file
 * @brief defintions to helper functions to set, unset and toggle single pins.
 */

#include <stdint.h>

#ifndef UTIL_h
#define UTIL_H

/**
 * @brief function to set a bit in @param reg.
 * @param reg pointer to register to modify.
 * @param bit to set to high.
 */
void set_pin_high(volatile uint8_t *reg, char bit);

/**
 * @breif function to unset a bit in @param reg.
 * @param reg pointer to register to modify.
 * @param bit to set to low.
 */
void set_pin_low(volatile uint8_t *reg, char bit);

/**
 * @brief function to toggle a bit in @param reg.
 * @param reg pointer to register to modify.
 * @param bit to toggle.
 */
void toggle_pin(volatile uint8_t *reg, char bit);

#endif //UTIL_h
