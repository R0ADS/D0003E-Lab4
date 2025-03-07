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
    int pos;
} Lcd;

#define initLcd(pos) {pos}

void switchSegment(void);
void printAt(Lcd *self, long);

#endif /* LCD_H_ */

