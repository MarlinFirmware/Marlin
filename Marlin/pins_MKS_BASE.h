/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * MKS BASE 1.0 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 *
 * Rev B - Override pin definitions for CASE_LIGHT and M3/M4/M5 spindle control
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS BASE 1.0 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "MKS BASE 1.0"

/* Microstepping pins (reverse engineered at V1.4 - due to closed source schematics)
// Some new batches have the HR4982 (Heroic) instead of the A4982 (Allegro) as stepper driver. While most of the functionality is similar, the HR variant obviously doesn't work with diode smoothers (no fast decay)
// But the Heroic has a 128 µStepping mode where the A4982 is doing quarter steps (MS1=L / MS2=H). To achieve comfortable tests with the M350/M351 commands, the following definitions have to made:
// Example: M350 X4 Y4 ; Set X and Y Axis to quarterstep Mode to achieve MS1=0 and MS2=1
// A new board with a HR4982 will now perform 128 µSteps per Fullstep
// XSTEP,YSTEP ... must be adapted with M92 accordingly (128/16 => multiply by factor 8).
*/
#define X_MS1_PIN           5   // Digital 3  / Pin 5   / PE3
#define X_MS2_PIN           6   // Digital 6  / Pin 14  / PH3
#define Y_MS1_PIN           59  // Analog 5   / Pin 92  / PF5
#define Y_MS2_PIN           58  // Analog 4   / Pin 93  / PF4
#define Z_MS1_PIN           22  // Digital 22 / Pin 78  / PA0
#define Z_MS2_PIN           39  // Digital 39 / Pin 70  / PG2
#define E0_MS1_PIN          63  // Analog 9   / Pin 86  / PK1
#define E0_MS2_PIN          64  // Analog 10  / Pin 87  / PK2
#define E1_MS1_PIN          57  // Analog 3   / Pin 93  / PF3
#define E1_MS2_PIN          4   // Digital 4  / Pin 1   / PG5

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN 7

#define CASE_LIGHT_PIN            2

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN     2  // MUST BE HARDWARE PWM
#define SPINDLE_LASER_ENABLE_PIN 15  // Pin should have a pullup!
#define SPINDLE_DIR_PIN          19

#include "pins_RAMPS.h"
