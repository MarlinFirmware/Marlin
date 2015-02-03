#ifndef PINS_H
#define PINS_H

/*==============================================================================
| Motherboard: RAMPS
| Revision: v1.4
| 
| Compatible with:
|   - RepRapDiscount Smart Controller
|   - bq LCD Smart Controller
|
==============================================================================*/

#if (MOTHERBOARD == 33)
#define KNOWN_BOARD 1
#define LARGE_FLASH true


#   ifndef __AVR_ATmega2560__
#   error Oops!  Make sure you have 'Arduino Mega' selected from the \
    'Tools -> Boards' menu.
#   endif

// Motors
#define X_STEP_PIN         	54
#define X_DIR_PIN          	55
#define X_ENABLE_PIN       	38

#define Y_STEP_PIN         	60
#define Y_DIR_PIN          	61
#define Y_ENABLE_PIN       	56

#define Z_STEP_PIN         	46
#define Z_DIR_PIN          	48
#define Z_ENABLE_PIN       	62

#define E0_STEP_PIN        	26
#define E0_DIR_PIN         	28
#define E0_ENABLE_PIN      	24

#define E1_STEP_PIN        	36
#define E1_DIR_PIN         	34
#define E1_ENABLE_PIN      	30

// Endstops
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_MIN_PIN          	14
#define Y_MAX_PIN          	15

#define Z_MIN_PIN          	18
#define Z_MAX_PIN          	19

// Heaters
#define HEATER_0_PIN       	10
#define HEATER_BED_PIN     	8

// Thermistors
#define TEMP_0_PIN         	13   // Analog Numbering
#define TEMP_1_PIN         	15   // Analog Numbering
#define TEMP_BED_PIN       	14   // Analog Numbering

// Cooling Fans
#define FAN_PIN            	9

// External Power Supply 
#define PS_ON_PIN          	12

// LEDs
#define LED_PIN            	13

/*------------------------------------------------------------------------------
| RepRapDiscount Smart Controller:
+-----------------------------------------------------------------------------*/
#   ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
// Reset Button
#   define KILL_PIN           	41
// LCD
#   define LCD_PINS_RS        	16
#   define LCD_PINS_ENABLE    	17
#   define LCD_PINS_D4        	23
#   define LCD_PINS_D5        	25
#   define LCD_PINS_D6        	27
#   define LCD_PINS_D7        	29
// Button & Encoder
#   define BTN_EN1            	31
#   define BTN_EN2            	33
#   define BTN_ENC            	35
// Buzzer
#   define BEEPER             	37
// SD Card Reader
#   define SDSS               	53
#   define SDCARDDETECT       	49
#   endif // REPRAP_DISCOUNT_SMART_CONTROLLER

/*------------------------------------------------------------------------------
| bq LCD Smart Controller:
+-----------------------------------------------------------------------------*/
#   ifdef BQ_LCD_SMART_CONTROLLER
// Reset Button
#   define KILL_PIN           	41
// LCD
#   define LCD_PIN_BL         	39
#   define LCD_PINS_RS        	16
#   define LCD_PINS_ENABLE    	17
#   define LCD_PINS_D4        	23
// Button & Encoder
#   define BTN_EN1            	31
#   define BTN_EN2            	33
#   define BTN_ENC            	35
// Buzzer
#   define BEEPER             	37
// SD Card Reader
#   define SDSS                 53
#   define SDCARDDETECT         49
#   endif // BQ_LCD_SMART_CONTROLLER

#endif // (MOTHERBOARD == 33)





/*==============================================================================
| Motherboard: bq CNC
| Revision: v1.1
| 
| Compatible with:
|   - bq LCD Smart Controller
|   - RepRapDiscount Smart Controller
|
==============================================================================*/

#if (MOTHERBOARD == 40)
#define KNOWN_BOARD 1
#define LARGE_FLASH true

#   ifndef __AVR_ATmega2560__
#   error Oops!  Make sure you have 'Arduino Mega' selected from the \
    'Tools -> Boards' menu.
#   endif // __AVR_ATmega2560__

// Motors
#define X_STEP_PIN      	54
#define X_DIR_PIN       	55
#define X_ENABLE_PIN    	38

#define Y_STEP_PIN      	60
#define Y_DIR_PIN       	61
#define Y_ENABLE_PIN    	56

#define Z_STEP_PIN      	46
#define Z_DIR_PIN       	48
#define Z_ENABLE_PIN    	77

#define E0_STEP_PIN     	26
#define E0_DIR_PIN      	28
#define E0_ENABLE_PIN   	24

#define E1_STEP_PIN     	36
#define E1_DIR_PIN      	34
#define E1_ENABLE_PIN   	30

// Digital potentiometers
#define DIGIPOTSS_PIN   	22
#define DIGIPOT_CHANNELS    { 4, 5, 3 , 0, 1 }

// Endstops
#define X_MIN_PIN       	3
#define X_MAX_PIN       	79

#define Y_MIN_PIN       	14
#define Y_MAX_PIN       	15

#define Z_MIN_PIN       	18
#define Z_MAX_PIN       	19

// Heaters
#define HEATER_0_PIN    	9
#define HEATER_1_PIN       	10
#define HEATER_BED_PIN  	8

// Thermistors
#define TEMP_0_PIN      	13  // Analog numbering: DIO67
#define TEMP_1_PIN      	14  // Analog numbering: DIO68
#define TEMP_2_PIN      	15  // Analog numbering: DIO69
#define TEMP_3_PIN      	8   // Analog numbering: DIO62
#define TEMP_BED_PIN    	TEMP_1_PIN

// Cooling Fans
#define FAN_0_PIN       	12
#define FAN_1_PIN       	7
#define FAN_PIN         	FAN_0_PIN    

// Auxiliar Power Channel Driver
#define AUX_DRIVER_PIN    2

// External Power Supply
#define PS_ON_PIN       	81

// LEDs
#define LED_PIN         	13

/*------------------------------------------------------------------------------
| RepRapDiscount Smart Controller:
+-----------------------------------------------------------------------------*/
#   ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
// Reset Button
#define KILL_PIN        	41
// LCD
#define LCD_PINS_RS         16
#define LCD_PINS_ENABLE     17
#define LCD_PINS_D4         23
#define LCD_PINS_D5         25
#define LCD_PINS_D6         27
#define LCD_PINS_D7         29
// Button & Encoder
#define BTN_EN1             31
#define BTN_EN2             33
#define BTN_ENC             35
// Buzzer
#define BEEPER              37
// SD Card Reader
#define SDSS                53
#define SDCARDDETECT        49
#   endif // REPRAP_DISCOUNT_SMART_CONTROLLER

/*------------------------------------------------------------------------------
| bq LCD Smart Controller:
+-----------------------------------------------------------------------------*/
#   ifdef BQ_LCD_SMART_CONTROLLER
// Reset Button
#   define KILL_PIN           	41
// LCD
#   define LCD_PIN_BL          39
#   define LCD_PINS_RS         16
#   define LCD_PINS_ENABLE     17
#   define LCD_PINS_D4         23
// Button & Encoder
#   define BTN_EN1             31
#   define BTN_EN2             33
#   define BTN_ENC             35
// Buzzer
#   define BEEPER              37
// SD Card Reader
#   define SDSS                53
#   define SDCARDDETECT        49
#   endif // BQ_LCD_SMART_CONTROLLER

#endif // (MOTHERBOARD == 77)





/*==============================================================================
| Defines for not used pins
==============================================================================*/

// Microsteppers
#ifndef X_MS1_PIN
#define X_MS1_PIN 			-1
#endif
#ifndef X_MS2_PIN
#define X_MS2_PIN 			-1
#endif

#ifndef Y_MS1_PIN
#define Y_MS1_PIN 			-1
#endif
#ifndef Y_MS2_PIN
#define Y_MS2_PIN 			-1
#endif

#ifndef Z_MS1_PIN
#define Z_MS1_PIN 			-1
#endif
#ifndef Z_MS2_PIN
#define Z_MS2_PIN 			-1
#endif

#ifndef E0_MS1_PIN
#define E0_MS1_PIN 			-1
#endif
#ifndef E0_MS2_PIN
#define E0_MS2_PIN 			-1
#endif

#ifndef E1_MS1_PIN
#define E1_MS1_PIN 			-1
#endif
#ifndef E1_MS2_PIN
#define E1_MS2_PIN 			-1
#endif

// Digital Potentiometers
#ifndef DIGIPOTSS_PIN
#define DIGIPOTSS_PIN 		-1
#endif

// Heaters
#ifndef HEATER_1_PIN
#define HEATER_1_PIN		-1
#endif

#ifndef HEATER_2_PIN
#define HEATER_2_PIN		-1
#endif

#ifndef HEATER_BED_PIN
#define HEATER_BED_PIN		-1
#endif

// Thermistors
#ifndef TEMP_1_PIN
#define TEMP_1_PIN			-1
#endif

#ifndef TEMP_2_PIN
#define TEMP_2_PIN			-1
#endif

#ifndef TEMP_BED_PIN
#define TEMP_BED_PIN		-1
#endif

// SD Card Reader
#ifndef SDPOWER
#define SDPOWER        		-1
#endif

#ifndef SDSS
#define SDSS               	-1
#endif

#ifndef SDCARDDETECT
#define SDCARDDETECT       	-1
#endif

// Auto-leveling platform servos
#ifdef SERVO0_PIN
#define SERVO0_PIN      	-1
#endif

#ifdef SERVO1_PIN
#define SERVO1_PIN      	-1
#endif

#ifdef SERVO2_PIN
#define SERVO2_PIN      	-1
#endif

#ifdef SERVO3_PIN
#define SERVO3_PIN      	-1
#endif





/*==============================================================================
| Other defines
==============================================================================*/

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

#define SENSITIVE_PINS { 0, 1, \
                    X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, \
                    Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, \
                    Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, \
                    PS_ON_PIN, HEATER_BED_PIN, FAN_PIN, \
                    _E0_PINS _E1_PINS _E2_PINS \
                    analogInputToDigitalPin(TEMP_0_PIN), \
                    analogInputToDigitalPin(TEMP_1_PIN), \
                    analogInputToDigitalPin(TEMP_2_PIN), \
                    analogInputToDigitalPin(TEMP_BED_PIN) }

#endif // PINS_H
