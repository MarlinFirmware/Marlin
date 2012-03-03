#ifndef __CONFIGURATION_IO_H
#define __CONFIGURATION_IO_H

// LCD, button, SD and Camera support

// Display Types
//#define ULTRA_LCD  //general lcd support, also 16x2 - ONLY SUPPORTED BY MOTHERBOARD 7
//#define SER_LCD //serLCD library lcd support - supported by Reprap's (Motherboard 3 and 33)
#define KS0108
//#define COLOR_LCD_SHIELD //Arduino shield rewired to be used - has three buttons on board 

//To add support for more Motherboards insert pin assignments in pin.h

// Button types
//#define TWO_BUTTON
//#define THREE_BUTTON
//#define ROTARY_ENCODER_BUTTON //Define steps per full rotation - expects a button press or a button on the side
//#define JOYSTICK // 4 way + button

//#define SDSUPPORT // Enable SD Card Support in Hardware Console

//#define PIEZO// Piezo attached?

// Macros for panels (Legacy support)
//#define ULTIPANEL
//#define NEWPANEL  //enable this if you have a click-encoder panel

#ifdef NEWPANEL
  #define ULTIPANEL
#endif

#ifdef ULTIPANEL
  #ifdef NEWPANEL
    #define ROTARY_ENCODER_BUTTON 20
  #endif 
  #define SDSUPPORT
  #define ULTRA_LCD
  #define LCD_WIDTH 20
  #define LCD_HEIGHT 4
  #define LCD_AVAILABLE 
  #define PIEZO
#endif

//no panel but just lcd 
#ifdef ULTRA_LCD
  #define LCD_WIDTH 16
  #define LCD_HEIGHT 2
  #define LCD_AVAILABLE 
#endif

#ifdef SER_LCD
  #define LCD_WIDTH 16
  #define LCD_HEIGHT 2
  #define LCD_AVAILABLE 
  #define SER_PIN 4
#endif

#ifdef KS0108
  #define LCD_WIDTH 20
  #define LCD_HEIGHT 8
  #define LCD_AVAILABLE 
#endif

#ifdef COLOR_LCD_SHIELD
  #define LCD_WIDTH 20
  #define LCD_HEIGHT 4
  #define LCD_AVAILABLE 
#endif

#ifdef SDSUPPORT
  #define SD_FINISHED_STEPPERRELEASE true  //if sd support and the file is finished: disable steppers?
  #define SD_FINISHED_RELEASECOMMAND "M84 X Y Z E" 
#endif

// The number of linear motions that can be in the plan at any give time.  
// THE BLOCK_BUFFER_SIZE NEEDS TO BE A POWER OF 2, i.g. 8,16,32 because shifts and ors are used to do the ringbuffering.
#if defined SDSUPPORT
  #define BLOCK_BUFFER_SIZE 16   // SD,LCD,Buttons take more memory, block buffer needs to be smaller
#else
  #define BLOCK_BUFFER_SIZE 16 // maximize block buffer
#endif

//Do not wait for M109 to finish when printing from SD card
//#define STOP_HEATING_WAIT_WHEN_SD_PRINTING

#ifdef LCD_AVAILABLE
  // Preheat Constants
  #define PLA_PREHEAT_HOTEND_TEMP 180 
  #define PLA_PREHEAT_HPB_TEMP 70
  #define PLA_PREHEAT_FAN_SPEED 255		// Insert Value between 0 and 255

  #define ABS_PREHEAT_HOTEND_TEMP 240
  #define ABS_PREHEAT_HPB_TEMP 100
  #define ABS_PREHEAT_FAN_SPEED 255		// Insert Value between 0 and 255
#endif
// M240  Triggers a camera by emulating a Canon RC-1 Remote
// Data from: http://www.doc-diy.net/photo/rc-1_hacked/
// #define PHOTOGRAPH_PIN     23

#endif
