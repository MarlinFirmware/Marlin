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
 * Multi-Material Unit
 * Set to one of these predefined models:
 *
 *   PRUSA_MMU1           : Průša MMU1 (The "multiplexer" version)
 *   PRUSA_MMU2           : Průša MMU2
 *   PRUSA_MMU2S          : Průša MMU2S (Requires MK3S extruder with motion sensor, EXTRUDERS = 5)
 *   EXTENDABLE_EMU_MMU2  : MMU with configurable number of filaments (ERCF, SMuFF or similar with Průša MMU2 compatible firmware)
 *   EXTENDABLE_EMU_MMU2S : MMUS with configurable number of filaments (ERCF, SMuFF or similar with Průša MMU2 compatible firmware)
 *
 * Requires NOZZLE_PARK_FEATURE to park print head in case MMU unit fails.
 * See additional options in Configuration_adv.h.
 * :["PRUSA_MMU1", "PRUSA_MMU2", "PRUSA_MMU2S", "EXTENDABLE_EMU_MMU2", "EXTENDABLE_EMU_MMU2S"]
 */
//#define MMU_MODEL PRUSA_MMU2

/**
 * Multi-Material-Unit supported models
 */
#define PRUSA_MMU1             1
#define PRUSA_MMU2             2
#define PRUSA_MMU2S            3
#define EXTENDABLE_EMU_MMU2   12
#define EXTENDABLE_EMU_MMU2S  13

#ifdef MMU_MODEL
  #define HAS_MMU 1
  #if MMU_MODEL == PRUSA_MMU1
    #define HAS_PRUSA_MMU1 1
  #elif MMU_MODEL % 10 == PRUSA_MMU2
    #define HAS_PRUSA_MMU2 1
  #elif MMU_MODEL % 10 == PRUSA_MMU2S
    #define HAS_PRUSA_MMU2 1
    #define HAS_PRUSA_MMU2S 1
  #endif
  #if MMU_MODEL >= EXTENDABLE_EMU_MMU2
    #define HAS_EXTENDABLE_MMU 1
  #endif
#endif

#undef PRUSA_MMU1
#undef PRUSA_MMU2
#undef PRUSA_MMU2S
#undef EXTENDABLE_EMU_MMU2
#undef EXTENDABLE_EMU_MMU2S

/**
 * Průša Multi-Material Unit (MMU)
 * Enable in Configuration.h
 *
 * These devices allow a single stepper driver on the board to drive
 * multi-material feeders with any number of stepper motors.
 */
#if HAS_PRUSA_MMU1
  /**
   * This option only allows the multiplexer to switch on tool-change.
   * Additional options to configure custom E moves are pending.
   *
   * Override the default DIO selector pins here, if needed.
   * Some pins files may provide defaults for these pins.
   */
  //#define E_MUX0_PIN 40  // Always Required
  //#define E_MUX1_PIN 42  // Needed for 3 to 8 inputs
  //#define E_MUX2_PIN 44  // Needed for 5 to 8 inputs
#elif HAS_PRUSA_MMU2
  // Serial port used for communication with MMU2.
  #define MMU2_SERIAL_PORT 2

  // Use hardware reset for MMU if a pin is defined for it
  //#define MMU2_RST_PIN 23

  // Enable if the MMU2 has 12V stepper motors (MMU2 Firmware 1.0.2 and up)
  //#define MMU2_MODE_12V

  // G-code to execute when MMU2 F.I.N.D.A. probe detects filament runout
  #define MMU2_FILAMENT_RUNOUT_SCRIPT "M600"

  // Add an LCD menu for MMU2
  //#define MMU2_MENUS

  // Settings for filament load / unload from the LCD menu.
  // This is for Průša MK3-style extruders. Customize for your hardware.
  #define MMU2_FILAMENTCHANGE_EJECT_FEED 80.0
  #define MMU2_LOAD_TO_NOZZLE_SEQUENCE \
    {  7.2, 1145 }, \
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

  /**
   * Using a sensor like the MMU2S
   * This mode requires a MK3S extruder with a sensor at the extruder idler, like the MMU2S.
   * See https://help.prusa3d.com/guide/3b-mk3s-mk2-5s-extruder-upgrade_41560#42048, step 11
   */
  #if HAS_PRUSA_MMU2S
    #define MMU2_C0_RETRY   5             // Number of retries (total time = timeout*retries)

    #define MMU2_CAN_LOAD_FEEDRATE 800    // (mm/min)
    #define MMU2_CAN_LOAD_SEQUENCE \
      {  0.1, MMU2_CAN_LOAD_FEEDRATE }, \
      {  60.0, MMU2_CAN_LOAD_FEEDRATE }, \
      { -52.0, MMU2_CAN_LOAD_FEEDRATE }

    #define MMU2_CAN_LOAD_RETRACT   6.0   // (mm) Keep under the distance between Load Sequence values
    #define MMU2_CAN_LOAD_DEVIATION 0.8   // (mm) Acceptable deviation

    #define MMU2_CAN_LOAD_INCREMENT 0.2   // (mm) To reuse within MMU2 module
    #define MMU2_CAN_LOAD_INCREMENT_SEQUENCE \
      { -MMU2_CAN_LOAD_INCREMENT, MMU2_CAN_LOAD_FEEDRATE }

    // Continue unloading if sensor detects filament after the initial unload move
    //#define MMU_IR_UNLOAD_MOVE
  #else

    /**
     * MMU1 Extruder Sensor
     *
     * Support for a Průša (or other) IR Sensor to detect filament near the extruder
     * and make loading more reliable. Suitable for an extruder equipped with a filament
     * sensor less than 38mm from the gears.
     *
     * During loading the extruder will stop when the sensor is triggered, then do a last
     * move up to the gears. If no filament is detected, the MMU2 can make some more attempts.
     * If all attempts fail, a filament runout will be triggered.
     */
    //#define MMU_EXTRUDER_SENSOR
    #if ENABLED(MMU_EXTRUDER_SENSOR)
      #define MMU_LOADING_ATTEMPTS_NR 5 // max. number of attempts to load filament if first load fail
    #endif

  #endif

  //#define MMU2_DEBUG  // Write debug info to serial output

#endif // HAS_PRUSA_MMU2
