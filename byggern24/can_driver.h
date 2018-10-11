#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

void can_init();
uint8_t can_send_msg(can_message msg);
can_message can_recieve_msg(uint8_t buffer);
void can_driver_test();

#endif /* CAN_DRIVER_H_ */
