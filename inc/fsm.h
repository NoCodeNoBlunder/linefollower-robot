/**
 * @file
 * @brief definition of types for fsm.c
 * @details
 * @date 2021-06-08
 * @author Fabian Indrunas
 */

#ifndef FSM_h
#define FSM_h

// forward declaration
struct FSM;

/**
 * @typedef State
 * @brief used to differentiate between states.
 * @details is used to add, store, and get the desired state from struct FSM
 * To implement a new state it has to be added here.
 */
typedef enum State{
    INIT,      // default State has to be the INIT state
    CHECK_STARTPOS,
    COUNTDOWN,
    LEAVE_START,
    FORWARD,
    LEFT_SOFT,
    LEFT_HARD,
    RIGHT_HARD,
    RIGHT_SOFT,
    CHECK_LAP,
    GOAL_REACHED,
    STILL,
    EXIT,
    STATECOUNT, // Is not an actual state but is there to know the amount of states.
} State;

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
    char *state_name;
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

/**
 * Function to create and store a new state
 * @param fsm pointer to struct of type FSM which represent an instance of FSM
 * @param state enum value which is used to differentiate states
 * @details this state has to exist in "fsm.h" STATE Typedef.
 * @param state_name string representing the state_name of the state for debugging. Should be the same as enum.
 * @param enter function pointer to the new states enter function implementation
 * @param update function pointer to the new states update function implementation
 * @details each state has to implement an update function.
 */
void add_state(FSM *fsm, State state, char *state_name, void (*enter), void (*update));

/**
 * Function to start the update loop
 * @details every iteration the current state's update function is called
 * @param data void pointer to be able to pass an argument to update
 */
void start_fsm_cycle(FSM *fsm, void *data);

/**
 * Function to switch between states.
 * @details changes state to the specified state and calls it's enter function
 * @param arg void pointer to be able to pass an argument to enter.
 * @param next_state enum value of the state to transition into
 */
void transition_to_state(FSM *fsm, void *arg, State next_state);

#endif //FSM_h
