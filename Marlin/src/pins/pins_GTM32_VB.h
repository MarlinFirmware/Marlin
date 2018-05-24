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
#define BOARD_NAME "GTM32 VB"

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
#define Y_ENABLE_PIN       PA15

#define Z_STEP_PIN         PD6
#define Z_DIR_PIN          PD3
#define Z_ENABLE_PIN       PB3

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


/*
#if ENABLED(ULTRA_LCD)

  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    #define LCD_PINS_RS         -1   // CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE     -1   // SID (MOSI)
    #define LCD_PINS_D4         -1   // SCK (CLK) clock
  #elif ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
    #define LCD_PINS_RS         PB8
    #define LCD_PINS_ENABLE     PD2
    #define LCD_PINS_D4         PB12
    #define LCD_PINS_D5         PB13
    #define LCD_PINS_D6         PB14
    #define LCD_PINS_D7         PB15
  #else
    #define LCD_PINS_RS         PB8
    #define LCD_PINS_ENABLE     PD2
    #define LCD_PINS_D4         PB12
    #define LCD_PINS_D5         PB13
    #define LCD_PINS_D6         PB14
    #define LCD_PINS_D7         PB15
    #if DISABLED(NEWPANEL)
      #define BEEPER_PIN        33
      // Buttons are attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK 38
      //#define SHIFT_LD 42
      //#define SHIFT_OUT 40
      //#define SHIFT_EN 17
    #endif
  #endif

  #if ENABLED(NEWPANEL)

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

      #define BEEPER_PIN        37

      #define BTN_EN1           31
      #define BTN_EN2           33
      #define BTN_ENC           35

      #define SD_DETECT_PIN     49
      #define KILL_PIN          41

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        #define LCD_BACKLIGHT_PIN 39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      #define BTN_EN1           64
      #define BTN_EN2           59
      #define BTN_ENC           63
      #define SD_DETECT_PIN     42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      #define BTN_EN1           47
      #define BTN_EN2           43
      #define BTN_ENC           32
      #define LCD_SDSS          53
      #define SD_DETECT_PIN     -1
      #define KILL_PIN          41

    #elif ENABLED(LCD_I2C_VIKI)

      #define BTN_EN1           22   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      #define BTN_EN2            7   // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.

      #define BTN_ENC           -1
      #define LCD_SDSS          53
      #define SD_DETECT_PIN     49

    #elif ENABLED(VIKI2) || ENABLED(miniVIKI)

      #define BEEPER_PIN        33

      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0         44
      #define DOGLCD_CS         45
      #define LCD_SCREEN_ROT_180

      #define BTN_EN1           22
      #define BTN_EN2            7
      #define BTN_ENC           39

      #define SDSS              53
      #define SD_DETECT_PIN     -1   // Pin 49 for display sd interface, 72 for easy adapter board

      #define KILL_PIN          31

      #define STAT_LED_RED_PIN  32
      #define STAT_LED_BLUE_PIN 35

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #define BTN_EN1           35
      #define BTN_EN2           37
      #define BTN_ENC           31
      #define SD_DETECT_PIN     49
      #define LCD_SDSS          53
      #define KILL_PIN          41
      #define BEEPER_PIN        23
      #define DOGLCD_CS         29
      #define DOGLCD_A0         27
      #define LCD_BACKLIGHT_PIN 33

    #elif ENABLED(MINIPANEL)

      #define BEEPER_PIN        42
      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0         44
      #define DOGLCD_CS         66
      #define LCD_BACKLIGHT_PIN 65   // backlight LED on A11/D65
      #define SDSS              53

      #define KILL_PIN          64
      // GLCD features
      //#define LCD_CONTRAST   190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
      // The encoder and click button
      #define BTN_EN1           40
      #define BTN_EN2           63
      #define BTN_ENC           59
      // not connected to a pin
      #define SD_DETECT_PIN     49

    #else

      // Beeper on AUX-4
      #define BEEPER_PIN        33

      // buttons are directly attached using AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        #define BTN_EN1         64
        #define BTN_EN2         59
        #define BTN_ENC         63
        #define SHIFT_OUT       40
        #define SHIFT_CLK       44
        #define SHIFT_LD        42
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1         59   // AUX2 PIN 3
        #define BTN_EN2         63   // AUX2 PIN 4
        #define BTN_ENC         49   // AUX3 PIN 7
      #else
        #define BTN_EN1         37
        #define BTN_EN2         35
        #define BTN_ENC         31
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN   -1
        #define KILL_PIN        -1
      #else
        //#define SD_DETECT_PIN -1   // Ramps doesn't use this
      #endif

    #endif
  #endif // NEWPANEL

#endif // ULTRA_LCD
*/

