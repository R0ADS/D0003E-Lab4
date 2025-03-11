/*
 * Joystick.c
 *
 * Created: 2025-03-01 00:48:04
 *  Author: anton
 */ 

#include <stdbool.h>
#include <avr/io.h>
#include "Joystick.h"

void holdStick(Joystick *self){
	if (!(PINB & (1 << PB7))) { //ner klickning
		self->hold = true;
		ASYNC(self->gui, decreaseGui, NULL); // Kalla p� GUI f�r att s�nka frekvensen
		AFTER(MSEC(100), self, holdStick, NULL);	// Kalla p� sig sj�lv igen (loop)
	}
	if (!(PINB & (1 << PB6))) {	// Up
		self->hold = true;
		ASYNC(self->gui, increaseGui, NULL);
		AFTER(MSEC(100), self, holdStick, NULL);
	}
	self->hold = false;
}

int joystickVert(Joystick *self){
	if (!self->hold){
		if (!(PINB & (1 << PB7))) { //ner klickning
			AFTER(MSEC(100), self, holdStick, NULL);	// Kalla p� sig sj�lv igen (loop)
		}
		if (!(PINB & (1 << PB6))) {	// Up
			AFTER(MSEC(100), self, holdStick, NULL);
		}
	}
	if (!(PINB & (1 << PB4))) { // Press
		ASYNC(self->gui, pressGui, NULL);
	}
}


/*if (self->msg) {
	ABORT(self->msg);
	self->msg = NULL;
}
self->msg = AFTER(MSEC(100), self, joystickVert, NULL);	// Kalla p� sig sj�lv igen (loop)*/

int joystickHor(Joystick *self) {
	if (!(PINE & (1 << PE3)) || (!(PINE & (1 << PE2)))) {	// Både och???
		self->msg = ASYNC(self->gui, switchGeneratorGui, NULL);
	}
}

