#include <avr/io.h>
#include <util/delay.h>

#include "util.h"
#include "usart_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"
#include "timer_driver.h";

int main(void)
{
	USART_Init(MYUBRR);
	spi_init();
	can_controller_init();
	can_init();
	
	printf("Hello from node 2!\n\r");

	//can_driver_test();
	timer_driver_init();

	while(1)
	{
// 		timer_driver_set_duty_cycle(-100);
// 		_delay_ms(2000);
// 		timer_driver_set_duty_cycle(0);
// 		_delay_ms(2000);
// 		timer_driver_set_duty_cycle(100);
// 		_delay_ms(2000);
	}
	

}