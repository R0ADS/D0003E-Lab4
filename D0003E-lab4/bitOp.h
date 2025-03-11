/*
 * bitOp.h
 *
 * Created: 2025-03-11 15:25:11
 *  Author: anton
 */ 

#include "TinyTimber.h"

#ifndef BITOP_H_
#define BITOP_H_

typedef struct{
	Object super;
	int isRight;
} BitOp;

#define initBitOp() {initObject()}

void writeBit(BitOp *self, int isRight);

#endif /* BITOP_H_ */