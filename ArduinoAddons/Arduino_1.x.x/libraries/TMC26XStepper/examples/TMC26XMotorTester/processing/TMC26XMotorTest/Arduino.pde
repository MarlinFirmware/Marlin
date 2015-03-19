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

String channelAStatus=null;
String channelBStatus=null;
String temperatureStatus=null;
boolean motor_connected = false;

RadioButton serialButtons;
Button serialOkButton;
Button helpButton;
Textarea statusArea;
String[] ports;
int activePortIndex = -1;

String identString="TMC26X Stepper Driver Motor Tester";
int connectTimeout=10*1000; //how long do we wait until the Arduino is connected

StringBuilder serialStringBuilder = new StringBuilder();

void setupSerialConfig() {
  Tab defaultTab = controlP5.getTab("default");
  //add the list of serial interfaces - it get's populated later
  serialButtons = controlP5.addRadioButton("serialport", 200, 100+TMCLogo.height*2+50);
  serialConfigElements.add(serialButtons);
  serialButtons.captionLabel().set("Select Serial Port");
  serialButtons.showBar();
  serialButtons.moveTo(defaultTab);
  //ad the ok button
  serialOkButton = controlP5.addButton("serialOk", 1, 200, height-300, 30, 30);
  serialConfigElements.add(serialOkButton);
  serialOkButton.setCaptionLabel("OK");
  runToggle.moveTo(defaultTab);
  //add the status area
  statusArea = controlP5.addTextarea("statusArea","",200,height-250,300,50);
  serialConfigElements.add(statusArea);
  statusArea.moveTo(defaultTab);
  
  helpButton =  controlP5.addButton("help", 1, 200, height-250, 80, 30);
  serialConfigElements.add(helpButton);
  helpButton.moveTo(defaultTab);
  

  //finally update the list of serial ports
  updateSerialPortList();
}

void updateSerialPortList() {
  //first remove all present serial ports
  List items = serialButtons.getItems();
  for (Object i:items) {
    Toggle item = (Toggle) i;
    serialButtons.removeItem(item.getName());
  }
  //add the serial ports
  ports = Serial.list();
  for (int i=0; i< ports.length; i++) {
    serialButtons.addItem(ports[i],i);
  }
  serialButtons.setValue(-1);
  serialOkButton.setVisible(false);
}

void serialport(int value) {
  //ok button is only active if a serial port is selected
  serialOkButton.setVisible(value>-1);
  if (value>-1) {
    statusArea.setText("");
  }
  activePortIndex = value;
}

void serialOk(int value) {
  String error = null;
  if (value!=0 && activePortIndex>-1) {
    try {
      arduinoPort = new Serial(this, ports[activePortIndex], 115200);
      int timeStarted = millis();
      StringBuilder identBuffer = new StringBuilder();
      while (!motor_connected && (millis()-timeStarted)<connectTimeout) {
        if (arduinoPort.available ()>0) {
          char c = arduinoPort.readChar();
          identBuffer.append(c);
          if (c=='\n') {
            if (identString.contains(identString)) {
              motor_connected = true;
              toggleUi(true);
              return;
            }
            identBuffer = new StringBuilder();
          }
        } 
      }
    } catch (RuntimeException e) {
      //we simply do nothing
      //TODO set status label
      error = "There was a problem with serial port "+ports[activePortIndex]+": "+e.getMessage();
    }
    //ok appearantly we did not find an motor tester - so lets deselect that port
    if (error == null) {
      error = "Could not find TMC26XMotorTester on serial port "+ports[activePortIndex];
    }
    statusArea.setText(error);
    Toggle selected = serialButtons.getItem(activePortIndex);
    selected.setState(false);
    serialOkButton.setVisible(false);
  }
}


void decodeSerial() {
  if (motor_connected) {
    while (arduinoPort.available ()>0) {
      char c = arduinoPort.readChar();
      serialStringBuilder.append(c);
      if (c=='\n') {
        decodeSerial(serialStringBuilder.toString());
        serialStringBuilder = new StringBuilder();
      }
    }
  }
}

void sendCommand(String command) {
  if (motor_connected) {
    arduinoPort.write(command+"\n");
  }
}

void decodeSerial(String line) {
  settingStatus=true;
  if (line.startsWith("#")) {
    String status = line.substring(1);
    StringTokenizer statusTokenizer = new StringTokenizer(status, ",");
    while (statusTokenizer.hasMoreTokens ()) {
      String statusToken = statusTokenizer.nextToken();
      if ("s".equals(statusToken)) {
        runToggle.setValue(0);
      } 
      else if ("r".equals(statusToken)) {
        runToggle.setValue(1);
      } 
      else if (statusToken.startsWith("e")) {
        int enabled = getValueOfToken(statusToken, 1);
        if (enabled!=0) {
          enabledToggle.setValue(1);
        } 
        else {
          enabledToggle.setValue(0);
        }
      }
      else if (statusToken.startsWith("S")) {
        speedSlider.setValue(getValueOfToken(statusToken, 1));
      } 
      else if (statusToken.startsWith("m")) {
        microsteppingButtons.activate("m_1/"+String.valueOf(getValueOfToken(statusToken, 1)));
      } 
      else if (statusToken.startsWith("sg")) {
        addStallGuardReading(getValueOfToken(statusToken, 2));
      } 
      else if (statusToken.startsWith("p")) {
        addPositionReading(getValueOfToken(statusToken, 1));
      } 
      else if (statusToken.startsWith("k")) {
        addCurrentReading(getValueOfToken(statusToken, 1));
      } 
      else if (statusToken.startsWith("t")) {
        sgtSlider.setValue(getValueOfToken(statusToken, 1));
      } 
      else if (statusToken.startsWith("f")) {
        sgFilterToggle.setValue(getValueOfToken(statusToken, 1));
      } 
      else if (statusToken.startsWith("d")) {
        setDirection(getValueOfToken(statusToken, 1));
      }
      else if (statusToken.startsWith("c")) {
        setCurrent(getValueOfToken(statusToken, 1));
      } 
      else if (statusToken.startsWith("a")) {
        if (statusToken.charAt(1)=='o') {
          channelAStatus="Open Load";
        } 
        else if (statusToken.charAt(1)=='g') {
          channelAStatus="Short to Ground!";
        } 
        else {
          channelAStatus=null;
        }
      } 
      else if (statusToken.startsWith("b")) {
        if (statusToken.charAt(1)=='o') {
          channelBStatus="Open Load";
        } 
        else if (statusToken.charAt(1)=='g') {
          channelBStatus="Short to Ground!";
        } 
        else {
          channelBStatus=null;
        }
      } 
      else if (statusToken.startsWith("x")) {
        if (statusToken.charAt(1)=='w') {
          temperatureStatus="Prewarning!";
        } 
        else if (statusToken.charAt(1)=='e') {
          temperatureStatus="Error";
        } 
        else {
          temperatureStatus=null;
        }
      }
      else if (statusToken.startsWith("Cm")) {
        //chopper mode is currently ignored
      } 
      else if (statusToken.startsWith("Co")) {
        constantOffSlider.setValue(getValueOfToken(statusToken, 2));
      }  
      else if (statusToken.startsWith("Cb")) {
        blankTimeSlider.setValue(getValueOfToken(statusToken, 2));
      } 
      else if (statusToken.startsWith("Cs")) {
        hysteresisStartSlider.setValue(getValueOfToken(statusToken, 2));
      } 
      else if (statusToken.startsWith("Ce")) {
        hysteresisEndSlider.setValue(getValueOfToken(statusToken, 2));
      } 
      else if (statusToken.startsWith("Cd")) {
        setHystDecrement(getValueOfToken(statusToken, 2));
      } 
      else if ("Ke+".equals(statusToken)) {
        coolStepActiveToggle.setValue(1);
      } 
      else if ("Ke-".equals(statusToken)) {
        coolStepActiveToggle.setValue(0);
      } 
      else if (statusToken.startsWith("Kl")) {
        coolStepMinSlider.setValue(getValueOfToken(statusToken, 2));
      } 
      else if (statusToken.startsWith("Ku")) {
        coolStepMaxSlider.setValue(getValueOfToken(statusToken, 2));
      }
      else if (statusToken.startsWith("Kn")) {
        coolStepDecrementButtons.activate(getValueOfToken(statusToken, 2));
      } 
      else if (statusToken.startsWith("Ki")) {
        coolStepIncrementButtons.activate(getValueOfToken(statusToken, 2));
      }
      else if (statusToken.startsWith("Km")) {
        coolStepMinButtons.activate(getValueOfToken(statusToken, 2));
      }
    }
  }
  else {
    println(line);
  }
  settingStatus=false;
}

int getValueOfToken(String token, int position) {
  String value = token.substring(position);
  try {
    return Integer.valueOf(value);
  } 
  catch (NumberFormatException e) {
    println("Unable to decode '"+value+"'of '"+token+"' !");
    return 0;
  }
}

void drawSerial() {
  //draw the logo and some epxlaining text while setting up the serial port
  if (!motor_connected) {
    image(TMCLogo,200, 100);
    fill(uiTextColor);
    text("Select the serial port where your Arduino is connected\nIf in doubt check it in the Arduino IDE.\nThe Motor Tester will automatically verify if it can find an Motor tester ath the port.",200,100+TMCLogo.height+50);
  }
}

void help(float value) {
  if (value!=0) {
    link(helpUrl);
  }
}

