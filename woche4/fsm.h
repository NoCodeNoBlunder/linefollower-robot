//
// Created by Fabian on 07.07.2021.
//

#ifndef FSM_C_FSM_H
#define FSM_C_FSM_H


#include "typedefs.h"

typedef enum {
    START,
    STEADY,
    ACCELRATION,
    TERMINATE,
    STATECOUNT,
} State ;

// forward declaration
struct FSM;

typedef struct ConcreteState {
    // struct FSM state;
    // TODO not needed state here is is clear because of array index.
    State state;
    char * stateName;
    void (*enter_ptr)(void);
    void (*update_ptr)(struct FSM*, void *arg);
} ConcreteState;

typedef struct FSM {
    ConcreteState *currentState;
    ConcreteState *states[STATECOUNT];
//    ConcreteState ** states; Wieso geht das nicht
} FSM;

// TODO hier muss bei den Funktionen die Signatur mit angegeben werden?
void addState(FSM *fsm, State state, char *name, void (*enter), void (*update));

void start_fsm_cycle(FSM *fsm_instance, void *data);

void switchState(FSM *fsm_instance, State nextState);

#endif //FSM_C_FSM_H
