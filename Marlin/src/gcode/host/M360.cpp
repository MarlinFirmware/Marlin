/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

/**
 * M360 Report Printer Configuration - Repetier Firmware
 * See https://github.com/repetier/Repetier-Firmware/blob/master/src/ArduinoAVR/Repetier/Printer.cpp
 */
#if ENABLED(REPETIER_GCODE_M360)

#include "../gcode.h"

#include "../../module/motion.h"
#include "../../module/planner.h"

#if HAS_EXTRUDERS
  #include "../../module/temperature.h"
#endif

#include <cstddef>

struct ProgStr {
  PGM_P ptr;
  constexpr ProgStr(PGM_P p) : ptr(p) {}
  ProgStr(FSTR_P f) : ptr(FTOP(f)) {}
  ProgStr(std::nullptr_t) : ptr(nullptr) {}

  constexpr operator PGM_P() const { return ptr; }
  constexpr explicit operator bool() const { return ptr != nullptr; }
};

static void config_prefix(ProgStr name, ProgStr pref=nullptr, const int8_t ind=-1) {
  SERIAL_ECHOPGM("Config:");
  if (pref) SERIAL_ECHOPGM_P(static_cast<PGM_P>(pref));
  if (ind >= 0) { SERIAL_ECHO(ind); SERIAL_CHAR(':'); }
  SERIAL_ECHOPGM_P(static_cast<PGM_P>(name), C(':'));
}

template<typename T>
static void config_line(ProgStr name, const T val, ProgStr pref=nullptr, const int8_t ind=-1) {
  config_prefix(name, pref, ind);
  SERIAL_ECHOLN(val);
}

template<typename T>
static void config_line_e(const int8_t e, ProgStr name, const T val) {
  config_line(name, val, PSTR("Extr."), e + 1);
}

/**
 * M360: Report Firmware configuration
 *       in RepRapFirmware-compatible format
 */
void GcodeSuite::M360() {
  //
  // Basics and Enabled items
  //
  config_line(F("Baudrate"),                    BAUDRATE);
  config_line(F("InputBuffer"),                 MAX_CMD_SIZE);
  config_line(F("PrintlineCache"),              BUFSIZE);
  config_line(F("MixingExtruder"),              bool(ENABLED(MIXING_EXTRUDER)));
  config_line(F("SDCard"),                      bool(ENABLED(HAS_MEDIA)));
  config_line(F("Fan"),                         bool(ENABLED(HAS_FAN)));
  config_line(F("LCD"),                         bool(ENABLED(HAS_DISPLAY)));
  config_line(F("SoftwarePowerSwitch"),         1);
  config_line(F("SupportLocalFilamentchange"),  bool(ENABLED(ADVANCED_PAUSE_FEATURE)));
  config_line(F("CaseLights"),                  bool(ENABLED(CASE_LIGHT_ENABLE)));
  config_line(F("ZProbe"),                      bool(ENABLED(HAS_BED_PROBE)));
  config_line(F("Autolevel"),                   bool(ENABLED(HAS_LEVELING)));
  config_line(F("EEPROM"),                      bool(ENABLED(EEPROM_SETTINGS)));

  //
  // Axis letters, in PROGMEM
  //
  #define _DEFINE_A_STR(Q) PGMSTR(Q##_STR, STR_##Q);
  MAIN_AXIS_MAP(_DEFINE_A_STR);

  //
  // Homing Directions
  //
  PGMSTR(H_DIR_STR, "HomeDir");
  #if X_HOME_DIR
    config_line(H_DIR_STR, X_HOME_DIR, X_STR);
  #endif
  #if Y_HOME_DIR
    config_line(H_DIR_STR, Y_HOME_DIR, Y_STR);
  #endif
  #if Z_HOME_DIR
    config_line(H_DIR_STR, Z_HOME_DIR, Z_STR);
  #endif
  #if I_HOME_DIR
    config_line(H_DIR_STR, I_HOME_DIR, I_STR);
  #endif
  #if J_HOME_DIR
    config_line(H_DIR_STR, J_HOME_DIR, J_STR);
  #endif
  #if K_HOME_DIR
    config_line(H_DIR_STR, K_HOME_DIR, K_STR);
  #endif
  #if U_HOME_DIR
    config_line(H_DIR_STR, U_HOME_DIR, U_STR);
  #endif
  #if V_HOME_DIR
    config_line(H_DIR_STR, V_HOME_DIR, V_STR);
  #endif
  #if W_HOME_DIR
    config_line(H_DIR_STR, W_HOME_DIR, W_STR);
  #endif

  #if ANY(CLASSIC_JERK, HAS_LINEAR_E_JERK)
    PGMSTR(JERK_STR, "Jerk");
  #endif

  //
  // XYZ Axis Jerk
  //
  #if ENABLED(CLASSIC_JERK)
    #define _REPORT_JERK(Q) config_line(Q##_STR, planner.max_jerk.Q, JERK_STR);
    if (TERN0(HAS_Y_AXIS, planner.max_jerk.x == planner.max_jerk.y))
      config_line(F("XY"), planner.max_jerk.x, JERK_STR);
    else {
      XY_MAP(_REPORT_JERK);
    }
    TERN_(HAS_Z_AXIS, config_line(Z_STR, planner.max_jerk.z, JERK_STR));
    SECONDARY_AXIS_MAP(_REPORT_JERK);
  #endif

  //
  // Firmware Retraction
  //
  config_line(F("SupportG10G11"), ENABLED(FWRETRACT));
  #if ENABLED(FWRETRACT)
    PGMSTR(RET_STR, "Retraction");
    PGMSTR(UNRET_STR, "RetractionUndo");
    PGMSTR(SPEED_STR, "Speed");
    // M10 Retract with swap (long) moves
    config_line(F("Length"),     fwretract.settings.retract_length, RET_STR);
    config_line(F("LongLength"), fwretract.settings.swap_retract_length, RET_STR);
    config_line(SPEED_STR,       fwretract.settings.retract_feedrate_mm_s, RET_STR);
    config_line(F("ZLift"),      fwretract.settings.retract_zraise, RET_STR);
    // M11 Recover (undo) with swap (long) moves
    config_line(F("ExtraLength"),     fwretract.settings.retract_recover_extra, UNRET_STR);
    config_line(F("ExtraLongLength"), fwretract.settings.swap_retract_recover_extra, UNRET_STR);
    config_line(SPEED_STR,            fwretract.settings.retract_recover_feedrate_mm_s, UNRET_STR);
    config_line(F("LongSpeed"),       fwretract.settings.swap_retract_recover_feedrate_mm_s, UNRET_STR);
  #endif

  //
  // Workspace boundaries
  //
  const xyz_pos_t dmin = NUM_AXIS_ARRAY(X_MIN_POS, Y_MIN_POS, Z_MIN_POS, I_MIN_POS, J_MIN_POS, K_MIN_POS, U_MIN_POS, V_MIN_POS, W_MIN_POS),
                  dmax = NUM_AXIS_ARRAY(X_MAX_POS, Y_MAX_POS, Z_MAX_POS, I_MAX_POS, J_MAX_POS, K_MAX_POS, U_MAX_POS, V_MAX_POS, W_MAX_POS);
  xyz_pos_t cmin = dmin, cmax = dmax;
  apply_motion_limits(cmin);
  apply_motion_limits(cmax);
  const xyz_pos_t wmin = cmin.asLogical(), wmax = cmax.asLogical();

  PGMSTR(MIN_STR, "Min");
  #define _REPORT_MIN(Q) config_line(MIN_STR, wmin.Q, Q##_STR);
  MAIN_AXIS_MAP(_REPORT_MIN);

  PGMSTR(MAX_STR, "Max");
  #define _REPORT_MAX(Q) config_line(MAX_STR, wmax.Q, Q##_STR);
  MAIN_AXIS_MAP(_REPORT_MAX);

  PGMSTR(SIZE_STR, "Size");
  #define _REPORT_SIZE(Q) config_line(SIZE_STR, wmax.Q - wmin.Q, Q##_STR);
  MAIN_AXIS_MAP(_REPORT_SIZE);

  //
  // Axis Steps per mm
  //
  PGMSTR(S_MM_STR, "Steps/mm");
  #define _REPORT_S_MM(Q) config_line(S_MM_STR, planner.settings.axis_steps_per_mm[_AXIS(Q)], Q##_STR);
  MAIN_AXIS_MAP(_REPORT_S_MM);

  //
  // Print and Travel Acceleration
  //
  #define _ACCEL(Q,B) _MIN(planner.settings.max_acceleration_mm_per_s2[Q##_AXIS], planner.settings.B)

  PGMSTR(P_ACC_STR, "PrintAccel");
  #define _REPORT_P_ACC(Q) config_line(P_ACC_STR, _ACCEL(Q, acceleration), Q##_STR);
  MAIN_AXIS_MAP(_REPORT_P_ACC);

  PGMSTR(T_ACC_STR, "TravelAccel");
  #define _REPORT_T_ACC(Q) config_line(T_ACC_STR, _ACCEL(Q, travel_acceleration), Q##_STR);
  MAIN_AXIS_MAP(_REPORT_T_ACC);

  //
  // Printer Type
  //
  config_prefix(PSTR("PrinterType"));
  SERIAL_ECHOLNPGM(
    TERN_(DELTA,         "Delta")
    TERN_(IS_SCARA,      "SCARA")
    TERN_(POLAR,         "Polar")
    TERN_(IS_CORE,       "Core")
    TERN_(MARKFORGED_XY, "MarkForgedXY")
    TERN_(MARKFORGED_YX, "MarkForgedYX")
    TERN_(IS_CARTESIAN,  "Cartesian")
  );

  //
  // Heated Bed
  //
  config_line(F("HeatedBed"), ENABLED(HAS_HEATED_BED));
  #if HAS_HEATED_BED
    config_line(F("MaxBedTemp"), BED_MAX_TARGET);
  #endif

  //
  // Per-Extruder settings
  //
  config_line(F("NumExtruder"), EXTRUDERS);
  #if HAS_EXTRUDERS
    EXTRUDER_LOOP() {
      #if HAS_LINEAR_E_JERK
        config_line_e(e, JERK_STR, planner.max_e_jerk[E_INDEX_N(e)]);
      #elif ENABLED(CLASSIC_JERK)
        config_line_e(e, JERK_STR, planner.max_jerk.e);
      #endif
      config_line_e(e, F("Acceleration"), planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(e)]);
      config_line_e(e, F("MaxSpeed"), planner.settings.max_feedrate_mm_s[E_AXIS_N(e)]);
      config_line_e(e, F("Diameter"), TERN(NO_VOLUMETRICS, DEFAULT_NOMINAL_FILAMENT_DIA, planner.filament_size[e]));
      config_line_e(e, F("MaxTemp"), thermalManager.hotend_maxtemp[e]);
    }
  #endif
}

#endif // REPETIER_GCODE_M360
