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
 * @def SAMPLE_SIZE determines how many sensor meassurements are taken.
 */
#define SAMPLE_SIZE 20 // TODO flascher Ort fuer diese Konstanten?
#define STR_BUF_SIZE 40

/// CPU clock speed
#ifndef F_CPU
#define F_CPU 16E6
#endif

/**
 *  @enum Threshold
 *  @brief enums values determine the threshold of each sensor
 *  @details due to inaccuracy a sensor might fire early or late
 */
enum Threshold{
    THRESHOLD_L = 512,
    THRESHOLD_M = 512,
    THRESHOLD_R = 512,
};

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
    unsigned char lapcounter;

    unsigned short sensor_left;
    unsigned short sensor_mid;
    unsigned short sensor_right;

    unsigned char left_eng_speed; // Only has values between 0 and 255
    unsigned char right_eng_speed;
} RoboterData;

void take_measurement(RoboterData *data);
char left_on_line(RoboterData *data);
char mid_on_line(RoboterData *data);
char right_on_line(RoboterData *data);
char all_on_line(RoboterData *data);

#endif //MAIN_h
