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

#if ENABLED(REPETIER_GCODE_M360)

#include "../gcode.h"

#include "../../module/motion.h"
#include "../../module/planner.h"

static void config_prefix(PGM_P const name, PGM_P const pref=nullptr) {
  SERIAL_ECHOPGM("Config:");
  if (pref) serialprintPGM(pref);
  serialprintPGM(name);
  SERIAL_CHAR(':');
}
static void config_line(PGM_P const name, const float val, PGM_P const pref=nullptr) {
  config_prefix(name, pref);
  SERIAL_ECHOLN(val);
}

/**
 * M360: Report Firmware configuration
 *       in RepRapFirmware-compatible format
 */
void GcodeSuite::M360() {
  PGMSTR(X_STR,    "X");
  PGMSTR(Y_STR,    "Y");
  PGMSTR(Z_STR,    "Z");
  PGMSTR(JERK_STR, "Jerk");

  //
  // Basics and Enabled items
  //
  config_line(PSTR("Baudrate"), BAUDRATE);
  config_line(PSTR("InputBuffer"), MAX_CMD_SIZE);
  config_line(PSTR("PrintlineCache"), BUFSIZE);
  config_line(PSTR("MixingExtruder"), ENABLED(MIXING_EXTRUDER));
  config_line(PSTR("SDCard"), ENABLED(SDSUPPORT));
  config_line(PSTR("Fan"), ENABLED(HAS_FAN));
  config_line(PSTR("LCD"), ENABLED(HAS_DISPLAY));
  config_line(PSTR("SoftwarePowerSwitch"), 1);
  config_line(PSTR("SupportLocalFilamentchange"), ENABLED(ADVANCED_PAUSE_FEATURE));
  config_line(PSTR("CaseLights"), ENABLED(CASE_LIGHT_ENABLE));
  config_line(PSTR("ZProbe"), ENABLED(HAS_BED_PROBE));
  config_line(PSTR("Autolevel"), ENABLED(HAS_LEVELING));
  config_line(PSTR("EEPROM"), ENABLED(EEPROM_SETTINGS));

  //
  // Homing Directions
  //
  PGMSTR(H_DIR_STR, "HomeDir");
  config_line(H_DIR_STR, X_HOME_DIR, X_STR);
  config_line(H_DIR_STR, Y_HOME_DIR, Y_STR);
  config_line(H_DIR_STR, Z_HOME_DIR, Z_STR);

  //
  // XYZ Axis Jerk
  //
  #if HAS_CLASSIC_JERK
    if (planner.max_jerk.x == planner.max_jerk.y)
      config_line(PSTR("XY"), planner.max_jerk.x, JERK_STR);
    else {
      config_line(X_STR, planner.max_jerk.x, JERK_STR);
      config_line(Y_STR, planner.max_jerk.y, JERK_STR);
    }
    config_line(Z_STR, planner.max_jerk.z, JERK_STR);
  #endif

  //
  // Firmware Retraction
  //
  config_line(PSTR("SupportG10G11"), ENABLED(FWRETRACT));
  #if ENABLED(FWRETRACT)
    PGMSTR(RET_STR, "Retraction");
    PGMSTR(UNRET_STR, "RetractionUndo");
    PGMSTR(SPEED_STR, "Speed");
    // M10 Retract with swap (long) moves
    config_line(PSTR("Length"), fwretract.settings.retract_length, RET_STR);
    config_line(SPEED_STR, fwretract.settings.retract_feedrate_mm_s, RET_STR);
    config_line(PSTR("ZLift"), fwretract.settings.retract_zraise, RET_STR);
    config_line(PSTR("LongLength"), fwretract.settings.swap_retract_length, RET_STR);
    // M11 Recover (undo) with swap (long) moves
    config_line(SPEED_STR, fwretract.settings.retract_recover_feedrate_mm_s, UNRET_STR);
    config_line(PSTR("ExtraLength"), fwretract.settings.retract_recover_extra, UNRET_STR);
    config_line(PSTR("ExtraLongLength"), fwretract.settings.swap_retract_recover_extra, UNRET_STR);
    config_line(PSTR("LongSpeed"), fwretract.settings.swap_retract_recover_feedrate_mm_s, UNRET_STR);
  #endif

  //
  // Workspace boundaries
  //
  const xyz_pos_t dmin = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS },
                  dmax = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
  xyz_pos_t cmin = dmin, cmax = dmax;
  apply_motion_limits(cmin);
  apply_motion_limits(cmax);
  const xyz_pos_t wmin = cmin.asLogical(), wmax = cmax.asLogical();

  PGMSTR(MIN_STR, "Min");
  PGMSTR(MAX_STR, "Max");
  PGMSTR(SIZE_STR, "Size");
  config_line(MIN_STR, wmin.x, X_STR);
  config_line(MIN_STR, wmin.y, Y_STR);
  config_line(MIN_STR, wmin.z, Z_STR);
  config_line(MAX_STR, wmax.x, X_STR);
  config_line(MAX_STR, wmax.y, Y_STR);
  config_line(MAX_STR, wmax.z, Z_STR);
  config_line(SIZE_STR, wmax.x - wmin.x, X_STR);
  config_line(SIZE_STR, wmax.y - wmin.y, Y_STR);
  config_line(SIZE_STR, wmax.z - wmin.z, Z_STR);

  //
  // Print and Travel Acceleration
  //
  #define _ACCEL(A,B) _MIN(planner.settings.max_acceleration_mm_per_s2[A##_AXIS], planner.settings.B)
  PGMSTR(P_ACC_STR, "PrintAccel");
  PGMSTR(T_ACC_STR, "TravelAccel");
  config_line(P_ACC_STR, _ACCEL(X, acceleration), X_STR);
  config_line(P_ACC_STR, _ACCEL(Y, acceleration), Y_STR);
  config_line(P_ACC_STR, _ACCEL(Z, acceleration), Z_STR);
  config_line(T_ACC_STR, _ACCEL(X, travel_acceleration), X_STR);
  config_line(T_ACC_STR, _ACCEL(Y, travel_acceleration), Y_STR);
  config_line(T_ACC_STR, _ACCEL(Z, travel_acceleration), Z_STR);

  config_prefix(PSTR("PrinterType"));
  SERIAL_ECHOLNPGM(
    TERN_(DELTA,         "Delta")
    TERN_(IS_SCARA,      "SCARA")
    TERN_(IS_CORE,       "Core")
    TERN_(MARKFORGED_XY, "MarkForged")
    TERN_(IS_CARTESIAN,  "Cartesian")
  );

  //
  // Heated Bed
  //
  config_line(PSTR("HeatedBed"), ENABLED(HAS_HEATED_BED));
  #if HAS_HEATED_BED
    config_line(PSTR("MaxBedTemp"), BED_MAX_TARGET);
  #endif

  //
  // Per-Extruder settings
  //
  config_line(PSTR("NumExtruder"), EXTRUDERS);
  #if EXTRUDERS
    #define DIAM_VALUE(N) TERN(NO_VOLUMETRICS, DEFAULT_NOMINAL_FILAMENT_DIA, planner.filament_size[N])
    #if HAS_LINEAR_E_JERK
      #define E_JERK_VAL(N) planner.max_e_jerk[E_INDEX_N(N)]
    #elif HAS_CLASSIC_JERK
      #define E_JERK_VAL(N) planner.max_jerk.e
    #else
      #define E_JERK_VAL(N) DEFAULT_EJERK
    #endif
    #define _EXTR_ITEM(N) do{ \
      PGMSTR(EXTR_STR, "Extr." STRINGIFY(INCREMENT(N)) ":"); \
      config_line(JERK_STR, E_JERK_VAL(N), EXTR_STR); \
      config_line(PSTR("MaxSpeed"), planner.settings.max_feedrate_mm_s[E_AXIS_N(N)], EXTR_STR); \
      config_line(PSTR("Acceleration"), planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(N)], EXTR_STR); \
      config_line(PSTR("Diameter"), DIAM_VALUE(N), EXTR_STR); \
      config_line(PSTR("MaxTemp"), (HEATER_##N##_MAXTEMP) - (HOTEND_OVERSHOOT), EXTR_STR); \
    }while(0)

    REPEAT(EXTRUDERS, _EXTR_ITEM);
  #endif
}

#endif
