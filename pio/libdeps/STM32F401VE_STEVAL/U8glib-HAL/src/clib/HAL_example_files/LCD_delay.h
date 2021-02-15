/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * LCD delay routines - used by all the drivers.
 *
 * These are based on the LPC1768 routines.
 *
 * Couldn't just call exact copies because the overhead resulted in the
 * one microsecond delay being about 4uS.
 */

#ifdef __cplusplus
  extern "C" {
#endif

void U8g_delay(int msec);
void u8g_MicroDelay(void);
void u8g_10MicroDelay(void);

#ifdef __cplusplus
  }
#endif