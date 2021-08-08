#ifndef IESLEDS_h
#define IESLEDS_h

void leds_Init();
void light_led(LineFollower diode);
void select_and_light_led(FSM *fsm, RoboterData *data);

#endif //IESLEDS_h
