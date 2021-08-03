/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "env_validate.h"

#define BOARD_INFO_NAME      "ZONESTAR ZM3E2 V1.0"
#define DISABLE_DEBUG
//#define DISABLE_JTAG
#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     (0x800) // 2KB
#define EEPROM_START_ADDRESS uint32(0x8000000 + (STM32_FLASH_SIZE) * 1024 - 2 * EEPROM_PAGE_SIZE)
#define E2END                (EEPROM_PAGE_SIZE - 1)

//=============================================================================
// ZONESTAR ZM3E2 V1.0 (STM32F130RCT6) board pin assignments
//=============================================================================
//	PA0		  PWR_HOLD	  //	PB0		  BEEP			      //	PC0	   		  HEATER_0
//	PA1     FAN_PIN     //	PB1     KILL            //	PC1         HEATER_BED
//	PA2	    TX2    		  //	PB2	    LCD_SDA         //	PC2	        TEMP_BED
//	PA3	    RX2    		  //	PB3	    E1_EN           //	PC3	        TEMP_E0
//	PA4     SD_CS		    //	PB4     Z_STOP          //	PC4         SD_DETECT
//	PA5     SD_SCK      //	PB5     Z_DIR           //	PC5         BTN_EN2
//	PA6     SD_MISO 	  //	PB6     Z_STEP          //	PC6         FAN1
//	PA7     SD_MOSI     //	PB7     Z_EN            //	PC7         FIL_RUNOUT
//	PA8     X_DIR       //	PB8     Y_STEP          //	PC8         X_EN
//	PA9     LCD_RS      //	PB9     Y_DIR           //	PC9         X_STEP
//	PA10    LCD_SCK     //	PB10    BTN_ENC         //	PC10        Z_MIN_PROBE_PIN
//	PA11    USB_D-      //	PB11    BTN_EN1         //	PC11        FIL_RUNOUT2
//	PA12    USB_D+      //	PB12    LED             //	PC12        E1_DIR
//	PA13    MS1         //	PB13    E0_EN           //	PC13        Y_STOP
//	PA14    MS2         //	PB14    E0_STEP         //	PC14        Y_EN
//	PA15    PWM         //	PB15    E0_DIR          //	PC15        X_STOP
//	PD0	 	  NC
//	PD1     NC
//	PD2	    E1_STEP
//=============================================================================

//EXP1 connector
//	   MARK     I/O     ZONESTAR_12864LCD      ZONESTAR_12864OLED
//	10 MOSI 	  PB1    	KILL					         SDA
//	 9 SCK  	  PB0		  BEEP					         SCK
//	 8 TX1  	  PA9		  DOGLCD_CS				       CS
//	 7 RX1  	  PA10	  DOGLCD_SCK				     DC
//	 6 ENA	 	  PC5    	BTN_EN2					       KNOB_ENB
//	 5 DAT  	  PB2    	DOGLCD_MOSI				     RESET
//	 4 TX3  	  PB10   	BTN_ENC					       KNOB_ENC
//	 3 RX3  	  PB11	  BTN_EN1					       KNOB_ENA
//	 2 +5V
//	 1 GND

//AUX1 connector
//	1	+5V
//	2	TX2  	PA2		UART2_TX
//	3	RX2  	PA3		UART2_RX
//	4	GND

//AUX2 connector to BLtouch
//	1	+5V
//	2	SEN  	PC10
//	3	PWM  	PA15
//	4	GND
//=============================================================================

//
// Limit Switches
//
#define X_STOP_PIN                          PC15
#define Y_STOP_PIN                          PC13
#define Z_STOP_PIN                          PB4

//
// Steppers
//
#define MS1_PIN                             PA13
#define MS2_PIN                             PA14

#define X_STEP_PIN                          PC9
#define X_DIR_PIN                           PA8
#define X_ENABLE_PIN                        PC8

#define Y_STEP_PIN                          PB8
#define Y_DIR_PIN                           PB9
#define Y_ENABLE_PIN                        PC14

#define Z_STEP_PIN                          PB6
#define Z_DIR_PIN                           PB5
#define Z_ENABLE_PIN                        PB7

#define E0_STEP_PIN                         PB14
#define E0_DIR_PIN                          PB15
#define E0_ENABLE_PIN                       PB13

#define E1_STEP_PIN                         PD2
#define E1_DIR_PIN                          PC12
#define E1_ENABLE_PIN                       PB3

//
// Heaters
//
#define HEATER_0_PIN                        PC0   // EXTRUDER 1
#define HEATER_BED_PIN                      PC1   // BED

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PC2   // Analog Input
#define TEMP_0_PIN                          PC3   // Analog Input

//
// Fans
//
#define FAN1_PIN                            PC6
#define FAN_PIN                             PA1

//Filament run out sensor
#define FIL_RUNOUT_PIN                      PC7   //E0_SW
//#define FIL_RUNOUT2_PIN                   PC11  //E1_SW

//BLTouch(3DTouch)
#define SERVO0_PIN                          PA15
#define Z_MIN_PROBE_PIN                     PC10

#define LED_PIN                             PB12
//#define KILL_PIN                          PB1   //@EXP1
#define SUICIDE_PIN                         PA0

//SD card
#define ENABLE_SPI1
#define SD_DETECT_PIN                       PC4
#define SCK_PIN                             PA5
#define MISO_PIN                            PA6
#define MOSI_PIN                            PA7
#define SS_PIN                              PA4

//
// LCD Pins
//
//================================================================================
//LCD 128x64
//================================================================================
//EXP1 connector
//	   MARK     I/O     ZONESTAR_12864LCD
//	10 MOSI 	  PB1    	KILL
//	 9 SCK  	  PB0		  BEEP
//	 8 TX1  	  PA9		  LCD_PINS_RS
//	 7 RX1  	  PA10	  LCD_PINS_D4
//	 6 ENA	 	  PC5    	BTN_EN2
//	 5 DAT  	  PB2    	LCD_PINS_ENABLE
//	 4 TX3  	  PB10   	BTN_ENC
//	 3 RX3  	  PB11	  BTN_EN1
//	 2 +5V
//	 1 GND

#if ENABLED(ZONESTAR_12864LCD)
  #define	LCDSCREEN_NAME		"ZONESTAR LCD12864"
  #define LCD_PINS_RS                       PA9
  #define LCD_PINS_ENABLE                   PB2
  #define LCD_PINS_D4                       PA10
  //#define KILL_PIN                        PB1
  #define BEEPER_PIN                        PB0
  #define BTN_EN1                           PB11
  #define BTN_EN2                           PC5
  #define BTN_ENC                           PB10
  #define BOARD_ST7920_DELAY_1 DELAY_NS(125)
  #define BOARD_ST7920_DELAY_2 DELAY_NS(200)
  #define BOARD_ST7920_DELAY_3 DELAY_NS(125)
#endif

//================================================================================
//OLED 128x64
//================================================================================
//	10 MOSI 	PB1   OLED_SDA
//	 9 SCK  	PB0		OLED_SCK
//	 8 TX1  	PA9		OLED_CS
//	 7 RX1  	PA10	OLED_DC
//	 6 ENA	 	PC5   KNOB_ENA
//	 5 DAT  	PB2   OLED_RESET
//	 4 TX3  	PB10  KNOB_ENC
//	 3 RX3  	PB11	KNOB_ENB

#if EITHER(ZONESTAR_12864OLED,ZONESTAR_12864OLED_SSD1306)
  #ifndef FORCE_SOFT_SPI
    #define FORCE_SOFT_SPI
  #endif
	#define LCDSCREEN_NAME	"ZONESTAR 12864OLED"
	#define LCD_PINS_RS                       PB2   //=LCD_RESET_PIN
	#define LCD_PINS_DC                       PA10  //DC
	#define DOGLCD_CS                         PA9   //CS
	#define DOGLCD_A0                  LCD_PINS_DC  //A0 = DC
	#define DOGLCD_MOSI                       PB1   //SDA
	#define DOGLCD_SCK                        PB0   //SCK
  	//Knob
	#define BTN_EN1                           PB11
	#define BTN_EN2                           PC5
	#define BTN_ENC                           PB10
#endif//ENABLED(ZONESTAR_OLED12864)
