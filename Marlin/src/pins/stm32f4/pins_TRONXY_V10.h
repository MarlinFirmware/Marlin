/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "env_validate.h"

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "Tronxy V10 supports up to 3 hotends / E steppers."
#endif

#define BOARD_INFO_NAME      "Tronxy V10 F446"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#define STEP_TIMER                             6
#define TEMP_TIMER                            14



// EEPROM
//
#if NO_EEPROM_SELECTED
  #undef NO_EEPROM_SELECTED
  //#define EEPROM_AT24CXX
  #define FLASH_EEPROM_EMULATION
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // SoC Flash (framework-arduinoststm32-maple/STM32F1/libraries/EEPROM/EEPROM.h)
  #define EEPROM_START_ADDRESS (0x8000000UL + (512 * 1024) - 2 * EEPROM_PAGE_SIZE)
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB, but will use 2x more (4KB)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE
#else
  #if ENABLED(EEPROM_AT24CXX)
    #define AT24CXX_SCL                     PB8
    #define AT24CXX_SDA                     PB9
    #define AT24CXX_WP                      PB7
  #else
    #define I2C_EEPROM                            // AT24C32
  #endif
  #define MARLIN_EEPROM_SIZE              0x1000  // 4K
#endif


//
// Limit Switches
//
#define X_MIN_PIN                           PC15
#define X_MAX_PIN                           PB0
#define Y_STOP_PIN                          PC14

#if ENABLED(Z_MULTI_ENDSTOPS)
  #if Z_HOME_DIR > 0 
  #define Z_MIN_PIN                           PF11
  #define Z_MAX_PIN                           PC13
  #else
  #define Z_MIN_PIN                           PC13
  #define Z_MAX_PIN                           PF11
  #endif
  #if ENABLED(FIX_MOUNTED_PROBE)
  #define Z_MIN_PROBE_PIN                     PE3
  #endif
#elif ENABLED(FIX_MOUNTED_PROBE)
  #define Z_STOP_PIN                          PE3
#else
  #define Z_MIN_PIN                           PC13
#endif


//
// Steppers
//
#define X_ENABLE_PIN                        PF0
#define X_STEP_PIN                          PE5
#define X_DIR_PIN                           PF1

#define Y_ENABLE_PIN                        PF5
#define Y_STEP_PIN                          PF9
#define Y_DIR_PIN                           PF3

#define Z_ENABLE_PIN                        PA5
#define Z_STEP_PIN                          PA6
#define Z_DIR_PIN                           PF15

#if defined(Z2_DRIVER_TYPE)
  #define Z2_ENABLE_PIN                       PF7
  #define Z2_STEP_PIN                         PF6
  #define Z2_DIR_PIN                          PF4
#elif EXTRUDERS > 2
  #define E2_ENABLE_PIN                       PF7
  #define E2_STEP_PIN                         PF6
  #define E2_DIR_PIN                          PF4
#endif

#define E0_ENABLE_PIN                       PF14
#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PF13

#define E1_ENABLE_PIN                       PG5
#define E1_STEP_PIN                         PD12
#define E1_DIR_PIN                          PG4


//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC3   // TH1
#define TEMP_1_PIN                          PC0   // TH2
#define TEMP_BED_PIN                        PC2   // TB1

#define HEATER_0_PIN                        PG7   // HEATER1
#define HEATER_1_PIN                        PA15 // HEATER2

#define HEATER_BED_PIN                      PE2   // HOT BED
  
//#define HEATER_BED_INVERTING              true


//
// Laser / Spindle
//
#if HAS_CUTTER
  #define SPINDLE_LASER_ENA_PIN             PB11  // wifi:TX
  #if ENABLED(SPINDLE_LASER_USE_PWM)
    #define SPINDLE_LASER_PWM_PIN           PB10  // wifi:RX-TIM2_CH3
    // The PWM pin definition const PinMap PinMap_PWM[] in PeripheralPins.c must be compounded here
    // See PWM_PIN(x) definition for details
  #endif
#endif

//
// Fans
//
#define CONTROLLER_FAN_PIN                  PD7 // BOARD FAN
#define FAN_PIN                             PG0 //FAN0
#define FAN1_PIN                            PB6 //FAN1
#define THROAT_FAN                          2
#define FAN2_PIN                            PG9 //FAN2 - hotend
#define FAN3_PIN                            PF10//FAN3
//#define FAN_SOFT_PWM


// Misc
//
#define BEEPER_PIN                          PA8

//#define LED_PIN                           PG10
#define PS_ON_PIN                           PG10  // Temporarily switch the machine with LED simulation

#if ENABLED(TRONXY_BACKUP_POWER)
  #define POWER_LOSS_PIN                    PF11  // Configure as drop-down input
#else
  #define POWER_LOSS_PIN                    PE1   // Output of LM393 comparator, configured as pullup
#endif
#define POWER_LM393_PIN                     PE0   // +V for the LM393 comparator, configured as output high


//
// Filament Sensors
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PE6   // MT_DET
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PF12
#endif


// SPI Flash
// #define SPI_FLASH_SIZE                  0x200000  // 2MB
// #define HAS_SPI_FLASH                          1

// SPI 2
#define W25QXX_CS_PIN                       PG15
#define W25QXX_MOSI_PIN                     PB5
#define W25QXX_MISO_PIN                     PB4
#define W25QXX_SCK_PIN                      PB3



//
// TFT with FSMC interface
//
#if HAS_FSMC_TFT
  // Shared FSMC Configs
  #define TOUCH_CS_PIN                      PD11   // SPI1_NSS
  #define TOUCH_SCK_PIN                     PB13   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PB14   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PB15   // SPI1_MOSI
  #define TFT_RESET_PIN                     PB12
  #define TFT_BACKLIGHT_PIN                 PG8
  //for Marlin UI support
  #define LCD_BACKLIGHT_PIN                 PG8

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL                  DMA_CH5

  #define TFT_CS_PIN                        PG12
  #define TFT_RS_PIN                        PG2

  //#define TFT_WIDTH                        480
  //#define TFT_HEIGHT                       320
  //#define TFT_PIXEL_OFFSET_X                48
  //#define TFT_PIXEL_OFFSET_Y                32
  //#define TFT_DRIVER                   ILI9488
  //#define TFT_BUFFER_SIZE                14400

  #if NEED_TOUCH_PINS
    #define TOUCH_CS_PIN                    PD11  // SPI1_NSS
    #define TOUCH_SCK_PIN                   PB13  // SPI1_SCK
    #define TOUCH_MISO_PIN                  PB14  // SPI1_MISO
    #define TOUCH_MOSI_PIN                  PB15  // SPI1_MOSI
  #endif

  #if LCD_CHIP_INDEX == 1  
    #define XPT2046_X_CALIBRATION            11166
    #define XPT2046_Y_CALIBRATION            17162
    #define XPT2046_X_OFFSET                   -10
    #define XPT2046_Y_OFFSET                   -16  
  #elif LCD_CHIP_INDEX == 3
    #define XPT2046_X_CALIBRATION           -17181
    #define XPT2046_Y_CALIBRATION            11434
    #define XPT2046_X_OFFSET                   501
    #define XPT2046_Y_OFFSET                    -9
  #elif LCD_CHIP_INDEX == 4
    #define XPT2046_X_CALIBRATION             -8553
    #define XPT2046_Y_CALIBRATION            -11667
    #define XPT2046_X_OFFSET                   253
    #define XPT2046_Y_OFFSET                   331
  #elif LCD_CHIP_INDEX == 2
    #define XPT2046_X_CALIBRATION            17184
    #define XPT2046_Y_CALIBRATION            10604
    #define XPT2046_X_OFFSET                   -31
    #define XPT2046_Y_OFFSET                   -29
  #endif
#endif


#define AT24CXX_SCL                         PB8
#define AT24CXX_SDA                         PB9
#define AT24CXX_WP                          PB7

//
// SD Card
//
#define SDIO_SUPPORT
#define SD_DETECT_PIN                       -1    // PF0, but not connected
#define SDIO_CLOCK                       4500000
#define SDIO_READ_RETRIES                     16

#define SDIO_D0_PIN                         PC8
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PC11
#define SDIO_CK_PIN                         PC12
#define SDIO_CMD_PIN                        PD2

#if USBHOST_HS_EN
  #define HAS_OTG_USB_HOST_SUPPORT
#endif

#define SPEAKER 