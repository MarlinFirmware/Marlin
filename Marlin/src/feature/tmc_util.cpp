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

#include "../inc/MarlinConfig.h"

#if HAS_TRINAMIC_CONFIG

#include "tmc_util.h"
#include "../MarlinCore.h"

#include "../module/stepper/indirection.h"
#include "../module/printcounter.h"
#include "../libs/duration_t.h"
#include "../gcode/gcode.h"

#if ENABLED(TMC_DEBUG)
  #include "../module/planner.h"
  #include "../libs/hex_print.h"
  #if ENABLED(MONITOR_DRIVER_STATUS)
    static uint16_t report_tmc_status_interval; // = 0
  #endif
#endif

#if HAS_LCD_MENU
  #include "../module/stepper.h"
#endif

/**
 * Check for over temperature or short to ground error flags.
 * Report and log warning of overtemperature condition.
 * Reduce driver current in a persistent otpw condition.
 * Keep track of otpw counter so we don't reduce current on a single instance,
 * and so we don't repeatedly report warning before the condition is cleared.
 */
#if ENABLED(MONITOR_DRIVER_STATUS)

  struct TMC_driver_data {
    uint32_t drv_status;
    bool is_otpw:1,
         is_ot:1,
         is_s2g:1,
         is_error:1
         #if ENABLED(TMC_DEBUG)
           , is_stall:1
           , is_stealth:1
           , is_standstill:1
           #if HAS_STALLGUARD
             , sg_result_reasonable:1
           #endif
         #endif
      ;
    #if ENABLED(TMC_DEBUG)
      #if HAS_TMCX1X0 || HAS_TMC220x
        uint8_t cs_actual;
      #endif
      #if HAS_STALLGUARD
        uint16_t sg_result;
      #endif
    #endif
  };

  #if HAS_TMCX1X0

    #if ENABLED(TMC_DEBUG)
      static uint32_t get_pwm_scale(TMC2130Stepper &st) { return st.PWM_SCALE(); }
    #endif

    static TMC_driver_data get_driver_data(TMC2130Stepper &st) {
      constexpr uint8_t OT_bp = 25, OTPW_bp = 26;
      constexpr uint32_t S2G_bm = 0x18000000;
      #if ENABLED(TMC_DEBUG)
        constexpr uint16_t SG_RESULT_bm = 0x3FF; // 0:9
        constexpr uint8_t STEALTH_bp = 14;
        constexpr uint32_t CS_ACTUAL_bm = 0x1F0000; // 16:20
        constexpr uint8_t STALL_GUARD_bp = 24;
        constexpr uint8_t STST_bp = 31;
      #endif
      TMC_driver_data data;
      const auto ds = data.drv_status = st.DRV_STATUS();
      #ifdef __AVR__

        // 8-bit optimization saves up to 70 bytes of PROGMEM per axis
        uint8_t spart;
        #if ENABLED(TMC_DEBUG)
          data.sg_result = ds & SG_RESULT_bm;
          spart = ds >> 8;
          data.is_stealth = TEST(spart, STEALTH_bp - 8);
          spart = ds >> 16;
          data.cs_actual = spart & (CS_ACTUAL_bm >> 16);
        #endif
        spart = ds >> 24;
        data.is_ot = TEST(spart, OT_bp - 24);
        data.is_otpw = TEST(spart, OTPW_bp - 24);
        data.is_s2g = !!(spart & (S2G_bm >> 24));
        #if ENABLED(TMC_DEBUG)
          data.is_stall = TEST(spart, STALL_GUARD_bp - 24);
          data.is_standstill = TEST(spart, STST_bp - 24);
          data.sg_result_reasonable = !data.is_standstill; // sg_result has no reasonable meaning while standstill
        #endif

      #else // !__AVR__

        data.is_ot = TEST(ds, OT_bp);
        data.is_otpw = TEST(ds, OTPW_bp);
        data.is_s2g = !!(ds & S2G_bm);
        #if ENABLED(TMC_DEBUG)
          constexpr uint8_t CS_ACTUAL_sb = 16;
          data.sg_result = ds & SG_RESULT_bm;
          data.is_stealth = TEST(ds, STEALTH_bp);
          data.cs_actual = (ds & CS_ACTUAL_bm) >> CS_ACTUAL_sb;
          data.is_stall = TEST(ds, STALL_GUARD_bp);
          data.is_standstill = TEST(ds, STST_bp);
          data.sg_result_reasonable = !data.is_standstill; // sg_result has no reasonable meaning while standstill
        #endif

      #endif // !__AVR__

      return data;
    }

  #endif // HAS_TMCX1X0

  #if HAS_TMC220x

    #if ENABLED(TMC_DEBUG)
      static uint32_t get_pwm_scale(TMC2208Stepper &st) { return st.pwm_scale_sum(); }
    #endif

    static TMC_driver_data get_driver_data(TMC2208Stepper &st) {
      constexpr uint8_t OTPW_bp = 0, OT_bp = 1;
      constexpr uint8_t S2G_bm = 0b11110; // 2..5
      TMC_driver_data data;
      const auto ds = data.drv_status = st.DRV_STATUS();
      data.is_otpw = TEST(ds, OTPW_bp);
      data.is_ot = TEST(ds, OT_bp);
      data.is_s2g = !!(ds & S2G_bm);
      #if ENABLED(TMC_DEBUG)
        constexpr uint32_t CS_ACTUAL_bm = 0x1F0000; // 16:20
        constexpr uint8_t STEALTH_bp = 30, STST_bp = 31;
        #ifdef __AVR__
          // 8-bit optimization saves up to 12 bytes of PROGMEM per axis
          uint8_t spart = ds >> 16;
          data.cs_actual = spart & (CS_ACTUAL_bm >> 16);
          spart = ds >> 24;
          data.is_stealth = TEST(spart, STEALTH_bp - 24);
          data.is_standstill = TEST(spart, STST_bp - 24);
        #else
          constexpr uint8_t CS_ACTUAL_sb = 16;
          data.cs_actual = (ds & CS_ACTUAL_bm) >> CS_ACTUAL_sb;
          data.is_stealth = TEST(ds, STEALTH_bp);
          data.is_standstill = TEST(ds, STST_bp);
        #endif
        TERN_(HAS_STALLGUARD, data.sg_result_reasonable = false);
      #endif
      return data;
    }

  #endif // TMC2208 || TMC2209

  #if HAS_DRIVER(TMC2660)

    #if ENABLED(TMC_DEBUG)
      static uint32_t get_pwm_scale(TMC2660Stepper) { return 0; }
    #endif

    static TMC_driver_data get_driver_data(TMC2660Stepper &st) {
      constexpr uint8_t OT_bp = 1, OTPW_bp = 2;
      constexpr uint8_t S2G_bm = 0b11000;
      TMC_driver_data data;
      const auto ds = data.drv_status = st.DRVSTATUS();
      uint8_t spart = ds & 0xFF;
      data.is_otpw = TEST(spart, OTPW_bp);
      data.is_ot = TEST(spart, OT_bp);
      data.is_s2g = !!(ds & S2G_bm);
      #if ENABLED(TMC_DEBUG)
        constexpr uint8_t STALL_GUARD_bp = 0;
        constexpr uint8_t STST_bp = 7, SG_RESULT_sp = 10;
        constexpr uint32_t SG_RESULT_bm = 0xFFC00; // 10:19
        data.is_stall = TEST(spart, STALL_GUARD_bp);
        data.is_standstill = TEST(spart, STST_bp);
        data.sg_result = (ds & SG_RESULT_bm) >> SG_RESULT_sp;
        data.sg_result_reasonable = true;
      #endif
      return data;
    }

  #endif // TMC2660

  #if ENABLED(STOP_ON_ERROR)
    void report_driver_error(const TMC_driver_data &data) {
      SERIAL_ECHOPGM(" driver error detected: 0x");
      SERIAL_PRINTLN(data.drv_status, HEX);
      if (data.is_ot) SERIAL_ECHOLNPGM("overtemperature");
      if (data.is_s2g) SERIAL_ECHOLNPGM("coil short circuit");
      TERN_(TMC_DEBUG, tmc_report_all(true, true, true, true));
      kill(PSTR("Driver error"));
    }
  #endif

  template<typename TMC>
  void report_driver_otpw(TMC &st) {
    char timestamp[14];
    duration_t elapsed = print_job_timer.duration();
    const bool has_days = (elapsed.value > 60*60*24L);
    (void)elapsed.toDigital(timestamp, has_days);
    SERIAL_EOL();
    SERIAL_ECHO(timestamp);
    SERIAL_ECHOPGM(": ");
    st.printLabel();
    SERIAL_ECHOLNPAIR(" driver overtemperature warning! (", st.getMilliamps(), "mA)");
  }

  template<typename TMC>
  void report_polled_driver_data(TMC &st, const TMC_driver_data &data) {
    const uint32_t pwm_scale = get_pwm_scale(st);
    st.printLabel();
    SERIAL_CHAR(':'); SERIAL_PRINT(pwm_scale, DEC);
    #if ENABLED(TMC_DEBUG)
      #if HAS_TMCX1X0 || HAS_TMC220x
        SERIAL_CHAR('/'); SERIAL_PRINT(data.cs_actual, DEC);
      #endif
      #if HAS_STALLGUARD
        SERIAL_CHAR('/');
        if (data.sg_result_reasonable)
          SERIAL_ECHO(data.sg_result);
        else
          SERIAL_CHAR('-');
      #endif
    #endif
    SERIAL_CHAR('|');
    if (st.error_count)       SERIAL_CHAR('E'); // Error
    if (data.is_ot)           SERIAL_CHAR('O'); // Over-temperature
    if (data.is_otpw)         SERIAL_CHAR('W'); // over-temperature pre-Warning
    #if ENABLED(TMC_DEBUG)
      if (data.is_stall)      SERIAL_CHAR('G'); // stallGuard
      if (data.is_stealth)    SERIAL_CHAR('T'); // stealthChop
      if (data.is_standstill) SERIAL_CHAR('I'); // standstIll
    #endif
    if (st.flag_otpw)         SERIAL_CHAR('F'); // otpw Flag
    SERIAL_CHAR('|');
    if (st.otpw_count > 0) SERIAL_PRINT(st.otpw_count, DEC);
    SERIAL_CHAR('\t');
  }

  #if CURRENT_STEP_DOWN > 0

    template<typename TMC>
    void step_current_down(TMC &st) {
      if (st.isEnabled()) {
        const uint16_t I_rms = st.getMilliamps() - (CURRENT_STEP_DOWN);
        if (I_rms > 50) {
          st.rms_current(I_rms);
          #if ENABLED(REPORT_CURRENT_CHANGE)
            st.printLabel();
            SERIAL_ECHOLNPAIR(" current decreased to ", I_rms);
          #endif
        }
      }
    }

  #else

    #define step_current_down(...)

  #endif

  template<typename TMC>
  bool monitor_tmc_driver(TMC &st, const bool need_update_error_counters, const bool need_debug_reporting) {
    TMC_driver_data data = get_driver_data(st);
    if (data.drv_status == 0xFFFFFFFF || data.drv_status == 0x0) return false;

    bool should_step_down = false;

    if (need_update_error_counters) {
      if (data.is_ot /* | data.s2ga | data.s2gb*/) st.error_count++;
      else if (st.error_count > 0) st.error_count--;

      #if ENABLED(STOP_ON_ERROR)
        if (st.error_count >= 10) {
          SERIAL_EOL();
          st.printLabel();
          report_driver_error(data);
        }
      #endif

      // Report if a warning was triggered
      if (data.is_otpw && st.otpw_count == 0)
        report_driver_otpw(st);

      #if CURRENT_STEP_DOWN > 0
        // Decrease current if is_otpw is true and driver is enabled and there's been more than 4 warnings
        if (data.is_otpw && st.otpw_count > 4 && st.isEnabled())
          should_step_down = true;
      #endif

      if (data.is_otpw) {
        st.otpw_count++;
        st.flag_otpw = true;
      }
      else if (st.otpw_count > 0) st.otpw_count = 0;
    }

    #if ENABLED(TMC_DEBUG)
      if (need_debug_reporting) report_polled_driver_data(st, data);
    #endif

    return should_step_down;
  }

  void monitor_tmc_drivers() {
    const millis_t ms = millis();

    // Poll TMC drivers at the configured interval
    static millis_t next_poll = 0;
    const bool need_update_error_counters = ELAPSED(ms, next_poll);
    if (need_update_error_counters) next_poll = ms + MONITOR_DRIVER_STATUS_INTERVAL_MS;

    // Also poll at intervals for debugging
    #if ENABLED(TMC_DEBUG)
      static millis_t next_debug_reporting = 0;
      const bool need_debug_reporting = report_tmc_status_interval && ELAPSED(ms, next_debug_reporting);
      if (need_debug_reporting) next_debug_reporting = ms + report_tmc_status_interval;
    #else
      constexpr bool need_debug_reporting = false;
    #endif

    if (need_update_error_counters || need_debug_reporting) {

      #if AXIS_IS_TMC(X) || AXIS_IS_TMC(X2)
      {
        bool result = false;
        #if AXIS_IS_TMC(X)
          if (monitor_tmc_driver(stepperX, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        #if AXIS_IS_TMC(X2)
          if (monitor_tmc_driver(stepperX2, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        if (result) {
          #if AXIS_IS_TMC(X)
            step_current_down(stepperX);
          #endif
          #if AXIS_IS_TMC(X2)
            step_current_down(stepperX2);
          #endif
        }
      }
      #endif

      #if AXIS_IS_TMC(Y) || AXIS_IS_TMC(Y2)
      {
        bool result = false;
        #if AXIS_IS_TMC(Y)
          if (monitor_tmc_driver(stepperY, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        #if AXIS_IS_TMC(Y2)
          if (monitor_tmc_driver(stepperY2, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        if (result) {
          #if AXIS_IS_TMC(Y)
            step_current_down(stepperY);
          #endif
          #if AXIS_IS_TMC(Y2)
            step_current_down(stepperY2);
          #endif
        }
      }
      #endif

      #if AXIS_IS_TMC(Z) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4)
      {
        bool result = false;
        #if AXIS_IS_TMC(Z)
          if (monitor_tmc_driver(stepperZ, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        #if AXIS_IS_TMC(Z2)
          if (monitor_tmc_driver(stepperZ2, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        #if AXIS_IS_TMC(Z3)
          if (monitor_tmc_driver(stepperZ3, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        #if AXIS_IS_TMC(Z4)
          if (monitor_tmc_driver(stepperZ4, need_update_error_counters, need_debug_reporting)) result = true;
        #endif
        if (result) {
          #if AXIS_IS_TMC(Z)
            step_current_down(stepperZ);
          #endif
          #if AXIS_IS_TMC(Z2)
            step_current_down(stepperZ2);
          #endif
          #if AXIS_IS_TMC(Z3)
            step_current_down(stepperZ3);
          #endif
          #if AXIS_IS_TMC(Z4)
            step_current_down(stepperZ4);
          #endif
        }
      }
      #endif

      #if AXIS_IS_TMC(E0)
        (void)monitor_tmc_driver(stepperE0, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E1)
        (void)monitor_tmc_driver(stepperE1, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E2)
        (void)monitor_tmc_driver(stepperE2, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E3)
        (void)monitor_tmc_driver(stepperE3, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E4)
        (void)monitor_tmc_driver(stepperE4, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E5)
        (void)monitor_tmc_driver(stepperE5, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E6)
        (void)monitor_tmc_driver(stepperE6, need_update_error_counters, need_debug_reporting);
      #endif
      #if AXIS_IS_TMC(E7)
        (void)monitor_tmc_driver(stepperE7, need_update_error_counters, need_debug_reporting);
      #endif

      if (TERN0(TMC_DEBUG, need_debug_reporting)) SERIAL_EOL();
    }
  }

#endif // MONITOR_DRIVER_STATUS

#if ENABLED(TMC_DEBUG)

  /**
   * M122 [S<0|1>] [Pnnn] Enable periodic status reports
   */
  #if ENABLED(MONITOR_DRIVER_STATUS)
    void tmc_set_report_interval(const uint16_t update_interval) {
      if ((report_tmc_status_interval = update_interval))
        SERIAL_ECHOLNPGM("axis:pwm_scale"
          #if HAS_STEALTHCHOP
            "/curr_scale"
          #endif
          #if HAS_STALLGUARD
            "/mech_load"
          #endif
          "|flags|warncount"
        );
    }
  #endif

  enum TMC_debug_enum : char {
    TMC_CODES,
    TMC_UART_ADDR,
    TMC_ENABLED,
    TMC_CURRENT,
    TMC_RMS_CURRENT,
    TMC_MAX_CURRENT,
    TMC_IRUN,
    TMC_IHOLD,
    TMC_GLOBAL_SCALER,
    TMC_CS_ACTUAL,
    TMC_PWM_SCALE,
    TMC_PWM_SCALE_SUM,
    TMC_PWM_SCALE_AUTO,
    TMC_PWM_OFS_AUTO,
    TMC_PWM_GRAD_AUTO,
    TMC_VSENSE,
    TMC_STEALTHCHOP,
    TMC_MICROSTEPS,
    TMC_TSTEP,
    TMC_TPWMTHRS,
    TMC_TPWMTHRS_MMS,
    TMC_OTPW,
    TMC_OTPW_TRIGGERED,
    TMC_TOFF,
    TMC_TBL,
    TMC_HEND,
    TMC_HSTRT,
    TMC_SGT,
    TMC_MSCNT
  };
  enum TMC_drv_status_enum : char {
    TMC_DRV_CODES,
    TMC_STST,
    TMC_OLB,
    TMC_OLA,
    TMC_S2GB,
    TMC_S2GA,
    TMC_DRV_OTPW,
    TMC_OT,
    TMC_STALLGUARD,
    TMC_DRV_CS_ACTUAL,
    TMC_FSACTIVE,
    TMC_SG_RESULT,
    TMC_DRV_STATUS_HEX,
    TMC_T157,
    TMC_T150,
    TMC_T143,
    TMC_T120,
    TMC_STEALTH,
    TMC_S2VSB,
    TMC_S2VSA
  };
  enum TMC_get_registers_enum : char {
    TMC_AXIS_CODES,
    TMC_GET_GCONF,
    TMC_GET_IHOLD_IRUN,
    TMC_GET_GSTAT,
    TMC_GET_IOIN,
    TMC_GET_TPOWERDOWN,
    TMC_GET_TSTEP,
    TMC_GET_TPWMTHRS,
    TMC_GET_TCOOLTHRS,
    TMC_GET_THIGH,
    TMC_GET_CHOPCONF,
    TMC_GET_COOLCONF,
    TMC_GET_PWMCONF,
    TMC_GET_PWM_SCALE,
    TMC_GET_DRV_STATUS,
    TMC_GET_DRVCONF,
    TMC_GET_DRVCTRL,
    TMC_GET_DRVSTATUS,
    TMC_GET_SGCSCONF,
    TMC_GET_SMARTEN
  };

  template<class TMC>
  static void print_vsense(TMC &st) { serialprintPGM(st.vsense() ? PSTR("1=.18") : PSTR("0=.325")); }

  #if HAS_DRIVER(TMC2130) || HAS_DRIVER(TMC5130)
    static void _tmc_status(TMC2130Stepper &st, const TMC_debug_enum i) {
      switch (i) {
        case TMC_PWM_SCALE: SERIAL_PRINT(st.PWM_SCALE(), DEC); break;
        case TMC_SGT: SERIAL_PRINT(st.sgt(), DEC); break;
        case TMC_STEALTHCHOP: serialprint_truefalse(st.en_pwm_mode()); break;
        default: break;
      }
    }
  #endif
  #if HAS_TMCX1X0
    static void _tmc_parse_drv_status(TMC2130Stepper &st, const TMC_drv_status_enum i) {
      switch (i) {
        case TMC_STALLGUARD: if (st.stallguard()) SERIAL_CHAR('*'); break;
        case TMC_SG_RESULT:  SERIAL_PRINT(st.sg_result(), DEC); break;
        case TMC_FSACTIVE:   if (st.fsactive())   SERIAL_CHAR('*'); break;
        case TMC_DRV_CS_ACTUAL: SERIAL_PRINT(st.cs_actual(), DEC); break;
        default: break;
      }
    }
  #endif

  #if HAS_DRIVER(TMC2160) || HAS_DRIVER(TMC5160)
    template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
    void print_vsense(TMCMarlin<TMC2160Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &) { }

    template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
    void print_vsense(TMCMarlin<TMC5160Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &) { }

    static void _tmc_status(TMC2160Stepper &st, const TMC_debug_enum i) {
      switch (i) {
        case TMC_PWM_SCALE: SERIAL_PRINT(st.PWM_SCALE(), DEC); break;
        case TMC_SGT: SERIAL_PRINT(st.sgt(), DEC); break;
        case TMC_STEALTHCHOP: serialprint_truefalse(st.en_pwm_mode()); break;
        case TMC_GLOBAL_SCALER:
          {
            uint16_t value = st.GLOBAL_SCALER();
            SERIAL_PRINT(value ?: 256, DEC);
            SERIAL_ECHOPGM("/256");
          }
          break;
        default: break;
      }
    }
  #endif

  #if HAS_TMC220x
    static void _tmc_status(TMC2208Stepper &st, const TMC_debug_enum i) {
      switch (i) {
        case TMC_PWM_SCALE_SUM: SERIAL_PRINT(st.pwm_scale_sum(), DEC); break;
        case TMC_PWM_SCALE_AUTO: SERIAL_PRINT(st.pwm_scale_auto(), DEC); break;
        case TMC_PWM_OFS_AUTO: SERIAL_PRINT(st.pwm_ofs_auto(), DEC); break;
        case TMC_PWM_GRAD_AUTO: SERIAL_PRINT(st.pwm_grad_auto(), DEC); break;
        case TMC_STEALTHCHOP: serialprint_truefalse(st.stealth()); break;
        case TMC_S2VSA: if (st.s2vsa()) SERIAL_CHAR('*'); break;
        case TMC_S2VSB: if (st.s2vsb()) SERIAL_CHAR('*'); break;
        default: break;
      }
    }

    #if HAS_DRIVER(TMC2209)
      template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
      static void _tmc_status(TMCMarlin<TMC2209Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const TMC_debug_enum i) {
        switch (i) {
          case TMC_SGT:       SERIAL_PRINT(st.SGTHRS(), DEC); break;
          case TMC_UART_ADDR: SERIAL_PRINT(st.get_address(), DEC); break;
          default:
            TMC2208Stepper *parent = &st;
            _tmc_status(*parent, i);
            break;
        }
      }
    #endif

    static void _tmc_parse_drv_status(TMC2208Stepper &st, const TMC_drv_status_enum i) {
      switch (i) {
        case TMC_T157: if (st.t157()) SERIAL_CHAR('*'); break;
        case TMC_T150: if (st.t150()) SERIAL_CHAR('*'); break;
        case TMC_T143: if (st.t143()) SERIAL_CHAR('*'); break;
        case TMC_T120: if (st.t120()) SERIAL_CHAR('*'); break;
        case TMC_DRV_CS_ACTUAL: SERIAL_PRINT(st.cs_actual(), DEC); break;
        default: break;
      }
    }

    #if HAS_DRIVER(TMC2209)
      static void _tmc_parse_drv_status(TMC2209Stepper &st, const TMC_drv_status_enum i) {
        switch (i) {
          case TMC_SG_RESULT: SERIAL_PRINT(st.SG_RESULT(), DEC); break;
          default:            _tmc_parse_drv_status(static_cast<TMC2208Stepper &>(st), i); break;
        }
      }
    #endif
  #endif

  #if HAS_DRIVER(TMC2660)
    static void _tmc_parse_drv_status(TMC2660Stepper, const TMC_drv_status_enum) { }
  #endif

  template <typename TMC>
  static void tmc_status(TMC &st, const TMC_debug_enum i) {
    SERIAL_CHAR('\t');
    switch (i) {
      case TMC_CODES: st.printLabel(); break;
      case TMC_ENABLED: serialprint_truefalse(st.isEnabled()); break;
      case TMC_CURRENT: SERIAL_ECHO(st.getMilliamps()); break;
      case TMC_RMS_CURRENT: SERIAL_ECHO(st.rms_current()); break;
      case TMC_MAX_CURRENT: SERIAL_PRINT((float)st.rms_current() * 1.41, 0); break;
      case TMC_IRUN:
        SERIAL_PRINT(st.irun(), DEC);
        SERIAL_ECHOPGM("/31");
        break;
      case TMC_IHOLD:
        SERIAL_PRINT(st.ihold(), DEC);
        SERIAL_ECHOPGM("/31");
        break;
      case TMC_CS_ACTUAL:
        SERIAL_PRINT(st.cs_actual(), DEC);
        SERIAL_ECHOPGM("/31");
        break;
      case TMC_VSENSE: print_vsense(st); break;
      case TMC_MICROSTEPS: SERIAL_ECHO(st.microsteps()); break;
      case TMC_TSTEP: {
        const uint32_t tstep_value = st.TSTEP();
        if (tstep_value != 0xFFFFF) SERIAL_ECHO(tstep_value); else SERIAL_ECHOPGM("max");
      } break;
      #if ENABLED(HYBRID_THRESHOLD)
        case TMC_TPWMTHRS: SERIAL_ECHO(uint32_t(st.TPWMTHRS())); break;
        case TMC_TPWMTHRS_MMS: {
          const uint32_t tpwmthrs_val = st.get_pwm_thrs();
          if (tpwmthrs_val) SERIAL_ECHO(tpwmthrs_val); else SERIAL_CHAR('-');
        } break;
      #endif
      case TMC_OTPW: serialprint_truefalse(st.otpw()); break;
      #if ENABLED(MONITOR_DRIVER_STATUS)
        case TMC_OTPW_TRIGGERED: serialprint_truefalse(st.getOTPW()); break;
      #endif
      case TMC_TOFF: SERIAL_PRINT(st.toff(), DEC); break;
      case TMC_TBL: SERIAL_PRINT(st.blank_time(), DEC); break;
      case TMC_HEND: SERIAL_PRINT(st.hysteresis_end(), DEC); break;
      case TMC_HSTRT: SERIAL_PRINT(st.hysteresis_start(), DEC); break;
      case TMC_MSCNT: SERIAL_PRINT(st.get_microstep_counter(), DEC); break;
      default: _tmc_status(st, i); break;
    }
  }

  #if HAS_DRIVER(TMC2660)
    template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
    void tmc_status(TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const TMC_debug_enum i) {
      SERIAL_CHAR('\t');
      switch (i) {
        case TMC_CODES: st.printLabel(); break;
        case TMC_ENABLED: serialprint_truefalse(st.isEnabled()); break;
        case TMC_CURRENT: SERIAL_ECHO(st.getMilliamps()); break;
        case TMC_RMS_CURRENT: SERIAL_ECHO(st.rms_current()); break;
        case TMC_MAX_CURRENT: SERIAL_PRINT((float)st.rms_current() * 1.41, 0); break;
        case TMC_IRUN:
          SERIAL_PRINT(st.cs(), DEC);
          SERIAL_ECHOPGM("/31");
          break;
        case TMC_VSENSE: serialprintPGM(st.vsense() ? PSTR("1=.165") : PSTR("0=.310")); break;
        case TMC_MICROSTEPS: SERIAL_ECHO(st.microsteps()); break;
        //case TMC_OTPW: serialprint_truefalse(st.otpw()); break;
        //case TMC_OTPW_TRIGGERED: serialprint_truefalse(st.getOTPW()); break;
        case TMC_SGT: SERIAL_PRINT(st.sgt(), DEC); break;
        case TMC_TOFF: SERIAL_PRINT(st.toff(), DEC); break;
        case TMC_TBL: SERIAL_PRINT(st.blank_time(), DEC); break;
        case TMC_HEND: SERIAL_PRINT(st.hysteresis_end(), DEC); break;
        case TMC_HSTRT: SERIAL_PRINT(st.hysteresis_start(), DEC); break;
        default: break;
      }
    }
  #endif

  template <typename TMC>
  static void tmc_parse_drv_status(TMC &st, const TMC_drv_status_enum i) {
    SERIAL_CHAR('\t');
    switch (i) {
      case TMC_DRV_CODES:     st.printLabel();  break;
      case TMC_STST:          if (!st.stst())   SERIAL_CHAR('*'); break;
      case TMC_OLB:           if (st.olb())     SERIAL_CHAR('*'); break;
      case TMC_OLA:           if (st.ola())     SERIAL_CHAR('*'); break;
      case TMC_S2GB:          if (st.s2gb())    SERIAL_CHAR('*'); break;
      case TMC_S2GA:          if (st.s2ga())    SERIAL_CHAR('*'); break;
      case TMC_DRV_OTPW:      if (st.otpw())    SERIAL_CHAR('*'); break;
      case TMC_OT:            if (st.ot())      SERIAL_CHAR('*'); break;
      case TMC_DRV_STATUS_HEX: {
        const uint32_t drv_status = st.DRV_STATUS();
        SERIAL_CHAR('\t');
        st.printLabel();
        SERIAL_CHAR('\t');
        print_hex_long(drv_status, ':');
        if (drv_status == 0xFFFFFFFF || drv_status == 0) SERIAL_ECHOPGM("\t Bad response!");
        SERIAL_EOL();
        break;
      }
      default: _tmc_parse_drv_status(st, i); break;
    }
  }

  static void tmc_debug_loop(const TMC_debug_enum i, const bool print_x, const bool print_y, const bool print_z, const bool print_e) {
    if (print_x) {
      #if AXIS_IS_TMC(X)
        tmc_status(stepperX, i);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_status(stepperX2, i);
      #endif
    }

    if (print_y) {
      #if AXIS_IS_TMC(Y)
        tmc_status(stepperY, i);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_status(stepperY2, i);
      #endif
    }

    if (print_z) {
      #if AXIS_IS_TMC(Z)
        tmc_status(stepperZ, i);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_status(stepperZ2, i);
      #endif
      #if AXIS_IS_TMC(Z3)
        tmc_status(stepperZ3, i);
      #endif
      #if AXIS_IS_TMC(Z4)
        tmc_status(stepperZ4, i);
      #endif
    }

    if (print_e) {
      #if AXIS_IS_TMC(E0)
        tmc_status(stepperE0, i);
      #endif
      #if AXIS_IS_TMC(E1)
        tmc_status(stepperE1, i);
      #endif
      #if AXIS_IS_TMC(E2)
        tmc_status(stepperE2, i);
      #endif
      #if AXIS_IS_TMC(E3)
        tmc_status(stepperE3, i);
      #endif
      #if AXIS_IS_TMC(E4)
        tmc_status(stepperE4, i);
      #endif
      #if AXIS_IS_TMC(E5)
        tmc_status(stepperE5, i);
      #endif
      #if AXIS_IS_TMC(E6)
        tmc_status(stepperE6, i);
      #endif
      #if AXIS_IS_TMC(E7)
        tmc_status(stepperE7, i);
      #endif
    }

    SERIAL_EOL();
  }

  static void drv_status_loop(const TMC_drv_status_enum i, const bool print_x, const bool print_y, const bool print_z, const bool print_e) {
    if (print_x) {
      #if AXIS_IS_TMC(X)
        tmc_parse_drv_status(stepperX, i);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_parse_drv_status(stepperX2, i);
      #endif
    }

    if (print_y) {
      #if AXIS_IS_TMC(Y)
        tmc_parse_drv_status(stepperY, i);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_parse_drv_status(stepperY2, i);
      #endif
    }

    if (print_z) {
      #if AXIS_IS_TMC(Z)
        tmc_parse_drv_status(stepperZ, i);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_parse_drv_status(stepperZ2, i);
      #endif
      #if AXIS_IS_TMC(Z3)
        tmc_parse_drv_status(stepperZ3, i);
      #endif
      #if AXIS_IS_TMC(Z4)
        tmc_parse_drv_status(stepperZ4, i);
      #endif
    }

    if (print_e) {
      #if AXIS_IS_TMC(E0)
        tmc_parse_drv_status(stepperE0, i);
      #endif
      #if AXIS_IS_TMC(E1)
        tmc_parse_drv_status(stepperE1, i);
      #endif
      #if AXIS_IS_TMC(E2)
        tmc_parse_drv_status(stepperE2, i);
      #endif
      #if AXIS_IS_TMC(E3)
        tmc_parse_drv_status(stepperE3, i);
      #endif
      #if AXIS_IS_TMC(E4)
        tmc_parse_drv_status(stepperE4, i);
      #endif
      #if AXIS_IS_TMC(E5)
        tmc_parse_drv_status(stepperE5, i);
      #endif
      #if AXIS_IS_TMC(E6)
        tmc_parse_drv_status(stepperE6, i);
      #endif
      #if AXIS_IS_TMC(E7)
        tmc_parse_drv_status(stepperE7, i);
      #endif
    }

    SERIAL_EOL();
  }

  /**
   * M122 report functions
   */

  void tmc_report_all(bool print_x, const bool print_y, const bool print_z, const bool print_e) {
    #define TMC_REPORT(LABEL, ITEM) do{ SERIAL_ECHOPGM(LABEL);  tmc_debug_loop(ITEM, print_x, print_y, print_z, print_e); }while(0)
    #define DRV_REPORT(LABEL, ITEM) do{ SERIAL_ECHOPGM(LABEL); drv_status_loop(ITEM, print_x, print_y, print_z, print_e); }while(0)
    TMC_REPORT("\t",                 TMC_CODES);
    #if HAS_DRIVER(TMC2209)
      TMC_REPORT("Address\t",        TMC_UART_ADDR);
    #endif
    TMC_REPORT("Enabled\t",          TMC_ENABLED);
    TMC_REPORT("Set current",        TMC_CURRENT);
    TMC_REPORT("RMS current",        TMC_RMS_CURRENT);
    TMC_REPORT("MAX current",        TMC_MAX_CURRENT);
    TMC_REPORT("Run current",        TMC_IRUN);
    TMC_REPORT("Hold current",       TMC_IHOLD);
    #if HAS_DRIVER(TMC2160) || HAS_DRIVER(TMC5160)
      TMC_REPORT("Global scaler",    TMC_GLOBAL_SCALER);
    #endif
    TMC_REPORT("CS actual",          TMC_CS_ACTUAL);
    TMC_REPORT("PWM scale",          TMC_PWM_SCALE);
    #if HAS_DRIVER(TMC2130) || HAS_DRIVER(TMC2224) || HAS_DRIVER(TMC2660) || HAS_TMC220x
      TMC_REPORT("vsense\t",         TMC_VSENSE);
    #endif
    TMC_REPORT("stealthChop",        TMC_STEALTHCHOP);
    TMC_REPORT("msteps\t",           TMC_MICROSTEPS);
    TMC_REPORT("tstep\t",            TMC_TSTEP);
    TMC_REPORT("PWM thresh.",        TMC_TPWMTHRS);
    TMC_REPORT("[mm/s]\t",           TMC_TPWMTHRS_MMS);
    TMC_REPORT("OT prewarn",         TMC_OTPW);
    #if ENABLED(MONITOR_DRIVER_STATUS)
      TMC_REPORT("triggered\n OTP\t", TMC_OTPW_TRIGGERED);
    #endif

    #if HAS_TMC220x
      TMC_REPORT("pwm scale sum",     TMC_PWM_SCALE_SUM);
      TMC_REPORT("pwm scale auto",    TMC_PWM_SCALE_AUTO);
      TMC_REPORT("pwm offset auto",   TMC_PWM_OFS_AUTO);
      TMC_REPORT("pwm grad auto",     TMC_PWM_GRAD_AUTO);
    #endif

    TMC_REPORT("off time",           TMC_TOFF);
    TMC_REPORT("blank time",         TMC_TBL);
    TMC_REPORT("hysteresis\n -end\t", TMC_HEND);
    TMC_REPORT(" -start\t",          TMC_HSTRT);
    TMC_REPORT("Stallguard thrs",    TMC_SGT);
    TMC_REPORT("uStep count",        TMC_MSCNT);
    DRV_REPORT("DRVSTATUS",          TMC_DRV_CODES);
    #if HAS_TMCX1X0 || HAS_TMC220x
      DRV_REPORT("sg_result",        TMC_SG_RESULT);
    #endif
    #if HAS_TMCX1X0
      DRV_REPORT("stallguard",       TMC_STALLGUARD);
      DRV_REPORT("fsactive",         TMC_FSACTIVE);
    #endif
    DRV_REPORT("stst\t",             TMC_STST);
    DRV_REPORT("olb\t",              TMC_OLB);
    DRV_REPORT("ola\t",              TMC_OLA);
    DRV_REPORT("s2gb\t",             TMC_S2GB);
    DRV_REPORT("s2ga\t",             TMC_S2GA);
    DRV_REPORT("otpw\t",             TMC_DRV_OTPW);
    DRV_REPORT("ot\t",               TMC_OT);
    #if HAS_TMC220x
      DRV_REPORT("157C\t",           TMC_T157);
      DRV_REPORT("150C\t",           TMC_T150);
      DRV_REPORT("143C\t",           TMC_T143);
      DRV_REPORT("120C\t",           TMC_T120);
      DRV_REPORT("s2vsa\t",          TMC_S2VSA);
      DRV_REPORT("s2vsb\t",          TMC_S2VSB);
    #endif
    DRV_REPORT("Driver registers:\n",TMC_DRV_STATUS_HEX);
    SERIAL_EOL();
  }

  #define PRINT_TMC_REGISTER(REG_CASE) case TMC_GET_##REG_CASE: print_hex_long(st.REG_CASE(), ':'); break

  #if HAS_TMCX1X0
    static void tmc_get_ic_registers(TMC2130Stepper &st, const TMC_get_registers_enum i) {
      switch (i) {
        PRINT_TMC_REGISTER(TCOOLTHRS);
        PRINT_TMC_REGISTER(THIGH);
        PRINT_TMC_REGISTER(COOLCONF);
        default: SERIAL_CHAR('\t'); break;
      }
    }
  #endif
  #if HAS_TMC220x
    static void tmc_get_ic_registers(TMC2208Stepper, const TMC_get_registers_enum) { SERIAL_CHAR('\t'); }
  #endif

  #if HAS_TRINAMIC_CONFIG
    template<class TMC>
    static void tmc_get_registers(TMC &st, const TMC_get_registers_enum i) {
      switch (i) {
        case TMC_AXIS_CODES: SERIAL_CHAR('\t'); st.printLabel(); break;
        PRINT_TMC_REGISTER(GCONF);
        PRINT_TMC_REGISTER(IHOLD_IRUN);
        PRINT_TMC_REGISTER(GSTAT);
        PRINT_TMC_REGISTER(IOIN);
        PRINT_TMC_REGISTER(TPOWERDOWN);
        PRINT_TMC_REGISTER(TSTEP);
        PRINT_TMC_REGISTER(TPWMTHRS);
        PRINT_TMC_REGISTER(CHOPCONF);
        PRINT_TMC_REGISTER(PWMCONF);
        PRINT_TMC_REGISTER(PWM_SCALE);
        PRINT_TMC_REGISTER(DRV_STATUS);
        default: tmc_get_ic_registers(st, i); break;
      }
      SERIAL_CHAR('\t');
    }
  #endif
  #if HAS_DRIVER(TMC2660)
    template <char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
    static void tmc_get_registers(TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const TMC_get_registers_enum i) {
      switch (i) {
        case TMC_AXIS_CODES: SERIAL_CHAR('\t'); st.printLabel(); break;
        PRINT_TMC_REGISTER(DRVCONF);
        PRINT_TMC_REGISTER(DRVCTRL);
        PRINT_TMC_REGISTER(CHOPCONF);
        PRINT_TMC_REGISTER(DRVSTATUS);
        PRINT_TMC_REGISTER(SGCSCONF);
        PRINT_TMC_REGISTER(SMARTEN);
        default: SERIAL_CHAR('\t'); break;
      }
      SERIAL_CHAR('\t');
    }
  #endif

  static void tmc_get_registers(TMC_get_registers_enum i, const bool print_x, const bool print_y, const bool print_z, const bool print_e) {
    if (print_x) {
      #if AXIS_IS_TMC(X)
        tmc_get_registers(stepperX, i);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_registers(stepperX2, i);
      #endif
    }

    if (print_y) {
      #if AXIS_IS_TMC(Y)
        tmc_get_registers(stepperY, i);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_registers(stepperY2, i);
      #endif
    }

    if (print_z) {
      #if AXIS_IS_TMC(Z)
        tmc_get_registers(stepperZ, i);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_registers(stepperZ2, i);
      #endif
      #if AXIS_IS_TMC(Z3)
        tmc_get_registers(stepperZ3, i);
      #endif
      #if AXIS_IS_TMC(Z4)
        tmc_get_registers(stepperZ4, i);
      #endif
    }

    if (print_e) {
      #if AXIS_IS_TMC(E0)
        tmc_get_registers(stepperE0, i);
      #endif
      #if AXIS_IS_TMC(E1)
        tmc_get_registers(stepperE1, i);
      #endif
      #if AXIS_IS_TMC(E2)
        tmc_get_registers(stepperE2, i);
      #endif
      #if AXIS_IS_TMC(E3)
        tmc_get_registers(stepperE3, i);
      #endif
      #if AXIS_IS_TMC(E4)
        tmc_get_registers(stepperE4, i);
      #endif
      #if AXIS_IS_TMC(E5)
        tmc_get_registers(stepperE5, i);
      #endif
      #if AXIS_IS_TMC(E6)
        tmc_get_registers(stepperE6, i);
      #endif
      #if AXIS_IS_TMC(E7)
        tmc_get_registers(stepperE7, i);
      #endif
    }

    SERIAL_EOL();
  }

  void tmc_get_registers(bool print_x, bool print_y, bool print_z, bool print_e) {
    #define _TMC_GET_REG(LABEL, ITEM) do{ SERIAL_ECHOPGM(LABEL); tmc_get_registers(ITEM, print_x, print_y, print_z, print_e); }while(0)
    #define TMC_GET_REG(NAME, TABS) _TMC_GET_REG(STRINGIFY(NAME) TABS, TMC_GET_##NAME)
    _TMC_GET_REG("\t", TMC_AXIS_CODES);
    TMC_GET_REG(GCONF, "\t\t");
    TMC_GET_REG(IHOLD_IRUN, "\t");
    TMC_GET_REG(GSTAT, "\t\t");
    TMC_GET_REG(IOIN, "\t\t");
    TMC_GET_REG(TPOWERDOWN, "\t");
    TMC_GET_REG(TSTEP, "\t\t");
    TMC_GET_REG(TPWMTHRS, "\t");
    TMC_GET_REG(TCOOLTHRS, "\t");
    TMC_GET_REG(THIGH, "\t\t");
    TMC_GET_REG(CHOPCONF, "\t");
    TMC_GET_REG(COOLCONF, "\t");
    TMC_GET_REG(PWMCONF, "\t");
    TMC_GET_REG(PWM_SCALE, "\t");
    TMC_GET_REG(DRV_STATUS, "\t");
  }

#endif // TMC_DEBUG

#if USE_SENSORLESS

  bool tmc_enable_stallguard(TMC2130Stepper &st) {
    const bool stealthchop_was_enabled = st.en_pwm_mode();

    st.TCOOLTHRS(0xFFFFF);
    st.en_pwm_mode(false);
    st.diag1_stall(true);

    return stealthchop_was_enabled;
  }
  void tmc_disable_stallguard(TMC2130Stepper &st, const bool restore_stealth) {
    st.TCOOLTHRS(0);
    st.en_pwm_mode(restore_stealth);
    st.diag1_stall(false);
  }

  bool tmc_enable_stallguard(TMC2209Stepper &st) {
    const bool stealthchop_was_enabled = !st.en_spreadCycle();

    st.TCOOLTHRS(0xFFFFF);
    st.en_spreadCycle(false);
    return stealthchop_was_enabled;
  }
  void tmc_disable_stallguard(TMC2209Stepper &st, const bool restore_stealth) {
    st.en_spreadCycle(!restore_stealth);
    st.TCOOLTHRS(0);
  }

  bool tmc_enable_stallguard(TMC2660Stepper) {
    // TODO
    return false;
  }
  void tmc_disable_stallguard(TMC2660Stepper, const bool) {};

#endif // USE_SENSORLESS

#if HAS_TMC_SPI
  #define SET_CS_PIN(st) OUT_WRITE(st##_CS_PIN, HIGH)
  void tmc_init_cs_pins() {
    #if AXIS_HAS_SPI(X)
      SET_CS_PIN(X);
    #endif
    #if AXIS_HAS_SPI(Y)
      SET_CS_PIN(Y);
    #endif
    #if AXIS_HAS_SPI(Z)
      SET_CS_PIN(Z);
    #endif
    #if AXIS_HAS_SPI(X2)
      SET_CS_PIN(X2);
    #endif
    #if AXIS_HAS_SPI(Y2)
      SET_CS_PIN(Y2);
    #endif
    #if AXIS_HAS_SPI(Z2)
      SET_CS_PIN(Z2);
    #endif
    #if AXIS_HAS_SPI(Z3)
      SET_CS_PIN(Z3);
    #endif
    #if AXIS_HAS_SPI(Z4)
      SET_CS_PIN(Z4);
    #endif
    #if AXIS_HAS_SPI(E0)
      SET_CS_PIN(E0);
    #endif
    #if AXIS_HAS_SPI(E1)
      SET_CS_PIN(E1);
    #endif
    #if AXIS_HAS_SPI(E2)
      SET_CS_PIN(E2);
    #endif
    #if AXIS_HAS_SPI(E3)
      SET_CS_PIN(E3);
    #endif
    #if AXIS_HAS_SPI(E4)
      SET_CS_PIN(E4);
    #endif
    #if AXIS_HAS_SPI(E5)
      SET_CS_PIN(E5);
    #endif
    #if AXIS_HAS_SPI(E6)
      SET_CS_PIN(E6);
    #endif
    #if AXIS_HAS_SPI(E7)
      SET_CS_PIN(E7);
    #endif
  }
#endif // HAS_TMC_SPI

template<typename TMC>
static bool test_connection(TMC &st) {
  SERIAL_ECHOPGM("Testing ");
  st.printLabel();
  SERIAL_ECHOPGM(" connection... ");
  const uint8_t test_result = st.test_connection();

  if (test_result > 0) SERIAL_ECHOPGM("Error: All ");

  const char *stat;
  switch (test_result) {
    default:
    case 0: stat = PSTR("OK"); break;
    case 1: stat = PSTR("HIGH"); break;
    case 2: stat = PSTR("LOW"); break;
  }
  serialprintPGM(stat);
  SERIAL_EOL();

  return test_result;
}

void test_tmc_connection(const bool test_x, const bool test_y, const bool test_z, const bool test_e) {
  uint8_t axis_connection = 0;

  if (test_x) {
    #if AXIS_IS_TMC(X)
      axis_connection += test_connection(stepperX);
    #endif
    #if AXIS_IS_TMC(X2)
      axis_connection += test_connection(stepperX2);
    #endif
  }

  if (test_y) {
    #if AXIS_IS_TMC(Y)
      axis_connection += test_connection(stepperY);
    #endif
    #if AXIS_IS_TMC(Y2)
      axis_connection += test_connection(stepperY2);
    #endif
  }

  if (test_z) {
    #if AXIS_IS_TMC(Z)
      axis_connection += test_connection(stepperZ);
    #endif
    #if AXIS_IS_TMC(Z2)
      axis_connection += test_connection(stepperZ2);
    #endif
    #if AXIS_IS_TMC(Z3)
      axis_connection += test_connection(stepperZ3);
    #endif
    #if AXIS_IS_TMC(Z4)
      axis_connection += test_connection(stepperZ4);
    #endif
  }

  if (test_e) {
    #if AXIS_IS_TMC(E0)
      axis_connection += test_connection(stepperE0);
    #endif
    #if AXIS_IS_TMC(E1)
      axis_connection += test_connection(stepperE1);
    #endif
    #if AXIS_IS_TMC(E2)
      axis_connection += test_connection(stepperE2);
    #endif
    #if AXIS_IS_TMC(E3)
      axis_connection += test_connection(stepperE3);
    #endif
    #if AXIS_IS_TMC(E4)
      axis_connection += test_connection(stepperE4);
    #endif
    #if AXIS_IS_TMC(E5)
      axis_connection += test_connection(stepperE5);
    #endif
    #if AXIS_IS_TMC(E6)
      axis_connection += test_connection(stepperE6);
    #endif
    #if AXIS_IS_TMC(E7)
      axis_connection += test_connection(stepperE7);
    #endif
  }

  if (axis_connection) LCD_MESSAGEPGM(MSG_ERROR_TMC);
}

#endif // HAS_TRINAMIC_CONFIG
