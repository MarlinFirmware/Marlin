/*
 TMC26XMotorTest.ino - - TMC26X Stepper library Example for Wiring/Arduino
 
 Copyright (c) 2011, Interactive Matter, Marcus Nowotny
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */
#include <SPI.h>
#include <TMC26XStepper.h>

//you may adapt this to your shield or breakout board connection
#define CS_PIN 2
#define DIR_PIN 6
#define STEP_PIN 7
#define ENABLE_PIN 8 //if it is not connected it won't be a problem


#define TIMER_CLOCK_FREQ 2000000.0 //2MHz for /8 prescale from 16MHz
#define INITIAL_CURRENT 500 //in mA
#define MAX_SPEED 1000


//we have a stepper motor with 200 steps per rotation, CS pin 2, dir pin 3, step pin 4 and a current of 300mA
TMC26XStepper tmc26XStepper = TMC26XStepper(200,CS_PIN,DIR_PIN,STEP_PIN,INITIAL_CURRENT);
char running;

void setup() {
  //configure the enable pin
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN,HIGH);

  startSerial();
  startMotor();
  //set this according to you stepper
  Serial.println(F("started"));
}

void loop() {
  loopSerial();
  runMotor();
}



