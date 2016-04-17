/*
 TMC26XMotorTest.pde - - TMC26X Stepper Tester for Processing
 
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
import controlP5.*;
import processing.serial.*;
import java.util.*;
import java.text.*;

ControlP5 controlP5;

Serial arduinoPort;

String helpUrl = "http://www.motioncontrol-community.org/";
String trinamicUrl = "http://trinamic.com";
String mcUrl = "http://www.motioncontrol-community.org/";

//TODO comde up with a nice color scheme
color activeColor = #01ADF1;
color foreGroundColor = #01ADF1; 
color uiTextColor = #4D4D4F;
color uiElementColor = #ffffff;
color labelColor = #f0f0f0;
color valueColor = #f0f0f0;
color backgroundColor = #EDEEEF;
color stallGuardColor = #969696;
color positionColor = #01ADF1;
color goodStatusColor = labelColor;
color badStatusColor = stallGuardColor;
color coolStepColor = #4D4D4F;
color diagramBackgroundColor = #ffffff;

CColor uiColor = new CColor( foreGroundColor, uiElementColor, activeColor, uiTextColor, uiTextColor);

Tab configureTab;
Tab runTab;
Tab activeTab;

boolean settingStatus=false;

boolean running = false;
int coolStepMin = 0;
int coolStepMax = 0;
boolean coolStepActive = false;

float maxCurrent = 1.7;

List controlElements = new LinkedList();
List serialConfigElements = new LinkedList();

PImage TMCLogo;
PImage MCLogo;

void setup() {
  size(1000, 800);
  //load the logos
  TMCLogo=loadImage("tmc_logo.jpg");
  MCLogo=loadImage("mc_logo.jpg");
  
  //create the UI
  controlP5 = new ControlP5(this);
  controlP5.setColor(uiColor);
  runTab = controlP5.getTab("default");
  configureTab =controlP5.addTab("configure"); 
  //customize the tabs a bit
  configureTab.setLabel("configure");
  controlElements.add(configureTab);
  activeTab =  controlP5.getTab("default");
  controlP5.setTabEventsActive(true);
  configureTab.activateEvent(true);
  runTab.activateEvent(true);

  //configuring the general UI l&f
  //the configuration UI

  setupRunConfig();
  setupChooperConfig();
  setupSerialConfig();
  //directly hide the controls again since we are not connected to the Arduino yet
  toggleUi(motor_connected);

  smooth();
  setupData();
}


void toggleUi(boolean show_controls) {
  for (Object c:controlElements) {
    ControllerInterface controller = (ControllerInterface) c;
    if (show_controls) {
      controller.show();
    } else {
      controller.hide();
    }
  }
  for (Object c:serialConfigElements) {
    ControllerInterface controller = (ControllerInterface) c;
    if (show_controls) {
      controller.hide();
    } else {
      controller.show();
    }
  }    
}

void draw() {
  background(backgroundColor);
  drawSerial();
  drawChopper();
  drawData();
  decodeSerial();
}


void controlEvent(ControlEvent theEvent) {
  if (theEvent.isGroup() && !settingStatus) {
    if ("microstepping".equals(theEvent.group().name())) { 
      microstepping((int)theEvent.group().value());
    } else 
    if ("direction".equals(theEvent.group().name())) {
      setDirection((int)theEvent.group().value());
    } else if ("decrement".equals(theEvent.group().name())) {
      setHysteresisDecrement((int)theEvent.group().value());
    } else if ("coolStepIncrement".equals(theEvent.group().name())) {
      setCoolStepIncrement((int)theEvent.group().value());
    } else if ("coolStepDecrement".equals(theEvent.group().name())) {
      setCoolStepDecrement((int)theEvent.group().value());
    } else if ("coolStepMin".equals(theEvent.group().name())) {
      setCoolStepMin((int)theEvent.group().value());
    }
  } 
  else if (theEvent.isTab()) {
    activeTab = theEvent.tab();
    println("Tab: "+activeTab.name());
  } 
}
