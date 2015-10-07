/****************************************************************************************
* Motherboard: bq CNC
* Revision: v1.3
* 
* Compatible with:
*   - bq LCD Smart Controller
*   - RepRapDiscount Smart Controller
****************************************************************************************/
#if MB(BQ_CNC)
 #define KNOWN_BOARD 1
 #define LARGE_FLASH true

 #ifndef __AVR_ATmega2560__
  # error Oops!  Make sure you have 'Arduino Mega' selected from the \
    'Tools -> Boards' menu.
 #endif // __AVR_ATmega2560__

 // Motors
 #define X_STEP_PIN         54
 #define X_DIR_PIN          55
 #define X_ENABLE_PIN       38

 #define Y_STEP_PIN         60
 #define Y_DIR_PIN          61
 #define Y_ENABLE_PIN       56

 #define Z_STEP_PIN         46
 #define Z_DIR_PIN          48
 #define Z_ENABLE_PIN       77

 #define E0_STEP_PIN        26
 #define E0_DIR_PIN         28
 #define E0_ENABLE_PIN      24

 // Digital potentiometers
 #define DIGIPOTSS_PIN      22
 #define DIGIPOT_CHANNELS   { 4, 5, 3 , 0, 1 }

 // Endstops
 #define X_MAX_PIN          79

 #define Y_MAX_PIN          15

 #define Z_MIN_PIN          18
 #define Z_MAX_PIN          19

 // Heater
 #define HEATER_0_PIN       9

 // Thermistor
 #define TEMP_0_PIN         13

 // Cooling Fans
 #define FAN_BOX_PIN        2
 #define FAN_BLOWER_PIN     12
 #define FAN_BLOCK_PIN      7

 #define FAN_PIN            FAN_BLOWER_PIN

 // LED
 #define LED_PIN            13

 //	Lights
 #define LIGHT_PIN			10

#endif // BQ_CNC

/****************************************************************************************
* bq LCD Smart Controller
****************************************************************************************/
#ifdef BQ_LCD_SMART_CONTROLLER
 // Reset Button
 #define KILL_PIN            41
 // LCD
 #define LCD_PIN_BL          39
 #define LCD_PINS_RS         16
 #define LCD_PINS_ENABLE     17
 #define LCD_PINS_D4         23
 // Button & Encoder
 #define BTN_EN1             31
 #define BTN_EN2             33
 #define BTN_ENC             35
 // Buzzer
 #define BEEPER              37
 // SD Card Reader
 #define SDSS                53
 #define SDCARDDETECT        49
#endif // BQ_LCD_SMART_CONTROLLER 
