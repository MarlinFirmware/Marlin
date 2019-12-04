/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(CNC_5X)

#include "../../gcode/gcode.h"
#include "../../feature/bedlevel/bedlevel.h"

#include "../../Marlin.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/motion.h"
#include "../../module/tool_change.h"
#include "../../module/temperature.h"
#include "../../lcd/ultralcd.h"
#include "../../lcd/menu/menu.h"

extern bool fast_move;/*=false*/




/**
 *   M168 - 5 Axis Simple Convertion
 *
 *  X Y Z - position of cartesians axis
 *  E A B - position of rotary axis
 *  
 * 
 */

// External references

// Private functions


float X_5,
      Y_5,
      Z_5,
      IX_5,
      IY_5,
      IZ_5,
      OX_5,
      OY_5,
      OZ_5,
      A_5,
      B_5,
      C_5,
      IA_5,
      IB_5,
      IC_5,
      OA_5,
      OB_5,
      OC_5,
      E_5,
      I,
      J = 0;

      

/**
 * M168
 */
void GcodeSuite::M168()
{
  
    if (parser.seen('X')) X_5 = parser.value_float();

      SERIAL_ECHO("X");
      SERIAL_ECHOLN(X_5);

    if (parser.seen('Y')) Y_5 = parser.value_float();

      SERIAL_ECHO("Y");
      SERIAL_ECHOLN(Y_5);
    
    if (parser.seen('Z')) Z_5 = parser.value_float();

      SERIAL_ECHO("Z");
      SERIAL_ECHOLN(Z_5);

    if (parser.seen('A')) A_5 = parser.value_float();
    
      SERIAL_ECHO("A");
      SERIAL_ECHOLN(A_5);

    if (parser.seen('B')) B_5 = parser.value_float();
   
      SERIAL_ECHO("B");
      SERIAL_ECHOLN(B_5);

    if (parser.seen('C')) C_5 = parser.value_float();
   
      SERIAL_ECHO("C");
      SERIAL_ECHOLN(C_5);


    #if ENABLED (A_5x) && ENABLED(B_5x)
        
      I=A_5;
      J=B_5;

    #endif

    #if ENABLED (A_5x) && ENABLED(C_5x)

      I=A_5;
      J=C_5;

    #endif

    #if ENABLED (B_5x) && ENABLED(C_5x)

      I=B_5;
      J=C_5;

    #endif

      destination[X_AXIS] = X_5;
      destination[Y_AXIS] = Y_5;
      destination[Z_AXIS] = Z_5;
      destination[E0_AXIS] = I;
      destination[E1_AXIS] = J;
    
      prepare_move_to_destination();
      
      planner.synchronize();

      //COPY(current_position, current_position5);

      SERIAL_ECHO("I");
      SERIAL_ECHOLN(current_position[E0_AXIS]);

      SERIAL_ECHO("J");
      SERIAL_ECHOLN(current_position[E1_AXIS]);


}

#endif

