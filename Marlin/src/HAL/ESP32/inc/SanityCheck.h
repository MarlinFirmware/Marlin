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

#if HAS_FSMC_TFT
  #error "Sorry! FSMC TFT displays are not available for HAL/ESP32."
#endif

#if ENABLED(EMERGENCY_PARSER) && RX_BUFFER_SIZE < 2
  #error "EMERGENCY_PARSER on ESP32 requires an RX_BUFFER_SIZE of 2 or more."
#endif

// Software SPI can't drive pins that the SD card's hardware SPI has taken over
#if HAS_MEDIA && NONE(SOFTWARE_SPI, TEMP_SENSOR_FORCE_HW_SPI)
  #define _MAX_TC_ON_SD_PINS(N) (TEMP_SENSOR_##N##_HAS_SPI_PINS && (TEMP_##N##_SCK_PIN == SD_SCK_PIN || TEMP_##N##_MISO_PIN == SD_MISO_PIN || TEMP_##N##_MOSI_PIN == SD_MOSI_PIN))
  #if _MAX_TC_ON_SD_PINS(0) || _MAX_TC_ON_SD_PINS(1) || _MAX_TC_ON_SD_PINS(2) || _MAX_TC_ON_SD_PINS(BED)
    #error "A MAX thermocouple using software SPI can't share pins with the SD card. Define only its CS pin, or enable TEMP_SENSOR_FORCE_HW_SPI."
  #endif
  #undef _MAX_TC_ON_SD_PINS
#endif

#if ENABLED(SPINDLE_LASER_USE_PWM) && SPINDLE_LASER_FREQUENCY > 78125
  #error "SPINDLE_LASER_FREQUENCY maximum value is 78125Hz for ESP32."
#endif
#if ENABLED(FAST_PWM_FAN) && FAST_PWM_FAN_FREQUENCY > 78125
  #error "FAST_PWM_FREQUENCY maximum value is 78125Hz for ESP32."
#endif

#if HAS_TMC_SW_SERIAL
  #error "TMC220x Software Serial is not supported on ESP32."
#endif

#if ENABLED(POSTMORTEM_DEBUGGING)
  #error "POSTMORTEM_DEBUGGING is not yet supported on ESP32."
#endif

#if MB(MKS_TINYBEE) && ENABLED(FAST_PWM_FAN)
  #error "FAST_PWM_FAN is not available on TinyBee."
#endif

#if USING_PULLDOWNS
  #error "PULLDOWN pin mode is not available on ESP32 boards."
#endif

#if ALL(I2S_STEPPER_STREAM, LIN_ADVANCE) && DISABLED(EXPERIMENTAL_I2S_LA)
  #error "I2S stream is currently incompatible with LIN_ADVANCE. Enable EXPERIMENTAL_I2S_LA to proceed anyway."
#endif

#if ALL(I2S_STEPPER_STREAM, PRINTCOUNTER) && PRINTCOUNTER_SAVE_INTERVAL > 0 && DISABLED(PRINTCOUNTER_SYNC)
  #error "PRINTCOUNTER_SAVE_INTERVAL may cause issues on ESP32 with an I2S expander. Define PRINTCOUNTER_SYNC in Configuration.h for an imperfect solution."
#endif
