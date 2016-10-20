/****************************************************************************************
* Motherboard: bq ZUM Mega 3D
* Revision: v0.1
* 
* Compatible with:
*   - bq LCD Smart Controller
*   - RepRapDiscount Smart Controller
****************************************************************************************/

#if MB(BQ_ZUM_MEGA_3D) || MB(BQ_HPH_ZUM_3D)
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

 #define E1_STEP_PIN        36
 #define E1_DIR_PIN         34
 #define E1_ENABLE_PIN      30

 // Digital potentiometers
 #define DIGIPOTSS_PIN      22
 #define DIGIPOT_CHANNELS    { 4, 5, 3 , 0, 1 }

 // Endstops
 #define X_MIN_PIN          3
 #define X_MAX_PIN          79

 #define Y_MIN_PIN          14
 #define Y_MAX_PIN          15

#ifdef LEVEL_SENSOR
 #define Z_MIN_PIN          19
 #define Z_MAX_PIN          18
#else
 #define Z_MIN_PIN          18
 #define Z_MAX_PIN          19
#endif

#ifdef FILAMENT_RUNOUT_SENSOR
	#define FILRUNOUT_PIN     57
#endif // FILAMENT_RUNOUT_SENSOR

 // Heaters
 #define HEATER_0_PIN       9
 #define HEATER_1_PIN       10
 #define HEATER_BED_PIN     -1

 // Thermistors
 #define TEMP_0_PIN         13  // Analog numbering: DIO67
 #define TEMP_1_PIN         14  // Analog numbering: DIO68
 #define TEMP_2_PIN         15  // Analog numbering: DIO69
 #define TEMP_3_PIN         8   // Analog numbering: DIO62
 #define TEMP_BED_PIN       TEMP_1_PIN

 // Cooling Fans
 #define FAN_BLOWER_PIN     12
 #define FAN_BLOCK_PIN		11 
 
 #define FAN_PIN            FAN_BLOWER_PIN 
  

 // Auxiliar Power Channel Driver
 #define AUX_DRIVER_PIN     2

 // External Power Supply
 #define PS_ON_PIN          81

 // LEDs
 #define LED_PIN            13

#endif // BQ_ZUM_3D

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
#else
  #ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
    #define LCD_PINS_RS 16
    #define LCD_PINS_ENABLE 23
    #define LCD_PINS_D4 39
    #define LCD_PINS_D5 17
    #define LCD_PINS_D6 43
    #define LCD_PINS_D7 41
    
     // Button & Encoder
	 #define BTN_EN1             31
	 #define BTN_EN2             33
	 #define BTN_ENC             35
	 // Buzzer
	 #define BEEPER              37
	 
	  // SD Card Reader
	 #define SDSS                53
	 #define SDCARDDETECT        49
  #endif //REPRAP_DISCOUNT_SMART_CONTROLLER
#endif // BQ_LCD_SMART_CONTROLLER 
