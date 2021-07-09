

#ifndef FSM_C_FSM_H
#define FSM_C_FSM_H

#include "typedefs.h"

typedef enum {
    INIT,     // default State has to be the first state.
    COUNTDOWN,
    STRAIGHT,
    LEFT,
    RIGHT,
    GOAL_REACHED,
    EXIT,
    STATECOUNT,
} State ;

// forward declaration
struct FSM;

typedef struct ConcreteState {
    State state;
    char * state_name; // unused but good for debugging.
    void (*enter_ptr)(struct FSM *fsm, void *arg);
    void (*update_ptr)(struct FSM *fsm, void *arg);
} ConcreteState;

typedef struct FSM {
    ConcreteState *current_state;
    ConcreteState *states[STATECOUNT];
} FSM;

// TODO hier muss bei den Funktionen die Signatur mit angegeben werden?
void add_state(FSM *fsm, State state, char *name, void (*enter), void (*update));

void start_fsm_cycle(FSM *fsm_instance, void *data);

void switch_state(FSM *fsm_instance, void *arg, State nextState);

#endif //FSM_C_FSM_H
