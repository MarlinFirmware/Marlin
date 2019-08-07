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

#include "../../../inc/MarlinConfig.h"

#if HAS_DRIVER(L6470)

#include "../../gcode.h"
#include "../../../module/stepper_indirection.h"
#include "../../../module/planner.h"
#include "../../../libs/L6470/L6470_Marlin.h"

#define DEBUG_OUT ENABLED(L6470_CHITCHAT)
#include "../../../core/debug_out.h"

static void jiggle_axis(const char axis_char, const float &min, const float &max, const float &rate) {
  char gcode_string[30], str1[11], str2[11];

  // Turn the motor(s) both directions
  sprintf_P(gcode_string, PSTR("G0 %c%s F%s"), axis_char, dtostrf(min, 1, 3, str1), dtostrf(rate, 1, 3, str2));
  process_subcommands_now(gcode_string);

  sprintf_P(gcode_string, PSTR("G0 %c%s F%s"), axis_char, dtostrf(max, 1, 3, str1), str2);
  process_subcommands_now(gcode_string);

  planner.synchronize();
}

/**
 *
 * M916: Increase KVAL_HOLD until thermal warning
 *
 *
 * J - select which driver(s) to monitor on multi-driver axis
 *     0 - (default) monitor all drivers on the axis or E0
 *     1 - monitor only X, Y, Z, E1
 *     2 - monitor only X2, Y2, Z2, E2
 *     3 - monitor only Z3, E3
 *
 * Xxxx, Yxxx, Zxxx, Exxx - axis to be monitored with displacement
 *     xxx (1-255) is distance moved on either side of current position
 *
 * F - feedrate
 *     optional - will use default max feedrate from configuration.h if not specified
 *
 * K - starting value for KVAL_HOLD (0 - 255)
 *     optional - will use & report current value from driver if not specified
 *
 */

/**
 * This routine is also useful for determining the approximate KVAL_HOLD
 * where the stepper stops losing steps. The sound will get noticeably quieter
 * as it stops losing steps.
 */

void GcodeSuite::M916() {

  DEBUG_ECHOLNPGM("M916");

  // Variables used by L6470_get_user_input function - some may not be used
  char axis_mon[3][3] = { "  ", "  ", "  " };  // list of Axes to be monitored
  uint8_t axis_index[3];
  uint16_t axis_status[3];
  uint8_t driver_count = 1;
  float position_max;
  float position_min;
  float final_feedrate;
  uint8_t kval_hold;
  uint8_t ocd_th_val = 0;
  uint8_t stall_th_val = 0;
  uint16_t over_current_threshold;
  constexpr bool over_current_flag = false;  // M916 doesn't play with the overcurrent thresholds

  uint8_t j;   // general purpose counter

  if (L6470.get_user_input(driver_count, axis_index, axis_mon, position_max, position_min, final_feedrate, kval_hold, over_current_flag, ocd_th_val, stall_th_val, over_current_threshold))
    return;  // quit if invalid user input

  DEBUG_ECHOLNPAIR("feedrate = ", final_feedrate);

  planner.synchronize();                  // Wait for moves to finish

  for (j = 0; j < driver_count; j++)
    L6470.get_status(axis_index[j]);      // Clear out error flags

  uint16_t status_composite = 0;

  DEBUG_ECHOLNPGM(".\n.");

  do {

    DEBUG_ECHOLNPAIR("kval_hold = ", kval_hold);   // set & report KVAL_HOLD for this run

    for (j = 0; j < driver_count; j++)
      L6470.set_param(axis_index[j], L6470_KVAL_HOLD, kval_hold);

    // Turn the motor(s) both directions
    jiggle_axis(axis_mon[0][0], position_min, position_max, final_feedrate);

    status_composite = 0;    // clear out the old bits

    for (j = 0; j < driver_count; j++) {
      axis_status[j] = (~L6470.get_status(axis_index[j])) & L6470_ERROR_MASK;    // bits of interest are all active low
      status_composite |= axis_status[j] ;
    }

    if (status_composite && (status_composite & STATUS_UVLO)) {
      DEBUG_ECHOLNPGM("Test aborted (Undervoltage lockout active)");
      for (j = 0; j < driver_count; j++) {
        DEBUG_ECHOPGM("...");
        L6470.error_status_decode(axis_status[j], axis_index[j]);
      }
      return;
    }

    // increment KVAL_HOLD if not yet at thermal warning/shutdown
    if (!(status_composite & (STATUS_TH_WRN | STATUS_TH_SD)))
      kval_hold++;

  } while (!(status_composite & (STATUS_TH_WRN | STATUS_TH_SD)) && kval_hold);  // exit when kval_hold == 0 (rolls over)

  DEBUG_ECHOPGM(".\n.\nThermal warning/shutdown ");
  if ((status_composite & (STATUS_TH_WRN | STATUS_TH_SD))) {
    DEBUG_ECHOLNPGM("has occurred");
    for (j = 0; j < driver_count; j++) {
      DEBUG_ECHOPGM("...");
      L6470.error_status_decode(axis_status[j], axis_index[j]);
    }
  }
  else
    DEBUG_ECHOLNPGM("(Unable to get)");

  DEBUG_ECHOLNPGM(".");
}

/**
 *
 * M917: Find minimum current thresholds
 *
 *   Decrease OCD current until overcurrent error
 *   Increase OCD until overcurrent error goes away
 *   Decrease stall threshold until stall
 *   Increase stall until stall error goes away
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
 * K - value for KVAL_HOLD (0 - 255)
 *     optional - will report current value from driver if not specified
 *
 */
void GcodeSuite::M917() {

  DEBUG_ECHOLNPGM("M917");

  char axis_mon[3][3] = { "  ", "  ", "  " };  // list of axes to be monitored
  uint8_t axis_index[3];
  uint16_t axis_status[3];
  uint8_t driver_count = 1;
  float position_max;
  float position_min;
  float final_feedrate;
  uint8_t kval_hold;
  uint8_t ocd_th_val = 0;
  uint8_t stall_th_val = 0;
  uint16_t over_current_threshold;
  constexpr bool over_current_flag = true;

  uint8_t j;   // general purpose counter

  if (L6470.get_user_input(driver_count, axis_index, axis_mon, position_max, position_min, final_feedrate, kval_hold, over_current_flag, ocd_th_val, stall_th_val, over_current_threshold))
    return;  // quit if invalid user input

  DEBUG_ECHOLNPAIR("feedrate = ", final_feedrate);

  planner.synchronize();                // Wait for moves to finish
  for (j = 0; j < driver_count; j++)
    L6470.get_status(axis_index[j]);    // Clear out error flags
  uint16_t status_composite = 0;
  uint8_t test_phase = 0;
        // 0 - decreasing OCD - exit when OCD warning occurs (ignore STALL)
        // 1 - increasing OCD - exit when OCD warning stops (ignore STALL) -
        // 2 - OCD finalized - decreasing STALL - exit when STALL warning happens
        // 3 - OCD finalized - increasing STALL - exit when STALL warning stop
        // 4 - all testing completed
  DEBUG_ECHOPAIR(".\n.\n.\nover_current threshold : ", (ocd_th_val + 1) * 375);   // first status display
  DEBUG_ECHOPAIR("  (OCD_TH:  : ", ocd_th_val);
  DEBUG_ECHOPAIR(")   Stall threshold: ", (stall_th_val + 1) * 31.25);
  DEBUG_ECHOPAIR("  (STALL_TH: ", stall_th_val);
  DEBUG_ECHOLNPGM(")");

  do {

    DEBUG_ECHOPAIR("STALL threshold : ", (stall_th_val + 1) * 31.25);
    DEBUG_ECHOLNPAIR("   OCD threshold : ", (ocd_th_val + 1) * 375);

    jiggle_axis(axis_mon[0][0], position_min, position_max, final_feedrate);

    status_composite = 0;    // clear out the old bits

    for (j = 0; j < driver_count; j++) {
      axis_status[j] = (~L6470.get_status(axis_index[j])) & L6470_ERROR_MASK;    // bits of interest are all active low
      status_composite |= axis_status[j];
    }

    if (status_composite && (status_composite & STATUS_UVLO)) {
      DEBUG_ECHOLNPGM("Test aborted (Undervoltage lockout active)");
      for (j = 0; j < driver_count; j++) {
        DEBUG_ECHOPGM("...");
        L6470.error_status_decode(axis_status[j], axis_index[j]);
      }
      return;
    }

    if (status_composite & (STATUS_TH_WRN | STATUS_TH_SD)) {
      DEBUG_ECHOLNPGM("thermal problem - waiting for chip(s) to cool down ");
      uint16_t status_composite_temp = 0;
      uint8_t k = 0;
      do {
        k++;
        if (!(k % 4)) {
          kval_hold *= 0.95;
          L6470_EOL();
          DEBUG_ECHOLNPAIR("Lowering KVAL_HOLD by about 5% to ", kval_hold);
          for (j = 0; j < driver_count; j++)
            L6470.set_param(axis_index[j], L6470_KVAL_HOLD, kval_hold);
        }
        DEBUG_ECHOLNPGM(".");
        reset_stepper_timeout(); // reset_stepper_timeout to keep steppers powered
        watchdog_reset();   // beat the dog
        safe_delay(5000);
        status_composite_temp = 0;
        for (j = 0; j < driver_count; j++) {
          axis_status[j] = (~L6470.get_status(axis_index[j])) & L6470_ERROR_MASK;    // bits of interest are all active low
          status_composite_temp |= axis_status[j];
        }
      }
      while (status_composite_temp & (STATUS_TH_WRN | STATUS_TH_SD));
      L6470_EOL();
    }
    if (status_composite & (STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B | STATUS_OCD)) {
      switch (test_phase) {

        case 0: {
          if (status_composite & STATUS_OCD) {
            // phase 0 with OCD warning - time to go to next phase
            if (ocd_th_val >=15) {
              ocd_th_val = 15;           // limit to max
              test_phase = 2;            // at highest value so skip phase 1
              DEBUG_ECHOLNPGM("LOGIC E0A OCD at highest - skip to 2");
            }
            else {
              ocd_th_val++;              // normal exit to next phase
              test_phase = 1;            // setup for first pass of phase 1
              DEBUG_ECHOLNPGM("LOGIC E0B - inc OCD  & go to 1");
            }
          }
          else {  // phase 0 without OCD warning - keep on decrementing if can
            if (ocd_th_val) {
              ocd_th_val--;              // try lower value
              DEBUG_ECHOLNPGM("LOGIC E0C - dec OCD");
            }
            else {
              test_phase = 2;            // at lowest value without warning so skip phase 1
              DEBUG_ECHOLNPGM("LOGIC E0D - OCD at latest - go to 2");
            }
          }
        } break;

        case 1: {
          if (status_composite & STATUS_OCD) {
            // phase 1 with OCD warning - increment if can
            if (ocd_th_val >= 15) {
              ocd_th_val = 15;           // limit to max
              test_phase = 2;            // at highest value so go to next phase
              DEBUG_ECHOLNPGM("LOGIC E1A - OCD at max - go to 2");
            }
            else {
              ocd_th_val++;              // try a higher value
              DEBUG_ECHOLNPGM("LOGIC E1B - inc OCD");
            }
          }
          else { // phase 1 without OCD warning - normal exit to phase 2
            test_phase = 2;
            DEBUG_ECHOLNPGM("LOGIC E1C - no OCD warning - go to 1");
          }
        } break;

        case 2: {
          if (status_composite & (STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B)) {
            // phase 2 with stall warning - time to go to next phase
            if (stall_th_val >= 127) {
              stall_th_val = 127;  // limit to max
              DEBUG_ECHOLNPGM("LOGIC E2A - STALL warning, STALL at max, quit");
              DEBUG_ECHOLNPGM("finished - STALL at maximum value but still have stall warning");
              test_phase = 4;
            }
            else {
              test_phase = 3;              // normal exit to next phase (found failing value of STALL)
              stall_th_val++;              // setup for first pass of phase 3
              DEBUG_ECHOLNPGM("LOGIC E2B - INC - STALL warning, inc Stall, go to 3");
            }
          }
          else {  // phase 2 without stall warning - decrement if can
            if (stall_th_val) {
              stall_th_val--;              // try a lower value
              DEBUG_ECHOLNPGM("LOGIC E2C - no STALL, dec STALL");
            }
            else {
              DEBUG_ECHOLNPGM("finished - STALL at lowest value but still do NOT have stall warning");
              test_phase = 4;
              DEBUG_ECHOLNPGM("LOGIC E2D - no STALL, at lowest so quit");
            }
          }
        } break;

        case 3: {
          if (status_composite & (STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B)) {
            // phase 3 with stall warning - increment if can
            if (stall_th_val >= 127) {
              stall_th_val = 127; // limit to max
              DEBUG_ECHOLNPGM("finished - STALL at maximum value but still have stall warning");
              test_phase = 4;
              DEBUG_ECHOLNPGM("LOGIC E3A - STALL, at max so quit");
            }
            else {
              stall_th_val++;              // still looking for passing value
              DEBUG_ECHOLNPGM("LOGIC E3B - STALL, inc stall");
            }
          }
          else {  //phase 3 without stall warning  but have OCD warning
            DEBUG_ECHOLNPGM("Hardware problem - OCD warning without STALL warning");
            test_phase = 4;
            DEBUG_ECHOLNPGM("LOGIC E3C - not STALLED, hardware problem (quit)");
          }
        } break;

      }

    }
    else {
      switch (test_phase) {
        case 0: { // phase 0 without OCD warning - keep on decrementing if can
          if (ocd_th_val) {
            ocd_th_val--;             // try lower value
            DEBUG_ECHOLNPGM("LOGIC N0A - DEC OCD");
          }
          else {
            test_phase = 2;           // at lowest value without warning so skip phase 1
            DEBUG_ECHOLNPGM("LOGIC N0B - OCD at lowest (go to phase 2)");
          }
        } break;

        case 1: DEBUG_ECHOLNPGM("LOGIC N1 (go directly to 2)"); // phase 1 without OCD warning - drop directly to phase 2

        case 2: { // phase 2 without stall warning - keep on decrementing if can
          if (stall_th_val) {
            stall_th_val--;              // try a lower value (stay in phase 2)
            DEBUG_ECHOLNPGM("LOGIC N2B - dec STALL");
          }
          else {
            DEBUG_ECHOLNPGM("finished - STALL at lowest value but still no stall warning");
            test_phase = 4;
            DEBUG_ECHOLNPGM("LOGIC N2C - STALL at lowest (quit)");
          }
        } break;

        case 3: { test_phase = 4;
           DEBUG_ECHOLNPGM("LOGIC N3 - finished!");
        } break;  // phase 3 without any warnings - desired exit
      }  //
    }  // end of status checks

    if (test_phase != 4) {
      for (j = 0; j < driver_count; j++) {                       // update threshold(s)
        L6470.set_param(axis_index[j], L6470_OCD_TH, ocd_th_val);
        L6470.set_param(axis_index[j], L6470_STALL_TH, stall_th_val);
        if (L6470.get_param(axis_index[j], L6470_OCD_TH) != ocd_th_val) DEBUG_ECHOLNPGM("OCD mismatch");
        if (L6470.get_param(axis_index[j], L6470_STALL_TH) != stall_th_val) DEBUG_ECHOLNPGM("STALL mismatch");
      }
    }

  } while (test_phase != 4);

  if (status_composite) {
    DEBUG_ECHOLNPGM("Completed with errors");
    for (j = 0; j < driver_count; j++) {
      DEBUG_ECHOPGM("...");
      L6470.error_status_decode(axis_status[j], axis_index[j]);
    }
  }
  else
    DEBUG_ECHOLNPGM("Completed with no errors");

} // M917

/**
 *
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
 * K - value for KVAL_HOLD (0 - 255) (optional)
 *     optional - will report current value from driver if not specified
 *
 */
void GcodeSuite::M918() {

  DEBUG_ECHOLNPGM("M918");

  char axis_mon[3][3] = { "  ", "  ", "  " };  // List of axes to monitor
  uint8_t axis_index[3];
  uint16_t axis_status[3];
  uint8_t driver_count = 1;
  float position_max, position_min;
  float final_feedrate;
  uint8_t kval_hold;
  uint8_t ocd_th_val = 0;
  uint8_t stall_th_val = 0;
  uint16_t over_current_threshold;
  constexpr bool over_current_flag = true;

  uint8_t j;   // general purpose counter

  if (L6470.get_user_input(driver_count, axis_index, axis_mon, position_max, position_min, final_feedrate, kval_hold, over_current_flag, ocd_th_val, stall_th_val, over_current_threshold))
    return;  // quit if invalid user input

  uint8_t m_steps = parser.byteval('M');
  LIMIT(m_steps, 0, 128);
  DEBUG_ECHOLNPAIR("M = ", m_steps);

  int8_t m_bits = -1;
       if (m_steps > 85) m_bits = 7;  // 128 (no synch output)
  else if (m_steps > 42) m_bits = 6;  //  64 (no synch output)
  else if (m_steps > 22) m_bits = 5;  //  32 (no synch output)
  else if (m_steps > 12) m_bits = 4;  //  16 (no synch output)
  else if (m_steps >  5) m_bits = 3;  //   8 (no synch output)
  else if (m_steps >  2) m_bits = 2;  //   4 (no synch output)
  else if (m_steps == 2) m_bits = 1;  //   2 (no synch output)
  else if (m_steps == 1) m_bits = 0;  //   1 (no synch output)
  else if (m_steps == 0) m_bits = 7;  // 128 (no synch output)

  if (m_bits >= 0) {
    const int micros = _BV(m_bits);
    if (micros < 100) { DEBUG_CHAR(' '); if (micros < 10) DEBUG_CHAR(' '); }
    DEBUG_ECHO(micros);
    DEBUG_ECHOPGM(" uSTEPS");
  }

  for (j = 0; j < driver_count; j++)
    L6470.set_param(axis_index[j], L6470_STEP_MODE, m_bits);   // set microsteps

  DEBUG_ECHOLNPAIR("target (maximum) feedrate = ",final_feedrate);

  float feedrate_inc = final_feedrate / 10, // start at 1/10 of max & go up by 1/10 per step)
        current_feedrate = 0;

  planner.synchronize();                  // Wait for moves to finish

  for (j = 0; j < driver_count; j++)
    L6470.get_status(axis_index[j]);      // Clear all error flags

  uint16_t status_composite = 0;
  DEBUG_ECHOLNPGM(".\n.\n.");             // Make the feedrate prints easier to see

  do {
    current_feedrate += feedrate_inc;
    DEBUG_ECHOLNPAIR("...feedrate = ", current_feedrate);

    jiggle_axis(axis_mon[0][0], position_min, position_max, current_feedrate);

    for (j = 0; j < driver_count; j++) {
      axis_status[j] = (~L6470.get_status(axis_index[j])) & 0x0800;    // bits of interest are all active low
      status_composite |= axis_status[j];
    }
    if (status_composite) break;       // quit if any errors flags are raised
  } while (current_feedrate < final_feedrate * 0.99);

  DEBUG_ECHOPGM("Completed with errors");
  if (status_composite) {
    DEBUG_ECHOLNPGM("errors");
    for (j = 0; j < driver_count; j++) {
      DEBUG_ECHOPGM("...");
      L6470.error_status_decode(axis_status[j], axis_index[j]);
    }
  }
  else
    DEBUG_ECHOLNPGM("no errors");

} // M918

#endif // HAS_DRIVER(L6470)
