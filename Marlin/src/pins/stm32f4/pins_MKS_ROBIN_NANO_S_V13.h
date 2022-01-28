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

//#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin Nano-S V1.3 supports up to 2 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "MKS Robin Nano-S V1.3"

//#define EXT_EXTRUDER_DRIVER 0 // Если нужен сменный драйвер в слоте второго экструдера как основной экструдер, установить в единицу!

#define SPI_DEVICE                             2

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

// Servos
//
#define SERVO0_PIN                          PA8   // Enable BLTOUCH

//
// Limit Switches
//
#define X_STOP_PIN                          PA15
#define Y_STOP_PIN                          PA12
#define Z_MIN_PIN                           PA11
#define Z_MAX_PIN                           PC4
#define FIL_RUNOUT_PIN                      PA4

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2

#define Y_ENABLE_PIN                        PE1
#define Y_STEP_PIN                          PE0
#define Y_DIR_PIN                           PB9

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB4

#if EXT_EXTRUDER_DRIVER
#define E0_ENABLE_PIN                       PA3
#define E0_STEP_PIN                         PA6
#define E0_DIR_PIN                          PA1
#else
#define E0_ENABLE_PIN                       PB3
#define E0_STEP_PIN                         PD6
#define E0_DIR_PIN                          PD3
#endif
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PD9
#endif

#ifndef EXT_EXTRUDER_DRIVER
#define E1_ENABLE_PIN                       PA3
#define E1_STEP_PIN                         PA6
#define E1_DIR_PIN                          PA1
#else
//#define E1_ENABLE_PIN                     PA3
//#define E1_STEP_PIN                       PA6
//#define E1_DIR_PIN                        PA1
#endif

#if HAS_TMC_UART
  //
  // Software serial
  // No Hardware serial for steppers
  //
  #define X_SERIAL_TX_PIN                   PA6
  #define X_SERIAL_RX_PIN                   PA1

  #define Y_SERIAL_TX_PIN                   PA6
  #define Y_SERIAL_RX_PIN                   PA1

  #define Z_SERIAL_TX_PIN                   PA6
  #define Z_SERIAL_RX_PIN                   PA1

  #define E0_SERIAL_TX_PIN                  PA6
  #define E0_SERIAL_RX_PIN                  PA1

  #define E1_SERIAL_TX_PIN                  PA6
  #define E1_SERIAL_RX_PIN                  PA1

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_1_PIN                          PC2   // TH2
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC3   // HEATER1
#define HEATER_1_PIN                        PB0   // HEATER2
#define HEATER_BED_PIN                      PA0   // HOT BED

#define FAN_PIN                             PB1  // NOZZLE FAN
#define FAN1_PIN                            PB0  // HEATSINK FAN
//
// Thermocouples
//
//#define TEMP_0_CS_PIN             HEATER_0_PIN  // TC1 - CS1
//#define TEMP_0_CS_PIN             HEATER_1_PIN  // TC2 - CS2

//
// Misc. Functions
//
#define MT_DET_1_PIN                        PA4
#define MT_DET_2_PIN                        PE6
#define MT_DET_PIN_INVERTING               false  // LVGL UI filament RUNOUT PIN STATE

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN            MT_DET_1_PIN
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN           MT_DET_2_PIN
#endif

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#define SDIO_SUPPORT
#define SDIO_CLOCK                       4500000  
#define SD_DETECT_PIN                       PD12

#define BEEPER_PIN                          PC5

//Touch screen
#define TOUCH_CS_PIN                      PA7   // SPI2_NSS
#define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
#define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
#define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI

#ifndef XPT2046_X_CALIBRATION
  #define XPT2046_X_CALIBRATION          17880
#endif
#ifndef XPT2046_Y_CALIBRATION
  #define XPT2046_Y_CALIBRATION         -12234
#endif
#ifndef XPT2046_X_OFFSET
  #define XPT2046_X_OFFSET                 -45
#endif
#ifndef XPT2046_Y_OFFSET
  #define XPT2046_Y_OFFSET                  349
#endif



#if HAS_FSMC_TFT
  // #define DOGLCD_MOSI                       -1    // Prevent auto-define by Conditionals_post.h
  // #define DOGLCD_SCK                        -1

  #define TFT_RESET_PIN                     PC6   // FSMC_RST
  #define TFT_BACKLIGHT_PIN                 PD13

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

  #define TFT_CS_PIN                 FSMC_CS_PIN
  #define TFT_RS_PIN                 FSMC_RS_PIN

  #define TFT_BUFFER_SIZE                  480*30
#endif



#define HAS_SPI_FLASH                          1
#if HAS_SPI_FLASH
  #define SPI_FLASH_SIZE               0x1000000  // 16MB
  #define W25QXX_CS_PIN                     PB12
  #define W25QXX_MOSI_PIN                   PB15
  #define W25QXX_MISO_PIN                   PB14
  #define W25QXX_SCK_PIN                    PB13
#endif

/*
Модуль MKS WIFI
*/
#define MKS_WIFI

#ifdef MKS_WIFI

 #define MKS_WIFI_SERIAL_NUM                SERIAL_PORT_2
 #define MKS_WIFI_UART                      USART1
  #undef PLATFORM_M997_SUPPORT

#ifdef MKS_WIFI_IO0
  #undef MKS_WIFI_IO0
  #define MKS_WIFI_IO0                       PC13
#endif
 #define MKS_WIFI_IO4                       PC7
 #define MKS_WIFI_IO_RST                    PA5
#endif
