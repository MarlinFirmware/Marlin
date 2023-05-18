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

void say_shaping() {
  SERIAL_ECHO_TERNARY(fxdTiCtrl.cfg_mode, "Fixed time controller ", "en", "dis", "abled");
  if (fxdTiCtrl.cfg_mode == ftMotionMode_DISABLED || fxdTiCtrl.cfg_mode == ftMotionMode_ENABLED) {
    SERIAL_ECHOLNPGM(".");
    return;
  }
  #if HAS_X_AXIS
    SERIAL_ECHOPGM(" with ");
    switch (fxdTiCtrl.cfg_mode) {
      default: break;
      //case ftMotionMode_ULENDO_FBS: SERIAL_ECHOLNPGM("Ulendo FBS."); return;
      case ftMotionMode_ZV: SERIAL_ECHOLNPGM("ZV"); break;
      case ftMotionMode_ZVD: SERIAL_ECHOLNPGM("ZVD"); break;
      case ftMotionMode_EI: SERIAL_ECHOLNPGM("EI"); break;
      case ftMotionMode_2HEI: SERIAL_ECHOLNPGM("2 Hump EI"); break;
      case ftMotionMode_3HEI: SERIAL_ECHOLNPGM("3 Hump EI"); break;
      case ftMotionMode_MZV: SERIAL_ECHOLNPGM("MZV"); break;
      //case ftMotionMode_DISCTF: SERIAL_ECHOLNPGM("discrete transfer functions"); break;
    }
    SERIAL_ECHOLNPGM(" shaping.");
  #endif
}

/**
 * M493: Set Fixed-time Motion Control parameters
 *
 *    S<mode> Set the motion / shaping mode. Shaping requires an X axis, at the minimum.
 *       0: NORMAL
 *       1: FIXED-TIME
 *      10: ZV
 *      11: ZVD
 *      12: EI
 *      13: 2HEI
 *      14: 3HEI
 *      15: MZV
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
 *    A<Hz> Set static/base frequency for the X axis
 *    F<Hz> Set frequency scaling for the X axis
 *
 *    B<Hz> Set static/base frequency for the Y axis
 *    H<Hz> Set frequency scaling for the Y axis
 */
void GcodeSuite::M493() {
  // Parse 'S' mode parameter.
  if (parser.seenval('S')) {
    const ftMotionMode_t val = (ftMotionMode_t)parser.value_byte();
    switch (val) {
      case ftMotionMode_DISABLED:
      case ftMotionMode_ENABLED:
      #if HAS_X_AXIS
        case ftMotionMode_ZVD:
        case ftMotionMode_2HEI:
        case ftMotionMode_3HEI:
        case ftMotionMode_MZV:
        //case ftMotionMode_ULENDO_FBS:
        //case ftMotionMode_DISCTF:
          fxdTiCtrl.cfg_mode = val;
          say_shaping();
          break;
      #endif
      default:
        SERIAL_ECHOLNPGM("?Invalid control mode [M] value.");
        return;
    }

    switch (val) {
      case ftMotionMode_ENABLED: fxdTiCtrl.reset(); break;
      #if HAS_X_AXIS
        case ftMotionMode_ZV:
        case ftMotionMode_ZVD:
        case ftMotionMode_EI:
        case ftMotionMode_2HEI:
        case ftMotionMode_3HEI:
        case ftMotionMode_MZV:
          fxdTiCtrl.updateShapingN(fxdTiCtrl.cfg_baseFreq[0] OPTARG(HAS_Y_AXIS, fxdTiCtrl.cfg_baseFreq[1]));
          fxdTiCtrl.updateShapingA();
          fxdTiCtrl.reset();
          break;
        //case ftMotionMode_ULENDO_FBS:
        //case ftMotionMode_DISCTF:
      #endif
      default: break;
    }
  }

  #if HAS_EXTRUDERS

    // Pressure control (linear advance) parameter.
    if (parser.seen('P')) {
      const bool val = parser.value_bool();
      fxdTiCtrl.cfg_linearAdvEna = val;
      SERIAL_ECHO_TERNARY(val, "Pressure control: Linear Advance ", "en", "dis", "abled.\n");
    }

    // Pressure control (linear advance) gain parameter.
    if (parser.seenval('K')) {
      const float val = parser.value_float();
      if (val >= 0.0f) {
        fxdTiCtrl.cfg_linearAdvK = val;
        SERIAL_ECHOPGM("Pressure control: Linear Advance gain set to: ");
        SERIAL_ECHO_F(val, 5);
        SERIAL_ECHOLNPGM(".");
      }
      else { // Value out of range.
        SERIAL_ECHOLNPGM("Pressure control: Linear Advance gain out of range.");
      }
    }

  #endif // HAS_EXTRUDERS

  #if HAS_Z_AXIS || HAS_EXTRUDERS

    // Dynamic frequency mode parameter.
    if (parser.seenval('D')) {
      if (WITHIN(fxdTiCtrl.cfg_mode, 10U, 19U)) {
        const dynFreqMode_t val = dynFreqMode_t(parser.value_byte());
        switch (val) {
          case dynFreqMode_DISABLED:
            fxdTiCtrl.cfg_dynFreqMode = val;
            SERIAL_ECHOLNPGM("Dynamic frequency mode disabled.");
            break;
          #if HAS_Z_AXIS
            case dynFreqMode_Z_BASED:
              fxdTiCtrl.cfg_dynFreqMode = val;
              SERIAL_ECHOLNPGM("Z-based Dynamic Frequency Mode.");
              break;
          #endif
          #if HAS_EXTRUDERS
            case dynFreqMode_MASS_BASED:
              fxdTiCtrl.cfg_dynFreqMode = val;
              SERIAL_ECHOLNPGM("Mass-based Dynamic Frequency Mode.");
              break;
          #endif
          default:
            SERIAL_ECHOLNPGM("?Invalid Dynamic Frequency Mode [D] value.");
            break;
        }
      }
      else {
        SERIAL_ECHOLNPGM("Incompatible shaper for [D] Dynamic Frequency mode.");
      }
    }

  #endif // HAS_Z_AXIS || HAS_EXTRUDERS

  #if HAS_X_AXIS

    // Parse frequency parameter (X axis).
    if (parser.seenval('A')) {
      if (WITHIN(fxdTiCtrl.cfg_mode, 10U, 19U)) {
        const float val = parser.value_float();
        const bool frequencyInRange = WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2);
        // TODO: Frequency minimum is dependent on the shaper used; the above check isn't always correct.
        if (frequencyInRange) {
          fxdTiCtrl.cfg_baseFreq[0] = val;
          fxdTiCtrl.updateShapingN(fxdTiCtrl.cfg_baseFreq[0] OPTARG(HAS_Y_AXIS, fxdTiCtrl.cfg_baseFreq[1]));
          fxdTiCtrl.reset();
          if (fxdTiCtrl.cfg_dynFreqMode) { SERIAL_ECHOPGM("Compensator base dynamic frequency (X/A axis) set to:"); }
          else { SERIAL_ECHOPGM("Compensator static frequency (X/A axis) set to: "); }
          SERIAL_ECHO_F( fxdTiCtrl.cfg_baseFreq[0], 2 );
          SERIAL_ECHOLNPGM(".");
        }
        else { // Frequency out of range.
          SERIAL_ECHOLNPGM("Invalid [A] frequency value.");
        }
      }
      else { // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("Incompatible mode for [A] frequency.");
      }
    }

    #if HAS_Z_AXIS || HAS_EXTRUDERS
      // Parse frequency scaling parameter (X axis).
      if (parser.seenval('F')) {
        const bool modeUsesDynFreq = (
             TERN0(HAS_Z_AXIS,    fxdTiCtrl.cfg_dynFreqMode == dynFreqMode_Z_BASED)
          || TERN0(HAS_EXTRUDERS, fxdTiCtrl.cfg_dynFreqMode == dynFreqMode_MASS_BASED)
        );

        if (modeUsesDynFreq) {
          const float val = parser.value_float();
          fxdTiCtrl.cfg_dynFreqK[0] = val;
          SERIAL_ECHOPGM("Frequency scaling (X/A axis) set to: ");
          SERIAL_ECHO_F(fxdTiCtrl.cfg_dynFreqK[0], 8);
          SERIAL_ECHOLNPGM(".");
        }
        else {
          SERIAL_ECHOLNPGM("Incompatible mode for [F] frequency scaling.");
        }
      }
    #endif // HAS_Z_AXIS || HAS_EXTRUDERS

  #endif // HAS_X_AXIS

  #if HAS_Y_AXIS

    // Parse frequency parameter (Y axis).
    if (parser.seenval('B')) {
      if (WITHIN(fxdTiCtrl.cfg_mode, 10U, 19U)) {
        const float val = parser.value_float();
        const bool frequencyInRange = WITHIN(val, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2);
        if (frequencyInRange) {
          fxdTiCtrl.cfg_baseFreq[1] = val;
          fxdTiCtrl.updateShapingN(fxdTiCtrl.cfg_baseFreq[0] OPTARG(HAS_Y_AXIS, fxdTiCtrl.cfg_baseFreq[1]));
          fxdTiCtrl.reset();
          if (fxdTiCtrl.cfg_dynFreqMode) { SERIAL_ECHOPGM("Compensator base dynamic frequency (Y/B axis) set to:"); }
          else { SERIAL_ECHOPGM("Compensator static frequency (Y/B axis) set to: "); }
          SERIAL_ECHO_F( fxdTiCtrl.cfg_baseFreq[1], 2 );
          SERIAL_ECHOLNPGM(".");
        }
        else { // Frequency out of range.
          SERIAL_ECHOLNPGM("Invalid frequency [B] value.");
        }
      }
      else { // Mode doesn't use frequency.
        SERIAL_ECHOLNPGM("Incompatible mode for [B] frequency.");
      }
    }

    #if HAS_Z_AXIS || HAS_EXTRUDERS
      // Parse frequency scaling parameter (Y axis).
      if (parser.seenval('H')) {
        const bool modeUsesDynFreq = (
             TERN0(HAS_Z_AXIS,    fxdTiCtrl.cfg_dynFreqMode == dynFreqMode_Z_BASED)
          || TERN0(HAS_EXTRUDERS, fxdTiCtrl.cfg_dynFreqMode == dynFreqMode_MASS_BASED)
        );

        if (modeUsesDynFreq) {
          const float val = parser.value_float();
          fxdTiCtrl.cfg_dynFreqK[1] = val;
          SERIAL_ECHOPGM("Frequency scaling (Y/B axis) set to: ");
          SERIAL_ECHO_F(val, 8);
          SERIAL_ECHOLNPGM(".");
        }
        else {
          SERIAL_ECHOLNPGM("Incompatible mode for [H] frequency scaling.");
        }
      }
    #endif // HAS_Z_AXIS || HAS_EXTRUDERS

  #endif // HAS_Y_AXIS
}

#endif // FT_MOTION
