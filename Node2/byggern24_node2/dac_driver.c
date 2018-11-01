#include "TWI_Master.h"

dac_init()
{
	TWI_Master_Initialise();	
}

dac_driver_send(uint8_t addr, uint8_t command, uint8_t data)
{
	//DAC address 01010000;
	
}
