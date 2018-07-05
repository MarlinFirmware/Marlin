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

#include "../inc/MarlinConfig.h"
#if HAS_TRINAMIC
  #include <TMCStepper.h>
#endif

extern bool report_tmc_status;

enum TMC_AxisEnum : char {
  TMC_X, TMC_Y, TMC_Z
  #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(X_DUAL_STEPPER_DRIVERS)
    , TMC_X2
  #endif
  #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
    , TMC_Y2
  #endif
  #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
    , TMC_Z2
  #endif
  #if ENABLED(Z_TRIPLE_STEPPER_DRIVERS)
    , TMC_Z3
  #endif
  #if E_STEPPERS
    , TMC_E0
    #if E_STEPPERS > 1
      , TMC_E1
      #if E_STEPPERS > 2
        , TMC_E2
        #if E_STEPPERS > 3
          , TMC_E3
          #if E_STEPPERS > 4
            , TMC_E4
            #if E_STEPPERS > 5
              , TMC_E5
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #endif // E_STEPPERS > 1
  #endif // E_STEPPERS
};

class TMCStorage {
  protected:
    virtual void abstract() = 0;
    uint16_t val_mA = 0;
    const char* label;
  public:
    TMCStorage(const char* tmc_label) : label(tmc_label) {}

    #if ENABLED(MONITOR_DRIVER_STATUS)
      uint8_t otpw_count = 0;
    #endif

    bool flag_otpw = false;
    bool getOTPW() { return flag_otpw; }
    void clear_otpw() { flag_otpw = 0; }
    uint16_t getMilliamps() { return val_mA; }
    void printLabel() { serialprintPGM(label); }
};

template <class TMC>
class TMCMarlin : public TMC, public TMCStorage {
  void abstract() override {};
  public:
    TMCMarlin(const char* tmc_label, uint16_t cs_pin, float RS) :
      TMC(cs_pin, RS),
      TMCStorage(tmc_label)
      {}
    TMCMarlin(const char* tmc_label, uint16_t CS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK) :
      TMC(CS, RS, pinMOSI, pinMISO, pinSCK),
      TMCStorage(tmc_label)
      {}
    uint16_t rms_current() { return TMC::rms_current(); }
    void rms_current(uint16_t mA) {
      this->val_mA = mA;
      TMC::rms_current(mA);
    }
    void rms_current(uint16_t mA, float mult) {
      this->val_mA = mA;
      TMC::rms_current(mA, mult);
    }
};
template<>
class TMCMarlin<TMC2208Stepper> : public TMC2208Stepper, public TMCStorage {
  void abstract() override {};
  public:
    TMCMarlin(const char* tmc_label, Stream * SerialPort, float RS, bool has_rx=true) :
      TMC2208Stepper(SerialPort, RS, has_rx=true),
      TMCStorage(tmc_label)
      {}
    TMCMarlin(const char* tmc_label, uint16_t RX, uint16_t TX, float RS, bool has_rx=true) :
      TMC2208Stepper(RX, TX, RS, has_rx=true),
      TMCStorage(tmc_label)
      {}
    uint16_t rms_current() { return TMC2208Stepper::rms_current(); }
    void rms_current(uint16_t mA) {
      this->val_mA = mA;
      TMC2208Stepper::rms_current(mA);
    }
    void rms_current(uint16_t mA, float mult) {
      this->val_mA = mA;
      TMC2208Stepper::rms_current(mA, mult);
    }
};

constexpr uint32_t _tmc_thrs(const uint16_t msteps, const int32_t thrs, const uint32_t spmm) {
  return 12650000UL * msteps / (256 * thrs * spmm);
}

template<typename TMC>
void tmc_get_current(TMC &st) {
  st.printLabel();
  SERIAL_ECHOLNPAIR(" driver current: ", st.getMilliamps());
}
template<typename TMC>
void tmc_set_current(TMC &st, const int mA) {
  st.rms_current(mA);
}
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
