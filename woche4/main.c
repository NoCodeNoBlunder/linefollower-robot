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
    RoboterData data = { .debug_mode = DEBUG_MODE, .start_counter_mode = 0 };

    add_state(&fsm, INIT, "Init", enter_init, update_init);
    add_state(&fsm, FORWARD, "Forward", enter_forward, update_forward);
    add_state(&fsm, LEFT_HARD, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT_HARD, "Right", enter_right, update_right);
    add_state(&fsm, RIGHT_SOFT, "SOFT RIGHT_HARD", enter_soft_right, update_soft_right);
    add_state(&fsm, LEFT_SOFT, "SOFT LEFT_HARD", enter_soft_left, update_soft_left);

    add_state(&fsm, CHECK_STARTPOS, "CHECK_STARTPOS", enter_check_starpos, update_check_startpos);
    add_state(&fsm, COUNTDOWN, "COUNTDOWN", enter_countdown, update_countdown);
    add_state(&fsm, CHECK_LAP, "CHECK_LAP", enter_check_lap, update_check_lap);

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
