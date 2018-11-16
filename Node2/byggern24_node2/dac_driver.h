#ifndef DAC_DRIVER_H_
#define DAC_DRIVER_H_

#include <stdint.h>

void dac_init();
void dac_driver_send(uint8_t data);

#endif /* DAC_DRIVER_H_ */