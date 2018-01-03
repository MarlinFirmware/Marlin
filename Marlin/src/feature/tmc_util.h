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

#ifndef _TMC_UTIL_H_
#define _TMC_UTIL_H_

#include <TMC2130Stepper.h>

#include "../inc/MarlinConfig.h"

extern bool report_tmc_status;
extern char extended_axis_codes[11][3];
enum TMC_AxisEnum {
  TMC_X, TMC_X2, TMC_Y, TMC_Y2, TMC_Z, TMC_Z2,
  TMC_E0, TMC_E1, TMC_E2, TMC_E3, TMC_E4
};

template<typename TMC>
void tmc_get_current(TMC &st, const char name[]);
template<typename TMC>
void tmc_set_current(TMC &st, const char name[], const int mA);
template<typename TMC>
void tmc_report_otpw(TMC &st, const char name[]);
template<typename TMC>
void tmc_clear_otpw(TMC &st, const char name[]);
template<typename TMC>
void tmc_get_pwmthrs(TMC &st, const char name[], const uint16_t spmm);
template<typename TMC>
void tmc_set_pwmthrs(TMC &st, const char name[], const int32_t thrs, const uint32_t spmm);
template<typename TMC>
void tmc_get_sgt(TMC &st, const char name[]);
template<typename TMC>
void tmc_set_sgt(TMC &st, const char name[], const int8_t sgt_val);

void _M122();
void monitor_tmc_driver();

/**
 * TMC2130 specific sensorless homing using stallGuard2.
 * stallGuard2 only works when in spreadCycle mode.
 * spreadCycle and stealthChop are mutually exclusive.
 *
 * Defined here because of limitations with templates and headers.
 */
#if ENABLED(SENSORLESS_HOMING)
  template<typename TMC>
  void tmc_sensorless_homing(TMC &st, bool enable=true) {
    #if ENABLED(STEALTHCHOP)
      if (enable) {
        st.coolstep_min_speed(1024UL * 1024UL - 1UL);
        st.stealthChop(0);
      }
      else {
        st.coolstep_min_speed(0);
        st.stealthChop(1);
      }
    #endif

    st.diag1_stall(enable ? 1 : 0);
  }
#endif

#endif // _TMC_UTIL_H_
