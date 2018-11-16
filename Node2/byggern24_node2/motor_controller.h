#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#include "util.h"

void motor_controller_init();
void motor_controller();
void motor_controller_set_point(uint16_t sp);
void motor_controller_activate();
void motor_controller_deactivate();

#endif // MOTOR_CONTROLLER_H_