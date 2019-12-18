# Marlin
 minor modifications
 
 new Board Crealty Ender 2: 
   new file: pins_MELZI_ENDER2.h
   modification: pins.h
   
   no beeper
   "Check"-port A2 / D29 for filament sensor
   "Off"-port, right pin PC1/D17 for Neopixels
   
  modifications for material preselection
     modification: Configuration.h
	  new 
	     defines PREHEAT_MATERIAL_COUNT X // 0 = no Preheat Constants, no menue items, max 10 constants
         define MATERIALS_SETUP_MENU_ITEM  //enable-disable the menu item for material modification 
         define REDUCED_MATERIAL_SETUP_MENU //if MATERIALS_SETUP_MENU_ITEM is enabled, reduce the Setup-Menu to the first material only
		 
		 define PREHEAT_X_NAME  // X 1..10 , NAME is for Display istead of fixed "PLA" or "ABS"
		 
		 define PREHEAT_1_NAME "User"  //
         define PREHEAT_1_TEMP_HOTEND 205
         define PREHEAT_1_TEMP_BED     60
         define PREHEAT_1_FAN_SPEED     200 // Value from 0 to 255
                     ... 
         define PREHEAT_10_NAME "TPU"
         define PREHEAT_10_TEMP_HOTEND 235
         define PREHEAT_10_TEMP_BED    70
         define PREHEAT_10_FAN_SPEED     128 // Value from 0 to 255
		 
 modification for led
     modification: Configuration_adv.h
	  new
	        define LED_DEFAULT_COLOR // instead of define each single led color 
 
 modifications for neopixel_case_light
      modification: Configuration_adv.h
	  new
            define CASE_LIGHT_COLOR_CONTROL_MENU     // Add a Case Light Color control option to the LCD Case Light menu if Neopixel-Caselight
            define CASE_LIGHT_COLOR_PRESETS          // Add Presets for color control option
                   
            define CASE_LIGHT_USE_NEOPIXEL_SPLIT  //splitted Pixelbus, first Pixels NEOPIXEL_LED and extra Neopixels for Caselight in same Pixelbus behind Neopixel-LED
            
			define CASE_LIGHT_USE_NEOPIXEL_EXCLUSIVE //second Pixelbus, exclusive for CASE_LIGHT

            define NEOPIXEL_CASE_LIGHT_PIXELS 22  //pixels in splitted Neopixel bus behind Neopixels for Led or in exclusive Pixelbus
            
			define NEOPIXEL_CASE_LIGHT_PIN    4        // Pin for exclusive Caselight-Neopixels pixelbus
     modification in "M355"
	   use same options like "M150" to setup case_light_color
	   
	   
 modification in CUSTOM_USER_MENUS
      modification: Configuration_adv.h
	  new
	        check for allowed options
			more user menues
            new manual leveling  // go to home-position and disable steppers
			new Z-Up // set position as z-reference an drive z 10mm, perfect for fast lifting extruder 