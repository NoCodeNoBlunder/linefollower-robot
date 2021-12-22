
/**
 * @file
 * @brief
 * Controlls the
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
 * @brief enables the overflow interrupt starting the 15s countdown
 */
void enter_countdown(void);\

/**
 * @brief
 * makes the leds blink with 5hz untill 15s elapse upon which it transitions to LEAVE_START
 */
void update_countdown(FSM *fsm, RoboterData *data);

/**
 * @brief Makes the robot stop and transmits a success mesage to Serial Port
 */
void enter_goal_reached(RoboterData *data);

/**
 * @brief Runs 60s end Countdown during which the leds blink with 5hz
 */
void update_goal_reached(FSM *fsm, RoboterData *data);


#endif //STATES_C_IESCOUNTDOWN_H
