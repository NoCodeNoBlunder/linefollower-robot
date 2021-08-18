
#ifndef IESMOTORS_C_LINEFOLLOWER_H
#define IESMOTORS_C_LINEFOLLOWER_H
#include "main.h"

void take_measurement(RoboterData *data);
char left_on_line(RoboterData *data);
char mid_on_line(RoboterData *data);
char right_on_line(RoboterData *data);
char all_on_line(RoboterData *data);
char all_off_line(RoboterData *data);


#endif //IESMOTORS_C_LINEFOLLOWER_H
