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
#define SAMPLE_SIZE 20
#define STR_BUF_SIZE 40
#define SHORT_wTIME 350

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
 * @enum Sensor
 * @brief enum stores the corrosponding pins for the sensors
 */
enum Sensor {
    LEFT_SENSOR = PC2,
    MID_SENSOR = PC1,
    RIGHT_SENSOR = PC0
};

/**
 * @struct RoboterData "main.h"
 */
typedef struct RoboterData {
    char debug_mode;
    short sensor_left;
    short sensor_mid;
    short sensor_right;

    unsigned char left_eng_speed; // Only has values between 0 and 255
    unsigned char right_eng_speed;
} RoboterData;



#endif //MAIN_h
