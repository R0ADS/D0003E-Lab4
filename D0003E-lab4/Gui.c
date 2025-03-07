/*
 * Gui.c
 *
 * Created: 2025-03-05 11:10:25
 *  Author: anton
 */ 

#include <stdbool.h>
#include "Lcd.h"
#include "Generators.h"

int isRight = 1; // bool but int
long rightFreq = 0;
long leftFreq = 0;
long storedLeftFreq;
long storedRightFreq;

void increase(Gui *self) {
    if (isRight) {
        rightFreq++;
        ASYNC(self->lcdR, printAt, rightFreq);
    }
    else {
        leftFreq++;
        ASYNC(self->lcdL, printAt, leftFreq);
        ASYNC(self->genL, updateFreq, leftFreq);
    }
}

void decrease(Gui *self) {
    if (isRight && (rightFreq != 0)) {
        rightFreq--;
        ASYNC(self->lcdR, printAt, rightFreq);
    }
    else if (leftFreq != 0) {
        leftFreq--;
        ASYNC(self->lcdL, printAt, leftFreq);
    }
}

void switchGenerator(Gui *self) {
    ASYNC(self->lcdL, switchSegment, NULL);
    isRight = !(isRight);
}

void press(Gui *self){
    if (isRight) {
        ASYNC(self->lcdR, printAt, rightFreq);
    }
    else {
        leftFreq++;
        ASYNC(self->lcdL, printAt, leftFreq);
        ASYNC(self->genL, updateFreq, leftFreq);
    }
}
