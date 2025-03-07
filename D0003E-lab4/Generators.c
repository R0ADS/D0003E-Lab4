/*
 * Generators.c
 *
 * Created: 2025-03-01 00:06:40
 *  Author: anton
 */ 
#include "Generators.h"
#include "Lcd.h"

long currentFreq = 0;
long storedFreq;

void increase(Generators *self) {
    currentFreq++;
    if (self->isRight) {
        ASYNC(self->lcd, printAt, {0, currentFreq});
    }
    else {
        ASYNC(self->lcd, printAt, {4, currentFreq});
    }
    // Uppdatera frekvens för actual pulsgenerering
    
    
}

// The thing that happens when pulling joystick down
void decrease(Generators *self) {
    if (currentFreq > 0) {
        currentFreq--;
        if (self->isRight) {
            ASYNC(self->lcd, printAt, {0, currentFreq});
        }
        else {
            ASYNC(self->lcd, printAt, {4, currentFreq});
        }
        // Uppdatara frekvens (osäker om man ska calla SYNC eller ASYNC)
    }
}

// The thing that happens when pressing the joystick
void press(Generators *self) {
    if (currentFreq != 0) {
        storedFreq = currentFreq;
        currentFreq = 0;
    }
    else {
        currentFreq = storedFreq;
    }
}

// Generates the pulse
void generatePulse(Generators *self, freq) {
    if (freq > 0) {                                 // Stops recursion if freq hits 0
        if (self->isRight) {
            PORTE ^= (1 << PE6);                    // This and the one below generates actual pulse
        }
        else {
            PORTE ^= (1 << PE4);
        }
    AFTER(MSEC(1000/freq), self, generatePulse)     // Calls itself according to freq
    }
}