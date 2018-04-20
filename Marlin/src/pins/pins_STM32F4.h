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

#if !defined(STM32F4)
  #error "Oops!  Make sure you have an STM32F4 board selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME "STM32F407VET6"
#define BOARD_NAME "Marlin for STM32"

// #define I2C_EEPROM

// #define LARGE_FLASH true

#define E2END 0xFFF // EEPROM end address (4kB)

// Ignore temp readings during develpment.
// #define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

#if E_STEPPERS > 2 || HOTENDS > 2
  #error "STM32F4 supports up to 2 hotends / E-steppers."
#endif

#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4

#define _STM32_PIN(_PORT,_PIN) ((_PORT * 16) + _PIN)

//
// Limit Switches
//
#define X_MIN_PIN          _STM32_PIN(PORTE, 0)
#define X_MAX_PIN          -1
#define Y_MIN_PIN          _STM32_PIN(PORTE, 1)
#define Y_MAX_PIN          -1
#define Z_MIN_PIN          _STM32_PIN(PORTE, 14)
#define Z_MAX_PIN          -1

//
// Z Probe (when not Z_MIN_PIN)
//

// #ifndef Z_MIN_PROBE_PIN
//   #define Z_MIN_PROBE_PIN  _STM32_PIN(PORTA, 4)
// #endif

//
// Steppers
//

#define X_STEP_PIN         _STM32_PIN(PORTD, 3)
#define X_DIR_PIN          _STM32_PIN(PORTD, 2)
#define X_ENABLE_PIN       _STM32_PIN(PORTD, 0)
// #ifndef X_CS_PIN
//   #define X_CS_PIN         _STM32_PIN(PORTD, 1)
// #endif

#define Y_STEP_PIN         _STM32_PIN(PORTE, 11)
#define Y_DIR_PIN          _STM32_PIN(PORTE, 10)
#define Y_ENABLE_PIN       _STM32_PIN(PORTE, 13)
// #ifndef Y_CS_PIN
//   #define Y_CS_PIN         _STM32_PIN(PORTE, 12)
// #endif

#define Z_STEP_PIN         _STM32_PIN(PORTD, 6)
#define Z_DIR_PIN          _STM32_PIN(PORTD, 7)
#define Z_ENABLE_PIN       _STM32_PIN(PORTD, 4)
// #ifndef Z_CS_PIN
//   #define Z_CS_PIN         _STM32_PIN(PORTD, 5)
// #endif

#define E0_STEP_PIN        _STM32_PIN(PORTB, 5)
#define E0_DIR_PIN         _STM32_PIN(PORTB, 6)
#define E0_ENABLE_PIN      _STM32_PIN(PORTB, 3)
// #ifndef E0_CS_PIN
//   #define E0_CS_PIN         _STM32_PIN(PORTB, 4)
// #endif

#define E1_STEP_PIN        _STM32_PIN(PORTE, 4)
#define E1_DIR_PIN         _STM32_PIN(PORTE, 2)
#define E1_ENABLE_PIN      _STM32_PIN(PORTE, 3)
// #ifndef E1_CS_PIN
//   #define E1_CS_PIN         _STM32_PIN(PORTE, 5)
// #endif

#define SCK_PIN            _STM32_PIN(PORTA, 5)
#define MISO_PIN           _STM32_PIN(PORTA, 6)
#define MOSI_PIN           _STM32_PIN(PORTA, 7)

//
// Temperature Sensors
//

#define TEMP_0_PIN         _STM32_PIN(PORTC, 0)   // Analog Input
#define TEMP_1_PIN         _STM32_PIN(PORTC, 1)   // Analog Input
#define TEMP_BED_PIN       _STM32_PIN(PORTC, 2)   // Analog Input

//
// Heaters / Fans
//

#define HEATER_0_PIN       _STM32_PIN(PORTA, 1)
#define HEATER_1_PIN       _STM32_PIN(PORTA, 2)
#define HEATER_BED_PIN     _STM32_PIN(PORTA, 0)

#define FAN_PIN            _STM32_PIN(PORTC, 6)
#define FAN1_PIN           _STM32_PIN(PORTC, 7)
#define FAN2_PIN           _STM32_PIN(PORTC, 8)

#define ORIG_E0_AUTO_FAN_PIN  FAN1_PIN // Use this by NOT overriding E0_AUTO_FAN_PIN

//
// Misc. Functions
//

//#define CASE_LIGHT_PIN_CI _STM32_PIN(PORTF, 13)  
//#define CASE_LIGHT_PIN_DO _STM32_PIN(PORTF, 14)  
//#define NEOPIXEL_PIN      _STM32_PIN(PORTF, 13)

//
// Prusa i3 MK2 Multi Material Multiplexer Support
//

// #define E_MUX0_PIN         _STM32_PIN(PORTG, 3)
// #define E_MUX1_PIN         _STM32_PIN(PORTG, 4)

//
// Servos
//

// #define SERVO0_PIN         _STM32_PIN(PORTE, 13)
// #define SERVO1_PIN         _STM32_PIN(PORTE, 14)


#define SDSS               _STM32_PIN(PORTE, 7)
#define SS_PIN             _STM32_PIN(PORTE, 7)
#define LED_PIN            _STM32_PIN(PORTB, 7)         //Alive
#define PS_ON_PIN          _STM32_PIN(PORTA, 10)
#define KILL_PIN           _STM32_PIN(PORTA, 8)     
#define PWR_LOSS           _STM32_PIN(PORTA, 4)         //Power loss / nAC_FAULT

//
// LCD / Controller
//

#define SD_DETECT_PIN      _STM32_PIN(PORTA, 15)     
#define BEEPER_PIN         _STM32_PIN(PORTC, 9)      
#define LCD_PINS_RS        _STM32_PIN(PORTE, 9)      
#define LCD_PINS_ENABLE    _STM32_PIN(PORTE, 8)      
#define LCD_PINS_D4        _STM32_PIN(PORTB, 12)     
#define LCD_PINS_D5        _STM32_PIN(PORTB, 13)     
#define LCD_PINS_D6        _STM32_PIN(PORTB, 14)     
#define LCD_PINS_D7        _STM32_PIN(PORTB, 15)     
#define BTN_EN1            _STM32_PIN(PORTC, 4)      
#define BTN_EN2            _STM32_PIN(PORTC, 5)      
#define BTN_ENC            _STM32_PIN(PORTC, 3)      

//
// Filament runout
//

#define FIL_RUNOUT_PIN     _STM32_PIN(PORTA, 3)

//
// ST7920 Delays
//

#define STM_NOP __asm__("nop\n\t")
#define STM_DELAY_SHORT { STM_NOP; STM_NOP; STM_NOP; STM_NOP; }
#define STM_DELAY_LONG  { STM_DELAY_SHORT; STM_DELAY_SHORT; STM_NOP; STM_NOP; }

#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 { STM_DELAY_SHORT; STM_DELAY_SHORT; }
#endif

#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 { STM_DELAY_SHORT; }
#endif

#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 { STM_DELAY_LONG; STM_DELAY_LONG; STM_DELAY_LONG; STM_DELAY_LONG; STM_DELAY_LONG; STM_DELAY_LONG; }
#endif
