/**
 * FYSETC
 *
 * Copyright (c) 2019 SoftwareSerialM [https://github.com/FYSETC/SoftwareSerialM]
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

#define HAL_PLATFORM_AVR          0
#define HAL_PLATFORM_DUE          1
#define HAL_PLATFORM_TEENSY31_32  2
#define HAL_PLATFORM_TEENSY35_36  3
#define HAL_PLATFORM_LPC1768      4
#define HAL_PLATFORM_STM32F1      5
#define HAL_PLATFORM_STM32_F4_F7  6
#define HAL_PLATFORM_STM32        7
#define HAL_PLATFORM_ESP32        8
#define HAL_PLATFORM_LINUX        9
#define HAL_PLATFORM_SAMD51      10

#ifndef HAL_SS_PLATFORM
  #ifdef __AVR__
    #define HAL_SS_PLATFORM HAL_PLATFORM_AVR
  #elif defined(ARDUINO_ARCH_SAM)
    #define HAL_SS_PLATFORM HAL_PLATFORM_DUE
  #elif defined(__MK20DX256__)
    #define HAL_SS_PLATFORM HAL_PLATFORM_TEENSY31_32
  #elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
    #define HAL_SS_PLATFORM HAL_PLATFORM_TEENSY35_36
  #elif defined(TARGET_LPC1768)
    #define HAL_SS_PLATFORM HAL_PLATFORM_LPC1768
  #elif defined(__STM32F1__) || defined(TARGET_STM32F1)
    #define HAL_SS_PLATFORM HAL_PLATFORM_STM32F1
  #elif defined(STM32GENERIC) && (defined(STM32F4) || defined(STM32F7))
    #define HAL_SS_PLATFORM HAL_PLATFORM_STM32_F4_F7
  #elif defined(ARDUINO_ARCH_STM32)
    #define HAL_SS_PLATFORM HAL_PLATFORM_STM32
  #elif defined(ARDUINO_ARCH_ESP32)
    #define HAL_SS_PLATFORM HAL_PLATFORM_ESP32
  #elif defined(__PLAT_LINUX__)
    #define HAL_SS_PLATFORM HAL_PLATFORM_LINUX
  #elif defined(__SAMD51__)
    #define HAL_SS_PLATFORM HAL_PLATFORM_SAMD51
  #else
    #error "Unsupported Platform!"
  #endif
#endif