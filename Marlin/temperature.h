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
#include "planner.h"
#ifdef PID_ADD_EXTRUSION_RATE
  #include "stepper.h"
#endif

// public functions
void tp_init();  //initialize the heating
void manage_heater(); //it is critical that this is called periodically.

#ifdef FILAMENT_SENSOR
// For converting raw Filament Width to milimeters 
 float analog2widthFil(); 
 
// For converting raw Filament Width to an extrusion ratio 
 int widthFil_to_size_ratio();
#endif

// low level conversion routines
// do not use these routines and variables outside of temperature.cpp
extern int target_temperature[4];  
extern float current_temperature[4];
#ifdef SHOW_TEMP_ADC_VALUES
  extern int current_temperature_raw[4];
  extern int current_temperature_bed_raw;
#endif
extern int target_temperature_bed;
extern float current_temperature_bed;
#ifdef TEMP_SENSOR_1_AS_REDUNDANT
  extern float redundant_temperature;
#endif

#if defined(CONTROLLERFAN_PIN) && CONTROLLERFAN_PIN > -1
  extern unsigned char soft_pwm_bed;
#endif

#ifdef PIDTEMP

  #ifdef PID_PARAMS_PER_EXTRUDER
    extern float Kp[EXTRUDERS], Ki[EXTRUDERS], Kd[EXTRUDERS], Kc[EXTRUDERS]; // one param per extruder
    #define PID_PARAM(param,e) param[e] // use macro to point to array value
  #else
    extern float Kp, Ki, Kd, Kc; // one param per extruder - saves 20 or 36 bytes of ram (inc array pointer)
    #define PID_PARAM(param, e) param // use macro to point directly to value
  #endif // PID_PARAMS_PER_EXTRUDER	
  float scalePID_i(float i);
  float scalePID_d(float d);
  float unscalePID_i(float i);
  float unscalePID_d(float d);

#endif
#ifdef PIDTEMPBED
  extern float bedKp,bedKi,bedKd;
#endif
  
  
#ifdef BABYSTEPPING
  extern volatile int babystepsTodo[3];
#endif
  
//high level conversion routines, for use outside of temperature.cpp
//inline so that there is no performance decrease.
//deg=degreeCelsius

FORCE_INLINE float degHotend(uint8_t extruder) { return current_temperature[extruder]; }
FORCE_INLINE float degBed() { return current_temperature_bed; }

#ifdef SHOW_TEMP_ADC_VALUES
  FORCE_INLINE float rawHotendTemp(uint8_t extruder) { return current_temperature_raw[extruder]; }
  FORCE_INLINE float rawBedTemp() { return current_temperature_bed_raw; }
#endif

FORCE_INLINE float degTargetHotend(uint8_t extruder) { return target_temperature[extruder]; }
FORCE_INLINE float degTargetBed() { return target_temperature_bed; }

FORCE_INLINE void setTargetHotend(const float &celsius, uint8_t extruder) { target_temperature[extruder] = celsius; }
FORCE_INLINE void setTargetBed(const float &celsius) { target_temperature_bed = celsius; }

FORCE_INLINE bool isHeatingHotend(uint8_t extruder) { return target_temperature[extruder] > current_temperature[extruder]; }
FORCE_INLINE bool isHeatingBed() { return target_temperature_bed > current_temperature_bed; }

FORCE_INLINE bool isCoolingHotend(uint8_t extruder) { return target_temperature[extruder] < current_temperature[extruder]; }
FORCE_INLINE bool isCoolingBed() { return target_temperature_bed < current_temperature_bed; }

#define degHotend0() degHotend(0)
#define degTargetHotend0() degTargetHotend(0)
#define setTargetHotend0(_celsius) setTargetHotend((_celsius), 0)
#define isHeatingHotend0() isHeatingHotend(0)
#define isCoolingHotend0() isCoolingHotend(0)
#if EXTRUDERS > 1
  #define degHotend1() degHotend(1)
  #define degTargetHotend1() degTargetHotend(1)
  #define setTargetHotend1(_celsius) setTargetHotend((_celsius), 1)
  #define isHeatingHotend1() isHeatingHotend(1)
  #define isCoolingHotend1() isCoolingHotend(1)
#else
  #define setTargetHotend1(_celsius) do{}while(0)
#endif
#if EXTRUDERS > 2
  #define degHotend2() degHotend(2)
  #define degTargetHotend2() degTargetHotend(2)
  #define setTargetHotend2(_celsius) setTargetHotend((_celsius), 2)
  #define isHeatingHotend2() isHeatingHotend(2)
  #define isCoolingHotend2() isCoolingHotend(2)
#else
  #define setTargetHotend2(_celsius) do{}while(0)
#endif
#if EXTRUDERS > 3
  #define degHotend3() degHotend(3)
  #define degTargetHotend3() degTargetHotend(3)
  #define setTargetHotend3(_celsius) setTargetHotend((_celsius), 3)
  #define isHeatingHotend3() isHeatingHotend(3)
  #define isCoolingHotend3() isCoolingHotend(3)
#else
  #define setTargetHotend3(_celsius) do{}while(0)
#endif
#if EXTRUDERS > 4
  #error Invalid number of extruders
#endif

int getHeaterPower(int heater);
void disable_heater();
void setWatch();
void updatePID();

#if defined (THERMAL_RUNAWAY_PROTECTION_PERIOD) && THERMAL_RUNAWAY_PROTECTION_PERIOD > 0
void thermal_runaway_protection(int *state, unsigned long *timer, float temperature, float target_temperature, int heater_id, int period_seconds, int hysteresis_degc);
static int thermal_runaway_state_machine[4]; // = {0,0,0,0};
static unsigned long thermal_runaway_timer[4]; // = {0,0,0,0};
static bool thermal_runaway = false;
#if TEMP_SENSOR_BED != 0
  static int thermal_runaway_bed_state_machine;
  static unsigned long thermal_runaway_bed_timer;
#endif
#endif

FORCE_INLINE void autotempShutdown() {
  #ifdef AUTOTEMP
    if (autotemp_enabled) {
      autotemp_enabled = false;
      if (degTargetHotend(active_extruder) > autotemp_min)
        setTargetHotend(0, active_extruder);
    }
  #endif
}

void PID_autotune(float temp, int extruder, int ncycles);

void setExtruderAutoFanState(int pin, bool state);
void checkExtruderAutoFans();

#endif
