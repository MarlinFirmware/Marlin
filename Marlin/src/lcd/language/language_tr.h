 /**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Turkish
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 * Bu çeviri dosyasındaki sorunlar ve düzeltmeler için iletişim;
 * Contact for issues and corrections in this translation file;
 * Yücel Temel - (info@elektromanyetix.com) - https://elektromanyetix.com/
 *
 */

#define DISPLAY_CHARSET_ISO10646_TR

namespace Language_tr {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Turkish");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" hazır.");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Geri");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("SD K. Yerleştirildi.");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("SD Kart Çıkarıldı.");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Enstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Yazılımsal Endstops");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Ana");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Gelişmiş Ayarlar");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Yapılandırma");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Oto. Başlat");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Motorları Durdur");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Hata Ayıklama");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Durum Çubuğu Testi");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Eksenleri Sıfırla");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("X Sıfırla");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Y Sıfırla");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Z Sıfırla");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Oto. Z-Hizalama");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("XYZ Sıfırlanıyor");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Başlatmak için tıkla");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Sonraki Nokta");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Hizalama Tamam!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Kaçınma Yüksekliği");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Offset Ayarla");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offset Tamam");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Sıfır Belirle");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Ön Isınma ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Ön Isınma ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Ön Isınma ") PREHEAT_1_LABEL _UxGT(" Nozul");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Ön Isınma ") PREHEAT_1_LABEL _UxGT(" Nozul ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Ön Isınma ") PREHEAT_1_LABEL _UxGT(" Tüm");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Ön Isınma ") PREHEAT_1_LABEL _UxGT(" Tabla");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Ön Isınma ") PREHEAT_1_LABEL _UxGT(" Ayarlar");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Ön Isınma ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Ön Isınma ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Ön Isınma ") PREHEAT_2_LABEL _UxGT(" Nozul");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Ön Isınma ") PREHEAT_2_LABEL _UxGT(" Nozul ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Ön Isınma ") PREHEAT_2_LABEL _UxGT(" Tüm");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Ön Isınma ") PREHEAT_2_LABEL _UxGT(" Tabla");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Ön Isınma ") PREHEAT_2_LABEL _UxGT(" Ayarlar");
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Özel Ön Isınma");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Soğut");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Gücü Aç");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Gücü Kapat");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Ekstrüzyon");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Geri Çek");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Eksen Hareketleri");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Tabla Hizalama");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Tabla Hizası");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Hizalama Köşeleri");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Sonraki Köşe");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Mesh Düzenle");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Mesh Düzenleme Durdu");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("İndeks X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("İndeks Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Z Değeri");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Özel Komutlar");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX Modu");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Oto-Park");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Kopyala");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Yansıtılmış kopya");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Tam Kontrol");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2. nozul X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2. nozul Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2. nozul Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("G29 Çalışıyor");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL Araçları");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("UBL Yatak Hizalama");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Elle Mesh Oluştur");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Altlık & Ölçü Ver");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Ölçü");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Yataktan Ölçü Kaldır");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Sonrakine Git");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("UBL'yi Etkinleştir");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("UBL'yi Etkisizleştir");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Yatak Sıcaklığı");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Yatak Sıcaklığı");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Nozul Sıcaklığı");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Nozul Sıcaklığı");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Mesh Düzenleme");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Özel Mesh Düzenleme");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("İnce Ayar Mesh");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Mesh Düzenleme Tamam");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Özel Mesh Oluştur");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Mesh Oluştur");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = _UxGT("Mesh Oluştur (") PREHEAT_1_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = _UxGT("Mesh Oluştur (") PREHEAT_2_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Soğuk Mesh Oluştur");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Mesh Yükseklik Ayarı");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Yükseklik miktarı");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Doğrulama Mesh");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = _UxGT("Doğrulama Mesh (") PREHEAT_1_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = _UxGT("Doğrulama Mesh (") PREHEAT_2_LABEL _UxGT(")");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Özel Mesh Doğrulama");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Tabla Mesh Devam et");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Mesh Hizalama");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-Nokta Hizalama");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Kafes Mesh Hizalama");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Mesh Seviyesi");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Yan Noktalar");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Haritalama Türü");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Mesh Çıkış Haritası");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Host için Çıktı");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("CSV için Çıktı");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Yazıcıda Yedek Kpalı");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("UBL Çıkış Bilgisi");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Dolgu Miktarı");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Manuel Dolgu");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Akıllı Dogu");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Mesh Dolgu");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Tümünü Geçersiz Kıl");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Yakını Geçersiz Kıl");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Tümünü İnce Ayarla");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Yakını İnce Ayarla");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Mesh Depolama");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Bellek Yuvası");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Yatak Mesh Yükle");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Yatak Mesh Kayıt Et");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Mesh %i yüklendi");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Mesh %i kayıtlandı");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Depolama Yok");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Hata: UBL Kayıt");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Hata: UBL Yenileme");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-Ofset Durduruldu");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Adım Adım UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Soğuk Mesh Oluştur");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Akıllı Dogu");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Doğrulama Mesh");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Tümünü İnce Ayarla");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Doğrulama Mesh");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Tümünü İnce Ayarla");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Yatak Mesh Kayıt Et");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("LED Kontrolü");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Işıklar");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Işık Hazır Ayarları");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Kırmızı");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Turuncu");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Sarı");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Yeşil");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Mavi");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Lacivert");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Menekşe");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Beyaz");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Varsayılan");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Özel Işıklar");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Kırmızı Şiddeti");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Yeşil Şiddeti");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Mavi Şiddeti");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Beyaz Şiddeti");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Parlaklık");
  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Hareket Ediyor..");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Durdur XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("X Hareketi");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Y Hareketi");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Z Hareketi");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Ekstruder");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Ekstruder *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Nozul Çok Soğuk");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("%smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Hız");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z Mesafesi");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Nozul");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Nozul ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Tabla");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Fan Hızı");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Fan Hızı ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Ekstra Fan Hızı");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Ekstra Fan Hızı ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Akış");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Akış ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Kontrol");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Çarpan");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Oto. Sıcaklık");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Açık");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Kapalı");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Seç");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Seç *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("İvme");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Sarsım");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-Sarsım");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-Sarsım");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-Sarsım");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-Sarsım");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Jonksiyon Sapması");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Hız Vektörü");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("HızVektör.max ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("HızVektör.max ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("HızVektör.max ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("HızVektör.max ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("HızVektör.max *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("HızVektör.min");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("HV.gezinme min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Ivme");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Max. ivme ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Max. ivme ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Max. ivme ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Max. ivme ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Max. ivme *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Ivme-geri çekme");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Ivme-gezinme");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Adım/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" adım/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" adım/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" adım/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E adım/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* adım/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Sıcaklık");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Hareket");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filaman");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("Ekstrüzyon/mm³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Filaman Çapı");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Filaman Çapı *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Çıkart mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Yükle mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("K İlerlet");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("K İlerlet *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD Kontrast");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Hafızaya Al");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Hafızadan Yükle");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Fabrika Ayarları");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("EEPROM'u başlat");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("SD Güncellemesi");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Yazıcıyı Resetle");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Yenile");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Bilgi Ekranı");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Hazırlık");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Ayar");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Duraklat");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Sürdür");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Durdur");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Kesinti Kurtarma");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("SD Karttan Yazdır");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("SD Kart Yok!");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Uyku...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Operatör bekleniyor.");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Baskı Duraklatıldı");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Baskı Yapılıyor...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Baskı Durduruldu!");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("İşlem yok.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("Kilitlendi. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("Durdu. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Geri Çek mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Swap Re.mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Geri Çekme V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Atlama mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("S UnRet mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("UnRet  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Oto. Geri Çekme");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("G.Çekme Boyu");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Takım Değişimi");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z Yükselt");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPD          = _UxGT("Birincil Hız");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPD        = _UxGT("Geri Çekme Hızı");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Filaman Değiştir");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Filaman Değiştir *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Filaman Yükle");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Filaman Yükle *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Filaman Çıkart");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Filaman Çıkart *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Tümünü Çıkart");
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("SD Kart Başlatılıyor");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("SD Kart Değiştir");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z Prob Açık. Tabla");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Çarpıklık Faktörü");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Self-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Sıfırla BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("BLTouch Aç");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("BLTouch Kapat");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Z-Prob Aç");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Z-Sensör Kapat");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Sıfırla %s%s%s Önce");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Miniadım X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Miniadım Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Miniadım Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Endstop iptal");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Isınma başarısız");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Yatak Isınma Başrsız");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Hata: Sıcaklık Aşımı");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("TERMAL PROBLEM");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("TABLA TERMAL PROBLEM");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Hata: MAX.SICAKLIK");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Hata: MIN.SICAKLIK");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Hata: MAX.SIC. TABLA");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Hata: MIN.SIC. TABLA");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Önce XY Sıfırla");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("YAZICI DURDURULDU");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Lütfen Resetleyin");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("G"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("S"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("D"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Isınıyor...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Soğuyor...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Tabla Isınıyor...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Tabla Soğuyor...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Kalibrasyonu");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Ayarla X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Ayarla Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Ayarla Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Ayarla Merkez");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta Ayarları");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Oto Kalibrasyon");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Delta Yük. Ayarla");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Z Prob Ofseti");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Çapral Mil");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Yükseklik");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Yarıçap");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Yazıcı Hakkında");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Yazıcı Bilgisi");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("3-Nokta Hizalama");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Doğrusal Hizalama");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("İki Yönlü Doğ. Hiza.");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Birleşik Tabla Hiza.");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Mesh Hizalama");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("İstatistikler");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Kontrolcü Bilgisi");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistörler");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Ekstruderler");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("İletişim Hızı");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protokol");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Aydınlatmayı Aç");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Aydınlatma Parlaklğı");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Yanlış Yazıcı");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Baskı Sayısı");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Tamamlanan");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Toplam Baskı Süresi");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("En Uzun Baskı Süresi");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Toplam Filaman");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Baskı");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Tamamlanan");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Süre");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("En Uzun");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Filaman");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Min Sıc.");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Max Sıc.");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Güç Kaynağı");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Sürücü Gücü");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Sürücü %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Sürücü %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Sürücü %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Sürücü %");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC EEPROM Yaz");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("BASKI DURAKLATILDI");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("FILAMAN YüKLE");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("FILAMAN ÇIKART");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("Seçenekler:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Daha Fazla Tasviye");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Baskıyı sürdür");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Nozul: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Runout Sensörü");
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("Sıfırlama Başarısız");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Probing Başarısız");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Çok Soğuk");
  //
  // Filament Değiştirme ekranları, 4 satırlı bir ekranda 3 satıra kadar gösterilir
  //                        ...veya 3 satırlı ekranda 2 satıra kadar
  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("Baskıya devam etmek", "için Butona bas"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Filaman değişimi", "için başlama", "bekleniyor"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Filamanı yükle", "ve devam için", "tuşa bas..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Nozulü Isıtmak için", "Butona Bas."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Nozul Isınıyor", "Lütfen Bekleyin..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Filamanın çıkması", "bekleniyor"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Filamanın yüklenmesi", "bekleniyor.."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Filaman Temizlemesi", "için bekle"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("Filaman Temizlemesi", "bitirmek için tıkla"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("Baskının devam ", "etmesi için bekle"));
  #else // LCD_HEIGHT < 4
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Sürdürmek İçin Tıkla"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Lütfen bekleyiniz..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Yükle ve bas"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Isıtmak için Tıkla"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Isınıyor..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Çıkartılıyor..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Yüklüyor..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Temizleniyor..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Bitirmek için Tıkla"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Sürdürülüyor..."));
  #endif // LCD_HEIGHT < 4
}
