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
 * Prusa Multi-Material Unit v2
 * Enable in Configuration.h
 */
#if ENABLED(PRUSA_MMU2)

  // Serial port used for communication with MMU2.
  // For AVR enable the UART port used for the MMU. (e.g., internalSerial)
  // For 32-bit boards check your HAL for available serial ports. (e.g., Serial2)
  #define INTERNAL_SERIAL_PORT 2
  #define MMU2_SERIAL internalSerial

  // Use hardware reset for MMU if a pin is defined for it
  //#define MMU2_RST_PIN 23

  // Enable if the MMU2 has 12V stepper motors (MMU2 Firmware 1.0.2 and up)
  //#define MMU2_MODE_12V

  // G-code to execute when MMU2 F.I.N.D.A. probe detects filament runout
  #define MMU2_FILAMENT_RUNOUT_SCRIPT "M600"

  // Add an LCD menu for MMU2
  //#define MMU2_MENUS
  #if ENABLED(MMU2_MENUS)
    // Settings for filament load / unload from the LCD menu.
    // This is for Prusa MK3-style extruders. Customize for your hardware.
    #define MMU2_FILAMENTCHANGE_EJECT_FEED 80.0
    #define MMU2_LOAD_TO_NOZZLE_SEQUENCE \
      {  7.2,  562 }, \
      { 14.4,  871 }, \
      { 36.0, 1393 }, \
      { 14.4,  871 }, \
      { 50.0,  198 }

    #define MMU2_RAMMING_SEQUENCE \
      {   1.0, 1000 }, \
      {   1.0, 1500 }, \
      {   2.0, 2000 }, \
      {   1.5, 3000 }, \
      {   2.5, 4000 }, \
      { -15.0, 5000 }, \
      { -14.0, 1200 }, \
      {  -6.0,  600 }, \
      {  10.0,  700 }, \
      { -10.0,  400 }, \
      { -50.0, 2000 }

  #endif

  //#define MMU2_DEBUG  // Write debug info to serial output

#endif // PRUSA_MMU2
