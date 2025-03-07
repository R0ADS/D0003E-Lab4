/*
 * Joystick.c
 *
 * Created: 2025-03-01 00:48:04
 *  Author: anton
 */ 

#include <stdbool.h>
#include "Joystick.h"

int joystickvert(Joystick *self){
	if (!(PINB & (1 << PB7))) { //ner klickning
		ASYNC(self->gui, decrease, NULL); // Kalla p� GUI f�r att s�nka frekvensen
		AFTER(MSEC(100), self, Joystickhold, NULL);	// Kalla p� sig sj�lv igen (loop)
	}
	if (!(PINB & (1 << PB6))) {
		ASYNC(self->gui, increase, NULL);
		AFTER(MSEC(100), self, Joystickhold, NULL);
	}
	if (!(PINB & (1 << PB4))) { // press
		if (self->risingEdge) {
			self->risingEdge = false;
			ASYNC(self->gui, press, NULL);
		}
		else{
			self->risingEdge = true;
		}
	}
	return 0;
}

/*int joyStickVert(Joystick *self){
	if (!(PINB & (1 << PB7))) { //ner klickning
		async(self->gui, decrease); // Kalla p� GUI f�r att s�nka frekvensen
	}
	if (!(PINB & (1 << PB6))) { // upp klickning
		async(self->gui, Increase, NULL); // kalla p� GUI f�r att h�ja frekvensen
	}
	if (!(PINB & (1 << PB4))) { // press
		if (self->risingEdge) {
			self->risingEdge = false;
			async(self->gui, press, NULL);
		}
		else{
			self->risingEdge = true;
		}
	}
	return 0;
}*/

int joyStickWhore(Joystick *self) {
	if (!(PINE & (1 << PE3))) || (!(PINE & (1 << PE2))) {	// Både och???
		async(self->gui, switchGenerator, NULL);
	}
}

