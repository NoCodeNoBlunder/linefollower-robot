/**
 * @headerfile iesleds.h
 */

#ifndef IESLEDS_h
#define IESLEDS_h

/**
 * @breif function to initilize the leds.
 */
void leds_Init();

/**
 * @bried function to light corrosponding led's
 * @param next_led_mode switch variable to decide which led to light.
 */
void light_led(LineFollower next_led_mode);

/**
 * @breif function to select and light corrosponding led's
 * @param fsm pointer to struct of type FSM.
 * @param data pointer to RoboterData instance which holds the robots sensor information upon which is decided which leds to light
 */
void select_and_light_led(FSM *fsm, RoboterData *data);

#endif //IESLEDS_h
