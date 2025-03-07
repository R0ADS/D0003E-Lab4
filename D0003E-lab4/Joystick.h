/*
 * Joystick.h
 *
 * Created: 2025-03-01 00:48:13
 *  Author: anton
 */ 

#include <stdbool.h>
#include "TinyTimber.h"
#include "Gui.h"

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef struct{
	Gui *gui;
	bool risingEdge;
	} Joystick;

#define initJoystick(gui) {gui};

int joystickvert(Joystick *self);
int joyStickWhore(Joystick *self);


#endif /* JOYSTICK_H_ */