#ifndef SRAM_DRIVER_H_
#define SRAM_DRIVER_H_

#include <stdint.h>

#define START_SRAM 0x1800

void SRAM_init();
void SRAM_write(uint16_t address, uint8_t data);
uint8_t SRAM_read(uint16_t address);
void SRAM_test();



#endif 