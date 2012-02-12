#ifndef PINS_H
#define PINS_H

#if MOTHERBOARD == 99
#define	KNOWN_BOARD 1

#define X_STEP_PIN          2
#define X_DIR_PIN           3
#define X_ENABLE_PIN        -1
#define X_MIN_PIN           -1
#define X_MAX_PIN           16

#define Y_STEP_PIN          5
#define Y_DIR_PIN           6
#define Y_ENABLE_PIN       -1
#define Y_MIN_PIN           67
#define Y_MAX_PIN          -1

#define Z_STEP_PIN          62
#define Z_DIR_PIN           63
#define Z_ENABLE_PIN       -1
#define Z_MIN_PIN           59
#define Z_MAX_PIN          -1

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
* Arduino pin assignment
*
*                  ATMega168
*                   +-\/-+
*             PC6  1|    |28  PC5 (AI 5 / D19)
*       (D 0) PD0  2|    |27  PC4 (AI 4 / D18)
*       (D 1) PD1  3|    |26  PC3 (AI 3 / D17)
*       (D 2) PD2  4|    |25  PC2 (AI 2 / D16)
*  PWM+ (D 3) PD3  5|    |24  PC1 (AI 1 / D15)
*       (D 4) PD4  6|    |23  PC0 (AI 0 / D14)
*             VCC  7|    |22  GND
*             GND  8|    |21  AREF
*             PB6  9|    |20  AVCC
*             PB7 10|    |19  PB5 (D 13)
*  PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
*  PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
*       (D 7) PD7 13|    |16  PB2 (D 10) PWM
*       (D 8) PB0 14|    |15  PB1 (D 9)  PWM
*                   +----+
****************************************************************************************/
#if MOTHERBOARD == 0
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega168__
#error Oops!  Make sure you have 'Arduino Diecimila' selected from the boards menu.
#endif

#define X_STEP_PIN          2
#define X_DIR_PIN           3
#define X_ENABLE_PIN       -1
#define X_MIN_PIN           4
#define X_MAX_PIN           9

#define Y_STEP_PIN         10
#define Y_DIR_PIN           7
#define Y_ENABLE_PIN       -1
#define Y_MIN_PIN           8
#define Y_MAX_PIN          13

#define Z_STEP_PIN         19
#define Z_DIR_PIN          18
#define Z_ENABLE_PIN        5
#define Z_MIN_PIN          17
#define Z_MAX_PIN          16

#define E0_STEP_PIN         11
#define E0_DIR_PIN          12
#define E0_ENABLE_PIN       -1

#define SDPOWER          -1
#define SDSS          -1
#define LED_PIN            -1
#define FAN_PIN            -1
#define PS_ON_PIN          15
#define KILL_PIN           -1

#define HEATER_0_PIN        6
#define HEATER_1_PIN        -1
#define HEATER_2_PIN        -1
#define TEMP_0_PIN          0    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN          -1   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_2_PIN          -1   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define HEATER_BED_PIN      -1
#define TEMP_BED_PIN        -1
#endif



/****************************************************************************************
* Sanguino/RepRap Motherboard with direct-drive extruders
*
*                        ATMega644P
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
#if MOTHERBOARD == 1
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          18
#define X_ENABLE_PIN       19
#define X_MIN_PIN          20
#define X_MAX_PIN          21

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_ENABLE_PIN       19
#define Y_MIN_PIN          25
#define Y_MAX_PIN          26

#define Z_STEP_PIN         29
#define Z_DIR_PIN          30
#define Z_ENABLE_PIN       31
#define Z_MIN_PIN           2
#define Z_MAX_PIN           1

#define E0_STEP_PIN         12
#define E0_DIR_PIN          16
#define E0_ENABLE_PIN        3

#define SDPOWER          -1
#define SDSS          -1
#define LED_PIN             0
#define FAN_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN       14
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define TEMP_0_PIN          4 //D27   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN         -1 
#define TEMP_2_PIN         -1 
#define HEATER_BED_PIN     -1
#define TEMP_BED_PIN       -1
/*  Unused (1) (2) (3) 4 5 6 7 8 9 10 11 12 13 (14) (15) (16) 17 (18) (19) (20) (21) (22) (23) 24 (25) (26) (27) 28 (29) (30) (31)  */



#endif


/****************************************************************************************
* RepRap Motherboard  ****---NOOOOOO RS485/EXTRUDER CONTROLLER!!!!!!!!!!!!!!!!!---*******
*
****************************************************************************************/
#if MOTHERBOARD == 2
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN      15
#define X_DIR_PIN       18
#define X_ENABLE_PIN    19
#define X_MIN_PIN       20
#define X_MAX_PIN       21

#define Y_STEP_PIN      23
#define Y_DIR_PIN       22
#define Y_ENABLE_PIN    24
#define Y_MIN_PIN       25
#define Y_MAX_PIN       26

#define Z_STEP_PINN     27
#define Z_DIR_PINN      28
#define Z_ENABLE_PIN    29
#define Z_MIN_PIN       30
#define Z_MAX_PIN       31

#define E0_STEP_PIN      17
#define E0_DIR_PIN       16
#define E0_ENABLE_PIN    -1

#define SDPOWER          -1
#define SDSS          4
#define LED_PIN          0

#define SD_CARD_WRITE    2
#define SD_CARD_DETECT   3
#define SD_CARD_SELECT   4

//our RS485 pins
#define TX_ENABLE_PIN	12
#define RX_ENABLE_PIN	13

//pin for controlling the PSU.
#define PS_ON_PIN       14

#define FAN_PIN         -1
#define KILL_PIN        -1

#define HEATER_0_PIN    -1
#define HEATER_1_PIN    -1
#define HEATER_2_PIN    -1
#define TEMP_0_PIN      -1    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN      -1    
#define TEMP_2_PIN      -1    
#define HEATER_BED_PIN  -1
#define TEMP_BED_PIN    -1

#endif

/****************************************************************************************
* Arduino Mega pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 33
#define MOTHERBOARD 3
#define RAMPS_V_1_3
#endif
#if MOTHERBOARD == 3
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

#ifdef RAMPS_V_1_3

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2   //2 //Max endstops default to disabled "-1", set to commented value to enable.

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15   //15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19   //19

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13
#define FAN_PIN            4
#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10   // EXTRUDER 1
#define HEATER_1_PIN       9    // EXTRUDER 2
#define HEATER_2_PIN       -1   // EXTRUDER 2
#define TEMP_0_PIN         13   // ANALOG NUMBERING
#define TEMP_1_PIN         15   // ANALOG NUMBERING
#define TEMP_2_PIN         -1   // ANALOG NUMBERING
#define HEATER_BED_PIN     8    // BED
#define TEMP_BED_PIN       14   // ANALOG NUMBERING


#else // RAMPS_V_1_1 or RAMPS_V_1_2 as default

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
#endif

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

#endif

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
#define X_MIN_PIN          17
#define X_MAX_PIN          -1

#define Y_STEP_PIN         10
#define Y_DIR_PIN           7
#define Y_ENABLE_PIN       -1
#define Y_MIN_PIN           8
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         13
#define Z_DIR_PIN           3
#define Z_ENABLE_PIN        2
#define Z_MIN_PIN           4
#define Z_MAX_PIN          -1

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
* Gen6 pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 5
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
    #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

//x axis pins
    #define X_STEP_PIN      15
    #define X_DIR_PIN       18
    #define X_ENABLE_PIN    19
    #define X_MIN_PIN       20
    #define X_MAX_PIN       -1
    
    //y axis pins
    #define Y_STEP_PIN      23
    #define Y_DIR_PIN       22
    #define Y_ENABLE_PIN    24
    #define Y_MIN_PIN       25
    #define Y_MAX_PIN       -1
    
    //z axis pins
    #define Z_STEP_PIN      27
    #define Z_DIR_PIN       28
    #define Z_ENABLE_PIN    29
    #define Z_MIN_PIN       30
    #define Z_MAX_PIN       -1
    
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
    #define HEATER_BED_PIN  -1    //changed @ rkoeppl 20110410
    #define TEMP_BED_PIN    -1    //changed @ rkoeppl 20110410
    
    #define SDPOWER          -1
    #define SDSS          17
    #define LED_PIN         -1    //changed @ rkoeppl 20110410
    #define FAN_PIN         -1    //changed @ rkoeppl 20110410
    #define PS_ON_PIN       -1    //changed @ rkoeppl 20110410
    //our pin for debugging.
    
    #define DEBUG_PIN        0
    
    //our RS485 pins
    #define TX_ENABLE_PIN	12
    #define RX_ENABLE_PIN	13

    
#endif

/****************************************************************************************
* Sanguinololu pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 62
#define MOTHERBOARD 6
#define SANGUINOLOLU_V_1_2 
#endif
#if MOTHERBOARD == 6
#define KNOWN_BOARD 1
#ifndef __AVR_ATmega644P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_MIN_PIN          18
#define X_MAX_PIN           -1

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_MIN_PIN          19
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         3
#define Z_DIR_PIN          2
#define Z_MIN_PIN          20
#define Z_MAX_PIN          -1

#define E0_STEP_PIN         1
#define E0_DIR_PIN          0

#define LED_PIN            -1

#define FAN_PIN            -1 

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
#define E0_ENABLE_PIN       14

#else

#define HEATER_BED_PIN       14  // (bed)
#define X_ENABLE_PIN       -1
#define Y_ENABLE_PIN       -1
#define Z_ENABLE_PIN       -1
#define E0_ENABLE_PIN       -1

#endif

#define TEMP_0_PIN          7   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 33 extruder)
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        6   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)
#define SDPOWER            -1
#define SDSS               31

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
#define E1_ENABLE_PIN       51

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
    
    #define BLEN_C 2
    #define BLEN_B 1
    #define BLEN_A 0
    
    #define SDCARDDETECT 38
    
      //encoder rotation values
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1
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
* Teensylu 0.7 pin assingments (ATMEGA90USB)
* Requires the Teensyduino software with Teensy2.0++ selected in arduino IDE!
****************************************************************************************/
#if MOTHERBOARD == 8
#define MOTHERBOARD 8
#define KNOWN_BOARD 1


#define X_STEP_PIN          0  
#define X_DIR_PIN           1  
#define X_ENABLE_PIN       39 
#define X_MIN_PIN          13 
#define X_MAX_PIN          -1    

#define Y_STEP_PIN          2  
#define Y_DIR_PIN           3 
#define Y_ENABLE_PIN       38 
#define Y_MIN_PIN          14 
#define Y_MAX_PIN          -1    

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5 
#define Z_ENABLE_PIN       23 
#define Z_MIN_PIN          15 
#define Z_MAX_PIN          -1    

#define E0_STEP_PIN         6  
#define E0_DIR_PIN          7 
#define E0_ENABLE_PIN       19 



#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     20  // Bed
#define FAN_PIN            22  // Fan   

#define TEMP_0_PIN          7  // Extruder
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        6  // Bed

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
#endif

/****************************************************************************************
* Gen3+ pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 9
#define MOTHERBOARD 6
#define KNOWN_BOARD 1
#ifndef __AVR_ATmega644P__
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          18
#define X_MIN_PIN          20
#define X_MAX_PIN           -1

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_MIN_PIN          25
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         27
#define Z_DIR_PIN          28
#define Z_MIN_PIN          30
#define Z_MAX_PIN          -1

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




#ifndef KNOWN_BOARD
#error Unknown MOTHERBOARD value in configuration.h
#endif

//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN
#if EXTRUDERS == 3
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN
  #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN
#elif EXTRUDERS == 2
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN
  #define _E2_PINS -1
#elif EXTRUDERS == 1
  #define _E1_PINS -1 
  #define _E2_PINS -1
#else
  #error Unsupported number of extruders
#endif
#define SENSITIVE_PINS {0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, LED_PIN, PS_ON_PIN, \
                        HEATER_0_PIN, HEATER_1_PIN, HEATER_2_PIN, \
                        HEATER_BED_PIN, FAN_PIN,                  \
                        _E0_PINS, _E1_PINS, _E2_PINS,             \
                        TEMP_0_PIN, TEMP_1_PIN, TEMP_2_PIN, TEMP_BED_PIN }
#endif
