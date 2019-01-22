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

#if ENABLED(TMC_DEBUG)
  #include "../module/planner.h"
  #include "../libs/hex_print_routines.h"
  #if ENABLED(MONITOR_DRIVER_STATUS)
    static bool report_tmc_status; // = false;
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
    bool is_otpw,
         is_ot,
         is_s2ga,
         is_s2gb,
         is_error;
  };
  #if HAS_DRIVER(TMC2130)
    #if ENABLED(TMC_DEBUG)
      static uint32_t get_pwm_scale(TMC2130Stepper &st) { return st.PWM_SCALE(); }
      static uint8_t get_status_response(TMC2130Stepper &st, uint32_t) { return st.status_response & 0xF; }
    #endif
    static TMC_driver_data get_driver_data(TMC2130Stepper &st) {
      constexpr uint32_t OTPW_bm = 0x4000000UL;
      constexpr uint8_t OTPW_bp = 26;
      constexpr uint32_t OT_bm = 0x2000000UL;
      constexpr uint8_t OT_bp = 25;
      constexpr uint8_t S2GA_bp = 27;
      constexpr uint8_t S2GB_bp = 28;
      TMC_driver_data data;
      data.drv_status = st.DRV_STATUS();
      data.is_otpw = (data.drv_status & OTPW_bm) >> OTPW_bp;
      data.is_ot = (data.drv_status & OT_bm) >> OT_bp;
      data.is_s2ga = (data.drv_status >> S2GA_bp) & 0b1;
      data.is_s2gb = (data.drv_status >> S2GB_bp) & 0b1;
      return data;
    }
  #endif
  #if HAS_DRIVER(TMC2208)
    #if ENABLED(TMC_DEBUG)
      static uint32_t get_pwm_scale(TMC2208Stepper &st) { return st.pwm_scale_sum(); }
      static uint8_t get_status_response(TMC2208Stepper &st, uint32_t drv_status) {
        uint8_t gstat = st.GSTAT();
        uint8_t response = 0;
        response |= (drv_status >> (31 - 3)) & 0b1000;
        response |= gstat & 0b11;
        return response;
      }
    #endif
    static TMC_driver_data get_driver_data(TMC2208Stepper &st) {
      constexpr uint32_t OTPW_bm = 0b1ul;
      constexpr uint8_t OTPW_bp = 0;
      constexpr uint32_t OT_bm = 0b10ul;
      constexpr uint8_t OT_bp = 1;
      constexpr uint8_t S2GA_bp = 2;
      constexpr uint8_t S2GB_bp = 3;
      TMC_driver_data data;
      data.drv_status = st.DRV_STATUS();
      data.is_otpw = (data.drv_status & OTPW_bm) >> OTPW_bp;
      data.is_ot = (data.drv_status & OT_bm) >> OT_bp;
      data.is_s2ga = (data.drv_status >> S2GA_bp) & 0b1;
      data.is_s2gb = (data.drv_status >> S2GB_bp) & 0b1;
      return data;
    }
  #endif
  #if HAS_DRIVER(TMC2660)
    #if ENABLED(TMC_DEBUG)
      static uint32_t get_pwm_scale(TMC2660Stepper) { return 0; }
      static uint8_t get_status_response(TMC2660Stepper, uint32_t drv_status) {
        return drv_status & 0xFF;
      }
    #endif
    static TMC_driver_data get_driver_data(TMC2660Stepper &st) {
      constexpr uint32_t OTPW_bm = 0x4UL;
      constexpr uint8_t OTPW_bp = 2;
      constexpr uint32_t OT_bm = 0x2UL;
      constexpr uint8_t OT_bp = 1;
      TMC_driver_data data;
      data.drv_status = st.DRVSTATUS();
      data.is_otpw = (data.drv_status & OTPW_bm) >> OTPW_bp;
      data.is_ot = (data.drv_status & OT_bm) >> OT_bp;
      return data;
    }
  #endif

  #if ENABLED(STOP_ON_ERROR)
    void report_driver_error(const TMC_driver_data &data) {
      SERIAL_ECHOPGM(" driver error detected: 0x");
      SERIAL_PRINTLN(data.drv_status, HEX);
      if (data.is_ot) SERIAL_ECHOLNPGM("overtemperature");
      if (data.is_s2ga) SERIAL_ECHOLNPGM("short to ground (coil A)");
      if (data.is_s2gb) SERIAL_ECHOLNPGM("short to ground (coil B)");
      #if ENABLED(TMC_DEBUG)
        tmc_report_all(true, true, true, true);
      #endif
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
    SERIAL_ECHOPGM(" driver overtemperature warning! (");
    SERIAL_ECHO(st.getMilliamps());
    SERIAL_ECHOLNPGM("mA)");
  }

  template<typename TMC>
  void report_polled_driver_data(TMC &st, const TMC_driver_data &data) {
    const uint32_t pwm_scale = get_pwm_scale(st);
    st.printLabel();
    SERIAL_ECHOPAIR(":", pwm_scale);
    SERIAL_ECHOPGM(" |0b"); SERIAL_PRINT(get_status_response(st, data.drv_status), BIN);
    SERIAL_ECHOPGM("| ");
    if (st.error_count) SERIAL_CHAR('E');
    else if (data.is_ot) SERIAL_CHAR('O');
    else if (data.is_otpw) SERIAL_CHAR('W');
    else if (st.otpw_count > 0) SERIAL_PRINT(st.otpw_count, DEC);
    else if (st.flag_otpw) SERIAL_CHAR('F');
    SERIAL_CHAR('\t');
  }

  template<typename TMC>
  void monitor_tmc_driver(TMC &st) {
    TMC_driver_data data = get_driver_data(st);
    if ((data.drv_status == 0xFFFFFFFF) || (data.drv_status == 0x0)) return;

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
    if (data.is_otpw && st.otpw_count == 0) {
      report_driver_otpw(st);
    }
    #if CURRENT_STEP_DOWN > 0
      // Decrease current if is_otpw is true and driver is enabled and there's been more than 4 warnings
      if (data.is_otpw && st.otpw_count > 4) {
        uint16_t I_rms = st.getMilliamps();
        if (st.isEnabled() && I_rms > 100) {
          st.rms_current(I_rms - (CURRENT_STEP_DOWN));
          #if ENABLED(REPORT_CURRENT_CHANGE)
            st.printLabel();
            SERIAL_ECHOLNPAIR(" current decreased to ", st.getMilliamps());
          #endif
        }
      }
    #endif

    if (data.is_otpw) {
      st.otpw_count++;
      st.flag_otpw = true;
    }
    else if (st.otpw_count > 0) st.otpw_count = 0;

    #if ENABLED(TMC_DEBUG)
      if (report_tmc_status) {
        report_polled_driver_data(st, data);
      }
    #endif
  }

  #define HAS_HW_COMMS(ST) AXIS_DRIVER_TYPE(ST, TMC2130) || AXIS_DRIVER_TYPE(ST, TMC2660) || (AXIS_DRIVER_TYPE(ST, TMC2208) && defined(ST##_HARDWARE_SERIAL))

  void monitor_tmc_driver() {
    static millis_t next_poll = 0;
    const millis_t ms = millis();
    if (ELAPSED(ms, next_poll)) {
      next_poll = ms + 500;
      #if HAS_HW_COMMS(X)
        monitor_tmc_driver(stepperX);
      #endif
      #if HAS_HW_COMMS(Y)
        monitor_tmc_driver(stepperY);
      #endif
      #if HAS_HW_COMMS(Z)
        monitor_tmc_driver(stepperZ);
      #endif
      #if HAS_HW_COMMS(X2)
        monitor_tmc_driver(stepperX2);
      #endif
      #if HAS_HW_COMMS(Y2)
        monitor_tmc_driver(stepperY2);
      #endif
      #if HAS_HW_COMMS(Z2)
        monitor_tmc_driver(stepperZ2);
      #endif
      #if HAS_HW_COMMS(Z3)
        monitor_tmc_driver(stepperZ3);
      #endif
      #if HAS_HW_COMMS(E0)
        monitor_tmc_driver(stepperE0);
      #endif
      #if HAS_HW_COMMS(E1)
        monitor_tmc_driver(stepperE1);
      #endif
      #if HAS_HW_COMMS(E2)
        monitor_tmc_driver(stepperE2);
      #endif
      #if HAS_HW_COMMS(E3)
        monitor_tmc_driver(stepperE3);
      #endif
      #if HAS_HW_COMMS(E4)
        monitor_tmc_driver(stepperE4);
      #endif
      #if HAS_HW_COMMS(E5)
        monitor_tmc_driver(stepperE5);
      #endif

      #if ENABLED(TMC_DEBUG)
        if (report_tmc_status) SERIAL_EOL();
      #endif
    }
  }

#endif // MONITOR_DRIVER_STATUS

#if ENABLED(TMC_DEBUG)

  /**
   * M122 S[1,0] Enable periodic status reports
   */
  #if ENABLED(MONITOR_DRIVER_STATUS)
    void tmc_set_report_status(const bool status) {
      if ((report_tmc_status = status))
        SERIAL_ECHOLNPGM("axis:pwm_scale |status_response|");
    }
  #endif

  enum TMC_debug_enum : char {
    TMC_CODES,
    TMC_ENABLED,
    TMC_CURRENT,
    TMC_RMS_CURRENT,
    TMC_MAX_CURRENT,
    TMC_IRUN,
    TMC_IHOLD,
    TMC_CS_ACTUAL,
    TMC_PWM_SCALE,
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
    TMC_SGT
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

  #if HAS_DRIVER(TMC2130)
    static void tmc_status(TMC2130Stepper &st, const TMC_debug_enum i) {
      switch (i) {
        case TMC_PWM_SCALE: SERIAL_PRINT(st.PWM_SCALE(), DEC); break;
        case TMC_SGT: SERIAL_PRINT(st.sgt(), DEC); break;
        case TMC_STEALTHCHOP: serialprintPGM(st.en_pwm_mode() ? PSTR("true") : PSTR("false")); break;
        default: break;
      }
    }
    static void _tmc_parse_drv_status(TMC2130Stepper &st, const TMC_drv_status_enum i) {
      switch (i) {
        case TMC_STALLGUARD: if (st.stallguard()) SERIAL_CHAR('X'); break;
        case TMC_SG_RESULT:  SERIAL_PRINT(st.sg_result(), DEC);   break;
        case TMC_FSACTIVE:   if (st.fsactive())   SERIAL_CHAR('X'); break;
        case TMC_DRV_CS_ACTUAL: SERIAL_PRINT(st.cs_actual(), DEC); break;
        default: break;
      }
    }
  #endif

  #if HAS_DRIVER(TMC2208)
    static void tmc_status(TMC2208Stepper &st, const TMC_debug_enum i) {
      switch (i) {
        case TMC_PWM_SCALE: SERIAL_PRINT(st.pwm_scale_sum(), DEC); break;
        case TMC_STEALTHCHOP: serialprintPGM(st.stealth() ? PSTR("true") : PSTR("false")); break;
        case TMC_S2VSA: if (st.s2vsa()) SERIAL_CHAR('X'); break;
        case TMC_S2VSB: if (st.s2vsb()) SERIAL_CHAR('X'); break;
        default: break;
      }
    }
    static void _tmc_parse_drv_status(TMC2208Stepper &st, const TMC_drv_status_enum i) {
      switch (i) {
        case TMC_T157: if (st.t157()) SERIAL_CHAR('X'); break;
        case TMC_T150: if (st.t150()) SERIAL_CHAR('X'); break;
        case TMC_T143: if (st.t143()) SERIAL_CHAR('X'); break;
        case TMC_T120: if (st.t120()) SERIAL_CHAR('X'); break;
        case TMC_DRV_CS_ACTUAL: SERIAL_PRINT(st.cs_actual(), DEC); break;
        default: break;
      }
    }
  #endif

  #if HAS_DRIVER(TMC2660)
    static void _tmc_parse_drv_status(TMC2660Stepper, const TMC_drv_status_enum) { }
  #endif

  template <typename TMC>
  static void tmc_status(TMC &st, const TMC_debug_enum i, const float spmm) {
    SERIAL_CHAR('\t');
    switch (i) {
      case TMC_CODES: st.printLabel(); break;
      case TMC_ENABLED: serialprintPGM(st.isEnabled() ? PSTR("true") : PSTR("false")); break;
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
          uint32_t tstep_value = st.TSTEP();
          if (tstep_value == 0xFFFFF) SERIAL_ECHOPGM("max");
          else SERIAL_ECHO(tstep_value);
        }
        break;
      case TMC_TPWMTHRS: {
          uint32_t tpwmthrs_val = st.TPWMTHRS();
          SERIAL_ECHO(tpwmthrs_val);
        }
        break;
      case TMC_TPWMTHRS_MMS: {
          uint32_t tpwmthrs_val = st.TPWMTHRS();
          if (tpwmthrs_val)
            SERIAL_ECHO(12650000UL * st.microsteps() / (256 * tpwmthrs_val * spmm));
          else
            SERIAL_CHAR('-');
        }
        break;
      case TMC_OTPW: serialprintPGM(st.otpw() ? PSTR("true") : PSTR("false")); break;
      #if ENABLED(MONITOR_DRIVER_STATUS)
        case TMC_OTPW_TRIGGERED: serialprintPGM(st.getOTPW() ? PSTR("true") : PSTR("false")); break;
      #endif
      case TMC_TOFF: SERIAL_PRINT(st.toff(), DEC); break;
      case TMC_TBL: SERIAL_PRINT(st.blank_time(), DEC); break;
      case TMC_HEND: SERIAL_PRINT(st.hysteresis_end(), DEC); break;
      case TMC_HSTRT: SERIAL_PRINT(st.hysteresis_start(), DEC); break;
      default: tmc_status(st, i); break;
    }
  }

  #if HAS_DRIVER(TMC2660)
    template<char AXIS_LETTER, char DRIVER_ID>
    void tmc_status(TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID> &st, const TMC_debug_enum i, const float) {
      SERIAL_CHAR('\t');
      switch (i) {
        case TMC_CODES: st.printLabel(); break;
        case TMC_ENABLED: serialprintPGM(st.isEnabled() ? PSTR("true") : PSTR("false")); break;
        case TMC_CURRENT: SERIAL_ECHO(st.getMilliamps()); break;
        case TMC_RMS_CURRENT: SERIAL_ECHO(st.rms_current()); break;
        case TMC_MAX_CURRENT: SERIAL_PRINT((float)st.rms_current() * 1.41, 0); break;
        case TMC_IRUN:
          SERIAL_PRINT(st.cs(), DEC);
          SERIAL_ECHOPGM("/31");
          break;
        case TMC_VSENSE: serialprintPGM(st.vsense() ? PSTR("1=.165") : PSTR("0=.310")); break;
        case TMC_MICROSTEPS: SERIAL_ECHO(st.microsteps()); break;
        //case TMC_OTPW: serialprintPGM(st.otpw() ? PSTR("true") : PSTR("false")); break;
        //case TMC_OTPW_TRIGGERED: serialprintPGM(st.getOTPW() ? PSTR("true") : PSTR("false")); break;
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
      case TMC_STST:          if (st.stst())         SERIAL_CHAR('X'); break;
      case TMC_OLB:           if (st.olb())          SERIAL_CHAR('X'); break;
      case TMC_OLA:           if (st.ola())          SERIAL_CHAR('X'); break;
      case TMC_S2GB:          if (st.s2gb())         SERIAL_CHAR('X'); break;
      case TMC_S2GA:          if (st.s2ga())         SERIAL_CHAR('X'); break;
      case TMC_DRV_OTPW:      if (st.otpw())         SERIAL_CHAR('X'); break;
      case TMC_OT:            if (st.ot())           SERIAL_CHAR('X'); break;
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
        tmc_status(stepperX, i, planner.settings.axis_steps_per_mm[X_AXIS]);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_status(stepperX2, i, planner.settings.axis_steps_per_mm[X_AXIS]);
      #endif
    }

    if (print_y) {
      #if AXIS_IS_TMC(Y)
        tmc_status(stepperY, i, planner.settings.axis_steps_per_mm[Y_AXIS]);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_status(stepperY2, i, planner.settings.axis_steps_per_mm[Y_AXIS]);
      #endif
    }

    if (print_z) {
      #if AXIS_IS_TMC(Z)
        tmc_status(stepperZ, i, planner.settings.axis_steps_per_mm[Z_AXIS]);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_status(stepperZ2, i, planner.settings.axis_steps_per_mm[Z_AXIS]);
      #endif
      #if AXIS_IS_TMC(Z3)
        tmc_status(stepperZ3, i, planner.settings.axis_steps_per_mm[Z_AXIS]);
      #endif
    }

    if (print_e) {
      #if AXIS_IS_TMC(E0)
        tmc_status(stepperE0, i, planner.settings.axis_steps_per_mm[E_AXIS]);
      #endif
      #if AXIS_IS_TMC(E1)
        tmc_status(stepperE1, i, planner.settings.axis_steps_per_mm[E_AXIS
          #if ENABLED(DISTINCT_E_FACTORS)
            + 1
          #endif
        ]);
      #endif
      #if AXIS_IS_TMC(E2)
        tmc_status(stepperE2, i, planner.settings.axis_steps_per_mm[E_AXIS
          #if ENABLED(DISTINCT_E_FACTORS)
            + 2
          #endif
        ]);
      #endif
      #if AXIS_IS_TMC(E3)
        tmc_status(stepperE3, i, planner.settings.axis_steps_per_mm[E_AXIS
          #if ENABLED(DISTINCT_E_FACTORS)
            + 3
          #endif
        ]);
      #endif
      #if AXIS_IS_TMC(E4)
        tmc_status(stepperE4, i, planner.settings.axis_steps_per_mm[E_AXIS
          #if ENABLED(DISTINCT_E_FACTORS)
            + 4
          #endif
        ]);
      #endif
      #if AXIS_IS_TMC(E5)
        tmc_status(stepperE5, i, planner.settings.axis_steps_per_mm[E_AXIS
          #if ENABLED(DISTINCT_E_FACTORS)
            + 5
          #endif
        ]);
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
    TMC_REPORT("Enabled\t",          TMC_ENABLED);
    TMC_REPORT("Set current",        TMC_CURRENT);
    TMC_REPORT("RMS current",        TMC_RMS_CURRENT);
    TMC_REPORT("MAX current",        TMC_MAX_CURRENT);
    TMC_REPORT("Run current",        TMC_IRUN);
    TMC_REPORT("Hold current",       TMC_IHOLD);
    TMC_REPORT("CS actual\t",        TMC_CS_ACTUAL);
    TMC_REPORT("PWM scale",          TMC_PWM_SCALE);
    TMC_REPORT("vsense\t",           TMC_VSENSE);
    TMC_REPORT("stealthChop",        TMC_STEALTHCHOP);
    TMC_REPORT("msteps\t",           TMC_MICROSTEPS);
    TMC_REPORT("tstep\t",            TMC_TSTEP);
    TMC_REPORT("pwm\nthreshold\t",   TMC_TPWMTHRS);
    TMC_REPORT("[mm/s]\t",           TMC_TPWMTHRS_MMS);
    TMC_REPORT("OT prewarn",         TMC_OTPW);
    #if ENABLED(MONITOR_DRIVER_STATUS)
      TMC_REPORT("OT prewarn has\n"
                 "been triggered",   TMC_OTPW_TRIGGERED);
    #endif
    TMC_REPORT("off time\t",         TMC_TOFF);
    TMC_REPORT("blank time",         TMC_TBL);
    TMC_REPORT("hysteresis\n-end\t", TMC_HEND);
    TMC_REPORT("-start\t",           TMC_HSTRT);
    TMC_REPORT("Stallguard thrs",    TMC_SGT);

    DRV_REPORT("DRVSTATUS",          TMC_DRV_CODES);
    #if HAS_DRIVER(TMC2130)
      DRV_REPORT("stallguard\t",     TMC_STALLGUARD);
      DRV_REPORT("sg_result\t",      TMC_SG_RESULT);
      DRV_REPORT("fsactive\t",       TMC_FSACTIVE);
    #endif
    DRV_REPORT("stst\t",             TMC_STST);
    DRV_REPORT("olb\t",              TMC_OLB);
    DRV_REPORT("ola\t",              TMC_OLA);
    DRV_REPORT("s2gb\t",             TMC_S2GB);
    DRV_REPORT("s2ga\t",             TMC_S2GA);
    DRV_REPORT("otpw\t",             TMC_DRV_OTPW);
    DRV_REPORT("ot\t",               TMC_OT);
    #if HAS_DRIVER(TMC2208)
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

  #if HAS_DRIVER(TMC2130)
    static void tmc_get_ic_registers(TMC2130Stepper &st, const TMC_get_registers_enum i) {
      switch (i) {
        PRINT_TMC_REGISTER(TCOOLTHRS);
        PRINT_TMC_REGISTER(THIGH);
        PRINT_TMC_REGISTER(COOLCONF);
        default: SERIAL_CHAR('\t'); break;
      }
    }
  #endif
  #if HAS_DRIVER(TMC2208)
    static void tmc_get_ic_registers(TMC2208Stepper, const TMC_get_registers_enum) { SERIAL_CHAR('\t'); }
  #endif

  #if HAS_TRINAMIC
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
    template <char AXIS_LETTER, char DRIVER_ID>
    static void tmc_get_registers(TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID> &st, const TMC_get_registers_enum i) {
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
    bool stealthchop_was_enabled = st.en_pwm_mode();

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
  bool tmc_enable_stallguard(TMC2660Stepper) {
    // TODO
    return false;
  }
  void tmc_disable_stallguard(TMC2660Stepper, const bool) {};

#endif // USE_SENSORLESS

#if TMC_HAS_SPI
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
  }
#endif // TMC_HAS_SPI

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
  }

  if (axis_connection) ui.set_status_P(PSTR("TMC CONNECTION ERROR"));
}

#if HAS_LCD_MENU

  void init_tmc_section() {
    #if AXIS_IS_TMC(X)
      stepperX.init_lcd_variables(X_AXIS);
    #endif
    #if AXIS_IS_TMC(Y)
      stepperY.init_lcd_variables(Y_AXIS);
    #endif
    #if AXIS_IS_TMC(Z)
      stepperZ.init_lcd_variables(Z_AXIS);
    #endif
    #if AXIS_IS_TMC(X2)
      stepperX2.init_lcd_variables(X_AXIS);
    #endif
    #if AXIS_IS_TMC(Y2)
      stepperY2.init_lcd_variables(Y_AXIS);
    #endif
    #if AXIS_IS_TMC(Z2)
      stepperZ2.init_lcd_variables(Z_AXIS);
    #endif
    #if AXIS_IS_TMC(Z3)
      stepperZ3.init_lcd_variables(Z_AXIS);
    #endif
    #if AXIS_IS_TMC(E0)
      stepperE0.init_lcd_variables(E_AXIS);
    #endif
    #if AXIS_IS_TMC(E1)
      stepperE1.init_lcd_variables(E_AXIS_N(1));
    #endif
    #if AXIS_IS_TMC(E2)
      stepperE2.init_lcd_variables(E_AXIS_N(2));
    #endif
    #if AXIS_IS_TMC(E3)
      stepperE3.init_lcd_variables(E_AXIS_N(3));
    #endif
    #if AXIS_IS_TMC(E4)
      stepperE4.init_lcd_variables(E_AXIS_N(4));
    #endif
    #if AXIS_IS_TMC(E5)
      stepperE5.init_lcd_variables(E_AXIS_N(5));
    #endif
  }

#endif

#endif // HAS_TRINAMIC
