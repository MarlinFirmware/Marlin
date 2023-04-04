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

/**
 * Creality CREALITY_CR4NTxxC10 (STM32F401RET6) board pin assignments
 *
 * Sold as "E3 Free-runs Silent Motherboard"
 */
#include "env_validate.h"

#if HOTENDS > 1 || E_STEPPERS > 1
   #error "Creality Ender Series only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "CR4NT220622C10"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender 3 Series"
#endif
#define BOARD_WEBSITE_URL "www.creality.com"

#define BOARD_NO_NATIVE_USB
// #undef DISABLE_DEBUG // DISABLE_(DEBUG|JTAG) is not supported for STM32F4.
// #define DISABLE_JTAG

//
// EEPROM
//
//#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  //#define SDCARD_EEPROM_EMULATION
//#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA11
  #define IIC_EEPROM_SCL                    PA12
  #define MARLIN_EEPROM_SIZE                0x800  // 2Kb (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE                0x800  // 2Kb
#endif


//
// Limit Switches
//
#define X_STOP_PIN                          PA5
#define Y_STOP_PIN                          PA6

#ifdef BLTOUCH
  #define Z_STOP_PIN         PB0  // BLTouch IN PIN  原理图TOUCH的管脚已经变-----zy
  #define SERVO0_PIN         PB1  // BLTouch PWM-OUT PIN  原理图TOUCH的管脚已经变-----zy
  #define Z_STOP_PIN_NADD    PA7   //Added z-axis limit switch  rock_20210816
#else
  #define Z_STOP_PIN         PA7 //Z轴限位开关
#endif

//#define one (c14 || a15)

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB0   // BLTouch IN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA4   // "Pulled-high"
#endif

#define HAS_CHECKFILAMENT
/* CHECKFILAMENT */
#if ENABLED(HAS_CHECKFILAMENT)
  #define CHECKFILAMENT_PIN  PA4
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PC3
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PC2
#endif

#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PB9
#endif

#define Y_ENABLE_PIN                        PC3
#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB8
#endif

#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PB7
#endif

#define Z_ENABLE_PIN                        PC3
#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB6
#endif

#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB5
#endif

#define E0_ENABLE_PIN                       PC3
#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                       PB4
#endif

#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                        PB3
#endif


#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  #define X_HARDWARE_SERIAL  Serial6
  #define Y_HARDWARE_SERIAL  Serial6
  #define Z_HARDWARE_SERIAL  Serial6
  #define E0_HARDWARE_SERIAL Serial6

  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z2_HARDWARE_SERIAL Serial1

  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1
  //#define E5_HARDWARE_SERIAL Serial1
  //#define E6_HARDWARE_SERIAL Serial1
  //#define E7_HARDWARE_SERIAL Serial1

  #ifndef X_SLAVE_ADDRESS
    #define X_SLAVE_ADDRESS 0
  #endif

  #ifndef Y_SLAVE_ADDRESS
    #define Y_SLAVE_ADDRESS 1
  #endif

  #ifndef Z_SLAVE_ADDRESS
    #define Z_SLAVE_ADDRESS 2
  #endif

  #ifndef E0_SLAVE_ADDRESS
    #define E0_SLAVE_ADDRESS 3
  #endif


  // Software serial


    #define X_SERIAL_TX_PIN                   PC6
    #define X_SERIAL_RX_PIN                   PC7

    #define Y_SERIAL_TX_PIN                   X_SERIAL_TX_PIN
    #define Y_SERIAL_RX_PIN                   X_SERIAL_RX_PIN

    #define Z_SERIAL_TX_PIN                   X_SERIAL_TX_PIN
    #define Z_SERIAL_RX_PIN                   X_SERIAL_RX_PIN

    #define E0_SERIAL_TX_PIN                  X_SERIAL_TX_PIN
    #define E0_SERIAL_RX_PIN                  X_SERIAL_RX_PIN

    #define E1_SERIAL_TX_PIN                X_SERIAL_TX_PIN
    #define E1_SERIAL_RX_PIN                X_SERIAL_RX_PIN

    #define X_DIAG_PIN                          PC13
    #define Y_DIAG_PIN                          PC14
    #define Z_DIAG_PIN                          PC15
    #define E0_DIAG_PIN                         PA15



  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif


// Temperature Sensors
//
#define TEMP_0_PIN                          PC5   // TH1
#define TEMP_BED_PIN                        PC4   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1    // HEATER1
#define HEATER_BED_PIN                      PB10 //PA15   // HOT BED

#ifndef FAN0_PIN
  #define FAN0_PIN                           PA0   // FAN
#endif
#if PIN_EXISTS(FAN0)
  #define FAN_SOFT_PWM
#endif

//
// SD Card
//
#define SD_DETECT_PIN                    PC1
#define SDCARD_CONNECTION                ONBOARD
#define ONBOARD_SPI_DEVICE               1
// #define ONBOARD_SD_CS_PIN                PA4   // SDSS
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE                  // This board's SD is only seen by the printer

#if ENABLED(CR10_STOCKDISPLAY) && NONE(RET6_12864_LCD, VET6_12864_LCD)
  #error "Define RET6_12864_LCD or VET6_12864_LCD to select pins for CR10_STOCKDISPLAY with the Creality V4 controller."
#endif

#if ENABLED(RET6_12864_LCD)

  // RET6 12864 LCD
  #define LCD_PINS_RS                       PB12
  #define LCD_PINS_ENABLE                   PB15
  #define LCD_PINS_D4                       PB13

  #define BTN_ENC                           PB2
  #define BTN_EN1                           PA2
  #define BTN_EN2                           PB14

  #ifndef HAS_PIN_27_BOARD
    #define BEEPER_PIN                      PC0
  #endif

#elif ENABLED(VET6_12864_LCD)

  // VET6 12864 LCD
  #define LCD_PINS_RS                       PA4
  //#define LCD_PINS_ENABLE                   PA7
  #define LCD_PINS_D4                       PA5

  #define BTN_ENC                           PC5
  #define BTN_EN1                           PB10
  #define BTN_EN2                           PA6

#elif ENABLED(DWIN_CREALITY_LCD)

  // RET6 DWIN ENCODER LCD
  #define BTN_ENC                           PB14
  #define BTN_EN1                           PB15
  #define BTN_EN2                           PB12

  //#define LCD_LED_PIN                     PB2
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                      PB13
    #undef SPEAKER
  #endif

#elif ENABLED(DWIN_VET6_CREALITY_LCD)

  // VET6 DWIN ENCODER LCD
  #define BTN_ENC                           PA6
  //#define BTN_EN1                           PA7
  #define BTN_EN2                           PA4

  #define BEEPER_PIN                        PA5

#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
 #if HAS_CUTTER
   // #undef HEATER_0_PIN
   // #undef HEATER_BED_PIN
  // #undef FAN0_PIN
  // #define SPINDLE_LASER_ENA_PIN            PC0  // FET 1
  // #define SPINDLE_LASER_PWM_PIN            PC0  // Bed FET
  // #define SPINDLE_DIR_PIN                  PC0  // FET 4
   #define SPINDLE_LASER_ENA_PIN            PC0  // FET 1
   #define SPINDLE_LASER_PWM_PIN            PC0  // Bed FET
   #define SPINDLE_DIR_PIN                  PC0  // FET 4

   #define LASER_SOFT_PWM_PIN				PC0  //激光软PWM引脚
  #endif
