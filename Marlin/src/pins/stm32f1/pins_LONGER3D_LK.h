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
 * Longer3D LK1/LK2 & Alfawise U20/U30 (STM32F103VET6) board pin assignments
 */

#if NOT_TARGET(__STM32F1__, STM32F1xx)
  #error "Oops! Select a STM32F1 board in 'Tools > Board.'"
#elif HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Longer3D only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "Longer3D"

#define BOARD_NO_NATIVE_USB

//#define DISABLE_DEBUG                           //  We still want to debug with STLINK...
#define DISABLE_JTAG                              //  We free the jtag pins (PA15) but keep STLINK
                                                  //  Release PB4 (STEP_X_PIN) from JTAG NRST role.
//
// Limit Switches
//
#define X_MIN_PIN                           PC1   // pin 16
#define X_MAX_PIN                           PC0   // pin 15 (Filament sensor on Alfawise setup)
#define Y_MIN_PIN                           PC15  // pin 9
#define Y_MAX_PIN                           PC14  // pin 8 (Unused in stock Alfawise setup)
#define Z_MIN_PIN                           PE6   // pin 5 Standard Endstop or Z_Probe endstop function
#define Z_MAX_PIN                           PE5   // pin 4 (Unused in stock Alfawise setup)
                                 // May be used for BLTouch Servo function on older variants (<= V08)
#define ONBOARD_ENDSTOPPULLUPS

//
// Filament Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC0   // XMAX plug on PCB used as filament runout sensor on Alfawise boards (inverting true)
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PB5   // pin 91
#define X_STEP_PIN                          PB4   // pin 90
#define X_DIR_PIN                           PB3   // pin 89

#define Y_ENABLE_PIN                        PB8   // pin 95
#define Y_STEP_PIN                          PB7   // pin 93
#define Y_DIR_PIN                           PB6   // pin 92

#define Z_ENABLE_PIN                        PE1   // pin 98
#define Z_STEP_PIN                          PE0   // pin 97
#define Z_DIR_PIN                           PB9   // pin 96

#define E0_ENABLE_PIN                       PE4   // pin 3
#define E0_STEP_PIN                         PE3   // pin 2
#define E0_DIR_PIN                          PE2   // pin 1

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // pin 23 (Nozzle 100K/3950 thermistor)
#define TEMP_BED_PIN                        PA1   // pin 24 (Hot Bed 100K/3950 thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PD3   // pin 84 (Nozzle Heat Mosfet)
#define HEATER_BED_PIN                      PA8   // pin 67 (Hot Bed Mosfet)

#define FAN_PIN                             PA15  // pin 77 (4cm Fan)
#define FAN_SOFT_PWM                              // Required to avoid issues with heating or STLink
#define FAN_MIN_PWM                           35  // Fan will not start in 1-30 range
#define FAN_MAX_PWM                          255

//#define BEEPER_PIN                        PD13  // pin 60 (Servo PWM output 5V/GND on Board V0G+) made for BL-Touch sensor
                                                  // Can drive a PC Buzzer, if connected between PWM and 5V pins

#define LED_PIN                             PC2   // pin 17

// Longer3D board mosfets are passing by default
// Avoid nozzle heat and fan start before serial init
#define BOARD_OPENDRAIN_MOSFETS

#define BOARD_PREINIT() { \
  OUT_WRITE_OD(HEATER_0_PIN, 0); \
  OUT_WRITE_OD(HEATER_BED_PIN, 0); \
  OUT_WRITE_OD(FAN_PIN, 0); \
}

//
// PWM for a servo probe
// Other servo devices are not supported on this board!
//
#if HAS_Z_SERVO_PROBE
  #define SERVO0_PIN                        PD13  // Open drain PWM pin on the V0G (GND or floating 5V)
  #define SERVO0_PWM_OD                           // Comment this if using PE5

  //#define SERVO0_PIN                      PE5   // Pulled up PWM pin on the V08 (3.3V or 0)
  //#undef Z_MAX_PIN                              // Uncomment if using ZMAX connector (PE5)
#endif

//
// TFT with FSMC interface
//
#if HAS_FSMC_TFT
  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7   // pin 88 = FSMC_NE1
  #define FSMC_RS_PIN                       PD11  // pin 58 A16 Register. Only one address needed
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #define TFT_CS_PIN                 FSMC_CS_PIN
  #define TFT_RS_PIN                 FSMC_RS_PIN

  #define TFT_RESET_PIN                     PC4   // pin 33
  #define TFT_BACKLIGHT_PIN                 PD12  // pin 59
  #define TFT_BACKLIGHT_PWM                 150   // Brightness with alt. TIM4 chan 1 (1-255)

  #define DOGLCD_MOSI                       -1    // Prevent auto-define by Conditionals_post.h
  #define DOGLCD_SCK                        -1

  // Buffer for Color UI
  #define TFT_BUFFER_SIZE                   3200
#endif

#if defined(TFT_BACKLIGHT_PWM) && !defined(MAPLE_STM32F1)
  #define HAS_LCD_BRIGHTNESS 1
  #define DEFAULT_LCD_BRIGHTNESS TFT_BACKLIGHT_PWM
#endif

#if ENABLED(SDIO_SUPPORT)
  #define SD_SS_PIN                         -1    // else SDSS set to PA4 in M43 (spi_pins.h)
#endif

/**
 * Note: Alfawise U20/U30 boards DON'T use SPI2, as the hardware designer
 * mixed up MOSI and MISO pins. SPI is managed in SW, and needs pins
 * declared below.
 */
#if NEED_TOUCH_PINS
  #define TOUCH_CS_PIN                      PB12  // pin 51 SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // pin 52
  #define TOUCH_MOSI_PIN                    PB14  // pin 53 (Inverted MOSI/MISO = No HW SPI2)
  #define TOUCH_MISO_PIN                    PB15  // pin 54
  #define TOUCH_INT_PIN                     PC6   // pin 63 (PenIRQ coming from ADS7843)
#endif

//
// Persistent Storage
// If no option is selected below the SD Card will be used
//
#if NO_EEPROM_SELECTED
  //#define SPI_EEPROM
  //#define HAS_SPI_FLASH                      1  // need MARLIN_DEV_MODE for M993/M994 eeprom backup tests
  #define FLASH_EEPROM_EMULATION
#endif

#if ENABLED(SPI_EEPROM)
  // SPI1 EEPROM Winbond W25Q64 (8MB/64Mbits)
  #define SPI_CHAN_EEPROM1                     1
  #define SPI_EEPROM1_CS_PIN                PC5   // pin 34
  #define EEPROM_SCK_PIN      BOARD_SPI1_SCK_PIN  // PA5 pin 30
  #define EEPROM_MISO_PIN    BOARD_SPI1_MISO_PIN  // PA6 pin 31
  #define EEPROM_MOSI_PIN    BOARD_SPI1_MOSI_PIN  // PA7 pin 32
  #define EEPROM_PAGE_SIZE               0x1000U  // 4KB (from datasheet)
  #define MARLIN_EEPROM_SIZE 16UL * (EEPROM_PAGE_SIZE)   // Limit to 64KB for now...
#elif HAS_SPI_FLASH
  #define SPI_FLASH_SIZE                0x40000U  // limit to 256KB (M993 will reboot with 512)
  #define SPI_FLASH_CS_PIN                  PC5
  #define SPI_FLASH_MOSI_PIN                PA7
  #define SPI_FLASH_MISO_PIN                PA6
  #define SPI_FLASH_SCK_PIN                 PA5
#elif ENABLED(FLASH_EEPROM_EMULATION)
  // SoC Flash (framework-arduinoststm32-maple/STM32F1/libraries/EEPROM/EEPROM.h)
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE (EEPROM_PAGE_SIZE)
#else
  #define MARLIN_EEPROM_SIZE              0x800U  // On SD, Limit to 2KB, require this amount of RAM
#endif
