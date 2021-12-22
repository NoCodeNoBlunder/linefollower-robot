
/**
 * @file
 * @brief provides basic functionality to take measurements from all 3 sensors.
 */

#ifndef IESMOTORS_C_LINEFOLLOWER_H
#define IESMOTORS_C_LINEFOLLOWER_H
#include "main.h"

/**
 * @brief takes measurement of all sensors and stores the values in @param data struct.
 * @param data pointer to struct containing all information regarding the robot.
 */
void take_measurement(RoboterData *data);

/**
 * @brief
 * Checks wether the left sensors last messurement is above the threshold
 * @return true if the left sensor detects the line else false.
 */
char left_on_line(RoboterData *data);

/**
 * @brief
 * Checks wether the middle sensors last messurement is above the threshold
 * @return true if the middle sensor detects the line else false.
 */
char mid_on_line(RoboterData *data);

/**
 * @brief
 * Checks wether the right sensors last messurement is above the threshold
 * @return true if the right sensor detects the line else false.
 */
char right_on_line(RoboterData *data);

/**
 * @brief Checks wether all sensors last messurement are above the threshold
 * @return true if all sensors detect the line.
 * @details this happends when the robot is on the startfield or in a sharp turn.
 */
char all_on_line(RoboterData *data);

/**
 * @brief Checks wether all sensor are detecting white.
 * @return returns true if all sensor dont detect the line else false.
 */
char all_off_line(RoboterData *data);


#endif //IESMOTORS_C_LINEFOLLOWER_H
