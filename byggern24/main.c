#include "stdio.h"
#include "avr/io.h"
#include "util/delay.h"

#include "usart_driver.h"
#include "sram_driver.h"

#define F_CPU 4915200 // clock frequency in Hz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


void test_LED()
{
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
	DDRA = 0xFF;
	SRAM_init();
	SRAM_test();
	
 	//uint16_t add = 1;
 	//uint8_t val = 50;
	
	//SRAM_write(add, val);
	//uint8_t read = SRAM_read(add);
	//volatile char *ext_ram = (char *) 0x1880; // Start address for the SRAM
	//ext_ram[0] = val;
	//unsigned int read = ext_ram[0];
	//printf("\r\nRECIEVED: %d", read);
	
	//uint8_t val = 147;
	//volatile char *ext_ram = (char *) 0x14FD; // Start address for the SRAM
	//ext_ram[0] = val;
	//unsigned int read = ext_ram[0];
	//printf("\r\nRECIEVED: %d", read);
	while(1)
	{
	}
}