/**
 * @file fsm.c
 * @author Fabian Indrunas
 * @brief lightweight finite state machine implementation
 * @version 0.1
 * @date 2021-06-08
 *
 * @details the FSM...
 */

#include "../lib/stdlib.h"
#include "fsm.h"

// TODO add FSM INit method to make more clear that there has to be a INIT and EXIT state?
// ONLY Store name of states if debug mode is enabled.

// forward declaration
void exit_fsm_cycle(FSM *fsm);

/**
 * Function to create and store a new state
 * @param fsm pointer to struct of type FSM which represent an instance of FSM
 * @param state enum value which is used to differentiate states
 * @details this state has to exist in "fsm.h" STATE Typedef.
 * @param state_name string representing the state_name of the state for debugging. Should be the same as enum.
 * @param enter function pointer to the new states enter function implementation
 * @param update function pointer to the new states update function implementation
 * @details each state has to implement an update function.
 */
void add_state(FSM *fsm, State state, char *state_name, void (*enter), void (*update)) {
    ConcreteState *new_state = malloc(sizeof(ConcreteState));
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

/**
 * Function to start the update loop
 * @details every iteration the current state's update function is called
 * @param data void pointer to be able to pass an argument to update
 */
void start_fsm_cycle(FSM *fsm, void *data) {
    fsm->current_state->enter_function(data);   // enter is called once for the default state.

    while(fsm->current_state->state != EXIT) {
        fsm->current_state->update_function(fsm, data);
    }

    exit_fsm_cycle(fsm);                      // Shuts down the Robot and terminates the programm.
}

/**
 * Function to switch between states.
 * @details changes state to the specified state and calls it's enter function
 * @param arg void pointer to be able to pass an argument to enter.
 * @param next_state enum value of the state to transition into
 */
void transition_to_state(FSM *fsm, void *arg, State next_state) {
    fsm->current_state = fsm->states[next_state];

    if (fsm->current_state->enter_function != NULL) {
        fsm->current_state->enter_function(arg);
    }
}

/**
 * Function to shutdown the fsm and free storage
 */
void exit_fsm_cycle(FSM *fsm) {
    for (int i = 0; i < STATECOUNT; ++i) {
        free(fsm->states[i]);
        fsm->states[i] = NULL;
    }

    fsm = NULL;
}




