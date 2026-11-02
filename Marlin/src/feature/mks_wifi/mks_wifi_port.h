/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * feature/mks_wifi/mks_wifi_port.h
 *
 * Maps WIFI_SERIAL_PORT (set by the pins file) onto the USART and the DMA
 * receive channel the MKS WiFi module drives directly.
 *
 * The module doesn't go through MarlinSerial: it takes the USART over, runs it
 * at 1958400 baud to flash the ESP, and receives bulk file data over DMA. That
 * needs the concrete peripheral, the DMA channel wired to its RX, and (on the
 * STM32F1 maple HAL) the name of the USART interrupt vector.
 *
 * Every MKS board that carries the module wires it to USART1, so that's the
 * only mapping exercised on real hardware. Ports 2 and 3 follow the reference
 * manuals and are here so a new board is a pins-file change, but they have not
 * been tested.
 */

#include "../../inc/MarlinConfig.h"

#ifndef WIFI_SERIAL_PORT
  #error "MKS_WIFI_MODULE requires WIFI_SERIAL_PORT to be set by the pins file."
#endif

//
// The USART itself
//
#if WIFI_SERIAL_PORT == 1
  #define WIFI_USART        USART1
  #define WIFI_USART_BASE   USART1_BASE
  #define WIFI_USART_IRQ    __irq_usart1
#elif WIFI_SERIAL_PORT == 2
  #define WIFI_USART        USART2
  #define WIFI_USART_BASE   USART2_BASE
  #define WIFI_USART_IRQ    __irq_usart2
#elif WIFI_SERIAL_PORT == 3
  #define WIFI_USART        USART3
  #define WIFI_USART_BASE   USART3_BASE
  #define WIFI_USART_IRQ    __irq_usart3
#else
  #error "MKS_WIFI_MODULE only supports WIFI_SERIAL_PORT 1, 2 or 3."
#endif

//
// DMA channel/stream wired to that USART's RX
//
#ifdef __STM32F1__

  // Maple HAL. DMA1 throughout: USART1_RX=CH5, USART2_RX=CH6, USART3_RX=CH3.
  #define WIFI_DMA_DEV      DMA1
  #if WIFI_SERIAL_PORT == 1
    #define WIFI_DMA_CH     DMA_CH5
    #define WIFI_DMA_CH_NUM 5
  #elif WIFI_SERIAL_PORT == 2
    #define WIFI_DMA_CH     DMA_CH6
    #define WIFI_DMA_CH_NUM 6
  #else
    #define WIFI_DMA_CH     DMA_CH3
    #define WIFI_DMA_CH_NUM 3
  #endif

  // Interrupt status flags sit in a 4-bit field per channel
  #define WIFI_DMA_IFCR_MASK (0xF << ((WIFI_DMA_CH_NUM - 1) * 4))

#elif defined(STM32F1xx)

  // STM32 HAL on F1. Same channel mapping as above.
  #define WIFI_DMA_CLK_ENABLE()  __HAL_RCC_DMA1_CLK_ENABLE()
  #if WIFI_SERIAL_PORT == 1
    #define WIFI_DMA_INSTANCE    DMA1_Channel5
  #elif WIFI_SERIAL_PORT == 2
    #define WIFI_DMA_INSTANCE    DMA1_Channel6
  #else
    #define WIFI_DMA_INSTANCE    DMA1_Channel3
  #endif

#else

  // STM32 HAL on F4 and friends. All three USARTs use DMA channel 4, but
  // USART1_RX is on DMA2 while USART2_RX / USART3_RX are on DMA1.
  #define WIFI_DMA_REQUEST       DMA_CHANNEL_4
  #if WIFI_SERIAL_PORT == 1
    #define WIFI_DMA_CLK_ENABLE()  __HAL_RCC_DMA2_CLK_ENABLE()
    #define WIFI_DMA_INSTANCE    DMA2_Stream2
  #elif WIFI_SERIAL_PORT == 2
    #define WIFI_DMA_CLK_ENABLE()  __HAL_RCC_DMA1_CLK_ENABLE()
    #define WIFI_DMA_INSTANCE    DMA1_Stream5
  #else
    #define WIFI_DMA_CLK_ENABLE()  __HAL_RCC_DMA1_CLK_ENABLE()
    #define WIFI_DMA_INSTANCE    DMA1_Stream1
  #endif

#endif
