#ifndef GAME_H_
#define GAME_H_

#include "util.h"

int score;

void send_input_info();
void send_multi_board();

void menu_state_machine_setup(int* arrow_max, int* arrow_pos);
void menu_state_machine(direction* old_dir, direction* dir, int* arrow_max, int* arrow_pos);
void menu_set_state(menu* current_state, int* arrow_max, int* arrow_pos);
void menu_move_arrow(direction* dir, int* arrow_max, int* arrow_pos);
void menu_intro_screen();
void music_init();
void start_game();
void play_music(int song);
void stop_music();


#endif /* GAME_H_ */