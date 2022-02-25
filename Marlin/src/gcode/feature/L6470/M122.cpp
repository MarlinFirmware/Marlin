/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_L64XX

#include "../../gcode.h"
#include "../../../libs/L64XX/L64XX_Marlin.h"
#include "../../../module/stepper/indirection.h"

void echo_yes_no(const bool yes);

inline void L6470_say_status(const L64XX_axis_t axis) {
  if (L64xxManager.spi_abort) return;
  const L64XX_Marlin::L64XX_shadow_t &sh = L64xxManager.shadow;
  L64xxManager.get_status(axis);
  L64xxManager.say_axis(axis);
  #if ENABLED(L6470_CHITCHAT)
    char temp_buf[20];
    sprintf_P(temp_buf, PSTR("   status: %4x   "), sh.STATUS_AXIS_RAW);
    SERIAL_ECHO(temp_buf);
    print_bin(sh.STATUS_AXIS_RAW);
    switch (sh.STATUS_AXIS_LAYOUT) {
      case L6470_STATUS_LAYOUT: SERIAL_ECHOPGM("   L6470"); break;
      case L6474_STATUS_LAYOUT: SERIAL_ECHOPGM("   L6474"); break;
      case L6480_STATUS_LAYOUT: SERIAL_ECHOPGM("   L6480/powerSTEP01"); break;
    }
  #endif
  SERIAL_ECHOPGM("\n...OUTPUT: ");
  SERIAL_ECHOF(sh.STATUS_AXIS & STATUS_HIZ ? F("OFF") : F("ON "));
  SERIAL_ECHOPGM("   BUSY: "); echo_yes_no((sh.STATUS_AXIS & STATUS_BUSY) == 0);
  SERIAL_ECHOPGM("   DIR: ");
  SERIAL_ECHOF((((sh.STATUS_AXIS & STATUS_DIR) >> 4) ^ L64xxManager.index_to_dir[axis]) ? F("FORWARD") : F("REVERSE"));
  if (sh.STATUS_AXIS_LAYOUT == L6480_STATUS_LAYOUT) {
    SERIAL_ECHOPGM("   Last Command: ");
    if (sh.STATUS_AXIS & sh.STATUS_AXIS_WRONG_CMD) SERIAL_ECHOPGM("VALID");
    else                                           SERIAL_ECHOPGM("ERROR");
    SERIAL_ECHOPGM("\n...THERMAL: ");
    switch ((sh.STATUS_AXIS & (sh.STATUS_AXIS_TH_SD | sh.STATUS_AXIS_TH_WRN)) >> 11) {
      case 0: SERIAL_ECHOPGM("DEVICE SHUTDOWN"); break;
      case 1: SERIAL_ECHOPGM("BRIDGE SHUTDOWN"); break;
      case 2: SERIAL_ECHOPGM("WARNING        "); break;
      case 3: SERIAL_ECHOPGM("OK             "); break;
    }
  }
  else {
    SERIAL_ECHOPGM("   Last Command: ");
    if (!(sh.STATUS_AXIS & sh.STATUS_AXIS_WRONG_CMD)) SERIAL_ECHOPGM("IN");
    SERIAL_ECHOPGM("VALID    ");
    SERIAL_ECHOF(sh.STATUS_AXIS & sh.STATUS_AXIS_NOTPERF_CMD ?  F("COMPLETED    ") : F("Not PERFORMED"));
    SERIAL_ECHOPGM("\n...THERMAL: ", !(sh.STATUS_AXIS & sh.STATUS_AXIS_TH_SD) ? "SHUTDOWN       " : !(sh.STATUS_AXIS & sh.STATUS_AXIS_TH_WRN) ? "WARNING        " : "OK             ");
  }
  SERIAL_ECHOPGM("   OVERCURRENT:"); echo_yes_no((sh.STATUS_AXIS & sh.STATUS_AXIS_OCD) == 0);
  if (sh.STATUS_AXIS_LAYOUT != L6474_STATUS_LAYOUT) {
    SERIAL_ECHOPGM("   STALL:"); echo_yes_no((sh.STATUS_AXIS & sh.STATUS_AXIS_STEP_LOSS_A) == 0 || (sh.STATUS_AXIS & sh.STATUS_AXIS_STEP_LOSS_B) == 0);
    SERIAL_ECHOPGM("   STEP-CLOCK MODE:"); echo_yes_no((sh.STATUS_AXIS & sh.STATUS_AXIS_SCK_MOD) != 0);
  }
  else {
    SERIAL_ECHOPGM("   STALL: NA "
                   "   STEP-CLOCK MODE: NA"
                   "   UNDER VOLTAGE LOCKOUT: "); echo_yes_no((sh.STATUS_AXIS & sh.STATUS_AXIS_UVLO) == 0);
  }
  SERIAL_EOL();
}

/**
 * M122: Debug L6470 drivers
 */
void GcodeSuite::M122() {
  L64xxManager.pause_monitor(true); // Keep monitor_driver() from stealing status
  L64xxManager.spi_active = true;   // Tell set_directions() a series of SPI transfers is underway

  //if (parser.seen('S'))
  //  tmc_set_report_interval(parser.value_bool());
  //else

  #if AXIS_IS_L64XX(X)
    L6470_say_status(X);
  #endif
  #if AXIS_IS_L64XX(X2)
    L6470_say_status(X2);
  #endif
  #if AXIS_IS_L64XX(Y)
    L6470_say_status(Y);
  #endif
  #if AXIS_IS_L64XX(Y2)
    L6470_say_status(Y2);
  #endif
  #if AXIS_IS_L64XX(Z)
    L6470_say_status(Z);
  #endif
  #if AXIS_IS_L64XX(Z2)
    L6470_say_status(Z2);
  #endif
  #if AXIS_IS_L64XX(Z3)
    L6470_say_status(Z3);
  #endif
  #if AXIS_IS_L64XX(Z4)
    L6470_say_status(Z4);
  #endif
  #if AXIS_IS_L64XX(E0)
    L6470_say_status(E0);
  #endif
  #if AXIS_IS_L64XX(E1)
    L6470_say_status(E1);
  #endif
  #if AXIS_IS_L64XX(E2)
    L6470_say_status(E2);
  #endif
  #if AXIS_IS_L64XX(E3)
    L6470_say_status(E3);
  #endif
  #if AXIS_IS_L64XX(E4)
    L6470_say_status(E4);
  #endif
  #if AXIS_IS_L64XX(E5)
    L6470_say_status(E5);
  #endif
  #if AXIS_IS_L64XX(E6)
    L6470_say_status(E6);
  #endif
  #if AXIS_IS_L64XX(E7)
    L6470_say_status(E7);
  #endif

  L64xxManager.spi_active = false;   // done with all SPI transfers - clear handshake flags
  L64xxManager.spi_abort = false;
  L64xxManager.pause_monitor(false);
}

#endif // HAS_L64XX
