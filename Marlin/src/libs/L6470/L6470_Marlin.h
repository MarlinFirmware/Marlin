/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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


extern bool L6470_index_to_DIR[MAX_L6470];
extern uint8_t L6470_axis_xref[MAX_L6470];
extern char L6470_index_to_Axis[MAX_L6470][3];
extern uint8_t L6470_dir_commands[MAX_L6470];
extern volatile bool L6470_SPI_abort;           // flags to guarantee graceful switch if stepper interrupts L6470 SPI transfer
extern bool L6470_SPI_active;                   // flags to guarantee graceful switch if stepper interrupts L6470 SPI transfer

uint16_t L6470_get_status(uint8_t axis);

uint32_t L6470_get_param(uint8_t axis, uint8_t param);

void L6470_set_param(uint8_t axis, uint8_t param, uint32_t value);

bool L6470_get_user_input(uint8_t &driver_count, uint8_t axis_index[3], char axis_mon[3][3],
                          float &position_max, float &position_min, float &final_feedrate, uint8_t &kval_hold,
                          bool over_current_flag, uint8_t &OCD_TH_val, uint8_t &STALL_TH_val, uint16_t &over_current_threshold);

void print_bin(const uint16_t val);

void L6470_error_status_decode(const uint16_t status, const uint8_t axis);

void monitor_L6470_driver();

void L6470_init();
void L6470_SPI_init();
void L6470_init_to_defaults();
void L6470_Transfer(uint8_t L6470_buf[], const uint8_t length);
uint8_t L6470_Transfer(uint8_t data, int _SSPin, const uint8_t chain_position);

void L6470_say_axis(const uint8_t axis, const bool label=true);
