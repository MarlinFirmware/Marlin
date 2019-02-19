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

#include "../inc/MarlinConfig.h"
#include "../lcd/ultralcd.h"
#if HAS_TRINAMIC
  #include <TMCStepper.h>
#endif
#if HAS_LCD_MENU
  #include "../module/planner.h"
#endif

#define TMC_X_LABEL 'X', '0'
#define TMC_Y_LABEL 'Y', '0'
#define TMC_Z_LABEL 'Z', '0'

#define TMC_X2_LABEL 'X', '2'
#define TMC_Y2_LABEL 'Y', '2'
#define TMC_Z2_LABEL 'Z', '2'
#define TMC_Z3_LABEL 'Z', '3'

#define TMC_E0_LABEL 'E', '0'
#define TMC_E1_LABEL 'E', '1'
#define TMC_E2_LABEL 'E', '2'
#define TMC_E3_LABEL 'E', '3'
#define TMC_E4_LABEL 'E', '4'
#define TMC_E5_LABEL 'E', '5'

#define CHOPPER_DEFAULT_12V  { 3, -1, 1 }
#define CHOPPER_DEFAULT_19V  { 4,  1, 1 }
#define CHOPPER_DEFAULT_24V  { 4,  2, 1 }
#define CHOPPER_DEFAULT_36V  { 5,  2, 4 }
#define CHOPPER_PRUSAMK3_24V { 4,  1, 4 }
#define CHOPPER_MARLIN_119   { 5,  2, 3 }

#if ENABLED(MONITOR_DRIVER_STATUS) && !defined(MONITOR_DRIVER_STATUS_INTERVAL_MS)
  #define MONITOR_DRIVER_STATUS_INTERVAL_MS 500u
#endif

constexpr uint16_t _tmc_thrs(const uint16_t msteps, const int32_t thrs, const uint32_t spmm) {
  return 12650000UL * msteps / (256 * thrs * spmm);
}

template<char AXIS_LETTER, char DRIVER_ID>
class TMCStorage {
  protected:
    // Only a child class has access to constructor => Don't create on its own! "Poor man's abstract class"
    TMCStorage() {}

  public:
    uint16_t val_mA = 0;

    #if ENABLED(MONITOR_DRIVER_STATUS)
      uint8_t otpw_count = 0,
              error_count = 0;
      bool flag_otpw = false;
      inline bool getOTPW() { return flag_otpw; }
      inline void clear_otpw() { flag_otpw = 0; }
    #endif

    inline uint16_t getMilliamps() { return val_mA; }

    inline void printLabel() {
      SERIAL_CHAR(AXIS_LETTER);
      if (DRIVER_ID > '0') SERIAL_CHAR(DRIVER_ID);
    }

    struct {
      #if HAS_STEALTHCHOP
        bool stealthChop_enabled = false;
      #endif
      #if ENABLED(HYBRID_THRESHOLD)
        uint8_t hybrid_thrs = 0;
      #endif
      #if ENABLED(SENSORLESS_HOMING)
        int8_t homing_thrs = 0;
      #endif
    } stored;
};

template<class TMC, char AXIS_LETTER, char DRIVER_ID>
class TMCMarlin : public TMC, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(uint16_t cs_pin, float RS) :
      TMC(cs_pin, RS)
      {}
    TMCMarlin(uint16_t CS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK) :
      TMC(CS, RS, pinMOSI, pinMISO, pinSCK)
      {}
    inline uint16_t rms_current() { return TMC::rms_current(); }
    inline void rms_current(uint16_t mA) {
      this->val_mA = mA;
      TMC::rms_current(mA);
    }
    inline void rms_current(uint16_t mA, float mult) {
      this->val_mA = mA;
      TMC::rms_current(mA, mult);
    }

    #if HAS_STEALTHCHOP
      inline void refresh_stepping_mode() { this->en_pwm_mode(this->stored.stealthChop_enabled); }
      inline bool get_stealthChop_status() { return this->en_pwm_mode(); }
    #endif

    #if HAS_LCD_MENU

      inline void init_lcd_variables(const AxisEnum spmm_id) {
        #if ENABLED(HYBRID_THRESHOLD)
          this->stored.hybrid_thrs = _tmc_thrs(this->microsteps(), this->TPWMTHRS(), planner.settings.axis_steps_per_mm[spmm_id]);
        #endif
        #if ENABLED(SENSORLESS_HOMING)
          this->stored.homing_thrs = this->sgt();
        #endif
      }

      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if ENABLED(HYBRID_THRESHOLD)
        inline void refresh_hybrid_thrs(float spmm) { this->TPWMTHRS(_tmc_thrs(this->microsteps(), this->stored.hybrid_thrs, spmm)); }
      #endif
      #if ENABLED(SENSORLESS_HOMING)
        inline void refresh_homing_thrs() { this->sgt(this->stored.homing_thrs); }
      #endif
    #endif
};
template<char AXIS_LETTER, char DRIVER_ID>
class TMCMarlin<TMC2208Stepper, AXIS_LETTER, DRIVER_ID> : public TMC2208Stepper, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(Stream * SerialPort, float RS, bool has_rx=true) :
      TMC2208Stepper(SerialPort, RS, has_rx=true)
      {}
    TMCMarlin(uint16_t RX, uint16_t TX, float RS, bool has_rx=true) :
      TMC2208Stepper(RX, TX, RS, has_rx=true)
      {}
    uint16_t rms_current() { return TMC2208Stepper::rms_current(); }
    inline void rms_current(uint16_t mA) {
      this->val_mA = mA;
      TMC2208Stepper::rms_current(mA);
    }
    inline void rms_current(uint16_t mA, float mult) {
      this->val_mA = mA;
      TMC2208Stepper::rms_current(mA, mult);
    }

    #if HAS_STEALTHCHOP
      inline void refresh_stepping_mode() { en_spreadCycle(!this->stored.stealthChop_enabled); }
      inline bool get_stealthChop_status() { return !this->en_spreadCycle(); }
    #endif

    #if HAS_LCD_MENU

      inline void init_lcd_variables(const AxisEnum spmm_id) {
        #if ENABLED(HYBRID_THRESHOLD)
          this->stored.hybrid_thrs = _tmc_thrs(this->microsteps(), this->TPWMTHRS(), planner.settings.axis_steps_per_mm[spmm_id]);
        #endif
      }

      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if ENABLED(HYBRID_THRESHOLD)
        inline void refresh_hybrid_thrs(float spmm) { this->TPWMTHRS(_tmc_thrs(this->microsteps(), this->stored.hybrid_thrs, spmm)); }
      #endif
    #endif
};
template<char AXIS_LETTER, char DRIVER_ID>
class TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID> : public TMC2660Stepper, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(uint16_t cs_pin, float RS) :
      TMC2660Stepper(cs_pin, RS)
      {}
    TMCMarlin(uint16_t CS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK) :
      TMC2660Stepper(CS, RS, pinMOSI, pinMISO, pinSCK)
      {}
    inline uint16_t rms_current() { return TMC2660Stepper::rms_current(); }
    inline void rms_current(uint16_t mA) {
      this->val_mA = mA;
      TMC2660Stepper::rms_current(mA);
    }

    #if HAS_LCD_MENU
      inline void init_lcd_variables(const AxisEnum spmm_id) {
        #if ENABLED(SENSORLESS_HOMING)
          this->stored.homing_thrs = this->sgt();
        #endif
      }

      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if ENABLED(SENSORLESS_HOMING)
        inline void refresh_homing_thrs() { this->sgt(this->stored.homing_thrs); }
      #endif
    #endif
};

template<typename TMC>
void tmc_get_current(TMC &st) {
  st.printLabel();
  SERIAL_ECHOLNPAIR(" driver current: ", st.getMilliamps());
}
template<typename TMC>
void tmc_set_current(TMC &st, const int mA) {
  st.rms_current(mA);
}

#if ENABLED(MONITOR_DRIVER_STATUS)
  template<typename TMC>
  void tmc_report_otpw(TMC &st) {
    st.printLabel();
    SERIAL_ECHOPGM(" temperature prewarn triggered: ");
    serialprintPGM(st.getOTPW() ? PSTR("true") : PSTR("false"));
    SERIAL_EOL();
  }
  template<typename TMC>
  void tmc_clear_otpw(TMC &st) {
    st.clear_otpw();
    st.printLabel();
    SERIAL_ECHOLNPGM(" prewarn flag cleared");
  }
#endif
template<typename TMC>
void tmc_get_pwmthrs(TMC &st, const uint16_t spmm) {
  st.printLabel();
  SERIAL_ECHOLNPAIR(" stealthChop max speed: ", _tmc_thrs(st.microsteps(), st.TPWMTHRS(), spmm));
}
template<typename TMC>
void tmc_set_pwmthrs(TMC &st, const int32_t thrs, const uint32_t spmm) {
  st.TPWMTHRS(_tmc_thrs(st.microsteps(), thrs, spmm));
}
template<typename TMC>
void tmc_get_sgt(TMC &st) {
  st.printLabel();
  SERIAL_ECHOPGM(" homing sensitivity: ");
  SERIAL_PRINTLN(st.sgt(), DEC);
}
template<typename TMC>
void tmc_set_sgt(TMC &st, const int8_t sgt_val) {
  st.sgt(sgt_val);
}

void monitor_tmc_driver();
void test_tmc_connection(const bool test_x, const bool test_y, const bool test_z, const bool test_e);

#if ENABLED(TMC_DEBUG)
  #if ENABLED(MONITOR_DRIVER_STATUS)
    void tmc_set_report_interval(const uint16_t update_interval);
  #endif
  void tmc_report_all(const bool print_x, const bool print_y, const bool print_z, const bool print_e);
  void tmc_get_registers(const bool print_x, const bool print_y, const bool print_z, const bool print_e);
#endif

#if HAS_LCD_MENU
  void init_tmc_section();
#endif

/**
 * TMC2130 specific sensorless homing using stallGuard2.
 * stallGuard2 only works when in spreadCycle mode.
 * spreadCycle and stealthChop are mutually exclusive.
 *
 * Defined here because of limitations with templates and headers.
 */
#if USE_SENSORLESS
  // Track enabled status of stealthChop and only re-enable where applicable
  struct sensorless_t {
    bool x, y, z, x2, y2, z2, z3;
  };

  bool tmc_enable_stallguard(TMC2130Stepper &st);
  void tmc_disable_stallguard(TMC2130Stepper &st, const bool restore_stealth);

  bool tmc_enable_stallguard(TMC2660Stepper);
  void tmc_disable_stallguard(TMC2660Stepper, const bool);
#endif

#if TMC_HAS_SPI
  void tmc_init_cs_pins();
#endif
