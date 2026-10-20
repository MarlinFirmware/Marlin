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
 * Prusa Buddy (STM32F407VGT6) as found in the Prusa MINI / MINI+
 *
 * Pin assignments follow the Prusa-Firmware-Buddy sources for the
 * BUDDY_2209_02 board revision (src/common/hwio_pindef.h, src/main.cpp,
 * src/stm32f4xx_hal_msp.c). Early production boards with the same
 * STM32F407VGT6 share this layout.
 *
 * Onboard hardware:
 *  - Four TMC2209 drivers on a single-wire UART bus (USART2 TX / PD5)
 *  - ST25DV64K 8K I2C EEPROM (I2C1, PB8 / PB9)
 *  - W25X 8M SPI flash on SPI3 (PC10 / PC11 / PC12, CS PD7), unused by Marlin
 *  - USB-A host port on USB OTG_HS (PB14 / PB15) for a USB flash drive
 *  - Micro USB device port on USB OTG_FS (PA11 / PA12) for USB serial
 *  - ESP-01 WiFi header on USART6 (PC6 / PC7)
 *  - Debug UART on USART1 (PB6 / PB7)
 *  - 12 MHz HSE crystal
 *
 * There is no SD card slot and no EXP1 / EXP2 header. The display is the
 * Prusa "MINI LCD28" (ST7789V 240x320 SPI TFT with a rotary encoder) on
 * SPI2. Enable PRUSA_MINI_LCD for it.
 *
 * The PRUSA_BUDDY environment keeps the Prusa bootloader in the first 128K
 * of flash and links Marlin at 0x08020200. The build also produces an
 * unsigned firmware.bbf that the bootloader flashes from a USB drive. The
 * bootloader only accepts unsigned firmware once the appendix has been
 * broken. Use PRUSA_BUDDY_no_bootloader to flash Marlin at 0x08000000 over
 * the SWD header, replacing the bootloader.
 */

#include "env_validate.h"

#define BOARD_INFO_NAME      "Prusa Buddy"
#define DEFAULT_MACHINE_NAME "Prusa MINI"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Prusa Buddy supports up to 1 hotend / E stepper."
#endif

#define HAS_OTG_USB_HOST_SUPPORT                  // USB Flash Drive support

// Avoid conflict with TIMER_TONE
#define STEP_TIMER 10

//
// Onboard I2C EEPROM (ST25DV64K)
//
#if SHALL_USE_EEPROM(I2C_EEPROM)
  #define I2C_EEPROM
  #define MARLIN_EEPROM_SIZE             0x2000U  // 8K
  #define EEPROM_DEVICE_ADDRESS               0x53
  #define I2C_SCL_PIN                       PB8
  #define I2C_SDA_PIN                       PB9
#endif

//
// USB Host power switch (active LOW)
//
#if ENABLED(USB_FLASH_DRIVE_SUPPORT)
  #define USB_HOST_POWER_PIN                PE5
  #define BOARD_PREINIT() OUT_WRITE(USB_HOST_POWER_PIN, LOW)
#endif

//
// Trinamic StallGuard pins
//
#define X_DIAG_PIN                          PE2
#define Y_DIAG_PIN                          PE1
#define Z_DIAG_PIN                          PE3
#define E0_DIAG_PIN                         PA15

//
// Limit Switches
//
// X and Y have no endstop inputs. Sensorless homing is required.
//
#define X_STOP_PIN                    X_DIAG_PIN
#define Y_STOP_PIN                    Y_DIAG_PIN
#define Z_STOP_PIN                          PA8   // MINDA / SuperPINDA

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA8   // MINDA / SuperPINDA
#endif

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                      PB4

//
// Steppers
//
#define X_STEP_PIN                          PD1
#define X_DIR_PIN                           PD0
#define X_ENABLE_PIN                        PD3

#define Y_STEP_PIN                          PD13
#define Y_DIR_PIN                           PD12
#define Y_ENABLE_PIN                        PD14

#define Z_STEP_PIN                          PD4
#define Z_DIR_PIN                           PD15
#define Z_ENABLE_PIN                        PD2

#define E0_STEP_PIN                         PD9
#define E0_DIR_PIN                          PD8
#define E0_ENABLE_PIN                       PD10

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0   // THERM_0 (hotend)
#define TEMP_BED_PIN                        PA4   // THERM_1 (bed)
#define TEMP_1_PIN                          PA6   // THERM_PINDA / heatbreak
#define TEMP_BOARD_PIN                      PA5   // THERM_2 (board)

#if HOTENDS == 1 && DISABLED(HEATERS_PARALLEL)
  #if TEMP_SENSOR_PROBE
    #define TEMP_PROBE_PIN            TEMP_1_PIN
  #elif TEMP_SENSOR_CHAMBER
    #define TEMP_CHAMBER_PIN          TEMP_1_PIN
  #endif
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // HEAT0
#define HEATER_BED_PIN                      PB0   // BED_HEAT

#define FAN0_PIN                            PE11  // FAN (print fan)
#define FAN1_PIN                            PE9   // FAN1 (heatbreak fan) Set E0_AUTO_FAN_PIN to FAN1_PIN

#ifndef E0_FAN_TACHO_PIN
  #define E0_FAN_TACHO_PIN                  PE14  // FAN1 tachometer
#endif

//
// TMC2209 stepper drivers
//
#if HAS_TMC_UART
  /**
   * All four drivers share one single-wire UART bus on USART2 TX (PD5)
   * in hardware half-duplex mode. The environment defines PIN_SERIAL2_TX
   * without PIN_SERIAL2_RX so the STM32duino core configures USART2 for
   * single-wire half-duplex operation.
   */
  #define X_HARDWARE_SERIAL              MSerial2
  #define Y_HARDWARE_SERIAL              MSerial2
  #define Z_HARDWARE_SERIAL              MSerial2
  #define E0_HARDWARE_SERIAL             MSerial2

  #define X_SLAVE_ADDRESS                        1
  #define Y_SLAVE_ADDRESS                        3
  #define Z_SLAVE_ADDRESS                        0
  #define E0_SLAVE_ADDRESS                       2

  // The bus is open-drain with only the MCU's internal pull-up, so edges
  // are too slow for the drivers above 19200 baud.
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                    19200
  #endif
#endif

//
// Misc. Functions
//
#define BEEPER_PIN                          PA0   // Onboard buzzer (TIM2_CH1)

//
// LCD / Controller
//
// Prusa MINI LCD28: ST7789V 240x320 SPI TFT on SPI2 plus a rotary encoder
//
#if ENABLED(PRUSA_MINI_LCD)
  #define TFT_CS_PIN                        PC9
  #define TFT_DC_PIN                        PD11
  #define TFT_A0_PIN                  TFT_DC_PIN
  #define TFT_RESET_PIN                     PC8

  #define TFT_SCK_PIN                       PB10  // SPI2_SCK
  #define TFT_MISO_PIN                      PC2   // SPI2_MISO
  #define TFT_MOSI_PIN                      PC3   // SPI2_MOSI

  #define BTN_EN1                           PE15
  #define BTN_EN2                           PE13
  #define BTN_ENC                           PE12

  #define LCD_USE_DMA_SPI
#elif HAS_WIRED_LCD
  #error "Prusa Buddy only supports PRUSA_MINI_LCD."
#endif

//
// WiFi (ESP-01 on USART6)
//
#if ENABLED(WIFISUPPORT)
  #define ESP_WIFI_MODULE_COM                  6  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
  #define ESP_WIFI_MODULE_BAUDRATE      BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
  #define ESP_WIFI_MODULE_RESET_PIN         PC13
  #define ESP_WIFI_MODULE_GPIO0_PIN         PE6
#endif
