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

int main() {
    FSM fsm;
    /**
     * if debug_mode is set to 0 no data will be sent to Serial PORT.
     */
    RoboterData data = {data.debug_mode = 1 };

    add_state(&fsm, INIT, "Init", enter_init, update_init);
    add_state(&fsm, FORWARD, "Forward", enter_forward, update_forward);
    add_state(&fsm, LEFT, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT, "Right", enter_right, update_right);

    start_fsm_cycle(&fsm, &data);

    return EXIT_SUCCESS;
}