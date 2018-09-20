#include "stdio.h"
#include "avr/io.h"

#include "util.h"
#include "usart_driver.h"
#include "sram_driver.h"
#include "adc_driver.h"
#include "joystick.h"

#define F_CPU 4915200 // clock frequency in Hz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include "util/delay.h"


void test_LED()
{
	DDRA = 0xFF;
	// LED on
	PORTA = 0b00000001;            // PC0 = High = Vcc
	_delay_ms(1000);                // wait 500 milliseconds
		
	//LED off
	PORTA = 0b00000000;            // PC0 = Low = 0v
	_delay_ms(1000);                // wait 500 milliseconds
}

void test_transmit_receive()
{
	unsigned char test;
	printf("Your voice is my command");
	test = USART_Receive();
	USART_Transmit(test);
}

void test_latch()
{
	// LED on
	PORTA = 0b11111111;            // PC0 = High = Vcc
	_delay_ms(1000);                // wait 500 milliseconds
	
	//LED off
	PORTA = 0b00000000;            // PC0 = Low = 0v
	_delay_ms(1000);                // wait 500 milliseconds
}

int main(void)
{
	USART_Init(MYUBRR);
	SRAM_init();
	joystick_init();
	//SRAM_test();
	
	struct Position j_pos;
	uint8_t sliderL; 
	uint8_t sliderR = 0;
	
	while(1)
	{
		_delay_ms(500);
		j_pos = get_joystick_position();
		sliderL = get_left_slider();
		sliderR = get_right_slider();
		enum direction dir = get_joystick_direction();
		printf("\r\nX: %d\t|\tY: %d\t|\tSL: %d\t|\tSR: %d |\tdir: %d", j_pos.x, j_pos.y, sliderL, sliderR, dir);
		
		
	}
}