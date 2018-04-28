//ANYCUBIC TRIGORILLA MAINBOARD
#ifndef __AVR_ATmega2560__
	#error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "TRIGORILLA"
#define LARGE_FLASH        true

// Misc PINs
#define BUZZER 			   31
#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13
#define Z_MIN_PROBE_PIN     2
#define FIL_RUNOUT_PIN	   19

#ifdef OutageTest
	#define OUTAGETEST_PIN  79
	#define OUTAGECON_PIN   58
#endif

// Steppers
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Y2_STEP_PIN        36
#define Y2_DIR_PIN         34
#define Y2_ENABLE_PIN      30

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62

#define Z2_STEP_PIN        36
#define Z2_DIR_PIN         34
#define Z2_ENABLE_PIN      30

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

// EndStops
#define X_MIN_PIN           3
#define Y_MIN_PIN          42
#define Z_MIN_PIN          18

#define X_MAX_PIN          43
#define Y_MAX_PIN          -1 
#define Z_MAX_PIN          -1

// Fans
#define FAN_PIN             9
#define FAN2_PIN           44
#define CONTROLLER_FAN_PIN  7

// Heaters
#define HEATER_0_PIN       10
#define HEATER_1_PIN       45
#define HEATER_BED_PIN      8

// Temperatursensoren
#define TEMP_0_PIN         13
#define TEMP_1_PIN         15
#define TEMP_2_PIN         12
#define TEMP_BED_PIN       14

// Servos
#ifdef NUM_SERVOS
	#define SERVO0_PIN      11
	
	#if NUM_SERVOS > 1
		#define SERVO1_PIN    6
	#endif
	
	#if NUM_SERVOS > 2
		#define SERVO2_PIN    5
	#endif
	
	#if NUM_SERVOS > 3
		#define SERVO3_PIN    4
	#endif
#endif	

#if defined(ANYCUBIC_TFT_MODEL)
  #define BEEPER_PIN       31
  #define SD_DETECT_PIN    49
#endif

// LCD
#if defined(REPRAP_DISCOUNT_SMART_CONTROLLER) || defined(G3D_PANEL) || defined(ANYCUBIC_TFT_MODEL)
	#define KILL_PIN        41
#else
	#define KILL_PIN        -1
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
			#define BEEPER_PIN 31
			
			#define BTN_EN1 33
			#define BTN_EN2 33
			#define BTN_ENC 35
			
			#define SD_DETECT_PIN 49
			#elif defined(LCD_I2C_PANELOLU2)
				#define BTN_EN1 47  //reverse if the encoder turns the wrong way.
				#define BTN_EN2 43
				#define BTN_ENC 32
				#define SDSS 53
				#define SD_DETECT_PIN -1
				#define KILL_PIN 41
			#elif defined(LCD_I2C_VIKI)
				#define BTN_EN1 22  //reverse if the encoder turns the wrong way.
				#define BTN_EN2 7
				#define BTN_ENC -1
				#define SDSS 53
				#define SD_DETECT_PIN 49
			#elif defined(FULL_GRAPHIC_SMALL_PANEL)
				#define BEEPER_PIN 37

				// Pins for DOGM SPI LCD Support
				#define DOGLCD_A0  23
				#define DOGLCD_CS  27
				#define LCD_PIN_BL	25	// backlight LED on PA3

				#define KILL_PIN 41
				// GLCD features
				//#define LCD_CONTRAST 190
				// Uncomment screen orientation
				// #define LCD_SCREEN_ROT_90
				// #define LCD_SCREEN_ROT_180
				// #define LCD_SCREEN_ROT_270
				//The encoder and click button
				#define BTN_EN1 33
				#define BTN_EN2 -1
				#define BTN_ENC 35  //the click switch
				//not connected to a pin
				#define SD_DETECT_PIN 49
			#elif defined(MULTIPANEL)
				//         #define BEEPER_PIN 37
				// Pins for DOGM SPI LCD Support
				#define DOGLCD_A0  17
				#define DOGLCD_CS  16
				#define LCD_PIN_BL	23	// backlight LED on A11/D65
				#define SDSS   53

				#define KILL_PIN 64
				// GLCD features
				//#define LCD_CONTRAST 190
				// Uncomment screen orientation
				// #define LCD_SCREEN_ROT_90
				// #define LCD_SCREEN_ROT_180
				// #define LCD_SCREEN_ROT_270
				//The encoder and click button
				#define BTN_EN1 -1
				#define BTN_EN2 33
				#define BTN_ENC 35  //the click switch
				//not connected to a pin
				#define SD_DETECT_PIN 49
			#else
				//arduino pin which triggers an piezzo beeper
				#define BEEPER_PIN 31  // Beeper on AUX-4
			
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
				#define BTN_ENC -1  //the click
			#endif
			
			#ifdef G3D_PANEL
				#define SD_DETECT_PIN 49
				#else
				#define SD_DETECT_PIN -1  // Ramps does not use this port
			#endif
			
		#endif
		
		#define LCD_PINS_RS 16
		#define LCD_PINS_ENABLE 17
		#define LCD_PINS_D4 23
		#define LCD_PINS_D5 25
		#define LCD_PINS_D6 27
		#define LCD_PINS_D7 29
		
	#endif
#endif	