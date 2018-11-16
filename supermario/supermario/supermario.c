/*
 * CODE BASED ON FOLLOWING AUTHOR
 *
 * Author : Whiskey dick
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Music.h"
#include "bit_macros.h"
#include "UART_driver.h"

uint8_t bpm;
uint8_t note_pointer;
uint16_t note;
uint32_t length;
uint16_t key;
uint32_t num_cycles;
uint16_t* current_song;
uint8_t pick_song;


void music_delay(uint16_t delay) {
	while (delay--) {
		_delay_us(0.75);
	}
}

void init()
{
	set_bit(DDRD, melody_pin);
	clear_bit(DDRD, DDD7);
	clear_bit(DDRD, DDD6);	

	bpm = 0;
	note_pointer = 0;
	pick_song = UNDERWORLD;
	key = 0;
	note = 0;
}

int main(void) {
	init();
	int play_song;
	while(1) {
		pick_song = (PIND & 0b10000000);
		play_song = (PIND & 0b01000000);
		if (play_song == 64)
		{
			if (pick_song == 128)
			{
				current_song = mario_melody;
				bpm = mario_melody[0];
			}else if (pick_song == 0)
			{
				current_song = underworld_melody;
				bpm = underworld_melody[0];
			}

			// Play next tone in the song
			note_pointer++;
			note = current_song[note_pointer];
			note_pointer++;
			key = current_song[note_pointer];
			length = note/bpm;
			
			// Make room for pause notes
			if (key == 0) {
				key = 500;
				num_cycles = length*1000/2/key;
			}
			else {
				num_cycles = length*1000/2/key;				// Duration of key-tone
			}
			
			// Play tone on Melody_pin
			for (uint32_t i = 0; i < num_cycles; i++) {
				if (key == 500) {
					clear_bit(PORTD,melody_pin);
					music_delay(key);
					clear_bit(PORTD,melody_pin);
					music_delay(key);
					} else {
					set_bit(PORTD,melody_pin);
					music_delay(key);
					clear_bit(PORTD, melody_pin);
					music_delay(key);
				}
			}
			
			// Small delay between each tone
			_delay_us(SIXTEENTH);
			// If key was zero, set zero again in case song is over
			if (key == 500) {
				key = 0;
			}
		}else{
			note_pointer = 0;
		}		
	}
	
}