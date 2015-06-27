/*
 TMC26XExample.ino - - TMC26X Stepper library Example for Wiring/Arduino
 
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

//we have a stepper motor with 200 steps per rotation, CS pin 2, dir pin 6, step pin 7 and a current of 300mA
TMC26XStepper tmc26XStepper = TMC26XStepper(200,2,6,7,700);
int curr_step;
int speed =  0;
int speedDirection = 100;
int maxSpeed = 1000;

void setup() {
  Serial.begin(9600);
  Serial.println("==============================");
  Serial.println("TMC26X Stepper Driver Demo App");
  Serial.println("==============================");
  //set this according to you stepper
  Serial.println("Configuring stepper driver");
  //char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement
  tmc26XStepper.setSpreadCycleChopper(2,24,8,6,0);
  tmc26XStepper.setRandomOffTime(0);
  
  tmc26XStepper.setMicrosteps(32);
  tmc26XStepper.setStallGuardThreshold(4,0);
  Serial.println("config finished, starting");
  tmc26XStepper.start();
  Serial.println("started");
}

void loop() {
  if (!tmc26XStepper.isMoving()) {
    speed+=speedDirection;
    if (speed>maxSpeed) {
      speed = maxSpeed;
      speedDirection = -speedDirection;
    } else if (speed<0) {
      speedDirection = -speedDirection;
      speed=speedDirection;
    }
    //setting the speed
    Serial.print("setting speed to ");
    Serial.println(speed);
    tmc26XStepper.setSpeed(speed);
    //we want some kind of constant running time - so the length is just a product of speed
    Serial.print("Going ");
    Serial.print(10*speed);
    Serial.println(" steps");
    tmc26XStepper.step(10*speed);
  } else {
    //we put out the status every 100 steps
    if (tmc26XStepper.getStepsLeft()%100==0) {
      Serial.print("Stall Guard: ");
      Serial.println(tmc26XStepper.getCurrentStallGuardReading());
    }    
  }  
  tmc26XStepper.move();
}


