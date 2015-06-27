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

Slider speedSlider;
Toggle runToggle;
RadioButton directionButtons;
Toggle enabledToggle;
RadioButton microsteppingButtons;
Slider sgtSlider;
Button sgtPlus;
Button sgtMinus;
Toggle sgFilterToggle;
Slider currentSlider;
Slider coolStepMinSlider;
Slider coolStepMaxSlider;
Toggle coolStepActiveToggle;
RadioButton coolStepIncrementButtons;
RadioButton coolStepDecrementButtons;
RadioButton coolStepMinButtons;
List runControls = new LinkedList();

Button trinamicButton;
Button motionControlButton;

void setupRunConfig() {
  //the run configuration
  //controlP5.getControlFont().setSize(10); - the font is too small, try to increase it!
  //add a button te let the motor run
  runToggle = controlP5.addToggle("run", false, 20, 40, 30, 30);
  controlElements.add(runToggle);
  runToggle.moveTo(runTab);
  //add some directions buttons
  directionButtons = controlP5.addRadioButton("direction", 20, 90);
  controlElements.add(directionButtons);
  directionButtons.addItem("forward", 1);
  directionButtons.addItem("backward", -1);
  directionButtons.activate(0);
  directionButtons.moveTo(runTab);
  enabledToggle = controlP5.addToggle("enabled", false, 20, 220, 30, 30);
  controlElements.add(enabledToggle);
  enabledToggle.moveTo(runTab);

  // add a vertical slider for speed  
  speedSlider = controlP5.addSlider("speed", 1, 100, 10, 85, 40, 20, 210);
  controlElements.add(speedSlider);
  speedSlider.moveTo(runTab);

  //ad a multilist for the microstepping setting
  microsteppingButtons = controlP5.addRadioButton("microstepping", 150, 40);
  controlElements.add(microsteppingButtons);
  microsteppingButtons.addItem("m_1/1", 1);
  microsteppingButtons.addItem("m_1/2", 2);
  microsteppingButtons.addItem("m_1/4", 4);
  microsteppingButtons.addItem("m_1/8", 8);
  microsteppingButtons.addItem("m_1/16", 16);
  microsteppingButtons.addItem("m_1/32", 32);
  microsteppingButtons.addItem("m_1/64", 64);
  microsteppingButtons.addItem("m_1/128", 128);
  microsteppingButtons.addItem("m_1/256", 256);
  for (Object o:microsteppingButtons.getItems()) {
    Toggle t = (Toggle) o;
    t.setCaptionLabel(t.getName().substring(2));
  }
  microsteppingButtons.showBar();
  microsteppingButtons.moveTo(runTab);



  currentSlider = controlP5.addSlider("current", 0.46, maxCurrent, 0.4, 250, 40, 20, 210);
  controlElements.add(currentSlider);
  currentSlider.moveTo(runTab);

  // add a vertical slider for stallGuard threshold  
  sgtPlus = controlP5.addButton("sgtplus", 0, 400, 40, 20, 20);
  controlElements.add(sgtPlus);
  sgtPlus.setCaptionLabel("+");
  sgtPlus.moveTo(runTab);
  sgtMinus = controlP5.addButton("sgtminus", 1, 400, 70, 20, 20);
  controlElements.add(sgtMinus);
  sgtMinus.setCaptionLabel("-");
  sgtMinus.moveTo(runTab);
  sgtSlider = controlP5.addSlider("stallguardthreshold", -64, 63, 0, 350, 40, 20, 150);
  controlElements.add(sgtSlider);
  sgtSlider.setSliderMode(Slider.FIX);
  sgtSlider.setCaptionLabel("Stall Guard Threshold");
  sgtSlider.moveTo(runTab);
  //ading some buttons to have finer sg control
  //adding a button for the filter
  sgFilterToggle = controlP5.addToggle("sgfilter", false, 350, 220, 30, 30);
  controlElements.add(sgFilterToggle);
  sgFilterToggle.setCaptionLabel("Stall GuardFilter");
  sgFilterToggle.moveTo(runTab);
  
  //add the coolstep sliders
  coolStepMaxSlider = controlP5.addSlider("coolStepUpper", 0, 480, 0, 500, 40, 20, 90);
  controlElements.add(coolStepMaxSlider);
  coolStepMaxSlider.setCaptionLabel("Cool Step Hysteresis");
  coolStepMaxSlider.moveTo(runTab);

  coolStepMinSlider = controlP5.addSlider("coolStepLower", 0, 480, 0, 500, 160, 20, 90);
  controlElements.add(coolStepMinSlider);
  coolStepMinSlider.setCaptionLabel("Cool Step Minimum");
  coolStepMinSlider.moveTo(runTab);
  
  coolStepActiveToggle = controlP5.addToggle("coolStepActive", false, 600, 220, 30, 30);  
  controlElements.add(coolStepActiveToggle);
  coolStepActiveToggle.setCaptionLabel("Enable CoolStep");
  coolStepActiveToggle.moveTo(runTab);

  coolStepIncrementButtons = controlP5.addRadioButton("coolStepIncrement", 600, 40);
  controlElements.add(coolStepIncrementButtons);
  coolStepIncrementButtons.captionLabel().set("Cool Step  Increment");
  coolStepIncrementButtons.addItem("i_1", 0);
  coolStepIncrementButtons.addItem("i_2", 1);
  coolStepIncrementButtons.addItem("i_4", 2);
  coolStepIncrementButtons.addItem("i_8", 3);
  for (Object o:coolStepIncrementButtons.getItems()) {
    Toggle t = (Toggle) o;
    t.setCaptionLabel(t.getName().substring(2));
  }
  coolStepIncrementButtons.showBar();
  coolStepIncrementButtons.moveTo(runTab);

  coolStepDecrementButtons = controlP5.addRadioButton("coolStepDecrement", 600, 110);
  controlElements.add(coolStepDecrementButtons);
  coolStepDecrementButtons.captionLabel().set("Cool Step Decrement");
  coolStepDecrementButtons.addItem("d_32", 0);
  coolStepDecrementButtons.addItem("d_8", 1);
  coolStepDecrementButtons.addItem("d_2", 2);
  coolStepDecrementButtons.addItem("d_1", 3);
  for (Object o:coolStepDecrementButtons.getItems()) {
    Toggle t = (Toggle) o;
    t.setCaptionLabel(t.getName().substring(2));
  }
  coolStepDecrementButtons.showBar();
  coolStepDecrementButtons.moveTo(runTab);

  coolStepMinButtons = controlP5.addRadioButton("coolStepMin", 600, 180);
  controlElements.add(coolStepMinButtons);
  coolStepMinButtons.addItem("s_1/2", 0);
  coolStepMinButtons.addItem("s_1/4", 1);
  for (Object o:coolStepMinButtons.getItems()) {
    Toggle t = (Toggle) o;
    t.setCaptionLabel(t.getName().substring(2));
  }
  coolStepMinButtons.showBar();
  coolStepMinButtons.moveTo(runTab);
  
  trinamicButton = controlP5.addButton("trinamicLogo", 1.0, 750, 40, 200, 100);
  trinamicButton.setImage(TMCLogo);
  trinamicButton.moveTo(runTab);
  controlElements.add(trinamicButton);
  
  motionControlButton = controlP5.addButton("mcLogo", 1.0, 750, 150, 200, 100);
  motionControlButton.setImage(MCLogo);
  motionControlButton.moveTo(runTab);
  controlElements.add(motionControlButton);
}

void speed(int theSpeed) {
  if (!settingStatus) {
    int speed = (int) theSpeed;
    println("setting speed to "+speed);
    sendCommand("S"+speed);
  }
}

void run(int value) {
  if (!settingStatus) {
    println("button pressed");
    if (running) {
      println("stopping motor");
      sendCommand("s");
      running = false;
    } 
    else {
      println("starting motor");
      sendCommand("r");
      running = true;
    }
  }
}

void enabled(int value) {
  if (!settingStatus) {
    println("enabled: "+value);
    sendCommand("e"+value);
  }
}

void microstepping(int value) {
  if (!settingStatus) {
    println("microstepping: "+value);
    sendCommand("m"+value);
  }
}

void stallguardthreshold(int value) {
  if (!settingStatus) {
    println("stall guard threshold: "+value);
    sendCommand("t"+value);
  }
  if (value==sgtSlider.max()) {
    sgtPlus.lock();
  } 
  else {
    sgtPlus.unlock();
  }
  if (value==sgtSlider.min()) {
    sgtMinus.lock();
  } 
  else {
    sgtMinus.unlock();
  }
}

void sgtplus(int value) {
  sgtSlider.setValue(sgtSlider.value()+1);
}

void sgtminus(int value) {
  sgtSlider.setValue(sgtSlider.value()-1);
}

void sgfilter(int value) {
  if (!settingStatus) {
    println("filter: "+value);
    sendCommand("f"+value);
  }
}  

void current(float value) {
  if (!settingStatus) {
    int realValue=(int)(value*1000.0);
    println("current: "+((float)realValue/1000.0)+" = "+realValue);
    sendCommand("c"+realValue);
    if (activeTab!=null && "run".equals(activeTab.name())) {
      motorCurrentBox.setValue(value);
    }
  }
}

void coolStepUpper(int value) {
  coolStepMax=value;
  if (!settingStatus) {
    sendCommand("Ku"+value);
  }
}

void coolStepLower(int value) {
  coolStepMin = value;
  if (!settingStatus) {
    sendCommand("Kl"+value);
  }
}


void setCoolStepIncrement(int value) {
  if (!settingStatus) {
    println("cool step increment :"+value);
    sendCommand("Ki"+value);
  }
}

void setCoolStepDecrement(int value) {
  if (!settingStatus) {
    println("cool step decrement :"+value);
    sendCommand("Kn"+value);
  }
}

void setCoolStepMin(int value) {
  if (!settingStatus) {
    println("cool step minimum :"+value);
    sendCommand("Km"+value);
  }
}

void coolStepActive(int value) {
  if (!settingStatus) {
    coolStepActive = (value!=0);
    sendCommand(coolStepActive? "K+":"K-");
  }
}

void setCurrent(int current) {
  currentSlider.setValue((float)current/1000.0);
}

void setDirection(int direction) {
  if (!settingStatus) {
    if (direction<0) {
      println("back");
      sendCommand("d-1");
    } 
    else {
      sendCommand("d1");
    }
  }
}

void trinamicLogo(float value){
  if (value!=0) {
    link(trinamicUrl);
  }
}

void mcLogo(float value) {
  if (value!=0) {
    link(mcUrl);
  }
}

