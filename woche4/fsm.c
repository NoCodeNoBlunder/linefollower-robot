/**
 * @file fsm.c
 * @author Fabian Indrunas
 * @brief lightweight finite state machine implementation
 * @version 0.1
 * @date 2021-06-08
 *
 * @details the FSM
 */

#include <stdlib.h>
#include <stdio.h>      // Unused
#include <util/delay.h> // Unused

#include "fsm.h"
#include "main.h"       // Unused
#include "iesusart.h"   // Unused

#define SHORT_wTIME 250

// forward declaration
void exit_fsm_cycle(FSM *fsm);


// TODO was möchte ich hier alles rüberbringen?
// first state added becomes the default state.

/**
 * Function to create and store a new state
 * @param fsm pointer to struct of type FSM which represent an instance of FSM
 * @param state enum value which is used to differentiate states
 * @details this state has to exist in "fsm.h" STATE Typedef.
 * @param name string representing the name of the state for debugging. Should be the same as enum.
 * @param enter function pointer to the new states enter function implementation
 * @param update function pointer to the new states update function implementation
 * @details each state has to implement an update function.
 */
void add_state(FSM *fsm, State state, char *name, void (*enter), void (*update)) {
    ConcreteState *new_state = (ConcreteState*)malloc(sizeof (ConcreteState));
    new_state->state = state;
    new_state->state_name = name;
    new_state->enter_ptr = enter;
    new_state->update_ptr = update;

    fsm->states[state] = new_state;

    /* The first State added becomes the default state also automatically adds Exit state. */
    if (state == 0) {
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
    fsm->current_state->enter_ptr(data);   // enter is called once for the default state.

    while(fsm->current_state->state != EXIT) {
        fsm->current_state->update_ptr(fsm, data);
    }

    exit_fsm_cycle(fsm);                      // Shuts down the Robot and terminates the programm.
}

/**
 * Function to switch between states.
 * @details changes state to the specified state and calls it's enter function
 * @param arg void pointer to be able to pass an argument to enter.
 * @param next_state enum value of the state to transition into
 */
void switch_state(FSM *fsm, void *arg, State next_state) {
    fsm -> current_state = fsm ->states[next_state];
    fsm ->current_state ->enter_ptr(arg);
}

/**
 * Function to shutdown the fsm and free storage
 */
void exit_fsm_cycle(FSM *fsm) {
    // TODO stop robot macht vlt doch sinn dafür eine Methode in der Main zu schreiben.

    // TODO Gebe ich dadurch den ganzen durch malloc allocierten Speicher wieder frei?
    for (int i = 0; i < STATECOUNT; ++i) {
        free(fsm ->states[i]);
        fsm -> states[i] = NULL;
    }

    fsm = NULL;
}




