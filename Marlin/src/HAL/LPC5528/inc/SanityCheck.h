/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Test LPC5528-specific configuration values for errors at compile-time.
 */

//
// Serial ports
//
// Flexcomm 8 is high-speed SPI only and has no USART function, and Flexcomm 7
// is claimed by the Arduino core's SPI driver, which defines
// FLEXCOMM7_IRQHandler unconditionally. Flexcomm 0 has no MarlinSerial
// instance. That leaves 1..6, or -1 for USB CDC.
//
#if SERIAL_PORT == 7 || SERIAL_PORT_2 == 7 || SERIAL_PORT_3 == 7 || LCD_SERIAL_PORT == 7
  #error "SERIAL_PORT 7 is not available on LPC5528. Flexcomm 7 is used by the Arduino core's SPI driver."
#endif

//
// EEPROM
//
#if ENABLED(FLASH_EEPROM_EMULATION)
  #error "FLASH_EEPROM_EMULATION is not implemented for LPC5528. Use I2C_EEPROM or SDCARD_EEPROM_EMULATION."
#elif ENABLED(SRAM_EEPROM_EMULATION)
  #error "SRAM_EEPROM_EMULATION is not implemented for LPC5528. Use I2C_EEPROM or SDCARD_EEPROM_EMULATION."
#endif

//
// Features that need HAL support this port does not have yet
//
#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  #error "ENDSTOP_INTERRUPTS_FEATURE is not implemented for LPC5528. (Needs a PINT-based attachInterrupt.)"
#endif

#if ENABLED(PINS_DEBUGGING)
  #error "PINS_DEBUGGING is not supported on LPC5528. (No pinsDebug.h for this HAL.)"
#endif

//
// The tft/ sources are excluded from the build in ini/lpc5528.ini
//
#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI)
  #error "TFT_COLOR_UI, TFT_LVGL_UI and TFT_CLASSIC_UI are not supported on LPC5528 yet."
#endif

//
// Libraries that need the Arduino core to define ARDUINO, which it does not.
// Defining it makes U8glib compile its AVR-oriented COM drivers, so these
// remain unavailable for now.
//
#if HAS_MARLINUI_HD44780
  #error "Character LCDs (HD44780) are not supported on LPC5528. The LiquidCrystal library needs the pre-1.0 Arduino API."
#endif

#if ENABLED(NEOPIXEL_LED)
  #error "NEOPIXEL_LED is not supported on LPC5528. The Adafruit NeoPixel library needs ARDUINO to be defined."
#endif
