#include "oled_driver.h"

#include <string.h>

#include "util.h"
#include "util/delay.h"
#include "avr/io.h"
#include "fonts.h"
#include "joystick.h"

void oled_init()
{
	oled_write_cmd(0xae);		//display off
	oled_write_cmd(0xa1);		//segment remap
	oled_write_cmd(0xda);		//common pads hardware: alternative
	oled_write_cmd(0x12);
	oled_write_cmd(0xc8);		//common output scan direction:com63~com0
	oled_write_cmd(0xa8);		//multiplex ration mode:63
	oled_write_cmd(0x3f);
	oled_write_cmd(0xd5);		//display divide ratio/osc. freq. mode
	oled_write_cmd(0x80);
	oled_write_cmd(0x81);		//contrast control
	oled_write_cmd(0x50);
	oled_write_cmd(0xd9);		//set pre-charge period
	oled_write_cmd(0x21);
	oled_write_cmd(0x20);		//Set Memory Addressing  Mode 
	oled_write_cmd(0x00);
	oled_write_cmd(0xdb);		//VCOM deselect level mode
	oled_write_cmd(0x30);
	oled_write_cmd(0xad);		//master configuration
	oled_write_cmd(0x00);
	oled_write_cmd(0xa4);		//out follows RAM content
	oled_write_cmd(0xa6);		//set normal display
	oled_write_cmd(0xaf);		//display on
}

void oled_write_data(unsigned char ins_d)
{
	volatile char *ext_oled = (char *) OLED_DATA_MEM; // OLED Data
	ext_oled[0] = ins_d;
}

void oled_write_cmd(unsigned char ins_c)
{
	volatile char *ext_oled = (char *) OLED_COMMAND_MEM; // OLED Command
	ext_oled[0] = ins_c;
}

//clear OLED screen
void oled_reset()
{
	oled_goto_line(0);
	oled_goto_column(0);
	for(int i = 0; i < 1024; i++)
	{
		oled_write_data(0x00);
	}
}

//go to specific page of OLED (0 - 7)
void oled_goto_line(uint8_t line)
{
	oled_write_cmd(0x22);
	oled_write_cmd(line);
	oled_write_cmd(7);
}

//go to specific column of OLED (0 - 127)
void oled_goto_column(uint8_t column)
{
	oled_write_cmd(0x21);
	oled_write_cmd(column);
	oled_write_cmd(127);
}

//clear all columns of specific page (0 - 7)
void oled_clear_line(uint8_t line)
{
	oled_goto_line(line);
	for (int i = 0; i < 128; i++)
	{
		oled_write_data(0x00);
	}
}

//go to page and column (0 - 7 | 0 - 127)
void oled_pos(uint8_t row,uint8_t column)
{
	oled_goto_line(row);
	oled_goto_column(column);
}

void oled_print_arrow(uint8_t row, uint8_t col)
{
	oled_pos(row, col);
	oled_write_data(0b00011000);
	oled_write_data(0b00011000);
	oled_write_data(0b01111110);
	oled_write_data(0b00111100);
	oled_write_data(0b00011000);
}

//print a string starting on the current position
//size: 1 - small | 2 - medium | 3 - large 
void oled_print(char* name, int size)
{
	for (int i = 0; i < strlen(name); i++)
	{
		oled_print_char(name[i], size);
	}
}

//print a character on the current position
//size: 1 - small | 2 - medium | 3 - large
void oled_print_char(char character, int size)
{
	char mychar;
	int code = ((int) character) - 32;
	
	if (size == 3)
	{
		for (int k = 0; k < 8; k++)
		{
			mychar =  pgm_read_byte_near(font8[code] + k);
			mychar = mychar;
			oled_write_data(mychar);
		}
	}else if (size == 2)
	{
		for (int k = 0; k < 5; k++)
		{
			mychar =  pgm_read_byte_near(font5[code] + k);
			oled_write_data(mychar);
		}
	}else 
	{
		for (int k = 0; k < 4; k++)
		{
			mychar =  pgm_read_byte_near(font4[code] + k);
			oled_write_data(mychar);
		}
	}
	
}

void oled_print_special_char (int code)
{
	char mychar;
	
	for (int k = 0; k < 8; k++)
	{
		mychar =  pgm_read_byte_near(specialChar[code] + k);
		oled_write_data(mychar);
	}
}

//write a specific pixel on (x,y)
void oled_write_pixel(int x, int y)
{
	int page = y / 8;
	int page_y = 8 - (y % 8);
	oled_goto_line(page);
	oled_goto_column(x);
	uint8_t data;
	switch (page_y)
	{
		case 1:
		data = 0b10000000;
		break;
		case 2:
		data = 0b01000000;
		break;
		case 3:
		data = 0b00100000;
		break;
		case 4:
		data = 0b00010000;
		break;
		case 5:
		data = 0b00001000;
		break;
		case 6:
		data = 0b00000100;
		break;
		case 7:
		data = 0b00000010;
		break;
		case 8:
		data = 0b00000001;
		break;
	}
	oled_write_data(data);
}

//draw a line
void oled_line(int x0, int y0, int x1, int y1)
{
	if (x0 > x1)
	{
		int temp_x0 = x0;
		x0 = x1;
		x1 = temp_x0;
		int temp_y0 = y0;
		y0 = y1;
		y1 = temp_y0;
	}
	float a = ((float)(y0 - y1)) / (x0 - x1);
	float b = y0 - a * x0;
	printf("a: %d, b: %d\n\r", a, b);
	int temp_x, temp_y;
	write_pixel(x0, y0);
	write_pixel(x1, y1);
	for (int i = 1; i < x1 - x0; ++i)
	{
		temp_x = x0 + i;
		temp_y = (a * temp_x + b);
		printf("x: %d, y: %d\n\r", temp_x, temp_y);
		write_pixel(temp_x, temp_y);
	}
	
}

//draw a circle
void oled_circle(int x, int y, int r)
{
	int temp_x, temp_y;
	for (int i = -r; i <= r; ++i)
	{
		for (int j = -r; j <= r; ++j)
		{
			temp_x = i;
			temp_y = j;
			if ((temp_x * temp_x + temp_y * temp_y) == (r * r))
			{
				write_pixel(x + temp_x, y + temp_y);
				
			}
			else if  (((temp_x * temp_x + temp_y * temp_y) <= (r * r) + r/2) && (((temp_x * temp_x + temp_y * temp_y) >= (r * r) - r/2)))
			{
				write_pixel(x + temp_x, y + temp_y);
				
			}
				
		}
	}
}

void oled_animate_mario_large()
{
	int row = 0;
	int clm = 0;
	int i, j;
	
	//oled_reset();
	for (i = 0; i < 5; i++)
	{
		oled_print_mario_large(i,0);
		_delay_ms(400);
		oled_reset();
	}
	i--;
	for (j = 0; j < 96; j = j+3)
	{
		oled_reset();
		oled_print_mario_large(i,j);
		_delay_ms(100);	
	}	
}

void oled_print_mario_large(uint8_t row,uint8_t column)
{
	oled_pos(row, column);
	oled_print_special_char(10);
	oled_print_special_char(11);
	oled_print_special_char(12);
	oled_print_special_char(13);
	oled_pos(row+1, column);
	oled_print_special_char(14);
	oled_print_special_char(15);
	oled_print_special_char(16);
	oled_print_special_char(17);
	oled_pos(row+2, column);
	oled_print_special_char(18);
	oled_print_special_char(19);
	oled_print_special_char(20);
	oled_print_special_char(21);
	oled_pos(row+3, column);
	oled_print_special_char(22);
	oled_print_special_char(23);
	oled_print_special_char(24);
	oled_print_special_char(25);
}

void oled_test()
{
	oled_reset();
	/*oled_write_cmd(0x22);
	oled_write_cmd(0x00);
	oled_write_cmd(0x05);*/

	//oled_print_arrow(2, 64);
	oled_animate_mario_large();

	//oled_line(65, 63, 26, 12);
	//oled_circle(80,30,21);
	
	//oled_clear_line(6);
	//oled_write_cmd(0x40);
}

void oled_print_ntnu_logo(uint8_t row, uint8_t column)
{
	oled_print_1st_part_ntnu_logo(row, column);
	oled_print_2nd_part_ntnu_logo(row + 1, column);
	oled_print_3rd_part_ntnu_logo(row + 2, column);
	oled_print_4th_part_ntnu_logo(row + 3, column);	
}

void oled_print_4th_part_ntnu_logo(uint8_t row, uint8_t column)
{
	oled_pos(row, column);
	oled_print_special_char(38);
	oled_print_special_char(39);
	oled_print_special_char(40);
	oled_print_special_char(41);
}

void oled_print_3rd_part_ntnu_logo(uint8_t row, uint8_t column)
{
	oled_pos(row, column);
	oled_print_special_char(34);
	oled_print_special_char(35);
	oled_print_special_char(36);
	oled_print_special_char(37);
}

void oled_print_2nd_part_ntnu_logo(uint8_t row, uint8_t column)
{
	oled_pos(row, column);
	oled_print_special_char(30);
	oled_print_special_char(31);
	oled_print_special_char(32);
	oled_print_special_char(33);
}

void oled_print_1st_part_ntnu_logo(uint8_t row, uint8_t column)
{
	oled_pos(row, column);
	oled_print_special_char(26);
	oled_print_special_char(27);
	oled_print_special_char(28);
	oled_print_special_char(29);
}

void oled_animate_ntnu()
{
	int row = 0, column = 0;
	
	oled_reset();
	oled_print_4th_part_ntnu_logo(row, column);
	_delay_ms(200);
	
	oled_reset();
	oled_print_3rd_part_ntnu_logo(row, column);
	oled_print_4th_part_ntnu_logo(row + 1, column);
	_delay_ms(200);
	
	oled_reset();
	oled_print_2nd_part_ntnu_logo(row, column);
	oled_print_3rd_part_ntnu_logo(row + 1, column);
	oled_print_4th_part_ntnu_logo(row + 2, column);
	_delay_ms(200);
	
	oled_reset();
	oled_print_ntnu_logo(row, column);
	_delay_ms(800);
	
	
	row = 5;
	oled_pos(row, column);
	oled_print("NTNU", 3);
	_delay_ms(800);
	
	row = 4;
	column = 40;
	oled_pos(row, column);
	oled_print("CHALLENGE", 3);
	row = 5;
	oled_pos(row, column);
	oled_print("THE OCEAN", 3);
	_delay_ms(800);
	oled_pos(row, column);
	oled_print("POLLUTION", 3);
	_delay_ms(800);
	oled_pos(row, column);
	oled_print("PING PONG", 3);
	_delay_ms(1000);
	
	row = 1;
	column = 40;
	oled_pos(row, column);
	oled_print("?", 3);
	_delay_ms(10);
	column = 48;
	oled_print("?", 3);
	_delay_ms(10);
	column = 54;
	oled_print("?", 3);
	_delay_ms(10);
}

void oled_ingame_display(int score)
{
	oled_reset();
	int row = 2, column = 8;
	char text[10];
	sprintf(text, "SCORE %d\0", score);
	oled_pos(row, column);
	oled_print(text, 3);
}