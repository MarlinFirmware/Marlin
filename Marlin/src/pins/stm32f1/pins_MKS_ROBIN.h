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
 * MKS Robin (STM32F130ZET6) board pin assignments
 *
 * https://github.com/makerbase-mks/MKS-Robin/tree/master/MKS%20Robin/Hardware
 */

#if NOT_TARGET(STM32F1, STM32F1xx)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS Robin"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_JTAG

//
// Enable SD EEPROM to prevent infinite boot loop
//
#ifdef ARDUINO_ARCH_STM32
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U) // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE (EEPROM_PAGE_SIZE)
#else
  #define SDCARD_EEPROM_EMULATION
#endif

//
// Servos
//
#define SERVO0_PIN                          PC3   // XS1 - 5
#define SERVO1_PIN                          PA1   // XS1 - 6
#define SERVO2_PIN                          PF9   // XS2 - 5
#define SERVO3_PIN                          PF8   // XS2 - 6

//
// Limit Switches
//
#define X_MIN_PIN                           PB12
#define X_MAX_PIN                           PB0
#define Y_MIN_PIN                           PC5
#define Y_MAX_PIN                           PC4
#define Z_MIN_PIN                           PA4
#define Z_MAX_PIN                           PF7

//
// Steppers
//
#define X_ENABLE_PIN                        PB9
#define X_STEP_PIN                          PB8
#define X_DIR_PIN                           PB5

#define Y_ENABLE_PIN                        PB4
#define Y_STEP_PIN                          PG15
#define Y_DIR_PIN                           PG10

#define Z_ENABLE_PIN                        PD7
#define Z_STEP_PIN                          PD3
#define Z_DIR_PIN                           PG14

#define E0_ENABLE_PIN                       PG13
#define E0_STEP_PIN                         PG8
#define E0_DIR_PIN                          PA15

#define E1_ENABLE_PIN                       PA12
#define E1_STEP_PIN                         PA11
#define E1_DIR_PIN                          PA8

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_1_PIN                          PC2   // TH2
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC7   // HEATER1
#define HEATER_1_PIN                        PA6   // HEATER2
#define HEATER_BED_PIN                      PC6   // HOT BED

#define FAN_PIN                             PA7   // FAN

/**
 * Note: MKS Robin board is using SPI2 interface. Make sure your stm32duino library is configured accordingly
 */
//#define MAX6675_SS_PIN                    PE5   // TC1 - CS1
//#define MAX6675_SS_PIN                    PE6   // TC2 - CS2

#define POWER_LOSS_PIN                      PA2   // PW_DET
#define PS_ON_PIN                           PA3   // PW_OFF
#define FIL_RUNOUT_PIN                      PF11  // MT_DET

#ifdef ARDUINO_ARCH_STM32F1
  #define BEEPER_PIN                        PC13
#else
  #define BEEPER_PIN                        -1
#endif
#define LED_PIN                             PB2

#if HAS_FSMC_TFT || HAS_GRAPHICAL_TFT
  #define TFT_CS_PIN                        PG12  // NE4
  #define TFT_RS_PIN                        PF0   // A0
#endif

#if HAS_FSMC_TFT
  /**
   * Note: MKS Robin TFT screens use various TFT controllers
   * Supported screens are based on the ILI9341, ST7789V and ILI9328 (320x240)
   * ILI9488 is not supported
   * Define init sequences for other screens in u8g_dev_tft_320x240_upscale_from_128x64.cpp
   *
   * If the screen stays white, disable 'LCD_RESET_PIN'
   * to let the bootloader init the screen.
   *
   * Setting an 'LCD_RESET_PIN' may cause a flicker when entering the LCD menu
   * because Marlin uses the reset as a failsafe to revive a glitchy LCD.
   */
  //#define LCD_RESET_PIN                   PF6
  #define LCD_BACKLIGHT_PIN                 PG11
  #define FSMC_CS_PIN                 TFT_CS_PIN
  #define FSMC_RS_PIN                 TFT_RS_PIN

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5
#elif HAS_GRAPHICAL_TFT
  #define TFT_RESET_PIN                     PF6
  #define TFT_BACKLIGHT_PIN                 PG11
#endif

#if NEED_TOUCH_PINS
  #define TOUCH_CS_PIN                      PB1   // SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
  #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
  #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI
  #define TOUCH_INT_PIN                     -1
#endif

// SPI1(PA7) & SPI3(PB5) not available
#define SPI_DEVICE                             2

#if ENABLED(SDIO_SUPPORT)
  #define SCK_PIN                           PB13  // SPI2
  #define MISO_PIN                          PB14  // SPI2
  #define MOSI_PIN                          PB15  // SPI2
  #define SD_DETECT_PIN                     PF12  // SD_CD
#else
  // SD as custom software SPI (SDIO pins)
  #define SCK_PIN                           PC12
  #define MISO_PIN                          PC8
  #define MOSI_PIN                          PD2
  #define SS_PIN                            -1
  #define ONBOARD_SD_CS_PIN                 PC11
  #define SDSS                              PD2
  #define SD_DETECT_PIN                     -1
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  MSerial1
  //#define Y_HARDWARE_SERIAL  MSerial1
  //#define Z_HARDWARE_SERIAL  MSerial1
  //#define E0_HARDWARE_SERIAL MSerial1
  //#define E1_HARDWARE_SERIAL MSerial1

  // Unused servo pins may be repurposed with SoftwareSerialM
  //#define X_SERIAL_TX_PIN                 PF8   // SERVO3_PIN -- XS2 - 6
  //#define Y_SERIAL_TX_PIN                 PF9   // SERVO2_PIN -- XS2 - 5
  //#define Z_SERIAL_TX_PIN                 PA1   // SERVO1_PIN -- XS1 - 6
  //#define E0_SERIAL_TX_PIN                PC3   // SERVO0_PIN -- XS1 - 5
  //#define X_SERIAL_RX_PIN      X_SERIAL_TX_PIN
  //#define Y_SERIAL_RX_PIN      Y_SERIAL_TX_PIN
  //#define Z_SERIAL_RX_PIN      Z_SERIAL_TX_PIN
  //#define E0_SERIAL_RX_PIN    E0_SERIAL_TX_PIN

  // Reduce baud rate for software serial reliability
  #if HAS_TMC_SW_SERIAL
    #define TMC_BAUD_RATE                  19200
  #endif
#endif
