/**
 * @file main.c
 * @author Fabian Indrunas
 * @brief Configuration of FSM.
 * @version 0.1
 * @date 2021-06-08
 */

#include <stdlib.h>
#include "main.h"
#include "states.h"

#include "iesleds.h"
#include <util/delay.h>

int main() {
    FSM fsm;
    /**
     * if debug_mode is set to 0 no data will be sent to Serial PORT.
     */
    RoboterData data = {data.debug_mode = 0 };

    add_state(&fsm, INIT, "Init", enter_init, update_init);
    add_state(&fsm, FORWARD, "Forward", enter_forward, update_forward);
    add_state(&fsm, LEFT, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT, "Right", enter_right, update_right);
    add_state(&fsm, SOFT_RIGHT, "SOFT RIGHT", enter_soft_right, update_soft_right);
    add_state(&fsm, SOFT_LEFT, "SOFT LEFT", enter_soft_left, update_soft_left);

    start_fsm_cycle(&fsm, &data);
    
    /*leds_Init();
    light_led(LEFT_LF);
    _delay_ms(1000);
    light_led(MID_LF);
    _delay_ms(1000);
    light_led(RIGHT_LF);
    _delay_ms(1000);
    light_led(ALL);
    _delay_ms(100000);*/

    return EXIT_SUCCESS;
}
