/*
  temperature.h - temperature controller
  Part of Marlin

  Copyright (c) 2011 Erik van der Zalm

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef temperature_h
#define temperature_h 

#include "Marlin.h"
#include "fastio.h"
#ifdef PID_ADD_EXTRUSION_RATE
  #include "stepper.h"
#endif

// public functions
void tp_init();  //initialise the heating
void manage_heater(); //it is critical that this is called periodically.


enum TempSensor {TEMPSENSOR_HOTEND_0=0,TEMPSENSOR_BED=1, TEMPSENSOR_HOTEND_1=2};

//low leven conversion routines
// do not use this routines and variables outsie of temperature.cpp
int temp2analog(int celsius);
int temp2analogBed(int celsius);
float analog2temp(int raw);
float analog2tempBed(int raw);
extern int target_raw[3];  
extern int current_raw[3];
extern float Kp,Ki,Kd,Kc;

#ifdef PIDTEMP
  extern float pid_setpoint ;
#endif
  
#ifdef WATCHPERIOD
  extern int watch_raw[3] ;
  extern unsigned long watchmillis;
#endif



//high level conversion routines, for use outside of temperature.cpp
//inline so that there is no performance decrease.
//deg=degreeCelsius

inline float degHotend0(){  return analog2temp(current_raw[TEMPSENSOR_HOTEND_0]);};
inline float degHotend1(){  return analog2temp(current_raw[TEMPSENSOR_HOTEND_1]);};
inline float degBed() {  return analog2tempBed(current_raw[TEMPSENSOR_BED]);};

inline float degTargetHotend0() {  return analog2temp(target_raw[TEMPSENSOR_HOTEND_0]);};
inline float degTargetHotend1() {  return analog2temp(target_raw[TEMPSENSOR_HOTEND_1]);};
inline float degTargetBed() {   return analog2tempBed(target_raw[TEMPSENSOR_BED]);};

inline void setTargetHotend0(const float &celsius) 
{  
  target_raw[TEMPSENSOR_HOTEND_0]=temp2analog(celsius);
  #ifdef PIDTEMP
    pid_setpoint = celsius;
  #endif //PIDTEMP
};
inline void setTargetHotend1(const float &celsius) {  target_raw[TEMPSENSOR_HOTEND_1]=temp2analog(celsius);};
inline void setTargetBed(const float &celsius)     {  target_raw[TEMPSENSOR_BED     ]=temp2analogBed(celsius);};

inline bool isHeatingHotend0() {return target_raw[TEMPSENSOR_HOTEND_0] > current_raw[TEMPSENSOR_HOTEND_0];};
inline bool isHeatingHotend1() {return target_raw[TEMPSENSOR_HOTEND_1] > current_raw[TEMPSENSOR_HOTEND_1];};
inline bool isHeatingBed() {return target_raw[TEMPSENSOR_BED] > current_raw[TEMPSENSOR_BED];};

inline bool isCoolingHotend0() {return target_raw[TEMPSENSOR_HOTEND_0] < current_raw[TEMPSENSOR_HOTEND_0];};
inline bool isCoolingHotend1() {return target_raw[TEMPSENSOR_HOTEND_1] < current_raw[TEMPSENSOR_HOTEND_1];};
inline bool isCoolingBed() {return target_raw[TEMPSENSOR_BED] < current_raw[TEMPSENSOR_BED];};

void disable_heater();
void setWatch();
void updatePID();

#endif

