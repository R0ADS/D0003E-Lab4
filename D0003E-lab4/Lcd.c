/*
 * LCD.c
 *
 * Created: 2025-03-01 02:10:12
 *  Author: anton
 */ 

 #include "Generators.h"
 #include "TinyTimber.h"
 #include "Lcd.h"
 #include <avr/io.h>
 //#include <util/delay.h>  // Testa att avkommentera om inte funkar
 #include <stdbool.h>
 
 bool isStalled = 0;
 
 const uint8_t characters[11][4] = {			// 2D array to keep characters in (e.g [0][x] is '0')
     [0] = {0b0001, 0b0101, 0b0101, 0b0001},
     [1] = {0b0000, 0b0001, 0b0001, 0b0000},
     [2] = {0b0001 ,0b0001, 0b1110, 0b0001},
     [3] = {0b0001, 0b0001, 0b1011, 0b0001},
     [4] = {0b0000, 0b0101, 0b1011, 0b0000},
     [5] = {0b0001 ,0b0100 ,0b1011 ,0b0001},
     [6] = {0b0001, 0b0100, 0b1111, 0b0001},
     [7] = {0b0001, 0b0001, 0b0001, 0b0000}, // This is option two (looks disgusting) --> {0b1001, 0b0000, 0b0000, 0b0100}
     [8] = {0b0001, 0b0101, 0b1111, 0b0001},
     [9] = {0b0001, 0b0101, 0b1011, 0b0001},
     [10] = {0b1001, 0b1111, 0b1111, 0b1111},// This spot is for the "invalid" character (also used for reset)
 };
 
 //Help function for writeChar (fills array with elements that match character)
 void displayChar(char c, bool odd, volatile uint8_t *address) {
     uint8_t array[4];
     if (c < '0' || c > '9') {	// Invalid character
         for (int i = 0; i < 4; i++) {
             array[i] = characters[10][i];
         }
     }
     else {						// Valid character
         for (int i = 0; i < 4; i++) {
             array[i] = characters[c-48][i];
         }
     }
     if (odd) {					// If odd digit, shifts in order to set correct digit
         for (int i = 0; i < 4; i++) {
             array[i] <<= 4;
         }
     }							// This part sets the character in needed registers
     *address |= array[0];		// (K, -, -, A)
     *(address+5) |= array[1];	// (J, F, H, B)
     *(address+10) |= array[2];	// (L, E, G, C)
     *(address+15) |= array[3];	// (M, P, N, D)
 }
 
 
 // Removes previous character at given location
 void removeOldChar(bool odd, volatile uint8_t *address) {
     // AND:s 0 on the parts that need to be reset
     if (odd) {
         *address &= 0b00001001;
         for (int i = 5; i < 20; i += 5) {
             *(address + i) &= 0b00001111;
         }
     }
     else {
         *address &= 0b10010000;
         for (int i = 5; i < 20; i += 5) {
             *(address + i) &= 0b11110000;
         }
     }
 }
 
 // Takes character ('0' - '9') and a position (0 - 5 (NOT CHECKED)) and prints it on the screen
 void writeChar(char c, int pos) {
     volatile uint8_t *address = &LCDDR0;	// Create pointer to start register
     if (pos % 2 == 0) {	// Even
         address += pos/2;					// Change address according to position
         removeOldChar(false, address);
         displayChar(c, false, address);
     }
     else {				// Odd
         address += (pos - 1) / 2;			// Change address according to position
         removeOldChar(true, address);
         displayChar(c, true, address);
     }
 };

// Switches between two segments 
void switchSegment() {
    LCDDR13 = !(LCDDR13 & 1);
    LCDDR18 = !(LCDDR18 & 1);
}
 
 // Displays last two digits of num in pos and pos+1
void printAt(Lcd *self, long num) {
	int pp = self->pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
 }

 void printAtArray(long input[]) {  // [pos, num]
	int pp = input[0];
    long num = input[1];
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
 }
