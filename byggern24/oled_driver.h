#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include "oled_driver.h"
#include "avr/io.h"

#define OLED_COMMAND_MEM 0x1000
#define OLED_DATA_MEM 0x1200

void oled_init();

void write_d(unsigned char ins_d);

void write_c(unsigned char ins_c);

void oled_reset();

void oled_home();

void oled_goto_line(uint8_t line);

void oled_goto_column(uint8_t column);

void oled_clear_line(uint8_t line);

void oled_pos(uint8_t row,uint8_t column);

void oled_print(char*);

void oled_test();

#endif