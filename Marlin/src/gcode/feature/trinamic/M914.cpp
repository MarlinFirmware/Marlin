/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(HAVE_TMC2130) && ENABLED(SENSORLESS_HOMING)

#include "../../gcode.h"
#include "../../../feature/tmc2130.h"
#include "../../../module/stepper_indirection.h"

inline void tmc2130_get_sgt(TMC2130Stepper &st, const char name) {
  SERIAL_CHAR(name);
  SERIAL_ECHOPGM(" driver homing sensitivity set to ");
  SERIAL_ECHOLN(st.sgt());
}
inline void tmc2130_set_sgt(TMC2130Stepper &st, const char name, const int8_t sgt_val) {
  st.sgt(sgt_val);
  tmc2130_get_sgt(st, name);
}

/**
 * M914: Set SENSORLESS_HOMING sensitivity.
 */
void GcodeSuite::M914() {
  #if ENABLED(X_IS_TMC2130)
    if (parser.seen(axis_codes[X_AXIS])) tmc2130_set_sgt(stepperX, 'X', parser.value_int());
    else tmc2130_get_sgt(stepperX, 'X');
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (parser.seen(axis_codes[Y_AXIS])) tmc2130_set_sgt(stepperY, 'Y', parser.value_int());
    else tmc2130_get_sgt(stepperY, 'Y');
  #endif
}

#endif // HAVE_TMC2130 && SENSORLESS_HOMING
