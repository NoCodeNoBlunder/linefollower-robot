/**
 * @headerfile
 */

#ifndef STATES_h
#define STATES_h

#include "main.h"

void enter_init(void);
void update_init(FSM *fsm, RoboterData *data);

/**
 * @sa enter_init(void)
 * @param data
 */
void enter_forward(RoboterData *data);
void update_forward(FSM *fsm, RoboterData *data);

void enter_left_hard(RoboterData *data);
void update_left_hard(FSM *fsm, RoboterData *data);

void enter_right_hard(RoboterData *data);
void update_right_hard(FSM *fsm, RoboterData *data);

void enter_soft_left(RoboterData *data);
void update_soft_left(FSM *fsm, RoboterData *data);

void enter_soft_right(RoboterData *data);
void update_soft_right(FSM *fsm, RoboterData *data);

void enter_leave_start(RoboterData *data);
void update_leave_start(FSM *fsm, RoboterData *data);

void update_check_startpos(FSM *fsm, RoboterData *data);

void enter_goal_reached(RoboterData *data);
void update_goal_reached(FSM *fsm, RoboterData *data);

#endif //STATES_h
