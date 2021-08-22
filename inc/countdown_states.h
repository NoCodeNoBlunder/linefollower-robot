
/**
 * @headerfile
 *
 */

#ifndef STATES_C_IESCOUNTDOWN_H
#define STATES_C_IESCOUNTDOWN_H

#include "fsm.h"
#include "main.h"

void enter_countdown(void);
void update_countdown(FSM *fsm, RoboterData *data);
void Timer1_init();

#endif //STATES_C_IESCOUNTDOWN_H
