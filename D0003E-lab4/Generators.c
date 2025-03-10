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

int currentFreq = 0;
int storedFreq;

void increase(Generators *self) {
    currentFreq++;
    if (self->isRight) {
        int temp[] = {0, currentFreq};
        ASYNC(self->lcd, printAt, currentFreq*10 + 4);
    }
    else {
        ASYNC(self->lcd, printAt, currentFreq*10);
    }
    // Uppdatera frekvens för actual pulsgenerering
    
    
}

// The thing that happens when pulling joystick down
void decrease(Generators *self) {
    if (currentFreq > 0) {
        currentFreq--;
        if (self->isRight) {
            ASYNC(self->lcd, printAt, currentFreq*10 + 4);
        }
        else {
            ASYNC(self->lcd, printAt, currentFreq*10);
        }
        // Uppdatara frekvens (osäker om man ska calla SYNC eller ASYNC)
    }
}

// The thing that happens when pressing the joystick
void press(Generators *self) {
    if (currentFreq != 0) { // Maybe fix call later
        storedFreq = currentFreq;
        currentFreq = 1;
		ASYNC(self, decrease, NULL);
    }
    else {
        currentFreq = storedFreq + 1;
		ASYNC(self, decrease, NULL);
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