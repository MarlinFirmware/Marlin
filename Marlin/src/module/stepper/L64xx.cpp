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

/**
 * stepper/L64xx.cpp
 * Stepper driver indirection for L64XX drivers
 */

#include "../../inc/MarlinConfig.h"

#if HAS_L64XX

#include "L64xx.h"

#if AXIS_IS_L64XX(X)
  L64XX_CLASS(X) stepperX(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(X2)
  L64XX_CLASS(X2) stepperX2(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(Y)
  L64XX_CLASS(Y) stepperY(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(Y2)
  L64XX_CLASS(Y2) stepperY2(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(Z)
  L64XX_CLASS(Z) stepperZ(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(Z2)
  L64XX_CLASS(Z2) stepperZ2(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(Z3)
  L64XX_CLASS(Z3) stepperZ3(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(Z4)
  L64XX_CLASS(Z4) stepperZ4(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(I)
  L64XX_CLASS(I) stepperI(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(J)
  L64XX_CLASS(J) stepperJ(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(K)
  L64XX_CLASS(K) stepperK(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(U)
  L64XX_CLASS(u) stepperU(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(V)
  L64XX_CLASS(v) stepperV(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(W)
  L64XX_CLASS(w) stepperW(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E0)
  L64XX_CLASS(E0) stepperE0(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E1)
  L64XX_CLASS(E1) stepperE1(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E2)
  L64XX_CLASS(E2) stepperE2(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E3)
  L64XX_CLASS(E3) stepperE3(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E4)
  L64XX_CLASS(E4) stepperE4(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E5)
  L64XX_CLASS(E5) stepperE5(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E6)
  L64XX_CLASS(E6) stepperE6(L6470_CHAIN_SS_PIN);
#endif
#if AXIS_IS_L64XX(E7)
  L64XX_CLASS(E7) stepperE7(L6470_CHAIN_SS_PIN);
#endif

// Not using L64XX class init method because it
// briefly sends power to the steppers

inline void L6470_init_chip(L64XX &st, const int ms, const int oc, const int sc, const int mv, const int slew_rate) {
  st.set_handlers(L64xxManager.spi_init, L64xxManager.transfer_single, L64xxManager.transfer_chain);  // specify which external SPI routines to use
  switch (st.L6470_status_layout) {
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
      //st.SetParam(st.L64XX_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_OC_SD_DISABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ);
      //st.SetParam(L6474_TVAL, 0xFF);
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
  #if AXIS_IS_L64XX(Z4)
    L6470_INIT_CHIP(Z4);
  #endif
  #if AXIS_IS_L64XX(I)
    L6470_INIT_CHIP(I);
  #endif
  #if AXIS_IS_L64XX(J)
    L6470_INIT_CHIP(J);
  #endif
  #if AXIS_IS_L64XX(K)
    L6470_INIT_CHIP(K);
  #endif
  #if AXIS_IS_L64XX(U)
    L6470_INIT_CHIP(U);
  #endif
  #if AXIS_IS_L64XX(V)
    L6470_INIT_CHIP(V);
  #endif
  #if AXIS_IS_L64XX(W)
    L6470_INIT_CHIP(W);
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
  #if AXIS_IS_L64XX(E6)
    L6470_INIT_CHIP(E6);
  #endif
  #if AXIS_IS_L64XX(E7)
    L6470_INIT_CHIP(E7);
  #endif
}

#endif // HAS_L64XX
