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
  const bool is_shaping = AXIS_IS_SHAPING(X) || AXIS_IS_SHAPING(Y) || AXIS_IS_SHAPING(Z) || AXIS_IS_SHAPING(E);
  bool sep = false;
  if (is_shaping) {
    #define STEPPER_E_NAME 'E'
    #define _SAY_SHAPER(A) if (AXIS_IS_SHAPING(A)) say_shaper_type(_AXIS(A), sep, STEPPER_##A##_NAME);
    SERIAL_ECHOPGM(" (");
    SHAPED_CODE(_SAY_SHAPER(A), _SAY_SHAPER(B), _SAY_SHAPER(C), _SAY_SHAPER(E));
    SERIAL_CHAR(')');
  }
  SERIAL_EOL();

  const bool z_based = TERN0(HAS_DYNAMIC_FREQ_MM, ftMotion.cfg.dynFreqMode == dynFreqMode_Z_BASED),
             g_based = TERN0(HAS_DYNAMIC_FREQ_G,  ftMotion.cfg.dynFreqMode == dynFreqMode_MASS_BASED),
             dynamic = z_based || g_based;

  // FT Dynamic Frequency Mode
  if (is_shaping) {
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
    if (ftMotion.cfg.active) {
      SERIAL_ECHO_TERNARY(ftMotion.cfg.linearAdvEna, "Linear Advance ", "en", "dis", "abled");
      SERIAL_ECHOLNPGM(". Gain: ", ftMotion.cfg.linearAdvK);
    }
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
    #if ENABLED(FTM_SHAPER_Z)
      , " C", c.baseFreq.z
    #endif
    #if ENABLED(FTM_SHAPER_E)
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
      #if ENABLED(FTM_SHAPER_Z)
        , " L", c.dynFreqK.z
      #endif
      #if ENABLED(FTM_SHAPER_E)
        , " O", c.dynFreqK.e
      #endif
    #endif

    , " G", c.axis_sync_enabled

    #if HAS_EXTRUDERS
      , " P", c.linearAdvEna, " K", c.linearAdvK
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
 *    X/Y/Z/E<mode> Set the vibration compensator [input shaper] mode for an axis.
 *              Users / slicers must remember to set the mode for all relevant axes!
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
 *    A<Hz>   Set X static/base frequency
 *    F<Hz>   Set X frequency scaling
 *    I<flt>  Set X damping ratio
 *    Q<flt>  Set X vibration tolerance
 *
 *    B<Hz>   Set Y static/base frequency
 *    H<Hz>   Set Y frequency scaling
 *    J<flt>  Set Y damping ratio
 *    R<flt>  Set Y vibration tolerance
 *
 * With FTM_SHAPING_Z:
 *    C<Hz>   Set Z static/base frequency
 *    L<Hz>   Set Z frequency scaling
 *    O<flt>  Set Z damping ratio
 *    M<flt>  Set Z vibration tolerance
 *
 * With FTM_SHAPING_E:
 *    W<Hz>   Set E static/base frequency
 *    O<Hz>   Set E frequency scaling
 *    U<flt>  Set E damping ratio
 *    V<flt>  Set E vibration tolerance
 *
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

  #if NUM_AXES_SHAPED > 0

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

    #define _SET_SHAPER(A) if (parser.seenval(CHARIFY(A)) && set_shaper(_AXIS(A), CHARIFY(A))) return;
    SHAPED_MAP(_SET_SHAPER);

  #endif // NUM_AXES_SHAPED > 0

  #if HAS_EXTRUDERS

    // Pressure control (linear advance) parameter.
    if (parser.seen('P')) {
      const bool val = parser.value_bool();
      ftMotion.cfg.linearAdvEna = val;
      flag.report = true;
    }

    // Pressure control (linear advance) gain parameter.
    if (parser.seenval('K')) {
      const float val = parser.value_float();
      if (WITHIN(val, 0.0f, 10.0f)) {
        ftMotion.cfg.linearAdvK = val;
        flag.report = true;
      }
      else // Value out of range.
        SERIAL_ECHOLNPGM("Linear Advance gain out of range.");
    }

  #endif // HAS_EXTRUDERS

  // Parse '?' axis synchronization parameter.
  if (parser.seen('?')) {
    const bool enabled = parser.value_bool();
    if (enabled != ftMotion.cfg.axis_sync_enabled) {
      ftMotion.cfg.axis_sync_enabled = enabled;
      flag.report = true;
    }
  }

  #if HAS_DYNAMIC_FREQ

    // Dynamic frequency mode parameter.
    if (parser.seenval('D')) {
      if (AXIS_IS_SHAPING(X) || AXIS_IS_SHAPING(Y) || AXIS_IS_SHAPING(Z) || AXIS_IS_SHAPING(E)) {
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

    // Parse X frequency parameter
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
      // Parse X frequency scaling parameter
      if (parser.seenval('F')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.x = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " [", C('F'), "] frequency scaling.");
      }
    #endif

    // Parse X zeta parameter
    if (parser.seenval('I')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(X)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta.x = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_A_NAME), " zeta [", C('I'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " zeta parameter.");
    }

    // Parse X vtol parameter
    if (parser.seenval('Q')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(X)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol.x = val;
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

    // Parse Y frequency parameter
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
      // Parse Y frequency scaling parameter
      if (parser.seenval('H')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.y = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " [", C('H'), "] frequency scaling.");
      }
    #endif

    // Parse Y zeta parameter
    if (parser.seenval('J')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(Y)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta.y = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " zeta [", C('J'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " zeta parameter.");
    }

    // Parse Y vtol parameter
    if (parser.seenval('R')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(Y)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol.y = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " vtol [", C('R'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " vtol parameter.");
    }

  #endif // HAS_Y_AXIS

  #if ENABLED(FTM_SHAPER_Z)

    // Parse Z frequency parameter
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
      // Parse Z frequency scaling parameter
      if (parser.seenval('L')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.z = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " [", C('L'), "] frequency scaling.");
      }
    #endif

    // Parse Z zeta parameter
    if (parser.seenval('O')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(Z)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta.z = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " zeta [", C('O'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " zeta parameter.");
    }

    // Parse Z vtol parameter
    if (parser.seenval('M')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(Z)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol.z = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " vtol [", C('M'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " vtol parameter.");
    }

  #endif // FTM_SHAPER_Z

  #if ENABLED(FTM_SHAPER_E)

    // Parse E frequency parameter
    if (parser.seenval('W')) {
      if (AXIS_IS_SHAPING(E)) {
        const float val = parser.value_float();
        if (WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2)) {
          ftMotion.cfg.baseFreq.e = val;
          flag.update = flag.report = true;
        }
        else // Frequency out of range.
          SERIAL_ECHOLNPGM("?Invalid ", C('E'), " frequency [", C('W'), "] value.");
      }
      else // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " [", C('W'), "] frequency.");
    }

    #if HAS_DYNAMIC_FREQ
      // Parse E frequency scaling parameter
      if (parser.seenval('O')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.e = parser.value_float();
          flag.report = true;
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " [", C('O'), "] frequency scaling.");
      }
    #endif

    // Parse E zeta parameter
    if (parser.seenval('U')) {
      const float val = parser.value_float();
      if (AXIS_IS_SHAPING(E)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta.e = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C('E'), " zeta [", C('U'), "] value."); // Zeta out of range
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " zeta parameter.");
    }

    // Parse E vtol parameter
    if (parser.seenval('V')) {
      const float val = parser.value_float();
      if (AXIS_IS_EISHAPING(E)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol.e = val;
          flag.update = true;
        }
        else
          SERIAL_ECHOLNPGM("?Invalid ", C('E'), " vtol [", C('V'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " vtol parameter.");
    }

  #endif // FTM_SHAPER_E

  if (flag.update) ftMotion.update_shaping_params();

  if (flag.report) say_shaping();
}

#endif // FT_MOTION
