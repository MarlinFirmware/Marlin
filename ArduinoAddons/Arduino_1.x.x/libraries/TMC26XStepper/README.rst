Arduino TMC26X Stepper Motor Controller Library
===============================================

License
-------

TMC26XStepper.cpp - - TMC 260/261/262 Stepper library for Wiring/Arduino
 
based on the stepper library by Tom Igoe, et. al.
 
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

About
-----

The TMC26X is a stepper motor controller for bipolar stepper motors. From the trinamic web site:

 The TMC262 is the first energy efficient high current high precision microstepping driver 
 IC for bipolar stepper motors. The unique high resolution sensorless load detection stallGuard2™ 
 is used to for the world’s first integrated load dependent current control feature called coolStep™.
 The ability to read out the load and detect an overload makes the TMC262 an optimum choice for 
 drives where a high reliability is desired at a low cost. The new patented spreadCycle PWM mixed 
 decay chopper scheme ensures best zero crossing performance as well as high speed operation. 
 The TMC262 can be driven with Step & Direction signals as well as by serial SPI™ interface. 
 Using the microPlyer allows to operate the motor with highest 256 μStep smoothness reducing the 
 input frequency to 16 μSteps. A full set of protection and diagnostic features makes this device 
 very rugged. It directly drives external MOSFETs for currents of up to 6A. This way it reaches 
 highest energy efficiency and allows driving of a high motor current without cooling measures 
 even at high environment temperatures.


The unique features of the TMC26X are that everything can (and must) be controlled in software:

* the motor current
* microstepping
* stall protection
* current reduction according to load
* stallGuard2™ sensorless load detection
* coolStep™ load dependent current control
* spreadCycle hysteresis PWM chopper 
* microPlyer 16-to-256 μStep multiplier
* full protection and diagnostics

This makes the TMC26X a bit harder to use than other stepper motors but much more versatile.
This library resolves all the complicated stuff so that you can use TMC26X straight away.
Furthermore, all the settings are implemented in high level interfaces so that configuring your
motor is a breeze.

How to use
----------

Check out the Setup Guide here: 
And the How To here: