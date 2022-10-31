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
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   = displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_it {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 1;
  LSTR LANGUAGE                           = _UxGT("Italiano");

  LSTR WELCOME_MSG                        = MACHINE_NAME _UxGT(" pronta.");
  LSTR MSG_YES                            = _UxGT("Si");
  LSTR MSG_NO                             = _UxGT("No");
  LSTR MSG_HIGH                           = _UxGT("ALTO");
  LSTR MSG_LOW                            = _UxGT("BASSO");
  LSTR MSG_BACK                           = _UxGT("Indietro");
  LSTR MSG_ERROR                          = _UxGT("Errore");
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Annullando...");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Media inserito");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Media rimosso");
  LSTR MSG_MEDIA_WAITING                  = _UxGT("Aspettando media");
  LSTR MSG_MEDIA_INIT_FAIL                = _UxGT("Iniz.Media fallita");
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Err.leggendo media");
  LSTR MSG_MEDIA_USB_REMOVED              = _UxGT("Dispos.USB rimosso");
  LSTR MSG_MEDIA_USB_FAILED               = _UxGT("Avvio USB fallito");
  LSTR MSG_KILL_SUBCALL_OVERFLOW          = _UxGT("Overflow subchiamate");
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Finecor."); // Max 8 characters
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Finecorsa Soft");
  LSTR MSG_MAIN                           = _UxGT("Menu principale");
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Impostaz. avanzate");
  LSTR MSG_TOOLBAR_SETUP                  = _UxGT("Cnf barra strumenti");
  LSTR MSG_OPTION_DISABLED                = _UxGT("Opzione disab.");
  LSTR MSG_CONFIGURATION                  = _UxGT("Configurazione");
  LSTR MSG_RUN_AUTO_FILES                 = _UxGT("Esegui files auto");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Disabilita Motori");
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menu di debug");
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test barra avanzam.");
  LSTR MSG_HOMING                         = _UxGT("Azzeramento");
  LSTR MSG_AUTO_HOME                      = _UxGT("Auto Home");
  LSTR MSG_AUTO_HOME_A                    = _UxGT("Home @");
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Home X");
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Home Y");
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Home Z");
  LSTR MSG_FILAMENT_SET                   = _UxGT("Impostaz.filamento");
  LSTR MSG_FILAMENT_MAN                   = _UxGT("Gestione filamento");
  LSTR MSG_MANUAL_LEVELING                = _UxGT("Livel.manuale");
  LSTR MSG_LEVBED_FL                      = _UxGT("Davanti Sinistra");
  LSTR MSG_LEVBED_FR                      = _UxGT("Davanti Destra");
  LSTR MSG_LEVBED_C                       = _UxGT("Centro");
  LSTR MSG_LEVBED_BL                      = _UxGT("Dietro Sinistra");
  LSTR MSG_LEVBED_BR                      = _UxGT("Dietro Destra");
  LSTR MSG_MANUAL_MESH                    = _UxGT("Mesh Manuale");
  LSTR MSG_AUTO_MESH                      = _UxGT("Generaz.Autom.Mesh");
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Allineam.automat. Z");
  LSTR MSG_ITERATION                      = _UxGT("Iterazione G34: %i");
  LSTR MSG_DECREASING_ACCURACY            = _UxGT("Precisione in calo!");
  LSTR MSG_ACCURACY_ACHIEVED              = _UxGT("Precisione raggiunta");
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Home assi XYZ");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Premi per iniziare");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Punto successivo");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Livel. terminato!");
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Fade Height");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Imp. offset home");
  LSTR MSG_HOME_OFFSET_X                  = _UxGT("Offset home X");
  LSTR MSG_HOME_OFFSET_Y                  = _UxGT("Offset home Y");
  LSTR MSG_HOME_OFFSET_Z                  = _UxGT("Offset home Z");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Offset applicato");
  LSTR MSG_TRAMMING_WIZARD                = _UxGT("Wizard Tramming");
  LSTR MSG_SELECT_ORIGIN                  = _UxGT("Selez. origine");
  LSTR MSG_LAST_VALUE_SP                  = _UxGT("Ultimo valore ");
  #if HAS_PREHEAT
    LSTR MSG_PREHEAT_1                    = _UxGT("Preriscalda ") PREHEAT_1_LABEL;
    LSTR MSG_PREHEAT_1_H                  = _UxGT("Preriscalda ") PREHEAT_1_LABEL " ~";
    LSTR MSG_PREHEAT_1_END                = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Ugello");
    LSTR MSG_PREHEAT_1_END_E              = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Ugello ~");
    LSTR MSG_PREHEAT_1_ALL                = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Tutto");
    LSTR MSG_PREHEAT_1_BEDONLY            = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" Piatto");
    LSTR MSG_PREHEAT_1_SETTINGS           = _UxGT("Preris.") PREHEAT_1_LABEL _UxGT(" conf");
    #ifdef PREHEAT_2_LABEL
      LSTR MSG_PREHEAT_2                  = _UxGT("Preris.") PREHEAT_2_LABEL;
      LSTR MSG_PREHEAT_2_SETTINGS         = _UxGT("Preris.") PREHEAT_2_LABEL _UxGT(" conf");
    #endif
    #ifdef PREHEAT_3_LABEL
      LSTR MSG_PREHEAT_3                  = _UxGT("Preris.") PREHEAT_3_LABEL;
      LSTR MSG_PREHEAT_3_SETTINGS         = _UxGT("Preris.") PREHEAT_3_LABEL _UxGT(" conf");
    #endif
    LSTR MSG_PREHEAT_M                    = _UxGT("Preriscalda $");
    LSTR MSG_PREHEAT_M_H                  = _UxGT("Preriscalda $ ~");
    LSTR MSG_PREHEAT_M_END                = _UxGT("Preris.$ Ugello");
    LSTR MSG_PREHEAT_M_END_E              = _UxGT("Preris.$ Ugello ~");
    LSTR MSG_PREHEAT_M_ALL                = _UxGT("Preris.$ Tutto");
    LSTR MSG_PREHEAT_M_BEDONLY            = _UxGT("Preris.$ Piatto");
    LSTR MSG_PREHEAT_M_SETTINGS           = _UxGT("Preris.$ conf");
  #endif
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Prerisc.personal.");
  LSTR MSG_COOLDOWN                       = _UxGT("Raffredda");

  LSTR MSG_CUTTER_FREQUENCY               = _UxGT("Frequenza");
  LSTR MSG_LASER_MENU                     = _UxGT("Controllo laser");
  LSTR MSG_SPINDLE_MENU                   = _UxGT("Controllo mandrino");
  LSTR MSG_LASER_POWER                    = _UxGT("Potenza laser");
  LSTR MSG_SPINDLE_POWER                  = _UxGT("Potenza mandrino");
  LSTR MSG_LASER_TOGGLE                   = _UxGT("Alterna Laser");
  LSTR MSG_LASER_EVAC_TOGGLE              = _UxGT("Alterna soffiatore");
  LSTR MSG_LASER_ASSIST_TOGGLE            = _UxGT("Alterna aria supp.");
  LSTR MSG_LASER_PULSE_MS                 = _UxGT("ms impulso di test");
  LSTR MSG_LASER_FIRE_PULSE               = _UxGT("Spara impulso");
  LSTR MSG_FLOWMETER_FAULT                = _UxGT("Err.flusso refrig.");
  LSTR MSG_SPINDLE_TOGGLE                 = _UxGT("Alterna mandrino");
  LSTR MSG_SPINDLE_EVAC_TOGGLE            = _UxGT("Alterna vuoto");
  LSTR MSG_SPINDLE_FORWARD                = _UxGT("Mandrino in avanti");
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Inverti mandrino");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Accendi aliment.");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Spegni aliment.");
  LSTR MSG_EXTRUDE                        = _UxGT("Estrudi");
  LSTR MSG_RETRACT                        = _UxGT("Ritrai");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Muovi Asse");
  LSTR MSG_BED_LEVELING                   = _UxGT("Livella piano");
  LSTR MSG_LEVEL_BED                      = _UxGT("Livella piano");
  LSTR MSG_BED_TRAMMING                   = _UxGT("Tarat.fine piano");
  LSTR MSG_BED_TRAMMING_MANUAL            = _UxGT("Tarat.fine Manuale");
  LSTR MSG_BED_TRAMMING_RAISE             = _UxGT("Regola la vite finché la sonda non rileva il piano.");
  LSTR MSG_BED_TRAMMING_IN_RANGE          = _UxGT("Tolleranza raggiunta su tutti gli angoli. Piano livellato!");
  LSTR MSG_BED_TRAMMING_GOOD_POINTS       = _UxGT("Punti buoni: ");
  LSTR MSG_BED_TRAMMING_LAST_Z            = _UxGT("Ultimo Z: ");
  LSTR MSG_NEXT_CORNER                    = _UxGT("Prossimo punto");
  LSTR MSG_MESH_EDITOR                    = _UxGT("Editor Mesh");
  LSTR MSG_MESH_VIEWER                    = _UxGT("Visualiz. Mesh");
  LSTR MSG_EDIT_MESH                      = _UxGT("Modifica Mesh");
  LSTR MSG_MESH_VIEW                      = _UxGT("Visualizza Mesh");
  LSTR MSG_EDITING_STOPPED                = _UxGT("Modif. Mesh Fermata");
  LSTR MSG_NO_VALID_MESH                  = _UxGT("Mesh non valida");
  LSTR MSG_ACTIVATE_MESH                  = _UxGT("Attiva livellamento");
  LSTR MSG_PROBING_POINT                  = _UxGT("Punto sondato");
  LSTR MSG_MESH_X                         = _UxGT("Indice X");
  LSTR MSG_MESH_Y                         = _UxGT("Indice Y");
  LSTR MSG_MESH_INSET                     = _UxGT("Mesh Inset");
  LSTR MSG_MESH_MIN_X                     = _UxGT("Mesh X minimo");
  LSTR MSG_MESH_MAX_X                     = _UxGT("Mesh X massimo");
  LSTR MSG_MESH_MIN_Y                     = _UxGT("Mesh Y minimo");
  LSTR MSG_MESH_MAX_Y                     = _UxGT("Mesh Y massimo");
  LSTR MSG_MESH_AMAX                      = _UxGT("Massimizza area");
  LSTR MSG_MESH_CENTER                    = _UxGT("Area centrale");
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Valore di Z");
  LSTR MSG_MESH_CANCEL                    = _UxGT("Mesh cancellato");
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Comandi personaliz.");
  LSTR MSG_M48_TEST                       = _UxGT("Test sonda M48");
  LSTR MSG_M48_POINT                      = _UxGT("Punto M48");
  LSTR MSG_M48_OUT_OF_BOUNDS              = _UxGT("Sonda oltre i limiti");
  LSTR MSG_M48_DEVIATION                  = _UxGT("Deviazione");
  LSTR MSG_IDEX_MENU                      = _UxGT("Modo IDEX");
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Strumenti Offsets");
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Park");
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplicazione");
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Copia speculare");
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Pieno controllo");
  LSTR MSG_IDEX_DUPE_GAP                  = _UxGT("X-Gap-X duplicato");
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("2° ugello Z");
  LSTR MSG_HOTEND_OFFSET_A                = _UxGT("2° ugello @");
  LSTR MSG_UBL_DOING_G29                  = _UxGT("G29 in corso");
  LSTR MSG_UBL_TOOLS                      = _UxGT("Strumenti UBL");
  LSTR MSG_UBL_LEVEL_BED                  = _UxGT("Livel.letto unificato");
  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Punto inclinaz.");
  LSTR MSG_UBL_TILT_MESH                  = _UxGT("Inclina Mesh");
  LSTR MSG_UBL_TILTING_GRID               = _UxGT("Dim.griglia inclin.");
  LSTR MSG_UBL_MESH_TILTED                = _UxGT("Mesh inclinata");
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Mesh Manuale");
  LSTR MSG_UBL_MESH_WIZARD                = _UxGT("Creaz.guid.mesh UBL");
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Metti spes. e misura");
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Misura");
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("Rimuovi e mis.piatto");
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Spostamento succes.");
  LSTR MSG_UBL_ACTIVATE_MESH              = _UxGT("Attiva UBL");
  LSTR MSG_UBL_DEACTIVATE_MESH            = _UxGT("Disattiva UBL");
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Temp. Piatto");
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Temp. Piatto");
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Temp. Ugello");
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Temp. Ugello");
  LSTR MSG_UBL_MESH_EDIT                  = _UxGT("Modifica Mesh");
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Modif.Mesh personal.");
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Ritocca Mesh");
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Modif.Mesh fatta");
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Crea Mesh personal.");
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Crea Mesh");
  #if HAS_PREHEAT
    LSTR MSG_UBL_BUILD_MESH_M             = _UxGT("Crea Mesh ($)");
    LSTR MSG_UBL_VALIDATE_MESH_M          = _UxGT("Valida Mesh ($)");
  #endif
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Crea Mesh a freddo");
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Aggiusta Alt. Mesh");
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Altezza");
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Valida Mesh");
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26 riscald.letto");
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("G26 riscald.ugello");
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Priming manuale...");
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Prime a lung.fissa");
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Priming terminato");
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 Annullato");
  LSTR MSG_G26_LEAVING                    = _UxGT("Uscita da G26");
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Valida Mesh pers.");
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Continua Mesh");
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Livell. Mesh");
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("Livell. 3 Punti");
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Livell. Griglia Mesh");
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Livella Mesh");
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Punti laterali");
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Tipo di Mappa");
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Esporta Mappa");
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Esporta per Host");
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Esporta in CSV");
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Backup esterno");
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Esporta Info UBL");
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Riempimento");
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Riempimento Manuale");
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Riempimento Smart");
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Riempimento Mesh");
  LSTR MSG_UBL_MESH_FILLED                = _UxGT("Pts mancanti riempiti");
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Invalida Tutto");
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Invalid.Punto Vicino");
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Ritocca Tutto");
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Ritocca Punto Vicino");
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Mesh Salvate");
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Slot di memoria");
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Carica Mesh Piatto");
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Salva Mesh Piatto");
  LSTR MSG_UBL_INVALID_SLOT               = _UxGT("Prima selez. uno slot Mesh");
  LSTR MSG_MESH_LOADED                    = _UxGT("Mesh %i caricata");
  LSTR MSG_MESH_SAVED                     = _UxGT("Mesh %i salvata");
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Nessuna memoria");
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Err: Salvataggio UBL");
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Err: Ripristino UBL");
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Z-Offset: ");
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Z-Offset Fermato");
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("UBL passo passo");
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1.Crea Mesh a freddo");
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2.Riempimento Smart");
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3.Valida Mesh");
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4.Ritocca All");
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5.Valida Mesh");
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6.Ritocca All");
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7.Salva Mesh Piatto");

  LSTR MSG_LED_CONTROL                    = _UxGT("Controllo LED");
  LSTR MSG_LEDS                           = _UxGT("Luci");
  LSTR MSG_LED_PRESETS                    = _UxGT("Presets luce");
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Rosso");
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Arancione");
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Giallo");
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Verde");
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Blu");
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indaco");
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Viola");
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Bianco");
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Predefinito");
  LSTR MSG_LED_CHANNEL_N                  = _UxGT("Canale =");
  LSTR MSG_LEDS2                          = _UxGT("Luci #2");
  LSTR MSG_NEO2_PRESETS                   = _UxGT("Presets luce #2");
  LSTR MSG_NEO2_BRIGHTNESS                = _UxGT("Luminosità");
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("Luci personalizzate");
  LSTR MSG_INTENSITY_R                    = _UxGT("Intensità rosso");
  LSTR MSG_INTENSITY_G                    = _UxGT("Intensità verde");
  LSTR MSG_INTENSITY_B                    = _UxGT("Intensità blu");
  LSTR MSG_INTENSITY_W                    = _UxGT("Intensità bianco");
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Luminosità");

  LSTR MSG_MOVING                         = _UxGT("In movimento...");
  LSTR MSG_FREE_XY                        = _UxGT("XY liberi");
  LSTR MSG_MOVE_X                         = _UxGT("Muovi X");
  LSTR MSG_MOVE_Y                         = _UxGT("Muovi Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Muovi Z");
  LSTR MSG_MOVE_N                         = _UxGT("Muovi @");
  LSTR MSG_MOVE_E                         = _UxGT("Estrusore");
  LSTR MSG_MOVE_EN                        = _UxGT("Estrusore *");
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Ugello freddo");
  LSTR MSG_MOVE_N_MM                      = _UxGT("Muovi di $mm");
  LSTR MSG_MOVE_01MM                      = _UxGT("Muovi di 0.1mm");
  LSTR MSG_MOVE_1MM                       = _UxGT("Muovi di 1mm");
  LSTR MSG_MOVE_10MM                      = _UxGT("Muovi di 10mm");
  LSTR MSG_MOVE_100MM                     = _UxGT("Muovi di 100mm");
  LSTR MSG_MOVE_0001IN                    = _UxGT("Muovi di 0.001\"");
  LSTR MSG_MOVE_001IN                     = _UxGT("Muovi di 0.01\"");
  LSTR MSG_MOVE_01IN                      = _UxGT("Muovi di 0.1\"");
  LSTR MSG_MOVE_1IN                       = _UxGT("Muovi di 1\"");
  LSTR MSG_SPEED                          = _UxGT("Velocità");
  LSTR MSG_BED_Z                          = _UxGT("Piatto Z");
  LSTR MSG_NOZZLE                         = _UxGT("Ugello");
  LSTR MSG_NOZZLE_N                       = _UxGT("Ugello ~");
  LSTR MSG_NOZZLE_PARKED                  = _UxGT("Ugello parcheggiato");
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Ugello in pausa");
  LSTR MSG_BED                            = _UxGT("Piatto");
  LSTR MSG_CHAMBER                        = _UxGT("Camera");
  LSTR MSG_COOLER                         = _UxGT("Raffreddam. laser");
  LSTR MSG_COOLER_TOGGLE                  = _UxGT("Alterna raffreddam.");
  LSTR MSG_FLOWMETER_SAFETY               = _UxGT("Sicurezza flusso");
  LSTR MSG_LASER                          = _UxGT("Laser");
  LSTR MSG_FAN_SPEED                      = _UxGT("Vel. ventola");     // Max 15 characters
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Vel. ventola ~");   // Max 15 characters
  LSTR MSG_STORED_FAN_N                   = _UxGT("Ventola mem. ~");   // Max 15 characters
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Extra vel.vent.");  // Max 15 characters
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Extra v.vent. ~");  // Max 15 characters
  LSTR MSG_CONTROLLER_FAN                 = _UxGT("Controller vent.");
  LSTR MSG_CONTROLLER_FAN_IDLE_SPEED      = _UxGT("Vel. inattivo");
  LSTR MSG_CONTROLLER_FAN_AUTO_ON         = _UxGT("Modo autom.");
  LSTR MSG_CONTROLLER_FAN_SPEED           = _UxGT("Vel. attivo");
  LSTR MSG_CONTROLLER_FAN_DURATION        = _UxGT("Tempo inattivo");
  LSTR MSG_FLOW                           = _UxGT("Flusso");
  LSTR MSG_FLOW_N                         = _UxGT("Flusso ~");
  LSTR MSG_CONTROL                        = _UxGT("Controllo");
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemp");
  LSTR MSG_LCD_ON                         = _UxGT("On");
  LSTR MSG_LCD_OFF                        = _UxGT("Off");
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("Calibrazione PID");
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("Calib.PID *");
  LSTR MSG_PID_CYCLE                      = _UxGT("Ciclo PID");
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("Calibr.PID eseguita");
  LSTR MSG_PID_AUTOTUNE_FAILED            = _UxGT("Calibr.PID fallito!");
  LSTR MSG_BAD_EXTRUDER_NUM               = _UxGT("Estrusore invalido.");
  LSTR MSG_TEMP_TOO_HIGH                  = _UxGT("Temp.troppo alta.");
  LSTR MSG_TIMEOUT                        = _UxGT("Tempo scaduto.");
  LSTR MSG_PID_BAD_EXTRUDER_NUM           = _UxGT("Calibrazione fallita! Estrusore errato.");
  LSTR MSG_PID_TEMP_TOO_HIGH              = _UxGT("Calibrazione fallita! Temperatura troppo alta.");
  LSTR MSG_PID_TIMEOUT                    = _UxGT("Calibrazione fallita! Tempo scaduto.");
  LSTR MSG_MPC_MEASURING_AMBIENT          = _UxGT("Testing heat loss");
  LSTR MSG_MPC_AUTOTUNE                   = _UxGT("MPC Autotune");
  LSTR MSG_MPC_EDIT                       = _UxGT("Modif.MPC *");
  LSTR MSG_MPC_POWER_E                    = _UxGT("Potenza *");
  LSTR MSG_MPC_BLOCK_HEAT_CAPACITY_E      = _UxGT("C blocco *");
  LSTR MSG_SENSOR_RESPONSIVENESS_E        = _UxGT("Reattiv.Sens. *");
  LSTR MSG_MPC_AMBIENT_XFER_COEFF_E       = _UxGT("H ambiente *");
  LSTR MSG_MPC_AMBIENT_XFER_COEFF_FAN_E   = _UxGT("H Amb. vent. *");
  LSTR MSG_SELECT                         = _UxGT("Seleziona");
  LSTR MSG_SELECT_E                       = _UxGT("Seleziona *");
  LSTR MSG_ACC                            = _UxGT("Accel");
  LSTR MSG_JERK                           = _UxGT("Jerk");
  LSTR MSG_VA_JERK                        = _UxGT("Max Jerk ") STR_A;
  LSTR MSG_VB_JERK                        = _UxGT("Max Jerk ") STR_B;
  LSTR MSG_VC_JERK                        = _UxGT("Max Jerk ") STR_C;
  LSTR MSG_VN_JERK                        = _UxGT("Max Jerk @");
  LSTR MSG_VE_JERK                        = _UxGT("Max Jerk E");
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Deviaz. giunzioni");
  LSTR MSG_MAX_SPEED                      = _UxGT("Vel.massima (mm/s)");
  LSTR MSG_VMAX_A                         = _UxGT("Vel.Massima ") STR_A;
  LSTR MSG_VMAX_B                         = _UxGT("Vel.Massima ") STR_B;
  LSTR MSG_VMAX_C                         = _UxGT("Vel.Massima ") STR_C;
  LSTR MSG_VMAX_N                         = _UxGT("Vel.Massima @");
  LSTR MSG_VMAX_E                         = _UxGT("Vel.Massima E");
  LSTR MSG_VMAX_EN                        = _UxGT("Vel.Massima *");
  LSTR MSG_VMIN                           = _UxGT("Vel.Minima");
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vel.Min spostam.");
  LSTR MSG_ACCELERATION                   = _UxGT("Accelerazione");
  LSTR MSG_AMAX_A                         = _UxGT("Acc.Massima ") STR_A;
  LSTR MSG_AMAX_B                         = _UxGT("Acc.Massima ") STR_B;
  LSTR MSG_AMAX_C                         = _UxGT("Acc.Massima ") STR_C;
  LSTR MSG_AMAX_N                         = _UxGT("Acc.Massima @");
  LSTR MSG_AMAX_E                         = _UxGT("Acc.Massima E");
  LSTR MSG_AMAX_EN                        = _UxGT("Acc.Massima *");
  LSTR MSG_A_RETRACT                      = _UxGT("A-Ritrazione");
  LSTR MSG_A_TRAVEL                       = _UxGT("A-Spostamento");
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Frequenza max");
  LSTR MSG_XY_FREQUENCY_FEEDRATE          = _UxGT("Feed min");
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Passi/mm");
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" passi/mm");
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" passi/mm");
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" passi/mm");
  LSTR MSG_N_STEPS                        = _UxGT("@ passi/mm");
  LSTR MSG_E_STEPS                        = _UxGT("E passi/mm");
  LSTR MSG_EN_STEPS                       = _UxGT("* passi/mm");
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatura");
  LSTR MSG_MOTION                         = _UxGT("Movimento");
  LSTR MSG_FILAMENT                       = _UxGT("Filamento");
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;
  LSTR MSG_VOLUMETRIC_LIMIT               = _UxGT("Limite E in mm") SUPERSCRIPT_THREE;
  LSTR MSG_VOLUMETRIC_LIMIT_E             = _UxGT("Limite E *");
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Diam. filo");
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Diam. filo *");
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Rimuovi mm");
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Carica mm");
  LSTR MSG_ADVANCE_K                      = _UxGT("K Avanzamento");
  LSTR MSG_ADVANCE_K_E                    = _UxGT("K Avanzamento *");
  LSTR MSG_CONTRAST                       = _UxGT("Contrasto LCD");
  LSTR MSG_BRIGHTNESS                     = _UxGT("Luminosità LCD");
  LSTR MSG_SCREEN_TIMEOUT                 = _UxGT("Timeout LCD (m)");
  LSTR MSG_BRIGHTNESS_OFF                 = _UxGT("Spegni Retroillum.");
  LSTR MSG_STORE_EEPROM                   = _UxGT("Salva impostazioni");
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Carica impostazioni");
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Ripristina imp.");
  LSTR MSG_INIT_EEPROM                    = _UxGT("Inizializza EEPROM");
  LSTR MSG_ERR_EEPROM_CRC                 = _UxGT("Err: CRC EEPROM");
  LSTR MSG_ERR_EEPROM_INDEX               = _UxGT("Err: Indice EEPROM");
  LSTR MSG_ERR_EEPROM_VERSION             = _UxGT("Err: Versione EEPROM");
  LSTR MSG_SETTINGS_STORED                = _UxGT("Impostazioni mem.");
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("Aggiorna media");
  LSTR MSG_RESET_PRINTER                  = _UxGT("Resetta stampante");
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Aggiorna");
  LSTR MSG_INFO_SCREEN                    = _UxGT("Schermata info");
  LSTR MSG_INFO_MACHINENAME               = _UxGT("Nome macchina");
  LSTR MSG_INFO_SIZE                      = _UxGT("Dimens.");
  LSTR MSG_INFO_FWVERSION                 = _UxGT("Versione firmware");
  LSTR MSG_INFO_BUILD                     = _UxGT("Dataora compilaz.");
  LSTR MSG_PREPARE                        = _UxGT("Prepara");
  LSTR MSG_TUNE                           = _UxGT("Regola");
  LSTR MSG_POWER_MONITOR                  = _UxGT("Controllo aliment.");
  LSTR MSG_CURRENT                        = _UxGT("Corrente");
  LSTR MSG_VOLTAGE                        = _UxGT("Tensione");
  LSTR MSG_POWER                          = _UxGT("Potenza");
  LSTR MSG_START_PRINT                    = _UxGT("Avvia stampa");
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Prossimo");
  LSTR MSG_BUTTON_INIT                    = _UxGT("Inizializza");
  LSTR MSG_BUTTON_STOP                    = _UxGT("Stop");
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Stampa");
  LSTR MSG_BUTTON_RESET                   = _UxGT("Resetta");
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("Ignora");
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Annulla");
  LSTR MSG_BUTTON_CONFIRM                 = _UxGT("Conferma");
  LSTR MSG_BUTTON_CONTINUE                = _UxGT("Continua");
  LSTR MSG_BUTTON_DONE                    = _UxGT("Fatto");
  LSTR MSG_BUTTON_BACK                    = _UxGT("Indietro");
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Procedi");
  LSTR MSG_BUTTON_SKIP                    = _UxGT("Salta");
  LSTR MSG_BUTTON_INFO                    = _UxGT("Info");
  LSTR MSG_BUTTON_LEVEL                   = _UxGT("Livello");
  LSTR MSG_BUTTON_PAUSE                   = _UxGT("Pausa");
  LSTR MSG_BUTTON_RESUME                  = _UxGT("Riprendi");
  LSTR MSG_BUTTON_ADVANCED                = _UxGT("Avanzato");
  LSTR MSG_BUTTON_SAVE                    = _UxGT("Memorizza");
  LSTR MSG_BUTTON_PURGE                   = _UxGT("Spurga");
  LSTR MSG_PAUSING                        = _UxGT("Messa in pausa...");
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pausa stampa");
  LSTR MSG_ADVANCED_PAUSE                 = _UxGT("Pausa Avanzata");
  LSTR MSG_RESUME_PRINT                   = _UxGT("Riprendi stampa");
  LSTR MSG_HOST_START_PRINT               = _UxGT("Host Avvio");
  LSTR MSG_STOP_PRINT                     = _UxGT("Arresta stampa");
  LSTR MSG_END_LOOPS                      = _UxGT("Fine cicli di rip.");
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Stampa Oggetto");
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Cancella Oggetto");
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Canc. Oggetto =");
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Ripresa da PowerLoss");
  LSTR MSG_CONTINUE_PRINT_JOB             = _UxGT("Cont.proc.stampa");
  LSTR MSG_MEDIA_MENU                     = _UxGT("Stampa da media");
  LSTR MSG_NO_MEDIA                       = _UxGT("Media non presente");
  LSTR MSG_DWELL                          = _UxGT("Sospensione...");
  LSTR MSG_USERWAIT                       = _UxGT("Premi tasto..");
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Stampa sospesa");
  LSTR MSG_PRINTING                       = _UxGT("Stampa...");
  LSTR MSG_STOPPING                       = _UxGT("Fermata...");
  LSTR MSG_REMAINING_TIME                 = _UxGT("Rimanente");
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Stampa Annullata");
  LSTR MSG_PRINT_DONE                     = _UxGT("Stampa Eseguita");
  LSTR MSG_PRINTER_KILLED                 = _UxGT("Stampante uccisa!");
  LSTR MSG_TURN_OFF                       = _UxGT("Spegni stampante");
  LSTR MSG_NO_MOVE                        = _UxGT("Nessun Movimento");
  LSTR MSG_KILLED                         = _UxGT("UCCISO. ");
  LSTR MSG_STOPPED                        = _UxGT("ARRESTATO. ");
  LSTR MSG_FWRETRACT                      = _UxGT("Ritraz.da firmware");
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Ritrai mm");
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Scamb. Ritrai mm");
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Ritrai  V");
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Salta mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Avanza mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Scamb. Avanza mm");
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Avanza V");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("Scamb. Avanza V");
  LSTR MSG_AUTORETRACT                    = _UxGT("AutoRitrai");
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Lunghezza scambio");
  LSTR MSG_FILAMENT_SWAP_EXTRA            = _UxGT("Extra scambio");
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Lunghezza spurgo");
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Cambio utensile");
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Risalita Z");
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Velocità innesco");
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Velocità ritrazione");
  LSTR MSG_FILAMENT_PARK_ENABLED          = _UxGT("Parcheggia testa");
  LSTR MSG_SINGLENOZZLE_UNRETRACT_SPEED   = _UxGT("Recover Speed");
  LSTR MSG_SINGLENOZZLE_FAN_SPEED         = _UxGT("Velocità ventola");
  LSTR MSG_SINGLENOZZLE_FAN_TIME          = _UxGT("Tempo ventola");
  LSTR MSG_TOOL_MIGRATION_ON              = _UxGT("Auto ON");
  LSTR MSG_TOOL_MIGRATION_OFF             = _UxGT("Auto OFF");
  LSTR MSG_TOOL_MIGRATION                 = _UxGT("Migrazione utensile");
  LSTR MSG_TOOL_MIGRATION_AUTO            = _UxGT("Auto-migrazione");
  LSTR MSG_TOOL_MIGRATION_END             = _UxGT("Ultimo estrusore");
  LSTR MSG_TOOL_MIGRATION_SWAP            = _UxGT("Migra a *");
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Cambia filamento");
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Cambia filam. *");
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Carica filamento");
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Carica filamento *");
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Rimuovi filamento");
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Rimuovi filam. *");
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Rimuovi tutto");
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Collega media");
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Cambia media");
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Rilascia media");
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Z probe fuori piatto");
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Fattore distorsione");
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Autotest BLTouch");
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Resetta BLTouch");
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Estendi BLTouch");
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("BLTouch modo SW");
  LSTR MSG_BLTOUCH_SPEED_MODE             = _UxGT("Alta Velocità");
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("BLTouch modo 5V");
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("BLTouch modo OD");
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("BLTouch modo mem.");
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Metti BLTouch a 5V");
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Metti BLTouch a OD");
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Segnala modo");
  LSTR MSG_BLTOUCH_MODE_CHANGE            = _UxGT("PERICOLO: impostazioni errate possono cause danni! Procedo comunque?");
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Inizializ.TouchMI");
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test Z offset");
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Memorizzare");
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Estendi TouchMI");
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Estendi Sonda-Z");
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Ritrai BLTouch");
  LSTR MSG_MANUAL_STOW                    = _UxGT("Ritrai Sonda-Z");
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s%s%s prima");
  LSTR MSG_ZPROBE_SETTINGS                = _UxGT("Impostazioni sonda");
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Offsets sonda");
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Offset X sonda");
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Offset Y sonda");
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Offset Z sonda");
  LSTR MSG_ZPROBE_MARGIN                  = _UxGT("Margine sonda");
  LSTR MSG_Z_FEED_RATE                    = _UxGT("Velocità Z");
  LSTR MSG_ENABLE_HS_MODE                 = _UxGT("Abilita modo HS");
  LSTR MSG_MOVE_NOZZLE_TO_BED             = _UxGT("Muovi ugel.su letto");
  LSTR MSG_BABYSTEP_X                     = _UxGT("Babystep X");
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Babystep Y");
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Babystep Z");
  LSTR MSG_BABYSTEP_N                     = _UxGT("Babystep @");
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Totali");
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Interrompi se FC");
  LSTR MSG_HEATING_FAILED_LCD             = _UxGT("Risc.Fallito");   // Max 12 characters
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Err: TEMP RIDONDANTE");
  LSTR MSG_THERMAL_RUNAWAY                = _UxGT("TEMP FUORI CONTROLLO");
  LSTR MSG_TEMP_MALFUNCTION               = _UxGT("MALFUNZIONAMENTO TEMP");
  LSTR MSG_THERMAL_RUNAWAY_BED            = _UxGT("TEMP PIAT.FUORI CTRL");
  LSTR MSG_THERMAL_RUNAWAY_CHAMBER        = _UxGT("T.CAMERA FUORI CTRL");
  LSTR MSG_THERMAL_RUNAWAY_COOLER         = _UxGT("RAFFREDAM.FUORI CTRL");
  LSTR MSG_COOLING_FAILED                 = _UxGT("Raffreddam. fallito");
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err: TEMP MASSIMA");
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err: TEMP MINIMA");
  LSTR MSG_HALTED                         = _UxGT("STAMPANTE FERMATA");
  LSTR MSG_PLEASE_WAIT                    = _UxGT("Attendere prego...");
  LSTR MSG_PLEASE_RESET                   = _UxGT("Riavviare prego");
  LSTR MSG_PREHEATING                     = _UxGT("Preriscaldam...");
  LSTR MSG_HEATING                        = _UxGT("Riscaldamento...");
  LSTR MSG_COOLING                        = _UxGT("Raffreddamento..");
  LSTR MSG_BED_HEATING                    = _UxGT("Risc. piatto...");
  LSTR MSG_BED_COOLING                    = _UxGT("Raffr. piatto...");
  LSTR MSG_PROBE_HEATING                  = _UxGT("Risc. sonda...");
  LSTR MSG_PROBE_COOLING                  = _UxGT("Raffr. sonda...");
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Risc. camera...");
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Raffr. camera...");
  LSTR MSG_LASER_COOLING                  = _UxGT("Raffr. laser...");
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibraz. Delta");
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibra X");
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibra Y");
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibra Z");
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibra centro");
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Impostaz. Delta");
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Auto calibrazione");
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Barra Diagonale");
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Altezza");
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Raggio");
  LSTR MSG_INFO_MENU                      = _UxGT("Info su stampante");
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Info. stampante");
  LSTR MSG_3POINT_LEVELING                = _UxGT("Livel. a 3 punti");
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Livel. Lineare");
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Livel. Bilineare");
  LSTR MSG_UBL_LEVELING                   = _UxGT("Livel.piatto unific.");
  LSTR MSG_MESH_LEVELING                  = _UxGT("Livel. Mesh");
  LSTR MSG_MESH_DONE                      = _UxGT("Sond.mesh eseguito");
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Statistiche");
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Info. scheda");
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Termistori");
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Estrusori");
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Baud");
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocollo");
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Controllo fuga: OFF");
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Controllo fuga: ON");
  LSTR MSG_HOTEND_IDLE_TIMEOUT            = _UxGT("Timeout inatt.ugello");
  LSTR MSG_FAN_SPEED_FAULT                = _UxGT("Err.vel.della ventola");

  LSTR MSG_CASE_LIGHT                     = _UxGT("Luci Case");
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Luminosità Luci");
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("STAMPANTE ERRATA");

  LSTR MSG_COLORS_GET                     = _UxGT("Ottieni colori");
  LSTR MSG_COLORS_SELECT                  = _UxGT("Selez.colori");
  LSTR MSG_COLORS_APPLIED                 = _UxGT("Colori applicati");
  LSTR MSG_COLORS_RED                     = _UxGT("Rosso");
  LSTR MSG_COLORS_GREEN                   = _UxGT("Verde");
  LSTR MSG_COLORS_BLUE                    = _UxGT("Blu");
  LSTR MSG_COLORS_WHITE                   = _UxGT("Bianco");
  LSTR MSG_UI_LANGUAGE                    = _UxGT("Lingua UI");
  LSTR MSG_SOUND_ENABLE                   = _UxGT("Abilita suono");
  LSTR MSG_LOCKSCREEN                     = _UxGT("Blocca Schermo");
  LSTR MSG_LOCKSCREEN_LOCKED              = _UxGT("Stamp. bloccata,");
  LSTR MSG_LOCKSCREEN_UNLOCK              = _UxGT("Scroll x sbloccare.");
  LSTR MSG_PLEASE_WAIT_REBOOT             = _UxGT("Attendere fino al riavvio.");

  #if LCD_WIDTH >= 20 || HAS_DWIN_E3V2
    LSTR MSG_MEDIA_NOT_INSERTED           = _UxGT("Nessun supporto inserito.");
    LSTR MSG_PLEASE_PREHEAT               = _UxGT("Si prega di preriscaldare l'hot end.");
    LSTR MSG_INFO_PRINT_COUNT_RESET       = _UxGT("Azzera contatori stampa");
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Contatori stampa");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Tempo totale");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Lavoro più lungo");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Totale estruso");
  #else
    LSTR MSG_MEDIA_NOT_INSERTED           = _UxGT("No Supporto");
    LSTR MSG_PLEASE_PREHEAT               = _UxGT("Prerisc. hot end.");
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Stampe");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Durata");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Più lungo");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Estruso");
  #endif

  LSTR MSG_INFO_COMPLETED_PRINTS          = _UxGT("Completate");
  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temp min");
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temp max");
  LSTR MSG_INFO_PSU                       = _UxGT("Alimentatore");
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Potenza Drive");
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("Driver @ %");
  LSTR MSG_ERROR_TMC                      = _UxGT("ERR.CONNESSIONE TMC");
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("Scrivi DAC EEPROM");
  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("CAMBIO FILAMENTO");
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("STAMPA IN PAUSA");
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("CARICA FILAMENTO");
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("RIMUOVI FILAMENTO");
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("OPZIONI RIPRESA:");
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Spurga di più");
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Riprendi stampa");
  LSTR MSG_FILAMENT_CHANGE_PURGE_CONTINUE = _UxGT("Spurga o continua?");
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Ugello: ");
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Sens.filo termin.");  // Max 17 characters
  LSTR MSG_RUNOUT_DISTANCE_MM             = _UxGT("Dist mm filo term.");
  LSTR MSG_RUNOUT_ENABLE                  = _UxGT("Abil.filo termin.");
  LSTR MSG_RUNOUT_ACTIVE                  = _UxGT("Filo termin. attivo");
  LSTR MSG_INVERT_EXTRUDER                = _UxGT("Inverti estrusore");
  LSTR MSG_EXTRUDER_MIN_TEMP              = _UxGT("Temp.min estrusore");
  LSTR MSG_FANCHECK                       = _UxGT("Verif.tacho vent.");  // Max 17 characters
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Home fallito");
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Sondaggio fallito");

  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("SCELTA FILAMENTO");
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");
  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("Agg.firmware MMU!");
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU chiede attenz.");
  LSTR MSG_MMU2_RESUME                    = _UxGT("MMU riprendi");
  LSTR MSG_MMU2_RESUMING                  = _UxGT("MMU ripresa...");
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("MMU carica");
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("MMU carica tutto");
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Carica fino ugello");
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("MMU espelli");
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("MMU espelli ~");
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("MMU scarica");
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Caric.fil. %i...");
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Esplus.filam. ...");
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Scaric.filam. ...");
  LSTR MSG_MMU2_ALL                       = _UxGT("Tutto");
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filamento ~");
  LSTR MSG_MMU2_RESET                     = _UxGT("Azzera MMU");
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Azzeramento MMU...");
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Rimuovi, click");

  LSTR MSG_MIX                            = _UxGT("Miscela");
  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Componente =");
  LSTR MSG_MIXER                          = _UxGT("Miscelatore");
  LSTR MSG_GRADIENT                       = _UxGT("Gradiente");
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Gradiente pieno");
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Alterna miscela");
  LSTR MSG_CYCLE_MIX                      = _UxGT("Ciclo miscela");
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Miscela gradiente");
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Inverti gradiente");
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("V-tool attivo");
  LSTR MSG_START_VTOOL                    = _UxGT("V-tool iniziale");
  LSTR MSG_END_VTOOL                      = _UxGT("V-tool finale");
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("V-tool alias");
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Ripristina V-tools");
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Commit mix V-tool");
  LSTR MSG_VTOOLS_RESET                   = _UxGT("V-tools ripristin.");
  LSTR MSG_START_Z                        = _UxGT("Z inizio:");
  LSTR MSG_END_Z                          = _UxGT("Z fine:");

  LSTR MSG_GAMES                          = _UxGT("Giochi");
  LSTR MSG_BRICKOUT                       = _UxGT("Brickout");
  LSTR MSG_INVADERS                       = _UxGT("Invaders");
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");
  LSTR MSG_MAZE                           = _UxGT("Maze");

  LSTR MSG_BAD_PAGE                       = _UxGT("Indice pag. errato");
  LSTR MSG_BAD_PAGE_SPEED                 = _UxGT("Vel. pag. errata");

  LSTR MSG_EDIT_PASSWORD                  = _UxGT("Modif.password");
  LSTR MSG_LOGIN_REQUIRED                 = _UxGT("Login richiesto");
  LSTR MSG_PASSWORD_SETTINGS              = _UxGT("Impostaz.password");
  LSTR MSG_ENTER_DIGIT                    = _UxGT("Inserisci cifra");
  LSTR MSG_CHANGE_PASSWORD                = _UxGT("Imp./Modif.password");
  LSTR MSG_REMOVE_PASSWORD                = _UxGT("Elimina password");
  LSTR MSG_PASSWORD_SET                   = _UxGT("La password è ");
  LSTR MSG_START_OVER                     = _UxGT("Ricominciare");
  LSTR MSG_REMINDER_SAVE_SETTINGS         = _UxGT("Ricordati di mem.!");
  LSTR MSG_PASSWORD_REMOVED               = _UxGT("Password eliminata");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_3_LINE("Premi per", "riprendere", "la stampa"));
    LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Parcheggiando..."));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Attendere avvio", "del cambio", "di filamento"));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Inserisci il", "filamento e premi", "per continuare"));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Premi per", "riscaldare ugello"));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Riscaldam. ugello", "Attendere prego..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_3_LINE("Attendere", "l'espulsione", "del filamento"));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_3_LINE("Attendere", "il caricamento", "del filamento"));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_3_LINE("Attendere", "lo spurgo", "del filamento"));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_3_LINE("Premi x terminare", "lo spurgo", "del filamento"));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_3_LINE("Attendere", "la ripresa", "della stampa..."));
  #else // LCD_HEIGHT < 4
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_1_LINE("Premi x continuare"));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_1_LINE("Attendere..."));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_1_LINE("Inserisci e premi"));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_1_LINE("Riscalda ugello"));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_1_LINE("Riscaldamento..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_1_LINE("Espulsione..."));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_1_LINE("Caricamento..."));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_1_LINE("Spurgo filamento"));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_1_LINE("Premi x terminare"));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_1_LINE("Ripresa..."));
  #endif // LCD_HEIGHT < 4

  LSTR MSG_TMC_DRIVERS                    = _UxGT("Driver TMC");
  LSTR MSG_TMC_CURRENT                    = _UxGT("Correnti driver");
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Soglia modo ibrido");
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Sensorless homing");
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Stealthchop");
  LSTR MSG_TMC_STEALTH_ENABLED            = _UxGT("Stealthchop");

  LSTR MSG_SERVICE_RESET                  = _UxGT("Resetta");
  LSTR MSG_SERVICE_IN                     = _UxGT(" tra:");

  LSTR MSG_BACKLASH                       = _UxGT("Gioco");
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Correzione");
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Appianamento");

  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Livello asse X");
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Auto Calibra");
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    LSTR MSG_HEATER_TIMEOUT               = _UxGT("Timeout inattività, temperatura diminuita. Premere OK per riscaldare e riprendere di nuovo.");
  #else
    LSTR MSG_HEATER_TIMEOUT               = _UxGT("Timeout riscaldatore");
  #endif
  LSTR MSG_REHEAT                         = _UxGT("Riscalda");
  LSTR MSG_REHEATING                      = _UxGT("Riscaldando...");
  LSTR MSG_REHEATDONE                     = _UxGT("Riscaldato");

  LSTR MSG_PROBE_WIZARD                   = _UxGT("Wizard Z offset");
  LSTR MSG_PROBE_WIZARD_PROBING           = _UxGT("Altezza di riferimento sonda");
  LSTR MSG_PROBE_WIZARD_MOVING            = _UxGT("Spostati in posizione di rilevazione");

  LSTR MSG_XATC                           = _UxGT("Proc.guid.X-Twist");
  LSTR MSG_XATC_DONE                      = _UxGT("Proc.guid.X-Twist eseg.!");
  LSTR MSG_XATC_UPDATE_Z_OFFSET           = _UxGT("Agg.Offset-Z sonda a ");

  LSTR MSG_SOUND                          = _UxGT("Suoni");

  LSTR MSG_TOP_LEFT                       = _UxGT("Alto sinistra");
  LSTR MSG_BOTTOM_LEFT                    = _UxGT("Basso sinistra");
  LSTR MSG_TOP_RIGHT                      = _UxGT("Alto destra");
  LSTR MSG_BOTTOM_RIGHT                   = _UxGT("Basso destra");
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Calibrazione completata");
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Calibrazione fallita");

  LSTR MSG_DRIVER_BACKWARD                = _UxGT(" driver invertito");

  LSTR MSG_SD_CARD                        = _UxGT("Scheda SD");
  LSTR MSG_USB_DISK                       = _UxGT("Disco USB");

  LSTR MSG_HOST_SHUTDOWN                  = _UxGT("Arresta host");
}
