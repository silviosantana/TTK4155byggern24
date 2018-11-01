#include <avr/io.h>
#include <util/delay.h>

#include "util.h"
#include "usart_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"
#include "timer_driver.h";
#include "adc_driver.h";
#include "game.h";
#include "TWI_Master.h";

int main(void)
{
	USART_Init(MYUBRR);
	spi_init();
	can_controller_init();
	can_init();
	timer_driver_init();
	adc_init();
	TWI_Master_Initialise();
	
	printf("Hello from node 2!\n\r");

	uint16_t score = 0;
	while (1)
	{
		score = record_score(score);
		printf("ADC read: %d\t Score: %d\n\r", adc_read(0), score);
	}

}