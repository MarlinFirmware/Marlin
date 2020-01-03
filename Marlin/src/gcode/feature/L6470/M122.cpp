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

#if HAS_L64XX

#include "../../gcode.h"
#include "../../../libs/L6470/L6470_Marlin.h"
#include "../../../module/stepper/indirection.h"

inline void echo_yes_no(uint32_t yes) { uint8_t _yes = !(yes == 0); serialprintPGM(_yes ? PSTR(" YES") : PSTR(" NO ")); }

inline void L6470_say_status(const L6470_axis_t axis) {
  if (L64xx_MARLIN.spi_abort) return;
  const L64XX_Marlin::L64XX_shadow_t &sh = L64xx_MARLIN.shadow;
  L64xx_MARLIN.get_status(axis);
  L64xx_MARLIN.say_axis(axis);
  #if ENABLED(L6470_CHITCHAT)
    char temp_buf[20];
    sprintf_P(temp_buf, PSTR("   status: %4x   "), sh.STATUS_AXIS_RAW);
    SERIAL_ECHO(temp_buf);
    print_bin(sh.STATUS_AXIS_RAW);
    switch (sh.STATUS_AXIS_LAYOUT) {
      case 0: {serialprintPGM(PSTR("   L6470")); break;}
      case 1: {serialprintPGM(PSTR("   L6474")); break;}
      case 2: {serialprintPGM(PSTR("   L6480/powerSTEP01")); break;}
    }
  #endif
  SERIAL_ECHOPGM("\n...OUTPUT: ");
  serialprintPGM(sh.STATUS_AXIS & STATUS_HIZ ? PSTR("OFF") : PSTR("ON "));
  SERIAL_ECHOPGM("   BUSY: "); echo_yes_no(!(sh.STATUS_AXIS & STATUS_BUSY));
  SERIAL_ECHOPGM("   DIR: ");
  serialprintPGM((((sh.STATUS_AXIS & STATUS_DIR) >> 4) ^ L64xx_MARLIN.index_to_dir[axis]) ? PSTR("FORWARD") : PSTR("REVERSE"));
  SERIAL_ECHOPGM("   Last Command: ");
  if (sh.STATUS_AXIS & sh.STATUS_AXIS_WRONG_CMD) SERIAL_ECHOPGM("IN");
  SERIAL_ECHOPGM("VALID    ");
  if (sh.STATUS_AXIS_LAYOUT == 2) {
    SERIAL_ECHOPGM("\n...THERMAL: ");
    switch ((sh.STATUS_AXIS & (sh.STATUS_AXIS_TH_SD | sh.STATUS_AXIS_TH_WRN)) >> 11) {
      case 0: SERIAL_ECHOPGM("DEVICE SHUTDOWN"); break;
      case 1: SERIAL_ECHOPGM("BRIDGE SHUTDOWN"); break;
      case 2: SERIAL_ECHOPGM("WARNING        "); break;
      case 3: SERIAL_ECHOPGM("OK             "); break;
    }
  }
  else {
    serialprintPGM(sh.STATUS_AXIS & sh.STATUS_AXIS_NOTPERF_CMD ?  PSTR("COMPLETED    ") : PSTR("Not PERFORMED"));
    SERIAL_ECHOPAIR("\n...THERMAL: ", !(sh.STATUS_AXIS & sh.STATUS_AXIS_TH_SD) ? "SHUTDOWN       " : !(sh.STATUS_AXIS & sh.STATUS_AXIS_TH_WRN) ? "WARNING        " : "OK             ");
  }
  SERIAL_ECHOPGM("   OVERCURRENT:"); echo_yes_no(!(sh.STATUS_AXIS & sh.STATUS_AXIS_OCD));
  if (sh.STATUS_AXIS_LAYOUT != 1) {
    SERIAL_ECHOPGM("   STALL:"); echo_yes_no(!(sh.STATUS_AXIS & sh.STATUS_AXIS_STEP_LOSS_A) || !(sh.STATUS_AXIS & sh.STATUS_AXIS_STEP_LOSS_B));
    SERIAL_ECHOPGM("   STEP-CLOCK MODE:"); echo_yes_no(sh.STATUS_AXIS & sh.STATUS_AXIS_SCK_MOD);
  }
  else {
    SERIAL_ECHOPGM("   STALL: NA ");
    SERIAL_ECHOPGM("   STEP-CLOCK MODE: NA");
    SERIAL_ECHOPGM("   UNDER VOLTAGE LOCKOUT: "); echo_yes_no(!(sh.STATUS_AXIS & sh.STATUS_AXIS_UVLO));
  }
  SERIAL_EOL();
}



/**
 * M122: Debug L6470 drivers
 */
void GcodeSuite::M122() {
  L64xx_active = true;               //  keep monitor_driver() from stealing status
  L64xx_MARLIN.spi_active = true;    // let set_directions() know we're in the middle of a series of SPI transfers

  //if (parser.seen('S'))
  // tmc_set_report_interval(parser.value_bool());
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

  L64xx_MARLIN.spi_active = false;   // done with all SPI transfers - clear handshake flags
  L64xx_MARLIN.spi_abort = false;
  L64xx_active = false;
}

#endif // HAS_L64XX
