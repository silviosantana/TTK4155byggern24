#include "adc_driver.h"

#include <stdint.h>

#include "util.h"
#include "avr/io.h"
#include "util/delay.h"
#include "sram_driver.h"


void ADC_init ()
{
	//Enable interruption
	set_bit(GICR, INT1);
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