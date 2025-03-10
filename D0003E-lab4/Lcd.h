/*
 * lcd.h
 *
 * Created: 2025-03-01 02:10:31
 *  Author: anton
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "TinyTimber.h"

typedef struct {
    Object super;
} Lcd;

#define initLcd() {initObject()}

void switchSegment(void);
int  printAt(Lcd *self, int input);

#endif /* LCD_H_ */

