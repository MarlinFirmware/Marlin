/*
  Hysteresis.h - A class that manages hysteresis by inserting extra plan_buffer_line when necessary
 
  Copyright (c) 2012 Neil James Martin
 
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

#include "Configuration.h"

#define DECLARE_HYSTERESIS_MCODES(REPORT_CODE, SET_CODE)                \
   case REPORT_CODE:                                                    \
    {                                                                   \
      hysteresis.ReportToSerial();                                      \
    }                                                                   \
    break;                                                              \
   case SET_CODE:                                                       \
    {                                                                   \
      if(code_seen('X')) hysteresis.SetAxis( X_AXIS, code_value() );    \
      if(code_seen('Y')) hysteresis.SetAxis( Y_AXIS, code_value() );    \
      if(code_seen('Z')) hysteresis.SetAxis( Z_AXIS, code_value() );    \
      if(code_seen('E')) hysteresis.SetAxis( E_AXIS, code_value() );    \
    }                                                                   \
    break;

//===========================================================================

class Hysteresis
{
public:
  Hysteresis( float x_mm, float y_mm, float z_mm, float e_mm );
  
  void Set( float x_mm, float y_mm, float z_mm, float e_mm );
  void SetAxis( int axis, float mm );
  void ReportToSerial();
  void InsertCorrection(const float &x, const float &y, const float &z, const float &e);

private:
  void          calcSteps();
  float         m_hysteresis_mm[NUM_AXIS];
  long          m_hysteresis_steps[NUM_AXIS];
  unsigned char m_prev_direction_bits;
  unsigned char m_hysteresis_bits;
};

//===========================================================================

extern Hysteresis hysteresis;
extern long position[4]; // defined in planner.cpp
