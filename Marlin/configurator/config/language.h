#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "Configuration.h"

#define LANGUAGE_CONCAT(M)       #M
#define GENERATE_LANGUAGE_INCLUDE(M)  LANGUAGE_CONCAT(language_##M.h)


// NOTE: IF YOU CHANGE LANGUAGE FILES OR MERGE A FILE WITH CHANGES
//
//   ==> ALWAYS TRY TO COMPILE MARLIN WITH/WITHOUT "ULTIPANEL" / "ULTRALCD" / "SDSUPPORT" #define IN "Configuration.h"
//   ==> ALSO TRY ALL AVAILABLE LANGUAGE OPTIONS

// Languages
// en    English
// pl    Polish
// fr    French
// de    German
// es    Spanish
// ru    Russian
// it    Italian
// pt    Portuguese
// pt-br Portuguese (Brazil)
// fi    Finnish
// an    Aragonese
// nl    Dutch
// ca    Catalan
// eu    Basque-Euskera

#ifndef LANGUAGE_INCLUDE
  // pick your language from the list above
  #define LANGUAGE_INCLUDE GENERATE_LANGUAGE_INCLUDE(en)
#endif

#define PROTOCOL_VERSION "1.0"

#if MB(ULTIMAKER)|| MB(ULTIMAKER_OLD)|| MB(ULTIMAIN_2)
  #define MACHINE_NAME "Ultimaker"
  #define FIRMWARE_URL "http://firmware.ultimaker.com"
#elif MB(RUMBA)
  #define MACHINE_NAME "Rumba"
#elif MB(3DRAG)
  #define MACHINE_NAME "3Drag"
  #define FIRMWARE_URL "http://3dprint.elettronicain.it/"
#elif MB(K8200)
  #define MACHINE_NAME "K8200"
#elif MB(5DPRINT)
  #define MACHINE_NAME "Makibox"
#elif MB(SAV_MKI)
  #define MACHINE_NAME "SAV MkI"
  #define FIRMWARE_URL "https://github.com/fmalpartida/Marlin/tree/SAV-MkI-config"
#elif MB(WITBOX)
  #define MACHINE_NAME "WITBOX"
  #define FIRMWARE_URL "http://www.bq.com/gb/downloads-witbox.html"
#elif MB(HEPHESTOS)
  #define MACHINE_NAME "HEPHESTOS"
  #define FIRMWARE_URL "http://www.bq.com/gb/downloads-prusa-i3-hephestos.html"
#else // Default firmware set to Mendel
  #define MACHINE_NAME "Mendel"
  #define FIRMWARE_URL "https://github.com/MarlinFirmware/Marlin"
#endif

#ifdef CUSTOM_MENDEL_NAME
  #undef MACHINE_NAME
  #define MACHINE_NAME CUSTOM_MENDEL_NAME
#endif

#ifndef MACHINE_UUID
  #define MACHINE_UUID "00000000-0000-0000-0000-000000000000"
#endif


#define STRINGIFY_(n) #n
#define STRINGIFY(n) STRINGIFY_(n)


// Common LCD messages

  /* nothing here yet */

// Common serial messages
#define MSG_MARLIN "Marlin"

// Serial Console Messages (do not translate those!)

#define MSG_Enqueing                        "enqueing \""
#define MSG_POWERUP                         "PowerUp"
#define MSG_EXTERNAL_RESET                  " External Reset"
#define MSG_BROWNOUT_RESET                  " Brown out Reset"
#define MSG_WATCHDOG_RESET                  " Watchdog Reset"
#define MSG_SOFTWARE_RESET                  " Software Reset"
#define MSG_AUTHOR                          " | Author: "
#define MSG_CONFIGURATION_VER               " Last Updated: "
#define MSG_FREE_MEMORY                     " Free Memory: "
#define MSG_PLANNER_BUFFER_BYTES            "  PlannerBufferBytes: "
#define MSG_OK                              "ok"
#define MSG_FILE_SAVED                      "Done saving file."
#define MSG_ERR_LINE_NO                     "Line Number is not Last Line Number+1, Last Line: "
#define MSG_ERR_CHECKSUM_MISMATCH           "checksum mismatch, Last Line: "
#define MSG_ERR_NO_CHECKSUM                 "No Checksum with line number, Last Line: "
#define MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM "No Line Number with checksum, Last Line: "
#define MSG_FILE_PRINTED                    "Done printing file"
#define MSG_BEGIN_FILE_LIST                 "Begin file list"
#define MSG_END_FILE_LIST                   "End file list"
#define MSG_M104_INVALID_EXTRUDER           "M104 Invalid extruder "
#define MSG_M105_INVALID_EXTRUDER           "M105 Invalid extruder "
#define MSG_M200_INVALID_EXTRUDER           "M200 Invalid extruder "
#define MSG_M218_INVALID_EXTRUDER           "M218 Invalid extruder "
#define MSG_M221_INVALID_EXTRUDER           "M221 Invalid extruder "
#define MSG_ERR_NO_THERMISTORS              "No thermistors - no temperature"
#define MSG_M109_INVALID_EXTRUDER           "M109 Invalid extruder "
#define MSG_HEATING                         "Heating..."
#define MSG_HEATING_COMPLETE                "Heating done."
#define MSG_BED_HEATING                     "Bed Heating."
#define MSG_BED_DONE                        "Bed done."
#define MSG_M115_REPORT                     "FIRMWARE_NAME:Marlin V1; Sprinter/grbl mashup for gen6 FIRMWARE_URL:" FIRMWARE_URL " PROTOCOL_VERSION:" PROTOCOL_VERSION " MACHINE_TYPE:" MACHINE_NAME " EXTRUDER_COUNT:" STRINGIFY(EXTRUDERS) " UUID:" MACHINE_UUID "\n"
#define MSG_COUNT_X                         " Count X: "
#define MSG_ERR_KILLED                      "Printer halted. kill() called!"
#define MSG_ERR_STOPPED                     "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define MSG_RESEND                          "Resend: "
#define MSG_UNKNOWN_COMMAND                 "Unknown command: \""
#define MSG_ACTIVE_EXTRUDER                 "Active Extruder: "
#define MSG_INVALID_EXTRUDER                "Invalid extruder"
#define MSG_X_MIN                           "x_min: "
#define MSG_X_MAX                           "x_max: "
#define MSG_Y_MIN                           "y_min: "
#define MSG_Y_MAX                           "y_max: "
#define MSG_Z_MIN                           "z_min: "
#define MSG_Z_MAX                           "z_max: "
#define MSG_M119_REPORT                     "Reporting endstop status"
#define MSG_ENDSTOP_HIT                     "TRIGGERED"
#define MSG_ENDSTOP_OPEN                    "open"
#define MSG_HOTEND_OFFSET                   "Hotend offsets:"

#define MSG_SD_CANT_OPEN_SUBDIR             "Cannot open subdir"
#define MSG_SD_INIT_FAIL                    "SD init fail"
#define MSG_SD_VOL_INIT_FAIL                "volume.init failed"
#define MSG_SD_OPENROOT_FAIL                "openRoot failed"
#define MSG_SD_CARD_OK                      "SD card ok"
#define MSG_SD_WORKDIR_FAIL                 "workDir open failed"
#define MSG_SD_OPEN_FILE_FAIL               "open failed, File: "
#define MSG_SD_FILE_OPENED                  "File opened: "
#define MSG_SD_SIZE                         " Size: "
#define MSG_SD_FILE_SELECTED                "File selected"
#define MSG_SD_WRITE_TO_FILE                "Writing to file: "
#define MSG_SD_PRINTING_BYTE                "SD printing byte "
#define MSG_SD_NOT_PRINTING                 "Not SD printing"
#define MSG_SD_ERR_WRITE_TO_FILE            "error writing to file"
#define MSG_SD_CANT_ENTER_SUBDIR            "Cannot enter subdir: "

#define MSG_STEPPER_TOO_HIGH                "Steprate too high: "
#define MSG_ENDSTOPS_HIT                    "endstops hit: "
#define MSG_ERR_COLD_EXTRUDE_STOP           " cold extrusion prevented"
#define MSG_ERR_LONG_EXTRUDE_STOP           " too long extrusion prevented"
#define MSG_BABYSTEPPING_X                  "Babystepping X"
#define MSG_BABYSTEPPING_Y                  "Babystepping Y"
#define MSG_BABYSTEPPING_Z                  "Babystepping Z"
#define MSG_SERIAL_ERROR_MENU_STRUCTURE     "Error in menu structure"

#define MSG_ERR_EEPROM_WRITE                "Error writing to EEPROM!"

// LCD Menu Messages

// Add your own character. Reference: https://github.com/MarlinFirmware/Marlin/pull/1434 photos
//                                and https://www.sparkfun.com/datasheets/LCD/HD44780.pdf page 17-18
#ifdef DOGLCD
  #define STR_Ae "\304"               // 'Ä' U8glib
  #define STR_ae "\344"               // 'ä'
  #define STR_Oe "\326"               // 'Ö'
  #define STR_oe STR_Oe               // 'ö'
  #define STR_Ue "\334"               // 'Ü'
  #define STR_ue STR_Ue               // 'ü'
  #define STR_sz "\337"               // 'ß'
  #define STR_h2 "\262"               // '²'
  #define STR_h3 "\263"               // '³'
  #define STR_Deg "\260"              // '°'
  #define STR_THERMOMETER "\377"
#else
  #ifdef DISPLAY_CHARSET_HD44780_JAPAN // HD44780 ROM Code: A00 (Japan)
    #define STR_ae "\xe1"
    #define STR_Ae STR_ae
    #define STR_oe "\357"
    #define STR_Oe STR_oe
    #define STR_ue "\365"
    #define STR_Ue STR_ue
    #define STR_sz "\342"
    #define STR_h2 "2"
    #define STR_h3 "3"
    #define STR_Deg "\271"
    #define STR_THERMOMETER "\002"
  #endif
  #ifdef DISPLAY_CHARSET_HD44780_WESTERN // HD44780 ROM Code: A02 (Western)
    #define STR_Ae "\216"
    #define STR_ae "\204"
    #define STR_Oe "\211"
    #define STR_oe "\204"
    #define STR_Ue "\212"
    #define STR_ue "\201"
    #define STR_sz "\160"
    #define STR_h2 "\262"
    #define STR_h3 "\263"
    #define STR_Deg "\337"
    #define STR_THERMOMETER "\002"
  #endif
#endif
/*
#define TESTSTRING000 "\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
#define TESTSTRING020 "\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
#define TESTSTRING040 "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
#define TESTSTRING060 "\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
#define TESTSTRING100 "\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
#define TESTSTRING120 "\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
#define TESTSTRING140 "\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
#define TESTSTRING160 "\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
#define TESTSTRING200 "\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
#define TESTSTRING220 "\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
#define TESTSTRING240 "\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
#define TESTSTRING260 "\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
#define TESTSTRING300 "\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
#define TESTSTRING320 "\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
#define TESTSTRING340 "\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
#define TESTSTRING360 "\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377"
*/

#include LANGUAGE_INCLUDE
#include "language_en.h"

#endif //__LANGUAGE_H
