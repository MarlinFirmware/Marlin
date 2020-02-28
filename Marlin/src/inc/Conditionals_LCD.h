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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Conditionals_LCD.h
 * Conditionals that need to be set before Configuration_adv.h or pins.h
 */

#if ENABLED(CARTESIO_UI)

  #define DOGLCD
  #define IS_ULTIPANEL

#elif ENABLED(ZONESTAR_LCD)

  #define ADC_KEYPAD
  #define IS_RRW_KEYPAD
  #define REPRAPWORLD_KEYPAD_MOVE_STEP 10.0
  #define ADC_KEY_NUM 8
  #define IS_ULTIPANEL

  // This helps to implement ADC_KEYPAD menus
  #define REVERSE_MENU_DIRECTION
  #define ENCODER_PULSES_PER_STEP 1
  #define ENCODER_STEPS_PER_MENU_ITEM 1
  #define ENCODER_FEEDRATE_DEADZONE 2

#elif ENABLED(RADDS_DISPLAY)
  #define IS_ULTIPANEL
  #define ENCODER_PULSES_PER_STEP 2

#elif EITHER(ANET_FULL_GRAPHICS_LCD, BQ_LCD_SMART_CONTROLLER)

  #define IS_RRD_FG_SC

#elif ANY(miniVIKI, VIKI2, ELB_FULL_GRAPHIC_CONTROLLER, AZSMZ_12864)

  #define IS_ULTRA_LCD
  #define DOGLCD
  #define IS_ULTIPANEL

  #if ENABLED(miniVIKI)
    #define U8GLIB_ST7565_64128N
  #elif ENABLED(VIKI2)
    #define U8GLIB_ST7565_64128N
  #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
    #define U8GLIB_LM6059_AF
    #define SD_DETECT_INVERTED
  #elif ENABLED(AZSMZ_12864)
    #define U8GLIB_ST7565_64128N
  #endif

#elif ENABLED(OLED_PANEL_TINYBOY2)

  #define IS_U8GLIB_SSD1306
  #define IS_ULTIPANEL

#elif ENABLED(RA_CONTROL_PANEL)

  #define LCD_I2C_TYPE_PCA8574
  #define LCD_I2C_ADDRESS 0x27   // I2C Address of the port expander
  #define IS_ULTIPANEL

#elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

  #define DOGLCD
  #define U8GLIB_ST7920
  #define IS_ULTIPANEL

#elif ENABLED(CR10_STOCKDISPLAY)

  #define IS_RRD_FG_SC
  #ifndef ST7920_DELAY_1
    #define ST7920_DELAY_1 DELAY_NS(125)
  #endif
  #ifndef ST7920_DELAY_2
    #define ST7920_DELAY_2 DELAY_NS(125)
  #endif
  #ifndef ST7920_DELAY_3
    #define ST7920_DELAY_3 DELAY_NS(125)
  #endif

#elif ENABLED(MKS_12864OLED)

  #define IS_RRD_SC
  #define U8GLIB_SH1106

#elif ENABLED(MKS_12864OLED_SSD1306)

  #define IS_RRD_SC
  #define IS_U8GLIB_SSD1306

#elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

  #define MINIPANEL

#elif ANY(FYSETC_MINI_12864_X_X, FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1, FYSETC_GENERIC_12864_1_1)

  #define FYSETC_MINI_12864
  #define DOGLCD
  #define IS_ULTIPANEL
  #define LED_COLORS_REDUCE_GREEN
  #if ENABLED(PSU_CONTROL) && EITHER(FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1)
    #define LED_BACKLIGHT_TIMEOUT 10000
  #endif

  // Require LED backlighting enabled
  #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
    #define RGB_LED
  #elif ENABLED(FYSETC_MINI_12864_2_1)
    #define LED_CONTROL_MENU
    #define NEOPIXEL_LED
    #undef NEOPIXEL_TYPE
    #define NEOPIXEL_TYPE       NEO_RGB
    #if NEOPIXEL_PIXELS < 3
      #undef NEOPIXELS_PIXELS
      #define NEOPIXEL_PIXELS     3
    #endif
    #ifndef NEOPIXEL_BRIGHTNESS
      #define NEOPIXEL_BRIGHTNESS 127
    #endif
    //#define NEOPIXEL_STARTUP_TEST
  #endif

#elif ENABLED(ULTI_CONTROLLER)

  #define IS_ULTIPANEL
  #define U8GLIB_SSD1309
  #define LCD_RESET_PIN LCD_PINS_D6 //  This controller need a reset pin
  #define ENCODER_PULSES_PER_STEP 2
  #define ENCODER_STEPS_PER_MENU_ITEM 2

#elif ENABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602)

  #define IS_RRD_SC
  #define LCD_WIDTH 16
  #define LCD_HEIGHT 2

#endif

#if ENABLED(IS_RRD_FG_SC)
  #define REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
#endif

#if EITHER(MAKRPANEL, MINIPANEL)
  #define IS_ULTIPANEL
  #define DOGLCD
  #if ENABLED(MAKRPANEL)
    #define U8GLIB_ST7565_64128N
  #endif
#endif

#if ENABLED(IS_U8GLIB_SSD1306)
  #define U8GLIB_SSD1306
#endif

#if ENABLED(OVERLORD_OLED)
  #define IS_ULTIPANEL
  #define U8GLIB_SH1106
  /**
   * PCA9632 for buzzer and LEDs via i2c
   * No auto-inc, red and green leds switched, buzzer
   */
  #define PCA9632
  #define PCA9632_NO_AUTO_INC
  #define PCA9632_GRN         0x00
  #define PCA9632_RED         0x02
  #define PCA9632_BUZZER
  #define PCA9632_BUZZER_DATA { 0x09, 0x02 }

  #define ENCODER_PULSES_PER_STEP     1 // Overlord uses buttons
  #define ENCODER_STEPS_PER_MENU_ITEM 1
#endif

// 128x64 I2C OLED LCDs - SSD1306/SSD1309/SH1106
#define HAS_SSD1306_OLED_I2C ANY(U8GLIB_SSD1306, U8GLIB_SSD1309, U8GLIB_SH1106)
#if HAS_SSD1306_OLED_I2C
  #define IS_ULTRA_LCD
  #define DOGLCD
#endif

// ST7920-based graphical displays
#if ANY(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER, LCD_FOR_MELZI, SILVER_GATE_GLCD_CONTROLLER)
  #define DOGLCD
  #define U8GLIB_ST7920
  #define IS_RRD_SC
#endif

// RepRapDiscount LCD or Graphical LCD with rotary click encoder
#if ENABLED(IS_RRD_SC)
  #define REPRAP_DISCOUNT_SMART_CONTROLLER
#endif

/**
 * SPI Ultipanels
 */

// Basic Ultipanel-like displays
#if ANY(ULTIMAKERCONTROLLER, REPRAP_DISCOUNT_SMART_CONTROLLER, G3D_PANEL, RIGIDBOT_PANEL, PANEL_ONE, U8GLIB_SH1106)
  #define IS_ULTIPANEL
#endif

// Einstart OLED has Cardinal nav via pins defined in pins_EINSTART-S.h
#if ENABLED(U8GLIB_SH1106_EINSTART)
  #define DOGLCD
  #define IS_ULTIPANEL
#endif

// FSMC/SPI TFT Panels
#if ENABLED(FSMC_GRAPHICAL_TFT)
  #define DOGLCD
  #define IS_ULTIPANEL
  #define DELAYED_BACKLIGHT_INIT
#endif

/**
 * I2C Panels
 */

#if EITHER(LCD_SAINSMART_I2C_1602, LCD_SAINSMART_I2C_2004)

  #define LCD_I2C_TYPE_PCF8575
  #define LCD_I2C_ADDRESS 0x27   // I2C Address of the port expander

  #if ENABLED(LCD_SAINSMART_I2C_2004)
    #define LCD_WIDTH 20
    #define LCD_HEIGHT 4
  #endif

#elif ENABLED(LCD_I2C_PANELOLU2)

  // PANELOLU2 LCD with status LEDs, separate encoder and click inputs

  #define LCD_I2C_TYPE_MCP23017
  #define LCD_I2C_ADDRESS 0x20 // I2C Address of the port expander
  #define LCD_USE_I2C_BUZZER   // Enable buzzer on LCD (optional)
  #define IS_ULTIPANEL

#elif ENABLED(LCD_I2C_VIKI)

  /**
   * Panucatt VIKI LCD with status LEDs, integrated click & L/R/U/P buttons, separate encoder inputs
   *
   * This uses the LiquidTWI2 library v1.2.3 or later ( https://github.com/lincomatic/LiquidTWI2 )
   * Make sure the LiquidTWI2 directory is placed in the Arduino or Sketchbook libraries subdirectory.
   * Note: The pause/stop/resume LCD button pin should be connected to the Arduino
   *       BTN_ENC pin (or set BTN_ENC to -1 if not used)
   */
  #define LCD_I2C_TYPE_MCP23017
  #define LCD_I2C_ADDRESS 0x20 // I2C Address of the port expander
  #define LCD_USE_I2C_BUZZER   // Enable buzzer on LCD (requires LiquidTWI2 v1.2.3 or later)
  #define IS_ULTIPANEL

  #define ENCODER_FEEDRATE_DEADZONE 4

  #define STD_ENCODER_PULSES_PER_STEP 1
  #define STD_ENCODER_STEPS_PER_MENU_ITEM 2

#elif ENABLED(G3D_PANEL)

  #define STD_ENCODER_PULSES_PER_STEP 2
  #define STD_ENCODER_STEPS_PER_MENU_ITEM 1

#elif ANY(REPRAP_DISCOUNT_SMART_CONTROLLER, miniVIKI, VIKI2, ELB_FULL_GRAPHIC_CONTROLLER, AZSMZ_12864, OLED_PANEL_TINYBOY2, BQ_LCD_SMART_CONTROLLER, LCD_I2C_PANELOLU2)

  #define STD_ENCODER_PULSES_PER_STEP 4
  #define STD_ENCODER_STEPS_PER_MENU_ITEM 1

#endif

#ifndef STD_ENCODER_PULSES_PER_STEP
  #if ENABLED(TOUCH_BUTTONS)
    #define STD_ENCODER_PULSES_PER_STEP 2
  #else
    #define STD_ENCODER_PULSES_PER_STEP 5
  #endif
#endif
#ifndef STD_ENCODER_STEPS_PER_MENU_ITEM
  #define STD_ENCODER_STEPS_PER_MENU_ITEM 1
#endif
#ifndef ENCODER_PULSES_PER_STEP
  #define ENCODER_PULSES_PER_STEP STD_ENCODER_PULSES_PER_STEP
#endif
#ifndef ENCODER_STEPS_PER_MENU_ITEM
  #define ENCODER_STEPS_PER_MENU_ITEM STD_ENCODER_STEPS_PER_MENU_ITEM
#endif
#ifndef ENCODER_FEEDRATE_DEADZONE
  #define ENCODER_FEEDRATE_DEADZONE 6
#endif

// Shift register panels
// ---------------------
// 2 wire Non-latching LCD SR from:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!shiftregister-connection
#if ENABLED(FF_INTERFACEBOARD)
  #define SR_LCD_3W_NL    // Non latching 3 wire shift register
  #define IS_ULTIPANEL
#elif ENABLED(SAV_3DLCD)
  #define SR_LCD_2W_NL    // Non latching 2 wire shift register
  #define IS_ULTIPANEL
#endif

#if ENABLED(IS_ULTIPANEL)
  #define ULTIPANEL
#endif
#if ENABLED(ULTIPANEL)
  #define IS_ULTRA_LCD
  #ifndef NEWPANEL
    #define NEWPANEL
  #endif
#endif

#if ENABLED(IS_ULTRA_LCD)
  #define ULTRA_LCD
#endif

#if ENABLED(IS_RRW_KEYPAD)
  #define REPRAPWORLD_KEYPAD
#endif

// Keypad needs a move step
#if ENABLED(REPRAPWORLD_KEYPAD)
  #define NEWPANEL
  #ifndef REPRAPWORLD_KEYPAD_MOVE_STEP
    #define REPRAPWORLD_KEYPAD_MOVE_STEP 1.0
  #endif
#endif

// Extensible UI serial touch screens. (See src/lcd/extensible_ui)
#if ANY(MALYAN_LCD, DGUS_LCD, TOUCH_UI_FTDI_EVE)
  #define IS_EXTUI
  #define EXTENSIBLE_UI
#endif

// Aliases for LCD features
#define HAS_SPI_LCD          ENABLED(ULTRA_LCD)
#define HAS_DISPLAY         (HAS_SPI_LCD || ENABLED(EXTENSIBLE_UI))
#define HAS_GRAPHICAL_LCD    ENABLED(DOGLCD)
#define HAS_CHARACTER_LCD   (HAS_SPI_LCD && !HAS_GRAPHICAL_LCD)
#define HAS_LCD_MENU        (ENABLED(ULTIPANEL) && DISABLED(NO_LCD_MENUS))
#define HAS_ADC_BUTTONS      ENABLED(ADC_KEYPAD)
#define HAS_DGUS_LCD         ANY(DGUS_LCD_UI_ORIGIN, DGUS_LCD_UI_FYSETC, DGUS_LCD_UI_HIPRECY)

#if HAS_GRAPHICAL_LCD
  #ifndef LCD_PIXEL_WIDTH
    #define LCD_PIXEL_WIDTH 128
  #endif
  #ifndef LCD_PIXEL_HEIGHT
    #define LCD_PIXEL_HEIGHT 64
  #endif
#endif

/**
 * Extruders have some combination of stepper motors and hotends
 * so we separate these concepts into the defines:
 *
 *  EXTRUDERS    - Number of Selectable Tools
 *  HOTENDS      - Number of hotends, whether connected or separate
 *  E_STEPPERS   - Number of actual E stepper motors
 *  E_MANUAL     - Number of E steppers for LCD move options
 *
 */

#if EXTRUDERS == 0
  #undef DISTINCT_E_FACTORS
  #undef SINGLENOZZLE
  #undef SWITCHING_EXTRUDER
  #undef SWITCHING_NOZZLE
  #undef MIXING_EXTRUDER
  #undef MK2_MULTIPLEXER
  #undef PRUSA_MMU2
#endif

#if ENABLED(SWITCHING_EXTRUDER)   // One stepper for every two EXTRUDERS
  #if EXTRUDERS > 4
    #define E_STEPPERS    3
  #elif EXTRUDERS > 2
    #define E_STEPPERS    2
  #else
    #define E_STEPPERS    1
  #endif
  #if DISABLED(SWITCHING_NOZZLE)
    #define HOTENDS       E_STEPPERS
  #endif
#elif ENABLED(MIXING_EXTRUDER)
  #define E_STEPPERS      MIXING_STEPPERS
  #define E_MANUAL        1
  #define DUAL_MIXING_EXTRUDER (MIXING_STEPPERS == 2)
#elif ENABLED(SWITCHING_TOOLHEAD)
  #define E_STEPPERS      EXTRUDERS
  #define E_MANUAL        EXTRUDERS
#elif ENABLED(PRUSA_MMU2)
  #define E_STEPPERS 1
#endif

// No inactive extruders with MK2_MULTIPLEXER or SWITCHING_NOZZLE
#if EITHER(MK2_MULTIPLEXER, SWITCHING_NOZZLE)
  #undef DISABLE_INACTIVE_EXTRUDER
#endif

// Prusa MK2 Multiplexer and MMU 2.0 force SINGLENOZZLE
#if EITHER(MK2_MULTIPLEXER, PRUSA_MMU2)
  #define SINGLENOZZLE
#endif

#if EITHER(SINGLENOZZLE, MIXING_EXTRUDER)         // One hotend, one thermistor, no XY offset
  #undef HOTENDS
  #define HOTENDS       1
  #undef HOTEND_OFFSET_X
  #undef HOTEND_OFFSET_Y
#endif

#ifndef HOTENDS
  #define HOTENDS EXTRUDERS
#endif
#ifndef E_STEPPERS
  #define E_STEPPERS EXTRUDERS
#endif
#ifndef E_MANUAL
  #define E_MANUAL EXTRUDERS
#endif

// Helper macros for extruder and hotend arrays
#define HOTEND_LOOP() for (int8_t e = 0; e < HOTENDS; e++)
#define ARRAY_BY_EXTRUDERS(V...) ARRAY_N(EXTRUDERS, V)
#define ARRAY_BY_EXTRUDERS1(v1) ARRAY_BY_EXTRUDERS(v1, v1, v1, v1, v1, v1)
#define ARRAY_BY_HOTENDS(V...) ARRAY_N(HOTENDS, V)
#define ARRAY_BY_HOTENDS1(v1) ARRAY_BY_HOTENDS(v1, v1, v1, v1, v1, v1)

#define DO_SWITCH_EXTRUDER (ENABLED(SWITCHING_EXTRUDER) && (DISABLED(SWITCHING_NOZZLE) || SWITCHING_EXTRUDER_SERVO_NR != SWITCHING_NOZZLE_SERVO_NR))
#define SWITCHING_NOZZLE_TWO_SERVOS defined(SWITCHING_NOZZLE_E1_SERVO_NR)

#define HAS_DUPLICATION_MODE EITHER(DUAL_X_CARRIAGE, MULTI_NOZZLE_DUPLICATION)

#define HAS_HOTEND_OFFSET (HOTENDS > 1)

/**
 * Default hotend offsets, if not defined
 */
#if HAS_HOTEND_OFFSET
  #ifndef HOTEND_OFFSET_X
    #define HOTEND_OFFSET_X { 0 } // X offsets for each extruder
  #endif
  #ifndef HOTEND_OFFSET_Y
    #define HOTEND_OFFSET_Y { 0 } // Y offsets for each extruder
  #endif
  #ifndef HOTEND_OFFSET_Z
    #define HOTEND_OFFSET_Z { 0 } // Z offsets for each extruder
  #endif
#endif

/**
 * DISTINCT_E_FACTORS affects how some E factors are accessed
 */
#if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
  #define XYZE_N (XYZ + E_STEPPERS)
  #define E_AXIS_N(E) AxisEnum(E_AXIS + E)
  #define UNUSED_E(E) NOOP
#else
  #undef DISTINCT_E_FACTORS
  #define XYZE_N XYZE
  #define E_AXIS_N(E) E_AXIS
  #define UNUSED_E(E) UNUSED(E)
#endif

/**
 * The BLTouch Probe emulates a servo probe
 * and uses "special" angles for its state.
 */
#if ENABLED(BLTOUCH)
  #ifndef Z_PROBE_SERVO_NR
    #define Z_PROBE_SERVO_NR 0
  #endif
  #ifndef NUM_SERVOS
    #define NUM_SERVOS (Z_PROBE_SERVO_NR + 1)
  #endif
  #undef DEACTIVATE_SERVOS_AFTER_MOVE
  #if NUM_SERVOS == 1
    #undef SERVO_DELAY
    #define SERVO_DELAY { 50 }
  #endif

  // Always disable probe pin inverting for BLTouch
  #undef Z_MIN_PROBE_ENDSTOP_INVERTING
  #define Z_MIN_PROBE_ENDSTOP_INVERTING false
  #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #undef Z_MIN_ENDSTOP_INVERTING
    #define Z_MIN_ENDSTOP_INVERTING false
  #endif
#endif

#ifndef PREHEAT_1_LABEL
  #define PREHEAT_1_LABEL "PLA"
#endif

#ifndef PREHEAT_2_LABEL
  #define PREHEAT_2_LABEL "ABS"
#endif

/**
 * Set a flag for a servo probe (or BLTouch)
 */
#define HAS_Z_SERVO_PROBE (defined(Z_PROBE_SERVO_NR) && Z_PROBE_SERVO_NR >= 0)
#define HAS_SERVO_ANGLES (HAS_Z_SERVO_PROBE || EITHER(SWITCHING_EXTRUDER, SWITCHING_NOZZLE))
#if !HAS_SERVO_ANGLES
  #undef EDITABLE_SERVO_ANGLES
#endif

/**
 * Set flags for enabled probes
 */
#define HAS_BED_PROBE (HAS_Z_SERVO_PROBE || ANY(FIX_MOUNTED_PROBE, NOZZLE_AS_PROBE, TOUCH_MI_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, SOLENOID_PROBE, SENSORLESS_PROBING, RACK_AND_PINION_PROBE))
#define PROBE_SELECTED (HAS_BED_PROBE || EITHER(PROBE_MANUALLY, MESH_BED_LEVELING))

#if HAS_BED_PROBE
  #define HAS_PROBE_XY_OFFSET   DISABLED(NOZZLE_AS_PROBE)
  #define HAS_CUSTOM_PROBE_PIN  DISABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
  #define HOMING_Z_WITH_PROBE   (Z_HOME_DIR < 0 && !HAS_CUSTOM_PROBE_PIN)
  #ifndef Z_PROBE_LOW_POINT
    #define Z_PROBE_LOW_POINT -5
  #endif
  #if ENABLED(Z_PROBE_ALLEN_KEY)
    #define PROBE_TRIGGERED_WHEN_STOWED_TEST // Extra test for Allen Key Probe
  #endif
  #ifdef MULTIPLE_PROBING
    #if EXTRA_PROBING
      #define TOTAL_PROBING (MULTIPLE_PROBING + EXTRA_PROBING)
    #else
      #define TOTAL_PROBING MULTIPLE_PROBING
    #endif
  #endif
#else
  // Clear probe pin settings when no probe is selected
  #undef Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
#endif

#ifdef GRID_MAX_POINTS_X
  #define GRID_MAX_POINTS ((GRID_MAX_POINTS_X) * (GRID_MAX_POINTS_Y))
#endif

#define HAS_EXTRA_ENDSTOPS           ANY(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS, Z_MULTI_ENDSTOPS)
#define HAS_SOFTWARE_ENDSTOPS        EITHER(MIN_SOFTWARE_ENDSTOPS, MAX_SOFTWARE_ENDSTOPS)
#define HAS_RESUME_CONTINUE          ANY(EXTENSIBLE_UI, NEWPANEL, EMERGENCY_PARSER)
#define HAS_COLOR_LEDS               ANY(BLINKM, RGB_LED, RGBW_LED, PCA9632, PCA9533, NEOPIXEL_LED)
#define HAS_LEDS_OFF_FLAG            (BOTH(PRINTER_EVENT_LEDS, SDSUPPORT) && HAS_RESUME_CONTINUE)
#define HAS_PRINT_PROGRESS           EITHER(SDSUPPORT, LCD_SET_PROGRESS_MANUALLY)
#define HAS_PRINT_PROGRESS_PERMYRIAD (HAS_PRINT_PROGRESS && EITHER(PRINT_PROGRESS_SHOW_DECIMALS, SHOW_REMAINING_TIME))
#define HAS_SERVICE_INTERVALS        (ENABLED(PRINTCOUNTER) && (SERVICE_INTERVAL_1 > 0 || SERVICE_INTERVAL_2 > 0 || SERVICE_INTERVAL_3 > 0))
#define HAS_FILAMENT_SENSOR          ENABLED(FILAMENT_RUNOUT_SENSOR)

#define HAS_GAMES     ANY(MARLIN_BRICKOUT, MARLIN_INVADERS, MARLIN_SNAKE, MARLIN_MAZE)
#define HAS_GAME_MENU (1 < ENABLED(MARLIN_BRICKOUT) + ENABLED(MARLIN_INVADERS) + ENABLED(MARLIN_SNAKE) + ENABLED(MARLIN_MAZE))

#define IS_SCARA     ENABLED(MORGAN_SCARA)
#define IS_KINEMATIC (ENABLED(DELTA) || IS_SCARA)
#define IS_CARTESIAN !IS_KINEMATIC

#ifndef INVERT_X_DIR
  #define INVERT_X_DIR false
#endif
#ifndef INVERT_Y_DIR
  #define INVERT_Y_DIR false
#endif
#ifndef INVERT_Z_DIR
  #define INVERT_Z_DIR false
#endif
#ifndef INVERT_E_DIR
  #define INVERT_E_DIR false
#endif

#if ENABLED(SLIM_LCD_MENUS)
  #define BOOT_MARLIN_LOGO_SMALL
#endif

#define IS_RE_ARM_BOARD MB(RAMPS_14_RE_ARM_EFB, RAMPS_14_RE_ARM_EEB, RAMPS_14_RE_ARM_EFF, RAMPS_14_RE_ARM_EEF, RAMPS_14_RE_ARM_SF)

// Linear advance uses Jerk since E is an isolated axis
#define HAS_LINEAR_E_JERK  (DISABLED(CLASSIC_JERK) && ENABLED(LIN_ADVANCE))

// This flag indicates some kind of jerk storage is needed
#define HAS_CLASSIC_JERK   (ENABLED(CLASSIC_JERK) || IS_KINEMATIC)

// E jerk exists with JD disabled (of course) but also when Linear Advance is disabled on Delta/SCARA
#define HAS_CLASSIC_E_JERK (ENABLED(CLASSIC_JERK) || (IS_KINEMATIC && DISABLED(LIN_ADVANCE)))

#ifndef SPI_SPEED
  #define SPI_SPEED SPI_FULL_SPEED
#endif

/**
 * This setting is also used by M109 when trying to calculate
 * a ballpark safe margin to prevent wait-forever situation.
 */
#ifndef EXTRUDE_MINTEMP
  #define EXTRUDE_MINTEMP 170
#endif
