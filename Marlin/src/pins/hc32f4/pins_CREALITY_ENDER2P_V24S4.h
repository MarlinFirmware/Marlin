/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// Creality Ender 2 Pro v2.4.S4_170 (HC32f460kcta)
//
#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Creality v2.4.S4 only supports one hotend and E-stepper"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Creality v2.4.S4"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender 2 Pro"
#endif

//
// Onboard crystal oscillator
//
#ifndef BOARD_XTAL_FREQUENCY
  #define BOARD_XTAL_FREQUENCY           8000000  // 8 MHz XTAL
#endif

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
//#define DISABLE_DEBUG
//#define DISABLE_JTAG

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM
  //#define SDCARD_EEPROM_EMULATION
  #undef NO_EEPROM_SELECTED
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA12
  #define IIC_EEPROM_SCL                    PA11
  #define MARLIN_EEPROM_SIZE               0x800 // 2K (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE               0x800 // 2K
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                        PB0   // BLTouch OUT *
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PA5
#define Y_STOP_PIN                          PA6
#define Z_STOP_PIN                          PB0 // BLTOUCH *

// #define X_MAX_PIN                           PA4
#define Y_MAX_PIN                           -1
#define Z_MAX_PIN                           -1

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB1   // BLTouch IN *
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15   // "Pulled-high" *
#endif

//
// Steppers
//
  #define X_ENABLE_PIN       PC2
  #define X_STEP_PIN         PC1
  #define X_DIR_PIN          PC0

  #define Y_ENABLE_PIN       PH2
  #define Y_STEP_PIN         PB9
  #define Y_DIR_PIN          PB8

  #define Z_ENABLE_PIN       PB7
  #define Z_STEP_PIN         PB6
  #define Z_DIR_PIN          PB5

  #define E0_ENABLE_PIN      PB4
  #define E0_STEP_PIN        PB3
  #define E0_DIR_PIN         PA15


//
// Temperature Sensors
//
#define TEMP_0_PIN		PC5   // HEATER1 ADC1_IN15
#define TEMP_BED_PIN       PC4   // HOT BED ADC1_IN14
#define  ADC_CHANNEL_COUNT      2u

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1   // HEATER1
#define HEATER_BED_PIN                      PB10   // HOT BED

#define FAN0_PIN                            PA0   // FAN0

#if ENABLED(CR10_STOCKDISPLAY)     // LCD used for C2

 
  #define BEEPER_PIN		             PC6 //LCD_ENC


  #define BTN_ENC 		               PC7	//LCD_RST
  #define BTN_EN1 		               PA2	// UART4-TX
  #define BTN_EN2 		               PB13	// SPI MISO

  #define LCD_PINS_RS 	  	         PB15//SPI3_NSS
  #define LCD_PINS_EN                PB12// SPI3_MOSI
  #define LCD_PINS_D4 	             PB14// SPI3_CLK

#endif


// ****** checked with klipper/config/printer-creality-ender2pro-hc32-2022.cfg ; https://github.com/Klipper3d/klipper/blob/master/config/printer-creality-ender2pro-hc32-2022.cfg

//
// SD Card
//
#define SD_DETECT_PIN                       PA10
#define ONBOARD_SPI_DEVICE                     1
#define ONBOARD_SD_CS_PIN                   PC3   // SDSS
#define ONBOARD_SDIO
#define NO_SD_HOST_DRIVE                          // This board's SD is only seen by the printer

  #ifndef LCD_SERIAL_PORT
    #define LCD_SERIAL_PORT                    1
  #endif

/*
 * SDIO Pins
 */
  #define BOARD_SDIO_D0 			PC8
  #define BOARD_SDIO_D1 			PC9
  #define BOARD_SDIO_D2 			PC10
  #define BOARD_SDIO_D3 			PC11
  #define BOARD_SDIO_CLK 			PC12
  #define BOARD_SDIO_CMD 			PD2
  #define BOARD_SDIO_DET 			PA10

//
// USART Pins
//

// Display
#define BOARD_USART1_TX_PIN     PA2// LCD
#define BOARD_USART1_RX_PIN     PA3

// Host
#define BOARD_USART2_TX_PIN     PA8//USB
#define BOARD_USART2_RX_PIN     PA7


// Onboard LED (HIGH = off, LOW = on)
#ifndef LED_BUILTIN
  #define LED_BUILTIN         PC3
#endif

#define BOARD_NO_NATIVE_USB

/*
 * SPI3 Pins (LCD)
 */
  #define SPI_NSS             PB15  //SPI3_NSS
  #define SPI_SCK             PB14 // SPI3_CLK
  #define SPI_MISO            PB13// SPI3_MISO
  #define SPI_MOSI            PB12 // SPI3_MOSI


// # This file contains pin mappings for the Creality Ender2 Pro
// #  with the HC32F460. The board is CR-FDM-v2.5.S4.170
// # To use this config, during "make menuconfig" select HC32F460

// # Flash this firmware by copying "out/klipper.bin" to a SD card
// # as /user/firmware.bin
// # Turn on the printer with the card inserted.

// # See docs/Config_Reference.md for a description of parameters.

// [stepper_x]
// step_pin: PC1
// dir_pin: PC0
// enable_pin: !PC2
// microsteps: 16
// rotation_distance: 40
// endstop_pin: ^PA5
// position_min: -20
// position_endstop: -20
// position_max: 165
// homing_speed: 50

// [stepper_y]
// step_pin: PB9
// dir_pin: PB8
// enable_pin: !PH2
// microsteps: 16
// rotation_distance: 40
// endstop_pin: ^PA6
// position_min: -5
// position_endstop: -5
// position_max: 165
// homing_speed: 50

// [stepper_z]
// step_pin: PB6
// dir_pin: !PB5
// enable_pin: !PB7
// microsteps: 16
// rotation_distance: 8
// endstop_pin: ^PB0
// position_endstop: 0.0
// position_max: 180

// [extruder]
// max_extrude_only_distance: 100.0
// step_pin: PB3
// dir_pin: PA15
// enable_pin: !PB4
// microsteps: 16
// rotation_distance: 27.53480577
// nozzle_diameter: 0.400
// filament_diameter: 1.750
// heater_pin: PA1
// sensor_pin: PC5
// sensor_type: Generic 3950
// control: pid
// pid_Kp: 29.634
// pid_Ki: 2.102
// pid_Kd: 104.459
// min_temp: 0
// max_temp: 260

// [heater_bed]
// heater_pin: PB10
// sensor_type: Generic 3950
// sensor_pin: PC4
// control: pid
// pid_Kp: 72.921
// pid_Ki: 1.594
// pid_Kd: 834.031
// min_temp: 0
// max_temp: 80

// [fan]
// pin: PA0

// [mcu]
// serial: /dev/serial/by-id/usb-1a86_USB_Serial-if00-port0
// restart_method: command

// [printer]
// kinematics: cartesian
// max_velocity: 300
// max_accel: 3000
// max_z_velocity: 5
// max_z_accel: 100

// [display]
// lcd_type: st7920
// cs_pin: PB15
// sclk_pin: PB14
// sid_pin: PB12
// encoder_pins: ^PB13,^PA2
// click_pin: ^!PC7


 // Data from kiel ".165 firmware version" 
  
// /**
//  * CREALITY v4.2.7 (STM32F103) board pin assignments
//  */

// #ifndef HC32F46x
//   #error "Oops! Select an HC32F46x board in 'options > c/c++->defines.'"
// #endif

// #if defined(CR10_STOCKDISPLAY) && !defined RET6_12864_LCD && !definedVET6_12864_LCD
//   #error "Define RET6_12864_LCD or VET6_12864_LCD to select pins for CR10_STOCKDISPLAY with the Creality V4 controller."
// #endif

// #if HOTENDS > 1 || E_STEPPERS > 1
//   #error "Creality V4 only supports one hotend / E-stepper. Comment out this line to continue."
// #endif

// #define BOARD_INFO_NAME      "Creality v2.4.S4"
// #define DEFAULT_MACHINE_NAME "Creality3D"



// #define BOARD_NO_NATIVE_USB
// /* LED0 Port/Pin definition */
// #define  LED0          PC3 // TEST
// /*
//  * SPI3 Pins
//  */
// #define SPI_NSS			PB15  //SPI3_NSS
// #define SPI_SCK                  	PB14 // SPI3_CLK
// #define SPI_MISO			PB13// SPI3_MISO
// #define SPI_MOSI			PB12 // SPI3_MOSI

// /*
//  * SDIO Pins
//  */
// #define BOARD_SDIO_D0 			PC8
// #define BOARD_SDIO_D1 			PC9
// #define BOARD_SDIO_D2 			PC10
// #define BOARD_SDIO_D3 			PC11
// #define BOARD_SDIO_CLK 			PC12
// #define BOARD_SDIO_CMD 			PD2
// #define BOARD_SDIO_DET 			PA10

// // USARTS

// #define BOARD_USART2_TX_PIN     PA8//USB
// #define BOARD_USART2_RX_PIN     PA7

// #define BOARD_USART1_TX_PIN     PA2// LCD
// #define BOARD_USART1_RX_PIN     PA3

// //
// // EEPROM
// //
//   #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
//   //#define SDCARD_EEPROM_EMULATION

// #ifdef IIC_BL24CXX_EEPROM
// #define IIC_EEPROM_SDA       PA12
// #define IIC_EEPROM_SCL       PA11
//   #define MARLIN_EEPROM_SIZE               0x800  // 2Kb (24C16)
// #endif

// //
// // Servos
// //
// #ifndef SERVO0_PIN
// #define SERVO0_PIN		 PB2  // BLTouch OUT PIN
// #endif

// //
// // Limit Switches
// //
// #define X_STOP_PIN                          PA5
// #define Y_STOP_PIN                          PA6
// #define Z_STOP_PIN                          PB0
// #define X_MAX_PIN                           PA4
// #define Y_MAX_PIN                           -1
// #define Z_MAX_PIN                           -1

// #ifndef Z_MIN_PROBE_PIN
//   #define Z_MIN_PROBE_PIN                   PB1   // BLTouch IN
// #endif

// //
// // Filament Runout Sensor
// //
// #ifndef FIL_RUNOUT_PIN
//   #define FIL_RUNOUT_PIN                    PC15   // "Pulled-high"
// #endif


// //
// // Steppers
// //
// #define X_ENABLE_PIN       PC2
// #define X_STEP_PIN         PC1
// #define X_DIR_PIN          PC0

// #define Y_ENABLE_PIN       PH2
// #define Y_STEP_PIN         PB9
// #define Y_DIR_PIN          PB8

// #define Z_ENABLE_PIN       PB7
// #define Z_STEP_PIN         PB6
// #define Z_DIR_PIN          PB5

// #define E0_ENABLE_PIN      PB4
// #define E0_STEP_PIN        PB3
// #define E0_DIR_PIN         PA15

// //
// // Temperature Sensors
// //
// #define TEMP_0_PIN		PC5   // HEATER1 ADC1_IN15
// #define TEMP_BED_PIN       PC4   // HOT BED ADC1_IN14
// #define  ADC_CHANNEL_COUNT      2u

// //
// // Heaters / Fans
// //
// #define HEATER_0_PIN                        PA1   // HEATER1
// #define HEATER_BED_PIN                      PB10   // HOT BED

// #ifndef FAN_PIN
//   #define FAN_PIN                           PA0   // FAN
// #endif
// #if PIN_EXISTS(FAN)
//   #define FAN_SOFT_PWM
// #endif

// //
// // SD Card
// //
// #define SD_DETECT_PIN      PA10
// #define SDCARD_CONNECTION                ONBOARD
// #define ONBOARD_SPI_DEVICE                     1
// #define ONBOARD_SD_CS_PIN                   PC3   // SDSS
// #define SDIO_SUPPORT
// #define NO_SD_HOST_DRIVE                          // This board's SD is only seen by the printer


// #if ENABLED(RET6_12864_LCD)

// /* RET6 12864 LCD */
// #define LCD_PINS_RS 	  	 PB15//SPI3_NSS
// #define LCD_PINS_ENABLE    PB12// SPI3_MOSI
// #define LCD_PINS_D4 	        PB14// SPI3_CLK

// #define BTN_ENC 		   PC7	//LCD_RST
// #define BTN_EN1 		   PA2	// UART4-TX
// #define BTN_EN2 		   PB13	// SPI MISO

// #define BEEPER_PIN		   PC6 //LCD_ENC

// #elif ENABLED(VET6_12864_LCD)

//   /* VET6 12864 LCD */
//   #define LCD_PINS_RS                       PA4
//   #define LCD_PINS_ENABLE                   PA7
//   #define LCD_PINS_D4                       PA5

//   #define BTN_ENC                           PC5
//   #define BTN_EN1                           PB10
//   #define BTN_EN2                           PA6

// #elif ENABLED(DWIN_CREALITY_LCD)

//   // RET6 DWIN ENCODER LCD
//   #define BTN_ENC                           PB14
//   #define BTN_EN1                           PB15
//   #define BTN_EN2                           PB12

//   //#define LCD_LED_PIN                     PB2
//   #ifndef BEEPER_PIN
//     #define BEEPER_PIN                      PB13
//     #undef SPEAKER
//   #endif

// #elif ENABLED(DWIN_VET6_CREALITY_LCD)

//   // VET6 DWIN ENCODER LCD
//   #define BTN_ENC                           PA6
//   #define BTN_EN1                           PA7
//   #define BTN_EN2                           PA4

//   #define BEEPER_PIN                        PA5

// #endif

