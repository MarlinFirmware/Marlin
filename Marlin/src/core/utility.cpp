/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../Marlin.h"
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

#if ENABLED(DEBUG_LEVELING_FEATURE)

  #include "../module/probe.h"
  #include "../module/motion.h"
  #include "../module/stepper.h"
  #include "../libs/numtostr.h"
  #include "../feature/bedlevel/bedlevel.h"

  void log_machine_info() {
    SERIAL_ECHOLNPGM("Machine Type: "
      #if ENABLED(DELTA)
        "Delta"
      #elif IS_SCARA
        "SCARA"
      #elif IS_CORE
        "Core"
      #else
        "Cartesian"
      #endif
    );

    SERIAL_ECHOLNPGM("Probe: "
      #if ENABLED(PROBE_MANUALLY)
        "PROBE_MANUALLY"
      #elif ENABLED(FIX_MOUNTED_PROBE)
        "FIX_MOUNTED_PROBE"
      #elif ENABLED(BLTOUCH)
        "BLTOUCH"
      #elif HAS_Z_SERVO_PROBE
        "SERVO PROBE"
      #elif ENABLED(TOUCH_MI_PROBE)
        "TOUCH_MI_PROBE"
      #elif ENABLED(Z_PROBE_SLED)
        "Z_PROBE_SLED"
      #elif ENABLED(Z_PROBE_ALLEN_KEY)
        "Z_PROBE_ALLEN_KEY"
      #elif ENABLED(SOLENOID_PROBE)
        "SOLENOID_PROBE"
      #else
        "NONE"
      #endif
    );

    #if HAS_BED_PROBE
      SERIAL_ECHOPAIR(
        "Probe Offset X:" STRINGIFY(zprobe_offset[X_AXIS])
                    " Y:" STRINGIFY(zprobe_offset[Y_AXIS])
                    " Z:", zprobe_offset[Z_AXIS]
      );
      if ((zprobe_offset[X_AXIS]) > 0)
        SERIAL_ECHOPGM(" (Right");
      else if ((zprobe_offset[X_AXIS]) < 0)
        SERIAL_ECHOPGM(" (Left");
      else if ((zprobe_offset[Y_AXIS]) != 0)
        SERIAL_ECHOPGM(" (Middle");
      else
        SERIAL_ECHOPGM(" (Aligned With");

      if ((zprobe_offset[Y_AXIS]) > 0) {
        #if IS_SCARA
          SERIAL_ECHOPGM("-Distal");
        #else
          SERIAL_ECHOPGM("-Back");
        #endif
      }
      else if ((zprobe_offset[Y_AXIS]) < 0) {
        #if IS_SCARA
          SERIAL_ECHOPGM("-Proximal");
        #else
          SERIAL_ECHOPGM("-Front");
        #endif
      }
      else if ((zprobe_offset[X_AXIS]) != 0)
        SERIAL_ECHOPGM("-Center");

      if (zprobe_offset[Z_AXIS] < 0)
        SERIAL_ECHOPGM(" & Below");
      else if (zprobe_offset[Z_AXIS] > 0)
        SERIAL_ECHOPGM(" & Above");
      else
        SERIAL_ECHOPGM(" & Same Z as");
      SERIAL_ECHOLNPGM(" Nozzle)");
    #endif

    #if HAS_ABL_OR_UBL
      SERIAL_ECHOLNPGM("Auto Bed Leveling: "
        #if ENABLED(AUTO_BED_LEVELING_LINEAR)
          "LINEAR"
        #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
          "BILINEAR"
        #elif ENABLED(AUTO_BED_LEVELING_3POINT)
          "3POINT"
        #elif ENABLED(AUTO_BED_LEVELING_UBL)
          "UBL"
        #endif
      );
      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height)
            SERIAL_ECHOLNPAIR("Z Fade: ", planner.z_fade_height);
        #endif
        #if ABL_PLANAR
          const float diff[XYZ] = {
            planner.get_axis_position_mm(X_AXIS) - current_position[X_AXIS],
            planner.get_axis_position_mm(Y_AXIS) - current_position[Y_AXIS],
            planner.get_axis_position_mm(Z_AXIS) - current_position[Z_AXIS]
          };
          SERIAL_ECHOPGM("ABL Adjustment X");
          if (diff[X_AXIS] > 0) SERIAL_CHAR('+');
          SERIAL_ECHO(diff[X_AXIS]);
          SERIAL_ECHOPGM(" Y");
          if (diff[Y_AXIS] > 0) SERIAL_CHAR('+');
          SERIAL_ECHO(diff[Y_AXIS]);
          SERIAL_ECHOPGM(" Z");
          if (diff[Z_AXIS] > 0) SERIAL_CHAR('+');
          SERIAL_ECHO(diff[Z_AXIS]);
        #else
          #if ENABLED(AUTO_BED_LEVELING_UBL)
            SERIAL_ECHOPGM("UBL Adjustment Z");
            const float rz = ubl.get_z_correction(current_position[X_AXIS], current_position[Y_AXIS]);
          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
            SERIAL_ECHOPGM("ABL Adjustment Z");
            const float rz = bilinear_z_offset(current_position);
          #endif
          SERIAL_ECHO(ftostr43sign(rz, '+'));
          #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
            if (planner.z_fade_height) {
              SERIAL_ECHOPAIR(" (", ftostr43sign(rz * planner.fade_scaling_factor_for_z(current_position[Z_AXIS]), '+'));
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
        SERIAL_ECHOPAIR("MBL Adjustment Z", ftostr43sign(mbl.get_z(current_position[X_AXIS], current_position[Y_AXIS]
          #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
            , 1.0
          #endif
        ), '+'));
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height) {
            SERIAL_ECHOPAIR(" (", ftostr43sign(
              mbl.get_z(current_position[X_AXIS], current_position[Y_AXIS], planner.fade_scaling_factor_for_z(current_position[Z_AXIS])), '+'
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
