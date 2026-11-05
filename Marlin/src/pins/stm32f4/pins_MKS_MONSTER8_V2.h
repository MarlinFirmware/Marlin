/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define BOARD_INFO_NAME "MKS Monster8 V2"

//
// Limit Switches
//
#define X_STOP_PIN                          PA14
#define Y_STOP_PIN                          PA15

//
// Steppers
//
#define E4_ENABLE_PIN                       PB6   // Driver7

//
// Misc. Functions
//
#define PW_DET                              PA13  // MT_DET
#define PW_OFF                              PB12  // Z+
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA13  // MT_DET
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PB12  // Z+
#endif

/**
 * The MKS bootloader hands over with USB OTG_HS still clocked and its embedded
 * PHY powered. That PHY drives the OTG_HS_DM / OTG_HS_DP pads (PB14 / PB15)
 * directly and overrides the GPIO output driver, so those two pins cannot be
 * driven at all - Marlin sets MODER and ODR and the pins never move. Release
 * it before anything else runs, unless this build really uses OTG_HS.
 *
 * Power the PHY down before stopping the clock; the register is unreachable
 * once the peripheral is unclocked.
 */
#if !defined(USE_USB_HS) && !defined(USE_USB_HS_IN_FS)
  #define BOARD_PREINIT() do{ \
    if (TEST(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN_Pos)) { \
      *(volatile uint32_t *)(USB_OTG_HS_PERIPH_BASE + 0x38) &= ~USB_OTG_GCCFG_PWRDWN; \
      __HAL_RCC_USB_OTG_HS_CLK_DISABLE(); \
    } \
  }while(0)
#endif

//
// MKS WIFI MODULE
//
/**
 * The module (MKS Robin WIFI V1.1) is an ESP-12S carrier that plugs in through
 * two 8-pin connectors, J1A and J1B. These simply break out the ESP's own pads
 * 1-8 and 15-22, so J1A pin N is ESP pad N and J1B pin N is ESP pad N+14.
 *
 * Note the module silkscreens the two strapping lines IO1 and IO2 while the
 * mainboard calls them WIFI_IO0 and WIFI_IO1, so they read one apart. The pad
 * numbers below are the way to tell them apart. Marlin uses the board names.
 *
 *   The two sit one behind the other, both running left to right with pin 1 at
 *   the left. J1A is the black connector; J1B is the green one, nearer the
 *   front edge of the board:
 *
 *                 ------------------------
 *        J1A     | 1  2  3  4  5  6  7  8 |   black
 *                 ------------------------
 *         pad      1  2  3  4  5  6  7  8
 *
 *                 ------------------------
 *        J1B     | 1  2  3  4  5  6  7  8 |   green
 *                 ------------------------
 *         pad     15 16 17 18 19 20 21 22
 *
 *                        front of board
 *
 *   J1A  pad  ESP function      Board        J1B  pad  ESP function      Board
 *   ---  ---  ----------------  ----------   ---  ---  ----------------  ----------
 *    1    1   RST               -             1   15   GND               GND
 *    2    2   ADC / TOUT        -             2   16   GPIO15            WIFI_CS
 *    3    3   EN (CH_PD)        WIFI_RST      3   17   GPIO2             -
 *    4    4   GPIO16            -             4   18   GPIO0             WIFI_IO0
 *    5    5   GPIO14  HSPI CLK  -             5   19   GPIO4             WIFI_IO1
 *    6    6   GPIO12  HSPI MISO -             6   20   GPIO5             -
 *    7    7   GPIO13  HSPI MOSI -             7   21   GPIO3   RXD       USART1_TX
 *    8    8   VCC               +3V3          8   22   GPIO1   TXD       USART1_RX
 *
 *   Board nets, in MCU pins:  WIFI_RST PD14   WIFI_IO0 PB14   WIFI_IO1 PB15
 *                             USART1_TX PA9   USART1_RX PA10  WIFI_CS via PD11
 *
 * WIFI_RST reaches the ESP's EN pad, not its RST pad - pad 1 is brought out to
 * J1A but the board never drives it. The HSPI pins are wired on the module but
 * never driven, since the module firmware is UART-only.
 *
 * PB14/PB15 are the MCU's own OTG_HS_DM/OTG_HS_DP, so they are shared with the
 * U-Disk port through two jumper blocks to its left, silkscreened 'WiFi (L)'
 * and 'OTA (R)'. Each row of the 2x3 is a separate selector, so the two shunts
 * are placed independently - one can be right while the other is wrong:
 *
 *              WiFi (L)              OTA (R)
 *               -----------------------
 *   WIFI_IO1 | 5         3         1 | OTG_DP  (U-Disk D+)
 *   WIFI_IO0 | 6         4         2 | OTG_DM  (U-Disk D-)
 *               -----------------------
 *                     PB15/PB14
 *
 *               -----------------------
 *    WIFI_CS | 1         2         3 | E4_UART_CS
 *               -----------------------
 *                       PD11
 *
 * With the 2x3 set to the right the module still powers up and talks on its own
 * UART, because USART1 and WIFI_RST are not jumpered, but WIFI_IO0 never
 * reaches it. It cannot be put into its bootloader, so 'MksWifi.bin' updates
 * fail to connect while everything else appears to work.
 */
#if ENABLED(MKS_WIFI_MODULE)
  #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
    #error "MKS_WIFI_MODULE and USB_FLASH_DRIVE_SUPPORT both need PB14/PB15 on MKS Monster8 V2. Enable only one."
  #endif
  #define WIFI_SERIAL_PORT                     1  // USART1
  #define WIFI_IO0_PIN                      PB14  // MKS ESP WIFI IO0 PIN
  #define WIFI_IO1_PIN                      PB15  // MKS ESP WIFI IO1 PIN
  #define WIFI_RESET_PIN                    PD14  // MKS ESP WIFI RESET PIN
#endif

// The FYSETC_MINI_12864_2_1 uses one of the EXP pins
#define BOARD_NEOPIXEL_PIN                  PC5

#include "pins_MKS_MONSTER8_common.h"
