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

#include "../../inc/MarlinConfig.h"

#if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"

#include "../../feature/bedlevel/bedlevel.h"

#if HAS_SPI_LCD
  #include "../../lcd/ultralcd.h"
#endif

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
void GcodeSuite::M48() {

  if (axis_unhomed_error()) return;

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

  xy_float_t next_pos = current_position;

  const xy_pos_t probe_pos = {
    parser.linearval('X', next_pos.x + probe_offset_xy.x),
    parser.linearval('Y', next_pos.y + probe_offset_xy.y)
  };

  if (!position_is_reachable_by_probe(probe_pos)) {
    SERIAL_ECHOLNPGM("? (X,Y) out of bounds.");
    return;
  }

  bool seen_L = parser.seen('L');
  uint8_t n_legs = seen_L ? parser.value_byte() : 0;
  if (n_legs > 15) {
    SERIAL_ECHOLNPGM("?Number of legs in movement not plausible (0-15).");
    return;
  }
  if (n_legs == 1) n_legs = 2;

  const bool schizoid_flag = parser.boolval('S');
  if (schizoid_flag && !seen_L) n_legs = 7;

  /**
   * Now get everything to the specified probe point So we can safely do a
   * probe to get us close to the bed.  If the Z-Axis is far from the bed,
   * we don't want to use that as a starting point for each probe.
   */
  if (verbose_level > 2)
    SERIAL_ECHOLNPGM("Positioning the probe...");

  // Disable bed level correction in M48 because we want the raw data when we probe

  #if HAS_LEVELING
    const bool was_enabled = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  remember_feedrate_scaling_off();

  float mean = 0.0, sigma = 0.0, min = 99999.9, max = -99999.9, sample_set[n_samples];

  // Move to the first point, deploy, and probe
  const float t = probe_at_point(probe_pos, raise_after, verbose_level);
  bool probing_good = !isnan(t);

  if (probing_good) {
    randomSeed(millis());

    for (uint8_t n = 0; n < n_samples; n++) {
      #if HAS_SPI_LCD
        // Display M48 progress in the status bar
        ui.status_printf_P(0, PSTR(S_FMT ": %d/%d"), GET_TEXT(MSG_M48_POINT), int(n + 1), int(n_samples));
      #endif
      if (n_legs) {
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

        for (uint8_t l = 0; l < n_legs - 1; l++) {
          float delta_angle;

          if (schizoid_flag) {
            // The points of a 5 point star are 72 degrees apart.  We need to
            // skip a point and go to the next one on the star.
            delta_angle = dir * 2.0 * 72.0;
          }
          else {
            // If we do this line, we are just trying to move further
            // around the circle.
            delta_angle = dir * (float) random(25, 45);
          }

          angle += delta_angle;
          while (angle > 360.0) angle -= 360.0; // We probably do not need to keep the angle between 0 and 2*PI, but the
                                                // Arduino documentation says the trig functions should not be given values
          while (angle < 0.0) angle += 360.0;   // outside of this range.   It looks like they behave correctly with
                                                // numbers outside of the range, but just to be safe we clamp them.

          const xy_pos_t noz_pos = probe_pos - probe_offset_xy;
          next_pos.set(noz_pos.x + cos(RADIANS(angle)) * radius,
                       noz_pos.y + sin(RADIANS(angle)) * radius);

          #if DISABLED(DELTA)
            LIMIT(next_pos.x, X_MIN_POS, X_MAX_POS);
            LIMIT(next_pos.y, Y_MIN_POS, Y_MAX_POS);
          #else
            // If we have gone out too far, we can do a simple fix and scale the numbers
            // back in closer to the origin.
            while (!position_is_reachable_by_probe(next_pos)) {
              next_pos *= 0.8f;
              if (verbose_level > 3)
                SERIAL_ECHOLNPAIR_P(PSTR("Moving inward: X"), next_pos.x, SP_Y_STR, next_pos.y);
            }
          #endif

          if (verbose_level > 3)
            SERIAL_ECHOLNPAIR_P(PSTR("Going to: X"), next_pos.x, SP_Y_STR, next_pos.y);

          do_blocking_move_to_xy(next_pos);
        } // n_legs loop
      } // n_legs

      // Probe a single point
      sample_set[n] = probe_at_point(probe_pos, raise_after, 0);

      // Break the loop if the probe fails
      probing_good = !isnan(sample_set[n]);
      if (!probing_good) break;

      /**
       * Get the current mean for the data points we have so far
       */
      float sum = 0.0;
      for (uint8_t j = 0; j <= n; j++) sum += sample_set[j];
      mean = sum / (n + 1);

      NOMORE(min, sample_set[n]);
      NOLESS(max, sample_set[n]);

      /**
       * Now, use that mean to calculate the standard deviation for the
       * data points we have so far
       */
      sum = 0.0;
      for (uint8_t j = 0; j <= n; j++)
        sum += sq(sample_set[j] - mean);

      sigma = SQRT(sum / (n + 1));
      if (verbose_level > 0) {
        if (verbose_level > 1) {
          SERIAL_ECHO(n + 1);
          SERIAL_ECHOPAIR(" of ", int(n_samples));
          SERIAL_ECHOPAIR_F(": z: ", sample_set[n], 3);
          if (verbose_level > 2) {
            SERIAL_ECHOPAIR_F(" mean: ", mean, 4);
            SERIAL_ECHOPAIR_F(" sigma: ", sigma, 6);
            SERIAL_ECHOPAIR_F(" min: ", min, 3);
            SERIAL_ECHOPAIR_F(" max: ", max, 3);
            SERIAL_ECHOPAIR_F(" range: ", max-min, 3);
          }
          SERIAL_EOL();
        }
      }

    } // n_samples loop
  }

  STOW_PROBE();

  if (probing_good) {
    SERIAL_ECHOLNPGM("Finished!");

    if (verbose_level > 0) {
      SERIAL_ECHOPAIR_F("Mean: ", mean, 6);
      SERIAL_ECHOPAIR_F(" Min: ", min, 3);
      SERIAL_ECHOPAIR_F(" Max: ", max, 3);
      SERIAL_ECHOLNPAIR_F(" Range: ", max-min, 3);
    }

    SERIAL_ECHOLNPAIR_F("Standard Deviation: ", sigma, 6);
    SERIAL_EOL();

    #if HAS_SPI_LCD
      // Display M48 results in the status bar
      char sigma_str[8];
      ui.status_printf_P(0, PSTR(S_FMT ": %s"), GET_TEXT(MSG_M48_DEVIATION), dtostrf(sigma, 2, 6, sigma_str));
    #endif
  }

  restore_feedrate_and_scaling();

  // Re-enable bed level correction if it had been on
  #if HAS_LEVELING
    set_bed_leveling_enabled(was_enabled);
  #endif

  report_current_position();
}

#endif // Z_MIN_PROBE_REPEATABILITY_TEST
