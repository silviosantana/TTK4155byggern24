/*
 * can_driver.h
 *
 * Created: 04.10.2018 19:19:58
 *  Author: silviohc
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

void can_init();
uint8_t can_send_msg(Message msg, uint8_t buffer);
Message can_recieve_msg(uint8_t buffer);

#endif /* CAN_DRIVER_H_ */