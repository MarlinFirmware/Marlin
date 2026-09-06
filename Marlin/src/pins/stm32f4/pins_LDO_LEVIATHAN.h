/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * LDO Leviathan V1.2 (STM32F446ZET6) pin assignments
 * Schematic: https://github.com/MotorDynamicsLab/Leviathan/blob/master/Schematic/Leviathan_V1.2.pdf
 * Pins are derived from LDO's KiCad project (MotorDynamicsLab/Leviathan, KiCad/V1.2/) netlist.
 *
 * Thermistor pullups are 2.2kΩ, so Marlin's built-in sensor types misread,
 * typically MINTEMP at room temperature. Use TEMP_SENSOR_* 1000 with the
 * matching *_PULLUP_RESISTOR_OHMS 2200.
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND
  #error "The LDO Leviathan has only one heater output, so only 1 hotend is supported."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "LDO Leviathan V1.2"
#endif
#define BOARD_WEBSITE_URL "github.com/MotorDynamicsLab/Leviathan"

// If you have the LDO Leviathan Extension Board, enable LEVIATHAN_EXTENSION_BOARD
// https://github.com/MotorDynamicsLab/LeviathanExt
//#define LEVIATHAN_EXTENSION_BOARD

// Avoid conflict with TIMER_TONE (TIM6 in the variant)
#define STEP_TIMER 8

//
// EEPROM Emulation
//
#if SHALL_USE_EEPROM(FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define MARLIN_EEPROM_SIZE              0x1000  // 4K

  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#endif

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PG1   // HV-STEPPER-0
#define Y_DIAG_PIN                          PE10  // HV-STEPPER-1
#define Z_DIAG_PIN                          PD6   // STEPPER-0
#define Z2_DIAG_PIN                         PD1   // STEPPER-1
#define Z3_DIAG_PIN                         PA15  // STEPPER-2
#define Z4_DIAG_PIN                         PC6   // STEPPER-3
#define E0_DIAG_PIN                         PD12  // STEPPER-4

//
// Limit Switches
//
// The DIAG pins have their own MCU pins rather than being jumpered onto the
// endstop inputs, so sensorless homing needs each stop pointed at its driver.
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
#else
  #define X_STOP_PIN                        PC1   // X-ENDSTOP (J30)
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
#else
  #define Y_STOP_PIN                        PC2   // Y-ENDSTOP (J31)
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
#else
  #define Z_STOP_PIN                        PC3   // Z-ENDSTOP (J32)
#endif

// The extra Z drivers have no endstop inputs of their own, and Z_MULTI_ENDSTOPS
// requires a stop pin per Z stepper, so point each at its own driver.
#if ENABLED(Z_MULTI_ENDSTOPS)
  #ifdef Z2_STALL_SENSITIVITY
    #define Z2_STOP_PIN              Z2_DIAG_PIN
  #endif
  #ifdef Z3_STALL_SENSITIVITY
    #define Z3_STOP_PIN              Z3_DIAG_PIN
  #endif
  #ifdef Z4_STALL_SENSITIVITY
    #define Z4_STOP_PIN              Z4_DIAG_PIN
  #endif
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PF1   // Z-PROBE (J21), voltage via J20 jumper
#endif

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                      PC0   // FILAMENT-SENSOR (J29)

//
// Misc. Functions
//
#define LED_PIN                             PE1   // Status LED (also used by Katapult)
#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                    PE6   // LED-STRIP (LM3407 constant-current driver, PWM dimming)
#endif

//
// Steppers
//
#define X_STEP_PIN                          PB10  // HV-STEPPER-0 (TMC5160, 0.075Ω sense)
#define X_DIR_PIN                           PB11
#define X_ENABLE_PIN                        PG0
#ifndef X_CS_PIN
  #define X_CS_PIN                          PE15
#endif

#define Y_STEP_PIN                          PF15  // HV-STEPPER-1 (TMC5160, 0.075Ω sense)
#define Y_DIR_PIN                           PF14
#define Y_ENABLE_PIN                        PE9
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PE11
#endif

#define Z_STEP_PIN                          PD4   // STEPPER-0 (TMC2209, 0.110Ω sense)
#define Z_DIR_PIN                           PD3
#define Z_ENABLE_PIN                        PD7

#define Z2_STEP_PIN                         PC12  // STEPPER-1 (TMC2209, 0.110Ω sense)
#define Z2_DIR_PIN                          PC11
#define Z2_ENABLE_PIN                       PD2

#define Z3_STEP_PIN                         PC9   // STEPPER-2 (TMC2209, 0.110Ω sense)
#define Z3_DIR_PIN                          PC8
#define Z3_ENABLE_PIN                       PC10

#define Z4_STEP_PIN                         PG7   // STEPPER-3 (TMC2209, 0.110Ω sense)
#define Z4_DIR_PIN                          PG6
#define Z4_ENABLE_PIN                       PC7

#define E0_STEP_PIN                         PD10  // STEPPER-4 (TMC2209, 0.110Ω sense)
#define E0_DIR_PIN                          PD9
#define E0_ENABLE_PIN                       PD13

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA1   // TH0 (J26)
#define TEMP_BED_PIN                        PA2   // TH1 (J27)
#define TEMP_1_PIN                          PA0   // TH2 (J18) - free for chamber, etc.
#define TEMP_2_PIN                          PA3   // TH3 (J19) - free for chamber, etc.

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PG10  // HEATER
#define HEATER_BED_PIN                      PG11  // HEATBED

#define FAN0_PIN                            PB7   // FAN0 (J22), voltage via J13 jumper
#define FAN1_PIN                            PB3   // FAN1 (J23), voltage via J14 jumper
#define FAN2_PIN                            PF7   // FAN2 (J24), voltage via J15 jumper
#define FAN3_PIN                            PF9   // FAN3 (J25), voltage via J16 jumper

// Fan tachometer inputs. FAN0 tach is PB8 per the netlist (mainline Klipper's
// PB0 is wrong, since PB0 connects to no fan circuit, only to EXP3).
//#define E0_FAN_TACHO_PIN                  PB8   // FAN0 tach; FAN1 = PB4, FAN2 = PF6, FAN3 = PF8

//
// SD Support
//
#if !defined(SDCARD_CONNECTION) && DISABLED(NO_LCD_SDCARD)
  #define SDCARD_CONNECTION                  LCD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #error "The Leviathan has no onboard SD card socket. Use SDCARD_CONNECTION LCD."
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "CUSTOM_CABLE is not a supported SDCARD_CONNECTION for this board."
#endif

//
// SPI pins for TMC5160 stepper drivers (X, Y)
//
// Wired to hardware SPI4, but Marlin's hardware TMC SPI path only uses the
// default SPI bus, which this board dedicates to the SD/LCD (SPI1 on EXP2).
#define TMC_USE_SW_SPI
#ifndef TMC_SPI_MOSI
  #define TMC_SPI_MOSI                      PE14
#endif
#ifndef TMC_SPI_MISO
  #define TMC_SPI_MISO                      PE13
#endif
#ifndef TMC_SPI_SCK
  #define TMC_SPI_SCK                       PE12
#endif

#if HAS_TMC_UART
  //
  // TMC2209 stepper drivers - One dedicated UART pin per driver, no shared
  // bus and no slave addressing.
  //
  #define Z_SERIAL_TX_PIN                   PD5
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN
  #define Z2_SERIAL_TX_PIN                  PD0
  #define Z2_SERIAL_RX_PIN      Z2_SERIAL_TX_PIN
  #define Z3_SERIAL_TX_PIN                  PA8
  #define Z3_SERIAL_RX_PIN      Z3_SERIAL_TX_PIN
  #define Z4_SERIAL_TX_PIN                  PG8
  #define Z4_SERIAL_RX_PIN      Z4_SERIAL_TX_PIN
  #define E0_SERIAL_TX_PIN                  PD11
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif

#endif // HAS_TMC_UART

/**               ------                                      ------
 * (BEEPER) PG9  | 1  2 | PG12 (BTN_ENC)         (MISO) PA6  | 1  2 | PA5  (SCK)
 * (LCD_EN) PG13 | 3  4 | PG14 (LCD_RS)       (BTN_EN1) PE2  | 3  4 | PA4  (SD_SS)
 * (LCD_D4) PC13   5  6 | PC14 (LCD_D5)       (BTN_EN2) PE3    5  6 | PA7  (MOSI)
 * (LCD_D6) PC15 | 7  8 | PF0  (LCD_D7)     (SD_DETECT) PE5  | 7  8 | RESET
 *           GND | 9 10 | 5V                             GND | 9 10 | PE4  (KILL)
 *                ------                                      ------
 *                 EXP1                                        EXP2
 *
 * The encoder lines carry onboard 4.7kΩ pullups. LDO's schematic names
 * EXP2 pad 3 "BTN_EN2" and pad 5 "BTN_EN1"; if the encoder turns the wrong
 * way, enable REVERSE_ENCODER_DIRECTION.
 */
#define EXP1_01_PIN                         PG9
#define EXP1_02_PIN                         PG12
#define EXP1_03_PIN                         PG13
#define EXP1_04_PIN                         PG14
#define EXP1_05_PIN                         PC13
#define EXP1_06_PIN                         PC14
#define EXP1_07_PIN                         PC15
#define EXP1_08_PIN                         PF0

#define EXP2_01_PIN                         PA6
#define EXP2_02_PIN                         PA5
#define EXP2_03_PIN                         PE2
#define EXP2_04_PIN                         PA4
#define EXP2_05_PIN                         PE3
#define EXP2_06_PIN                         PA7
#define EXP2_07_PIN                         PE5
#define EXP2_08_PIN                         -1
#define EXP2_10_PIN                         PE4

#if SD_CONNECTION_IS(LCD)
  #define SD_SS_PIN                  EXP2_04_PIN
  #define SD_SCK_PIN                 EXP2_02_PIN
  #define SD_MISO_PIN                EXP2_01_PIN
  #define SD_MOSI_PIN                EXP2_06_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
#endif

//
// LCD / Controller
//

#if IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS               EXP2_03_PIN
  #endif

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_01_PIN
  #define BTN_ENC                    EXP1_02_PIN
  #ifndef KILL_PIN
    #define KILL_PIN                 EXP2_10_PIN
  #endif

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP1_03_PIN
    #define BTN_EN2                  EXP1_05_PIN

    #define LCD_PINS_EN              EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

  #else

    #define LCD_PINS_RS              EXP1_04_PIN

    #define BTN_EN1                  EXP2_03_PIN
    #define BTN_EN2                  EXP2_05_PIN

    #define LCD_PINS_EN              EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS              EXP1_03_PIN
      #define DOGLCD_A0              EXP1_04_PIN
      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN          EXP1_05_PIN  // Must be high or open for LCD to operate normally.
      #if ANY(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN      EXP1_06_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN      EXP1_07_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN      EXP1_08_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         EXP1_06_PIN
      #endif
    #endif // FYSETC_MINI_12864

    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_06_PIN
      #define LCD_PINS_D6            EXP1_07_PIN
      #define LCD_PINS_D7            EXP1_08_PIN

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1               120
  #define BOARD_ST7920_DELAY_2                80
  #define BOARD_ST7920_DELAY_3               580
#endif

#if HAS_SPI_TFT
  #define TFT_CS_PIN                 EXP2_04_PIN
  #define TFT_A0_PIN                 EXP2_07_PIN
  #define TFT_SCK_PIN                EXP2_02_PIN
  #define TFT_MISO_PIN               EXP2_01_PIN
  #define TFT_MOSI_PIN               EXP2_06_PIN

  #define TOUCH_INT_PIN              EXP1_07_PIN
  #define TOUCH_MISO_PIN             EXP1_06_PIN
  #define TOUCH_MOSI_PIN             EXP1_03_PIN
  #define TOUCH_SCK_PIN              EXP1_05_PIN
  #define TOUCH_CS_PIN               EXP1_04_PIN

  #define BTN_EN1                    EXP2_03_PIN
  #define BTN_EN2                    EXP2_05_PIN
  #define BTN_ENC                    EXP1_02_PIN
#endif

//
// NeoPixel LED
//
#ifndef BOARD_NEOPIXEL_PIN
  #define BOARD_NEOPIXEL_PIN                PF10  // (J28) level-shifted to 5V
#endif

// CAN bus (PB5 RX / PB6 TX, J33) is present on the board but not used by Marlin.

/**
 * LDO Leviathan Extension Board, a plug-in board for the EXP3 header (J34).
 * Schematic: https://github.com/MotorDynamicsLab/LeviathanExt/blob/master/Schematic/Leviathan_Ext_V1.1.pdf
 * Pins are derived from LDO's KiCad project (MotorDynamicsLab/LeviathanExt, KiCad/) netlist.
 *
 * Adds two TMC5160 HV steppers, two fans, two thermistors, and a buzzer.
 * The HV steppers are assigned to X2/Y2 to suit the all-wheel-drive CoreXY
 * layout LDO ships the board for. Set X2_DRIVER_TYPE and Y2_DRIVER_TYPE to
 * TMC5160 to use them.
 */
#if ENABLED(LEVIATHAN_EXTENSION_BOARD)

  //
  // Trinamic Stallguard pins
  //
  #define X2_DIAG_PIN                       PG3   // HV-STEPPER-2
  #define Y2_DIAG_PIN                       PE7   // HV-STEPPER-3

  // Without these, *_DUAL_ENDSTOPS silently falls back to sharing the X/Y
  // endstop input, which would read the wrong driver when homing sensorless.
  #if ENABLED(X_DUAL_ENDSTOPS) && defined(X2_STALL_SENSITIVITY)
    #define X2_STOP_PIN              X2_DIAG_PIN
  #endif
  #if ENABLED(Y_DUAL_ENDSTOPS) && defined(Y2_STALL_SENSITIVITY)
    #define Y2_STOP_PIN              Y2_DIAG_PIN
  #endif

  //
  // Steppers
  //
  #define X2_STEP_PIN                       PD15  // HV-STEPPER-2 (TMC5160, 0.075Ω sense)
  #define X2_DIR_PIN                        PD14
  #define X2_ENABLE_PIN                     PG2
  #ifndef X2_CS_PIN
    #define X2_CS_PIN                       PB12
  #endif

  #define Y2_STEP_PIN                       PG4   // HV-STEPPER-3 (TMC5160, 0.075Ω sense)
  #define Y2_DIR_PIN                        PE8
  #define Y2_ENABLE_PIN                     PB0
  #ifndef Y2_CS_PIN
    #define Y2_CS_PIN                       PG5
  #endif

  // These drivers are wired to SPI2, not the SPI4 the onboard drivers use, so
  // they need their own software SPI bus.
  #define X2_SPI_MOSI                       PB15
  #define X2_SPI_MISO                       PB14
  #define X2_SPI_SCK                        PB13
  #define Y2_SPI_MOSI                X2_SPI_MOSI
  #define Y2_SPI_MISO                X2_SPI_MISO
  #define Y2_SPI_SCK                  X2_SPI_SCK

  //
  // Temperature Sensors
  //
  // Also 2.2kΩ pullups, so the note above applies to these as well.
  #define TEMP_3_PIN                        PC4   // TH4 - free for chamber, etc.
  #define TEMP_4_PIN                        PC5   // TH5 - free for chamber, etc.

  //
  // Fans
  //
  // PF3 and PF5 have no timer on either MCU, so these fans only switch on and
  // off unless FAN_SOFT_PWM is enabled.
  #define FAN4_PIN                          PF5   // FAN4, voltage via J6 jumper
  #define FAN5_PIN                          PF3   // FAN5, voltage via J7 jumper

  // Fan tachometer inputs: FAN4 = PF4, FAN5 = PF2
  //#define E1_FAN_TACHO_PIN                PF4

  //
  // Buzzer
  //
  // Passive transducer, so SPEAKER is required to produce tones. A wired LCD's
  // beeper takes precedence, since EXP1 is assigned further above.
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                      PB1
  #endif

#endif // LEVIATHAN_EXTENSION_BOARD
