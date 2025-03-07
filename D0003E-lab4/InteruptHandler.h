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
	//Object super;
	Joystick *Joystick;
	bool risingEdge;
} InteruptHandler;

#define initInteruptHandler(Joystick) { initObject(), Joystick, false}

void genSwitch	(InteruptHandler *self);
void freqChange (InteruptHandler *self);

#endif /* INTERUPTHANDLER_H_ */