/*
 * Generators.c
 *
 * Created: 2025-03-01 00:06:40
 *  Author: anton
 */ 
#include "generators.h"
#include "Lcd.h"
#include <avr/io.h>
long n = 0;

void test(Generators *self) {
   if (!(PINB & (1 << PB7))) {
       n=77;
       printAt(self->pos, n);
   }
   if (!(PINB & (1 << PB6))) {
       n=66;
       printAt(self->pos, n);
   }
}