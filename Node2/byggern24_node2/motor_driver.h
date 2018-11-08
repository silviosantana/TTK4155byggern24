/*
 * motor_driver.h
 *
 * Created: 01.11.2018 18:58:02
 *  Author: silviohc
 */ 

#include <avr/io.h>

#include "util.h"

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#define MJ1_OE PH5
#define MJ1_SEL PH3
#define MJ1_DIR PH1
#define MJ1_RST PH6
#define MJ1_EN PH5
 

void motor_init();
uint16_t motor_get_encoder();
void motor_move(int dir, int8_t speed);
void control_solenoid(uint8_t push);

#endif /* MOTOR_DRIVER_H_ */