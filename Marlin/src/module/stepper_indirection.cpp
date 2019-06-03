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

/**
 * stepper_indirection.cpp
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Part of Marlin
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "stepper_indirection.h"

#include "../inc/MarlinConfig.h"

#include "stepper.h"

#if HAS_DRIVER(L6470)
  #include "L6470/L6470_Marlin.h"
#endif

//
// TMC26X Driver objects and inits
//
#if HAS_DRIVER(TMC26X)
  #include <SPI.h>

  #ifdef STM32F7
    #include "../HAL/HAL_STM32F7/TMC2660.h"
  #else
    #include <TMC26XStepper.h>
  #endif

  #define _TMC26X_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_CS_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

  #if AXIS_DRIVER_TYPE_X(TMC26X)
    _TMC26X_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(TMC26X)
    _TMC26X_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(TMC26X)
    _TMC26X_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(TMC26X)
    _TMC26X_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(TMC26X)
    _TMC26X_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(TMC26X)
    _TMC26X_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(TMC26X)
    _TMC26X_DEFINE(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_E0(TMC26X)
    _TMC26X_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(TMC26X)
    _TMC26X_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(TMC26X)
    _TMC26X_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(TMC26X)
    _TMC26X_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(TMC26X)
    _TMC26X_DEFINE(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(TMC26X)
    _TMC26X_DEFINE(E5);
  #endif

  #define _TMC26X_INIT(A) do{ \
    stepper##A.setMicrosteps(A##_MICROSTEPS); \
    stepper##A.start(); \
  }while(0)

  void tmc26x_init_to_defaults() {
    #if AXIS_DRIVER_TYPE_X(TMC26X)
      _TMC26X_INIT(X);
    #endif
    #if AXIS_DRIVER_TYPE_X2(TMC26X)
      _TMC26X_INIT(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y(TMC26X)
      _TMC26X_INIT(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(TMC26X)
      _TMC26X_INIT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z(TMC26X)
      _TMC26X_INIT(Z);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(TMC26X)
      _TMC26X_INIT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(TMC26X)
      _TMC26X_INIT(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(TMC26X)
      _TMC26X_INIT(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(TMC26X)
      _TMC26X_INIT(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(TMC26X)
      _TMC26X_INIT(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(TMC26X)
      _TMC26X_INIT(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(TMC26X)
      _TMC26X_INIT(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(TMC26X)
      _TMC26X_INIT(E5);
    #endif
  }
#endif // TMC26X

#if HAS_TRINAMIC
  #include <HardwareSerial.h>
  #include <SPI.h>
  #include "planner.h"
  #include "../core/enum.h"

  enum StealthIndex : uint8_t { STEALTH_AXIS_XY, STEALTH_AXIS_Z, STEALTH_AXIS_E };
  #define _TMC_INIT(ST, STEALTH_INDEX) tmc_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, stealthchop_by_axis[STEALTH_INDEX])

  //   IC = TMC model number
  //   ST = Stepper object letter
  //   L  = Label characters
  //   AI = Axis Enum Index
  // SWHW = SW/SH UART selection
  #if ENABLED(TMC_USE_SW_SPI)
    #define __TMC_SPI_DEFINE(IC, ST, L, AI) TMCMarlin<IC##Stepper, L, AI> stepper##ST(ST##_CS_PIN, ST##_RSENSE, TMC_SW_MOSI, TMC_SW_MISO, TMC_SW_SCK)
  #else
    #define __TMC_SPI_DEFINE(IC, ST, L, AI) TMCMarlin<IC##Stepper, L, AI> stepper##ST(ST##_CS_PIN, ST##_RSENSE)
  #endif

  #define TMC_UART_HW_DEFINE(IC, ST, L, AI) TMCMarlin<IC##Stepper, L, AI> stepper##ST(&ST##_HARDWARE_SERIAL, ST##_RSENSE)
  #define TMC_UART_SW_DEFINE(IC, ST, L, AI) TMCMarlin<IC##Stepper, L, AI> stepper##ST(ST##_SERIAL_RX_PIN, ST##_SERIAL_TX_PIN, ST##_RSENSE, ST##_SERIAL_RX_PIN > -1)

  #define _TMC_SPI_DEFINE(IC, ST, AI) __TMC_SPI_DEFINE(IC, ST, TMC_##ST##_LABEL, AI)
  #define TMC_SPI_DEFINE(ST, AI) _TMC_SPI_DEFINE(ST##_DRIVER_TYPE, ST, AI##_AXIS)

  #define _TMC_UART_DEFINE(SWHW, IC, ST, AI) TMC_UART_##SWHW##_DEFINE(IC, ST, TMC_##ST##_LABEL, AI)
  #define TMC_UART_DEFINE(SWHW, ST, AI) _TMC_UART_DEFINE(SWHW, ST##_DRIVER_TYPE, ST, AI##_AXIS)

  #if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
    #define TMC_SPI_DEFINE_E(AI) TMC_SPI_DEFINE(E##AI, E##AI)
    #define TMC_UART_DEFINE_E(SWHW, AI) TMC_UART_DEFINE(SWHW, E##AI, E##AI)
  #else
    #define TMC_SPI_DEFINE_E(AI) TMC_SPI_DEFINE(E##AI, E)
    #define TMC_UART_DEFINE_E(SWHW, AI) TMC_UART_DEFINE(SWHW, E##AI, E)
  #endif

  // Stepper objects of TMC2130/TMC2160/TMC2660/TMC5130/TMC5160 steppers used
  #if AXIS_HAS_SPI(X)
    TMC_SPI_DEFINE(X, X);
  #endif
  #if AXIS_HAS_SPI(X2)
    TMC_SPI_DEFINE(X2, X);
  #endif
  #if AXIS_HAS_SPI(Y)
    TMC_SPI_DEFINE(Y, Y);
  #endif
  #if AXIS_HAS_SPI(Y2)
    TMC_SPI_DEFINE(Y2, Y);
  #endif
  #if AXIS_HAS_SPI(Z)
    TMC_SPI_DEFINE(Z, Z);
  #endif
  #if AXIS_HAS_SPI(Z2)
    TMC_SPI_DEFINE(Z2, Z);
  #endif
  #if AXIS_HAS_SPI(Z3)
    TMC_SPI_DEFINE(Z3, Z);
  #endif
  #if AXIS_HAS_SPI(E0)
    TMC_SPI_DEFINE_E(0);
  #endif
  #if AXIS_HAS_SPI(E1)
    TMC_SPI_DEFINE_E(1);
  #endif
  #if AXIS_HAS_SPI(E2)
    TMC_SPI_DEFINE_E(2);
  #endif
  #if AXIS_HAS_SPI(E3)
    TMC_SPI_DEFINE_E(3);
  #endif
  #if AXIS_HAS_SPI(E4)
    TMC_SPI_DEFINE_E(4);
  #endif
  #if AXIS_HAS_SPI(E5)
    TMC_SPI_DEFINE_E(5);
  #endif

#endif

#if HAS_DRIVER(TMC2130)
  template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
  void tmc_init(TMCMarlin<TMC2130Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const bool stealth) {
    st.begin();

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    st.CHOPCONF(chopconf.sr);

    st.rms_current(mA, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.iholddelay(10);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current

    st.en_pwm_mode(stealth);
    st.stored.stealthChop_enabled = stealth;

    PWMCONF_t pwmconf{0};
    pwmconf.pwm_freq = 0b01; // f_pwm = 2/683 f_clk
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_grad = 5;
    pwmconf.pwm_ampl = 180;
    st.PWMCONF(pwmconf.sr);

    #if ENABLED(HYBRID_THRESHOLD)
      st.set_pwm_thrs(thrs);
    #else
      UNUSED(thrs);
    #endif

    st.GSTAT(); // Clear GSTAT
  }
#endif // TMC2130

#if HAS_DRIVER(TMC2160)
  template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
  void tmc_init(TMCMarlin<TMC2160Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const bool stealth) {
    st.begin();

    static constexpr int8_t timings[] = CHOPPER_TIMING; // Default 4, -2, 1

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = timings[0];
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = timings[1] + 3;
    chopconf.hstrt = timings[2] - 1;
    st.CHOPCONF(chopconf.sr);

    st.rms_current(mA, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.iholddelay(10);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current
    st.TCOOLTHRS(0xFFFFF);

    #if ENABLED(ADAPTIVE_CURRENT)
      COOLCONF_t coolconf{0};
      coolconf.semin = INCREASE_CURRENT_THRS;
      coolconf.semax = REDUCE_CURRENT_THRS;
      st.COOLCONF(coolconf.sr);
    #endif

    st.en_pwm_mode(stealth);

    PWMCONF_t pwmconf{0};
    pwmconf.pwm_freq = 0b01; // f_pwm = 2/683 f_clk
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_grad = 5;
    pwmconf.pwm_ampl = 180;
    st.PWMCONF(pwmconf.sr);

    #if ENABLED(HYBRID_THRESHOLD)
      st.set_pwm_thrs(thrs);
    #else
      UNUSED(thrs);
    #endif

    st.GSTAT(); // Clear GSTAT
  }
#endif // TMC2160

//
// TMC2208 Driver objects and inits
//
#if HAS_DRIVER(TMC2208)
  // Stepper objects of TMC2208 steppers used
  #if AXIS_DRIVER_TYPE_X(TMC2208)
    #ifdef X_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, X, X);
    #else
      TMC_UART_DEFINE(SW, X, X);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_X2(TMC2208)
    #ifdef X2_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, X2, X);
    #else
      TMC_UART_DEFINE(SW, X2, X);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_Y(TMC2208)
    #ifdef Y_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, Y, Y);
    #else
      TMC_UART_DEFINE(SW, Y, Y);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_Y2(TMC2208)
    #ifdef Y2_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, Y2, Y);
    #else
      TMC_UART_DEFINE(SW, Y2, Y);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_Z(TMC2208)
    #ifdef Z_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, Z, Z);
    #else
      TMC_UART_DEFINE(SW, Z, Z);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_Z2(TMC2208)
    #ifdef Z2_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, Z2, Z);
    #else
      TMC_UART_DEFINE(SW, Z2, Z);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_Z3(TMC2208)
    #ifdef Z3_HARDWARE_SERIAL
      TMC_UART_DEFINE(HW, Z3, Z);
    #else
      TMC_UART_DEFINE(SW, Z3, Z);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_E0(TMC2208)
    #ifdef E0_HARDWARE_SERIAL
      TMC_UART_DEFINE_E(HW, 0);
    #else
      TMC_UART_DEFINE_E(SW, 0);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_E1(TMC2208)
    #ifdef E1_HARDWARE_SERIAL
      TMC_UART_DEFINE_E(HW, 1);
    #else
      TMC_UART_DEFINE_E(SW, 1);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_E2(TMC2208)
    #ifdef E2_HARDWARE_SERIAL
      TMC_UART_DEFINE_E(HW, 2);
    #else
      TMC_UART_DEFINE_E(SW, 2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_E3(TMC2208)
    #ifdef E3_HARDWARE_SERIAL
      TMC_UART_DEFINE_E(HW, 3);
    #else
      TMC_UART_DEFINE_E(SW, 3);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_E4(TMC2208)
    #ifdef E4_HARDWARE_SERIAL
      TMC_UART_DEFINE_E(HW, 4);
    #else
      TMC_UART_DEFINE_E(SW, 4);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE_E5(TMC2208)
    #ifdef E5_HARDWARE_SERIAL
      TMC_UART_DEFINE_E(HW, 5);
    #else
      TMC_UART_DEFINE_E(SW, 5);
    #endif
  #endif

  void tmc2208_serial_begin() {
    #if AXIS_DRIVER_TYPE_X(TMC2208)
      #ifdef X_HARDWARE_SERIAL
        X_HARDWARE_SERIAL.begin(115200);
      #else
        stepperX.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_X2(TMC2208)
      #ifdef X2_HARDWARE_SERIAL
        X2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperX2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_Y(TMC2208)
      #ifdef Y_HARDWARE_SERIAL
        Y_HARDWARE_SERIAL.begin(115200);
      #else
        stepperY.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_Y2(TMC2208)
      #ifdef Y2_HARDWARE_SERIAL
        Y2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperY2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_Z(TMC2208)
      #ifdef Z_HARDWARE_SERIAL
        Z_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_Z2(TMC2208)
      #ifdef Z2_HARDWARE_SERIAL
        Z2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_Z3(TMC2208)
      #ifdef Z3_HARDWARE_SERIAL
        Z3_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ3.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_E0(TMC2208)
      #ifdef E0_HARDWARE_SERIAL
        E0_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE0.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_E1(TMC2208)
      #ifdef E1_HARDWARE_SERIAL
        E1_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE1.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_E2(TMC2208)
      #ifdef E2_HARDWARE_SERIAL
        E2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_E3(TMC2208)
      #ifdef E3_HARDWARE_SERIAL
        E3_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE3.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_E4(TMC2208)
      #ifdef E4_HARDWARE_SERIAL
        E4_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE4.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE_E5(TMC2208)
      #ifdef E5_HARDWARE_SERIAL
        E5_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE5.beginSerial(115200);
      #endif
    #endif
  }

  template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
  void tmc_init(TMCMarlin<TMC2208Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const bool stealth) {
    TMC2208_n::GCONF_t gconf{0};
    gconf.pdn_disable = true; // Use UART
    gconf.mstep_reg_select = true; // Select microsteps with UART
    gconf.i_scale_analog = false;
    gconf.en_spreadcycle = !stealth;
    st.GCONF(gconf.sr);
    st.stored.stealthChop_enabled = stealth;

    TMC2208_n::CHOPCONF_t chopconf{0};
    chopconf.tbl = 0b01; // blank_time = 24
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    st.CHOPCONF(chopconf.sr);

    st.rms_current(mA, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.iholddelay(10);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current

    TMC2208_n::PWMCONF_t pwmconf{0};
    pwmconf.pwm_lim = 12;
    pwmconf.pwm_reg = 8;
    pwmconf.pwm_autograd = true;
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_freq = 0b01;
    pwmconf.pwm_grad = 14;
    pwmconf.pwm_ofs = 36;
    st.PWMCONF(pwmconf.sr);

    #if ENABLED(HYBRID_THRESHOLD)
      st.set_pwm_thrs(thrs);
    #else
      UNUSED(thrs);
    #endif

    st.GSTAT(0b111); // Clear
    delay(200);
  }
#endif // TMC2208

#if HAS_DRIVER(TMC2660)
  template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
  void tmc_init(TMCMarlin<TMC2660Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const uint16_t mA, const uint16_t microsteps, const uint32_t, const bool) {
    st.begin();

    TMC2660_n::CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    st.CHOPCONF(chopconf.sr);

    st.rms_current(mA);
    st.microsteps(microsteps);
    st.intpol(INTERPOLATE);
    st.diss2g(true); // Disable short to ground protection. Too many false readings?

    #if ENABLED(TMC_DEBUG)
      st.rdsel(0b01);
    #endif
  }
#endif // TMC2660

#if HAS_DRIVER(TMC5130)
  template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
  void tmc_init(TMCMarlin<TMC5130Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const bool stealth) {
    st.begin();

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    st.CHOPCONF(chopconf.sr);

    st.rms_current(mA, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.iholddelay(10);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current

    st.en_pwm_mode(stealth);
    st.stored.stealthChop_enabled = stealth;

    PWMCONF_t pwmconf{0};
    pwmconf.pwm_freq = 0b01; // f_pwm = 2/683 f_clk
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_grad = 5;
    pwmconf.pwm_ampl = 180;
    st.PWMCONF(pwmconf.sr);

    #if ENABLED(HYBRID_THRESHOLD)
      st.set_pwm_thrs(thrs);
    #else
      UNUSED(thrs);
    #endif

    st.GSTAT(); // Clear GSTAT
  }
#endif // TMC5130

#if HAS_DRIVER(TMC5160)
  template<char AXIS_LETTER, char DRIVER_ID, AxisEnum AXIS_ID>
  void tmc_init(TMCMarlin<TMC5160Stepper, AXIS_LETTER, DRIVER_ID, AXIS_ID> &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const bool stealth) {
    st.begin();

    int8_t timings[] = CHOPPER_TIMING; // Default 4, -2, 1

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = timings[0];
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = timings[1] + 3;
    chopconf.hstrt = timings[2] - 1;
    st.CHOPCONF(chopconf.sr);

    st.rms_current(mA, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.iholddelay(10);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current

    #if ENABLED(ADAPTIVE_CURRENT)
      COOLCONF_t coolconf{0};
      coolconf.semin = INCREASE_CURRENT_THRS;
      coolconf.semax = REDUCE_CURRENT_THRS;
      st.COOLCONF(coolconf.sr);
    #endif

    st.en_pwm_mode(stealth);

    PWMCONF_t pwmconf{0};
    pwmconf.pwm_freq = 0b01; // f_pwm = 2/683 f_clk
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_grad = 5;
    pwmconf.pwm_ampl = 180;
    st.PWMCONF(pwmconf.sr);

    #if ENABLED(HYBRID_THRESHOLD)
      st.set_pwm_thrs(thrs);
    #else
      UNUSED(thrs);
    #endif
    st.GSTAT(); // Clear GSTAT
  }
#endif // TMC5160

void restore_stepper_drivers() {
  #if AXIS_IS_TMC(X)
    stepperX.push();
  #endif
  #if AXIS_IS_TMC(X2)
    stepperX2.push();
  #endif
  #if AXIS_IS_TMC(Y)
    stepperY.push();
  #endif
  #if AXIS_IS_TMC(Y2)
    stepperY2.push();
  #endif
  #if AXIS_IS_TMC(Z)
    stepperZ.push();
  #endif
  #if AXIS_IS_TMC(Z2)
    stepperZ2.push();
  #endif
  #if AXIS_IS_TMC(Z3)
    stepperZ3.push();
  #endif
  #if AXIS_IS_TMC(E0)
    stepperE0.push();
  #endif
  #if AXIS_IS_TMC(E1)
    stepperE1.push();
  #endif
  #if AXIS_IS_TMC(E2)
    stepperE2.push();
  #endif
  #if AXIS_IS_TMC(E3)
    stepperE3.push();
  #endif
  #if AXIS_IS_TMC(E4)
    stepperE4.push();
  #endif
  #if AXIS_IS_TMC(E5)
    stepperE5.push();
  #endif
}

void reset_stepper_drivers() {

  #if HAS_DRIVER(TMC26X)
    tmc26x_init_to_defaults();
  #endif

  #if HAS_DRIVER(L6470)
    L6470.init_to_defaults();
  #endif

  #if HAS_TRINAMIC
    static constexpr bool stealthchop_by_axis[] = {
      #if ENABLED(STEALTHCHOP_XY)
        true
      #else
        false
      #endif
      ,
      #if ENABLED(STEALTHCHOP_Z)
        true
      #else
        false
      #endif
      ,
      #if ENABLED(STEALTHCHOP_E)
        true
      #else
        false
      #endif
    };
  #endif

  #if AXIS_IS_TMC(X)
    _TMC_INIT(X, STEALTH_AXIS_XY);
  #endif
  #if AXIS_IS_TMC(X2)
    _TMC_INIT(X2, STEALTH_AXIS_XY);
  #endif
  #if AXIS_IS_TMC(Y)
    _TMC_INIT(Y, STEALTH_AXIS_XY);
  #endif
  #if AXIS_IS_TMC(Y2)
    _TMC_INIT(Y2, STEALTH_AXIS_XY);
  #endif
  #if AXIS_IS_TMC(Z)
    _TMC_INIT(Z, STEALTH_AXIS_Z);
  #endif
  #if AXIS_IS_TMC(Z2)
    _TMC_INIT(Z2, STEALTH_AXIS_Z);
  #endif
  #if AXIS_IS_TMC(Z3)
    _TMC_INIT(Z3, STEALTH_AXIS_Z);
  #endif
  #if AXIS_IS_TMC(E0)
    _TMC_INIT(E0, STEALTH_AXIS_E);
  #endif
  #if AXIS_IS_TMC(E1)
    _TMC_INIT(E1, STEALTH_AXIS_E);
  #endif
  #if AXIS_IS_TMC(E2)
    _TMC_INIT(E2, STEALTH_AXIS_E);
  #endif
  #if AXIS_IS_TMC(E3)
    _TMC_INIT(E3, STEALTH_AXIS_E);
  #endif
  #if AXIS_IS_TMC(E4)
    _TMC_INIT(E4, STEALTH_AXIS_E);
  #endif
  #if AXIS_IS_TMC(E5)
    _TMC_INIT(E5, STEALTH_AXIS_E);
  #endif

  #if USE_SENSORLESS
    #if X_SENSORLESS
      #if AXIS_HAS_STALLGUARD(X)
        stepperX.sgt(X_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(X2)
        stepperX2.sgt(X_STALL_SENSITIVITY);
      #endif
    #endif
    #if Y_SENSORLESS
      #if AXIS_HAS_STALLGUARD(Y)
        stepperY.sgt(Y_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(Y2)
        stepperY2.sgt(Y_STALL_SENSITIVITY);
      #endif
    #endif
    #if Z_SENSORLESS
      #if AXIS_HAS_STALLGUARD(Z)
        stepperZ.sgt(Z_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(Z2)
        stepperZ2.sgt(Z_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(Z3)
        stepperZ3.sgt(Z_STALL_SENSITIVITY);
      #endif
    #endif
  #endif

  #ifdef TMC_ADV
    TMC_ADV()
  #endif

  #if HAS_TRINAMIC
    stepper.set_directions();
  #endif
}

//
// L6470 Driver objects and inits
//
#if HAS_DRIVER(L6470)

  // create stepper objects

  #define _L6470_DEFINE(ST) L6470 stepper##ST((const int)L6470_CHAIN_SS_PIN)

  // L6470 Stepper objects
  #if AXIS_DRIVER_TYPE_X(L6470)
    _L6470_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(L6470)
    _L6470_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(L6470)
    _L6470_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(L6470)
    _L6470_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(L6470)
    _L6470_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(L6470)
    _L6470_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(L6470)
    _L6470_DEFINE(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_E0(L6470)
    _L6470_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(L6470)
    _L6470_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(L6470)
    _L6470_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(L6470)
    _L6470_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(L6470)
    _L6470_DEFINE(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(L6470)
    _L6470_DEFINE(E5);
  #endif

  // not using L6470 library's init command because it
  // briefly sends power to the steppers

  #define _L6470_INIT_CHIP(Q) do{                             \
    stepper##Q.resetDev();                                    \
    stepper##Q.softFree();                                    \
    stepper##Q.SetParam(L6470_CONFIG, CONFIG_PWM_DIV_1        \
                                    | CONFIG_PWM_MUL_2        \
                                    | CONFIG_SR_290V_us       \
                                    | CONFIG_OC_SD_DISABLE    \
                                    | CONFIG_VS_COMP_DISABLE  \
                                    | CONFIG_SW_HARD_STOP     \
                                    | CONFIG_INT_16MHZ);      \
    stepper##Q.SetParam(L6470_KVAL_RUN, 0xFF);                \
    stepper##Q.SetParam(L6470_KVAL_ACC, 0xFF);                \
    stepper##Q.SetParam(L6470_KVAL_DEC, 0xFF);                \
    stepper##Q.setMicroSteps(Q##_MICROSTEPS);                 \
    stepper##Q.setOverCurrent(Q##_OVERCURRENT);               \
    stepper##Q.setStallCurrent(Q##_STALLCURRENT);             \
    stepper##Q.SetParam(L6470_KVAL_HOLD, Q##_MAX_VOLTAGE);    \
    stepper##Q.SetParam(L6470_ABS_POS, 0);                    \
    stepper##Q.getStatus();                                   \
  }while(0)

  void L6470_Marlin::init_to_defaults() {
    #if AXIS_DRIVER_TYPE_X(L6470)
      _L6470_INIT_CHIP(X);
    #endif
    #if AXIS_DRIVER_TYPE_X2(L6470)
      _L6470_INIT_CHIP(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y(L6470)
      _L6470_INIT_CHIP(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(L6470)
      _L6470_INIT_CHIP(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z(L6470)
      _L6470_INIT_CHIP(Z);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(L6470)
      _L6470_INIT_CHIP(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(L6470)
      _L6470_INIT_CHIP(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(L6470)
      _L6470_INIT_CHIP(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(L6470)
      _L6470_INIT_CHIP(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(L6470)
      _L6470_INIT_CHIP(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(L6470)
      _L6470_INIT_CHIP(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(L6470)
      _L6470_INIT_CHIP(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(L6470)
      _L6470_INIT_CHIP(E5);
    #endif
  }

#endif // L6470
