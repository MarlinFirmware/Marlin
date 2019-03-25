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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(LIN_ADVANCE)

#include "../../gcode.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"

#if ENABLED(EXTRA_LIN_ADVANCE_K)
  float saved_extruder_advance_K[EXTRUDERS];
  uint8_t linAdvMode = 0;
#endif


/**
 * M900: Get or Set Linear Advance K-factor
 *  T<tool>     Which tool to address
 *  K<factor>   Set current advance K factor. Slot 0.
 *  L<factor>   Set secondary advance K factor. Slot 1. Requires EXTRA_LIN_ADVANCE_K.
 *  M<0/1>      Set which slot to use. Requires EXTRA_LIN_ADVANCE_K.
 */
void GcodeSuite::M900() {

  #if EXTRUDERS < 2
    constexpr uint8_t tmp_extruder = 0;
  #else
    const uint8_t tmp_extruder = parser.intval('T',active_extruder);// ? parser.value_int() : ;
    if (tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHOLNPGM("?T value out of range.");
      return;
    }
  #endif


  #if ENABLED(EXTRA_LIN_ADVANCE_K)
    if (parser.seenval('M')){
      bool newMode = parser.value_bool();
      if (bitRead(linAdvMode,tmp_extruder) != newMode){
        bitWrite(linAdvMode,tmp_extruder,newMode);
        planner.synchronize();
        float temp = planner.extruder_advance_K[tmp_extruder];
        planner.extruder_advance_K[tmp_extruder] = saved_extruder_advance_K[tmp_extruder];
        saved_extruder_advance_K[tmp_extruder] = temp;
      }
    }

    if (parser.seenval('K')) {
      const float newK = parser.floatval('K');
      if (WITHIN(newK, 0, 10)) {
        if (bitRead(linAdvMode,tmp_extruder)){
          saved_extruder_advance_K[tmp_extruder] = newK;
        }
        else{
          planner.synchronize();
          planner.extruder_advance_K[tmp_extruder] = newK;
        }
      }
      else
        SERIAL_ECHOLNPGM("?K value out of range (0-10).");
    }

    if (parser.seenval('L')) {
      const float newL = parser.floatval('L');
      if (WITHIN(newL, 0, 10)) {
        if (bitRead(linAdvMode,tmp_extruder)){
          planner.synchronize();
          planner.extruder_advance_K[tmp_extruder] = newL;
        }
        else{
          saved_extruder_advance_K[tmp_extruder] = newL;
        }
      }
      else
        SERIAL_ECHOLNPGM("?L value out of range (0-10).");
    }

    if (!parser.seen_any()){
      SERIAL_ECHO_START();
    #if EXTRUDERS < 2
      SERIAL_ECHOLNPAIR("Mode =",bitRead(linAdvMode,0));
      SERIAL_ECHOPAIR_F("Slot ",bitRead(linAdvMode,0));
      SERIAL_ECHOLNPAIR("Advance K=", planner.extruder_advance_K[0]);
      SERIAL_ECHOPAIR_F("Slot ",1-bitRead(linAdvMode,0));
      SERIAL_ECHOLNPAIR("Advance K=", saved_extruder_advance_K[0]);
    #else
      LOOP_L_N(i, EXTRUDERS) {
        SERIAL_ECHOLNPAIR("Extruder ",i);
        SERIAL_ECHOLNPAIR("Mode = ",bitRead(linAdvMode,i));
        SERIAL_ECHOPAIR_F("Slot ",bitRead(linAdvMode,i));
        SERIAL_ECHOLNPAIR(" Advance =", planner.extruder_advance_K[i]);
        SERIAL_ECHOPAIR_F("Slot ",1-bitRead(linAdvMode,i));
        SERIAL_ECHOLNPAIR(" Advance =", saved_extruder_advance_K[i]);
        SERIAL_EOL();
      }
      SERIAL_EOL();
#endif
    }

  #else


  if (parser.seenval('K')) {
    const float newK = parser.floatval('K');
    if (WITHIN(newK, 0, 10)) {
      planner.synchronize();
      planner.extruder_advance_K[tmp_extruder] = newK;
    }
    else
      SERIAL_ECHOLNPGM("?K value out of range (0-10).");
  }
  else {
    SERIAL_ECHO_START();
    #if EXTRUDERS < 2
      SERIAL_ECHOLNPAIR("Advance K=", planner.extruder_advance_K[0]);
    #else
      SERIAL_ECHOPGM("Advance K");
      LOOP_L_N(i, EXTRUDERS) {
        SERIAL_CHAR(' '); SERIAL_ECHO(int(i));
        SERIAL_CHAR('='); SERIAL_ECHO(planner.extruder_advance_K[i]);
      }
      SERIAL_EOL();
    #endif
  }
  #endif
}

#endif // LIN_ADVANCE
