/*
 * Generators.c
 *
 * Created: 2025-03-01 00:06:40
 *  Author: anton
 */ 
#include "generators.h"
#include "Lcd.h"
#include <avr/io.h>

PORTE ^= (1 << PE4);