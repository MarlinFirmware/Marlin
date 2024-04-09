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

void say_shaping() {
    // FT Enabled
  SERIAL_ECHO_TERNARY(ftMotion.cfg.mode, "Fixed-Time Motion ", "en", "dis", "abled");

  // FT Shaping
  #if HAS_X_AXIS
    if (CMPNSTR_HAS_SHAPER(X_AXIS)) {
      SERIAL_ECHOPGM(" with X/A axis ");
      switch (ftMotion.cfg.cmpnstr[X_AXIS]) {
        default: break;
        case ftMotionCmpnstr_ZV:    SERIAL_ECHOPGM("ZV");        break;
        case ftMotionCmpnstr_ZVD:   SERIAL_ECHOPGM("ZVD");       break;
        case ftMotionCmpnstr_ZVDD:  SERIAL_ECHOPGM("ZVDD");      break;
        case ftMotionCmpnstr_ZVDDD: SERIAL_ECHOPGM("ZVDDD");     break;
        case ftMotionCmpnstr_EI:    SERIAL_ECHOPGM("EI");        break;
        case ftMotionCmpnstr_2HEI:  SERIAL_ECHOPGM("2 Hump EI"); break;
        case ftMotionCmpnstr_3HEI:  SERIAL_ECHOPGM("3 Hump EI"); break;
        case ftMotionCmpnstr_MZV:   SERIAL_ECHOPGM("MZV");       break;
      }
      SERIAL_ECHOPGM(" shaping");
    }
  #endif
  #if HAS_Y_AXIS
    if (CMPNSTR_HAS_SHAPER(Y_AXIS)) {
      SERIAL_ECHOPGM(" and with Y/B axis ");
      switch (ftMotion.cfg.cmpnstr[Y_AXIS]) {
        default: break;
        case ftMotionCmpnstr_ZV:    SERIAL_ECHOPGM("ZV");        break;
        case ftMotionCmpnstr_ZVD:   SERIAL_ECHOPGM("ZVD");       break;
        case ftMotionCmpnstr_ZVDD:  SERIAL_ECHOPGM("ZVDD");      break;
        case ftMotionCmpnstr_ZVDDD: SERIAL_ECHOPGM("ZVDDD");     break;
        case ftMotionCmpnstr_EI:    SERIAL_ECHOPGM("EI");        break;
        case ftMotionCmpnstr_2HEI:  SERIAL_ECHOPGM("2 Hump EI"); break;
        case ftMotionCmpnstr_3HEI:  SERIAL_ECHOPGM("3 Hump EI"); break;
        case ftMotionCmpnstr_MZV:   SERIAL_ECHOPGM("MZV");       break;
      }
      SERIAL_ECHOPGM(" shaping");
    }
  #endif
  
  SERIAL_ECHOLNPGM(".");

  const bool z_based = TERN0(HAS_DYNAMIC_FREQ_MM, ftMotion.cfg.dynFreqMode == dynFreqMode_Z_BASED),
             g_based = TERN0(HAS_DYNAMIC_FREQ_G,  ftMotion.cfg.dynFreqMode == dynFreqMode_MASS_BASED),
             dynamic = z_based || g_based;

  // FT Dynamic Frequency Mode
  if (CMPNSTR_HAS_SHAPER(X_AXIS) || CMPNSTR_HAS_SHAPER(Y_AXIS)) {
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
      SERIAL_ECHO_TERNARY(dynamic, "X/A ", "base dynamic", "static", " compensator frequency: ");
      SERIAL_ECHO(p_float_t(ftMotion.cfg.baseFreq[X_AXIS], 2), F("Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(ftMotion.cfg.dynFreqK[X_AXIS], 2), F("Hz/"), z_based ? F("mm") : F("g"));
      #endif
      SERIAL_EOL();
    #endif

    #if HAS_Y_AXIS
      SERIAL_ECHO_TERNARY(dynamic, "Y/B ", "base dynamic", "static", " compensator frequency: ");
      SERIAL_ECHO(p_float_t(ftMotion.cfg.baseFreq[Y_AXIS], 2), F(" Hz"));
      #if HAS_DYNAMIC_FREQ
        if (dynamic) SERIAL_ECHO(F(" scaling: "), p_float_t(ftMotion.cfg.dynFreqK[Y_AXIS], 2), F("Hz/"), z_based ? F("mm") : F("g"));
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
  SERIAL_ECHOPGM("  M493 S", c.mode);
  #if HAS_X_AXIS
    SERIAL_ECHOPGM(" A", c.baseFreq[X_AXIS]);
  #endif
  #if HAS_Y_AXIS
    SERIAL_ECHOPGM(" B", c.baseFreq[Y_AXIS]);
  #endif

  #if HAS_DYNAMIC_FREQ
    SERIAL_ECHOPGM(" D", c.dynFreqMode);
    #if HAS_X_AXIS
      SERIAL_ECHOPGM(" F", c.dynFreqK[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      SERIAL_ECHOPGM(" H", c.dynFreqK[Y_AXIS]);
    #endif
  #endif
  
  #if HAS_EXTRUDERS
    SERIAL_ECHOPGM(" P", c.linearAdvEna, " K", c.linearAdvK);
  #endif
  SERIAL_EOL();
}

/**
 * M493: Set Fixed-time Motion Control parameters
 *
 *    S<mode> Set the motion / shaping mode. Shaping requires an X axis, at the minimum.
 * 
 *       0: Standard Motion
 *       1: Fixed-Time Motion
 * 
 *    X/Y<compensator mode> Set the vibration compensator [input shaper] mode of the X and
 *                          Y axes respectively. Requires an X axis, at the minimum.
 *      1: ZV    : Zero Vibration
 *      2: ZVD   : Zero Vibration and Derivative
 *      3: ZVDD  : Zero Vibration, Derivative, and Double Derivative
 *      4: ZVDDD : Zero Vibration, Derivative, Double Derivative, and Triple Derivative
 *      5: EI    : Extra-Intensive
 *      6: 2HEI  : 2-Hump Extra-Intensive
 *      7: 3HEI  : 3-Hump Extra-Intensive
 *      8: MZV   : Mass-based Zero Vibration
 *
 *    P<bool> Enable (1) or Disable (0) Linear Advance pressure control
 *
 *    K<gain> Set Linear Advance gain
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
 */
void GcodeSuite::M493() {
  struct { bool update_shpr_params:1, reset_ft:1, report_h:1; } flag = { false };

  if (!parser.seen_any())
    flag.report_h = true;

  // Parse 'S' mode parameter.
  if (parser.seenval('S')) {
    const ftMotionMode_t newmm = (ftMotionMode_t)parser.value_byte();

    if (newmm != ftMotion.cfg.mode) {
      switch (newmm) {
        default: SERIAL_ECHOLNPGM("?Invalid control mode [S] value."); return;
        case ftMotionMode_DISABLED: flag.reset_ft = true;
        case ftMotionMode_ENABLED:
          ftMotion.cfg.mode = newmm;
          flag.report_h = true;
          break;
      }
    }
  }

  #if HAS_X_AXIS
    // Parse 'X' mode parameter.
    if (parser.seenval('X')) {
      const ftMotionCmpnstr_t newmm = (ftMotionCmpnstr_t)parser.value_byte();

      if (newmm != ftMotion.cfg.cmpnstr[X_AXIS]) {
        switch (newmm) {
          default: SERIAL_ECHOLNPGM("?Invalid compensator / shaper [X] value."); return;
          case ftMotionCmpnstr_NONE:
          case ftMotionCmpnstr_ZV:
          case ftMotionCmpnstr_ZVD:
          case ftMotionCmpnstr_ZVDD:
          case ftMotionCmpnstr_ZVDDD:
          case ftMotionCmpnstr_EI:
          case ftMotionCmpnstr_2HEI:
          case ftMotionCmpnstr_3HEI:
          case ftMotionCmpnstr_MZV:
            flag.update_shpr_params = true;
            ftMotion.cfg.cmpnstr[X_AXIS] = newmm;
            flag.report_h = true;
            break;
        }
      }
    }
  #endif

  #if HAS_Y_AXIS
    // Parse 'Y' mode parameter.
    if (parser.seenval('Y')) {
      const ftMotionCmpnstr_t newmm = (ftMotionCmpnstr_t)parser.value_byte();

      if (newmm != ftMotion.cfg.cmpnstr[Y_AXIS]) {
        switch (newmm) {
          default: SERIAL_ECHOLNPGM("?Invalid compensator / shaper [Y] value."); return;
          case ftMotionCmpnstr_NONE:
          case ftMotionCmpnstr_ZV:
          case ftMotionCmpnstr_ZVD:
          case ftMotionCmpnstr_ZVDD:
          case ftMotionCmpnstr_ZVDDD:
          case ftMotionCmpnstr_EI:
          case ftMotionCmpnstr_2HEI:
          case ftMotionCmpnstr_3HEI:
          case ftMotionCmpnstr_MZV:
            flag.update_shpr_params = true;
            ftMotion.cfg.cmpnstr[Y_AXIS] = newmm;
            flag.report_h = true;
             break;
        }
      }
    }
  #endif

  #if HAS_EXTRUDERS

    // Pressure control (linear advance) parameter.
    if (parser.seen('P')) {
      const bool val = parser.value_bool();
      ftMotion.cfg.linearAdvEna = val;
      flag.report_h = true;
      SERIAL_ECHO_TERNARY(val, "Linear Advance ", "en", "dis", "abled.\n");
    }

    // Pressure control (linear advance) gain parameter.
    if (parser.seenval('K')) {
      const float val = parser.value_float();
      if (val >= 0.0f) {
        ftMotion.cfg.linearAdvK = val;
        flag.report_h = true;
      }
      else // Value out of range.
        SERIAL_ECHOLNPGM("Linear Advance gain out of range.");
    }

  #endif // HAS_EXTRUDERS

  #if HAS_DYNAMIC_FREQ

    // Dynamic frequency mode parameter.
    if (parser.seenval('D')) {
      if (CMPNSTR_HAS_SHAPER(X_AXIS) || CMPNSTR_HAS_SHAPER(Y_AXIS)) {
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
            flag.report_h = true;
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
      if (CMPNSTR_HAS_SHAPER(X_AXIS)) {
        const float val = parser.value_float();
        // TODO: Frequency minimum is dependent on the shaper used; the above check isn't always correct.
        if (WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2)) {
          ftMotion.cfg.baseFreq[X_AXIS] = val;
          flag.update_shpr_params = flag.reset_ft = flag.report_h = true;
        }
        else // Frequency out of range.
          SERIAL_ECHOLNPGM("Invalid [", C('A'), "] frequency value.");
      }
      else // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("Wrong mode for [", C('A'), "] frequency.");
    }

    #if HAS_DYNAMIC_FREQ
      // Parse frequency scaling parameter (X axis).
      if (parser.seenval('F')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK[X_AXIS] = parser.value_float();
          flag.report_h = true;
        }
        else
          SERIAL_ECHOLNPGM("Wrong mode for [", C('F'), "] frequency scaling.");
      }
    #endif

    // Parse zeta parameter (X axis).
    if (parser.seenval('I')) {
      const float val = parser.value_float();
      if (CMPNSTR_HAS_SHAPER(X_AXIS)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta[0] = val;
          flag.update_shpr_params = true;
        }
        else
          SERIAL_ECHOLNPGM("Invalid X zeta [", C('I'), "] value."); // Zeta out of range.
      }
      else
        SERIAL_ECHOLNPGM("Wrong mode for zeta parameter.");
    }

    // Parse vtol parameter (X axis).
    if (parser.seenval('Q')) {
      const float val = parser.value_float();
      if (CMPNSTR_IS_EISHAPER(X_AXIS)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol[0] = val;
          flag.update_shpr_params = true;
        }
        else
          SERIAL_ECHOLNPGM("Invalid X vtol [", C('Q'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("Wrong mode for vtol parameter.");
    }

  #endif // HAS_X_AXIS

  #if HAS_Y_AXIS

    // Parse frequency parameter (Y axis).
    if (parser.seenval('B')) {
      if (CMPNSTR_HAS_SHAPER(Y_AXIS)) {
        const float val = parser.value_float();
        if (WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2)) {
          ftMotion.cfg.baseFreq[Y_AXIS] = val;
          flag.update_shpr_params = flag.reset_ft = flag.report_h = true;
        }
        else // Frequency out of range.
          SERIAL_ECHOLNPGM("Invalid frequency [", C('B'), "] value.");
      }
      else // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("Wrong mode for [", C('B'), "] frequency.");
    }

    #if HAS_DYNAMIC_FREQ
      // Parse frequency scaling parameter (Y axis).
      if (parser.seenval('H')) {
        if (modeUsesDynFreq) {
          ftMotion.cfg.dynFreqK[Y_AXIS] = parser.value_float();
          flag.report_h = true;
        }
        else
          SERIAL_ECHOLNPGM("Wrong mode for [", C('H'), "] frequency scaling.");
      }
    #endif

    // Parse zeta parameter (Y axis).
    if (parser.seenval('J')) {
      const float val = parser.value_float();
      if (CMPNSTR_HAS_SHAPER(Y_AXIS)) {
        if (WITHIN(val, 0.01f, 1.0f)) {
          ftMotion.cfg.zeta[1] = val;
          flag.update_shpr_params = true;
        }
        else
          SERIAL_ECHOLNPGM("Invalid Y zeta [", C('J'), "] value."); // Zeta Out of range
      }
      else
        SERIAL_ECHOLNPGM("Wrong mode for zeta parameter.");
    }

    // Parse vtol parameter (Y axis).
    if (parser.seenval('R')) {
      const float val = parser.value_float();
      if (CMPNSTR_IS_EISHAPER(Y_AXIS)) {
        if (WITHIN(val, 0.00f, 1.0f)) {
          ftMotion.cfg.vtol[1] = val;
          flag.update_shpr_params = true;
        }
        else
          SERIAL_ECHOLNPGM("Invalid Y vtol [", C('R'), "] value."); // VTol out of range.
      }
      else
        SERIAL_ECHOLNPGM("Wrong mode for vtol parameter.");
    }

  #endif // HAS_Y_AXIS

  planner.synchronize();

  if (flag.update_shpr_params) ftMotion.update_shaping_params();

  if (flag.reset_ft) {
    stepper.ftMotion_syncPosition();
    ftMotion.reset();
  }

  if (flag.report_h) say_shaping();
}

#endif // FT_MOTION
