#include "can_driver.h"

#include <stdint.h>

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "util.h"
#include "can_controller_driver.h"
#include "usart_driver.h"
#include "timer_driver.h"
#include "motor_driver.h"
#include "motor_controller.h"
#include "MCP2515.h"
#include "game.h"


ISR(INT4_vect)
{
	cli();
	can_message* msg = (can_message *) malloc(1*sizeof(can_message));
	//CHECK WHICH BUFFER RECIEVED MESSAGE
	//printf("message INTERRUPTION recieved\n\r");
	//print(my_buf);
	uint8_t canInt = can_controller_read(MCP_CANINTF);
	if ((canInt & MCP_RX0IF) == MCP_RX0IF)
	{
		//printf("X:\tY:\tLeft:\tRight:\tDir:\tpush:\n\r");
		//for (uint8_t byte = 0; byte < msg->length; byte++) {
		//	printf("%d \t", (int8_t) msg->data[byte]);
		//}

		can_receive_msg(0, msg);
		
		// Control servo
		timer_driver_set_duty_cycle(msg->data[3]);
		
		// Control solenoid
		control_solenoid(msg->data[5]);

		// Control Motor
		if (game_get_mode() == 1) {motor_controller_set_point(msg->data[2]);}
		else {motor_move_joystick(msg->data[4], msg->data[0]);}

	}else
	{
		can_controller_write(MCP_CANINTF, 0x00);
		free(msg);
		can_controller_write(MCP_CANINTF, 0x00);
		sei();
		return;
	}
	can_controller_write(MCP_CANINTF, 0x00);
	free(msg);
	can_controller_write(MCP_CANINTF, 0x00);
	sei();
}

void can_init ()
{
	//set loopback mode
	uint8_t mode = can_controller_set_mode(MODE_NORMAL);
	if (mode != MODE_NORMAL) {printf("Could not set normal mode! I am sorry!\n\r");}
	
	//enable RX interrupts
	//can_controller_bit_modify(MCP_CANINTE, MCP_RX_INT, MCP_RX_INT);
	//can_controller_write(MCP_CANINTE, MCP_RX_INT);
	
	//Set PL0 as output for solenoid
	set_bit(DDRL, PL0);
	
	// SET UP INTERRUPTION TO RECIEVE MESSAGE
	//DDRD &= ~(1<<PD0);
	//cli();
	clear_bit(EICRB, ISC40);
	set_bit(EICRB, ISC41); //interrupt on the falling edge
	set_bit(EIMSK, INT4); //enable external interrupt INT4
	can_controller_write(MCP_CANINTF, 0x00);
	can_controller_write(MCP_EFLG, 0x00);
	//sei();									//enable Global Interrupt
}

uint8_t can_send_msg(can_message* msg)
{
	//writhe higher id
	can_controller_write(MCP_TXB0SIDH, msg->id);
	
	// Define data length of message
	uint8_t data_length = msg->length;
	can_controller_write(MCP_TXB0DLC, msg->length);
	
	// Write data bytes to transmit buffer
	uint8_t* data_bytes = msg->data;
	for (uint8_t byte = 0; byte < data_length; byte++) {
		can_controller_write(MCP_TXB0Dm + byte, data_bytes[byte]);
	}

	// Request to send message, send if successful
	can_controller_request_to_send();
	_delay_ms(50);
}

// CALLED WHEN  AN INTERRUPTION OCCURS
void can_receive_msg(uint8_t buffer, can_message* msg)
{

	//READ RX BUFFER - save ID on RXBnSIDH to Message.id (2 TIMES)
	uint8_t id_high = can_controller_read(MCP_RXB0SIDH + buffer*16);
	uint8_t mask;


	uint8_t data_length = can_controller_read(MCP_RXB0DLC + buffer*16);
	mask = 0x0F;
	msg->length = (data_length & mask);

	//READ RX BUFFER - save DATA RXBnDm to Message.data (8 TIMES)
	for (uint8_t byte = 0; byte < data_length; byte++) {
		msg->data[byte] = can_controller_read(MCP_RXB0Dm + buffer*16 + byte);
	}
}

void can_driver_test()
{
	can_message msg;
	printf("enter can driver test \n\r");
	msg.id = 1;
	msg.data[0] = 25;
	msg.data[1] = 38;
	msg.data[2] = 95;
	msg.length = 3;
	can_send_msg(&msg);
	
	_delay_ms(100);
	
	msg.id = 5;
	msg.data[0] = 72;
	msg.data[1] = 58;

	msg.length = 2;
	can_send_msg(&msg);
}
