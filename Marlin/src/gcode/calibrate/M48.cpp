/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"


#include "../../feature/bedlevel/bedlevel.h"


#if HAS_LEVELING
  #include "../../module/planner.h"
#endif

/**
 * M48: Z probe repeatability measurement function.
 *
 * Usage:
 *   M48 <P#> <X#> <Y#> <V#> <E> <L#>
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
    SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-4).");
    return;
  }

  if (verbose_level > 0)
    SERIAL_PROTOCOLLNPGM("M48 Z-Probe Repeatability Test");

  const int8_t n_samples = parser.byteval('P', 10);
  if (!WITHIN(n_samples, 4, 50)) {
    SERIAL_PROTOCOLLNPGM("?Sample size not plausible (4-50).");
    return;
  }

  const bool stow_probe_after_each = parser.boolval('E');

  float X_current = current_position[X_AXIS],
        Y_current = current_position[Y_AXIS];

  const float X_probe_location = parser.linearval('X', X_current + X_PROBE_OFFSET_FROM_EXTRUDER),
              Y_probe_location = parser.linearval('Y', Y_current + Y_PROBE_OFFSET_FROM_EXTRUDER);

  #if DISABLED(DELTA)
    if (!WITHIN(X_probe_location, MIN_PROBE_X, MAX_PROBE_X)) {
      out_of_range_error(PSTR("X"));
      return;
    }
    if (!WITHIN(Y_probe_location, MIN_PROBE_Y, MAX_PROBE_Y)) {
      out_of_range_error(PSTR("Y"));
      return;
    }
  #else
    if (!position_is_reachable_by_probe(X_probe_location, Y_probe_location)) {
      SERIAL_PROTOCOLLNPGM("? (X,Y) location outside of probeable radius.");
      return;
    }
  #endif

  bool seen_L = parser.seen('L');
  uint8_t n_legs = seen_L ? parser.value_byte() : 0;
  if (n_legs > 15) {
    SERIAL_PROTOCOLLNPGM("?Number of legs in movement not plausible (0-15).");
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
    SERIAL_PROTOCOLLNPGM("Positioning the probe...");

  // Disable bed level correction in M48 because we want the raw data when we probe

  #if HAS_LEVELING
    const bool was_enabled = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  setup_for_endstop_or_probe_move();

  double mean = 0.0, sigma = 0.0, min = 99999.9, max = -99999.9, sample_set[n_samples];

  // Move to the first point, deploy, and probe
  const float t = probe_pt(X_probe_location, Y_probe_location, stow_probe_after_each, verbose_level);
  bool probing_good = !isnan(t);

  if (probing_good) {
    randomSeed(millis());

    for (uint8_t n = 0; n < n_samples; n++) {
      if (n_legs) {
        const int dir = (random(0, 10) > 5.0) ? -1 : 1;  // clockwise or counter clockwise
        float angle = random(0, 360);
        const float radius = random(
          #if ENABLED(DELTA)
            (int) (0.1250000000 * (DELTA_PROBEABLE_RADIUS)),
            (int) (0.3333333333 * (DELTA_PROBEABLE_RADIUS))
          #else
            (int) 5.0, (int) (0.125 * min(X_BED_SIZE, Y_BED_SIZE))
          #endif
        );

        if (verbose_level > 3) {
          SERIAL_ECHOPAIR("Starting radius: ", radius);
          SERIAL_ECHOPAIR("   angle: ", angle);
          SERIAL_ECHOPGM(" Direction: ");
          if (dir > 0) SERIAL_ECHOPGM("Counter-");
          SERIAL_ECHOLNPGM("Clockwise");
        }

        for (uint8_t l = 0; l < n_legs - 1; l++) {
          double delta_angle;

          if (schizoid_flag)
            // The points of a 5 point star are 72 degrees apart.  We need to
            // skip a point and go to the next one on the star.
            delta_angle = dir * 2.0 * 72.0;

          else
            // If we do this line, we are just trying to move further
            // around the circle.
            delta_angle = dir * (float) random(25, 45);

          angle += delta_angle;

          while (angle > 360.0)   // We probably do not need to keep the angle between 0 and 2*PI, but the
            angle -= 360.0;       // Arduino documentation says the trig functions should not be given values
          while (angle < 0.0)     // outside of this range.   It looks like they behave correctly with
            angle += 360.0;       // numbers outside of the range, but just to be safe we clamp them.

          X_current = X_probe_location - (X_PROBE_OFFSET_FROM_EXTRUDER) + cos(RADIANS(angle)) * radius;
          Y_current = Y_probe_location - (Y_PROBE_OFFSET_FROM_EXTRUDER) + sin(RADIANS(angle)) * radius;

          #if DISABLED(DELTA)
            X_current = constrain(X_current, X_MIN_POS, X_MAX_POS);
            Y_current = constrain(Y_current, Y_MIN_POS, Y_MAX_POS);
          #else
            // If we have gone out too far, we can do a simple fix and scale the numbers
            // back in closer to the origin.
            while (!position_is_reachable_by_probe(X_current, Y_current)) {
              X_current *= 0.8;
              Y_current *= 0.8;
              if (verbose_level > 3) {
                SERIAL_ECHOPAIR("Pulling point towards center:", X_current);
                SERIAL_ECHOLNPAIR(", ", Y_current);
              }
            }
          #endif
          if (verbose_level > 3) {
            SERIAL_PROTOCOLPGM("Going to:");
            SERIAL_ECHOPAIR(" X", X_current);
            SERIAL_ECHOPAIR(" Y", Y_current);
            SERIAL_ECHOLNPAIR(" Z", current_position[Z_AXIS]);
          }
          do_blocking_move_to_xy(X_current, Y_current);
        } // n_legs loop
      } // n_legs

      // Probe a single point
      sample_set[n] = probe_pt(X_probe_location, Y_probe_location, stow_probe_after_each, 0);

      // Break the loop if the probe fails
      probing_good = !isnan(sample_set[n]);
      if (!probing_good) break;

      /**
       * Get the current mean for the data points we have so far
       */
      double sum = 0.0;
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
          SERIAL_PROTOCOL(n + 1);
          SERIAL_PROTOCOLPGM(" of ");
          SERIAL_PROTOCOL((int)n_samples);
          SERIAL_PROTOCOLPGM(": z: ");
          SERIAL_PROTOCOL_F(sample_set[n], 3);
          if (verbose_level > 2) {
            SERIAL_PROTOCOLPGM(" mean: ");
            SERIAL_PROTOCOL_F(mean, 4);
            SERIAL_PROTOCOLPGM(" sigma: ");
            SERIAL_PROTOCOL_F(sigma, 6);
            SERIAL_PROTOCOLPGM(" min: ");
            SERIAL_PROTOCOL_F(min, 3);
            SERIAL_PROTOCOLPGM(" max: ");
            SERIAL_PROTOCOL_F(max, 3);
            SERIAL_PROTOCOLPGM(" range: ");
            SERIAL_PROTOCOL_F(max-min, 3);
          }
          SERIAL_EOL();
        }
      }

    } // n_samples loop
  }

  STOW_PROBE();

  if (probing_good) {
    SERIAL_PROTOCOLLNPGM("Finished!");

    if (verbose_level > 0) {
      SERIAL_PROTOCOLPGM("Mean: ");
      SERIAL_PROTOCOL_F(mean, 6);
      SERIAL_PROTOCOLPGM(" Min: ");
      SERIAL_PROTOCOL_F(min, 3);
      SERIAL_PROTOCOLPGM(" Max: ");
      SERIAL_PROTOCOL_F(max, 3);
      SERIAL_PROTOCOLPGM(" Range: ");
      SERIAL_PROTOCOL_F(max-min, 3);
      SERIAL_EOL();
    }

    SERIAL_PROTOCOLPGM("Standard Deviation: ");
    SERIAL_PROTOCOL_F(sigma, 6);
    SERIAL_EOL();
    SERIAL_EOL();
  }

  clean_up_after_endstop_or_probe_move();

  // Re-enable bed level correction if it had been on
  #if HAS_LEVELING
    set_bed_leveling_enabled(was_enabled);
  #endif

  report_current_position();
}

#endif // Z_MIN_PROBE_REPEATABILITY_TEST
