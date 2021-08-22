/**
 * @headerfile iesadc.h
 * @version 0.1
 * @date 2021-06-08
 */

#ifndef IESADC_h
#define IESADC_h

#include <stdint.h>

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples);

#endif //IESADC_h
