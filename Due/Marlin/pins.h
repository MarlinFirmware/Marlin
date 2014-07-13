#ifndef	_PINS_H
#define	_PINS_H

// --------------------------------------------------------------------------
// Arduino Due version
// --------------------------------------------------------------------------

/* Arduino Due supports the following shields:
 *
 * RADDS
 * RAMPS-FD v1
 * RAMPS-FD v2
 *
 * Extras
 *  - LCDs
 *  - SDRAMPS
 *
 */


#if MOTHERBOARD != 88
#define X_MS1_PIN -1
#define X_MS2_PIN -1
#define Y_MS1_PIN -1
#define Y_MS2_PIN -1
#define Z_MS1_PIN -1
#define Z_MS2_PIN -1
#define E0_MS1_PIN -1
#define E0_MS2_PIN -1
#define E1_MS1_PIN -1
#define E1_MS2_PIN -1
#define DIGIPOTSS_PIN -1
#endif


/****************************************************************************************
* Arduino Due pin assignment
*
****************************************************************************************/

// RADDS
#if MOTHERBOARD == 402
#define KNOWN_BOARD 1
//
#ifndef __SAM3X8E__
 #error Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu.
#endif
//
#define RADDS

#define X_STEP_PIN         24
#define X_DIR_PIN          23
#define X_ENABLE_PIN       26
#define X_MIN_PIN          28
#define X_MAX_PIN          34   //2 //Max endstops default to disabled "-1", set to commented value to enable.

#define Y_STEP_PIN         17
#define Y_DIR_PIN          16
#define Y_ENABLE_PIN       22
#define Y_MIN_PIN          30
#define Y_MAX_PIN          36   //15

#define Z_STEP_PIN         2
#define Z_DIR_PIN          3
#define Z_ENABLE_PIN       15
#define Z_MIN_PIN          32
#define Z_MAX_PIN          38

#define E0_STEP_PIN        61
#define E0_DIR_PIN         60
#define E0_ENABLE_PIN      62

#define E1_STEP_PIN        64
#define E1_DIR_PIN         63
#define E1_ENABLE_PIN      65

#define E2_STEP_PIN        51
#define E2_DIR_PIN         53
#define E2_ENABLE_PIN      49

#define SDPOWER            -1
#define SDSS               4 //10 Display
#define LED_PIN            -1

#define BEEPER             41

#define FAN_PIN            9

#define CONTROLLERFAN_PIN  8 //Pin used for the fan to cool controller

#define PS_ON_PIN          40

#define KILL_PIN           -1



#define HEATER_0_PIN       13
#define HEATER_1_PIN       7
#define HEATER_2_PIN       12

#define TEMP_0_PIN         7   // ANALOG NUMBERING
#define TEMP_1_PIN         3   // ANALOG NUMBERING
#define TEMP_2_PIN         6   // ANALOG NUMBERING
#define TEMP_3_PIN         5   // ANALOG NUMBERING
#define TEMP_4_PIN         4   // ANALOG NUMBERING

#define HEATER_BED_PIN     9    // BED ???

#define TEMP_BED_PIN       14   // ANALOG NUMBERING ??

  #ifdef NUM_SERVOS
    #define SERVO0_PIN         11 //??

    #if NUM_SERVOS > 1
      #define SERVO1_PIN         6 //??
    #endif

    #if NUM_SERVOS > 2
      #define SERVO2_PIN         5 //??
    #endif

    #if NUM_SERVOS > 3
      #define SERVO3_PIN         4 //??
    #endif
  #endif


// ??? works with radds??  #ifdef ULTRA_LCD
//
//    #ifdef NEWPANEL
//      #define LCD_PINS_RS 16
//      #define LCD_PINS_ENABLE 17
//      #define LCD_PINS_D4 23
//      #define LCD_PINS_D5 25
//      #define LCD_PINS_D6 27
//      #define LCD_PINS_D7 29
//
//      #ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
//        #define BEEPER 37
//
//        #define BTN_EN1 31
//        #define BTN_EN2 33
//        #define BTN_ENC 35
//
//        #define SDCARDDETECT 49
//      #elif defined(LCD_I2C_PANELOLU2)
//        #define BTN_EN1 47  //reverse if the encoder turns the wrong way.
//        #define BTN_EN2 43
//        #define BTN_ENC 32
//        #define SDSS 53
//        #define SDCARDDETECT -1
//        #define KILL_PIN 41
//      #elif defined(LCD_I2C_VIKI)
//        #define BTN_EN1 22  //reverse if the encoder turns the wrong way.
//        #define BTN_EN2 7
//        #define BTN_ENC -1
//        #define SDSS 53
//        #define SDCARDDETECT 49
//      #else
//        //arduino pin which triggers an piezzo beeper
//        #define BEEPER 33  // Beeper on AUX-4
//
//        //buttons are directly attached using AUX-2
//        #ifdef REPRAPWORLD_KEYPAD
//          #define BTN_EN1 64 // encoder
//          #define BTN_EN2 59 // encoder
//          #define BTN_ENC 63 // enter button
//          #define SHIFT_OUT 40 // shift register
//          #define SHIFT_CLK 44 // shift register
//          #define SHIFT_LD 42 // shift register
//        #else
//          #define BTN_EN1 37
//          #define BTN_EN2 35
//          #define BTN_ENC 31  //the click
//        #endif
//
//        #ifdef G3D_PANEL
//          #define SDCARDDETECT 49
//        #else
//          #define SDCARDDETECT -1  // Ramps does not use this port
//        #endif
//
//      #endif
//
//      #if MOTHERBOARD == 77
//        #define BEEPER -1
//
//        #define LCD_PINS_RS 27
//        #define LCD_PINS_ENABLE 29
//        #define LCD_PINS_D4 37
//        #define LCD_PINS_D5 35
//        #define LCD_PINS_D6 33
//        #define LCD_PINS_D7 31
//
//       //buttons
//       #define BTN_EN1 16
//       #define BTN_EN2 17
//       #define BTN_ENC 23 //the click
//
//    #endif
//    #else //old style panel with shift register
//      //arduino pin witch triggers an piezzo beeper
//      #define BEEPER 33		//No Beeper added
//
//      //buttons are attached to a shift register
//	// Not wired this yet
//      //#define SHIFT_CLK 38
//      //#define SHIFT_LD 42
//      //#define SHIFT_OUT 40
//      //#define SHIFT_EN 17
//
//      #define LCD_PINS_RS 16
//      #define LCD_PINS_ENABLE 17
//      #define LCD_PINS_D4 23
//      #define LCD_PINS_D5 25
//      #define LCD_PINS_D6 27
//      #define LCD_PINS_D7 29
//    #endif
//  #endif //ULTRA_LCD


// SPI for Max6675 Thermocouple

//works with radds??? #ifndef SDSUPPORT
//// these pins are defined in the SD library if building with SD support
//  #define MAX_SCK_PIN          52
//  #define MAX_MISO_PIN         50
//  #define MAX_MOSI_PIN         51
//  #define MAX6675_SS       53
//#else
//  #define MAX6675_SS       49
//#endif

#endif //MOTHERBOARD == 402

// RAMPS-FD
#if MOTHERBOARD == 403 || MOTHERBOARD == 404
#define KNOWN_BOARD 1

//
#ifndef __SAM3X8E__
 #error Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu.
#endif


//
#if MOTHERBOARD == 403
  #define RAMPS_FD_V1
#else
  #define RAMPS_FD_V2
#endif

#define X_STEP_PIN         63
#define X_DIR_PIN          62
#define X_ENABLE_PIN       48
#define X_MIN_PIN          22
#define X_MAX_PIN           -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.

#define Y_STEP_PIN         65
#define Y_DIR_PIN          64
#define Y_ENABLE_PIN       46
#define Y_MIN_PIN          24
#define Y_MAX_PIN          -1   //15

#define Z_STEP_PIN         67
#define Z_DIR_PIN          66
#define Z_ENABLE_PIN       44
#define Z_MIN_PIN          26
#define Z_MAX_PIN          -1

#define E0_STEP_PIN        36
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      42

#define E1_STEP_PIN        43
#define E1_DIR_PIN         41
#define E1_ENABLE_PIN      39

#define E2_STEP_PIN        32
#define E2_DIR_PIN         47
#define E2_ENABLE_PIN      45

#define SDPOWER            -1
#define SDSS               4
#define LED_PIN            13

#define BEEPER             -1

#define FAN_PIN            -1

#define CONTROLLERFAN_PIN  -1 //Pin used for the fan to cool controller

#define PS_ON_PIN          -1

#define KILL_PIN           -1


#define HEATER_BED_PIN     8    // BED

#define HEATER_0_PIN       9
#define HEATER_1_PIN       10
#define HEATER_2_PIN       11

#define TEMP_BED_PIN       0   // ANALOG NUMBERING

#define TEMP_0_PIN         1   // ANALOG NUMBERING
#define TEMP_1_PIN         -1  // 2    // ANALOG NUMBERING
#define TEMP_2_PIN         -1  // 3     // ANALOG NUMBERING

#define TEMP_3_PIN         -1   // ANALOG NUMBERING
#define TEMP_4_PIN         -1   // ANALOG NUMBERING



  #ifdef NUM_SERVOS
    #define SERVO0_PIN         11

    #if NUM_SERVOS > 1
      #define SERVO1_PIN         6
    #endif

    #if NUM_SERVOS > 2
      #define SERVO2_PIN         5
    #endif

    #if NUM_SERVOS > 3
      #define SERVO3_PIN         4
    #endif
  #endif


  #ifdef ULTRA_LCD

    #ifdef NEWPANEL
      #define LCD_PINS_RS 16
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4 23
      #define LCD_PINS_D5 25
      #define LCD_PINS_D6 27
      #define LCD_PINS_D7 29

      #ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
        #define BEEPER 37

        #define BTN_EN1 31
        #define BTN_EN2 33
        #define BTN_ENC 35

        #define SDCARDDETECT 49
      #elif defined(LCD_I2C_PANELOLU2)
        #define BTN_EN1 47  //reverse if the encoder turns the wrong way.
        #define BTN_EN2 43
        #define BTN_ENC 32
        #define SDSS 53
        #define SDCARDDETECT -1
        #define KILL_PIN 41
      #elif defined(LCD_I2C_VIKI)
        #define BTN_EN1 22  //reverse if the encoder turns the wrong way.
        #define BTN_EN2 7
        #define BTN_ENC -1
        #define SDSS 53
        #define SDCARDDETECT 49
      #else
        //arduino pin which triggers an piezzo beeper
        #define BEEPER 33  // Beeper on AUX-4

        //buttons are directly attached using AUX-2
        #ifdef REPRAPWORLD_KEYPAD
          #define BTN_EN1 64 // encoder
          #define BTN_EN2 59 // encoder
          #define BTN_ENC 63 // enter button
          #define SHIFT_OUT 40 // shift register
          #define SHIFT_CLK 44 // shift register
          #define SHIFT_LD 42 // shift register
        #else
          #define BTN_EN1 37
          #define BTN_EN2 35
          #define BTN_ENC 31  //the click
        #endif

        #ifdef G3D_PANEL
          #define SDCARDDETECT 49
        #else
          #define SDCARDDETECT -1  // Ramps does not use this port
        #endif

      #endif

      #if MOTHERBOARD == 77
        #define BEEPER -1

        #define LCD_PINS_RS 27
        #define LCD_PINS_ENABLE 29
        #define LCD_PINS_D4 37
        #define LCD_PINS_D5 35
        #define LCD_PINS_D6 33
        #define LCD_PINS_D7 31

       //buttons
       #define BTN_EN1 16
       #define BTN_EN2 17
       #define BTN_ENC 23 //the click

    #endif
    #else //old style panel with shift register
      //arduino pin witch triggers an piezzo beeper
      #define BEEPER 33		//No Beeper added

      //buttons are attached to a shift register
	// Not wired this yet
      //#define SHIFT_CLK 38
      //#define SHIFT_LD 42
      //#define SHIFT_OUT 40
      //#define SHIFT_EN 17

      #define LCD_PINS_RS 16
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4 23
      #define LCD_PINS_D5 25
      #define LCD_PINS_D6 27
      #define LCD_PINS_D7 29
    #endif
  #endif //ULTRA_LCD


// SPI for Max6675 Thermocouple

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define MAX_SCK_PIN          52
  #define MAX_MISO_PIN         50
  #define MAX_MOSI_PIN         51
  #define MAX6675_SS       53
#else
  #define MAX6675_SS       49
#endif

#endif //MOTHERBOARD == 403

// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------


#ifndef KNOWN_BOARD
#error Unknown MOTHERBOARD value in configuration.h
#endif

//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN,
#if EXTRUDERS > 1
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN,
#else
  #define _E1_PINS
#endif
#if EXTRUDERS > 2
  #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN,
#else
  #define _E2_PINS
#endif

#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN X_STOP_PIN
    #define X_MAX_PIN -1
  #else
    #define X_MIN_PIN -1
    #define X_MAX_PIN X_STOP_PIN
  #endif
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN Y_STOP_PIN
    #define Y_MAX_PIN -1
  #else
    #define Y_MIN_PIN -1
    #define Y_MAX_PIN Y_STOP_PIN
  #endif
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN Z_STOP_PIN
    #define Z_MAX_PIN -1
  #else
    #define Z_MIN_PIN -1
    #define Z_MAX_PIN Z_STOP_PIN
  #endif
#endif

#ifdef DISABLE_MAX_ENDSTOPS
#define X_MAX_PIN          -1
#define Y_MAX_PIN          -1
#define Z_MAX_PIN          -1
#endif

#ifdef DISABLE_MIN_ENDSTOPS
#define X_MIN_PIN          -1
#define Y_MIN_PIN          -1
#define Z_MIN_PIN          -1
#endif

#define SENSITIVE_PINS {0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, PS_ON_PIN, \
                        HEATER_BED_PIN, FAN_PIN,                  \
                        _E0_PINS _E1_PINS _E2_PINS             \
                        analogInputToDigitalPin(TEMP_0_PIN), analogInputToDigitalPin(TEMP_1_PIN), analogInputToDigitalPin(TEMP_2_PIN), analogInputToDigitalPin(TEMP_BED_PIN) }





#endif	/* _PINS_H */
