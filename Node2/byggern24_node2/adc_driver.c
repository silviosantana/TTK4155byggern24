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
	
	// select the channel 0
	//ADMUX &= ~(1 << MUX4) & ~(1 << MUX3) & ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);     // clears the bottom 3 bits before ORing
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
	 ch &= 0b00000111;  // AND operation with 7
	 ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single conversion
	// write '1' to ADSC
	set_bit(ADCSRA, ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	//while(ADCSRA & (1<<ADSC));
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