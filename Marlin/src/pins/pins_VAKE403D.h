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

#if !defined(STM32F4) && !defined(STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#endif

#define DEFAULT_MACHINE_NAME "STM32F446VET6"
#define BOARD_NAME "STM32F4 VAkE Board"

//#define I2C_EEPROM

#define E2END 0xFFF // EEPROM end address (4kB)

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "STM32F4 supports up to 2 hotends / E-steppers."
#endif

#if 0
//
// Servos
//
//#define SERVO0_PIN         5

//
// Limit Switches
//
#define X_MIN_PIN          PE10 //_STM32_PIN(PORTE, 10)
#define X_MAX_PIN          -1
#define Y_MIN_PIN          PE9 // _STM32_PIN(PORTE, 9)
#define Y_MAX_PIN          -1
#define Z_MIN_PIN          PE8 //_STM32_PIN(PORTE, 8)
#define Z_MAX_PIN          -1

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  PA4 //_STM32_PIN(PORTA, 4)
#endif

//
// Steppers
//

#define STEPPER_ENABLE_PIN	PE0  // _STM32_PIN(PORTE, 0)

#define X_STEP_PIN         PC6 //_STM32_PIN(PORTC, 6)
#define X_DIR_PIN          PC7 //_STM32_PIN(PORTC, 7)
#define X_ENABLE_PIN       PC8 //_STM32_PIN(PORTC, 8)


#define Y_STEP_PIN         PD9 //_STM32_PIN(PORTD, 9)
#define Y_DIR_PIN          PD10 //_STM32_PIN(PORTD, 10)
#define Y_ENABLE_PIN       PD11 //_STM32_PIN(PORTD, 11)

#define Z_STEP_PIN         PE15 //_STM32_PIN(PORTE, 15)
#define Z_DIR_PIN          PB10 //_STM32_PIN(PORTB, 10)
#define Z_ENABLE_PIN       PD8 //_STM32_PIN(PORTD, 8)


#define E0_STEP_PIN        PB1 //_STM32_PIN(PORTB, 1)
#define E0_DIR_PIN         PB2 //_STM32_PIN(PORTB, 2)
#define E0_ENABLE_PIN      PE11 //_STM32_PIN(PORTE, 11)


#define E1_STEP_PIN        PC4 //_STM32_PIN(PORTC, 4)
#define E1_DIR_PIN         PC5 //_STM32_PIN(PORTC, 5)
#define E1_ENABLE_PIN      PB0 //_STM32_PIN(PORTB, 0)


#define E2_STEP_PIN        PC13 //_STM32_PIN(PORTC, 13)
#define E2_DIR_PIN         PC14 //_STM32_PIN(PORTC, 14)
#define E2_ENABLE_PIN      PC15 //_STM32_PIN(PORTC, 15)


#define E3_STEP_PIN        PE5 //_STM32_PIN(PORTE, 5)
#define E3_DIR_PIN         PE4 //_STM32_PIN(PORTE, 4)
#define E3_ENABLE_PIN      PE6 //_STM32_PIN(PORTE, 6)


#define E4_STEP_PIN        PE1 //_STM32_PIN(PORTE, 1)
#define E4_DIR_PIN         PE2 //_STM32_PIN(PORTE, 2)
#define E4_ENABLE_PIN      PE3 //_STM32_PIN(PORTE, 3)

#define SPI4_SCK_PIN       PE12 //_STM32_PIN(PORTE, 12)
#define SPI4_MISO_PIN      PE13 //_STM32_PIN(PORTE, 13)
#define SPI4_MOSI_PIN      PE14 //_STM32_PIN(PORTE, 14)

//VAkE with TMC2130 : TODO !!!
#define X_CS_PIN	   PC8 //_STM32_PIN(PORTC,8)			//OK
#define Y_CS_PIN	   PD11 //_STM32_PIN(PORTD,11)			//OK
#define Z_CS_PIN	   PD8 //_STM32_PIN(PORTD,8)			//OK
#define E0_CS_PIN	   PE11 //_STM32_PIN(PORTE,11)			//OK
#define E1_CS_PIN	   PB0 //_STM32_PIN(PORTB,0)			//OK

//
// Temperature Sensors (Analog inputs)
//

#define TEMP_0_PIN       PC0 //_STM32_PIN(PORTC, 0)		//OK
#define TEMP_1_PIN       PC1 //_STM32_PIN(PORTC, 1)		//OK
#define TEMP_2_PIN       PC2 //_STM32_PIN(PORTC, 2)		//OK
#define TEMP_3_PIN       PC3 //_STM32_PIN(PORTC, 3)		//OK

// DEFINE BED PIN
#define TEMP_BED_PIN     TEMP_3_PIN   			//OK


//
// Heaters / Fans
//
#define HEATER_0_PIN        PD15 //_STM32_PIN(PORTD, 15)	//OK
#define HEATER_1_PIN        PD14 //_STM32_PIN(PORTD, 14)	//OK
#define HEATER_BED_PIN      PD12 //_STM32_PIN(PORTD, 12)	//OK

#define FAN_PIN             PD13 //_STM32_PIN(PORTD, 13)	//OK
#define FAN1_PIN            PA0 //_STM32_PIN(PORTA, 0)	//OK
#define FAN2_PIN            PA1 //_STM32_PIN(PORTA, 1)	//OK
//#define E0_AUTO_FAN_PIN 	_STM32_PIN(PORTA, 1)

//
// Misc. Functions
//
#define LED_PIN            PB14 //_STM32_PIN(PORTB, 14)	//OK
#define PS_ON_PIN          PE0 //_STM32_PIN(PORTE, 0)		//OK
#define KILL_PIN           PD5 //_STM32_PIN(PORTD, 5)		//OK

//
// LCD / Controller
//
#define SDSS               PB6 //_STM32_PIN(PORTB, 6)		//OK
#define SD_DETECT_PIN      PB7 //_STM32_PIN(PORTB, 7)		//OK
#define BEEPER_PIN         PC9 //_STM32_PIN(PORTC, 9)		//OK
#define LCD_PINS_RS        PC12 //_STM32_PIN(PORTC, 12)	i	//OK
#define LCD_PINS_ENABLE    PD7 //_STM32_PIN(PORTD, 7)		//OK
#define LCD_PINS_D4        PD1 //_STM32_PIN(PORTD, 1)		//OK
#define LCD_PINS_D5        PD2 //_STM32_PIN(PORTD, 2)		//OK
#define LCD_PINS_D6        PD3 //_STM32_PIN(PORTD, 3)		//OK
#define LCD_PINS_D7        PD4 //_STM32_PIN(PORTD, 4)		//OK
#define BTN_EN1            PD6 //_STM32_PIN(PORTD, 6)		//OK
#define BTN_EN2            PD0 //_STM32_PIN(PORTD, 0)		//OK
#define BTN_ENC            PC11 //_STM32_PIN(PORTC, 11)		//OK

//
// On Board SD
//
//#define SDSS               _STM32_PIN(PORTB, 15)		//OK
//#define SD_DETECT_PIN      _STM32_PIN(PORTA, 8)		//OK
#endif



//#if 0 
//
// Limit Switches
//
#define X_MIN_PIN          PE10   // X_STOP
#define X_MAX_PIN          -1
#define Y_MIN_PIN          PE9    // Y_STOP
#define Y_MAX_PIN          -1
#define Z_MIN_PIN          PE8    // Z_STOP
#define Z_MAX_PIN          -1

//
// Z Probe (when not Z_MIN_PIN)
//

//#ifndef Z_MIN_PROBE_PIN
#define Z_MIN_PROBE_PIN  PA4
//#endif

//
// Steppers
//

#define STEPPER_ENABLE_PIN	PB2

#define X_STEP_PIN         PC6	// X_STEP
#define X_DIR_PIN          PC7  // X_DIR 
#define X_ENABLE_PIN       PB2  //
#ifndef X_CS_PIN
  #define X_CS_PIN         PC8	// X_CS
#endif

#define Y_STEP_PIN         PD9	// Y_STEP
#define Y_DIR_PIN          PD10  // Y_DIR
#define Y_ENABLE_PIN       PB2   //
#ifndef Y_CS_PIN
  #define Y_CS_PIN         PD11  // Y_CS
#endif

#define Z_STEP_PIN         PE15	 // Z_STEP
#define Z_DIR_PIN          PB10	 // Z_DIR
#define Z_ENABLE_PIN       PB2
#ifndef Z_CS_PIN
  #define Z_CS_PIN         PD8
#endif

#define E0_STEP_PIN        PB1
#define E0_DIR_PIN         PB13
#define E0_ENABLE_PIN      PB2 
#ifndef E0_CS_PIN
  #define E0_CS_PIN         PE11
#endif

#define E1_STEP_PIN        PC4
#define E1_DIR_PIN         PC5
#define E1_ENABLE_PIN      PB2
#ifndef E1_CS_PIN
  #define E1_CS_PIN         PB0
#endif

#define SCK_PIN            PE12 //PA5   // SPI1 for SD card
#define MISO_PIN           PE13 //PA6
#define MOSI_PIN           PE14 //PA7

// added for SD card : optional or not ???
//#define SD_CHIP_SELECT_PIN SDSS   // The default chip select pin for the SD card is SS.
// The following three pins must not be redefined for hardware SPI.
//#define SPI_MOSI_PIN MOSI_PIN       // SPI Master Out Slave In pin
//#define SPI_MISO_PIN MISO_PIN       // SPI Master In Slave Out pin
//#define SPI_SCK_PIN SCK_PIN         // SPI Clock pin


//
// Temperature Sensors (Analog inputs)
//

#define TEMP_0_PIN         PC0   // Analog Input
#define TEMP_1_PIN         PC1   // Analog Input
#define TEMP_2_PIN         PC2   // Analog Input
#define TEMP_3_PIN         PC3   // Analog Input

// DEFINE BED PIN
#define TEMP_BED_PIN       TEMP_3_PIN   // Analog Input

//
// Heaters / Fans
//

#define HEATER_0_PIN       PD15
#define HEATER_1_PIN       PD14
#define HEATER_BED_PIN     PD12

//#ifndef FAN_PIN
  #define FAN_PIN          PD13
//#endif
#define FAN1_PIN           PB5 //PA0
#define FAN2_PIN           PB4 //PA1

#define ORIG_E0_AUTO_FAN_PIN  FAN1_PIN // Use this by NOT overriding E0_AUTO_FAN_PIN

//
// Misc. Functions
//

//#define CASE_LIGHT_PIN_CI PF13
//#define CASE_LIGHT_PIN_DO PF14
//#define NEOPIXEL_PIN      PF13

//
// Prusa i3 MK2 Multi Material Multiplexer Support
//

//#define E_MUX0_PIN         PG3
//#define E_MUX1_PIN         PG4

//
// Servos
//

//#define SERVO0_PIN         PE13
//#define SERVO1_PIN         PE14


#define SDSS               PB6		// CS for SD card in LCD
#define SS_PIN             PB_15        //USD_CS -> CS for onboard SD
#define LED_PIN            PB14        //Alive
#define PS_ON_PIN          PE0
#define KILL_PIN           PD5
#define PWR_LOSS           PA4  //?       //Power loss / nAC_FAULT

//
// LCD / Controller
//

#define SD_DETECT_PIN      PB7
#define BEEPER_PIN         PC9
#define LCD_PINS_RS        PC12
#define LCD_PINS_ENABLE    PD7
#define LCD_PINS_D4        PD1
#define LCD_PINS_D5        PD2
#define LCD_PINS_D6        PD3
#define LCD_PINS_D7        PD4
#define BTN_EN1            PD6
#define BTN_EN2            PD0
#define BTN_ENC            PB12

//
// Filament runout
//

#define FIL_RUNOUT_PIN     PA3

//
// ST7920 Delays
//
#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(96)
#endif

#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(48)
#endif

#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(715)
#endif

//#endif
