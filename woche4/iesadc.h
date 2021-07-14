/**
 * @file iesadc.h
 * @version 0.1
 * @date 2021-06-08
 */


#ifndef E3T4_C_IESADC_H
#define E3T4_C_IESADC_H

// TODO Wieso musste ich das jetzt inkludieren und eben nicht?
#include "stdint.h"

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples);

#endif // E3T4_C_IESADC_H
