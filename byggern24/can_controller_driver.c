#include "stdio.h"

#include "avr/io.h"
#include "can_controller_driver.h"
#include "spi_driver.h"

#define CAN_CS 4

void can_controller_reset()
{
	PORTB &= ~(1<<CAN_CS); // Select CAN-controller
	spi_send(MCP_RESET);
	PORTB |= (1<<CAN_CS); // Deselect CAN-controller
}

uint8_t can_controller_init()
{
    uint8_t value;

    spi_init(); // Initialize SPI
    can_controller_reset(); // Send reset-command
	

    // Self-test
    value = can_controller_read(MCP_CANSTAT);
    if ((value & MODE_MASK)  != MODE_CONFIG) 
    {
        printf("%d can_controller is NOT in configuration mode after reset!\n\r", value);
        return 1;
    }

    // More initialization
    return 0;
}

uint8_t can_controller_read(uint8_t address)
{
	uint8_t result;

	PORTB &= ~(1<<CAN_CS); // Select CAN-controller

	spi_send(MCP_READ); // Send read instruction
	spi_send(address); // Send address
	result = spi_read(); // Read result

	PORTB |= (1<<CAN_CS); // Deselect CAN-controller

	return result;
}

uint8_t can_controller_set_mode(uint8_t mode)
{
	uint8_t modestat;
	can_controller_bit_modify(0x0F, MODE_MASK, mode);
	modestat = can_controller_read(MCP_CANSTAT);
	return (modestat & MODE_MASK);
}

void can_controller_request_to_send()
{
		PORTB &= ~(1<<CAN_CS);
		
		// The buffer types are MCP_RTS_TX0, MCP_RTS_TX1, MCP_RTS_TX2 and MCP_RTS_ALL
		spi_send(MCP_RTS_TX0);

		PORTB |= ~(1<<CAN_CS);
}

void can_controller_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
		PORTB &= ~(1<<CAN_CS); // Select CAN-controller

		spi_send(MCP_BITMOD); // Send read instruction
		spi_send(address); // Send address
		spi_send(mask); // Send mask,
		spi_send(data); // Send data

		PORTB |= (1<<CAN_CS); // Deselect CAN-controller
}

uint8_t can_controller_read_status()
{
	uint8_t status;

	PORTB &= ~(1<<CAN_CS);
	
	spi_send(MCP_CANSTAT);
	status = spi_read();

	PORTB |= ~(1<<CAN_CS);

	return status;
}

void can_controller_write(uint8_t address, uint8_t data)
{
	PORTB &= ~(1<<CAN_CS);
	
	spi_send(MCP_WRITE);
	spi_send(address);
	spi_send(data);

	PORTB |= ~(1<<CAN_CS);
}

void can_controller_load_ID_to_buffer(uint8_t buffer, uint8_t* id)
{
	PORTB &= ~(1 << CAN_CS);

	switch (buffer)
	{
	case 0:
		spi_send(MCP_LOAD_TX0);
		spi_send(id[0]);
		spi_send(id[1]);
		break;
	case 1:
		spi_send(MCP_LOAD_TX1);
		spi_send(id[0]);
		spi_send(id[1]);
		break;
	case 2:
		spi_send(MCP_LOAD_TX2);
		spi_send(id[0]);
		spi_send(id[1]);
		break;
	}

	PORTB |= ~(1 << CAN_CS);
}

void can_controller_load_data_to_buffer(uint8_t buffer, uint8_t* data)
{
	PORTB &= ~(1 << CAN_CS);

	switch (buffer)
	{
	case 0:
		spi_send(MCP_LOAD_TX0 + 1);
		for (int i = 0; i < 8; ++i)
			spi_send(data[i]);
		break;
	case 1:
		spi_send(MCP_LOAD_TX1 + 1);
		for (int i = 0; i < 8; ++i)
			spi_send(data[i]);
		break;
	case 2:
		spi_send(MCP_LOAD_TX2 + 1);
		for (int i = 0; i < 8; ++i)
			spi_send(data[i]);		
		break;
	}

	PORTB |= ~(1 << CAN_CS);
}

void can_controller_test()
{

	printf("mode value: %d", can_controller_set_mode(MODE_LOOPBACK));
}
