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

#include "../../inc/MarlinConfig.h"

#if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../lcd/marlinui.h"

#include "../../feature/bedlevel/bedlevel.h"

#if HAS_LEVELING
  #include "../../module/planner.h"
#endif

/**
 * M48: Z probe repeatability measurement function.
 *
 * Usage:
 *   M48 <P#> <X#> <Y#> <V#> <E> <L#> <S>
 *     P = Number of sampled points (4-50, default 10)
 *     X = Sample X position
 *     Y = Sample Y position
 *     V = Verbose level (0-4, default=1)
 *     E = Engage Z probe for each reading
 *     L = Number of legs of movement before probe
 *     S = Schizoid (Or Star if you prefer)
 *
 * This function requires the machine to be homed before invocation.
 */

extern const char SP_Y_STR[];

void GcodeSuite::M48() {

  if (homing_needed_error()) return;

  const int8_t verbose_level = parser.byteval('V', 1);
  if (!WITHIN(verbose_level, 0, 4)) {
    SERIAL_ECHOLNPGM("?(V)erbose level implausible (0-4).");
    return;
  }

  if (verbose_level > 0)
    SERIAL_ECHOLNPGM("M48 Z-Probe Repeatability Test");

  const int8_t n_samples = parser.byteval('P', 10);
  if (!WITHIN(n_samples, 4, 50)) {
    SERIAL_ECHOLNPGM("?Sample size not plausible (4-50).");
    return;
  }

  const ProbePtRaise raise_after = parser.boolval('E') ? PROBE_PT_STOW : PROBE_PT_RAISE;

  // Test at the current position by default, overridden by X and Y
  const xy_pos_t test_position = {
    parser.linearval('X', current_position.x + probe.offset_xy.x),  // If no X use the probe's current X position
    parser.linearval('Y', current_position.y + probe.offset_xy.y)   // If no Y, ditto
  };

  if (!probe.can_reach(test_position)) {
    ui.set_status_P(GET_TEXT(MSG_M48_OUT_OF_BOUNDS), 99);
    SERIAL_ECHOLNPGM("? (X,Y) out of bounds.");
    return;
  }

  // Get the number of leg moves per test-point
  bool seen_L = parser.seen('L');
  uint8_t n_legs = seen_L ? parser.value_byte() : 0;
  if (n_legs > 15) {
    SERIAL_ECHOLNPGM("?Legs of movement implausible (0-15).");
    return;
  }
  if (n_legs == 1) n_legs = 2;

  // Schizoid motion as an optional stress-test
  const bool schizoid_flag = parser.boolval('S');
  if (schizoid_flag && !seen_L) n_legs = 7;

  if (verbose_level > 2)
    SERIAL_ECHOLNPGM("Positioning the probe...");

  // Always disable Bed Level correction before probing...

  #if HAS_LEVELING
    const bool was_enabled = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  // Work with reasonable feedrates
  remember_feedrate_scaling_off();

  // Working variables
  float mean = 0.0,     // The average of all points so far, used to calculate deviation
        sigma = 0.0,    // Standard deviation of all points so far
        min = 99999.9,  // Smallest value sampled so far
        max = -99999.9, // Largest value sampled so far
        sample_set[n_samples];  // Storage for sampled values

  auto dev_report = [](const bool verbose, const float &mean, const float &sigma, const float &min, const float &max, const bool final=false) {
    if (verbose) {
      SERIAL_ECHOPAIR_F("Mean: ", mean, 6);
      if (!final) SERIAL_ECHOPAIR_F(" Sigma: ", sigma, 6);
      SERIAL_ECHOPAIR_F(" Min: ", min, 3);
      SERIAL_ECHOPAIR_F(" Max: ", max, 3);
      SERIAL_ECHOPAIR_F(" Range: ", max-min, 3);
      if (final) SERIAL_EOL();
    }
    if (final) {
      SERIAL_ECHOLNPAIR_F("Standard Deviation: ", sigma, 6);
      SERIAL_EOL();
    }
  };

  // Move to the first point, deploy, and probe
  const float t = probe.probe_at_point(test_position, raise_after, verbose_level);
  bool probing_good = !isnan(t);

  if (probing_good) {
    randomSeed(millis());

    float sample_sum = 0.0;

    LOOP_L_N(n, n_samples) {
      #if HAS_WIRED_LCD
        // Display M48 progress in the status bar
        ui.status_printf_P(0, PSTR(S_FMT ": %d/%d"), GET_TEXT(MSG_M48_POINT), int(n + 1), int(n_samples));
      #endif

      // When there are "legs" of movement move around the point before probing
      if (n_legs) {

        // Pick a random direction, starting angle, and radius
        const int dir = (random(0, 10) > 5.0) ? -1 : 1;  // clockwise or counter clockwise
        float angle = random(0, 360);
        const float radius = random(
          #if ENABLED(DELTA)
            int(0.1250000000 * (DELTA_PRINTABLE_RADIUS)),
            int(0.3333333333 * (DELTA_PRINTABLE_RADIUS))
          #else
            int(5), int(0.125 * _MIN(X_BED_SIZE, Y_BED_SIZE))
          #endif
        );
        if (verbose_level > 3) {
          SERIAL_ECHOPAIR("Start radius:", radius, " angle:", angle, " dir:");
          if (dir > 0) SERIAL_CHAR('C');
          SERIAL_ECHOLNPGM("CW");
        }

        // Move from leg to leg in rapid succession
        LOOP_L_N(l, n_legs - 1) {

          // Move some distance around the perimeter
          float delta_angle;
          if (schizoid_flag) {
            // The points of a 5 point star are 72 degrees apart.
            // Skip a point and go to the next one on the star.
            delta_angle = dir * 2.0 * 72.0;
          }
          else {
            // Just move further along the perimeter.
            delta_angle = dir * (float)random(25, 45);
          }
          angle += delta_angle;

          // Trig functions work without clamping, but just to be safe...
          while (angle > 360.0) angle -= 360.0;
          while (angle < 0.0) angle += 360.0;

          // Choose the next position as an offset to chosen test position
          const xy_pos_t noz_pos = test_position - probe.offset_xy;
          xy_pos_t next_pos = {
            noz_pos.x + float(cos(RADIANS(angle))) * radius,
            noz_pos.y + float(sin(RADIANS(angle))) * radius
          };

          #if ENABLED(DELTA)
            // If the probe can't reach the point on a round bed...
            // Simply scale the numbers to bring them closer to origin.
            while (!probe.can_reach(next_pos)) {
              next_pos *= 0.8f;
              if (verbose_level > 3)
                SERIAL_ECHOLNPAIR_P(PSTR("Moving inward: X"), next_pos.x, SP_Y_STR, next_pos.y);
            }
          #else
            // For a rectangular bed just keep the probe in bounds
            LIMIT(next_pos.x, X_MIN_POS, X_MAX_POS);
            LIMIT(next_pos.y, Y_MIN_POS, Y_MAX_POS);
          #endif

          if (verbose_level > 3)
            SERIAL_ECHOLNPAIR_P(PSTR("Going to: X"), next_pos.x, SP_Y_STR, next_pos.y);

          do_blocking_move_to_xy(next_pos);
        } // n_legs loop
      } // n_legs

      // Probe a single point
      const float pz = probe.probe_at_point(test_position, raise_after, 0);

      // Break the loop if the probe fails
      probing_good = !isnan(pz);
      if (!probing_good) break;

      // Store the new sample
      sample_set[n] = pz;

      // Keep track of the largest and smallest samples
      NOMORE(min, pz);
      NOLESS(max, pz);

      // Get the mean value of all samples thus far
      sample_sum += pz;
      mean = sample_sum / (n + 1);

      // Calculate the standard deviation so far.
      // The value after the last sample will be the final output.
      float dev_sum = 0.0;
      LOOP_LE_N(j, n) dev_sum += sq(sample_set[j] - mean);
      sigma = SQRT(dev_sum / (n + 1));

      if (verbose_level > 1) {
        SERIAL_ECHO(n + 1);
        SERIAL_ECHOPAIR(" of ", int(n_samples));
        SERIAL_ECHOPAIR_F(": z: ", pz, 3);
        SERIAL_CHAR(' ');
        dev_report(verbose_level > 2, mean, sigma, min, max);
        SERIAL_EOL();
      }

    } // n_samples loop
  }

  probe.stow();

  if (probing_good) {
    SERIAL_ECHOLNPGM("Finished!");
    dev_report(verbose_level > 0, mean, sigma, min, max, true);

    #if HAS_WIRED_LCD
      // Display M48 results in the status bar
      char sigma_str[8];
      ui.status_printf_P(0, PSTR(S_FMT ": %s"), GET_TEXT(MSG_M48_DEVIATION), dtostrf(sigma, 2, 6, sigma_str));
    #endif
  }

  restore_feedrate_and_scaling();

  // Re-enable bed level correction if it had been on
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(was_enabled));

  report_current_position();
}

#endif // Z_MIN_PROBE_REPEATABILITY_TEST
