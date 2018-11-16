#ifndef GAME_H_
#define GAME_H_

#include "util.h"

int score;

void send_multi_board();

menu* menu_set_state(menu* current_state, int* arrow_max, int arrow_pos);
menu* menu_move_arrow(int dir, menu* current_state, int* arrow_max, int* arrow_pos);
menu* menu_state_machine_setup();
void menu_intro_screen();
void music_init();
void start_game();
void play_music(int song);
void stop_music();


#endif /* GAME_H_ */