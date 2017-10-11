#ifndef BOARD_NAME
  #define BOARD_NAME "ESP32"
#endif

#define LARGE_FLASH true

// //
// // Servos
// //
// #ifdef IS_RAMPS_13
//   #define SERVO0_PIN        7 // RAMPS_13 // Will conflict with BTN_EN2 on LCD_I2C_VIKI
// #else
//   #define SERVO0_PIN       11
// #endif
// #define SERVO1_PIN          6
// #define SERVO2_PIN          5
// #define SERVO3_PIN          4

//
// Limit Switches
//
#define X_MIN_PIN          19
#define Y_MIN_PIN          18
#define Z_MIN_PIN          5

//
// Z Probe (when not Z_MIN_PIN)
//

//#define Z_MIN_PROBE_PIN  5

#define SLED_PIN           -1

//
// Steppers
//
#define X_STEP_PIN         25
#define X_DIR_PIN          33
#define X_ENABLE_PIN       128
#define X_CS_PIN           -1

#define Y_STEP_PIN         16
#define Y_DIR_PIN          17
#define Y_ENABLE_PIN       129
#define Y_CS_PIN           -1

#define Z_STEP_PIN         2
#define Z_DIR_PIN          4
#define Z_ENABLE_PIN       130
#define Z_CS_PIN           -1

#define E0_STEP_PIN        27
#define E0_DIR_PIN         26
#define E0_ENABLE_PIN      131
#define E0_CS_PIN          -1

//
// Temperature Sensors
//
#define TEMP_0_PIN         34   // Analog Input
#define TEMP_BED_PIN       35   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS       66 // Do not use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS       66 // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

//
// Augmentation for auto-assigning RAMPS plugs
//
#if DISABLED(IS_RAMPS_EEB) && DISABLED(IS_RAMPS_EEF) && DISABLED(IS_RAMPS_EFB) && DISABLED(IS_RAMPS_EFF) && DISABLED(IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  #if HOTENDS > 1
    #if TEMP_SENSOR_BED
      #define IS_RAMPS_EEB
    #else
      #define IS_RAMPS_EEF
    #endif
  #elif TEMP_SENSOR_BED
    #define IS_RAMPS_EFB
  #else
    #define IS_RAMPS_EFF
  #endif
#endif

//
// Heaters / Fans
//
#ifndef MOSFET_D_PIN
  #define MOSFET_D_PIN  -1
#endif
#ifndef RAMPS_D8_PIN
  #define RAMPS_D8_PIN   8
#endif
#ifndef RAMPS_D9_PIN
  #define RAMPS_D9_PIN   9
#endif
#ifndef RAMPS_D10_PIN
  #define RAMPS_D10_PIN 10
#endif

#define HEATER_0_PIN     5
// #define FAN_PIN          4

#define HEATER_BED_PIN   34

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13

// Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
#define FILWIDTH_PIN        5   // Analog Input

// define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
#define FIL_RUNOUT_PIN      4

#define PS_ON_PIN          12