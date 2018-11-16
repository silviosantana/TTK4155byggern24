#include <stdio.h>

#include "util.h"
#include "avr/io.h"
#include "util/delay.h"
#include "usart_driver.h"
#include "sram_driver.h"
#include "adc_driver.h"
#include "joystick.h"
#include "oled_driver.h"
#include "spi_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"
#include "game.h"

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

int main(void)
{
	// Initializations
	USART_Init(MYUBRR);
	SRAM_init();
	joystick_init();
	oled_init();
	spi_init();
	can_controller_init();
	can_init();
	
	printf("START ============================\n\r");
	
	int arrow_pos = 2;
	int arrow_max = 2;
	menu current_state;
	menu_state_machine_setup(&current_state, &arrow_max, &arrow_pos);
	
	direction dir = NEUTRAL;
	direction old_dir = NEUTRAL;
	
	while (1)
	{	
		menu_state_machine(&current_state, &arrow_pos, &arrow_max, &old_dir, &dir);
	}
	printf("END ==============================\n\r");
}