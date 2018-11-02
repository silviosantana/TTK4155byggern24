#include "game.h"

#include <avr/io.h>
#include "util/delay.h"

#include "util.h"
#include "adc_driver.h"

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