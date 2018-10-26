#include "util.h"
#include <avr/io.h>
#include "avr/interrupt.h"

void timer_driver_init()
{
	DDRB |= (1 << PB5);
	cli();
	//TOP = 16MHz/pre-scale/50Hz
	ICR1 = 39999;
	
	//duty_cycle
	OCR1A = 3999;
	
	//Set pre-scale to 8
	TCCR1B &= ~(1 << CS10);
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12);
	
	//Set mode of operation to 14 (Fast PWM with ICRn)
	TCCR1A |= (1 << WGM11);
	TCCR1A &= ~(1 << WGM10);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	
	//Enable compare interruption on reg OCRnA
	TCCR1A |= (1 << COM1A1);
	TCCR1A &= ~(1 << COM1A0);

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