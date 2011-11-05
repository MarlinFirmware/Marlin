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

void manage_inactivity(byte debug);

void tp_init();
void manage_heater();
//int temp2analogu(int celsius, const short table[][2], int numtemps);
//float analog2tempu(int raw, const short table[][2], int numtemps);
int temp2analog(int celsius);
int temp2analogBed(int celsius);
float analog2temp(int raw);
float analog2tempBed(int raw);

#ifdef HEATER_USES_THERMISTOR
    #define HEATERSOURCE 1
#endif
#ifdef BED_USES_THERMISTOR
    #define BEDSOURCE 1
#endif

//#define temp2analogh( c ) temp2analogu((c),temptable,NUMTEMPS)
//#define analog2temp( c ) analog2tempu((c),temptable,NUMTEMPS


extern float Kp;
extern float Ki;
extern float Kd;
extern float Kc;

extern int target_raw[3];
extern int current_raw[3];
extern double pid_setpoint;

#endif
