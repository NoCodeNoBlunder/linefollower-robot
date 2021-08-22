
/**
 * @headerfile check_lap.h
 *
 */

#ifndef CHECK_LAP_H
#define CHECK_LAP_H

#include "main.h"

/**
 * @brief function to initilize Timer2
 */
void Timer2_init();

/**
 * @breif function gets executed when the CHECK_LAP state is entered.
 * @param data
 */
void enter_check_lap(RoboterData *data);

/**
 * @brief state to check whether the Robot is in fact on the starting postion or not.
 * @param fsm
 * @param data
 */
void update_check_lap(FSM *fsm, RoboterData *data);

#endif //CHECK_LAP_H
