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
 * BOARD_TRONXY_CXY_446_V10
 * 
 * CXY-V6-191121 / CXY-446-V10-220413
 */

#pragma once

#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2 || NUM_RUNOUT_SENSORS > 2
  #error "Only 2 Hotends / E steppers / Filament Runout sensors are currently supported!"
#endif

#define BOARD_INFO_NAME      "BOARD_TRONXY_CXY_446_V10"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#define STEP_TIMER                         6
#define TEMP_TIMER                         14

//
// EEPROM
//

// Onboard I2C EEPROM
#if NO_EEPROM_SELECTED
  #undef NO_EEPROM_SELECTED
  #define I2C_EEPROM
  #define MARLIN_EEPROM_SIZE                0x800 // 2K (FT24C16A)
#endif

//
//  SPI Flash
//
#define SPI_FLASH                           // W25Q16

#if ENABLED(SPI_FLASH)
  #define SPI_DEVICE                        1
  #define SPI_FLASH_SIZE                    0x1000000 // 16MB
  // SPI1/SPI3
  #define SPI_FLASH_CS_PIN                  PG15 // W25Q16 CS - USART6_CTS, FMC_SDNCAS, DCMI_D13, EVENTOUT
  #define SPI_FLASH_MOSI_PIN                PB5 // W25Q16 DIN - TIM3_CH2, I2C1_SMBA, SPI1_MOSI/I2S1_SD, SPI3_MOSI/I2S3_SD, CAN2_RX, OTG_HS_ULPI_D7, FMC_SDCKE1, DCMI_D10, EVENTOUT
  #define SPI_FLASH_MISO_PIN                PB4 // W25Q16 DOUT - NJTRST, TIM3_CH1, I2C3_SDA, SPI1_MISO, SPI3_MISO, SPI2_NSS/I2S2_WS, EVENTOUT
  #define SPI_FLASH_SCK_PIN                 PB3 // W25Q16 DCLK - JTDO/TRACESWO, TIM2_CH2, I2C2_SDA, SPI1_SCK/I2S1_CK, SPI3_SCK/I2S3_CK, EVENTOUT
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PC15 // X STOP SW - EVENTOUT, OSC32_OUT

#define X_MIN_PIN                           X_STOP_PIN
#define X_MAX_PIN                           PB0 // E0 TMC UART candidate - TIM1_CH2N, TIM3_CH3, TIM8_CH2N, SPI3_MOSI/I2S3_SD, UART4_CTS, OTG_HS_ULPI_D1, SDIO_D1, EVENTOUT, ADC12_IN8

#define Y_STOP_PIN                          PC14 // Y STOP SW - EVENTOUT, OSC32_IN

#if ENABLED(Z_MULTI_ENDSTOPS)
  #if X_HOME_DIR > 0                             // Swap Z1/Z2 for dual Z with max homing
    #define Z_MIN_PIN                       PF11 // Z2 STOP SW - SAI2_SD_B, FMC_SDNRAS, DCMI_D12, EVENTOUT
    #define Z_MAX_PIN                       PC13 // Z STOP SW - EVENTOUT, TAMP_1/WKUP1
  #else
    #define Z_MIN_PIN                       PC13 // Z STOP SW
    #define Z_MAX_PIN                       PF11 // Z2 STOP SW
  #endif
  // PE3 is usually connected to Probe
  #if ENABLED(FIX_MOUNTED_PROBE)
    #define Z_MIN_PROBE_PIN                 PE3 // BED PROBE - TRACED0, SAI1_SD_B, FMC_A19, EVENTOUT
  #endif
#elif ENABLED(FIX_MOUNTED_PROBE)
  #define Z_STOP_PIN                        PE3 // BED PROBE
#else
  #define Z_STOP_PIN                        PC13 // Z STOP SW
#endif

//
// Filament Sensors
//
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #define FIL_RUNOUT_PIN                    PE6 // E0 STOP SW - TRACED3, TIM9_CH2, SPI4_MOSI, SAI1_SD_A, FMC_A22, DCMI_D7, EVENTOUT

  #if NUM_RUNOUT_SENSORS == 2
    #define FIL_RUNOUT2_PIN                 PF12 // E1 STOP SW - FMC_A6, EVENTOUT
  #endif
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PF0 // X TMC2225 EN - I2C2_SDA, FMC_A0, EVENTOUT
#define X_STEP_PIN                          PE5 // X TMC2225 STEP - TRACED2, TIM9_CH1, SPI4_MISO, SAI1_SCK_A, FMC_A21, DCMI_D6, EVENTOUT
#define X_DIR_PIN                           PF1 // X TMC2225 DIR - I2C2_SCL, FMC_A1, EVENTOUT

#define Y_ENABLE_PIN                        PF5 // Y TMC2225 EN - FMC_A5, EVENTOUT, ADC3_IN15
#define Y_STEP_PIN                          PF9 // Y TMC2225 STEP - SAI1_FS_B, TIM14_CH1, QUADSPI_BK1_IO1, EVENTOUT, ADC3_IN7
#define Y_DIR_PIN                           PF3 // Y TMC2225 DIR - FMC_A3, EVENTOUT, ADC3_IN9

#define Z_ENABLE_PIN                        PA5 // Z TMC2225 EN - TIM2_CH1/TIM2_ETR, TIM8_CH1N, SPI1_SCK/I2S1_CK, OTG_HS_ULPI_CK, EVENTOUT, ADC12_IN5, DAC_OUT2
#define Z_STEP_PIN                          PA6 // Z TMC2225 STEP - TIM1_BKIN, TIM3_CH1, TIM8_BKIN, SPI1_MISO, I2S2_MCK, TIM13_CH1, DCMI_PIXCLK, EVENTOUT, ADC12_IN6
#define Z_DIR_PIN                           PF15 // Z TMC2225 DIR -  FMPI2C1_SDA, FMC_A9, EVENTOUT

#ifdef Z2_DRIVER_TYPE
  #define Z2_ENABLE_PIN                     PF7 // Z2 TMC2225 EN - TIM11_CH1, SAI1_MCLK_B, QUADSPI_BK1_IO2, EVENTOUT, ADC3_IN5
  #define Z2_STEP_PIN                       PF6 // Z2 TMC2225 STEP - TIM10_CH1, SAI1_SD_B, QUADSPI_BK1_IO3, EVENTOUT, ADC3_IN4
  #define Z2_DIR_PIN                        PF4 // Z2 TMC2225 DIR - FMC_A4, EVENTOUT, ADC3_IN14
#endif

#define E0_ENABLE_PIN                       PF14 // E0 TMC2225 EN - FMPI2C1_SCL, FMC_A8, EVENTOUT
#define E0_STEP_PIN                         PB1 // E0 TMC2225 STEP - TIM1_CH3N, TIM3_CH4, TIM8_CH3N, OTG_HS_ULPI_D2, SDIO_D2, EVENTOUT, ADC12_IN9
#define E0_DIR_PIN                          PF13 // E0 TMC2225 DIR - FMPI2C1_SMBA, FMC_A7, EVENTOUT

#if (EXTRUDERS == 2)
  #ifndef E1_DRIVER_TYPE
    #error "E1_DRIVER_TYPE must be defined in Configuration.h to use 2 extruders!"
  #else
    #define E1_ENABLE_PIN                   PG5 // E1 TMC2225 EN - FMC_A15/FMC_BA1, EVENTOUT
    #define E1_STEP_PIN                     PD12 // E1 TMC2225 STEP - TIM4_CH1, FMPI2C1_SCL, USART3_RTS, QUADSPI_BK1_IO1, SAI2_FS_A, FMC_A17, EVENTOUT
    #define E1_DIR_PIN                      PG4 // E1 TMC2225 DIR - FMC_A14/FMC_BA0, EVENTOUT
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 Software Serial
   * 
   * Only uses 1 pin for UART
   * Modification to board required.
   * 
   * Use at your own risk!!!
   * 
   * Instructions: https://zenn.dev/marbocub/articles/tronxy-stm32f4-mainboard-tmc-serial-wiring
   */
  // Comment out error line below once pins have been configured to continue.
  #error "TMC UART not supported by default on this board. Modification to board required. See pins_TRONXY_CXY_446_V10.h for details."
  /*
  #define X_SERIAL_TX_PIN                    PE4 // TRACED1, SPI4_NSS, SAI1_FS_A, FMC_A20, DCMI_D4, EVENTOUT
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                    PF2 // I2C2_SMBA, FMC_A2, EVENTOUT
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                    PA4 // SPI1_NSS/I2S1_WS, SPI3_NSS/I2S3_WS, USART2_CK, OTG_HS_SOF, DCMI_HSYNC, EVENTOUTADC12_IN4, DAC_OUT1
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN
  
  #ifdef Z2_DRIVER_TYPE
    #define Z2_SERIAL_TX_PIN                 PF8 // SAI1_SCK_B, TIM13_CH1, QUADSPI_BK1_IO0, EVENTOUT
    #define Z2_SERIAL_RX_PIN    Z2_SERIAL_TX_PIN
  #endif

  #define E0_SERIAL_TX_PIN                   PB0 // TIM1_CH2N, TIM3_CH3, TIM8_CH2N, SPI3_MOSI/I2S3_SD, UART4_CTS, OTG_HS_ULPI_D1, SDIO_D1, EVENTOUTADC12_IN8
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  // Unsure on this pin assignment
  //#define E1_SERIAL_TX_PIN                 PD5  // USART2_TX, FMC_NWE, EVENTOUT
  //#define E1_SERIAL_RX_PIN    E1_SERIAL_TX_PIN
  */
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC3 // Hotend #1 Therm - SPI2_MOSI/I2S2_SD, OTG_HS_ULPI_NXT, FMC_SDCKE0, EVENTOUT, ADC123_IN13

#if EXTRUDERS == 2
  #define TEMP_1_PIN                        PC0 // Hotend #2 Therm - SAI1_MCLK_B, OTG_HS_ULPI_STP, FMC_SDNWE, EVENTOUT, ADC123_IN10
#endif

#define TEMP_BED_PIN                        PC2 // Bed Therm - SPI2_MISO, OTG_HS_ULPI_DIR, FMC_SDNE0, EVENTOUT, ADC123_IN12            

//
// Heaters
// 
#define HEATER_0_PIN                        PG7 // Hotend #1 Heater - USART6_CK, FMC_INT, DCMI_D13, EVENTOUT

#if EXTRUDERS == 2
  #define HEATER_1_PIN                      PA15 // Hotend #2 Heater - JTDI, TIM2_CH1/TIM2_ETR, HDMI_CEC, SPI1_NSS/I2S1_WS, SPI3_NSS/I2S3_WS, UART4_RTS, EVENTOUT
#endif

#define HEATER_BED_PIN                      PE2 // Bed Heater - TRACECLK, SPI4_SCK, SAI1_MCLK_A, QUADSPI_BK1_IO2, FMC_A23, EVENTOUT

//
// Fans
//
#define FAN0_PIN                          	PG0 // Part Cooling Fan #1 - FMC_A10, EVENTOUT

#if EXTRUDERS == 2
	#define FAN1_PIN                          PB6 // Part Cooling Fan #2 - TIM4_CH1, HDMI_CEC, I2C1_SCL, USART1_TX, CAN2_TX, QUADSPI_BK1_NCS, FMC_SDNE1, DCMI_D5, EVENTOUT
	#define FAN2_PIN                          PG9 // Extruder/Hotend #1 Heatsink Fan - SPDIFRX_IN3, USART6_RX, QUADSPI_BK2_IO2, SAI2_FS_B, FMC_NE2/FMC_NCE3, DCMI_VSYNC, EVENTOUT
	#define FAN3_PIN                          PF10 // Extruder/Hotend #2 Heatsink Fan - DCMI_D11, EVENTOUT, ADC3_IN8
#else
	#define FAN1_PIN                          PG9 // Extruder/Hotend #1 Heatsink Fan - SPDIFRX_IN3, USART6_RX, QUADSPI_BK2_IO2, SAI2_FS_B, FMC_NE2/FMC_NCE3, DCMI_VSYNC, EVENTOUT
#endif

#define CONTROLLER_FAN_PIN                  PD7 // USART2_CK, SPDIFRX_IN0, FMC_NE1, EVENTOUT

// Define so that hotend heatsink fans automatically start when hotends reach 50 degrees.
#if EXTRUDERS == 2
  #undef E0_AUTO_FAN_PIN
  #undef E1_AUTO_FAN_PIN
	#define E0_AUTO_FAN_PIN FAN2_PIN
	#define E1_AUTO_FAN_PIN FAN3_PIN
#else
  #undef E0_AUTO_FAN_PIN
	#define E0_AUTO_FAN_PIN FAN1_PIN
#endif

#define FAN_SOFT_PWM_REQUIRED 

//
// Laser / Servos  
// 
// NOTE: SPINDLE_LASER_PWM_PIN and SERVO0_PIN are the same pin.
//       Only one feature can be enabled at a time.
//
#if ENABLED(LASER_FEATURE)
  #if NUM_SERVOS > 0
    #error "NUM_SERVOS must equal 0 to enable LASER_FEATURE"
  #else
    #define SPINDLE_LASER_ENA_PIN             PB11 // WiFi TXD (Pin5) - TIM2_CH4, I2C2_SDA, USART3_RX, SAI2_SD_A, EVENTOUT
    #if ENABLED(SPINDLE_LASER_USE_PWM)
      #define SPINDLE_LASER_PWM_PIN           PB10 // WiFi RXD (Pin4) - TIM2_CH3, I2C2_SCL, SPI2_SCK/I2S2_CK, SAI1_SCK_A, USART3_TX, OTG_HS_ULPI_D3, EVENTOUT
      /**
       * NOTE: The PWM pin definition const PinMap PinMap_PWM[] in PeripheralPins.c must be compounded here.
       * See PWM_PIN(x) definition for details.
       */
    #endif
  #endif
#endif

#if NUM_SERVOS > 0 
  #if ENABLED(LASER_FEATURE)
    #error "LASER_FEATURE must be disabled to set NUM_SERVOS greater than 0"
  #elif NUM_SERVOS > 1
    #error "Only one servo is currently supported"
  #else
    #define SERVO0_PIN                        PB10 // WiFi RXD (Pin4) - TIM2_CH3, I2C2_SCL, SPI2_SCK/I2S2_CK, SAI1_SCK_A, USART3_TX, OTG_HS_ULPI_D3, EVENTOUT
    /** 
     * Enabled after enabling NUM_SERVOS > 0.
     * 
     * NOTE: Shares PB10 pin with laser. Features cannot be enabled at the same time.
     */
  #endif
#endif

//
// TFT with FSMC interface
//
#if ANY(TFT_TRONXY_X5SA, MKS_ROBIN_TFT43)

  //SPI2
  #define TOUCH_CS_PIN                      PD11 // TOUCH SCREEN HR2046 CS - FMPI2C1_SMBA, USART3_CTS, QUADSPI_BK1_IO0, SAI2_SD_A, FMC_A16, EVENTOUT
  #define TOUCH_SCK_PIN                     PB13 // TOUCH SCREEN HR2046 DCLK - TIM1_CH1N, SPI2_SCK/I2S2_CK, USART3_CTS, CAN2_TX, OTG_HS_ULPI_D6, EVENTOUT
  #define TOUCH_MISO_PIN                    PB14 // TOUCH SCREEN HR2046 DOUT - TIM1_CH2N, TIM8_CH2N, SPI2_MISO, USART3_RTS, TIM12_CH1, OTG_HS_DM, EVENTOUT
  #define TOUCH_MOSI_PIN                    PB15 // TOUCH SCREEN HR2046 DIN - RTC_REFIN, TIM1_CH3N, TIM8_CH3N, SPI2_MOSI/I2S2_SD, TIM12_CH2, OTG_HS_DP, EVENTOUT

  #define TFT_RESET_PIN                     PB12 // TOUCH SCREEN HR2046 CS	- TIM1_BKIN, I2C2_SMBA, SPI2_NSS/I2S2_WS, SAI1_SCK_B, USART3_CK, CAN2_RX, OTG_HS_ULPI_D5, OTG_HS_ID, EVENTOUT
  #define TFT_BACKLIGHT_PIN                 PG8 // LCD MODULE BACKLIGHT - SPDIFRX_IN2, USART6_RTS, FMC_SDCLK, EVENTOUT

  #define LCD_USE_DMA_FSMC                  // Use DMA transfers to send data to the TFT
  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL                  DMA_CH5

  #define TFT_CS_PIN                        PG12 // SPI4_MISO, SPDIFRX_IN1, USART6_RTS, FMC_NE4, EVENTOUT
  #define TFT_RS_PIN                        PG2 // FMC_A12, EVENTOUT

  #define FSMC_CS_PIN                       TFT_CS_PIN
  #define FSMC_RS_PIN                       TFT_RS_PIN
  
  #if ENABLED(TFT_LVGL_UI)
    #define HAS_SPI_FLASH_FONT                   1
    #define HAS_GCODE_PREVIEW                    1
    #define HAS_GCODE_DEFAULT_VIEW_IN_FLASH      0
    #define HAS_LANG_SELECT_SCREEN               1
    #define HAS_BAK_VIEW_IN_FLASH                0
    #define HAS_LOGO_IN_FLASH                    0
  #elif ENABLED(TFT_COLOR_UI)
    #define TFT_DRIVER                     ILI9488
    #define TFT_BUFFER_SIZE                  14400
  #endif

  //Touch Screen calibration
  #if ANY(TFT_LVGL_UI, TFT_COLOR_UI, TFT_CLASSIC_UI)
    #if DISABLED(TOUCH_SCREEN_CALIBRATION)
      #error "TFT screen requires TOUCH_SCREEN_CALIBRATION"
    #elif ANY(TOUCH_CALIBRATION_X, TOUCH_CALIBRATION_Y, TOUCH_OFFSET_X, TOUCH_OFFSET_Y)
      #undef TOUCH_CALIBRATION_X
      #undef TOUCH_CALIBRATION_Y
      #undef TOUCH_OFFSET_X
      #undef TOUCH_OFFSET_Y
    #endif
    #if ENABLED(TFT_TRONXY_X5SA)
      #define TOUCH_CALIBRATION_X           -17181
      #define TOUCH_CALIBRATION_Y            11434
      #define TOUCH_OFFSET_X                   501
      #define TOUCH_OFFSET_Y                    -9
    #elif ENABLED(MKS_ROBIN_TFT43)
      #define XPT2046_X_CALIBRATION          17184
      #define XPT2046_Y_CALIBRATION          10604
      #define XPT2046_X_OFFSET                 -31
      #define XPT2046_Y_OFFSET                 -29
    #endif
  #endif
#else
  #error "Only TFT_TRONXY_X5SA and MKS_ROBIN_TFT43 are currently supported with this board."
#endif

//
// SD Card / Flash Drive
//

// USB Flash Drive Support
#if ENABLED(USBHOST_HS_EN)
  #define HAS_OTG_USB_HOST_SUPPORT
#endif

// SD Card
#define ONBOARD_SDIO
#define SD_DETECT_PIN                     -1
#define SDIO_CLOCK                        4500000
#define SDIO_READ_RETRIES                 16

#define SDIO_D0_PIN                       PC8 // TRACED0, TIM3_CH3, TIM8_CH3, UART5_RTS, USART6_CK, SDIO_D0, DCMI_D2, EVENTOUT
#define SDIO_D1_PIN                       PC9 // MCO2, TIM3_CH4, TIM8_CH4, I2C3_SDA, I2S_CKIN, UART5_CTS, QUADSPI_BK1_IO0, SDIO_D1, DCMI_D3, EVENTOUT
#define SDIO_D2_PIN                       PC10 // SPI3_SCK/I2S3_CK, USART3_TX, UART4_TX, QUADSPI_BK1_IO1, SDIO_D2, DCMI_D8, EVENTOUT
#define SDIO_D3_PIN                       PC11 // SPI3_MISO, USART3_RX, UART4_RX, QUADSPI_BK2_NCS, SDIO_D3, DCMI_D4, EVENTOUT
#define SDIO_CK_PIN                       PC12 // I2C2_SDA, SPI3_MOSI/I2S3_SD, USART3_CK, UART5_TX, SDIO_CK, DCMI_D9, EVENTOUT
#define SDIO_CMD_PIN                      PD2 // TIM3_ETR, UART5_RX, SDIO_CMD, DCMI_D11, EVENTOUT

//
// Power Loss / Power Supply Control
// 
// NOTE: PS_ON_PIN and LED_PIN are the same pin.
//

#if ENABLED(PSU_CONTROL)
  // LED - Temporarily switch the machine with LED simulation
  #define PS_ON_PIN                       PG10 // SAI2_SD_B, FMC_NE3, DCMI_D2, EVENTOUT
  // 24V DET - Output of LM393 comparator, configured as pullup
  #define POWER_LOSS_PIN                  PE1 // FMC_NBL1, DCMI_D3, EVENTOUT 
  // +V for the LM393 comparator, configured as output high
  #define POWER_LM393_PIN                 PE0 // TIM4_ETR, SAI2_MCLK_A, FMC_NBL0, DCMI_D2, EVENTOUT
#endif

//
// Misc
//
#ifndef PS_ON_PIN
  #define LED_PIN                           PG10 // SAI2_SD_B, FMC_NE3, DCMI_D2, EVENTOUT
#endif

#define BEEPER_PIN                        PA8 // MCO1, TIM1_CH1, I2C3_SCL, USART1_CK, OTG_FS_SOF, EVENTOUT