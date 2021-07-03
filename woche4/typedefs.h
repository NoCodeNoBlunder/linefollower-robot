
#ifndef E3T4_C_TYPEDEFS_H
#define E3T4_C_TYPEDEFS_H

typedef enum {
    IDLE,
    FORWARD,
    BACKWARD,
    LEFT_TURN,
    RIGHT_TURN
} DriveMode;

typedef enum {
    LEFT_ENG = PD5,
    RIGHT_ENG = PD6
} Motor ;

typedef enum {
    ENG_STILL = 0,
    ENG_SLOW = 50,
    ENG_MID = 155,
    ENG_FAST = 200
} Speed;


typedef enum {
    LEFT_SENSOR = PC0,
    MID_SENSOR = PC1,
    RIGHT_SENSOR = PC2
} Sensor;

typedef struct {
    DriveMode mode;
    short sensor_left;
    short sensor_mid;
    short sensor_right;

    char left_eng_rot;
    char right_eng_rot;
    char left_eng_dir;
    char right_eng_dir;

} RoboterData;
#endif //E3T4_C_TYPEDEFS_H
