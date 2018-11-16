#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include <stdint.h>

void timer_driver_init();
void timer_driver_set_duty_cycle(int8_t position);

#endif /* TIMER_DRIVER_H_ */