#ifndef CAN_CONTROLLER_DRIVER_H_
#define CAN_CONTROLLER_DRIVER_H_

#include <stdint.h>

uint8_t can_controller_init();
uint8_t can_controller_read(uint8_t address);
void can_controller_write(uint8_t address, uint8_t data);
void can_controller_request_to_send();
uint8_t can_controller_read_status();
void can_controller_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void can_controller_reset();
uint8_t can_controller_set_mode(uint8_t mode);
void can_controller_load_ID_to_buffer(uint8_t buffer, uint8_t* id);
void can_controller_load_data_to_buffer(uint8_t buffer, uint8_t* data);

#endif // CAN_CONTROLLER_DRIVER_H_
