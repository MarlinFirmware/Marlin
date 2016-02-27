/**
 * bq ZUM Mega 3D board definition
 */

#ifndef __AVR_ATmega2560__
  #error Oops! Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH         true

#define LED_PIN             13

#define X_STEP_PIN          54
#define X_DIR_PIN           55
#define X_ENABLE_PIN        38
#define X_MIN_PIN           3
#define X_MAX_PIN           79

#define Y_STEP_PIN          60
#define Y_DIR_PIN           61
#define Y_ENABLE_PIN        56
#define Y_MIN_PIN           14
#define Y_MAX_PIN           15

#define Z_STEP_PIN          46
#define Z_DIR_PIN           48
#define Z_ENABLE_PIN        77
#define Z_MIN_PIN           18 // Z-MIN Label
#define Z_MAX_PIN           19 // IND_S_5V

#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24

#define E1_STEP_PIN         36
#define E1_DIR_PIN          34
#define E1_ENABLE_PIN       30

#define FAN_PIN             12
#define HEATER_0_PIN        9
#define TEMP_0_PIN          13

#define FAN1_PIN            7
#define HEATER_1_PIN        10
#define TEMP_1_PIN          14

#define HEATER_BED_PIN      8
#define TEMP_BED_PIN        15

#undef DIGIPOTSS_PIN
#define DIGIPOTSS_PIN       22
#define DIGIPOT_CHANNELS    { 4, 5, 3 , 0, 1 }

#define PS_ON_PIN           81 // External Power Supply

#if ENABLED(BQ_LCD_SMART_CONTROLLER)
	#define KILL_PIN        41

	#define LCD_PIN_BL      39
	#define LCD_PINS_RS     16
	#define LCD_PINS_ENABLE 17
	#define LCD_PINS_D4     23

	#define BTN_EN1         31
	#define BTN_EN2         33
	#define BTN_ENC         35

	#define BEEPER_PIN      37

    #define SDPOWER         -1
	#define SDSS            53
	#define SD_DETECT_PIN   49
#endif

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
 	#undef Z_MIN_PIN
 	#undef Z_MAX_PIN

	#define Z_MIN_PIN       19 // IND_S_5V
	#define Z_MAX_PIN       18 // Z-MIN Label
#endif
