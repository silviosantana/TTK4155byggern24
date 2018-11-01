/*
 * motor_driver.h
 *
 * Created: 01.11.2018 18:58:02
 *  Author: silviohc
 */ 


#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#define MJ1_OE PH5
#define MJ1_SEL PH3
#define MJ1_DIR PH1
#define MJ1_RST PH6
#define MJ1_EN PH5
 

void motor_init();
uint16_t motor_get_encoder();
void motor_move(Position pos);

#endif /* MOTOR_DRIVER_H_ */