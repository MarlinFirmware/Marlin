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
 * FLSUN HiSpeed V1 (STM32F130VET6) board pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "FLSUN hispeed supports 1 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "FLSUN HISPEED"

//
//
//
#define DISABLE_DEBUG

//
// EEPROM
//
#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2KB
#endif

//
// Limit Switches
//
#define X_DIAG_PIN                          PA15  //-X
#define Y_DIAG_PIN                          PA12  //-Y
#define Z_DIAG_PIN                          PA11  //-Z
#define E0_DIAG_PIN                         PC4   //+Z

#define X_STOP_PIN                          PA15  //-X
#define Y_STOP_PIN                          PA12  //-Y
#define Z_MIN_PIN                           PA11  //-Z
#define Z_MAX_PIN                           PC4   //+Z

//
// Steppers
//
#define X_ENABLE_PIN                        PE4 //X_EN
#define X_STEP_PIN                          PE3 //X_STEP
#define X_DIR_PIN                           PE2 //X_DIR
#ifndef X_CS_PIN
  #define X_CS_PIN                          PA10 //RXD1 /PD5
#endif

#define Y_ENABLE_PIN                        PE1 //Y_EN
#define Y_STEP_PIN                          PE0 //Y_STEP
#define Y_DIR_PIN                           PB9 //Y_DIR
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PA9   //TXD1  /PD7
#endif

#define Z_ENABLE_PIN                        PB8 //Z_EN
#define Z_STEP_PIN                          PB5 //Z_STEP
#define Z_DIR_PIN                           PB4 //Z_DIR
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PA8   //IO0 /PD4
#endif

#define E0_ENABLE_PIN                       PB3 //E0_EN
#define E0_STEP_PIN                         PD6 //E0_STEP
#define E0_DIR_PIN                          PD3 //E0_DIR
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PC7   //IO1 /PD9
#endif

// Motor current PWM pins in orig //
//#define MOTOR_CURRENT_PWM_XY_PIN            PA6 //Vref Control XY
//#define MOTOR_CURRENT_PWM_Z_PIN             PA7 //Vref Control Z
//#define MOTOR_CURRENT_PWM_E_PIN             PB0 //Vref Control E1
//#define MOTOR_CURRENT_PWM_RANGE 1500            // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
//#define DEFAULT_PWM_MOTOR_CURRENT  { 1030, 1030, 1030 } // 1.05Amp per driver, here is XY, Z and E. This values determined empirically.

// This is a kind of workaround in case native marlin "digipot" interface won't work.
// Required to enable related code in STM32F1/HAL.cpp
//#ifndef MKS_ROBIN_MINI_VREF_PWM
//  #define MKS_ROBIN_MINI_VREF_PWM
//#endif

#define VREF_XY_PIN                       PA6 //Vref Control XY
#define VREF_Z_PIN                        PA7 //Vref Control Z
#define VREF_E1_PIN                       PB0 //Vref Control E

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //

  #define X_SERIAL_TX_PIN                   PA10  //RXD1  /PD5
  #define X_SERIAL_RX_PIN                   PA10  //RXD1  /PD5

  #define Y_SERIAL_TX_PIN                   PA9   //TXD1  /PD7
  #define Y_SERIAL_RX_PIN                   PA9   //TXD1  /PD7

  #define Z_SERIAL_TX_PIN                   PA8   //IO0  /PD4
  #define Z_SERIAL_RX_PIN                   PA8   //IO0  /PD4

  #define E0_SERIAL_TX_PIN                  PC7   //IO1 /PD9 
  #define E0_SERIAL_RX_PIN                  PC7   //IO1 /PD9 

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif // TMC2208 || TMC2209

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TEMP_E0
#define TEMP_BED_PIN                        PC0   // TEMP_BED

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC3   // HEATER_E0
#define HEATER_BED_PIN                      PA0   // HEATER_BED-WKUP

#define FAN_PIN                             PB1   // FAN

//
// Misc. Functions
//
//#define POWER_LOSS_PIN                      PA1   // PW_SO 
#define POWER_LOSS_PIN                      PA2   // PW_DET (UPS)
#define PS_ON_PIN                           PA3   // PW_CN /PW_OFF

#define MT_DET_1_PIN                        PA4 //MT_DET
//#define MT_DET_2_PIN                        PE6 //FALA_CTRL /EXT_IRQ
#define MT_DET_PIN_INVERTING false

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    MT_DET_1_PIN
#endif
//#define LED_PIN                           PB2

//#define WIFI_IO0_PIN                        PA8 //PA10   //USUART1_RX  /PC13 
//#define WIFI_IO1_PIN       			            PC7 //PA9    //USUART1_TX  /PC7
//#define WIFI_RESET_PIN			              	PA5 //WIFI_CTRL   /PE9

//
// SD Card
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#define SDIO_SUPPORT
#define SDIO_CLOCK                       4500000    // 4.5 MHz
#define SD_DETECT_PIN                       PD12    //FSCM_A17/USUART3_RX /SD_CD
#define ONBOARD_SD_CS_PIN                   PC11    //SD_CS
//#define SD_MOSI                             PD2   //SD_MOSI
//#define SD_MISO                             PC8   //SD_MISO
//#define SD_CS                               PC10  //SD_CS
//#define SD_SCK                              PC12  //SD_SCK
//#define SD_CD                               PD13  //SD_CD

// SPI1(PA7) & SPI3(PB5) not available
//
// Note: FLSun Hispeed (clone MKS_Robin_miniV2) board is using SPI2 interface.
//

#define ENABLE_SPI2

#if ENABLED(SDIO_SUPPORT)
  #define SCK_PIN                           PB13  // SPI2_SCK
  #define MISO_PIN                          PB14  // SPI2_MISO
  #define MOSI_PIN                          PB15  // SPI2_MOSI
  #define SD_DETECT_PIN                     PD12  // SD_CD
#endif
//
// LCD / Controller
//
#ifndef BEEPER_PIN
  #define BEEPER_PIN                        PC5
#endif

/**
 * Note: MKS Robin TFT screens use various TFT controllers.
 * If the screen stays white, disable 'LCD_RESET_PIN'
 * to let the bootloader init the screen.
*/
  #define LCD_USE_DMA_FSMC       // Use DMA transfers to send data to the TFT
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

// Marlin UI
#if HAS_GRAPHICAL_TFT // TFT robin V2.0 (320x240) 

  #define TFT_RESET_PIN                     PC6   // FSMC_RST
  #define TFT_BACKLIGHT_PIN                 PD13  // FSMC_LIGHT
  #define TFT_CS_PIN                        PD7   // FSMC_NE4     /NE4
  #define TFT_RS_PIN                        PD11  // FSMC_A0    /A0
  //#define LCD_BACKLIGHT_PIN                 PD13  // FSMC_LIGHT
  #define FSMC_CS_PIN                       PD7   // FSMC_NE4
  #define FSMC_RS_PIN                       PD11  // FSMC_A0

/**** test to activation LVGL **********
#elif ENABLED(TFT_LVGL_UI) //MKS UI

  #define FSMC_CS_PIN                       PD7   // FSMC_NE4 /NE1
  #define FSMC_RS_PIN                       PD11  // A0

  #define TOUCH_CS_PIN                      PC2   // SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
  #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
  #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI

  #define LCD_BACKLIGHT_PIN                 PD13   // FSMC_NE4

  //#define XPT2046_X_CALIBRATION            17880
  //#define XPT2046_Y_CALIBRATION           -12234
  //#define XPT2046_X_OFFSET                   -45
  //#define XPT2046_Y_OFFSET                   349

  //#define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  //#define FSMC_CS_PIN                       PD7
  //#define FSMC_RS_PIN                       PD11
  //#define FSMC_DMA_DEV                      DMA2
  //#define FSMC_DMA_CHANNEL               DMA_CH5
*/

#else                 //  FSMC_GRAPHICAL_TFT
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
  //#define LCD_RESET_PIN                   PC6   // FSMC_RST
  #define LCD_BACKLIGHT_PIN                 PD13  // FSMC_LIGHT
  #define FSMC_CS_PIN                       PD7   // FSMC_NE4
  #define FSMC_RS_PIN                       PD11  // FSMC_A0
#endif

#if NEED_TOUCH_PINS
  #define TOUCH_CS_PIN                      PC2   // SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
  #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
  #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI
  #define TOUCH_INT_PIN                     -1
#endif

// end defintion  MKS robin TFT

#define HAS_SPI_FLASH 1
#define SPI_FLASH_SIZE                 0x1000000  // 16MB
#if HAS_SPI_FLASH
  #define W25QXX_CS_PIN                     PB12  //SPI2_NSS / Flash chip-select
  #define W25QXX_MOSI_PIN                   PB15
  #define W25QXX_MISO_PIN                   PB14
  #define W25QXX_SCK_PIN                    PB13
#endif

