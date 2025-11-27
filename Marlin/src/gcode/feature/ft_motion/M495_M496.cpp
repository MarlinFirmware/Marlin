/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(FTM_RESONANCE_TEST)

#include "../../gcode.h"
#include "../../../module/ft_motion.h"
#include "../../../module/ft_motion/resonance_generator.h"

void say_resonance_test() {
  const ftm_resonance_test_params_t &p = ftMotion.rtg.rt_params;
  SERIAL_ECHO_START();
  SERIAL_ECHOLN(F("M495 "), F("Resonance Test"));
  SERIAL_ECHOLNPGM("  Axis: ", p.axis == NO_AXIS_ENUM ? C('-') : C(AXIS_CHAR(p.axis)));
  SERIAL_ECHOLNPGM("  Freq Range (F..T): ", p.min_freq, " .. ", p.max_freq, " Hz");
  SERIAL_ECHOLNPGM("  Octave Duration (O): ", p.octave_duration, " s");
  SERIAL_ECHOLNPGM("  Accel/Hz (A): ", p.accel_per_hz);
}

/**
 * M495: Configure and run the resonance test.
 *       With no parameters report the current settings.
 *
 * Parameters:
 *   A<accel/Hz>   Accel per Hz. (Default 60)
 *   F<Hz>         Start frequency. (Default 5.0)
 *   S             Start the test.
 *   T<Hz>         End frequency. (Default 100.0f)
 *   O<float>      Octave duration for logarithmic progression
 *   C<int>        Amplitude correction factor. (Default 5)
 *   X             Flag to select the X axis.
 *   Y             Flag to select the Y axis.
 *   Z             Flag to select the Z axis.
 *   H<float>      Get the Resonance Frequency from Timeline value. (Default 0)
 *
 * Examples:
 *   M495 S       : Start the test with default or last-used parameters
 *   M495 X S     : Start the test on the X axis with default or last-used parameters
 *   M495 H<val>  : Get Resonance Frequency from Timeline value
 *
 */
void GcodeSuite::M495() {
  if (!parser.seen_any()) return say_resonance_test();

  ftm_resonance_test_params_t &p = ftMotion.rtg.rt_params;

  const bool seenX = parser.seen_test('X'), seenY = parser.seen_test('Y'), seenZ = parser.seen_test('Z');

  if (seenX + seenY + seenZ == 1) {
    const AxisEnum a = seenX ? X_AXIS : seenY ? Y_AXIS : Z_AXIS;
    p.axis = a;
    SERIAL_ECHOLN(C(AXIS_CHAR(a)), F("-axis selected"), F(" for "), F("Resonance Test"));
  }
  else if (seenX + seenY + seenZ > 1) {
    SERIAL_ECHOLN(F("?Specify X, Y, or Z axis"), F(" for "), F("Resonance Test"));
    return;
  }

  if (parser.seenval('A')) {
    const float val = parser.value_float();
    if (p.axis == Z_AXIS && val > 15.0f) {
      p.accel_per_hz = 15.0f;
      SERIAL_ECHOLNPGM("Accel/Hz set to max 15 mm/s for Z Axis");
    }
    else {
      p.accel_per_hz = val;
      SERIAL_ECHOLNPGM("Accel/Hz set to ", p.accel_per_hz);
    }
  }

  if (parser.seenval('F')) {
    const float val = parser.value_float();
    if (val >= 5.0f) {
      p.min_freq = val;
      SERIAL_ECHOLNPGM("Start Frequency set to ", p.min_freq, " Hz");
    }
    else {
      SERIAL_ECHOLN(F("?Invalid "), F("Start [F]requency. (minimum 5.0 Hz)"));
    }
  }

  if (parser.seenval('T')) {
    const float val = parser.value_float();
    if (val > p.min_freq && val <= 200.0f) {
      p.max_freq = val;
      SERIAL_ECHOLNPGM("End Frequency set to ", p.max_freq, " Hz");
    }
    else {
      SERIAL_ECHOLN(F("?Invalid "), F("End Frequency [T]. (StartFreq .. 200 Hz)"));
    }
  }

  if (parser.seenval('O')) {
    const float val = parser.value_float();
    if (WITHIN(val, 20, 60)) {
      p.octave_duration = val;
      SERIAL_ECHOLNPGM("Octave Duration set to ",p.octave_duration, " s");
    }
    else {
      SERIAL_ECHOLN(F("?Invalid "), F("octave duration [O]. (20..60 s)"));
    }
  }

  if (parser.seenval('C')) {
    const int val = parser.value_int();
    if (WITHIN(val, 1, 8)) {
      p.amplitude_correction = val;
      SERIAL_ECHOLNPGM("Amplitude Correction Factor set to ", p.amplitude_correction);
    }
    else {
      SERIAL_ECHOLN(F("?Invalid "), F("Amplitude [C]orrection Factor. (1..8)"));
    }
  }

  if (parser.seenval('G')) {
    const float val = parser.value_float();
    if (WITHIN(val, 0, 100)) {
      ftMotion.rtg.timeline = val;
      SERIAL_ECHOLNPGM("Resonance Frequency set to ", ftMotion.rtg.getFrequencyFromTimeline(), " Hz");
    }
    else {
      SERIAL_ECHOLN(F("?Invalid "), F("Timeline value (0..100 s)"));
    }
  }

  if (parser.seen_test('S')) {
    if (ftMotion.cfg.active) {
      if (p.axis != NO_AXIS_ENUM) {
        if (p.max_freq > p.min_freq) {
          SERIAL_ECHOLN(F("Starting "), F("Resonance Test"));
          ftMotion.start_resonance_test();
          // The function returns immediately, the test runs in the background.
        }
        else {
          SERIAL_ECHOLNPGM("?End Frequency must be greater than Start Frequency");
        }
      }
      else {
        SERIAL_ECHOLN(F("?Specify X, Y, or Z axis"), F(" first"));
      }
    }
    else {
      SERIAL_ECHOLN(F("?Activate FT Motion to run the "), F("Resonance Test"));
    }
  }
}

/**
 * M496: Abort the resonance test (via Emergency Parser)
 */
void GcodeSuite::M496() {
  if (ftMotion.rtg.isActive()) {
      ftMotion.rtg.abort();
      EmergencyParser::rt_stop_by_M496 = false;
      #if DISABLED(MARLIN_SMALL_BUILD)
        SERIAL_ECHOLN(F("Resonance Test"), F(" aborted."));
      #endif
      return;
  }
  #if DISABLED(MARLIN_SMALL_BUILD)
    SERIAL_ECHOLN(F("No active "), F("Resonance Test"), F(" to abort."));
  #endif
}

#endif // FTM_RESONANCE_TEST
