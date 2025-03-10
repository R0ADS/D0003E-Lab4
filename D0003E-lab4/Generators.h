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

typedef struct {
    Object super;
    int currentFreq;
    int storedFreq;
    int isRight;
    Lcd *lcd;
} Generators;

void decrease(Generators *self);
void increase(Generators *self);
void press(Generators *self);

#define initGenerator(lcd, isRight) {initObject(), 0, 0, isRight, lcd};

#endif /* GENERATORS_H_ */