/**
 * German
 *
 * LCD Menu Messages
 * Please note these are limited to 17 characters!
 *
 */
#ifndef LANGUAGE_DE_H
#define LANGUAGE_DE_H

// uncomment your style
//#define UMLAUTE_DE 1 //                     ü > ü
//#define UMLAUTE_DE 2 // for HITACHI_HD44780 ü > \365
//#define UMLAUTE_DE 3 // for Langform        ü > ue
//#define UMLAUTE_DE 4 // for my (your) way

//please test next. REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER defines  REPRAP_DISCOUNT_SMART_CONTROLLER to.

#ifndef UMLAUTE_DE
  #if defined (REPRAP_DISCOUNT_SMART_CONTROLLER) && !( defined ( REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER ) )
    #define UMLAUTE_DE 2
  #else
	#define UMLAUTE_DE 3
  #endif
#endif

#if UMLAUTE_DE <= 1
  #define MSG_MAIN                            "Hauptmenü"
  #define MSG_PREHEAT_PLA                     "Vorwärmen PLA"
  #define MSG_PREHEAT_PLA_N                   "Vorwärmen PLA "
  #define MSG_PREHEAT_PLA_SETTINGS            "Vorwärm. PLA Ein."
  #define MSG_PREHEAT_ABS                     "Vorwärmen ABS"
  #define MSG_PREHEAT_ABS_N                   "Vorwärmen ABS "
  #define MSG_PREHEAT_ABS_SETTINGS            "Vorwärm. ABS Ein."
  #define MSG_COOLDOWN                        "Abkühlen"
  #define MSG_NOZZLE                          "Düse"
  #define MSG_FAN_SPEED                       "Lüftergeschw."
  #define MSG_CARD_MENU                       "SDKarten Menü"
#elif UMLAUTE_DE <= 2
  // Romcode A00; ä > \341 \xe1, ö > \357 \xef, ü > \365 \xf5, ß > \342 \xe2, ° > \337 \xdf               // octal hex
  // Romcode A02; ä > \344 \xe4, ö > \366 \xe6, ü > \374 \xfc, ß > \337 \xdf, ° > \260 \xb0, Ä > \304 \xc4, Ö > \326 \xd6, Ü > \334 \xdc
  #define MSG_MAIN                            "Hauptmen\365"
  #define MSG_PREHEAT_PLA                     "Vorw\341rmen PLA"
  #define MSG_PREHEAT_PLA_N                   "Vorw\341rmen PLA "
  #define MSG_PREHEAT_PLA_SETTINGS            "Vorw\341rm. PLA Ein."
  #define MSG_PREHEAT_ABS                     "Vorw\341rmen ABS"
  #define MSG_PREHEAT_ABS_N                   "Vorw\341rmen ABS "
  #define MSG_PREHEAT_ABS_SETTINGS            "Vorw\341rm. ABS Ein."
  #define MSG_COOLDOWN                        "Abk\365hlen"
  #define MSG_NOZZLE                          "D\365se"
  #define MSG_FAN_SPEED                       "L\365ftergeschw."
  #define MSG_CARD_MENU                       "SDKarten Men\365"
#elif UMLAUTE_DE <= 3 
  #define MSG_MAIN                            "Hauptmenue"
  #define MSG_PREHEAT_PLA                     "Vorwaermen PLA"
  #define MSG_PREHEAT_PLA_N                   "Vorwaermen PLA "
  #define MSG_PREHEAT_PLA_SETTINGS            "Vorwaer. PLA Ein."
  #define MSG_PREHEAT_ABS                     "Vorwaermen ABS"
  #define MSG_PREHEAT_ABS_N                   "Vorwaermen ABS "
  #define MSG_PREHEAT_ABS_SETTINGS            "Vorwaer. ABS Ein."
  #define MSG_COOLDOWN                        "Abkuehlen"
  #define MSG_NOZZLE                          "Duese"
  #define MSG_FAN_SPEED                       "Lueftergeschw."
  #define MSG_CARD_MENU                       "SDKarten Menue"
#elif UMLAUTE_DE <= 4 
  #define MSG_MAIN                            "Hauptauswahl.."
  #define MSG_PREHEAT_PLA                     "Vorheizen PLA.."  
  #define MSG_PREHEAT_PLA_N                   "Vorheizen PLA "   
  #define MSG_PREHEAT_PLA_SETTINGS            "Vorh. PLA E.."    
  #define MSG_PREHEAT_ABS                     "Vorheizen ABS.."  
  #define MSG_PREHEAT_ABS_N                   "Vorheizen ABS "   
  #define MSG_PREHEAT_ABS_SETTINGS            "Vorh. ABS E.."    
  #define MSG_COOLDOWN                        "Heizungen Aus"    
  #define MSG_NOZZLE                          "Duese"           
  #define MSG_FAN_SPEED                       "Luftmenge"       
  #define MSG_CARD_MENU                       "SDKarte.."      
#endif

// messages on infoscreen
#define WELCOME_MSG                           MACHINE_NAME " Bereit."
#define MSG_SD_INSERTED                       "SDKarte erkannt."
#define MSG_SD_REMOVED                        "SDKarte entfernt."
#define MSG_DWELL                             "Warten..."
#define MSG_USERWAIT                          "Warte auf Nutzer"
#define MSG_RESUMING                          "Druck fortsetzung"
#define MSG_PRINT_ABORTED                     "Print aborted"
#define MSG_NO_MOVE                           "Kein Zug."
#define MSG_KILLED                            "KILLED"
#define MSG_STOPPED                           "GESTOPPT"
#define MSG_ZPROBE_OUT                        "Z probe out. bed"
#define MSG_POSITION_UNKNOWN                  "Home X/Y before Z"

// menu
//#define MSG_MAIN                            "Hauptmenü"
  #define MSG_WATCH                           "Beobachten"
  #define MSG_TUNE                            "Justierung"
    #define MSG_SPEED                         "Geschw"
  //#define MSG_NOZZLE                        "Duese"           
    #define MSG_BED                           "Bett"
  //#define MSG_FAN_SPEED                     "Luftmenge"       
    #define MSG_FLOW                          "Fluss"
    #define MSG_BABYSTEP_X                    "Babystep X"
    #define MSG_BABYSTEP_Y                    "Babystep Y"
    #define MSG_BABYSTEP_Z                    "Babystep Z"
    #define MSG_FILAMENTCHANGE                "Filament wechseln"
    
  #define MSG_PREPARE                         "Vorbereitung"
    #define MSG_AUTOSTART                     "Autostart"
    #define MSG_DISABLE_STEPPERS              "Stepper abschalt."// M84
    #define MSG_AUTO_HOME                     "Auto Nullpunkt" //G28
    #define MSG_SET_HOME_OFFSETS              "Set home offsets"
    #define MSG_SET_ORIGIN                    "Setze Nullpunkt" //"G92 X0 Y0 Z0"
  //#define MSG_PREHEAT_PLA                   "Vorheizen PLA.."   
  //  #define MSG_PREHEAT_PLA_N               "Vorheizen PLA "   // menu item
      #define MSG_PREHEAT_PLA_ALL             "Vorw. PLA Alle"
      #define MSG_PREHEAT_PLA_BEDONLY         "Vorw. PLA Bett"
  //#define MSG_PREHEAT_ABS                   "Vorheizen ABS.."    
  //  #define MSG_PREHEAT_ABS_N               "Vorheizen ABS "   // menu item
      #define MSG_PREHEAT_ABS_ALL             "Vorw. ABS Alle"
      #define MSG_PREHEAT_ABS_BEDONLY         "Vorw. ABS Bett"
    #define MSG_COOLDOWN                      "Heizungen Aus"     
    #define MSG_SWITCH_PS_ON                  "Switch Power On"
    #define MSG_SWITCH_PS_OFF                 "Switch Power Off"
    #define MSG_MOVE_AXIS                     "Achsen bewegen"
      #define MSG_MOVE_10MM                   "10mm bewegen"
        #define MSG_MOVE_X                    "X bewegen"
        #define MSG_MOVE_Y                    "Y bewegen"
      #define MSG_MOVE_1MM                    "1mm bewegen"
  //    #define MSG_MOVE_X                    "X bewegen"
  //    #define MSG_MOVE_Y                    "Y bewegen"
        #define MSG_MOVE_Z                    "Z bewegen"
        #define MSG_MOVE_E                    "Extruder"
     #define MSG_MOVE_01MM                    "0.1mm bewegen"
  //    #define MSG_MOVE_X                    "X bewegen"
  //    #define MSG_MOVE_Y                    "Y bewegen"
  //    #define MSG_MOVE_Z                    "Z bewegen"
  //    #define MSG_MOVE_E                    "Extruder"

  //#define MSG_DELTA_CALIBRATE               "Delta Calibration" 
  //  ..

  #define MSG_CONTROL                         "Einstellungen"
    #define MSG_TEMPERATURE                   "Temperatur"
    //#define MSG_NOZZLE                      "Duese"            // menu item
    //#define MSG_BED                         "Bett"
    //#define MSG_FAN_SPEED                   "Luftmenge"        // menu item
      #define MSG_AUTOTEMP                    "AutoTemp"
      #define MSG_MIN                         "\002 Min"
      #define MSG_MAX                         "\002 Max"
      #define MSG_FACTOR                      "\002 Faktor"
      #define MSG_PID_P                       "PID-P"
      #define MSG_PID_I                       "PID-I"
      #define MSG_PID_D                       "PID-D"
      #define MSG_PID_C                       "PID-C"

    #define MSG_MOTION                        "Bewegung"
      #define MSG_ZPROBE_ZOFFSET              "Z Offset"
      #define MSG_ACC                         "Acc"
      #define MSG_VXY_JERK                    "Vxy-jerk"
      #define MSG_VZ_JERK                     "Vz-jerk"
      #define MSG_VE_JERK                     "Ve-jerk"
      #define MSG_VMAX                        "Vmax "
      #define MSG_X                           "x"
      #define MSG_Y                           "y"
      #define MSG_Z                           "z"
      #define MSG_E                           "e"
      #define MSG_VMIN                        "Vmin"
      #define MSG_VTRAV_MIN                   "VTrav min"
      #define MSG_AMAX                        "Amax "
      #define MSG_A_RETRACT                   "A-Retract"
      #define MSG_XSTEPS                      "Xsteps/mm"
      #define MSG_YSTEPS                      "Ysteps/mm"
      #define MSG_ZSTEPS                      "Zsteps/mm"
      #define MSG_ESTEPS                      "Esteps/mm"
      #define MSG_ENDSTOP_ABORT               "Endstop abort"
      // SCARA here
    #define MSG_VOLUMETRIC                    "Filament"
      #define MSG_VOLUMETRIC_ENABLED		      "E in mm3"
      #define MSG_FILAMENT_SIZE_EXTRUDER_0    "Fil. Dia. 1"
      #define MSG_FILAMENT_SIZE_EXTRUDER_1    "Fil. Dia. 2"
      #define MSG_FILAMENT_SIZE_EXTRUDER_2    "Fil. Dia. 3"
    #define MSG_CONTRAST                      "LCD contrast"
    #define MSG_RETRACT                       "Retract"
      #define MSG_AUTORETRACT                 "AutoRetr."
      #define MSG_CONTROL_RETRACT             "Retract mm"
      #define MSG_CONTROL_RETRACT_SWAP        "Wechs. Retract mm"
      #define MSG_CONTROL_RETRACTF            "Retract  V"
      #define MSG_CONTROL_RETRACT_ZLIFT       "Hop mm"
      #define MSG_CONTROL_RETRACT_RECOVER     "UnRet +mm"
      #define MSG_CONTROL_RETRACT_RECOVER_SWAP "Wechs. UnRet +mm"
      #define MSG_CONTROL_RETRACT_RECOVERF    "UnRet  V"
    #define MSG_STORE_EPROM                   "EPROM speichern"
    #define MSG_LOAD_EPROM                    "EPROM laden"
    #define MSG_RESTORE_FAILSAFE              "Standardkonfig."

  #define MSG_NO_CARD                         "Keine SDKarte"
  #define MSG_PAUSE_PRINT                     "Druck anhalten"
  #define MSG_RESUME_PRINT                    "Druck fortsetz"
  #define MSG_STOP_PRINT                      "Druck stoppen"
//#define MSG_CARD_MENU                       "SDKarte.."       
    #define MSG_REFRESH                       "Aktualisieren"
  #define MSG_INIT_SDCARD                     "Init. SD-Card"// Manually initialize the SD-card via user interface
  #define MSG_CNG_SDCARD                      "Change SD-Card"// SD-card changed by user

// unused
#define MSG_EXTRUDE                           "Extrude"
// callback
#define MSG_ON                                "Ein"
#define MSG_OFF                               "Aus"

// don't know

#ifdef DELTA_CALIBRATION_MENU
    #define MSG_DELTA_CALIBRATE               "Delta Calibration"
//    #define MSG_AUTO_HOME                   "Auto Nullpunkt"
      #define MSG_DELTA_CALIBRATE_X           "Calibrate X"
      #define MSG_DELTA_CALIBRATE_Y           "Calibrate Y"
      #define MSG_DELTA_CALIBRATE_Z           "Calibrate Z"
      #define MSG_DELTA_CALIBRATE_CENTER      "Calibrate Center"
#endif // DELTA_CALIBRATION_MENU

//#ifdef SCARA
// MSG_XSCALE used in ultralcd.cpp but defined nowhere
// MSG_YSCALE used in ultralcd.cpp but defined nowhere
//#define MSG_XSCALE                          "X-Faktor"
//#define MSG_YSCALE                          "Y-Faktor"
//#endif

#endif // LANGUAGE_DE_H
