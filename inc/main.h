/**
 * @file
 * @brief
 * Contains Definitions that are used accross the hole project.
 * @author Fabian Indrunas
 */

#ifndef MAIN_h
#define MAIN_h

#include <avr/io.h>
#include "fsm.h"

/**
 * @def determines the CPU clock speed.
 */
#ifndef F_CPU
#define F_CPU 16E6
#endif

/**
 * @def determines how many laps the Roboter has to drive.
 */
#ifndef LAPS
#define LAPS 3
#endif

/**
 * @enum LineFollower
 * @brief
 * enum stores the corrosponding pins for the sensors and also stor
 */
typedef enum LineFollower { // TODO Besseren Namen
    LEFT_LF = PC2,
    MID_LF = PC1,
    RIGHT_LF = PC0,
    LEFT_AND_RIGHT = -1,
    LEFT_AND_MID = -2,
    RIGHT_AND_MID = -3,
    ALL = -4,
    NONE = -5,
} LineFollower;

/**
 * @struct RoboterData
 * @brief Stores all the relevant information regarding the robot.
 */
typedef struct RoboterData_S {
    unsigned char calibration_mode;
    unsigned char start_counter_mode; /**< Stores if the robot is in COUNTDOWN_MODE. */
    unsigned char debug_mode; /**< Stores if the robot is in DEBUG_MODE. */
    unsigned char lapcounter_mode; /**< Stores if the robot is in LAPCOUNTER_MODE. */
    unsigned char laps_to_go; /**< Stores how many laps the robot has to drive. */

    unsigned short sensor_left; /**< Stores the most recent value of the lest sensor. */
    unsigned short sensor_mid;  /**< Stores the most recent value of the middle sensor. */
    unsigned short sensor_right; /**< Stores the most recent value of the right sensor. */

    unsigned char left_eng_speed;  /**< Stores the left engines duty cycle setting. */
    unsigned char right_eng_speed; /**< Stores the right engines current duty cycle setting. */
} RoboterData;


#endif //MAIN_h
