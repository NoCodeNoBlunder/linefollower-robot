
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "fsm.h"
#include "typedefs.h"
#include "iesusart.h"
#include <util/delay.h>



// forward declaration
void terminate_fsm_cycle(FSM *fsm);

void start_fsm_cycle(FSM *fsm_instance, void *data) {

    // enter wird einmalig für die erste state aufgerufen!
    // enter sollte auch eine void pointer übergeben bekommen!
    fsm_instance -> currentState ->enter_ptr(fsm_instance, data);

    USART_print("FSL cycle initililized\n");

    // TODO brauche ich hier eine Abbruchbedienung?
    while(fsm_instance -> currentState -> state != TERMINATE) {

        char buf[10];
        sprintf(buf, "%d -- %d", fsm_instance -> currentState -> state, TERMINATE);
        USART_print(buf);

        // TODO Muss ich hier die complette Signatur angeben??
        void (*update_fun)(struct FSM*, void *) = fsm_instance ->currentState ->update_ptr;
        if (*update_fun != NULL) { update_fun(fsm_instance, data); }

        USART_print(fsm_instance ->currentState->stateName);
        USART_print("\n\n");
        _delay_ms( 500);
    }


    terminate_fsm_cycle(fsm_instance);
    USART_print("WHILE LOOP LEFT!!!\n");
}

void switchState(FSM *fsm_instance, State nextState, void *arg) {
    fsm_instance -> currentState = fsm_instance ->states[nextState];

    void (*enter_fun)(struct FSM *, void *) = fsm_instance ->currentState ->enter_ptr;
    // Hier muss dereferenziert werden!
    if (*enter_fun != NULL) {
        USART_print("GEHST DU hier rein?");
        enter_fun(fsm_instance, arg);
    }
    // fsm_instance -> currentState -> enter_ptr(fsm_instance);
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
        addState(fsm, TERMINATE, "Terminate", NULL, NULL);

        USART_print("Die State wird gesetzt?\n");
        USART_transmitByte(TERMINATE + 48);
        USART_print("\n");
    }
}

// TODO was überlegen
void terminate_fsm_cycle(FSM *fsm) {
    // TODO Gebe ich dadurch den ganzen durch malloc allocierten Speicher wieder frei?

    USART_print("enter termination?\n");
    for (int i = 0; i < STATECOUNT; ++i) {
        free(fsm ->states[i]);
        fsm -> states[i] = NULL;
    }

    fsm = NULL;
}




