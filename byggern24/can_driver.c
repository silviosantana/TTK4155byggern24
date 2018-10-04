#include "util.h"
#include "can_driver.h"


void can_init ()
{
	//init can controller
	//set loopback mode
	// SET UP INTERRUPTION TO RECIEVE MESSAGE
	
}


uint8_t can_send_msg(Message msg, uint8_t buffer)
{
	// LOAD TX BUFFER - load ID TXBnSIDH (2 TIMES)
	// LOAD TX BUFFER - load DATA TXBnDm (msg.length TIMES)
	// REQUEST SEND BUFFER n - buffer
	// return successful (or not)
}


// CALLED WHEN  AN INTERRUPTION OCCURS
Message can_recieve_msg(uint8_t buffer)
{
	//READ RX BUFFER - save ID on RXBnSIDH to Message.id (2 TIMES)
	//READ RX BUFFER - save DATA RXBnDm to Message.data (8 TIMES)
	
}