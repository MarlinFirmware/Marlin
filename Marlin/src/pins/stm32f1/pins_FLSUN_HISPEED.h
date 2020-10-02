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
 * MKS Robin Mini USB Use UART3(PB10-TX,PB11-RX)     
 * #define SERIAL_PORT_2 3
 */

#if NOT_TARGET(__STM32F1__)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "FLSUN hispeed supports 1 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "FLSun HiSpeedV1"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// EEPROM
//
/*
#if EITHER(NO_EEPROM_SELECTED, I2C_EEPROM)
  #define I2C_EEPROM                              // EEPROM on I2C-0
  #define MARLIN_EEPROM_SIZE 0x1000               // 4KB
#endif
*/

#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U)          // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE   EEPROM_PAGE_SIZE  // 2KB
#endif

//
// Limit Switches
//
#define X_STOP_PIN                         PA15   ///-X
#define Y_STOP_PIN                         PA12   ///-Y
#define Z_MIN_PIN                          PA11   ///-Z
#define Z_MAX_PIN                           PC4   ///+Z

//
// Steppers
//
#define X_ENABLE_PIN                        PE4   ///X_EN
#define X_STEP_PIN                          PE3   ///X_STEP
#define X_DIR_PIN                           PE2   ///X_DIR

#define Y_ENABLE_PIN                        PE1   ///Y_EN
#define Y_STEP_PIN                          PE0   ///Y_STEP
#define Y_DIR_PIN                           PB9   ///Y_DIR

#define Z_ENABLE_PIN                        PB8   ///Z_EN
#define Z_STEP_PIN                          PB5   ///Z_STEP
#define Z_DIR_PIN                           PB4   ///Z_DIR

#define E0_ENABLE_PIN                       PB3   ///E0_EN
#define E0_STEP_PIN                         PD6   ///E0_STEP
#define E0_DIR_PIN                          PD3   ///E0_DIR

#if HAS_TMC220x
  
  #if ENABLED(HARDWARE_SERIAL)  /*  TMC2209 */ 
    #define  X_SLAVE_ADDRESS 3    // |  |  :
    #define  Y_SLAVE_ADDRESS 2    // :  |  :
    #define  Z_SLAVE_ADDRESS 1    // |  :  :
    //#define E0_SLAVE_ADDRESS 0    // :  :  :

    #define X_SERIAL_TX_PIN                  PA9  //TXD1  /PD5
    #define X_SERIAL_RX_PIN                  PA9  //TXD1  /PD5

    #define Y_SERIAL_TX_PIN                   PA9  //TXD1  /PD7
    #define Y_SERIAL_RX_PIN                   PA9  //TXD1  /PD7

    #define Z_SERIAL_TX_PIN                   PA9  //TXD1  /PD4
    #define Z_SERIAL_RX_PIN                   PA9  //TXD1  /PD4

/*
 * TMC2208 stepper UART-configurable by PDN_UART pin
 *   Software serial 
 */

  #elif ENABLED(SOFTWARE_SERIAL)  /*  TMC2208   */
    #define  X_SLAVE_ADDRESS 0
    #define  Y_SLAVE_ADDRESS 0
    #define  Z_SLAVE_ADDRESS 0

    #define X_SERIAL_TX_PIN                  PA10  //RXD1  /PD5
    #define X_SERIAL_RX_PIN                  PA10  //RXD1  /PD5

    #define Y_SERIAL_TX_PIN                   PA9  //TXD1  /PD7
    #define Y_SERIAL_RX_PIN                   PA9  //TXD1  /PD7

    #define Z_SERIAL_TX_PIN                   PC7  //IO1  /PD4
    #define Z_SERIAL_RX_PIN                   PC7  //IO1  /PD4

  #endif  
  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#else
// Motor current PWM pins

  #define MOTOR_CURRENT_PWM_XY_PIN            PA6 //VREF2/3 CONTROL XY
  #define MOTOR_CURRENT_PWM_Z_PIN             PA7 //VREF4 CONTROL Z
  #define MOTOR_CURRENT_PWM_RANGE             1500  // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
  #ifndef DEFAULT_PWM_MOTOR_CURRENT
   #define DEFAULT_PWM_MOTOR_CURRENT { 800, 800, 230 } 
  #endif
 
  #ifdef ESP_WIFI
    #define WIFI_IO0_PIN                        PA8  // PC13 MKS ESP WIFI IO0 PIN
    #define WIFI_IO1_PIN       			            PC7   // MKS ESP WIFI IO1 PIN
    #define WIFI_RESET_PIN			              	PA5   // MKS ESP WIFI RESET PIN
  #endif 
#endif

//=================EXTRUDER=======================//

//#if HAS_TMC220x
#if AXIS_DRIVER_TYPE(E0,TMC2208)
    #define E0_SLAVE_ADDRESS 0

    #define E0_SERIAL_TX_PIN                  PA8  //IO0 /PD9 
    #define E0_SERIAL_RX_PIN                  PA8  //IO0 /PD9 
    #define TMC_BAUD_RATE 19200
#else
// Motor current PWM pins
  //#define VREF_E0_PIN                       PB0 //Vref Control E
  #define MOTOR_CURRENT_PWM_E_PIN             PB0 //VREF1 CONTROL E
  #define MOTOR_CURRENT_PWM_RANGE             1500  // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
  #ifndef DEFAULT_PWM_MOTOR_CURRENT
   #define DEFAULT_PWM_MOTOR_CURRENT { 800, 800, 230 } 
  #endif
#endif


//
// Temperature Sensors(THM)
//
#define TEMP_0_PIN                          PC1   /// TEMP_E0
#define TEMP_BED_PIN                        PC0   /// TEMP_BED

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC3   /// HEATER_E0
#define HEATER_BED_PIN                      PA0   /// HEATER_BED-WKUP

#define FAN_PIN                             PB1   /// E_FAN
//#define CONTROLLER_FAN_PIN                  PD6   // BOARD FAN

//
// Misc. Functions
//
//#define POWER_LOSS_PIN                     PA1   // PW_SO 
#if ENABLED(BACKUP_POWER_SUPPLY)
  #define POWER_LOSS_PIN                     PA2   // PW_DET (UPS) MKSPWC
#endif

// Enable Power Supply Control
#if ENABLED(PSU_CONTROL)
  #define KILL_PIN 			                     PA2   // PW_DET
  #define KILL_PIN_INVERTING 		            true  //
  //#define PS_ON_PIN                          PA3  // PW_CN /PW_OFF
#endif

#define MT_DET_1_PIN                         PA4   //  MT_DET
#define MT_DET_2_PIN                         PE6   //  FALA_CRTL
#define MT_DET_PIN_INVERTING               false

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN            MT_DET_1_PIN
#endif

//#define SERVO0_PIN                          PA8   // use IO0 to enable BLTOUCH support/remove Mks_Wifi

//
//
// LED / NEOPixel
//
//
//#define LED_PIN                              PB2  //BOOT1

//#define LED_PWM                              PA8
#if ENABLED(NEOPIXEL_LED)
  #define NEOPIXEL_PIN                       PA8   // USED WIFI RX PIN
#endif

//
// SD Card
//
#define SDIO_SUPPORT
#define SDIO_CLOCK                       4500000  // 4.5 MHz /* 18 MHz (18000000) or 4.5MHz (450000) */
//#define SDIO_CLOCK                      18000000  // 18 MHz (18000000)
#if ENABLED(SDIO_SUPPORT)
  #define SCK_PIN                           PB13  // SPI2
  #define MISO_PIN                          PB14  // SPI2
  #define MOSI_PIN                          PB15  // SPI2
  #define SD_DETECT_PIN                     PD12  // SD_CD
#endif

//
// LCD / Controller
//
#ifndef BEEPER_PIN
  #define BEEPER_PIN                        PC5
#endif

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

 /* MKS Robin TFT v2.0 with ILI9341 */
 // Read display identification information (0xD3 on ILI9341)
  //#define XPT2046_X_CALIBRATION  12013
  //#define XPT2046_Y_CALIBRATION  -8711
  //#define XPT2046_X_OFFSET         -32
  //#define XPT2046_Y_OFFSET         256
  
  /* MKS Robin TFT v1.1 with ILI9328 */
  //#define XPT2046_X_CALIBRATION -11792
  //#define XPT2046_Y_CALIBRATION   8947
  //#define XPT2046_X_OFFSET         342
  //#define XPT2046_Y_OFFSET         -19

  /* MKS Robin TFT v1.1 with R61505 */
  //#define XPT2046_X_CALIBRATION  12489
  //#define XPT2046_Y_CALIBRATION   9210
  //#define XPT2046_X_OFFSET         -52
  //#define XPT2046_Y_OFFSET         -17
  
  /* QQS-Pro use MKS Robin TFT v2.0 */
  //+++++++++++++++++++++++//

  #define XPT2046_X_CALIBRATION         12013
  #define XPT2046_Y_CALIBRATION         -8711
  #define XPT2046_X_OFFSET                -32
  #define XPT2046_Y_OFFSET                256

  #define CS_PIN                           PD7
  #define DC_PIN                          PD11
  
  #define TFT_RST_PIN                       PC6  // FSMC_RST
  #define TFT_BACKLIGHT_PIN                PD13  // FSMC_LIGHT
  #define TFT_CS_PIN                     CS_PIN
  #define TFT_DC_PIN                     DC_PIN
  #define TFT_A0_PIN                     DC_PIN

  #define FSMC_CS_PIN                    CS_PIN
  #define FSMC_RS_PIN                    DC_PIN

  #define LCD_USE_DMA_FSMC      // Use DMA transfers to send data to the TFT
  #define FSMC_DMA_DEV                     DMA2
  #define FSMC_DMA_CHANNEL              DMA_CH5

#if ENABLED(FSMC_GRAPHICAL_TFT)

  #define LCD_RESET_PIN             TFT_RST_PIN    // FSMC_RST
  #define LCD_BACKLIGHT_PIN   TFT_BACKLIGHT_PIN

  #if NEED_TOUCH_PINS
    #define TOUCH_CS_PIN                    PC2   // SPI2_NSS
    #define TOUCH_SCK_PIN                  PB13   // SPI2_SCK
    #define TOUCH_MISO_PIN                 PB14   // SPI2_MISO
    #define TOUCH_MOSI_PIN                 PB15   // SPI2_MOSI
  
    #ifndef  BUTTON_DELAY_EDIT
      #define BUTTON_DELAY_EDIT                50   // (ms) Button repeat delay for edit screens
    #endif
    #ifndef BUTTON_DELAY_MENU 
      #define BUTTON_DELAY_MENU               250   // (ms) Button repeat delay for menus
    #endif
  
    #define TFT_MARLINUI_COLOR           0xFFFF   // White
    #define TFT_BTARROWS_COLOR           0xDEE6   // 11011 110111 00110 Yellow
    #define TFT_BTOKMENU_COLOR           0x145F   // 00010 100010 11111 Cyan
  #endif

#elif ENABLED(TFT_320x240)
  #define TFT_RESET_PIN                       PC6
  //#define TFT_BACKLIGHT_PIN                 PD13

  //#define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  //#define FSMC_CS_PIN                       PD7
  //#define FSMC_RS_PIN                       PD11
  //#define FSMC_DMA_DEV                      DMA2
  //#define FSMC_DMA_CHANNEL               DMA_CH5

    #if NEED_TOUCH_PINS
      #define TOUCH_CS_PIN                      PC2    // SPI2_NSS
      #define TOUCH_SCK_PIN                    PB13    // SPI2_SCK
      #define TOUCH_MISO_PIN                   PB14    // SPI2_MISO
      #define TOUCH_MOSI_PIN                   PB15    // SPI2_MOSI
    #endif

  #define TFT_BUFFER_SIZE                     14400

#endif
// end defintion  MKS robin TFT

// SPI
// Note: FLSun Hispeed (clone MKS_Robin_miniV2) board is using SPI2 interface.
//
#define ENABLE_SPI2
// SPI Flash
#define HAS_SPI_FLASH                         1
#define SPI_FLASH_SIZE                0x1000000   // 16MB

#if HAS_SPI_FLASH
// SPI 2
  #define W25QXX_CS_PIN                    PB12   //SPI2_NSS / Flash chip-select
  #define W25QXX_MOSI_PIN                  PB15
  #define W25QXX_MISO_PIN                  PB14
  #define W25QXX_SCK_PIN                   PB13
#endif
