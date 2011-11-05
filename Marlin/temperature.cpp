/*
  temperature.c - temperature control
  Part of Marlin
  
 Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 This firmware is a mashup between Sprinter and grbl.
  (https://github.com/kliment/Sprinter)
  (https://github.com/simen/grbl/tree)
 
 It has preliminary support for Matthew Roberts advance algorithm 
    http://reprap.org/pipermail/reprap-dev/2011-May/003323.html

 This firmware is optimized for gen6 electronics.
 */

#include "fastio.h"
#include "Configuration.h"
#include "pins.h"
#include "Marlin.h"
#include "ultralcd.h"
#include "streaming.h"
#include "temperature.h"

int target_bed_raw = 0;
int current_bed_raw = 0;

int target_raw[3] = {0, 0, 0};
int current_raw[3] = {0, 0, 0};
unsigned char temp_meas_ready = false;

unsigned long previous_millis_heater, previous_millis_bed_heater;

#ifdef PIDTEMP
  double temp_iState = 0;
  double temp_dState = 0;
  double pTerm;
  double iTerm;
  double dTerm;
      //int output;
  double pid_error;
  double temp_iState_min;
  double temp_iState_max;
  double pid_setpoint = 0.0;
  double pid_input;
  double pid_output;
  bool pid_reset;
  float HeaterPower;
  
  float Kp=DEFAULT_Kp;
  float Ki=DEFAULT_Ki;
  float Kd=DEFAULT_Kd;
  float Kc=DEFAULT_Kc;
#endif //PIDTEMP

#ifdef HEATER_0_MINTEMP
int minttemp_0 = temp2analog(HEATER_0_MINTEMP);
#endif //MINTEMP
#ifdef HEATER_0_MAXTEMP
int maxttemp_0 = temp2analog(HEATER_0_MAXTEMP);
#endif //MAXTEMP

#ifdef HEATER_1_MINTEMP
int minttemp_1 = temp2analog(HEATER_1_MINTEMP);
#endif //MINTEMP
#ifdef HEATER_1_MAXTEMP
int maxttemp_1 = temp2analog(HEATER_1_MAXTEMP);
#endif //MAXTEMP

#ifdef BED_MINTEMP
int bed_minttemp = temp2analog(BED_MINTEMP);
#endif //BED_MINTEMP
#ifdef BED_MAXTEMP
int bed_maxttemp = temp2analog(BED_MAXTEMP);
#endif //BED_MAXTEMP

void manage_heater()
{
#ifdef USE_WATCHDOG
  wd_reset();
#endif
  
  float pid_input;
  float pid_output;
  if(temp_meas_ready != true)   //better readability
    return; 

CRITICAL_SECTION_START;
    temp_meas_ready = false;
CRITICAL_SECTION_END;

#ifdef PIDTEMP
    pid_input = analog2temp(current_raw[TEMPSENSOR_HOTEND]);

#ifndef PID_OPENLOOP
    pid_error = pid_setpoint - pid_input;
    if(pid_error > 10){
      pid_output = PID_MAX;
      pid_reset = true;
    }
    else if(pid_error < -10) {
      pid_output = 0;
      pid_reset = true;
    }
    else {
      if(pid_reset == true) {
        temp_iState = 0.0;
        pid_reset = false;
      }
      pTerm = Kp * pid_error;
      temp_iState += pid_error;
      temp_iState = constrain(temp_iState, temp_iState_min, temp_iState_max);
      iTerm = Ki * temp_iState;
      //K1 defined in Configuration.h in the PID settings
      #define K2 (1.0-K1)
      dTerm = (Kd * (pid_input - temp_dState))*K2 + (K1 * dTerm);
      temp_dState = pid_input;
      #ifdef PID_ADD_EXTRUSION_RATE
        pTerm+=Kc*current_block->speed_e; //additional heating if extrusion speed is high
      #endif
      pid_output = constrain(pTerm + iTerm - dTerm, 0, PID_MAX);
    }
#endif //PID_OPENLOOP
#ifdef PID_DEBUG
     Serial.print(" Input ");
     Serial.print(pid_input);
     Serial.print(" Output ");
     Serial.print(pid_output);    
     Serial.print(" pTerm ");
     Serial.print(pTerm); 
     Serial.print(" iTerm ");
     Serial.print(iTerm); 
     Serial.print(" dTerm ");
     Serial.print(dTerm); 
     Serial.println();
#endif //PID_DEBUG
    analogWrite(HEATER_0_PIN, pid_output);
#endif //PIDTEMP

#ifndef PIDTEMP
    if(current_raw[0] >= target_raw[0])
    {
      WRITE(HEATER_0_PIN,LOW);
    }
    else 
    {
      WRITE(HEATER_0_PIN,HIGH);
    }
#endif
    
  if(millis() - previous_millis_bed_heater < BED_CHECK_INTERVAL)
    return;
  previous_millis_bed_heater = millis();
  
  #if TEMP_1_PIN > -1
    if(current_raw[TEMPSENSOR_BED] >= target_raw[TEMPSENSOR_BED])
    {
      WRITE(HEATER_1_PIN,LOW);
    }
    else 
    {
      WRITE(HEATER_1_PIN,HIGH);
    }
  #endif
  }

// Takes hot end temperature value as input and returns corresponding raw value. 
// For a thermistor, it uses the RepRap thermistor temp table.
// This is needed because PID in hydra firmware hovers around a given analog value, not a temp value.
// This function is derived from inversing the logic from a portion of getTemperature() in FiveD RepRap firmware.
int temp2analog(int celsius) {
  #ifdef HEATER_0_USES_THERMISTOR
    int raw = 0;
    byte i;
    
    for (i=1; i<NUMTEMPS_HEATER_0; i++)
    {
      if (heater_0_temptable[i][1] < celsius)
      {
        raw = heater_0_temptable[i-1][0] + 
          (celsius - heater_0_temptable[i-1][1]) * 
          (heater_0_temptable[i][0] - heater_0_temptable[i-1][0]) /
          (heater_0_temptable[i][1] - heater_0_temptable[i-1][1]);  
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == NUMTEMPS_0) raw = heater_0_temptable[i-1][0];

    return (1023 * OVERSAMPLENR) - raw;
  #elif defined HEATER_0_USES_AD595
    return celsius * (1024.0 / (5.0 * 100.0) ) * OVERSAMPLENR;
  #endif
}

// Takes bed temperature value as input and returns corresponding raw value. 
// For a thermistor, it uses the RepRap thermistor temp table.
// This is needed because PID in hydra firmware hovers around a given analog value, not a temp value.
// This function is derived from inversing the logic from a portion of getTemperature() in FiveD RepRap firmware.
int temp2analogBed(int celsius) {
  #ifdef BED_USES_THERMISTOR

    int raw = 0;
    byte i;
    
    for (i=1; i<BNUMTEMPS; i++)
    {
      if (bedtemptable[i][1] < celsius)
      {
        raw = bedtemptable[i-1][0] + 
          (celsius - bedtemptable[i-1][1]) * 
          (bedtemptable[i][0] - bedtemptable[i-1][0]) /
          (bedtemptable[i][1] - bedtemptable[i-1][1]);
      
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == BNUMTEMPS) raw = bedtemptable[i-1][0];

    return (1023 * OVERSAMPLENR) - raw;
  #elif defined BED_USES_AD595
    return celsius * (1024.0 / (5.0 * 100.0) ) * OVERSAMPLENR;
  #endif
}

// Derived from RepRap FiveD extruder::getTemperature()
// For hot end temperature measurement.
float analog2temp(int raw) {
  #ifdef HEATER_0_USES_THERMISTOR
    float celsius = 0;
    byte i;  
    raw = (1023 * OVERSAMPLENR) - raw;
    for (i=1; i<NUMTEMPS_HEATER_0; i++)
    {
      if (heater_0_temptable[i][0] > raw)
      {
        celsius  = heater_0_temptable[i-1][1] + 
          (raw - heater_0_temptable[i-1][0]) * 
          (float)(heater_0_temptable[i][1] - heater_0_temptable[i-1][1]) /
          (float)(heater_0_temptable[i][0] - heater_0_temptable[i-1][0]);

        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == NUMTEMPS_HEATER_0) celsius = heater_0_temptable[i-1][1];

    return celsius;
  #elif defined HEATER_0_USES_AD595
    return raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR;
  #endif
}

// Derived from RepRap FiveD extruder::getTemperature()
// For bed temperature measurement.
float analog2tempBed(int raw) {
  #ifdef BED_USES_THERMISTOR
    int celsius = 0;
    byte i;

    raw = (1023 * OVERSAMPLENR) - raw;

    for (i=1; i<BNUMTEMPS; i++)
    {
      if (bedtemptable[i][0] > raw)
      {
        celsius  = bedtemptable[i-1][1] + 
          (raw - bedtemptable[i-1][0]) * 
          (bedtemptable[i][1] - bedtemptable[i-1][1]) /
          (bedtemptable[i][0] - bedtemptable[i-1][0]);

        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == BNUMTEMPS) celsius = bedtemptable[i-1][1];

    return celsius;
    
  #elif defined BED_USES_AD595
    return raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR;
  #endif
}

void tp_init()
{
#if (HEATER_0_PIN > -1) 
  SET_OUTPUT(HEATER_0_PIN);
#endif  
#if (HEATER_1_PIN > -1) 
  SET_OUTPUT(HEATER_1_PIN);
#endif  
#if (HEATER_2_PIN > -1) 
  SET_OUTPUT(HEATER_2_PIN);
#endif  

#ifdef PIDTEMP
  temp_iState_min = 0.0;
  temp_iState_max = PID_INTEGRAL_DRIVE_MAX / Ki;
#endif //PIDTEMP

// Set analog inputs
  ADCSRA = 1<<ADEN | 1<<ADSC | 1<<ADIF | 0x07;
  
// Use timer0 for temperature measurement
// Interleave temperature interrupt with millies interrupt
  OCR0B = 128;
  TIMSK0 |= (1<<OCIE0B);  
}



// Timer 0 is shared with millies
ISR(TIMER0_COMPB_vect)
{
  //these variables are only accesible from the ISR, but static, so they don't loose their value
  static unsigned char temp_count = 0;
  static unsigned long raw_temp_0_value = 0;
  static unsigned long raw_temp_1_value = 0;
  static unsigned long raw_temp_2_value = 0;
  static unsigned char temp_state = 0;
  
  switch(temp_state) {
    case 0: // Prepare TEMP_0
            #if (TEMP_0_PIN > -1)
              #if TEMP_0_PIN < 8
                DIDR0 = 1 << TEMP_0_PIN; 
              #else
                DIDR2 = 1<<(TEMP_0_PIN - 8); 
                ADCSRB = 1<<MUX5;
              #endif
              ADMUX = ((1 << REFS0) | (TEMP_0_PIN & 0x07));
              ADCSRA |= 1<<ADSC; // Start conversion
            #endif
            #ifdef ULTIPANEL
              buttons_check();
            #endif
            temp_state = 1;
            break;
    case 1: // Measure TEMP_0
            #if (TEMP_0_PIN > -1)
              raw_temp_0_value += ADC;
            #endif
            temp_state = 2;
            break;
    case 2: // Prepare TEMP_1
            #if (TEMP_1_PIN > -1)
              #if TEMP_1_PIN < 7
                DIDR0 = 1<<TEMP_1_PIN; 
              #else
                DIDR2 = 1<<(TEMP_1_PIN - 8); 
                ADCSRB = 1<<MUX5;
              #endif
              ADMUX = ((1 << REFS0) | (TEMP_1_PIN & 0x07));
              ADCSRA |= 1<<ADSC; // Start conversion
            #endif
            #ifdef ULTIPANEL
              buttons_check();
            #endif
            temp_state = 3;
            break;
    case 3: // Measure TEMP_1
            #if (TEMP_1_PIN > -1)
              raw_temp_1_value += ADC;
            #endif
            temp_state = 4;
            break;
    case 4: // Prepare TEMP_2
            #if (TEMP_2_PIN > -1)
              #if TEMP_2_PIN < 7
                DIDR0 = 1 << TEMP_2_PIN; 
              #else
                DIDR2 = 1<<(TEMP_2_PIN - 8); 
                ADCSRB = 1<<MUX5;
              #endif
              ADMUX = ((1 << REFS0) | (TEMP_2_PIN & 0x07));
              ADCSRA |= 1<<ADSC; // Start conversion
            #endif
            #ifdef ULTIPANEL
              buttons_check();
            #endif
            temp_state = 5;
            break;
    case 5: // Measure TEMP_2
            #if (TEMP_2_PIN > -1)
              raw_temp_2_value += ADC;
            #endif
            temp_state = 0;
            temp_count++;
            break;
    default:
            Serial.println("!! Temp measurement error !!");
            break;
  }
    
  if(temp_count >= 16) // 6 ms * 16 = 96ms.
  {
    #ifdef HEATER_0_USES_AD595
      current_raw[0] = raw_temp_0_value;
    #else
      current_raw[0] = 16383 - raw_temp_0_value;
    #endif
    
    #ifdef HEATER_1_USES_AD595
      current_raw[2] = raw_temp_2_value;
    #else
      current_raw[2] = 16383 - raw_temp_2_value;
    #endif
    
    #ifdef BED_USES_AD595
      current_raw[1] = raw_temp_1_value;
    #else
      current_raw[1] = 16383 - raw_temp_1_value;
    #endif
    
    temp_meas_ready = true;
    temp_count = 0;
    raw_temp_0_value = 0;
    raw_temp_1_value = 0;
    raw_temp_2_value = 0;
#ifdef HEATER_0_MAXTEMP
  #if (HEATER_0_PIN > -1)
    if(current_raw[TEMPSENSOR_HOTEND] >= maxttemp) {
      target_raw[TEMPSENSOR_HOTEND] = 0;
      analogWrite(HEATER_0_PIN, 0);
      Serial.println("!! Temperature extruder 0 switched off. MAXTEMP triggered !!");
      kill();
    }
  #endif
#endif
    if(current_raw[TEMPSENSOR_AUX] >= maxttemp) {
      target_raw[TEMPSENSOR_AUX] = 0;
    if(current_raw[2] >= maxttemp_1) {
      analogWrite(HEATER_2_PIN, 0);
      Serial.println("!! Temperature extruder 1 switched off. MAXTEMP triggered !!");
      kill()
    }
  #endif
#endif //MAXTEMP
#ifdef HEATER_0_MINTEMP
  #if (HEATER_0_PIN > -1)
    if(current_raw[TEMPSENSOR_HOTEND] <= minttemp) {
      target_raw[TEMPSENSOR_HOTEND] = 0;
      analogWrite(HEATER_0_PIN, 0);
      Serial.println("!! Temperature extruder 0 switched off. MINTEMP triggered !!");
      kill();
    }
  #endif
#endif
#ifdef HEATER_1_MINTEMP
  #if (HEATER_2_PIN > -1)
    if(current_raw[TEMPSENSOR_AUX] <= minttemp) {
      target_raw[TEMPSENSOR_AUX] = 0;
      analogWrite(HEATER_2_PIN, 0);
      Serial.println("!! Temperature extruder 1 switched off. MINTEMP triggered !!");
      kill();
    }
  #endif
#endif //MAXTEMP
#ifdef BED_MINTEMP
  #if (HEATER_1_PIN > -1)
    if(current_raw[1] <= bed_minttemp) {
      target_raw[1] = 0;
      WRITE(HEATER_1_PIN, 0);
      Serial.println("!! Temperatur heated bed switched off. MINTEMP triggered !!");
      kill();
    }
  #endif
#endif
#ifdef BED_MAXTEMP
  #if (HEATER_1_PIN > -1)
    if(current_raw[1] >= bed_maxttemp) {
      target_raw[1] = 0;
      WRITE(HEATER_1_PIN, 0);
      Serial.println("!! Temperature heated bed switched off. MAXTEMP triggered !!");
      kill();
    }
  #endif
#endif
  }
}
