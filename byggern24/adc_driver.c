#include "stdint.h"
#include "sram_driver.h"
#include "avr/io.h"
#include "util/delay.h"


void ADC_init ()
{
	//Enable interruption
	GICR |= (1 << INT1);
}

unsigned int ADC_read (unsigned int chn)
{
	
	if (chn < 1 && chn > 4)
	{
		return 0;
	} else 
	{
		volatile char *ext_adc = (char *) 0x1400; // Start address for the SRAM
		uint8_t val = chn + 3;
		
		ext_adc[0] = val;
		_delay_us(200);
		return ext_adc[0];
	}
	

}