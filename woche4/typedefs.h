
#ifndef E3T4_C_TYPEDEFS_H
#define E3T4_C_TYPEDEFS_H

#include <avr/io.h>

// TODO not all have to be enum or typedefs!
// TODO I Should not have everything here it pollutes the name space!
typedef enum {
    IDLE,
    FORWARD,
    BACKWARD,
    LEFT_TURN,
    RIGHT_TURN,
    LEFT_SPIN,
    RIGHT_SPIN
} DriveMode;

typedef enum {
    LEFT_ENG = PD5,
    RIGHT_ENG = PD6
} Motor;

typedef enum {
    ENG_STILL = 0,
    ENG_SLOW = 70,
    ENG_MID = 155,
    ENG_FAST = 200
} Speed;

typedef enum {
    LEFT_SENSOR = PC0,
    MID_SENSOR = PC1,
    RIGHT_SENSOR = PC2
} Sensor;

// TODO add DriveMode name to better debug
typedef struct {
    DriveMode mode;
    short sensor_left;
    short sensor_mid;
    short sensor_right;
    unsigned char left_eng_speed; // Only has values between 0 and 255
    unsigned char right_eng_speed;
} RoboterData;

#endif //E3T4_C_TYPEDEFS_H
