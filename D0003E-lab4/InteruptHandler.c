/*
 * InteruptHandler.c
 *
 * Created: 2025-03-01 00:07:40
 *  Author: anton
 */ 

#include <avr/io.h>
#include "InteruptHandler.h"
#include "TinyTimber.h"
#include "Joystick.h"

void FreqChange(InteruptHandler *self){
    ASYNC(self->Joystick, joystickvert, NULL)
}

void genSwitch(InteruptHandler *self) {
	if (self->risingEdge) {
		self->risingEdge = false;
		ASYNC(self->gui, press, NULL);
	}
	else{
		self->risingEdge = true;
	}
}
