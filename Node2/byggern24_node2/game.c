#include "game.h"

#include "avr/io.h"
#include "util/delay.h"
#include "util.h"
#include "adc_driver.h"
#include "motor_controller.h"

int game_mode;

void set_game_mode(int gm)
{
	game_mode = gm;
}

void game_toggle_mode()
{
	if (game_mode == 2)
	{
		game_mode = 0;
	}else
	{
		game_mode = 2;
	}
	
}

int game_get_mode()
{
	return game_mode;
}

uint16_t record_score(uint16_t score)
{
	uint16_t score_interval = 0;
	uint16_t adc = adc_read(0);
		
	if (adc < 300)
	{
		score += 1;
		score_interval = 1;
		_delay_ms(1000);
	}
	
		
	while(score_interval == 1)
	{
		if (adc_read(0) > 300)
		{
			score_interval = 0;
			break;
		}
	}
	return score;
}