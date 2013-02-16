/*

  Breakorino
  
  A little game for dogm128 graphics module.

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
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
    
  User Interface:
    Analog Potentiometer: 
      Assign pin number to variable "sensorPin". Default is analog pin 0.
    Digital Input:
      Assign left button pin to variable uiKeyLeftPin. Default is DOGS102 Shield
      Assign right button pin to variable uiKeyRightPin. Default is DOGS102 Shield
  
  FPS (in game)
    DOGXL160_HW_GR, no DOG_REVERSE, no DOG_DOUBLE_MEMORY		16 FPS
    DOGXL160_HW_GR no DOG_REVERSE, with DOG_DOUBLE_MEMORY		22 FPS
    DOGXL160_HW_BW, no DOG_REVERSE, no DOG_DOUBLE_MEMORY		24 FPS
    DOGXL160_HW_BW, no DOG_REVERSE, with DOG_DOUBLE_MEMORY		28 FPS
    
  
*/

#include <Dogm.h>

#ifdef DOGM132_HW
#error Breakorino can not be used on a DOGM132 display
#endif

unsigned long next_sec_time;
uint8_t fps, frame_cnt;

int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module
int sensorPin = 0;  // analog input

// configuration values
uint8_t uiKeyLeftPin = 4;
uint8_t uiKeyRightPin = 2;
uint8_t uiMinIncrement = 2*8;
uint8_t uiMaxIncrement = 4*8;

// output values
uint8_t playerLocation = 0;

// internal values
uint8_t isKeyPad = 0;
uint8_t uiActualIncrement = 2*8;

// initialize the dogm library
Dogm dogm(a0Pin);

// setup the user interface
void uiSetup(void) {
  // configure input keys 
  pinMode(uiKeyLeftPin, INPUT);           // set pin to input
  digitalWrite(uiKeyLeftPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyRightPin, INPUT);           // set pin to input
  digitalWrite(uiKeyRightPin, HIGH);       // turn on pullup resistors
  // configure internal variables
  isKeyPad = 0;
  // assign some (more or less) useful values to the output variables
  playerLocation = 127;
}

// calculate new output values
void uiStep(void) {
  uint8_t is_key_pressed = 0;
  uint8_t real_inc;
  if ( isKeyPad == 0 )
  {
    int sensorValue;
    // assume, that no keypad is used and derive the location from analog input
    sensorValue = analogRead(sensorPin);
    sensorValue >>= 2;
    playerLocation = sensorValue;
    // now check if some keys are pressed
    if ( digitalRead(uiKeyLeftPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyRightPin) == LOW )
      isKeyPad = 1;
  }
  if ( isKeyPad != 0 )
  {
    real_inc = uiActualIncrement;
    real_inc >>= 3;
    if ( digitalRead(uiKeyLeftPin) == LOW )
    {
      is_key_pressed = 1;
      if ( playerLocation <= 255 -  real_inc )
	playerLocation += real_inc;
    }
    if ( digitalRead(uiKeyRightPin) == LOW )
    {
      is_key_pressed = 1;
      if ( playerLocation >= 0+  real_inc )
	playerLocation -= real_inc;
    }
    if ( is_key_pressed == 0 )
    {
      uiActualIncrement = uiMinIncrement;
    }
    else
    {
      if ( uiActualIncrement < uiMaxIncrement )
	uiActualIncrement++;
    }
  }
}


void setup() {
  uiSetup();
  bo_Setup(0);
  next_sec_time = millis() + 1000UL;
  fps = 0;
  frame_cnt = 0;
}

void loop() {
  dogm.start();
  do{
    bo_Draw();
    bo_DrawFPS(fps);
  } while( dogm.next() );
  dog_Delay(1);
  uiStep();
  bo_SetX(playerLocation);
  bo_Step();  
  
  frame_cnt++;
  if ( next_sec_time < millis() )
  {
    fps = frame_cnt;
    frame_cnt = 0;
    next_sec_time = millis() + 1000UL;
  }
}


