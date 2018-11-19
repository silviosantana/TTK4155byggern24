#include "motor_driver.h"

#include <avr/io.h>
#include <util/delay.h>

#include "util.h"

void motor_reset_encoder()
{
	clear_bit(PORTH, MJ1_RST);
	_delay_us(50);
	set_bit(PORTH, MJ1_RST);
}

void motor_init()
{
	//set MJ1 motor input as MCU output
	set_bit(DDRH, MJ1_OE);		//Enable output of encoder
	set_bit(DDRH, MJ1_SEL);		//Get high byte
	set_bit(DDRH, MJ1_DIR);		//Set direction
	set_bit(DDRH, MJ1_RST);		//Reset the counter
	set_bit(DDRH, MJ1_EN);		//Enable motor
	
	//set MJ2 motor output as MCU input
	clear_bit(DDRK, PK0);
	clear_bit(DDRK, PK1);
	clear_bit(DDRK, PK2);
	clear_bit(DDRK, PK3);
	clear_bit(DDRK, PK4);
	clear_bit(DDRK, PK5);
	clear_bit(DDRK, PK6);
	clear_bit(DDRK, PK7);
	
	//set MJ1 !OE to one - active low 
	set_bit(PORTH, MJ1_OE);
	
	motor_reset_encoder();
	
}


void motor_set_speed(uint8_t speed)
{
	//send speed through DAC/i2C
	dac_driver_send(speed);
}

uint16_t motor_get_encoder()
{
	uint16_t encod_counter;
	uint8_t LSB_counter;
	
	clear_bit(PORTH, MJ1_OE);
	clear_bit(PORTH, MJ1_SEL);
	_delay_us(20);
	encod_counter = PINK;
	
	set_bit(PORTH, MJ1_SEL);
	_delay_ms(20);
	LSB_counter = PINK;
	
	set_bit(PORTH, MJ1_OE);
	//motor_reset_encoder();
	
	//8-bit left shift
	encod_counter = encod_counter*256;
	//add LSB
	encod_counter += LSB_counter;
	
	return encod_counter;
}

void motor_driver_test()
{
	control_solenoid(1);
	_delay_ms(20);
	control_solenoid(0);
	enum direction dir = RIGHT;
	uint8_t speed = 70;
	uint16_t encoder_motor;
	
	encoder_motor = motor_get_encoder();
	printf("MOTOR COUNTER: %d \n\r", encoder_motor);
	
	motor_move(dir, speed);
	encoder_motor = motor_get_encoder();
	printf("MOTOR COUNTER: %d \n\r", encoder_motor);
	
	_delay_ms(500);
	control_solenoid(1);
	_delay_ms(20);
	control_solenoid(0);
	dir = LEFT;
	motor_move(dir, speed);
	encoder_motor = motor_get_encoder();
	printf("MOTOR COUNTER: %d \n\r", encoder_motor);
}

void motor_move_joystick(int dir, int8_t speed)
{
	//set motor EN to 1
	set_bit(PORTH, MJ1_EN);
	

	if (dir == RIGHT)
	{
		set_bit(PORTH, MJ1_DIR);
	}else if (dir == LEFT)
	{
		clear_bit(PORTH, MJ1_DIR);
	}

	uint8_t speed_converted = (uint8_t) abs((int)(speed*1.3));
	//printf("DIR: %d | SPEED: %d | SP_conv: %d\n\r", dir, speed, speed_converted);
	motor_set_speed(speed_converted);
}

void motor_move(int dir, uint8_t speed)
{
	//set motor EN to 1
	set_bit(PORTH, MJ1_EN);

	if (dir == RIGHT)
	{
		set_bit(PORTH, MJ1_DIR);
	}else if (dir == LEFT)
	{
		clear_bit(PORTH, MJ1_DIR);
	}

	//set speed
	motor_set_speed(speed);
}

void control_solenoid(uint8_t push)
{
	if (push == 1)
	{
		clear_bit(PORTL, PL0);
	}
	else
	{
		set_bit(PORTL, PL0);
	}
}