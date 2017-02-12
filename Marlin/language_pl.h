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
 * Polish
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_PL_H
  #define LANGUAGE_PL_H

#define DISPLAY_CHARSET_ISO10646_PL
#define MAPPER_C3C4C5_PL

#ifndef WELCOME_MSG
  #define WELCOME_MSG							MACHINE_NAME _UxGT(" gotowy.")
#endif
#ifndef MSG_SD_INSERTED
  #if ENABLED(DOGLCD)
    #define MSG_SD_INSERTED						_UxGT("Karta włożona")
  #else
    #define MSG_SD_INSERTED						_UxGT("Karta wlozona")
  #endif
#endif
#ifndef MSG_SD_REMOVED
  #if ENABLED(DOGLCD)
    #define MSG_SD_REMOVED						_UxGT("Karta wyjęta")
  #else
    #define MSG_SD_REMOVED						_UxGT("Karta wyjeta")
  #endif
#endif
#ifndef MSG_LCD_ENDSTOPS
  #if ENABLED(DOGLCD)
    #define MSG_LCD_ENDSTOPS					_UxGT("Krańców.") // Max length 8 characters
  #else
    #define MSG_LCD_ENDSTOPS					_UxGT("Krancow.") // Max length 8 characters
  #endif
#endif
#ifndef MSG_MAIN
  #if ENABLED(DOGLCD)
    #define MSG_MAIN							_UxGT("Menu główne")
  #else
    #define MSG_MAIN							_UxGT("Menu glowne")
  #endif
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                      	_UxGT("Autostart")
#endif
#ifndef MSG_DISABLE_STEPPERS
  #if ENABLED(DOGLCD)
    #define MSG_DISABLE_STEPPERS				_UxGT("Wyłącz silniki")
  #else
    #define MSG_DISABLE_STEPPERS				_UxGT("Wylacz silniki")
  #endif
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME							_UxGT("Zeruj osie XYZ")
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X						_UxGT("Zeruj X")
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y						_UxGT("Zeruj Y")
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z						_UxGT("Zeruj Z")
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING					_UxGT("Pozycja zerowa")
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING					_UxGT("Kliknij by rozp.")
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #if ENABLED(DOGLCD)
    #define MSG_LEVEL_BED_NEXT_POINT			_UxGT("Następny punkt")
  #else
    #define MSG_LEVEL_BED_NEXT_POINT			_UxGT("Nastepny punkt")
  #endif
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE					_UxGT("Wypoziomowano!")
#endif
#ifndef MSG_LEVEL_BED_CANCEL
  #define MSG_LEVEL_BED_CANCEL					_UxGT("Anuluj")
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS					_UxGT("Ust. poz. zer.")
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED				_UxGT("Poz. zerowa ust.")
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN						_UxGT("Ustaw punkt zero")
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1							_UxGT("Rozgrzej PLA")
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N						MSG_PREHEAT_1 _UxGT(" ")
#endif 
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL						MSG_PREHEAT_1 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #if ENABLED(DOGLCD)
    #define MSG_PREHEAT_1_BEDONLY				_UxGT("Rozgrzej stół PLA")
  #else
    #define MSG_PREHEAT_1_BEDONLY				_UxGT("Rozgrzej stol PLA")
  #endif
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS				_UxGT("Ustaw. rozg. PLA")
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2							_UxGT("Rozgrzej ABS")
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N						MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL						MSG_PREHEAT_2 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #if ENABLED(DOGLCD)
    #define MSG_PREHEAT_2_BEDONLY				_UxGT("Rozgrzej stół ABS")
  #else
    #define MSG_PREHEAT_2_BEDONLY				_UxGT("Rozgrzej stol ABS")
  #endif
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS				_UxGT("Ustaw. rozg. ABS")
#endif
#ifndef MSG_PREHEAT_3
  #define MSG_PREHEAT_3							_UxGT("Rozgrzej PET")
#endif
#ifndef MSG_PREHEAT_3_N
  #define MSG_PREHEAT_3_N						MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_3_ALL
  #define MSG_PREHEAT_3_ALL						MSG_PREHEAT_2 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_3_BEDONLY
  #if ENABLED(DOGLCD)
    #define MSG_PREHEAT_3_BEDONLY				_UxGT("Rozgrzej stół PET")
  #else
    #define MSG_PREHEAT_3_BEDONLY				_UxGT("Rozgrzej stol PET")
  #endif
#endif
#ifndef MSG_PREHEAT_3_SETTINGS
  #define MSG_PREHEAT_3_SETTINGS				_UxGT("Ustaw. rozg. PET")
#endif
#ifndef MSG_PREHEAT_4
  #define MSG_PREHEAT_4							_UxGT("Rozgrzej RUBBER")
#endif
#ifndef MSG_PREHEAT_4_N
  #define MSG_PREHEAT_4_N						MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_4_ALL
  #define MSG_PREHEAT_4_ALL						MSG_PREHEAT_2 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_4_BEDONLY
  #if ENABLED(DOGLCD)
    #define MSG_PREHEAT_4_BEDONLY				_UxGT("Rozgrzej stół RUBBER")
  #else
    #define MSG_PREHEAT_4_BEDONLY				_UxGT("Rozgrzej stol RUBBER")
  #endif
#endif
#ifndef MSG_PREHEAT_4_SETTINGS
  #define MSG_PREHEAT_4_SETTINGS				_UxGT("Ustaw. rozg. RUBBER")
#endif
#ifndef MSG_PREHEAT_5
  #define MSG_PREHEAT_5							_UxGT("Rozgrzej HIPS")
#endif
#ifndef MSG_PREHEAT_5_N
  #define MSG_PREHEAT_5_N						MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_5_ALL
  #define MSG_PREHEAT_5_ALL						MSG_PREHEAT_2 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_5_BEDONLY
  #if ENABLED(DOGLCD)
    #define MSG_PREHEAT_5_BEDONLY				_UxGT("Rozgrzej stół HIPS")
  #else
    #define MSG_PREHEAT_5_BEDONLY				_UxGT("Rozgrzej stol HIPS")
  #endif
#endif
#ifndef MSG_PREHEAT_5_SETTINGS
  #define MSG_PREHEAT_5_SETTINGS				_UxGT("Ustaw. rozg. HIPS")
#endif
#ifndef MSG_PREHEAT_5
  #define MSG_PREHEAT_6							_UxGT("Rozgrzej NYLON")
#endif
#ifndef MSG_PREHEAT_6_N
  #define MSG_PREHEAT_6_N						MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_6_ALL
  #define MSG_PREHEAT_6_ALL						MSG_PREHEAT_2 _UxGT(" wsz.")
#endif
#ifndef MSG_PREHEAT_6_BEDONLY
  #if ENABLED(DOGLCD)
    #define MSG_PREHEAT_6_BEDONLY				_UxGT("Rozgrzej stół NYLON")
  #else
    #define MSG_PREHEAT_6_BEDONLY				_UxGT("Rozgrzej stol NYLON")
  #endif
#endif
#ifndef MSG_PREHEAT_6_SETTINGS
  #define MSG_PREHEAT_6_SETTINGS				_UxGT("Ustaw. rozg. NYLON")
#endif
#ifndef MSG_COOLDOWN
  #if ENABLED(DOGLCD)
    #define MSG_COOLDOWN						_UxGT("Chłodzenie")
  #else
    #define MSG_COOLDOWN						_UxGT("Chlodzenie")
  #endif
#endif
#ifndef MSG_SWITCH_PS_ON
  #if ENABLED(DOGLCD)
    #define MSG_SWITCH_PS_ON					_UxGT("Włącz zasilacz")
  #else
    #define MSG_SWITCH_PS_ON					_UxGT("Wlacz zasilacz")
  #endif
#endif
#ifndef MSG_SWITCH_PS_OFF
  #if ENABLED(DOGLCD)
    #define MSG_SWITCH_PS_OFF					_UxGT("Wyłącz zasilacz")
  #else
    #define MSG_SWITCH_PS_OFF					_UxGT("Wylacz zasilacz")
  #endif
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE							_UxGT("Ekstruzja")
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT							_UxGT("Wycofanie")
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS							_UxGT("Ruch osi")
#endif
#ifndef MSG_LEVEL_BED
  #if ENABLED(DOGLCD)
    #define MSG_LEVEL_BED						_UxGT("Poziom. stołu")
  #else
    #define MSG_LEVEL_BED						_UxGT("Poziom. stolu")
  #endif
#endif
#ifndef MSG_MOVE_X
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_X							_UxGT("Przesuń oś X")
  #else
    #define MSG_MOVE_X							_UxGT("Przesun os X")
  #endif
#endif
#ifndef MSG_MOVE_Y
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_Y							_UxGT("Przesuń oś Y")
  #else
    #define MSG_MOVE_Y							_UxGT("Przesun os Y")
  #endif
#endif
#ifndef MSG_MOVE_Z
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_Z							_UxGT("Przesuń oś Z")
  #else
    #define MSG_MOVE_Z							_UxGT("Przesun os Z")
  #endif
#endif
#ifndef MSG_MOVE_E
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_E							_UxGT("Ekstruzja (oś E)")
  #else
  #define MSG_MOVE_E							_UxGT("Ekstruzja (os E)")
  #endif
#endif
#ifndef MSG_MOVE_01MM
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_01MM						_UxGT("Przesuń co .1mm")
  #else
    #define MSG_MOVE_01MM						_UxGT("Przesun co .1mm")
  #endif
#endif
#ifndef MSG_MOVE_1MM
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_1MM						_UxGT("Przesuń co 1mm")
  #else
    #define MSG_MOVE_1MM						_UxGT("Przesun co 1mm")
  #endif
#endif
#ifndef MSG_MOVE_10MM
  #if ENABLED(DOGLCD)
    #define MSG_MOVE_10MM						_UxGT("Przesuń co 10mm")
  #else
    #define MSG_MOVE_10MM						_UxGT("Przesun co 10mm")
  #endif
#endif
#ifndef MSG_SPEED
  #if ENABLED(DOGLCD)
    #define MSG_SPEED							_UxGT("Prędkość")
  #else
    #define MSG_SPEED							_UxGT("Predkosc")
  #endif
#endif
#ifndef MSG_BED_Z
  #if ENABLED(DOGLCD)
    #define MSG_BED_Z							_UxGT("Stół Z")
  #else
    #define MSG_BED_Z							_UxGT("Stol Z")
  #endif
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE							_UxGT("Dysza")
#endif
#ifndef MSG_BED
  #if ENABLED(DOGLCD)
    #define MSG_BED								_UxGT("Stół")
  #else
    #define MSG_BED								_UxGT("Stol")
  #endif
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED							_UxGT("Obroty wiatraka")
#endif
#ifndef MSG_FLOW
  #if ENABLED(DOGLCD)
    #define MSG_FLOW							_UxGT("Przepływ")
  #else
    #define MSG_FLOW							_UxGT("Przeplyw")
  #endif
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL							_UxGT("Ustawienia")
#endif
#ifndef MSG_MIN
  #define MSG_MIN								LCD_STR_THERMOMETER _UxGT(" Min")
#endif
#ifndef MSG_MAX
  #define MSG_MAX								LCD_STR_THERMOMETER _UxGT(" Max")
#endif
#ifndef MSG_FACTOR
  #if ENABLED(DOGLCD)
    #define MSG_FACTOR							LCD_STR_THERMOMETER _UxGT(" Mnożnik")
  #else
    #define MSG_FACTOR							LCD_STR_THERMOMETER _UxGT(" Mnoznik")
  #endif
#endif
#ifndef MSG_AUTOTEMP
  #define MSG_AUTOTEMP							_UxGT("Auto. temperatura")
#endif
#ifndef MSG_ON
  #if ENABLED(DOGLCD)
    #define MSG_ON								_UxGT("Wł. ")
  #else
    #define MSG_ON								_UxGT("Wl. ")
  #endif
#endif
#ifndef MSG_OFF
  #if ENABLED(DOGLCD)
    #define MSG_OFF								_UxGT("Wył.")
  #else
    #define MSG_OFF								_UxGT("Wyl.")
  #endif
#endif
#ifndef MSG_PID_P
  #define MSG_PID_P								_UxGT("PID-P")
#endif
#ifndef MSG_PID_I
  #define MSG_PID_I								_UxGT("PID-I")
#endif
#ifndef MSG_PID_D
  #define MSG_PID_D								_UxGT("PID-D")
#endif
#ifndef MSG_PID_C
  #define MSG_PID_C								_UxGT("PID-C")
#endif
#ifndef MSG_SELECT
  #define MSG_SELECT							_UxGT("Select")
#endif
#ifndef MSG_ACC
  #if ENABLED(DOGLCD)
    #define MSG_ACC								_UxGT("Przyśpieszenie")
  #else
    #define MSG_ACC								_UxGT("Przyspieszenie")
  #endif
#endif
#ifndef MSG_VX_JERK
  #define MSG_VX_JERK							_UxGT("Zryw Vx")
#endif
#ifndef MSG_VY_JERK
  #define MSG_VY_JERK							_UxGT("Zryw Vy")
#endif
#ifndef MSG_VZ_JERK
  #define MSG_VZ_JERK							_UxGT("Zryw Vz")
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK							_UxGT("Zryw Ve")
#endif
#ifndef MSG_VMAX
  #define MSG_VMAX								_UxGT("Vmax ")
#endif
#ifndef MSG_VMIN
  #define MSG_VMIN								_UxGT("Vmin")
#endif
#ifndef MSG_VTRAV_MIN
  #define MSG_VTRAV_MIN							_UxGT("Vskok min")
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX								_UxGT("Amax")
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT							_UxGT("A-wycofanie")
#endif
#ifndef MSG_A_TRAVEL
  #if ENABLED(DOGLCD)
    #define MSG_A_TRAVEL						_UxGT("A-przesuń.")
  #else
    #define MSG_A_TRAVEL						_UxGT("A-przesun.")
  #endif
#endif
#ifndef MSG_XSTEPS
  #define MSG_XSTEPS							_UxGT("krokiX/mm")
#endif
#ifndef MSG_YSTEPS
  #define MSG_YSTEPS							_UxGT("krokiY/mm")
#endif
#ifndef MSG_ZSTEPS
  #define MSG_ZSTEPS							_UxGT("krokiZ/mm")
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS							_UxGT("krokiE/mm")
#endif
#ifndef MSG_E1STEPS
  #define MSG_E1STEPS							_UxGT("krokiE1/mm")
#endif
#ifndef MSG_E2STEPS
  #define MSG_E2STEPS							_UxGT("krokiE2/mm")
#endif
#ifndef MSG_E3STEPS
  #define MSG_E3STEPS							_UxGT("krokiE3/mm")
#endif
#ifndef MSG_E4STEPS
  #define MSG_E4STEPS							_UxGT("krokiE4/mm")
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE						_UxGT("Temperatura")
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION							_UxGT("Ruch")
#endif
#ifndef MSG_VOLUMETRIC
  #define MSG_VOLUMETRIC						_UxGT("Filament")
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED				_UxGT("E w mm3")
#endif
#ifndef MSG_FILAMENT_DIAM
  #if ENABLED(DOGLCD)
    #define MSG_FILAMENT_DIAM					_UxGT("Śr. fil.")
  #else
    #define MSG_FILAMENT_DIAM					_UxGT("Sr. fil.")
  #endif
#endif
#ifndef MSG_CONTRAST
  #define MSG_CONTRAST							_UxGT("Kontrast LCD")
#endif
#ifndef MSG_STORE_EPROM
  #if ENABLED(DOGLCD)
    #define MSG_STORE_EPROM						_UxGT("Zapisz w pamięci")
  #else
    #define MSG_STORE_EPROM						_UxGT("Zapisz w pamieci")
  #endif
#endif
#ifndef MSG_LOAD_EPROM
  #if ENABLED(DOGLCD)
    #define MSG_LOAD_EPROM						_UxGT("Wczytaj z pamięci")
  #else
    #define MSG_LOAD_EPROM						_UxGT("Wczytaj z pamieci")
  #endif
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE					_UxGT("Ustaw. fabryczne")
#endif
#ifndef MSG_REFRESH
  #if ENABLED(DOGLCD)
    #define MSG_REFRESH							_UxGT("Odśwież")
  #else
    #define MSG_REFRESH							_UxGT("Odswiez")
  #endif
#endif
#ifndef MSG_WATCH
  #if ENABLED(DOGLCD)
    #define MSG_WATCH							_UxGT("Ekran główny")
  #else
    #define MSG_WATCH							_UxGT("Ekran glowny")
  #endif
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE							_UxGT("Przygotuj")
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE								_UxGT("Strojenie")
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT						_UxGT("Pauza")
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT						_UxGT("Wznowienie")
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT						_UxGT("Stop")
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU							_UxGT("Karta SD")
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD							_UxGT("Brak karty")
#endif
#ifndef MSG_DWELL
  #if ENABLED(DOGLCD)
    #define MSG_DWELL							_UxGT("Uśpij...")
  #else
    #define MSG_DWELL							_UxGT("Uspij...")
  #endif
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT							_UxGT("Oczekiwanie...")
#endif
#ifndef MSG_RESUMING
  #define MSG_RESUMING							_UxGT("Wznawianie druku")
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED						_UxGT("Druk przerwany")
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE							_UxGT("Brak ruchu")
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED							_UxGT("Ubity. ")
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED							_UxGT("Zatrzymany. ")
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT					_UxGT("Wycofaj mm")
#endif
#ifndef MSG_CONTROL_RETRACT_SWAP
  #define MSG_CONTROL_RETRACT_SWAP				_UxGT("Z Wycof. mm")
#endif
#ifndef MSG_CONTROL_RETRACTF
  #define MSG_CONTROL_RETRACTF					_UxGT("Wycofaj  V")
#endif
#ifndef MSG_CONTROL_RETRACT_ZLIFT
  #define MSG_CONTROL_RETRACT_ZLIFT				_UxGT("Skok Z mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER			_UxGT("Cof. wycof. mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP		_UxGT("Z Cof. wyc. mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF			_UxGT("Cof. wycof.  V")
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT						_UxGT("Auto. wycofanie")
#endif
#ifndef MSG_FILAMENTCHANGE
  #if ENABLED(DOGLCD)
    #define MSG_FILAMENTCHANGE					_UxGT("Zmień filament")
  #else
    #define MSG_FILAMENTCHANGE					_UxGT("Zmien filament")
  #endif
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD						_UxGT("Inicjal. karty SD")
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD						_UxGT("Zmiana karty SD")
#endif
#ifndef MSG_ZPROBE_OUT
  #if ENABLED(DOGLCD)
    #define MSG_ZPROBE_OUT						_UxGT("Sonda Z za stołem")
  #else
    #define MSG_ZPROBE_OUT						_UxGT("Sonda Z za stolem")
  #endif
#endif
#ifndef MSG_BLTOUCH_SELFTEST
  #define MSG_BLTOUCH_SELFTEST					_UxGT("BLTouch Self-Test")
#endif
#ifndef MSG_BLTOUCH_RESET
  #define MSG_BLTOUCH_RESET						_UxGT("Reset BLTouch")
#endif
#ifndef MSG_HOME
  #define MSG_HOME								_UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST								_UxGT("first")
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET					_UxGT("Offset Z")
#endif
#ifndef MSG_BABYSTEP_X
  #define MSG_BABYSTEP_X						_UxGT("Babystep X")
#endif
#ifndef MSG_BABYSTEP_Y
  #define MSG_BABYSTEP_Y						_UxGT("Babystep Y")
#endif
#ifndef MSG_BABYSTEP_Z
  #define MSG_BABYSTEP_Z						_UxGT("Babystep Z")
#endif
#ifndef MSG_ENDSTOP_ABORT
  #if ENABLED(DOGLCD)
    #define MSG_ENDSTOP_ABORT					_UxGT("Błąd krancowki")
  #else
    #define MSG_ENDSTOP_ABORT					_UxGT("Blad krancowki")
  #endif
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD				_UxGT("Rozgrz. nieudane")
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #if ENABLED(DOGLCD)
    #define MSG_ERR_REDUNDANT_TEMP				_UxGT("Błąd temperatury")
  #else
    #define MSG_ERR_REDUNDANT_TEMP				_UxGT("Blad temperatury")
  #endif
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY					_UxGT("Zanik temp.")
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP						_UxGT("Err max temp")
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP						_UxGT("Err min temp")
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #if ENABLED(DOGLCD)
    #define MSG_ERR_MAXTEMP_BED					_UxGT("Err max temp stołu")
  #else
    #define MSG_ERR_MAXTEMP_BED					_UxGT("Err max temp stolu")
  #endif
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #if ENABLED(DOGLCD)
    #define MSG_ERR_MINTEMP_BED					_UxGT("Err min temp stołu")
  #else
    #define MSG_ERR_MINTEMP_BED					_UxGT("Err min temp stolu")
  #endif
#endif
#ifndef MSG_ERR_Z_HOMING
  #define MSG_ERR_Z_HOMING						_UxGT("G28 Z Forbidden")
#endif
#ifndef MSG_ERR_Z_HOMING
  #define MSG_HALTED							_UxGT("Drukarka zatrzym.")
#endif
#ifndef MSG_PLEASE_RESET
  #if ENABLED(DOGLCD)
    #define MSG_PLEASE_RESET					_UxGT("Proszę zresetować")
  #else
    #define MSG_PLEASE_RESET					_UxGT("Prosze zresetowac")
  #endif
#endif
#ifndef MSG_PLEASE_RESET
  #define MSG_SHORT_DAY							_UxGT("d") // One character only
#endif
#ifndef MSG_SHORT_HOUR
  #define MSG_SHORT_HOUR						_UxGT("g") // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE						_UxGT("m") // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING							_UxGT("Rozgrzewanie...")
#endif
#ifndef MSG_HEATING_COMPLETE
  #define MSG_HEATING_COMPLETE					_UxGT("Rozgrzano")
#endif
#ifndef MSG_BED_HEATING
  #if ENABLED(DOGLCD)
    #define MSG_BED_HEATING						_UxGT("Rozgrzewanie stołu...")
  #else
    #define MSG_BED_HEATING						_UxGT("Rozgrzewanie stolu...")
  #endif
#endif
#ifndef MSG_BED_DONE
  #if ENABLED(DOGLCD)
    #define MSG_BED_DONE						_UxGT("Rozgrzano stół")
  #else
    #define MSG_BED_DONE						_UxGT("Rozgrzano stol")
  #endif
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE					_UxGT("Kalibrowanie Delty")
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X					_UxGT("Kalibruj X")
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y					_UxGT("Kalibruj Y")
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z					_UxGT("Kalibruj Z")
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #if ENABLED(DOGLCD)
    #define MSG_DELTA_CALIBRATE_CENTER			_UxGT("Kalibruj środek")
  #else
    #define MSG_DELTA_CALIBRATE_CENTER			_UxGT("Kalibruj srodek")
  #endif
#endif

#ifndef MSG_INFO_MENU
  #define MSG_INFO_MENU							_UxGT("O drukarce")
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU					_UxGT("Info drukarki")
#endif
#ifndef MSG_INFO_STATS_MENU
  #define MSG_INFO_STATS_MENU					_UxGT("Statystyki")
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU					_UxGT("Board Info")
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU				_UxGT("Termistory")
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS					_UxGT("Ekstrudery")
#endif
#ifndef MSG_INFO_BAUDRATE
  #if ENABLED(DOGLCD)
    #define MSG_INFO_BAUDRATE					_UxGT("Prędkość USB")
  #else
    #define MSG_INFO_BAUDRATE					_UxGT("Predkosc USB")
  #endif
#endif
#ifndef MSG_INFO_PROTOCOL
  #if ENABLED(DOGLCD)
    #define MSG_INFO_PROTOCOL					_UxGT("Protokół")
  #else
    #define MSG_INFO_PROTOCOL					_UxGT("Protokol")
  #endif
#endif
#ifndef MSG_LIGHTS_ON
  #if ENABLED(DOGLCD)
    #define MSG_LIGHTS_ON						_UxGT("Oświetlenie wł.")
  #else
    #define MSG_LIGHTS_ON						_UxGT("Oswietlenie wl.")
#endif
#endif
#ifndef MSG_LIGHTS_OFF
  #if ENABLED(DOGLCD)
    #define MSG_LIGHTS_OFF						_UxGT("Oświetlenie wył.")
  #else
    #define MSG_LIGHTS_OFF						_UxGT("Oswietlenie wyl.")
  #endif
#endif


#if LCD_WIDTH >= 20
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT				_UxGT("Wydrukowano")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #if ENABLED(DOGLCD)
      #define MSG_INFO_COMPLETED_PRINTS			_UxGT("Ukończono")
    #else
      #define MSG_INFO_COMPLETED_PRINTS			_UxGT("Ukonczono")
    #endif
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME					_UxGT("Czas druku")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #if ENABLED(DOGLCD)
      #define MSG_INFO_PRINT_LONGEST			_UxGT("Najdł. druk")
    #else
      #define MSG_INFO_PRINT_LONGEST			_UxGT("Najdl. druk")
    #endif
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #if ENABLED(DOGLCD)
      #define MSG_INFO_PRINT_FILAMENT			_UxGT("Użyty fil.")
    #else
      #define MSG_INFO_PRINT_FILAMENT			_UxGT("Uzyty fil.")
    #endif
  #endif
  #else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT				_UxGT("Wydrukowano")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #if ENABLED(DOGLCD)
      #define MSG_INFO_COMPLETED_PRINTS			_UxGT("Ukończono")
    #else
      #define MSG_INFO_COMPLETED_PRINTS			_UxGT("Ukonczono")
    #endif
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME					_UxGT("Razem")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #if ENABLED(DOGLCD)
      #define MSG_INFO_PRINT_LONGEST			_UxGT("Najdł. druk")
    #else
      #define MSG_INFO_PRINT_LONGEST			_UxGT("Najdl. druk")
    #endif
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #if ENABLED(DOGLCD)
      #define MSG_INFO_PRINT_FILAMENT			_UxGT("Użyty fil.")
    #else
      #define MSG_INFO_PRINT_FILAMENT			_UxGT("Uzyty fil.")
    #endif
  #endif
#endif

#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP						_UxGT("Min Temp")
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP						_UxGT("Max Temp")
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU							_UxGT("Zasilacz")
#endif

#ifndef MSG_DRIVE_STRENGTH
  #if ENABLED(DOGLCD)
    #define MSG_DRIVE_STRENGTH					_UxGT("Siła silnika")
  #else
    #define MSG_DRIVE_STRENGTH					_UxGT("Sila silnika")
  #endif
#endif
#ifndef MSG_DAC_PERCENT
  #if ENABLED(DOGLCD)
    #define MSG_DAC_PERCENT						_UxGT("Siła %")
  #else
    #define MSG_DAC_PERCENT						_UxGT("Sila %")
  #endif
#endif
#ifndef MSG_DAC_EEPROM_WRITE
  #define MSG_DAC_EEPROM_WRITE					_UxGT("Zapisz DAC EEPROM")
#endif

#ifndef MSG_FILAMENT_CHANGE_HEADER
  #if ENABLED(DOGLCD)
    #define MSG_FILAMENT_CHANGE_HEADER			_UxGT("ZMIEŃ FILAMENT")
  #else
    #define MSG_FILAMENT_CHANGE_HEADER			_UxGT("ZMIEN FILAMENT")
  #endif
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #if ENABLED(DOGLCD)
    #define MSG_FILAMENT_CHANGE_OPTION_HEADER	_UxGT("ZMIEŃ OPCJE:")
  #else
    #define MSG_FILAMENT_CHANGE_OPTION_HEADER	_UxGT("ZMIEN OPCJE:")
  #endif
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_EXTRUDE
  #if ENABLED(DOGLCD)
    #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE	_UxGT("Ekstruduj więcej")
  #else
    #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE	_UxGT("Ekstruduj wiecej")
  #endif
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #if ENABLED(DOGLCD)
    #define MSG_FILAMENT_CHANGE_OPTION_RESUME	_UxGT("Wznów drukowanie")
  #else
    #define MSG_FILAMENT_CHANGE_OPTION_RESUME	_UxGT("Wznow drukowanie")
  #endif
#endif

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1			_UxGT("Czekam na ")
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_INIT_2		_UxGT("zmianę filamentu")
    #else
      #define MSG_FILAMENT_CHANGE_INIT_2		_UxGT("zmiane filamentu")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1		_UxGT("Czekam na")
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_UNLOAD_2		_UxGT("wyjęcie filamentu")
    #else
      #define MSG_FILAMENT_CHANGE_UNLOAD_2		_UxGT("wyjecie filamentu")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_INSERT_1		_UxGT("Włóz filament")
    #else
      #define MSG_FILAMENT_CHANGE_INSERT_1		_UxGT("Wloz filament")
    #endif
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_INSERT_2		_UxGT("i naciśnij przycisk")
    #else
      #define MSG_FILAMENT_CHANGE_INSERT_2		_UxGT("i nacisnij przycisk")
    #endif
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_INSERT_3		_UxGT("aby kontynuować...")
    #else
      #define MSG_FILAMENT_CHANGE_INSERT_3		_UxGT("aby kontynuowac...")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1			_UxGT("Czekam na")
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_LOAD_2		_UxGT("włożenie filamentu")
    #else
      #define MSG_FILAMENT_CHANGE_LOAD_2		_UxGT("wlozenie filamentu")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1		_UxGT("Czekam na")
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2		_UxGT("ekstruzje filamentu")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1		_UxGT("Czekam na")
    #define MSG_FILAMENT_CHANGE_RESUME_2		_UxGT("wznowienie druku")
  #endif
  #else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_INIT_1		_UxGT("Proszę czekać...")
    #else
      #define MSG_FILAMENT_CHANGE_INIT_1		_UxGT("Prosze czekac...")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1		_UxGT("Wysuwanie...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_INSERT_1		_UxGT("Włóż i naciśnij prz.")
    #else
      #define MSG_FILAMENT_CHANGE_INSERT_1		_UxGT("Wloz i nacisnij prz.")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #if ENABLED(DOGLCD)
      #define MSG_FILAMENT_CHANGE_LOAD_1		_UxGT("Ładowanie...")
    #else
      #define MSG_FILAMENT_CHANGE_LOAD_1		_UxGT("Ladowanie...")
    #endif
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1		_UxGT("Ekstruzja...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1		_UxGT("Wznowienie...")
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_PL_H
