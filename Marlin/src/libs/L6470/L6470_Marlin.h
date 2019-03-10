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
#pragma once

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

#define dSPIN_STEP_CLOCK_FWD dSPIN_STEP_CLOCK
#define dSPIN_STEP_CLOCK_REV dSPIN_STEP_CLOCK+1
#define HAS_L64XX_EXTRUDER (AXIS_IS_L64XX(E0) || AXIS_IS_L64XX(E1) || AXIS_IS_L64XX(E2) || AXIS_IS_L64XX(E3) || AXIS_IS_L64XX(E4) || AXIS_IS_L64XX(E5))

typedef enum : uint8_t { X, Y, Z, X2, Y2, Z2, Z3, E0, E1, E2, E3, E4, E5 } L6470_axis_t;

class L6470_Marlin : public L64XXHelper {
public:
  static const char * const index_to_axis[MAX_L6470];

  static bool index_to_dir[MAX_L6470];
  static uint8_t dir_commands[MAX_L6470];

  // Flags to guarantee graceful switch if stepper interrupts L6470 SPI transfer
  static volatile bool spi_abort;
  static bool spi_active;

  L6470_Marlin() { L64XX::set_helper(this); }

  static void init();
  static void init_to_defaults();

  static uint16_t get_stepper_status(L64XX &st);

  static uint16_t get_status(const L6470_axis_t axis);

  static uint32_t get_param(const L6470_axis_t axis, const uint8_t param);

  static void set_param(const L6470_axis_t axis, const uint8_t param, const uint32_t value);

  //static void send_command(const L6470_axis_t axis, uint8_t command);

  static bool get_user_input(uint8_t &driver_count, L6470_axis_t axis_index[3], char axis_mon[3][3],
                            float &position_max, float &position_min, float &final_feedrate, uint8_t &kval_hold,
                            bool over_current_flag, uint8_t &OCD_TH_val, uint8_t &STALL_TH_val, uint16_t &over_current_threshold);

  static void monitor_update(uint8_t stepper_index, uint16_t status);

  static void monitor_driver();

  static void transfer(uint8_t L6470_buf[], const uint8_t length);

  //static char* index_to_axis(const uint8_t index);
  static void say_axis(const L6470_axis_t axis, const bool label=true);
  static void error_status_decode(const uint16_t status, const L6470_axis_t axis);

  // ~40 bytes SRAM to simplify status decode routines
  typedef struct {
    bool STATUS_AXIS_LAYOUT;              // Copy of L6470_status_layout
    uint8_t AXIS_OCD_TH_MAX;              // Size of OCD_TH field
    uint8_t AXIS_STALL_TH_MAX;            // Size of STALL_TH field
    float AXIS_OCD_CURRENT_CONSTANT_INV;   // mA per count
    float AXIS_STALL_CURRENT_CONSTANT_INV; // mA per count
    uint8_t L6470_AXIS_CONFIG,            // Address of the CONFIG register
            L6470_AXIS_STATUS;            // Address of the STATUS register
    uint16_t L6470_ERROR_MASK,            // STATUS_UVLO | STATUS_TH_WRN | STATUS_TH_SD  | STATUS_OCD | STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B
             STATUS_AXIS_RAW,             // Copy of status register contents
             STATUS_AXIS,                 // Copy of status register contents but with all error bits active low
             STATUS_AXIS_OCD,             // Overcurrent detected bit position
             STATUS_AXIS_SCK_MOD,         // Step clock mode is active bit position
             STATUS_AXIS_STEP_LOSS_A,     // Stall detected on A bridge bit position
             STATUS_AXIS_STEP_LOSS_B,     // Stall detected on B bridge bit position
             STATUS_AXIS_TH_SD,           // Thermal shutdown bit position
             STATUS_AXIS_TH_WRN,          // Thermal warning bit position
             STATUS_AXIS_UVLO,            // Undervoltage lockout is active bit position
             STATUS_AXIS_WRONG_CMD,       // Last command not valid bit position
             STATUS_AXIS_CMD_ERR,         // Command error bit position
             STATUS_AXIS_NOTPERF_CMD;     // Last command not performed bit position
  } L64XX_shadow_t;

  static L64XX_shadow_t shadow;

  //static uint32_t UVLO_ADC;               // ADC undervoltage event

protected:
  // L64XXHelper methods
  static void spi_init();
  static uint8_t transfer(uint8_t data, int16_t ss_pin);
  static uint8_t transfer(uint8_t data, int16_t ss_pin, uint8_t chain_position);
};

extern L6470_Marlin L64helper;
