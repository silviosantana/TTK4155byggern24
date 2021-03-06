#include "sram_driver.h"

#include <stdint.h>

#include "util.h"
#include "avr/io.h" 

void SRAM_init()
{
	//Enable XMEM
	set_bit(MCUCR, SRE);
	
	//Mask JTAG pins
	SFIOR |= (1 << XMM2) | (0 << XMM1) | (0 << XMM0);
}

void SRAM_write(uint16_t address, uint8_t data)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	ext_ram[address] = data;
}

uint8_t SRAM_read(uint16_t address)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	return ext_ram[address];
}

void SRAM_test()
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	
	// rand() stores some internal state, so calling this function in a loop will yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		
		//printf("ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
		
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}