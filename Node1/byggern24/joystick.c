#include "joystick.h"

#include "util.h"
#include "avr/io.h"
#include "util/delay.h"
#include "adc_driver.h"

static int x_middle_pos;
static int y_middle_pos;

void joystick_init()
{
	x_middle_pos = ADC_read(2);
	y_middle_pos = ADC_read(1);
}

struct Position get_joystick_position()
{
	struct Position j_position;
	
	float x_step = (255.0 - x_middle_pos)/100.0;
	float y_step = (255.0 - y_middle_pos)/100.0;
	
	int x_range = ADC_read(2);
	int y_range = ADC_read(1);

	j_position.x = (int) ((x_range - x_middle_pos)/x_step);
	j_position.y = (int) ((y_range - y_middle_pos)/y_step);
	
	return j_position;
}

int get_joystick_direction(){
	struct Position j_pos;
	
	j_pos = get_joystick_position();
	
	if (j_pos.x >= -10 && j_pos.x <= 10 && j_pos.y >= -10 && j_pos.y <= 10)
	{
		return NEUTRAL;
	} else if (j_pos.x < -10 && abs(j_pos.x) >= abs(j_pos.y))
	{
		return LEFT;
	} else if (j_pos.x > 10 && j_pos.x >= abs(j_pos.y))
	{
		return RIGHT;
	} else if (j_pos.y < -10 && abs(j_pos.y) > abs(j_pos.x))
	{
		return DOWN;
	}else 
	{
		return UP;
	}
}

int get_left_slider()
{
	int range = ADC_read(3);
	
	return (int) ((255 - range)/2.55);
}

int get_right_slider()
{
	int range = ADC_read(4);
	
	return (int) (range/2.55);
}

int get_joystick_right_button()
{
	uint8_t push = (PINB & 0b00000001);
	
	return push;
}

int get_joystick_left_button()
{
	uint8_t push = (PINB & 0b00000010);
	
	return push;
}

void joystick_test()
{
	struct Position j_pos;
	uint8_t sliderL;
	uint8_t sliderR = 0;
	
	while(1)
	{
		_delay_ms(500);
		j_pos = get_joystick_position();
		sliderL = get_left_slider();
		sliderR = get_right_slider();
		direction dir = get_joystick_direction();
		printf("\r\nX: %d\t|\tY: %d\t|\tSL: %d\t|\tSR: %d |\tdir: %d", j_pos.x, j_pos.y, sliderL, sliderR, dir);
	}
}
