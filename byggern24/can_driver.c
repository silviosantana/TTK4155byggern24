#include "util.h"
#include "can_driver.h"
#include "can_controller_driver.h"


void can_init ()
{
	//init can controller
	//set loopback mode
	// SET UP INTERRUPTION TO RECIEVE MESSAGE
	
}


uint8_t can_send_msg(can_message msg)
{
    unsigned int id = msg.id;
    char id_high = id / 8;
    char id_low = id % 8;
    id_low = id_low * 0b1000000;
    can_controller_write(MCP_TXB0SIDH, id_high);
    can_controller_write(MCP_TXB0SIDL, id_low);

    
	// Define data length of message
	uint8_t data_length = msg.length;
	can_controller_write(MCP_TXB0DLC, data_length);
	
	// Write data bytes to transmit buffer
	char* data_bytes = msg.data;
	for (uint8_t byte = 0; byte < data_length; byte++) {
		can_controller_write(MCP_TXB0Dm + byte, data_bytes[byte]);
	}
	
	// Request to send message, send if successful
    can_controller_request_to_send();
}


// CALLED WHEN  AN INTERRUPTION OCCURS
can_message can_recieve_msg(can_message msg)
{
	//READ RX BUFFER - save ID on RXBnSIDH to Message.id (2 TIMES)
    uint8_t id_high = can_controller_read(MCP_RXB0SIDH);
	uint8_t id_low = can_controller_read(MCP_RXB0SIDL);
	uint8_t mask = 0b11100000;
	id_low = (id_low & mask);
	msg.id = 0b1000*id_high + id_low/0b100000;

	uint8_t data_length = can_controller_read(MCP_RXB0DLC);
	mask = 0b1111;
	msg.length = (data_length & mask);

	//READ RX BUFFER - save DATA RXBnDm to Message.data (8 TIMES)	
	for (uint8_t byte = 0; byte < data_length; byte++) {
		msg.data[byte] = can_controller_read(MCP_RXB0Dm + byte);
    }
}
