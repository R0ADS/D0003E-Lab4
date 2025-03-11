/*
 * Generators.h
 *
 * Created: 2025-03-01 00:06:52
 *  Author: anton
 */ 


#ifndef GENERATORS_H_
#define GENERATORS_H_

#include "TinyTimber.h"
#include "Lcd.h"
#include "bitOp.h"

typedef struct {
    Object super;
    int currentFreq;
    int storedFreq;
    int risingEdge;
    int isRight;
    Lcd *lcd;
	BitOp *bitOp;
} Generators;

void decrease(Generators *self);
void increase(Generators *self);
void press(Generators *self);
void generatePulse(Generators *self);

#define initGenerator(lcd, isRight, bitOp) {initObject(), 0, 0, 1, isRight, lcd, bitOp};

#endif /* GENERATORS_H_ */