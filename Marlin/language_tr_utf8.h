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
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_TR_UTF_H
#define LANGUAGE_TR_UTF_H

//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_TR

#define WELCOME_MSG                         MACHINE_NAME " hazır."                                       //hazır.
#define MSG_SD_INSERTED                     "SD Yerleşti."                                               //SD Yerleşti.
#define MSG_SD_REMOVED                      "SD Çıkarıldı."                                              //SD Çıkarıldı.
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters                        //Endstops
#define MSG_MAIN                            "Ana"                                                        //Ana
#define MSG_AUTOSTART                       "Otobaşlat"                                                  //Otobaşlat
#define MSG_DISABLE_STEPPERS                "Motorları Durdur"                                           //Motorları Durdur
#define MSG_AUTO_HOME                       "Eksenleri Sıfırla"                                          //Eksenleri Sıfırla
#define MSG_AUTO_HOME_X                     "X Sıfırla"                                                  //X Sıfırla
#define MSG_AUTO_HOME_Y                     "Y Sıfırla"                                                  //Y Sıfırla
#define MSG_AUTO_HOME_Z                     "Z Sıfırla"                                                  //Z Sıfırla
#define MSG_LEVEL_BED_HOMING                "XYZ Sıfırlanıyor"                                           //XYZ Sıfırlanıyor
#define MSG_LEVEL_BED_WAITING               "Başlatmak için tıkla"                                       //Başlatmak için tıkla
#define MSG_LEVEL_BED_NEXT_POINT            "Sıradaki Nokta"                                             //Sıradaki Nokta
#define MSG_LEVEL_BED_DONE                  "Seviyeleme Tamam!"                                          //Seviyeleme Tamam!
#define MSG_LEVEL_BED_CANCEL                "İptal"                                                      //İptal
#define MSG_SET_HOME_OFFSETS                "Offset Ayarla"                                              //Offset Ayarla
#define MSG_HOME_OFFSETS_APPLIED            "Offset Tamam"                                               //Offset Tamam
#define MSG_SET_ORIGIN                      "Sıfır Belirle"                                              //Sıfır Belirle
#define MSG_PREHEAT_1                       "Ön Isınma PLA"                                              //Ön Isınma PLA
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "                                            // 
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " Tüm"                                         // Tüm
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Tabla"                                       // Tabla
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " Ayar"                                        // Ayar
#define MSG_PREHEAT_2                       "Ön Isınma ABS"                                              //Ön Isınma ABS
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "                                            // 
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " Tüm"                                         // Tüm
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Tabla"                                       // Tabla
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " Ayar"                                        // Ayar
#define MSG_COOLDOWN                        "Soğut"                                                      //Soğut
#define MSG_SWITCH_PS_ON                    "Gücü Aç"                                                    //Gücü Aç
#define MSG_SWITCH_PS_OFF                   "Gücü Kapat"                                                 //Gücü Kapat
#define MSG_EXTRUDE                         "Extrude"                                                    //Extrude
#define MSG_RETRACT                         "Geri Çek"                                                   //Geri Çek
#define MSG_MOVE_AXIS                       "Eksen Yönet"                                                //Eksenleri Yönet
#define MSG_LEVEL_BED                       "Tabla Seviyele"                                             //Tabla Seviyele
#define MSG_MOVE_X                          "X"                                                          //X
#define MSG_MOVE_Y                          "Y"                                                          //Y
#define MSG_MOVE_Z                          "Z"                                                          //Z
#define MSG_MOVE_E                          "Ekstruder"                                                  //Ekstruder
#define MSG_MOVE_01MM                       "0.1mm"                                                      //0.1mm
#define MSG_MOVE_1MM                        "1mm"                                                        //1mm
#define MSG_MOVE_10MM                       "10mm"                                                       //10mm
#define MSG_SPEED                           "Hız"                                                        //Hız
#define MSG_BED_Z                           "Tabla Z"                                                    //Tabla Z
#define MSG_NOZZLE                          "Nozül"                                                      //Nozül
#define MSG_BED                             "Tabla"                                                      //Tabla
#define MSG_FAN_SPEED                       "Fan Hızı"                                                   //Fan Hızı
#define MSG_FLOW                            "Akış"                                                       //Akış
#define MSG_CONTROL                         "Kontrol"                                                    //Kontrol
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"                               // Min
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"                               // Max
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Çarpan"                            // Çarpan
#define MSG_AUTOTEMP                        "Autotemp"                                                   // Autotemp
#define MSG_ON                              "On "                                                        //On 
#define MSG_OFF                             "Off"                                                        //Off
#define MSG_PID_P                           "PID-P"                                                      //PID-P
#define MSG_PID_I                           "PID-I"                                                      //PID-I
#define MSG_PID_D                           "PID-D"                                                      //PID-D
#define MSG_PID_C                           "PID-C"                                                      //PID-C
#define MSG_SELECT                          "Seç"                                                        //Seç
#define MSG_ACC                             "İvme"                                                       //İvme
#define MSG_VX_JERK                         "Vx-Jerk"                                                    //Vx-Jerk
#define MSG_VY_JERK                         "Vy-Jerk"                                                    //Vy-Jerk
#define MSG_VZ_JERK                         "Vz-jerk"                                                    //Vz-Jerk
#define MSG_VE_JERK                         "Ve-jerk"                                                    //Ve-Jerk
#define MSG_VMAX                            "Vmax "                                                      //Vmax
#define MSG_VMIN                            "Vmin"                                                       //Vmin
#define MSG_VTRAV_MIN                       "VTrav min"                                                  //Vtrav min
#define MSG_AMAX                            "Amax "                                                      //Amax
#define MSG_A_RETRACT                       "A-retract"                                                  //A-retract
#define MSG_A_TRAVEL                        "A-travel"                                                   //A-travel
#define MSG_XSTEPS                          "Xsteps/mm"                                                  //Xsteps/mm
#define MSG_YSTEPS                          "Ysteps/mm"                                                  //Ysteps/mm
#define MSG_ZSTEPS                          "Zsteps/mm"                                                  //Zsteps/mm
#define MSG_ESTEPS                          "Esteps/mm"                                                  //Esteps/mm
#define MSG_TEMPERATURE                     "Sıcaklık"                                                   //Sıcaklık
#define MSG_MOTION                          "Hareket"                                                    //Hareket
#define MSG_VOLUMETRIC                      "Filaman"                                                    //Filaman
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"                                                   //E in mm3
#define MSG_FILAMENT_DIAM                   "Fil. Çap"                                                   //Fil. Çap
#define MSG_CONTRAST                        "LCD Kontrast"                                               //LCD Kontrast
#define MSG_STORE_EPROM                     "Hafızaya Al"                                                //Hafızaya Al
#define MSG_LOAD_EPROM                      "Hafızadan Yükle"                                            //Hafızadan Yükle
#define MSG_RESTORE_FAILSAFE                "Fabrika Ayarları"                                           //Fabrika Ayarları
#define MSG_REFRESH                         "Yenile"                                                     //Yenile
#define MSG_WATCH                           "Bilgi Ekranı"                                               //Bilgi Ekranı
#define MSG_PREPARE                         "Hazırlık"                                                   //Hazırlık
#define MSG_TUNE                            "Ayar"                                                       //Ayar
#define MSG_PAUSE_PRINT                     "Duraklat"                                                   //Duraklat
#define MSG_RESUME_PRINT                    "Sürdür"                                                     //Sürdür
#define MSG_STOP_PRINT                      "Durdur"                                                     //Durdur
#define MSG_CARD_MENU                       "SD den Yazdır"                                              //SD den Yazdır
#define MSG_NO_CARD                         "SD Kart Yok"                                                //SD Kart Yok
#define MSG_DWELL                           "Uyku..."                                                    //Uyku...
#define MSG_USERWAIT                        "Operatör bekleniyor..."                                     //Operatör bekleniyor...
#define MSG_RESUMING                        "Baskı Sürdürülüyor"                                         //Baskı Sürdürülüyor
#define MSG_PRINT_ABORTED                   "Baskı Durduruldu"                                           //Baskı Durduruldu
#define MSG_NO_MOVE                         "İşlem yok."                                                 //İşlem yok.
#define MSG_KILLED                          "Kilitlendi. "                                               //Kilitlendi.
#define MSG_STOPPED                         "Durdu. "                                                    //Durdu.
#define MSG_CONTROL_RETRACT                 "Geri Çek mm"                                                //Geri Çek mm
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"                                                 //Swap Re.mm
#define MSG_CONTROL_RETRACTF                "Geri Çekme  V"                                              //Geri Çekme V
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"                                                     //Hop mm
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"                                                  //UnRet +mm
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"                                                 //S UnRet+mm
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"                                                   //UnRet V
#define MSG_AUTORETRACT                     "AutoRetr."                                                  //AutoRetr.
#define MSG_FILAMENTCHANGE                  "Filaman Değiştir"                                           //Filaman Değiştir
#define MSG_INIT_SDCARD                     "Init. SD"                                                   //Init. SD
#define MSG_CNG_SDCARD                      "SD Değiştir"                                                //SD Değiştir
#define MSG_ZPROBE_OUT                      "Z Prob Açık. Tabla"                                         //Z Prob Açık. Tabla
#define MSG_BLTOUCH_SELFTEST                "BLTouch Self-Test"                                          //BLTouch Self-Test
#define MSG_BLTOUCH_RESET                   "Sıfırla BLTouch"                                            //Sıfırla BLTouch
#define MSG_HOME                            "Sıfırla"                                                    //Sıfırla
#define MSG_FIRST                           "önce"                                                       //Önce
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"                                                   //Z Offset
#define MSG_BABYSTEP_X                      "Miniadım X"                                                 //Miniadım X
#define MSG_BABYSTEP_Y                      "Miniadım Y"                                                 //Miniadım Y
#define MSG_BABYSTEP_Z                      "Miniadım Z"                                                 //Miniadım Z
#define MSG_ENDSTOP_ABORT                   "Endstop iptal"                                              //Endstop iptal
#define MSG_HEATING_FAILED_LCD              "Isınma başarısız"                                           //Isınma başarısız
#define MSG_ERR_REDUNDANT_TEMP              "Hata: Geçersiz Sıcaklık"                                    //Hata: Geçersiz Sıcaklık
#define MSG_THERMAL_RUNAWAY                 "TERMAL PROBLEM"                                             //TERMAL PROBLEM
#define MSG_ERR_MAXTEMP                     "Hata: MAXSICAKLIK"                                          //Hata: MAXSICAKLIK
#define MSG_ERR_MINTEMP                     "Hata: MINSICAKLIK"                                          //Hata: MINSICAKLIK
#define MSG_ERR_MAXTEMP_BED                 "Hata: MAXSIC. TABLA"                                        //Hata: MAXSIC. TABLA
#define MSG_ERR_MINTEMP_BED                 "Hata: MINSIC. TABLA"                                        //Hata: MINSIC. TABLA
#define MSG_ERR_Z_HOMING                    "G28 Z Yapılamaz"                                            //G28 Z Yapılamaz
#define MSG_HALTED                          "YAZICI DURDURULDU"                                          //YAZICI DURDURULDU
#define MSG_PLEASE_RESET                    "Lütfen resetleyin"                                          //Lütfen resetleyin
#define MSG_SHORT_DAY                       "G" // One character only                                    //G
#define MSG_SHORT_HOUR                      "S" // One character only                                    //S
#define MSG_SHORT_MINUTE                    "D" // One character only                                    //D
#define MSG_HEATING                         "Isınıyor..."                                                //Isınıyor...
#define MSG_HEATING_COMPLETE                "Isınma tamam."                                              //Isınma tamam.
#define MSG_BED_HEATING                     "Tabla Isınıyor."                                            //Tabla Isınıyor.
#define MSG_BED_DONE                        "Tabla hazır."                                               //Tabla hazır.
#define MSG_DELTA_CALIBRATE                 "Delta Kalibrasyonu"                                         //Delta Kalibrasyonu
#define MSG_DELTA_CALIBRATE_X               "Ayarla X"                                                   //Ayarla X
#define MSG_DELTA_CALIBRATE_Y               "Ayarla Y"                                                   //Ayarla Y
#define MSG_DELTA_CALIBRATE_Z               "Ayarla Z"                                                   //Ayarla Z
#define MSG_DELTA_CALIBRATE_CENTER          "Ayarla Merkez"                                              //Ayarla Merkez

#define MSG_INFO_MENU                       "Yazıcı Hakkında"                                            //Yazıcı Hakkında
#define MSG_INFO_PRINTER_MENU               "Yazıcı Bilgisi"                                             //Yazıcı Bilgisi
#define MSG_INFO_STATS_MENU                 "İstatistikler"                                              //İstatistikler
#define MSG_INFO_BOARD_MENU                 "Kontrolör Bilgisi"                                          //Kontrol Bilgisi
#define MSG_INFO_THERMISTOR_MENU            "Termistörler"                                               //Termistörler
#define MSG_INFO_EXTRUDERS                  "Ekstruderler"                                               //Ekstruderler
#define MSG_INFO_BAUDRATE                   "İletişim Hızı"                                              //İletişim Hızı
#define MSG_INFO_PROTOCOL                   "Protokol"                                                   //Protokol
#define MSG_LIGHTS_ON                       "Aydınlatmayı Aç"                                            //Aydınlatmayı Aç
#define MSG_LIGHTS_OFF                      "Aydınlatmayı Kapa"                                          //Aydınlaymayı Kapa

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              "Baskı Sayısı"                                               //Baskı Sayısı
  #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"                                                 //Tamamlanan
  #define MSG_INFO_PRINT_TIME               "Toplam Baskı Süresi"                                        //Toplam Baskı Süresi
  #define MSG_INFO_PRINT_LONGEST            "En Uzun Baskı Süresi"                                       //En Uzun Baskı Süresi
  #define MSG_INFO_PRINT_FILAMENT           "Toplam Filaman"                                             //Toplam Filaman
#else
  #define MSG_INFO_PRINT_COUNT              "Baskı"                                                      //Baskı
  #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"                                                 //Tamamlanan
  #define MSG_INFO_PRINT_TIME               "Süre"                                                       //Süre
  #define MSG_INFO_PRINT_LONGEST            "En Uzun"                                                    //En Uzun
  #define MSG_INFO_PRINT_FILAMENT           "Filaman"                                                    //Filaman
#endif

#define MSG_INFO_MIN_TEMP                   "Min Sıc."                                                   //Min Sıcak.
#define MSG_INFO_MAX_TEMP                   "Max Sıc."                                                   //Max Sıcak.
#define MSG_INFO_PSU                        "Güç Kaynağı"                                                //Güç Kaynağı

#define MSG_DRIVE_STRENGTH                  "Sürücü Gücü"                                                //Sürücü Gücü
#define MSG_DAC_PERCENT                     "Sürücü %"                                                   //Sürücü %
#define MSG_DAC_EEPROM_WRITE                "DAC'ı EEPROM'a Yaz"                                         //DAC'ı EEPROM'a Yaz
#define MSG_FILAMENT_CHANGE_HEADER          "Filaman Değiştir"                                           //Filaman Değiştir
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "Seçenekler:"                                                //Seçenekler:
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Daha Akıt"                                                  //Daha Akıt
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Baskıyı sürdür"                                             //Baskıyı sürdür

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Başlama bekleniyor"                                       //Başlama bekleniyor
  #define MSG_FILAMENT_CHANGE_INIT_2          "filamanın"                                                //filamanın
  #define MSG_FILAMENT_CHANGE_INIT_3          "değişimi"                                                 //değişimi
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Bekleniyor"                                               //Bekleniyor
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filamanın çıkması"                                        //filamanın çıkması
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Filamanı yükle"                                           //Filamanı yükle
  #define MSG_FILAMENT_CHANGE_INSERT_2        "ve devam için"                                            //ve devam için
  #define MSG_FILAMENT_CHANGE_INSERT_3        "tuşa bas..."                                              //tuşa bas...
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Bekleniyor"                                               //Bekleniyor
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filamanın yüklenmesi"                                     //filamanın yüklenmesi
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Bekleniyor"                                               //Bekleniyor
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filaman akması"                                           //filaman akması
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Baskının sürdürülmesini"                                  //Baskının sürdürülmesini
  #define MSG_FILAMENT_CHANGE_RESUME_2        "bekle"                                                    //bekle
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Lütfen bekleyiniz..."                                     //Lütfen bekleyiniz...
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Çıkartılıyor..."                                          //Çıkartılıyor...
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Yükle ve bas"                                             //Yükle ve bas
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Yüklüyor..."                                              //Yüklüyor...
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Akıtılıyor..."                                            //Akıtılıyor...
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Sürdürülüyor..."                                          //Sürdürülüyor...
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_TR_UTF_H
