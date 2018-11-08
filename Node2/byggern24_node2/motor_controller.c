#include "motor_controller.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "motor_driver.h"
#include "util.h"

float kp, ki;
float integral, dt;
uint16_t error;
uint16_t set_point;
uint16_t max_motor_pos, min_motor_pos;

void motor_controller_calibration()
{
// 	motor_move(RIGHT, 100);
// 	_delay_ms(1000);
// 	motor_reset_encoder();
// 	motor_move(LEFT, 100);
// 	_delay_ms(1000);
// 	max_motor_pos = motor_get_encoder();
// 	motor_move(RIGHT, 100);
// 	_delay_ms(1000);
// 	motor_move(RIGHT, 0);

	max_motor_pos = 8800;
	min_motor_pos = 100;

}

void motor_controller_init()
{
	kp = 2.5;
	ki = 2;
	integral = 0;
	error = 0;
	set_point = 0;
	dt = 0.1;
	
	motor_controller_calibration();
	cli();
	
	TCCR4A |= 0;
	// set up timer with prescaler = 64 and CTC mode
	TCCR4B |= (1 << WGM42)|(1 << CS41)|(1 << CS40);
	// initialize counter
	TCNT4 = 0;
	// initialize compare value
	OCR4A = 24999;
	// enable compare interrupt
	TIMSK4 |= (1 << OCIE4A);
	sei();
}

void motor_controller()
{
	enum direction target_dir;
	int8_t speed;
	uint16_t motor_cur_pos = motor_get_encoder();
	
	
	error = set_point - motor_cur_pos;
	
	if (error < 0)
	{
		target_dir = LEFT;
	}else
	{
		target_dir = RIGHT;
	}
	
	integral = integral + error * dt;
	if (error < 10){
		integral = 0;
	}
	speed = abs((int8_t) (kp*error + ki*integral));
	
	if (speed > 255)
	{
		speed = 255;
	}
	printf("ERROR: %d \t SPEED: %d\t DIR: %d \t MPOS: %d\n\r", error, speed, target_dir, motor_cur_pos);
	motor_move(target_dir, speed);
}

void motor_controller_set_point(uint16_t sp)
{
	set_point = (max_motor_pos-min_motor_pos)*sp/100 + min_motor_pos;
}

ISR(TIMER4_COMPA_vect){
	
	cli();
	motor_controller();
	sei();
}