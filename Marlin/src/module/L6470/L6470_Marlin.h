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

#if HAS_DRIVER(L6470)

#include <L6470.h>

#define MAX_L6470  (7 + MAX_EXTRUDERS) // Maximum number of axes in Marlin

#define L6470_ERROR_MASK  (STATUS_UVLO | STATUS_TH_WRN | STATUS_TH_SD  | STATUS_OCD | STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B)
#define dSPIN_STEP_CLOCK_FWD dSPIN_STEP_CLOCK
#define dSPIN_STEP_CLOCK_REV dSPIN_STEP_CLOCK+1
#define HAS_L6470_EXTRUDER ( AXIS_DRIVER_TYPE(E0, L6470) || AXIS_DRIVER_TYPE(E1, L6470) || AXIS_DRIVER_TYPE(E2, L6470) \
                          || AXIS_DRIVER_TYPE(E3, L6470) || AXIS_DRIVER_TYPE(E4, L6470) || AXIS_DRIVER_TYPE(E5, L6470) )

#if HAS_L6470_EXTRUDER
  #define L6470_E0_INDEX  ((L6470_driver_enum)E0)       // Start of extruder indexes
  extern uint8_t L6470_E_dir_commands[MAX_EXTRUDERS];   // Array to hold direction command for each extruder
#endif

extern bool L6470_index_to_DIR[MAX_L6470];
extern uint8_t L6470_axis_xref[MAX_L6470];
extern char L6470_index_to_Axis[MAX_L6470][3];

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

#endif // HAS_DRIVER(L6470)
