/*
 * HDSP-2490 LED display test code
 *
 * Copyright (C) 2017 Chris Taylor
 *
 * This program is free software: you can redistribute it and/or modify
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "font.h"

const char sample [][5] = {
  "HDSP","2490","Test"
};
const int lines = 3;

int allOnTime = 400;
int onTime = 1;
int offTime = 0;
        
const int numBits = 28;

const int Vb    = 7;         // Display enable
const int Clk   = 8;         // Clock signal
const int DI    = 9;         // Data in
const int colCount = 5;
const int rowCount = 7;

int cols[] = { 2,3,4,5,6 };  // Led column pins

void setup() {
  for (int i = 0; i < colCount; i++) {
    pinMode(cols[i], OUTPUT);
  }
  pinMode(Vb, OUTPUT);
  pinMode(Clk, OUTPUT);
  pinMode(DI, OUTPUT);

  // Turn off all 5 column bits  
  for (int c = 0; c < colCount; c++) {
    digitalWrite(cols[c], LOW);
  }

  // display some text
  for (int i=0; i<lines; i++) {
    for (int j = 0; j < allOnTime; j++) {
      printChars(sample[i]);
    }
  }

  // Feed in 28 bits of data (7 row bits x 4 digits)
  for (int i = 0; i < numBits; i++) {
    digitalWrite(Clk, HIGH);
    digitalWrite(DI, HIGH); // all leds ON
    digitalWrite(Clk, LOW);
  }
  digitalWrite(Vb, LOW);
  // Turn on all 5 column bits  
  for (int c = 0; c < colCount; c++) {
    digitalWrite(cols[c], HIGH);
  }

  offTime = 2; // lower intensity for all LEDS on
}

void loop() {
  digitalWrite(Vb, HIGH);
  delay(onTime);
  digitalWrite(Vb, LOW);
  delay(offTime);
}

void printChars(const char s[])
{
  for (int c=0; c<colCount; ++c) {
    for (int i=3; i>=0; i--) {
      char ch = s[i];
      // ensure ch is in bounds
      if (ch < 32 || ch > 126) {
        ch = 32;
      }
      // convert ASCII code to font index)
      ch -= 32;
      byte b = font[ch][c];
      // bit shift through byte and output to DI pin
        for (int k=rowCount-1; k>=0; k--) {
          digitalWrite(Clk, HIGH);
          digitalWrite(DI, !!(b & (1 << k)));
          digitalWrite(Clk, LOW);
      }
    }
    digitalWrite(cols[c], HIGH);
    digitalWrite(Vb, HIGH);
    delay(onTime);
    digitalWrite(Vb, LOW);
    delay(offTime);
    digitalWrite(cols[c], LOW);
  }
}
