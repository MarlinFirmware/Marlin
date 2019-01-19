#pragma once

#define BOARD_NAME "ADSK"

#include "pins_CNC_shield.h"

#define SDSS                52

#define SERVO0_PIN          61 // Analog pin 7, Digital pin 61
#define Z_MIN_PROBE_PIN     62 // Analog pin 8, Digital pin 62

#if defined(ZONESTAR_LCD)

/*
 The 2004 LCD should be powered with 5V.
 The next LCD pins RS,D4,D5,D6,D7 have internal pull-ups to 5V and as result the 5V will be on these pins.
 Luckily these internal pull-ups have really high resistance and adding 33K pull-down resistors will create
 simple voltage divider that will bring the voltage down just slightly bellow 3.3V.
 
 This LCD also has buttons that connected to the same ADC pin with different voltage divider combinations.
 On the LCD panel there is internal pull-up resistor of the 4.7K connected to 5V.
 Connecting another 4.7K pull-down resistor between ADC pin and the GND
   will result in scaled values for voltage dividers and will bring them down to be always below 3.3V.
 
 For 2004 LCD to work with 3.3V board like Arduino DUE the next required:
 Pull-down resistors of 33K between each of LCD pins RS,D4,D5,D6,D7 and the GND.
 Pull-down resistor of 4.7K between ADC_KEYPAD_PIN and the GND
 
 All these modifications will still work with 5V based boards but require proper scaled ADC values
 */

#if defined(__SAM3X8E__)
    #define AREF_VOLTS      3.3
#else
    #define AREF_VOLTS      5.0
#endif

#define LCD_PINS_ENABLE     14
#define LCD_PINS_RS         15
#define LCD_PINS_D4         16
#define LCD_PINS_D5         17
#define LCD_PINS_D6         18
#define LCD_PINS_D7         19
#define ADC_KEYPAD_PIN      6//60 // Analog pin 6, Digital pin 60

/* 
 The below defines will scale all the values to work properly on both 5V (Mega) and 3.3V (DUE) boards
 with all pull-up resistors added for 3.3V
*/

#define ADC_BUTTONS_VALUE_SCALE         (5.0/AREF_VOLTS) // The LCD module pullup voltage is 5.0V but ADC reference voltage is 3.3V

#define ADC_BUTTONS_R_PULLDOWN          4.7  // Moves voltage down to be bellow 3.3V instead of 5V
// the resistors values will be scaled because of 4.7K pulldown parallel resistor 
#define _ADC_BUTTONS_R_SCALED(r)        ((r)*ADC_BUTTONS_R_PULLDOWN/((r)+ADC_BUTTONS_R_PULLDOWN))

// buttons pullup resistor
#define ADC_BUTTONS_R_PULLUP            4.7 // the resistor on the 2004 LCD panel
// buttons resistors with scaled values because of parallel pulldown resistor
#define ADC_BUTTONS_LEFT_R_PULLDOWN     _ADC_BUTTONS_R_SCALED(0.47)
#define ADC_BUTTONS_RIGHT_R_PULLDOWN    _ADC_BUTTONS_R_SCALED(4.7)
#define ADC_BUTTONS_UP_R_PULLDOWN       _ADC_BUTTONS_R_SCALED(1.0)
#define ADC_BUTTONS_DOWN_R_PULLDOWN     _ADC_BUTTONS_R_SCALED(10.0)
#define ADC_BUTTONS_MIDDLE_R_PULLDOWN   _ADC_BUTTONS_R_SCALED(2.2)

#endif /* ZONESTAR_LCD */


/* RJ45 8 pins extruder connector

1 - GND (Please do not connect to the same GND as extruder heater to prevent ground offset voltage)
2 - thermistor
3 - SERVO PWM
4 - extruder heater
5 - FAN (print cooling)
6 - FAN (extruder cooling)
7 - Probe signal
8 - 5V

Standard ethernet pairs: 1&2, 3&6, 4&5, 7&8
Use CAT7 cable to have all pairs shielded

*/
