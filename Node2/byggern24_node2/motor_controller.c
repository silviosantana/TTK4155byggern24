#include "motor_controller.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "motor_driver.h"
#include "game.h"
#include "util.h"

double kp, ki,kd;
double integral, dt;
double error, prev_error;
int set_point;
int max_motor_pos, min_motor_pos;

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

 	min_motor_pos = 500;
	 
}

void motor_controller_init()
{
	kp = 1;
	ki = 12.5;
	kd = 0.1;
	integral = 0;
	error = 0;
	prev_error = 0;
	set_point = 0;
	dt = 0.05;
	
	motor_controller_calibration();
	
	
	TCCR4A |= 0;
	// set up timer with prescaler = 64 and CTC mode
	TCCR4B |= (1 << WGM42)|(1 << CS41)|(1 << CS40);
	// initialize counter
	TCNT4 = 0;
	// initialize compare value
	OCR4A = 12499;
	TIMSK4 |= (1 << OCIE4A);
	
}

void motor_controller_activate()
{
	//cli();
	
	
	// enable compare interrupt
	TIMSK4 |= (1 << OCIE4A);
	//sei();
}

void motor_controller_deactivate()
{
	//cli();
	// disable compare interrupt
	clear_bit(TIMSK4, OCIE4A);
	//sei();
}

void motor_controller()
{
	if (game_get_mode() == 1)
	{
		enum direction target_dir;
		uint8_t speed;
		int motor_cur_pos = motor_get_encoder();
		double derivative;
		
		int curr_pos = (int) 100.0*((double)(motor_cur_pos - min_motor_pos))/((double)(max_motor_pos - min_motor_pos));
		
		error = (double)(set_point - curr_pos);
		
		if (error < 0)
		{
			target_dir = RIGHT;
		}else
		{
			target_dir = LEFT;
		}
		
		integral = integral + error * dt;
		derivative = (error - prev_error)/dt;

		if (abs((int)error) < 6){
			error = 0;
			integral = 0;
		}

		//speed = abs((uint8_t) (kp*error + ki*integral + kd*derivative));
		speed = (uint8_t) abs((int)(kp*error + ki*integral));
		
		if (speed > 150)
		{
			speed = 150;
		}
		
		else if (speed < 55 && abs(error) > 5)
		{
			speed = 55;
		}
		//	printf("MAXP: %d | BCP: %d | CP: %d | SP: %d\n\r", max_motor_pos, motor_cur_pos, curr_pos, set_point );
		
		prev_error = error;
		
		motor_move(target_dir, speed);
	}
	
}

void motor_controller_set_point(uint16_t sp)
{
	set_point = sp;
}

ISR(TIMER4_COMPA_vect){
	
	cli();
	motor_controller();
	sei();
}