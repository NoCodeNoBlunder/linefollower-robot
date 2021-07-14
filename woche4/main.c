/**
 * @file main.c
 * @author Fabian Indrunas
 * @brief Configures the FSM
 * @version 0.1
 * @date 2021-06-08
 */

#include <util/delay.h>
#include <stdlib.h> // used for EXIT_SUCCESS only!
#include "main.h"
#include "states.h"

#include "iesusart.h"
#include "iesadc.h"
#include "iesmotors.h"
#include "stdio.h"

int main() {
    FSM fsm;
    RoboterData data = {data.debug_mode = 1}; // if this is set to 0 Debug mode is disabled. No data will be sent to Serial PORT.

    add_state(&fsm, INIT, "Init", enter_init, update_init);
    add_state(&fsm, FORWARD, "Forward", enter_forward, update_forward);
    add_state(&fsm, LEFT, "Left", enter_left, update_left);
    add_state(&fsm, RIGHT, "Right", enter_right, update_right);

    start_fsm_cycle(&fsm, &data);

    return EXIT_SUCCESS;
}