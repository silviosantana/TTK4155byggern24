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
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row,uint8_t column);
void oled_print(char* name, int size);
void oled_print_arrow(uint8_t row, uint8_t col);
void oled_test();
void oled_write_pixel(int x, int y);
void oled_line(int x0, int y0, int x1, int y1);
void oled_circle(int x, int y, int r);
void oled_animate_mario_large();
void oled_print_mario_large(uint8_t row,uint8_t column);
void oled_test();
void oled_print_ntnu_logo(uint8_t row, uint8_t column);
void oled_animate_ntnu();
void oled_print_special_char (int code);
void oled_print_char(char character, int size);

#endif // OLED_DRIVER_H_