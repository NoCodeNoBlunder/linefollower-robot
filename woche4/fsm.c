
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "fsm.h"
#include "typedefs.h"
void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void start_fsm_cycle(FSM *fsm_instance, void *data) {

    // start
    fsm_instance -> currentState ->enter_ptr();

    // TODO brauche ich hier eine Abbruchbedienung?
    while(fsm_instance -> currentState -> state != TERMINATE) {
        // Muss ich hier die complette Signatur angeben??
        void (*update_fun)(struct FSM*, void *arg) = fsm_instance ->currentState ->update_ptr;
        if (update_fun != NULL) {
            update_fun(fsm_instance, data);
            // fsm_instance -> currentState-> update_ptr(fsm_instance, data);
        }

        printf("%s\n", fsm_instance ->currentState->stateName);
        waitFor(1);

        printf("\n");
    }
}

void switchState(FSM *fsm_instance, State nextState) {
    fsm_instance -> currentState = fsm_instance ->states[nextState];
    fsm_instance -> currentState -> enter_ptr();
}

// TODO muss hier nich die vollständige Signatur angegeben werden?
void addState(FSM *fsm, State state, char *name, void (*enter), void (*update)) {
    ConcreteState *new_state = (ConcreteState*)malloc(sizeof (ConcreteState));
    new_state ->state = state;
    new_state ->stateName = name;
    new_state ->enter_ptr = enter;
    new_state ->update_ptr = update;

    fsm ->states[state] = new_state;

    // The first State added becomes the default state
    if (state == 0) {
        fsm ->currentState = new_state;
        addState(fsm, TERMINATE, "Terminate", NULL, NULL);
    }
}





