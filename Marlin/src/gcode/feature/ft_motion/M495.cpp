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
#include "../../../module/ft_motion/trajectory_resonance.h"

void say_resonance_test() {
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("M495 Resonance Test Parameters:");
  SERIAL_ECHOLNPGM("  Axis: ", !ftMotion.rtg->rt_params.axis ? C('X') : ftMotion.rtg->rt_params.axis == 1 ? C('Y') : C('Z'), "-axis");
  SERIAL_ECHOLNPGM("  Freq Range (F..T): ", ftMotion.rtg->rt_params.min_freq, " .. ", ftMotion.rtg->rt_params.max_freq, " Hz");
  SERIAL_ECHOLNPGM("  Rate (R): ", ftMotion.rtg->rt_params.hz_per_sec, " Hz/s");
  SERIAL_ECHOLNPGM("  Accel/Hz (A): ", ftMotion.rtg->rt_params.accel_per_hz);
  SERIAL_ECHOLNPGM("Use S to start the test with default values or with last set parameters");
  SERIAL_ECHOLNPGM("Use X/Y/Z S to start the test on X/Y/Z axis"); 
}

/**
 * M495: Configure and run the resonance test.
 *       With no parameters report the current settings.
 *
 * Parameters:
 *   A<accel/Hz>   Accel per Hz. (Default 60)
 *   F<Hz>         Start frequency. (Default 5.0)
 *   R<Hz/s>       Frequency increase rate. (Default 1.0)
 *   S             Start the test.
 *   T<Hz>         End frequency. (Default 100.0f)
 *   C<int>        Amplitude correction factor. (Default 2)
 *   X             Flag to select the X axis.
 *   Y             Flag to select the Y axis.
 *   Z             Flag to select the Z axis.
 *   H<float>      Get the Resonance Frequency from Timeline value. (Default 0)
 *
 * Examples:
 *   M495 S       : Start the test with default values or with last set parameters
 *   M495 X S     : Start the test on X axis with default values or with last set parameters
 *   M495 Y S     : Start the test on Y axis with default values or with last set parameters
 *   M495 Z S     : Start the test on Z axis with default values or with last set parameters
 *   M495 H<val>  : Get Resonance Frequency from Timeline value
 *
 */
void GcodeSuite::M495() {
  if (!parser.seen_any()) return say_resonance_test();

  const bool seenX = parser.seen_test('X'), seenY = parser.seen_test('Y'), seenZ = parser.seen_test('Z') ;
  if (seenX + seenY + seenZ > 1) {
      SERIAL_ECHOLNPGM("?Select only one axis for Resonance Test.");
      return;
    }
    else {
      ftMotion.rtg->rt_params.axis = seenX ? X_AXIS : seenY ? Y_AXIS : Z_AXIS;
      SERIAL_ECHOLN(C('X' + seenY + 2 * seenZ), F("-axis selected for Resonance Test."));
  }

  if (parser.seenval('A')) {
    ftMotion.rtg->rt_params.accel_per_hz = parser.value_float();
    SERIAL_ECHOLNPGM("Accel/Hz set to ", ftMotion.rtg->rt_params.accel_per_hz);
  }

  if (parser.seenval('F')) {
    const float val = parser.value_float();
    if (val >= 5.0f) {
      ftMotion.rtg->rt_params.min_freq = val;
      SERIAL_ECHOLNPGM("Start Frequency set to ", ftMotion.rtg->rt_params.min_freq, " Hz");
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid Start [F]requency. (minimum 5.0 Hz).");
    }
  }

  if (parser.seenval('T')) {
    const float val = parser.value_float();
    if (val > ftMotion.rtg->rt_params.min_freq && val <= 200.0f) {
      ftMotion.rtg->rt_params.max_freq = val;
      SERIAL_ECHOLNPGM("End Frequency set to ", ftMotion.rtg->rt_params.max_freq, " Hz");
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid end frequency [T]. (Start Frequency .. 200 Hz).");
    }
  }

  if (parser.seenval('R')) {
    const float val = parser.value_float();
    if (WITHIN(val, 1, 10)) {
      ftMotion.rtg->rt_params.hz_per_sec = val;
      SERIAL_ECHOLNPGM("Frequency Increase Rate set to ", ftMotion.rtg->rt_params.hz_per_sec, " Hz/s.");
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid frequency increase rate [R]. (1..10 Hz/s).");
    }
  }

  if (parser.seenval('C')) {
    const int val = parser.value_int();
    if (WITHIN(val, 1, 8)) {
      ftMotion.rtg->rt_params.amplitude_correction = val;
      SERIAL_ECHOLNPGM("Amplitude Correction Factor set to ", ftMotion.rtg->rt_params.amplitude_correction);
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid Amplitude [C]orrection Factor. (1..8).");
    }
  }

  if (parser.seen_test('S')) {
    if (ftMotion.cfg.active) {
      if (ftMotion.rtg->rt_params.max_freq > ftMotion.rtg->rt_params.min_freq) {
        SERIAL_ECHOLNPGM("Starting Resonance Test...");
        ftMotion.start_resonance_test();
        // The function returns immediately, the test runs in the background.
      }
      else {
        SERIAL_ECHOLNPGM("?End Frequency must be greater than Start Frequency.");
      }
    }
    else {
      SERIAL_ECHOLNPGM("?Activate FT Motion to run the Resonance Test.");
    }
  }

  if (parser.seenval('G')) {
    const float val = parser.value_float();
    if (WITHIN(val, 0, 100)) {
      ftMotion.rtg->timeline = val;
      SERIAL_ECHOLNPGM("Resonance Frequency set to ", ftMotion.rtg->getFrequencyFromTimeline(), " Hz.");
    }
    else {
      SERIAL_ECHOLNPGM("Invalid Timeline value (0..100 s).");
    }
  }
}

#endif // FTM_RESONANCE_TEST
