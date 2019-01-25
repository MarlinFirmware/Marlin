/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if HAS_DRIVER(L6470)

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

void L6470_report_current(L6470 &motor, const uint8_t axis) {
  if (L6470.spi_abort) return;  // don't do anything if set_directions() has occurred
  const uint16_t status = motor.getStatus() ;
  const uint8_t overcurrent_threshold = (uint8_t)motor.GetParam(L6470_OCD_TH),
                stall_threshold = (uint8_t)motor.GetParam(L6470_STALL_TH),
                motor_status = (status  & (STATUS_MOT_STATUS)) >> 13,
                adc_out = motor.GetParam(L6470_ADC_OUT),
                adc_out_limited = constrain(adc_out, 8, 24);
  const float comp_coef = 1600.0f / adc_out_limited;
  const int microsteps = _BV(motor.GetParam(L6470_STEP_MODE) & 0x07);
  char temp_buf[80];
  L6470.say_axis(axis);
  #if ENABLED(L6470_CHITCHAT)
    sprintf_P(temp_buf, PSTR("   status: %4x   "), status);
    SERIAL_ECHO(temp_buf);
    print_bin(status);
  #endif
  sprintf_P(temp_buf, PSTR("\n...OverCurrent Threshold: %2d (%4d mA)"), overcurrent_threshold, (overcurrent_threshold + 1) * 375);
  SERIAL_ECHO(temp_buf);
  sprintf_P(temp_buf, PSTR("   Stall Threshold: %2d (%7.2f mA)"), stall_threshold, (stall_threshold + 1) * 31.25);
  SERIAL_ECHO(temp_buf);
  SERIAL_ECHOPGM("   Motor Status: ");
  const char * const stat_str;
  switch (motor_status) {
    default:
    case 0: stat_str = PSTR("stopped"); break;
    case 1: stat_str = PSTR("accelerating"); break;
    case 2: stat_str = PSTR("decelerating"); break;
    case 3: stat_str = PSTR("at constant speed"); break;
  }
  serialprintPGM(stat_str);
  SERIAL_EOL();
  SERIAL_ECHOPAIR("...microsteps: ", microsteps);
  SERIAL_ECHOPAIR("   ADC_OUT: ", adc_out);
  SERIAL_ECHOPGM("   Vs_compensation: ");
  serialprintPGM((motor.GetParam(L6470_CONFIG) & CONFIG_EN_VSCOMP) ? PSTR("ENABLED ") : PSTR("DISABLED"));
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
}

void GcodeSuite::M906() {
  #define L6470_SET_KVAL_HOLD(Q) stepper##Q.SetParam(L6470_KVAL_HOLD, value)

  L6470_ECHOLNPGM("M906");

  bool report_current = true;

  #if HAS_DRIVER(L6470)
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
        #if AXIS_DRIVER_TYPE_X(L6470)
          if (index == 0) L6470_SET_KVAL_HOLD(X);
        #endif
        #if AXIS_DRIVER_TYPE_X2(L6470)
          if (index == 1) L6470_SET_KVAL_HOLD(X2);
        #endif
        break;
      case Y_AXIS:
        #if AXIS_DRIVER_TYPE_Y(L6470)
          if (index == 0) L6470_SET_KVAL_HOLD(Y);
        #endif
        #if AXIS_DRIVER_TYPE_Y2(L6470)
          if (index == 1) L6470_SET_KVAL_HOLD(Y2);
        #endif
        break;
      case Z_AXIS:
        #if AXIS_DRIVER_TYPE_Z(L6470)
          if (index == 0) L6470_SET_KVAL_HOLD(Z);
        #endif
        #if AXIS_DRIVER_TYPE_Z2(L6470)
          if (index == 1) L6470_SET_KVAL_HOLD(Z2);
        #endif
        #if AXIS_DRIVER_TYPE_Z3(L6470)
          if (index == 2) L6470_SET_KVAL_HOLD(Z3);
        #endif
        break;
      case E_AXIS: {
        const int8_t target_extruder = get_target_extruder_from_command();
        if (target_extruder < 0) return;
        switch (target_extruder) {
          #if AXIS_DRIVER_TYPE_E0(L6470)
            case 0: L6470_SET_KVAL_HOLD(E0); break;
          #endif
          #if AXIS_DRIVER_TYPE_E1(L6470)
            case 1: L6470_SET_KVAL_HOLD(E1); break;
          #endif
          #if AXIS_DRIVER_TYPE_E2(L6470)
            case 2: L6470_SET_KVAL_HOLD(E2); break;
          #endif
          #if AXIS_DRIVER_TYPE_E3(L6470)
            case 3: L6470_SET_KVAL_HOLD(E3); break;
          #endif
          #if AXIS_DRIVER_TYPE_E4(L6470)
            case 4: L6470_SET_KVAL_HOLD(E4); break;
          #endif
          #if AXIS_DRIVER_TYPE_E5(L6470)
            case 5: L6470_SET_KVAL_HOLD(E5); break;
          #endif
        }
      } break;
    }
  }

  if (report_current) {
    #define L6470_REPORT_CURRENT(Q) L6470_report_current(stepper##Q, Q)

    L6470.spi_active = true;    // let set_directions() know we're in the middle of a series of SPI transfers

    #if AXIS_DRIVER_TYPE_X(L6470)
      L6470_REPORT_CURRENT(X);
    #endif
    #if AXIS_DRIVER_TYPE_X2(L6470)
      L6470_REPORT_CURRENT(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y(L6470)
      L6470_REPORT_CURRENT(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(L6470)
      L6470_REPORT_CURRENT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z(L6470)
      L6470_REPORT_CURRENT(Z);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(L6470)
      L6470_REPORT_CURRENT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(L6470)
      L6470_REPORT_CURRENT(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(L6470)
      L6470_REPORT_CURRENT(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(L6470)
      L6470_REPORT_CURRENT(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(L6470)
      L6470_REPORT_CURRENT(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(L6470)
      L6470_REPORT_CURRENT(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(L6470)
      L6470_REPORT_CURRENT(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(L6470)
      L6470_REPORT_CURRENT(E5);
    #endif

    L6470.spi_active = false;   // done with all SPI transfers - clear handshake flags
    L6470.spi_abort = false;
  }
}

#endif // HAS_DRIVER(L6470)
