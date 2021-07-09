#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "fsm.h"
#include "typedefs.h"
#include "iesusart.h"

#define SHORT_wTIME 250

// forward declaration
void terminate_fsm_cycle(FSM *fsm);

// TODO kann ich das effizienter machen?
void start_fsm_cycle(FSM *fsm_instance, void *data) {

    // enter wird einmalig für die erste state aufgerufen!
    fsm_instance -> current_state ->enter_ptr(data);

    while(fsm_instance -> current_state -> state != EXIT) {

        // USART_print(fsm_instance ->current_state->state_name);

        fsm_instance ->current_state ->update_ptr(fsm_instance, data);

        _delay_ms(SHORT_wTIME);
    }

    terminate_fsm_cycle(fsm_instance); // Terminates Shuts down the Robot and terminates the programm.
}

void switch_state(FSM *fsm_instance, void *arg, State nextState) {
    fsm_instance -> current_state = fsm_instance ->states[nextState];
    fsm_instance ->current_state ->enter_ptr(arg);
}

// TODO muss hier nich die vollständige Signatur angegeben werden?
void add_state(FSM *fsm, State state, char *name, void (*enter), void (*update)) {
    ConcreteState *new_state = (ConcreteState*)malloc(sizeof (ConcreteState));
    new_state ->state = state;
    new_state ->state_name = name;
    new_state ->enter_ptr = enter;
    new_state ->update_ptr = update;

    fsm ->states[state] = new_state;

    // The first State added becomes the default state also automatically adds Termination state.
    if (state == 0) {
        fsm ->current_state = new_state;
        add_state(fsm, EXIT, "Terminate", NULL, NULL);
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




