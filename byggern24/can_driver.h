#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

typedef struct {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message;

void can_init();
uint8_t can_send_msg(can_message msg);

can_message can_recieve_msg(uint8_t buffer);
void can_driver_test();
can_message can_recieve_msg();


#endif /* CAN_DRIVER_H_ */
