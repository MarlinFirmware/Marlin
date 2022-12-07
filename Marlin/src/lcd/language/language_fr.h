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
 * French
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_fr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Français");

  LSTR WELCOME_MSG                        = MACHINE_NAME _UxGT(" prête.");
  LSTR MSG_YES                            = _UxGT("Oui");
  LSTR MSG_NO                             = _UxGT("Non");
  LSTR MSG_BACK                           = _UxGT("Retour");
  LSTR MSG_MEDIA_ABORTING                 = _UxGT("Annulation...");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Média inséré");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Média retiré");
  LSTR MSG_MEDIA_WAITING                  = _UxGT("Attente média");
  LSTR MSG_MEDIA_READ_ERROR               = _UxGT("Err lecture média");
  LSTR MSG_MEDIA_USB_REMOVED              = _UxGT("USB débranché");
  LSTR MSG_MEDIA_USB_FAILED               = _UxGT("Erreur média USB");
  LSTR MSG_LCD_ENDSTOPS                   = _UxGT("Butées");
  LSTR MSG_LCD_SOFT_ENDSTOPS              = _UxGT("Butées SW");
  LSTR MSG_MAIN                           = _UxGT("Menu principal");
  LSTR MSG_ADVANCED_SETTINGS              = _UxGT("Config. avancée");
  LSTR MSG_CONFIGURATION                  = _UxGT("Configuration");
  LSTR MSG_RUN_AUTO_FILES                 = _UxGT("Exéc. auto.gcode");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Arrêter moteurs");
  LSTR MSG_DEBUG_MENU                     = _UxGT("Menu debug");
  LSTR MSG_PROGRESS_BAR_TEST              = _UxGT("Test barre progress.");
  LSTR MSG_HOMING                         = _UxGT("Origine");
  LSTR MSG_AUTO_HOME                      = _UxGT("Origine auto");
  LSTR MSG_AUTO_HOME_A                    = _UxGT("Origine @ auto");
  LSTR MSG_AUTO_HOME_X                    = _UxGT("Origine X auto");
  LSTR MSG_AUTO_HOME_Y                    = _UxGT("Origine Y auto");
  LSTR MSG_AUTO_HOME_Z                    = _UxGT("Origine Z auto");
  LSTR MSG_AUTO_Z_ALIGN                   = _UxGT("Align. Z auto");
  LSTR MSG_LEVEL_BED_HOMING               = _UxGT("Origine XYZ...");
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Clic pour commencer");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Point suivant");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Mise à niveau OK!");
  LSTR MSG_Z_FADE_HEIGHT                  = _UxGT("Hauteur lissée");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Régl. décal origine");
  LSTR MSG_HOME_OFFSET_X                  = _UxGT("Décal. origine X");
  LSTR MSG_HOME_OFFSET_Y                  = _UxGT("Décal. origine Y");
  LSTR MSG_HOME_OFFSET_Z                  = _UxGT("Décal. origine Z");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Décalages appliqués");
  LSTR MSG_TRAMMING_WIZARD                = _UxGT("Assistant Molettes");
  LSTR MSG_SELECT_ORIGIN                  = _UxGT("Molette du lit"); // Not a selection of the origin
  LSTR MSG_LAST_VALUE_SP                  = _UxGT("Ecart origine ");
  #if HAS_PREHEAT
    LSTR MSG_PREHEAT_1                    = _UxGT("Préchauffage ") PREHEAT_1_LABEL;
    LSTR MSG_PREHEAT_1_H                  = _UxGT("Préchauffage ") PREHEAT_1_LABEL " ~";
    LSTR MSG_PREHEAT_1_END                = _UxGT("Préch. ") PREHEAT_1_LABEL _UxGT(" buse");
    LSTR MSG_PREHEAT_1_END_E              = _UxGT("Préch. ") PREHEAT_1_LABEL _UxGT(" buse ~");
    LSTR MSG_PREHEAT_1_ALL                = _UxGT("Préch. ") PREHEAT_1_LABEL _UxGT(" Tout");
    LSTR MSG_PREHEAT_1_BEDONLY            = _UxGT("Préch. ") PREHEAT_1_LABEL _UxGT(" lit");
    LSTR MSG_PREHEAT_1_SETTINGS           = _UxGT("Régler préch. ") PREHEAT_1_LABEL;

    LSTR MSG_PREHEAT_M                    = _UxGT("Préchauffage $");
    LSTR MSG_PREHEAT_M_H                  = _UxGT("Préchauffage $ ~");
    LSTR MSG_PREHEAT_M_END                = _UxGT("Préch. $ buse");
    LSTR MSG_PREHEAT_M_END_E              = _UxGT("Préch. $ buse ~");
    LSTR MSG_PREHEAT_M_ALL                = _UxGT("Préch. $ Tout");
    LSTR MSG_PREHEAT_M_BEDONLY            = _UxGT("Préch. $ lit");
    LSTR MSG_PREHEAT_M_SETTINGS           = _UxGT("Régler préch. $");
  #endif
  LSTR MSG_PREHEAT_CUSTOM                 = _UxGT("Préchauf. perso");
  LSTR MSG_COOLDOWN                       = _UxGT("Refroidir");
  LSTR MSG_LASER_MENU                     = _UxGT("Contrôle Laser");
  LSTR MSG_LASER_POWER                    = _UxGT("Puissance");
  LSTR MSG_SPINDLE_REVERSE                = _UxGT("Inverser broches");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Allumer alim.");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Eteindre alim.");
  LSTR MSG_EXTRUDE                        = _UxGT("Extrusion");
  LSTR MSG_RETRACT                        = _UxGT("Rétractation");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Déplacer un axe");
  LSTR MSG_BED_LEVELING                   = _UxGT("Régler Niv. lit");
  LSTR MSG_LEVEL_BED                      = _UxGT("Niveau du lit");
  LSTR MSG_BED_TRAMMING                   = _UxGT("Niveau des coins");
  LSTR MSG_BED_TRAMMING_RAISE             = _UxGT("Relever le coin jusqu'à la sonde");
  LSTR MSG_BED_TRAMMING_IN_RANGE          = _UxGT("Coins dans la tolérance. Niveau lit.");
  LSTR MSG_NEXT_CORNER                    = _UxGT("Coin suivant");
  LSTR MSG_MESH_EDITOR                    = _UxGT("Modif. maille"); // 13 car. max
  LSTR MSG_EDIT_MESH                      = _UxGT("Modifier grille");
  LSTR MSG_EDITING_STOPPED                = _UxGT("Modification arrêtée");
  LSTR MSG_PROBING_POINT                  = _UxGT("Mesure point");
  LSTR MSG_MESH_X                         = _UxGT("Index X");
  LSTR MSG_MESH_Y                         = _UxGT("Index Y");
  LSTR MSG_MESH_EDIT_Z                    = _UxGT("Valeur Z");
  LSTR MSG_CUSTOM_COMMANDS                = _UxGT("Commandes perso");

  LSTR MSG_LCD_TILTING_MESH               = _UxGT("Mesure point");
  LSTR MSG_M48_TEST                       = _UxGT("Ecart sonde Z M48");
  LSTR MSG_M48_DEVIATION                  = _UxGT("Ecart");
  LSTR MSG_M48_POINT                      = _UxGT("Point M48");
  LSTR MSG_IDEX_MENU                      = _UxGT("Mode IDEX");
  LSTR MSG_IDEX_MODE_AUTOPARK             = _UxGT("Auto-Park");
  LSTR MSG_IDEX_MODE_DUPLICATE            = _UxGT("Duplication");
  LSTR MSG_IDEX_MODE_MIRRORED_COPY        = _UxGT("Copie miroir");
  LSTR MSG_IDEX_MODE_FULL_CTRL            = _UxGT("Contrôle complet");
  LSTR MSG_OFFSETS_MENU                   = _UxGT("Offsets Outil");
  LSTR MSG_HOTEND_OFFSET_Z                = _UxGT("Buse 2 Z");
  LSTR MSG_HOTEND_OFFSET_A                = _UxGT("Buse 2 @");
  LSTR MSG_G26_HEATING_BED                = _UxGT("G26: Chauffage du lit");
  LSTR MSG_G26_HEATING_NOZZLE             = _UxGT("Buse en chauffe...");
  LSTR MSG_G26_MANUAL_PRIME               = _UxGT("Amorce manuelle...");
  LSTR MSG_G26_FIXED_LENGTH               = _UxGT("Amorce longueur fixe");
  LSTR MSG_G26_PRIME_DONE                 = _UxGT("Amorce terminée");
  LSTR MSG_G26_CANCELED                   = _UxGT("G26 annulé");
  LSTR MSG_G26_LEAVING                    = _UxGT("Sortie G26");
  LSTR MSG_UBL_DOING_G29                  = _UxGT("G29 en cours");
  LSTR MSG_UBL_TOOLS                      = _UxGT("Outils UBL");
  LSTR MSG_UBL_LEVEL_BED                  = _UxGT("Niveau lit unifié");
  LSTR MSG_UBL_MANUAL_MESH                = _UxGT("Maillage manuel");
  LSTR MSG_UBL_BC_INSERT                  = _UxGT("Poser câle & mesurer");
  LSTR MSG_UBL_BC_INSERT2                 = _UxGT("Mesure");
  LSTR MSG_UBL_BC_REMOVE                  = _UxGT("ôter et mesurer lit");
  LSTR MSG_UBL_MOVING_TO_NEXT             = _UxGT("Aller au suivant");
  LSTR MSG_UBL_ACTIVATE_MESH              = _UxGT("Activer l'UBL");
  LSTR MSG_UBL_DEACTIVATE_MESH            = _UxGT("Désactiver l'UBL");
  LSTR MSG_UBL_SET_TEMP_BED               = _UxGT("Température lit");
  LSTR MSG_UBL_BED_TEMP_CUSTOM            = _UxGT("Température lit");
  LSTR MSG_UBL_SET_TEMP_HOTEND            = _UxGT("Température buse");
  LSTR MSG_UBL_HOTEND_TEMP_CUSTOM         = _UxGT("Température buse");
  LSTR MSG_UBL_MESH_EDIT                  = _UxGT("Modifier grille");
  LSTR MSG_UBL_EDIT_CUSTOM_MESH           = _UxGT("Modif. grille perso");
  LSTR MSG_UBL_FINE_TUNE_MESH             = _UxGT("Réglage fin");
  LSTR MSG_UBL_DONE_EDITING_MESH          = _UxGT("Terminer");
  LSTR MSG_UBL_BUILD_MESH_MENU            = _UxGT("Créer la grille");
  #if HAS_PREHEAT
    LSTR MSG_UBL_BUILD_MESH_M             = _UxGT("Créer grille $");
    LSTR MSG_UBL_VALIDATE_MESH_M          = _UxGT("Impr. grille $");
  #endif
  LSTR MSG_UBL_BUILD_CUSTOM_MESH          = _UxGT("Créer grille ...");
  LSTR MSG_UBL_BUILD_COLD_MESH            = _UxGT("Mesure à froid");
  LSTR MSG_UBL_MESH_HEIGHT_ADJUST         = _UxGT("Ajuster haut. couche");
  LSTR MSG_UBL_MESH_HEIGHT_AMOUNT         = _UxGT("Hauteur (x0.1mm)");
  LSTR MSG_UBL_VALIDATE_MESH_MENU         = _UxGT("Vérifier grille");
  LSTR MSG_UBL_VALIDATE_CUSTOM_MESH       = _UxGT("Impr. grille ...");
  LSTR MSG_UBL_CONTINUE_MESH              = _UxGT("Continuer grille");
  LSTR MSG_UBL_MESH_LEVELING              = _UxGT("Niveau par mailles");
  LSTR MSG_UBL_3POINT_MESH_LEVELING       = _UxGT("Niveau à 3 points");
  LSTR MSG_UBL_GRID_MESH_LEVELING         = _UxGT("Niveau par grille");
  LSTR MSG_UBL_MESH_LEVEL                 = _UxGT("Effectuer mesures");
  LSTR MSG_UBL_SIDE_POINTS                = _UxGT("Points latéraux");
  LSTR MSG_UBL_MAP_TYPE                   = _UxGT("Type de carte");
  LSTR MSG_UBL_OUTPUT_MAP                 = _UxGT("Exporter grille");
  LSTR MSG_UBL_OUTPUT_MAP_HOST            = _UxGT("Export pour hôte");
  LSTR MSG_UBL_OUTPUT_MAP_CSV             = _UxGT("Export en CSV");
  LSTR MSG_UBL_OUTPUT_MAP_BACKUP          = _UxGT("Export sauvegarde");
  LSTR MSG_UBL_INFO_UBL                   = _UxGT("Infos debug UBL");
  LSTR MSG_UBL_FILLIN_AMOUNT              = _UxGT("Nombre de points");
  LSTR MSG_UBL_MANUAL_FILLIN              = _UxGT("Remplissage manuel");
  LSTR MSG_UBL_SMART_FILLIN               = _UxGT("Remplissage auto");
  LSTR MSG_UBL_FILLIN_MESH                = _UxGT("Remplissage grille");
  LSTR MSG_UBL_INVALIDATE_ALL             = _UxGT("Tout effacer");
  LSTR MSG_UBL_INVALIDATE_CLOSEST         = _UxGT("Effacer le + près");
  LSTR MSG_UBL_FINE_TUNE_ALL              = _UxGT("Réglage fin (tous)");
  LSTR MSG_UBL_FINE_TUNE_CLOSEST          = _UxGT("Réglage fin + près");
  LSTR MSG_UBL_STORAGE_MESH_MENU          = _UxGT("Stockage grille");
  LSTR MSG_UBL_STORAGE_SLOT               = _UxGT("Slot mémoire");
  LSTR MSG_UBL_LOAD_MESH                  = _UxGT("Charger la grille");
  LSTR MSG_UBL_SAVE_MESH                  = _UxGT("Stocker la grille");
  LSTR MSG_MESH_LOADED                    = _UxGT("Grille %i chargée");
  LSTR MSG_MESH_SAVED                     = _UxGT("Grille %i enreg.");
  LSTR MSG_UBL_NO_STORAGE                 = _UxGT("Pas de mémoire");
  LSTR MSG_UBL_SAVE_ERROR                 = _UxGT("Err: Enreg. UBL");
  LSTR MSG_UBL_RESTORE_ERROR              = _UxGT("Err: Ouvrir UBL");
  LSTR MSG_UBL_Z_OFFSET                   = _UxGT("Z-Offset: ");
  LSTR MSG_UBL_Z_OFFSET_STOPPED           = _UxGT("Décal. Z arrêté");
  LSTR MSG_UBL_STEP_BY_STEP_MENU          = _UxGT("Assistant UBL");
  LSTR MSG_UBL_1_BUILD_COLD_MESH          = _UxGT("1.Mesure à froid");
  LSTR MSG_UBL_2_SMART_FILLIN             = _UxGT("2.Compléter auto.");
  LSTR MSG_UBL_3_VALIDATE_MESH_MENU       = _UxGT("3.Vérifier grille");
  LSTR MSG_UBL_4_FINE_TUNE_ALL            = _UxGT("4.Réglage fin");
  LSTR MSG_UBL_5_VALIDATE_MESH_MENU       = _UxGT("5.Vérifier grille");
  LSTR MSG_UBL_6_FINE_TUNE_ALL            = _UxGT("6.Réglage fin");
  LSTR MSG_UBL_7_SAVE_MESH                = _UxGT("7.Stocker grille");

  LSTR MSG_LED_CONTROL                    = _UxGT("Contrôle LED");
  LSTR MSG_LEDS                           = _UxGT("Lumière");
  LSTR MSG_LED_PRESETS                    = _UxGT("Préregl. LED");
  LSTR MSG_SET_LEDS_RED                   = _UxGT("Rouge");
  LSTR MSG_SET_LEDS_ORANGE                = _UxGT("Orange");
  LSTR MSG_SET_LEDS_YELLOW                = _UxGT("Jaune");
  LSTR MSG_SET_LEDS_GREEN                 = _UxGT("Vert");
  LSTR MSG_SET_LEDS_BLUE                  = _UxGT("Bleu");
  LSTR MSG_SET_LEDS_INDIGO                = _UxGT("Indigo");
  LSTR MSG_SET_LEDS_VIOLET                = _UxGT("Violet");
  LSTR MSG_SET_LEDS_WHITE                 = _UxGT("Blanc");
  LSTR MSG_SET_LEDS_DEFAULT               = _UxGT("Defaut");
  LSTR MSG_CUSTOM_LEDS                    = _UxGT("LEDs perso.");
  LSTR MSG_INTENSITY_R                    = _UxGT("Intensité rouge");
  LSTR MSG_INTENSITY_G                    = _UxGT("Intensité vert");
  LSTR MSG_INTENSITY_B                    = _UxGT("Intensité bleu");
  LSTR MSG_INTENSITY_W                    = _UxGT("Intensité blanc");
  LSTR MSG_LED_BRIGHTNESS                 = _UxGT("Luminosité");

  LSTR MSG_MOVING                         = _UxGT("Déplacement...");
  LSTR MSG_FREE_XY                        = _UxGT("Débloquer XY");
  LSTR MSG_MOVE_X                         = _UxGT("Déplacer X");
  LSTR MSG_MOVE_Y                         = _UxGT("Déplacer Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Déplacer Z");
  LSTR MSG_MOVE_N                         = _UxGT("Déplacer @");
  LSTR MSG_MOVE_E                         = _UxGT("Extruder");
  LSTR MSG_MOVE_EN                        = _UxGT("Extruder *");
  LSTR MSG_HOTEND_TOO_COLD                = _UxGT("Buse trop froide");
  LSTR MSG_MOVE_N_MM                      = _UxGT("Déplacer $mm");
  LSTR MSG_MOVE_01MM                      = _UxGT("Déplacer 0.1mm");
  LSTR MSG_MOVE_1MM                       = _UxGT("Déplacer 1mm");
  LSTR MSG_MOVE_10MM                      = _UxGT("Déplacer 10mm");
  LSTR MSG_MOVE_50MM                      = _UxGT("Déplacer 50mm");
  LSTR MSG_MOVE_100MM                     = _UxGT("Déplacer 100mm");
  LSTR MSG_MOVE_0001IN                    = _UxGT("Déplacer 0.001\"");
  LSTR MSG_MOVE_001IN                     = _UxGT("Déplacer 0.01\"");
  LSTR MSG_MOVE_01IN                      = _UxGT("Déplacer 0.1\"");
  LSTR MSG_MOVE_05IN                      = _UxGT("Déplacer 0.5\"");
  LSTR MSG_MOVE_1IN                       = _UxGT("Déplacer 1\"");
  LSTR MSG_SPEED                          = _UxGT("Vitesse");
  LSTR MSG_BED_Z                          = _UxGT("Lit Z");
  LSTR MSG_NOZZLE                         = _UxGT("Buse");
  LSTR MSG_NOZZLE_N                       = _UxGT("Buse ~");
  LSTR MSG_BED                            = _UxGT("Lit");
  LSTR MSG_CHAMBER                        = _UxGT("Caisson");
  LSTR MSG_FAN_SPEED                      = _UxGT("Vit.  ventil.  "); // 15 car. max
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Vit.  ventil. ~");
  LSTR MSG_STORED_FAN_N                   = _UxGT("Vit.  enreg.  ~");
  LSTR MSG_EXTRA_FAN_SPEED                = _UxGT("Extra ventil.  ");
  LSTR MSG_EXTRA_FAN_SPEED_N              = _UxGT("Extra ventil. ~");

  LSTR MSG_FLOW                           = _UxGT("Flux");
  LSTR MSG_FLOW_N                         = _UxGT("Flux ~");
  LSTR MSG_CONTROL                        = _UxGT("Contrôler");
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Min");
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Max");
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Facteur");
  LSTR MSG_AUTOTEMP                       = _UxGT("Temp. Auto.");
  LSTR MSG_LCD_ON                         = _UxGT("Marche");
  LSTR MSG_LCD_OFF                        = _UxGT("Arrêt");
  LSTR MSG_PID_AUTOTUNE                   = _UxGT("PID Autotune");
  LSTR MSG_PID_AUTOTUNE_E                 = _UxGT("PID Autotune *");
  LSTR MSG_PID_AUTOTUNE_DONE              = _UxGT("Tuning PID terminé");
  LSTR MSG_PID_BAD_EXTRUDER_NUM           = _UxGT("Echec Autotune! E incorrect");
  LSTR MSG_PID_TEMP_TOO_HIGH              = _UxGT("Echec Autotune! Temp. trop haute");
  LSTR MSG_PID_TIMEOUT                    = _UxGT("Echec Autotune! Opér. expirée");
  LSTR MSG_SELECT                         = _UxGT("Sélectionner");
  LSTR MSG_SELECT_E                       = _UxGT("Sélectionner *");
  LSTR MSG_ACC                            = _UxGT("Accélération");
  LSTR MSG_JERK                           = _UxGT("Jerk");
  LSTR MSG_VA_JERK                        = _UxGT("V") STR_A _UxGT(" jerk");
  LSTR MSG_VB_JERK                        = _UxGT("V") STR_B _UxGT(" jerk");
  LSTR MSG_VC_JERK                        = _UxGT("V") STR_C _UxGT(" jerk");
  LSTR MSG_VN_JERK                        = _UxGT("V@ jerk");
  LSTR MSG_VE_JERK                        = _UxGT("Ve jerk");
  LSTR MSG_MAX_SPEED                      = _UxGT("Max Vélocité");
  LSTR MSG_VMAX_A                         = _UxGT("Vit. Max ") STR_A;
  LSTR MSG_VMAX_B                         = _UxGT("Vit. Max ") STR_B;
  LSTR MSG_VMAX_C                         = _UxGT("Vit. Max ") STR_C;
  LSTR MSG_VMAX_N                         = _UxGT("Vit. Max @");
  LSTR MSG_VMAX_E                         = _UxGT("Vit. Max E");
  LSTR MSG_VMAX_EN                        = _UxGT("Vit. Max *");
  LSTR MSG_JUNCTION_DEVIATION             = _UxGT("Déviat. jonct.");
  LSTR MSG_VMIN                           = _UxGT("Vit. Min");
  LSTR MSG_VTRAV_MIN                      = _UxGT("Vmin course");
  LSTR MSG_ACCELERATION                   = _UxGT("Accélération");
  LSTR MSG_AMAX_A                         = _UxGT("Max Accél. ") STR_A;
  LSTR MSG_AMAX_B                         = _UxGT("Max Accél. ") STR_B;
  LSTR MSG_AMAX_C                         = _UxGT("Max Accél. ") STR_C;
  LSTR MSG_AMAX_N                         = _UxGT("Max Accél. @");
  LSTR MSG_AMAX_E                         = _UxGT("Max Accél. E");
  LSTR MSG_AMAX_EN                        = _UxGT("Max Accél. *");
  LSTR MSG_A_RETRACT                      = _UxGT("Acc.rétraction");
  LSTR MSG_A_TRAVEL                       = _UxGT("Acc.course");
  LSTR MSG_XY_FREQUENCY_LIMIT             = _UxGT("Fréquence max");
  LSTR MSG_XY_FREQUENCY_FEEDRATE          = _UxGT("Vitesse min");
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Pas/mm");
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" pas/mm");
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" pas/mm");
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" pas/mm");
  LSTR MSG_N_STEPS                        = _UxGT("@ pas/mm");
  LSTR MSG_E_STEPS                        = _UxGT("E pas/mm");
  LSTR MSG_EN_STEPS                       = _UxGT("* pas/mm");
  LSTR MSG_TEMPERATURE                    = _UxGT("Température");
  LSTR MSG_MOTION                         = _UxGT("Mouvement");
  LSTR MSG_FILAMENT                       = _UxGT("Filament");
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E en mm") SUPERSCRIPT_THREE;
  LSTR MSG_VOLUMETRIC_LIMIT               = _UxGT("Limite en mm") SUPERSCRIPT_THREE;
  LSTR MSG_VOLUMETRIC_LIMIT_E             = _UxGT("Limite *");
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Diamètre fil.");
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Diamètre fil. *");
  LSTR MSG_FILAMENT_UNLOAD                = _UxGT("Retrait mm");
  LSTR MSG_FILAMENT_LOAD                  = _UxGT("Charger mm");
  LSTR MSG_ADVANCE_K                      = _UxGT("Avance K");
  LSTR MSG_ADVANCE_K_E                    = _UxGT("Avance K *");
  LSTR MSG_BRIGHTNESS                     = _UxGT("Luminosité LCD");
  LSTR MSG_CONTRAST                       = _UxGT("Contraste LCD");
  LSTR MSG_SCREEN_TIMEOUT                 = _UxGT("Veille LCD (m)");
  LSTR MSG_BRIGHTNESS_OFF                 = _UxGT("Éteindre l'écran LCD");
  LSTR MSG_STORE_EEPROM                   = _UxGT("Enregistrer config.");
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Charger config.");
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Restaurer défauts");
  LSTR MSG_INIT_EEPROM                    = _UxGT("Initialiser EEPROM");
  LSTR MSG_SETTINGS_STORED                = _UxGT("Config. enregistrée");
  LSTR MSG_MEDIA_UPDATE                   = _UxGT("MaJ Firmware SD");
  LSTR MSG_RESET_PRINTER                  = _UxGT("RaZ imprimante");
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH  _UxGT("Actualiser");
  LSTR MSG_INFO_SCREEN                    = _UxGT("Surveiller");
  LSTR MSG_PREPARE                        = _UxGT("Préparer");
  LSTR MSG_TUNE                           = _UxGT("Régler");
  LSTR MSG_START_PRINT                    = _UxGT("Démarrer impression");
  LSTR MSG_BUTTON_NEXT                    = _UxGT("Suivant");
  LSTR MSG_BUTTON_INIT                    = _UxGT("Init.");
  LSTR MSG_BUTTON_STOP                    = _UxGT("Stop");
  LSTR MSG_BUTTON_PRINT                   = _UxGT("Imprimer");
  LSTR MSG_BUTTON_RESET                   = _UxGT("Reset");
  LSTR MSG_BUTTON_IGNORE                  = _UxGT("Ignorer");
  LSTR MSG_BUTTON_CANCEL                  = _UxGT("Annuler");
  LSTR MSG_BUTTON_DONE                    = _UxGT("Terminé");
  LSTR MSG_BUTTON_BACK                    = _UxGT("Retour");
  LSTR MSG_BUTTON_PROCEED                 = _UxGT("Procéder");
  LSTR MSG_BUTTON_SKIP                    = _UxGT("Passer");
  LSTR MSG_PAUSING                        = _UxGT("Mise en pause...");
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pause impression");
  LSTR MSG_RESUME_PRINT                   = _UxGT("Reprendre impr.");
  LSTR MSG_STOP_PRINT                     = _UxGT("Arrêter impr.");
  LSTR MSG_PRINTING_OBJECT                = _UxGT("Impression objet");
  LSTR MSG_CANCEL_OBJECT                  = _UxGT("Annuler objet");
  LSTR MSG_CANCEL_OBJECT_N                = _UxGT("Annuler objet =");
  LSTR MSG_OUTAGE_RECOVERY                = _UxGT("Récup. coup.");
  LSTR MSG_MEDIA_MENU                     = _UxGT("Impression SD");
  LSTR MSG_NO_MEDIA                       = _UxGT("Pas de média");
  LSTR MSG_DWELL                          = _UxGT("Repos...");
  LSTR MSG_USERWAIT                       = _UxGT("Attente utilis.");
  LSTR MSG_PRINT_PAUSED                   = _UxGT("Impr. en pause");
  LSTR MSG_PRINTING                       = _UxGT("Impression");
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Impr. annulée");
  LSTR MSG_NO_MOVE                        = _UxGT("Moteurs bloqués");
  LSTR MSG_KILLED                         = _UxGT("KILLED");
  LSTR MSG_STOPPED                        = _UxGT("STOPPÉ");
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Rétractation mm");
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Ech. rétr. mm");
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Vit. rétract°");
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Saut Z mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Rét.reprise mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Ech.reprise mm");
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("V.rét. reprise");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAPF  = _UxGT("V.éch. reprise");
  LSTR MSG_AUTORETRACT                    = _UxGT("Rétraction auto");
  LSTR MSG_TOOL_CHANGE                    = _UxGT("Changement outil");
  LSTR MSG_TOOL_CHANGE_ZLIFT              = _UxGT("Augmenter Z");
  LSTR MSG_SINGLENOZZLE_PRIME_SPEED       = _UxGT("Vitesse primaire");
  LSTR MSG_SINGLENOZZLE_WIPE_RETRACT      = _UxGT("Purge Retract");
  LSTR MSG_SINGLENOZZLE_RETRACT_SPEED     = _UxGT("Vitesse rétract°");
  LSTR MSG_FILAMENT_PARK_ENABLED          = _UxGT("Garer Extrudeur");
  LSTR MSG_SINGLENOZZLE_UNRETRACT_SPEED   = _UxGT("Vitesse reprise");
  LSTR MSG_SINGLENOZZLE_FAN_SPEED         = _UxGT("Vit.  ventil.");
  LSTR MSG_SINGLENOZZLE_FAN_TIME          = _UxGT("Temps ventil.");
  LSTR MSG_TOOL_MIGRATION_ON              = _UxGT("Auto ON");
  LSTR MSG_TOOL_MIGRATION_OFF             = _UxGT("Auto OFF");
  LSTR MSG_TOOL_MIGRATION                 = _UxGT("Migration d'outil");
  LSTR MSG_TOOL_MIGRATION_AUTO            = _UxGT("Migration auto");
  LSTR MSG_TOOL_MIGRATION_END             = _UxGT("Extrudeur Final");
  LSTR MSG_TOOL_MIGRATION_SWAP            = _UxGT("Migrer vers *");
  LSTR MSG_NOZZLE_STANDBY                 = _UxGT("Attente buse");
  LSTR MSG_FILAMENT_SWAP_LENGTH           = _UxGT("Longueur retrait");
  LSTR MSG_FILAMENT_SWAP_EXTRA            = _UxGT("Longueur Extra");
  LSTR MSG_FILAMENT_PURGE_LENGTH          = _UxGT("Longueur de purge");
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Changer filament");
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Changer filament *");
  LSTR MSG_FILAMENTLOAD                   = _UxGT("Charger filament");
  LSTR MSG_FILAMENTLOAD_E                 = _UxGT("Charger filament *");
  LSTR MSG_FILAMENTUNLOAD                 = _UxGT("Retrait filament");
  LSTR MSG_FILAMENTUNLOAD_E               = _UxGT("Retrait filament *");
  LSTR MSG_FILAMENTUNLOAD_ALL             = _UxGT("Retirer tout");
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Charger le média");
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Actualiser média");
  LSTR MSG_RELEASE_MEDIA                  = _UxGT("Retirer le média");
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Sonde Z hors lit");
  LSTR MSG_SKEW_FACTOR                    = _UxGT("Facteur écart");
  LSTR MSG_BLTOUCH                        = _UxGT("BLTouch");
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("Self-Test");
  LSTR MSG_BLTOUCH_RESET                  = _UxGT("Reset");
  LSTR MSG_BLTOUCH_STOW                   = _UxGT("Ranger");
  LSTR MSG_BLTOUCH_DEPLOY                 = _UxGT("Déployer");
  LSTR MSG_BLTOUCH_SW_MODE                = _UxGT("Mode SW");
  LSTR MSG_BLTOUCH_5V_MODE                = _UxGT("Mode 5V");
  LSTR MSG_BLTOUCH_OD_MODE                = _UxGT("Mode OD");
  LSTR MSG_BLTOUCH_MODE_STORE             = _UxGT("Appliquer Mode");
  LSTR MSG_BLTOUCH_MODE_STORE_5V          = _UxGT("Mise en 5V");
  LSTR MSG_BLTOUCH_MODE_STORE_OD          = _UxGT("Mise en OD");
  LSTR MSG_BLTOUCH_MODE_ECHO              = _UxGT("Afficher Mode");
  LSTR MSG_TOUCHMI_PROBE                  = _UxGT("TouchMI");
  LSTR MSG_TOUCHMI_INIT                   = _UxGT("Init. TouchMI");
  LSTR MSG_TOUCHMI_ZTEST                  = _UxGT("Test décalage Z");
  LSTR MSG_TOUCHMI_SAVE                   = _UxGT("Sauvegarde");
  LSTR MSG_MANUAL_DEPLOY_TOUCHMI          = _UxGT("Déployer TouchMI");
  LSTR MSG_MANUAL_DEPLOY                  = _UxGT("Déployer Sonde Z");
  LSTR MSG_MANUAL_STOW                    = _UxGT("Ranger Sonde Z");
  LSTR MSG_HOME_FIRST                     = _UxGT("Origine %s%s%s Premier");
  LSTR MSG_ZPROBE_OFFSETS                 = _UxGT("Position sonde Z");
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("Décalage X");
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Décalage Y");
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Décalage Z");
  LSTR MSG_BABYSTEP_X                     = _UxGT("Babystep X");
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Babystep Y");
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Babystep Z");
  LSTR MSG_BABYSTEP_N                     = _UxGT("Babystep @");
  LSTR MSG_BABYSTEP_TOTAL                 = _UxGT("Total");
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Butée abandon");
  LSTR MSG_HEATING_FAILED_LCD             = _UxGT("Err de chauffe");
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Err TEMP. REDONDANTE");
  LSTR MSG_THERMAL_RUNAWAY                = _UxGT("Err THERMIQUE");
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Err TEMP. MAX");
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Err TEMP. MIN");

  LSTR MSG_HALTED                         = _UxGT("IMPR. STOPPÉE");
  LSTR MSG_PLEASE_RESET                   = _UxGT("Redémarrer SVP");
  LSTR MSG_SHORT_DAY                      = _UxGT("j"); // One character only
  LSTR MSG_SHORT_HOUR                     = _UxGT("h"); // One character only
  LSTR MSG_SHORT_MINUTE                   = _UxGT("m"); // One character only

  LSTR MSG_HEATING                        = _UxGT("en chauffe...");
  LSTR MSG_COOLING                        = _UxGT("Refroidissement");
  LSTR MSG_BED_HEATING                    = _UxGT("Lit en chauffe...");
  LSTR MSG_BED_COOLING                    = _UxGT("Refroid. du lit...");
  LSTR MSG_PROBE_HEATING                  = _UxGT("Probe en chauffe...");
  LSTR MSG_PROBE_COOLING                  = _UxGT("Refroid. Probe...");
  LSTR MSG_CHAMBER_HEATING                = _UxGT("Chauffe caisson...");
  LSTR MSG_CHAMBER_COOLING                = _UxGT("Refroid. caisson...");
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Calibration Delta");
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Calibrer X");
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Calibrer Y");
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Calibrer Z");
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Calibrer centre");
  LSTR MSG_DELTA_SETTINGS                 = _UxGT("Réglages Delta");
  LSTR MSG_DELTA_AUTO_CALIBRATE           = _UxGT("Calibration Auto");
  LSTR MSG_DELTA_DIAG_ROD                 = _UxGT("Diagonale");
  LSTR MSG_DELTA_HEIGHT                   = _UxGT("Hauteur");
  LSTR MSG_DELTA_RADIUS                   = _UxGT("Rayon");

  LSTR MSG_INFO_MENU                      = _UxGT("Infos imprimante");
  LSTR MSG_INFO_PRINTER_MENU              = _UxGT("Infos imprimante");
  LSTR MSG_3POINT_LEVELING                = _UxGT("Niveau à 3 points");
  LSTR MSG_LINEAR_LEVELING                = _UxGT("Niveau linéaire");
  LSTR MSG_BILINEAR_LEVELING              = _UxGT("Niveau bilinéaire");
  LSTR MSG_UBL_LEVELING                   = _UxGT("Niveau lit unifié");
  LSTR MSG_MESH_LEVELING                  = _UxGT("Niveau par grille");
  LSTR MSG_MESH_DONE                      = _UxGT("Niveau terminé");
  LSTR MSG_INFO_STATS_MENU                = _UxGT("Stats. imprimante");
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Infos carte");
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Thermistances");
  LSTR MSG_INFO_EXTRUDERS                 = _UxGT("Extrudeurs");
  LSTR MSG_INFO_BAUDRATE                  = _UxGT("Bauds");
  LSTR MSG_INFO_PROTOCOL                  = _UxGT("Protocole");
  LSTR MSG_INFO_RUNAWAY_OFF               = _UxGT("Protection inactive");
  LSTR MSG_INFO_RUNAWAY_ON                = _UxGT("Protection active");
  LSTR MSG_HOTEND_IDLE_TIMEOUT            = _UxGT("Hotend Idle Timeout");

  LSTR MSG_CASE_LIGHT                     = _UxGT("Lumière caisson");
  LSTR MSG_CASE_LIGHT_BRIGHTNESS          = _UxGT("Luminosité");
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Imprimante incorrecte");

  #if LCD_WIDTH >= 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Nbre impressions");
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Terminées");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Tps impr. total");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Impr. la + longue");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total filament");
  #else
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Impressions");
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Terminées");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Total");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("+ long");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Filament");
  #endif

  LSTR MSG_INFO_MIN_TEMP                  = _UxGT("Temp Min");
  LSTR MSG_INFO_MAX_TEMP                  = _UxGT("Temp Max");
  LSTR MSG_INFO_PSU                       = _UxGT("Alim.");
  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Puiss. moteur ");
  LSTR MSG_DAC_PERCENT_N                  = _UxGT("Driver @ %");
  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC EEPROM sauv.");
  LSTR MSG_ERROR_TMC                      = _UxGT("ERREUR CONNEXION TMC");

  LSTR MSG_FILAMENT_CHANGE_HEADER         = _UxGT("CHANGER FILAMENT");
  LSTR MSG_FILAMENT_CHANGE_HEADER_PAUSE   = _UxGT("IMPR. PAUSE");
  LSTR MSG_FILAMENT_CHANGE_HEADER_LOAD    = _UxGT("CHARGER FIL");
  LSTR MSG_FILAMENT_CHANGE_HEADER_UNLOAD  = _UxGT("DECHARGER FIL");
  LSTR MSG_FILAMENT_CHANGE_OPTION_HEADER  = _UxGT("OPTIONS REPRISE:");
  LSTR MSG_FILAMENT_CHANGE_OPTION_PURGE   = _UxGT("Purger encore");
  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Reprendre impr.");
  LSTR MSG_FILAMENT_CHANGE_NOZZLE         = _UxGT("  Buse: ");
  LSTR MSG_RUNOUT_SENSOR                  = _UxGT("Capteur fil.");
  LSTR MSG_KILL_HOMING_FAILED             = _UxGT("Echec origine");
  LSTR MSG_LCD_PROBING_FAILED             = _UxGT("Echec sonde");

  LSTR MSG_KILL_MMU2_FIRMWARE             = _UxGT("MAJ firmware MMU!!");
  LSTR MSG_MMU2_CHOOSE_FILAMENT_HEADER    = _UxGT("CHOISIR FILAMENT");
  LSTR MSG_MMU2_MENU                      = _UxGT("MMU");
  LSTR MSG_MMU2_NOT_RESPONDING            = _UxGT("MMU ne répond plus");
  LSTR MSG_MMU2_RESUME                    = _UxGT("Continuer Imp. MMU");
  LSTR MSG_MMU2_RESUMING                  = _UxGT("Reprise MMU...");
  LSTR MSG_MMU2_LOAD_FILAMENT             = _UxGT("Charge dans MMU");
  LSTR MSG_MMU2_LOAD_ALL                  = _UxGT("Charger tous dans MMU");
  LSTR MSG_MMU2_LOAD_TO_NOZZLE            = _UxGT("Charger dans buse");
  LSTR MSG_MMU2_EJECT_FILAMENT            = _UxGT("Ejecter fil. du MMU");
  LSTR MSG_MMU2_EJECT_FILAMENT_N          = _UxGT("Ejecter fil. ~");
  LSTR MSG_MMU2_UNLOAD_FILAMENT           = _UxGT("Retrait filament");
  LSTR MSG_MMU2_LOADING_FILAMENT          = _UxGT("Chargem. fil. %i...");
  LSTR MSG_MMU2_EJECTING_FILAMENT         = _UxGT("Ejection fil...");
  LSTR MSG_MMU2_UNLOADING_FILAMENT        = _UxGT("Retrait fil....");
  LSTR MSG_MMU2_ALL                       = _UxGT("Tous");
  LSTR MSG_MMU2_FILAMENT_N                = _UxGT("Filament ~");
  LSTR MSG_MMU2_RESET                     = _UxGT("Réinit. MMU");
  LSTR MSG_MMU2_RESETTING                 = _UxGT("Réinit. MMU...");
  LSTR MSG_MMU2_EJECT_RECOVER             = _UxGT("Retrait, click");

  LSTR MSG_MIX_COMPONENT_N                = _UxGT("Composante =");
  LSTR MSG_MIXER                          = _UxGT("Mixeur");
  LSTR MSG_GRADIENT                       = _UxGT("Dégradé");
  LSTR MSG_FULL_GRADIENT                  = _UxGT("Dégradé complet");
  LSTR MSG_TOGGLE_MIX                     = _UxGT("Toggle mix");
  LSTR MSG_CYCLE_MIX                      = _UxGT("Cycle mix");
  LSTR MSG_GRADIENT_MIX                   = _UxGT("Mix dégradé");
  LSTR MSG_REVERSE_GRADIENT               = _UxGT("Inverser dégradé");
  LSTR MSG_ACTIVE_VTOOL                   = _UxGT("Active V-tool");
  LSTR MSG_START_VTOOL                    = _UxGT("Début V-tool");
  LSTR MSG_END_VTOOL                      = _UxGT("  Fin V-tool");
  LSTR MSG_GRADIENT_ALIAS                 = _UxGT("Alias V-tool");
  LSTR MSG_RESET_VTOOLS                   = _UxGT("Réinit. V-tools");
  LSTR MSG_COMMIT_VTOOL                   = _UxGT("Valider Mix V-tool");
  LSTR MSG_VTOOLS_RESET                   = _UxGT("V-tools réinit. ok");
  LSTR MSG_START_Z                        = _UxGT("Début Z:");
  LSTR MSG_END_Z                          = _UxGT("  Fin Z:");
  LSTR MSG_GAMES                          = _UxGT("Jeux");
  LSTR MSG_BRICKOUT                       = _UxGT("Casse-briques");
  LSTR MSG_INVADERS                       = _UxGT("Invaders");
  LSTR MSG_SNAKE                          = _UxGT("Sn4k3");
  LSTR MSG_MAZE                           = _UxGT("Labyrinthe");

  LSTR MSG_BAD_PAGE                       = _UxGT("Erreur index page");
  LSTR MSG_BAD_PAGE_SPEED                 = _UxGT("Erreur vitesse page");

  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_2_LINE("Presser bouton", "pour reprendre"));
    LSTR MSG_PAUSE_PRINT_PARKING          = _UxGT(MSG_1_LINE("Parking..."));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_2_LINE("Attente filament", "pour démarrer"));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Insérer filament", "et app. bouton", "pour continuer..."));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_2_LINE("Presser le bouton", "pour chauffer..."));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_2_LINE("Buse en chauffe", "Patienter SVP..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Attente", "retrait du filament"));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Attente", "chargement filament"));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_2_LINE("Attente", "Purge filament"));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_2_LINE("Presser pour finir", "la purge du filament"));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Attente reprise", "impression"));
  #else // LCD_HEIGHT < 4
    // Up to 2 lines allowed
    LSTR MSG_ADVANCED_PAUSE_WAITING       = _UxGT(MSG_1_LINE("Clic pour continuer"));
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_1_LINE("Patience..."));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_1_LINE("Insérer fil."));
    LSTR MSG_FILAMENT_CHANGE_HEAT         = _UxGT(MSG_1_LINE("Chauffer ?"));
    LSTR MSG_FILAMENT_CHANGE_HEATING      = _UxGT(MSG_1_LINE("Chauffage..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_1_LINE("Retrait fil..."));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_1_LINE("Chargement..."));
    LSTR MSG_FILAMENT_CHANGE_PURGE        = _UxGT(MSG_1_LINE("Purge..."));
    LSTR MSG_FILAMENT_CHANGE_CONT_PURGE   = _UxGT(MSG_1_LINE("Terminer ?"));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_1_LINE("Reprise..."));
  #endif // LCD_HEIGHT < 4

  LSTR MSG_TMC_CURRENT                    = _UxGT("Courant driver");
  LSTR MSG_TMC_HYBRID_THRS                = _UxGT("Seuil hybride");
  LSTR MSG_TMC_HOMING_THRS                = _UxGT("Home sans capteur");
  LSTR MSG_TMC_STEPPING_MODE              = _UxGT("Mode pas à pas");
  LSTR MSG_TMC_STEALTH_ENABLED            = _UxGT("StealthChop activé");
  LSTR MSG_SERVICE_RESET                  = _UxGT("Réinit.");
  LSTR MSG_SERVICE_IN                     = _UxGT("  dans:");
  LSTR MSG_BACKLASH_CORRECTION            = _UxGT("Correction");
  LSTR MSG_BACKLASH_SMOOTHING             = _UxGT("Lissage");

  LSTR MSG_LEVEL_X_AXIS                   = _UxGT("Niveau axe X");
  LSTR MSG_AUTO_CALIBRATE                 = _UxGT("Etalon. auto.");
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    LSTR MSG_HEATER_TIMEOUT               = _UxGT("En protection, temp. réduite. Ok pour rechauffer et continuer.");
  #else
    LSTR MSG_HEATER_TIMEOUT               = _UxGT("En protection");
  #endif
  LSTR MSG_REHEAT                         = _UxGT("Chauffer");
  LSTR MSG_REHEATING                      = _UxGT("Réchauffe...");

  LSTR MSG_PROBE_WIZARD                   = _UxGT("Assistant Sonde Z");
  LSTR MSG_PROBE_WIZARD_PROBING           = _UxGT("Mesure référence");
  LSTR MSG_PROBE_WIZARD_MOVING            = _UxGT("Dépl. vers pos");

  LSTR MSG_SOUND                          = _UxGT("Sons");

  LSTR MSG_TOP_LEFT                       = _UxGT("Coin haut gauche");
  LSTR MSG_BOTTOM_LEFT                    = _UxGT("Coin bas gauche");
  LSTR MSG_TOP_RIGHT                      = _UxGT("Coin haut droit");
  LSTR MSG_BOTTOM_RIGHT                   = _UxGT("Coin bas droit");
  LSTR MSG_CALIBRATION_COMPLETED          = _UxGT("Calibration terminée");
  LSTR MSG_CALIBRATION_FAILED             = _UxGT("Échec de l'étalonnage");

  LSTR MSG_SD_CARD                        = _UxGT("Carte SD");
  LSTR MSG_USB_DISK                       = _UxGT("Clé USB");
}
