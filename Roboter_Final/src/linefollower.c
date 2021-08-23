
/**
 * @file
 * @brief
 * Determines the sensors Thresholds
 */

#include "../inc/linefollower.h"
#include "../inc/adc.h"

/**
 * @def
 * determines how many sensor meassurements are taken per function call.
 */
#define SAMPLE_SIZE 20

/** @enum Threshold
 *  @brief enums values determine the threshold of each sensor.
 *  When a sensors value is bigger than the Threshold the sensor detects a line.
 *  @details due to inaccuracy a sensor might fire early or late
 */
enum Threshold{
    THRESHOLD_L = 250, /**< Threshold of the left sensor*/
    THRESHOLD_M = 512, /**< Threshold of the middle sensor*/
    THRESHOLD_R = 412, /**< Threshold of the right sensor*/
};

void take_measurement(RoboterData *data) {
    data->sensor_left = ADC_read_avg(LEFT_LF, SAMPLE_SIZE);
    data->sensor_right = ADC_read_avg(RIGHT_LF, SAMPLE_SIZE);
    data->sensor_mid = ADC_read_avg(MID_LF, SAMPLE_SIZE);
}

char left_on_line(RoboterData *data) {
    return data->sensor_left >= THRESHOLD_L;
}

char mid_on_line(RoboterData *data) {
    return data->sensor_mid >= THRESHOLD_M;
}

char right_on_line(RoboterData *data) {
    return data->sensor_right >= THRESHOLD_R;
}

char all_on_line(RoboterData *data) {
    return left_on_line(data) && mid_on_line(data) && right_on_line(data);
}

char all_off_line(RoboterData *data) {
    return !left_on_line(data) && !mid_on_line(data) && !right_on_line(data);
}
