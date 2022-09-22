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

#include "env_validate.h"

/**
 * 2017 Victor Perez Marlin for stm32f1 test
 */
#define BOARD_INFO_NAME      "TRONXY_F446"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// EEPROM
//

#if NO_EEPROM_SELECTED
  #if TRONXY_UI
    #define EEPROM_AT24CXX
  #else
    #define FLASH_EEPROM_EMULATION
  #endif
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // SoC Flash (framework-arduinoststm32-maple/STM32F1/libraries/EEPROM/EEPROM.h)
  #define EEPROM_START_ADDRESS (0x8000000UL + (512 * 1024) - 2 * EEPROM_PAGE_SIZE)
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB, but will use 2x more (4KB)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE
#else
  #ifndef EEPROM_AT24CXX
  #define I2C_EEPROM                              //AT24C32
  #endif
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PC15
#define X_MAX_PIN                           PB0
#define Y_STOP_PIN                          PC14

//PE3通常接Probe
//PC13-Z_MIN_PIN
//PF11-Z_MAX_PIN
#if DUAL_Z_ENDSTOP_PROBE
  #define Z_MIN_PROBE_PIN                   PE3
  #if DUAL_ZMAX_HOME                              //由于换大端后,固件用Z_MAX作Z1限位,但实际上,我们依然用Z_MIN作Z1的限位
  #define Z_MIN_PIN                         PF11
  #define Z_MAX_PIN                         PC13
  #else
  #define Z_MIN_PIN                         PC13
  #define Z_MAX_PIN                         PF11
  #endif
#elif defined(FIX_MOUNTED_PROBE)
  #define Z_STOP_PIN                        PE3
#else
  #define Z_MIN_PIN                         PC13
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

#if NUM_Z_STEPPER_DRIVERS > 1
  #define Z2_ENABLE_PIN                     PF7
  #define Z2_STEP_PIN                       PF6
  #define Z2_DIR_PIN                        PF4
#elif EXTRUDERS > 2
  #define E2_ENABLE_PIN                     PF7
  #define E2_STEP_PIN                       PF6
  #define E2_DIR_PIN                        PF4
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

//
// Heaters
//
#define HEATER_0_PIN                        PG7   // HEATER1

#define STEP_TIMER                             6
#define TEMP_TIMER                            14

#if ENABLED(LASER_FEATURE)
  #define SPINDLE_LASER_ENA_PIN             PB11  //wifi:TX
#endif
#if ENABLED(SPINDLE_LASER_USE_PWM)
  #define SPINDLE_LASER_PWM_PIN             PB10  //wifi:RX-TIM2_CH3
  //这里必须复合PeripheralPins.c中的PWM pin定义const PinMap PinMap_PWM[]
  //详见PWM_PIN(x)定义
#endif
// #define SERVO0_PIN                            PB10 //使能NUM_SERVOS>1后开放

#define TEMP_BED_PIN                        PC2   // TB1
#define HEATER_BED_PIN                      PE2   // HOT BED
//#define HEATER_BED_INVERTING              true

//
// Fans
//
#define CONTROLLER_FAN_PIN                  PD7   // BOARD FAN
#define FAN_PIN                             PG0   //FAN0
#define FAN1_PIN                            PB6   //FAN1
#define THROAT_FAN                             2
#define FAN2_PIN                            PG9   //FAN2
#define FAN3_PIN                            PF10  //FAN3
#define FAN_SOFT_PWM                              //YSZ-WORK:EN,不定义,风扇不受控
//
// Misc
//
#define BEEPER_PIN                          PA8
  //#define LED_PIN                         PG10
#define PS_ON_PIN                           PG10  //YSZ-COMMENT:暂时以LED模拟开关机
#if TRONXY_BACKUP_POWER
  #define POWER_LOSS_PIN                    PF11  // 配置成下拉输入
#else
  #define POWER_LOSS_PIN                    PE1   // PW_DET YSZ-COMMENT:这里接的是比较器LM393的输出,配置成上拉输入
#endif
#define POWER_LM393_PIN                     PE0   //YSZ-COMMENT:这里接的是比较器LM393的正极,必须配置成输出,高电平
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
// TronXY TFT Support
//

// Shared FSMC Configs
#define TOUCH_CS_PIN                        PD11  // SPI1_NSS
#define TOUCH_SCK_PIN                       PB13  // SPI1_SCK
#define TOUCH_MISO_PIN                      PB14  // SPI1_MISO
#define TOUCH_MOSI_PIN                      PB15  // SPI1_MOSI

#define TFT_RESET_PIN                       PB12
#define TFT_BACKLIGHT_PIN                   PG8

#define LCD_USE_DMA_FSMC                          // Use DMA transfers to send data to the TFT
#define FSMC_DMA_DEV                        DMA2
#define FSMC_DMA_CHANNEL                 DMA_CH5

#define TFT_CS_PIN                          PG12
#define TFT_RS_PIN                          PG2

// #define TFT_WIDTH                          480
// #define TFT_HEIGHT                         320
// #define TFT_PIXEL_OFFSET_X                  48
// #define TFT_PIXEL_OFFSET_Y                  32
// #define TFT_DRIVER                     ILI9488
// #define TFT_BUFFER_SIZE                  14400

#if (LCD_CHIP_INDEX == 1 && (TRONXY_UI == 1 || TRONXY_UI == 2)) || LCD_CHIP_INDEX == 3
  #define XPT2046_X_CALIBRATION           -17181
  #define XPT2046_Y_CALIBRATION            11434
  #define XPT2046_X_OFFSET                   501
  #define XPT2046_Y_OFFSET                    -9
#elif LCD_CHIP_INDEX == 1 && TRONXY_UI == 4
  #define XPT2046_X_CALIBRATION            11166
  #define XPT2046_Y_CALIBRATION            17162
  #define XPT2046_X_OFFSET                   -10
  #define XPT2046_Y_OFFSET                   -16
#elif LCD_CHIP_INDEX == 4 && TRONXY_UI == 3
  // #define XPT2046_X_CALIBRATION             8781
  // #define XPT2046_Y_CALIBRATION            11773
  // #define XPT2046_X_OFFSET                   -17
  // #define XPT2046_Y_OFFSET                   -16
  //倒过来
  #define XPT2046_X_CALIBRATION            -8553
  #define XPT2046_Y_CALIBRATION           -11667
  #define XPT2046_X_OFFSET                   253
  #define XPT2046_Y_OFFSET                   331
#elif LCD_CHIP_INDEX == 2
  #define XPT2046_X_CALIBRATION            17184
  #define XPT2046_Y_CALIBRATION            10604
  #define XPT2046_X_OFFSET                   -31
  #define XPT2046_Y_OFFSET                   -29
#endif

#define AT24CXX_SCL                         PB8
#define AT24CXX_SDA                         PB9
#define AT24CXX_WP                          PB7

//
// SD Card
//
#define SDIO_SUPPORT
#define SD_DETECT_PIN                       -1    // PF0, but it isn't connected
#define SDIO_CLOCK                       4500000
#define SDIO_READ_RETRIES                     16

#define SDIO_D0_PIN                         PC8
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PC11
#define SDIO_CK_PIN                         PC12
#define SDIO_CMD_PIN                        PD2

#define SPEAKER                                   //蜂鸣器无源
