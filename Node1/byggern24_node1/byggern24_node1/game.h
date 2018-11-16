#ifndef GAME_H_
#define GAME_H_

#include "util.h"

void send_game_init();
void send_multi_board_output();

menu* menu_state_machine_setup(menu* current_state, int* arrow_max, int* arrow_pos);
menu* menu_set_state(menu* next_state, int* arrow_max, int* arrow_pos);
void menu_state_machine(menu* current_state, int* arrow_max, int* arrow_pos, direction* old_dir, direction* dir);
void menu_move_arrow(int dir, menu* current_state, int* arrow_max, int* arrow_pos);
void menu_intro_screen();

#endif /* GAME_H_ */