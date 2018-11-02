#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "TWI_Master.h"

#include "util/delay.h"
#include "util.h"

void dac_init()
{
	//reset ADC and put it to normal mode
	//uint8_t cmd = 0b00010000;
	TWI_Master_Initialise();
	sei();
	//set SCL and SDA as outputs
 	set_bit(DDRD, PD0);
 	set_bit(DDRD, PD1);	
	//dac_driver_send(cmd, 0, 0);
}

void dac_driver_send(uint8_t data)
{
	//DAC address + rd/!wr 01010000;
	uint8_t addr = 0b01010000;
	uint8_t msg[3];
	
	msg[0] = addr;
	msg[1] = 0x00;
	msg[2] = data;
	
	TWI_Start_Transceiver_With_Data( msg, 3);
	
// 	if (has_data == 1)
// 	{
// 		TWI_Start_Transceiver_With_Data( msg, 3);
// 	}else
// 	{
// 		TWI_Start_Transceiver_With_Data( msg, 2);
// 	}

}

