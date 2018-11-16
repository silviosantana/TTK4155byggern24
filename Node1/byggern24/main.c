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
	music_init();
	printf("START ============================\n\r");
	
	int arrow_pos = 2;
	int arrow_max = 2;
	
	menu* current_state = menu_state_machine_setup(&arrow_max, arrow_pos);
	
	enum direction dir = NEUTRAL;
	enum direction old_dir = NEUTRAL;
 	oled_animate_ntnu();
	_delay_ms(1000);
	play_music(1);
 	oled_animate_mario_large();
// 	play_music(0);
// 	_delay_ms(10000);
// 	stop_music();
// 	_delay_ms(1000);
// 	play_music(1);
	
	while (1)
	{
		send_multi_board();
		_delay_ms(50);

		old_dir = dir;
		dir = get_joystick_direction();
		
		if (old_dir == NEUTRAL)
		{
			current_state = menu_move_arrow(dir, current_state, &arrow_max, &arrow_pos);
		}

		
	}

	printf("END ==============================\n\r");
}