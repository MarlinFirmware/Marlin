/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#if HAS_DRIVER(L6470)

#include "../../gcode.h"
#include "../../../libs/L6470/L6470_Marlin.h"
#include "../../../module/stepper_indirection.h"

inline void echo_yes_no(const bool yes) { serialprintPGM(yes ? PSTR(" YES") : PSTR(" NO ")); }

void L6470_status_decode(const uint16_t status, const uint8_t axis) {
  if (L6470.spi_abort) return;  // don't do anything if set_directions() has occurred
  L6470.say_axis(axis);
  #if ENABLED(L6470_CHITCHAT)
    char temp_buf[20];
    sprintf_P(temp_buf, PSTR("   status: %4x   "), status);
    SERIAL_ECHO(temp_buf);
    print_bin(status);
  #endif
  SERIAL_ECHOPGM("\n...OUTPUT: ");
  serialprintPGM(status & STATUS_HIZ ? PSTR("OFF") : PSTR("ON "));
  SERIAL_ECHOPGM("   BUSY: "); echo_yes_no(!(status & STATUS_BUSY));
  SERIAL_ECHOPGM("   DIR: ");
  serialprintPGM((((status & STATUS_DIR) >> 4) ^ L6470.index_to_dir[axis]) ? PSTR("FORWARD") : PSTR("REVERSE"));
  SERIAL_ECHOPGM("   Last Command: ");
  if (status & STATUS_WRONG_CMD) SERIAL_ECHOPGM("IN");
  SERIAL_ECHOPGM("VALID    ");
  serialprintPGM(status & STATUS_NOTPERF_CMD ? PSTR("Not PERFORMED") : PSTR("COMPLETED    "));
  SERIAL_ECHOPAIR("\n...THERMAL: ", !(status & STATUS_TH_SD) ? "SHUTDOWN" : !(status & STATUS_TH_WRN) ? "WARNING " : "OK      ");
  SERIAL_ECHOPGM("   OVERCURRENT:"); echo_yes_no(!(status & STATUS_OCD));
  SERIAL_ECHOPGM("   STALL:"); echo_yes_no(!(status & STATUS_STEP_LOSS_A) || !(status & STATUS_STEP_LOSS_B));
  SERIAL_ECHOPGM("   STEP-CLOCK MODE:"); echo_yes_no(status & STATUS_SCK_MOD);
  SERIAL_EOL();
}

/**
 * M122: Debug L6470 drivers
 */
void GcodeSuite::M122() {

  L6470.spi_active = true;    // let set_directions() know we're in the middle of a series of SPI transfers

  #define L6470_SAY_STATUS(Q) L6470_status_decode(stepper##Q.getStatus(), Q)

  //if (parser.seen('S'))
  // tmc_set_report_interval(parser.value_bool());
  //else

  #if AXIS_DRIVER_TYPE_X(L6470)
    L6470_SAY_STATUS(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(L6470)
    L6470_SAY_STATUS(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(L6470)
    L6470_SAY_STATUS(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(L6470)
    L6470_SAY_STATUS(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(L6470)
    L6470_SAY_STATUS(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(L6470)
    L6470_SAY_STATUS(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(L6470)
    L6470_SAY_STATUS(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_E0(L6470)
    L6470_SAY_STATUS(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(L6470)
    L6470_SAY_STATUS(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(L6470)
    L6470_SAY_STATUS(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(L6470)
    L6470_SAY_STATUS(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(L6470)
    L6470_SAY_STATUS(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(L6470)
    L6470_SAY_STATUS(E5);
  #endif

  L6470.spi_active = false;   // done with all SPI transfers - clear handshake flags
  L6470.spi_abort = false;
}

#endif // HAS_DRIVER(L6470)
