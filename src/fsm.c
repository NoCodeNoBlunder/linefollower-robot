/**
 * @file fsm.c
 * @author Fabian Indrunas
 * @brief lightweight finite state machine implementation
 * @version 0.1
 * @date 2021-06-08
 *
 * @details the FSM...
 */

#include "stdio.h"
#include "../inc/usart.h"

#include <stdlib.h>
#include "../inc/fsm.h"

// TODO add FSM INit method to make more clear that there has to be a INIT and EXIT state?
// ONLY Store name of states if debug mode is enabled.

// forward declaration
void exit_fsm_cycle(FSM *fsm);


void add_state(FSM *fsm, State state, char *state_name, void (*enter)(RoboterData*), void (*update)(FSM*, RoboterData *)) {
    ConcreteState *new_state = (ConcreteState*)malloc(sizeof(ConcreteState));
    if (new_state == NULL) { exit(EXIT_FAILURE); }

    new_state->state = state;
    new_state->state_name = state_name;
    new_state->enter_function = enter;
    new_state->update_function = update;

    fsm->states[state] = new_state;

    /* The INIT State becomes the default state. Also automatically adds Exit state. */
    if (state == INIT) {
        fsm->current_state = new_state;
        add_state(fsm, EXIT, "Exit", NULL, NULL);
    }
}

void start_fsm_cycle(FSM *fsm, RoboterData *data) {

//    USART_Init(UBRR_SETTING);
//
//    RoboterData *data2 = data;
//    static char str_buf[20];
//    sprintf(str_buf, "%d %d %d %d %d\n", data2->calibration_mode, data2->debug_mode, data2->start_counter_mode, data2->lapcounter_mode, data2->laps_to_go);
//    USART_print(str_buf);

    fsm->current_state->enter_function(data);   // enter is called once for the default state.

    while(fsm->current_state->state != EXIT) {
        fsm->current_state->update_function(fsm, data);
    }

    exit_fsm_cycle(fsm);                      // Shuts down the Robot and terminates the programm.
}

void transition_to_state(FSM *fsm, RoboterData *arg, State next_state) {
    fsm->current_state = fsm->states[next_state];

    if (fsm->current_state->enter_function != NULL) {
        fsm->current_state->enter_function(arg);
    }
}

/**
 * @brief shuts down the fsm and frees storage.
 */
void exit_fsm_cycle(FSM *fsm) {
    for (int i = 0; i < STATECOUNT; ++i) {
        free(fsm->states[i]);
        fsm->states[i] = NULL;
    }

    fsm = NULL;
}




