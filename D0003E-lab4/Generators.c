/*
 * Generators.c
 *
 * Created: 2025-03-01 00:06:40
 *  Author: anton
 */ 
#include "Generators.h"
#include "Lcd.h"
#include "TinyTimber.h"
#include "bitOp.h"
#include <avr/io.h>


// Updates dispaly
void updateDisplay(Generators *self) {
	if (self->isRight) {
		ASYNC(self->lcd, printAt, self->currentFreq*10 + 4);
	}
	else {
		ASYNC(self->lcd, printAt, self->currentFreq*10);
    }
}

// Increases frequency
void increase(Generators *self) {
	if (self->currentFreq != 99){
		self->currentFreq++;
		ASYNC(self, updateDisplay, NULL);
	}   
}

// The thing that happens when pulling joystick down
void decrease(Generators *self) {
    if (self->currentFreq > 0) {
        self->currentFreq--;
        ASYNC(self, updateDisplay, NULL);
        // Uppdatara frekvens (osäker om man ska calla SYNC eller ASYNC)
    }
}

// The thing that happens when pressing the joystick
void press(Generators *self) {
    if (self->currentFreq != 0) { // Maybe fix call later
        self->storedFreq = self->currentFreq;
        self->currentFreq = 0;
		ASYNC(self, updateDisplay, NULL);
    }
    else {
        self->currentFreq = self->storedFreq;
		ASYNC(self, updateDisplay, NULL);
    }
}

void generatePulse(Generators *self){
    if (self->currentFreq != 0 || self->risingEdge){
        ASYNC(self->bitOp, writeBit, NULL);
		self->risingEdge ^= (self->risingEdge);
		AFTER(MSEC(500/self->currentFreq), self, generatePulse, NULL);
    }
	else{
    self->risingEdge ^= (self->risingEdge);
    AFTER(MSEC(500), self, generatePulse, NULL);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}