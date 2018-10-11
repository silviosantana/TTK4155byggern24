#include "stdio.h"
#include "avr/io.h"

#include "util.h"
#include "usart_driver.h"
#include "sram_driver.h"
#include "adc_driver.h"
#include "joystick.h"
#include "oled_driver.h"
#include "spi_driver.h"
#include "can_controller_driver.h"
#include "can_driver.h"

#include "util/delay.h"


void test_LED()
{
	DDRA = 0xFF;
	// LED on
	PORTA = 0b00000001;            // PC0 = High = Vcc
	_delay_ms(1000);                // wait 500 milliseconds
		
	//LED off
	PORTA = 0b00000000;            // PC0 = Low = 0v
	_delay_ms(1000);                // wait 500 milliseconds
}

void test_transmit_receive()
{
	unsigned char test;
	printf("Your voice is my command");
	test = USART_Receive();
	USART_Transmit(test);
}

void test_latch()
{
	// LED on
	PORTA = 0b11111111;            // PC0 = High = Vcc
	_delay_ms(1000);                // wait 500 milliseconds
	
	//LED off
	PORTA = 0b00000000;            // PC0 = Low = 0v
	_delay_ms(1000);                // wait 500 milliseconds
}

void test_joystick ()
{
	struct Position j_pos;
	uint8_t sliderL;
	uint8_t sliderR = 0;
	
	while(1)
	{
		_delay_ms(500);
		j_pos = get_joystick_position();
		sliderL = get_left_slider();
		sliderR = get_right_slider();
		enum direction dir = get_joystick_direction();
		printf("\r\nX: %d\t|\tY: %d\t|\tSL: %d\t|\tSR: %d |\tdir: %d", j_pos.x, j_pos.y, sliderL, sliderR, dir);
		
		
	}
}

void test_ui()
{
	menu* state1 = NULL;
	menu* state2 = NULL;
	menu* state3 = NULL;
	menu* state4 = NULL;
	
	state1 = (menu*) malloc(sizeof(menu));
	state2 = (menu*) malloc(sizeof(menu));
	state3 = (menu*) malloc(sizeof(menu));
	state4 = (menu*) malloc(sizeof(menu));

	state1->name = "State 1";
	state1->val = 1;
	state1->children[0] = state2;
	state1->children[1] = state3;
	state1->children[2] = state4;
	
	state2->name = "State 2";
	state2->val = 2;
	state2->parent = state1;
	
	state3->name = "State 3";
	state3->val = 3;
	state3->parent = state1;
	
	state4->name = "State 4";
	state4->val = 4;
	state4->parent = state1;
	
	menu* current_state = state1;
	
	int arrow_pos = 2;
	int arrow_max = 2;
	
	current_state = state_machine(current_state, &arrow_max, arrow_pos);
	
	struct Position j_pos;
	uint8_t sliderL;
	uint8_t sliderR = 0;
	
	while(1)
	{
		_delay_ms(500);
		
		j_pos = get_joystick_position();
		sliderL = get_left_slider();
		sliderR = get_right_slider();
		enum direction dir = get_joystick_direction();
		
		
		//printf("\r\nX: %d\t|\tY: %d\t|\tSL: %d\t|\tSR: %d |\tdir: %d", j_pos.x, j_pos.y, sliderL, sliderR, dir);
		
		current_state = move_arrow(dir, current_state, &arrow_max, &arrow_pos);
	}
}

int main(void)
{
	
	USART_Init(MYUBRR);
	SRAM_init();
	joystick_init();
	oled_init();
	spi_init();
	//can_controller_init();
	printf("START============================\n\r");
	can_init();
	
	
	
	
	
	//test_ui();
	//SRAM_test();
	
	//oled_test();
	
	//test_joystick();
	//can_controller_test();
	can_driver_test();

	
	printf("END   ============================\n\r");

	
	

	
}