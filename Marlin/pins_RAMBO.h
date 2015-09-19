/**
 * Rambo pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH true

// Servo support
#define SERVO0_PIN 22 // Motor header MX1
#define SERVO1_PIN 23 // Motor header MX2
#define SERVO2_PIN 24 // Motor header MX3
#define SERVO2_PIN  5 // PWM header pin 5

#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN         -1
#endif

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

#define X_STEP_PIN 37
#define X_DIR_PIN 48
#define X_MIN_PIN 12
#define X_MAX_PIN 24
#define X_ENABLE_PIN 29
#define X_MS1_PIN 40
#define X_MS2_PIN 41

#define Y_STEP_PIN 36
#define Y_DIR_PIN 49
#define Y_MIN_PIN 11
#define Y_MAX_PIN 23
#define Y_ENABLE_PIN 28
#define Y_MS1_PIN 69
#define Y_MS2_PIN 39

#define Z_STEP_PIN 35
#define Z_DIR_PIN 47
#define Z_MIN_PIN 10
#define Z_MAX_PIN 30
#define Z_ENABLE_PIN 27
#define Z_MS1_PIN 68
#define Z_MS2_PIN 67

#define HEATER_BED_PIN 3
#define TEMP_BED_PIN 2

#define HEATER_0_PIN  9
#define TEMP_0_PIN 0

#define HEATER_1_PIN 7
#define TEMP_1_PIN 1

#if ENABLED(BARICUDA)
  #define HEATER_2_PIN 6
#else
  #define HEATER_2_PIN -1
#endif

#define TEMP_2_PIN -1

#define E0_STEP_PIN         34
#define E0_DIR_PIN          43
#define E0_ENABLE_PIN       26
#define E0_MS1_PIN 65
#define E0_MS2_PIN 66

#define E1_STEP_PIN         33
#define E1_DIR_PIN          42
#define E1_ENABLE_PIN       25
#define E1_MS1_PIN 63
#define E1_MS2_PIN 64

#undef DIGIPOTSS_PIN
#define DIGIPOTSS_PIN 38
#define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13
#define FAN_PIN            8

/**********************************************************
  Fan Pins
  Fan_0 8
  Fan_1 6
  Fan_2 2
***********************************************************/
#define PS_ON_PIN          4
#define KILL_PIN           -1 //80 with Smart Controller LCD
#define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.

#if ENABLED(ULTRA_LCD)

  #define KILL_PIN 80

  #if ENABLED(NEWPANEL)

    #define BEEPER_PIN 79      // Beeper on AUX-4

    #define LCD_PINS_RS 70
    #define LCD_PINS_ENABLE 71
    #define LCD_PINS_D4 72
    #define LCD_PINS_D5 73
    #define LCD_PINS_D6 74
    #define LCD_PINS_D7 75

    //buttons are directly attached using AUX-2
    #define BTN_EN1 76
    #define BTN_EN2 77
    #define BTN_ENC 78  //the click

    #define BLEN_C 2
    #define BLEN_B 1
    #define BLEN_A 0

    #define SD_DETECT_PIN 81 // Ramps doesn't use this

  #else //!NEWPANEL - old style panel with shift register

    #define BEEPER_PIN 33    // No Beeper added

    //buttons are attached to a shift register
    // Not wired yet
    //#define SHIFT_CLK 38
    //#define SHIFT_LD 42
    //#define SHIFT_OUT 40
    //#define SHIFT_EN 17

    #define LCD_PINS_RS 75
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29

    //bits in the shift register that carry the buttons for:
    // left up center down right red
    #define BL_LE 7
    #define BL_UP 6
    #define BL_MI 5
    #define BL_DW 4
    #define BL_RI 3
    #define BL_ST 2
    #define BLEN_B 1
    #define BLEN_A 0

  #endif // !NEWPANEL

#endif // ULTRA_LCD

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN 44
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0  70
  #define DOGLCD_CS  71
  #define LCD_SCREEN_ROT_180

  //The encoder and click button
  #define BTN_EN1 85
  #define BTN_EN2 84
  #define BTN_ENC 83  //the click switch

  #define SD_DETECT_PIN -1 // Pin 72 if using easy adapter board

  #if ENABLED(TEMP_STAT_LEDS)
    #define STAT_LED_RED      22
    #define STAT_LED_BLUE     32
  #endif
#endif // VIKI2/miniVIKI

#if ENABLED(FILAMENT_SENSOR)
  //Filip added pin for Filament sensor analog input
  #define FILWIDTH_PIN        3
#endif
