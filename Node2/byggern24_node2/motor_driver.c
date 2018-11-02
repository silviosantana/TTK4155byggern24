#include "motor_driver.h"

#include <avr/io.h>

#include "util.h"

void motor_init()
{
	set_bit(DDRH, MJ1_OE);
	set_bit(DDRH, MJ1_SEL);
	set_bit(DDRH, MJ1_DIR);
	set_bit(DDRH, MJ1_RST);
	set_bit(DDRH, MJ1_EN);
	
	clear_bit(DDRK, PK0);
	clear_bit(DDRK, PK1);
	clear_bit(DDRK, PK2);
	clear_bit(DDRK, PK3);
	clear_bit(DDRK, PK4);
	clear_bit(DDRK, PK5);
	clear_bit(DDRK, PK6);
	clear_bit(DDRK, PK7);
	
	
	
}

void motor_move(int dir, uint8_t speed)
{
	set_bit(PORTH, MJ1_EN);
	
	if (dir == LEFT)
	{
		set_bit(PORTH, MJ1_DIR);
	}else if (dir == RIGHT)
	{
		clear_bit(PORTH, MJ1_DIR);
	}
	
	//set speed
	
}
