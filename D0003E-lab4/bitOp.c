/*
 * bitOp.c
 *
 * Created: 2025-03-11 15:24:58
 *  Author: anton
 */ 

#include <avr/io.h>
#include "bitOp.h"

void writeBit(BitOp *self){                                 // Stops recursion if freq hits 0
	if (self->isRight) {
		PORTE ^= (1 << PE6);                    // This and the one below generates actual pulse
	}
	else {
		PORTE ^= (1 << PE4);
	}
}