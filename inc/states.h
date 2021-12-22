/**
 * @file
 * @brief Contains most of the robots State's functionality
 * @author Fabian Indrunas
 */

#ifndef STATES_h
#define STATES_h

#include "main.h"

/**
 * @brief Initilizes all modules.
 */
void enter_init(void);

/**
 * @brief Transitions to CHECK_START if COUNTER_MODE is enabled
 * else transitions to LEAVE_START
 * @param fsm pointer to fsm instance which stores all states and the current state.
 * @param data stores all the information regarding the robot.
 */
void update_init(FSM *fsm, RoboterData *data);

/**
 * @brief
 * Makes the robot drive forward and lights the middle led.
 */
void enter_forward(RoboterData *data);

/**
 * @brief
 * Takes a measurement and determines wether the robot should transition to another state.
 */
void update_forward(FSM *fsm, RoboterData *data);

/**
 * @brief
 *  Makes the robot perform a sharp left turn and lights the left led.
 */
void enter_left_hard(RoboterData *data);

/**
 * @brief continues the sharp turn until the left line detects the line again.
 * @details This state can only be entered from a SOFT_LEFT to achieve smoother turns.
 */
void update_left_hard(FSM *fsm, RoboterData *data);

/**
 * @brief Makes the robot perform a sharp right turn and lights the right led.
 */
void enter_right_hard(RoboterData *data);

/**
 * @brief continues the shart turn untill the right line detects the line again.
 * @details This state can only be entered from a SOFT_RIGHT to achieve smoother turns.
 */
void update_right_hard(FSM *fsm, RoboterData *data);

/**
 * @brief Makes the robot perform a soft left turn and lights the left and middle led.
 */
void enter_soft_left(RoboterData *data);

/**\
 * @brief Check wether the robot should continue the soft turn, transition into FORWARD or
 * LEFT_HARD
 */
void update_soft_left(FSM *fsm, RoboterData *data);

/**
 * @brief Makes the robot perform a soft right turn and lights the right and middle led.
 */
void enter_soft_right(RoboterData *data);

/**
 * @brief Checks wether the robot should continue the soft turn, transition into FORWARD or
 * RIGHT_HARD
 */
void update_soft_right(FSM *fsm, RoboterData *data);

/**
 * @brief
 * Makes to Robot drive Forward
 */
void enter_leave_start(RoboterData *data);

/**
 * @brief
 * The robot continues to drive forward untill it leaves the startfield.
 */
void update_leave_start(FSM *fsm, RoboterData *data);

/**
 * @brief
 * checks wether the robot is on the startfield.
 */
void update_check_startpos(FSM *fsm, RoboterData *data);


#endif //STATES_h
