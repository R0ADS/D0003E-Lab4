/*
 * D0003E-lab4.c
 *
 * Created: 2025-02-24 15:52:04
 * Author : Antbin
 */ 

#include <stdbool.h>
#include <avr/io.h>
#include "Generators.h"
#include "Gui.h"
#include "Init.h"
#include "InteruptHandler.h"
#include "Joystick.h"
#include "Lcd.h"
#include "TinyTimber.h"


int main() {
	Init();
	Lcd displayL = initLcd(0);
	Lcd displayR = initLcd(4);
	Generators genL = initGenerator(&displayL);
	Generators genR = initGenerator(&displayR);
	Gui gui = initGui(&genL, &genR);
	Joystick joy = initJoystick(&gui);
	InteruptHandler inter = initInteruptHandler(&joy);
	INSTALL(&inter, FreqChange, IRQ_PCINT1);
	INSTALL(&inter, genSwitch, IRQ_PCINT0);
	return tinytimber(NULL,NULL,NULL);		
}