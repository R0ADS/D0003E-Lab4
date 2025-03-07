/*
 * Gui.h
 *
 * Created: 2025-03-05 11:10:35
 *  Author: anton
 */ 

#include "Gui.h"
#include "Lcd.h"
#include "Generators.h"
#include <avr/io.h>

#ifndef GUI_H_
#define GUI_H_

typedef struct {
    Generator *genL;
    Generator *genR;
} Gui;

#define initGui(leftGen, rightGen) {leftGen, rightGen};

void increase(Gui *self);
void decrease(Gui *self);
void switchGenerator(Gui *self);

#endif /* GUI_H_ */