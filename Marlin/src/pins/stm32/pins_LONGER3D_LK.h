/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Longer3D LK1/LK2 & Alfawise U20/U30 (STM32F103VET6) board pin assignments
 */

#if !defined(__STM32F1__) && !defined(STM32F1xx)
  #error "Oops! Select a STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "Longer3D board only supports 1 hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_NAME "Longer3D"
#define ALFAWISE_UX0             // Common to all Longer3D STM32F1 boards (used for Open drain mosfets)

//#define DISABLE_DEBUG          //  We still want to debug with STLINK...
#define DISABLE_JTAG             //  We free the jtag pins (PA15) but keep STLINK
                                 //  Release PB4 (STEP_X_PIN) from JTAG NRST role.
//
// Limit Switches
//
#define X_MIN_PIN          PC1   // pin 16
#define X_MAX_PIN          PC0   // pin 15 (Filament sensor on Alfawise setup)
#define Y_MIN_PIN          PC15  // pin 9
#define Y_MAX_PIN          PC14  // pin 8 (Unused in stock Alfawise setup)
#define Z_MIN_PIN          PE6   // pin 5 Standard Endstop or Z_Probe endstop function
#define Z_MAX_PIN          PE5   // pin 4 (Unused in stock Alfawise setup)
                                 // May be used for BLTouch Servo function on older variants (<= V08)

//
// Filament Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PC0   // XMAX plug on PCB used as filament runout sensor on Alfawise boards (inverting true)
#endif

//
// Steppers
//
#define X_ENABLE_PIN       PB5   // pin 91
#define X_STEP_PIN         PB4   // pin 90
#define X_DIR_PIN          PB3   // pin 89

#define Y_ENABLE_PIN       PB8   // pin 95
#define Y_STEP_PIN         PB7   // pin 93
#define Y_DIR_PIN          PB6   // pin 92

#define Z_ENABLE_PIN       PE1   // pin 98
#define Z_STEP_PIN         PE0   // pin 97
#define Z_DIR_PIN          PB9   // pin 96

#define E0_ENABLE_PIN      PE4   // pin 3
#define E0_STEP_PIN        PE3   // pin 2
#define E0_DIR_PIN         PE2   // pin 1

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // pin 23 (Nozzle 100K/3950 thermistor)
#define TEMP_BED_PIN       PA1   // pin 24 (Hot Bed 100K/3950 thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN       PD3   // pin 84 (Nozzle Heat Mosfet)
#define HEATER_BED_PIN     PA8   // pin 67 (Hot Bed Mosfet)

#define FAN_PIN            PA15  // pin 77 (4cm Fan)
#define FAN_SOFT_PWM             // Required to avoid issues with heating or STLink
#define FAN_MIN_PWM        35    // Fan will not start in 1-30 range
#define FAN_MAX_PWM        255

//#define BEEPER_PIN       PD13  // pin 60 (Servo PWM output 5V/GND on Board V0G+) made for BL-Touch sensor
                                 // Can drive a PC Buzzer, if connected between PWM and 5V pins

#define LED_PIN            PC2   // pin 17

//
// PWM
//
#define SERVO0_PIN         PD13  // Open drain PWM pin on the V0G (GND or floating 5V)
#define SERVO0_PWM_OD            // Comment this if using PE5

//#define SERVO0_PIN       PE5   // Pulled up PWM pin on the V08 (3.3V or 0)

/**
 * Note: Alfawise screens use various TFT controllers. Supported screens
 * are based on the ILI9341, ILI9328 and ST7798V. Define init sequences for
 * other screens in u8g_dev_tft_320x240_upscale_from_128x64.cpp
 *
 * If the screen stays white, disable 'LCD_RESET_PIN' to let the bootloader
 * init the screen.
 *
 * Setting an 'LCD_RESET_PIN' may cause a flicker when entering the LCD menu
 * because Marlin uses the reset as a failsafe to revive a glitchy LCD.
 */

#define LCD_RESET_PIN      PC4   // pin 33
#define LCD_BACKLIGHT_PIN  PD12  // pin 59
#define FSMC_CS_PIN        PD7   // pin 88 = FSMC_NE1
#define FSMC_RS_PIN        PD11  // pin 58 A16 Register. Only one address needed

#define LCD_USE_DMA_FSMC   // Use DMA transfers to send data to the TFT
#define FSMC_DMA_DEV       DMA2
#define FSMC_DMA_CHANNEL   DMA_CH5

#define DOGLCD_MOSI        -1  // Prevent auto-define by Conditionals_post.h
#define DOGLCD_SCK         -1

/**
 * Note: Alfawise U20/U30 boards DON'T use SPI2, as the hardware designer
 * mixed up MOSI and MISO pins. SPI is managed in SW, and needs pins
 * declared below.
 */
#if ENABLED(TOUCH_BUTTONS)
  #define TOUCH_CS_PIN     PB12  // pin 51 SPI2_NSS
  #define TOUCH_SCK_PIN    PB13  // pin 52
  #define TOUCH_MOSI_PIN   PB14  // pin 53
  #define TOUCH_MISO_PIN   PB15  // pin 54
  #define TOUCH_INT_PIN    PC6   // pin 63 (PenIRQ coming from ADS7843)

  #define BTN_ENC          PB0   // pin 35 unconnected pin on Alfawise. (PC13 to try)
  #define BTN_EN1          -1    // Real pin is needed to enable encoder's push button
  #define BTN_EN2          -1    // functionality used by touch screen
#endif

//
// SPI1 (EEPROM W25Q64 + DAC OUT)
//
#undef E2END
#define E2END              0x7FF // EEPROM end address (reserve 2kB on sd/sram, real spi one is 8MB/64Mbits)
/*
#define SPI_EEPROM         1   // If commented this will create a file on the SD card as a replacement
#define SPI_CHAN_EEPROM1   1
#define SPI_EEPROM1_CS     PC5 // pin 34

//#define EEPROM_SCK  BOARD_SPI1_SCK_PIN   // PA5 pin 30
//#define EEPROM_MISO BOARD_SPI1_MISO_PIN  // PA6 pin 31
//#define EEPROM_MOSI BOARD_SPI1_MOSI_PIN  // PA7 pin 32
*/
