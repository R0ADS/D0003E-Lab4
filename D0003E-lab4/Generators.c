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
    // Uppdatera frekvens för actual pulsgenerering
    
    ASYNC(self->lcd, printAt, currentFreq);
}

void decrease(Generators *self) {
    if (currentFreq > 0) {
        currentFreq--;
        // Uppdatara frekvens
        ASYNC(self->lcd, printAt, currentFreq);
    }
}

// Basically reset
void press(Generators *self) {
    if (currentFreq != 0) {
        storedFreq = currentFreq;
        currentFreq = 0;
    }
    else {
        currentFreq = storedFreq;
    }
}







// This will be used to generate actual pulse: PORTE ^= (1 << PE4);