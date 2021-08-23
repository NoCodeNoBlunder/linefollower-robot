
/**
 * @file
 * @brief this file defines
 */

#ifndef STATES_C_IESCOUNTDOWN_H
#define STATES_C_IESCOUNTDOWN_H

#include "fsm.h"
#include "main.h"

/**
 * @brief
 * Initilizes Timer1 to determine when the countdown is finsihed.
 * @details This is done with the overflow interrupt.
 */
void Timer1_init();

/**
 * @brief enables the overflow interrupt enabling the 15s countdown
 */
void enter_countdown(void);\

/**
 * @brief
 * makes the leds blink with 5hz untill 15s elapse upon which it transitions to LEAVE_START
 */
void update_countdown(FSM *fsm, RoboterData *data);

#endif //STATES_C_IESCOUNTDOWN_H
