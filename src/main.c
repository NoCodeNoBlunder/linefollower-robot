/**
 * @file main.c
 * @author Fabian Indrunas
 * @brief Configuration of FSM.
 * @version 0.1
 * @date 2021-06-08
 */

#include "stdio.h"
#include "../inc/usart.h"

#include <stdlib.h>
#include <stdbool.h>
#include "../inc/main.h"
#include "../inc/states.h"
#include "../inc/countdown_states.h"
#include "../inc/check_lap_state.h"

/* TODO eventuell muss ich diese Werte nicht speichern im struct sonder einfach in main.h speichern
 * und benutzen */

#ifndef CALIBRATION_MODE
#define CALIBRATION_MODE false
#endif

#ifndef DEBUG_MODE
#define DEBUG_MODE false
#endif

#ifndef COUNTDOWN_MODE
#define COUNTDOWN_MODE true
#endif

#ifndef LAPCOUNTER_MODE
#define LAPCOUNTER_MODE true
#endif


void output(RoboterData * data) {
    static char str_buf[40];
    sprintf(str_buf, "%d %d %d %d %d\n", data->calibration_mode, data->debug_mode, data->start_counter_mode, data->lapcounter_mode, data->laps_to_go);
    USART_print(str_buf);
}

int main() {
    FSM fsm;
    /**
     * if debug_mode is set to 0 no data will be sent to Serial PORT.
     */
    RoboterData data = { .calibration_mode = CALIBRATION_MODE,
                         .debug_mode = DEBUG_MODE,
                         .start_counter_mode = COUNTDOWN_MODE,
                         .lapcounter_mode = LAPCOUNTER_MODE,
                         .laps_to_go = LAPS };

//    void *data2 = &data;
//    USART_Init(UBRR_SETTING);
//    output(data2);

//    void *data2 = &data;
//    static char str_buf[20];
//    sprintf(str_buf, "%d %d %d %d %d\n", data2->calibration_mode, data2->debug_mode, data2->start_counter_mode, data2->lapcounter_mode, data2->laps_to_go;
//    USART_print(str_buf);

    add_state(&fsm, INIT, "INIT", enter_init, update_init);
//    add_state(&fsm, FORWARD, "FORWARD", enter_forward, update_forward);
//    add_state(&fsm, LEFT_SOFT, "LEFT_SOFT", enter_soft_left, update_soft_left);
//    add_state(&fsm, RIGHT_SOFT, "RIGHT_SOFT", enter_soft_right, update_soft_right);
//    add_state(&fsm, LEFT_HARD, "LEFT_HARD", enter_left_hard, update_left_hard);
//    add_state(&fsm, RIGHT_HARD, "RIGHT_HARD", enter_right_hard, update_right_hard);
//
//    add_state(&fsm, COUNTDOWN, "COUNTDOWN", enter_countdown, update_countdown);
//    add_state(&fsm, CHECK_STARTPOS, "CHECK_STARTPOS", NULL, update_check_startpos);
//    add_state(&fsm, LEAVE_START, "LEAVE_START", enter_leave_start, update_leave_start);
//    add_state(&fsm, CHECK_LAP, "CHECK_LAP", enter_check_lap, update_check_lap);
//    add_state(&fsm, GOAL_REACHED, "GOAL_REACHED", enter_goal_reached, update_goal_reached);
//
//    add_state(&fsm, CALIBRATION, "CALIBRATION", enter_calibration, update_calibration);

    start_fsm_cycle(&fsm, &data);

    return EXIT_SUCCESS;
}
