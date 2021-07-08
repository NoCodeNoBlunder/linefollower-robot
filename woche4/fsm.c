#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "fsm.h"
#include "typedefs.h"
#include "iesusart.h"

#define SHORT_wTIME 250

// forward declaration
void terminate_fsm_cycle(FSM *fsm);

void start_fsm_cycle(FSM *fsm_instance, void *data) {

    // enter wird einmalig für die erste state aufgerufen!
    fsm_instance -> currentState ->enter_ptr(fsm_instance, data);

    while(fsm_instance -> currentState -> state != SHUTDOWN) {

        void (*update_fun)(struct FSM*, void *) = fsm_instance ->currentState ->update_ptr;
        if (*update_fun != NULL) { update_fun(fsm_instance, data); }

        _delay_ms(SHORT_wTIME);
    }

    terminate_fsm_cycle(fsm_instance); // Terminates the Programm and shuts down Robot
}

void switchState(FSM *fsm_instance, State nextState, void *arg) {
    fsm_instance -> currentState = fsm_instance ->states[nextState];
    void (*enter_fun)(struct FSM *, void *) = fsm_instance ->currentState ->enter_ptr;
    // Hier muss dereferenziert werden!
    if (*enter_fun != NULL) {
        enter_fun(fsm_instance, arg);
    }
}

// TODO muss hier nich die vollständige Signatur angegeben werden?
void addState(FSM *fsm, State state, char *name, void (*enter), void (*update)) {
    ConcreteState *new_state = (ConcreteState*)malloc(sizeof (ConcreteState));
    new_state ->state = state;
    new_state ->stateName = name;
    new_state ->enter_ptr = enter;
    new_state ->update_ptr = update;

    fsm ->states[state] = new_state;

    // The first State added becomes the default state also automatically adds Termination state.
    if (state == 0) {
        fsm ->currentState = new_state;
        addState(fsm, SHUTDOWN, "Terminate", NULL, NULL);
    }
}

void terminate_fsm_cycle(FSM *fsm) {
    // TODO Gebe ich dadurch den ganzen durch malloc allocierten Speicher wieder frei?
    for (int i = 0; i < STATECOUNT; ++i) {
        free(fsm ->states[i]);
        fsm -> states[i] = NULL;
    }

    fsm = NULL;
}




