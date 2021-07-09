
/**
 * @file fsm.h
 * @brief Basic serial communication via USART for ATMEGA328
 * @version 0.1
 * @date 2021-06-08
 */

#ifndef FSM_C_FSM_H
#define FSM_C_FSM_H

// forward declaration
struct FSM;

/* These are all the states of the FSM */
typedef enum {
    INIT,      // default State has to be the first state.
    COUNTDOWN,
    FORWARD,
    LEFT,
    RIGHT,
    GOAL_REACHED,
    EXIT,
    STATECOUNT, // Is not an actual state but is there to know the statecount.
} State ;

/* Members of a State */
typedef struct ConcreteState {
    State state;
    char * state_name; // unused but good for debugging.
    void (*enter_ptr)(void *arg);
    void (*update_ptr)(struct FSM *fsm, void *arg);
} ConcreteState;

typedef struct FSM {
    ConcreteState *current_state;
    ConcreteState *states[STATECOUNT];
} FSM;

// TODO hier muss bei den Funktionen die Signatur mit angegeben werden?
// Nur der Return Typ der Funktionen muss hier angegeben werden.
void add_state(FSM *fsm, State state, char *name, void (*enter), void (*update));

void start_fsm_cycle(FSM *fsm_instance, void *data);

void switch_state(FSM *fsm, void *arg, State next_state);

#endif //FSM_C_FSM_H
