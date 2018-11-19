#include "adc_driver.h"

#include "avr/io.h"
#include "util/delay.h"
#include "util.h"

void adc_init ()
{
	// AREF = AVcc

	set_bit(ADMUX, REFS0);
	clear_bit(ADMUX, REFS1);
	clear_bit(ADMUX, ADLAR);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
	 ch &= 0b00000111;  // AND operation with 7
	 ADMUX = (ADMUX & 0xF8)|ch;
	
	// start single conversion
	set_bit(ADCSRA, ADSC);

	loop_until_bit_is_clear( ADCSRA, ADSC );
	
	return (ADC);
}

void adc_driver_test ()
{
	uint16_t adc;
	while (1)
	{
		adc = adc_read(0);
		printf("ADC read: %d\n\r", adc);
	}
}