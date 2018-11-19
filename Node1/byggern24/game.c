#include "game.h"

#include <stdint.h>
#include <string.h>

#include "util.h"
#include "util/delay.h"
#include "joystick.h"
#include "can_driver.h"
#include "oled_driver.h"

void send_input_info()
{
	can_message msg;
	msg.id = 1;
	msg.data[0] = gamemode;
	msg.length = 1;
	
	can_send_msg(&msg);
}

void send_multi_board()
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
	msg.length = 6;
	
	can_send_msg(&msg);
}

void menu_state_machine_setup(int* arrow_max, int* arrow_pos)
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
	state2->parent = state2;
	state2->children[0] = state2;
	
	state3->name = "Set Motor Controller";
	state3->val = 3;
	state3->parent = state1;
	state3->children[0] = state4;
	state3->children[1] = state5;
	
	state4->name = "Slider";
	state4->val = 4;
	state4->parent = state3;
	
	state5->name = "Joystick";
	state5->val = 5;
	state5->parent = state3;
	
	menu_set_state(state1, arrow_max, arrow_pos);
}

void menu_state_machine(direction* old_dir, direction* dir, int* arrow_max, int* arrow_pos)
{
	int s = main_state->val;
	printf("state %d\n\r", s);
	
	switch (s)
	{
		case 1: // Menu
			*old_dir = *dir;
			*dir = get_joystick_direction();
			//printf("direction %d,%d\n\r", *old_dir, *dir);
			if (*old_dir == NEUTRAL){
				menu_move_arrow(dir, arrow_max, arrow_pos);}
			break;
		
		case 2: // Start Game
			send_multi_board();
			_delay_ms(50);
			break;
		
		case 3: // Set Mode
			*old_dir = *dir;
			*dir = get_joystick_direction();
			if (*old_dir == NEUTRAL){
				menu_move_arrow(dir, arrow_max, arrow_pos);}
			break;
		
		case 4: // Use Slider
			*old_dir = *dir;
			*dir = get_joystick_direction();
			if (*old_dir == NEUTRAL){
				menu_move_arrow(dir, arrow_max, arrow_pos);}
			break;
		
		case 5: // Use Joystick
			*old_dir = *dir;
			*dir = get_joystick_direction();
			if (*old_dir == NEUTRAL){
				menu_move_arrow(dir, arrow_max, arrow_pos);}
			break;
		
		default:
		break;
	}
}

//set state of menu
void menu_set_state(menu* current_state, int* arrow_max, int* arrow_pos)
{
	printf("Setting up state!\n\r");
	oled_reset();
	int s = current_state->val;
	
	switch (s)
	{
		case 1: // Menu
			printf("Menu on %d,100 for %s \n\r", *arrow_pos, current_state->name);
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
			send_input_info();
		  	oled_animate_ntnu();
		 	_delay_ms(1000);
		 	play_music(1);
		  	oled_animate_mario_large();
		  	play_music(0);
			printf("Start Game\n\r");
			oled_ingame_display(0);
			break;
		
		case 3: // Set Mode
			printf("Set Mode\n\r");
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			oled_pos(2,0);
			oled_print(current_state->children[0]->name, 2);
			oled_pos(3,0);
			oled_print(current_state->children[1]->name, 2);
			break;
		
		case 4: // Use Slider
			gamemode = 1;
			printf("Use Slider\n\r");
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			break;
		
		case 5: // Use Joystick
			gamemode = 0;
			printf("Use Joystick\n\r");
			oled_pos(0,0);
			oled_print(current_state->name, 3);
			break;
		
		default:
			printf("No state \n\r");
			break;
	}
	main_state = current_state;
}

//move menu arrow to navigate among menu levels
void menu_move_arrow(direction* dir, int* arrow_max, int* arrow_pos)
{
	//printf("direction %d\n\r", *dir);
	direction s = *dir;
	switch (s)
	{
		case LEFT:
			menu_set_state(main_state->parent, *arrow_max, *arrow_pos);
			printf("%s\n\r", main_state->name);
			break;
		case RIGHT:
			menu_set_state(main_state->children[*arrow_pos - 2], *arrow_max, *arrow_pos);
			printf("%s\n\r", main_state->name);
			break;
		case UP:
			oled_pos(*arrow_pos,100);
			oled_print(" ", 3);
			(*arrow_pos)--;
			if (*arrow_pos == 1)
			{
				*arrow_pos = *arrow_max;
			}
			oled_pos(*arrow_pos,100);
			oled_print_special_char(5);
			break;
		case DOWN:
			oled_pos(*arrow_pos,100);
			oled_print(" ", 3);
			(*arrow_pos)++;
			if (*arrow_pos == *arrow_max + 1)
			{
				*arrow_pos = 2;
			}
			oled_pos(*arrow_pos,100);
			oled_print_special_char(5);
			break;
		default:
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

void start_game()
{
	
}

void display_score()
{
	oled_ingame_display(score);
}

void music_init()
{
	set_bit(DDRB, PB2);
	set_bit(DDRB, PB3);
	clear_bit(PORTB, PB2);
	clear_bit(PORTB, PB3);
}

void play_music(int song)
{
	if (song == 0)
		clear_bit(PORTB, PB2);
	else if (song == 1)
		set_bit(PORTB, PB2);
	else return;
	set_bit(PORTB, PB3);
}

void stop_music()
{
	clear_bit(PORTB, PB3);
}