/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * HJC2560-C Rev2.x pin assignments
 */

/**
 * Rev 2.x   2 Aug 2018
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

// BOARD Info
#define DEFAULT_MACHINE_NAME    "HCMaker7"
#define BOARD_NAME              "HJC2560-C"
#define BOARD_REV               "Rev: 2.1"        //主板版本：Rev 2.1, Rs=0.05R
#define R_SENSOR                "R-sensor: R050"             //检测电阻

// FIRMWARE Info
#define FIRMWARE_NAME           "Marlin 1-1-9"           
#define FIRMWARE_REV            "Rev: 1.4"        //固件版本：Rev 1.4


//
// Limit Switches
//
#define X_STOP_PIN         22
#define Y_STOP_PIN         26
#define Z_STOP_PIN         29
#define EXP_STOP_PIN       28

//
// Steppers
//
#define X_STEP_PIN         25
#define X_DIR_PIN          23
#define X_ENABLE_PIN       27

#define Y_STEP_PIN         32
#define Y_DIR_PIN          33
#define Y_ENABLE_PIN       31

#define Z_STEP_PIN         35
#define Z_DIR_PIN          36
#define Z_ENABLE_PIN       34

#define E0_STEP_PIN        42
#define E0_DIR_PIN         43
#define E0_ENABLE_PIN      37

#define E1_STEP_PIN        49
#define E1_DIR_PIN         47
#define E1_ENABLE_PIN      48

#define MOTOR_CURRENT_PWM_XY_PIN   44
#define MOTOR_CURRENT_PWM_Z_PIN    45
#define MOTOR_CURRENT_PWM_E_PIN    46
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE  2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}

//
// Temperature Sensors
//
#define TEMP_0_PIN          8   // Analog Input
#define TEMP_1_PIN          9   // Analog Input
#define TEMP_BED_PIN       10   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN        2
#define HEATER_1_PIN        3
#define HEATER_BED_PIN      4

//#ifndef FAN_PIN
  #define FAN_PIN           7  //默认不使用PWM_FAN冷却喷嘴，如果需要，则取消注释
//#endif

//
// Misc. Functions
//
#define SDSS               53
#define SD_DETECT_PIN      39
//#define LED_PIN           8           
#define CASE_LIGHT_PIN      8           //8 默认挤出机风扇作为Case LED，如果需要PWM FAN,则需要将FAN_PIN置为7，LED_PIN置为8
//#define SAFETY_TRIGGERED_PIN     28   // PIN to detect the safety circuit has triggered
//#define MAIN_VOLTAGE_MEASURE_PIN 14   // ANALOG PIN to measure the main voltage, with a 100k - 4k7 resitor divider.

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)

  #define BEEPER_PIN 18

  #if ENABLED(NEWPANEL)

  	#if ENABLED(HJC_LCD_SMART_CONTROLLER)
   		 #define LCD_PINS_RS    		20		//LCD_CS
    	 #define LCD_PINS_ENABLE 		15		//LCD_SDA
    	 #define LCD_PINS_D4    		14		//LCD_SCK
		   #define LCD_BACKLIGHT_PIN 	    5    	//LCD_Backlight
//         #ifndef LCD_CONTRAST_PIN
//         	#define LCD_CONTRAST_PIN    5    //LCD_Contrast
//         #endif
		    // define digital pin 24 for the filament runout sensor. 
		  #ifndef FIL_RUNOUT_PIN
		 	  #define FIL_RUNOUT_PIN   	24		//Filament runout 
      #endif
	  #else
    	#define LCD_PINS_RS    			20		//LCD_CS
   		#define LCD_PINS_ENABLE 		15		//LCD_SDA
   		#define LCD_PINS_D4    			14		//LCD_SCK
      #define LCD_PINS_D5    			21		
   		#define LCD_PINS_D6     		5
   		#define LCD_PINS_D7     		6
    #endif

   	// buttons are directly attached
   	#define BTN_EN1 41
   	#define BTN_EN2 40
   	#define BTN_ENC 19

    #define SD_DETECT_PIN 		39

  #else // !NEWPANEL - Old style panel with shift register

    // buttons are attached to a shift register
    #define SHIFT_CLK 		  	38
    #define SHIFT_LD 		    	42
    #define SHIFT_OUT 		  	40
    #define SHIFT_EN 		    	17

    #define LCD_PINS_RS 		  16
    #define LCD_PINS_ENABLE 	5
    #define LCD_PINS_D4 	  	6
    #define LCD_PINS_D5 	  	21
    #define LCD_PINS_D6 	  	20
    #define LCD_PINS_D7 	  	19

    #define SD_DETECT_PIN      -1

  #endif // !NEWPANEL

#endif // ULTRA_LCD


//
// M3/M4/M5 - Spindle/Laser Control，默认，主轴/激光器使用独立的PWM引脚(D9)控制
// 如果使用了PWM_FAN,Case_Light和Spindle/Laser,  并希望Spindle/Laser使用CASE_Light的线，则将
//SPINDLE_LASER_PWM_PIN 定义为8
//
#if ENABLED(SPINDLE_LASER_ENABLE)   // use the LED_PIN for spindle speed control or case light
//  #undef LED_PIN
  #define SPINDLE_DIR_PIN           16
  #define SPINDLE_LASER_ENABLE_PIN  17   // Pin should have a pullup!
  #define SPINDLE_LASER_PWM_PIN     9   // MUST BE HARDWARE PWM，
//#else
//  #undef LED_PIN
//  #define CASE_LIGHT_PIN            7
#endif

//
// Servos
//
//#ifndef SERVO0_PIN
//  #define SERVO0_PIN       11
//#endif
