
/**
 * @file
 * @brief this file defines
 */

#ifndef STATES_C_IESCOUNTDOWN_H
#define STATES_C_IESCOUNTDOWN_H

#include "fsm.h"
#include "main.h"

void Timer1_init();
void enter_countdown(void);
void update_countdown(FSM *fsm, RoboterData *data);

#endif //STATES_C_IESCOUNTDOWN_H
