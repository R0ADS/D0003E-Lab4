/*
 * Generators.c
 *
 * Created: 2025-03-01 00:06:40
 *  Author: anton
 */ 
#include "Generators.h"
#include "Lcd.h"
#include "TinyTimber.h"
#include <avr/io.h>


// Updates dispaly
void updateDisplay(Generators *self) {
    if (self->isRight) {
        int temp[] = {0, self->currentFreq};
        ASYNC(self->lcd, printAt, self->currentFreq*10 + 4);
    }
    else {
        ASYNC(self->lcd, printAt, self->currentFreq*10);
    }
}

// Increases frequency
void increase(Generators *self) {
    self->currentFreq++;
    ASYNC(self, updateDisplay, NULL);
    // Uppdatera frekvens för actual pulsgenerering
    
    
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

// Generates the pulse
void generatePulse(Generators *self, int freq) {
    if (freq > 0) {                                 // Stops recursion if freq hits 0
        if (self->isRight) {
            PORTE ^= (1 << PE6);                    // This and the one below generates actual pulse
        }
        else {
            PORTE ^= (1 << PE4);
        }
    AFTER(MSEC(1000/freq), self, generatePulse, NULL);    // Calls itself according to freq
    }
}