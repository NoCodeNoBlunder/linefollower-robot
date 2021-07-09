/**
 * @file main.h
 * @version 0.1
 * @date 2021-06-08
 */


#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <avr/io.h>
// DONt use var

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

#endif //TYPEDEFS_H
