/**
 * @file motor_controller.h
 * @breif controlls the robots motors and therefor determines his driving direction and speed.
 * @date 2021-06-08
 * @author Fabian Indrunas
 */

#ifndef IESMOTORS_h
#define IESMOTORS_h

#include "main.h"
#include "fsm.h"

/**
 * @brief function to initilize Timer0 for the pwm to work in order to controll the engines duty cycles.
 */
void motors_Init();

/**
 *
 * @param data pointer to Roboterdata struct
 * @param
 */
void set_direction(RoboterData *data, State state);

#endif // IESMOTORS_h