#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#include <stdint.h>

#include "util.h"

void can_init();

uint8_t can_send_msg(can_message* msg);

void can_recieve_msg(uint8_t buffer, can_message* msg);

void can_driver_test();

#endif
