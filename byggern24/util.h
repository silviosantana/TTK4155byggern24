
#ifndef UTIL_H_
#define UTIL_H_

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

#endif /* UTIL_H_ */