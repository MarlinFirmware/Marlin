/**
* How to use this firmware
* Uncomment (this means removing the 2 // in front of #define a printer model.
* 
* If you have EZABL uncomment #define EZABL_ENABLE and then uncommend the mount
* you are using with the printer. 
* 
* If you have a custom/unsupported mount uncomment #define CUSTOM_MOUNT
* and enter your offsets below in the CUSTOM MOUNT section.
* 
* There are other features in the TH3D Extras section so look there for V6 Hotend,
* Bootscreen settings, Titan Extruder and more. You only need to edit this file.
* 
* For the Tornado, TAZ5, and CR-10S select Tools > Board > Arduino Mega 2560
* For the Ender 2 and CR-10 select Tools > Board > Sanguino(1284P Boards)
* 
* Then select the COM port your printer is on from the Tools menu.
* 
* Once you have your settings click the arrow in the upper left to upload to the board.
* 
* Thats it! 
* 
* !!!!!!!CAUTION!!!!!!!
* Be sure to either send M502 then M500 to reset your EEPROM or on the printer
* LCD go to Control > Initialize EEPROM to clear out the EEPROM to defaults.
* 
* ONLY UNCOMMENT ONE PRINTER TYPE. IF YOU UNCOMMENT MORE THAN ONE YOU WILL GET ERRORS.
*/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#define CONFIGURATION_H_VERSION 010107

//===========================================================================
//============================ TH3D Configuration ===========================
//===========================================================================

//===========================================================================
// Creality CR-10 Options
//===========================================================================
//#define CR10
//#define CR10_MINI
//#define CR10_S4
//#define CR10_S5

//#define EZOUT_ENABLE

// EZABL Settings - Uncomment #define EZABL_ENABLE and uncomment your mount 
// type you are using to enable EZABL Bed Leveing features
//#define EZABL_ENABLE

// Probe Mounts
//#define CR10_VOLCANO
//#define CR10_V6HEAVYDUTY
//#define CR10_OEM
//#define CR10_FANG
//#define CUSTOM_PROBE

//===========================================================================
// Creality CR-10S Options
//===========================================================================
//#define CR10S
//#define CR10S_MINI
//#define CR10S_S4
//#define CR10S_S5

// If you are having issues with your CR-10S filament sensor uncomment 
// the below line to disable it in the firmware
//#define CR10S_NOFILAMENTSENSOR

// EZABL Settings - Uncomment #define EZABL_ENABLE and uncomment your mount 
// type you are using to enable EZABL Bed Leveing features
//#define EZABL_ENABLE

// Probe Mounts
//#define CR10_VOLCANO
//#define CR10_V6HEAVYDUTY
//#define CR10_OEM
//#define CR10_FANG
//#define CUSTOM_PROBE

//===========================================================================
// Creality Ender 2 Options
//===========================================================================
//#define ENDER2

// EZABL Settings - Uncomment #define EZABL_ENABLE and uncomment your mount 
// type you are using to enable EZABL Bed Leveing features
//#define EZABL_ENABLE

// Probe Mounts
//#define ENDER2_OEM
//#define ENDER2_V6
//#define CUSTOM_PROBE

//===========================================================================
// TEVO Tornado Options
//===========================================================================
//#define TORNADO

// EZABL Settings - Uncomment #define EZABL_ENABLE and uncomment your mount 
// type you are using to enable EZABL Bed Leveing features
//#define EZABL_ENABLE

// Probe Mounts
//#define TORNADO_OEM
//#define CUSTOM_PROBE

// Use Tornado Bootscreen instead of TH3D
//#define TORNADO_BOOT

//===========================================================================
// Lulzbot TAZ5 Options
//===========================================================================
//#define TAZ5

// EZABL Settings - Uncomment #define EZABL_ENABLE and uncomment your mount 
// type you are using to enable EZABL Bed Leveing features
//#define EZABL_ENABLE

// Probe Mounts
//#define TAZ5_OEM
//#define CUSTOM_PROBE

//===========================================================================
// TH3D EXTRAS
//===========================================================================

// If you want more or less EZABL probe points change the number below
// Default is 4 which gives you 4x4 grid. Do not go over 10 here.
// Ender 2 will be best with a 3x3 grid, change to a 3 for Ender 2
#define EZABL_POINTS 4

// If you are using a V6 Hotend with the V6 Thermistor 
// uncomment the below line  to set the correct thermistor settings
//#define V6_HOTEND

// If you are using an AC bed with a standalone controller (like the Keenovo heaters) 
// uncomment the below line to disable the heated bed control in the firmware
//#define AC_BED

// If you are using an E3D or TH3D Titan Extruder uncomment the below line 
// to setup the firmware to the correct steps and direction
//#define TITAN_EXTRUDER
// If your titan uses steps/mm other than 463 change it below, this works for most Titans
#define TITAN_EXTRUDER_STEPS 463

// Use TinyMachines Bootscreen instead of TH3D
//#define TM3D_BOOT

// Use your own printer name
//#define USER_PRINTER_NAME "Change Me" 

//===========================================================================
// IF YOU HAVE A CUSTOM PROBE MOUNT OR ONE THAT IS NOT PRE-SUPPORTED
// UNCOMMENT THE CUSTOM_PROBE OPTION AND ENTER YOUR PROBE LOCATION BELOW
//===========================================================================
#if ENABLED(CUSTOM_PROBE)
  /**
  *   Z Probe to nozzle (X,Y) offset, relative to (0, 0).
  *   X and Y offsets must be integers.
  *
  *   In the following example the X and Y offsets are both positive:
  *   #define X_PROBE_OFFSET_FROM_EXTRUDER 10
  *   #define Y_PROBE_OFFSET_FROM_EXTRUDER 10
  *
  *      +-- BACK ---+
  *      |           |
  *    L |    (+) P  | R <-- probe (20,20)
  *    E |           | I
  *    F | (-) N (+) | G <-- nozzle (10,10)
  *    T |           | H
  *      |    (-)    | T
  *      |           |
  *      O-- FRONT --+
  *    (0,0)
  */
  #define X_PROBE_OFFSET_FROM_EXTRUDER 10  // X offset: -left  +right  [of the nozzle]
  #define Y_PROBE_OFFSET_FROM_EXTRUDER 10  // Y offset: -front +behind [the nozzle]1
#endif

/**
 * LCD LANGUAGE
 *
 * Select the language to display on the LCD. These languages are available:
 *
 *    en, an, bg, ca, cn, cz, cz_utf8, de, el, el-gr, es, eu, fi, fr, fr_utf8, gl,
 *    hr, it, kana, kana_utf8, nl, pl, pt, pt_utf8, pt-br, pt-br_utf8, ru, sk_utf8,
 *    tr, uk, zh_CN, zh_TW, test
 *
 * :{ 'en':'English', 'an':'Aragonese', 'bg':'Bulgarian', 'ca':'Catalan', 'cn':'Chinese', 
 * 'cz':'Czech', 'cz_utf8':'Czech (UTF8)', 'de':'German', 'el':'Greek', 'el-gr':'Greek (Greece)', 
 * 'es':'Spanish', 'eu':'Basque-Euskera', 'fi':'Finnish', 'fr':'French', 'fr_utf8':'French (UTF8)', 
 * 'gl':'Galician', 'hr':'Croatian', 'it':'Italian', 'kana':'Japanese', 'kana_utf8':'Japanese (UTF8)', 
 * 'nl':'Dutch', 'pl':'Polish', 'pt':'Portuguese', 'pt-br':'Portuguese (Brazilian)', 
 * 'pt-br_utf8':'Portuguese (Brazilian UTF8)', 'pt_utf8':'Portuguese (UTF8)', 'ru':'Russian', 
 * 'sk_utf8':'Slovak (UTF8)', 'tr':'Turkish', 'uk':'Ukrainian', 'zh_CN':'Chinese (Simplified)', 
 * 'zh_TW':'Chinese (Taiwan)', test':'TEST' }
 */
#define LCD_LANGUAGE en

#include "Configuration_backend.h"

#endif // CONFIGURATION_H
