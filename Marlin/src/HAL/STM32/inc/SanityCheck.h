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
 * Test STM32-specific configuration values for errors at compile-time.
 */
//#if ENABLED(SPINDLE_LASER_USE_PWM) && !(SPINDLE_LASER_PWM_PIN == 4 || SPINDLE_LASER_PWM_PIN == 6 || SPINDLE_LASER_PWM_PIN == 11)
//  #error "SPINDLE_LASER_PWM_PIN must use SERVO0, SERVO1 or SERVO3 connector"
//#endif


#if ENABLED(SDCARD_EEPROM_EMULATION) && DISABLED(SDSUPPORT)
  #undef SDCARD_EEPROM_EMULATION // Avoid additional error noise
  #if USE_FALLBACK_EEPROM
    #warning "EEPROM type not specified. Fallback is SDCARD_EEPROM_EMULATION."
  #endif
  #error "SDCARD_EEPROM_EMULATION requires SDSUPPORT. Enable SDSUPPORT or choose another EEPROM emulation."
#endif

#if !defined(STM32F4xx) && ENABLED(FLASH_EEPROM_LEVELING)
  #error "FLASH_EEPROM_LEVELING is currently only supported on STM32F4 hardware."
#endif

#if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
  #error "SERIAL_STATS_MAX_RX_QUEUED is not supported on STM32."
#elif ENABLED(SERIAL_STATS_DROPPED_RX)
  #error "SERIAL_STATS_DROPPED_RX is not supported on STM32."
#endif

#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI) && NOT_TARGET(STM32H7xx, STM32F4xx, STM32F1xx)
  #error "TFT_COLOR_UI, TFT_LVGL_UI and TFT_CLASSIC_UI are currently only supported on STM32H7, STM32F4 and STM32F1 hardware."
#endif

/**
 * Check for common serial pin conflicts
 */
#define CHECK_SERIAL_PIN(N) ( \
     BTN_EN1 == N || DOGLCD_CS == N || HEATER_BED_PIN == N || FAN_PIN == N || \
     SDIO_D2_PIN == N || SDIO_D3_PIN == N || SDIO_CK_PIN == N || SDIO_CMD_PIN == N \
)
#if CONF_SERIAL_IS(1)
  #if defined(UART1_TX_PIN) && CHECK_SERIAL_PIN(UART1_TX_PIN)
    #error "Serial Port 1 TX IO pins conflict with another pin on the board."
  #endif
  #if defined(UART1_RX_PIN) && CHECK_SERIAL_PIN(UART1_RX_PIN)
    #error "Serial Port 1 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if CONF_SERIAL_IS(2)
  #if defined(UART2_TX_PIN) && CHECK_SERIAL_PIN(UART2_TX_PIN)
    #error "Serial Port 2 TX IO pins conflict with another pin on the board."
  #endif
  #if defined(UART2_RX_PIN) && CHECK_SERIAL_PIN(UART2_RX_PIN)
    #error "Serial Port 2 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if CONF_SERIAL_IS(3)
  #if defined(UART3_TX_PIN) && CHECK_SERIAL_PIN(UART3_TX_PIN)
    #error "Serial Port 3 TX IO pins conflict with another pin on the board."
  #endif
  #if defined(UART3_RX_PIN) && CHECK_SERIAL_PIN(UART3_RX_PIN)
    #error "Serial Port 3 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if CONF_SERIAL_IS(4)
  #if defined(UART4_TX_PIN) && CHECK_SERIAL_PIN(UART4_TX_PIN)
    #error "Serial Port 4 TX IO pins conflict with another pin on the board."
  #endif
  #if defined(UART4_RX_PIN) && CHECK_SERIAL_PIN(UART4_RX_PIN)
    #error "Serial Port 4 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if CONF_SERIAL_IS(5)
  #if defined(UART5_TX_PIN) && CHECK_SERIAL_PIN(UART5_TX_PIN)
    #error "Serial Port 5 TX IO pins conflict with another pin on the board."
  #endif
  #if defined(UART5_RX_PIN) && CHECK_SERIAL_PIN(UART5_RX_PIN)
    #error "Serial Port 5 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if CONF_SERIAL_IS(6)
  #if defined(UART6_TX_PIN) && CHECK_SERIAL_PIN(UART6_TX_PIN)
    #error "Serial Port 6 TX IO pins conflict with another pin on the board."
  #endif
  #if defined(UART6_RX_PIN) && CHECK_SERIAL_PIN(UART6_RX_PIN)
    #error "Serial Port 6 RX IO pins conflict with another pin on the board."
  #endif
#endif
#undef CHECK_SERIAL_PIN
