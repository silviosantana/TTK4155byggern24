#ifndef JOYSTICK_H_
#define JOYSTICK_H_

void joystick_init();
struct Position get_joystick_position();
int get_joystick_direction();
int get_left_slider();
int get_right_slider();
int get_joystick_right_button();
int get_joystick_left_button();
void joystick_test();

static int x_middle_pos;
static int y_middle_pos;

#endif // JOYSTICK_H_