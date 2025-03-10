/*
 * Gui.h
 *
 * Created: 2025-03-05 11:10:35
 *  Author: anton
 */ 

 
#include "Lcd.h"
#include "Generators.h"

#ifndef GUI_H_
#define GUI_H_

typedef struct {
	Object super;
    Generators *genL;
    Generators *genR;
	Lcd *lcd;
} Gui;

#define initGui(leftGen, rightGen, lcd) {initObject(), leftGen, rightGen, lcd};

void increaseGui(Gui *self);
void decreaseGui(Gui *self);
void switchGeneratorGui(Gui *self);
void pressGui(Gui *self);

#endif /* GUI_H_ */