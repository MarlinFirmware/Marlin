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

#if HAS_DRIVER(ST_L6470)

#include "../../gcode.h"
#include "../../../module/L6470/L6470_Marlin.h"
#include "../../../module/stepper_indirection.h"

inline void echo_yes_no(const bool yes) { serialprintPGM(yes ? PSTR(" YES") : PSTR(" NO ")); }

void L6470_status_decode(const uint16_t status, const uint8_t axis) {
  char temp_buf[10];
  SERIAL_ECHOPAIR("AXIS: ", L6470_index_to_Axis[axis][0]);
  SERIAL_ECHO(L6470_index_to_Axis[axis][1]);
  sprintf_P(temp_buf, PSTR("  %4x   "), status);
  SERIAL_ECHO(temp_buf);
  print_bin(status);
  SERIAL_ECHOPGM("\n...OUTPUT: ");
  serialprintPGM(status & STATUS_HIZ ? PSTR("OFF") : PSTR("ON "));
  SERIAL_ECHOPGM("   BUSY: "); echo_yes_no(!(status & STATUS_BUSY));
  SERIAL_ECHOPGM("   DIR: ");
  serialprintPGM((((status & STATUS_DIR) >> 4) ^ L6470_index_to_DIR[axis]) ? PSTR("FORWARD") : PSTR("REVERSE"));
  SERIAL_ECHOPGM("   Last Command: ");
  if (status & STATUS_WRONG_CMD) SERIAL_ECHOPGM("IN");
  SERIAL_ECHOPGM("VALID    ");
  serialprintPGM(status & STATUS_NOTPERF_CMD ? PSTR("Not PERFORMED") : PSTR("COMPLETED    "));
  SERIAL_ECHOPAIR("\n...THERMAL: ", !(status & STATUS_TH_SD) ? "SHUTDOWN" : !(status & STATUS_TH_WRN) ? "WARNING " : "OK      ");
  SERIAL_ECHOPGM("   OVERCURRENT:"); echo_yes_no(status & STATUS_OCD);
  SERIAL_ECHOPGM("   STALL:"); echo_yes_no(!(status & STATUS_STEP_LOSS_A) || !(status & STATUS_STEP_LOSS_B));
  SERIAL_ECHOPGM("   STEP-CLOCK MODE:") echo_yes_no(status & STATUS_SCK_MOD);
  SERIAL_EOL();
}

/**
 * M122: Debug L6470 drivers
 */
void GcodeSuite::M122() {

  #define L6470_SAY_STATUS(Q) L6470_status_decode(stepper##Q.getStatus(), Q)

  //if (parser.seen('S'))
  // tmc_set_report_status(parser.value_bool());
  //else

  #if AXIS_DRIVER_TYPE_X(ST_L6470)
    L6470_SAY_STATUS(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(ST_L6470)
    L6470_SAY_STATUS(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(ST_L6470)
    L6470_SAY_STATUS(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
    L6470_SAY_STATUS(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(ST_L6470)
    L6470_SAY_STATUS(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
    L6470_SAY_STATUS(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
    L6470_SAY_STATUS(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_E0(ST_L6470)
    L6470_SAY_STATUS(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(ST_L6470)
    L6470_SAY_STATUS(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(ST_L6470)
    L6470_SAY_STATUS(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(ST_L6470)
    L6470_SAY_STATUS(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(ST_L6470)
    L6470_SAY_STATUS(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(ST_L6470)
    L6470_SAY_STATUS(E5);
  #endif
}

#endif // HAS_DRIVER(ST_L6470)
