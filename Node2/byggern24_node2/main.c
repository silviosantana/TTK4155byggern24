#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "util.h"
#include "usart_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"
#include "timer_driver.h";
#include "adc_driver.h";
#include "game.h";
#include "dac_driver.h"
#include "TWI_Master.h";



int main(void)
{
//	cli();
	USART_Init(MYUBRR);
	spi_init();
	can_controller_init();
	can_init();
	timer_driver_init();
	adc_init();
	dac_init();
//	sei();
	printf("Hello from node 2!\n\r");
	
	//uint16_t score = 0;
	while (1)
	{
// 		score = record_score(score);
// 		printf("ADC read: %d\t Score: %d\n\r", adc_read(0), score);
// 		
 		//dac_driver_send(255);
		_delay_ms(500);
		printf("RUUUUNNNN\n\r");
		
	}

}