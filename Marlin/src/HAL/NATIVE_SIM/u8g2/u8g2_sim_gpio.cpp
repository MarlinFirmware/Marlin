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

/**
 * U8G2 GPIO and Delay functions for NATIVE_SIM platform
 *
 * These functions provide the hardware abstraction layer for U8G2
 * on the NATIVE_SIM platform, using the same pin simulation system
 * as the existing u8glib implementation.
 */

#ifdef __PLAT_NATIVE_SIM__

#include "../../../inc/MarlinConfig.h"

#if HAS_MARLINUI_U8G2

#include <U8g2lib.h>
#include "../fastio.h"
#include "../../shared/Delay.h"
#include "u8g2_sim_gpio.h"

// Use the same pin functions as the existing u8g implementation
extern "C" {
  void u8g_SetPinOutput(uint8_t internal_pin_number);
  void u8g_SetPinInput(uint8_t internal_pin_number);
  void u8g_SetPinLevel(uint8_t pin, uint8_t pin_status);
  uint8_t u8g_GetPinLevel(uint8_t pin);

  // SPI transfer functions from u8glib implementation
  uint8_t swSpiTransfer_mode_0(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin);
  uint8_t swSpiTransfer_mode_3(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin);
}

// SPI transfer functions for U8G2 (copied from u8glib implementation)
uint8_t u8g2_swSpiTransfer_mode_0(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin ) {
  for (uint8_t i = 0; i < 8; ++i) {
    if (spi_speed == 0) {
      WRITE_PIN(mosi_pin, !!(b & 0x80));
      WRITE_PIN(sck_pin, HIGH);
      b <<= 1;
      if (miso_pin >= 0 && READ_PIN(miso_pin)) b |= 1;
      WRITE_PIN(sck_pin, LOW);
    }
    else {
      const uint8_t state = (b & 0x80) ? HIGH : LOW;
      for (uint8_t j = 0; j < spi_speed; ++j)
        WRITE_PIN(mosi_pin, state);

      for (uint8_t j = 0; j < spi_speed + (miso_pin >= 0 ? 0 : 1); ++j)
        WRITE_PIN(sck_pin, HIGH);

      b <<= 1;
      if (miso_pin >= 0 && READ_PIN(miso_pin)) b |= 1;

      for (uint8_t j = 0; j < spi_speed; ++j)
        WRITE_PIN(sck_pin, LOW);
    }
  }
  return b;
}

uint8_t u8g2_swSpiTransfer_mode_3(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin ) {
  for (uint8_t i = 0; i < 8; ++i) {
    const uint8_t state = (b & 0x80) ? HIGH : LOW;
    if (spi_speed == 0) {
      WRITE_PIN(sck_pin, LOW);
      WRITE_PIN(mosi_pin, state);
      WRITE_PIN(mosi_pin, state);  // need some setup time
      WRITE_PIN(sck_pin, HIGH);
    }
    else {
      for (uint8_t j = 0; j < spi_speed + (miso_pin >= 0 ? 0 : 1); ++j)
        WRITE_PIN(sck_pin, LOW);

      for (uint8_t j = 0; j < spi_speed; ++j)
        WRITE_PIN(mosi_pin, state);

      for (uint8_t j = 0; j < spi_speed; ++j)
        WRITE_PIN(sck_pin, HIGH);
    }
    b <<= 1;
    if (miso_pin >= 0 && READ_PIN(miso_pin)) b |= 1;
  }
  return b;
}

#undef SPI_SPEED
#define SPI_SPEED 2  // About 2 MHz

static uint8_t sck_pin, mosi_pin, cs_pin, dc_pin, reset_pin;

/**
 * U8G2 GPIO and delay callback function for NATIVE_SIM
 * This uses the same pin simulation system as the existing u8glib implementation
 */
uint8_t u8x8_native_sim_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  // Safety check
  if (!u8x8) return 0;

  switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      // Initialize pins - get pin numbers from u8x8 structure
      sck_pin = u8x8_GetPinValue(u8x8, U8X8_PIN_SPI_CLOCK);
      mosi_pin = u8x8_GetPinValue(u8x8, U8X8_PIN_SPI_DATA);
      cs_pin = u8x8_GetPinValue(u8x8, U8X8_PIN_CS);
      dc_pin = u8x8_GetPinValue(u8x8, U8X8_PIN_DC);
      reset_pin = u8x8_GetPinValue(u8x8, U8X8_PIN_RESET);

      // Set pin directions using the simulator-compatible functions
      if (sck_pin != U8X8_PIN_NONE) u8g_SetPinOutput(sck_pin);
      if (mosi_pin != U8X8_PIN_NONE) u8g_SetPinOutput(mosi_pin);
      if (cs_pin != U8X8_PIN_NONE) u8g_SetPinOutput(cs_pin);
      if (dc_pin != U8X8_PIN_NONE) u8g_SetPinOutput(dc_pin);
      if (reset_pin != U8X8_PIN_NONE) u8g_SetPinOutput(reset_pin);
      break;

    case U8X8_MSG_GPIO_SPI_CLOCK:
      if (sck_pin != U8X8_PIN_NONE) u8g_SetPinLevel(sck_pin, arg_int);
      break;

    case U8X8_MSG_GPIO_SPI_DATA:
      if (mosi_pin != U8X8_PIN_NONE) u8g_SetPinLevel(mosi_pin, arg_int);
      break;

    case U8X8_MSG_GPIO_CS:
      if (cs_pin != U8X8_PIN_NONE) u8g_SetPinLevel(cs_pin, arg_int);
      break;

    case U8X8_MSG_GPIO_DC:
      if (dc_pin != U8X8_PIN_NONE) u8g_SetPinLevel(dc_pin, arg_int);
      break;

    case U8X8_MSG_GPIO_RESET:
      if (reset_pin != U8X8_PIN_NONE) u8g_SetPinLevel(reset_pin, arg_int);
      break;

    case U8X8_MSG_DELAY_NANO:
      // Nanosecond delay - use minimum delay
      DELAY_NS(arg_int);
      break;

    case U8X8_MSG_DELAY_100NANO:
      // 100 nanosecond delay
      DELAY_NS(arg_int * 100);
      break;

    case U8X8_MSG_DELAY_10MICRO:
      // 10 microsecond delay
      DELAY_US(arg_int * 10);
      break;

    case U8X8_MSG_DELAY_MILLI:
      // Millisecond delay
      delay(arg_int);
      break;

    default:
      return 0;
  }
  return 1;
}

/**
 * U8G2 4-wire software SPI callback function for NATIVE_SIM
 */
uint8_t u8x8_byte_native_sim_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  // Safety check
  if (!u8x8) return 0;

  uint8_t *data;

  switch (msg) {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while (arg_int > 0) {
        // Directly transmit byte using SPI simulation - don't call back into U8G2!
        // Use the pins that were stored during initialization
        if (sck_pin != U8X8_PIN_NONE && mosi_pin != U8X8_PIN_NONE) {
          #if U8G_SPI_USE_MODE_3
            u8g2_swSpiTransfer_mode_3(*data, SPI_SPEED, sck_pin, -1, mosi_pin);
          #else
            u8g2_swSpiTransfer_mode_0(*data, SPI_SPEED, sck_pin, -1, mosi_pin);
          #endif
        }
        data++;
        arg_int--;
      }
      break;

    case U8X8_MSG_BYTE_INIT:
      // Disable chip select
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;

    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;

    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;

    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;

    default:
      return 0;
  }
  return 1;
}

/**
 * U8G2 hardware SPI callback function for NATIVE_SIM (simulated)
 * Since this is a simulation, we'll use the same software SPI implementation
 */
uint8_t u8x8_byte_native_sim_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  // For simulation purposes, hardware SPI is the same as software SPI
  return u8x8_byte_native_sim_4wire_sw_spi(u8x8, msg, arg_int, arg_ptr);
}

#endif // HAS_MARLINUI_U8G2
#endif // __PLAT_NATIVE_SIM__
