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

#include "../../../inc/MarlinConfig.h"

#if HAS_L64XX

#include "../../gcode.h"
#include "../../../libs/L6470/L6470_Marlin.h"
#include "../../../module/stepper_indirection.h"
#include "../../../module/planner.h"

/**
 *
 * M906: report or set KVAL_HOLD which sets the maximum effective voltage provided by the
 *       PWMs to the steppers
 *
 * J - select which driver(s) to monitor on multi-driver axis
 *     0 - (default) monitor all drivers on the axis or E0
 *     1 - monitor only X, Y, Z or E1
 *     2 - monitor only X2, Y2, Z2 or E2
 *     3 - monitor only Z3 or E3
 *     4 - monitor only E4
 *     5 - monitor only E5
 * Xxxx, Yxxx, Zxxx, Exxx - axis to be monitored with displacement
 *     xxx (1-255) is distance moved on either side of current position
 *
 * I - over current threshold
 *     optional - will report current value from driver if not specified
 *
 * K - value for KVAL_HOLD (0 - 255) (optional)
 *     optional - will report current value from driver if not specified
 *
 */

/**
 * Sets KVAL_HOLD wich affects the current being driven through the stepper.
 *
 * L6470 is used in the STEP-CLOCK mode.  KVAL_HOLD is the only KVAL_xxx
 * that affects the effective voltage seen by the stepper.
 *
 */

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

void L6470_report_current(L64XX &motor, const L6470_axis_t axis) {
  if (L64xx_MARLIN.spi_abort) return;  // don't do anything if set_directions() has occurred
  const uint16_t status = L64xx_MARLIN.get_status(axis);
  const uint8_t OverCurrent_Threshold = (uint8_t)motor.GetParam(L6470_OCD_TH),
                Stall_Threshold = (uint8_t)motor.GetParam(L6470_STALL_TH),
                motor_status = (status  & (STATUS_MOT_STATUS)) >> 5,
                L6470_ADC_out = motor.GetParam(L6470_ADC_OUT),
                L6470_ADC_out_limited = constrain(L6470_ADC_out, 8, 24);
  const float comp_coef = 1600.0f / L6470_ADC_out_limited;
  const int MicroSteps = _BV(motor.GetParam(L6470_STEP_MODE) & 0x07);
  char temp_buf[80];
  const L64XX_Marlin::L64XX_shadow_t &sh = L64xx_MARLIN.shadow;
  L64xx_MARLIN.say_axis(axis);
  #if ENABLED(L6470_CHITCHAT)
    sprintf_P(temp_buf, PSTR("   status: %4x   "), sh.STATUS_AXIS_RAW);
    SERIAL_ECHO(temp_buf);
    print_bin(sh.STATUS_AXIS_RAW);
    if (sh.STATUS_AXIS_LAYOUT) SERIAL_ECHOPGM("   L6470");
    else  SERIAL_ECHOPGM("   L6480/powerSTEP01");
  #endif
  sprintf_P(temp_buf, PSTR("\n...OverCurrent Threshold: %2d (%7.0f mA)"), OverCurrent_Threshold, (OverCurrent_Threshold + 1) * motor.OCD_CURRENT_CONSTANT_INV);
  SERIAL_ECHO(temp_buf);
  sprintf_P(temp_buf, PSTR("   Stall Threshold: %2d (%7.0f mA)"), Stall_Threshold, (Stall_Threshold + 1) * motor.STALL_CURRENT_CONSTANT_INV);
  SERIAL_ECHO(temp_buf);
  SERIAL_ECHOPGM("   Motor Status: ");
  switch (motor_status) {
    case 0: SERIAL_ECHOPGM("stopped"); break;
    case 1: SERIAL_ECHOPGM("accelerating"); break;
    case 2: SERIAL_ECHOPGM("decelerating"); break;
    case 3: SERIAL_ECHOPGM("at constant speed"); break;
  }
  SERIAL_EOL();
  SERIAL_ECHOPAIR("...MicroSteps: ", MicroSteps);
  SERIAL_ECHOPAIR("   ADC_OUT: ", L6470_ADC_out);
  SERIAL_ECHOPGM("   Vs_compensation: ");
  serialprintPGM((motor.GetParam(sh.L6470_AXIS_CONFIG) & CONFIG_EN_VSCOMP) ? PSTR("ENABLED ") : PSTR("DISABLED"));
  sprintf_P(temp_buf, PSTR("   Compensation coefficient: ~%4.2f\n"), comp_coef * 0.01f);
  SERIAL_ECHO(temp_buf);
  SERIAL_ECHOPAIR("...KVAL_HOLD: ", motor.GetParam(L6470_KVAL_HOLD));
  SERIAL_ECHOPAIR("   KVAL_RUN : ", motor.GetParam(L6470_KVAL_RUN));
  SERIAL_ECHOPAIR("   KVAL_ACC: ", motor.GetParam(L6470_KVAL_ACC));
  SERIAL_ECHOPAIR("   KVAL_DEC: ", motor.GetParam(L6470_KVAL_DEC));
  SERIAL_ECHOPGM("   V motor max =  ");
  switch (motor_status) {
    case 0: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_HOLD)\n"), float(motor.GetParam(L6470_KVAL_HOLD)) * 100 / 256); break;
    case 1: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_RUN) \n"), float(motor.GetParam(L6470_KVAL_RUN)) * 100 / 256); break;
    case 2: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_ACC) \n"), float(motor.GetParam(L6470_KVAL_ACC)) * 100 / 256); break;
    case 3: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_DEC) \n"), float(motor.GetParam(L6470_KVAL_DEC)) * 100 / 256); break;
  }
  SERIAL_ECHO(temp_buf);
  #if ENABLED(L6470_CHITCHAT)
    SERIAL_ECHOPGM("...SLEW RATE: ");
    if (sh.STATUS_AXIS_LAYOUT) {
      switch ((motor.GetParam(sh.L6470_AXIS_CONFIG) & CONFIG_POW_SR) >> CONFIG_POW_SR_BIT) {
        case 0: {SERIAL_ECHOLNPGM("320V/uS") ; break; }
        case 1: {SERIAL_ECHOLNPGM("75V/uS")  ; break; }
        case 2: {SERIAL_ECHOLNPGM("110V/uS") ; break; }
        case 3: {SERIAL_ECHOLNPGM("260V/uS") ; break; }
      }
    }
    else {
      switch (motor.GetParam(L6470_GATECFG1) & CONFIG1_SR ) {
        case CONFIG1_SR_220V_us: {SERIAL_ECHOLNPGM("220V/uS") ; break; }
        case CONFIG1_SR_400V_us: {SERIAL_ECHOLNPGM("400V/uS") ; break; }
        case CONFIG1_SR_520V_us: {SERIAL_ECHOLNPGM("520V/uS") ; break; }
        case CONFIG1_SR_980V_us: {SERIAL_ECHOLNPGM("980V/uS") ; break; }
        default: {SERIAL_ECHOLNPGM("unknown") ; break; }
      }
    }
  #endif
}

void GcodeSuite::M906() {
  #define L6470_SET_KVAL_HOLD(Q) stepper##Q.SetParam(L6470_KVAL_HOLD, value)

  L6470_ECHOLNPGM("M906");

  bool report_current = true;

  #if HAS_L64XX
    const uint8_t index = parser.byteval('I');
  #endif

  LOOP_XYZE(i) if (uint8_t value = parser.byteval(axis_codes[i])) {

    report_current = false;

    if (planner.has_blocks_queued() || planner.cleaning_buffer_counter) {
      SERIAL_ECHOLNPGM("ERROR - can't set KVAL_HOLD while steppers are moving");
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
      case Y_AXIS:
        #if AXIS_IS_L64XX(Y)
          if (index == 0) L6470_SET_KVAL_HOLD(Y);
        #endif
        #if AXIS_IS_L64XX(Y2)
          if (index == 1) L6470_SET_KVAL_HOLD(Y2);
        #endif
        break;
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
        break;
      case E_AXIS: {
        const int8_t target_extruder = get_target_extruder_from_command();
        if (target_extruder < 0) return;
        switch (target_extruder) {
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
        }
      } break;
    }
  }

  if (report_current) {
    #define L6470_REPORT_CURRENT(Q) L6470_report_current(stepper##Q, Q)

    L64xx_MARLIN.spi_active = true;    // let set_directions() know we're in the middle of a series of SPI transfers

    #if AXIS_IS_L64XX(X)
      L6470_REPORT_CURRENT(X);
    #endif
    #if AXIS_IS_L64XX(X2)
      L6470_REPORT_CURRENT(X2);
    #endif
    #if AXIS_IS_L64XX(Y)
      L6470_REPORT_CURRENT(Y);
    #endif
    #if AXIS_IS_L64XX(Y2)
      L6470_REPORT_CURRENT(Y2);
    #endif
    #if AXIS_IS_L64XX(Z)
      L6470_REPORT_CURRENT(Z);
    #endif
    #if AXIS_IS_L64XX(Z2)
      L6470_REPORT_CURRENT(Z2);
    #endif
    #if AXIS_IS_L64XX(Z3)
      L6470_REPORT_CURRENT(Z3);
    #endif
    #if AXIS_IS_L64XX(E0)
      L6470_REPORT_CURRENT(E0);
    #endif
    #if AXIS_IS_L64XX(E1)
      L6470_REPORT_CURRENT(E1);
    #endif
    #if AXIS_IS_L64XX(E2)
      L6470_REPORT_CURRENT(E2);
    #endif
    #if AXIS_IS_L64XX(E3)
      L6470_REPORT_CURRENT(E3);
    #endif
    #if AXIS_IS_L64XX(E4)
      L6470_REPORT_CURRENT(E4);
    #endif
    #if AXIS_IS_L64XX(E5)
      L6470_REPORT_CURRENT(E5);
    #endif

    L64xx_MARLIN.spi_active = false;   // done with all SPI transfers - clear handshake flags
    L64xx_MARLIN.spi_abort = false;
  }
}

#endif // HAS_L64XX
