/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * MKS BASE with Heroic HR4982 stepper drivers
 */

#include "pins_MKS_BASE_15.h"

/**
 * Some new boards use HR4982 (Heroic) instead of the A4982 (Allegro) stepper drivers.
 * Most the functionality is similar, the HR variant obviously doesn't work with diode
 * smoothers (no fast decay). And the Heroic has a 128 µStepping mode where the A4982
 * is doing quarter steps (MS1=0, MS2=1).
 */
#define HEROIC_STEPPER_DRIVERS
