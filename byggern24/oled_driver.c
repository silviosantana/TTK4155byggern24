#include "oled_driver.h"
#include "avr/io.h"

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

void oled_reset()
{
	oled_goto_line(0);
	for(int i = 0; i < 1024; i++)
	{
		oled_write_data(0x00);
	}
	
}

void oled_home()
{
	
}

void oled_goto_line(uint8_t line)
{
	oled_write_cmd(0x22);
	oled_write_cmd(line);
	oled_write_cmd(7);
}

void oled_goto_column(uint8_t column)
{
	oled_write_cmd(0x21);
	oled_write_cmd(column);
	oled_write_cmd(127);
}

void oled_clear_line(uint8_t line)
{
	oled_goto_line(line);
	for (int i = 0; i < 128; i++)
	{
		oled_write_data(0x00);
	}
}

void oled_pos(uint8_t row,uint8_t column)
{
	oled_goto_line(row);
	oled_goto_column(column);
}

void oled_print(char* name)
{
	
}

void oled_test(){

	oled_reset();
	/*oled_write_cmd(0x22);
	oled_write_cmd(0x00);
	oled_write_cmd(0x05);*/
	
	//oled_goto_line(4);
	//oled_goto_column(39);

	for(int i = 0; i < 1024; i++)
	{
		oled_write_data(0xff);
	}
	
	oled_clear_line(6);
	//oled_write_cmd(0x40);

	
}