
#ifndef E3T4_C_TYPEDEFS_H
#define E3T4_C_TYPEDEFS_H

#include <avr/io.h>

// TODO add DriveMode name to better debug
typedef struct {
    short sensor_left;
    short sensor_mid;
    short sensor_right;

    // scheiß auf eng speed!
    unsigned char left_eng_speed; // Only has values between 0 and 255
    unsigned char right_eng_speed;
} RoboterData;

#endif //E3T4_C_TYPEDEFS_H
