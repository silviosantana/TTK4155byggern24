#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define F_OSC 16000000UL
#define F_CPU F_OSC
#define BAUD 9600
#define MYUBRR F_CPU/16UL/BAUD-1

struct Position 
{
	int x;
	int y;
};

enum direction {NEUTRAL, LEFT, RIGHT, UP, DOWN};

typedef struct Menu
{
	int* val;
	char* name;
	struct Menu* parent;
	struct Menu* children[5];
	
} menu;

typedef struct {
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
} can_message;

#endif 