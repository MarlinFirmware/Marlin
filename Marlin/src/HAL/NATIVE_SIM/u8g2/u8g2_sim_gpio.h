/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * U8G2 GPIO and Delay functions for NATIVE_SIM platform
 *
 * These functions provide the hardware abstraction layer for U8G2
 * on the NATIVE_SIM platform, using the same pin simulation system
 * as the existing u8glib implementation.
 */

#ifdef __PLAT_NATIVE_SIM__

#include <stdint.h>
#include <U8g2lib.h>

#ifdef __cplusplus
extern "C" {
#endif

// U8G2 GPIO and delay callback function
uint8_t u8x8_native_sim_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

// U8G2 4-wire SW SPI callback function
uint8_t u8x8_byte_native_sim_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

// U8G2 HW SPI callback function (simulation)
uint8_t u8x8_byte_native_sim_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#ifdef __cplusplus
}
#endif

#endif // __PLAT_NATIVE_SIM__
