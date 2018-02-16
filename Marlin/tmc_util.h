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

#include "MarlinConfig.h"

extern bool report_tmc_status;
extern char extended_axis_codes[11][3];

enum TMC_AxisEnum {
  TMC_X, TMC_X2, TMC_Y, TMC_Y2, TMC_Z, TMC_Z2,
  TMC_E0, TMC_E1, TMC_E2, TMC_E3, TMC_E4
};

constexpr uint32_t _tmc_thrs(const uint16_t msteps, const int32_t thrs, const uint32_t spmm) {
  return 12650000UL * msteps / (256 * thrs * spmm);
}

void _tmc_say_current(const char name[], const uint16_t curr);
void _tmc_say_otpw(const char name[], const bool otpw);
void _tmc_say_otpw_cleared(const char name[]);
void _tmc_say_pwmthrs(const char name[], const uint32_t thrs);
void _tmc_say_sgt(const char name[], const int8_t sgt);

template<typename TMC>
void tmc_get_current(TMC &st, const char name[]) {
  _tmc_say_current(name, st.getCurrent());
}
template<typename TMC>
void tmc_set_current(TMC &st, const char name[], const int mA) {
  st.setCurrent(mA, R_SENSE, HOLD_MULTIPLIER);
  tmc_get_current(st, name);
}
template<typename TMC>
void tmc_report_otpw(TMC &st, const char name[]) {
  _tmc_say_otpw(name, st.getOTPW());
}
template<typename TMC>
void tmc_clear_otpw(TMC &st, const char name[]) {
  st.clear_otpw();
  _tmc_say_otpw_cleared(name);
}
template<typename TMC>
void tmc_get_pwmthrs(TMC &st, const char name[], const uint16_t spmm) {
  _tmc_say_pwmthrs(name, _tmc_thrs(st.microsteps(), st.TPWMTHRS(), spmm));
}
template<typename TMC>
void tmc_set_pwmthrs(TMC &st, const char name[], const int32_t thrs, const uint32_t spmm) {
  st.TPWMTHRS(_tmc_thrs(st.microsteps(), thrs, spmm));
  tmc_get_pwmthrs(st, name, spmm);
}
template<typename TMC>
void tmc_get_sgt(TMC &st, const char name[]) {
  _tmc_say_sgt(name, st.sgt());
}
template<typename TMC>
void tmc_set_sgt(TMC &st, const char name[], const int8_t sgt_val) {
  st.sgt(sgt_val);
  tmc_get_sgt(st, name);
}

void monitor_tmc_driver();

#if ENABLED(TMC_DEBUG)
  void tmc_set_report_status(const bool status);
  void tmc_report_all();
#endif

/**
 * TMC2130 specific sensorless homing using stallGuard2.
 * stallGuard2 only works when in spreadCycle mode.
 * spreadCycle and stealthChop are mutually exclusive.
 *
 * Defined here because of limitations with templates and headers.
 */
#if ENABLED(SENSORLESS_HOMING)
  void tmc_sensorless_homing(TMC2130Stepper &st, bool enable=true);
#endif

#if ENABLED(HAVE_TMC2130)
  void tmc_init_cs_pins();
#endif

#endif // _TMC_UTIL_H_
