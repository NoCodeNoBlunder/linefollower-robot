
#include "linefollower.h"
#include "iesadc.h"

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
