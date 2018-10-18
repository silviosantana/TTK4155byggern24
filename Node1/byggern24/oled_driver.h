#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include "avr/io.h"
#include "util.h"

#define OLED_COMMAND_MEM 0x1000
#define OLED_DATA_MEM 0x1200

void oled_init();

void oled_write_data(unsigned char ins_d);

void oled_write_cmd(unsigned char ins_c);

void oled_reset();

void oled_home();

void oled_goto_line(uint8_t line);

void oled_goto_column(uint8_t column);

void oled_clear_line(uint8_t line);

void oled_pos(uint8_t row,uint8_t column);

void oled_print(char* name, int size);

void OLED_print_arrow(uint8_t row, uint8_t col);

void oled_test();

menu* state_machine(menu* current_state, int* arrow_max, int arrow_pos);

menu* move_arrow(int dir, menu* current_state, int* arrow_max, int* arrow_pos);

void write_pixel(int x, int y);

void oled_line(int x0, int y0, int x1, int y1);

void oled_circle(int x, int y, int r);

void oled_animate_mario_large();

void oled_print_mario_large(uint8_t row,uint8_t column);

#endif