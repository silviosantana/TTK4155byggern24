#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define F_OSC 16000000UL
#define F_CPU F_OSC
#define BAUD 9600
#define MYUBRR F_CPU/16UL/BAUD-1
#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit) (reg & (1 << bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))

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


extern int calibrate;
extern int new_mode;
#endif // UTIL_H_