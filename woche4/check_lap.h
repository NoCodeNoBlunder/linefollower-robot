
#ifndef CHECK_LAP_H
#define CHECK_LAP_H

#include "main.h"

void Timer2_init();
void enter_check_lap(RoboterData *data);
void update_check_lap(FSM *fsm, RoboterData *data);

#endif //CHECK_LAP_H
