
/**
 * @file
 * @brief provides basic functionality to take measurements from all 3 sensors.
 */

#ifndef IESMOTORS_C_LINEFOLLOWER_H
#define IESMOTORS_C_LINEFOLLOWER_H
#include "main.h"

/**
 * @fn
 * @brief takes measurement of all sensors and stores the values in @param data struct.
 * @param data pointer to struct containing all information regarding the robot.
 */
void take_measurement(RoboterData *data);

/**
 *
 * @param data
 * @return
 */
char left_on_line(RoboterData *data);

/**
 *
 * @param data
 * @return
 */
char mid_on_line(RoboterData *data);

/**
 *
 * @param data
 * @return
 */
char right_on_line(RoboterData *data);

/**
 *
 * @param data
 * @return
 */
char all_on_line(RoboterData *data);

/**
 *
 * @param data
 * @return
 */
char all_off_line(RoboterData *data);


#endif //IESMOTORS_C_LINEFOLLOWER_H
