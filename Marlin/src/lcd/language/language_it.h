/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Italian
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Substitutions are applied for the following characters when used
 * in menu items that call lcd_put_u8str_ind_P with an index:
 *
 *   = displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_it {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 1;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Italiano");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" pronta.");
  PROGMEM Language_Str MSG_MARLIN                          = _UxGT("Marlin");
  PROGMEM Language_Str MSG_YES                             = _UxGT("Si");
  PROGMEM Language_Str MSG_NO                              = _UxGT("No");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Indietro");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Annullando...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Media inserito");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Media rimosso");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Aspettando media");
  PROGMEM Language_Str MSG_SD_INIT_FAIL                    = _UxGT("Inizial.SD fallita");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Err.leggendo media");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("Dispos.USB rimosso");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Avvio USB fallito");
  PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW           = _UxGT("Overflow subchiamate");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Finecor."); // Max 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Finecorsa Soft");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Menu principale");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Impostaz. avanzate");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Configurazione");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Esegui files auto");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Disabilita Motori");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Menu di debug");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Test barra avanzam.");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Auto Home");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Home X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Home Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Home Z");
  PROGMEM Language_Str MSG_AUTO_HOME_I                     = _UxGT("Home ") LCD_STR_I;
  PROGMEM Language_Str MSG_AUTO_HOME_J                     = _UxGT("Home ") LCD_STR_J;
  PROGMEM Language_Str MSG_AUTO_HOME_K                     = _UxGT("Home ") LCD_STR_K;
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Allineam.automat. Z");
  PROGMEM Language_Str MSG_ITERATION                       = _UxGT("Iterazione G34: %i");
  PROGMEM Language_Str MSG_DECREASING_ACCURACY             = _UxGT("Precisione in calo!");
  PROGMEM Language_Str MSG_ACCURACY_ACHIEVED               = _UxGT("Precisione raggiunta");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Home assi XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Premi per iniziare");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Punto successivo");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Livel. terminato!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Fade Height");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Imp. offset home");
  PROGMEM Language_Str MSG_HOME_OFFSET_X                   = _UxGT("Offset home X");
  PROGMEM Language_Str MSG_HOME_OFFSET_Y                   = _UxGT("Offset home Y");
  PROGMEM Language_Str MSG_HOME_OFFSET_Z                   = _UxGT("Offset home Z");
  PROGMEM Language_Str MSG_HOME_OFFSET_I                   = _UxGT("Offset home ") LCD_STR_I;
  PROGMEM Language_Str MSG_HOME_OFFSET_J                   = _UxGT("Offset home ") LCD_STR_J;
  PROGMEM Language_Str MSG_HOME_OFFSET_K                   = _UxGT("Offset home ") LCD_STR_K;
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offset applicato");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Imposta Origine");
  PROGMEM Language_Str MSG_TRAMMING_WIZARD                 = _UxGT("Wizard Tramming");
  PROGMEM Language_Str MSG_SELECT_ORIGIN                   = _UxGT("Selez. origine");
  PROGMEM Language_Str MSG_LAST_VALUE_SP                   = _UxGT("Ultimo valore ");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Preriscalda ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Preriscalda ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Ugello");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Ugello ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Tutto");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Piatto");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" conf");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Preriscalda $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Preriscalda $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Preris.$ Ugello");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Preris.$ Ugello ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Preris.$ Tutto");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Preris.$ Piatto");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Preris.$ conf");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Prerisc.personal.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Raffredda");

  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Frequenza");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Controllo laser");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Controllo mandrino");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Potenza laser");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Potenza mandrino");
  PROGMEM Language_Str MSG_LASER_TOGGLE                    = _UxGT("Alterna Laser");
  PROGMEM Language_Str MSG_LASER_EVAC_TOGGLE               = _UxGT("Alterna soffiatore");
  PROGMEM Language_Str MSG_LASER_ASSIST_TOGGLE             = _UxGT("Alterna aria supp.");
  PROGMEM Language_Str MSG_LASER_PULSE_MS                  = _UxGT("ms impulso di test");
  PROGMEM Language_Str MSG_LASER_FIRE_PULSE                = _UxGT("Spara impulso");
  PROGMEM Language_Str MSG_FLOWMETER_FAULT                 = _UxGT("Err.flusso refrig.");
  PROGMEM Language_Str MSG_SPINDLE_TOGGLE                  = _UxGT("Alterna mandrino");
  PROGMEM Language_Str MSG_SPINDLE_EVAC_TOGGLE             = _UxGT("Alterna vuoto");
  PROGMEM Language_Str MSG_SPINDLE_FORWARD                 = _UxGT("Mandrino in avanti");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Inverti mandrino");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Accendi aliment.");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Spegni aliment.");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Estrudi");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Ritrai");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Muovi Asse");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Livella piano");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Livella piano");
  PROGMEM Language_Str MSG_BED_TRAMMING                    = _UxGT("Tramming piano");
  PROGMEM Language_Str MSG_BED_TRAMMING_RAISE              = _UxGT("Regola la vite finché la sonda non rileva il piano.");
  PROGMEM Language_Str MSG_BED_TRAMMING_IN_RANGE           = _UxGT("Tolleranza raggiunta su tutti gli angoli. Piano livellato!");
  PROGMEM Language_Str MSG_BED_TRAMMING_GOOD_POINTS        = _UxGT("Punti buoni: ");
  PROGMEM Language_Str MSG_BED_TRAMMING_LAST_Z             = _UxGT("Ultimo Z: ");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Prossimo punto");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Editor Mesh");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Modifica Mesh");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Modif. Mesh Fermata");
  PROGMEM Language_Str MSG_PROBING_POINT                   = _UxGT("Punto sondato");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Indice X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Indice Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Valore di Z");
  PROGMEM Language_Str MSG_CUSTOM_COMMANDS                 = _UxGT("Comandi personaliz.");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("Test sonda M48");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("Punto M48");
  PROGMEM Language_Str MSG_M48_OUT_OF_BOUNDS               = _UxGT("Sonda oltre i limiti");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Deviazione");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("Modo IDEX");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Strumenti Offsets");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Park");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplicazione");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Copia speculare");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Pieno controllo");
  PROGMEM Language_Str MSG_IDEX_DUPE_GAP                   = _UxGT("X-Gap-X duplicato");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2° ugello X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2° ugello Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2° ugello Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("G29 in corso");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Strumenti UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Livel.letto unificato");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Punto inclinaz.");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Mesh Manuale");
  PROGMEM Language_Str MSG_UBL_MESH_WIZARD                 = _UxGT("Creaz.guid.mesh UBL");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Metti spes. e misura");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Misura");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Rimuovi e mis.piatto");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Spostamento succes.");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Attiva UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Disattiva UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Temp. Piatto");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Temp. Piatto");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Temp. Ugello");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Temp. Ugello");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Modifica Mesh");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Modif.Mesh personal.");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Ritocca Mesh");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Modif.Mesh fatta");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Crea Mesh personal.");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Crea Mesh");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Crea Mesh ($)");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("Valida Mesh ($)");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Crea Mesh a freddo");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Aggiusta Alt. Mesh");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Altezza");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Valida Mesh");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 riscald.letto");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 riscald.ugello");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Priming manuale...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Prime a lung.fissa");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Priming terminato");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Annullato");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Uscita da G26");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Valida Mesh pers.");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Continua Mesh");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Livell. Mesh");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("Livell. 3 Punti");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Livell. Griglia Mesh");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Livella Mesh");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Punti laterali");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Tipo di Mappa");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Esporta Mappa");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Esporta per Host");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Esporta in CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Backup esterno");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Esporta Info UBL");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Riempimento");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Riempimento Manuale");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Riempimento Smart");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Riempimento Mesh");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Invalida Tutto");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Invalid.Punto Vicino");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Ritocca All");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Ritocca Punto Vicino");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Mesh Salvate");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Slot di memoria");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Carica Mesh Piatto");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Salva Mesh Piatto");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Mesh %i caricata");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Mesh %i salvata");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Nessuna memoria");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Err: Salvataggio UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Err: Ripristino UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-Offset: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-Offset Fermato");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("UBL passo passo");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Crea Mesh a freddo");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Riempimento Smart");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Valida Mesh");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Ritocca All");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Valida Mesh");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Ritocca All");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Salva Mesh Piatto");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Controllo LED");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Luci");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Presets luce");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Rosso");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Arancione");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Giallo");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Verde");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Blu");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indaco");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Viola");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Bianco");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Predefinito");
  PROGMEM Language_Str MSG_LED_CHANNEL_N                   = _UxGT("Canale =");
  PROGMEM Language_Str MSG_LEDS2                           = _UxGT("Luci #2");
  PROGMEM Language_Str MSG_NEO2_PRESETS                    = _UxGT("Presets luce #2");
  PROGMEM Language_Str MSG_NEO2_BRIGHTNESS                 = _UxGT("Luminosità");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Luci personalizzate");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Intensità rosso");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Intensità verde");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Intensità blu");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Intensità bianco");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Luminosità");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("In movimento...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("XY liberi");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Muovi X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Muovi Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Muovi Z");
  PROGMEM Language_Str MSG_MOVE_I                          = _UxGT("Muovi ") LCD_STR_I;
  PROGMEM Language_Str MSG_MOVE_J                          = _UxGT("Muovi ") LCD_STR_J;
  PROGMEM Language_Str MSG_MOVE_K                          = _UxGT("Muovi ") LCD_STR_K;
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Estrusore");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Estrusore *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Ugello freddo");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Muovi di %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Muovi di 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Muovi di 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Muovi di 10mm");
  PROGMEM Language_Str MSG_MOVE_100MM                      = _UxGT("Muovi di 100mm");
  PROGMEM Language_Str MSG_MOVE_0001IN                     = _UxGT("Muovi di 0.001\"");
  PROGMEM Language_Str MSG_MOVE_001IN                      = _UxGT("Muovi di 0.01\"");
  PROGMEM Language_Str MSG_MOVE_01IN                       = _UxGT("Muovi di 0.1\"");
  PROGMEM Language_Str MSG_MOVE_1IN                        = _UxGT("Muovi di 1\"");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Velocità");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Piatto Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Ugello");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Ugello ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Ugello parcheggiato");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Ugello in pausa");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Piatto");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Camera");
  PROGMEM Language_Str MSG_COOLER                          = _UxGT("Raffreddam. laser");
  PROGMEM Language_Str MSG_COOLER_TOGGLE                   = _UxGT("Alterna raffreddam.");
  PROGMEM Language_Str MSG_FLOWMETER_SAFETY                = _UxGT("Sicurezza flusso");
  PROGMEM Language_Str MSG_LASER                           = _UxGT("Laser");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Vel. ventola");     // Max 15 characters
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Vel. ventola ~");   // Max 15 characters
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Ventola mem. ~");   // Max 15 characters
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Extra vel.vent.");  // Max 15 characters
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Extra v.vent. ~");  // Max 15 characters
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Controller vent.");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Vel. inattivo");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Modo autom.");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Vel. attivo");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Tempo inattivo");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flusso");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flusso ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Controllo");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Autotemp");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("On");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Off");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("Calibrazione PID");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("Calibraz. PID *");
  PROGMEM Language_Str MSG_PID_CYCLE                       = _UxGT("Ciclo PID");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("Calibr.PID eseguita");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Calibrazione fallita. Estrusore errato.");
  PROGMEM Language_Str MSG_PID_TEMP_TOO_HIGH               = _UxGT("Calibrazione fallita. Temperatura troppo alta.");
  PROGMEM Language_Str MSG_PID_TIMEOUT                     = _UxGT("Calibrazione fallita! Tempo scaduto.");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Seleziona");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Seleziona *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Accel");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Jerk");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-jerk");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-jerk");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-jerk");
  PROGMEM Language_Str MSG_VI_JERK                         = _UxGT("V") LCD_STR_I _UxGT("-jerk");
  PROGMEM Language_Str MSG_VJ_JERK                         = _UxGT("V") LCD_STR_J _UxGT("-jerk");
  PROGMEM Language_Str MSG_VK_JERK                         = _UxGT("V") LCD_STR_K _UxGT("-jerk");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-jerk");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Deviaz. giunzioni");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Velocità");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vmax ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vmax ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vmax ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_I                          = _UxGT("Vmax ") LCD_STR_I;
  PROGMEM Language_Str MSG_VMAX_J                          = _UxGT("Vmax ") LCD_STR_J;
  PROGMEM Language_Str MSG_VMAX_K                          = _UxGT("Vmax ") LCD_STR_K;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vmax ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vmax *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vmin");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VTrav min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Accelerazione");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Max ") LCD_STR_A _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Max ") LCD_STR_B _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Max ") LCD_STR_C _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_I                          = _UxGT("Max ") LCD_STR_I _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_J                          = _UxGT("Max ") LCD_STR_J _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_K                          = _UxGT("Max ") LCD_STR_K _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Max ") LCD_STR_E _UxGT(" Accel");
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Max * Accel");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-Ritrazione");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-Spostamento");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Frequenza max");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Feed min");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Passi/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" passi/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" passi/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" passi/mm");
  PROGMEM Language_Str MSG_I_STEPS                         = LCD_STR_I _UxGT(" passi/mm");
  PROGMEM Language_Str MSG_J_STEPS                         = LCD_STR_J _UxGT(" passi/mm");
  PROGMEM Language_Str MSG_K_STEPS                         = LCD_STR_K _UxGT(" passi/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E passi/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* passi/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperatura");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Movimento");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filamento");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT                = _UxGT("Limite E in mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT_E              = _UxGT("Limite E *");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Diam. filo");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Diam. filo *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Rimuovi mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Carica mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("K Avanzamento");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("K Avanzamento *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Contrasto LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Salva impostazioni");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Carica impostazioni");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Ripristina imp.");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Inizializza EEPROM");
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("Err: CRC EEPROM");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("Err: Indice EEPROM");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("Err: Versione EEPROM");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Impostazioni mem.");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Aggiorna media");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Resetta stampante");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH _UxGT("Aggiorna");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Schermata info");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Prepara");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Regola");
  PROGMEM Language_Str MSG_POWER_MONITOR                   = _UxGT("Controllo aliment.");
  PROGMEM Language_Str MSG_CURRENT                         = _UxGT("Corrente");
  PROGMEM Language_Str MSG_VOLTAGE                         = _UxGT("Tensione");
  PROGMEM Language_Str MSG_POWER                           = _UxGT("Potenza");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Avvia stampa");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Prossimo");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Inizializza");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Stop");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Stampa");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Resetta");
  PROGMEM Language_Str MSG_BUTTON_IGNORE                   = _UxGT("Ignora");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Annulla");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Fatto");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Indietro");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Procedi");
  PROGMEM Language_Str MSG_BUTTON_SKIP                     = _UxGT("Salta");
  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Messa in pausa...");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pausa stampa");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Riprendi stampa");
  PROGMEM Language_Str MSG_HOST_START_PRINT                = _UxGT("Host Avvio");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Arresta stampa");
  PROGMEM Language_Str MSG_END_LOOPS                       = _UxGT("Fine cicli di rip.");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Stampa Oggetto");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Cancella Oggetto");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Canc. Oggetto =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Ripresa da PowerLoss");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Stampa da media");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Media non presente");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Sospensione...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Premi tasto..");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Stampa sospesa");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Stampa...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Stampa Annullata");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Stampa Eseguita");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Nessun Movimento");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("UCCISO. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ARRESTATO. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Ritrai mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Scamb. Ritrai mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Ritrai  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Salta mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Avanza mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Scamb. Avanza mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Avanza V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("Scamb. Avanza V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("AutoRitrai");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Lunghezza scambio");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA             = _UxGT("Extra scambio");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Lunghezza spurgo");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Cambio utensile");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Risalita Z");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Velocità innesco");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Velocità ritrazione");
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Parcheggia testa");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Recover Speed");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED          = _UxGT("Velocità ventola");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME           = _UxGT("Tempo ventola");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Auto ON");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Auto OFF");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Migrazione utensile");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Auto-migrazione");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Ultimo estrusore");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Migra a *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Cambia filamento");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Cambia filam. *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Carica filamento");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Carica filamento *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Rimuovi filamento");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Rimuovi filam. *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Rimuovi tutto");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Collega media");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Cambia media");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Rilascia media");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z probe fuori piatto");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Fattore distorsione");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Autotest BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Resetta BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Estendi BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("BLTouch modo SW");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("BLTouch modo 5V");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("BLTouch modo OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("BLTouch modo mem.");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Metti BLTouch a 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Metti BLTouch a OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Segnala modo");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("PERICOLO: impostazioni errate possono cause danni! Procedo comunque?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Inizializ.TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Test Z offset");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Memorizzare");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Estendi TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Estendi Sonda-Z");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Ritrai BLTouch");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Ritrai Sonda-Z");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s prima");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Offsets sonda");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Offset X sonda");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Offset Y sonda");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Offset Z sonda");
  PROGMEM Language_Str MSG_MOVE_NOZZLE_TO_BED              = _UxGT("Muovi ugel.su letto");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystep X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystep Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystep Z");
  PROGMEM Language_Str MSG_BABYSTEP_I                      = _UxGT("Babystep ") LCD_STR_I;
  PROGMEM Language_Str MSG_BABYSTEP_J                      = _UxGT("Babystep ") LCD_STR_J;
  PROGMEM Language_Str MSG_BABYSTEP_K                      = _UxGT("Babystep ") LCD_STR_K;
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Totali");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Interrompi se FC");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Risc.Fallito");   // Max 12 characters
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Err: TEMP RIDONDANTE");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("TEMP FUORI CONTROLLO");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("TEMP PIAT.FUORI CTRL");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("T.CAMERA FUORI CTRL");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_COOLER          = _UxGT("RAFFREDAM.FUORI CTRL");
  PROGMEM Language_Str MSG_COOLING_FAILED                  = _UxGT("Raffreddam. fallito");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err: TEMP MASSIMA");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err: TEMP MINIMA");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("STAMPANTE FERMATA");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Riavviare prego");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("g");  // One character
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h");  // One character
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m");  // One character
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Riscaldamento...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Raffreddamento..");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Risc. piatto...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Raffr. piatto...");
  PROGMEM Language_Str MSG_PROBE_HEATING                   = _UxGT("Risc. sonda...");
  PROGMEM Language_Str MSG_PROBE_COOLING                   = _UxGT("Raffr. sonda...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Risc. camera...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Raffr. camera...");
  PROGMEM Language_Str MSG_LASER_COOLING                   = _UxGT("Raffr. laser...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Calibraz. Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibra X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibra Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibra Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibra centro");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Impostaz. Delta");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto calibrazione");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Imp. altezza Delta");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Offset sonda-Z");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Barra Diagonale");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Altezza");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Raggio");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Info su stampante");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info. stampante");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("Livel. a 3 punti");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Livel. Lineare");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Livel. Bilineare");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Livel.piatto unific.");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Livel. Mesh");
  PROGMEM Language_Str MSG_MESH_DONE                       = _UxGT("Sond.mesh eseguito");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Statistiche");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Info. scheda");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistori");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Estrusori");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baud");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocollo");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Controllo fuga: OFF");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Controllo fuga: ON");
  PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT             = _UxGT("Timeout inatt.ugello");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Luci Case");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Luminosità Luci");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("STAMPANTE ERRATA");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Contat. stampa");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completati");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Tempo totale");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Lavoro più lungo");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Totale estruso");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Stampe");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Completati");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Durata");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Più lungo");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Estruso");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Temp min");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Temp max");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Alimentatore");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Potenza Drive");
  PROGMEM Language_Str MSG_DAC_PERCENT_A                   = _UxGT("Driver ") LCD_STR_A _UxGT(" %");
  PROGMEM Language_Str MSG_DAC_PERCENT_B                   = _UxGT("Driver ") LCD_STR_B _UxGT(" %");
  PROGMEM Language_Str MSG_DAC_PERCENT_C                   = _UxGT("Driver ") LCD_STR_C _UxGT(" %");
  PROGMEM Language_Str MSG_DAC_PERCENT_I                   = _UxGT("Driver ") LCD_STR_I _UxGT(" %");
  PROGMEM Language_Str MSG_DAC_PERCENT_J                   = _UxGT("Driver ") LCD_STR_J _UxGT(" %");
  PROGMEM Language_Str MSG_DAC_PERCENT_K                   = _UxGT("Driver ") LCD_STR_K _UxGT(" %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("Driver E %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("ERR.CONNESSIONE TMC");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Scrivi DAC EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("CAMBIO FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("STAMPA IN PAUSA");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("CARICA FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("RIMUOVI FILAMENTO");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("OPZIONI RIPRESA:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Spurga di più");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Riprendi stampa");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Ugello: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Sens.filo termin.");  // Max 17 characters
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Dist mm filo term.");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Home fallito");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Sondaggio fallito");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("SCELTA FILAMENTO");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("Agg.firmware MMU!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU chiede attenz.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("MMU riprendi");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("MMU ripresa...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("MMU carica");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("MMU carica tutto");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Carica fino ugello");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("MMU espelli");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("MMU espelli ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("MMU scarica");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Caric.fil. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Esplus.filam. ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Scaric.filam. ...");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Tutto");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Filamento ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Azzera MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("Azzeramento MMU...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Rimuovi, click");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Miscela");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Componente =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Miscelatore");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Gradiente");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Gradiente pieno");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Alterna miscela");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Ciclo miscela");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Miscela gradiente");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Inverti gradiente");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("V-tool attivo");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("V-tool iniziale");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT("V-tool finale");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("V-tool alias");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("Ripristina V-tools");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("Commit mix V-tool");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("V-tools ripristin.");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Z inizio:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT("Z fine:");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Giochi");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Invaders");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Sn4k3");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Maze");

  PROGMEM Language_Str MSG_BAD_PAGE                        = _UxGT("Indice pag. errato");
  PROGMEM Language_Str MSG_BAD_PAGE_SPEED                  = _UxGT("Vel. pag. errata");

  PROGMEM Language_Str MSG_EDIT_PASSWORD                   = _UxGT("Modif.password");
  PROGMEM Language_Str MSG_LOGIN_REQUIRED                  = _UxGT("Login richiesto");
  PROGMEM Language_Str MSG_PASSWORD_SETTINGS               = _UxGT("Impostaz.password");
  PROGMEM Language_Str MSG_ENTER_DIGIT                     = _UxGT("Inserisci cifra");
  PROGMEM Language_Str MSG_CHANGE_PASSWORD                 = _UxGT("Imp./Modif.password");
  PROGMEM Language_Str MSG_REMOVE_PASSWORD                 = _UxGT("Elimina password");
  PROGMEM Language_Str MSG_PASSWORD_SET                    = _UxGT("La password è ");
  PROGMEM Language_Str MSG_START_OVER                      = _UxGT("Ricominciare");
  PROGMEM Language_Str MSG_REMINDER_SAVE_SETTINGS          = _UxGT("Ricordati di mem.!");
  PROGMEM Language_Str MSG_PASSWORD_REMOVED                = _UxGT("Password eliminata");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_3_LINE("Premi per", "riprendere", "la stampa"));
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("Parcheggiando..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Attendere avvio", "del cambio", "di filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Inserisci il", "filamento e premi", "per continuare"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Premi per", "riscaldare ugello"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Riscaldam. ugello", "Attendere prego..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Attendere", "l'espulsione", "del filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_3_LINE("Attendere", "il caricamento", "del filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_3_LINE("Attendere", "lo spurgo", "del filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_3_LINE("Premi x terminare", "lo spurgo", "del filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Attendere", "la ripresa", "della stampa..."));
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Premi x continuare"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Attendere..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Inserisci e premi"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Riscalda ugello"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Riscaldamento..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Espulsione..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Caricamento..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Spurgo filamento"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Premi x terminare"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Ripresa..."));
  #endif // LCD_HEIGHT < 4

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Driver TMC");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Correnti driver");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Soglia modo ibrido");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Sensorless homing");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Stealthchop");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("Stealthchop");

  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Resetta");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" tra:");

  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Gioco");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_I                      = LCD_STR_I;
  PROGMEM Language_Str MSG_BACKLASH_J                      = LCD_STR_J;
  PROGMEM Language_Str MSG_BACKLASH_K                      = LCD_STR_K;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Correzione");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Appianamento");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("Livello asse X");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Auto Calibra");
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Timeout inattività, temperatura diminuita. Premere OK per riscaldare e riprendere di nuovo.");
  #else
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Timeout riscaldatore");
  #endif
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Riscalda");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Riscaldando...");
  PROGMEM Language_Str MSG_REHEATDONE                      = _UxGT("Riscaldato");

  PROGMEM Language_Str MSG_PROBE_WIZARD                    = _UxGT("Wizard Z offset");
  PROGMEM Language_Str MSG_PROBE_WIZARD_PROBING            = _UxGT("Altezza di riferimento sonda");
  PROGMEM Language_Str MSG_PROBE_WIZARD_MOVING             = _UxGT("Spostati in posizione di rilevazione");

  PROGMEM Language_Str MSG_SOUND                           = _UxGT("Suoni");

  PROGMEM Language_Str MSG_TOP_LEFT                        = _UxGT("Alto sinistra");
  PROGMEM Language_Str MSG_BOTTOM_LEFT                     = _UxGT("Basso sinistra");
  PROGMEM Language_Str MSG_TOP_RIGHT                       = _UxGT("Alto destra");
  PROGMEM Language_Str MSG_BOTTOM_RIGHT                    = _UxGT("Basso destra");
  PROGMEM Language_Str MSG_CALIBRATION_COMPLETED           = _UxGT("Calibrazione completata");
  PROGMEM Language_Str MSG_CALIBRATION_FAILED              = _UxGT("Calibrazione fallita");

  PROGMEM Language_Str MSG_DRIVER_BACKWARD                 = _UxGT(" driver invertito");

  PROGMEM Language_Str MSG_SD_CARD                         = _UxGT("Scheda SD");
  PROGMEM Language_Str MSG_USB_DISK                        = _UxGT("Disco USB");
}
