/**
 * CartesioV12 pin assignments
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define BOARD_NAME            "CN Controls V12"

//#define LARGE_FLASH true

#define X_ENABLE_PIN          26
#define X_STEP_PIN            25
#define X_DIR_PIN             27
#define X_MIN_PIN             19
#define X_MAX_PIN             -1

#define Y_ENABLE_PIN          29
#define Y_STEP_PIN            28
#define Y_DIR_PIN             30
#define Y_MIN_PIN             22
#define Y_MAX_PIN             -1

#define Z_ENABLE_PIN          32
#define Z_STEP_PIN            31
#define Z_DIR_PIN             33
#define Z_MIN_PIN             23
#define Z_MAX_PIN             -1

#define E0_ENABLE_PIN         58
#define E0_STEP_PIN           57
#define E0_DIR_PIN            55
#define HEATER_0_PIN          11
#define TEMP_0_PIN             0  // ANALOG INPUT !!

#define E1_ENABLE_PIN         60
#define E1_STEP_PIN           61
#define E1_DIR_PIN            62
#define HEATER_1_PIN           9
#define TEMP_1_PIN             9  // 9 for tool2 -> 13 for chambertemp

#define E2_ENABLE_PIN         44
#define E2_STEP_PIN           46
#define E2_DIR_PIN            66
#define HEATER_2_PIN           6
#define TEMP_2_PIN            13  // 10 for tool3 -> 13 for chambertemp

#define E3_ENABLE_PIN         47
#define E3_STEP_PIN           45
#define E3_DIR_PIN            69
#define HEATER_3_PIN           3
#define TEMP_3_PIN            11  // 11 for tool4 -> 13 for chambertemp

#define HEATER_BED_PIN        24
#define TEMP_BED_PIN          14  // ANALOG INPUT !!

// Tools

//#define TOOL_0_PIN          56
//#define TOOL_0_PWM_PIN      10  // red warning led at dual extruder
//#define TOOL_1_PIN          59
//#define TOOL_1_PWM_PIN       8  // lights at dual extruder
//#define TOOL_2_PIN           4
//#define TOOL_2_PWM_PIN       5
//#define TOOL_3_PIN          14
//#define TOOL_3_PWM_PIN       2

// Common I/O

//#define TEMP_CHAMBER_PIN    13  // ANALOG INPUT !!
#define FIL_RUNOUT_PIN        18
//#define PWM_1_PIN           12
//#define PWM_2_PIN           13
//#define SPARE_IO            17
#define FAN_PIN                5  // 5 is PWMtool3 -> 7 is common PWM pin for all tools

// User interface
#define BEEPER_PIN            16

// Pins for DOGM SPI LCD Support
#define DOGLCD_A0             39
#define DOGLCD_CS             35
#define DOGLCD_MOSI           48
#define DOGLCD_SCK            49
#define LCD_SCREEN_ROT_180

// The encoder and click button
#define BTN_EN1               36
#define BTN_EN2               34
#define BTN_ENC               38

// Hardware buttons for manual movement of XYZ
#define SHIFT_OUT             42
#define SHIFT_LD              41
#define SHIFT_CLK             40

//#define UI1                 43
//#define UI2                 37

// Other
#define SDSS                  53
#define SD_DETECT_PIN         15
#define STAT_LED_BLUE         -1
#define STAT_LED_RED          10 // TOOL_0_PWM_PIN
