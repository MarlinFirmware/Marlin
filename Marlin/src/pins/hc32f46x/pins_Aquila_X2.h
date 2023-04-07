/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

//
// Voxlab Aqula X2 (HC32F46x) board pin assignments
//

#ifndef TARGET_HC32F46x
#error "Oops! the aquila X2 is a HC32F46x board and requires TARGET_HC32F46x to be defined"
#endif

#ifndef TARGET_STM32F1
#error "Oops! HC32F46x boards require that TARGET_STM32F1 is defined aswell!"
#endif

#if HOTENDS > 1 || E_STEPPERS > 1
#error "the aquila X2 only supports one hotend and E-stepper"
#endif

#define BOARD_INFO_NAME "Aquila X2 (H32)"
#define DEFAULT_MACHINE_NAME "Aquila X2 (H32)"

//
// EEPROM
//
#if NO_EEPROM_SELECTED
#define IIC_BL24CXX_EEPROM
// #define SDCARD_EEPROM_EMULATION
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
#define IIC_EEPROM_SDA PA11
#define IIC_EEPROM_SCL PA12
#define MARLIN_EEPROM_SIZE 0x800 // 2Kb (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
#define MARLIN_EEPROM_SIZE 0x800 // 2K
#endif

//
// Servos
//
#define SERVO0_PIN PB0 // BLTouch OUT

//
// Limit Switches
//
#define X_STOP_PIN PA5
#define Y_STOP_PIN PA6
#define Z_STOP_PIN PA7
#define Z_MIN_PROBE_PIN PB1 // BLTouch IN

//
// Filament Runout Sensor
//
// pulled high, switch to ground closed when filament is present
// (LOW == present; HIGH == runout)
#define FIL_RUNOUT_PIN PA4

//
// Steppers
//
#define X_ENABLE_PIN PC3
#define X_STEP_PIN PC2
#define X_DIR_PIN PB9

#define Y_ENABLE_PIN PC3
#define Y_STEP_PIN PB8
#define Y_DIR_PIN PB7

#define Z_ENABLE_PIN PC3
#define Z_STEP_PIN PB6
#define Z_DIR_PIN PB5

#define E0_ENABLE_PIN PC3
#define E0_STEP_PIN PB4
#define E0_DIR_PIN PB3

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
// #define DISABLE_DEBUG
// #define DISABLE_JTAG

//
// Temperature Sensors
//
#define TEMP_0_PIN PC5   // HEATER1 ADC1_IN15
#define TEMP_BED_PIN PC4 // HOT BED ADC1_IN14

//
// Heaters / Fans
//
#define HEATER_0_PIN PA1   // HEATER1
#define HEATER_BED_PIN PA2 // HOT BED

#define FAN0_PIN PA0 // FAN
#define FAN_SOFT_PWM
#define FAN_SOFT_PWM_REQUIRED

//
// SD Card
//
#define SD_DETECT_PIN PA10
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE

//
// SDIO Pins
//
#define BOARD_SDIO_D0 PC8
#define BOARD_SDIO_D1 PC9
#define BOARD_SDIO_D2 PC10
#define BOARD_SDIO_D3 PC11
#define BOARD_SDIO_CLK PC12
#define BOARD_SDIO_CMD PD2
#define BOARD_SDIO_DET PA10

//
// DWIN Screen, Encoder and Beeper
//
#undef LCD_SERIAL_PORT
#define LCD_SERIAL_PORT 1

#define BTN_ENC PB14 // SPI_MISO
#define BTN_EN1 PB15 // SPI3_MOSI
#define BTN_EN2 PB12 // SPI3_NSS

#define BEEPER_PIN PB13 // SPI3_CLK

//
// USARTS
//
#define BOARD_USART1_TX_PIN PC0
#define BOARD_USART1_RX_PIN PC1

#define BOARD_USART2_TX_PIN PA9
#define BOARD_USART2_RX_PIN PA15

#define BOARD_USART3_TX_PIN PE5
#define BOARD_USART3_RX_PIN PE4

//
// Misc.
//
#define LED PA3
