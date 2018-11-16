#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "util.h"
#include "usart_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"
#include "timer_driver.h";
#include "adc_driver.h";
#include "game.h";
#include "dac_driver.h"
#include "TWI_Master.h";
#include "motor_driver.h";
#include "motor_controller.h"

int main(void)
{
	// Initializations
	cli();
	USART_Init(MYUBRR);
	spi_init();
	timer_driver_init();
	adc_init();
	dac_init();
	motor_init();
	can_controller_init();
	can_init();					// Establish comm between nodes
	game_init();				// Establish comm between nodes
	motor_controller_init();
	sei();
	
	printf("Hello from node 2!\n\r");
	
	uint16_t score = 0, prev_score = 0;
	//uint16_t value = 0;
	
	while (1)
	{
		score = record_score(score);
		if (score != prev_score)
		{
			uint8_t score_msg = (uint8_t) score; 
			can_message score_can = {1, 1, score_msg};
			can_send_msg(&score_can);
			prev_score = score;
		}
		//printf("ADC read: %d\t Score: %d\n\r", adc_read(0), score);
		
		//motor_driver_test();
		//value = value + 1;
 		//dac_driver_send(value);
 		//printf("%d\n\r", value);
		_delay_ms(100);
		
	}

}