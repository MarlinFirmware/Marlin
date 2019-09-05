/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(STM32F4) || defined(STM32F4xx)
  #define HAL_IS_STM32F4 1
#elif defined(STM32F7)
  #define HAL_IS_STM32F7 1
#endif

#ifdef __AVR__
  #define HAL_DIR HAL_AVR
  #define HAL_IS_AVR 1
#elif defined(ARDUINO_ARCH_SAM)
  #define HAL_DIR HAL_DUE
  #define HAL_IS_DUE 1
#elif defined(__MK20DX256__)
  #define HAL_DIR HAL_TEENSY31_32
  #define HAL_IS_TEENSY31_32 1
#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
  #define HAL_DIR HAL_TEENSY35_36
  #define HAL_IS_TEENSY31_36 1
#elif defined(TARGET_LPC1768)
  #define HAL_DIR HAL_LPC1768
  #define HAL_IS_LPC1768 1
#elif defined(__STM32F1__) || defined(TARGET_STM32F1)
  #define HAL_DIR HAL_STM32F1
  #define HAL_IS_STM32F1 1
#elif defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)
  #define HAL_DIR HAL_STM32
  #define HAL_IS_STM32 1
#elif HAL_IS_STM32F4 || HAL_IS_STM32F7
  #define HAL_DIR HAL_STM32_F4_F7
  #define HAL_IS_STM32_F4_F7 1
#elif defined(ARDUINO_ARCH_ESP32)
  #define HAL_DIR HAL_ESP32
  #define HAL_IS_ESP32 1
#elif defined(__PLAT_LINUX__)
  #define HAL_DIR HAL_LINUX
  #define HAL_IS_LINUX 1
#elif defined(__SAMD51__)
  #define HAL_DIR HAL_SAMD51
  #define HAL_IS_SAMD51 1
#else
  #error "Unsupported Platform!"
#endif

#define XSTR_(M) #M
#define XSTR(M) XSTR_(M)
#define HAL_PATH(PATH, NAME) XSTR(PATH/HAL_DIR/NAME)
