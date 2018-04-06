/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if !defined(STM32F7)
  #error "Oops!  Make sure you have an STM32F7 board selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME "The-Borge"
#define BOARD_NAME "The-Borge"

#define LARGE_FLASH true

#define E2END 0xFFF   // EEPROM end address

// Ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

#if E_STEPPERS > 3 || HOTENDS > 3
  #error "The-Borg supports up to 3 hotends / E-steppers."
#endif

#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4
#define PORTF 5
#define PORTG 6

#define _STM32_PIN(_PORT,_PIN) ((_PORT * 16) + _PIN)

//
// Limit Switches
//
#define X_MIN_PIN          _STM32_PIN(PORTE,  9)
#define X_MAX_PIN          _STM32_PIN(PORTE, 10)
#define Y_MIN_PIN          _STM32_PIN(PORTE,  7)
#define Y_MAX_PIN          _STM32_PIN(PORTE,  8)
#define Z_MIN_PIN          _STM32_PIN(PORTF, 15)
#define Z_MAX_PIN          _STM32_PIN(PORTG,  0)
#define E_MIN_PIN          _STM32_PIN(PORTE,  2)
#define E_MAX_PIN          _STM32_PIN(PORTE,  3)

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  _STM32_PIN(PORTA,  4)
#endif

//
// Steppers
//
#define STEPPER_ENABLE_PIN _STM32_PIN(PORTE,  0)

#define X_STEP_PIN         _STM32_PIN(PORTC,  6)   // 96, 39 in Arduino
#define X_DIR_PIN          _STM32_PIN(PORTC,  7)
#define X_ENABLE_PIN       _STM32_PIN(PORTC,  8)


#define Y_STEP_PIN         _STM32_PIN(PORTD,  9)
#define Y_DIR_PIN          _STM32_PIN(PORTD, 10)
#define Y_ENABLE_PIN       _STM32_PIN(PORTD, 11)

#define Z_STEP_PIN         _STM32_PIN(PORTE, 15)
#define Z_DIR_PIN          _STM32_PIN(PORTG,  1)
#define Z_ENABLE_PIN       _STM32_PIN(PORTD,  8)


#define E0_STEP_PIN        _STM32_PIN(PORTB,  1)
#define E0_DIR_PIN         _STM32_PIN(PORTB,  2)
#define E0_ENABLE_PIN      _STM32_PIN(PORTE, 11)


#define E1_STEP_PIN        _STM32_PIN(PORTC,  4)
#define E1_DIR_PIN         _STM32_PIN(PORTC,  5)
#define E1_ENABLE_PIN      _STM32_PIN(PORTB,  0)


#define E2_STEP_PIN        _STM32_PIN(PORTC, 13)
#define E2_DIR_PIN         _STM32_PIN(PORTC, 14)
#define E2_ENABLE_PIN      _STM32_PIN(PORTC, 15)

#define Z2_STEP_PIN        _STM32_PIN(PORTC, 13)
#define Z2_DIR_PIN         _STM32_PIN(PORTC, 14)
#define Z2_ENABLE_PIN      _STM32_PIN(PORTC, 15)


#define SCK_PIN            _STM32_PIN(PORTA,  5)
#define MISO_PIN           _STM32_PIN(PORTA,  6)
#define MOSI_PIN           _STM32_PIN(PORTA,  7)

#define SPI1_SCK_PIN       _STM32_PIN(PORTA,  5)
#define SPI1_MISO_PIN      _STM32_PIN(PORTA,  6)
#define SPI1_MOSI_PIN      _STM32_PIN(PORTA,  7)

#define SPI6_SCK_PIN       _STM32_PIN(PORTG, 13)
#define SPI6_MISO_PIN      _STM32_PIN(PORTG, 12)
#define SPI6_MOSI_PIN      _STM32_PIN(PORTG, 14)

//
// Temperature Sensors
//

#define TEMP_0_PIN         _STM32_PIN(PORTC,  3)   // Analog Input
#define TEMP_1_PIN         _STM32_PIN(PORTC,  2)   // Analog Input
#define TEMP_2_PIN         _STM32_PIN(PORTC,  1)   // Analog Input
#define TEMP_3_PIN         _STM32_PIN(PORTC,  0)   // Analog Input

#define TEMP_BED_PIN       _STM32_PIN(PORTF, 10)   // Analog Input

#define TEMP_5_PIN         _STM32_PIN(PORTE, 12)   // Analog Input, Probe temp

//
// Heaters / Fans
//
#define HEATER_0_PIN       _STM32_PIN(PORTD, 15)
#define HEATER_1_PIN       _STM32_PIN(PORTD, 14)
#define HEATER_BED_PIN     _STM32_PIN(PORTF,  6)

#define FAN_PIN            _STM32_PIN(PORTD, 13)
#define FAN1_PIN           _STM32_PIN(PORTA,  0)
#define FAN2_PIN           _STM32_PIN(PORTA,  1)

#define ORIG_E0_AUTO_FAN_PIN  _STM32_PIN(PORTA, 1)   // Use this by NOT overriding E0_AUTO_FAN_PIN

//
// Misc. Functions
//

//#define CASE_LIGHT_PIN_CI _STM32_PIN(PORTF, 13)
//#define CASE_LIGHT_PIN_DO _STM32_PIN(PORTF, 14)
//#define NEOPIXEL_PIN     _STM32_PIN(PORTF, 13)

//
// Prusa i3 MK2 Multi Material Multiplexer Support
//

#define E_MUX0_PIN         _STM32_PIN(PORTG,  3)
#define E_MUX1_PIN         _STM32_PIN(PORTG,  4)

//
// Servos
//

#define SERVO0_PIN         _STM32_PIN(PORTE, 13)
#define SERVO1_PIN         _STM32_PIN(PORTE, 14)


#define SDSS               _STM32_PIN(PORTA,  8)
#define SS_PIN             _STM32_PIN(PORTA,  8)
#define LED_PIN            _STM32_PIN(PORTA,  2)   // Alive
#define PS_ON_PIN          _STM32_PIN(PORTA,  3)
#define KILL_PIN           -1//_STM32_PIN(PORTD,  5)   // EXP2-10
#define PWR_LOSS           _STM32_PIN(PORTG,  5)   // Power loss / nAC_FAULT

//
// MAX7219_DEBUG
//
#define MAX7219_CLK_PIN    _STM32_PIN(PORTG, 10)   // EXP1-1
#define MAX7219_DIN_PIN    _STM32_PIN(PORTD,  7)   // EXP1-3
#define MAX7219_LOAD_PIN   _STM32_PIN(PORTD,  1)   // EXP1-5

//
// LCD / Controller
//
//#define SD_DETECT_PIN      -1 //_STM32_PIN(PORTB, 6))     // EXP2-4
#define BEEPER_PIN         _STM32_PIN(PORTG, 10)   // EXP1-1
#define LCD_PINS_RS        _STM32_PIN(PORTG,  9)   // EXP1-4
#define LCD_PINS_ENABLE    _STM32_PIN(PORTD,  7)   // EXP1-3
#define LCD_PINS_D4        _STM32_PIN(PORTD,  1)   // EXP1-5
#define LCD_PINS_D5        _STM32_PIN(PORTF,  0)   // EXP1-6
#define LCD_PINS_D6        _STM32_PIN(PORTD,  3)   // EXP1-7
#define LCD_PINS_D7        _STM32_PIN(PORTD,  4)   // EXP1-8
#define BTN_EN1            _STM32_PIN(PORTD,  6)   // EXP2-5
#define BTN_EN2            _STM32_PIN(PORTD,  0)   // EXP2-3
#define BTN_ENC            _STM32_PIN(PORTG, 11)   // EXP1-2
