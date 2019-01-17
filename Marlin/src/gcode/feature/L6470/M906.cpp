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

#if HAS_DRIVER(ST_L6470)

#include "../../gcode.h"
#include "../../../module/L6470/L6470_Marlin.h"
#include "../../../module/stepper_indirection.h"

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

#define L6470_GetParam(P,Q) stepper##Q.GetParam(P)
#define L6470_status(Q) stepper##Q.getStatus()

#define L6470_GET_INFO_CURRENT(Q) {                                                                                               \
  SERIAL_ECHOPAIR("AXIS: ", L6470_index_to_Axis[Q][0]);                                                                           \
  SERIAL_ECHO(L6470_index_to_Axis[Q][1]);                                                                                         \
  OverCurrent_Threshold = (uint8_t)L6470_GetParam(L6470_OCD_TH, Q);                                                               \
  sprintf_P(temp_buf, PSTR("...OverCurrent Threshold: %2d (%4d mA)"), OverCurrent_Threshold, (OverCurrent_Threshold + 1) * 375);  \
  SERIAL_ECHO(temp_buf);                                                                                                          \
  Stall_Threshold = (uint8_t)L6470_GetParam(L6470_STALL_TH, Q);                                                                   \
  sprintf_P(temp_buf, PSTR("   Stall Threshold: %2d (%7.2f mA)"), Stall_Threshold, (Stall_Threshold + 1) * 31.25);                \
  SERIAL_ECHO(temp_buf);                                                                                                          \
  motor_status = (L6470_status(Q) & (STATUS_MOT_STATUS)) >> 13;                                                                   \
  SERIAL_ECHO("   Motor Status: ");                                                                                               \
  switch (motor_status) {                                                                                                         \
    case 0: SERIAL_ECHO("stopped"); break;                                                                                        \
    case 1: SERIAL_ECHO("accelerating"); break;                                                                                   \
    case 2: SERIAL_ECHO("decelerating"); break;                                                                                   \
    case 3: SERIAL_ECHO("at constant speed"); break;                                                                              \
  }                                                                                                                               \
  SERIAL_EOL();                                                                                                                   \
  MicroSteps = L6470_GetParam(L6470_STEP_MODE, Q) & 0x07;                                                                         \
  MicroSteps = 0x1 << MicroSteps;                                                                                                 \
  SERIAL_ECHOPAIR("...MicroSteps: ", MicroSteps);                                                                                 \
  L6470_ADC_out = L6470_GetParam(L6470_ADC_OUT, Q);                                                                               \
  L6470_ADC_out_limited = L6470_ADC_out;                                                                                          \
  LIMIT(L6470_ADC_out_limited, 8, 24);                                                                                            \
  comp_coef = 1600 / L6470_ADC_out_limited;                                                                                       \
  SERIAL_ECHOPAIR("   ADC_OUT: ", L6470_ADC_out) ;                                                                                \
  SERIAL_ECHOPAIR("   Vs_compensation: ", (L6470_GetParam(L6470_CONFIG, Q) & CONFIG_EN_VSCOMP) ? "ENABLED " : "DISABLED" );       \
  sprintf_P(temp_buf, PSTR("   Compensation coefficient: %4.2f (approximately)\n"), comp_coef / * 0.01f);                         \
  SERIAL_ECHO(temp_buf);                                                                                                          \
  SERIAL_ECHOPAIR("...KVAL_HOLD: ", L6470_GetParam(L6470_KVAL_HOLD, Q));                                                          \
  SERIAL_ECHOPAIR("   KVAL_RUN : ", L6470_GetParam(L6470_KVAL_RUN, Q));                                                          \
  SERIAL_ECHOPAIR("   KVAL_ACC: ", L6470_GetParam(L6470_KVAL_ACC, Q));                                                            \
  SERIAL_ECHOPAIR("   KVAL_DEC: ", L6470_GetParam(L6470_KVAL_DEC, Q));                                                            \
  SERIAL_ECHO("   V motor max =  ");                                                                                              \
  switch (motor_status) {                                                                                                         \
    case 0: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_HOLD)\n"), float(L6470_GetParam(L6470_KVAL_HOLD, Q)) * 100 / 256); break;    \
    case 1: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_RUN) \n"), float(L6470_GetParam(L6470_KVAL_RUN,  Q)) * 100 / 256); break;    \
    case 2: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_ACC) \n"), float(L6470_GetParam(L6470_KVAL_ACC,  Q)) * 100 / 256); break;    \
    case 3: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_DEC) \n"), float(L6470_GetParam(L6470_KVAL_DEC,  Q)) * 100 / 256); break;    \
  }                                                                                                                               \
  SERIAL_ECHO(temp_buf);                                                                                                          \
}


void GcodeSuite::M906() {
  #define L6470_SET_KVAL_HOLD(Q) stepper##Q.SetParam(L6470_KVAL_HOLD, value)

  SERIAL_ECHOLN("M906");

  bool report_current = true;

  #if HAS_DRIVER(ST_L6470)
    const uint8_t index = parser.byteval('I');
  #endif

  LOOP_XYZE(i) if (uint8_t value = parser.byteval(axis_codes[i])) {
    report_current = false;
    switch (i) {
      case X_AXIS:
        #if AXIS_DRIVER_TYPE_X(ST_L6470)
          if (index == 0) L6470_SET_KVAL_HOLD(X);
        #endif
        #if AXIS_DRIVER_TYPE_X2(ST_L6470)
          if (index == 1) L6470_SET_KVAL_HOLD(X2);
        #endif
        break;
      case Y_AXIS:
        #if AXIS_DRIVER_TYPE_Y(ST_L6470)
          if (index == 0) L6470_SET_KVAL_HOLD(Y);
        #endif
        #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
          if (index == 1) L6470_SET_KVAL_HOLD(Y2);
        #endif
        break;
      case Z_AXIS:
        #if AXIS_DRIVER_TYPE_Z(ST_L6470)
          if (index == 0) L6470_SET_KVAL_HOLD(Z);
        #endif
        #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
          if (index == 1) L6470_SET_KVAL_HOLD(Z2);
        #endif
        #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
          if (index == 2) L6470_SET_KVAL_HOLD(Z3);
        #endif
        break;
      case E_AXIS: {
        const int8_t target_extruder = get_target_extruder_from_command();
        if (target_extruder < 0) return;
        switch (target_extruder) {
          #if AXIS_DRIVER_TYPE_E0(ST_L6470)
            case 0: L6470_SET_KVAL_HOLD(E0); break;
          #endif
          #if AXIS_DRIVER_TYPE_E1(ST_L6470)
            case 1: L6470_SET_KVAL_HOLD(E1); break;
          #endif
          #if AXIS_DRIVER_TYPE_E2(ST_L6470)
            case 2: L6470_SET_KVAL_HOLD(E2); break;
          #endif
          #if AXIS_DRIVER_TYPE_E3(ST_L6470)
            case 3: L6470_SET_KVAL_HOLD(E3); break;
          #endif
          #if AXIS_DRIVER_TYPE_E4(ST_L6470)
            case 4: L6470_SET_KVAL_HOLD(E4); break;
          #endif
          #if AXIS_DRIVER_TYPE_E5(ST_L6470)
            case 5: L6470_SET_KVAL_HOLD(E5); break;
          #endif
        }
      } break;
    }
  }

  if (report_current) {
    // variables used by macro
    char temp_buf[80];
    uint8_t OverCurrent_Threshold,
            MicroSteps,
            Stall_Threshold,
            motor_status,
            L6470_ADC_out,
            L6470_ADC_out_limited;
    float comp_coef;

    SERIAL_ECHOPAIR("AXIS: ", L6470_index_to_Axis[X][0]);
    SERIAL_ECHO(L6470_index_to_Axis[X][1]);
    OverCurrent_Threshold = (uint8_t)L6470_GetParam(L6470_OCD_TH, X);
    sprintf_P(temp_buf, PSTR("...OverCurrent Threshold: %2d  (%4d mA)"), OverCurrent_Threshold, (OverCurrent_Threshold + 1) * 375 ) ;
    SERIAL_ECHO(temp_buf);
    Stall_Threshold = (uint8_t)L6470_GetParam(L6470_STALL_TH, X);
    sprintf_P(temp_buf, PSTR("   Stall Threshold:  %2d  (%7.2f mA)"),  Stall_Threshold, (Stall_Threshold + 1) * 31.25 )  ;
    SERIAL_ECHO(temp_buf);
    motor_status = (L6470_status(X) & STATUS_MOT_STATUS ) >> 13 ;
    SERIAL_ECHO("   Motor Status: ");
    switch (motor_status ) {
      case 0: SERIAL_ECHO(" stopped"); break;
      case 1: SERIAL_ECHO(" accelerating"); break;
      case 2: SERIAL_ECHO(" decelerating"); break;
      case 3: SERIAL_ECHO(" at constant speed"); break;
    }
    SERIAL_EOL();

    MicroSteps = L6470_GetParam(L6470_STEP_MODE, X)  & 0x07;
    MicroSteps = 0x1 << MicroSteps;
    SERIAL_ECHOPAIR("...MicroSteps: ", MicroSteps);
    L6470_ADC_out = L6470_GetParam(L6470_ADC_OUT, X);
    L6470_ADC_out_limited = L6470_ADC_out;
    LIMIT(L6470_ADC_out_limited, 8,24);
    comp_coef = 1600/L6470_ADC_out_limited;
    SERIAL_ECHOPAIR("   ADC_OUT: ", L6470_ADC_out) ;
    SERIAL_ECHOPAIR("   Vs_compensation: ", (L6470_GetParam(L6470_CONFIG, X) & CONFIG_EN_VSCOMP) ? "ENABLED " : "DISABLED" );
    sprintf_P(temp_buf, PSTR("   Compensation coefficient: %4.2f (approximately)\n"), comp_coef * 0.01f);
    SERIAL_ECHO(temp_buf);
    SERIAL_ECHOPAIR("...KVAL_HOLD: ", L6470_GetParam(L6470_KVAL_HOLD, X));
    SERIAL_ECHOPAIR("   KVAL_RUN : ", L6470_GetParam(L6470_KVAL_RUN , X));
    SERIAL_ECHOPAIR("   KVAL_ACC: ", L6470_GetParam(L6470_KVAL_ACC, X));
    SERIAL_ECHOPAIR("   KVAL_DEC: ", L6470_GetParam(L6470_KVAL_DEC, X));
    SERIAL_ECHO("   V motor max =  ");
    switch (motor_status) {
      case 0: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_HOLD)\n"), float(L6470_GetParam(L6470_KVAL_HOLD, X)) * 100 / 256); break;
      case 1: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_RUN) \n"), float(L6470_GetParam(L6470_KVAL_RUN, X)) * 100 / 256); break;
      case 2: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_ACC) \n"), float(L6470_GetParam(L6470_KVAL_ACC, X)) * 100 / 256); break;
      case 3: sprintf_P(temp_buf, PSTR(" %4.1f%% (KVAL_DEC) \n"), float(L6470_GetParam(L6470_KVAL_DEC, X)) * 100 / 256); break;
    }
    SERIAL_ECHO(temp_buf);

    #if AXIS_DRIVER_TYPE_X2(ST_L6470)
      L6470_GET_INFO_CURRENT(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y(ST_L6470)
      L6470_GET_INFO_CURRENT(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
      L6470_GET_INFO_CURRENT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z(ST_L6470)
      L6470_GET_INFO_CURRENT(Z);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
      L6470_GET_INFO_CURRENT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
      L6470_GET_INFO_CURRENT(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(ST_L6470)
      L6470_GET_INFO_CURRENT(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(ST_L6470)
      L6470_GET_INFO_CURRENT(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(ST_L6470)
      L6470_GET_INFO_CURRENT(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(ST_L6470)
      L6470_GET_INFO_CURRENT(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(ST_L6470)
      L6470_GET_INFO_CURRENT(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(ST_L6470)
      L6470_GET_INFO_CURRENT(E5);
    #endif
  }
}

#endif // HAS_DRIVER(ST_L6470)
