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

#if ENABLED(FT_MOTION)

#include "../../gcode.h"
#include "../../../module/ft_motion.h"
#include "../../../module/stepper.h"

void say_shaper_type(const AxisEnum a, bool &sep, const char axis_name) {
  if (sep) SERIAL_ECHOPGM(" ; ");
  SERIAL_CHAR(axis_name, '=');
  switch (ftMotion.cfg.shaper[a]) {
    default: break;
    case ftMotionShaper_ZV:    SERIAL_ECHOPGM("ZV");        break;
    case ftMotionShaper_ZVD:   SERIAL_ECHOPGM("ZVD");       break;
    case ftMotionShaper_ZVDD:  SERIAL_ECHOPGM("ZVDD");      break;
    case ftMotionShaper_ZVDDD: SERIAL_ECHOPGM("ZVDDD");     break;
    case ftMotionShaper_EI:    SERIAL_ECHOPGM("EI");        break;
    case ftMotionShaper_2HEI:  SERIAL_ECHOPGM("2 Hump EI"); break;
    case ftMotionShaper_3HEI:  SERIAL_ECHOPGM("3 Hump EI"); break;
    case ftMotionShaper_MZV:   SERIAL_ECHOPGM("MZV");       break;
  }
  sep = true;
}

void say_shaping() {
  // FT Enabled
  SERIAL_ECHO_TERNARY(ftMotion.cfg.active, "Fixed-Time Motion ", "en", "dis", "abled");

  // FT Shaping
  bool sep = false;
  SERIAL_ECHOPGM(" (");
  #if HAS_X_AXIS
    if (AXIS_IS_SHAPING(X)) say_shaper_type(X_AXIS, sep, STEPPER_A_NAME);
  #endif
  #if HAS_Y_AXIS
    if (AXIS_IS_SHAPING(Y)) say_shaper_type(Y_AXIS, sep, STEPPER_B_NAME);
  #endif
  #if HAS_Z_AXIS
    if (AXIS_IS_SHAPING(Z)) say_shaper_type(Z_AXIS, sep, STEPPER_C_NAME);
  #endif
  SERIAL_ECHOLNPGM(")");

  const bool z_based = TERN0(HAS_DYNAMIC_FREQ_MM, ftMotion.cfg.dynFreqMode == dynFreqMode_Z_BASED),
             g_based = TERN0(HAS_DYNAMIC_FREQ_G,  ftMotion.cfg.dynFreqMode == dynFreqMode_MASS_BASED),
             dynamic = z_based || g_based;

  // FT Dynamic Frequency Mode
  if (AXIS_IS_SHAPING(X) || AXIS_IS_SHAPING(Y) || AXIS_IS_SHAPING(Z)) {
    #if HAS_DYNAMIC_FREQ
      SERIAL_ECHOPGM("Dynamic Frequency Mode ");
      switch (ftMotion.cfg.dynFreqMode) {
        default:
        case dynFreqMode_DISABLED: SERIAL_ECHOPGM("disabled"); break;
        #if HAS_DYNAMIC_FREQ_MM
          case dynFreqMode_Z_BASED: SERIAL_ECHOPGM("Z-based"); break;
        #endif
        #if HAS_DYNAMIC_FREQ_G
          case dynFreqMode_MASS_BASED: SERIAL_ECHOPGM("Mass-based"); break;
        #endif
      }
      SERIAL_ECHOLNPGM(".");
    #endif

    #if HAS_X_AXIS
      SERIAL_CHAR(STEPPER_A_NAME);
      SERIAL_ECHO_TERNARY(dynamic, " ", "base dynamic", "static", " shaper frequency: ");
      SERIAL_ECHO(p_float_t(ftMotion.cfg.baseFreq.x, 2), F("Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(ftMotion.cfg.dynFreqK.x, 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif

    #if HAS_Y_AXIS
      SERIAL_CHAR(STEPPER_B_NAME);
      SERIAL_ECHO_TERNARY(dynamic, " ", "base dynamic", "static", " shaper frequency: ");
      SERIAL_ECHO(p_float_t(ftMotion.cfg.baseFreq.y, 2), F(" Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(ftMotion.cfg.dynFreqK.y, 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif

    #if ENABLED(FTM_SHAPER_Z)
      SERIAL_CHAR(STEPPER_C_NAME);
      SERIAL_ECHO_TERNARY(dynamic, " ", "base dynamic", "static", " shaper frequency: ");
      SERIAL_ECHO(p_float_t(ftMotion.cfg.baseFreq.z, 2), F(" Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(ftMotion.cfg.dynFreqK.z, 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif
  }

  #if HAS_EXTRUDERS
    SERIAL_ECHO_TERNARY(ftMotion.cfg.linearAdvEna, "Linear Advance ", "en", "dis", "abled");
    if (ftMotion.cfg.linearAdvEna)
      SERIAL_ECHOLNPGM(". Gain: ", ftMotion.cfg.linearAdvK);
    else
      SERIAL_EOL();
  #endif
}

void GcodeSuite::M493_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_FT_MOTION));
  const ft_config_t &c = ftMotion.cfg;
  SERIAL_ECHOLNPGM(
    "  M493 S", c.active
    #if HAS_X_AXIS
      , " A", c.baseFreq.x
    #endif
    #if HAS_Y_AXIS
      , " B", c.baseFreq.y
    #endif
    #if HAS_Z_AXIS
      , " C", c.baseFreq.z
    #endif
    #if HAS_EXTRUDERS
      , " E", c.baseFreq.e
    #endif

    #if HAS_DYNAMIC_FREQ
      , " D", c.dynFreqMode
      #if HAS_X_AXIS
        , " F", c.dynFreqK.x
      #endif
      #if HAS_Y_AXIS
        , " H", c.dynFreqK.y
      #endif
      #if HAS_Z_AXIS
        , " L", c.dynFreqK.z
      #endif
      #if HAS_EXTRUDERS
        , " O", c.dynFreqK.z
      #endif
    #endif

    , " G", c.axis_sync_enabled

    #if HAS_EXTRUDERS
      , " P", c.linearAdvEna, " K", c.linearAdvK
    #endif

    #if ENABLED(FTM_SMOOTHING)
      #if HAS_X_AXIS
        , " T", c.smoothingTime.x
      #endif
      #if HAS_Y_AXIS
        , " U", c.smoothingTime.y
      #endif
      #if HAS_Z_AXIS
        , " V", c.smoothingTime.z
      #endif
      #if HAS_EXTRUDERS
        , " W", c.smoothingTime.e
      #endif
    #endif
  );
}

/**
 * M493: Set Fixed-time Motion Control parameters
 *
 *    S<bool> Set Fixed-Time motion mode on or off.
 *       0: Fixed-Time Motion OFF (Standard Motion)
 *       1: Fixed-Time Motion ON
 *
 *    X/Y<mode> Set the vibration compensator [input shaper] mode for X / Y axis.
 *              Users / slicers must remember to set the mode for both axes!
 *       0: NONE  : No input shaper
 *       1: ZV    : Zero Vibration
 *       2: ZVD   : Zero Vibration and Derivative
 *       3: ZVDD  : Zero Vibration, Derivative, and Double Derivative
 *       4: ZVDDD : Zero Vibration, Derivative, Double Derivative, and Triple Derivative
 *       5: EI    : Extra-Intensive
 *       6: 2HEI  : 2-Hump Extra-Intensive
 *       7: 3HEI  : 3-Hump Extra-Intensive
 *       8: MZV   : Mass-based Zero Vibration
 *
 *    P<bool> Enable (1) or Disable (0) Linear Advance pressure control
 *
 *    K<gain> Set Linear Advance gain
 *
 *    G<bool> Enable (1) or Disable (0) axis synchronization.
 *
 *    D<mode> Set Dynamic Frequency mode
 *       0: DISABLED
 *       1: Z-based (Requires a Z axis)
 *       2: Mass-based (Requires X and E axes)
 *
 *    A<Hz>   Set static/base frequency for the X axis
 *    F<Hz>   Set frequency scaling for the X axis
 *    I 0.0   Set damping ratio for the X axis
 *    Q 0.00  Set the vibration tolerance for the X axis
 *
 *    B<Hz> Set static/base frequency for the Y axis
 *    H<Hz> Set frequency scaling for the Y axis
 *    J 0.0   Set damping ratio for the Y axis
 *    R 0.00  Set the vibration tolerance for the Y axis
 *
 *    C<Hz> Set static/base frequency for the Z axis
 *    L<Hz> Set frequency scaling for the Z axis
 *    M 0.0   Set damping ratio for the Z axis
 *    N 0.00  Set the vibration tolerance for the Z axis
 *
 *    T<time> Set smoothing time for the X axis
 *    U<time> Set smoothing time for the Y axis
 *    V<time> Set smoothing time for the Z axis
 *    W<time> Set smoothing time for the E axis
 */
void GcodeSuite::M493() {
  struct { bool update:1, report:1; } flag = { false };

  if (!parser.seen_any())
    flag.report = true;

  // Parse 'S' mode parameter.
  if (parser.seen('S')) {
    const bool active = parser.value_bool();
    if (active != ftMotion.cfg.active) {
      stepper.ftMotion_syncPosition();
      ftMotion.cfg.active = active;
      flag.report = true;
    }
  }

  #if HAS_X_AXIS || HAS_Y_AXIS || HAS_Z_AXIS

    auto set_shaper = [&](const AxisEnum axis, const char c) {
      const ftMotionShaper_t newsh = (ftMotionShaper_t)parser.value_byte();
      if (newsh != ftMotion.cfg.shaper[axis]) {
        switch (newsh) {
          default: SERIAL_ECHOLNPGM("?Invalid [", C(c), "] shaper."); return true;
          case ftMotionShaper_NONE:
          case ftMotionShaper_ZV:
          case ftMotionShaper_ZVD:
          case ftMotionShaper_ZVDD:
          case ftMotionShaper_ZVDDD:
          case ftMotionShaper_EI:
          case ftMotionShaper_2HEI:
          case ftMotionShaper_3HEI:
          case ftMotionShaper_MZV:
            ftMotion.cfg.shaper[axis] = newsh;
            flag.update = flag.report = true;
            break;
        }
      }
      return false;
    };

    #if HAS_X_AXIS
      if (parser.seenval('X') && set_shaper(X_AXIS, 'X')) return;  // Parse 'X' mode parameter
    #endif
    #if HAS_Y_AXIS
      if (parser.seenval('Y') && set_shaper(Y_AXIS, 'Y')) return;  // Parse 'Y' mode parameter
    #endif
    #if HAS_Z_AXIS
      if (parser.seenval('Z') && set_shaper(Z_AXIS, 'Z')) return;  // Parse 'Z' mode parameter
    #endif

  #endif // HAS_X_AXIS || HAS_Y_AXIS || HAS_Z_AXIS

  #if HAS_EXTRUDERS

    // Pressure control (linear advance) parameter.
    if (parser.seen('P')) {
      const bool val = parser.value_bool();
      ftMotion.cfg.linearAdvEna = val;
      flag.report = true;
      SERIAL_ECHO_TERNARY(val, "Linear Advance ", "en", "dis", "abled.\n");
    }

    // Pressure control (linear advance) gain parameter.
    if (parser.seenval('K')) {
      const float val = parser.value_float();
      if (val >= 0.0f) {
        ftMotion.cfg.linearAdvK = val;
        flag.report = true;
      }
      else // Value out of range.
        SERIAL_ECHOLNPGM("Linear Advance gain out of range.");
    }

  #endif // HAS_EXTRUDERS

  // Parse 'G' axis synchronization parameter.
  if (parser.seen('G')) {
    const bool enabled = parser.value_bool();
    if (enabled != ftMotion.cfg.axis_sync_enabled) {
      ftMotion.cfg.axis_sync_enabled = enabled;
      flag.report = true;
    }
  }

  #if HAS_DYNAMIC_FREQ

    // Dynamic frequency mode parameter.
    if (parser.seenval('D')) {
      if (AXIS_IS_SHAPING(X) || AXIS_IS_SHAPING(Y) || AXIS_IS_SHAPING(Z)) {
        const dynFreqMode_t val = dynFreqMode_t(parser.value_byte());
        switch (val) {
          #if HAS_DYNAMIC_FREQ_MM
            case dynFreqMode_Z_BASED:
          #endif
          #if HAS_DYNAMIC_FREQ_G
            case dynFreqMode_MASS_BASED:
          #endif
          case dynFreqMode_DISABLED:
            ftMotion.cfg.dynFreqMode = val;
            flag.report = true;
            break;
          default:
            SERIAL_ECHOLNPGM("?Invalid Dynamic Frequency Mode [D] value.");
            break;
        }
      }
      else {
        SERIAL_ECHOLNPGM("?Wrong shaper for [D] Dynamic Frequency mode.");
      }
    }

    const bool modeUsesDynFreq = (
         TERN0(HAS_DYNAMIC_FREQ_MM, ftMotion.cfg.dynFreqMode == dynFreqMode_Z_BASED)
      || TERN0(HAS_DYNAMIC_FREQ_G,  ftMotion.cfg.dynFreqMode == dynFreqMode_MASS_BASED)
    );

  #endif // HAS_DYNAMIC_FREQ

  #if HAS_X_AXIS

    // Parse frequency parameter (X axis).
    if (parser.seenval('A')) {
      if (AXIS_IS_SHAPING(X)) {
        const float val = parser.value_float();
        // TODO: Frequency minimum is dependent on the shaper used; the above check isn't always correct.
        if (WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2)) {
          ftMotion.cfg.baseFreq.x = val;
          flag.update = flag.report = true;
        }
        else // Frequency out of range.
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_A_NAME), " [", C('A'), "] frequency value.");
      }
      else // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " [", C('A'), "] frequency.");
    }

    #if HAS_DYNAMIC_FREQ
      // Parse frequency scaling parameter (X axis).
      if (parser.seenval('F')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.x = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " [", C('F'), "] frequency scaling.");
      }
    #endif

    // Parse zeta parameter (X axis).
    if (parser.seenval('I')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(X)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta[0] = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_A_NAME), " zeta [", C('I'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " zeta parameter.");
    }

    // Parse vtol parameter (X axis).
    if (parser.seenval('Q')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(X)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol[0] = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_A_NAME), " vtol [", C('Q'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " vtol parameter.");
    }

  #endif // HAS_X_AXIS

  #if HAS_Y_AXIS

    // Parse frequency parameter (Y axis).
    if (parser.seenval('B')) {
      if (AXIS_IS_SHAPING(Y)) {
        const float val = parser.value_float();
        if (WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2)) {
          ftMotion.cfg.baseFreq.y = val;
          flag.update = flag.report = true;
        }
        else // Frequency out of range.
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " frequency [", C('B'), "] value.");
      }
      else // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " [", C('B'), "] frequency.");
    }

    #if HAS_DYNAMIC_FREQ
      // Parse frequency scaling parameter (Y axis).
      if (parser.seenval('H')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.y = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " [", C('H'), "] frequency scaling.");
      }
    #endif

    // Parse zeta parameter (Y axis).
    if (parser.seenval('J')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(Y)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta[1] = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " zeta [", C('J'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " zeta parameter.");
    }

    // Parse vtol parameter (Y axis).
    if (parser.seenval('R')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(Y)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol[1] = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " vtol [", C('R'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " vtol parameter.");
    }

  #endif // HAS_Y_AXIS

  #if HAS_Z_AXIS

    // Parse frequency parameter (Z axis).
    if (parser.seenval('C')) {
      if (AXIS_IS_SHAPING(Z)) {
        const float val = parser.value_float();
        if (WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2)) {
          ftMotion.cfg.baseFreq.z = val;
          flag.update = flag.report = true;
        }
        else // Frequency out of range.
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " frequency [", C('C'), "] value.");
      }
      else // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " [", C('C'), "] frequency.");
    }

    #if HAS_DYNAMIC_FREQ
      // Parse frequency scaling parameter (Z axis).
      if (parser.seenval('L')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.z = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " [", C('L'), "] frequency scaling.");
      }
    #endif

    // Parse zeta parameter (Z axis).
    if (parser.seenval('M')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(Z)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta[2] = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " zeta [", C('M'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " zeta parameter.");
    }

    // Parse vtol parameter (Z axis).
    if (parser.seenval('N')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(Z)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol[2] = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " vtol [", C('N'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " vtol parameter.");
    }

  #endif // HAS_Z_AXIS

  #if ENABLED(FTM_SMOOTHING)
    #if HAS_X_AXIS
      // Parse smoothing time parameter (X axis).
      if (parser.seenval('T')) {
        const float val = parser.value_float();
        if (val >= 0.0f && val <= FTM_MAX_SMOOTHING_TIME) {
          ftMotion.set_smoothing_time(X_AXIS, val);
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_A_NAME), " smoothing time [", C('T'), "] value.");
      }
    #endif

    #if HAS_Y_AXIS
      // Parse smoothing time parameter (Y axis).
      if (parser.seenval('U')) {
        const float val = parser.value_float();
        if (val >= 0.0f && val <= FTM_MAX_SMOOTHING_TIME) {
          ftMotion.set_smoothing_time(Y_AXIS, val);
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " smoothing time [", C('U'), "] value.");
      }
    #endif

    #if HAS_Z_AXIS
      // Parse smoothing time parameter (Z axis).
      if (parser.seenval('V')) {
        const float val = parser.value_float();
        if (val >= 0.0f && val <= FTM_MAX_SMOOTHING_TIME) {
          ftMotion.set_smoothing_time(Z_AXIS, val);
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " smoothing time [", C('V'), "] value.");
      }
    #endif

    #if HAS_EXTRUDERS
      // Parse smoothing time parameter (E axis).
      if (parser.seenval('W')) {
        const float val = parser.value_float();
        if (val >= 0.0f && val <= FTM_MAX_SMOOTHING_TIME) {
          ftMotion.set_smoothing_time(E_AXIS, val);
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C('E'), " smoothing time [", C('W'), "] value.");
      }
    #endif

  #endif // FTM_SMOOTHING

  if (flag.update) ftMotion.update_shaping_params();

  if (flag.report) say_shaping();
}

#endif // FT_MOTION
