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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * HAL/ST7920.h
 * For the HALs that provide direct access to the ST7920 display
 * (bypassing U8G), it will allow the LIGHTWEIGHT_UI to operate.
 */

#if HAS_GRAPHICAL_LCD && ENABLED(LIGHTWEIGHT_UI)
  void ST7920_cs();
  void ST7920_ncs();
  void ST7920_set_cmd();
  void ST7920_set_dat();
  void ST7920_write_byte(const uint8_t data);
#endif
