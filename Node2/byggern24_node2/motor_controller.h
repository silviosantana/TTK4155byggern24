/*
 * motor_controller.h
 *
 * Created: 08.11.2018 18:45:28
 *  Author: chrisgsk
 */ 
#include "util.h"

#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

void motor_controller_init();
void motor_controller();
void motor_controller_set_point(uint16_t sp);

#endif /* MOTOR_CONTROLLER_H_ */