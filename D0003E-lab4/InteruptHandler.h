/*
 * InteruptHandler.h
 *
 * Created: 2025-03-01 00:07:49
 *  Author: anton
 */ 

#include <stdbool.h>
#include "Joystick.h"
#include "TinyTimber.h"

#ifndef INTERUPTHANDLER_H_
#define INTERUPTHANDLER_H_

typedef struct {
	Object super;
	Joystick *Joystick;
	Gui *gui;
} InteruptHandler;

#define initInteruptHandler(Joystick) { initObject(), Joystick}

void genSwitch	(InteruptHandler *self);
void FreqChange (InteruptHandler *self);

#endif /* INTERUPTHANDLER_H_ */