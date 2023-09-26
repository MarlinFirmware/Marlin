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
 * POLAR Kinematics
 *  developed by Kadir ilkimen for PolarBear CNC and babyBear
 *  https://github.com/kadirilkimen/Polar-Bear-Cnc-Machine
 *  https://github.com/kadirilkimen/babyBear-3D-printer
 *
 * A polar machine can have different configurations.
 * This kinematics is only compatible with the following configuration:
 *        X : Independent linear
 *   Y or B : Polar
 *        Z : Independent linear
 *
 * For example, PolarBear has CoreXZ plus Polar Y or B.
 *
 * Motion problem for Polar axis near center / origin:
 *
 * 3D printing:
 * Movements very close to the center of the polar axis take more time than others.
 * This brief delay results in more material deposition due to the pressure in the nozzle.
 *
 * Current Kinematics and feedrate scaling deals with this by making the movement as fast
 * as possible. It works for slow movements but doesn't work well with fast ones. A more
 * complicated extrusion compensation must be implemented.
 *
 * Ideally, it should estimate that a long rotation near the center is ahead and will cause
 * unwanted deposition. Therefore it can compensate the extrusion beforehand.
 *
 * Laser cutting:
 * Same thing would be a problem for laser engraving too. As it spends time rotating at the
 * center point, more likely it will burn more material than it should. Therefore similar
 * compensation would be implemented for laser-cutting operations.
 *
 * Milling:
 * This shouldn't be a problem for cutting/milling operations.
 */
//#define POLAR
#if ENABLED(POLAR)
  #define DEFAULT_SEGMENTS_PER_SECOND 180   // If movement is choppy try lowering this value
  #define PRINTABLE_RADIUS 82.0f            // (mm) Maximum travel of X axis

  // Movements fall inside POLAR_FAST_RADIUS are assigned the highest possible feedrate
  // to compensate unwanted deposition related to the near-origin motion problem.
  #define POLAR_FAST_RADIUS 3.0f            // (mm)

  // Radius which is unreachable by the tool.
  // Needed if the tool is not perfectly aligned to the center of the polar axis.
  #define POLAR_CENTER_OFFSET 0.0f          // (mm)

  #define FEEDRATE_SCALING                  // Convert XY feedrate from mm/s to degrees/s on the fly
#endif
