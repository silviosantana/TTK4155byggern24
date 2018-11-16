#include "game.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "util/delay.h"
#include "joystick.h"
#include "can_driver.h"
#include "oled_driver.h"

void send_game_init()
{
	struct Position j_pos;
	uint8_t sliderL;
	uint8_t sliderR = 0;
	uint8_t r_button, l_button;
	
	j_pos = get_joystick_position();
	sliderL = get_left_slider();
	sliderR = get_right_slider();
	direction dir = get_joystick_direction();
	r_button = get_joystick_right_button();
	l_button = get_joystick_left_button();
	can_message msg;
	msg.id = 1;
	msg.data[0] = j_pos.x;
	msg.data[1] = j_pos.y;
	msg.data[2] = sliderL;
	msg.data[3] = sliderR;
	msg.data[4] = dir;
	msg.data[5] = r_button;
	msg.data[6] = l_button;
	msg.length = 7;
	
	can_send_msg(&msg);
}


void send_multi_board_output()
{
	struct Position j_pos;
	uint8_t sliderL;
	uint8_t sliderR = 0;
	uint8_t r_button, l_button;
	
	j_pos = get_joystick_position();
	sliderL = get_left_slider();
	sliderR = get_right_slider();
	direction dir = get_joystick_direction();
	r_button = get_joystick_right_button();
	l_button = get_joystick_left_button();
	can_message msg;
	msg.id = 1;
	msg.data[0] = j_pos.x;
	msg.data[1] = j_pos.y;
	msg.data[2] = sliderL;
	msg.data[3] = sliderR;
	msg.data[4] = dir;
	msg.data[5] = r_button;
	msg.data[6] = l_button;
	msg.length = 7;
	
	can_send_msg(&msg);
}

// Sets state for state machine
menu* menu_state_machine_setup(menu* current_state, int* arrow_max, int* arrow_pos)
{
	menu* state1 = NULL; // Menu
	menu* state2 = NULL; // Start Game
	menu* state3 = NULL; // Set Mode
	menu* state4 = NULL; // Use Slider
	menu* state5 = NULL; // Use Joystick
	
	state1 = (menu*) malloc(sizeof(menu));
	state2 = (menu*) malloc(sizeof(menu));
	state3 = (menu*) malloc(sizeof(menu));
	state4 = (menu*) malloc(sizeof(menu));
	state5 = (menu*) malloc(sizeof(menu));
	
	state1->name = "Menu";
	state1->val = 1;
	state1->parent = state1;
	state1->children[0] = state2;
	state1->children[1] = state3;
	
	state2->name = "Start Game";
	state2->val = 2;
	state2->parent = state1;
	
	state3->name = "Set Motor Controller";
	state3->val = 3;
	state3->parent = state1;
	state3->children[0] = state4;
	state3->children[1] = state5;
	
	state4->name = "Slider";
	state4->val = 4;
	state4->parent = state3;
	state4->children[0] = state1;
	
	state5->name = "Joystick";
	state5->val = 5;
	state5->parent = state3;
	state5->children[0] = state1;
	
	current_state = state1;
	menu_set_state(current_state, arrow_max, arrow_pos);
}

// A menu state machine
void menu_state_machine(menu* current_state, int* arrow_max, int* arrow_pos, direction* old_dir, direction* dir)
{
	int s = current_state->val;
	
	switch (s)
	{
		case 1: // Menu		
			// Move arrow only if it has been in neutral position
			old_dir = dir;
			dir = get_joystick_direction();
			if (old_dir == NEUTRAL) {
				menu_move_arrow(dir, current_state, arrow_max, arrow_pos);
			}
			break;
		case 2: // Start Game
			send_multi_board_output(); // Sends CAN-message with multi-board outputs
			_delay_ms(50);
			break;
		case 3: // Set Mode
			// Move arrow only if it has been in neutral position
			old_dir = dir;
			dir = get_joystick_direction();
			if (old_dir == NEUTRAL) {
				menu_move_arrow(dir, current_state, arrow_max, arrow_pos);
			}
			break;
		case 4: // Use Slider
			// Move arrow only if it has been in neutral position
			old_dir = dir;
			dir = get_joystick_direction();
			if (old_dir == NEUTRAL) {
				menu_move_arrow(dir, current_state, arrow_max, arrow_pos);
			}
				break;
		case 5: // Use Joystick
			// Move arrow only if it has been in neutral position
			old_dir = dir;
			dir = get_joystick_direction();
			if (old_dir == NEUTRAL) {
				menu_move_arrow(dir, current_state, arrow_max, arrow_pos);
			}
			break;
		default:
			break;
	}
}

//set state of menu
menu* menu_set_state(menu* current_state, int* arrow_max, int* arrow_pos)
{
	oled_reset();
	int s = current_state->val;
	
	switch (s)
	{
		case 1: // Menu
			oled_pos(*arrow_pos,100);
			oled_print_special_char(5);
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			oled_pos(2,0);
			oled_print(current_state->children[0]->name, 2);
			oled_pos(3,0);
			oled_print(current_state->children[1]->name, 2);
			*arrow_max = 3;
			break;
		case 2: // Start Game
			send_game_init();
			break;
		case 3: // Set Mode
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			oled_pos(2,0);
			oled_print(current_state->children[0]->name, 2);
			oled_pos(3,0);
			oled_print(current_state->children[1]->name, 2);
			break;
		case 4: // Use Slider
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			break;
		case 5: // Use Joystick
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			break;
		default:
			break;
	}
	
	return current_state;
}

//move menu arrow to navigate among menu levels
void menu_move_arrow(int dir, menu* current_state, int* arrow_max, int* arrow_pos)
{
	direction choice = dir;
	
	switch(choice)
	{
		case LEFT:
			current_state = current_state->parent;
			menu_set_state(current_state, arrow_max, arrow_pos);
			printf("%s\n\r", current_state->name);
			break;
		case RIGHT:
			current_state = current_state->children[*arrow_pos - 2];
			menu_set_state(current_state, arrow_max, arrow_pos);
			printf("%s\n\r", current_state->name);
			break;
		case UP:
			oled_pos(*arrow_pos,100);
			oled_print(" ", 3);
			(*arrow_pos)--;
			if (*arrow_pos == 1) {*arrow_pos = *arrow_max;}
			oled_pos(*arrow_pos,100);
			oled_print_special_char(5);
			break;
		case DOWN:
			oled_pos(*arrow_pos,100);
			oled_print(" ", 3);
			(*arrow_pos)++;
			if (*arrow_pos == *arrow_max + 1) {*arrow_pos = 2;}
			oled_pos(*arrow_pos,100);
			oled_print_special_char(5);
			break;
	}
}

void menu_intro_screen()
{
	oled_reset();
	oled_animate_ntnu();
	_delay_ms(800);
	oled_animate_mario_large();
}