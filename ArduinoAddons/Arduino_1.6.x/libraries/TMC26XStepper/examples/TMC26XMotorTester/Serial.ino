/*
 TMC26XMotorTest.ino - - TMC26X Stepper library tester for Wiring/Arduino
 
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
 #define INPUT_BUFFER_LENGTH 32

#define SERIAL_SPEED 115200
#define STATUS_COUNTER 100

char inputBuffer[INPUT_BUFFER_LENGTH+1]; //ad on character to keep the trainling 0
unsigned char inputBufferPosition;

void startSerial() {
  Serial.begin(SERIAL_SPEED);
  Serial.println(F("================================="));
  Serial.println(F("TMC26X Stepper Driver Motor Tester"));
  Serial.println(F("================================="));
  //empty the input buffer
  for (unsigned char i=0; i< INPUT_BUFFER_LENGTH+1; i++) {
    inputBuffer[i]=0;
  }
  inputBufferPosition=0;
}

void loopSerial() {
  if (Serial.available()>0 && inputBufferPosition<INPUT_BUFFER_LENGTH) {
    char c = Serial.read();
    //Read the char
    inputBuffer[inputBufferPosition]=c;
    inputBufferPosition++;
    //always terminate the string
    inputBuffer[inputBufferPosition]=0;
    //and if the line ended we execute the command
    if (c=='\n') {
      executeSerialCommand();   
    }
  }
  if (motor_moved) {
    Serial.print("#sg");
    Serial.print(tmc26XStepper.getCurrentStallGuardReading(),DEC);
    Serial.print(",p");
    Serial.print(tmc26XStepper.getMotorPosition(),DEC);
    Serial.print(",k");
    Serial.print(tmc26XStepper.getCurrentCurrent(),DEC);
    Serial.println(',');
    motor_moved=0;
  }
  if (motor_counter>STATUS_COUNTER) {
    motor_counter=0;
    char moving = tmc26XStepper.isMoving();
    Serial.print('#');
    if (moving) {
      Serial.print('r');
    } 
    else {
      Serial.print('s');
    }
    Serial.print(',');
    Serial.print('d');
    Serial.print(direction);
    Serial.print(',');
    Serial.print("c");
    Serial.print(tmc26XStepper.getCurrent(),DEC);
    Serial.print(',');
    Serial.print('S');
    Serial.print(tmc26XStepper.getSpeed(),DEC);
    Serial.print(',');
    Serial.print('m');
    Serial.print(tmc26XStepper.getMicrosteps(),DEC);
    Serial.print(',');
    Serial.print("t");
    Serial.print(tmc26XStepper.getStallGuardThreshold(),DEC);
    Serial.print(',');
    Serial.print('f');
    Serial.print(tmc26XStepper.getStallGuardFilter(),DEC);
    Serial.print(',');
    //print out the general cool step config
    if (tmc26XStepper.isCoolStepEnabled()) {
      Serial.print("Ke+,");
    } 
    else {
      Serial.print("Ke-,");
    }
    Serial.print("Kl");
    Serial.print(tmc26XStepper.getCoolStepLowerSgThreshold(),DEC);
    Serial.print(",Ku");
    Serial.print(tmc26XStepper.getCoolStepUpperSgThreshold(),DEC);
    Serial.print(",Kn");
    Serial.print(tmc26XStepper.getCoolStepNumberOfSGReadings(),DEC);
    Serial.print(",Ki");
    Serial.print(tmc26XStepper.getCoolStepCurrentIncrementSize(),DEC);
    Serial.print(",Km");
    Serial.print(tmc26XStepper.getCoolStepLowerCurrentLimit(),DEC);
    Serial.print(',');
    //detect the winding status
    if (tmc26XStepper.isOpenLoadA()) {
      Serial.print("ao,");
    } 
    else if(tmc26XStepper.isShortToGroundA()) {
      Serial.print("ag,");
    } 
    else {
      Serial.print("a-,");
    }
    //detect the winding status
    if (tmc26XStepper.isOpenLoadB()) {
      Serial.print("bo,");
    } 
    else if(tmc26XStepper.isShortToGroundB()) {
      Serial.print("bg,");
    } 
    else {
      Serial.print("b-,");
    }
    char temperature = tmc26XStepper.getOverTemperature();
    if (temperature==0) {
      Serial.print("x-,");
    } 
    else if (temperature==TMC26X_OVERTEMPERATURE_PREWARING) {
      Serial.print("xw,");
    } 
    else {
      Serial.print("xe,");
    }
    if (tmc26XStepper.isEnabled()) {
      Serial.print("e1,");
    } 
    else {
      Serial.print("e0,");
    }
    //write out the current chopper config
    Serial.print("Cm");
    Serial.print(chopperMode,DEC);
    Serial.print(",Co");
    Serial.print(t_off,DEC);
    Serial.print(",Cb");
    Serial.print(t_blank,DEC);
    if (chopperMode==0) {
      Serial.print(",Cs");
      Serial.print(h_start,DEC);
      Serial.print(",Ce");
      Serial.print(h_end,DEC);
      Serial.print(",Cd");
      Serial.print(h_decrement,DEC);
    }
    Serial.print(',');
    Serial.println();
  }
}

void executeSerialCommand() {
  Serial.print("Executing ");
  Serial.println(inputBuffer);
  //stimple runn & stop commands
  switch(inputBuffer[0]) {
  case 's':
    running = 0;
    break;
  case 'r':
    running = -1;
    break;
  case 'S':
    {
      int targetSpeed = decode(1);
      setSpeed(targetSpeed);
    }
    break;
  case 'm':
    {
      int microstepping = decode(1);
      setMicrostepping(microstepping);
    }
    break;
  case 't':
    {
      int threshold = decode(1);
      setStallGuardThreshold(threshold);
    }
    break;
  case 'f':
    {
      int filter = decode(1);
      setStallGuardFilter(filter);
    }
    break;
  case 'd':
    {
      int value = decode(1);
      tmc26XStepper.stop();
      if (value<0) {
        direction=-1;
      } 
      else {
        direction=1;
      }
    }
    break;
  case 'c':
    {
      int current = decode(1);
      setCurrent(current);
    }
    break;
  case 'e':
    {
      int enabled = decode(1);
      if (enabled) {
        tmc26XStepper.setEnabled(true);
      } 
      else {
        tmc26XStepper.setEnabled(false);
      }
    }
    break;
  case 'C':
    switch(inputBuffer[1]) {
    case 'o':
      {
        int value = decode(2);
        if (value>0 && value<16) {
          t_off=value;
          updateChopper();
        }
      }
      break;
    case 'b':
      {
        int value = decode(2);
        if (value>=0 && value<=3) {
          t_blank=value;
          updateChopper();
        }
      }
      break;
    case 's':
      {
        int value = decode(2);
        if (value>=0 && value<=8) {
          h_start=value;
          updateChopper();
        }
      }
      break;
    case 'e':
      {
        int value = decode(2);
        if (value>=-3 && value<=12) {
          h_end=value;
          updateChopper();
        }
      }
      break;
    case 'd':
      {
        int value = decode(2);
        if (value>=0 && value<=3) {
          h_decrement=value;
          updateChopper();
        }
      }
      break;
    }
    break;
  case 'K':
    switch(inputBuffer[1]) {
    case '+':
      tmc26XStepper.setCoolStepEnabled(true);
      break;
    case '-':
      tmc26XStepper.setCoolStepEnabled(false);
      break;
    case 'l':
      {
        int value = decode(2);
        if (value>=0 && value<480) {
          lower_SG_threshold=value;
          updateCoolStep();
        }
      }
      break;
    case 'u':
      {
        int value = decode(2);
        if (value>=0 && value<480) {
          upper_SG_threshold=value;
          updateCoolStep();
        }
      }
      break;
    case 'n':
      {
        int value = decode(2);
        if (value>=0 && value<4) {
          number_of_SG_readings=value;
          updateCoolStep();
        }
      }
      break;
    case 'i':
      {
        int value = decode(2);
        if (value>=0 && value<4) {
          current_increment_step_size=value;
          updateCoolStep();
        }
      }
      break;
    case 'm':
      {
        int value = decode(2);
        if (value>=0 && value<2) {
          lower_current_limit=value;
          updateCoolStep();
        }
      }
      break;
    }
    break;
  }
  //at the end delete buffer
  inputBufferPosition=0;
  inputBuffer[0]=0;
}

int decode(unsigned char startPosition) {
  int result=0;
  boolean negative = false;
  if (inputBuffer[startPosition]=='-') {
    negative=true;
    startPosition++;
  }
  for (unsigned char i=startPosition; i< (INPUT_BUFFER_LENGTH+1) && inputBuffer[i]!=0; i++) {
    char number = inputBuffer[i];
    //this very dumb approac can lead to errors, but we expect only numbers after the command anyway
    if (number <= '9' && number >='0') {
      result *= 10;
      result += number - '0';
    } 
  }
  if (negative) {
    return -result;
  } 
  else {
    return result;
  }
}



