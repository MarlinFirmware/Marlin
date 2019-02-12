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

#include <L6470.h>

#if ENABLED(L6470_CHITCHAT)
  #define L6470_EOL()           SERIAL_EOL()
  #define L6470_CHAR(C)         SERIAL_CHAR(C)
  #define L6470_ECHO(V)         SERIAL_ECHO(V)
  #define L6470_ECHOLN(V)       SERIAL_ECHOLN(V)
  #define L6470_ECHOPGM(S)      SERIAL_ECHOPGM(S)
  #define L6470_ECHOLNPGM(S)    SERIAL_ECHOLNPGM(S)
  #define L6470_ECHOPAIR(S,V)   SERIAL_ECHOPAIR(S,V)
  #define L6470_ECHOLNPAIR(S,V) SERIAL_ECHOLNPAIR(S,V)
#else
  #define L6470_EOL()           NOOP
  #define L6470_CHAR(C)         NOOP
  #define L6470_ECHO(V)         NOOP
  #define L6470_ECHOLN(V)       NOOP
  #define L6470_ECHOPGM(S)      NOOP
  #define L6470_ECHOLNPGM(S)    NOOP
  #define L6470_ECHOPAIR(S,V)   NOOP
  #define L6470_ECHOLNPAIR(S,V) NOOP
#endif

#define L6470_GETPARAM(P,Q) stepper##Q.GetParam(P)

#define MAX_L6470  (7 + MAX_EXTRUDERS) // Maximum number of axes in Marlin

#define L6470_ERROR_MASK  (STATUS_UVLO | STATUS_TH_WRN | STATUS_TH_SD  | STATUS_OCD | STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B)
#define dSPIN_STEP_CLOCK_FWD dSPIN_STEP_CLOCK
#define dSPIN_STEP_CLOCK_REV dSPIN_STEP_CLOCK+1
#define HAS_L6470_EXTRUDER ( AXIS_DRIVER_TYPE_E0(L6470) || AXIS_DRIVER_TYPE_E1(L6470) || AXIS_DRIVER_TYPE_E2(L6470) \
                          || AXIS_DRIVER_TYPE_E3(L6470) || AXIS_DRIVER_TYPE_E4(L6470) || AXIS_DRIVER_TYPE_E5(L6470) )

class L6470_Marlin {
public:
  static bool index_to_dir[MAX_L6470];
  static uint8_t axis_xref[MAX_L6470];
  static char index_to_axis[MAX_L6470][3];
  static uint8_t dir_commands[MAX_L6470];

  // flags to guarantee graceful switch if stepper interrupts L6470 SPI transfer
  static volatile bool spi_abort;
  static bool spi_active;

  L6470_Marlin() {}

  static uint16_t get_status(const uint8_t axis);

  static uint32_t get_param(uint8_t axis, uint8_t param);

  static void set_param(uint8_t axis, uint8_t param, uint32_t value);

  static bool get_user_input(uint8_t &driver_count, uint8_t axis_index[3], char axis_mon[3][3],
                             float &position_max, float &position_min, float &final_feedrate, uint8_t &kval_hold,
                             bool over_current_flag, uint8_t &OCD_TH_val, uint8_t &STALL_TH_val, uint16_t &over_current_threshold);

  static void error_status_decode(const uint16_t status, const uint8_t axis);

  static void monitor_driver();

  static void init();
  static void init_to_defaults();

  static void say_axis(const uint8_t axis, const bool label=true);

private:
  void populate_chain_array();
};

extern L6470_Marlin L6470;
