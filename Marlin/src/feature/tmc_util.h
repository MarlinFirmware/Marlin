/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfig.h"
#include "../lcd/ultralcd.h"

#if HAS_TRINAMIC

#include <TMCStepper.h>
#include "../module/planner.h"

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
#define CHOPPER_PRUSAMK3_24V { 3, -2, 6 }
#define CHOPPER_MARLIN_119   { 5,  2, 3 }

#if ENABLED(MONITOR_DRIVER_STATUS) && !defined(MONITOR_DRIVER_STATUS_INTERVAL_MS)
  #define MONITOR_DRIVER_STATUS_INTERVAL_MS 500u
#endif

constexpr uint16_t _tmc_thrs(const uint16_t msteps, const uint32_t thrs, const uint32_t spmm) {
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
      #if USE_SENSORLESS
        int16_t homing_thrs = 0;
      #endif
    } stored;
};

template<class TMC, char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
class TMCMarlin : public TMC, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(const uint16_t cs_pin, const float RS) :
      TMC(cs_pin, RS)
      {}
    TMCMarlin(const uint16_t CS, const float RS, const uint16_t pinMOSI, const uint16_t pinMISO, const uint16_t pinSCK) :
      TMC(CS, RS, pinMOSI, pinMISO, pinSCK)
      {}
    inline uint16_t rms_current() { return TMC::rms_current(); }
    inline void rms_current(uint16_t mA) {
      this->val_mA = mA;
      TMC::rms_current(mA);
    }
    inline void rms_current(const uint16_t mA, const float mult) {
      this->val_mA = mA;
      TMC::rms_current(mA, mult);
    }

    #if HAS_STEALTHCHOP
      inline void refresh_stepping_mode() { this->en_pwm_mode(this->stored.stealthChop_enabled); }
      inline bool get_stealthChop_status() { return this->en_pwm_mode(); }
    #endif
    #if ENABLED(HYBRID_THRESHOLD)
      uint32_t get_pwm_thrs() {
        return _tmc_thrs(this->microsteps(), this->TPWMTHRS(), planner.settings.axis_steps_per_mm[AXIS_ID]);
      }
      void set_pwm_thrs(const uint32_t thrs) {
        TMC::TPWMTHRS(_tmc_thrs(this->microsteps(), thrs, planner.settings.axis_steps_per_mm[AXIS_ID]));
        #if HAS_LCD_MENU
          this->stored.hybrid_thrs = thrs;
        #endif
      }
    #endif
    #if USE_SENSORLESS
      inline int16_t homing_threshold() { return TMC::sgt(); }
      void homing_threshold(int16_t sgt_val) {
        sgt_val = (int16_t)constrain(sgt_val, sgt_min, sgt_max);
        TMC::sgt(sgt_val);
        #if HAS_LCD_MENU
          this->stored.homing_thrs = sgt_val;
        #endif
      }
    #endif

    #if HAS_LCD_MENU
      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if ENABLED(HYBRID_THRESHOLD)
        inline void refresh_hybrid_thrs() { set_pwm_thrs(this->stored.hybrid_thrs); }
      #endif
      #if USE_SENSORLESS
        inline void refresh_homing_thrs() { homing_threshold(this->stored.homing_thrs); }
      #endif
    #endif

    static constexpr int8_t sgt_min = -64,
                            sgt_max =  63;
};
template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
class TMCMarlin<TMC2208Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> : public TMC2208Stepper, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(Stream * SerialPort, const float RS, const uint8_t) :
      TMC2208Stepper(SerialPort, RS, /*has_rx=*/true)
      {}
    TMCMarlin(const uint16_t RX, const uint16_t TX, const float RS, const uint8_t, const bool has_rx=true) :
      TMC2208Stepper(RX, TX, RS, has_rx)
      {}
    uint16_t rms_current() { return TMC2208Stepper::rms_current(); }
    inline void rms_current(const uint16_t mA) {
      this->val_mA = mA;
      TMC2208Stepper::rms_current(mA);
    }
    inline void rms_current(const uint16_t mA, const float mult) {
      this->val_mA = mA;
      TMC2208Stepper::rms_current(mA, mult);
    }

    #if HAS_STEALTHCHOP
      inline void refresh_stepping_mode() { en_spreadCycle(!this->stored.stealthChop_enabled); }
      inline bool get_stealthChop_status() { return !this->en_spreadCycle(); }
    #endif
    #if ENABLED(HYBRID_THRESHOLD)
      uint32_t get_pwm_thrs() {
        return _tmc_thrs(this->microsteps(), this->TPWMTHRS(), planner.settings.axis_steps_per_mm[AXIS_ID]);
      }
      void set_pwm_thrs(const uint32_t thrs) {
        TMC2208Stepper::TPWMTHRS(_tmc_thrs(this->microsteps(), thrs, planner.settings.axis_steps_per_mm[AXIS_ID]));
        #if HAS_LCD_MENU
          this->stored.hybrid_thrs = thrs;
        #endif
      }
    #endif

    #if HAS_LCD_MENU
      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if ENABLED(HYBRID_THRESHOLD)
        inline void refresh_hybrid_thrs() { set_pwm_thrs(this->stored.hybrid_thrs); }
      #endif
    #endif
};

template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
class TMCMarlin<TMC2209Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> : public TMC2209Stepper, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(Stream * SerialPort, const float RS, const uint8_t addr) :
      TMC2209Stepper(SerialPort, RS, addr)
      {}
    TMCMarlin(const uint16_t RX, const uint16_t TX, const float RS, const uint8_t addr, const bool) :
      TMC2209Stepper(RX, TX, RS, addr)
      {}
    uint8_t get_address() { return slave_address; }
    uint16_t rms_current() { return TMC2209Stepper::rms_current(); }
    inline void rms_current(const uint16_t mA) {
      this->val_mA = mA;
      TMC2209Stepper::rms_current(mA);
    }
    inline void rms_current(const uint16_t mA, const float mult) {
      this->val_mA = mA;
      TMC2209Stepper::rms_current(mA, mult);
    }

    #if HAS_STEALTHCHOP
      inline void refresh_stepping_mode() { en_spreadCycle(!this->stored.stealthChop_enabled); }
      inline bool get_stealthChop_status() { return !this->en_spreadCycle(); }
    #endif
    #if ENABLED(HYBRID_THRESHOLD)
      uint32_t get_pwm_thrs() {
        return _tmc_thrs(this->microsteps(), this->TPWMTHRS(), planner.settings.axis_steps_per_mm[AXIS_ID]);
      }
      void set_pwm_thrs(const uint32_t thrs) {
        TMC2209Stepper::TPWMTHRS(_tmc_thrs(this->microsteps(), thrs, planner.settings.axis_steps_per_mm[AXIS_ID]));
        #if HAS_LCD_MENU
          this->stored.hybrid_thrs = thrs;
        #endif
      }
    #endif
    #if USE_SENSORLESS
      inline int16_t homing_threshold() { return TMC2209Stepper::SGTHRS(); }
      void homing_threshold(int16_t sgt_val) {
        sgt_val = (int16_t)constrain(sgt_val, sgt_min, sgt_max);
        TMC2209Stepper::SGTHRS(sgt_val);
        #if HAS_LCD_MENU
          this->stored.homing_thrs = sgt_val;
        #endif
      }
    #endif

    #if HAS_LCD_MENU
      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if ENABLED(HYBRID_THRESHOLD)
        inline void refresh_hybrid_thrs() { set_pwm_thrs(this->stored.hybrid_thrs); }
      #endif
      #if USE_SENSORLESS
        inline void refresh_homing_thrs() { homing_threshold(this->stored.homing_thrs); }
      #endif
    #endif

    static constexpr uint8_t sgt_min = 0,
                             sgt_max = 255;
};

template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
class TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> : public TMC2660Stepper, public TMCStorage<AXIS_LETTER, DRIVER_ID> {
  public:
    TMCMarlin(const uint16_t cs_pin, const float RS) :
      TMC2660Stepper(cs_pin, RS)
      {}
    TMCMarlin(const uint16_t CS, const float RS, const uint16_t pinMOSI, const uint16_t pinMISO, const uint16_t pinSCK) :
      TMC2660Stepper(CS, RS, pinMOSI, pinMISO, pinSCK)
      {}
    inline uint16_t rms_current() { return TMC2660Stepper::rms_current(); }
    inline void rms_current(const uint16_t mA) {
      this->val_mA = mA;
      TMC2660Stepper::rms_current(mA);
    }

    #if USE_SENSORLESS
      inline int16_t homing_threshold() { return TMC2660Stepper::sgt(); }
      void homing_threshold(int16_t sgt_val) {
        sgt_val = (int16_t)constrain(sgt_val, sgt_min, sgt_max);
        TMC2660Stepper::sgt(sgt_val);
        #if HAS_LCD_MENU
          this->stored.homing_thrs = sgt_val;
        #endif
      }
    #endif

    #if HAS_LCD_MENU
      inline void refresh_stepper_current() { rms_current(this->val_mA); }

      #if USE_SENSORLESS
        inline void refresh_homing_thrs() { homing_threshold(this->stored.homing_thrs); }
      #endif
    #endif

    static constexpr int8_t sgt_min = -64,
                            sgt_max =  63;
};

template<typename TMC>
void tmc_print_current(TMC &st) {
  st.printLabel();
  SERIAL_ECHOLNPAIR(" driver current: ", st.getMilliamps());
}

#if ENABLED(MONITOR_DRIVER_STATUS)
  template<typename TMC>
  void tmc_report_otpw(TMC &st) {
    st.printLabel();
    SERIAL_ECHOPGM(" temperature prewarn triggered: ");
    serialprint_truefalse(st.getOTPW());
    SERIAL_EOL();
  }
  template<typename TMC>
  void tmc_clear_otpw(TMC &st) {
    st.clear_otpw();
    st.printLabel();
    SERIAL_ECHOLNPGM(" prewarn flag cleared");
  }
#endif
#if ENABLED(HYBRID_THRESHOLD)
  template<typename TMC>
  void tmc_print_pwmthrs(TMC &st) {
    st.printLabel();
    SERIAL_ECHOLNPAIR(" stealthChop max speed: ", st.get_pwm_thrs());
  }
#endif
#if USE_SENSORLESS
  template<typename TMC>
  void tmc_print_sgt(TMC &st) {
    st.printLabel();
    SERIAL_ECHOPGM(" homing sensitivity: ");
    SERIAL_PRINTLN(st.homing_threshold(), DEC);
  }
#endif

void monitor_tmc_driver();
void test_tmc_connection(const bool test_x, const bool test_y, const bool test_z, const bool test_e);

#if ENABLED(TMC_DEBUG)
  #if ENABLED(MONITOR_DRIVER_STATUS)
    void tmc_set_report_interval(const uint16_t update_interval);
  #endif
  void tmc_report_all(const bool print_x, const bool print_y, const bool print_z, const bool print_e);
  void tmc_get_registers(const bool print_x, const bool print_y, const bool print_z, const bool print_e);
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

  bool tmc_enable_stallguard(TMC2209Stepper &st);
  void tmc_disable_stallguard(TMC2209Stepper &st, const bool restore_stealth);

  bool tmc_enable_stallguard(TMC2660Stepper);
  void tmc_disable_stallguard(TMC2660Stepper, const bool);
#endif

#if TMC_HAS_SPI
  void tmc_init_cs_pins();
#endif

#endif // HAS_TRINAMIC
