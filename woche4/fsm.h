/**
 * @file fsm.h
 * @brief definition of types for fsm.c
 * @details
 * @version 0.1
 * @date 2021-06-08
 */

#ifndef FSM_C_FSM_H
#define FSM_C_FSM_H

// forward declaration
struct FSM;

/* These are all the states of the FSM */

/**
 * @struct State "fsm.h"
 * @brief used to differentiate between states.
 * @details is used to add, store, and get the desired state from struct FSM
 * Every state added has to exist here.
 * @var EXIT shuts down fsm update loop and is implemented automatically.
 * @var STATECOUNT is not an actual state but is used to know the ammount of states.
 *
 */
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
/**
 * @struct ConcreteState "fsm.h"
 * @brief stores all relevant information regarding the state
 * @var state is used to distingish this state from other states
 * @var state_name is a string storing the name of the state for debugging
 * @var enter_function is a void function pointer to the implementation of this state's enter function
 * @var update_function is a void function pointer to implementation of this state's update function
 */
typedef struct ConcreteState {
    State state;
    char * state_name; // unused but good for debugging.
    void (*enter_function)(void *arg);
    void (*update_function)(struct FSM *fsm, void *arg);
} ConcreteState;

/**
 * @struct FSM "fsm.h"
 * @brief Stores all states.
 * @var current_state is a pointer to the current state
 * @var states is a pointer array to each state
 */
typedef struct FSM {
    ConcreteState *current_state;
    ConcreteState *states[STATECOUNT];
} FSM;

// TODO hier muss bei den Funktionen die Signatur mit angegeben werden?
// Nur der Return Typ der Funktionen muss hier angegeben werden.
void add_state(FSM *fsm, State state, char *state_name, void (*enter), void (*update));

void start_fsm_cycle(FSM *fsm, void *data);

void transition_to_state(FSM *fsm, void *arg, State next_state);

#endif //FSM_C_FSM_H
