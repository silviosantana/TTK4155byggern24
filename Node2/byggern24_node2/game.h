#ifndef GAME_H_
#define GAME_H_

#include <avr/io.h>

void set_game_mode(int gm);
uint16_t record_score(uint16_t score);
void game_toggle_mode();
int game_get_mode();

#endif // GAME_H_