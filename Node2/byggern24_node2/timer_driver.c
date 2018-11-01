#include "util.h"
#include <avr/io.h>
#include "avr/interrupt.h"

void timer_driver_init()
{
	set_bit(DDRB, PB5);
	cli();
	//TOP = 16MHz/pre-scale/50Hz
	ICR1 = 39999;
	
	//duty_cycle
	OCR1A = 3999;
	
	//Set pre-scale to 8
	clear_bit(TCCR1B, CS10);
	set_bit(TCCR1B, CS11);
	clear_bit(TCCR1B, CS12);
	
	//Set mode of operation to 14 (Fast PWM with ICRn)
	set_bit(TCCR1A, WGM11);
	clear_bit(TCCR1A, WGM10);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	
	//Enable compare interruption on reg OCRnA
	set_bit(TCCR1A, COM1A1);
	clear_bit(TCCR1A, COM1A0);

	sei();
}

void timer_driver_set_duty_cycle(int8_t position)
{
	int duty_cycle = 10 * position + 2999;
	
	if (duty_cycle < 1999)
		duty_cycle = 1999;
	else if (duty_cycle > 3999)
		duty_cycle = 3999;
	
	OCR1A = (uint16_t) duty_cycle;
}