/**
 * @file main.h
 * @version 0.1
 * @date 2021-06-08
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
 * @brief enum stores the corrosponding pins for the sensors and also stor
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
 * @struct RoboterData "main.h"
 */
typedef struct RoboterData {
    char start_counter_mode;
    char debug_mode;
    char lapcounter_mode;
    unsigned char laps_to_go;

    unsigned short sensor_left;
    unsigned short sensor_mid;
    unsigned short sensor_right;

    unsigned char left_eng_speed; // Only has values between 0 and 255
    unsigned char right_eng_speed;
} RoboterData;


#endif //MAIN_h
