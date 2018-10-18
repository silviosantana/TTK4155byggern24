#include <avr/io.h>
#include <util/delay.h>

#include "util.h"
#include "usart_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"

int main(void)
{
	USART_Init(MYUBRR);
	
	spi_init();
	can_controller_init();
	can_init();
	
	printf("Hello from node 2!\n\r");

	can_driver_test();
	while(1)
	{

	}
	

}