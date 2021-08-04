
#ifndef STATES_h
#define STATES_h

#include "main.h"

void enter_init(void);
void update_init(FSM *fsm, RoboterData *data);

void enter_forward(RoboterData *data);
void update_forward(FSM *fsm, RoboterData *data);

void enter_left(RoboterData *data);
void update_left(FSM *fsm, RoboterData *data);

void enter_right(RoboterData *data);
void update_right(FSM *fsm, RoboterData *data);

void enter_soft_left(RoboterData *data);
void update_soft_left(FSM *fsm, RoboterData *data);

void enter_soft_right(RoboterData *data);
void update_soft_right(FSM *fsm, RoboterData *data);

#endif //STATES_h
