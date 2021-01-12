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
//#if ENABLED(SPINDLE_LASER_PWM) && !(SPINDLE_LASER_PWM_PIN == 4 || SPINDLE_LASER_PWM_PIN == 6 || SPINDLE_LASER_PWM_PIN == 11)
//  #error "SPINDLE_LASER_PWM_PIN must use SERVO0, SERVO1 or SERVO3 connector"
//#endif


#if ENABLED(SDCARD_EEPROM_EMULATION) && DISABLED(SDSUPPORT)
  #undef SDCARD_EEPROM_EMULATION // Avoid additional error noise
  #if USE_FALLBACK_EEPROM
    #warning "EEPROM type not specified. Fallback is SDCARD_EEPROM_EMULATION."
  #endif
  #error "SDCARD_EEPROM_EMULATION requires SDSUPPORT. Enable SDSUPPORT or choose another EEPROM emulation."
#endif

#if defined(STM32F4xx) && BOTH(PRINTCOUNTER, FLASH_EEPROM_EMULATION)
  #warning "FLASH_EEPROM_EMULATION may cause long delays when writing and should not be used while printing."
  #error "Disable PRINTCOUNTER or choose another EEPROM emulation."
#endif

#if !defined(STM32F4xx) && ENABLED(FLASH_EEPROM_LEVELING)
  #error "FLASH_EEPROM_LEVELING is currently only supported on STM32F4 hardware."
#endif

#if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
  #error "SERIAL_STATS_MAX_RX_QUEUED is not supported on this platform."
#elif ENABLED(SERIAL_STATS_DROPPED_RX)
  #error "SERIAL_STATS_DROPPED_RX is not supported on this platform."
#endif

#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI) && NOT_TARGET(STM32F4xx, STM32F1xx)
  #error "TFT_COLOR_UI, TFT_LVGL_UI and TFT_CLASSIC_UI are currently only supported on STM32F4 and STM32F1 hardware."
#endif
