/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// adapted from  I2C/master/master.c example
//   https://www-users.cs.york.ac.uk/~pcc/MCP/HAPR-Course-web/CMSIS/examples/html/master_8c_source.html

#ifdef __PLAT_NATIVE_SIM__

#include <cstdint>

#ifdef __cplusplus
  extern "C" {
#endif

uint8_t u8g_i2c_start(const uint8_t sla) {
  return 1;
}

void u8g_i2c_init(const uint8_t clock_option) {
}

uint8_t u8g_i2c_send_byte(uint8_t data) {
  return 1;
}

void u8g_i2c_stop() {
}

#ifdef __cplusplus
  }
#endif

#endif // __PLAT_NATIVE_SIM__
