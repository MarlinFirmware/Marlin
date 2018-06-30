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

#ifndef __STM32F1__
  #error "Oops!  Make sure you have an STM32F1 board selected from the 'Tools -> Boards' menu."
#endif

/**
 * 24.05.2018 Axel "chepo" Sepúlveda Marlin for (stm32f103vet6) sch of board https://github.com/chepo92/Smartto/blob/master/Rostock301/Hardware_GTM32_PRO_VB.pdf
 */

#define DEFAULT_MACHINE_NAME "STM32F103VET6"
#define BOARD_NAME "GTM32 Pro VB"

<<<<<<< HEAD
// Ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Steppers
//
#define X_STEP_PIN         PC6		
#define X_DIR_PIN          PD13		
#define X_ENABLE_PIN       PA8		

#define Y_STEP_PIN         PA12
#define Y_DIR_PIN          PA11
#define Y_ENABLE_PIN       PA15		// core conflict?

#define Z_STEP_PIN         PD6		
#define Z_DIR_PIN          PD3		
#define Z_ENABLE_PIN       PB3		// core conflict?
=======
// Ignore temp readings during development.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define X_MIN_PIN          PE5
#define X_MAX_PIN          PE4
#define Y_MIN_PIN          PE3
#define Y_MAX_PIN          PE2
#define Z_MIN_PIN          PE1
#define Z_MAX_PIN          PE0

//
// Steppers
//
#define X_STEP_PIN         PC6
#define X_DIR_PIN          PD13
#define X_ENABLE_PIN       PA8

#define Y_STEP_PIN         PA12
#define Y_DIR_PIN          PA11
#define Y_ENABLE_PIN       PA15   // core conflict?

#define Z_STEP_PIN         PD6
#define Z_DIR_PIN          PD3
#define Z_ENABLE_PIN       PB3    // core conflict?
>>>>>>> 15e4f82f8a24b779d0d323b3223be3469ac0993b

#define E0_STEP_PIN        PC14
#define E0_DIR_PIN         PC13
#define E0_ENABLE_PIN      PC15

#define E1_STEP_PIN        PA0
#define E1_DIR_PIN         PB6
#define E1_ENABLE_PIN      PA1

#define E2_STEP_PIN        PB2
#define E2_DIR_PIN         PB11
#define E2_ENABLE_PIN      PC4

//
<<<<<<< HEAD
// Endstops
//
#define X_MIN_PIN          PE5
#define X_MAX_PIN          PE4

#define Y_MIN_PIN          PE3
#define Y_MAX_PIN          PE2

#define Z_MIN_PIN          PE1
#define Z_MAX_PIN          PE0

//
// Misc. Functions
//
#define SDPOWER            -1
#define SDSS               -1
#define LED_PIN            PD12
#define PS_ON_PIN          -1
#define KILL_PIN           -1

//
// Heaters 
//
#define HEATER_0_PIN       PB4   // EXTRUDER 1
#define HEATER_1_PIN       PB5	 // EXTRUDER 2
#define HEATER_2_PIN       PB0	 // EXTRUDER 3

#define HEATER_BED_PIN     PB1		 // BED
#define HEATER_BED2_PIN    -1    // BED2
#define HEATER_BED3_PIN    -1    // BED3

//
// Fans
//
#define FAN_PIN            -1    // (Sprinter config)
#define FAN0_PIN           PB7  // ToDo: Add functionality
#define FAN1_PIN           PB8  // ToDo: Add functionality
#define FAN2_PIN           PB9  // ToDo: Add functionality

//
// Temperature Sensors
//
#define TEMP_BED_PIN       PC0   // PORT NUMBERING
#define TEMP_0_PIN         PC1   // PORT NUMBERING
#define TEMP_1_PIN         PC2   // PORT NUMBERING
#define TEMP_2_PIN         PC3   // PORT NUMBERING

//
// LCD Pins			
//
#define LCD_D15 			-1
#define LCD_D14 			-1
#define LCD_D13 			-1
#define LCD_D12 			-1
#define LCD_D11 			-1
#define LCD_D10 			-1
#define LCD_D9 				-1
#define LCD_D8 				-1
#define LCD_D7 				-1
#define LCD_D6 				-1
#define LCD_D5 				-1
#define LCD_WR 				-1
#define LCD_RD 				-1
#define LCD_RS 				-1
#define LCD_CS 				-1
#define RESET 				-1
#define LCD_D0 				-1
#define LCD_D1 				-1
#define LCD_D2 				-1
#define LCD_D3 				-1
#define LCD_D4 				-1
#define T_PEN 				-1
#define SPI2_MOSI 			-1
#define SPI2_MISO 			-1
#define SPI2_SCK 			-1
#define SPI2_NSS 			-1
#define SD_DATA2 			-1
#define SD_DATA3 			-1
#define SD_CMD 				-1
#define SD_CLK 				-1
#define SD_DATA0 			-1
#define SD_DATA1 			-1
#define SD_CD 				-1

//
// UART LCD Pins
//
#define LCD_D13 			-1  			//	TXD3
#define LCD_D14 			-1				//	RXD3

//
// SD Card
//
#define SD_DATA2 			-1
#define SD_DATA3 			-1
#define SD_CMD 				-1
#define SD_CLK 				-1
#define SD_DATA0 			-1 
#define SD_DATA1 			-1
#define SD_CD 				-1

//
// Debug
//
#define SDA 				-1
#define SCL 				-1

//
// Wifi
//
#define ES 					-1
#define WIFI_LED 			-1
#define USART2_RX 			-1
#define USART2_TX 			-1
#define RST_W 				-1

//
// Boot
//
#define RTS 				-1
#define BOOT0 				-1

//
// Beep
//
#define BEEP 				-1

//
// SPI Flash
//
#define SPI1_NSS 			-1
#define SPI1_MISO			-1
#define SPI1_SCK			-1
#define SPI1_MOSI			-1

//
// Onboard mSD Card Socket
//
#define SPI1_MISO 			-1
#define SPI1_NSS 			-1
#define SPI1_SCK 			-1
#define SPI1_MOSI 			-1
=======
// Temperature Sensors
//
#define TEMP_BED_PIN       PC0
#define TEMP_0_PIN         PC1
#define TEMP_1_PIN         PC2
#define TEMP_2_PIN         PC3

//
// Heaters / Fans
//
#define HEATER_0_PIN       PB4   // EXTRUDER 1
#define HEATER_1_PIN       PB5   // EXTRUDER 2
#define HEATER_2_PIN       PB0   // EXTRUDER 3

#define HEATER_BED_PIN     PB1   // BED

#define FAN_PIN            PB7   // (Sprinter config)
#define FAN1_PIN           PB8
#define FAN2_PIN           PB9

//
// Misc. Functions
//
#define LED_PIN            PD12
>>>>>>> 15e4f82f8a24b779d0d323b3223be3469ac0993b
