#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>

void adc_init ();
uint16_t adc_read(uint8_t ch);
void adc_driver_test();

#endif // ADC_DRIVER_H_