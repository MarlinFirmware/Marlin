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
 unsigned int motor_counter = 0;
unsigned char motor_moved = 0;
int sgThreshold = 4;
int sgFilter = 0;
int direction = 1;

unsigned int lower_SG_threshold = 0;
unsigned int upper_SG_threshold = 0;
unsigned char number_of_SG_readings=0;
unsigned char current_increment_step_size=0;
unsigned char lower_current_limit=0;


char chopperMode = 0; //0 for spread, 1 for constant off
char t_off = 2;
char t_blank = 24;
char h_start = 8;
char h_end = 6;
char h_decrement = 0;


void startMotor() {
  Serial.println(F("Configuring stepper driver"));
  //char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement
  tmc26XStepper.setSpreadCycleChopper(t_off,t_blank,h_start,h_end,h_decrement);
  tmc26XStepper.setRandomOffTime(0);

  tmc26XStepper.setMicrosteps(32);
  tmc26XStepper.setStallGuardThreshold(sgThreshold,sgFilter);
  //  Serial.println("config finished, starting");
  digitalWrite(ENABLE_PIN,LOW);
  tmc26XStepper.start();
  tmc26XStepper.setSpeed(10);
  TCNT2=setupTimer2(10000);
  sei();
}

void runMotor() {
  if (running && !tmc26XStepper.isMoving()) {
    tmc26XStepper.step(direction*10000);
    Serial.println("run");
  }
  if (!running & tmc26XStepper.isMoving()) {
    tmc26XStepper.stop();
    Serial.println("stop");
  }
}

void setSpeed(unsigned int targetSpeed) {
  if (targetSpeed>0 && targetSpeed<MAX_SPEED) {
    Serial.print(F("Setting speed: ")); 
    Serial.println(targetSpeed);
    tmc26XStepper.setSpeed(targetSpeed);
  } 
  else {
    Serial.print(F("improper speed "));
    Serial.println(targetSpeed);
  }
}

void setMicrostepping(int microstepping) {
  if (microstepping<1 || microstepping>256) {
    Serial.print(F("Improperd microstepping setting [1...256]: "));
    Serial.print(microstepping);
  } 
  else {
    tmc26XStepper.setMicrosteps(microstepping);
  }
}

void setStallGuardThreshold(int threshold) {
  if (threshold<-64 || threshold > 63) {
    Serial.print(F("Improper Stall Guard Threshold [-64...63]: "));
    Serial.println(threshold);
  } 
  else {
    sgThreshold = threshold;
    tmc26XStepper.setStallGuardThreshold(threshold,sgFilter);
  }
}

void setStallGuardFilter(int filter) {
  if (filter) {
    sgFilter=1;
  } 
  else {
    sgFilter=0;
  }
  tmc26XStepper.setStallGuardThreshold(sgThreshold,sgFilter);
}

void setCurrent(int current) {
  if (current>0 && current <1700) {
    tmc26XStepper.setCurrent(current);
  } 
  else {
    Serial.print(F("Improper current {0 ... 1200}: "));
    Serial.print(current);
  }
}

void updateChopper() {
  //we can do only spread now
  if (chopperMode==0) {
    tmc26XStepper.setSpreadCycleChopper(t_off,t_blank,h_start,h_end,h_decrement);
  }    
}

void updateCoolStep() {
  tmc26XStepper.setCoolStepConfiguration(
    lower_SG_threshold, upper_SG_threshold, number_of_SG_readings,
    current_increment_step_size, lower_current_limit);
}

//from http://www.uchobby.com/index.php/2007/11/24/arduino-interrupts/
//Setup Timer2.s
//Configures the ATMega168 8-Bit Timer2 to generate an interrupt
//at the specified frequency.
//Returns the timer load value which must be loaded into TCNT2
//inside your ISR routine.
//See the example usage below.
unsigned char setupTimer2(float timeoutFrequency){
  unsigned char result; //The timer load value.

  //Calculate the timer load value
  result=(int)((257.0-(TIMER_CLOCK_FREQ/timeoutFrequency))+0.5);
  //The 257 really should be 256 but I get better results with 257.

  //Timer2 Settings: Timer Prescaler /8, mode 0
  //Timer clock = 16MHz/8 = 2Mhz or 0.5us
  //The /8 prescale gives us a good range to work with
  //so we just hard code this for now.
  TCCR2A = 0;
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 = 1<<TOIE2;

  //load the timer for its first cycle
  TCNT2=result;

  return(result);
}

ISR(TIMER2_OVF_vect) {
  motor_moved = tmc26XStepper.move();
  motor_counter++;
}




