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

//
// NOTE: All tests assume each axis uses matching driver chips.
//

#include "../../../inc/MarlinConfig.h"

#if HAS_L64XX

#include "../../gcode.h"
#include "../../../module/stepper/indirection.h"
#include "../../../module/planner.h"
#include "../../../libs/L64XX/L64XX_Marlin.h"

#define DEBUG_OUT ENABLED(L6470_CHITCHAT)
#include "../../../core/debug_out.h"

/**
 * M916: increase KVAL_HOLD until get thermal warning
 *       NOTE - on L6474 it is TVAL that is used
 *
 * J - select which driver(s) to monitor on multi-driver axis
 *     0 - (default) monitor all drivers on the axis or E0
 *     1 - monitor only X, Y, Z, E1
 *     2 - monitor only X2, Y2, Z2, E2
 *     3 - monitor only Z3, E3
 *     4 - monitor only Z4, E4
 *
 * Xxxx, Yxxx, Zxxx, Exxx - axis to be monitored with displacement
 *     xxx (1-255) is distance moved on either side of current position
 *
 * F - feedrate
 *     optional - will use default max feedrate from configuration.h if not specified
 *
 * T - current (mA) setting for TVAL (0 - 4A in 31.25mA increments, rounds down) - L6474 only
 *     optional - will report current value from driver if not specified
 *
 * K - value for KVAL_HOLD (0 - 255) (ignored for L6474)
 *     optional - will report current value from driver if not specified
 *
 * D - time (in seconds) to run each setting of KVAL_HOLD/TVAL
 *     optional - defaults to zero (runs each setting once)
 */

/**
 * This routine is also useful for determining the approximate KVAL_HOLD
 * where the stepper stops losing steps. The sound will get noticeably quieter
 * as it stops losing steps.
 */

void GcodeSuite::M916() {

  DEBUG_ECHOLNPGM("M916");

  L64xxManager.pause_monitor(true); // Keep monitor_driver() from stealing status

  // Variables used by L64xxManager.get_user_input function - some may not be used
  char axis_mon[3][3] = { {"  "}, {"  "}, {"  "} };   // list of Axes to be monitored
  L64XX_axis_t axis_index[3];
  uint16_t axis_status[3];
  uint8_t driver_count = 1;
  float position_max;
  float position_min;
  float final_feedrate;
  uint8_t kval_hold;
  uint8_t OCD_TH_val = 0;
  uint8_t STALL_TH_val = 0;
  uint16_t over_current_threshold;
  constexpr uint8_t over_current_flag = false;  // M916 doesn't play with the overcurrent thresholds

  #define DRIVER_TYPE_L6474(Q) AXIS_DRIVER_TYPE_##Q(L6474)

  uint8_t j;   // general purpose counter

  if (L64xxManager.get_user_input(driver_count, axis_index, axis_mon, position_max, position_min, final_feedrate, kval_hold, over_current_flag, OCD_TH_val, STALL_TH_val, over_current_threshold))
    return;  // quit if invalid user input

  DEBUG_ECHOLNPAIR("feedrate = ", final_feedrate);

  planner.synchronize();                             // wait for all current movement commands to complete

  const L64XX_Marlin::L64XX_shadow_t &sh = L64xxManager.shadow;
  for (j = 0; j < driver_count; j++)
    L64xxManager.get_status(axis_index[j]);  // clear out any pre-existing error flags

  char temp_axis_string[] = " ";
  temp_axis_string[0] = axis_mon[0][0];  // need to have a string for use within sprintf format section
  char gcode_string[80];
  uint16_t status_composite = 0;

  uint16_t M91x_counter = kval_hold;
  uint16_t M91x_counter_max;
  if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT) {
    M91x_counter_max = 128;  // TVAL is 7 bits
    LIMIT(M91x_counter, 0U, 127U);
  }
  else
    M91x_counter_max = 256;  // KVAL_HOLD is 8 bits

  uint8_t M91x_delay_s = parser.byteval('D');   // get delay in seconds
  millis_t M91x_delay_ms = M91x_delay_s * 60 * 1000;
  millis_t M91x_delay_end;

  DEBUG_ECHOLNPGM(".\n.");

  do {

    if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT)
      DEBUG_ECHOLNPAIR("TVAL current (mA) = ", (M91x_counter + 1) * sh.AXIS_STALL_CURRENT_CONSTANT_INV);        // report TVAL current for this run
    else
      DEBUG_ECHOLNPAIR("kval_hold = ", M91x_counter);                                   // report KVAL_HOLD for this run

    for (j = 0; j < driver_count; j++)
      L64xxManager.set_param(axis_index[j], L6470_KVAL_HOLD, M91x_counter);  //set KVAL_HOLD or TVAL (same register address)

    M91x_delay_end = millis() + M91x_delay_ms;
    do {
      // turn the motor(s) both directions
      sprintf_P(gcode_string, PSTR("G0 %s%03d F%03d"), temp_axis_string, uint16_t(position_min), uint16_t(final_feedrate));
      gcode.process_subcommands_now_P(gcode_string);

      sprintf_P(gcode_string, PSTR("G0 %s%03d F%03d"), temp_axis_string, uint16_t(position_max), uint16_t(final_feedrate));
      gcode.process_subcommands_now_P(gcode_string);

      // get the status after the motors have stopped
      planner.synchronize();

      status_composite = 0;    // clear out the old bits

      for (j = 0; j < driver_count; j++) {
        axis_status[j] = (~L64xxManager.get_status(axis_index[j])) & sh.L6470_ERROR_MASK;    // bits of interest are all active low
        status_composite |= axis_status[j] ;
      }

      if (status_composite) break;
    } while (millis() < M91x_delay_end);

    if (status_composite) break;

    M91x_counter++;

  } while (!(status_composite & (sh.STATUS_AXIS_TH_WRN | sh.STATUS_AXIS_TH_SD)) && (M91x_counter < M91x_counter_max));

  DEBUG_ECHOLNPGM(".");

  #if ENABLED(L6470_CHITCHAT)
    if (status_composite) {
      L64xxManager.error_status_decode(status_composite, axis_index[0],
        sh.STATUS_AXIS_TH_SD, sh.STATUS_AXIS_TH_WRN,
        sh.STATUS_AXIS_STEP_LOSS_A, sh.STATUS_AXIS_STEP_LOSS_B,
        sh.STATUS_AXIS_OCD, sh.STATUS_AXIS_LAYOUT);
      DEBUG_ECHOLNPGM(".");
    }
  #endif

  if ((status_composite & (sh.STATUS_AXIS_TH_WRN | sh.STATUS_AXIS_TH_SD)))
    DEBUG_ECHOLNPGM(".\n.\nTest completed normally - Thermal warning/shutdown has occurred");
  else if (status_composite)
    DEBUG_ECHOLNPGM(".\n.\nTest completed abnormally - non-thermal error has occured");
  else
    DEBUG_ECHOLNPGM(".\n.\nTest completed normally - Unable to get to thermal warning/shutdown");

  L64xxManager.pause_monitor(false);
}

/**
 * M917: Find minimum current thresholds
 *
 *   Decrease OCD current until overcurrent error
 *   Increase OCD until overcurrent error goes away
 *   Decrease stall threshold until stall (not done on L6474)
 *   Increase stall until stall error goes away (not done on L6474)
 *
 * J - select which driver(s) to monitor on multi-driver axis
 *     0 - (default) monitor all drivers on the axis or E0
 *     1 - monitor only X, Y, Z, E1
 *     2 - monitor only X2, Y2, Z2, E2
 * Xxxx, Yxxx, Zxxx, Exxx - axis to be monitored with displacement
 *     xxx (1-255) is distance moved on either side of current position
 *
 * F - feedrate
 *     optional - will use default max feedrate from Configuration.h if not specified
 *
 * I - starting over-current threshold
 *     optional - will report current value from driver if not specified
 *     if there are multiple drivers on the axis then all will be set the same
 *
 * T - current (mA) setting for TVAL (0 - 4A in 31.25mA increments, rounds down) - L6474 only
 *     optional - will report current value from driver if not specified
 *
 * K - value for KVAL_HOLD (0 - 255) (ignored for L6474)
 *     optional - will report current value from driver if not specified
 */
void GcodeSuite::M917() {

  DEBUG_ECHOLNPGM("M917");

  L64xxManager.pause_monitor(true); // Keep monitor_driver() from stealing status

  char axis_mon[3][3] = { {"  "}, {"  "}, {"  "} };   // list of Axes to be monitored
  L64XX_axis_t axis_index[3];
  uint16_t axis_status[3];
  uint8_t driver_count = 1;
  float position_max;
  float position_min;
  float final_feedrate;
  uint8_t kval_hold;
  uint8_t OCD_TH_val = 0;
  uint8_t STALL_TH_val = 0;
  uint16_t over_current_threshold;
  constexpr uint8_t over_current_flag = true;

  uint8_t j;   // general purpose counter

  if (L64xxManager.get_user_input(driver_count, axis_index, axis_mon, position_max, position_min, final_feedrate, kval_hold, over_current_flag, OCD_TH_val, STALL_TH_val, over_current_threshold))
    return;  // quit if invalid user input

  DEBUG_ECHOLNPAIR("feedrate = ", final_feedrate);

  planner.synchronize();                             // wait for all current movement commands to complete

  const L64XX_Marlin::L64XX_shadow_t &sh = L64xxManager.shadow;
  for (j = 0; j < driver_count; j++)
    L64xxManager.get_status(axis_index[j]);  // clear error flags
  char temp_axis_string[] = " ";
  temp_axis_string[0] = axis_mon[0][0];   // need a sprintf format string
  char gcode_string[80];
  uint16_t status_composite = 0;
  uint8_t test_phase = 0;                 // 0 - decreasing OCD - exit when OCD warning occurs (ignore STALL)
                                          // 1 - increasing OCD - exit when OCD warning stops (ignore STALL)
                                          // 2 - OCD finalized - decreasing STALL - exit when STALL warning happens
                                          // 3 - OCD finalized - increasing STALL - exit when STALL warning stop
                                          // 4 - all testing completed
  DEBUG_ECHOPAIR(".\n.\n.\nover_current threshold : ", (OCD_TH_val + 1) * 375);   // first status display
  DEBUG_ECHOPAIR("  (OCD_TH:  : ", OCD_TH_val);
  if (sh.STATUS_AXIS_LAYOUT != L6474_STATUS_LAYOUT) {
    DEBUG_ECHOPAIR(")   Stall threshold: ", (STALL_TH_val + 1) * 31.25);
    DEBUG_ECHOPAIR("  (STALL_TH: ", STALL_TH_val);
  }
  DEBUG_ECHOLNPGM(")");

  do {

    if (sh.STATUS_AXIS_LAYOUT != L6474_STATUS_LAYOUT) DEBUG_ECHOPAIR("STALL threshold : ", (STALL_TH_val + 1) * 31.25);
    DEBUG_ECHOLNPAIR("   OCD threshold : ", (OCD_TH_val + 1) * 375);

    sprintf_P(gcode_string, PSTR("G0 %s%03d F%03d"), temp_axis_string, uint16_t(position_min), uint16_t(final_feedrate));
    gcode.process_subcommands_now_P(gcode_string);

    sprintf_P(gcode_string, PSTR("G0 %s%03d F%03d"), temp_axis_string, uint16_t(position_max), uint16_t(final_feedrate));
    gcode.process_subcommands_now_P(gcode_string);

    planner.synchronize();

    status_composite = 0;    // clear out the old bits

    for (j = 0; j < driver_count; j++) {
      axis_status[j] = (~L64xxManager.get_status(axis_index[j])) & sh.L6470_ERROR_MASK;    // bits of interest are all active low
      status_composite |= axis_status[j];
    }

    if (status_composite && (status_composite & sh.STATUS_AXIS_UVLO)) {
      DEBUG_ECHOLNPGM("Test aborted (Undervoltage lockout active)");
      #if ENABLED(L6470_CHITCHAT)
        for (j = 0; j < driver_count; j++) {
          if (j) DEBUG_ECHOPGM("...");
          L64xxManager.error_status_decode(axis_status[j], axis_index[j],
            sh.STATUS_AXIS_TH_SD, sh.STATUS_AXIS_TH_WRN,
            sh.STATUS_AXIS_STEP_LOSS_A, sh.STATUS_AXIS_STEP_LOSS_B,
            sh.STATUS_AXIS_OCD, sh.STATUS_AXIS_LAYOUT);
        }
      #endif
      return;
    }

    if (status_composite & (sh.STATUS_AXIS_TH_WRN | sh.STATUS_AXIS_TH_SD)) {
      DEBUG_ECHOLNPGM("thermal problem - waiting for chip(s) to cool down ");
      uint16_t status_composite_temp = 0;
      uint8_t k = 0;
      do {
        k++;
        if (!(k % 4)) {
          kval_hold *= 0.95;
          DEBUG_EOL();
          DEBUG_ECHOLNPAIR("Lowering KVAL_HOLD by about 5% to ", kval_hold);
          for (j = 0; j < driver_count; j++)
            L64xxManager.set_param(axis_index[j], L6470_KVAL_HOLD, kval_hold);
        }
        DEBUG_ECHOLNPGM(".");
        gcode.reset_stepper_timeout(); // reset_stepper_timeout to keep steppers powered
        watchdog_refresh();;   // beat the dog
        safe_delay(5000);
        status_composite_temp = 0;
        for (j = 0; j < driver_count; j++) {
          axis_status[j] = (~L64xxManager.get_status(axis_index[j])) & sh.L6470_ERROR_MASK;    // bits of interest are all active low
          status_composite_temp |= axis_status[j];
        }
      }
      while (status_composite_temp & (sh.STATUS_AXIS_TH_WRN | sh.STATUS_AXIS_TH_SD));
      DEBUG_EOL();
    }
    if (status_composite & (sh.STATUS_AXIS_STEP_LOSS_A | sh.STATUS_AXIS_STEP_LOSS_B | sh.STATUS_AXIS_OCD)) {
      switch (test_phase) {

        case 0: {
          if (status_composite & sh.STATUS_AXIS_OCD) {
            // phase 0 with OCD warning - time to go to next phase
            if (OCD_TH_val >= sh.AXIS_OCD_TH_MAX) {
              OCD_TH_val = sh.AXIS_OCD_TH_MAX;           // limit to max
              test_phase = 2;            // at highest value so skip phase 1
              //DEBUG_ECHOLNPGM("LOGIC E0A OCD at highest - skip to 2");
              DEBUG_ECHOLNPGM("OCD at highest - OCD finalized");
            }
            else {
              OCD_TH_val++;              // normal exit to next phase
              test_phase = 1;            // setup for first pass of phase 1
              //DEBUG_ECHOLNPGM("LOGIC E0B - inc OCD  & go to 1");
              DEBUG_ECHOLNPGM("inc OCD");
            }
          }
          else {  // phase 0 without OCD warning - keep on decrementing if can
            if (OCD_TH_val) {
              OCD_TH_val--;              // try lower value
              //DEBUG_ECHOLNPGM("LOGIC E0C - dec OCD");
              DEBUG_ECHOLNPGM("dec OCD");
            }
            else {
              test_phase = 2;            // at lowest value without warning so skip phase 1
              //DEBUG_ECHOLNPGM("LOGIC E0D - OCD at latest - go to 2");
              DEBUG_ECHOLNPGM("OCD finalized");
            }
          }
        } break;

        case 1: {
          if (status_composite & sh.STATUS_AXIS_OCD) {
            // phase 1 with OCD warning - increment if can
            if (OCD_TH_val >= sh.AXIS_OCD_TH_MAX) {
              OCD_TH_val = sh.AXIS_OCD_TH_MAX;           // limit to max
              test_phase = 2;            // at highest value so go to next phase
              //DEBUG_ECHOLNPGM("LOGIC E1A - OCD at max - go to 2");
              DEBUG_ECHOLNPGM("OCD finalized");
            }
            else {
              OCD_TH_val++;              // try a higher value
              //DEBUG_ECHOLNPGM("LOGIC E1B - inc OCD");
              DEBUG_ECHOLNPGM("inc OCD");
            }
          }
          else { // phase 1 without OCD warning - normal exit to phase 2
            test_phase = 2;
            //DEBUG_ECHOLNPGM("LOGIC E1C - no OCD warning - go to 1");
            DEBUG_ECHOLNPGM("OCD finalized");
          }
        } break;

        case 2: {
          if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT) {  // skip all STALL_TH steps if L6474
            test_phase = 4;
            break;
          }
          if (status_composite & (sh.STATUS_AXIS_STEP_LOSS_A | sh.STATUS_AXIS_STEP_LOSS_B)) {
            // phase 2 with stall warning - time to go to next phase
            if (STALL_TH_val >= 127) {
              STALL_TH_val = 127;  // limit to max
              //DEBUG_ECHOLNPGM("LOGIC E2A - STALL warning, STALL at max, quit");
              DEBUG_ECHOLNPGM("finished - STALL at maximum value but still have stall warning");
              test_phase = 4;
            }
            else {
              test_phase = 3;              // normal exit to next phase (found failing value of STALL)
              STALL_TH_val++;              // setup for first pass of phase 3
              //DEBUG_ECHOLNPGM("LOGIC E2B - INC - STALL warning, inc Stall, go to 3");
              DEBUG_ECHOLNPGM("inc Stall");
            }
          }
          else {  // phase 2 without stall warning - decrement if can
            if (STALL_TH_val) {
              STALL_TH_val--;              // try a lower value
              //DEBUG_ECHOLNPGM("LOGIC E2C - no STALL, dec STALL");
              DEBUG_ECHOLNPGM("dec STALL");
            }
            else {
              DEBUG_ECHOLNPGM("finished - STALL at lowest value but still do NOT have stall warning");
              test_phase = 4;
              //DEBUG_ECHOLNPGM("LOGIC E2D - no STALL, at lowest so quit");
            }
          }
        } break;

        case 3: {
          if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT) {  // skip all STALL_TH steps if L6474
            test_phase = 4;
            break;
          }
          if (status_composite & (sh.STATUS_AXIS_STEP_LOSS_A | sh.STATUS_AXIS_STEP_LOSS_B)) {
            // phase 3 with stall warning - increment if can
            if (STALL_TH_val >= 127) {
              STALL_TH_val = 127; // limit to max
              DEBUG_ECHOLNPGM("finished - STALL at maximum value but still have stall warning");
              test_phase = 4;
              //DEBUG_ECHOLNPGM("LOGIC E3A - STALL, at max so quit");
            }
            else {
              STALL_TH_val++;              // still looking for passing value
              //DEBUG_ECHOLNPGM("LOGIC E3B - STALL, inc stall");
              DEBUG_ECHOLNPGM("inc stall");
            }
          }
          else {  //phase 3 without stall warning  but have OCD warning
            DEBUG_ECHOLNPGM("Hardware problem - OCD warning without STALL warning");
            test_phase = 4;
            //DEBUG_ECHOLNPGM("LOGIC E3C - not STALLED, hardware problem (quit)");
          }
        } break;

      }

    }
    else {
      switch (test_phase) {
        case 0: { // phase 0 without OCD warning - keep on decrementing if can
          if (OCD_TH_val) {
            OCD_TH_val--;             // try lower value
            //DEBUG_ECHOLNPGM("LOGIC N0A - DEC OCD");
            DEBUG_ECHOLNPGM("DEC OCD");
          }
          else {
            test_phase = 2;           // at lowest value without warning so skip phase 1
            //DEBUG_ECHOLNPGM("LOGIC N0B - OCD at lowest (go to phase 2)");
            DEBUG_ECHOLNPGM("OCD finalized");
          }
        } break;

        case 1: //DEBUG_ECHOLNPGM("LOGIC N1 (go directly to 2)"); // phase 1 without OCD warning - drop directly to phase 2
                DEBUG_ECHOLNPGM("OCD finalized");

        case 2: { // phase 2 without stall warning - keep on decrementing if can
          if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT) {  // skip all STALL_TH steps if L6474
            test_phase = 4;
            break;
          }
          if (STALL_TH_val) {
            STALL_TH_val--;              // try a lower value (stay in phase 2)
            //DEBUG_ECHOLNPGM("LOGIC N2B - dec STALL");
            DEBUG_ECHOLNPGM("dec STALL");
          }
          else {
            DEBUG_ECHOLNPGM("finished - STALL at lowest value but still no stall warning");
            test_phase = 4;
            //DEBUG_ECHOLNPGM("LOGIC N2C - STALL at lowest (quit)");
          }
        } break;

        case 3: {
          if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT) {  // skip all STALL_TH steps if L6474
            test_phase = 4;
            break;
          }
          test_phase = 4;
           //DEBUG_ECHOLNPGM("LOGIC N3 - finished!");
           DEBUG_ECHOLNPGM("finished!");
        } break;  // phase 3 without any warnings - desired exit
      }  //
    }  // end of status checks

    if (test_phase != 4) {
      for (j = 0; j < driver_count; j++) {                       // update threshold(s)
        L64xxManager.set_param(axis_index[j], L6470_OCD_TH, OCD_TH_val);
        if (sh.STATUS_AXIS_LAYOUT != L6474_STATUS_LAYOUT) L64xxManager.set_param(axis_index[j], L6470_STALL_TH, STALL_TH_val);
        if (L64xxManager.get_param(axis_index[j], L6470_OCD_TH) != OCD_TH_val) DEBUG_ECHOLNPGM("OCD mismatch");
        if ((L64xxManager.get_param(axis_index[j], L6470_STALL_TH) != STALL_TH_val) && (sh.STATUS_AXIS_LAYOUT != L6474_STATUS_LAYOUT)) DEBUG_ECHOLNPGM("STALL mismatch");
      }
    }

  } while (test_phase != 4);

  DEBUG_ECHOLNPGM(".");
  if (status_composite) {
    #if ENABLED(L6470_CHITCHAT)
      for (j = 0; j < driver_count; j++) {
        if (j) DEBUG_ECHOPGM("...");
        L64xxManager.error_status_decode(axis_status[j], axis_index[j],
          sh.STATUS_AXIS_TH_SD, sh.STATUS_AXIS_TH_WRN,
          sh.STATUS_AXIS_STEP_LOSS_A, sh.STATUS_AXIS_STEP_LOSS_B,
          sh.STATUS_AXIS_OCD, sh.STATUS_AXIS_LAYOUT);
      }
      DEBUG_ECHOLNPGM(".");
    #endif
    DEBUG_ECHOLNPGM("Completed with errors");
  }
  else
    DEBUG_ECHOLNPGM("Completed with no errors");
  DEBUG_ECHOLNPGM(".");

  L64xxManager.pause_monitor(false);
}

/**
 * M918: increase speed until error or max feedrate achieved (as shown in configuration.h))
 *
 * J - select which driver(s) to monitor on multi-driver axis
 *     0 - (default) monitor all drivers on the axis or E0
 *     1 - monitor only X, Y, Z, E1
 *     2 - monitor only X2, Y2, Z2, E2
 * Xxxx, Yxxx, Zxxx, Exxx - axis to be monitored with displacement
 *     xxx (1-255) is distance moved on either side of current position
 *
 * I - over current threshold
 *     optional - will report current value from driver if not specified
 *
 * T - current (mA) setting for TVAL (0 - 4A in 31.25mA increments, rounds down) - L6474 only
 *     optional - will report current value from driver if not specified
 *
 * K - value for KVAL_HOLD (0 - 255) (ignored for L6474)
 *     optional - will report current value from driver if not specified
 *
 * M - value for microsteps (1 - 128) (optional)
 *     optional - will report current value from driver if not specified
 */
void GcodeSuite::M918() {

  DEBUG_ECHOLNPGM("M918");

  L64xxManager.pause_monitor(true); // Keep monitor_driver() from stealing status

  char axis_mon[3][3] = { {"  "}, {"  "}, {"  "} };   // list of Axes to be monitored
  L64XX_axis_t axis_index[3];
  uint16_t axis_status[3];
  uint8_t driver_count = 1;
  float position_max, position_min;
  float final_feedrate;
  uint8_t kval_hold;
  uint8_t OCD_TH_val = 0;
  uint8_t STALL_TH_val = 0;
  uint16_t over_current_threshold;
  constexpr uint8_t over_current_flag = true;

  const L64XX_Marlin::L64XX_shadow_t &sh = L64xxManager.shadow;

  uint8_t j;   // general purpose counter

  if (L64xxManager.get_user_input(driver_count, axis_index, axis_mon, position_max, position_min, final_feedrate, kval_hold, over_current_flag, OCD_TH_val, STALL_TH_val, over_current_threshold))
    return;  // quit if invalid user input

  L64xxManager.get_status(axis_index[0]); // populate shadow array

  uint8_t m_steps = parser.byteval('M');

  if (m_steps != 0) {
    LIMIT(m_steps, 1, sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT ? 16 : 128);  // L6474

    uint8_t stepVal;
    for (stepVal = 0; stepVal < 8; stepVal++) {  // convert to L64xx register value
      if (m_steps == 1) break;
      m_steps >>= 1;
    }

    if (sh.STATUS_AXIS_LAYOUT == L6474_STATUS_LAYOUT)
      stepVal |= 0x98;  // NO SYNC
    else
      stepVal |= (!SYNC_EN) | SYNC_SEL_1 | stepVal;

    for (j = 0; j < driver_count; j++) {
      L64xxManager.set_param(axis_index[j], dSPIN_HARD_HIZ, 0);          // can't write STEP register if stepper being powered
                                                                         //   results in an extra NOOP being sent (data 00)
      L64xxManager.set_param(axis_index[j], L6470_STEP_MODE, stepVal);   // set microsteps
    }
  }
  m_steps = L64xxManager.get_param(axis_index[0], L6470_STEP_MODE) & 0x07;   // get microsteps

  DEBUG_ECHOLNPAIR("Microsteps = ", _BV(m_steps));
  DEBUG_ECHOLNPAIR("target (maximum) feedrate = ", final_feedrate);

  const float feedrate_inc = final_feedrate / 10,   // Start at 1/10 of max & go up by 1/10 per step
              fr_limit = final_feedrate * 0.99f;    // Rounding-safe comparison value
  float current_feedrate = 0;

  planner.synchronize();                            // Wait for moves to complete

  for (j = 0; j < driver_count; j++)
    L64xxManager.get_status(axis_index[j]);         // Clear error flags

  char temp_axis_string[2] = " ";
  temp_axis_string[0] = axis_mon[0][0];             // Need a sprintf format string
  //temp_axis_string[1] = '\n';

  char gcode_string[80];
  uint16_t status_composite = 0;
  DEBUG_ECHOLNPGM(".\n.\n.");                       // Make feedrate outputs easier to read

  do {
    current_feedrate += feedrate_inc;
    DEBUG_ECHOLNPAIR("...feedrate = ", current_feedrate);

    sprintf_P(gcode_string, PSTR("G0 %s%03d F%03d"), temp_axis_string, uint16_t(position_min), uint16_t(current_feedrate));
    gcode.process_subcommands_now_P(gcode_string);

    sprintf_P(gcode_string, PSTR("G0 %s%03d F%03d"), temp_axis_string, uint16_t(position_max), uint16_t(current_feedrate));
    gcode.process_subcommands_now_P(gcode_string);

    planner.synchronize();

    for (j = 0; j < driver_count; j++) {
      axis_status[j] = (~L64xxManager.get_status(axis_index[j])) & 0x0800;  // Bits of interest are all active LOW
      status_composite |= axis_status[j];
    }
    if (status_composite) break;              // Break on any error
  } while (current_feedrate < fr_limit);

  DEBUG_ECHOPGM("Completed with ");
  if (status_composite) {
    DEBUG_ECHOLNPGM("errors");
    #if ENABLED(L6470_CHITCHAT)
      for (j = 0; j < driver_count; j++) {
        if (j) DEBUG_ECHOPGM("...");
        L64xxManager.error_status_decode(axis_status[j], axis_index[j],
          sh.STATUS_AXIS_TH_SD, sh.STATUS_AXIS_TH_WRN,
          sh.STATUS_AXIS_STEP_LOSS_A, sh.STATUS_AXIS_STEP_LOSS_B,
          sh.STATUS_AXIS_OCD, sh.STATUS_AXIS_LAYOUT);
      }
    #endif
  }
  else
    DEBUG_ECHOLNPGM("no errors");

  L64xxManager.pause_monitor(false);
}

#endif // HAS_L64XX
