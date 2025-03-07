/*
 * Joystick.c
 *
 * Created: 2025-03-01 00:48:04
 *  Author: anton
 */ 

#include <stdbool.h>
#include <avr/io.h>
#include "Joystick.h"


int joystickVert(Joystick *self){
	if (!(PINB & (1 << PB7))) { //ner klickning
		ASYNC(self->gui, decreaseGui, NULL); // Kalla p� GUI f�r att s�nka frekvensen
		AFTER(MSEC(100), self, joystickVert, NULL);	// Kalla p� sig sj�lv igen (loop)
	}
	if (!(PINB & (1 << PB6))) {
		ASYNC(self->gui, increaseGui, NULL);
		AFTER(MSEC(100), self, joystickVert, NULL);
	}
	if (!(PINB & (1 << PB4))) { // press
		if (self->risingEdge) {
			self->risingEdge = false;
			ASYNC(self->gui, pressGui, NULL);
		}
		else{
			self->risingEdge = true;
		}
	}
	return 0;
}


int joystickWhore(Joystick *self) {
	if (!(PINE & (1 << PE3)) || (!(PINE & (1 << PE2)))) {	// Både och???
		ASYNC(self->gui, switchGeneratorGui, NULL);
	}
}

