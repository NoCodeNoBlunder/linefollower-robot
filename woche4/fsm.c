#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>


#include "fsm.h"
#include "typedefs.h"
#include "iesusart.h"

#define SHORT_wTIME 250

// forward declaration
void exit_fsm_cycle(FSM *fsm);

void add_state(FSM *fsm, State state, char *name, void (*enter), void (*update)) {
    ConcreteState *new_state = (ConcreteState*)malloc(sizeof (ConcreteState));
    new_state ->state = state;
    new_state ->state_name = name;
    new_state ->enter_ptr = enter;
    new_state ->update_ptr = update;

    fsm ->states[state] = new_state;

    /* The first State added becomes the default state also automatically adds Exit state. */
    if (state == 0) {
        fsm ->current_state = new_state;
        add_state(fsm, EXIT, "Exit", NULL, NULL);
    }
}

/* Switches to next state and invokes its enter() */
void switch_state(FSM *fsm, void *arg, State next_state) {
    fsm -> current_state = fsm ->states[next_state];
    fsm ->current_state ->enter_ptr(arg);
}

/* Start the fsm update loop */
void start_fsm_cycle(FSM *fsm_instance, void *data) {

    fsm_instance -> current_state ->enter_ptr(data);   // enter is called once for the default state.

    while(fsm_instance -> current_state -> state != EXIT) {
        fsm_instance ->current_state ->update_ptr(fsm_instance, data);
    }

    exit_fsm_cycle(fsm_instance);                      // Shuts down the Robot and terminates the programm.
}

void exit_fsm_cycle(FSM *fsm) {
    // TODO stop robot macht vlt doch sinn dafür eine Methode in der Main zu schreiben.

    // TODO Gebe ich dadurch den ganzen durch malloc allocierten Speicher wieder frei?
    for (int i = 0; i < STATECOUNT; ++i) {
        free(fsm ->states[i]);
        fsm -> states[i] = NULL;
    }

    fsm = NULL;
}




