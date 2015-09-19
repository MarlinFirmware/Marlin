/**
 * Mini-Rambo pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560 or Rambo' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH         true


#undef X_MS1_PIN
#undef X_MS2_PIN
#undef Y_MS1_PIN
#undef Y_MS2_PIN
#undef Z_MS1_PIN
#undef Z_MS2_PIN
#undef E0_MS1_PIN
#undef E0_MS2_PIN
#undef E1_MS1_PIN
#undef E1_MS2_PIN

#define X_STEP_PIN          37
#define X_DIR_PIN           48
#define X_MIN_PIN           12
#define X_MAX_PIN           30
#define X_ENABLE_PIN        29
#define X_MS1_PIN           40
#define X_MS2_PIN           41

#define Y_STEP_PIN          36
#define Y_DIR_PIN           49
#define Y_MIN_PIN           11
#define Y_MAX_PIN           24
#define Y_ENABLE_PIN        28
#define Y_MS1_PIN           69
#define Y_MS2_PIN           39

#define Z_STEP_PIN          35
#define Z_DIR_PIN           47
#define Z_MIN_PIN           10
#define Z_MAX_PIN           23
#define Z_ENABLE_PIN        27
#define Z_MS1_PIN           68
#define Z_MS2_PIN           67

#define HEATER_BED_PIN       4
#define TEMP_BED_PIN         2

#define HEATER_0_PIN         3
#define TEMP_0_PIN           0

#define HEATER_1_PIN         7
#define TEMP_1_PIN           1

#if ENABLED(BARICUDA)
  #define HEATER_2_PIN       6
#else
  #define HEATER_2_PIN      -1
#endif

#define TEMP_2_PIN          -1

#define E0_STEP_PIN         34
#define E0_DIR_PIN          43
#define E0_ENABLE_PIN       26
#define E0_MS1_PIN          65
#define E0_MS2_PIN          66

#define E1_STEP_PIN         -1
#define E1_DIR_PIN          -1
#define E1_ENABLE_PIN       -1
#define E1_MS1_PIN          -1
#define E1_MS2_PIN          -1

#define MOTOR_CURRENT_PWM_XY_PIN 46
#define MOTOR_CURRENT_PWM_Z_PIN  45
#define MOTOR_CURRENT_PWM_E_PIN  44
//Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#define MOTOR_CURRENT_PWM_RANGE 2000
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}
#define SDPOWER             -1
#define SDSS                53
#define LED_PIN             13
#define FAN_PIN              8
#define FAN_1_PIN            6
#define PS_ON_PIN           -1
#define KILL_PIN            -1  // 80 with Smart Controller LCD
#define SUICIDE_PIN         -1  // PIN that has to be turned on right after start, to keep power flowing.

#if ENABLED(ULTRA_LCD)

  #define KILL_PIN          32

  #if ENABLED(NEWPANEL)

    #define BEEPER_PIN      84  // Beeper on AUX-4
    #define LCD_PINS_RS     82
    #define LCD_PINS_ENABLE 18
    #define LCD_PINS_D4     19
    #define LCD_PINS_D5     70
    #define LCD_PINS_D6     85
    #define LCD_PINS_D7     71

    //buttons are directly attached using AUX-2
    #define BTN_EN1         14
    #define BTN_EN2         72
    #define BTN_ENC          9  // the click

    #define SD_DETECT_PIN   15

  #endif //NEWPANEL
#endif //ULTRA_LCD
