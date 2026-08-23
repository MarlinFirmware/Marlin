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
 * The 4pi routes only the chip's dedicated UART (PA11/PA12) to its expansion
 * header; the four USARTs have no accessible pins.
 */
#if defined(SERIAL_PORT) && SERIAL_PORT != 0 && SERIAL_PORT != -1
  #error "SERIAL_PORT must be 0 (the UART on PA11/PA12) for SAM3U boards."
#endif
#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 >= 0
  #error "SAM3U boards have only one usable hardware serial port. Disable SERIAL_PORT_2."
#endif
#if defined(SERIAL_PORT_3) && SERIAL_PORT_3 >= 0
  #error "SAM3U boards have only one usable hardware serial port. Disable SERIAL_PORT_3."
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
