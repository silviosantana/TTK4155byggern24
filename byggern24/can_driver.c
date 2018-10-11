#include "util.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "can_driver.h"
#include "can_controller_driver.h"


#include "util/delay.h"

ISR(INT1_vect)
{
	cli();
	can_message msg;
	//CHECK WHICH BUFFER RECIEVED MESSAGE
	printf("message INTERRUPTION recieved\n\r");
	uint8_t canInt = can_controller_read(MCP_CANINTF);
	if ((canInt & MCP_RX0IF) == MCP_RX0IF)
	{
		can_controller_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
		msg = can_recieve_msg(0);
		can_controller_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
		printf("DATA ON RX0\n\r");
		for (uint8_t byte = 0; byte < msg.length; byte++) {
			printf("%d \n\r", msg.data[byte]);
		}
		
		
	}else if ((canInt & MCP_RX1IF) == MCP_RX1IF)
	{
		can_controller_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
		msg = can_recieve_msg(1);
		
		printf("DATA ON RX1\n\r");
		for (uint8_t byte = 0; byte < msg.length; byte++) {
			printf("%d \n\r", msg.data[byte]);
		}
	}else
	{
		printf("No message available on the CAN REX BUFFERs\n\r");
		return;
	}
	
	printf("Done\n\r");
	

	sei();
	// 	for (uint8_t byte = 0; byte < msg.length; byte++) {
	// 		printf("%d \n\r", msg.data[byte]);
	// 	}
}


void can_init ()
{
	can_controller_init();
	
	//set loopback mode
	uint8_t mode = can_controller_set_mode(MODE_LOOPBACK);
	if (mode != MODE_LOOPBACK)
	{
		printf("Could not set loopback mode! I am sorry!\n\r");
	}
		
	//enable RX interrupts
	can_controller_bit_modify(MCP_CANINTE, MCP_RX_INT, MCP_RX_INT);
	//can_controller_write(MCP_CANINTE, MCP_RX_INT);
	
	// SET UP INTERRUPTION TO RECIEVE MESSAGE
	DDRD &= ~(1<<PD3);
	cli();
	GICR |= (1 << INT1);						//enable external interrupt INT1
	MCUCR &= ~(1 << ISC11);
	MCUCR &= ~(1 << ISC10);	//interrupt on the falling edge
	sei();									//enable Global Interrupt
}


uint8_t can_send_msg(can_message msg)
{


	//writhe higher id
	can_controller_write(MCP_TXB0SIDH, msg.id);
    
	// Define data length of message
	uint8_t data_length = msg.length;
	can_controller_write(MCP_TXB0DLC, msg.length);
	
	// Write data bytes to transmit buffer
	uint8_t* data_bytes = msg.data;
	for (uint8_t byte = 0; byte < data_length; byte++) {
		can_controller_write(MCP_TXB0Dm + byte, data_bytes[byte]);
	}

	// Request to send message, send if successful
	printf("request to send\n\r");
	can_controller_request_to_send();
	
	_delay_ms(300);
	
	printf("TXCTRL %02x\n\r", can_controller_read(0x30));
	printf("TXCTRL %02x\n\r", can_controller_read(0x30));
	printf("MCP_CANINTE %02x\n\r", can_controller_read(MCP_CANINTE)); 
	printf("CANINTF %02x\n\r", can_controller_read(MCP_CANINTF)); 
	printf("EFLG %02x\n\r", can_controller_read(MCP_EFLG));
	printf("MCP_RXB0CTRL %02x\n\r", can_controller_read(MCP_RXB0CTRL));
	
	if ((can_controller_read(MCP_CANINTF) & 0x04) == 4)
	{
		can_controller_bit_modify(MCP_CANINTF, 0x04, 0x00);
	}
}


// CALLED WHEN  AN INTERRUPTION OCCURS

can_message can_recieve_msg(uint8_t buffer)
{
	can_message msg;
	//READ RX BUFFER - save ID on RXBnSIDH to Message.id (2 TIMES)
    uint8_t id_high = can_controller_read(MCP_RXB0SIDH + buffer*16);
	uint8_t mask;


	uint8_t data_length = can_controller_read(MCP_RXB0DLC + buffer*16);
	mask = 0x0F;
	msg.length = (data_length & mask);

	//READ RX BUFFER - save DATA RXBnDm to Message.data (8 TIMES)	
	for (uint8_t byte = 0; byte < data_length; byte++) {
		msg.data[byte] = can_controller_read(MCP_RXB0Dm + buffer*16 + byte);
    }
	

	return msg;
}


void can_driver_test()
{
	can_message msg;
	printf("enter can driver test \n\r");
	msg.id = 1;
	msg.data[0] = 254;
	msg.data[1] = 78;
	msg.data[2] = 2;
	msg.length = 3;
	can_send_msg(msg);
	
	msg.id = 5;
	msg.data[0] = 25;
	msg.data[1] = 52;

	msg.length = 2;
	can_send_msg(msg);
}

