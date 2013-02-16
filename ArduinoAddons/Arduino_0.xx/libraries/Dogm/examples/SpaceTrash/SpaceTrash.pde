/*

  SpaceTrash
  
  A little game for dogm128 library graphics module.
  Optimized for the DOGM132 module.
  Will also run on an ATMEL ATTINY

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
      Assign up button pin to variable uiKeyUpPin. Default is DOGS102 Shield
      Assign down button pin to variable uiKeyDownPin. Default is DOGS102 Shield
      Assign fire button pin to variable uiKeyFirePin. Default is DOGS102 Shield
	  Remember to define DOGS102_HW
    additionally another button set can be define
      Assign up button pin to variable uiKeyUpPin2. Default is DOGM132 Shield
      Assign down button pin to variable uiKeyDownPin2. Default is DOGM132 Shield
      Assign fire button pin to variable uiKeyFirePin2. Default is DOGM132 Shield
	  Remember to define DOGM132_HW and DOG_REVERSE

  FPS
  Arduino Uno, FPS at end of 1st level
    DOGXL160_HW_GR		no 2x Mem		no REV			14 FPS
    DOGXL160_HW_GR		no 2x Mem		with REV			14 FPS
    DOGXL160_HW_GR		with 2x Mem		no REV			18 FPS
    DOGXL160_HW_GR		with 2x Mem		with REV			17 FPS
    DOGXL160_HW_BW		no 2x Mem		no REV			19 FPS
    DOGXL160_HW_BW		no 2x Mem		with REV			19 FPS
    DOGXL160_HW_BW		with 2x Mem		no REV			21 FPS
    DOGXL160_HW_BW		with 2x Mem		with REV			21 FPS
  
*/

#include <Dogm.h>


int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module
uint16_t sensorPin = 0;  // analog input


// frames per second
unsigned long next_sec_time;
uint8_t fps, frame_cnt;

// generic configuration values
uint8_t uiIncrement = 2;

// DOGS102 configuration values
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeyFirePin = 4;

// DOGM132 configuration values
uint8_t uiKeyUpPin2 = 7;
uint8_t uiKeyDownPin2 = 3;
uint8_t uiKeyFirePin2 = 2;

// output values
uint8_t shipLocation = 0;
uint8_t isAutoFire = 1;
uint8_t isFire = 0;

// internal values
uint8_t isKeyPad = 0;



// initialize the dogm library
Dogm dogm(a0Pin);

// setup the user interface
void uiSetup(void) {
  // configure input keys 
  
  pinMode(uiKeyUpPin, INPUT);           // set pin to input
  digitalWrite(uiKeyUpPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyDownPin, INPUT);           // set pin to input
  digitalWrite(uiKeyDownPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyFirePin, INPUT);           // set pin to input
  digitalWrite(uiKeyFirePin, HIGH);       // turn on pullup resistors

  pinMode(uiKeyUpPin2, INPUT);           // set pin to input
  digitalWrite(uiKeyUpPin2, HIGH);       // turn on pullup resistors
  pinMode(uiKeyDownPin2, INPUT);           // set pin to input
  digitalWrite(uiKeyDownPin2, HIGH);       // turn on pullup resistors
  pinMode(uiKeyFirePin2, INPUT);           // set pin to input
  digitalWrite(uiKeyFirePin2, HIGH);       // turn on pullup resistors
  
  
  // configure internal variables
  isKeyPad = 0;
  // assign some (more or less) useful values to the output variables
  shipLocation = 0;
}

// calculate new output values
void uiStep(void) {
  if ( isKeyPad == 0 )
  {
    int sensorValue;
    // assume, that no keypad is used and derive the location from analog input
    sensorValue = analogRead(sensorPin);
    sensorValue >>= 2;
    shipLocation = sensorValue;
    // now check if some keys are pressed
    if ( digitalRead(uiKeyUpPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyDownPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyFirePin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyUpPin2) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyDownPin2) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyFirePin2) == LOW )
      isKeyPad = 1;
  }
  if ( isKeyPad != 0 )
  {
    isAutoFire = 0;
    if ( digitalRead(uiKeyUpPin) == LOW || digitalRead(uiKeyUpPin2) == LOW )
      if ( shipLocation <= 255 -  uiIncrement )
	shipLocation += uiIncrement;
    if ( digitalRead(uiKeyDownPin) == LOW || digitalRead(uiKeyDownPin2) == LOW )
      if ( shipLocation >= 0+  uiIncrement )
	shipLocation -= uiIncrement;
    isFire = 0;
    if ( digitalRead(uiKeyFirePin) == LOW || digitalRead(uiKeyFirePin2) == LOW )
      isFire = 1;
  }
}

void setup() {
  uiSetup();
  next_sec_time = millis() + 1000UL;
  fps = 0;
  frame_cnt = 0;
}

void loop() {
  
  dogm.showLibInfo();
  
  dogm.start();
  do{
    st_Draw(fps);
  } while( dogm.next() );
  dog_Delay(10);
  uiStep();
  st_Step(shipLocation, isAutoFire, isFire);
  
  frame_cnt++;
  if ( next_sec_time < millis() )
  {
    fps = frame_cnt;
    frame_cnt = 0;
    next_sec_time = millis() + 1000UL;
  }
}


