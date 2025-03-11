/*
 * Gui.c
 *
 * Created: 2025-03-05 11:10:25
 *  Author: anton
 */ 

#include <stdbool.h>
#include "Gui.h"


// NOTE: The main purpose of this object is to distinguish current display segment and call methods accordingly
void increaseGui(Gui *self) {
    if (self->isRight) {
        ASYNC(self->genR, increase, NULL);
    }
    else {
        ASYNC(self->genL, increase, NULL);
    }
}

void decreaseGui(Gui *self) {
    if (self->isRight) {
        ASYNC(self->genR, decrease, NULL);
    }
    else {
        ASYNC(self->genL, printAt, NULL);
    }
}

void switchGeneratorGui(Gui *self) {
    ASYNC(self->lcd, switchSegment, NULL);
    self->isRight = !(self->isRight);
}

void pressGui(Gui *self) {
    if (self->isRight) {
        ASYNC(self->genR, press, NULL);
    }
    else {
        ASYNC(self->genL, press, NULL);
    }
}

void start(Gui *self){
	ASYNC(self->genL, generatePulse, NULL);
	ASYNC(self->genR, generatePulse, NULL);
}