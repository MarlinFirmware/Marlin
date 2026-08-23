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
 * Test SAM3U (4pi) specific configuration values for errors at compile-time.
 */

// Pins are plain numeric macros here (see HAL/SAM3U/include/pinmapping.h), so
// a missing PA0 means the board pin file was not picked up at all.
#ifndef PA0
  #error "SAM3U pin names are missing. Check that the board's pins file includes the HAL pin map."
#endif

/**
 * Serial ports.
 *
 * The SAM3U has five asynchronous ports plus native USB, and this HAL supports
 * any of them in any of Marlin's three slots:
 *
 *   -1  Native USB CDC (UDPHS)
 *    0  UART    PA11/PA12
 *    1  USART0  PA19/PA18
 *    2  USART1  PA21/PA20
 *    3  USART2  PA23/PA22
 *    4  USART3  PC13/PC12
 *
 * On the 4pi, however, only the UART and USB are actually wired to anything
 * usable - every USART shares pins with a board function, and two of them sit
 * on heater outputs. Those are rejected below rather than left to fail as a
 * runaway heater or a dead axis.
 */
#define _SAM3U_BAD_PORT(N) (N > 4)

#if defined(SERIAL_PORT) && (SERIAL_PORT < -1 || SERIAL_PORT > 4)
  #error "SERIAL_PORT must be -1 (native USB) or 0-4 (UART, USART0-3) on SAM3U."
#endif
#if defined(SERIAL_PORT_2) && (SERIAL_PORT_2 < -1 || SERIAL_PORT_2 > 4)
  #error "SERIAL_PORT_2 must be -1 (native USB) or 0-4 (UART, USART0-3) on SAM3U."
#endif
#if defined(SERIAL_PORT_3) && (SERIAL_PORT_3 < -1 || SERIAL_PORT_3 > 4)
  #error "SERIAL_PORT_3 must be -1 (native USB) or 0-4 (UART, USART0-3) on SAM3U."
#endif

// No port may be claimed twice
#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 != -1 && SERIAL_PORT_2 == SERIAL_PORT
  #error "SERIAL_PORT_2 must not be the same port as SERIAL_PORT."
#endif
#if defined(SERIAL_PORT_3) && SERIAL_PORT_3 != -1
  #if SERIAL_PORT_3 == SERIAL_PORT
    #error "SERIAL_PORT_3 must not be the same port as SERIAL_PORT."
  #elif defined(SERIAL_PORT_2) && SERIAL_PORT_3 == SERIAL_PORT_2
    #error "SERIAL_PORT_3 must not be the same port as SERIAL_PORT_2."
  #endif
#endif

/**
 * Board-level restriction: on the 4pi the USART pins are all spoken for.
 *
 *   USART0  PA19 = expansion CS3, PA18 = motor supply detect  (least harmful)
 *   USART1  PA21 = HOTEND1 heater, PA20 = BED heater          (dangerous)
 *   USART2  PA23 = HOTEND2 heater, PA22 = Y enable            (dangerous)
 *   USART3  PC13 = E1 enable,      PC12 = Z_MIN endstop       (breaks homing)
 */
#if MB(4PI)

  #if SAM3U_SERIAL_IN_USE(2) || SAM3U_SERIAL_IN_USE(3)
    #error "On the 4pi, USART1/USART2 (SERIAL_PORT 2 or 3) are wired to heater outputs. Use -1 (USB) or 0 (UART)."
  #endif
  #if SAM3U_SERIAL_IN_USE(4)
    #error "On the 4pi, USART3 (SERIAL_PORT 4) is wired to the E1 enable and Z_MIN endstop. Use -1 (USB) or 0 (UART)."
  #endif
  #if SAM3U_SERIAL_IN_USE(1)
    #warning "On the 4pi, USART0 (SERIAL_PORT 1) takes over the expansion header's CS3 and the motor supply detect."
  #endif

  /**
   * Keep the UART pin documentation in pins_4PI.h honest.
   *
   * Those UARTn_*_PIN defines exist to record which board function each USART
   * would steal. If someone moves one of those functions to a different pin
   * without updating the table, the rejections above would start describing a
   * conflict that no longer exists - or worse, stop describing one that does.
   */
  static_assert(UART2_RX_PIN == HEATER_0_PIN,   "pins_4PI.h: UART2_RX_PIN no longer matches HEATER_0_PIN.");
  static_assert(UART2_TX_PIN == HEATER_BED_PIN, "pins_4PI.h: UART2_TX_PIN no longer matches HEATER_BED_PIN.");
  static_assert(UART3_RX_PIN == HEATER_1_PIN,   "pins_4PI.h: UART3_RX_PIN no longer matches HEATER_1_PIN.");
  static_assert(UART3_TX_PIN == Y_ENABLE_PIN,   "pins_4PI.h: UART3_TX_PIN no longer matches Y_ENABLE_PIN.");
  static_assert(UART4_RX_PIN == E1_ENABLE_PIN,  "pins_4PI.h: UART4_RX_PIN no longer matches E1_ENABLE_PIN.");
  static_assert(UART4_TX_PIN == Z_MIN_PIN,      "pins_4PI.h: UART4_TX_PIN no longer matches Z_MIN_PIN.");
  static_assert(UART1_TX_PIN == POWER_MONITOR_VOLTAGE_PIN, "pins_4PI.h: UART1_TX_PIN no longer matches POWER_MONITOR_VOLTAGE_PIN.");

#endif

/**
 * All three TC0 channels are taken: stepper, temperature and tone. There is no
 * fourth channel for the timer-based servo driver.
 */
#if HAS_SERVOS && HAS_SOUND
  #error "SAM3U has only three timer channels. Enabling both SERVO and SPEAKER/BEEPER leaves none free. Disable one."
#endif

/**
 * The SAM3U PIO has pull-ups but no pull-downs, so any option that relies on
 * an internal pull-down cannot work.
 */
#if ENABLED(ENDSTOPPULLDOWNS)
  #error "SAM3U has no internal pull-downs. Use ENDSTOPPULLUPS or external resistors."
#endif
#if ENABLED(Z_MIN_PROBE_PULLDOWN)
  #error "SAM3U has no internal pull-downs. Use Z_MIN_PROBE_PULLUP or an external resistor."
#endif

/**
 * The SAM3U4E has 256KB of flash and 48KB of usable SRAM - half the Due's on
 * both counts. Features that are merely large elsewhere can fail to fit here.
 */
#if ENABLED(SDSUPPORT) && ENABLED(LONG_FILENAME_HOST_SUPPORT) && ENABLED(SDCARD_SORT_ALPHA) && !defined(SDSORT_LIMIT)
  #warning "SDCARD_SORT_ALPHA uses a lot of the SAM3U's limited SRAM. Consider setting SDSORT_LIMIT low."
#endif

/**
 * On the 4pi the SPI bus is shared with the AD5206 motor-current digipot, whose
 * chip select is the same pin spi_pins.h defaults SD_SS_PIN to. Driving it as
 * an SD chip select would corrupt the stepper current settings on every card
 * access, so make the collision a build error rather than a mystery.
 *
 * The onboard socket is on HSMCI and cannot be reached by Marlin's SPI driver
 * at all - this is only about an external SPI card reader on the expansion
 * header, which should use one of the spare selects there.
 */
#if HAS_MEDIA && defined(DIGIPOTSS_PIN) && defined(SD_SS_PIN) && SD_SS_PIN == DIGIPOTSS_PIN
  #error "SD_SS_PIN collides with DIGIPOTSS_PIN (the AD5206 digipot). Set SD_SS_PIN to one of the expansion header selects: EXP_CS1_PIN, EXP_CS2_PIN or EXP_CS3_PIN."
#endif

// The ADC10 (10-bit) converter is not wired up by this HAL - only ADC12B.
#if HAL_ADC_RESOLUTION != 12
  #error "SAM3U HAL uses the 12-bit ADC12B. HAL_ADC_RESOLUTION must be 12."
#endif
