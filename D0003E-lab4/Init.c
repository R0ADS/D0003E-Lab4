/*
 * CFile1.c
 *
 * Created: 2025-02-26 13:09:09
 *  Author: anton
 */ 
#include <avr/io.h>
#include "init.h"

void Init(void) {
	CLKPR = 0x80;    // Fix clock speed
	CLKPR = 0x00;
	
	// LCD init
	LCDCCR = (1 << LCDCC3) | (1 << LCDCC2) | (1 << LCDCC1) | (1 << LCDCC0);
	//       (                      Voltage                              )
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX1) | (1 << LCDMUX0) | (1 << LCDPM2) | (1 << LCDPM1) | (1 << LCDPM0);
	//       (asynch ext)   (        duty cycle           )   (                  segments                 )
	LCDFRR = (1 << LCDCD2) | (1 << LCDCD1) | (1 << LCDCD0);
	//       (            Delen med D                    )
	LCDCRA = (1 << LCDAB) | (1 << LCDEN);
	//       (low power )   (enable lcd)
	
	// Stuffs for the joystick and interrupt
	PORTB = SET(PB7) | SET(PB6) | SET(PB4);									// Set corresponding Output pin
	DDRB = (1 << DDB7);									// Unsure
	PORTE = 0b00010000;
	PCMSK1 |= (1 << PCINT15);							// Pin change interrupt 1
	EIMSK |= (1 << PCIE1);								// Enalbe pin change on I/O pin 7
}