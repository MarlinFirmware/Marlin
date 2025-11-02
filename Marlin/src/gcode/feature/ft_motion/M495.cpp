/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../module/ft_motion/resonance_trajectory_generator.h"

void say_resonance_test() {
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("M495 Resonance Test Parameters:");
  SERIAL_ECHOLNPGM("  Axis: ", ftMotion.rtg->rt_params.axis ? "Y":"X", "-axis");
  SERIAL_ECHOLNPGM("  Freq Range (F..T): ", ftMotion.rtg->rt_params.min_freq, " .. ", ftMotion.rtg->rt_params.max_freq, " Hz");
  SERIAL_ECHOLNPGM("  Rate (R): ", ftMotion.rtg->rt_params.hz_per_sec, " Hz/s");
  SERIAL_ECHOLNPGM("  Accel/Hz (A): ", ftMotion.rtg->rt_params.accel_per_hz);
  SERIAL_ECHOLNPGM("Use S to start the test with default values or with last set parameters");
  SERIAL_ECHOLNPGM("Use X S to start the test on X axis");
  SERIAL_ECHOLNPGM("Use Y S to start the test on Y axis");
}

/**
 * M495: Configure and run the resonance test
 * M495 S   : Start the test with default values or with last set parameters
 * M495 X S : Start the test on X axis with default values or with last set parameters
 * M495 Y S : Start the test on Y axis with default values or with last set parameters
 *
 *   [A<accel/Hz>] Accel per Hz (default 60.00f).
 *   [F<Hz>]       Start frequency (default 5.0f).
 *   [R<Hz/s>]     Frequency increase rate (default 1.0f).
 *   [S]           Start the test.
 *   [T<Hz>]       End frequency (default 100.0f).
 *   [C<int>]      Amplitude correction factor (default 2).
 *   [X]           Select X axis.
 *   [Y]           Select Y axis.
 *   If no parameters are given, the current configuration is reported.
 **/

void GcodeSuite::M495() {
  if (!parser.seen_any()) {
    say_resonance_test();
    return;
  }

  if (parser.seen('X')) {
    ftMotion.rtg->rt_params.axis = X_AXIS;
    SERIAL_ECHOLNPGM("X-axis selected for Resonance Test.");
  }
  if (parser.seen('Y')) {
    ftMotion.rtg->rt_params.axis = Y_AXIS;
    SERIAL_ECHOLNPGM("Y-axis selected for Resonance Test.");
  }

  if (parser.seenval('A')) {
    ftMotion.rtg->rt_params.accel_per_hz = parser.value_float();
    SERIAL_ECHOLNPGM("Set Accel/Hz to ", ftMotion.rtg->rt_params.accel_per_hz);
  }

  if (parser.seenval('F')) {
    const float val = parser.value_float();
    if (val >= 5.0f) {
      ftMotion.rtg->rt_params.min_freq = val;
      SERIAL_ECHOLNPGM("Set start frequency to ", ftMotion.rtg->rt_params.min_freq, " Hz");
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid start frequency [F]. Must be >= 5.0 Hz.");
    }
  }

  if (parser.seenval('T')) {
    const float val = parser.value_float();
    if (val > ftMotion.rtg->rt_params.min_freq && val <= 200.0f) {
      ftMotion.rtg->rt_params.max_freq = val;
      SERIAL_ECHOLNPGM("Set end frequency to ", ftMotion.rtg->rt_params.max_freq, " Hz");
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid end frequency [T]. Must be > start frequency and <= 200.0 Hz.");
    }
  }

  if (parser.seenval('R')) {
    const float val = parser.value_float();
    if (WITHIN(val,1.0f, 10.0f)) {
      ftMotion.rtg->rt_params.hz_per_sec = val;
      SERIAL_ECHOLNPGM("Set frequency increase rate to ", ftMotion.rtg->rt_params.hz_per_sec, " Hz/s");
    }
    else {
      SERIAL_ECHOLNPGM("Invalid frequency increase rate [R]. Must be >= 1 and =<10 Hz/s.");
    }
  }

  if (parser.seenval('C')) {
    const int val = parser.value_int();
    if (WITHIN(val, 1, 8)) {
      ftMotion.rtg->rt_params.amplitude_correction = val;
      SERIAL_ECHOLNPGM("Set amplitude correction factor to ", ftMotion.rtg->rt_params.amplitude_correction);
    }
    else {
      SERIAL_ECHOLNPGM("Invalid amplitude correction factor [C]. Must be between 1 and 8.");
    }
  }
  
  if (parser.seen('S')) {
    if(ftMotion.cfg.active) {
      if (ftMotion.rtg->rt_params.max_freq > ftMotion.rtg->rt_params.min_freq) {
        SERIAL_ECHOLNPGM("Starting Resonance Test...");
        ftMotion.start_resonanceTest();
        // The function returns immediately, the test runs in the background.
      }
      else {
        SERIAL_ECHOLNPGM("End frequency must be greater than start frequency.");
      }
    }
    else {
      SERIAL_ECHOLNPGM("FTMotion must be active to run the Resonance Test.");
    }
  }
}

#endif // FTM_RESONANCE_TEST