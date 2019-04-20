/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Firmware-based and LCD-controlled retract
 *
 * Add G10 / G11 commands for automatic firmware-based retract / recover.
 * Use M207 and M208 to define parameters for retract / recover.
 *
 * Use M209 to enable or disable auto-retract.
 * With auto-retract enabled, all G1 E moves within the set range
 * will be converted to firmware-based retract/recover moves.
 *
 * Be sure to turn off auto-retract during filament change.
 *
 * Note that M207 / M208 / M209 settings are saved to EEPROM.
 *
 */
//#define FWRETRACT
#if ENABLED(FWRETRACT)
  #define FWRETRACT_AUTORETRACT           // costs ~500 bytes of PROGMEM
  #if ENABLED(FWRETRACT_AUTORETRACT)
    #define MIN_AUTORETRACT 0.1           // When auto-retract is on, convert E moves of this length and over
    #define MAX_AUTORETRACT 10.0          // Upper limit for auto-retract conversion
  #endif
  #define RETRACT_LENGTH 3                // Default retract length (positive mm)
  #define RETRACT_LENGTH_SWAP 13          // Default swap retract length (positive mm), for extruder change
  #define RETRACT_FEEDRATE 45             // Default feedrate for retracting (mm/s)
  #define RETRACT_ZRAISE 0                // Default retract Z-raise (mm)
  #define RETRACT_RECOVER_LENGTH 0        // Default additional recover length (mm, added to retract length when recovering)
  #define RETRACT_RECOVER_LENGTH_SWAP 0   // Default additional swap recover length (mm, added to retract length when recovering from extruder change)
  #define RETRACT_RECOVER_FEEDRATE 8      // Default feedrate for recovering from retraction (mm/s)
  #define RETRACT_RECOVER_FEEDRATE_SWAP 8 // Default feedrate for recovering from swap retraction (mm/s)
  #if ENABLED(MIXING_EXTRUDER)
    //#define RETRACT_SYNC_MIXING         // Retract and restore all mixing steppers simultaneously
  #endif
#endif

