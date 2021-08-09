
#ifndef STATES_C_IESCOUNTDOWN_H
#define STATES_C_IESCOUNTDOWN_H

#include "fsm.h"
#include "main.h"

#define F_LEDS 5 // in hz
#define COUNTDOWN_DURATION 15 // in Sekunden
#define TIMER_SIZE 256.0 // in bit as float
#define OF_FREQUENCY (F_CPU / TIMER_SIZE)
#define SECONDS_PER_OF (1 / OF_FREQUENCY) // Einheiten das ist in Sekunden!
#define LED_CYCLE_TIME ((1.0 / F_LEDS) / 2.0) // == 0.1  100ms
#define OVERFLOWS_PER_CYCLE ((unsigned int)(LED_CYCLE_TIME / SECONDS_PER_OF))
#define CYCLE_AMMOUNT ((short)(COUNTDOWN_DURATION * F_LEDS))

void enter_countdown(void);
void update_countdown(FSM *fsm, RoboterData *data);

#endif //STATES_C_IESCOUNTDOWN_H
