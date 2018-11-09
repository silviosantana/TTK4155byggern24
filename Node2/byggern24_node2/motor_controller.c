#include "motor_controller.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "motor_driver.h"
#include "util.h"

float kp, ki,kd;
float integral, dt;
float error, prev_error;
uint16_t set_point;
uint16_t max_motor_pos, min_motor_pos;

void motor_controller_calibration()
{
	motor_move(RIGHT, 200);
	_delay_ms(1000);
	motor_reset_encoder();
	motor_move(LEFT, 200);
	_delay_ms(1000);
	max_motor_pos = motor_get_encoder();
	max_motor_pos -= 300;
	motor_move(RIGHT, 200);
	_delay_ms(1000);
	motor_move(RIGHT, 0);

// 	motor_move(RIGHT, 255);
// 	_delay_ms(500);
// 	motor_reset_encoder();
// 	max_motor_pos = 8800;
 	min_motor_pos = 100;

}

void motor_controller_init()
{
	kp = 1;
	ki = 4;
	kd = -0.1;
	integral = 0;
	error = 0;
	prev_error = 0;
	set_point = 0;
	dt = 0.05;
	
	motor_controller_calibration();
	cli();
	
	TCCR4A |= 0;
	// set up timer with prescaler = 64 and CTC mode
	TCCR4B |= (1 << WGM42)|(1 << CS41)|(1 << CS40);
// 	TCCR4B &= ~(1 << CS40);
// 	TCCR4B |= (1 << WGM42)|(1 << CS41);
	// initialize counter
	TCNT4 = 0;
	// initialize compare value
	OCR4A = 12499;
	// enable compare interrupt
	TIMSK4 |= (1 << OCIE4A);
	sei();
}

void motor_controller()
{
	enum direction target_dir;
	uint8_t speed;
	uint16_t motor_cur_pos = motor_get_encoder();
	float derivative;
	
	float refer = 100.0*((float)(motor_cur_pos - min_motor_pos))/((float)(max_motor_pos - min_motor_pos));
	
	error = (float)(set_point - refer);
	
	if (error < 0)
	{
		target_dir = RIGHT;
		//printf("Woohoo, goin' to tha left!");
	}else
	{
		target_dir = LEFT;
	}
	
	integral = integral + error * dt;

	if (abs(error) < 3){
		error = 0;
	}
	if (abs(error) < 1)
	{
		integral = 0;
	}
	derivative = (error - prev_error)/dt;
	speed = abs((uint8_t) (kp*error + ki*integral + kd*derivative));
	printf("%d\n\r", speed);
	if (speed > 200)
	{
		speed = 200;
	}else if (speed < 55 && abs(error) > 15)
	{
		speed = 55;
	}
	
	prev_error = error;
	
	motor_move(target_dir, speed);
}

void motor_controller_set_point(uint16_t sp)
{
	set_point = sp;
	//set_point = (int)(((float)(max_motor_pos-min_motor_pos))*sp/100.0) + min_motor_pos;
	//printf("set point: %d \t sp: %d \n\r", set_point, sp);
}

ISR(TIMER4_COMPA_vect){
	
	cli();
	motor_controller();
	sei();
}