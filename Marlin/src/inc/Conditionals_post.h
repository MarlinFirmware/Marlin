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
 * Conditionals_post.h
 * Internal defines that depend on Configurations and Pins but are not user-editable.
 */

#ifdef GITHUB_ACTIONS
  // Extras for CI testing
#endif

// ADC
#ifdef BOARD_ADC_VREF
  #define ADC_VREF BOARD_ADC_VREF
#else
  #define ADC_VREF HAL_ADC_VREF
#endif

// Linear advance uses Jerk since E is an isolated axis
#if BOTH(HAS_JUNCTION_DEVIATION, LIN_ADVANCE)
  #define HAS_LINEAR_E_JERK 1
#endif

// Determine which type of 'EEPROM' is in use
#if ENABLED(EEPROM_SETTINGS)
  // EEPROM type may be defined by compile flags, configs, HALs, or pins
  // Set additional flags to let HALs choose in their Conditionals_post.h
  #if ANY(FLASH_EEPROM_EMULATION, SRAM_EEPROM_EMULATION, SDCARD_EEPROM_EMULATION, QSPI_EEPROM)
    #define USE_EMULATED_EEPROM 1
  #elif EITHER(I2C_EEPROM, SPI_EEPROM)
    #define USE_WIRED_EEPROM    1
  #elif ENABLED(IIC_BL24CXX_EEPROM)
    // nothing
  #else
    #define USE_FALLBACK_EEPROM 1
  #endif
#else
  #undef I2C_EEPROM
  #undef SPI_EEPROM
  #undef QSPI_EEPROM
  #undef SDCARD_EEPROM_EMULATION
  #undef SRAM_EEPROM_EMULATION
  #undef FLASH_EEPROM_EMULATION
  #undef IIC_BL24CXX_EEPROM
#endif

#ifdef TEENSYDUINO
  #undef max
  #define max(a,b) ((a)>(b)?(a):(b))
  #undef min
  #define min(a,b) ((a)<(b)?(a):(b))

  #undef NOT_A_PIN    // Override Teensyduino legacy CapSense define work-around
  #define NOT_A_PIN 0 // For PINS_DEBUGGING
#endif

/**
 * Axis lengths and center
 */
#if HAS_I_AXIS && !defined(AXIS4_NAME)
  #define AXIS4_NAME 'A'
#endif
#if HAS_J_AXIS && !defined(AXIS5_NAME)
  #define AXIS5_NAME 'B'
#endif
#if HAS_K_AXIS && !defined(AXIS6_NAME)
  #define AXIS6_NAME 'C'
#endif
#if HAS_U_AXIS && !defined(AXIS7_NAME)
  #define AXIS7_NAME 'U'
#endif
#if HAS_V_AXIS && !defined(AXIS8_NAME)
  #define AXIS8_NAME 'V'
#endif
#if HAS_W_AXIS && !defined(AXIS9_NAME)
  #define AXIS9_NAME 'W'
#endif

#if ANY(AXIS4_ROTATES, AXIS5_ROTATES, AXIS6_ROTATES, AXIS7_ROTATES, AXIS8_ROTATES, AXIS9_ROTATES)
  #define HAS_ROTATIONAL_AXES 1
#endif

#define X_MAX_LENGTH (X_MAX_POS - (X_MIN_POS))
#if HAS_Y_AXIS
  #define Y_MAX_LENGTH (Y_MAX_POS - (Y_MIN_POS))
#endif
#if HAS_Z_AXIS
  #define Z_MAX_LENGTH (Z_MAX_POS - (Z_MIN_POS))
#else
  #undef CONTROLLER_FAN_USE_Z_ONLY
#endif
#if HAS_I_AXIS
  #define I_MAX_LENGTH (I_MAX_POS - (I_MIN_POS))
#endif
#if HAS_J_AXIS
  #define J_MAX_LENGTH (J_MAX_POS - (J_MIN_POS))
#endif
#if HAS_K_AXIS
  #define K_MAX_LENGTH (K_MAX_POS - (K_MIN_POS))
#endif
#if HAS_U_AXIS
  #define U_MAX_LENGTH (U_MAX_POS - (U_MIN_POS))
#endif
#if HAS_V_AXIS
  #define V_MAX_LENGTH (V_MAX_POS - (V_MIN_POS))
#endif
#if HAS_W_AXIS
  #define W_MAX_LENGTH (W_MAX_POS - (W_MIN_POS))
#endif

// Defined only if the sanity-check is bypassed
#ifndef X_BED_SIZE
  #define X_BED_SIZE X_MAX_LENGTH
#endif
#if HAS_Y_AXIS && !defined(Y_BED_SIZE)
  #define Y_BED_SIZE Y_MAX_LENGTH
#endif
#if HAS_I_AXIS && !defined(I_BED_SIZE)
  #define I_BED_SIZE I_MAX_LENGTH
#endif
#if HAS_J_AXIS && !defined(J_BED_SIZE)
  #define J_BED_SIZE J_MAX_LENGTH
#endif
#if HAS_K_AXIS && !defined(K_BED_SIZE)
  #define K_BED_SIZE K_MAX_LENGTH
#endif
#if HAS_U_AXIS && !defined(U_BED_SIZE)
  #define U_BED_SIZE U_MAX_LENGTH
#endif
#if HAS_V_AXIS && !defined(V_BED_SIZE)
  #define V_BED_SIZE V_MAX_LENGTH
#endif
#if HAS_W_AXIS && !defined(W_BED_SIZE)
  #define W_BED_SIZE W_MAX_LENGTH
#endif

// Require 0,0 bed center for Delta, SCARA, and Polargraph
#if IS_KINEMATIC
  #define BED_CENTER_AT_0_0
#endif

// Define center values for future use
#define _X_HALF_BED ((X_BED_SIZE) / 2)
#if HAS_Y_AXIS
  #define _Y_HALF_BED ((Y_BED_SIZE) / 2)
#endif
#if HAS_I_AXIS
  #define _I_HALF_IMAX ((I_BED_SIZE) / 2)
#endif
#if HAS_J_AXIS
  #define _J_HALF_JMAX ((J_BED_SIZE) / 2)
#endif
#if HAS_K_AXIS
  #define _K_HALF_KMAX ((K_BED_SIZE) / 2)
#endif
#if HAS_U_AXIS
  #define _U_HALF_UMAX ((U_BED_SIZE) / 2)
#endif
#if HAS_V_AXIS
  #define _V_HALF_VMAX ((V_BED_SIZE) / 2)
#endif
#if HAS_W_AXIS
  #define _W_HALF_WMAX ((W_BED_SIZE) / 2)
#endif

#define X_CENTER TERN(BED_CENTER_AT_0_0, 0, _X_HALF_BED)
#if HAS_Y_AXIS
  #define Y_CENTER TERN(BED_CENTER_AT_0_0, 0, _Y_HALF_BED)
  #define XY_CENTER { X_CENTER, Y_CENTER }
#endif
#if HAS_I_AXIS
  #define I_CENTER _I_HALF_BED
#endif
#if HAS_J_AXIS
  #define J_CENTER _J_HALF_BED
#endif
#if HAS_K_AXIS
  #define K_CENTER _K_HALF_BED
#endif
#if HAS_U_AXIS
  #define U_CENTER _U_HALF_BED
#endif
#if HAS_V_AXIS
  #define V_CENTER _V_HALF_BED
#endif
#if HAS_W_AXIS
  #define W_CENTER _W_HALF_BED
#endif

// Get the linear boundaries of the bed
#define X_MIN_BED (X_CENTER - _X_HALF_BED)
#define X_MAX_BED (X_MIN_BED + X_BED_SIZE)
#if HAS_Y_AXIS
  #define Y_MIN_BED (Y_CENTER - _Y_HALF_BED)
  #define Y_MAX_BED (Y_MIN_BED + Y_BED_SIZE)
#endif
#if HAS_I_AXIS
  #define I_MINIM (I_CENTER - _I_HALF_BED_SIZE)
  #define I_MAXIM (I_MINIM + I_BED_SIZE)
#endif
#if HAS_J_AXIS
  #define J_MINIM (J_CENTER - _J_HALF_BED_SIZE)
  #define J_MAXIM (J_MINIM + J_BED_SIZE)
#endif
#if HAS_K_AXIS
  #define K_MINIM (K_CENTER - _K_HALF_BED_SIZE)
  #define K_MAXIM (K_MINIM + K_BED_SIZE)
#endif
#if HAS_U_AXIS
  #define U_MINIM (U_CENTER - _U_HALF_BED_SIZE)
  #define U_MAXIM (U_MINIM + U_BED_SIZE)
#endif
#if HAS_V_AXIS
  #define V_MINIM (V_CENTER - _V_HALF_BED_SIZE)
  #define V_MAXIM (V_MINIM + V_BED_SIZE)
#endif
#if HAS_W_AXIS
  #define W_MINIM (W_CENTER - _W_HALF_BED_SIZE)
  #define W_MAXIM (W_MINIM + W_BED_SIZE)
#endif

/**
 * Dual X Carriage
 */
#if ENABLED(DUAL_X_CARRIAGE)
  #ifndef X1_MIN_POS
    #define X1_MIN_POS X_MIN_POS
  #endif
  #ifndef X1_MAX_POS
    #define X1_MAX_POS X_BED_SIZE
  #endif
#endif

// Calibration codes only for non-core axes
#if EITHER(BACKLASH_GCODE, CALIBRATION_GCODE)
  #if ANY(IS_CORE, MARKFORGED_XY, MARKFORGED_YX)
    #define CAN_CALIBRATE(A,B) (_AXIS(A) == B)
  #else
    #define CAN_CALIBRATE(A,B) true
  #endif
#endif
#define AXIS_CAN_CALIBRATE(A) CAN_CALIBRATE(A,NORMAL_AXIS)

/**
 * No adjustable bed on non-cartesians
 */
#if IS_KINEMATIC
  #undef LCD_BED_TRAMMING
#endif

/**
 * SCARA cannot use SLOWDOWN and requires QUICKHOME
 * Printable radius assumes joints can fully extend
 */
#if IS_SCARA
  #undef SLOWDOWN
  #if ENABLED(AXEL_TPARA)
    #define SCARA_PRINTABLE_RADIUS (TPARA_LINKAGE_1 + TPARA_LINKAGE_2)
  #else
    #define QUICK_HOME
    #define SCARA_PRINTABLE_RADIUS (SCARA_LINKAGE_1 + SCARA_LINKAGE_2)
  #endif
#endif

/**
 * Set the home position based on settings or manual overrides
 */
#ifdef MANUAL_X_HOME_POS
  #define X_HOME_POS MANUAL_X_HOME_POS
#else
  #define X_END_POS TERN(X_HOME_TO_MIN, X_MIN_POS, X_MAX_POS)
  #if ENABLED(BED_CENTER_AT_0_0)
    #define X_HOME_POS TERN(DELTA, 0, X_END_POS)
  #else
    #define X_HOME_POS TERN(DELTA, X_MIN_POS + (X_BED_SIZE) * 0.5, X_END_POS)
  #endif
#endif

#if HAS_Y_AXIS
  #ifdef MANUAL_Y_HOME_POS
    #define Y_HOME_POS MANUAL_Y_HOME_POS
  #else
    #define Y_END_POS TERN(Y_HOME_TO_MIN, Y_MIN_POS, Y_MAX_POS)
    #if ENABLED(BED_CENTER_AT_0_0)
      #define Y_HOME_POS TERN(DELTA, 0, Y_END_POS)
    #else
      #define Y_HOME_POS TERN(DELTA, Y_MIN_POS + (Y_BED_SIZE) * 0.5, Y_END_POS)
    #endif
  #endif
#endif

#ifdef MANUAL_Z_HOME_POS
  #define Z_HOME_POS MANUAL_Z_HOME_POS
#else
  #define Z_HOME_POS TERN(Z_HOME_TO_MIN, Z_MIN_POS, Z_MAX_POS)
#endif

#if HAS_I_AXIS
  #ifdef MANUAL_I_HOME_POS
    #define I_HOME_POS MANUAL_I_HOME_POS
  #else
    #define I_HOME_POS TERN(I_HOME_TO_MIN, I_MIN_POS, I_MAX_POS)
  #endif
#endif
#if HAS_J_AXIS
  #ifdef MANUAL_J_HOME_POS
    #define J_HOME_POS MANUAL_J_HOME_POS
  #else
    #define J_HOME_POS TERN(J_HOME_TO_MIN, J_MIN_POS, J_MAX_POS)
  #endif
#endif
#if HAS_K_AXIS
  #ifdef MANUAL_K_HOME_POS
    #define K_HOME_POS MANUAL_K_HOME_POS
  #else
    #define K_HOME_POS TERN(K_HOME_TO_MIN, K_MIN_POS, K_MAX_POS)
  #endif
#endif
#if HAS_U_AXIS
  #ifdef MANUAL_U_HOME_POS
    #define U_HOME_POS MANUAL_U_HOME_POS
  #else
    #define U_HOME_POS TERN(U_HOME_TO_MIN, U_MIN_POS, U_MAX_POS)
  #endif
#endif
#if HAS_V_AXIS
  #ifdef MANUAL_V_HOME_POS
    #define V_HOME_POS MANUAL_V_HOME_POS
  #else
    #define V_HOME_POS TERN(V_HOME_TO_MIN, V_MIN_POS, V_MAX_POS)
  #endif
#endif
#if HAS_W_AXIS
  #ifdef MANUAL_W_HOME_POS
    #define W_HOME_POS MANUAL_W_HOME_POS
  #else
    #define W_HOME_POS TERN(W_HOME_TO_MIN, W_MIN_POS, W_MAX_POS)
  #endif
#endif

/**
 * If DELTA_HEIGHT isn't defined use the old setting
 */
#if ENABLED(DELTA) && !defined(DELTA_HEIGHT)
  #define DELTA_HEIGHT Z_HOME_POS
#endif

/**
 * Z Sled Probe requires Z_SAFE_HOMING
 */
#if ENABLED(Z_PROBE_SLED)
  #define Z_SAFE_HOMING
#endif

/**
 * DELTA should ignore Z_SAFE_HOMING and SLOWDOWN
 */
#if ENABLED(DELTA)
  #undef Z_SAFE_HOMING
  #undef SLOWDOWN
#endif

#ifndef MESH_INSET
  #define MESH_INSET 0
#endif

/**
 * Safe Homing Options
 */
#if ENABLED(Z_SAFE_HOMING)
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    // Home close to center so grid points have z heights very close to 0
    #define _SAFE_POINT(A) (((GRID_MAX_POINTS_##A) / 2) * (A##_BED_SIZE - 2 * (MESH_INSET)) / (GRID_MAX_POINTS_##A - 1) + MESH_INSET)
  #else
    #define _SAFE_POINT(A) A##_CENTER
  #endif
  #ifndef Z_SAFE_HOMING_X_POINT
    #define Z_SAFE_HOMING_X_POINT _SAFE_POINT(X)
  #endif
  #ifndef Z_SAFE_HOMING_Y_POINT
    #define Z_SAFE_HOMING_Y_POINT _SAFE_POINT(Y)
  #endif
#endif

#ifdef GRID_MAX_POINTS_X
  #define GRID_MAX_CELLS_X (GRID_MAX_POINTS_X - 1)
  #define GRID_MAX_CELLS_Y (GRID_MAX_POINTS_Y - 1)
#endif

/**
 * Host keep alive
 */
#ifndef DEFAULT_KEEPALIVE_INTERVAL
  #define DEFAULT_KEEPALIVE_INTERVAL 2
#endif

/**
 * Provide a MAX_AUTORETRACT for older configs
 */
#if ENABLED(FWRETRACT) && !defined(MAX_AUTORETRACT)
  #define MAX_AUTORETRACT 99
#endif

/**
 * Provide a DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT in case NO_VOLUMETRICS is enabled
 */
#ifndef DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT
  #define DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT 0.00
#endif

/**
 * LCD Contrast for Graphical Displays
 */
#if ENABLED(CARTESIO_UI)
  #define _LCD_CONTRAST_MIN   60
  #define _LCD_CONTRAST_INIT  90
  #define _LCD_CONTRAST_MAX  140
#elif ENABLED(miniVIKI)
  #define _LCD_CONTRAST_MIN   75
  #define _LCD_CONTRAST_INIT  95
  #define _LCD_CONTRAST_MAX  115
#elif ENABLED(VIKI2)
  #define _LCD_CONTRAST_INIT 140
#elif ENABLED(WYH_L12864)
  #define _LCD_CONTRAST_INIT 190
#elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
  #define _LCD_CONTRAST_MIN   90
  #define _LCD_CONTRAST_INIT 110
  #define _LCD_CONTRAST_MAX  130
#elif ENABLED(AZSMZ_12864)
  #define _LCD_CONTRAST_MIN  120
  #define _LCD_CONTRAST_INIT 190
#elif EITHER(MKS_LCD12864A, MKS_LCD12864B)
  #define _LCD_CONTRAST_MIN  120
  #define _LCD_CONTRAST_INIT 205
#elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)
  #define _LCD_CONTRAST_MIN  120
  #define _LCD_CONTRAST_INIT 195
#elif EITHER(MKS_MINI_12864_V3, BTT_MINI_12864_V1)
  #define _LCD_CONTRAST_MIN  255
  #define _LCD_CONTRAST_INIT 255
#elif ENABLED(FYSETC_MINI_12864)
  #define _LCD_CONTRAST_MIN  180
  #define _LCD_CONTRAST_INIT 220
#elif ENABLED(ULTI_CONTROLLER)
  #define _LCD_CONTRAST_INIT 127
  #define _LCD_CONTRAST_MAX  254
#elif ENABLED(MAKRPANEL)
  #define _LCD_CONTRAST_INIT  17
#elif ENABLED(MINIPANEL)
  #define _LCD_CONTRAST_INIT 150
#elif ENABLED(ZONESTAR_12864OLED)
  #define _LCD_CONTRAST_MIN   64
  #define _LCD_CONTRAST_INIT 128
#elif ENABLED(EMOTION_TECH_LCD)
  #define _LCD_CONTRAST_INIT 140
#elif IS_TFTGLCD_PANEL
  #define _LCD_CONTRAST_INIT 250
#endif

#ifdef _LCD_CONTRAST_INIT
  #define HAS_LCD_CONTRAST 1
  #ifndef LCD_CONTRAST_MIN
    #ifdef _LCD_CONTRAST_MIN
      #define LCD_CONTRAST_MIN _LCD_CONTRAST_MIN
    #else
      #define LCD_CONTRAST_MIN 0
    #endif
  #endif
  #ifndef LCD_CONTRAST_INIT
    #define LCD_CONTRAST_INIT _LCD_CONTRAST_INIT
  #endif
  #ifndef LCD_CONTRAST_MAX
    #ifdef _LCD_CONTRAST_MAX
      #define LCD_CONTRAST_MAX _LCD_CONTRAST_MAX
    #elif _LCD_CONTRAST_INIT > 63
      #define LCD_CONTRAST_MAX 255
    #else
      #define LCD_CONTRAST_MAX 63   // ST7567 6-bits contrast
    #endif
  #endif
  #ifndef LCD_CONTRAST_DEFAULT
    #define LCD_CONTRAST_DEFAULT LCD_CONTRAST_INIT
  #endif
#endif

/**
 * Override the SD_DETECT_STATE set in Configuration_adv.h
 * and enable sharing of onboard SD host drives (all platforms but AGCM4)
 */
#if ENABLED(SDSUPPORT)

  #if HAS_SD_HOST_DRIVE && SD_CONNECTION_IS(ONBOARD) && DISABLED(KEEP_SD_DETECT)
    //
    // The external SD card is not used. Hardware SPI is used to access the card.
    // When sharing the SD card with a PC we want the menu options to
    // mount/unmount the card and refresh it. So we disable card detect.
    //
    #undef SD_DETECT_PIN
    #define HAS_SHARED_MEDIA 1
  #endif

  // Extender cable doesn't support SD_DETECT_PIN
  #if ENABLED(NO_SD_DETECT)
    #undef SD_DETECT_PIN
  #endif

  // Not onboard or custom cable
  #if SD_CONNECTION_IS(LCD) || !defined(SDCARD_CONNECTION)
    #define SD_CONNECTION_TYPICAL 1
  #endif

  // Set SD_DETECT_STATE based on hardware if not overridden
  #if PIN_EXISTS(SD_DETECT)
    #define HAS_SD_DETECT 1
    #ifndef SD_DETECT_STATE
      #if ALL(SD_CONNECTION_TYPICAL, HAS_MARLINUI_MENU, ELB_FULL_GRAPHIC_CONTROLLER)
        #define SD_DETECT_STATE HIGH
      #else
        #define SD_DETECT_STATE LOW
      #endif
    #endif
  #endif

  #if DISABLED(USB_FLASH_DRIVE_SUPPORT) || BOTH(MULTI_VOLUME, VOLUME_SD_ONBOARD)
    #if ENABLED(SDIO_SUPPORT)
      #define NEED_SD2CARD_SDIO 1
    #else
      #define NEED_SD2CARD_SPI 1
    #endif
  #endif

  #if HAS_SD_DETECT && NONE(HAS_GRAPHICAL_TFT, LCD_USE_DMA_FSMC, HAS_FSMC_GRAPHICAL_TFT, HAS_SPI_GRAPHICAL_TFT, IS_DWIN_MARLINUI, EXTENSIBLE_UI, HAS_DWIN_E3V2)
    #define REINIT_NOISY_LCD 1  // Have the LCD re-init on SD insertion
  #endif

#endif

/**
 * Power Supply
 */
#ifndef PSU_NAME
  #if DISABLED(PSU_CONTROL)
    #define PSU_NAME "Generic"  // No control
  #elif PSU_ACTIVE_STATE
    #define PSU_NAME "XBox"     // X-Box 360 (203W)
  #else
    #define PSU_NAME "ATX"      // ATX style
  #endif
#endif

#if ENABLED(PSU_CONTROL)
  #ifndef PSU_POWERUP_DELAY
    #define PSU_POWERUP_DELAY 250
  #endif
  #ifndef POWER_OFF_DELAY
    #define POWER_OFF_DELAY   0
  #endif
#endif

/**
 * Temp Sensor defines; set up pins as needed.
 */

// Usurp a sensor to do redundant readings
#if TEMP_SENSOR_REDUNDANT
  #ifndef TEMP_SENSOR_REDUNDANT_SOURCE
    #define TEMP_SENSOR_REDUNDANT_SOURCE E1
  #endif
  #ifndef TEMP_SENSOR_REDUNDANT_TARGET
    #define TEMP_SENSOR_REDUNDANT_TARGET E0
  #endif
  #if !PIN_EXISTS(TEMP_REDUNDANT)
    #ifndef TEMP_SENSOR_REDUNDANT_MAX_DIFF
      #define TEMP_SENSOR_REDUNDANT_MAX_DIFF 10
    #endif
    #if REDUNDANT_TEMP_MATCH(SOURCE, BOARD)
      #if !PIN_EXISTS(TEMP_BOARD)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to BOARD requires TEMP_BOARD_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_BOARD_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, COOLER)
      #if !PIN_EXISTS(TEMP_COOLER)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to COOLER requires TEMP_COOLER_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_COOLER_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, PROBE)
      #if !PIN_EXISTS(TEMP_PROBE)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to PROBE requires TEMP_PROBE_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_PROBE_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, CHAMBER)
      #if !PIN_EXISTS(TEMP_CHAMBER)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to CHAMBER requires TEMP_CHAMBER_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_CHAMBER_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, BED)
      #if !PIN_EXISTS(TEMP_BED)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to BED requires TEMP_BED_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_BED_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E0)
      #if !PIN_EXISTS(TEMP_0)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E0 requires TEMP_0_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_0_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E1)
      #if !PIN_EXISTS(TEMP_1)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E1 requires TEMP_1_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_1_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E2)
      #if !PIN_EXISTS(TEMP_2)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E2 requires TEMP_2_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_2_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E3)
      #if !PIN_EXISTS(TEMP_3)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E3 requires TEMP_3_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_3_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E4)
      #if !PIN_EXISTS(TEMP_4)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E4 requires TEMP_4_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_4_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E5)
      #if !PIN_EXISTS(TEMP_5)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E5 requires TEMP_5_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_5_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E6)
      #if !PIN_EXISTS(TEMP_6)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E6 requires TEMP_6_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_6_PIN
      #endif
    #elif REDUNDANT_TEMP_MATCH(SOURCE, E7)
      #if !PIN_EXISTS(TEMP_7)
        #error "TEMP_SENSOR_REDUNDANT_SOURCE set to E7 requires TEMP_7_PIN."
      #else
        #define TEMP_REDUNDANT_PIN TEMP_7_PIN
      #endif
    #endif
  #endif
#endif

/**
 * Compatibility layer for MAX (SPI) temp boards
 */
#if HAS_MAX_TC

  // Translate old _SS, _CS, _SCK, _DO, _DI, _MISO, and _MOSI PIN defines.
  #if TEMP_SENSOR_IS_MAX_TC(0) || (TEMP_SENSOR_IS_MAX_TC(REDUNDANT) && REDUNDANT_TEMP_MATCH(SOURCE, E1))

    #if !PIN_EXISTS(TEMP_0_CS) // SS, CS
      #if PIN_EXISTS(MAX6675_SS)
        #define TEMP_0_CS_PIN MAX6675_SS_PIN
      #elif PIN_EXISTS(MAX6675_CS)
        #define TEMP_0_CS_PIN MAX6675_CS_PIN
      #elif PIN_EXISTS(MAX31855_SS)
        #define TEMP_0_CS_PIN MAX31855_SS_PIN
      #elif PIN_EXISTS(MAX31855_CS)
        #define TEMP_0_CS_PIN MAX31855_CS_PIN
      #elif PIN_EXISTS(MAX31865_SS)
        #define TEMP_0_CS_PIN MAX31865_SS_PIN
      #elif PIN_EXISTS(MAX31865_CS)
        #define TEMP_0_CS_PIN MAX31865_CS_PIN
      #endif
    #endif

    #if TEMP_SENSOR_0_IS_MAX6675
      #if !PIN_EXISTS(TEMP_0_MISO) // DO
        #if PIN_EXISTS(MAX6675_MISO)
          #define TEMP_0_MISO_PIN MAX6675_MISO_PIN
        #elif PIN_EXISTS(MAX6675_DO)
          #define TEMP_0_MISO_PIN MAX6675_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_0_SCK) && PIN_EXISTS(MAX6675_SCK)
        #define TEMP_0_SCK_PIN MAX6675_SCK_PIN
      #endif

    #elif TEMP_SENSOR_0_IS_MAX31855
      #if !PIN_EXISTS(TEMP_0_MISO) // DO
        #if PIN_EXISTS(MAX31855_MISO)
          #define TEMP_0_MISO_PIN MAX31855_MISO_PIN
        #elif PIN_EXISTS(MAX31855_DO)
          #define TEMP_0_MISO_PIN MAX31855_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_0_SCK) && PIN_EXISTS(MAX31855_SCK)
        #define TEMP_0_SCK_PIN MAX31855_SCK_PIN
      #endif

    #elif TEMP_SENSOR_0_IS_MAX31865
      #if !PIN_EXISTS(TEMP_0_MISO) // DO
        #if PIN_EXISTS(MAX31865_MISO)
          #define TEMP_0_MISO_PIN MAX31865_MISO_PIN
        #elif PIN_EXISTS(MAX31865_DO)
          #define TEMP_0_MISO_PIN MAX31865_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_0_SCK) && PIN_EXISTS(MAX31865_SCK)
        #define TEMP_0_SCK_PIN MAX31865_SCK_PIN
      #endif
      #if !PIN_EXISTS(TEMP_0_MOSI) && PIN_EXISTS(MAX31865_MOSI) // MOSI for '65 only
        #define TEMP_0_MOSI_PIN MAX31865_MOSI_PIN
      #endif
    #endif

    // Software SPI - enable if MISO/SCK are defined.
    #if PIN_EXISTS(TEMP_0_MISO) && PIN_EXISTS(TEMP_0_SCK) && DISABLED(TEMP_SENSOR_0_FORCE_HW_SPI)
      #if TEMP_SENSOR_0_IS_MAX31865 && !PIN_EXISTS(TEMP_0_MOSI)
        #error "TEMP_SENSOR_0 MAX31865 requires TEMP_0_MOSI_PIN defined for Software SPI. To use Hardware SPI instead, undefine MISO/SCK or enable TEMP_SENSOR_0_FORCE_HW_SPI."
      #else
        #define TEMP_SENSOR_0_HAS_SPI_PINS 1
      #endif
    #endif

  #endif // TEMP_SENSOR_IS_MAX_TC(0)

  #if TEMP_SENSOR_IS_MAX_TC(1) || (TEMP_SENSOR_IS_MAX_TC(REDUNDANT) && REDUNDANT_TEMP_MATCH(SOURCE, E1))

    #if !PIN_EXISTS(TEMP_1_CS) // SS2, CS2
      #if PIN_EXISTS(MAX6675_SS2)
        #define TEMP_1_CS_PIN MAX6675_SS2_PIN
      #elif PIN_EXISTS(MAX6675_CS)
        #define TEMP_1_CS_PIN MAX6675_CS2_PIN
      #elif PIN_EXISTS(MAX31855_SS2)
        #define TEMP_1_CS_PIN MAX31855_SS2_PIN
      #elif PIN_EXISTS(MAX31855_CS2)
        #define TEMP_1_CS_PIN MAX31855_CS2_PIN
      #elif PIN_EXISTS(MAX31865_SS2)
        #define TEMP_1_CS_PIN MAX31865_SS2_PIN
      #elif PIN_EXISTS(MAX31865_CS2)
        #define TEMP_1_CS_PIN MAX31865_CS2_PIN
      #endif
    #endif

    #if TEMP_SENSOR_1_IS_MAX6675
      #if !PIN_EXISTS(TEMP_1_MISO) // DO
        #if PIN_EXISTS(MAX6675_MISO)
          #define TEMP_1_MISO_PIN MAX6675_MISO_PIN
        #elif PIN_EXISTS(MAX6675_DO)
          #define TEMP_1_MISO_PIN MAX6675_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_1_SCK) && PIN_EXISTS(MAX6675_SCK)
        #define TEMP_1_SCK_PIN MAX6675_SCK_PIN
      #endif

    #elif TEMP_SENSOR_1_IS_MAX31855
      #if !PIN_EXISTS(TEMP_1_MISO) // DO
        #if PIN_EXISTS(MAX31855_MISO)
          #define TEMP_1_MISO_PIN MAX31855_MISO_PIN
        #elif PIN_EXISTS(MAX31855_DO)
          #define TEMP_1_MISO_PIN MAX31855_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_1_SCK) && PIN_EXISTS(MAX31855_SCK)
        #define TEMP_1_SCK_PIN MAX31855_SCK_PIN
      #endif

    #elif TEMP_SENSOR_1_IS_MAX31865
      #if !PIN_EXISTS(TEMP_1_MISO) // DO
        #if PIN_EXISTS(MAX31865_MISO)
          #define TEMP_1_MISO_PIN MAX31865_MISO_PIN
        #elif PIN_EXISTS(MAX31865_DO)
          #define TEMP_1_MISO_PIN MAX31865_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_1_SCK) && PIN_EXISTS(MAX31865_SCK)
        #define TEMP_1_SCK_PIN MAX31865_SCK_PIN
      #endif
      #if !PIN_EXISTS(TEMP_1_MOSI) && PIN_EXISTS(MAX31865_MOSI) // MOSI for '65 only
        #define TEMP_1_MOSI_PIN MAX31865_MOSI_PIN
      #endif
    #endif

    // Software SPI - enable if MISO/SCK are defined.
    #if PIN_EXISTS(TEMP_1_MISO) && PIN_EXISTS(TEMP_1_SCK) && DISABLED(TEMP_SENSOR_1_FORCE_HW_SPI)
      #if TEMP_SENSOR_1_IS_MAX31865 && !PIN_EXISTS(TEMP_1_MOSI)
        #error "TEMP_SENSOR_1 MAX31865 requires TEMP_1_MOSI_PIN defined for Software SPI. To use Hardware SPI instead, undefine MISO/SCK or enable TEMP_SENSOR_1_FORCE_HW_SPI."
      #else
        #define TEMP_SENSOR_1_HAS_SPI_PINS 1
      #endif
    #endif

  #endif // TEMP_SENSOR_IS_MAX_TC(1)

  #if TEMP_SENSOR_IS_MAX_TC(2) || (TEMP_SENSOR_IS_MAX_TC(REDUNDANT) && REDUNDANT_TEMP_MATCH(SOURCE, E2))

    #if !PIN_EXISTS(TEMP_2_CS) // SS3, CS3
      #if PIN_EXISTS(MAX6675_SS3)
        #define TEMP_2_CS_PIN MAX6675_SS3_PIN
      #elif PIN_EXISTS(MAX6675_CS)
        #define TEMP_2_CS_PIN MAX6675_CS3_PIN
      #elif PIN_EXISTS(MAX31855_SS3)
        #define TEMP_2_CS_PIN MAX31855_SS3_PIN
      #elif PIN_EXISTS(MAX31855_CS3)
        #define TEMP_2_CS_PIN MAX31855_CS3_PIN
      #elif PIN_EXISTS(MAX31865_SS3)
        #define TEMP_2_CS_PIN MAX31865_SS3_PIN
      #elif PIN_EXISTS(MAX31865_CS3)
        #define TEMP_2_CS_PIN MAX31865_CS3_PIN
      #endif
    #endif

    #if TEMP_SENSOR_2_IS_MAX6675
      #if !PIN_EXISTS(TEMP_2_MISO) // DO
        #if PIN_EXISTS(MAX6675_MISO)
          #define TEMP_2_MISO_PIN MAX6675_MISO_PIN
        #elif PIN_EXISTS(MAX6675_DO)
          #define TEMP_2_MISO_PIN MAX6675_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_2_SCK) && PIN_EXISTS(MAX6675_SCK)
        #define TEMP_2_SCK_PIN MAX6675_SCK_PIN
      #endif

    #elif TEMP_SENSOR_2_IS_MAX31855
      #if !PIN_EXISTS(TEMP_2_MISO) // DO
        #if PIN_EXISTS(MAX31855_MISO)
          #define TEMP_2_MISO_PIN MAX31855_MISO_PIN
        #elif PIN_EXISTS(MAX31855_DO)
          #define TEMP_2_MISO_PIN MAX31855_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_2_SCK) && PIN_EXISTS(MAX31855_SCK)
        #define TEMP_2_SCK_PIN MAX31855_SCK_PIN
      #endif

    #elif TEMP_SENSOR_2_IS_MAX31865
      #if !PIN_EXISTS(TEMP_2_MISO) // DO
        #if PIN_EXISTS(MAX31865_MISO)
          #define TEMP_2_MISO_PIN MAX31865_MISO_PIN
        #elif PIN_EXISTS(MAX31865_DO)
          #define TEMP_2_MISO_PIN MAX31865_DO_PIN
        #endif
      #endif
      #if !PIN_EXISTS(TEMP_2_SCK) && PIN_EXISTS(MAX31865_SCK)
        #define TEMP_2_SCK_PIN MAX31865_SCK_PIN
      #endif
      #if !PIN_EXISTS(TEMP_2_MOSI) && PIN_EXISTS(MAX31865_MOSI) // MOSI for '65 only
        #define TEMP_2_MOSI_PIN MAX31865_MOSI_PIN
      #endif
    #endif

    // Software SPI - enable if MISO/SCK are defined.
    #if PIN_EXISTS(TEMP_2_MISO) && PIN_EXISTS(TEMP_2_SCK) && DISABLED(TEMP_SENSOR_2_FORCE_HW_SPI)
      #if TEMP_SENSOR_2_IS_MAX31865 && !PIN_EXISTS(TEMP_2_MOSI)
        #error "TEMP_SENSOR_2 MAX31865 requires TEMP_2_MOSI_PIN defined for Software SPI. To use Hardware SPI instead, undefine MISO/SCK or enable TEMP_SENSOR_2_FORCE_HW_SPI."
      #else
        #define TEMP_SENSOR_2_HAS_SPI_PINS 1
      #endif
    #endif

  #endif // TEMP_SENSOR_IS_MAX_TC(2)

  //
  // User-defined thermocouple libraries
  //
  // Add LIB_MAX6675 / LIB_MAX31855 / LIB_MAX31865 to the build_flags
  // to select a USER library for MAX6675, MAX31855, MAX31865
  //
  #if BOTH(HAS_MAX6675, LIB_MAX6675)
    #define USE_LIB_MAX6675 1
  #endif
  #if BOTH(HAS_MAX31855, LIB_MAX31855)
    #define USE_ADAFRUIT_MAX31855 1
  #endif
  #if BOTH(HAS_MAX31865, LIB_MAX31865)
    #define USE_ADAFRUIT_MAX31865 1
  #elif HAS_MAX31865
    #define LIB_INTERNAL_MAX31865 1
  #endif

#endif // HAS_MAX_TC

/**
 * X_DUAL_ENDSTOPS endstop reassignment
 */
#if ENABLED(X_DUAL_ENDSTOPS)
  #if X_HOME_TO_MAX
    #ifndef X2_MAX_ENDSTOP_INVERTING
      #if X2_USE_ENDSTOP == _XMIN_
        #define X2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _XMAX_
        #define X2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _YMIN_
        #define X2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _YMAX_
        #define X2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _ZMIN_
        #define X2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _ZMAX_
        #define X2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
      #endif
    #endif
    #if !PIN_EXISTS(X2_MAX)
      #undef X2_MAX_PIN
      #if PIN_EXISTS(X2_STOP)
        #define X2_MAX_PIN X2_STOP_PIN
      #endif
    #endif
  #else
    #ifndef X2_MIN_ENDSTOP_INVERTING
      #if X2_USE_ENDSTOP == _XMIN_
        #define X2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _XMAX_
        #define X2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _YMIN_
        #define X2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _YMAX_
        #define X2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _ZMIN_
        #define X2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
      #elif X2_USE_ENDSTOP == _ZMAX_
        #define X2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
      #endif
    #endif
    #if !PIN_EXISTS(X2_MIN)
      #undef X2_MIN_PIN
      #if PIN_EXISTS(X2_STOP)
        #define X2_MIN_PIN X2_STOP_PIN
      #endif
    #endif
  #endif
  #ifndef X2_MAX_ENDSTOP_INVERTING
    #define X2_MAX_ENDSTOP_INVERTING false
  #endif
  #ifndef X2_MIN_ENDSTOP_INVERTING
    #define X2_MIN_ENDSTOP_INVERTING false
  #endif
#endif

/**
 * Y_DUAL_ENDSTOPS endstop reassignment
 */
#if ENABLED(Y_DUAL_ENDSTOPS)
  #if Y_HOME_TO_MAX
    #ifndef Y2_MAX_ENDSTOP_INVERTING
      #if Y2_USE_ENDSTOP == _XMIN_
        #define Y2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _XMAX_
        #define Y2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _YMIN_
        #define Y2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _YMAX_
        #define Y2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _ZMIN_
        #define Y2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _ZMAX_
        #define Y2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
      #endif
    #endif
    #if !PIN_EXISTS(Y2_MAX)
      #undef Y2_MAX_PIN
      #if PIN_EXISTS(Y2_STOP)
        #define Y2_MAX_PIN Y2_STOP_PIN
      #endif
    #endif
  #else
    #ifndef Y2_MIN_ENDSTOP_INVERTING
      #if Y2_USE_ENDSTOP == _XMIN_
        #define Y2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _XMAX_
        #define Y2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _YMIN_
        #define Y2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _YMAX_
        #define Y2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _ZMIN_
        #define Y2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
      #elif Y2_USE_ENDSTOP == _ZMAX_
        #define Y2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
      #endif
    #endif
    #if !PIN_EXISTS(Y2_MIN)
      #undef Y2_MIN_PIN
      #if PIN_EXISTS(Y2_STOP)
        #define Y2_MIN_PIN Y2_STOP_PIN
      #endif
    #endif
  #endif
  #ifndef Y2_MAX_ENDSTOP_INVERTING
    #define Y2_MAX_ENDSTOP_INVERTING false
  #endif
  #ifndef Y2_MIN_ENDSTOP_INVERTING
    #define Y2_MIN_ENDSTOP_INVERTING false
  #endif
#endif

/**
 * Z_MULTI_ENDSTOPS endstop reassignment
 */
#if ENABLED(Z_MULTI_ENDSTOPS)

  #if Z_HOME_TO_MAX
    #ifndef Z2_MAX_ENDSTOP_INVERTING
      #if Z2_USE_ENDSTOP == _XMIN_
        #define Z2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _XMAX_
        #define Z2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _YMIN_
        #define Z2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _YMAX_
        #define Z2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _ZMIN_
        #define Z2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _ZMAX_
        #define Z2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
      #endif
    #endif
    #if !PIN_EXISTS(Z2_MAX)
      #undef Z2_MAX_PIN
      #if PIN_EXISTS(Z2_STOP)
        #define Z2_MAX_PIN Z2_STOP_PIN
      #endif
    #endif
  #else
    #ifndef Z2_MIN_ENDSTOP_INVERTING
      #if Z2_USE_ENDSTOP == _XMIN_
        #define Z2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _XMAX_
        #define Z2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _YMIN_
        #define Z2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _YMAX_
        #define Z2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _ZMIN_
        #define Z2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
      #elif Z2_USE_ENDSTOP == _ZMAX_
        #define Z2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
      #endif
    #endif
    #if !PIN_EXISTS(Z2_MIN)
      #undef Z2_MIN_PIN
      #if PIN_EXISTS(Z2_STOP)
        #define Z2_MIN_PIN Z2_STOP_PIN
      #endif
    #endif
  #endif
  #ifndef Z2_MAX_ENDSTOP_INVERTING
    #define Z2_MAX_ENDSTOP_INVERTING false
  #endif
  #ifndef Z2_MIN_ENDSTOP_INVERTING
    #define Z2_MIN_ENDSTOP_INVERTING false
  #endif

  #if NUM_Z_STEPPERS >= 3
    #if Z_HOME_TO_MAX
      #ifndef Z3_MAX_ENDSTOP_INVERTING
        #if Z3_USE_ENDSTOP == _XMIN_
          #define Z3_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _XMAX_
          #define Z3_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _YMIN_
          #define Z3_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _YMAX_
          #define Z3_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _ZMIN_
          #define Z3_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _ZMAX_
          #define Z3_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #endif
      #endif
      #if !PIN_EXISTS(Z3_MAX)
        #undef Z3_MAX_PIN
        #if PIN_EXISTS(Z3_STOP)
          #define Z3_MAX_PIN Z3_STOP_PIN
        #endif
      #endif
    #else
      #ifndef Z3_MIN_ENDSTOP_INVERTING
        #if Z3_USE_ENDSTOP == _XMIN_
          #define Z3_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _XMAX_
          #define Z3_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _YMIN_
          #define Z3_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _YMAX_
          #define Z3_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _ZMIN_
          #define Z3_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #elif Z3_USE_ENDSTOP == _ZMAX_
          #define Z3_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #endif
      #endif
      #if !PIN_EXISTS(Z3_MIN)
        #undef Z3_MIN_PIN
        #if PIN_EXISTS(Z3_STOP)
          #define Z3_MIN_PIN Z3_STOP_PIN
        #endif
      #endif
    #endif
    #ifndef Z3_MAX_ENDSTOP_INVERTING
      #define Z3_MAX_ENDSTOP_INVERTING false
    #endif
    #ifndef Z3_MIN_ENDSTOP_INVERTING
      #define Z3_MIN_ENDSTOP_INVERTING false
    #endif
  #endif

  #if NUM_Z_STEPPERS >= 4
    #if Z_HOME_TO_MAX
      #ifndef Z4_MAX_ENDSTOP_INVERTING
        #if Z4_USE_ENDSTOP == _XMIN_
          #define Z4_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _XMAX_
          #define Z4_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _YMIN_
          #define Z4_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _YMAX_
          #define Z4_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _ZMIN_
          #define Z4_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _ZMAX_
          #define Z4_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #endif
      #endif
      #if !PIN_EXISTS(Z4_MAX)
        #undef Z4_MAX_PIN
        #if PIN_EXISTS(Z4_STOP)
          #define Z4_MAX_PIN Z4_STOP_PIN
        #endif
      #endif
    #else
      #ifndef Z4_MIN_ENDSTOP_INVERTING
        #if Z4_USE_ENDSTOP == _XMIN_
          #define Z4_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _XMAX_
          #define Z4_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _YMIN_
          #define Z4_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _YMAX_
          #define Z4_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _ZMIN_
          #define Z4_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #elif Z4_USE_ENDSTOP == _ZMAX_
          #define Z4_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #endif
      #endif
      #if !PIN_EXISTS(Z4_MIN)
        #undef Z4_MIN_PIN
        #if PIN_EXISTS(Z4_STOP)
          #define Z4_MIN_PIN Z4_STOP_PIN
        #endif
      #endif
    #endif
    #ifndef Z4_MAX_ENDSTOP_INVERTING
      #define Z4_MAX_ENDSTOP_INVERTING false
    #endif
    #ifndef Z4_MIN_ENDSTOP_INVERTING
      #define Z4_MIN_ENDSTOP_INVERTING false
    #endif
  #endif

#endif // Z_MULTI_ENDSTOPS

/**
 * Set ENDSTOPPULLUPS for active endstop switches
 */
#if ENABLED(ENDSTOPPULLUPS)
  #if ENABLED(USE_XMAX_PLUG)
    #define ENDSTOPPULLUP_XMAX
  #endif
  #if ENABLED(USE_YMAX_PLUG)
    #define ENDSTOPPULLUP_YMAX
  #endif
  #if ENABLED(USE_ZMAX_PLUG)
    #define ENDSTOPPULLUP_ZMAX
  #endif
  #if ENABLED(USE_IMAX_PLUG)
    #define ENDSTOPPULLUP_IMAX
  #endif
  #if ENABLED(USE_JMAX_PLUG)
    #define ENDSTOPPULLUP_JMAX
  #endif
  #if ENABLED(USE_KMAX_PLUG)
    #define ENDSTOPPULLUP_KMAX
  #endif
  #if ENABLED(USE_UMAX_PLUG)
    #define ENDSTOPPULLUP_UMAX
  #endif
  #if ENABLED(USE_VMAX_PLUG)
    #define ENDSTOPPULLUP_VMAX
  #endif
  #if ENABLED(USE_WMAX_PLUG)
    #define ENDSTOPPULLUP_WMAX
  #endif
  #if ENABLED(USE_XMIN_PLUG)
    #define ENDSTOPPULLUP_XMIN
  #endif
  #if ENABLED(USE_YMIN_PLUG)
    #define ENDSTOPPULLUP_YMIN
  #endif
  #if ENABLED(USE_ZMIN_PLUG)
    #define ENDSTOPPULLUP_ZMIN
  #endif
  #if ENABLED(USE_IMIN_PLUG)
    #define ENDSTOPPULLUP_IMIN
  #endif
  #if ENABLED(USE_JMIN_PLUG)
    #define ENDSTOPPULLUP_JMIN
  #endif
  #if ENABLED(USE_KMIN_PLUG)
    #define ENDSTOPPULLUP_KMIN
  #endif
  #if ENABLED(USE_UMIN_PLUG)
    #define ENDSTOPPULLUP_UMIN
  #endif
  #if ENABLED(USE_VMIN_PLUG)
    #define ENDSTOPPULLUP_VMIN
  #endif
  #if ENABLED(USE_WMIN_PLUG)
    #define ENDSTOPPULLUP_WMIN
  #endif
#endif

/**
 * Set ENDSTOPPULLDOWNS for active endstop switches
 */
#if ENABLED(ENDSTOPPULLDOWNS)
  #if ENABLED(USE_XMAX_PLUG)
    #define ENDSTOPPULLDOWN_XMAX
  #endif
  #if ENABLED(USE_YMAX_PLUG)
    #define ENDSTOPPULLDOWN_YMAX
  #endif
  #if ENABLED(USE_ZMAX_PLUG)
    #define ENDSTOPPULLDOWN_ZMAX
  #endif
  #if ENABLED(USE_IMAX_PLUG)
    #define ENDSTOPPULLDOWN_IMAX
  #endif
  #if ENABLED(USE_JMAX_PLUG)
    #define ENDSTOPPULLDOWN_JMAX
  #endif
  #if ENABLED(USE_KMAX_PLUG)
    #define ENDSTOPPULLDOWN_KMAX
  #endif
  #if ENABLED(USE_UMAX_PLUG)
    #define ENDSTOPPULLDOWN_UMAX
  #endif
  #if ENABLED(USE_VMAX_PLUG)
    #define ENDSTOPPULLDOWN_VMAX
  #endif
  #if ENABLED(USE_WMAX_PLUG)
    #define ENDSTOPPULLDOWN_WMAX
  #endif
  #if ENABLED(USE_XMIN_PLUG)
    #define ENDSTOPPULLDOWN_XMIN
  #endif
  #if ENABLED(USE_YMIN_PLUG)
    #define ENDSTOPPULLDOWN_YMIN
  #endif
  #if ENABLED(USE_ZMIN_PLUG)
    #define ENDSTOPPULLDOWN_ZMIN
  #endif
  #if ENABLED(USE_IMIN_PLUG)
    #define ENDSTOPPULLDOWN_IMIN
  #endif
  #if ENABLED(USE_JMIN_PLUG)
    #define ENDSTOPPULLDOWN_JMIN
  #endif
  #if ENABLED(USE_KMIN_PLUG)
    #define ENDSTOPPULLDOWN_KMIN
  #endif
  #if ENABLED(USE_UMIN_PLUG)
    #define ENDSTOPPULLDOWN_UMIN
  #endif
  #if ENABLED(USE_VMIN_PLUG)
    #define ENDSTOPPULLDOWN_VMIN
  #endif
  #if ENABLED(USE_WMIN_PLUG)
    #define ENDSTOPPULLDOWN_WMIN
  #endif
#endif

/**
 * Shorthand for pin tests, used wherever needed
 */

// Steppers
#if PIN_EXISTS(X_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(X))
  #define HAS_X_ENABLE 1
#endif
#if PIN_EXISTS(X_DIR)
  #define HAS_X_DIR 1
#endif
#if PIN_EXISTS(X_STEP)
  #define HAS_X_STEP 1
#endif
#if PIN_EXISTS(X_MS1)
  #define HAS_X_MS_PINS 1
#endif

#if PIN_EXISTS(X2_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(X2))
  #define HAS_X2_ENABLE 1
#endif
#if PIN_EXISTS(X2_DIR)
  #define HAS_X2_DIR 1
#endif
#if PIN_EXISTS(X2_STEP)
  #define HAS_X2_STEP 1
#endif
#if PIN_EXISTS(X2_MS1)
  #define HAS_X2_MS_PINS 1
#endif

/**
 * Set defaults for missing (newer) options
 */
#if !defined(DISABLE_INACTIVE_X) && ENABLED(DISABLE_X)
  #define DISABLE_INACTIVE_X 1
#endif

#if HAS_Y_AXIS
  #if PIN_EXISTS(Y_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Y))
    #define HAS_Y_ENABLE 1
  #endif
  #if PIN_EXISTS(Y_DIR)
    #define HAS_Y_DIR 1
  #endif
  #if PIN_EXISTS(Y_STEP)
    #define HAS_Y_STEP 1
  #endif
  #if PIN_EXISTS(Y_MS1)
    #define HAS_Y_MS_PINS 1
  #endif

  #if PIN_EXISTS(Y2_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Y2))
    #define HAS_Y2_ENABLE 1
  #endif
  #if PIN_EXISTS(Y2_DIR)
    #define HAS_Y2_DIR 1
  #endif
  #if PIN_EXISTS(Y2_STEP)
    #define HAS_Y2_STEP 1
  #endif
  #if PIN_EXISTS(Y2_MS1)
    #define HAS_Y2_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_Y) && ENABLED(DISABLE_Y)
    #define DISABLE_INACTIVE_Y 1
  #endif
#else
  #undef DISABLE_INACTIVE_Y
#endif

#if HAS_Z_AXIS
  #if PIN_EXISTS(Z_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z))
    #define HAS_Z_ENABLE 1
  #endif
  #if PIN_EXISTS(Z_DIR)
    #define HAS_Z_DIR 1
  #endif
  #if PIN_EXISTS(Z_STEP)
    #define HAS_Z_STEP 1
  #endif
  #if PIN_EXISTS(Z_MS1)
    #define HAS_Z_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_Z) && ENABLED(DISABLE_Z)
    #define DISABLE_INACTIVE_Z 1
  #endif
#else
  #undef DISABLE_INACTIVE_Z
#endif

#if NUM_Z_STEPPERS >= 2
  #if PIN_EXISTS(Z2_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z2))
    #define HAS_Z2_ENABLE 1
  #endif
  #if PIN_EXISTS(Z2_DIR)
    #define HAS_Z2_DIR 1
  #endif
  #if PIN_EXISTS(Z2_STEP)
    #define HAS_Z2_STEP 1
  #endif
  #if PIN_EXISTS(Z2_MS1)
    #define HAS_Z2_MS_PINS 1
  #endif
#endif

#if NUM_Z_STEPPERS >= 3
  #if PIN_EXISTS(Z3_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z3))
    #define HAS_Z3_ENABLE 1
  #endif
  #if PIN_EXISTS(Z3_DIR)
    #define HAS_Z3_DIR 1
  #endif
  #if PIN_EXISTS(Z3_STEP)
    #define HAS_Z3_STEP 1
  #endif
  #if PIN_EXISTS(Z3_MS1)
    #define HAS_Z3_MS_PINS 1
  #endif
#endif

#if NUM_Z_STEPPERS >= 4
  #if PIN_EXISTS(Z4_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z4))
    #define HAS_Z4_ENABLE 1
  #endif
  #if PIN_EXISTS(Z4_DIR)
    #define HAS_Z4_DIR 1
  #endif
  #if PIN_EXISTS(Z4_STEP)
    #define HAS_Z4_STEP 1
  #endif
  #if PIN_EXISTS(Z4_MS1)
    #define HAS_Z4_MS_PINS 1
  #endif
#endif

#if HAS_I_AXIS
  #if PIN_EXISTS(I_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(I))
    #define HAS_I_ENABLE 1
  #endif
  #if PIN_EXISTS(I_DIR)
    #define HAS_I_DIR 1
  #endif
  #if PIN_EXISTS(I_STEP)
    #define HAS_I_STEP 1
  #endif
  #if PIN_EXISTS(I_MS1)
    #define HAS_I_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_I) && ENABLED(DISABLE_I)
    #define DISABLE_INACTIVE_I 1
  #endif
#else
  #undef DISABLE_INACTIVE_I
#endif

#if HAS_J_AXIS
  #if PIN_EXISTS(J_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(J))
    #define HAS_J_ENABLE 1
  #endif
  #if PIN_EXISTS(J_DIR)
    #define HAS_J_DIR 1
  #endif
  #if PIN_EXISTS(J_STEP)
    #define HAS_J_STEP 1
  #endif
  #if PIN_EXISTS(J_MS1)
    #define HAS_J_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_J) && ENABLED(DISABLE_J)
    #define DISABLE_INACTIVE_J 1
  #endif
#else
  #undef DISABLE_INACTIVE_J
#endif

#if HAS_K_AXIS
  #if PIN_EXISTS(K_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(K))
    #define HAS_K_ENABLE 1
  #endif
  #if PIN_EXISTS(K_DIR)
    #define HAS_K_DIR 1
  #endif
  #if PIN_EXISTS(K_STEP)
    #define HAS_K_STEP 1
  #endif
  #if PIN_EXISTS(K_MS1)
    #define HAS_K_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_K) && ENABLED(DISABLE_K)
    #define DISABLE_INACTIVE_K 1
  #endif
#else
  #undef DISABLE_INACTIVE_K
#endif

#if HAS_U_AXIS
  #if PIN_EXISTS(U_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(U))
    #define HAS_U_ENABLE 1
  #endif
  #if PIN_EXISTS(U_DIR)
    #define HAS_U_DIR 1
  #endif
  #if PIN_EXISTS(U_STEP)
    #define HAS_U_STEP 1
  #endif
  #if PIN_EXISTS(U_MS1)
    #define HAS_U_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_U) && ENABLED(DISABLE_U)
    #define DISABLE_INACTIVE_U 1
  #endif
#else
  #undef DISABLE_INACTIVE_U
#endif

#if HAS_V_AXIS
  #if PIN_EXISTS(V_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(V))
    #define HAS_V_ENABLE 1
  #endif
  #if PIN_EXISTS(V_DIR)
    #define HAS_V_DIR 1
  #endif
  #if PIN_EXISTS(V_STEP)
    #define HAS_V_STEP 1
  #endif
  #if PIN_EXISTS(V_MS1)
    #define HAS_V_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_V) && ENABLED(DISABLE_V)
    #define DISABLE_INACTIVE_V 1
  #endif
#else
  #undef DISABLE_INACTIVE_V
#endif

#if HAS_W_AXIS
  #if PIN_EXISTS(W_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(W))
    #define HAS_W_ENABLE 1
  #endif
  #if PIN_EXISTS(W_DIR)
    #define HAS_W_DIR 1
  #endif
  #if PIN_EXISTS(W_STEP)
    #define HAS_W_STEP 1
  #endif
  #if PIN_EXISTS(W_MS1)
    #define HAS_W_MS_PINS 1
  #endif
  #if !defined(DISABLE_INACTIVE_W) && ENABLED(DISABLE_W)
    #define DISABLE_INACTIVE_W 1
  #endif
#else
  #undef DISABLE_INACTIVE_W
#endif

// Extruder steppers and solenoids
#if HAS_EXTRUDERS

  #if PIN_EXISTS(E0_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E0))
    #define HAS_E0_ENABLE 1
  #endif
  #if PIN_EXISTS(E0_DIR)
    #define HAS_E0_DIR 1
  #endif
  #if PIN_EXISTS(E0_STEP)
    #define HAS_E0_STEP 1
  #endif
  #if PIN_EXISTS(E0_MS1)
    #define HAS_E0_MS_PINS 1
  #endif

  #if E_STEPPERS > 1 || ENABLED(E_DUAL_STEPPER_DRIVERS)
    #if PIN_EXISTS(E1_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E1))
      #define HAS_E1_ENABLE 1
    #endif
    #if PIN_EXISTS(E1_DIR)
      #define HAS_E1_DIR 1
    #endif
    #if PIN_EXISTS(E1_STEP)
      #define HAS_E1_STEP 1
    #endif
    #if PIN_EXISTS(E1_MS1)
      #define HAS_E1_MS_PINS 1
    #endif
  #endif

  #if E_STEPPERS > 2
    #if PIN_EXISTS(E2_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E2))
      #define HAS_E2_ENABLE 1
    #endif
    #if PIN_EXISTS(E2_DIR)
      #define HAS_E2_DIR 1
    #endif
    #if PIN_EXISTS(E2_STEP)
      #define HAS_E2_STEP 1
    #endif
    #if PIN_EXISTS(E2_MS1)
      #define HAS_E2_MS_PINS 1
    #endif
  #endif

  #if E_STEPPERS > 3
    #if PIN_EXISTS(E3_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E3))
      #define HAS_E3_ENABLE 1
    #endif
    #if PIN_EXISTS(E3_DIR)
      #define HAS_E3_DIR 1
    #endif
    #if PIN_EXISTS(E3_STEP)
      #define HAS_E3_STEP 1
    #endif
    #if PIN_EXISTS(E3_MS1)
      #define HAS_E3_MS_PINS 1
    #endif
  #endif

  #if E_STEPPERS > 4
    #if PIN_EXISTS(E4_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E4))
      #define HAS_E4_ENABLE 1
    #endif
    #if PIN_EXISTS(E4_DIR)
      #define HAS_E4_DIR 1
    #endif
    #if PIN_EXISTS(E4_STEP)
      #define HAS_E4_STEP 1
    #endif
    #if PIN_EXISTS(E4_MS1)
      #define HAS_E4_MS_PINS 1
    #endif
  #endif

  #if E_STEPPERS > 5
    #if PIN_EXISTS(E5_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E5))
      #define HAS_E5_ENABLE 1
    #endif
    #if PIN_EXISTS(E5_DIR)
      #define HAS_E5_DIR 1
    #endif
    #if PIN_EXISTS(E5_STEP)
      #define HAS_E5_STEP 1
    #endif
    #if PIN_EXISTS(E5_MS1)
      #define HAS_E5_MS_PINS 1
    #endif
  #endif

  #if E_STEPPERS > 6
    #if PIN_EXISTS(E6_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E6))
      #define HAS_E6_ENABLE 1
    #endif
    #if PIN_EXISTS(E6_DIR)
      #define HAS_E6_DIR 1
    #endif
    #if PIN_EXISTS(E6_STEP)
      #define HAS_E6_STEP 1
    #endif
    #if PIN_EXISTS(E6_MS1)
      #define HAS_E6_MS_PINS 1
    #endif
  #endif

  #if E_STEPPERS > 7
    #if PIN_EXISTS(E7_ENABLE) || (ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E7))
      #define HAS_E7_ENABLE 1
    #endif
    #if PIN_EXISTS(E7_DIR)
      #define HAS_E7_DIR 1
    #endif
    #if PIN_EXISTS(E7_STEP)
      #define HAS_E7_STEP 1
    #endif
    #if PIN_EXISTS(E7_MS1)
      #define HAS_E7_MS_PINS 1
    #endif
  #endif

  #if !defined(DISABLE_INACTIVE_E) && ENABLED(DISABLE_E)
    #define DISABLE_INACTIVE_E 1
  #endif
#else
  #undef DISABLE_INACTIVE_E
#endif // HAS_EXTRUDERS

/**
 * Set solenoid flags if any features use solenoids
 *   - EXT_SOLENOID (M380, M381) to enable/disable the extruder solenoid
 *   - MANUAL_SOLENOID_CONTROL (M380, M381) to enable/disable solenoids by index
 *   - PARKING_EXTRUDER uses SOL0_PIN and SOL1_PIN
 *   - SOLENOID_PROBE uses SOL1_PIN
 *   - Z_PROBE_SLED uses SOL1_PIN, when defined (unless EXT_SOLENOID is enabled)
 */
#if ANY(EXT_SOLENOID, MANUAL_SOLENOID_CONTROL, PARKING_EXTRUDER, SOLENOID_PROBE, Z_PROBE_SLED)
  #if PIN_EXISTS(SOL0) && (EITHER(MANUAL_SOLENOID_CONTROL, PARKING_EXTRUDER) || BOTH(EXT_SOLENOID, HAS_EXTRUDERS))
    #define HAS_SOLENOID_0 1
  #endif
  #if PIN_EXISTS(SOL1) && (ANY(MANUAL_SOLENOID_CONTROL, PARKING_EXTRUDER, SOLENOID_PROBE, Z_PROBE_SLED) || TERN0(EXT_SOLENOID, E_STEPPERS > 1))
    #define HAS_SOLENOID_1 1
  #endif
  #if PIN_EXISTS(SOL2) && (ENABLED(MANUAL_SOLENOID_CONTROL) || TERN0(EXT_SOLENOID, E_STEPPERS > 2))
    #define HAS_SOLENOID_2 2
  #endif
  #if PIN_EXISTS(SOL3) && (ENABLED(MANUAL_SOLENOID_CONTROL) || TERN0(EXT_SOLENOID, E_STEPPERS > 3))
    #define HAS_SOLENOID_3 3
  #endif
  #if PIN_EXISTS(SOL4) && (ENABLED(MANUAL_SOLENOID_CONTROL) || TERN0(EXT_SOLENOID, E_STEPPERS > 4))
    #define HAS_SOLENOID_4 4
  #endif
  #if PIN_EXISTS(SOL5) && (ENABLED(MANUAL_SOLENOID_CONTROL) || TERN0(EXT_SOLENOID, E_STEPPERS > 5))
    #define HAS_SOLENOID_5 5
  #endif
  #if PIN_EXISTS(SOL6) && (ENABLED(MANUAL_SOLENOID_CONTROL) || TERN0(EXT_SOLENOID, E_STEPPERS > 6))
    #define HAS_SOLENOID_6 6
  #endif
  #if PIN_EXISTS(SOL7) && (ENABLED(MANUAL_SOLENOID_CONTROL) || TERN0(EXT_SOLENOID, E_STEPPERS > 7))
    #define HAS_SOLENOID_7 7
  #endif
#endif

//
// Trinamic Stepper Drivers
//

#if HAS_TRINAMIC_CONFIG
  #if ANY(STEALTHCHOP_E, STEALTHCHOP_XY, STEALTHCHOP_Z, STEALTHCHOP_I, STEALTHCHOP_J, STEALTHCHOP_K, STEALTHCHOP_U, STEALTHCHOP_V, STEALTHCHOP_W)
    #define STEALTHCHOP_ENABLED 1
  #endif
  #if EITHER(SENSORLESS_HOMING, SENSORLESS_PROBING)
    #define USE_SENSORLESS 1
  #endif
  // Disable Z axis sensorless homing if a probe is used to home the Z axis
  #if HOMING_Z_WITH_PROBE
    #undef Z_STALL_SENSITIVITY
    #undef Z2_STALL_SENSITIVITY
    #undef Z3_STALL_SENSITIVITY
    #undef Z4_STALL_SENSITIVITY
  #endif

  #if AXIS_IS_TMC(X)
    #if defined(X_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(X)
      #define X_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(X)
      #define X_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define X_SPI_SENSORLESS X_SENSORLESS
    #endif
    #ifndef X_INTERPOLATE
      #define X_INTERPOLATE INTERPOLATE
    #endif
    #ifndef X_HOLD_MULTIPLIER
      #define X_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef X_SLAVE_ADDRESS
      #define X_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(X2)
    #if defined(X2_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(X2)
      #define X2_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(X2)
      #define X2_HAS_STEALTHCHOP 1
    #endif
    #ifndef X2_INTERPOLATE
      #define X2_INTERPOLATE X_INTERPOLATE
    #endif
    #ifndef X2_HOLD_MULTIPLIER
      #define X2_HOLD_MULTIPLIER X_HOLD_MULTIPLIER
    #endif
    #ifndef X2_SLAVE_ADDRESS
      #define X2_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(Y)
    #if defined(Y_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Y)
      #define Y_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      #define Y_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define Y_SPI_SENSORLESS Y_SENSORLESS
    #endif
    #ifndef Y_INTERPOLATE
      #define Y_INTERPOLATE INTERPOLATE
    #endif
    #ifndef Y_HOLD_MULTIPLIER
      #define Y_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef Y_SLAVE_ADDRESS
      #define Y_SLAVE_ADDRESS 0
    #endif
    #if HAS_DUAL_Y_STEPPERS
      #if defined(Y2_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Y2)
        #define Y2_SENSORLESS 1
      #endif
      #if AXIS_HAS_STEALTHCHOP(Y2)
        #define Y2_HAS_STEALTHCHOP 1
      #endif
      #ifndef Y2_INTERPOLATE
        #define Y2_INTERPOLATE Y_INTERPOLATE
      #endif
      #ifndef Y2_HOLD_MULTIPLIER
        #define Y2_HOLD_MULTIPLIER Y_HOLD_MULTIPLIER
      #endif
      #ifndef Y2_SLAVE_ADDRESS
        #define Y2_SLAVE_ADDRESS 0
      #endif
    #endif
    #if HAS_U_AXIS
      #define U_SPI_SENSORLESS U_SENSORLESS
    #endif
    #if HAS_V_AXIS
      #define V_SPI_SENSORLESS V_SENSORLESS
    #endif
    #if HAS_W_AXIS
      #define W_SPI_SENSORLESS W_SENSORLESS
    #endif
  #endif

  #if AXIS_IS_TMC(Z)
    #if defined(Z_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Z)
      #define Z_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      #define Z_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define Z_SPI_SENSORLESS Z_SENSORLESS
    #endif
    #ifndef Z_INTERPOLATE
      #define Z_INTERPOLATE INTERPOLATE
    #endif
    #ifndef Z_HOLD_MULTIPLIER
      #define Z_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef Z_SLAVE_ADDRESS
      #define Z_SLAVE_ADDRESS 0
    #endif
    #if NUM_Z_STEPPERS >= 2
      #if defined(Z2_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Z2)
        #define Z2_SENSORLESS 1
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z2)
        #define Z2_HAS_STEALTHCHOP 1
      #endif
      #ifndef Z2_INTERPOLATE
        #define Z2_INTERPOLATE Z_INTERPOLATE
      #endif
      #ifndef Z2_HOLD_MULTIPLIER
        #define Z2_HOLD_MULTIPLIER Z_HOLD_MULTIPLIER
      #endif
      #ifndef Z2_SLAVE_ADDRESS
        #define Z2_SLAVE_ADDRESS 0
      #endif
    #endif
    #if NUM_Z_STEPPERS >= 3
      #if defined(Z3_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Z3)
        #define Z3_SENSORLESS 1
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z3)
        #define Z3_HAS_STEALTHCHOP 1
      #endif
      #ifndef Z3_INTERPOLATE
        #define Z3_INTERPOLATE Z_INTERPOLATE
      #endif
      #ifndef Z3_HOLD_MULTIPLIER
        #define Z3_HOLD_MULTIPLIER Z_HOLD_MULTIPLIER
      #endif
      #ifndef Z3_SLAVE_ADDRESS
        #define Z3_SLAVE_ADDRESS 0
      #endif
    #endif
    #if NUM_Z_STEPPERS >= 4
      #if defined(Z4_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Z4)
        #define Z4_SENSORLESS 1
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z4)
        #define Z4_HAS_STEALTHCHOP 1
      #endif
      #ifndef Z4_INTERPOLATE
        #define Z4_INTERPOLATE Z_INTERPOLATE
      #endif
      #ifndef Z4_HOLD_MULTIPLIER
        #define Z4_HOLD_MULTIPLIER Z_HOLD_MULTIPLIER
      #endif
      #ifndef Z4_SLAVE_ADDRESS
        #define Z4_SLAVE_ADDRESS 0
      #endif
    #endif
  #endif

  #if AXIS_IS_TMC(I)
    #if defined(I_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(I)
      #define I_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(I)
      #define I_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define I_SPI_SENSORLESS I_SENSORLESS
    #endif
    #ifndef I_INTERPOLATE
      #define I_INTERPOLATE INTERPOLATE
    #endif
    #ifndef I_HOLD_MULTIPLIER
      #define I_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef I_SLAVE_ADDRESS
      #define I_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(J)
    #if defined(J_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(J)
      #define J_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(J)
      #define J_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define J_SPI_SENSORLESS J_SENSORLESS
    #endif
    #ifndef J_INTERPOLATE
      #define J_INTERPOLATE INTERPOLATE
    #endif
    #ifndef J_HOLD_MULTIPLIER
      #define J_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef J_SLAVE_ADDRESS
      #define J_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(K)
    #if defined(K_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(K)
      #define K_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(K)
      #define K_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define K_SPI_SENSORLESS K_SENSORLESS
    #endif
    #ifndef K_INTERPOLATE
      #define K_INTERPOLATE INTERPOLATE
    #endif
    #ifndef K_HOLD_MULTIPLIER
      #define K_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef K_SLAVE_ADDRESS
      #define K_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(U)
    #if defined(U_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(U)
      #define U_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(U)
      #define U_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define U_SPI_SENSORLESS U_SENSORLESS
    #endif
    #ifndef U_INTERPOLATE
      #define U_INTERPOLATE INTERPOLATE
    #endif
    #ifndef U_HOLD_MULTIPLIER
      #define U_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef U_SLAVE_ADDRESS
      #define U_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(V)
    #if defined(V_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(V)
      #define V_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(V)
      #define V_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define V_SPI_SENSORLESS V_SENSORLESS
    #endif
    #ifndef V_INTERPOLATE
      #define V_INTERPOLATE INTERPOLATE
    #endif
    #ifndef V_HOLD_MULTIPLIER
      #define V_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef V_SLAVE_ADDRESS
      #define V_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(W)
    #if defined(W_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(W)
      #define W_SENSORLESS 1
    #endif
    #if AXIS_HAS_STEALTHCHOP(W)
      #define W_HAS_STEALTHCHOP 1
    #endif
    #if ENABLED(SPI_ENDSTOPS)
      #define W_SPI_SENSORLESS W_SENSORLESS
    #endif
    #ifndef W_INTERPOLATE
      #define W_INTERPOLATE INTERPOLATE
    #endif
    #ifndef W_HOLD_MULTIPLIER
      #define W_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef W_SLAVE_ADDRESS
      #define W_SLAVE_ADDRESS 0
    #endif
  #endif

  #if AXIS_IS_TMC(E0)
    #if AXIS_HAS_STEALTHCHOP(E0)
      #define E0_HAS_STEALTHCHOP 1
    #endif
    #ifndef E0_INTERPOLATE
      #define E0_INTERPOLATE INTERPOLATE
    #endif
    #ifndef E0_HOLD_MULTIPLIER
      #define E0_HOLD_MULTIPLIER HOLD_MULTIPLIER
    #endif
    #ifndef E0_SLAVE_ADDRESS
      #define E0_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E1)
    #if AXIS_HAS_STEALTHCHOP(E1)
      #define E1_HAS_STEALTHCHOP 1
    #endif
    #ifndef E1_INTERPOLATE
      #define E1_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E1_HOLD_MULTIPLIER
      #define E1_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E1_SLAVE_ADDRESS
      #define E1_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E2)
    #if AXIS_HAS_STEALTHCHOP(E2)
      #define E2_HAS_STEALTHCHOP 1
    #endif
    #ifndef E2_INTERPOLATE
      #define E2_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E2_HOLD_MULTIPLIER
      #define E2_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E2_SLAVE_ADDRESS
      #define E2_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E3)
    #if AXIS_HAS_STEALTHCHOP(E3)
      #define E3_HAS_STEALTHCHOP 1
    #endif
    #ifndef E3_INTERPOLATE
      #define E3_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E3_HOLD_MULTIPLIER
      #define E3_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E3_SLAVE_ADDRESS
      #define E3_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E4)
    #if AXIS_HAS_STEALTHCHOP(E4)
      #define E4_HAS_STEALTHCHOP 1
    #endif
    #ifndef E4_INTERPOLATE
      #define E4_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E4_HOLD_MULTIPLIER
      #define E4_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E4_SLAVE_ADDRESS
      #define E4_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E5)
    #if AXIS_HAS_STEALTHCHOP(E5)
      #define E5_HAS_STEALTHCHOP 1
    #endif
    #ifndef E5_INTERPOLATE
      #define E5_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E5_HOLD_MULTIPLIER
      #define E5_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E5_SLAVE_ADDRESS
      #define E5_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E6)
    #if AXIS_HAS_STEALTHCHOP(E6)
      #define E6_HAS_STEALTHCHOP 1
    #endif
    #ifndef E6_INTERPOLATE
      #define E6_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E6_HOLD_MULTIPLIER
      #define E6_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E6_SLAVE_ADDRESS
      #define E6_SLAVE_ADDRESS 0
    #endif
  #endif
  #if AXIS_IS_TMC(E7)
    #if AXIS_HAS_STEALTHCHOP(E7)
      #define E7_HAS_STEALTHCHOP 1
    #endif
    #ifndef E7_INTERPOLATE
      #define E7_INTERPOLATE E0_INTERPOLATE
    #endif
    #ifndef E7_HOLD_MULTIPLIER
      #define E7_HOLD_MULTIPLIER E0_HOLD_MULTIPLIER
    #endif
    #ifndef E7_SLAVE_ADDRESS
      #define E7_SLAVE_ADDRESS 0
    #endif
  #endif
#endif // HAS_TRINAMIC_CONFIG

#if ANY_AXIS_HAS(HW_SERIAL)
  #define HAS_TMC_HW_SERIAL 1
#endif
#if ANY_AXIS_HAS(SW_SERIAL)
  #define HAS_TMC_SW_SERIAL 1
#endif

#if DISABLED(SENSORLESS_HOMING)
  #undef SENSORLESS_BACKOFF_MM
#endif

//
// Set USING_HW_SERIALn flags for used Serial Ports
//

// Flag the indexed hardware serial ports in use
#define SERIAL_IN_USE(N) (   (defined(SERIAL_PORT)      && N == SERIAL_PORT) \
                          || (defined(SERIAL_PORT_2)    && N == SERIAL_PORT_2) \
                          || (defined(SERIAL_PORT_3)    && N == SERIAL_PORT_3) \
                          || (defined(MMU2_SERIAL_PORT) && N == MMU2_SERIAL_PORT) \
                          || (defined(LCD_SERIAL_PORT)  && N == LCD_SERIAL_PORT) )

// Flag the named hardware serial ports in use
#define TMC_UART_IS(A,N) (defined(A##_HARDWARE_SERIAL) && (CAT(HW_,A##_HARDWARE_SERIAL) == HW_Serial##N || CAT(HW_,A##_HARDWARE_SERIAL) == HW_MSerial##N))
#define ANY_SERIAL_IS(N) (  SERIAL_IN_USE(N) \
                         || TMC_UART_IS(X,  N) || TMC_UART_IS(Y , N) || TMC_UART_IS(Z , N) \
                         || TMC_UART_IS(I,  N) || TMC_UART_IS(J , N) || TMC_UART_IS(K , N) \
                         || TMC_UART_IS(U,  N) || TMC_UART_IS(V , N) || TMC_UART_IS(W , N) \
                         || TMC_UART_IS(X2, N) || TMC_UART_IS(Y2, N) || TMC_UART_IS(Z2, N) || TMC_UART_IS(Z3, N) || TMC_UART_IS(Z4, N) \
                         || TMC_UART_IS(E0, N) || TMC_UART_IS(E1, N) || TMC_UART_IS(E2, N) || TMC_UART_IS(E3, N) || TMC_UART_IS(E4, N) )

#define HW_Serial    501
#define HW_Serial0   502
#define HW_Serial1   503
#define HW_Serial2   504
#define HW_Serial3   505
#define HW_Serial4   506
#define HW_Serial5   507
#define HW_Serial6   508
#define HW_MSerial0  509
#define HW_MSerial1  510
#define HW_MSerial2  511
#define HW_MSerial3  512
#define HW_MSerial4  513
#define HW_MSerial5  514
#define HW_MSerial6  515
#define HW_MSerial7  516
#define HW_MSerial8  517
#define HW_MSerial9  518
#define HW_MSerial10 519

#if SERIAL_IN_USE(-1)
  #define USING_HW_SERIALUSB 1
#endif
#if ANY_SERIAL_IS(0)
  #define USING_HW_SERIAL0 1
#endif
#if ANY_SERIAL_IS(1)
  #define USING_HW_SERIAL1 1
#endif
#if ANY_SERIAL_IS(2)
  #define USING_HW_SERIAL2 1
#endif
#if ANY_SERIAL_IS(3)
  #define USING_HW_SERIAL3 1
#endif
#if ANY_SERIAL_IS(4)
  #define USING_HW_SERIAL4 1
#endif
#if ANY_SERIAL_IS(5)
  #define USING_HW_SERIAL5 1
#endif
#if ANY_SERIAL_IS(6)
  #define USING_HW_SERIAL6 1
#endif
#if ANY_SERIAL_IS(7)
  #define USING_HW_SERIAL7 1
#endif
#if ANY_SERIAL_IS(8)
  #define USING_HW_SERIAL8 1
#endif
#if ANY_SERIAL_IS(9)
  #define USING_HW_SERIAL9 1
#endif
#if ANY_SERIAL_IS(10)
  #define USING_HW_SERIAL10 1
#endif

#undef HW_Serial
#undef HW_Serial0
#undef HW_Serial1
#undef HW_Serial2
#undef HW_Serial3
#undef HW_Serial4
#undef HW_Serial5
#undef HW_Serial6
#undef HW_MSerial0
#undef HW_MSerial1
#undef HW_MSerial2
#undef HW_MSerial3
#undef HW_MSerial4
#undef HW_MSerial5
#undef HW_MSerial6
#undef HW_MSerial7
#undef HW_MSerial8
#undef HW_MSerial9
#undef HW_MSerial10

#undef _SERIAL_ID
#undef _TMC_UART_IS
#undef TMC_UART_IS
#undef ANY_SERIAL_IS

// Clean up unused ESP_WIFI pins
#ifdef ESP_WIFI_MODULE_COM
  #if !SERIAL_IN_USE(ESP_WIFI_MODULE_COM)
    #undef ESP_WIFI_MODULE_COM
    #undef ESP_WIFI_MODULE_BAUDRATE
    #undef ESP_WIFI_MODULE_RESET_PIN
    #undef ESP_WIFI_MODULE_ENABLE_PIN
    #undef ESP_WIFI_MODULE_TXD_PIN
    #undef ESP_WIFI_MODULE_RXD_PIN
    #undef ESP_WIFI_MODULE_GPIO0_PIN
    #undef ESP_WIFI_MODULE_GPIO2_PIN
    #undef ESP_WIFI_MODULE_GPIO4_PIN
  #endif
#endif

//
// Endstops and bed probe
//

// Is an endstop plug used for extra Z endstops or the probe?
#define IS_PROBE_PIN(A,M) (USES_Z_MIN_PROBE_PIN && Z_MIN_PROBE_PIN == A##_##M##_PIN)
#define IS_X2_ENDSTOP(A,M) (ENABLED(X_DUAL_ENDSTOPS) && X2_USE_ENDSTOP == _##A##M##_)
#define IS_Y2_ENDSTOP(A,M) (ENABLED(Y_DUAL_ENDSTOPS) && Y2_USE_ENDSTOP == _##A##M##_)
#define IS_Z2_ENDSTOP(A,M) (ENABLED(Z_MULTI_ENDSTOPS) && Z2_USE_ENDSTOP == _##A##M##_)
#define IS_Z3_ENDSTOP(A,M) (ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPERS >= 3 && Z3_USE_ENDSTOP == _##A##M##_)
#define IS_Z4_ENDSTOP(A,M) (ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPERS >= 4 && Z4_USE_ENDSTOP == _##A##M##_)

#define _HAS_STOP(A,M) (HAS_##A##_AXIS && PIN_EXISTS(A##_##M) && !IS_PROBE_PIN(A,M) && !IS_X2_ENDSTOP(A,M) && !IS_Y2_ENDSTOP(A,M) && !IS_Z2_ENDSTOP(A,M) && !IS_Z3_ENDSTOP(A,M) && !IS_Z4_ENDSTOP(A,M))
#if _HAS_STOP(X,MIN)
  #define HAS_X_MIN 1
#endif
#if _HAS_STOP(X,MAX)
  #define HAS_X_MAX 1
#endif
#if _HAS_STOP(Y,MIN)
  #define HAS_Y_MIN 1
#endif
#if _HAS_STOP(Y,MAX)
  #define HAS_Y_MAX 1
#endif
#if _HAS_STOP(Z,MIN)
  #define HAS_Z_MIN 1
#endif
#if _HAS_STOP(Z,MAX)
  #define HAS_Z_MAX 1
#endif
#if _HAS_STOP(I,MIN)
  #define HAS_I_MIN 1
#endif
#if _HAS_STOP(I,MAX)
  #define HAS_I_MAX 1
#endif
#if _HAS_STOP(J,MIN)
  #define HAS_J_MIN 1
#endif
#if _HAS_STOP(J,MAX)
  #define HAS_J_MAX 1
#endif
#if _HAS_STOP(K,MIN)
  #define HAS_K_MIN 1
#endif
#if _HAS_STOP(K,MAX)
  #define HAS_K_MAX 1
#endif
#if _HAS_STOP(U,MIN)
  #define HAS_U_MIN 1
#endif
#if _HAS_STOP(U,MAX)
  #define HAS_U_MAX 1
#endif
#if _HAS_STOP(V,MIN)
  #define HAS_V_MIN 1
#endif
#if _HAS_STOP(V,MAX)
  #define HAS_V_MAX 1
#endif
#if _HAS_STOP(W,MIN)
  #define HAS_W_MIN 1
#endif
#if _HAS_STOP(W,MAX)
  #define HAS_W_MAX 1
#endif
#if PIN_EXISTS(X2_MIN)
  #define HAS_X2_MIN 1
#endif
#if PIN_EXISTS(X2_MAX)
  #define HAS_X2_MAX 1
#endif
#if PIN_EXISTS(Y2_MIN)
  #define HAS_Y2_MIN 1
#endif
#if PIN_EXISTS(Y2_MAX)
  #define HAS_Y2_MAX 1
#endif
#if PIN_EXISTS(Z2_MIN)
  #define HAS_Z2_MIN 1
#endif
#if PIN_EXISTS(Z2_MAX)
  #define HAS_Z2_MAX 1
#endif
#if PIN_EXISTS(Z3_MIN)
  #define HAS_Z3_MIN 1
#endif
#if PIN_EXISTS(Z3_MAX)
  #define HAS_Z3_MAX 1
#endif
#if PIN_EXISTS(Z4_MIN)
  #define HAS_Z4_MIN 1
#endif
#if PIN_EXISTS(Z4_MAX)
  #define HAS_Z4_MAX 1
#endif

#if HAS_BED_PROBE && PIN_EXISTS(Z_MIN_PROBE)
  #define HAS_Z_MIN_PROBE_PIN 1
#endif

#undef _HAS_STOP
#undef IS_PROBE_PIN
#undef IS_X2_ENDSTOP
#undef IS_Y2_ENDSTOP
#undef IS_Z2_ENDSTOP
#undef IS_Z3_ENDSTOP
#undef IS_Z4_ENDSTOP

//
// ADC Temp Sensors (Thermistor or Thermocouple with amplifier ADC interface)
//
#define HAS_ADC_TEST(P) (TEMP_SENSOR(P) && PIN_EXISTS(TEMP_##P) && !TEMP_SENSOR_IS_MAX_TC(P) && !TEMP_SENSOR_##P##_IS_DUMMY)
#if HOTENDS > 0 && HAS_ADC_TEST(0)
  #define HAS_TEMP_ADC_0 1
#endif
#if HOTENDS > 1 && HAS_ADC_TEST(1)
  #define HAS_TEMP_ADC_1 1
#endif
#if HOTENDS > 2 && HAS_ADC_TEST(2)
  #define HAS_TEMP_ADC_2 1
#endif
#if HOTENDS > 3 && HAS_ADC_TEST(3)
  #define HAS_TEMP_ADC_3 1
#endif
#if HOTENDS > 4 && HAS_ADC_TEST(4)
  #define HAS_TEMP_ADC_4 1
#endif
#if HOTENDS > 5 && HAS_ADC_TEST(5)
  #define HAS_TEMP_ADC_5 1
#endif
#if HOTENDS > 6 && HAS_ADC_TEST(6)
  #define HAS_TEMP_ADC_6 1
#endif
#if HOTENDS > 7 && HAS_ADC_TEST(7)
  #define HAS_TEMP_ADC_7 1
#endif
#if HAS_ADC_TEST(BED)
  #define HAS_TEMP_ADC_BED 1
#endif
#if HAS_ADC_TEST(PROBE)
  #define HAS_TEMP_ADC_PROBE 1
#endif
#if HAS_ADC_TEST(CHAMBER)
  #define HAS_TEMP_ADC_CHAMBER 1
#endif
#if HAS_ADC_TEST(COOLER)
  #define HAS_TEMP_ADC_COOLER 1
#endif
#if HAS_ADC_TEST(BOARD)
  #define HAS_TEMP_ADC_BOARD 1
#endif
#if HAS_ADC_TEST(REDUNDANT)
  #define HAS_TEMP_ADC_REDUNDANT 1
#endif

#define HAS_TEMP(N) (TEMP_SENSOR_IS_MAX_TC(N) || EITHER(HAS_TEMP_ADC_##N, TEMP_SENSOR_##N##_IS_DUMMY))
#if HAS_HOTEND && HAS_TEMP(0)
  #define HAS_TEMP_HOTEND 1
#endif
#if HAS_TEMP(BED)
  #define HAS_TEMP_BED 1
#endif
#if HAS_TEMP(CHAMBER)
  #define HAS_TEMP_CHAMBER 1
#endif
#if HAS_TEMP(PROBE)
  #define HAS_TEMP_PROBE 1
#endif
#if HAS_TEMP(COOLER)
  #define HAS_TEMP_COOLER 1
#endif
#if HAS_TEMP(BOARD)
  #define HAS_TEMP_BOARD 1
#endif
#if HAS_TEMP(REDUNDANT)
  #define HAS_TEMP_REDUNDANT 1
#endif

#if ENABLED(JOYSTICK)
  #if PIN_EXISTS(JOY_X)
    #define HAS_JOY_ADC_X 1
  #endif
  #if PIN_EXISTS(JOY_Y)
    #define HAS_JOY_ADC_Y 1
  #endif
  #if PIN_EXISTS(JOY_Z)
    #define HAS_JOY_ADC_Z 1
  #endif
  #if PIN_EXISTS(JOY_EN)
    #define HAS_JOY_ADC_EN 1
  #endif
#endif

// Heaters
#if PIN_EXISTS(HEATER_0)
  #define HAS_HEATER_0 1
#endif
#if PIN_EXISTS(HEATER_1)
  #define HAS_HEATER_1 1
#endif
#if PIN_EXISTS(HEATER_2)
  #define HAS_HEATER_2 1
#endif
#if PIN_EXISTS(HEATER_3)
  #define HAS_HEATER_3 1
#endif
#if PIN_EXISTS(HEATER_4)
  #define HAS_HEATER_4 1
#endif
#if PIN_EXISTS(HEATER_5)
  #define HAS_HEATER_5 1
#endif
#if PIN_EXISTS(HEATER_6)
  #define HAS_HEATER_6 1
#endif
#if PIN_EXISTS(HEATER_7)
  #define HAS_HEATER_7 1
#endif
#if PIN_EXISTS(HEATER_BED)
  #define HAS_HEATER_BED 1
#endif

// Shorthand for common combinations
#if HAS_TEMP_BED && HAS_HEATER_BED
  #define HAS_HEATED_BED 1
  #ifndef BED_OVERSHOOT
    #define BED_OVERSHOOT 10
  #endif
  #define BED_MAX_TARGET (BED_MAXTEMP - (BED_OVERSHOOT))
#else
  #undef PIDTEMPBED
  #undef PREHEAT_BEFORE_LEVELING
#endif

#if HAS_TEMP_COOLER && PIN_EXISTS(COOLER)
  #define HAS_COOLER 1
  #ifndef COOLER_OVERSHOOT
    #define COOLER_OVERSHOOT 2
  #endif
  #define COOLER_MIN_TARGET (COOLER_MINTEMP + (COOLER_OVERSHOOT))
  #define COOLER_MAX_TARGET (COOLER_MAXTEMP - (COOLER_OVERSHOOT))
#endif

#if HAS_HEATED_BED || HAS_TEMP_CHAMBER
  #define BED_OR_CHAMBER 1
#endif

#if HAS_TEMP_HOTEND || BED_OR_CHAMBER || HAS_TEMP_PROBE || HAS_TEMP_COOLER || HAS_TEMP_BOARD
  #define HAS_TEMP_SENSOR 1
#endif

#if HAS_TEMP_CHAMBER && PIN_EXISTS(HEATER_CHAMBER)
  #define HAS_HEATED_CHAMBER 1
  #ifndef CHAMBER_OVERSHOOT
    #define CHAMBER_OVERSHOOT 10
  #endif
  #define CHAMBER_MAX_TARGET (CHAMBER_MAXTEMP - (CHAMBER_OVERSHOOT))
#else
  #undef PIDTEMPCHAMBER
#endif

// PID heating
#if ANY(PIDTEMP, PIDTEMPBED, PIDTEMPCHAMBER)
  #define HAS_PID_HEATING 1
#endif

// Thermal protection
#if !HAS_HEATED_BED
  #undef THERMAL_PROTECTION_BED
#endif
#if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
  #define WATCH_HOTENDS 1
#endif
#if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0
  #define WATCH_BED 1
#endif
#if BOTH(HAS_HEATED_CHAMBER, THERMAL_PROTECTION_CHAMBER) && WATCH_CHAMBER_TEMP_PERIOD > 0
  #define WATCH_CHAMBER 1
#endif
#if BOTH(HAS_COOLER, THERMAL_PROTECTION_COOLER) && WATCH_COOLER_TEMP_PERIOD > 0
  #define WATCH_COOLER 1
#endif
#if NONE(THERMAL_PROTECTION_HOTENDS, THERMAL_PROTECTION_CHAMBER, THERMAL_PROTECTION_BED, THERMAL_PROTECTION_COOLER)
  #undef THERMAL_PROTECTION_VARIANCE_MONITOR
#endif
#if  (ENABLED(THERMAL_PROTECTION_HOTENDS) || !EXTRUDERS) \
  && (ENABLED(THERMAL_PROTECTION_BED)     || !HAS_HEATED_BED) \
  && (ENABLED(THERMAL_PROTECTION_CHAMBER) || !HAS_HEATED_CHAMBER) \
  && (ENABLED(THERMAL_PROTECTION_COOLER)  || !HAS_COOLER)
  #define THERMALLY_SAFE 1
#endif

// Auto fans
#if HAS_HOTEND && PIN_EXISTS(E0_AUTO_FAN)
  #define HAS_AUTO_FAN_0 1
#endif
#if HAS_MULTI_HOTEND && PIN_EXISTS(E1_AUTO_FAN)
  #define HAS_AUTO_FAN_1 1
#endif
#if HOTENDS > 2 && PIN_EXISTS(E2_AUTO_FAN)
  #define HAS_AUTO_FAN_2 1
#endif
#if HOTENDS > 3 && PIN_EXISTS(E3_AUTO_FAN)
  #define HAS_AUTO_FAN_3 1
#endif
#if HOTENDS > 4 && PIN_EXISTS(E4_AUTO_FAN)
  #define HAS_AUTO_FAN_4 1
#endif
#if HOTENDS > 5 && PIN_EXISTS(E5_AUTO_FAN)
  #define HAS_AUTO_FAN_5 1
#endif
#if HOTENDS > 6 && PIN_EXISTS(E6_AUTO_FAN)
  #define HAS_AUTO_FAN_6 1
#endif
#if HOTENDS > 7 && PIN_EXISTS(E7_AUTO_FAN)
  #define HAS_AUTO_FAN_7 1
#endif
#if HAS_TEMP_CHAMBER && PIN_EXISTS(CHAMBER_AUTO_FAN)
  #define HAS_AUTO_CHAMBER_FAN 1
#endif
#if HAS_TEMP_COOLER && PIN_EXISTS(COOLER_AUTO_FAN)
  #define HAS_AUTO_COOLER_FAN 1
#endif

#if ANY(HAS_AUTO_FAN_0, HAS_AUTO_FAN_1, HAS_AUTO_FAN_2, HAS_AUTO_FAN_3, HAS_AUTO_FAN_4, HAS_AUTO_FAN_5, HAS_AUTO_FAN_6, HAS_AUTO_FAN_7, HAS_AUTO_CHAMBER_FAN, HAS_AUTO_COOLER_FAN)
  #define HAS_AUTO_FAN 1
  #define _FANOVERLAP(I,T) (T##_AUTO_FAN_PIN == E##I##_AUTO_FAN_PIN)
  #if HAS_AUTO_CHAMBER_FAN
    #define _CHFANOVERLAP(I) || _FANOVERLAP(I,CHAMBER)
    #if (0 REPEAT(8, _CHFANOVERLAP))
      #define AUTO_CHAMBER_IS_E 1
    #endif
    #undef _CHFANOVERLAP
  #endif
  #if HAS_AUTO_COOLER_FAN
    #define _COFANOVERLAP(I) || _FANOVERLAP(I,COOLER)
    #if (0 REPEAT(8, _COFANOVERLAP))
      #define AUTO_COOLER_IS_E 1
    #endif
    #undef _COFANOVERLAP
  #endif
#endif

// Fans check
#if HAS_HOTEND && PIN_EXISTS(E0_FAN_TACHO)
  #define HAS_E0_FAN_TACHO 1
#endif
#if HOTENDS > 1 && PIN_EXISTS(E1_FAN_TACHO)
  #define HAS_E1_FAN_TACHO 1
#endif
#if HOTENDS > 2 && PIN_EXISTS(E2_FAN_TACHO)
  #define HAS_E2_FAN_TACHO 1
#endif
#if HOTENDS > 3 && PIN_EXISTS(E3_FAN_TACHO)
  #define HAS_E3_FAN_TACHO 1
#endif
#if HOTENDS > 4 && PIN_EXISTS(E4_FAN_TACHO)
  #define HAS_E4_FAN_TACHO 1
#endif
#if HOTENDS > 5 && PIN_EXISTS(E5_FAN_TACHO)
  #define HAS_E5_FAN_TACHO 1
#endif
#if HOTENDS > 6 && PIN_EXISTS(E6_FAN_TACHO)
  #define HAS_E6_FAN_TACHO 1
#endif
#if HOTENDS > 7 && PIN_EXISTS(E7_FAN_TACHO)
  #define HAS_E7_FAN_TACHO 1
#endif
#if ANY(HAS_E0_FAN_TACHO, HAS_E1_FAN_TACHO, HAS_E2_FAN_TACHO, HAS_E3_FAN_TACHO, HAS_E4_FAN_TACHO, HAS_E5_FAN_TACHO, HAS_E6_FAN_TACHO, HAS_E7_FAN_TACHO)
  #define HAS_FANCHECK 1
  #if HAS_AUTO_FAN && EXTRUDER_AUTO_FAN_SPEED != 255 && DISABLED(FOURWIRES_FANS)
    #define HAS_PWMFANCHECK 1
  #endif
#endif

#if !HAS_TEMP_SENSOR
  #undef AUTO_REPORT_TEMPERATURES
#endif
#if ANY(AUTO_REPORT_TEMPERATURES, AUTO_REPORT_SD_STATUS, AUTO_REPORT_POSITION, AUTO_REPORT_FANS)
  #define HAS_AUTO_REPORTING 1
#endif

#if !HAS_AUTO_CHAMBER_FAN || AUTO_CHAMBER_IS_E
  #undef AUTO_POWER_CHAMBER_FAN
#endif
#if !HAS_AUTO_COOLER_FAN || AUTO_COOLER_IS_E
  #undef AUTO_POWER_COOLER_FAN
#endif

// Print Cooling fans (limit)
#ifdef NUM_M106_FANS
  #define MAX_FANS NUM_M106_FANS
#else
  #define MAX_FANS 8  // Max supported fans
#endif

#define _NOT_E_AUTO(N,F) (E##N##_AUTO_FAN_PIN != FAN##F##_PIN)
#define _HAS_FAN(F) (PIN_EXISTS(FAN##F) \
                     && CONTROLLER_FAN_PIN != FAN##F##_PIN \
                     && _NOT_E_AUTO(0,F) \
                     && _NOT_E_AUTO(1,F) \
                     && _NOT_E_AUTO(2,F) \
                     && _NOT_E_AUTO(3,F) \
                     && _NOT_E_AUTO(4,F) \
                     && _NOT_E_AUTO(5,F) \
                     && _NOT_E_AUTO(6,F) \
                     && _NOT_E_AUTO(7,F) \
                     && F < MAX_FANS)
#if PIN_EXISTS(FAN)
  #define HAS_FAN0 1
#endif
#if _HAS_FAN(1)
  #define HAS_FAN1 1
#endif
#if _HAS_FAN(2)
  #define HAS_FAN2 1
#endif
#if _HAS_FAN(3)
  #define HAS_FAN3 1
#endif
#if _HAS_FAN(4)
  #define HAS_FAN4 1
#endif
#if _HAS_FAN(5)
  #define HAS_FAN5 1
#endif
#if _HAS_FAN(6)
  #define HAS_FAN6 1
#endif
#if _HAS_FAN(7)
  #define HAS_FAN7 1
#endif
#undef _NOT_E_AUTO
#undef _HAS_FAN

#if BED_OR_CHAMBER || HAS_FAN0
  #define BED_OR_CHAMBER_OR_FAN 1
#endif

/**
 * Up to 3 PWM fans
 */
#ifndef FAN_INVERTING
  #define FAN_INVERTING false
#endif

#if HAS_FAN7
  #define FAN_COUNT 8
#elif HAS_FAN6
  #define FAN_COUNT 7
#elif HAS_FAN5
  #define FAN_COUNT 6
#elif HAS_FAN4
  #define FAN_COUNT 5
#elif HAS_FAN3
  #define FAN_COUNT 4
#elif HAS_FAN2
  #define FAN_COUNT 3
#elif HAS_FAN1
  #define FAN_COUNT 2
#elif HAS_FAN0
  #define FAN_COUNT 1
#else
  #define FAN_COUNT 0
#endif

#if FAN_COUNT > 0
  #define HAS_FAN 1
#endif

/**
 * Part Cooling fan multipliexer
 */
#if PIN_EXISTS(FANMUX0)
  #define HAS_FANMUX 1
#endif

/**
 * MIN/MAX fan PWM scaling
 */
#ifndef FAN_OFF_PWM
  #define FAN_OFF_PWM 0
#endif
#ifndef FAN_MIN_PWM
  #if FAN_OFF_PWM > 0
    #define FAN_MIN_PWM (FAN_OFF_PWM + 1)
  #else
    #define FAN_MIN_PWM 0
  #endif
#endif
#ifndef FAN_MAX_PWM
  #define FAN_MAX_PWM 255
#endif
#if FAN_MIN_PWM < 0 || FAN_MIN_PWM > 255
  #error "FAN_MIN_PWM must be a value from 0 to 255."
#elif FAN_MAX_PWM < 0 || FAN_MAX_PWM > 255
  #error "FAN_MAX_PWM must be a value from 0 to 255."
#elif FAN_MIN_PWM > FAN_MAX_PWM
  #error "FAN_MIN_PWM must be less than or equal to FAN_MAX_PWM."
#elif FAN_OFF_PWM > FAN_MIN_PWM
  #error "FAN_OFF_PWM must be less than or equal to FAN_MIN_PWM."
#endif

/**
 * Controller Fan Settings
 */
#if PIN_EXISTS(CONTROLLER_FAN)
  #define HAS_CONTROLLER_FAN 1
  #if CONTROLLER_FAN_MIN_BOARD_TEMP
    #define HAS_CONTROLLER_FAN_MIN_BOARD_TEMP 1
  #endif
#endif

#if HAS_CONTROLLER_FAN
  #if ENABLED(CONTROLLER_FAN_USE_BOARD_TEMP)
    #define HAS_CONTROLLER_FAN_BOARD_TEMP_TRIGGER 1
    #ifndef CONTROLLER_FAN_TRIGGER_TEMP
      #define CONTROLLER_FAN_TRIGGER_TEMP 30
    #endif
  #else
    #undef CONTROLLER_FAN_TRIGGER_TEMP
  #endif
#endif

// Servos
#if PIN_EXISTS(SERVO0) && NUM_SERVOS > 0
  #define HAS_SERVO_0 1
#endif
#if PIN_EXISTS(SERVO1) && NUM_SERVOS > 1
  #define HAS_SERVO_1 1
#endif
#if PIN_EXISTS(SERVO2) && NUM_SERVOS > 2
  #define HAS_SERVO_2 1
#endif
#if PIN_EXISTS(SERVO3) && NUM_SERVOS > 3
  #define HAS_SERVO_3 1
#endif
#if NUM_SERVOS > 0
  #define HAS_SERVOS 1
  #if defined(PAUSE_SERVO_OUTPUT) && defined(RESUME_SERVO_OUTPUT)
    #define HAS_PAUSE_SERVO_OUTPUT 1
  #endif
#else
  #undef SERVO_DELAY
  #undef DEACTIVATE_SERVOS_AFTER_MOVE
  #undef EDITABLE_SERVO_ANGLES
  #undef SERVO_DETACH_GCODE
#endif

// Sensors
#if PIN_EXISTS(FILWIDTH)
  #define HAS_FILAMENT_WIDTH_SENSOR 1
#endif

// User Interface
#if ENABLED(FREEZE_FEATURE) && !PIN_EXISTS(FREEZE) && PIN_EXISTS(KILL)
  #define FREEZE_PIN KILL_PIN
#elif PIN_EXISTS(KILL) && TERN1(FREEZE_FEATURE, KILL_PIN != FREEZE_PIN)
  #define HAS_KILL 1
#endif
#if PIN_EXISTS(HOME)
  #define HAS_HOME 1
#endif
#if PIN_EXISTS(SUICIDE)
  #define HAS_SUICIDE 1
#endif
#if PIN_EXISTS(PHOTOGRAPH)
  #define HAS_PHOTOGRAPH 1
#endif

// Digital control
#if PIN_EXISTS(STEPPER_RESET)
  #define HAS_STEPPER_RESET 1
#endif
#if PIN_EXISTS(DIGIPOTSS)
  #define HAS_MOTOR_CURRENT_SPI 1
#endif
#if HAS_EXTRUDERS && PIN_EXISTS(MOTOR_CURRENT_PWM_E)
  #define HAS_MOTOR_CURRENT_PWM_E 1
#endif
#if HAS_MOTOR_CURRENT_PWM_E || ANY_PIN(MOTOR_CURRENT_PWM_X, MOTOR_CURRENT_PWM_Y, MOTOR_CURRENT_PWM_XY, MOTOR_CURRENT_PWM_Z, MOTOR_CURRENT_PWM_I, MOTOR_CURRENT_PWM_J, MOTOR_CURRENT_PWM_K, MOTOR_CURRENT_PWM_U, MOTOR_CURRENT_PWM_V, MOTOR_CURRENT_PWM_W)
  #define HAS_MOTOR_CURRENT_PWM 1
#endif

#if ANY(HAS_Z_MS_PINS, HAS_Z2_MS_PINS, HAS_Z3_MS_PINS, HAS_Z4_MS_PINS)
  #define HAS_SOME_Z_MS_PINS 1
#endif
#if ANY(HAS_E0_MS_PINS, HAS_E1_MS_PINS, HAS_E2_MS_PINS, HAS_E3_MS_PINS, HAS_E4_MS_PINS, HAS_E5_MS_PINS, HAS_E6_MS_PINS, HAS_E7_MS_PINS)
  #define HAS_SOME_E_MS_PINS 1
#endif
#if ANY(HAS_X_MS_PINS, HAS_X2_MS_PINS, HAS_Y_MS_PINS, HAS_Y2_MS_PINS, HAS_SOME_Z_MS_PINS, HAS_I_MS_PINS, HAS_J_MS_PINS, HAS_K_MS_PINS, HAS_U_MS_PINS, HAS_V_MS_PINS, HAS_W_MS_PINS, HAS_SOME_E_MS_PINS)
  #define HAS_MICROSTEPS 1
#endif

/**
 * Heater signal inversion defaults
 */

#if HAS_HEATER_0 && !defined(HEATER_0_INVERTING)
  #define HEATER_0_INVERTING false
#endif
#if HAS_HEATER_1 && !defined(HEATER_1_INVERTING)
  #define HEATER_1_INVERTING false
#endif
#if HAS_HEATER_2 && !defined(HEATER_2_INVERTING)
  #define HEATER_2_INVERTING false
#endif
#if HAS_HEATER_3 && !defined(HEATER_3_INVERTING)
  #define HEATER_3_INVERTING false
#endif
#if HAS_HEATER_4 && !defined(HEATER_4_INVERTING)
  #define HEATER_4_INVERTING false
#endif
#if HAS_HEATER_5 && !defined(HEATER_5_INVERTING)
  #define HEATER_5_INVERTING false
#endif
#if HAS_HEATER_6 && !defined(HEATER_6_INVERTING)
  #define HEATER_6_INVERTING false
#endif
#if HAS_HEATER_7 && !defined(HEATER_7_INVERTING)
  #define HEATER_7_INVERTING false
#endif

/**
 * Helper Macros for heaters and extruder fan
 */

#define WRITE_HEATER_0P(v) WRITE(HEATER_0_PIN, (v) ^ HEATER_0_INVERTING)
#if EITHER(HAS_MULTI_HOTEND, HEATERS_PARALLEL)
  #define WRITE_HEATER_1(v) WRITE(HEATER_1_PIN, (v) ^ HEATER_1_INVERTING)
  #if HOTENDS > 2
    #define WRITE_HEATER_2(v) WRITE(HEATER_2_PIN, (v) ^ HEATER_2_INVERTING)
    #if HOTENDS > 3
      #define WRITE_HEATER_3(v) WRITE(HEATER_3_PIN, (v) ^ HEATER_3_INVERTING)
      #if HOTENDS > 4
        #define WRITE_HEATER_4(v) WRITE(HEATER_4_PIN, (v) ^ HEATER_4_INVERTING)
        #if HOTENDS > 5
          #define WRITE_HEATER_5(v) WRITE(HEATER_5_PIN, (v) ^ HEATER_5_INVERTING)
          #if HOTENDS > 6
            #define WRITE_HEATER_6(v) WRITE(HEATER_6_PIN, (v) ^ HEATER_6_INVERTING)
            #if HOTENDS > 7
              #define WRITE_HEATER_7(v) WRITE(HEATER_7_PIN, (v) ^ HEATER_7_INVERTING)
            #endif // HOTENDS > 7
          #endif // HOTENDS > 6
        #endif // HOTENDS > 5
      #endif // HOTENDS > 4
    #endif // HOTENDS > 3
  #endif // HOTENDS > 2
#endif // HAS_MULTI_HOTEND || HEATERS_PARALLEL
#if ENABLED(HEATERS_PARALLEL)
  #define WRITE_HEATER_0(v) { WRITE_HEATER_0P(v); WRITE_HEATER_1(v); }
#else
  #define WRITE_HEATER_0(v) WRITE_HEATER_0P(v)
#endif

#ifndef MIN_POWER
  #define MIN_POWER 0
#endif

/**
 * Heated bed requires settings
 */
#if HAS_HEATED_BED
  #ifndef MIN_BED_POWER
    #define MIN_BED_POWER 0
  #endif
  #ifndef MAX_BED_POWER
    #define MAX_BED_POWER 255
  #endif
  #ifndef HEATER_BED_INVERTING
    #define HEATER_BED_INVERTING false
  #endif
  #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN, (v) ^ HEATER_BED_INVERTING)
#endif

/**
 * Heated chamber requires settings
 */
#if HAS_HEATED_CHAMBER
  #ifndef MIN_CHAMBER_POWER
    #define MIN_CHAMBER_POWER 0
  #endif
  #ifndef MAX_CHAMBER_POWER
    #define MAX_CHAMBER_POWER 255
  #endif
  #ifndef HEATER_CHAMBER_INVERTING
    #define HEATER_CHAMBER_INVERTING false
  #endif
  #define WRITE_HEATER_CHAMBER(v) WRITE(HEATER_CHAMBER_PIN, (v) ^ HEATER_CHAMBER_INVERTING)
#endif

/**
 * Laser Cooling requires settings
 */
#if HAS_COOLER
  #ifndef MAX_COOLER_POWER
    #define MAX_COOLER_POWER 255
  #endif
  #ifndef COOLER_INVERTING
    #define COOLER_INVERTING true
  #endif
  #define WRITE_HEATER_COOLER(v) WRITE(COOLER_PIN, (v) ^ COOLER_INVERTING)
#endif

#if HAS_HOTEND || HAS_HEATED_BED || HAS_HEATED_CHAMBER || HAS_COOLER
  #define HAS_TEMPERATURE 1
#endif

#if HAS_TEMPERATURE && ANY(HAS_MARLINUI_MENU, HAS_DWIN_E3V2, HAS_DGUS_LCD_CLASSIC)
  #ifdef PREHEAT_10_LABEL
    #define PREHEAT_COUNT 10
  #elif defined(PREHEAT_9_LABEL)
    #define PREHEAT_COUNT 9
  #elif defined(PREHEAT_8_LABEL)
    #define PREHEAT_COUNT 8
  #elif defined(PREHEAT_7_LABEL)
    #define PREHEAT_COUNT 7
  #elif defined(PREHEAT_6_LABEL)
    #define PREHEAT_COUNT 6
  #elif defined(PREHEAT_5_LABEL)
    #define PREHEAT_COUNT 5
  #elif defined(PREHEAT_4_LABEL)
    #define PREHEAT_COUNT 4
  #elif defined(PREHEAT_3_LABEL)
    #define PREHEAT_COUNT 3
  #elif defined(PREHEAT_2_LABEL)
    #define PREHEAT_COUNT 2
  #elif defined(PREHEAT_1_LABEL)
    #define PREHEAT_COUNT 1
  #endif
  #if PREHEAT_COUNT && ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_FAN)
    #define HAS_PREHEAT 1
  #endif
#endif

#if !HAS_PREHEAT
  #undef PREHEAT_SHORTCUT_MENU_ITEM
  #undef DGUS_PREHEAT_UI
#endif

/**
 * MIN/MAX case light PWM scaling
 */
#if ENABLED(CASE_LIGHT_ENABLE)
  #ifndef CASE_LIGHT_MAX_PWM
    #define CASE_LIGHT_MAX_PWM 255
  #elif !WITHIN(CASE_LIGHT_MAX_PWM, 1, 255)
    #error "CASE_LIGHT_MAX_PWM must be a value from 1 to 255."
  #endif
#endif

/**
 * Bed Probe dependencies
 */
#if EITHER(MESH_BED_LEVELING, HAS_BED_PROBE)
  #ifndef Z_PROBE_OFFSET_RANGE_MIN
    #define Z_PROBE_OFFSET_RANGE_MIN -20
  #endif
  #ifndef Z_PROBE_OFFSET_RANGE_MAX
    #define Z_PROBE_OFFSET_RANGE_MAX 20
  #endif
#endif
#if HAS_BED_PROBE
  #if BOTH(ENDSTOPPULLUPS, HAS_Z_MIN_PROBE_PIN)
    #define ENDSTOPPULLUP_ZMIN_PROBE
  #endif
  #ifndef XY_PROBE_FEEDRATE
    #define XY_PROBE_FEEDRATE ((homing_feedrate_mm_m.x + homing_feedrate_mm_m.y) / 2)
  #endif
  #ifndef NOZZLE_TO_PROBE_OFFSET
    #define NOZZLE_TO_PROBE_OFFSET { 0, 0, 0 }
  #endif
#else
  #undef NOZZLE_TO_PROBE_OFFSET
  #undef PROBING_STEPPERS_OFF
#endif

/**
 * XYZ Bed Skew Correction
 */
#if ENABLED(SKEW_CORRECTION)
  #define SKEW_FACTOR_MIN -1
  #define SKEW_FACTOR_MAX 1

  #define _GET_SIDE(a,b,c) (SQRT(2*sq(a)+2*sq(b)-4*sq(c))*0.5)
  #define _SKEW_SIDE(a,b,c) tan(M_PI*0.5-acos((sq(a)-sq(b)-sq(c))/(2*c*b)))
  #define _SKEW_FACTOR(a,b,c) _SKEW_SIDE(float(a),_GET_SIDE(float(a),float(b),float(c)),float(c))

  #ifndef XY_SKEW_FACTOR
    #if defined(XY_DIAG_AC) && defined(XY_DIAG_BD) && defined(XY_SIDE_AD)
      #define XY_SKEW_FACTOR _SKEW_FACTOR(XY_DIAG_AC, XY_DIAG_BD, XY_SIDE_AD)
    #else
      #define XY_SKEW_FACTOR 0.0
    #endif
  #endif
  #ifndef XZ_SKEW_FACTOR
    #if defined(XY_SIDE_AD) && !defined(XZ_SIDE_AD)
      #define XZ_SIDE_AD XY_SIDE_AD
    #endif
    #if defined(XZ_DIAG_AC) && defined(XZ_DIAG_BD) && defined(XZ_SIDE_AD)
      #define XZ_SKEW_FACTOR _SKEW_FACTOR(XZ_DIAG_AC, XZ_DIAG_BD, XZ_SIDE_AD)
    #else
      #define XZ_SKEW_FACTOR 0.0
    #endif
  #endif
  #ifndef YZ_SKEW_FACTOR
    #if defined(YZ_DIAG_AC) && defined(YZ_DIAG_BD) && defined(YZ_SIDE_AD)
      #define YZ_SKEW_FACTOR _SKEW_FACTOR(YZ_DIAG_AC, YZ_DIAG_BD, YZ_SIDE_AD)
    #else
      #define YZ_SKEW_FACTOR 0.0
    #endif
  #endif
#endif // SKEW_CORRECTION

/**
 * Heater, Fan, and Probe interactions
 */
#if !HAS_FAN
  #undef ADAPTIVE_FAN_SLOWING
  #undef NO_FAN_SLOWING_IN_PID_TUNING
#endif
#if !BOTH(HAS_BED_PROBE, HAS_FAN)
  #undef PROBING_FANS_OFF
#endif
#if !BOTH(HAS_BED_PROBE, HAS_EXTRUDERS)
  #undef PROBING_ESTEPPERS_OFF
#elif ENABLED(PROBING_STEPPERS_OFF)
  // PROBING_STEPPERS_OFF implies PROBING_ESTEPPERS_OFF, make sure it is defined
  #define PROBING_ESTEPPERS_OFF
#endif
#if EITHER(ADVANCED_PAUSE_FEATURE, PROBING_HEATERS_OFF)
  #define HEATER_IDLE_HANDLER 1
#endif
#if HAS_BED_PROBE && (ANY(PROBING_HEATERS_OFF, PROBING_STEPPERS_OFF, PROBING_ESTEPPERS_OFF, PROBING_FANS_OFF) || DELAY_BEFORE_PROBING > 0)
  #define HAS_QUIET_PROBING 1
#endif

/**
 * Advanced Pause - Filament Change
 */
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #if ANY(HAS_MARLINUI_MENU, EXTENSIBLE_UI, DWIN_LCD_PROUI, DWIN_CREALITY_LCD_JYERSUI) || BOTH(EMERGENCY_PARSER, HOST_PROMPT_SUPPORT)
    #define M600_PURGE_MORE_RESUMABLE 1
  #endif
  #ifndef FILAMENT_CHANGE_SLOW_LOAD_LENGTH
    #define FILAMENT_CHANGE_SLOW_LOAD_LENGTH 0
  #endif
#endif

#if HAS_MULTI_EXTRUDER && !defined(TOOLCHANGE_FS_EXTRA_PRIME)
  #define TOOLCHANGE_FS_EXTRA_PRIME 0
#endif

/**
 * Only constrain Z on DELTA / SCARA machines
 */
#if IS_KINEMATIC
  #undef MIN_SOFTWARE_ENDSTOP_X
  #undef MIN_SOFTWARE_ENDSTOP_Y
  #undef MAX_SOFTWARE_ENDSTOP_X
  #undef MAX_SOFTWARE_ENDSTOP_Y
#endif

/**
 * Bed Probing bounds
 */

#ifndef PROBING_MARGIN
  #define PROBING_MARGIN 0
#endif

#if IS_KINEMATIC
  #undef PROBING_MARGIN_LEFT
  #undef PROBING_MARGIN_RIGHT
  #undef PROBING_MARGIN_FRONT
  #undef PROBING_MARGIN_BACK
  #define PROBING_MARGIN_LEFT 0
  #define PROBING_MARGIN_RIGHT 0
  #define PROBING_MARGIN_FRONT 0
  #define PROBING_MARGIN_BACK 0
#else
  #ifndef PROBING_MARGIN_LEFT
    #define PROBING_MARGIN_LEFT PROBING_MARGIN
  #endif
  #ifndef PROBING_MARGIN_RIGHT
    #define PROBING_MARGIN_RIGHT PROBING_MARGIN
  #endif
  #ifndef PROBING_MARGIN_FRONT
    #define PROBING_MARGIN_FRONT PROBING_MARGIN
  #endif
  #ifndef PROBING_MARGIN_BACK
    #define PROBING_MARGIN_BACK PROBING_MARGIN
  #endif
#endif

#if ENABLED(DELTA)
  /**
   * Delta radius/rod trimmers/angle trimmers
   */
  #ifndef DELTA_ENDSTOP_ADJ
    #define DELTA_ENDSTOP_ADJ { 0, 0, 0 }
  #endif
  #ifndef DELTA_TOWER_ANGLE_TRIM
    #define DELTA_TOWER_ANGLE_TRIM { 0, 0, 0 }
  #endif
  #ifndef DELTA_RADIUS_TRIM_TOWER
    #define DELTA_RADIUS_TRIM_TOWER { 0, 0, 0 }
  #endif
  #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER
    #define DELTA_DIAGONAL_ROD_TRIM_TOWER { 0, 0, 0 }
  #endif
#endif

#ifndef DEFAULT_LEVELING_FADE_HEIGHT
  #define DEFAULT_LEVELING_FADE_HEIGHT 0.0
#endif

#if ENABLED(SEGMENT_LEVELED_MOVES) && !defined(LEVELED_SEGMENT_LENGTH)
  #define LEVELED_SEGMENT_LENGTH 5
#endif

/**
 * Default mesh area is an area with an inset margin on the print area.
 */
#if EITHER(MESH_BED_LEVELING, AUTO_BED_LEVELING_UBL)
  #if IS_KINEMATIC
    // Probing points may be verified at compile time within the radius
    // using static_assert(HYPOT2(X2-X1,Y2-Y1)<=sq(DELTA_PRINTABLE_RADIUS),"bad probe point!")
    // so that may be added to SanityCheck.h in the future.
    #define _MESH_MIN_X (X_MIN_BED + MESH_INSET)
    #define _MESH_MIN_Y (Y_MIN_BED + MESH_INSET)
    #define _MESH_MAX_X (X_MAX_BED - (MESH_INSET))
    #define _MESH_MAX_Y (Y_MAX_BED - (MESH_INSET))
  #else
    // Boundaries for Cartesian probing based on set limits
    #define _MESH_MIN_X (_MAX(X_MIN_BED + MESH_INSET, X_MIN_POS))  // UBL is careful not to probe off the bed.  It does not
    #define _MESH_MIN_Y (_MAX(Y_MIN_BED + MESH_INSET, Y_MIN_POS))  // need NOZZLE_TO_PROBE_OFFSET in the mesh dimensions
    #define _MESH_MAX_X (_MIN(X_MAX_BED - (MESH_INSET), X_MAX_POS))
    #define _MESH_MAX_Y (_MIN(Y_MAX_BED - (MESH_INSET), Y_MAX_POS))
  #endif

  // These may be overridden in Configuration.h if a smaller area is desired
  #ifndef MESH_MIN_X
    #define MESH_MIN_X _MESH_MIN_X
  #endif
  #ifndef MESH_MIN_Y
    #define MESH_MIN_Y _MESH_MIN_Y
  #endif
  #ifndef MESH_MAX_X
    #define MESH_MAX_X _MESH_MAX_X
  #endif
  #ifndef MESH_MAX_Y
    #define MESH_MAX_Y _MESH_MAX_Y
  #endif
#else
  #undef MESH_MIN_X
  #undef MESH_MIN_Y
  #undef MESH_MAX_X
  #undef MESH_MAX_Y
#endif

#define _POINT_COUNT (defined(PROBE_PT_1_X) + defined(PROBE_PT_2_X) + defined(PROBE_PT_3_X) + defined(PROBE_PT_1_Y) + defined(PROBE_PT_2_Y) + defined(PROBE_PT_3_Y))
#if _POINT_COUNT == 6
  #define HAS_FIXED_3POINT 1
#elif _POINT_COUNT > 0
  #error "For 3-Point Leveling all XY points must be defined (or none for the defaults)."
#endif
#undef _POINT_COUNT

/**
 * Buzzer/Speaker
 */
#if PIN_EXISTS(BEEPER)
  #define HAS_BEEPER 1
#endif
#if ANY(IS_TFTGLCD_PANEL, PCA9632_BUZZER, LCD_USE_I2C_BUZZER)
  #define USE_MARLINUI_BUZZER 1
#endif
#if EITHER(HAS_BEEPER, USE_MARLINUI_BUZZER)
  #define HAS_SOUND 1
#endif

#if ENABLED(LCD_USE_I2C_BUZZER)
  #ifndef LCD_FEEDBACK_FREQUENCY_HZ
    #define LCD_FEEDBACK_FREQUENCY_HZ 1000
  #endif
  #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
    #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 100
  #endif
#elif HAS_SOUND
  #ifndef LCD_FEEDBACK_FREQUENCY_HZ
    #define LCD_FEEDBACK_FREQUENCY_HZ 5000
  #endif
  #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
    #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 2
  #endif
#endif

#if HAS_SOUND
  #if LCD_FEEDBACK_FREQUENCY_DURATION_MS && LCD_FEEDBACK_FREQUENCY_HZ
    #define HAS_CHIRP 1
  #endif
#else
  #undef SOUND_MENU_ITEM   // No buzzer menu item without a buzzer
  #undef SOUND_ON_DEFAULT
#endif

/**
 * Make sure DOGLCD_SCK and DOGLCD_MOSI are defined.
 */
#if HAS_MARLINUI_U8GLIB
  #ifndef DOGLCD_SCK
    #define DOGLCD_SCK  SD_SCK_PIN
  #endif
  #ifndef DOGLCD_MOSI
    #define DOGLCD_MOSI SD_MOSI_PIN
  #endif
#endif

/**
 * Z_HOMING_HEIGHT / Z_CLEARANCE_BETWEEN_PROBES
 */
#ifndef Z_HOMING_HEIGHT
  #ifdef Z_CLEARANCE_BETWEEN_PROBES
    #define Z_HOMING_HEIGHT Z_CLEARANCE_BETWEEN_PROBES
  #else
    #define Z_HOMING_HEIGHT 0
  #endif
#endif

#if PROBE_SELECTED
  #ifndef Z_CLEARANCE_BETWEEN_PROBES
    #define Z_CLEARANCE_BETWEEN_PROBES Z_HOMING_HEIGHT
  #endif
  #if Z_CLEARANCE_BETWEEN_PROBES > Z_HOMING_HEIGHT
    #define Z_CLEARANCE_BETWEEN_MANUAL_PROBES Z_CLEARANCE_BETWEEN_PROBES
  #else
    #define Z_CLEARANCE_BETWEEN_MANUAL_PROBES Z_HOMING_HEIGHT
  #endif
  #ifndef Z_CLEARANCE_MULTI_PROBE
    #define Z_CLEARANCE_MULTI_PROBE Z_CLEARANCE_BETWEEN_PROBES
  #endif
  #if ENABLED(BLTOUCH) && !defined(BLTOUCH_DELAY)
    #define BLTOUCH_DELAY 500
  #endif
#endif

// Define a starting height for measuring manual probe points
#ifndef MANUAL_PROBE_START_Z
  #if EITHER(MESH_BED_LEVELING, PROBE_MANUALLY)
    // Leave MANUAL_PROBE_START_Z undefined so the prior Z height will be used.
    // Note: If Z_CLEARANCE_BETWEEN_MANUAL_PROBES is 0 there will be no raise between points
  #elif ENABLED(AUTO_BED_LEVELING_UBL) && defined(Z_CLEARANCE_BETWEEN_PROBES)
    #define MANUAL_PROBE_START_Z Z_CLEARANCE_BETWEEN_PROBES
  #endif
#endif

#ifndef __SAM3X8E__ //todo: hal: broken hal encapsulation
  #undef UI_VOLTAGE_LEVEL
  #undef RADDS_DISPLAY
  #undef MOTOR_CURRENT
#endif

// Updated G92 behavior shifts the workspace
#if DISABLED(NO_WORKSPACE_OFFSETS)
  #define HAS_POSITION_SHIFT 1
  #if IS_CARTESIAN
    #define HAS_HOME_OFFSET 1       // The home offset also shifts the coordinate space
    #define HAS_WORKSPACE_OFFSET 1  // Cumulative offset to workspace to save some calculation
    #define HAS_M206_COMMAND 1      // M206 sets the home offset for Cartesian machines
  #elif IS_SCARA
    #define HAS_SCARA_OFFSET 1      // The SCARA home offset applies only on G28
  #endif
#endif

#if EITHER(HAS_MARLINUI_MENU, TOUCH_UI_FTDI_EVE)
  // LCD timeout to status screen default is 15s
  #ifndef LCD_TIMEOUT_TO_STATUS
    #define LCD_TIMEOUT_TO_STATUS 15000
  #endif
  #if LCD_TIMEOUT_TO_STATUS
    #define HAS_SCREEN_TIMEOUT 1
  #endif
#endif

// Add commands that need sub-codes to this list
#if ANY(G38_PROBE_TARGET, CNC_COORDINATE_SYSTEMS, POWER_LOSS_RECOVERY)
  #define USE_GCODE_SUBCODES 1
#endif

// Parking Extruder
#if ENABLED(PARKING_EXTRUDER)
  #ifndef PARKING_EXTRUDER_GRAB_DISTANCE
    #define PARKING_EXTRUDER_GRAB_DISTANCE 0
  #endif
  #ifndef PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
    #define PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE HIGH
  #endif
#endif

// Number of VFAT entries used. Each entry has 13 UTF-16 characters
#if EITHER(SCROLL_LONG_FILENAMES, HAS_DWIN_E3V2)
  #define MAX_VFAT_ENTRIES (5)
#else
  #define MAX_VFAT_ENTRIES (2)
#endif

// Nozzle park for Delta
#if BOTH(NOZZLE_PARK_FEATURE, DELTA)
  #undef NOZZLE_PARK_Z_FEEDRATE
  #define NOZZLE_PARK_Z_FEEDRATE NOZZLE_PARK_XY_FEEDRATE
#endif

// Force SDCARD_SORT_ALPHA to be enabled for Graphical LCD on LPC1768
// on boards where SD card and LCD display share the same SPI bus
// because of a bug in the shared SPI implementation. (See #8122)
#if defined(TARGET_LPC1768) && IS_RRD_FG_SC && (SD_SCK_PIN == LCD_PINS_D4)
  #define SDCARD_SORT_ALPHA         // Keep one directory level in RAM. Changing directory levels
                                    // may still glitch the screen, but LCD updates clean it up.
  #undef SDSORT_LIMIT
  #undef SDSORT_USES_RAM
  #undef SDSORT_USES_STACK
  #undef SDSORT_CACHE_NAMES
  #define SDSORT_LIMIT       64
  #define SDSORT_USES_RAM    true
  #define SDSORT_USES_STACK  false
  #define SDSORT_CACHE_NAMES true
  #ifndef FOLDER_SORTING
    #define FOLDER_SORTING     -1
  #endif
  #ifndef SDSORT_GCODE
    #define SDSORT_GCODE       false
  #endif
  #ifndef SDSORT_DYNAMIC_RAM
    #define SDSORT_DYNAMIC_RAM false
  #endif
  #ifndef SDSORT_CACHE_VFATS
    #define SDSORT_CACHE_VFATS 2
  #endif
#endif

// Fallback SPI Speed for SD
#if ENABLED(SDSUPPORT) && !defined(SD_SPI_SPEED)
  #define SD_SPI_SPEED SPI_FULL_SPEED
#endif

#if HAS_WIRED_LCD
  // Get LCD character width/height, which may be overridden by pins, configs, etc.
  #ifndef LCD_WIDTH
    #if HAS_MARLINUI_U8GLIB
      #define LCD_WIDTH 21
    #elif IS_DWIN_MARLINUI
      // Defined by header
    #else
      #define LCD_WIDTH TERN(IS_ULTIPANEL, 20, 16)
    #endif
  #endif
  #ifndef LCD_HEIGHT
    #if HAS_MARLINUI_U8GLIB
      #define LCD_HEIGHT 5
    #elif IS_DWIN_MARLINUI
      // Defined by header
    #else
      #define LCD_HEIGHT TERN(IS_ULTIPANEL, 4, 2)
    #endif
  #endif
#endif

#if BUTTONS_EXIST(EN1, EN2, ENC)
  #define HAS_ROTARY_ENCODER 1
#endif

#if PIN_EXISTS(SAFE_POWER) && DISABLED(DISABLE_DRIVER_SAFE_POWER_PROTECT)
  #define HAS_DRIVER_SAFE_POWER_PROTECT 1
#endif

#if ANY(ENDSTOPPULLDOWNS, ENDSTOPPULLDOWN_ZMIN_PROBE, \
    ENDSTOPPULLDOWN_XMIN, ENDSTOPPULLDOWN_YMIN, ENDSTOPPULLDOWN_ZMIN, \
    ENDSTOPPULLDOWN_IMIN, ENDSTOPPULLDOWN_JMIN, ENDSTOPPULLDOWN_KMIN, \
    ENDSTOPPULLDOWN_XMAX, ENDSTOPPULLDOWN_YMAX, ENDSTOPPULLDOWN_ZMAX, \
    ENDSTOPPULLDOWN_IMAX, ENDSTOPPULLDOWN_JMAX, ENDSTOPPULLDOWN_KMAX, \
    POWER_LOSS_PULLDOWN, CALIBRATION_PIN_PULLDOWN, FIL_RUNOUT_PULLDOWN, \
    FIL_RUNOUT1_PULLDOWN, FIL_RUNOUT2_PULLDOWN, FIL_RUNOUT3_PULLDOWN, FIL_RUNOUT4_PULLDOWN, \
    FIL_RUNOUT5_PULLDOWN, FIL_RUNOUT6_PULLDOWN, FIL_RUNOUT7_PULLDOWN, FIL_RUNOUT8_PULLDOWN)
  #define USING_PULLDOWNS 1
#endif
