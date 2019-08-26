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
 * Turkish
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_TR_H
#define LANGUAGE_TR_H

#define MAPPER_C2C3_TR
#define DISPLAY_CHARSET_ISO10646_TR
#define CHARSIZE 2

#if DISABLED(DOGLCD)
  #error "Turkish needs a graphical display."
#endif

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" hazД±r.")                               // hazД±r.
#define MSG_SD_INSERTED                     _UxGT("SD YerleЕџti.")                                       // SD YerleЕџti.
#define MSG_SD_REMOVED                      _UxGT("SD Г‡Д±karД±ldД±.")                                      // SD Г‡Д±karД±ldД±.
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters                // Endstops
#define MSG_MAIN                            _UxGT("Ana")                                                // Ana
#define MSG_BACK                            _UxGT("Geri")                                               // Geri
#define MSG_AUTOSTART                       _UxGT("OtobaЕџlat")                                          // OtobaЕџlat
#define MSG_DISABLE_STEPPERS                _UxGT("MotorlarД± Durdur")                                   // MotorlarД± Durdur
#define MSG_DEBUG_MENU                      _UxGT("Hata AyД±klama")                                      // Hata AyД±klama
#define MSG_PROGRESS_BAR_TEST               _UxGT("Durum Г‡ubuДџu Testi")                                 // Durum Г‡ubuДџu Testi
#define MSG_AUTO_HOME                       _UxGT("Eksenleri SД±fД±rla")                                  // Eksenleri SД±fД±rla
#define MSG_AUTO_HOME_X                     _UxGT("X SД±fД±rla")                                          // X SД±fД±rla
#define MSG_AUTO_HOME_Y                     _UxGT("Y SД±fД±rla")                                          // Y SД±fД±rla
#define MSG_AUTO_HOME_Z                     _UxGT("Z SД±fД±rla")                                          // Z SД±fД±rla
#define MSG_TMC_Z_CALIBRATION               _UxGT("Ayarla Z")                                           // Ayarla Z
#define MSG_LEVEL_BED_HOMING                _UxGT("XYZ SД±fД±rlanД±yor")                                   // XYZ SД±fД±rlanД±yor
#define MSG_LEVEL_BED_WAITING               _UxGT("BaЕџlatmak iГ§in tД±kla")                               // BaЕџlatmak iГ§in tД±kla
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("SД±radaki Nokta")                                     // SД±radaki Nokta
#define MSG_LEVEL_BED_DONE                  _UxGT("Seviyeleme Tamam!")                                  // Seviyeleme Tamam!
#define MSG_SET_HOME_OFFSETS                _UxGT("Offset Ayarla")                                      // Offset Ayarla
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offset Tamam")                                       // Offset Tamam
#define MSG_SET_ORIGIN                      _UxGT("SД±fД±r Belirle")                                      // SД±fД±r Belirle
#define MSG_PREHEAT_1                       _UxGT("Г–n IsД±nma PLA")                                      // Г–n IsД±nma PLA
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")                                    //
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" TГјm")                                 //  TГјm
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" NozГјl")                               //  NozГјl
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Tabla")                               //  Tabla
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Ayar")                                //  Ayar
#define MSG_PREHEAT_2                       _UxGT("Г–n IsД±nma ABS")                                      // Г–n IsД±nma ABS
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")                                    //
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" TГјm")                                 //  TГјm
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" NozГјl")                               //  NozГјl
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Tabla")                               //  Tabla
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Ayar")                                //  Ayar
#define MSG_COOLDOWN                        _UxGT("SoДџut")                                              // SoДџut
#define MSG_SWITCH_PS_ON                    _UxGT("GГјcГј AГ§")                                            // GГјcГј AГ§
#define MSG_SWITCH_PS_OFF                   _UxGT("GГјcГј Kapat")                                         // GГјcГј Kapat
#define MSG_EXTRUDE                         _UxGT("Extrude")                                            // Extrude
#define MSG_RETRACT                         _UxGT("Geri Г‡ek")                                           // Geri Г‡ek
#define MSG_MOVE_AXIS                       _UxGT("Eksen YГ¶net")                                        // Eksenleri YГ¶net
#define MSG_BED_LEVELING                    _UxGT("Tabla Seviyele")                                     // Tabla Seviyele
#define MSG_LEVEL_BED                       _UxGT("Tabla Seviyele")                                     // Tabla Seviyele
#define MSG_MOVING                          _UxGT("KonumlanД±yor...")                                    // KonumlanД±yor...
#define MSG_FREE_XY                         _UxGT("Durdur XY")                                          // Durdur XY
#define MSG_MOVE_X                          _UxGT("X")                                                  // X
#define MSG_MOVE_Y                          _UxGT("Y")                                                  // Y
#define MSG_MOVE_Z                          _UxGT("Z")                                                  // Z
#define MSG_MOVE_E                          _UxGT("Ekstruder")                                          // Ekstruder
#define MSG_MOVE_01MM                       _UxGT("0.1mm")                                              // 0.1mm
#define MSG_MOVE_1MM                        _UxGT("1mm")                                                // 1mm
#define MSG_MOVE_10MM                       _UxGT("10mm")                                               // 10mm
#define MSG_SPEED                           _UxGT("HД±z")                                                // HД±z
#define MSG_BED_Z                           _UxGT("Z Mesafesi")                                         // Z Mesafesi
#define MSG_NOZZLE                          _UxGT("NozГјl")                                              // NozГјl
#define MSG_BED                             _UxGT("Tabla")                                              // Tabla
#define MSG_FAN_SPEED                       _UxGT("Fan HД±zД±")                                           // Fan HД±zД±
#define MSG_FLOW                            _UxGT("AkД±Еџ")                                               // AkД±Еџ
#define MSG_CONTROL                         _UxGT("Kontrol")                                            // Kontrol
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")                //  Min
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")                //  Max
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Г‡arpan")             //  Г‡arpan
#define MSG_AUTOTEMP                        _UxGT("Autotemp")                                           //  Autotemp
#define MSG_ON                              _UxGT("On ")                                                // On
#define MSG_OFF                             _UxGT("Off")                                                // Off
#define MSG_PID_P                           _UxGT("PID-P")                                              // PID-P
#define MSG_PID_I                           _UxGT("PID-I")                                              // PID-I
#define MSG_PID_D                           _UxGT("PID-D")                                              // PID-D
#define MSG_PID_C                           _UxGT("PID-C")                                              // PID-C
#define MSG_SELECT                          _UxGT("SeГ§")                                                // SeГ§
#define MSG_ACC                             _UxGT("Д°vme")                                               // Д°vme
#define MSG_JERK                            _UxGT("Jerk")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Va-jerk")
  #define MSG_VB_JERK                       _UxGT("Vb-jerk")
  #define MSG_VC_JERK                       _UxGT("Vc-jerk")
#else
  #define MSG_VA_JERK                       _UxGT("Vx-jerk")
  #define MSG_VB_JERK                       _UxGT("Vy-jerk")
  #define MSG_VC_JERK                       _UxGT("Vz-jerk")
#endif
#define MSG_VE_JERK                         _UxGT("Ve-jerk")                                            // Ve-Jerk
#define MSG_VMAX                            _UxGT("Vmax ")                                              // Vmax
#define MSG_VMIN                            _UxGT("Vmin")                                               // Vmin
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")                                          // Vtrav min
#define MSG_AMAX                            _UxGT("Amax ")                                              // Amax
#define MSG_A_RETRACT                       _UxGT("A-retract")                                          // A-retract
#define MSG_A_TRAVEL                        _UxGT("A-travel")                                           // A-travel
#define MSG_STEPS_PER_MM                    _UxGT("Steps/mm")                                           // Steps/mm
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("Asteps/mm")
  #define MSG_BSTEPS                        _UxGT("Bsteps/mm")
  #define MSG_CSTEPS                        _UxGT("Csteps/mm")
#else
  #define MSG_ASTEPS                        _UxGT("Xsteps/mm")
  #define MSG_BSTEPS                        _UxGT("Ysteps/mm")
  #define MSG_CSTEPS                        _UxGT("Zsteps/mm")
#endif
#define MSG_ESTEPS                          _UxGT("Esteps/mm")                                          // Esteps/mm
#define MSG_E1STEPS                         _UxGT("E1steps/mm")                                         // E1steps/mm
#define MSG_E2STEPS                         _UxGT("E2steps/mm")                                         // E2steps/mm
#define MSG_E3STEPS                         _UxGT("E3steps/mm")                                         // E3steps/mm
#define MSG_E4STEPS                         _UxGT("E4steps/mm")                                         // E4steps/mm
#define MSG_E5STEPS                         _UxGT("E5steps/mm")                                         // E4steps/mm
#define MSG_TEMPERATURE                     _UxGT("SД±caklД±k")                                           // SД±caklД±k
#define MSG_MOTION                          _UxGT("Hareket")                                            // Hareket
#define MSG_FILAMENT                        _UxGT("Filaman")                                            // Filaman
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")                                           // E in mm3
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Г‡ap")                                           // Fil. Г‡ap
#define MSG_ADVANCE_K                       _UxGT("K Д°lerlet")                                          // K Д°lerlet
#define MSG_CONTRAST                        _UxGT("LCD Kontrast")                                       // LCD Kontrast
#define MSG_STORE_EEPROM                    _UxGT("HafД±zaya Al")                                        // HafД±zaya Al
#define MSG_LOAD_EEPROM                     _UxGT("HafД±zadan YГјkle")                                    // HafД±zadan YГјkle
#define MSG_RESTORE_FAILSAFE                _UxGT("Fabrika AyarlarД±")                                   // Fabrika AyarlarД±
#define MSG_REFRESH                         _UxGT("Yenile")                                             // Yenile
#define MSG_WATCH                           _UxGT("Bilgi EkranД±")                                       // Bilgi EkranД±
#define MSG_PREPARE                         _UxGT("HazД±rlД±k")                                           // HazД±rlД±k
#define MSG_TUNE                            _UxGT("Ayar")                                               // Ayar
#define MSG_PAUSE_PRINT                     _UxGT("Duraklat")                                           // Duraklat
#define MSG_RESUME_PRINT                    _UxGT("SГјrdГјr")                                             // SГјrdГјr
#define MSG_STOP_PRINT                      _UxGT("Durdur")                                             // Durdur
#define MSG_CARD_MENU                       _UxGT("SD den YazdД±r")                                      // SD den YazdД±r
#define MSG_NO_CARD                         _UxGT("SD Kart Yok")                                        // SD Kart Yok
#define MSG_DWELL                           _UxGT("Uyku...")                                            // Uyku...
#define MSG_USERWAIT                        _UxGT("OperatГ¶r bekleniyor...")                             // OperatГ¶r bekleniyor...
#define MSG_PRINT_ABORTED                   _UxGT("BaskД± Durduruldu")                                   // BaskД± Durduruldu
#define MSG_NO_MOVE                         _UxGT("Д°Еџlem yok.")                                         // Д°Еџlem yok.
#define MSG_KILLED                          _UxGT("Kilitlendi. ")                                       // Kilitlendi.
#define MSG_STOPPED                         _UxGT("Durdu. ")                                            // Durdu.
#define MSG_CONTROL_RETRACT                 _UxGT("Geri Г‡ek mm")                                        // Geri Г‡ek mm
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Re.mm")                                         // Swap Re.mm
#define MSG_CONTROL_RETRACTF                _UxGT("Geri Г‡ekme  V")                                      // Geri Г‡ekme V
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")                                             // Hop mm
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")                                           // UnRet mm
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")                                         // S UnRetmm
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")                                           // UnRet V
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")                                          // AutoRetr.
#define MSG_FILAMENTCHANGE                  _UxGT("Filaman DeДџiЕџtir")                                   // Filaman DeДџiЕџtir
#define MSG_INIT_SDCARD                     _UxGT("Init. SD")                                           // Init. SD
#define MSG_CNG_SDCARD                      _UxGT("SD DeДџiЕџtir")                                        // SD DeДџiЕџtir
#define MSG_ZPROBE_OUT                      _UxGT("Z Prob AГ§Д±k. Tabla")                                 // Z Prob AГ§Д±k. Tabla
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")                                  // BLTouch Self-Test
#define MSG_BLTOUCH_RESET                   _UxGT("SД±fД±rla BLTouch")                                    // SД±fД±rla BLTouch
#define MSG_HOME                            _UxGT("SД±fД±rla")                                            // SД±fД±rla
#define MSG_FIRST                           _UxGT("Г¶nce")                                               // Г–nce
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")                                           // Z Offset
#define MSG_BABYSTEP_X                      _UxGT("MiniadД±m X")                                         // MiniadД±m X
#define MSG_BABYSTEP_Y                      _UxGT("MiniadД±m Y")                                         // MiniadД±m Y
#define MSG_BABYSTEP_Z                      _UxGT("MiniadД±m Z")                                         // MiniadД±m Z
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop iptal")                                      // Endstop iptal
#define MSG_HEATING_FAILED_LCD              _UxGT("IsД±nma baЕџarД±sД±z")                                   // IsД±nma baЕџarД±sД±z
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Hata: GeГ§ersiz SД±caklД±k")                            // Hata: GeГ§ersiz SД±caklД±k
#define MSG_THERMAL_RUNAWAY                 _UxGT("TERMAL PROBLEM")                                     // TERMAL PROBLEM
#define MSG_ERR_MAXTEMP                     _UxGT("Hata: MAXSICAKLIK")                                  // Hata: MAXSICAKLIK
#define MSG_ERR_MINTEMP                     _UxGT("Hata: MINSICAKLIK")                                  // Hata: MINSICAKLIK
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Hata: MAXSIC. TABLA")                                // Hata: MAXSIC. TABLA
#define MSG_ERR_MINTEMP_BED                 _UxGT("Hata: MINSIC. TABLA")                                // Hata: MINSIC. TABLA
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("YAZICI DURDURULDU")                                  // YAZICI DURDURULDU
#define MSG_PLEASE_RESET                    _UxGT("LГјtfen resetleyin")                                  // LГјtfen resetleyin
#define MSG_SHORT_DAY                       _UxGT("G") // One character only                            // G
#define MSG_SHORT_HOUR                      _UxGT("S") // One character only                            // S
#define MSG_SHORT_MINUTE                    _UxGT("D") // One character only                            // D
#define MSG_HEATING                         _UxGT("IsД±nД±yor...")                                        // IsД±nД±yor...
#define MSG_BED_HEATING                     _UxGT("Tabla IsД±nД±yor...")                                  // Tabla IsД±nД±yor...
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibrasyonu")                                 // Delta Kalibrasyonu
#define MSG_DELTA_CALIBRATE_X               _UxGT("Ayarla X")                                           // Ayarla X
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Ayarla Y")                                           // Ayarla Y
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Ayarla Z")                                           // Ayarla Z
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Ayarla Merkez")                                      // Ayarla Merkez
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Oto Kalibrasyon")                                    // Oto Kalibrasyon
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Delta YГјk. Ayarla")                                  // Delta YГјk. Ayarla
#define MSG_INFO_MENU                       _UxGT("YazД±cД± HakkД±nda")                                    // YazД±cД± HakkД±nda
#define MSG_INFO_PRINTER_MENU               _UxGT("YazД±cД± Bilgisi")                                     // YazД±cД± Bilgisi
#define MSG_INFO_STATS_MENU                 _UxGT("Д°statistikler")                                      // Д°statistikler
#define MSG_INFO_BOARD_MENU                 _UxGT("KontrolГ¶r Bilgisi")                                  // Kontrol Bilgisi
#define MSG_INFO_THERMISTOR_MENU            _UxGT("TermistГ¶rler")                                       // TermistГ¶rler
#define MSG_INFO_EXTRUDERS                  _UxGT("Ekstruderler")                                       // Ekstruderler
#define MSG_INFO_BAUDRATE                   _UxGT("Д°letiЕџim HД±zД±")                                      // Д°letiЕџim HД±zД±
#define MSG_INFO_PROTOCOL                   _UxGT("Protokol")                                           // Protokol
#define MSG_CASE_LIGHT                      _UxGT("AydД±nlatmayД±")                                       // AydД±nlatmayД± AГ§

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("BaskД± SayД±sД±")                                       // BaskД± SayД±sД±
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Tamamlanan")                                         // Tamamlanan
  #define MSG_INFO_PRINT_TIME               _UxGT("Toplam BaskД± SГјresi")                                // Toplam BaskД± SГјresi
  #define MSG_INFO_PRINT_LONGEST            _UxGT("En Uzun BaskД± SГјresi")                               // En Uzun BaskД± SГјresi
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Toplam Filaman")                                     // Toplam Filaman
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("BaskД±")                                              // BaskД±
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Tamamlanan")                                         // Tamamlanan
  #define MSG_INFO_PRINT_TIME               _UxGT("SГјre")                                               // SГјre
  #define MSG_INFO_PRINT_LONGEST            _UxGT("En Uzun")                                            // En Uzun
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Filaman")                                            // Filaman
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min SД±c.")                                           // Min SД±cak.
#define MSG_INFO_MAX_TEMP                   _UxGT("Max SД±c.")                                           // Max SД±cak.
#define MSG_INFO_PSU                        _UxGT("GГјГ§ KaynaДџД±")                                        // GГјГ§ KaynaДџД±

#define MSG_DRIVE_STRENGTH                  _UxGT("SГјrГјcГј GГјcГј")                                        // SГјrГјcГј GГјcГј
#define MSG_DAC_PERCENT                     _UxGT("SГјrГјcГј %")                                           // SГјrГјcГј %
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("SeГ§enekler:")                                        // SeГ§enekler:
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("BaskД±yД± sГјrdГјr")                                     // BaskД±yД± sГјrdГјr
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  NozГјl: ")                                          //   NozГјl:

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("BaЕџlama bekleniyor")                               // BaЕџlama bekleniyor
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("filamanД±n")                                        // filamanД±n
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("deДџiЕџimi")                                         // deДџiЕџimi
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Bekleniyor")                                       // Bekleniyor
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filamanД±n Г§Д±kmasД±")                                // filamanД±n Г§Д±kmasД±
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("FilamanД± yГјkle")                                   // FilamanД± yГјkle
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("ve devam iГ§in")                                    // ve devam iГ§in
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("tuЕџa bas...")                                      // tuЕџa bas...
  #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("NozГјlГј IsД±tmak iГ§in")                              // NozГјlГј IsД±tmak iГ§in
  #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("Butona Bas.")                                      // Butona Bas.
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("NozГјl IsД±nД±yor")                                   // NozГјl IsД±nД±yor
  #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("LГјtfen Bekleyin...")                               // LГјtfen Bekleyin...
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Bekleniyor")                                       // Bekleniyor
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filamanД±n yГјklenmesi")                             // filamanД±n yГјklenmesi
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("BaskД±nД±n sГјrdГјrГјlmesini")                          // BaskД±nД±n sГјrdГјrГјlmesini
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("bekle")                                            // bekle
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("LГјtfen bekleyiniz...")                             // LГјtfen bekleyiniz...
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Г‡Д±kartД±lД±yor...")                                  // Г‡Д±kartД±lД±yor...
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("YГјkle ve bas")                                     // YГјkle ve bas
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("YГјklГјyor...")                                      // YГјklГјyor...
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("SГјrdГјrГјlГјyor...")                                  // SГјrdГјrГјlГјyor...
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_TR_H

