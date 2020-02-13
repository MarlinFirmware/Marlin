/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(USE_CONTROLLER_FAN)
  #include "../../gcode.h"

  #include "../../../feature/controllerfan.h"

  /**
   * M710: Set controller fan settings
   * Yuu can set all parameters separate or together. See Examples.
   *
   * Examples:
   *   M710                   ; Return current Settings
   *   M710 S0                ; Turn off Controller Fan
   *   M710 I255              ; Set Fan Idle Speed Setting
   *   M710 R                 ; Reset settings to defaults
   *   M710 S255              ; Set Controller Fan Speed Setting to 100%
   *   M710 I255 A0           ; Set Fan Idle Speed Setting and AutoMode off
   *   M710 S255              ; Set Controller Fan Speed setting to 100%
   *   M710 I127 A1 S255 D160 ; Set Controller Fan idle Speed 50%, AutoMode On, Fan speed 100%, Duration to 160 Secs
   *
   */

   void GcodeSuite::M710() {

     if (parser.seenval('I') || parser.seenval('A') || parser.seenval('S') || parser.seenval('D') || parser.seen('U') || parser.seen('R') ) {
       if (parser.seenval('I')) {
         fanController.settings_fan.controllerFan_Idle_Speed = parser.byteval(  'I', fanController.settings_fan.controllerFan_Idle_Speed);
         SERIAL_ECHOLNPAIR("Idle Fan Speed is set to: ", fanController.settings_fan.controllerFan_Idle_Speed);
       }
       if (parser.seenval('A')) {
         fanController.settings_fan.controllerFan_AutoMode   = parser.boolval(  'A', fanController.settings_fan.controllerFan_AutoMode);
         SERIAL_ECHOPGM("Fan Auto Mode is set to: "); serialprintln_onoff(fanController.settings_fan.controllerFan_AutoMode);
       }
       if (parser.seenval('S')) {
         fanController.settings_fan.controllerFan_Speed      = parser.byteval(  'S', fanController.settings_fan.controllerFan_Speed);
         SERIAL_ECHOLNPAIR("Fan Speed is set to: ", fanController.settings_fan.controllerFan_Speed);
       }
       if (parser.seenval('D')) {
         fanController.settings_fan.controllerFan_Duration   = parser.ushortval('D', fanController.settings_fan.controllerFan_Duration);
         SERIAL_ECHOPAIR("Fan switch off Delay is set to: ", fanController.settings_fan.controllerFan_Duration); SERIAL_ECHOLN(" Seconds");
       }
       if (parser.seen('R')   ) {
         fanController.reset(); SERIAL_ECHOLN("Reset Controller Fan default settings successfully");
       }
       fanController.update(); SERIAL_ECHOLN("Updated Controller Fan settings. To save permanently use M500.");
     }
     else {
       SERIAL_ECHOLNPGM("\nM710 - Controller Fan - Current Settings: ");
       serial_echopair_PGM( PSTR("Fan Speed: "), (float) (fanController.settings_fan.controllerFan_Speed / 2.55) );
       serial_echopair_PGM( PSTR("% (Raw: "), (unsigned int) fanController.settings_fan.controllerFan_Speed);
       SERIAL_ECHOPGM(")\n");
       serial_echopair_PGM( PSTR("Fan Idle Speed: "), (float) (fanController.settings_fan.controllerFan_Idle_Speed / 2.55) );
       serial_echopair_PGM( PSTR("% (Raw: "), (unsigned int) fanController.settings_fan.controllerFan_Idle_Speed);
       SERIAL_ECHOPGM(")\n");
       SERIAL_ECHOPGM("Fan Auto Mode: "); serialprint_onoff(fanController.settings_fan.controllerFan_AutoMode);
       SERIAL_ECHOPGM("\n");
       serial_echopair_PGM( PSTR("Fan Switch off Delay: ") , (float) (fanController.settings_fan.controllerFan_Duration / 60.00 ) );
       serial_echopair_PGM( fanController.settings_fan.controllerFan_Duration >= 60 ? PSTR(" Minutes (RAW: ") : PSTR(" Seconds: " ),
                            (unsigned int) fanController.settings_fan.controllerFan_Duration );
       SERIAL_ECHOPGM(" Seconds)");

       SERIAL_EOL();
     }
   }

#endif // USE_CONTROLLER_FAN
