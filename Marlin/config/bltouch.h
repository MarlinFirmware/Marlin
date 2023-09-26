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

//#define BLTOUCH   // Or enable in config/probe.h

#if ENABLED(BLTOUCH)
  /**
   * Either: Use the defaults (recommended) or: For special purposes, use the following DEFINES
   * Do not activate settings that the probe might not understand. Clones might misunderstand
   * advanced commands.
   *
   * Note: If the probe is not deploying, do a "Reset" and "Self-Test" and then check the
   *       wiring of the BROWN, RED and ORANGE wires.
   *
   * Note: If the trigger signal of your probe is not being recognized, it has been very often
   *       because the BLACK and WHITE wires needed to be swapped. They are not "interchangeable"
   *       like they would be with a real switch. So please check the wiring first.
   *
   * Settings for all BLTouch and clone probes:
   */

  // Safety: The probe needs time to recognize the command.
  //         Minimum command delay (ms). Enable and increase if needed.
  //#define BLTOUCH_DELAY 500

  /**
   * Settings for BLTOUCH Classic 1.2, 1.3 or BLTouch Smart 1.0, 2.0, 2.2, 3.0, 3.1, and most clones:
   */

  // Feature: Switch into SW mode after a deploy. It makes the output pulse longer. Can be useful
  //          in special cases, like noisy or filtered input configurations.
  //#define BLTOUCH_FORCE_SW_MODE

  /**
   * Settings for BLTouch Smart 3.0 and 3.1
   * Summary:
   *   - Voltage modes: 5V and OD (open drain - "logic voltage free") output modes
   *   - High-Speed mode
   *   - Disable LCD voltage options
   */

  /**
   * Danger: Don't activate 5V mode unless attached to a 5V-tolerant controller!
   * V3.0 or 3.1: Set default mode to 5V mode at Marlin startup.
   * If disabled, OD mode is the hard-coded default on 3.0
   * On startup, Marlin will compare its EEPROM to this value. If the selected mode
   * differs, a mode set EEPROM write will be completed at initialization.
   * Use the option below to force an EEPROM write to a V3.1 probe regardless.
   */
  //#define BLTOUCH_SET_5V_MODE

  // Safety: Enable voltage mode settings in the LCD menu.
  //#define BLTOUCH_LCD_VOLTAGE_MENU

  /**
   * Safety: Activate if connecting a probe with an unknown voltage mode.
   * V3.0: Set a probe into mode selected above at Marlin startup. Required for 5V mode on 3.0
   * V3.1: Force a probe with unknown mode into selected mode at Marlin startup ( = Probe EEPROM write )
   * To preserve the life of the probe, use this once then turn it off and re-flash.
   */
  //#define BLTOUCH_FORCE_MODE_SET

  /**
   * Enable "HIGH SPEED" option for probing.
   * Danger: Disable if your probe sometimes fails. Only suitable for stable well-adjusted systems.
   * This feature was designed for Deltabots with very fast Z moves; however, higher speed Cartesians
   * might be able to use it. If the machine can't raise Z fast enough the BLTouch may go into ALARM.
   *
   * Set the default state here, change with 'M401 S' or UI, use M500 to save, M502 to reset.
   */
  //#define BLTOUCH_HS_MODE true

  #ifdef BLTOUCH_HS_MODE
    // The probe Z offset (M851 Z) is the height at which the probe triggers.
    // This must be large enough to keep the probe pin off the bed and prevent
    // it from snagging on the bed clips.
    #define BLTOUCH_HS_EXTRA_CLEARANCE    7 // Extra Z Clearance
  #endif

#endif // BLTOUCH
