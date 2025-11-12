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
  const ft_config_t &c = ftMotion.cfg;

  // FT Enabled
  SERIAL_ECHO_TERNARY(c.active, "Fixed-Time Motion ", "en", "dis", "abled");

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

  const bool z_based = TERN0(HAS_DYNAMIC_FREQ_MM, c.dynFreqMode == dynFreqMode_Z_BASED),
             g_based = TERN0(HAS_DYNAMIC_FREQ_G,  c.dynFreqMode == dynFreqMode_MASS_BASED),
             dynamic = z_based || g_based;

  // FT Dynamic Frequency Mode
  if (is_shaping) {
    #if HAS_DYNAMIC_FREQ
      SERIAL_ECHOPGM("Dynamic Frequency Mode ");
      switch (c.dynFreqMode) {
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
      SERIAL_ECHO(p_float_t(c.baseFreq.x, 2), F("Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(c.dynFreqK.x, 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif

    #if HAS_Y_AXIS
      SERIAL_CHAR(STEPPER_B_NAME);
      SERIAL_ECHO_TERNARY(dynamic, " ", "base dynamic", "static", " shaper frequency: ");
      SERIAL_ECHO(p_float_t(c.baseFreq.y, 2), F(" Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(c.dynFreqK.y, 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif

    #if ENABLED(FTM_SHAPER_Z)
      SERIAL_CHAR(STEPPER_C_NAME);
      SERIAL_ECHO_TERNARY(dynamic, " ", "base dynamic", "static", " shaper frequency: ");
      SERIAL_ECHO(p_float_t(c.baseFreq.z, 2), F(" Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(c.dynFreqK.z, 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif
  }
}

void GcodeSuite::M493_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_FT_MOTION));
  const ft_config_t &c = ftMotion.cfg;

  SERIAL_ECHOLNPGM(
    "  M493 S", c.active
    #if HAS_DYNAMIC_FREQ
      , " D", c.dynFreqMode
    #endif
    // Axis Synchronization
    , " H", c.axis_sync_enabled
  );

  #if HAS_DYNAMIC_FREQ
    #define F_REPORT(A) , F(" F"), c.dynFreqK.A
  #else
    #define F_REPORT(A)
  #endif
  #define _REPORT_M493_AXIS(A) \
    SERIAL_ECHOLN(F("  M493 "), C(AXIS_CHAR(_AXIS(A))) \
      , F(" C"), c.shaper.A \
      , F(" A"), c.baseFreq.A \
      F_REPORT(A) \
      , F(" I"), c.zeta.A \
      , F(" Q"), c.vtol.A \
    );
  // Shaper type for each axis
  SHAPED_MAP(_REPORT_M493_AXIS);
}

/**
 * M493: Set Fixed-time Motion Control parameters
 *
 *    S<bool> Set Fixed-Time motion mode on or off.
 *       0: Fixed-Time Motion OFF (Standard Motion)
 *       1: Fixed-Time Motion ON
 *
 *    V         Flag to request version (Version 2+). (No reply = Version < 2)
 *
 *    H<bool> Enable (1) or Disable (0) Axis Synchronization.
 *
 * Linear / Pressure Advance:
 *
 *    P<bool> Enable (1) or Disable (0) Linear Advance pressure control
 *
 * Specifying Axes (for A,C,F,I,Q):
 *
 *    X/Y/Z/E : Flag the axes (or core steppers) on which to apply the given parameters
 *              If none are given then XY is assumed.
 *
 * Compensator / Input Shaper:
 *
 *    C<mode>   Set Compensator Mode (Input Shaper) for the specified axes
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
 *    A<Hz>     Set static/base frequency for the specified axes
 *    I<flt>    Set damping ratio for the specified axes
 *    Q<flt>    Set vibration tolerance (vtol) for the specified axes
 *
 * Dynamic Frequency Mode:
 *
 *    D<mode> Set Dynamic Frequency mode (for all axis compensators)
 *       0: DISABLED
 *       1: Z-based (Requires a Z axis)
 *       2: Mass-based (Requires X and E axes)
 *
 *    F<Hz>     Set frequency scaling for the specified axes
 *
 */
void GcodeSuite::M493() {
  // Request version of FTM. (No response = Version < 2)
  if (parser.seen('V') && !parser.has_value()) {
    SERIAL_ECHOLNPGM("FTM V" STRINGIFY(FTM_VERSION));
    return;
  }

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

    const bool seenC = parser.seenval('C');
    const ftMotionShaper_t shaperVal = seenC ? (ftMotionShaper_t)parser.value_byte() : ftMotionShaper_NONE;
    const bool goodShaper = WITHIN(shaperVal, ftMotionShaper_NONE, ftMotionShaper_MZV);
    if (seenC && !goodShaper) {
      SERIAL_ECHOLN(F("?Invalid "), F("(C)ompensator value. (0-"), int(ftMotionShaper_MZV));
      return;
    }
    auto set_shaper = [&](const AxisEnum axis, ftMotionShaper_t newsh) {
      if (newsh != ftMotion.cfg.shaper[axis]) {
        ftMotion.cfg.shaper[axis] = newsh;
        flag.update = flag.report = true;
      }
    };
    if (seenC) {
      #define _SET_SHAPER(A) set_shaper(_AXIS(A), shaperVal);
      SHAPED_MAP(_SET_SHAPER);
    }

  #endif // NUM_AXES_SHAPED > 0

  // Parse 'H' Axis Synchronization parameter.
  if (parser.seenval('H')) {
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
            SERIAL_ECHOLN(F("?Invalid "), F("(D)ynamic Frequency Mode value."));
            break;
        }
      }
      else {
        SERIAL_ECHOLNPGM("?Wrong shaper for (D)ynamic Frequency Mode ", ftMotion.cfg.dynFreqMode, ".");
      }
    }

    const bool modeUsesDynFreq = (
         TERN0(HAS_DYNAMIC_FREQ_MM, ftMotion.cfg.dynFreqMode == dynFreqMode_Z_BASED)
      || TERN0(HAS_DYNAMIC_FREQ_G,  ftMotion.cfg.dynFreqMode == dynFreqMode_MASS_BASED)
    );

  #endif // HAS_DYNAMIC_FREQ

  // Frequency parameter
  const bool seenA = parser.seenval('A');
  const float baseFreqVal = seenA ? parser.value_float() : 0.0f;
  const bool goodBaseFreq = seenA && WITHIN(baseFreqVal, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2);
  if (seenA && !goodBaseFreq)
    SERIAL_ECHOLN(F("?Invalid "), F("(A) Base Frequency value. ("), int(FTM_MIN_SHAPE_FREQ), C('-'), int((FTM_FS) / 2), C(')'));

  #if HAS_DYNAMIC_FREQ
    // Dynamic Frequency parameter
    const bool seenF = parser.seenval('F');
    const float baseDynFreqVal = seenF ? parser.value_float() : 0.0f;
    if (seenF && !modeUsesDynFreq)
      SERIAL_ECHOLNPGM("?Wrong mode for (F)requency scaling.");
  #endif

  // Zeta parameter
  const bool seenI = parser.seenval('I');
  const float zetaVal = seenI ? parser.value_float() : 0.0f;
  const bool goodZeta = seenI && WITHIN(zetaVal, 0.01f, 1.0f);
  if (seenI && !goodZeta)
    SERIAL_ECHOLN(F("?Invalid "), F("(I) Zeta value. (0.01-1.0)")); // Zeta out of range

  // Vibration Tolerance parameter
  const bool seenQ = parser.seenval('Q');
  const float vtolVal = seenQ ? parser.value_float() : 0.0f;
  const bool goodVtol = seenQ && WITHIN(vtolVal, 0.00f, 1.0f);
  if (seenQ && !goodVtol)
    SERIAL_ECHOLN(F("?Invalid "), F("(Q) Vibration Tolerance value. (0.0-1.0)")); // VTol out of range

  const bool apply_xy = !parser.seen("XYZE");

  #if HAS_X_AXIS

    if (apply_xy || parser.seen_test('X')) {

      // Parse X frequency parameter
      if (seenA) {
        if (AXIS_IS_SHAPING(X)) {
          // TODO: Frequency minimum is dependent on the shaper used; the above check isn't always correct.
          if (goodBaseFreq) {
            ftMotion.cfg.baseFreq.x = baseFreqVal;
            flag.update = flag.report = true;
          }
        }
        else // Mode doesn't use frequency.
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " [A] frequency.");
      }

      #if HAS_DYNAMIC_FREQ
        // Parse X frequency scaling parameter
        if (seenF && modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.x = baseDynFreqVal;
          flag.report = true;
        }
      #endif

      // Parse X zeta parameter
      if (seenI) {
        if (AXIS_IS_SHAPING(X)) {
          if (goodZeta) {
            ftMotion.cfg.zeta.x = zetaVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " zeta parameter.");
      }

      // Parse X vtol parameter
      if (seenQ) {
        if (AXIS_IS_EISHAPING(X)) {
          if (goodVtol) {
            ftMotion.cfg.vtol.x = vtolVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_A_NAME), " vtol parameter.");
      }
    }

  #endif // HAS_X_AXIS

  #if HAS_Y_AXIS

    if (apply_xy || parser.seen_test('Y')) {

      // Parse Y frequency parameter
      if (seenA) {
        if (AXIS_IS_SHAPING(Y)) {
          if (goodBaseFreq) {
            ftMotion.cfg.baseFreq.y = baseFreqVal;
            flag.update = flag.report = true;
          }
        }
        else // Mode doesn't use frequency.
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " [A] frequency.");
      }

      #if HAS_DYNAMIC_FREQ
        // Parse Y frequency scaling parameter
        if (seenF && modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.y = baseDynFreqVal;
          flag.report = true;
        }
      #endif

      // Parse Y zeta parameter
      if (seenI) {
        if (AXIS_IS_SHAPING(Y)) {
          if (goodZeta) {
            ftMotion.cfg.zeta.y = zetaVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " zeta parameter.");
      }

      // Parse Y vtol parameter
      if (seenQ) {
        if (AXIS_IS_EISHAPING(Y)) {
          if (goodVtol) {
            ftMotion.cfg.vtol.y = vtolVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_B_NAME), " vtol parameter.");
      }
    }

  #endif // HAS_Y_AXIS

  #if ENABLED(FTM_SHAPER_Z)

    if (parser.seen_test('Z')) {

      // Parse Z frequency parameter
      if (seenA) {
        if (AXIS_IS_SHAPING(Z)) {
          if (goodBaseFreq) {
            ftMotion.cfg.baseFreq.z = baseFreqVal;
            flag.update = flag.report = true;
          }
        }
        else // Mode doesn't use frequency.
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " [A] frequency.");
      }

      #if HAS_DYNAMIC_FREQ
        // Parse Z frequency scaling parameter
        if (seenF && modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.z = baseDynFreqVal;
          flag.report = true;
        }
      #endif

      // Parse Z zeta parameter
      if (seenI) {
        if (AXIS_IS_SHAPING(Z)) {
          if (goodZeta) {
            ftMotion.cfg.zeta.z = zetaVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " zeta parameter.");
      }

      // Parse Z vtol parameter
      if (seenQ) {
        if (AXIS_IS_EISHAPING(Z)) {
          if (goodVtol) {
            ftMotion.cfg.vtol.z = vtolVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C(STEPPER_C_NAME), " vtol parameter.");
      }
    }

  #endif // FTM_SHAPER_Z

  #if ENABLED(FTM_SHAPER_E)

    if (parser.seen_test('E')) {

      // Parse E frequency parameter
      if (seenA) {
        if (AXIS_IS_SHAPING(E)) {
          if (goodBaseFreq) {
            ftMotion.cfg.baseFreq.e = baseFreqVal;
            flag.update = flag.report = true;
          }
        }
        else // Mode doesn't use frequency.
          SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " [A] frequency.");
      }

      #if HAS_DYNAMIC_FREQ
        // Parse E frequency scaling parameter
        if (seenF && modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK.e = baseDynFreqVal;
          flag.report = true;
        }
      #endif

      // Parse E zeta parameter
      if (seenI) {
        if (AXIS_IS_SHAPING(E)) {
          if (goodZeta) {
            ftMotion.cfg.zeta.e = zetaVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " zeta parameter.");
      }

      // Parse E vtol parameter
      if (seenQ) {
        if (AXIS_IS_EISHAPING(E)) {
          if (goodVtol) {
            ftMotion.cfg.vtol.e = vtolVal;
            flag.update = true;
          }
        }
        else
          SERIAL_ECHOLNPGM("?Wrong mode for ", C('E'), " vtol parameter.");
      }
    }

  #endif // FTM_SHAPER_E

  if (flag.update) ftMotion.update_shaping_params();

  if (flag.report) say_shaping();
}

#endif // FT_MOTION
