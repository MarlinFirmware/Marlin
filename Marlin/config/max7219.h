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
 * MAX7219 Debug Matrix
 *
 * Add support for a low-cost 8x8 LED Matrix based on the Max7219 chip as a realtime status display.
 * Requires 3 signal wires. Some useful debug options are included to demonstrate its usage.
 */
//#define MAX7219_DEBUG
#if ENABLED(MAX7219_DEBUG)
  #define MAX7219_CLK_PIN   64
  #define MAX7219_DIN_PIN   57
  #define MAX7219_LOAD_PIN  44

  //#define MAX7219_GCODE          // Add the M7219 G-code to control the LED matrix
  #define MAX7219_INIT_TEST    2   // Test pattern at startup: 0=none, 1=sweep, 2=spiral
  #define MAX7219_NUMBER_UNITS 1   // Number of Max7219 units in chain.
  #define MAX7219_ROTATE       0   // Rotate the display clockwise (in multiples of +/- 90°)
                                   // connector at:  right=0   bottom=-90  top=90  left=180
  //#define MAX7219_REVERSE_ORDER  // The order of the LED matrix units may be reversed
  //#define MAX7219_REVERSE_EACH   // The LEDs in each matrix unit row may be reversed
  //#define MAX7219_SIDE_BY_SIDE   // Big chip+matrix boards can be chained side-by-side

  /**
   * Sample debug features
   * If you add more debug displays, be careful to avoid conflicts!
   */
  #define MAX7219_DEBUG_PRINTER_ALIVE     // Blink corner LED of 8x8 matrix to show that the firmware is functioning
  #define MAX7219_DEBUG_PLANNER_HEAD  2   // Show the planner queue head position on this and the next LED matrix row
  #define MAX7219_DEBUG_PLANNER_TAIL  4   // Show the planner queue tail position on this and the next LED matrix row

  #define MAX7219_DEBUG_PLANNER_QUEUE 0   // Show the current planner queue depth on this and the next LED matrix row
                                          // If you experience stuttering, reboots, etc. this option can reveal how
                                          // tweaks made to the configuration are affecting the printer in real-time.
  #define MAX7219_DEBUG_PROFILE       6   // Display the fraction of CPU time spent in profiled code on this LED matrix
                                          // row. By default idle() is profiled so this shows how "idle" the processor is.
                                          // See class CodeProfiler.
  //#define MAX7219_DEBUG_MULTISTEPPING 6 // Show multi-stepping 1 to 128 on this LED matrix row.
#endif
