#include "util.h"
#include <avr/io.h>
#include "avr/interrupt.h"

void timer_driver_init()
{
	set_bit(DDRE, PE3);
	cli();
	//TOP = 16MHz/pre-scale/50Hz
	ICR3 = 39999;
	
	//duty_cycle
	OCR3A = 3999;
	
	//Set pre-scale to 8
	clear_bit(TCCR3B, CS30);
	set_bit(TCCR3B, CS31);
	clear_bit(TCCR3B, CS32);
	
	//Set mode of operation to 14 (Fast PWM with ICRn)
	set_bit(TCCR3A, WGM31);
	clear_bit(TCCR3A, WGM30);
	TCCR3B |= (1 << WGM33) | (1 << WGM32);
	
	//Enable compare interruption on reg OCRnA
	set_bit(TCCR3A, COM3A1);
	clear_bit(TCCR3A, COM3A0);

	sei();
}

void timer_driver_set_duty_cycle(int8_t position)
{
	int duty_cycle = 20 * (100 - position) + 1999;
	//printf("%d\n\r", duty_cycle);
	
	if (duty_cycle < 1999)
		duty_cycle = 1999;
	else if (duty_cycle > 3999)
		duty_cycle = 3999;
	
	OCR3A = (uint16_t) duty_cycle;
}