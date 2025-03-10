/*
 * Joystick.h
 *
 * Created: 2025-03-01 00:48:13
 *  Author: anton
 */ 

#include "TinyTimber.h"
#include "Gui.h"

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef struct{
	Object super;
	Gui *gui;
	//bool risingEdge;
	} Joystick;

#define initJoystick(gui) {initObject(), gui};

int joystickVert(Joystick *self);
int joystickHor(Joystick *self);


#endif /* JOYSTICK_H_ */