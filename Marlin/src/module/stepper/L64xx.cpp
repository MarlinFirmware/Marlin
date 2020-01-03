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


#include "../../inc/MarlinConfig.h"


#if HAS_L64XX
  #include "../../libs/L6470/L6470_Marlin.h"
#endif


//
// L6470 Driver objects and inits
//
#if HAS_L64XX

  #if AXIS_IS_L64XX(X)
    X_DRIVER_TYPE stepperX(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(X2)
    X2_DRIVER_TYPE stepperX2(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(Y)
    Y_DRIVER_TYPE stepperY(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(Y2)
    Y2_DRIVER_TYPE stepperY2(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(Z)
    Z_DRIVER_TYPE stepperZ(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(Z2)
    Z2_DRIVER_TYPE stepperZ2(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(Z3)
    Z3_DRIVER_TYPE stepperZ3(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(E0)
    E0_DRIVER_TYPE stepperE0(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(E1)
    E1_DRIVER_TYPE stepperE1(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(E2)
    E2_DRIVER_TYPE stepperE2(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(E3)
    E3_DRIVER_TYPE stepperE3(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(E4)
    E4_DRIVER_TYPE stepperE4(L6470_CHAIN_SS_PIN);
  #endif
  #if AXIS_IS_L64XX(E5)
    E5_DRIVER_TYPE stepperE5(L6470_CHAIN_SS_PIN);
  #endif

  // Not using L64XX class init method because it
  // briefly sends power to the steppers

  inline void L6470_init_chip(L64XX &st, const int ms, const int oc, const int sc, const int mv, const int slew_rate) {
    st.set_handlers(L64xx_MARLIN.spi_init, L64xx_MARLIN.transfer_single, L64xx_MARLIN.transfer_chain);  // specify which external SPI routines to use
    switch( st.L6470_status_layout) {
      case L6470_STATUS_LAYOUT: {
        st.resetDev();
        st.softFree();
        st.SetParam(st.L64XX_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_OC_SD_DISABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ);
        st.SetParam(L6470_KVAL_RUN, 0xFF);
        st.SetParam(L6470_KVAL_ACC, 0xFF);
        st.SetParam(L6470_KVAL_DEC, 0xFF);
        st.setMicroSteps(ms);
        st.setOverCurrent(oc);
        st.setStallCurrent(sc);
        st.SetParam(L6470_KVAL_HOLD, mv);
        st.SetParam(L6470_ABS_POS, 0);
        uint32_t config_temp = st.GetParam(st.L64XX_CONFIG);
        config_temp &= ~CONFIG_POW_SR;
        switch (slew_rate) {
          case 0: st.SetParam(st.L64XX_CONFIG, config_temp | CONFIG_SR_75V_us); break;
          default:
          case 1: st.SetParam(st.L64XX_CONFIG, config_temp | CONFIG_SR_110V_us); break;
          case 3:
          case 2: st.SetParam(st.L64XX_CONFIG, config_temp | CONFIG_SR_260V_us); break;
        }
        st.getStatus();
        st.getStatus();
        break;
      }

      case L6474_STATUS_LAYOUT: {
        st.free();
//        st.SetParam(st.L64XX_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_OC_SD_DISABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ);
//        st.SetParam(L6474_TVAL, 0xFF);
        st.setMicroSteps(ms);
        st.setOverCurrent(oc);
        st.setTVALCurrent(sc);
        st.SetParam(L6470_ABS_POS, 0);
        uint32_t config_temp = st.GetParam(st.L64XX_CONFIG);
        config_temp &= ~CONFIG_POW_SR & ~CONFIG_EN_TQREG;  // clear out slew rate and set current to be controlled by TVAL register
        switch (slew_rate) {
          case 0: st.SetParam(st.L64XX_CONFIG, config_temp | CONFIG_SR_75V_us); break;
          default:
          case 1: st.SetParam(st.L64XX_CONFIG, config_temp | CONFIG_SR_110V_us); break;
          case 3:
          case 2: st.SetParam(st.L64XX_CONFIG, config_temp | CONFIG_SR_260V_us); break;
//case 0: st.SetParam(st.L64XX_CONFIG, 0x2E88 | CONFIG_EN_TQREG | CONFIG_SR_75V_us); break;
//default:
//case 1: st.SetParam(st.L64XX_CONFIG, 0x2E88 | CONFIG_EN_TQREG | CONFIG_SR_110V_us); break;
//case 3:
//case 2: st.SetParam(st.L64XX_CONFIG, 0x2E88 | CONFIG_EN_TQREG | CONFIG_SR_260V_us); break;


//case 0: st.SetParam(st.L64XX_CONFIG, 0x2E88 ); break;
//default:
//case 1: st.SetParam(st.L64XX_CONFIG, 0x2E88 ); break;
//case 3:
//case 2: st.SetParam(st.L64XX_CONFIG, 0x2E88 ); break;
      }
        st.getStatus();
        st.getStatus();
        break;
      }

      case L6480_STATUS_LAYOUT: {
        st.resetDev();
        st.softFree();
        st.SetParam(st.L64XX_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_OC_SD_DISABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ);
        st.SetParam(L6470_KVAL_RUN, 0xFF);
        st.SetParam(L6470_KVAL_ACC, 0xFF);
        st.SetParam(L6470_KVAL_DEC, 0xFF);
        st.setMicroSteps(ms);
        st.setOverCurrent(oc);
        st.setStallCurrent(sc);
        st.SetParam(+-L6470_KVAL_HOLD, mv);
        st.SetParam(L6470_ABS_POS, 0);
        st.SetParam(st.L64XX_CONFIG,(st.GetParam(st.L64XX_CONFIG) | PWR_VCC_7_5V));
        st.getStatus();     // must clear out status bits before can set slew rate
        st.getStatus();
        switch (slew_rate) {
          case 0: st.SetParam(L6470_GATECFG1, CONFIG1_SR_220V_us); st.SetParam(L6470_GATECFG2, CONFIG2_SR_220V_us); break;
          default:
          case 1: st.SetParam(L6470_GATECFG1, CONFIG1_SR_400V_us); st.SetParam(L6470_GATECFG2, CONFIG2_SR_400V_us); break;
          case 2: st.SetParam(L6470_GATECFG1, CONFIG1_SR_520V_us); st.SetParam(L6470_GATECFG2, CONFIG2_SR_520V_us); break;
          case 3: st.SetParam(L6470_GATECFG1, CONFIG1_SR_980V_us); st.SetParam(L6470_GATECFG2, CONFIG2_SR_980V_us); break;
        }
        break;
      }
    }
  }

  #define L6470_INIT_CHIP(Q) L6470_init_chip(stepper##Q, Q##_MICROSTEPS, Q##_OVERCURRENT, Q##_STALLCURRENT, Q##_MAX_VOLTAGE, Q##_SLEW_RATE)

  void L64XX_Marlin::init_to_defaults() {
    #if AXIS_IS_L64XX(X)
      L6470_INIT_CHIP(X);
    #endif
    #if AXIS_IS_L64XX(X2)
      L6470_INIT_CHIP(X2);
    #endif
    #if AXIS_IS_L64XX(Y)
      L6470_INIT_CHIP(Y);
    #endif
    #if AXIS_IS_L64XX(Y2)
      L6470_INIT_CHIP(Y2);
    #endif
    #if AXIS_IS_L64XX(Z)
      L6470_INIT_CHIP(Z);
    #endif
    #if AXIS_IS_L64XX(Z2)
      L6470_INIT_CHIP(Z2);
    #endif
    #if AXIS_IS_L64XX(Z3)
      L6470_INIT_CHIP(Z3);
    #endif
    #if AXIS_IS_L64XX(E0)
      L6470_INIT_CHIP(E0);
    #endif
    #if AXIS_IS_L64XX(E1)
      L6470_INIT_CHIP(E1);
    #endif
    #if AXIS_IS_L64XX(E2)
      L6470_INIT_CHIP(E2);
    #endif
    #if AXIS_IS_L64XX(E3)
      L6470_INIT_CHIP(E3);
    #endif
    #if AXIS_IS_L64XX(E4)
      L6470_INIT_CHIP(E4);
    #endif
    #if AXIS_IS_L64XX(E5)
      L6470_INIT_CHIP(E5);
    #endif
  }

#endif // HAS_L64XX
