/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// RP2040 internal temperature sensor
// Formula: T = 27 - (ADC_voltage - 0.706) / 0.001721
// ADC_voltage = (RAW / OVERSAMPLENR) * 3.3 / 4096  (RAW is accumulated over OVERSAMPLENR samples)
// T = 27 - ((RAW / OVERSAMPLENR) * 3.3 / 4096 - 0.706) / 0.001721
// Simplified: T = 437.423 - (RAW / OVERSAMPLENR) * 0.46875

#define TEMP_SOC_SENSOR(RAW) (437.423f - ((RAW) / OVERSAMPLENR) * 0.46875f)
