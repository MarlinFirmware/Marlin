/*
  Hysteresis.cpp - A class that manages hysteresis by inserting extra plan_buffer_line when necessary
 
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

#include "Hysteresis.h"

#include "Marlin.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "ultralcd.h"
#include "language.h"

//===========================================================================

Hysteresis hysteresis( 0.0f, 0.0f, 0.0f, 0.0f );
float axis_shift[ NUM_AXIS ] = { 0.0f, 0.0f, 0.0f, 0.0f };

//===========================================================================
Hysteresis::Hysteresis( float x_mm, float y_mm, float z_mm, float e_mm )
{
  m_prev_direction_bits = 0;
  Set( x_mm, y_mm, z_mm, e_mm );
}

//===========================================================================
void Hysteresis::Set( float x_mm, float y_mm, float z_mm, float e_mm )
{
  m_hysteresis_mm[X_AXIS] = x_mm;
  m_hysteresis_mm[Y_AXIS] = y_mm;
  m_hysteresis_mm[Z_AXIS] = z_mm;
  m_hysteresis_mm[E_AXIS] = e_mm;
  m_hysteresis_bits = ((m_hysteresis_mm[X_AXIS]!=0.0f)?(1<<X_AXIS):0)
                    | ((m_hysteresis_mm[Y_AXIS]!=0.0f)?(1<<Y_AXIS):0)
                    | ((m_hysteresis_mm[Z_AXIS]!=0.0f)?(1<<Z_AXIS):0)
                    | ((m_hysteresis_mm[E_AXIS]!=0.0f)?(1<<E_AXIS):0);
  calcSteps();
}

//===========================================================================
void Hysteresis::SetAxis( int axis, float mm )
{
  m_hysteresis_mm[axis] = mm;
  if( mm != 0.0f )
  {
    m_hysteresis_bits |= (1<<axis);
  }
  else
  {
    m_hysteresis_bits &= ~(1<<axis);
  }
  calcSteps();
}

// convert mm to steps
void Hysteresis::calcSteps()
{
  for (int i=0; i<NUM_AXIS; i++)
  {
    m_hysteresis_steps[i] = (long)(m_hysteresis_mm[i]*axis_steps_per_unit[i]);
  }
}


//===========================================================================
void Hysteresis::ReportToSerial()
{
  SERIAL_PROTOCOLPGM("H=X");
  SERIAL_PROTOCOL(m_hysteresis_mm[X_AXIS]);
  SERIAL_PROTOCOLPGM(" Y");
  SERIAL_PROTOCOL(m_hysteresis_mm[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" Z");
  SERIAL_PROTOCOL(m_hysteresis_mm[Z_AXIS]);
  SERIAL_PROTOCOLPGM(" E");      
  SERIAL_PROTOCOL(m_hysteresis_mm[E_AXIS]);
  SERIAL_PROTOCOLPGM(" SHIFTS:x=");      
  SERIAL_PROTOCOL(axis_shift[X_AXIS]);
  SERIAL_PROTOCOLPGM(" y=");      
  SERIAL_PROTOCOL(axis_shift[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" z=");      
  SERIAL_PROTOCOL(axis_shift[Z_AXIS]);
  SERIAL_PROTOCOLPGM(" e=");      
  SERIAL_PROTOCOL(axis_shift[E_AXIS]);
  
  SERIAL_PROTOCOLLN("");
}

//===========================================================================
// direction 0: positive, 1: negative
unsigned char calc_direction_bits( const long* current_position, const long* destination )
{
  unsigned char direction_bits = 0;
  if (destination[X_AXIS] < current_position[X_AXIS]) { 
    direction_bits |= (1<<X_AXIS); 
  }
  if (destination[Y_AXIS] < current_position[Y_AXIS]) { 
    direction_bits |= (1<<Y_AXIS); 
  }
  if (destination[Z_AXIS] < current_position[Z_AXIS]) { 
    direction_bits |= (1<<Z_AXIS); 
  }
  if (destination[E_AXIS] < current_position[E_AXIS]) { 
    direction_bits |= (1<<E_AXIS); 
  }
  return direction_bits;
}

unsigned char calc_move_bits( const long* current_position, const long* destination )
{
  unsigned char move_bits = 0;
  if (destination[X_AXIS] != current_position[X_AXIS]) { 
    move_bits |= (1<<X_AXIS); 
  }
  if (destination[Y_AXIS] != current_position[Y_AXIS]) { 
    move_bits |= (1<<Y_AXIS); 
  }
  if (destination[Z_AXIS] != current_position[Z_AXIS]) { 
    move_bits |= (1<<Z_AXIS); 
  }
  if (destination[E_AXIS] != current_position[E_AXIS]) { 
    move_bits |= (1<<E_AXIS); 
  }
  return move_bits;
}
//===========================================================================
// insert a plan_buffer_line if required to handle any hysteresis
void Hysteresis::InsertCorrection(const float &x, const float &y, const float &z, const float &e)
{
  long destination[NUM_AXIS] = {x*axis_steps_per_unit[X_AXIS],y*axis_steps_per_unit[Y_AXIS],z*axis_steps_per_unit[Z_AXIS],e*axis_steps_per_unit[E_AXIS]};
  unsigned char direction_bits = calc_direction_bits( position, destination );
  unsigned char move_bits = calc_move_bits(position, destination);
  // if the direction has changed in any of the axis that need hysteresis corrections...
  unsigned char direction_change_bits = (direction_bits ^ m_prev_direction_bits) & move_bits;
  if( (direction_change_bits & m_hysteresis_bits) != 0 )
  {
    // calculate the position to move to that will fix the hysteresis
    for(int axis=0;axis<NUM_AXIS;++axis)
    {
      // if this axis changed direction...
      if( direction_change_bits & (1<<axis) )
      {
        long fix = (((direction_bits&(1<<axis))!=0)?-m_hysteresis_steps[axis]:m_hysteresis_steps[axis]);
        //... add the hysteresis: move the current position in the opposite direction so that the next travel move is longer
        position[axis] -= fix;
        axis_shift[axis] += fix;
      }
    }
  }

/*
        // debug output to display any hysteresis corrections.
        SERIAL_PROTOCOLPGM("From=X");
        SERIAL_PROTOCOL(current_position[X_AXIS]);
        SERIAL_PROTOCOLPGM(" Y");
        SERIAL_PROTOCOL(current_position[Y_AXIS]);
        SERIAL_PROTOCOLPGM(" Z");
        SERIAL_PROTOCOL(current_position[Z_AXIS]);
        SERIAL_PROTOCOLPGM(" E");      
        SERIAL_PROTOCOL(current_position[E_AXIS]);

        SERIAL_PROTOCOLLN("");

        SERIAL_PROTOCOLPGM("  To=X");
        SERIAL_PROTOCOL(fixed_pos[X_AXIS]);
        SERIAL_PROTOCOLPGM(" Y");
        SERIAL_PROTOCOL(fixed_pos[Y_AXIS]);
        SERIAL_PROTOCOLPGM(" Z");
        SERIAL_PROTOCOL(fixed_pos[Z_AXIS]);
        SERIAL_PROTOCOLPGM(" E");      
        SERIAL_PROTOCOL(fixed_pos[E_AXIS]);
        

        SERIAL_PROTOCOLLN("");
*/
  m_prev_direction_bits = (direction_bits & move_bits) | (m_prev_direction_bits & ~move_bits);
}

