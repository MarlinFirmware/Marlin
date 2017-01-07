/**
 * CartesioV11 pin assignments
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define BOARD_NAME            "CN Controls V11"

//#define LARGE_FLASH true

#define X_ENABLE_PIN          35
#define X_STEP_PIN            34
#define X_DIR_PIN             36
#define X_MIN_PIN             43
#define X_MAX_PIN             -1

#define Y_ENABLE_PIN          38
#define Y_STEP_PIN            37
#define Y_DIR_PIN             39
#define Y_MIN_PIN             45
#define Y_MAX_PIN             -1

#define Z_ENABLE_PIN          41
#define Z_STEP_PIN            40
#define Z_DIR_PIN             48
#define Z_MIN_PIN             42
#define Z_MAX_PIN             -1

#define E0_ENABLE_PIN          3
#define E0_STEP_PIN           29
#define E0_DIR_PIN            28
#define HEATER_0_PIN           5
#define TEMP_0_PIN             0  // ANALOG INPUT !!

#define E1_ENABLE_PIN         60
#define E1_STEP_PIN           61
#define E1_DIR_PIN            62
#define HEATER_1_PIN          58
#define TEMP_1_PIN             3  // 3 for tool2 -> 2 for chambertemp

#define E2_ENABLE_PIN         16
#define E2_STEP_PIN           15
#define E2_DIR_PIN            14
#define HEATER_2_PIN          64
#define TEMP_2_PIN             2  // 9 for tool3 -> 2 for chambertemp

#define E3_ENABLE_PIN         47
#define E3_STEP_PIN           44
#define E3_DIR_PIN            49
#define HEATER_3_PIN          46
#define TEMP_3_PIN            11  // 11 for tool4 -> 2 for chambertemp

#define HEATER_BED_PIN         2
#define TEMP_BED_PIN           1  // ANALOG INPUT !!

// Tools

//#define TOOL_0_PIN           4
//#define TOOL_1_PIN          59
//#define TOOL_2_PIN           8
//#define TOOL_3_PIN          30
//#define TOOL_PWM_PIN         7  // common PWM pin for all tools

// Common I/O

//#define TEMP_CHAMBER_PIN     2  // ANALOG INPUT !!
//#define FIL_RUNOUT_PIN      -1
//#define PWM_1_PIN           11
//#define PWM_2_PIN           10
//#define SPARE_IO            12
//#define FAN_PIN              7  // common PWM pin for all tools

// User interface
#define BEEPER_PIN             6

// Pins for DOGM SPI LCD Support
#define DOGLCD_A0             26
#define DOGLCD_CS             24
#define DOGLCD_MOSI           -1
#define DOGLCD_SCK            -1

// The encoder and click button
#define BTN_EN1               23
#define BTN_EN2               25
#define BTN_ENC               27

// Hardware buttons for manual movement of XYZ
#define SHIFT_OUT             19
#define SHIFT_LD              18
#define SHIFT_CLK             17

//#define UI1                 31
//#define UI2                 22

// Other
#define SDSS                  53
#define SD_DETECT_PIN         13
#define STAT_LED_BLUE         -1
#define STAT_LED_RED          31
