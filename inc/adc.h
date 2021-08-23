

/**
 * @file
 * @brief provides basic functionality to convert a analog signal to digital.
 */

#ifndef IESADC_h
#define IESADC_h


#include <stdint.h>

/**
 * @brief
 * Sets up the analog to ditigal converter
 */
void ADC_Init(void);

/**
 * @brief function to measure the voltage at pin @param channel
 * @param channel determines which channel to measure from
 * @return digital value between 0 and 1023
 */
uint16_t ADC_Read(uint8_t channel);

/**
 * @function function to measure the average voltage at pin @param channel @sa ADC_INIT()
 * @param channel determines which channel to measure from
 * @param nsamples determines the samplesize
 * @return the average value across nsamples
 */
uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples);

#endif //IESADC_h
