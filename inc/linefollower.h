
/**
 * @headerfile linefollower.h
 */

#ifndef IESMOTORS_C_LINEFOLLOWER_H
#define IESMOTORS_C_LINEFOLLOWER_H
#include "main.h"

/**
 *
 * @param data
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
