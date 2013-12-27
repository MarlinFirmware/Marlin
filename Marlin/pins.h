#ifndef PINS_H
#define PINS_H

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

#if MOTHERBOARD == 99
#define KNOWN_BOARD 1

#define X_STEP_PIN          2
#define X_DIR_PIN           3
#define X_ENABLE_PIN        -1
#define X_STOP_PIN          16

#define Y_STEP_PIN          5
#define Y_DIR_PIN           6
#define Y_ENABLE_PIN       -1
#define Y_STOP_PIN          67

#define Z_STEP_PIN          62
#define Z_DIR_PIN           63
#define Z_ENABLE_PIN       -1
#define Z_STOP_PIN          59

#define E0_STEP_PIN         65
#define E0_DIR_PIN          66
#define E0_ENABLE_PIN      -1

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            -1
#define FAN_PIN            -1
#define PS_ON_PIN           9
#define KILL_PIN           -1

#define HEATER_0_PIN        13
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define TEMP_0_PIN          6   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN         -1   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_2_PIN         -1   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define HEATER_BED_PIN      4
#define TEMP_BED_PIN       10

#endif /* 99 */

/****************************************************************************************
* Sethi 3D_1 pin assignment - www.sethi3d.com.br
*
****************************************************************************************/

#if MOTHERBOARD == 20
#define KNOWN_BOARD

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
#error Oops! Make sure you have 'Sethi 3D' selected from the 'Tools -> Boards' menu.

#endif

#ifndef GEN7_VERSION
#define GEN7_VERSION 12 // v1.x
#endif

//x axis pins
#define X_STEP_PIN 19
#define X_DIR_PIN 18
#define X_ENABLE_PIN 24
#define X_STOP_PIN 2

//y axis pins
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_ENABLE_PIN 24
#define Y_STOP_PIN 0

//z axis pins
#define Z_STEP_PIN 26
#define Z_DIR_PIN 25
#define Z_ENABLE_PIN 24
#define Z_MIN_PIN 1
#define Z_MAX_PIN 0

//extruder pins
#define E0_STEP_PIN 28
#define E0_DIR_PIN 27
#define E0_ENABLE_PIN 24

#define TEMP_0_PIN 1
#define TEMP_1_PIN -1
#define TEMP_2_PIN -1
#define TEMP_BED_PIN 2

#define HEATER_0_PIN 4
#define HEATER_1_PIN -1
#define HEATER_2_PIN -1
#define HEATER_BED_PIN 3

#define KILL_PIN -1

#define SDPOWER -1
#define SDSS -1 // SCL pin of I2C header
#define LED_PIN -1

#if (GEN7_VERSION >= 13)
// Gen7 v1.3 removed the fan pin
#define FAN_PIN -1
#else
#define FAN_PIN 31
#endif
#define PS_ON_PIN 15

//All these generations of Gen7 supply thermistor power
//via PS_ON, so ignore bad thermistor readings
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//our pin for debugging.
#define DEBUG_PIN 0

//our RS485 pins
#define TX_ENABLE_PIN 12
#define RX_ENABLE_PIN 13

#endif

/****************************************************************************************
* Gen7 v1.1, v1.2, v1.3 pin assignment
*
****************************************************************************************/


#if MOTHERBOARD == 12
#define MOTHERBOARD 11
#define GEN7_VERSION 13 // v1.3
#endif

#if MOTHERBOARD == 11
#define KNOWN_BOARD

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
#error Oops! Make sure you have 'Gen7' selected from the 'Tools -> Boards' menu.

#endif

#ifndef GEN7_VERSION
#define GEN7_VERSION 12 // v1.x
#endif

//x axis pins
#define X_STEP_PIN 19
#define X_DIR_PIN 18
#define X_ENABLE_PIN 24
#define X_STOP_PIN 7

//y axis pins
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_ENABLE_PIN 24
#define Y_STOP_PIN 5

//z axis pins
#define Z_STEP_PIN 26
#define Z_DIR_PIN 25
#define Z_ENABLE_PIN 24
#define Z_MIN_PIN 1
#define Z_MAX_PIN 0

//extruder pins
#define E0_STEP_PIN 28
#define E0_DIR_PIN 27
#define E0_ENABLE_PIN 24

#define TEMP_0_PIN 1
#define TEMP_1_PIN -1
#define TEMP_2_PIN -1
#define TEMP_BED_PIN 2

#define HEATER_0_PIN 4
#define HEATER_1_PIN -1
#define HEATER_2_PIN -1
#define HEATER_BED_PIN 3

#define KILL_PIN -1

#define SDPOWER -1
#define SDSS -1 // SCL pin of I2C header
#define LED_PIN -1

#if (GEN7_VERSION >= 13)
// Gen7 v1.3 removed the fan pin
#define FAN_PIN -1
#else
#define FAN_PIN 31
#endif
#define PS_ON_PIN 15

//All these generations of Gen7 supply thermistor power
//via PS_ON, so ignore bad thermistor readings
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//our pin for debugging.
#define DEBUG_PIN 0

//our RS485 pins
#define TX_ENABLE_PIN 12
#define RX_ENABLE_PIN 13

#endif

/****************************************************************************************
* Gen7 v1.4 pin assignment
*
****************************************************************************************/

#if MOTHERBOARD == 13
#define GEN7_VERSION 14 // v1.4
#endif

#if MOTHERBOARD == 13
#define KNOWN_BOARD

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
#error Oops! Make sure you have 'Gen7' selected from the 'Tools -> Boards' menu.

#endif

#ifndef GEN7_VERSION
#define GEN7_VERSION 14 // v1.x
#endif

//x axis pins
#define X_STEP_PIN 29
#define X_DIR_PIN 28
#define X_ENABLE_PIN 25
#define X_STOP_PIN 0

//y axis pins
#define Y_STEP_PIN 27
#define Y_DIR_PIN 26
#define Y_ENABLE_PIN 25
#define Y_STOP_PIN 1

//z axis pins
#define Z_STEP_PIN 23
#define Z_DIR_PIN 22
#define Z_ENABLE_PIN 25
#define Z_STOP_PIN 2

//extruder pins
#define E0_STEP_PIN 19
#define E0_DIR_PIN 18
#define E0_ENABLE_PIN 25

#define TEMP_0_PIN 1
#define TEMP_1_PIN -1
#define TEMP_2_PIN -1
#define TEMP_BED_PIN 0

#define HEATER_0_PIN 4
#define HEATER_1_PIN -1
#define HEATER_2_PIN -1
#define HEATER_BED_PIN 3

#define KILL_PIN -1

#define SDPOWER -1
#define SDSS -1 // SCL pin of I2C header
#define LED_PIN -1

#define FAN_PIN -1

#define PS_ON_PIN 15

//our pin for debugging.
#define DEBUG_PIN 0

//our RS485 pins
#define TX_ENABLE_PIN 12
#define RX_ENABLE_PIN 13

#endif

/*******************************************************************************
*********
* Gen7 Alfons3  pin assignment
*
********************************************************************************
********/
/* These Pins are assigned for the modified GEN7 Board from Alfons3 Please review the pins and adjust it for your needs*/

#if MOTHERBOARD == 10
#define KNOWN_BOARD

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
    #error Oops!  Make sure you have 'Gen7' selected from the 'Tools -> Boards' menu.

#endif

//x axis pins
    #define X_STEP_PIN      21                  // different from standard GEN7
    #define X_DIR_PIN       20                  // different from standard GEN7
    #define X_ENABLE_PIN    24
    #define X_STOP_PIN      0

    //y axis pins
    #define Y_STEP_PIN      23
    #define Y_DIR_PIN       22
    #define Y_ENABLE_PIN    24
    #define Y_STOP_PIN      1

    //z axis pins
    #define Z_STEP_PIN      26
    #define Z_DIR_PIN       25
    #define Z_ENABLE_PIN    24
    #define Z_STOP_PIN      2

    //extruder pins
    #define E0_STEP_PIN      28
    #define E0_DIR_PIN       27
    #define E0_ENABLE_PIN    24

    #define TEMP_0_PIN      2
    #define TEMP_1_PIN      -1
    #define TEMP_2_PIN      -1
    #define TEMP_BED_PIN        1   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)

    #define HEATER_0_PIN    4
    #define HEATER_1_PIN    -1
    #define HEATER_2_PIN    -1
    #define HEATER_BED_PIN      3  // (bed)

    #define SDPOWER         -1
    #define SDSS            31                  // SCL pin of I2C header || CS Pin for SD Card support
    #define LED_PIN         -1

    #define FAN_PIN         -1
    #define PS_ON_PIN       19
    //our pin for debugging.

    #define DEBUG_PIN        -1

    //our RS485 pins
    //#define TX_ENABLE_PIN       12
    //#define RX_ENABLE_PIN       13

    #define BEEPER -1
    #define SDCARDDETECT -1
    #define SUICIDE_PIN -1    //has to be defined; otherwise Power_off doesn't work

    #define KILL_PIN -1
    //Pins for 4bit LCD Support
    #define LCD_PINS_RS 18
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 16
    #define LCD_PINS_D5 15
    #define LCD_PINS_D6 13
    #define LCD_PINS_D7 14

     //buttons are directly attached
    #define BTN_EN1 11
    #define BTN_EN2 10
    #define BTN_ENC 12  //the click

#endif

/****************************************************************************************
* Arduino Mega pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 3 || MOTHERBOARD == 33 || MOTHERBOARD == 34 || MOTHERBOARD == 35 || MOTHERBOARD == 77 || MOTHERBOARD == 67
#define KNOWN_BOARD 1

//////////////////FIX THIS//////////////
#ifndef __AVR_ATmega1280__
 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
#endif


// uncomment one of the following lines for RAMPS v1.3 or v1.0, comment both for v1.2 or 1.1
// #define RAMPS_V_1_3
// #define RAMPS_V_1_0


#if MOTHERBOARD == 33 || MOTHERBOARD == 34 || MOTHERBOARD == 35 || MOTHERBOARD == 77 || MOTHERBOARD == 67

  #define LARGE_FLASH true
  
  #if MOTHERBOARD == 77
    #define X_STEP_PIN         54
    #define X_DIR_PIN          55
    #define X_ENABLE_PIN       38
    #define X_MIN_PIN           3
    #define X_MAX_PIN           -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.

    #define Y_STEP_PIN         60
    #define Y_DIR_PIN          61
    #define Y_ENABLE_PIN       56
    #define Y_MIN_PIN          14
    #define Y_MAX_PIN          -1   //15

    #define Z_STEP_PIN         46
    #define Z_DIR_PIN          48
    #define Z_ENABLE_PIN       63
    #define Z_MIN_PIN          18
    #define Z_MAX_PIN          -1

    #define Y2_STEP_PIN        36
    #define Y2_DIR_PIN         34
    #define Y2_ENABLE_PIN      30

    #define Z2_STEP_PIN        36
    #define Z2_DIR_PIN         34
    #define Z2_ENABLE_PIN      30

    #define E0_STEP_PIN        26
    #define E0_DIR_PIN         28
    #define E0_ENABLE_PIN      24

    #define E1_STEP_PIN        36
    #define E1_DIR_PIN         34
    #define E1_ENABLE_PIN      30

    #define SDPOWER            -1
    #define SDSS               25//53
    #define LED_PIN            13

    #define BEEPER             33    

  #else

    #define X_STEP_PIN         54
    #define X_DIR_PIN          55
    #define X_ENABLE_PIN       38
    #define X_MIN_PIN           3
    #define X_MAX_PIN           2

    #define Y_STEP_PIN         60
    #define Y_DIR_PIN          61
    #define Y_ENABLE_PIN       56
    #define Y_MIN_PIN          14
    #define Y_MAX_PIN          15

    #define Z_STEP_PIN         46
    #define Z_DIR_PIN          48
    #define Z_ENABLE_PIN       62
    #define Z_MIN_PIN          18
    #define Z_MAX_PIN          19

    #define Y2_STEP_PIN        36
    #define Y2_DIR_PIN         34
    #define Y2_ENABLE_PIN      30

    #define Z2_STEP_PIN        36
    #define Z2_DIR_PIN         34
    #define Z2_ENABLE_PIN      30

    #define E0_STEP_PIN        26
    #define E0_DIR_PIN         28
    #define E0_ENABLE_PIN      24

    #define E1_STEP_PIN        36
    #define E1_DIR_PIN         34
    #define E1_ENABLE_PIN      30

    #define SDPOWER            -1
    #define SDSS               53
    #define LED_PIN            13
  #endif

  #if MOTHERBOARD == 33 || MOTHERBOARD == 35 || MOTHERBOARD == 67
    #define FAN_PIN            9 // (Sprinter config)
  #else
    #define FAN_PIN            4 // IO pin. Buffer needed
  #endif

  #if MOTHERBOARD == 77
    #define FAN_PIN            8 
  #endif
  
  #if MOTHERBOARD == 35
    #define CONTROLLERFAN_PIN  10 //Pin used for the fan to cool controller
  #endif

  #define PS_ON_PIN          12

  #if defined(REPRAP_DISCOUNT_SMART_CONTROLLER) || defined(G3D_PANEL)
    #define KILL_PIN           41
  #else
    #define KILL_PIN           -1
  #endif

  #if MOTHERBOARD == 35
    #define HEATER_0_PIN       8
  #else
    #define HEATER_0_PIN       10   // EXTRUDER 1
  #endif

  #if MOTHERBOARD == 33 || MOTHERBOARD == 67
    #define HEATER_1_PIN       -1
  #else
    #define HEATER_1_PIN       9    // EXTRUDER 2 (FAN On Sprinter)
  #endif

  #define HEATER_2_PIN       -1 

  #if MOTHERBOARD == 77
    #define HEATER_0_PIN       10   
    #define HEATER_1_PIN       12 
    #define HEATER_2_PIN       6   
  #endif

  #define TEMP_0_PIN         13   // ANALOG NUMBERING
  #define TEMP_1_PIN         15   // ANALOG NUMBERING
  #define TEMP_2_PIN         -1   // ANALOG NUMBERING

  #if MOTHERBOARD == 35
    #define HEATER_BED_PIN     -1    // NO BED
  #else
    #if MOTHERBOARD == 77
      #define HEATER_BED_PIN     9    // BED
    #else
      #define HEATER_BED_PIN     8    // BED
    #endif
  #endif
  #define TEMP_BED_PIN       14   // ANALOG NUMBERING



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

  #ifdef TEMP_STAT_LEDS
    #if MOTHERBOARD == 67
      #define STAT_LED_RED       6
      #define STAT_LED_BLUE     11
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

#else // RAMPS_V_1_1 or RAMPS_V_1_2 as default (MOTHERBOARD == 3)

#define X_STEP_PIN         26
#define X_DIR_PIN          28
#define X_ENABLE_PIN       24
#define X_MIN_PIN           3
#define X_MAX_PIN          -1    //2

#define Y_STEP_PIN         38
#define Y_DIR_PIN          40
#define Y_ENABLE_PIN       36
#define Y_MIN_PIN          16
#define Y_MAX_PIN          -1    //17

#define Z_STEP_PIN         44
#define Z_DIR_PIN          46
#define Z_ENABLE_PIN       42
#define Z_MIN_PIN          18
#define Z_MAX_PIN          -1    //19

#define E0_STEP_PIN         32
#define E0_DIR_PIN          34
#define E0_ENABLE_PIN       30

#define SDPOWER            48
#define SDSS               53
#define LED_PIN            13
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#ifdef RAMPS_V_1_0 // RAMPS_V_1_0
  #define HEATER_0_PIN     12    // RAMPS 1.0
  #define HEATER_BED_PIN   -1    // RAMPS 1.0
  #define FAN_PIN          11    // RAMPS 1.0
#else // RAMPS_V_1_1 or RAMPS_V_1_2
  #define HEATER_0_PIN     10    // RAMPS 1.1
  #define HEATER_BED_PIN    8    // RAMPS 1.1
  #define FAN_PIN           9    // RAMPS 1.1
#endif
#define HEATER_1_PIN        -1
#define HEATER_2_PIN        -1
#define TEMP_0_PIN          2    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN          -1   
#define TEMP_2_PIN          -1   
#define TEMP_BED_PIN        1    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!

#endif // MOTHERBOARD == 33 || MOTHERBOARD == 34 || MOTHERBOARD == 35 || MOTHERBOARD == 77

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

#endif //MOTHERBOARD == 3 || MOTHERBOARD == 33 || MOTHERBOARD == 34 || MOTHERBOARD == 35 || MOTHERBOARD == 77

/****************************************************************************************
* Duemilanove w/ ATMega328P pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 4
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega328P__
#error Oops!  Make sure you have 'Arduino Duemilanove w/ ATMega328' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         19
#define X_DIR_PIN          18
#define X_ENABLE_PIN       -1
#define X_STOP_PIN         17

#define Y_STEP_PIN         10
#define Y_DIR_PIN           7
#define Y_ENABLE_PIN       -1
#define Y_STOP_PIN          8

#define Z_STEP_PIN         13
#define Z_DIR_PIN           3
#define Z_ENABLE_PIN        2
#define Z_STOP_PIN          4

#define E0_STEP_PIN         11
#define E0_DIR_PIN          12
#define E0_ENABLE_PIN       -1

#define SDPOWER          -1
#define SDSS          -1
#define LED_PIN            -1
#define FAN_PIN             5
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN        6
#define HEATER_1_PIN        -1
#define HEATER_2_PIN        -1
#define TEMP_0_PIN          0    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN          -1
#define TEMP_2_PIN          -1
#define HEATER_BED_PIN      -1
#define TEMP_BED_PIN        -1

#endif

/****************************************************************************************
* Elefu RA Board Pin Assignments
*
****************************************************************************************/
#if MOTHERBOARD == 21
#define	KNOWN_BOARD 1

#ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif


#define X_STEP_PIN		     49
#define X_DIR_PIN			     13
#define X_ENABLE_PIN		   48
#define X_MIN_PIN			     35
#define X_MAX_PIN			     -1 //34

#define Y_STEP_PIN         11
#define Y_DIR_PIN          9
#define Y_ENABLE_PIN       12
#define Y_MIN_PIN          33
#define Y_MAX_PIN          -1 //32

#define Z_STEP_PIN         7
#define Z_DIR_PIN          6
#define Z_ENABLE_PIN       8
#define Z_MIN_PIN          31
#define Z_MAX_PIN          -1 //30

#define E2_STEP_PIN        43
#define E2_DIR_PIN         47
#define E2_ENABLE_PIN      42

#define E1_STEP_PIN        18
#define E1_DIR_PIN         19
#define E1_ENABLE_PIN      38

#define E0_STEP_PIN        40
#define E0_DIR_PIN         41
#define E0_ENABLE_PIN      37

#define SDPOWER            -1
#define LED_PIN            -1 //Use +12V Aux port for LED Ring

#define FAN_PIN            16 //5V PWM

#define PS_ON_PIN          10 //Set to -1 if using a manual switch on the PWRSW Connector
#define SLEEP_WAKE_PIN		 26 //This feature still needs work

#define HEATER_0_PIN       45	//12V PWM1
#define HEATER_1_PIN       46	//12V PWM2
#define HEATER_2_PIN       17	//12V PWM3
#define HEATER_BED_PIN     44	//DOUBLE 12V PWM
#define TEMP_0_PIN         3	//ANALOG NUMBERING
#define TEMP_1_PIN         2 	//ANALOG NUMBERING
#define TEMP_2_PIN         1 	//ANALOG NUMBERING
#define TEMP_BED_PIN       0	//ANALOG NUMBERING

#define BEEPER 		         36

#define KILL_PIN           -1

// M240  Triggers a camera by emulating a Canon RC-1 Remote
// Data from: http://www.doc-diy.net/photo/rc-1_hacked/
#define PHOTOGRAPH_PIN     29

#ifdef RA_CONTROL_PANEL

  #define SDSS             53
  #define SDCARDDETECT     28

  #define BTN_EN1          14
  #define BTN_EN2          39
  #define BTN_ENC          15  //the click
    
  #define BLEN_C           2
  #define BLEN_B           1
  #define BLEN_A           0
    
  //encoder rotation values
  #define encrot0          0
  #define encrot1          2
  #define encrot2          3
  #define encrot3          1
  
#endif //RA_CONTROL_PANEL

#ifdef RA_DISCO
  //variables for which pins the TLC5947 is using
  #define TLC_CLOCK_PIN    25
  #define TLC_BLANK_PIN    23
  #define TLC_XLAT_PIN     22
  #define TLC_DATA_PIN     24

  //We also need to define pin to port number mapping for the 2560 to match the pins listed above. If you change the TLC pins, update this as well per the 2560 datasheet!
  //This currently only works with the RA Board.
  #define TLC_CLOCK_BIT 3 //bit 3 on port A
  #define TLC_CLOCK_PORT &PORTA //bit 3 on port A
  
  #define TLC_BLANK_BIT 1 //bit 1 on port A
  #define TLC_BLANK_PORT &PORTA //bit 1 on port A

  #define TLC_DATA_BIT 2 //bit 2 on port A
  #define TLC_DATA_PORT &PORTA //bit 2 on port A

  #define TLC_XLAT_BIT 0 //bit 0 on port A
  #define TLC_XLAT_PORT &PORTA //bit 0 on port A

  //change this to match your situation. Lots of TLCs takes up the arduino SRAM very quickly, so be careful 
  //Leave it at at least 1 if you have enabled RA_LIGHTING
  //The number of TLC5947 boards chained together for use with the animation, additional ones will repeat the animation on them, but are not individually addressable and mimic those before them. You can leave the default at 2 even if you only have 1 TLC5947 module.
  #define NUM_TLCS 2 

  //These TRANS_ARRAY values let you change the order the LEDs on the lighting modules will animate for chase functions. 
  //Modify them according to your specific situation.
  //NOTE: the array should be 8 long for every TLC you have. These defaults assume (2) TLCs.
  #define TRANS_ARRAY {0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8} //forwards
  //#define TRANS_ARRAY {7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15} //backwards
#endif //RA_LIGHTING


#endif /* Ra Board */


/****************************************************************************************
* Gen6 pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 5 || MOTHERBOARD == 51
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
#ifndef __AVR_ATmega1284P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif
#endif

//x axis pins
    #define X_STEP_PIN      15
    #define X_DIR_PIN       18
    #define X_ENABLE_PIN    19
    #define X_STOP_PIN      20

    //y axis pins
    #define Y_STEP_PIN      23
    #define Y_DIR_PIN       22
    #define Y_ENABLE_PIN    24
    #define Y_STOP_PIN      25

    //z axis pins
    #define Z_STEP_PIN      27
    #define Z_DIR_PIN       28
    #define Z_ENABLE_PIN    29
    #define Z_STOP_PIN      30

    //extruder pins
    #define E0_STEP_PIN      4    //Edited @ EJE Electronics 20100715
    #define E0_DIR_PIN       2    //Edited @ EJE Electronics 20100715
    #define E0_ENABLE_PIN    3    //Added @ EJE Electronics 20100715
    #define TEMP_0_PIN      5     //changed @ rkoeppl 20110410
    #define TEMP_1_PIN      -1    //changed @ rkoeppl 20110410


    #define TEMP_2_PIN      -1    //changed @ rkoeppl 20110410
    #define HEATER_0_PIN    14    //changed @ rkoeppl 20110410
    #define HEATER_1_PIN    -1
    #define HEATER_2_PIN    -1
    #if MOTHERBOARD == 5
    #define HEATER_BED_PIN  -1    //changed @ rkoeppl 20110410
    #define TEMP_BED_PIN    -1    //changed @ rkoeppl 20110410
    #else
    #define HEATER_BED_PIN   1    //changed @ rkoeppl 20110410
    #define TEMP_BED_PIN     0    //changed @ rkoeppl 20110410
    #endif
    #define SDPOWER          -1
    #define SDSS          17
    #define LED_PIN         -1    //changed @ rkoeppl 20110410
    #define FAN_PIN         -1    //changed @ rkoeppl 20110410
    #define PS_ON_PIN       -1    //changed @ rkoeppl 20110410
    #define KILL_PIN        -1    //changed @ drakelive 20120830
    //our pin for debugging.

    #define DEBUG_PIN        0

    //our RS485 pins
    #define TX_ENABLE_PIN 12
    #define RX_ENABLE_PIN 13


#endif

/****************************************************************************************
* Sanguinololu pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 64
#define STB
#endif
#if MOTHERBOARD == 63 || MOTHERBOARD == 66
#define MELZI
#endif
#if MOTHERBOARD == 65
#define AZTEEG_X1
#endif
#if MOTHERBOARD == 62 || MOTHERBOARD == 63 || MOTHERBOARD == 64 || MOTHERBOARD == 65 || MOTHERBOARD == 66
#undef MOTHERBOARD
#define MOTHERBOARD 6
#define SANGUINOLOLU_V_1_2
#endif
#if MOTHERBOARD == 6
#define KNOWN_BOARD 1
#ifndef __AVR_ATmega644P__
#ifndef __AVR_ATmega1284P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_STOP_PIN         18

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_STOP_PIN         19

#define Z_STEP_PIN         3
#define Z_DIR_PIN          2
#define Z_STOP_PIN         20

#define E0_STEP_PIN         1
#define E0_DIR_PIN          0

#define LED_PIN            -1

#define FAN_PIN            -1
 #if FAN_PIN == 12 || FAN_PIN ==13
  #define FAN_SOFT_PWM
#endif

#ifdef MELZI
 #define LED_PIN            27 /* On some broken versions of the Sanguino libraries the pin definitions are wrong, which then needs LED_PIN as pin 28. But you better upgrade your Sanguino libraries! See #368. */
 #define FAN_PIN            4 // Works for Panelolu2 too
#endif

#ifdef STB
 #define FAN_PIN            4
	//  Uncomment this if you have the first generation (V1.10) of STBs board
 #define LCD_PIN_BL         17 // LCD backlight LED
#endif

#ifdef AZTEEG_X1
 #define FAN_PIN            4
#endif

#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN       13 // (extruder)
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1

#ifdef SANGUINOLOLU_V_1_2

 #define HEATER_BED_PIN     12 // (bed)
 #define X_ENABLE_PIN       14
 #define Y_ENABLE_PIN       14
 #define Z_ENABLE_PIN       26
 #define E0_ENABLE_PIN      14

 #ifdef LCD_I2C_PANELOLU2
   #define FAN_PIN          4 // Uses Transistor1 (PWM) on Panelolu2's Sanguino Adapter Board to drive the fan
 #endif

#else

#define HEATER_BED_PIN      14  // (bed)
#define X_ENABLE_PIN       -1
#define Y_ENABLE_PIN       -1
#define Z_ENABLE_PIN       -1
#define E0_ENABLE_PIN      -1

#endif

#define TEMP_0_PIN          7   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 33 extruder)
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        6   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)
#define SDPOWER            -1
#define SDSS               31

/* On some broken versions of the Sanguino libraries the pin definitions are wrong, which then needs SDSS as pin 24. But you better upgrade your Sanguino libraries! See #368. */
//#define SDSS               24

 #ifdef ULTRA_LCD
   #ifdef NEWPANEL
     //we have no buzzer installed
     #define BEEPER -1
     //LCD Pins
     #ifdef DOGLCD
       // Pins for DOGM SPI LCD Support
       #define DOGLCD_A0  30
       #define DOGLCD_CS  29
       // GLCD features
       #define LCD_CONTRAST 1
       // Uncomment screen orientation
         // #define LCD_SCREEN_ROT_0
         // #define LCD_SCREEN_ROT_90
       #define LCD_SCREEN_ROT_180
         // #define LCD_SCREEN_ROT_270
       #else // standard Hitachi LCD controller
       #define LCD_PINS_RS        4
       #define LCD_PINS_ENABLE    17
       #define LCD_PINS_D4        30
       #define LCD_PINS_D5        29
       #define LCD_PINS_D6        28
       #define LCD_PINS_D7        27
     #endif
     //The encoder and click button
     #define BTN_EN1 11  
     #define BTN_EN2 10 
     #ifdef LCD_I2C_PANELOLU2
       #ifdef MELZI
         #define BTN_ENC 29 //the click switch
         #define SDSS 30 //to use the SD card reader on the Panelolu2 rather than the melzi board
       #else
         #define BTN_ENC 30 //the click switch
       #endif
     #else
       #define BTN_ENC 16  //the click switch
     #endif //Panelolu2
     //not connected to a pin
     #define SDCARDDETECT -1    
    
   #endif //Newpanel
 #endif //Ultipanel

 #ifdef MAKRPANEL
     #define BEEPER 29
     // Pins for DOGM SPI LCD Support
     #define DOGLCD_A0  30
     #define DOGLCD_CS  17
     #define LCD_PIN_BL	28	// backlight LED on PA3
     // GLCD features
     #define LCD_CONTRAST 1
     // Uncomment screen orientation
     #define LCD_SCREEN_ROT_0
       // #define LCD_SCREEN_ROT_90
       // #define LCD_SCREEN_ROT_180
       // #define LCD_SCREEN_ROT_270
     //The encoder and click button
     #define BTN_EN1 11
     #define BTN_EN2 10
     #define BTN_ENC 16  //the click switch
     //not connected to a pin
     #define SDCARDDETECT -1    
 #endif //Makrpanel

#endif


#if MOTHERBOARD == 7
#define KNOWN_BOARD
/*****************************************************************
* Ultimaker pin assignment
******************************************************************/

#ifndef __AVR_ATmega1280__
 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
#endif

#define LARGE_FLASH true

#define X_STEP_PIN 25
#define X_DIR_PIN 23
#define X_MIN_PIN 22
#define X_MAX_PIN 24
#define X_ENABLE_PIN 27

#define Y_STEP_PIN 31
#define Y_DIR_PIN 33
#define Y_MIN_PIN 26
#define Y_MAX_PIN 28
#define Y_ENABLE_PIN 29

#define Z_STEP_PIN 37
#define Z_DIR_PIN 39
#define Z_MIN_PIN 30
#define Z_MAX_PIN 32
#define Z_ENABLE_PIN 35

#define HEATER_BED_PIN 4
#define TEMP_BED_PIN 10

#define HEATER_0_PIN  2
#define TEMP_0_PIN 8

#define HEATER_1_PIN 3
#define TEMP_1_PIN 9

#define HEATER_2_PIN -1
#define TEMP_2_PIN -1

#define E0_STEP_PIN         43
#define E0_DIR_PIN          45
#define E0_ENABLE_PIN       41

#define E1_STEP_PIN         49
#define E1_DIR_PIN          47
#define E1_ENABLE_PIN       48

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13
#define FAN_PIN            7
#define PS_ON_PIN          12
#define KILL_PIN           -1
#define SUICIDE_PIN        54  //PIN that has to be turned on right after start, to keep power flowing.

#ifdef ULTRA_LCD

  #ifdef NEWPANEL
  //arduino pin witch triggers an piezzo beeper
    #define BEEPER 18

    #define LCD_PINS_RS 20
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 16
    #define LCD_PINS_D5 21
    #define LCD_PINS_D6 5
    #define LCD_PINS_D7 6

    //buttons are directly attached
    #define BTN_EN1 40
    #define BTN_EN2 42
    #define BTN_ENC 19  //the click
    
    #define SDCARDDETECT 38
    
  #else //old style panel with shift register
    //arduino pin witch triggers an piezzo beeper
    #define BEEPER 18

    //buttons are attached to a shift register
    #define SHIFT_CLK 38
    #define SHIFT_LD 42
    #define SHIFT_OUT 40
    #define SHIFT_EN 17

    #define LCD_PINS_RS 16
    #define LCD_PINS_ENABLE 5
    #define LCD_PINS_D4 6
    #define LCD_PINS_D5 21
    #define LCD_PINS_D6 20
    #define LCD_PINS_D7 19
  
    #define SDCARDDETECT -1
  #endif 
#endif //ULTRA_LCD

#endif

#if MOTHERBOARD == 71
#define KNOWN_BOARD
/*****************************************************************
* Ultimaker pin assignment (Old electronics)
******************************************************************/

#ifndef __AVR_ATmega1280__
 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
#endif

#define LARGE_FLASH true

#define X_STEP_PIN 25
#define X_DIR_PIN 23
#define X_MIN_PIN 15
#define X_MAX_PIN 14
#define X_ENABLE_PIN 27

#define Y_STEP_PIN 31
#define Y_DIR_PIN 33
#define Y_MIN_PIN 17
#define Y_MAX_PIN 16
#define Y_ENABLE_PIN 29

#define Z_STEP_PIN 37
#define Z_DIR_PIN 39
#define Z_MIN_PIN 19
#define Z_MAX_PIN 18
#define Z_ENABLE_PIN 35

#define HEATER_BED_PIN -1
#define TEMP_BED_PIN -1

#define HEATER_0_PIN  2
#define TEMP_0_PIN 8

#define HEATER_1_PIN 1
#define TEMP_1_PIN 1

#define HEATER_2_PIN -1
#define TEMP_2_PIN -1

#define E0_STEP_PIN         43
#define E0_DIR_PIN          45
#define E0_ENABLE_PIN       41

#define E1_STEP_PIN         -1
#define E1_DIR_PIN          -1
#define E1_ENABLE_PIN       -1

#define SDPOWER            -1
#define SDSS               -1
#define LED_PIN            -1
#define FAN_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.

#define LCD_PINS_RS 24
#define LCD_PINS_ENABLE 22
#define LCD_PINS_D4 36
#define LCD_PINS_D5 34
#define LCD_PINS_D6 32
#define LCD_PINS_D7 30

#endif


/****************************************************************************************
* RUMBA pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 80
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         17
#define X_DIR_PIN          16
#define X_ENABLE_PIN       48
#define X_MIN_PIN          37
#define X_MAX_PIN          36

#define Y_STEP_PIN         54
#define Y_DIR_PIN          47
#define Y_ENABLE_PIN       55
#define Y_MIN_PIN          35
#define Y_MAX_PIN          34

#define Z_STEP_PIN         57
#define Z_DIR_PIN          56
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          33
#define Z_MAX_PIN          32

#define E0_STEP_PIN        23
#define E0_DIR_PIN         22
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        26
#define E1_DIR_PIN         25
#define E1_ENABLE_PIN      27

#define E2_STEP_PIN        29
#define E2_DIR_PIN         28
#define E2_ENABLE_PIN      39

#define LED_PIN            13

#define FAN_PIN            7
//additional FAN1 PIN (e.g. useful for electronics fan or light on/off) on PIN 8

#define PS_ON_PIN          45
#define KILL_PIN           46

#if (TEMP_SENSOR_0==0)
 #define TEMP_0_PIN         -1
 #define HEATER_0_PIN       -1
#else
 #define HEATER_0_PIN        2    // EXTRUDER 1
 #if (TEMP_SENSOR_0==-1)
  #define TEMP_0_PIN         6    // ANALOG NUMBERING - connector *K1* on RUMBA thermocouple ADD ON is used
 #else
  #define TEMP_0_PIN         15   // ANALOG NUMBERING - default connector for thermistor *T0* on rumba board is used
 #endif
#endif 

#if (TEMP_SENSOR_1==0)
 #define TEMP_1_PIN         -1
 #define HEATER_1_PIN       -1
#else
 #define HEATER_1_PIN        3    // EXTRUDER 2
 #if (TEMP_SENSOR_1==-1)
  #define TEMP_1_PIN         5    // ANALOG NUMBERING - connector *K2* on RUMBA thermocouple ADD ON is used
 #else
  #define TEMP_1_PIN         14   // ANALOG NUMBERING - default connector for thermistor *T1* on rumba board is used
 #endif
#endif

#if (TEMP_SENSOR_2==0)
 #define TEMP_2_PIN         -1
 #define HEATER_2_PIN       -1
#else
 #define HEATER_2_PIN        6    // EXTRUDER 3
 #if (TEMP_SENSOR_2==-1)
  #define TEMP_2_PIN         7    // ANALOG NUMBERING - connector *K3* on RUMBA thermocouple ADD ON is used <-- this can not be used when TEMP_SENSOR_BED is defined as thermocouple
 #else
  #define TEMP_2_PIN         13   // ANALOG NUMBERING - default connector for thermistor *T2* on rumba board is used
 #endif
#endif

//optional for extruder 4 or chamber: #define TEMP_X_PIN         12   // ANALOG NUMBERING - default connector for thermistor *T3* on rumba board is used
//optional FAN1 can be used as 4th heater output: #define HEATER_3_PIN       8    // EXTRUDER 4

#if (TEMP_SENSOR_BED==0)
 #define TEMP_BED_PIN       -1
 #define HEATER_BED_PIN     -1
#else
 #define HEATER_BED_PIN      9    // BED
 #if (TEMP_SENSOR_BED==-1)
  #define TEMP_BED_PIN       7    // ANALOG NUMBERING - connector *K3* on RUMBA thermocouple ADD ON is used <-- this can not be used when TEMP_SENSOR_2 is defined as thermocouple
 #else
  #define TEMP_BED_PIN       11   // ANALOG NUMBERING - default connector for thermistor *THB* on rumba board is used
 #endif
#endif

#define SDPOWER            -1
#define SDSS               53
#define SDCARDDETECT       49
#define BEEPER             44
#define LCD_PINS_RS        19
#define LCD_PINS_ENABLE    42
#define LCD_PINS_D4        18
#define LCD_PINS_D5        38
#define LCD_PINS_D6        41
#define LCD_PINS_D7        40
#define BTN_EN1            11
#define BTN_EN2            12
#define BTN_ENC            43

#endif //MOTHERBOARD==80


/****************************************************************************************
* Teensylu 0.7 / Printrboard pin assignments (AT90USB1286)
* Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
  http://www.pjrc.com/teensy/teensyduino.html
* See http://reprap.org/wiki/Printrboard for more info
****************************************************************************************/
#if MOTHERBOARD == 8 || MOTHERBOARD == 81
#define KNOWN_BOARD 1
#define AT90USB 1286  // Disable MarlinSerial etc.

#ifndef __AVR_AT90USB1286__
#error Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true

#define X_STEP_PIN          0
#define X_DIR_PIN           1
#define X_ENABLE_PIN       39

#define Y_STEP_PIN          2
#define Y_DIR_PIN           3
#define Y_ENABLE_PIN       38

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5
#define Z_ENABLE_PIN       23

#define E0_STEP_PIN         6
#define E0_DIR_PIN          7
#define E0_ENABLE_PIN      19

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     20  // Bed
#define FAN_PIN            22  // Fan
// You may need to change FAN_PIN to 16 because Marlin isn't using fastio.h
// for the fan and Teensyduino uses a different pin mapping.

#if MOTHERBOARD == 8  // Teensylu
  #define X_STOP_PIN         13
  #define Y_STOP_PIN         14
  #define Z_STOP_PIN         15
  #define TEMP_0_PIN          7  // Extruder / Analog pin numbering
  #define TEMP_BED_PIN        6  // Bed / Analog pin numbering
#else  // Printrboard
  #define X_STOP_PIN         35
  #define Y_STOP_PIN          8
  #define Z_STOP_PIN         36
  #define TEMP_0_PIN          1  // Extruder / Analog pin numbering
  #define TEMP_BED_PIN        0  // Bed / Analog pin numbering
#endif

#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1

#define SDPOWER            -1
#define SDSS                8
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define SCK_PIN           9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif

#endif  // MOTHERBOARD == 8 (Teensylu) or 81 (Printrboard)

/****************************************************************************************
 * Brainwave 1.0 pin assignments (AT90USB646)
 * Requires hardware bundle for Arduino:
   https://github.com/unrepentantgeek/brainwave-arduino
 ****************************************************************************************/
#if MOTHERBOARD == 82
#define KNOWN_BOARD 1
#define AT90USB 646  // Disable MarlinSerial etc.

#ifndef __AVR_AT90USB646__
#error Oops!  Make sure you have 'Brainwave' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         27
#define X_DIR_PIN          29
#define X_ENABLE_PIN       28
#define X_STOP_PIN          7
#define X_ATT_PIN          26

#define Y_STEP_PIN         31
#define Y_DIR_PIN          33
#define Y_ENABLE_PIN       32
#define Y_STOP_PIN          6
#define Y_ATT_PIN          30

#define Z_STEP_PIN         17
#define Z_DIR_PIN          19
#define Z_ENABLE_PIN       18
#define Z_STOP_PIN          5
#define Z_ATT_PIN          16

#define E0_STEP_PIN        21
#define E0_DIR_PIN         23
#define E0_ENABLE_PIN      22
#define E0_ATT_PIN         20

#define HEATER_0_PIN        4  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     38  // Bed
#define FAN_PIN             3  // Fan

#define TEMP_0_PIN          7  // Extruder / Analog pin numbering
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        6  // Bed / Analog pin numbering

#define SDPOWER            -1
#define SDSS               -1
#define LED_PIN            39
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define SCK_PIN           9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif

#endif  // MOTHERBOARD == 82 (Brainwave)

//
// SAV Mk-I
// -----------------------------------------------------------------------------------
/****************************************************************************************
* SAV MkI pin assignments (AT90USB1286)
* Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
  http://www.pjrc.com/teensy/teensyduino.html
   RepRap Clone Wars project board.
****************************************************************************************/
#if MOTHERBOARD == 83  // SAV Mk-I
#define KNOWN_BOARD 1
#define AT90USB 1286  // Disable MarlinSerial etc.

#ifndef __AVR_AT90USB1286__
#error Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true


#define X_STEP_PIN         0
#define X_DIR_PIN          1
#define X_ENABLE_PIN       39

#define Y_STEP_PIN         2
#define Y_DIR_PIN          3
#define Y_ENABLE_PIN       38

#define Z_STEP_PIN         4
#define Z_DIR_PIN          5
#define Z_ENABLE_PIN       23

#define E0_STEP_PIN         6
#define E0_DIR_PIN          7
#define E0_ENABLE_PIN       19

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     20  // Bed
#define FAN_PIN            16  // Fan   -- from Teensyduino environment.
                               // For the fan and Teensyduino uses a different pin mapping.

  #define X_STOP_PIN         13
  #define Y_STOP_PIN         14
  #define Z_STOP_PIN         15
  #define TEMP_0_PIN          7  // Extruder / Analog pin numbering
  #define TEMP_BED_PIN        6  // Bed / Analog pin numbering

#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1

#define SDPOWER            -1
#define SDSS               20  // PB0 - 8 in marlin env.
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1
#define SDCARDDETECT       -1


#ifndef SDSUPPORT
   // these pins are defined in the SD library if building with SD support
  #define SCK_PIN          9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif

#define BEEPER             -1
#define LCD_PINS_RS        -1
#define LCD_PINS_ENABLE    -1
#define LCD_PINS_D4        -1
#define LCD_PINS_D5        -1
#define LCD_PINS_D6        -1
#define LCD_PINS_D7        -1
#define BTN_EN1            -1
#define BTN_EN2            -1
#define BTN_ENC            -1

// For LCD SHIFT register LCD
#define SR_DATA_PIN         0
#define SR_CLK_PIN          1

#endif  // MOTHERBOARD == 83


/****************************************************************************************
* Gen3+ pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 9
#define MOTHERBOARD 6   /*TODO: Figure out, Why is this done?*/
#define KNOWN_BOARD 1
#ifndef __AVR_ATmega644P__
#ifndef __AVR_ATmega1284P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          18
#define X_STOP_PIN         20

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_STOP_PIN         25

#define Z_STEP_PIN         27
#define Z_DIR_PIN          28
#define Z_STOP_PIN         30

#define E_STEP_PIN         17
#define E_DIR_PIN          21

#define LED_PIN            -1

#define FAN_PIN            -1

#define PS_ON_PIN         14
#define KILL_PIN           -1

#define HEATER_0_PIN       12 // (extruder)

#define HEATER_1_PIN       16 // (bed)
#define X_ENABLE_PIN       19
#define Y_ENABLE_PIN       24
#define Z_ENABLE_PIN       29
#define E_ENABLE_PIN       13

#define TEMP_0_PIN          0   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 33 extruder)
#define TEMP_1_PIN          5   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)
#define TEMP_2_PIN         -1
#define SDPOWER            -1
#define SDSS               4
#define HEATER_2_PIN       -1

#endif



/****************************************************************************************
* Open Motion controller with enable based extruders
*
*                        ATMega644
*
*                        +---\/---+
*            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
*            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
*       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
*        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
*        PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
*       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
*       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
*        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
*                  RST  9|        |32  AREF
*                  VCC 10|        |31  GND
*                  GND 11|        |30  AVCC
*                XTAL2 12|        |29  PC7 (D 23)
*                XTAL1 13|        |28  PC6 (D 22)
*       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
*       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
*  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
*  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
*       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
*       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
*       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
*                        +--------+
*
****************************************************************************************/
#if MOTHERBOARD == 90 //Alpha OMCA board
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644__
#error Oops!  Make sure you have 'SanguinoA' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         21
#define X_DIR_PIN          20
#define X_ENABLE_PIN       24
#define X_STOP_PIN         0

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_ENABLE_PIN       24
#define Y_STOP_PIN         1

#define Z_STEP_PIN         26
#define Z_DIR_PIN          25
#define Z_ENABLE_PIN       24
#define Z_STOP_PIN         2

#define E0_STEP_PIN         28
#define E0_DIR_PIN          27
#define E0_ENABLE_PIN       24

#define E1_STEP_PIN         -1 // 19
#define E1_DIR_PIN          -1 // 18
#define E1_ENABLE_PIN       24

#define E2_STEP_PIN         -1 // 17
#define E2_DIR_PIN          -1 // 16
#define E2_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               11
#define SDCARDDETECT       -1 // 10 optional also used as mode pin
#define LED_PIN            -1
#define FAN_PIN            3
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN       4
#define HEATER_1_PIN       -1 // 12
#define HEATER_2_PIN       -1 // 13
#define TEMP_0_PIN          0 //D27   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN         -1 // 1
#define TEMP_2_PIN         -1 // 2
#define HEATER_BED_PIN     -1 // 14/15
#define TEMP_BED_PIN       -1 // 1,2 or I2C
/*  Unused (1) (2) (3) 4 5 6 7 8 9 10 11 12 13 (14) (15) (16) 17 (18) (19) (20) (21) (22) (23) 24 (25) (26) (27) 28 (29) (30) (31)  */

#endif

#if MOTHERBOARD == 91  // Final OMCA board -- REF http://sanguino.cc/hardware
#define KNOWN_BOARD 1

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__)
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu. (Final OMCA board)
#endif

#define X_STEP_PIN         26
#define X_DIR_PIN          25
#define X_ENABLE_PIN       10
#define X_STOP_PIN         0

#define Y_STEP_PIN         28
#define Y_DIR_PIN          27
#define Y_ENABLE_PIN       10
#define Y_STOP_PIN         1

#define Z_STEP_PIN         23
#define Z_DIR_PIN          22
#define Z_ENABLE_PIN       10
#define Z_STOP_PIN         2

#define E0_STEP_PIN         24
#define E0_DIR_PIN          21
#define E0_ENABLE_PIN       10

/* future proofing */
#define __FS  20
#define __FD  19
#define __GS  18
#define __GD  13

#define UNUSED_PWM           14 /* PWM on LEFT connector */

#define E1_STEP_PIN         -1 // 21
#define E1_DIR_PIN          -1 // 20
#define E1_ENABLE_PIN       -1 // 19

#define E2_STEP_PIN         -1 // 21
#define E2_DIR_PIN          -1 // 20
#define E2_ENABLE_PIN       -1 // 18

#define SDPOWER            -1
#define SDSS               11
#define SDCARDDETECT       -1 // 10 optional also used as mode pin
#define LED_PIN            -1
#define FAN_PIN            14 /* PWM on MIDDLE connector */
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN        3 /*DONE PWM on RIGHT connector */
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define TEMP_0_PIN          0 // ANALOG INPUT NUMBERING
#define TEMP_1_PIN          1 // ANALOG
#define TEMP_2_PIN         -1 // 2
#define HEATER_BED_PIN      4
#define TEMP_BED_PIN        2 // 1,2 or I2C

#define I2C_SCL       16
#define I2C_SDA       17

#endif

#if MOTHERBOARD == 301
#define KNOWN_BOARD
/*****************************************************************
* Rambo Pin Assignments
******************************************************************/

#ifndef __AVR_ATmega2560__
#error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH true

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

#ifdef BARICUDA
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

#define DIGIPOTSS_PIN 38
#define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13
#define FAN_PIN            8
#define PS_ON_PIN          4
#define KILL_PIN           -1 //80 with Smart Controller LCD
#define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.

#ifdef ULTRA_LCD
  #define KILL_PIN 80
  #ifdef NEWPANEL
   //arduino pin which triggers an piezzo beeper
    #define BEEPER 79      // Beeper on AUX-4
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

    #define SDCARDDETECT 81    // Ramps does not use this port

    //encoder rotation values
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1
  #else //old style panel with shift register
    //arduino pin witch triggers an piezzo beeper
    #define BEEPER 33    No Beeper added
    //buttons are attached to a shift register
    // Not wired this yet
    // #define SHIFT_CLK 38
    // #define SHIFT_LD 42
    // #define SHIFT_OUT 40
    // #define SHIFT_EN 17

    #define LCD_PINS_RS 75
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29

    //encoder rotation values
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1

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
  #endif
#endif //ULTRA_LCD


#endif

/****************************************************************************************
* MegaTronics
*
****************************************************************************************/
#if MOTHERBOARD == 70
#define KNOWN_BOARD 1


 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif


#define LARGE_FLASH        true

#define X_STEP_PIN         26
#define X_DIR_PIN          28
#define X_ENABLE_PIN       24
#define X_MIN_PIN          41
#define X_MAX_PIN          37

#define Y_STEP_PIN         60 // A6
#define Y_DIR_PIN          61 // A7
#define Y_ENABLE_PIN       22
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         54 // A0
#define Z_DIR_PIN          55 // A1
#define Z_ENABLE_PIN       56 // A2
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E0_STEP_PIN        31
#define E0_DIR_PIN         32
#define E0_ENABLE_PIN      38

#define E1_STEP_PIN        34
#define E1_DIR_PIN         36
#define E1_ENABLE_PIN      30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13


#define FAN_PIN            7 // IO pin. Buffer needed
#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       9    // EXTRUDER 1
#define HEATER_1_PIN       8    // EXTRUDER 2 (FAN On Sprinter)
#define HEATER_2_PIN       -1

#if TEMP_SENSOR_0 == -1
#define TEMP_0_PIN         8   // ANALOG NUMBERING
#else
#define TEMP_0_PIN         13   // ANALOG NUMBERING

#endif

#define TEMP_1_PIN         15   // ANALOG NUMBERING
#define TEMP_2_PIN         -1   // ANALOG NUMBERING
#define HEATER_BED_PIN     10   // BED
#define TEMP_BED_PIN       14   // ANALOG NUMBERING

#define BEEPER 33     // Beeper on AUX-4


#ifdef ULTRA_LCD

  #ifdef NEWPANEL
  //arduino pin which triggers an piezzo beeper

    #define LCD_PINS_RS 16
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29

    //buttons are directly attached using AUX-2
    #define BTN_EN1 59
    #define BTN_EN2 64
    #define BTN_ENC 43  //the click
    
    #define BLEN_C 2
    #define BLEN_B 1
    #define BLEN_A 0
    
    #define SDCARDDETECT -1		// Ramps does not use this port
    
      //encoder rotation values
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1
#endif
#endif //ULTRA_LCD

#endif

/****************************************************************************************
* MegaTronics v2.0
*
****************************************************************************************/
#if MOTHERBOARD == 701
 #define KNOWN_BOARD 1
 
 
 #ifndef __AVR_ATmega2560__
 #error Oops! Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
 
 #define LARGE_FLASH        true
 
 #define X_STEP_PIN 26
 #define X_DIR_PIN 27
 #define X_ENABLE_PIN 25
 #define X_MIN_PIN 37
 #define X_MAX_PIN 40 //2 //Max endstops default to disabled "-1", set to commented value to enable.
 
 #define Y_STEP_PIN 4 // A6
 #define Y_DIR_PIN 54 // A0
 #define Y_ENABLE_PIN 5
 #define Y_MIN_PIN 41
 #define Y_MAX_PIN 38 //15
 
 #define Z_STEP_PIN 56 // A2
 #define Z_DIR_PIN 60 // A6
 #define Z_ENABLE_PIN 55 // A1
 #define Z_MIN_PIN 18
 #define Z_MAX_PIN 19
 
 #define E0_STEP_PIN 35
 #define E0_DIR_PIN 36
 #define E0_ENABLE_PIN 34
 
 #define E1_STEP_PIN 29
 #define E1_DIR_PIN 39
 #define E1_ENABLE_PIN 28
 
 #define E2_STEP_PIN 23
 #define E2_DIR_PIN 24
 #define E2_ENABLE_PIN 22
 
 #define SDPOWER -1
 #define SDSS 53
 #define LED_PIN 13
 
 #define FAN_PIN 7
 #define FAN2_PIN 6
 #define PS_ON_PIN 12
 #define KILL_PIN -1
 
 #define HEATER_0_PIN 9 // EXTRUDER 1
 #define HEATER_1_PIN 8 // EXTRUDER 2
 #define HEATER_2_PIN -1
 
 #if TEMP_SENSOR_0 == -1
   #define TEMP_0_PIN 4 // ANALOG NUMBERING
 #else
   #define TEMP_0_PIN 13 // ANALOG NUMBERING
 #endif
 
 
 #if TEMP_SENSOR_1 == -1
   #define TEMP_1_PIN 8 // ANALOG NUMBERING
 #else
   #define TEMP_1_PIN 15 // ANALOG NUMBERING
 #endif
 
 #define TEMP_2_PIN -1 // ANALOG NUMBERING
 
 #define HEATER_BED_PIN 10 // BED
 
 #if TEMP_SENSOR_BED == -1
   #define TEMP_BED_PIN 8 // ANALOG NUMBERING
 #else 
   #define TEMP_BED_PIN 14 // ANALOG NUMBERING
 #endif
 
 #define BEEPER 64	
 
 
 #define LCD_PINS_RS 14
 #define LCD_PINS_ENABLE 15
 #define LCD_PINS_D4 30
 #define LCD_PINS_D5 31
 #define LCD_PINS_D6 32
 #define LCD_PINS_D7 33
 
 
 //buttons are directly attached using keypad
 #define BTN_EN1 61
 #define BTN_EN2 59
 #define BTN_ENC 43 //the click
 
 #define BLEN_C 2
 #define BLEN_B 1
 #define BLEN_A 0
 
 #define SDCARDDETECT -1	// Megatronics does not use this port
 
   //encoder rotation values
 #define encrot0 0
 #define encrot1 2
 #define encrot2 3
 #define encrot3 1

#endif


/****************************************************************************************
* Minitronics v1.0
*
****************************************************************************************/
#if MOTHERBOARD == 702
 #define KNOWN_BOARD 1
 
 
 #ifndef __AVR_ATmega1281__
 #error Oops! Make sure you have 'Minitronics ' selected from the 'Tools -> Boards' menu.
 #endif
 
 #define LARGE_FLASH        true
 
 #define X_STEP_PIN 48
 #define X_DIR_PIN 47
 #define X_ENABLE_PIN 49
 #define X_MIN_PIN 5
 #define X_MAX_PIN -1 //2 //Max endstops default to disabled "-1", set to commented value to enable.
 
 #define Y_STEP_PIN 39 // A6
 #define Y_DIR_PIN 40 // A0
 #define Y_ENABLE_PIN 38
 #define Y_MIN_PIN 2
 #define Y_MAX_PIN -1 //15
 
 #define Z_STEP_PIN 42 // A2
 #define Z_DIR_PIN 43 // A6
 #define Z_ENABLE_PIN 41 // A1
 #define Z_MIN_PIN 6
 #define Z_MAX_PIN -1
 
 #define E0_STEP_PIN 45
 #define E0_DIR_PIN 44
 #define E0_ENABLE_PIN 27
 
 #define E1_STEP_PIN 36
 #define E1_DIR_PIN 35
 #define E1_ENABLE_PIN 37
 
 #define E2_STEP_PIN -1
 #define E2_DIR_PIN -1
 #define E2_ENABLE_PIN -1
 
 #define SDPOWER -1
 #define SDSS 16
 #define LED_PIN 46
 
 #define FAN_PIN 9
 #define FAN2_PIN -1
 #define PS_ON_PIN -1
 #define KILL_PIN -1
 
 #define HEATER_0_PIN 7 // EXTRUDER 1
 #define HEATER_1_PIN 8 // EXTRUDER 2
 #define HEATER_2_PIN -1
 

 #define TEMP_0_PIN 7 // ANALOG NUMBERING
 #define TEMP_1_PIN 6 // ANALOG NUMBERING
 #define TEMP_2_PIN -1 // ANALOG NUMBERING
 
 #define HEATER_BED_PIN 3 // BED
 #define TEMP_BED_PIN 6 // ANALOG NUMBERING
 
 #define BEEPER -1	
 
 
 #define LCD_PINS_RS -1
 #define LCD_PINS_ENABLE -1
 #define LCD_PINS_D4 -1
 #define LCD_PINS_D5 -1
 #define LCD_PINS_D6 -1
 #define LCD_PINS_D7 -1
 
 
 //buttons are directly attached using keypad
 #define BTN_EN1 -1
 #define BTN_EN2 -1
 #define BTN_ENC -1 //the click
 
 #define BLEN_C 2
 #define BLEN_B 1
 #define BLEN_A 0
 
 #define SDCARDDETECT -1	// Megatronics does not use this port
 
   //encoder rotation values
 #define encrot0 0
 #define encrot1 2
 #define encrot2 3
 #define encrot3 1

#endif

/****************************************************************************************
* Cheaptronic v1.0
*
****************************************************************************************/
#if MOTHERBOARD == 2
 #define KNOWN_BOARD 1

 #ifndef __AVR_ATmega2560__
 #error Oops! Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif

 #define LARGE_FLASH        true

 //X motor stepper
 #define X_STEP_PIN 14
 #define X_DIR_PIN 15
 #define X_ENABLE_PIN 24
 
 //X endstop
 #define X_MIN_PIN 3
 #define X_MAX_PIN -1

 //Y motor stepper
 #define Y_STEP_PIN 35
 #define Y_DIR_PIN 36
 #define Y_ENABLE_PIN 31

 //Y endstop
 #define Y_MIN_PIN 2
 #define Y_MAX_PIN -1
 
 //Z motor stepper
 #define Z_STEP_PIN 40
 #define Z_DIR_PIN 41
 #define Z_ENABLE_PIN 37

 //Z endstop
 #define Z_MIN_PIN 5
 #define Z_MAX_PIN -1
 
 //Extruder 0 stepper
 #define E0_STEP_PIN 26
 #define E0_DIR_PIN 28
 #define E0_ENABLE_PIN 25

 //Extruder 1 stepper
 #define E1_STEP_PIN 33
 #define E1_DIR_PIN 34
 #define E1_ENABLE_PIN 30

 #define SDPOWER -1
 #define SDSS -1
 #define LED_PIN -1

 //FAN
 #define FAN_PIN -1

 #define PS_ON_PIN -1
 #define KILL_PIN -1

 #define HEATER_0_PIN 19 // EXTRUDER 1
 #define HEATER_1_PIN 23 // EXTRUDER 2
 //HeatedBad
 #define HEATER_BED_PIN 22
 //Cheaptronic v1.0 hasent EXTRUDER 3
 #define HEATER_2_PIN -1
 
 //Temperature sensors
 #define TEMP_0_PIN 15
 #define TEMP_1_PIN 14
 #define TEMP_2_PIN -1
 #define TEMP_BED_PIN 13

 //Cheaptronic v1.0 dont support LCD
 #define LCD_PINS_RS -1
 #define LCD_PINS_ENABLE -1
 #define LCD_PINS_D4 -1
 #define LCD_PINS_D5 -1
 #define LCD_PINS_D6 -1
 #define LCD_PINS_D7 -1

 //Cheaptronic v1.0 dont support keypad
 #define BTN_EN1 -1
 #define BTN_EN2 -1
 #define BTN_ENC -1

 #define BLEN_C 2
 #define BLEN_B 1
 #define BLEN_A 0

 //Cheaptronic v1.0 does not use this port
 #define SDCARDDETECT -1

 //encoder rotation values
 #define encrot0 0
 #define encrot1 2
 #define encrot2 3
 #define encrot3 1

#endif



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
#endif

