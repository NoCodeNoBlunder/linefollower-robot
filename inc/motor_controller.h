/**
 * @file
 * @brief
 * Controls the robots motors and therefore determines it's driving direction and speed.
 * @author Fabian Indrunas
 */

#ifndef IESMOTORS_h
#define IESMOTORS_h

#include "main.h"
#include "fsm.h"

/**
 * @brief
 * Initilizes Timer0 for the pwm to work in order to controll the engines duty cycles.
 */
void motors_Init();

/**
 * @brief Sets the robots's engines polarity and duty cycles therefore controlling
 * the robots driving direction and its speed.
 * @param data pointer to Roboterdata struct
 * @param state determines how the engines will be set.
 */
void set_direction(RoboterData *data, State state);

#endif // IESMOTORS_h