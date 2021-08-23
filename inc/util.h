/**
 * @file
 * @brief defintions to helper functions.
 */

#include <stdint.h>

#ifndef UTIL_h
#define UTIL_H

/**
 * @brief function to set a pin.
 * @param reg pointer to register to modify.
 * @param pin to set to high.
 */
void set_pin_high(volatile uint8_t *reg, char pin);

/**
 * @breif function to unset a pin.
 * @param reg reg pointer to register to modify.
 * @param pin to set to low.
 */
void set_pin_low(volatile uint8_t *reg, char pin);

/**
 * @brief function to toggle a pin.
 * @param reg reg pointer to register to modify.
 * @param pin to toggle.
 */
void toggle_pin(volatile uint8_t *reg, char pin);

#endif //UTIL_h
