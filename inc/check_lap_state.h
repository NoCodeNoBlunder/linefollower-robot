
/**
 * @file
 * @brief Defines the enter and update function of the state CHECK_LAP
 * @author Fabian Indrunas
 */

#ifndef CHECK_LAP_H
#define CHECK_LAP_H

#include "main.h"

/**
 * @brief
 * Initilizes Timer2
 */
void Timer2_init();

/**
 * @breif function gets executed when the CHECK_LAP state is entered.
 */
void enter_check_lap(RoboterData *data);

/**
 * @brief state to check whether the Robot is in fact on the starting postion or not.
 */
void update_check_lap(FSM *fsm, RoboterData *data);

#endif //CHECK_LAP_H
