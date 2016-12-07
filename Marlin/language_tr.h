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
#ifndef LANGUAGE_TR_H
#define LANGUAGE_TR_H

//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_TR

#define WELCOME_MSG                         MACHINE_NAME " haz\xfdr."                                    //hazır.
#define MSG_SD_INSERTED                     "SD Yerle\xfeti."                                            //SD Yerleşti.
#define MSG_SD_REMOVED                      "SD \xc7\xfdkar\xfdld\xfd."                                  //SD Çıkarıldı.
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters                        //Endstops
#define MSG_MAIN                            "Ana"                                                        //Ana
#define MSG_AUTOSTART                       "Otoba\xfelat"                                               //Otobaşlat
#define MSG_DISABLE_STEPPERS                "Motorlar\xfd Durdur"                                        //Motorları Durdur
#define MSG_AUTO_HOME                       "Eksenleri S\xfd\x66\xfdrla"                                 //Eksenleri Sıfırla
#define MSG_AUTO_HOME_X                     "X S\xfd\x66\xfdrla"                                         //X Sıfırla
#define MSG_AUTO_HOME_Y                     "Y S\xfd\x66\xfdrla"                                         //Y Sıfırla
#define MSG_AUTO_HOME_Z                     "Z S\xfd\x66\xfdrla"                                         //Z Sıfırla
#define MSG_LEVEL_BED_HOMING                "XYZ S\xfd\x66\xfdrlan\xfdyor"                               //XYZ Sıfırlanıyor
#define MSG_LEVEL_BED_WAITING               "Ba\xfelatmak i\xe7in t\xfdkla"                              //Başlatmak için tıkla
#define MSG_LEVEL_BED_NEXT_POINT            "S\xfdradaki Nokta"                                          //Sıradaki Nokta
#define MSG_LEVEL_BED_DONE                  "Seviyeleme Tamam!"                                          //Seviyeleme Tamam!
#define MSG_LEVEL_BED_CANCEL                "\xddptal"                                                   //İptal
#define MSG_SET_HOME_OFFSETS                "Offset Ayarla"                                              //Offset Ayarla
#define MSG_HOME_OFFSETS_APPLIED            "Offset Tamam"                                               //Offset Tamam
#define MSG_SET_ORIGIN                      "S\xfd\x66\xfdr Belirle"                                     //Sıfır Belirle
#define MSG_PREHEAT_1                       "\xd6n Is\xfdnma PLA"                                        //Ön Isınma PLA
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "                                            // 
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " T\xfcm"                                      // Tüm
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Tabla"                                       // Tabla
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " Ayar"                                        // Ayar
#define MSG_PREHEAT_2                       "\xd6n Is\xfdnma ABS"                                        //Ön Isınma ABS
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "                                            // 
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " T\xfcm"                                      // Tüm
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Tabla"                                       // Tabla
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " Ayar"                                        // Ayar
#define MSG_COOLDOWN                        "So\xf0ut"                                                   //Soğut
#define MSG_SWITCH_PS_ON                    "G\xfcc\xfc A\xe7"                                           //Gücü Aç
#define MSG_SWITCH_PS_OFF                   "G\xfcc\xfc Kapat"                                           //Gücü Kapat
#define MSG_EXTRUDE                         "Extrude"                                                    //Extrude
#define MSG_RETRACT                         "Geri \xc7ek"                                                //Geri Çek
#define MSG_MOVE_AXIS                       "Eksen Y\xf6net"                                             //Eksenleri Yönet
#define MSG_LEVEL_BED                       "Tabla Seviyele"                                             //Tabla Seviyele
#define MSG_MOVE_X                          "X"                                                          //X
#define MSG_MOVE_Y                          "Y"                                                          //Y
#define MSG_MOVE_Z                          "Z"                                                          //Z
#define MSG_MOVE_E                          "Ekstruder"                                                  //Ekstruder
#define MSG_MOVE_01MM                       "0.1mm"                                                      //0.1mm
#define MSG_MOVE_1MM                        "1mm"                                                        //1mm
#define MSG_MOVE_10MM                       "10mm"                                                       //10mm
#define MSG_SPEED                           "H\xfdz"                                                     //Hız
#define MSG_BED_Z                           "Tabla Z"                                                    //Tabla Z
#define MSG_NOZZLE                          "Noz\xfcl"                                                   //Nozül
#define MSG_BED                             "Tabla"                                                      //Tabla
#define MSG_FAN_SPEED                       "Fan H\xfdz\xfd"                                             //Fan Hızı
#define MSG_FLOW                            "Ak\xfd\xfe"                                                 //Akış
#define MSG_CONTROL                         "Kontrol"                                                    //Kontrol
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Min"                               // Min
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Max"                               // Max
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " \xc7\x61rpan"                      // Çarpan
#define MSG_AUTOTEMP                        "Autotemp"                                                   // Autotemp
#define MSG_ON                              "On "                                                        //On 
#define MSG_OFF                             "Off"                                                        //Off
#define MSG_PID_P                           "PID-P"                                                      //PID-P
#define MSG_PID_I                           "PID-I"                                                      //PID-I
#define MSG_PID_D                           "PID-D"                                                      //PID-D
#define MSG_PID_C                           "PID-C"                                                      //PID-C
#define MSG_SELECT                          "Se\xe7"                                                     //Seç
#define MSG_ACC                             "\xddvme"                                                    //İvme
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
#define MSG_TEMPERATURE                     "S\xfd\x63\x61kl\xfdk"                                       //Sıcaklık
#define MSG_MOTION                          "Hareket"                                                    //Hareket
#define MSG_VOLUMETRIC                      "Filaman"                                                    //Filaman
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"                                                   //E in mm3
#define MSG_FILAMENT_DIAM                   "Fil. \xc7\x61p"                                             //Fil. Çap
#define MSG_CONTRAST                        "LCD Kontrast"                                               //LCD Kontrast
#define MSG_STORE_EPROM                     "Haf\xfdzaya Al"                                             //Hafızaya Al
#define MSG_LOAD_EPROM                      "Haf\xfdzadan Y\xfckle"                                      //Hafızadan Yükle
#define MSG_RESTORE_FAILSAFE                "Fabrika Ayarlar\xfd"                                        //Fabrika Ayarları
#define MSG_REFRESH                         "Yenile"                                                     //Yenile
#define MSG_WATCH                           "Bilgi Ekran\xfd"                                            //Bilgi Ekranı
#define MSG_PREPARE                         "Haz\xfdrl\xfdk"                                             //Hazırlık
#define MSG_TUNE                            "Ayar"                                                       //Ayar
#define MSG_PAUSE_PRINT                     "Duraklat"                                                   //Duraklat
#define MSG_RESUME_PRINT                    "S\xfcrd\xfcr"                                               //Sürdür
#define MSG_STOP_PRINT                      "Durdur"                                                     //Durdur
#define MSG_CARD_MENU                       "SD den Yazd\xfdr"                                           //SD den Yazdır
#define MSG_NO_CARD                         "SD Kart Yok"                                                //SD Kart Yok
#define MSG_DWELL                           "Uyku..."                                                    //Uyku...
#define MSG_USERWAIT                        "Operat\xf6r bekleniyor..."                                  //Operatör bekleniyor...
#define MSG_RESUMING                        "Bask\xfd S\xfcrd\xfcr\xfcl\xfcyor"                          //Baskı Sürdürülüyor
#define MSG_PRINT_ABORTED                   "Bask\xfd Durduruldu"                                        //Baskı Durduruldu
#define MSG_NO_MOVE                         "\xdd\xfelem yok."                                           //İşlem yok.
#define MSG_KILLED                          "Kilitlendi. "                                               //Kilitlendi.
#define MSG_STOPPED                         "Durdu. "                                                    //Durdu.
#define MSG_CONTROL_RETRACT                 "Geri \xc7ek mm"                                             //Geri Çek mm
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"                                                 //Swap Re.mm
#define MSG_CONTROL_RETRACTF                "Geri \xc7ekme  V"                                           //Geri Çekme V
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"                                                     //Hop mm
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"                                                  //UnRet +mm
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet+mm"                                                 //S UnRet+mm
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"                                                   //UnRet V
#define MSG_AUTORETRACT                     "AutoRetr."                                                  //AutoRetr.
#define MSG_FILAMENTCHANGE                  "Filaman De\xf0i\xfetir"                                     //Filaman Değiştir
#define MSG_INIT_SDCARD                     "Init. SD"                                                   //Init. SD
#define MSG_CNG_SDCARD                      "SD De\xf0i\xfetir"                                          //SD Değiştir
#define MSG_ZPROBE_OUT                      "Z Prob A\xe7\xfdk. Tabla"                                   //Z Prob Açık. Tabla
#define MSG_BLTOUCH_SELFTEST                "BLTouch Self-Test"                                          //BLTouch Self-Test
#define MSG_BLTOUCH_RESET                   "S\xfd\x66\xfdrla BLTouch"                                   //Sıfırla BLTouch
#define MSG_HOME                            "S\xfd\x66\xfdrla"                                           //Sıfırla
#define MSG_FIRST                           "\xf6nce"                                                    //Önce
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"                                                   //Z Offset
#define MSG_BABYSTEP_X                      "Miniad\xfdm X"                                              //Miniadım X
#define MSG_BABYSTEP_Y                      "Miniad\xfdm Y"                                              //Miniadım Y
#define MSG_BABYSTEP_Z                      "Miniad\xfdm Z"                                              //Miniadım Z
#define MSG_ENDSTOP_ABORT                   "Endstop iptal"                                              //Endstop iptal
#define MSG_HEATING_FAILED_LCD              "Is\xfdnma ba\xfe\x61\x72\xfds\xfdz"                         //Isınma başarısız
#define MSG_ERR_REDUNDANT_TEMP              "Hata: Ge\xe7ersiz S\xfd\x63akl\xfdk"                        //Hata: Geçersiz Sıcaklık
#define MSG_THERMAL_RUNAWAY                 "TERMAL PROBLEM"                                             //TERMAL PROBLEM
#define MSG_ERR_MAXTEMP                     "Hata: MAXSICAKLIK"                                          //Hata: MAXSICAKLIK
#define MSG_ERR_MINTEMP                     "Hata: MINSICAKLIK"                                          //Hata: MINSICAKLIK
#define MSG_ERR_MAXTEMP_BED                 "Hata: MAXSIC. TABLA"                                        //Hata: MAXSIC. TABLA
#define MSG_ERR_MINTEMP_BED                 "Hata: MINSIC. TABLA"                                        //Hata: MINSIC. TABLA
#define MSG_ERR_Z_HOMING                    "G28 Z Yap\xfdlamaz"                                         //G28 Z Yapılamaz
#define MSG_HALTED                          "YAZICI DURDURULDU"                                          //YAZICI DURDURULDU
#define MSG_PLEASE_RESET                    "L\xfctfen resetleyin"                                       //Lütfen resetleyin
#define MSG_SHORT_DAY                       "G" // One character only                                    //G
#define MSG_SHORT_HOUR                      "S" // One character only                                    //S
#define MSG_SHORT_MINUTE                    "D" // One character only                                    //D
#define MSG_HEATING                         "Is\xfdn\xfdyor..."                                          //Isınıyor...
#define MSG_HEATING_COMPLETE                "Is\xfdnma tamam."                                           //Isınma tamam.
#define MSG_BED_HEATING                     "Tabla Is\xfdn\xfdyor."                                      //Tabla Isınıyor.
#define MSG_BED_DONE                        "Tabla haz\xfdr."                                            //Tabla hazır.
#define MSG_DELTA_CALIBRATE                 "Delta Kalibrasyonu"                                         //Delta Kalibrasyonu
#define MSG_DELTA_CALIBRATE_X               "Ayarla X"                                                   //Ayarla X
#define MSG_DELTA_CALIBRATE_Y               "Ayarla Y"                                                   //Ayarla Y
#define MSG_DELTA_CALIBRATE_Z               "Ayarla Z"                                                   //Ayarla Z
#define MSG_DELTA_CALIBRATE_CENTER          "Ayarla Merkez"                                              //Ayarla Merkez

#define MSG_INFO_MENU                       "Yaz\xfd\x63\xfd Hakk\xfdnda"                                //Yazıcı Hakkında
#define MSG_INFO_PRINTER_MENU               "Yaz\xfd\x63\xfd Bilgisi"                                    //Yazıcı Bilgisi
#define MSG_INFO_STATS_MENU                 "\xddstatistikler"                                           //İstatistikler
#define MSG_INFO_BOARD_MENU                 "Kontrol\xf6r Bilgisi"                                       //Kontrol Bilgisi
#define MSG_INFO_THERMISTOR_MENU            "Termist\xf6rler"                                            //Termistörler
#define MSG_INFO_EXTRUDERS                  "Ekstruderler"                                               //Ekstruderler
#define MSG_INFO_BAUDRATE                   "\xddleti\xfeim H\xfdz\xfd"                                  //İletişim Hızı
#define MSG_INFO_PROTOCOL                   "Protokol"                                                   //Protokol
#define MSG_LIGHTS_ON                       "Ayd\xfdnlatmay\xfd A\xe7"                                   //Aydınlatmayı Aç
#define MSG_LIGHTS_OFF                      "Ayd\xfdnlatmay\xfd Kapa"                                    //Aydınlaymayı Kapa

#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              "Bask\xfd Say\xfds\xfd"                                      //Baskı Sayısı
  #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"                                                 //Tamamlanan
  #define MSG_INFO_PRINT_TIME               "Toplam Bask\xfd S\xfcresi"                                  //Toplam Baskı Süresi
  #define MSG_INFO_PRINT_LONGEST            "En Uzun Bask\xfd S\xfcresi"                                 //En Uzun Baskı Süresi
  #define MSG_INFO_PRINT_FILAMENT           "Toplam Filaman"                                             //Toplam Filaman
#else
  #define MSG_INFO_PRINT_COUNT              "Bask\xfd"                                                   //Baskı
  #define MSG_INFO_COMPLETED_PRINTS         "Tamamlanan"                                                 //Tamamlanan
  #define MSG_INFO_PRINT_TIME               "S\xfcre"                                                    //Süre
  #define MSG_INFO_PRINT_LONGEST            "En Uzun"                                                    //En Uzun
  #define MSG_INFO_PRINT_FILAMENT           "Filaman"                                                    //Filaman
#endif

#define MSG_INFO_MIN_TEMP                   "Min S\xfd\x63."                                             //Min Sıcak.
#define MSG_INFO_MAX_TEMP                   "Max S\xfd\x63."                                             //Max Sıcak.
#define MSG_INFO_PSU                        "G\xfc\xe7 Kayna\xf0\xfd"                                    //Güç Kaynağı

#define MSG_DRIVE_STRENGTH                  "\x53\xfc\x72\xfc\x63\xfc \x47\xfc\x63\xfc"                  //Sürücü Gücü
#define MSG_DAC_PERCENT                     "\x53\xfc\x72\xfc\x63\xfc %"                                 //Sürücü %
#define MSG_DAC_EEPROM_WRITE                "DAC\x27\xfd EEPROM\x27\x61 Yaz"                             //DAC'ı EEPROM'a Yaz
#define MSG_FILAMENT_CHANGE_HEADER          "Filaman De\xf0i\xfetir"                                     //Filaman Değiştir
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "Se\xe7enekler:"                                             //Seçenekler:
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Daha Ak\xfdt"                                               //Daha Akıt
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Bask\xfdy\xfd s\xfcrd\xfcr"                                 //Baskıyı sürdür

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Ba\xfelama bekleniyor"                                    //Başlama bekleniyor
  #define MSG_FILAMENT_CHANGE_INIT_2          "filaman\xfdn"                                             //filamanın
  #define MSG_FILAMENT_CHANGE_INIT_3          "de\xf0i\xfeimi"                                           //değişimi
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Bekleniyor"                                               //Bekleniyor
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filaman\xfdn \xe7\xfdkmas\xfd"                            //filamanın çıkması
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Filaman\xfd y\xfckle"                                     //Filamanı yükle
  #define MSG_FILAMENT_CHANGE_INSERT_2        "ve devam i\xe7in"                                         //ve devam için
  #define MSG_FILAMENT_CHANGE_INSERT_3        "tu\xfea bas..."                                           //tuşa bas...
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Bekleniyor"                                               //Bekleniyor
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filaman\xfdn y\xfcklenmesi"                               //filamanın yüklenmesi
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Bekleniyor"                                               //Bekleniyor
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filaman akmas\xfd"                                        //filaman akması
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Bask\xfdn\xfdn s\xfcrd\xfcr\xfclmesini"                   //Baskının sürdürülmesini
  #define MSG_FILAMENT_CHANGE_RESUME_2        "bekle"                                                    //bekle
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "L\xfctfen bekleyiniz..."                                  //Lütfen bekleyiniz...
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "\xc7\xfdkart\xfdl\xfdyor..."                              //Çıkartılıyor...
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Y\xfckle ve bas"                                          //Yükle ve bas
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Y\xfckl\xfcyor..."                                        //Yüklüyor...
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Ak\xfdt\xfdl\xfdyor..."                                   //Akıtılıyor...
  #define MSG_FILAMENT_CHANGE_RESUME_1        "S\xfcrd\xfcr\xfcl\xfcyor..."                              //Sürdürülüyor...
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_TR_H
