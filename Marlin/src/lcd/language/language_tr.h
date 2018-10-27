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

#define DISPLAY_CHARSET_ISO10646_TR
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" hazır.")                               // hazır.
#define MSG_SD_INSERTED                     _UxGT("SD Yerleşti.")                                       // SD Yerleşti.
#define MSG_SD_REMOVED                      _UxGT("SD Çıkarıldı.")                                      // SD Çıkarıldı.
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters                // Endstops
#define MSG_MAIN                            _UxGT("Ana")                                                // Ana
#define MSG_BACK                            _UxGT("Geri")                                               // Geri
#define MSG_AUTOSTART                       _UxGT("Otobaşlat")                                          // Otobaşlat
#define MSG_DISABLE_STEPPERS                _UxGT("Motorları Durdur")                                   // Motorları Durdur
#define MSG_DEBUG_MENU                      _UxGT("Hata Ayıklama")                                      // Hata Ayıklama
#define MSG_PROGRESS_BAR_TEST               _UxGT("Durum Çubuğu Testi")                                 // Durum Çubuğu Testi
#define MSG_AUTO_HOME                       _UxGT("Eksenleri Sıfırla")                                  // Eksenleri Sıfırla
#define MSG_AUTO_HOME_X                     _UxGT("X Sıfırla")                                          // X Sıfırla
#define MSG_AUTO_HOME_Y                     _UxGT("Y Sıfırla")                                          // Y Sıfırla
#define MSG_AUTO_HOME_Z                     _UxGT("Z Sıfırla")                                          // Z Sıfırla
#define MSG_LEVEL_BED_HOMING                _UxGT("XYZ Sıfırlanıyor")                                   // XYZ Sıfırlanıyor
#define MSG_LEVEL_BED_WAITING               _UxGT("Başlatmak için tıkla")                               // Başlatmak için tıkla
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Sıradaki Nokta")                                     // Sıradaki Nokta
#define MSG_LEVEL_BED_DONE                  _UxGT("Seviyeleme Tamam!")                                  // Seviyeleme Tamam!
#define MSG_SET_HOME_OFFSETS                _UxGT("Offset Ayarla")                                      // Offset Ayarla
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offset Tamam")                                       // Offset Tamam
#define MSG_SET_ORIGIN                      _UxGT("Sıfır Belirle")                                      // Sıfır Belirle
#define MSG_PREHEAT_1                       _UxGT("Ön Isınma PLA")                                      // Ön Isınma PLA
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")                                    //
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Tüm")                                 //  Tüm
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" Nozül")                               //  Nozül
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Tabla")                               //  Tabla
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" Ayar")                                //  Ayar
#define MSG_PREHEAT_2                       _UxGT("Ön Isınma ABS")                                      // Ön Isınma ABS
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")                                    //
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Tüm")                                 //  Tüm
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" Nozül")                               //  Nozül
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Tabla")                               //  Tabla
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" Ayar")                                //  Ayar
#define MSG_COOLDOWN                        _UxGT("Soğut")                                              // Soğut
#define MSG_SWITCH_PS_ON                    _UxGT("Gücü Aç")                                            // Gücü Aç
#define MSG_SWITCH_PS_OFF                   _UxGT("Gücü Kapat")                                         // Gücü Kapat
#define MSG_EXTRUDE                         _UxGT("Extrude")                                            // Extrude
#define MSG_RETRACT                         _UxGT("Geri Çek")                                           // Geri Çek
#define MSG_MOVE_AXIS                       _UxGT("Eksen Yönet")                                        // Eksenleri Yönet
#define MSG_BED_LEVELING                    _UxGT("Tabla Seviyele")                                     // Tabla Seviyele
#define MSG_LEVEL_BED                       _UxGT("Tabla Seviyele")                                     // Tabla Seviyele
#define MSG_MOVING                          _UxGT("Konumlanıyor...")                                    // Konumlanıyor...
#define MSG_FREE_XY                         _UxGT("Durdur XY")                                          // Durdur XY
#define MSG_MOVE_X                          _UxGT("X")                                                  // X
#define MSG_MOVE_Y                          _UxGT("Y")                                                  // Y
#define MSG_MOVE_Z                          _UxGT("Z")                                                  // Z
#define MSG_MOVE_E                          _UxGT("Ekstruder")                                          // Ekstruder
#define MSG_MOVE_01MM                       _UxGT("0.1mm")                                              // 0.1mm
#define MSG_MOVE_1MM                        _UxGT("1mm")                                                // 1mm
#define MSG_MOVE_10MM                       _UxGT("10mm")                                               // 10mm
#define MSG_SPEED                           _UxGT("Hız")                                                // Hız
#define MSG_BED_Z                           _UxGT("Z Mesafesi")                                         // Z Mesafesi
#define MSG_NOZZLE                          _UxGT("Nozül")                                              // Nozül
#define MSG_BED                             _UxGT("Tabla")                                              // Tabla
#define MSG_FAN_SPEED                       _UxGT("Fan Hızı")                                           // Fan Hızı
#define MSG_FLOW                            _UxGT("Akış")                                               // Akış
#define MSG_CONTROL                         _UxGT("Kontrol")                                            // Kontrol
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")                //  Min
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")                //  Max
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Çarpan")             //  Çarpan
#define MSG_AUTOTEMP                        _UxGT("Autotemp")                                           //  Autotemp
#define MSG_ON                              _UxGT("On ")                                                // On
#define MSG_OFF                             _UxGT("Off")                                                // Off
#define MSG_PID_P                           _UxGT("PID-P")                                              // PID-P
#define MSG_PID_I                           _UxGT("PID-I")                                              // PID-I
#define MSG_PID_D                           _UxGT("PID-D")                                              // PID-D
#define MSG_PID_C                           _UxGT("PID-C")                                              // PID-C
#define MSG_SELECT                          _UxGT("Seç")                                                // Seç
#define MSG_ACC                             _UxGT("İvme")                                               // İvme
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
#define MSG_TEMPERATURE                     _UxGT("Sıcaklık")                                           // Sıcaklık
#define MSG_MOTION                          _UxGT("Hareket")                                            // Hareket
#define MSG_FILAMENT                        _UxGT("Filaman")                                            // Filaman
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")                                           // E in mm3
#define MSG_FILAMENT_DIAM                   _UxGT("Fil. Çap")                                           // Fil. Çap
#define MSG_ADVANCE_K                       _UxGT("K İlerlet")                                          // K İlerlet
#define MSG_CONTRAST                        _UxGT("LCD Kontrast")                                       // LCD Kontrast
#define MSG_STORE_EEPROM                    _UxGT("Hafızaya Al")                                        // Hafızaya Al
#define MSG_LOAD_EEPROM                     _UxGT("Hafızadan Yükle")                                    // Hafızadan Yükle
#define MSG_RESTORE_FAILSAFE                _UxGT("Fabrika Ayarları")                                   // Fabrika Ayarları
#define MSG_REFRESH                         _UxGT("Yenile")                                             // Yenile
#define MSG_WATCH                           _UxGT("Bilgi Ekranı")                                       // Bilgi Ekranı
#define MSG_PREPARE                         _UxGT("Hazırlık")                                           // Hazırlık
#define MSG_TUNE                            _UxGT("Ayar")                                               // Ayar
#define MSG_PAUSE_PRINT                     _UxGT("Duraklat")                                           // Duraklat
#define MSG_RESUME_PRINT                    _UxGT("Sürdür")                                             // Sürdür
#define MSG_STOP_PRINT                      _UxGT("Durdur")                                             // Durdur
#define MSG_CARD_MENU                       _UxGT("SD den Yazdır")                                      // SD den Yazdır
#define MSG_NO_CARD                         _UxGT("SD Kart Yok")                                        // SD Kart Yok
#define MSG_DWELL                           _UxGT("Uyku...")                                            // Uyku...
#define MSG_USERWAIT                        _UxGT("Operatör bekleniyor...")                             // Operatör bekleniyor...
#define MSG_PRINT_ABORTED                   _UxGT("Baskı Durduruldu")                                   // Baskı Durduruldu
#define MSG_NO_MOVE                         _UxGT("İşlem yok.")                                         // İşlem yok.
#define MSG_KILLED                          _UxGT("Kilitlendi. ")                                       // Kilitlendi.
#define MSG_STOPPED                         _UxGT("Durdu. ")                                            // Durdu.
#define MSG_CONTROL_RETRACT                 _UxGT("Geri Çek mm")                                        // Geri Çek mm
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Re.mm")                                         // Swap Re.mm
#define MSG_CONTROL_RETRACTF                _UxGT("Geri Çekme  V")                                      // Geri Çekme V
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")                                             // Hop mm
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")                                           // UnRet mm
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")                                         // S UnRetmm
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")                                           // UnRet V
#define MSG_AUTORETRACT                     _UxGT("AutoRetr.")                                          // AutoRetr.
#define MSG_FILAMENTCHANGE                  _UxGT("Filaman Değiştir")                                   // Filaman Değiştir
#define MSG_INIT_SDCARD                     _UxGT("Init. SD")                                           // Init. SD
#define MSG_CNG_SDCARD                      _UxGT("SD Değiştir")                                        // SD Değiştir
#define MSG_ZPROBE_OUT                      _UxGT("Z Prob Açık. Tabla")                                 // Z Prob Açık. Tabla
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")                                  // BLTouch Self-Test
#define MSG_BLTOUCH_RESET                   _UxGT("Sıfırla BLTouch")                                    // Sıfırla BLTouch
#define MSG_HOME                            _UxGT("Sıfırla")                                            // Sıfırla
#define MSG_FIRST                           _UxGT("önce")                                               // Önce
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")                                           // Z Offset
#define MSG_BABYSTEP_X                      _UxGT("Miniadım X")                                         // Miniadım X
#define MSG_BABYSTEP_Y                      _UxGT("Miniadım Y")                                         // Miniadım Y
#define MSG_BABYSTEP_Z                      _UxGT("Miniadım Z")                                         // Miniadım Z
#define MSG_ENDSTOP_ABORT                   _UxGT("Endstop iptal")                                      // Endstop iptal
#define MSG_HEATING_FAILED_LCD              _UxGT("Isınma başarısız")                                   // Isınma başarısız
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Hata: Geçersiz Sıcaklık")                            // Hata: Geçersiz Sıcaklık
#define MSG_THERMAL_RUNAWAY                 _UxGT("TERMAL PROBLEM")                                     // TERMAL PROBLEM
#define MSG_ERR_MAXTEMP                     _UxGT("Hata: MAXSICAKLIK")                                  // Hata: MAXSICAKLIK
#define MSG_ERR_MINTEMP                     _UxGT("Hata: MINSICAKLIK")                                  // Hata: MINSICAKLIK
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Hata: MAXSIC. TABLA")                                // Hata: MAXSIC. TABLA
#define MSG_ERR_MINTEMP_BED                 _UxGT("Hata: MINSIC. TABLA")                                // Hata: MINSIC. TABLA
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("YAZICI DURDURULDU")                                  // YAZICI DURDURULDU
#define MSG_PLEASE_RESET                    _UxGT("Lütfen resetleyin")                                  // Lütfen resetleyin
#define MSG_SHORT_DAY                       _UxGT("G") // One character only                            // G
#define MSG_SHORT_HOUR                      _UxGT("S") // One character only                            // S
#define MSG_SHORT_MINUTE                    _UxGT("D") // One character only                            // D
#define MSG_HEATING                         _UxGT("Isınıyor...")                                        // Isınıyor...
#define MSG_BED_HEATING                     _UxGT("Tabla Isınıyor...")                                  // Tabla Isınıyor...
#define MSG_DELTA_CALIBRATE                 _UxGT("Delta Kalibrasyonu")                                 // Delta Kalibrasyonu
#define MSG_DELTA_CALIBRATE_X               _UxGT("Ayarla X")                                           // Ayarla X
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Ayarla Y")                                           // Ayarla Y
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Ayarla Z")                                           // Ayarla Z
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Ayarla Merkez")                                      // Ayarla Merkez
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Oto Kalibrasyon")                                    // Oto Kalibrasyon
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Delta Yük. Ayarla")                                  // Delta Yük. Ayarla
#define MSG_INFO_MENU                       _UxGT("Yazıcı Hakkında")                                    // Yazıcı Hakkında
#define MSG_INFO_PRINTER_MENU               _UxGT("Yazıcı Bilgisi")                                     // Yazıcı Bilgisi
#define MSG_INFO_STATS_MENU                 _UxGT("İstatistikler")                                      // İstatistikler
#define MSG_INFO_BOARD_MENU                 _UxGT("Kontrolör Bilgisi")                                  // Kontrol Bilgisi
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistörler")                                       // Termistörler
#define MSG_INFO_EXTRUDERS                  _UxGT("Ekstruderler")                                       // Ekstruderler
#define MSG_INFO_BAUDRATE                   _UxGT("İletişim Hızı")                                      // İletişim Hızı
#define MSG_INFO_PROTOCOL                   _UxGT("Protokol")                                           // Protokol
#define MSG_CASE_LIGHT                      _UxGT("Aydınlatmayı")                                       // Aydınlatmayı Aç

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Baskı Sayısı")                                       // Baskı Sayısı
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Tamamlanan")                                         // Tamamlanan
  #define MSG_INFO_PRINT_TIME               _UxGT("Toplam Baskı Süresi")                                // Toplam Baskı Süresi
  #define MSG_INFO_PRINT_LONGEST            _UxGT("En Uzun Baskı Süresi")                               // En Uzun Baskı Süresi
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Toplam Filaman")                                     // Toplam Filaman
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Baskı")                                              // Baskı
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Tamamlanan")                                         // Tamamlanan
  #define MSG_INFO_PRINT_TIME               _UxGT("Süre")                                               // Süre
  #define MSG_INFO_PRINT_LONGEST            _UxGT("En Uzun")                                            // En Uzun
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Filaman")                                            // Filaman
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Sıc.")                                           // Min Sıcak.
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Sıc.")                                           // Max Sıcak.
#define MSG_INFO_PSU                        _UxGT("Güç Kaynağı")                                        // Güç Kaynağı

#define MSG_DRIVE_STRENGTH                  _UxGT("Sürücü Gücü")                                        // Sürücü Gücü
#define MSG_DAC_PERCENT                     _UxGT("Sürücü %")                                           // Sürücü %
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("Seçenekler:")                                        // Seçenekler:
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Baskıyı sürdür")                                     // Baskıyı sürdür
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Nozül: ")                                          //   Nozül:

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Başlama bekleniyor")                               // Başlama bekleniyor
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("filamanın")                                        // filamanın
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("değişimi")                                         // değişimi
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Bekleniyor")                                       // Bekleniyor
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filamanın çıkması")                                // filamanın çıkması
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Filamanı yükle")                                   // Filamanı yükle
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("ve devam için")                                    // ve devam için
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("tuşa bas...")                                      // tuşa bas...
  #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Nozülü Isıtmak için")                              // Nozülü Isıtmak için
  #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("Butona Bas.")                                      // Butona Bas.
  #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Nozül Isınıyor")                                   // Nozül Isınıyor
  #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Lütfen Bekleyin...")                               // Lütfen Bekleyin...
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Bekleniyor")                                       // Bekleniyor
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filamanın yüklenmesi")                             // filamanın yüklenmesi
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Baskının sürdürülmesini")                          // Baskının sürdürülmesini
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("bekle")                                            // bekle
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Lütfen bekleyiniz...")                             // Lütfen bekleyiniz...
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Çıkartılıyor...")                                  // Çıkartılıyor...
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Yükle ve bas")                                     // Yükle ve bas
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Yüklüyor...")                                      // Yüklüyor...
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Sürdürülüyor...")                                  // Sürdürülüyor...
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_TR_H
