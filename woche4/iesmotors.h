#ifndef IESMOTORS
#define IESMOTORS
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "fsm.h"

void motors_Init();
void set_direction(RoboterData *data, State state);
#endif