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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#if PIO_PLATFORM_VERSION < 1001
  #error "nxplpc-arduino-lpc176x package is out of date, Please update the PlatformIO platforms, frameworks and libraries. You may need to remove the platform and let it reinstall automatically."
#endif
#if PIO_FRAMEWORK_VERSION < 2002
  #error "framework-arduino-lpc176x package is out of date, Please update the PlatformIO platforms, frameworks and libraries."
#endif

/**
 * Detect an old pins file by checking for old ADC pins values.
 */
#define _OLD_TEMP_PIN(P) PIN_EXISTS(P) && _CAT(P,_PIN) <= 7 && _CAT(P,_PIN) != 2 && _CAT(P,_PIN) != 3
#if _OLD_TEMP_PIN(TEMP_BED)
  #error "TEMP_BED_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_0)
  #error "TEMP_0_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_1)
  #error "TEMP_1_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_2)
  #error "TEMP_2_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_3)
  #error "TEMP_3_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_4)
  #error "TEMP_4_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_5)
  #error "TEMP_5_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_6)
  #error "TEMP_6_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif _OLD_TEMP_PIN(TEMP_7)
  #error "TEMP_7_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#endif
#undef _OLD_TEMP_PIN

/**
 * Because PWM hardware channels all share the same frequency, along with the
 * fallback software channels, FAST_PWM_FAN is incompatible with Servos.
 */
#if NUM_SERVOS > 0 && ENABLED(FAST_PWM_FAN)
  #error "BLTOUCH and Servos are incompatible with FAST_PWM_FAN on LPC176x boards."
#endif

/**
 * Test LPC176x-specific configuration values for errors at compile-time.
 */

//#if ENABLED(SPINDLE_LASER_PWM) && !(SPINDLE_LASER_PWM_PIN == 4 || SPINDLE_LASER_PWM_PIN == 6 || SPINDLE_LASER_PWM_PIN == 11)
//  #error "SPINDLE_LASER_PWM_PIN must use SERVO0, SERVO1 or SERVO3 connector"
//#endif

#if IS_RE_ARM_BOARD && ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) && HAS_DRIVER(TMC2130) && DISABLED(TMC_USE_SW_SPI)
  #error "Re-ARM with REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER and TMC2130 require TMC_USE_SW_SPI"
#endif

#if ENABLED(BAUD_RATE_GCODE)
  #error "BAUD_RATE_GCODE is not yet supported on LPC176x."
#endif

/**
 * Flag any serial port conflicts
 *
 *   Port  |  TX   |  RX   |
 *    ---  |  ---  |  ---  |
 * Serial  | P0_02 | P0_03 |
 * Serial1 | P0_15 | P0_16 |
 * Serial2 | P0_10 | P0_11 |
 * Serial3 | P0_00 | P0_01 |
 */
#if (defined(SERIAL_PORT) && SERIAL_PORT == 0) || (defined(SERIAL_PORT_2) && SERIAL_PORT_2 == 0) || (defined(DGUS_SERIAL_PORT) && DGUS_SERIAL_PORT == 0)
  #if  X_CS_PIN == P0_02 || TMC_SW_MISO == P0_02 || (E_STEPPERS && E_MUX1_PIN == P0_02) \
    || Y_CS_PIN == P0_03 || TMC_SW_MOSI == P0_03 || (E_STEPPERS && E_MUX0_PIN == P0_03)
    #error "Serial port assignment (0) conflicts with other pins!"
  #endif
#endif

#if SERIAL_PORT == 1 || SERIAL_PORT_2 == 1 || DGUS_SERIAL_PORT == 1
  #if TMC_SW_SCK == P0_15
    #error "Serial port assignment (1) conflicts with other pins!"
  #elif HAS_SPI_LCD
    #if  BTN_EN2 == P0_15 || SCK_PIN == P0_15 || LCD_PINS_D4 == P0_15 || DOGLCD_SCK == P0_15 || LCD_RESET_PIN == P0_15 || LCD_PINS_RS == P0_15 || SHIFT_CLK == P0_15 \
      || BTN_EN1 == P0_16 || LCD_SDSS == P0_16 || LCD_PINS_RS == P0_16 || MISO_PIN == P0_16 || DOGLCD_A0 == P0_16 || SS_PIN == P0_16 || LCD_SDSS == P0_16 || DOGLCD_CS == P0_16 || LCD_RESET_PIN == P0_16 || LCD_BACKLIGHT_PIN == P0_16
      #error "Serial port assignment (1) conflicts with other pins!"
    #endif
  #endif
#endif

#if SERIAL_PORT == 2 || SERIAL_PORT_2 == 2 || DGUS_SERIAL_PORT == 2
  #if  Y_MIN_PIN == P0_10 || Z_MIN_PROBE_PIN == P0_10 \
    || X_ENABLE_PIN == P0_10 || Y_ENABLE_PIN == P0_10 || X2_ENABLE_PIN == P0_10 || Y2_ENABLE_PIN == P0_10 || Z2_ENABLE_PIN == P0_10 || Z3_ENABLE_PIN == P0_10 || Z4_ENABLE_PIN == P0_10 \
    || X2_CS_PIN == P0_10 || Y2_CS_PIN == P0_10 || Z2_CS_PIN == P0_10 || Z3_CS_PIN == P0_10 || Z4_CS_PIN == P0_10 \
    || X_DIR_PIN == P0_11 || Y_DIR_PIN == P0_11 || X2_DIR_PIN == P0_11 || Y2_DIR_PIN == P0_11 || Z2_DIR_PIN == P0_11 || Z3_DIR_PIN == P0_11 || Z4_DIR_PIN == P0_11 \
    || X2_STEP_PIN == P0_11 || Y2_STEP_PIN == P0_11 || Z2_STEP_PIN == P0_11 || Z3_STEP_PIN == P0_11 || Z4_STEP_PIN == P0_11
    #error "Serial port assignment (2) conflicts with other pins!"
  #elif  (E_STEPPERS > 1 && (E1_ENABLE_PIN == P0_10 || E1_CS_PIN == P0_10)) || (E_STEPPERS > 0 && (E0_DIR_PIN == P0_11 || E0_STEP_PIN == P0_11))
    #error "Serial port assignment (2) conflicts with other pins!"
  #endif
#endif

#if SERIAL_PORT == 3 || SERIAL_PORT_2 == 3 || DGUS_SERIAL_PORT == 3
  #if  X_MIN_PIN == P0_00 || Y_SERIAL_TX_PIN == P0_00 || Y_SERIAL_RX_PIN == P0_00 \
    || X_MAX_PIN == P0_01 || X_SERIAL_TX_PIN == P0_01 || X_SERIAL_RX_PIN == P0_01
    #error "Serial port assignment (2) conflicts with other pins!"
  #elif E_STEPPERS > 1 && (E1_DIR_PIN == P0_00 || E1_STEP_PIN == P0_01)
    #error "Serial port assignment (2) conflicts with other pins!"
  #endif
#endif
