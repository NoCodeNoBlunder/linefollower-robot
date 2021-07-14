/**
 * @file iesusart.h
 * @version 0.1
 * @date 2021-06-08
 */


#ifndef IESMOTORS_h
#define IESMOTORS
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "fsm.h"

void motors_Init();
void set_direction(RoboterData *data, State state);
#endif // IESMOTORS_h