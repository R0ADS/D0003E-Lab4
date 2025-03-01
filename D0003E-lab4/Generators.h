/*
 * Generators.h
 *
 * Created: 2025-03-01 00:06:52
 *  Author: anton
 */ 


#ifndef GENERATORS_H_
#define GENERATORS_H_

#include <stdint.h>
#include "TinyTimber.h"

typedef struct {
    Object super;
    int pos;
} Generators;

#define initGenerator(pos) {pos}

void test(Generators *self);

#endif /* GENERATORS_H_ */