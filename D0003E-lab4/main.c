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
	Lcd display = initLcd();
	BitOp bitL = initBitOp(0);
	BitOp bitR = initBitOp(1);
	Generators genL = initGenerator(&display, 0, &bitL);
	Generators genR = initGenerator(&display, 1, &bitR);
	Gui gui = initGui(&genL, &genR, &display);
	Joystick joy = initJoystick(&gui);
	InteruptHandler inter = initInteruptHandler(&joy);
	INSTALL(&inter, FreqChange, IRQ_PCINT1);
	INSTALL(&inter, genSwitch, IRQ_PCINT0);
	return tinytimber(&gui, start, NULL);
	//return tinytimber(NULL,NULL,NULL);		
}