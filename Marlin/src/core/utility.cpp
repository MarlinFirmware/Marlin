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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "utility.h"

#include "../MarlinCore.h"
#include "../module/temperature.h"

void safe_delay(millis_t ms) {
  while (ms > 50) {
    ms -= 50;
    delay(50);
    thermalManager.manage_heater();
  }
  delay(ms);
  thermalManager.manage_heater(); // This keeps us safe if too many small safe_delay() calls are made
}

// A delay to provide brittle hosts time to receive bytes
#if ENABLED(SERIAL_OVERRUN_PROTECTION)

  #include "../gcode/gcode.h" // for set_autoreport_paused

  void serial_delay(const millis_t ms) {
    const bool was = gcode.set_autoreport_paused(true);
    safe_delay(ms);
    gcode.set_autoreport_paused(was);
  }
#endif

#if ENABLED(DEBUG_LEVELING_FEATURE)

  #include "../module/probe.h"
  #include "../module/motion.h"
  #include "../module/stepper.h"
  #include "../libs/numtostr.h"
  #include "../feature/bedlevel/bedlevel.h"

  void log_machine_info() {
    SERIAL_ECHOLNPGM("Machine Type: "
      TERN(DELTA, "Delta", "")
      TERN(IS_SCARA, "SCARA", "")
      TERN(IS_CORE, "Core", "")
      TERN(IS_CARTESIAN, "Cartesian", "")
    );

    SERIAL_ECHOLNPGM("Probe: "
      TERN(PROBE_MANUALLY, "PROBE_MANUALLY", "")
      TERN(NOZZLE_AS_PROBE, "NOZZLE_AS_PROBE", "")
      TERN(FIX_MOUNTED_PROBE, "FIX_MOUNTED_PROBE", "")
      TERN(BLTOUCH, "BLTOUCH", "")
      TERN(HAS_Z_SERVO_PROBE, TERN(BLTOUCH, "BLTOUCH", "SERVO PROBE"), "")
      TERN(TOUCH_MI_PROBE, "TOUCH_MI_PROBE", "")
      TERN(Z_PROBE_SLED, "Z_PROBE_SLED", "")
      TERN(Z_PROBE_ALLEN_KEY, "Z_PROBE_ALLEN_KEY", "")
      TERN(SOLENOID_PROBE, "SOLENOID_PROBE", "")
      TERN(PROBE_SELECTED, "", "NONE")
    );

    #if HAS_BED_PROBE

      #if !HAS_PROBE_XY_OFFSET
        SERIAL_ECHOPAIR("Probe Offset X0 Y0 Z", probe.offset.z, " (");
      #else
        SERIAL_ECHOPAIR_P(PSTR("Probe Offset X"), probe.offset_xy.x, SP_Y_STR, probe.offset_xy.y, SP_Z_STR, probe.offset.z);
        if (probe.offset_xy.x > 0)
          SERIAL_ECHOPGM(" (Right");
        else if (probe.offset_xy.x < 0)
          SERIAL_ECHOPGM(" (Left");
        else if (probe.offset_xy.y != 0)
          SERIAL_ECHOPGM(" (Middle");
        else
          SERIAL_ECHOPGM(" (Aligned With");

        if (probe.offset_xy.y > 0)
          serialprintPGM(ENABLED(IS_SCARA) ? PSTR("-Distal") : PSTR("-Back"));
        else if (probe.offset_xy.y < 0)
          serialprintPGM(ENABLED(IS_SCARA) ? PSTR("-Proximal") : PSTR("-Front"));
        else if (probe.offset_xy.x != 0)
          SERIAL_ECHOPGM("-Center");

        SERIAL_ECHOPGM(" & ");

      #endif

      serialprintPGM(probe.offset.z < 0 ? PSTR("Below") : probe.offset.z > 0 ? PSTR("Above") : PSTR("Same Z as"));
      SERIAL_ECHOLNPGM(" Nozzle)");

    #endif

    #if HAS_ABL_OR_UBL
      SERIAL_ECHOPGM("Auto Bed Leveling: "
        TERN(AUTO_BED_LEVELING_LINEAR, "LINEAR", "")
        TERN(AUTO_BED_LEVELING_BILINEAR, "BILINEAR", "")
        TERN(AUTO_BED_LEVELING_3POINT, "3POINT", "")
        TERN(AUTO_BED_LEVELING_UBL, "UBL", "")
      );

      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height)
            SERIAL_ECHOLNPAIR("Z Fade: ", planner.z_fade_height);
        #endif
        #if ABL_PLANAR
          SERIAL_ECHOPGM("ABL Adjustment X");
          LOOP_XYZ(a) {
            float v = planner.get_axis_position_mm(AxisEnum(a)) - current_position[a];
            SERIAL_CHAR(' ', XYZ_CHAR(a));
            if (v > 0) SERIAL_CHAR('+');
            SERIAL_ECHO(v);
          }
        #else
          #if ENABLED(AUTO_BED_LEVELING_UBL)
            SERIAL_ECHOPGM("UBL Adjustment Z");
            const float rz = ubl.get_z_correction(current_position);
          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
            SERIAL_ECHOPGM("ABL Adjustment Z");
            const float rz = bilinear_z_offset(current_position);
          #endif
          SERIAL_ECHO(ftostr43sign(rz, '+'));
          #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
            if (planner.z_fade_height) {
              SERIAL_ECHOPAIR(" (", ftostr43sign(rz * planner.fade_scaling_factor_for_z(current_position.z), '+'));
              SERIAL_CHAR(')');
            }
          #endif
        #endif
      }
      else
        SERIAL_ECHOLNPGM(" (disabled)");

      SERIAL_EOL();

    #elif ENABLED(MESH_BED_LEVELING)

      SERIAL_ECHOPGM("Mesh Bed Leveling");
      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        SERIAL_ECHOPAIR("MBL Adjustment Z", ftostr43sign(mbl.get_z(current_position), '+'));
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height) {
            SERIAL_ECHOPAIR(" (", ftostr43sign(
              mbl.get_z(current_position, planner.fade_scaling_factor_for_z(current_position.z)), '+'
            ));
            SERIAL_CHAR(')');
          }
        #endif
      }
      else
        SERIAL_ECHOPGM(" (disabled)");

      SERIAL_EOL();

    #endif // MESH_BED_LEVELING
  }

#endif // DEBUG_LEVELING_FEATURE
