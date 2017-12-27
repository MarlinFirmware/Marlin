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

#include "../inc/MarlinConfig.h"

#if HAS_TRINAMIC

#include "tmc_util.h"
#include "../Marlin.h"

#include "../module/stepper_indirection.h"
#include "../module/printcounter.h"
#include "../libs/duration_t.h"
#include "../gcode/gcode.h"

bool report_tmc_status = false;
char extended_axis_codes[11][3] = { "X", "X2", "Y", "Y2", "Z", "Z2", "E0", "E1", "E2", "E3", "E4" };

template<typename TMC>
void tmc_get_current(TMC &st, const char name[]) {
  SERIAL_ECHO(name);
  SERIAL_ECHOPGM(" axis driver current: ");
  SERIAL_ECHOLN(st.getCurrent());
}
template<typename TMC>
void tmc_set_current(TMC &st, const char name[], const int mA) {
  st.setCurrent(mA, R_SENSE, HOLD_MULTIPLIER);
  tmc_get_current(st, name);
}

template<typename TMC>
void tmc_report_otpw(TMC &st, const char name[]) {
  SERIAL_ECHO(name);
  SERIAL_ECHOPGM(" axis temperature prewarn triggered: ");
  serialprintPGM(st.getOTPW() ? PSTR("true") : PSTR("false"));
  SERIAL_EOL();
}
template<typename TMC>
void tmc_clear_otpw(TMC &st, const char name[]) {
  st.clear_otpw();
  SERIAL_ECHO(name);
  SERIAL_ECHOLNPGM(" prewarn flag cleared");
}

template<typename TMC>
void tmc_get_pwmthrs(TMC &st, const char name[], const uint16_t spmm) {
  SERIAL_ECHO(name);
  SERIAL_ECHOPGM(" stealthChop max speed set to ");
  SERIAL_ECHOLN(12650000UL * st.microsteps() / (256 * st.TPWMTHRS() * spmm));
}
template<typename TMC>
void tmc_set_pwmthrs(TMC &st, const char name[], const int32_t thrs, const uint32_t spmm) {
  st.TPWMTHRS(12650000UL * st.microsteps() / (256 * thrs * spmm));
  tmc_get_pwmthrs(st, name, spmm);
}

template<typename TMC>
void tmc_get_sgt(TMC &st, const char name[]) {
  SERIAL_ECHO(name);
  SERIAL_ECHOPGM(" driver homing sensitivity set to ");
  MYSERIAL.println(st.sgt(), DEC);
}
template<typename TMC>
void tmc_set_sgt(TMC &st, const char name[], const int8_t sgt_val) {
  st.sgt(sgt_val);
  tmc_get_sgt(st, name);
}

/*
 * Check for over temperature or short to ground error flags.
 * Report and log warning of overtemperature condition.
 * Reduce driver current in a persistent otpw condition.
 * Keep track of otpw counter so we don't reduce current on a single instance,
 * and so we don't repeatedly report warning before the condition is cleared.
 */
#if ENABLED(MONITOR_DRIVER_STATUS)
  struct TMC_driver_data {
    uint32_t drv_status;
    bool is_otpw;
    bool is_ot;
    bool is_error;
  };
  #if ENABLED(HAVE_TMC2130)
    static uint32_t get_pwm_scale(TMC2130Stepper &st) { return st.PWM_SCALE(); }
    static uint8_t get_status_response(TMC2130Stepper &st) { return st.status_response&0xF; }
    static TMC_driver_data get_driver_data(TMC2130Stepper &st) {
      constexpr uint32_t OTPW_bm = 0x4000000UL;
      constexpr uint8_t OTPW_bp = 26;
      constexpr uint32_t OT_bm = 0x2000000UL;
      constexpr uint8_t OT_bp = 25;
      constexpr uint8_t DRIVER_ERROR_bm = 0x2UL;
      constexpr uint8_t DRIVER_ERROR_bp = 1;
      TMC_driver_data data;
      data.drv_status = st.DRV_STATUS();
      data.is_otpw = (data.drv_status & OTPW_bm)>>OTPW_bp;
      data.is_ot = (data.drv_status & OT_bm)>>OT_bp;
      data.is_error = (st.status_response & DRIVER_ERROR_bm)>>DRIVER_ERROR_bp;
      return data;
    }
  #endif
  #if ENABLED(HAVE_TMC2208)
    static uint32_t get_pwm_scale(TMC2208Stepper &st) { return st.pwm_scale_sum(); }
    static uint8_t get_status_response(TMC2208Stepper &st) {
      uint32_t drv_status = st.DRV_STATUS();
      uint8_t gstat = st.GSTAT();
      uint8_t response = 0;
      response |= (drv_status >> (31-3)) & 0b1000;
      response |= gstat & 0b11;
      return response;
    }
    static TMC_driver_data get_driver_data(TMC2208Stepper &st) {
      constexpr uint32_t OTPW_bm = 0b1ul;
      constexpr uint8_t OTPW_bp = 0;
      constexpr uint32_t OT_bm = 0b10ul;
      constexpr uint8_t OT_bp = 1;
      TMC_driver_data data;
      data.drv_status = st.DRV_STATUS();
      data.is_otpw = (data.drv_status & OTPW_bm)>>OTPW_bp;
      data.is_ot = (data.drv_status & OT_bm)>>OT_bp;
      data.is_error = st.drv_err();
      return data;
    }
  #endif

  template<typename TMC>
  uint8_t monitor_tmc_driver(TMC &st, const char axisID, uint8_t otpw_cnt) {
    TMC_driver_data data = get_driver_data(st);

    #if ENABLED(STOP_ON_ERROR)
      if (data.is_error) {
        SERIAL_EOL();
        SERIAL_ECHO(axisID);
        SERIAL_ECHO(" driver error detected:");
        if (data.is_ot) SERIAL_ECHO("\novertemperature");
        if (st.s2ga()) SERIAL_ECHO("\nshort to ground (coil A)");
        if (st.s2gb()) SERIAL_ECHO("\nshort to ground (coil B)");
        SERIAL_EOL();
        #if ENABLED(TMC_DEBUG)
          _M122();
        #endif
        kill(PSTR("Driver error"));
      }
    #endif

    // Report if a warning was triggered
    if (data.is_otpw && otpw_cnt==0) {
      char timestamp[10];
      duration_t elapsed = print_job_timer.duration();
      const bool has_days = (elapsed.value > 60*60*24L);
      (void)elapsed.toDigital(timestamp, has_days);
      SERIAL_EOL();
      SERIAL_ECHO(timestamp);
      SERIAL_ECHOPGM(": ");
      SERIAL_ECHO(axisID);
      SERIAL_ECHOPGM(" driver overtemperature warning! (");
      SERIAL_ECHO(st.getCurrent());
      SERIAL_ECHOLN("mA)");
    }
    #if CURRENT_STEP_DOWN > 0
      // Decrease current if is_otpw is true and driver is enabled and there's been more then 4 warnings
      if (data.is_otpw && !st.isEnabled() && otpw_cnt > 4) {
        st.setCurrent(st.getCurrent() - CURRENT_STEP_DOWN, R_SENSE, HOLD_MULTIPLIER);
        #if ENABLED(REPORT_CURRENT_CHANGE)
          SERIAL_ECHO(axisID);
          SERIAL_ECHOLNPAIR(" current decreased to ", st.getCurrent());
        #endif
      }
    #endif

    if (data.is_otpw) {
      otpw_cnt++;
      st.flag_otpw = true;
    }
    else if (otpw_cnt>0) otpw_cnt--;

    if (report_tmc_status) {
      const uint32_t pwm_scale = get_pwm_scale(st);
      SERIAL_ECHO(axisID);
      SERIAL_ECHOPAIR(":", pwm_scale);
      SERIAL_ECHO(" |0b"); MYSERIAL.print(get_status_response(st), BIN);
      SERIAL_ECHO("| ");
      if (data.is_error) SERIAL_ECHO('E');
      else if (data.is_ot) SERIAL_ECHO('O');
      else if (data.is_otpw) SERIAL_ECHO('W');
      else if (otpw_cnt>0) MYSERIAL.print(otpw_cnt, DEC);
      else if (st.flag_otpw) SERIAL_ECHO('F');
      SERIAL_ECHO("\t");
    }

    return otpw_cnt;
  }

  #define HAS_HW_COMMS(ST) ENABLED(ST##_IS_TMC2130)|| (ENABLED(ST##_IS_TMC2208) && defined(ST##_HARDWARE_SERIAL))

  void monitor_tmc_driver() {
    static millis_t next_cOT = 0;
    if (ELAPSED(millis(), next_cOT)) {
      next_cOT = millis() + 500;
      #if HAS_HW_COMMS(X) || ENABLED(IS_TRAMS)
        static uint8_t x_otpw_cnt = 0;
        x_otpw_cnt = monitor_tmc_driver(stepperX, axis_codes[X_AXIS], x_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(Y) || ENABLED(IS_TRAMS)
        static uint8_t y_otpw_cnt = 0;
        y_otpw_cnt = monitor_tmc_driver(stepperY, axis_codes[Y_AXIS], y_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(Z) || ENABLED(IS_TRAMS)
        static uint8_t z_otpw_cnt = 0;
        z_otpw_cnt = monitor_tmc_driver(stepperZ, axis_codes[Z_AXIS], z_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(X2)
        static uint8_t x2_otpw_cnt = 0;
        x2_otpw_cnt = monitor_tmc_driver(stepperX2, axis_codes[X_AXIS], x2_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(Y2)
        static uint8_t y2_otpw_cnt = 0;
        y2_otpw_cnt = monitor_tmc_driver(stepperY2, axis_codes[Y_AXIS], y2_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(Z2)
        static uint8_t z2_otpw_cnt = 0;
        z2_otpw_cnt = monitor_tmc_driver(stepperZ2, axis_codes[Z_AXIS], z2_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(E0) || ENABLED(IS_TRAMS)
        static uint8_t e0_otpw_cnt = 0;
        e0_otpw_cnt = monitor_tmc_driver(stepperE0, axis_codes[E_AXIS], e0_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(E1)
        static uint8_t e1_otpw_cnt = 0;
        e1_otpw_cnt = monitor_tmc_driver(stepperE1, axis_codes[E_AXIS], e1_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(E2)
        static uint8_t e2_otpw_cnt = 0;
        e2_otpw_cnt = monitor_tmc_driver(stepperE2, axis_codes[E_AXIS], e2_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(E3)
        static uint8_t e3_otpw_cnt = 0;
        e3_otpw_cnt = monitor_tmc_driver(stepperE3, axis_codes[E_AXIS], e3_otpw_cnt);
      #endif
      #if HAS_HW_COMMS(E4)
        static uint8_t e4_otpw_cnt = 0;
        e4_otpw_cnt = monitor_tmc_driver(stepperE4, axis_codes[E_AXIS], e4_otpw_cnt);
      #endif

      if (report_tmc_status) SERIAL_EOL();
    }
  }

#endif // MONITOR_DRIVER_STATUS

#endif // HAS_TRINAMIC
