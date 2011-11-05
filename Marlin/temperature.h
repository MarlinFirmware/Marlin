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
#ifdef PID_ADD_EXTRUSION_RATE
  #include "stepper.h"
#endif

enum TempSensor {TEMPSENSOR_HOTEND=0,TEMPSENSOR_BED=1, TEMPSENSOR_AUX=2};

// ther must be only one instance of this class, and it is created in temperature.cpp by itself and is called "htr".
// all the variables are static, so that of the compiler optimization is more easy.
// I honestly hope that this increases readability and structure.
// none of the variables or routines should be called from an secondary process/interrupt with the exceptino of current_raw[].

class Heater
{
public:
  Heater(); //treplaces tp_init();
  ~Heater(); 
  
  void static manage_heater(); /// it is critical that this is called continously. 
  
  // conversion routines, const since they don't change any class variables.
  float const static temp2analog(const int celsius);
  float const static temp2analogBed(const int celsius);
  float const static analog2temp(const int raw);
  float const static analog2tempBed(const int raw);
  
  inline float const static celsius(const TempSensor s) 
    {
      if(s==TEMPSENSOR_BED) 
	return analog2tempBed(Heater::current_raw[s]); 
      else 
	return analog2temp(Heater::current_raw[s]);
    };
  inline float const static celsiusTarget(const TempSensor s) 
    {
      if(s==TEMPSENSOR_BED) 
	return analog2tempBed(Heater::target_raw[s]); 
      else 
	return analog2temp(Heater::target_raw[s]);
    };
  inline float static setCelsius(const TempSensor s, const int celsius) 
    {
      #ifdef PIDTEMP
	if(s==TEMPSENSOR_HOTEND)
            Heater::pid_setpoint = celsius;
      #endif //PIDTEM
      if(s==TEMPSENSOR_BED) 
	Heater::target_raw[s] = temp2analog(celsius); 
      else 
	Heater::target_raw[s] = temp2analogBed(celsius); 
    };

   inline bool const static isHeating(TempSensor s)
   { return (Heater::target_raw[s]>Heater::current_raw[s]);};
   inline bool const static isCooling(TempSensor s)
   { return (Heater::target_raw[s]<Heater::current_raw[s]);};

public:
  #ifdef PIDTEMP
    static float Kp;
    static float Ki;
    static float Kd;
    static float Kc;
  #endif
  
  static int target_raw[3];
  static float pid_setpoint;
  
  volatile static int current_raw[3]; //this are written by an ISR, so volatile.
  volatile static bool temp_meas_ready ; //also this is set by the ISR
  
  
private:
 
  

  static unsigned long previous_millis_heater, previous_millis_bed_heater;

  #ifdef PIDTEMP
    static float temp_iState;
    static float temp_dState;
    static float pTerm;
    static float iTerm;
    static float dTerm;
	//int output;
    static float pid_error;
    static float temp_iState_min;
    static float temp_iState_max;
    static float pid_input;
    static float pid_output;
    
    static bool pid_reset;
    static float HeaterPower;
    
  #endif //PIDTEMP

public: //but only accesed from the ISR hence not volatile
   #ifdef MINTEMP
  static int minttemp;
  #endif //MINTEMP
  #ifdef MAXTEMP
  static int maxttemp;
  #endif //MAXTEMP

  #ifdef BED_MINTEMP
  static int bed_minttemp ;
  #endif //BED_MINTEMP
  #ifdef BED_MAXTEMP
  static int bed_maxttemp;
  #endif //BED_MAXTEMP
  
};

extern Heater htr; //this creates the single, global instance 

#ifdef HEATER_USES_THERMISTOR
    #define HEATERSOURCE 1
#endif
#ifdef BED_USES_THERMISTOR
    #define BEDSOURCE 1
#endif


#endif
