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

#include "../../../inc/MarlinConfig.h"

#if HAS_L64XX

#include "../../gcode.h"
#include "../../../libs/L64XX/L64XX_Marlin.h"
#include "../../../module/stepper/indirection.h"
#include "../../../module/planner.h"

#define DEBUG_OUT ENABLED(L6470_CHITCHAT)
#include "../../../core/debug_out.h"

/**
 * MACRO to fetch information on the items associated with current limiting
 * and maximum voltage output.
 *
 * L6470 can be setup to shutdown if either current threshold is exceeded.
 *
 * L6470 output current can not be set directly.  It is set indirectly by
 * setting the maximum effective output voltage.
 *
 *  Effective output voltage is set by PWM duty cycle.
 *
 *  Maximum effective output voltage is affected by MANY variables.  The main ones are:
 *    KVAL_HOLD
 *    KVAL_RUN
 *    KVAL_ACC
 *    KVAL_DEC
 *    Vs compensation (if enabled)
 */
void L64XX_report_current(L64XX &motor, const L64XX_axis_t axis) {

  if (L64xxManager.spi_abort) return;  // don't do anything if set_directions() has occurred

  const L64XX_Marlin::L64XX_shadow_t &sh = L64xxManager.shadow;
  const uint16_t status = L64xxManager.get_status(axis);    //also populates shadow structure
  const uint8_t OverCurrent_Threshold = uint8_t(motor.GetParam(L6470_OCD_TH));

  auto say_axis_status = [](const L64XX_axis_t axis, const uint16_t status) {
    L64xxManager.say_axis(axis);
    #if ENABLED(L6470_CHITCHAT)
      char tmp[10];
      sprintf_P(tmp, PSTR("%4x   "), status);
      DEBUG_ECHOPGM("   status: ", tmp);
      print_bin(status);
    #else
      UNUSED(status);
    #endif
    SERIAL_EOL();
  };

  char temp_buf[10];

  switch (sh.STATUS_AXIS_LAYOUT) {
    case L6470_STATUS_LAYOUT:       // L6470
    case L6480_STATUS_LAYOUT: {     // L6480 & powerstep01
      const uint16_t Stall_Threshold = (uint8_t)motor.GetParam(L6470_STALL_TH),
                     motor_status = (status & (STATUS_MOT_STATUS)) >> 5,
                     L6470_ADC_out = motor.GetParam(L6470_ADC_OUT),
                     L6470_ADC_out_limited = constrain(L6470_ADC_out, 8, 24);
      const float comp_coef = 1600.0f / L6470_ADC_out_limited;
      const uint16_t MicroSteps = _BV(motor.GetParam(L6470_STEP_MODE) & 0x07);

      say_axis_status(axis, sh.STATUS_AXIS_RAW);

      SERIAL_ECHOPGM("...OverCurrent Threshold: ");
      sprintf_P(temp_buf, PSTR("%2d ("), OverCurrent_Threshold);
      SERIAL_ECHO(temp_buf);
      SERIAL_ECHO((OverCurrent_Threshold + 1) * motor.OCD_CURRENT_CONSTANT_INV);
      SERIAL_ECHOPGM(" mA)");
      SERIAL_ECHOPGM("   Stall Threshold: ");
      sprintf_P(temp_buf, PSTR("%2d ("), Stall_Threshold);
      SERIAL_ECHO(temp_buf);
      SERIAL_ECHO((Stall_Threshold + 1) * motor.STALL_CURRENT_CONSTANT_INV);
      SERIAL_ECHOPGM(" mA)");
      SERIAL_ECHOPGM("   Motor Status: ");
      switch (motor_status) {
        case 0: SERIAL_ECHOPGM("stopped"); break;
        case 1: SERIAL_ECHOPGM("accelerating"); break;
        case 2: SERIAL_ECHOPGM("decelerating"); break;
        case 3: SERIAL_ECHOPGM("at constant speed"); break;
      }
      SERIAL_EOL();

      SERIAL_ECHOPGM("...MicroSteps: ", MicroSteps,
                      "   ADC_OUT: ", L6470_ADC_out);
      SERIAL_ECHOPGM("   Vs_compensation: ");
      SERIAL_ECHOF((motor.GetParam(sh.L6470_AXIS_CONFIG) & CONFIG_EN_VSCOMP) ? F("ENABLED ") : F("DISABLED"));
      SERIAL_ECHOLNPGM("   Compensation coefficient: ~", comp_coef * 0.01f);

      SERIAL_ECHOPGM("...KVAL_HOLD: ", motor.GetParam(L6470_KVAL_HOLD),
                      "   KVAL_RUN : ", motor.GetParam(L6470_KVAL_RUN),
                      "   KVAL_ACC: ", motor.GetParam(L6470_KVAL_ACC),
                      "   KVAL_DEC: ", motor.GetParam(L6470_KVAL_DEC),
                      "   V motor max = ");
      switch (motor_status) {
        case 0: SERIAL_ECHO(motor.GetParam(L6470_KVAL_HOLD) * 100 / 256); SERIAL_ECHOPGM("% (KVAL_HOLD)"); break;
        case 1: SERIAL_ECHO(motor.GetParam(L6470_KVAL_RUN)  * 100 / 256); SERIAL_ECHOPGM("% (KVAL_RUN)"); break;
        case 2: SERIAL_ECHO(motor.GetParam(L6470_KVAL_ACC)  * 100 / 256); SERIAL_ECHOPGM("% (KVAL_ACC)"); break;
        case 3: SERIAL_ECHO(motor.GetParam(L6470_KVAL_DEC)  * 100 / 256); SERIAL_ECHOPGM("% (KVAL_HOLD)"); break;
      }
      SERIAL_EOL();

      #if ENABLED(L6470_CHITCHAT)
        DEBUG_ECHOPGM("...SLEW RATE: ");
        switch (sh.STATUS_AXIS_LAYOUT) {
          case L6470_STATUS_LAYOUT: {
            switch ((motor.GetParam(sh.L6470_AXIS_CONFIG) & CONFIG_POW_SR) >> CONFIG_POW_SR_BIT) {
              case 0: { DEBUG_ECHOLNPGM("320V/uS") ; break; }
              case 1: { DEBUG_ECHOLNPGM("75V/uS")  ; break; }
              case 2: { DEBUG_ECHOLNPGM("110V/uS") ; break; }
              case 3: { DEBUG_ECHOLNPGM("260V/uS") ; break; }
            }
            break;
          }
        case L6480_STATUS_LAYOUT: {
            switch (motor.GetParam(L6470_GATECFG1) & CONFIG1_SR ) {
              case CONFIG1_SR_220V_us: { DEBUG_ECHOLNPGM("220V/uS") ; break; }
              case CONFIG1_SR_400V_us: { DEBUG_ECHOLNPGM("400V/uS") ; break; }
              case CONFIG1_SR_520V_us: { DEBUG_ECHOLNPGM("520V/uS") ; break; }
              case CONFIG1_SR_980V_us: { DEBUG_ECHOLNPGM("980V/uS") ; break; }
              default: { DEBUG_ECHOLNPGM("unknown") ; break; }
            }
          }
        }
      #endif
      SERIAL_EOL();
      break;
    }

    case L6474_STATUS_LAYOUT: {  // L6474
      const uint16_t L6470_ADC_out = motor.GetParam(L6470_ADC_OUT) & 0x1F,
                     L6474_TVAL_val = motor.GetParam(L6474_TVAL) & 0x7F;

      say_axis_status(axis, sh.STATUS_AXIS_RAW);

      SERIAL_ECHOPGM("...OverCurrent Threshold: ");
      sprintf_P(temp_buf, PSTR("%2d ("), OverCurrent_Threshold);
      SERIAL_ECHO(temp_buf);
      SERIAL_ECHO((OverCurrent_Threshold + 1) * motor.OCD_CURRENT_CONSTANT_INV);
      SERIAL_ECHOPGM(" mA)");
      SERIAL_ECHOPGM("   TVAL: ");
      sprintf_P(temp_buf, PSTR("%2d ("), L6474_TVAL_val);
      SERIAL_ECHO(temp_buf);
      SERIAL_ECHO((L6474_TVAL_val + 1) * motor.STALL_CURRENT_CONSTANT_INV);
      SERIAL_ECHOLNPGM(" mA)   Motor Status: NA");

      const uint16_t MicroSteps = _BV(motor.GetParam(L6470_STEP_MODE) & 0x07); //NOMORE(MicroSteps, 16);
      SERIAL_ECHOPGM("...MicroSteps: ", MicroSteps,
                      "   ADC_OUT: ", L6470_ADC_out);

      SERIAL_ECHOLNPGM("   Vs_compensation: NA\n");
      SERIAL_ECHOLNPGM("...KVAL_HOLD: NA"
                       "   KVAL_RUN : NA"
                       "   KVAL_ACC: NA"
                       "   KVAL_DEC: NA"
                       "   V motor max =  NA");

      #if ENABLED(L6470_CHITCHAT)
        DEBUG_ECHOPGM("...SLEW RATE: ");
        switch ((motor.GetParam(sh.L6470_AXIS_CONFIG) & CONFIG_POW_SR) >> CONFIG_POW_SR_BIT) {
          case 0:  DEBUG_ECHOLNPGM("320V/uS") ; break;
          case 1:  DEBUG_ECHOLNPGM("75V/uS")  ; break;
          case 2:  DEBUG_ECHOLNPGM("110V/uS") ; break;
          case 3:  DEBUG_ECHOLNPGM("260V/uS") ; break;
          default: DEBUG_ECHOLNPGM("slew rate: ", (motor.GetParam(sh.L6470_AXIS_CONFIG) & CONFIG_POW_SR) >> CONFIG_POW_SR_BIT); break;
        }
      #endif
      SERIAL_EOL();
      SERIAL_EOL();
      break;
    }
  }
}

/**
 * M906: report or set KVAL_HOLD which sets the maximum effective voltage provided by the
 *       PWMs to the steppers
 *
 * On L6474 this sets the TVAL register (same address).
 *
 * I - select which driver(s) to change on multi-driver axis
 *     0 - (default) all drivers on the axis or E0
 *     1 - monitor only X, Y, Z or E1
 *     2 - monitor only X2, Y2, Z2 or E2
 *     3 - monitor only Z3 or E3
 *     4 - monitor only Z4 or E4
 *     5 - monitor only E5
 * Xxxx, Yxxx, Zxxx, Exxx - axis to change (optional)
 *     L6474 - current in mA (4A max)
 *     All others - 0-255
 *
 * Sets KVAL_HOLD which affects the current being driven through the stepper.
 *
 * L6470 is used in the STEP-CLOCK mode.  KVAL_HOLD is the only KVAL_xxx
 * that affects the effective voltage seen by the stepper.
 */
void GcodeSuite::M906() {

  L64xxManager.pause_monitor(true); // Keep monitor_driver() from stealing status

  #define L6470_SET_KVAL_HOLD(Q) (AXIS_IS_L64XX(Q) ? stepper##Q.setTVALCurrent(value) : stepper##Q.SetParam(L6470_KVAL_HOLD, uint8_t(value)))

  DEBUG_ECHOLNPGM("M906");

  uint8_t report_current = true;

  #if HAS_L64XX
    const uint8_t index = parser.byteval('I');
  #endif

  LOOP_LOGICAL_AXES(i) if (uint16_t value = parser.intval(axis_codes[i])) {

    report_current = false;

    if (planner.has_blocks_queued() || planner.cleaning_buffer_counter) {
      SERIAL_ECHOLNPGM("Test aborted. Can't set KVAL_HOLD while steppers are moving.");
      return;
    }

    switch (i) {
      case X_AXIS:
        #if AXIS_IS_L64XX(X)
          if (index == 0) L6470_SET_KVAL_HOLD(X);
        #endif
        #if AXIS_IS_L64XX(X2)
          if (index == 1) L6470_SET_KVAL_HOLD(X2);
        #endif
        break;

      #if HAS_Y_AXIS
        case Y_AXIS:
          #if AXIS_IS_L64XX(Y)
            if (index == 0) L6470_SET_KVAL_HOLD(Y);
          #endif
          #if AXIS_IS_L64XX(Y2)
            if (index == 1) L6470_SET_KVAL_HOLD(Y2);
          #endif
          break;
      #endif

      #if HAS_Z_AXIS
        case Z_AXIS:
          #if AXIS_IS_L64XX(Z)
            if (index == 0) L6470_SET_KVAL_HOLD(Z);
          #endif
          #if AXIS_IS_L64XX(Z2)
            if (index == 1) L6470_SET_KVAL_HOLD(Z2);
          #endif
          #if AXIS_IS_L64XX(Z3)
            if (index == 2) L6470_SET_KVAL_HOLD(Z3);
          #endif
          #if AXIS_DRIVER_TYPE_Z4(L6470)
            if (index == 3) L6470_SET_KVAL_HOLD(Z4);
          #endif
          break;
      #endif

      #if E_STEPPERS
        case E_AXIS: {
          const int8_t target_e_stepper = get_target_e_stepper_from_command();
          if (target_e_stepper < 0) return;
          switch (target_e_stepper) {
            #if AXIS_IS_L64XX(E0)
              case 0: L6470_SET_KVAL_HOLD(E0); break;
            #endif
            #if AXIS_IS_L64XX(E1)
              case 1: L6470_SET_KVAL_HOLD(E1); break;
            #endif
            #if AXIS_IS_L64XX(E2)
              case 2: L6470_SET_KVAL_HOLD(E2); break;
            #endif
            #if AXIS_IS_L64XX(E3)
              case 3: L6470_SET_KVAL_HOLD(E3); break;
            #endif
            #if AXIS_IS_L64XX(E4)
              case 4: L6470_SET_KVAL_HOLD(E4); break;
            #endif
            #if AXIS_IS_L64XX(E5)
              case 5: L6470_SET_KVAL_HOLD(E5); break;
            #endif
            #if AXIS_IS_L64XX(E6)
              case 6: L6470_SET_KVAL_HOLD(E6); break;
            #endif
            #if AXIS_IS_L64XX(E7)
              case 7: L6470_SET_KVAL_HOLD(E7); break;
            #endif
          }
        } break;
      #endif
    }
  }

  if (report_current) {
    #define L64XX_REPORT_CURRENT(Q) L64XX_report_current(stepper##Q, Q)

    L64xxManager.spi_active = true; // Tell set_directions() a series of SPI transfers is underway

    #if AXIS_IS_L64XX(X)
      L64XX_REPORT_CURRENT(X);
    #endif
    #if AXIS_IS_L64XX(X2)
      L64XX_REPORT_CURRENT(X2);
    #endif
    #if AXIS_IS_L64XX(Y)
      L64XX_REPORT_CURRENT(Y);
    #endif
    #if AXIS_IS_L64XX(Y2)
      L64XX_REPORT_CURRENT(Y2);
    #endif
    #if AXIS_IS_L64XX(Z)
      L64XX_REPORT_CURRENT(Z);
    #endif
    #if AXIS_IS_L64XX(Z2)
      L64XX_REPORT_CURRENT(Z2);
    #endif
    #if AXIS_IS_L64XX(Z3)
      L64XX_REPORT_CURRENT(Z3);
    #endif
    #if AXIS_IS_L64XX(Z4)
      L64XX_REPORT_CURRENT(Z4);
    #endif
    #if AXIS_IS_L64XX(E0)
      L64XX_REPORT_CURRENT(E0);
    #endif
    #if AXIS_IS_L64XX(E1)
      L64XX_REPORT_CURRENT(E1);
    #endif
    #if AXIS_IS_L64XX(E2)
      L64XX_REPORT_CURRENT(E2);
    #endif
    #if AXIS_IS_L64XX(E3)
      L64XX_REPORT_CURRENT(E3);
    #endif
    #if AXIS_IS_L64XX(E4)
      L64XX_REPORT_CURRENT(E4);
    #endif
    #if AXIS_IS_L64XX(E5)
      L64XX_REPORT_CURRENT(E5);
    #endif
    #if AXIS_IS_L64XX(E6)
      L64XX_REPORT_CURRENT(E6);
    #endif
    #if AXIS_IS_L64XX(E7)
      L64XX_REPORT_CURRENT(E7);
    #endif

    L64xxManager.spi_active = false;   // done with all SPI transfers - clear handshake flags
    L64xxManager.spi_abort = false;
    L64xxManager.pause_monitor(false);
  }
}

#endif // HAS_L64XX
