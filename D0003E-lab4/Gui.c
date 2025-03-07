/*
 * Gui.c
 *
 * Created: 2025-03-05 11:10:25
 *  Author: anton
 */ 

#include <stdbool.h>
#include "Gui.h"

int isRight = 1; // bool but int

void increaseGui(Gui *self) {
    if (isRight) {
        ASYNC(self->genR, increase, NULL);
    }
    ASYNC(self->genL, increase, NULL);
}

void decreaseGui(Gui *self) {
    if (isRight) {
        ASYNC(self->genR, decrease, NULL);
    }
    ASYNC(self->genL, printAt, NULL);
}

void switchGeneratorGui(Gui *self) {
    ASYNC(self->lcd, switchSegment, NULL);
    isRight = !(isRight);
}

void pressGui(Gui *self) {
    if (isRight) {
        ASYNC(self->genR, press, NULL);
    }
    ASYNC(self->genL, press, NULL);
}
