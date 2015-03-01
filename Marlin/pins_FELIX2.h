/****************************************************************************************
* FELIXprinters v2.0/3.0 pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == BOARD_FELIX2
#define KNOWN_BOARD 1

//////////////////FIX THIS//////////////
#ifndef __AVR_ATmega1280__
#ifndef __AVR_ATmega2560__
#error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif
#endif

#define LARGE_FLASH        true

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           -1

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          -1

#define Z2_STEP_PIN        -1
#define Z2_DIR_PIN         -1
#define Z2_ENABLE_PIN      -1

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define SDPOWER            1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9 // (Sprinter config)
#define PS_ON_PIN          12

#define KILL_PIN           -1

#define HEATER_0_PIN       10   // EXTRUDER 1
#define HEATER_1_PIN       7    // EXTRUDER 2 (FAN On Sprinter)
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     8    // BED

#define TEMP_0_PIN         13   // ANALOG NUMBERING
#define TEMP_1_PIN         15   // ANALOG NUMBERING
#define TEMP_2_PIN         -1   // ANALOG NUMBERING

#define TEMP_BED_PIN       14   // ANALOG NUMBERING

#ifdef ULTRA_LCD

#ifdef NEWPANEL
//encoder rotation values
#define encrot0 0
#define encrot1 2
#define encrot2 3
#define encrot3 1

#define BLEN_A 0
#define BLEN_B 1
#define BLEN_C 2

#define LCD_PINS_RS 16
#define LCD_PINS_ENABLE 17
#define LCD_PINS_D4 23
#define LCD_PINS_D5 25
#define LCD_PINS_D6 27
#define LCD_PINS_D7 29

//arduino pin which triggers an piezzo beeper
#define BEEPER 33	 // Beeper on AUX-4

//buttons are directly attached using AUX-2
#define BTN_EN1 37
#define BTN_EN2 35
#define BTN_ENC 31  //the click

#define SDCARDDETECT 6  // Ramps does not use this port
#endif //NEWPANEL
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

#endif//MOTHERBOARD == BOARD_FELIX2

