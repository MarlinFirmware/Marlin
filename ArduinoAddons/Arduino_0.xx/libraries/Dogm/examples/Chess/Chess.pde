/*

  Chess.pde
  
  A (extremly) simple chess program for the dogm128 library.
  
  (c) 2011 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 Arduino library.

  The dogm128 Arduino library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 Arduino library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
  Note:
    1) Set correct display hardware in Dogm/utility/dogm128.h
    2) Set top/bottom view (DOG_REVERSE) in Dogm/utility/dogm128.h
    See also: http://code.google.com/p/dogm128/wiki/install 
    
*/

#include <Dogm.h>

extern "C" {
void chess_Init(void);
void chess_Draw(void);
void chess_Step(uint8_t keycode);
}

// DOGS102 shield configuration values
#ifdef DOGS102_HW
uint8_t uiKeyPrev = 2;
uint8_t uiKeyNext = 4;
uint8_t uiKeySelect = 5;
uint8_t uiKeyBack = 3;
#else
// DOGM128-Shield	 configuration values
// DOGXL60-Shield configuration values
uint8_t uiKeyPrev = 7;
uint8_t uiKeyNext = 3;
uint8_t uiKeySelect = 2;
uint8_t uiKeyBack = 8;
#endif

uint8_t uiKeyCodeFirst = CHESS_KEY_NONE;
uint8_t uiKeyCodeSecond = CHESS_KEY_NONE;
uint8_t uiKeyCode = CHESS_KEY_NONE;


int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module
Dogm dogm(a0Pin);

void uiSetup(void) {
  // configure input keys 
  
  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyPrev, HIGH);       // turn on pullup resistors
  pinMode(uiKeyNext, INPUT);           // set pin to input
  digitalWrite(uiKeyNext, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelect, INPUT);           // set pin to input
  digitalWrite(uiKeySelect, HIGH);       // turn on pullup resistors
  pinMode(uiKeyBack, INPUT);           // set pin to input
  digitalWrite(uiKeyBack, HIGH);       // turn on pullup resistors
}

void uiStep(void)
{
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = CHESS_KEY_PREV;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = CHESS_KEY_NEXT;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = CHESS_KEY_SELECT;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = CHESS_KEY_BACK;
  else 
    uiKeyCodeFirst = CHESS_KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = CHESS_KEY_NONE;
}


void setup() {
  uiSetup();
  chess_Init();
}

void loop() {  
  uint8_t keyCode = CHESS_KEY_NONE;
  
  dogm.showLibInfo();

  dogm.start();
  do {
    chess_Draw();
    uiStep();
    if ( uiKeyCode != CHESS_KEY_NONE )
      keyCode = uiKeyCode;
  } while( dogm.next() );
  
  dog_Delay(10);
  chess_Step(keyCode);
  uiStep();
  keyCode = uiKeyCode;
}
