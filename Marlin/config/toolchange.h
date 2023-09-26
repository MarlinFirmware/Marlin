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
#pragma once
CONFIG_VERSION(02020000)

/**
 * Universal tool change settings.
 * Applies to all types of extruders except where explicitly noted.
 */
#if HAS_MULTI_EXTRUDER
  // Z raise distance for tool-change, as needed for some extruders
  #define TOOLCHANGE_ZRAISE                 2 // (mm)
  //#define TOOLCHANGE_ZRAISE_BEFORE_RETRACT  // Apply raise before swap retraction (if enabled)
  //#define TOOLCHANGE_NO_RETURN              // Never return to previous position on tool-change
  #if ENABLED(TOOLCHANGE_NO_RETURN)
    //#define EVENT_GCODE_AFTER_TOOLCHANGE "G12X"   // Extra G-code to run after tool-change
  #endif

  /**
   * Extra G-code to run while executing tool-change commands. Can be used to use an additional
   * stepper motor (e.g., I axis in Configuration.h) to drive the tool-changer.
   */
  //#define EVENT_GCODE_TOOLCHANGE_T0 "G28 A\nG1 A0"  // Extra G-code to run while executing tool-change command T0
  //#define EVENT_GCODE_TOOLCHANGE_T1 "G1 A10"        // Extra G-code to run while executing tool-change command T1
  //#define EVENT_GCODE_TOOLCHANGE_ALWAYS_RUN         // Always execute above G-code sequences. Use with caution!

  /**
   * Consider coordinates for EVENT_GCODE_TOOLCHANGE_Tx as relative to T0
   * so that moves in the specified axes are the same for all tools.
   */
  //#define TC_GCODE_USE_GLOBAL_X   // Use X position relative to Tool 0
  //#define TC_GCODE_USE_GLOBAL_Y   // Use Y position relative to Tool 0
  //#define TC_GCODE_USE_GLOBAL_Z   // Use Z position relative to Tool 0

  /**
   * Tool Sensors detect when tools have been picked up or dropped.
   * Requires the pins TOOL_SENSOR1_PIN, TOOL_SENSOR2_PIN, etc.
   */
  //#define TOOL_SENSOR

  /**
   * Retract and prime filament on tool-change to reduce
   * ooze and stringing and to get cleaner transitions.
   */
  //#define TOOLCHANGE_FILAMENT_SWAP
  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    // Load / Unload
    #define TOOLCHANGE_FS_LENGTH              12  // (mm) Load / Unload length
    #define TOOLCHANGE_FS_EXTRA_RESUME_LENGTH  0  // (mm) Extra length for better restart. Adjust with LCD or M217 B.
    #define TOOLCHANGE_FS_RETRACT_SPEED   (50*60) // (mm/min) (Unloading)
    #define TOOLCHANGE_FS_UNRETRACT_SPEED (25*60) // (mm/min) (On SINGLENOZZLE or Bowden loading must be slowed down)

    // Longer prime to clean out a SINGLENOZZLE
    #define TOOLCHANGE_FS_EXTRA_PRIME          0  // (mm) Extra priming length
    #define TOOLCHANGE_FS_PRIME_SPEED    (4.6*60) // (mm/min) Extra priming feedrate
    #define TOOLCHANGE_FS_WIPE_RETRACT         0  // (mm) Cutting retraction out of park, for less stringing, better wipe, etc. Adjust with LCD or M217 G.

    // Cool after prime to reduce stringing
    #define TOOLCHANGE_FS_FAN                 -1  // Fan index or -1 to skip
    #define TOOLCHANGE_FS_FAN_SPEED          255  // 0-255
    #define TOOLCHANGE_FS_FAN_TIME            10  // (seconds)

    // Use TOOLCHANGE_FS_PRIME_SPEED feedrate the first time each extruder is primed
    //#define TOOLCHANGE_FS_SLOW_FIRST_PRIME

    /**
     * Prime T0 the first time T0 is sent to the printer:
     *  [ Power-On -> T0 { Activate & Prime T0 } -> T1 { Retract T0, Activate & Prime T1 } ]
     * If disabled, no priming on T0 until switching back to T0 from another extruder:
     *  [ Power-On -> T0 { T0 Activated } -> T1 { Activate & Prime T1 } -> T0 { Retract T1, Activate & Prime T0 } ]
     * Enable with M217 V1 before printing to avoid unwanted priming on host connect.
     */
    //#define TOOLCHANGE_FS_PRIME_FIRST_USED

    /**
     * Tool Change Migration
     * This feature provides G-code and LCD options to switch tools mid-print.
     * All applicable tool properties are migrated so the print can continue.
     * Tools must be closely matching and other restrictions may apply.
     * Useful to:
     *   - Change filament color without interruption
     *   - Switch spools automatically on filament runout
     *   - Switch to a different nozzle on an extruder jam
     */
    #define TOOLCHANGE_MIGRATION_FEATURE

  #endif

  /**
   * Position to park head during tool change.
   * Doesn't apply to SWITCHING_TOOLHEAD, DUAL_X_CARRIAGE, or PARKING_EXTRUDER
   */
  //#define TOOLCHANGE_PARK
  #if ENABLED(TOOLCHANGE_PARK)
    #define TOOLCHANGE_PARK_XY    { X_MIN_POS + 10, Y_MIN_POS + 10 }
    #define TOOLCHANGE_PARK_XY_FEEDRATE 6000  // (mm/min)
    //#define TOOLCHANGE_PARK_X_ONLY          // X axis only move
    //#define TOOLCHANGE_PARK_Y_ONLY          // Y axis only move
  #endif
#endif // HAS_MULTI_EXTRUDER
