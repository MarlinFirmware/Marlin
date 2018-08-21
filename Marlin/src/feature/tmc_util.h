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

#include "../inc/MarlinConfigPre.h"

#if HAS_DRIVER(TMC2130)
  #include <TMC2130Stepper.h>
#endif

#if HAS_DRIVER(TMC2208)
  #include <TMC2208Stepper.h>
#endif

extern bool report_tmc_status;

enum TMC_AxisEnum : char { TMC_X, TMC_Y, TMC_Z, TMC_X2, TMC_Y2, TMC_Z2, TMC_E0, TMC_E1, TMC_E2, TMC_E3, TMC_E4 };

constexpr uint32_t _tmc_thrs(const uint16_t msteps, const int32_t thrs, const uint32_t spmm) {
  return 12650000UL * msteps / (256 * thrs * spmm);
}

void _tmc_say_axis(const TMC_AxisEnum axis);
void _tmc_say_current(const TMC_AxisEnum axis, const uint16_t curr);
void _tmc_say_otpw(const TMC_AxisEnum axis, const bool otpw);
void _tmc_say_otpw_cleared(const TMC_AxisEnum axis);
void _tmc_say_pwmthrs(const TMC_AxisEnum axis, const uint32_t thrs);
void _tmc_say_sgt(const TMC_AxisEnum axis, const int8_t sgt);
void _tmc_say_off_time(const TMC_AxisEnum axis, const uint8_t off_time);
void _tmc_say_fast_decay_time(const TMC_AxisEnum axis, const uint8_t fast_decay_time);
void _tmc_say_blank_time(const TMC_AxisEnum axis, const uint8_t blank_time);
void _tmc_say_hysteresis_end(const TMC_AxisEnum axis, const int8_t hysteresis_end);
void _tmc_say_hysteresis_start(const TMC_AxisEnum axis, const uint8_t hysteresis_start);
void _tmc_say_stealth_gradient(const TMC_AxisEnum axis, const uint8_t stealth_gradien);
void _tmc_say_stealth_amplitude(const TMC_AxisEnum axis, const uint8_t stealth_amplitude);
void _tmc_say_stealth_freq(const TMC_AxisEnum axis, const uint8_t stealth_freq);
void _tmc_say_stealth_autoscale(const TMC_AxisEnum axis, const bool stealth_autoscale);				
void _tmc_say_disable_I_comparator(const TMC_AxisEnum axis, const bool disable_I_comparator);							       

template<typename TMC>
void tmc_get_current(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_current(axis, st.getCurrent());
}
template<typename TMC>
void tmc_set_current(TMC &st, const int mA) {
  st.setCurrent(mA, R_SENSE, HOLD_MULTIPLIER);
}
template<typename TMC>
void tmc_report_otpw(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_otpw(axis, st.getOTPW());
}
template<typename TMC>
void tmc_clear_otpw(TMC &st, const TMC_AxisEnum axis) {
  st.clear_otpw();
  _tmc_say_otpw_cleared(axis);
}
template<typename TMC>
void tmc_get_pwmthrs(TMC &st, const TMC_AxisEnum axis, const uint16_t spmm) {
  _tmc_say_pwmthrs(axis, _tmc_thrs(st.microsteps(), st.TPWMTHRS(), spmm));
}
template<typename TMC>
void tmc_set_pwmthrs(TMC &st, const int32_t thrs, const uint32_t spmm) {
  st.TPWMTHRS(_tmc_thrs(st.microsteps(), thrs, spmm));
}
template<typename TMC>
void tmc_get_sgt(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_sgt(axis, st.sgt());
}
template<typename TMC>
void tmc_set_sgt(TMC &st, const int8_t sgt_val) {
  st.sgt(sgt_val);
}
template<typename TMC>
void tmc_get_off_time(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_off_time(axis, st.off_time());
}
template<typename TMC>
void tmc_set_off_time(TMC &st, const uint8_t off_time_val) {
  st.off_time(off_time_val);
}
template<typename TMC>
void tmc_get_fast_decay_time(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_fast_decay_time(axis, st.fast_decay_time());
  _tmc_say_fast_decay_time(axis, st.hysteresis_start());
}
template<typename TMC>
void tmc_set_fast_decay_time(TMC &st, const uint8_t fast_decay_time_val) {
  st.fast_decay_time(fast_decay_time_val);
  st.hysteresis_start(fast_decay_time_val << 1);
}
template<typename TMC>
void tmc_get_blank_time(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_blank_time(axis, st.blank_time());
}
template<typename TMC>
void tmc_set_blank_time(TMC &st, const uint8_t blank_time_val) {
  st.blank_time(blank_time_val);
}
template<typename TMC>
void tmc_get_hysteresis_end(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_hysteresis_end(axis, st.hysteresis_end());
}
template<typename TMC>
void tmc_set_hysteresis_end(TMC &st, const int8_t hysteresis_end_val) {
  st.hysteresis_end(hysteresis_end_val);
}
template<typename TMC>
void tmc_get_hysteresis_start(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_hysteresis_start(axis, st.hysteresis_start());
}
template<typename TMC>
void tmc_set_hysteresis_start(TMC &st, const uint8_t hysteresis_start_val) {
  st.hysteresis_start(hysteresis_start_val);
}
template<typename TMC>
void tmc_get_stealth_gradient(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_stealth_gradient(axis, st.stealth_gradient());
}
template<typename TMC>
void tmc_set_stealth_gradient(TMC &st, const uint8_t stealth_gradient_val) {
  st.stealth_gradient(stealth_gradient_val);
}
template<typename TMC>
void tmc_get_stealth_amplitude(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_stealth_amplitude(axis, st.stealth_amplitude());
}
template<typename TMC>
void tmc_set_stealth_amplitude(TMC &st, const uint8_t stealth_amplitude_val) {
  st.stealth_amplitude(stealth_amplitude_val);
}
template<typename TMC>
void tmc_get_stealth_freq(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_stealth_freq(axis, st.stealth_freq());
}
template<typename TMC>
void tmc_set_stealth_freq(TMC &st, const uint8_t stealth_freq_val) {
  st.stealth_freq(stealth_freq_val);
}
template<typename TMC>
void tmc_get_stealth_autoscale(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_stealth_autoscale(axis, st.stealth_autoscale());
}
template<typename TMC>
void tmc_set_stealth_autoscale(TMC &st, const bool stealth_autoscale_val) {
  st.stealth_autoscale(stealth_autoscale_val);
}
template<typename TMC>
void tmc_get_disable_I_comparator(TMC &st, const TMC_AxisEnum axis) {
  _tmc_say_disable_I_comparator(axis, st.disable_I_comparator());
}
template<typename TMC>
void tmc_set_disable_I_comparator(TMC &st, const bool disable_I_comparator_val) {
  st.disable_I_comparator(disable_I_comparator_val);
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
  void tmc_sensorless_homing(TMC2130Stepper &st, const bool enable=true);
#endif

#if HAS_DRIVER(TMC2130)
  void tmc_init_cs_pins();
#endif

#endif // _TMC_UTIL_H_
