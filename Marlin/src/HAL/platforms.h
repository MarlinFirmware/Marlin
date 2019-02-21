/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#ifdef __AVR__
  #define HAL_PLATFORM HAL_AVR
#elif defined(ARDUINO_ARCH_SAM)
  #define HAL_PLATFORM HAL_DUE
#elif defined(__MK20DX256__)
  #define HAL_PLATFORM HAL_TEENSY31_32
#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
  #define HAL_PLATFORM HAL_TEENSY35_36
#elif defined(TARGET_LPC1768)
  #define HAL_PLATFORM HAL_LPC1768
#elif defined(__STM32F1__) || defined(TARGET_STM32F1)
  #define HAL_PLATFORM HAL_STM32F1
#elif defined(STM32GENERIC) && defined(STM32F4)
  #define HAL_PLATFORM HAL_STM32F4
#elif defined(STM32GENERIC) && defined(STM32F7)
  #define HAL_PLATFORM HAL_STM32F7
#elif defined(ARDUINO_ARCH_STM32)
  #define HAL_PLATFORM HAL_STM32
#elif defined(ARDUINO_ARCH_ESP32)
  #define HAL_PLATFORM HAL_ESP32
#elif defined(__PLAT_LINUX__)
  #define HAL_PLATFORM HAL_LINUX
#else
  #error "Unsupported Platform!"
#endif

#define XSTR_(M) #M
#define XSTR(M) XSTR_(M)
#define HAL_PATH(PATH, NAME) XSTR(PATH/HAL_PLATFORM/NAME)
