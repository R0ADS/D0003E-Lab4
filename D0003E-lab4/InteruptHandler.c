/*
 * InteruptHandler.c
 *
 * Created: 2025-03-01 00:07:40
 *  Author: anton
 */ 

#include <avr/io.h>
#include "InteruptHandler.h"
#include "TinyTimber.h"

void horizontalButton() {
    
}

int isUp() {
    if (!(PINB & (1 << PB7))) { // Up
        return 1;
    }
    if (!(PINB & (1 << PB6))) { // Down
        return 0;
    }
}


