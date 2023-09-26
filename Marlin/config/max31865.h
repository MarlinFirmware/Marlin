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
 * Thermocouple Options — for MAX6675 (-2), MAX31855 (-3), and MAX31865 (-5).
 */
//#define TEMP_SENSOR_FORCE_HW_SPI                // Ignore SCK/MOSI/MISO pins; use CS and the default SPI bus.
//#define MAX31865_SENSOR_WIRES_0 2               // (2-4) Number of wires for the probe connected to a MAX31865 board.
//#define MAX31865_SENSOR_WIRES_1 2
//#define MAX31865_SENSOR_WIRES_2 2

//#define MAX31865_50HZ_FILTER                    // Use a 50Hz filter instead of the default 60Hz.
//#define MAX31865_USE_READ_ERROR_DETECTION       // Treat value spikes (20°C delta in under 1s) as read errors.

//#define MAX31865_USE_AUTO_MODE                  // Read faster and more often than 1-shot; bias voltage always on; slight effect on RTD temperature.
//#define MAX31865_MIN_SAMPLING_TIME_MSEC     100 // (ms) 1-shot: minimum read interval. Reduces bias voltage effects by leaving sensor unpowered for longer intervals.
//#define MAX31865_IGNORE_INITIAL_FAULTY_READS 10 // Ignore some read faults (keeping the temperature reading) to work around a possible issue (#23439).

//#define MAX31865_WIRE_OHMS_0              0.95f // For 2-wire, set the wire resistances for more accurate readings.
//#define MAX31865_WIRE_OHMS_1              0.0f
//#define MAX31865_WIRE_OHMS_2              0.0f
