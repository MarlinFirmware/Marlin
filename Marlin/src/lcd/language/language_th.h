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
 * Thai
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define CHARSIZE 1

  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" พร้อม")
  #define MSG_YES                             _UxGT("ตกลง")
  #define MSG_NO                              _UxGT("ยกเลิก")
  #define MSG_BACK                            _UxGT("กลับ")
  #define MSG_SD_INSERTED                     _UxGT("การ์ดถูกใส่แล้ว")
  #define MSG_SD_REMOVED                      _UxGT("การ์ดถูกถอดแล้ว")
  #define MSG_SD_RELEASED                     _UxGT("การ์ดถูกปลดแล้ว")
  #define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
  #define MSG_LCD_SOFT_ENDSTOPS               _UxGT("Soft Endstops")
  #define MSG_MAIN                            _UxGT("หลัก")
  #define MSG_ADVANCED_SETTINGS               _UxGT("การตั้งค่าเพิ่มเติม")
  #define MSG_CONFIGURATION                   _UxGT("การตั้งค่า")
  #define MSG_AUTOSTART                       _UxGT("เริ่มอัตโนมัติ")
  #define MSG_DISABLE_STEPPERS                _UxGT("ปลดมอเตอร์")
  #define MSG_DEBUG_MENU                      _UxGT("Debug Menu")
  #define MSG_PROGRESS_BAR_TEST               _UxGT("Progress Bar Test")
  #define MSG_AUTO_HOME                       _UxGT("Auto Home")
  #define MSG_AUTO_HOME_X                     _UxGT("Home X")
  #define MSG_AUTO_HOME_Y                     _UxGT("Home Y")
  #define MSG_AUTO_HOME_Z                     _UxGT("Home Z")
  #define MSG_AUTO_Z_ALIGN                    _UxGT("Auto Z-Align")
  #define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
  #define MSG_LEVEL_BED_WAITING               _UxGT("กดเพื่อเริ่ม")
  #define MSG_LEVEL_BED_NEXT_POINT            _UxGT("จุดถัดไป")
  #define MSG_LEVEL_BED_DONE                  _UxGT("เสร็จสิ้นการปรับระดับฐาน!")
  #define MSG_Z_FADE_HEIGHT                   _UxGT("ความสูง Fade")
  #define MSG_SET_HOME_OFFSETS                _UxGT("ตั้ง Home Offsets")
  #define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets ถูกนำไปใช้แล้ว")
  #define MSG_SET_ORIGIN                      _UxGT("ตั้งจุดเริ่มต้น")
  #define MSG_PREHEAT_1                       _UxGT("อุ่นเครื่อง " PREHEAT_1_LABEL)
  #define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
  #define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" ทั้งหมด")
  #define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" ที่หัวฉีด")
  #define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" ที่ฐาน")
  #define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" ตั้งค่า")
  #define MSG_PREHEAT_2                       _UxGT("อุ่นเครื่อง " PREHEAT_2_LABEL)
  #define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
  #define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" ทั้งหมด")
  #define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" ที่หัวฉีด")
  #define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" ที่ฐาน")
  #define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" ตั้งค่า")
  #define MSG_PREHEAT_CUSTOM                  _UxGT("ตั้งค่าอุ่นเครื่องเอง")
  #define MSG_COOLDOWN                        _UxGT("ลดอุณหภูมิ")
  #define MSG_EXTRUDE                         _UxGT("ฉีด")
  #define MSG_RETRACT                         _UxGT("ดึงกลับ")
  #define MSG_MOVE_AXIS                       _UxGT("เลื่อนแกน")
  #define MSG_BED_LEVELING                    _UxGT("การปรับระดับฐาน")
  #define MSG_LEVEL_BED                       _UxGT("ปรับระดับฐาน")
  #define MSG_LEVEL_CORNERS                   _UxGT("ปรับระดับฐานที่มุม")
  #define MSG_NEXT_CORNER                     _UxGT("มุมถัดไป")
  #define MSG_EDIT_MESH                       _UxGT("แก้ไข Mesh")
  #define MSG_EDITING_STOPPED                 _UxGT("หยุดการแก้ไข Mesh")
  #define MSG_USER_MENU                       _UxGT("แก้ไขคำสั่งเอง")
  #define MSG_UBL_UNHOMED                     _UxGT("กรุณา Home XYZ")
  #define MSG_UBL_TOOLS                       _UxGT("เครื่องมือ UBL")
  #define MSG_UBL_MANUAL_MESH                 _UxGT("สร้าง Mesh เอง")
  #define MSG_UBL_ACTIVATE_MESH               _UxGT("เริ่มการใช้งาน UBL")
  #define MSG_UBL_DEACTIVATE_MESH             _UxGT("ยกเลิกการใช้งาน UBL")
  #define MSG_UBL_SET_TEMP_BED                _UxGT("อุณหภูมิฐาน")
  #define MSG_UBL_SET_TEMP_HOTEND             _UxGT("อุณหภูมิหัวฉีด")
  #define MSG_UBL_MESH_EDIT                   _UxGT("แก้ไข Mesh")
  #define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("แก้ไข Mesh เอง")
  #define MSG_UBL_FINE_TUNE_MESH              _UxGT("แก้ไข Mesh อย่างละเอียด")
  #define MSG_UBL_DONE_EDITING_MESH           _UxGT("สิ้นสุดการแก้ไข Mesh")
  #define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("สร้าง Custom Mesh")
  #define MSG_UBL_BUILD_MESH_MENU             _UxGT("สร้าง Mesh")
  #define MSG_UBL_BUILD_MESH_M1               _UxGT("สร้าง Mesh (" PREHEAT_1_LABEL ")")
  #define MSG_UBL_BUILD_MESH_M2               _UxGT("สร้าง Mesh (" PREHEAT_2_LABEL ")")
  #define MSG_UBL_BUILD_COLD_MESH             _UxGT("สร้าง Cold Mesh")
  #define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("ปรับความสูง  Mesh")
  #define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("ระยะความสูง")
  #define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("ตรวจสอบ Mesh")
  #define MSG_UBL_VALIDATE_MESH_M1            _UxGT("ตรวจสอบ Mesh (" PREHEAT_1_LABEL ")")
  #define MSG_UBL_VALIDATE_MESH_M2            _UxGT("ตรวจสอบ Mesh (" PREHEAT_2_LABEL ")")
  #define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("ตรวจสอบ Custom Mesh")
  #define MSG_UBL_CONTINUE_MESH               _UxGT("ดำเนินการต่อ Bed Mesh")
  #define MSG_UBL_MESH_LEVELING               _UxGT("การปรับระดับ Mesh")
  #define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("ปรับระดับแบบ 3-Point")
  #define MSG_UBL_GRID_MESH_LEVELING          _UxGT("ปรับระดับแบบ Grid Mesh")
  #define MSG_UBL_MESH_LEVEL                  _UxGT("ปรับระดับ Mesh")
  #define MSG_UBL_MAP_TYPE                    _UxGT("ชนิด Map")
  #define MSG_UBL_OUTPUT_MAP                  _UxGT("แสดง Mesh Map")
  #define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("แสดงสำหรับ Host")
  #define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("แสดงสำหรับ CSV")
  #define MSG_UBL_INFO_UBL                    _UxGT("แสดงข้อมูล UBL")
  #define MSG_UBL_MANUAL_FILLIN               _UxGT("เพิ่ม Fill-in เอง")
  #define MSG_UBL_FINE_TUNE_ALL               _UxGT("จูนละเอียดทั้งหมด")
  #define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("จูนละเอียดตำแหน่งใกล้สุด")
  #define MSG_UBL_STORAGE_MESH_MENU           _UxGT("ช่องใช้งาน Mesh")
  #define MSG_UBL_LOAD_MESH                   _UxGT("ดึงข้อมูลฐาน Mesh")
  #define MSG_UBL_SAVE_MESH                   _UxGT("บันทึกข้อมูลฐาน Mesh")
  #define MSG_MESH_LOADED                     _UxGT("Mesh %i ดึงสำเร็จ")
  #define MSG_MESH_SAVED                      _UxGT("Mesh %i บันทึกสำเร็จ")

//LED Control
  #define MSG_LED_CONTROL                     _UxGT("การควบคุม LED")
  #define MSG_LEDS                            _UxGT("ระบบไฟ")
  #define MSG_LED_PRESETS                     _UxGT("Preset ระบบไฟ")
  #define MSG_SET_LEDS_RED                    _UxGT("สีแดง")
  #define MSG_SET_LEDS_ORANGE                 _UxGT("สีส้ม")
  #define MSG_SET_LEDS_YELLOW                 _UxGT("สีเหลือง")
  #define MSG_SET_LEDS_GREEN                  _UxGT("สีเขียว")
  #define MSG_SET_LEDS_BLUE                   _UxGT("สีน้ำเงิน")
  #define MSG_SET_LEDS_VIOLET                 _UxGT("สีม่วง")
  #define MSG_SET_LEDS_WHITE                  _UxGT("สีขาว")
  #define MSG_SET_LEDS_DEFAULT                _UxGT("การตั้งค่าเริ่มต้น")
  #define MSG_CUSTOM_LEDS                     _UxGT("ระบบไฟกำหนดเอง")
  #define MSG_INTENSITY_R                     _UxGT("ความเข้มสีแดง")
  #define MSG_INTENSITY_G                     _UxGT("ความเข้มสีเขียว")
  #define MSG_INTENSITY_B                     _UxGT("ความเข้มสีน้ำเงิน")
  #define MSG_INTENSITY_W                     _UxGT("ความเข้มสีขาว")
  #define MSG_LED_BRIGHTNESS                  _UxGT("ความสว่าง")

//Motion
  #define MSG_MOVING                          _UxGT("กำลังเลื่อน...")
  #define MSG_FREE_XY                         _UxGT("ปลดแกน XY")
  #define MSG_MOVE_X                          _UxGT("เลื่อนแกน X")
  #define MSG_MOVE_Y                          _UxGT("เลื่อนแกน Y")
  #define MSG_MOVE_Z                          _UxGT("เลื่อนแกน Z")
  #define MSG_MOVE_E                          _UxGT("หัวฉีด")
  #define MSG_HOTEND_TOO_COLD                 _UxGT("หัวฉีดเย็นเกินไป")
  #define MSG_MOVE_Z_DIST                     _UxGT("เลื่อน %smm")
  #define MSG_MOVE_01MM                       _UxGT("เลื่อน 0.1mm")
  #define MSG_MOVE_1MM                        _UxGT("เลื่อน 1mm")
  #define MSG_MOVE_10MM                       _UxGT("เลื่อน 10mm")
  #define MSG_SPEED                           _UxGT("ความเร็ว")
  #define MSG_BED_Z                           _UxGT("ระยะฐานกับแกน Z")
  #define MSG_NOZZLE                          _UxGT("หัวฉีด")
  #define MSG_BED                             _UxGT("ฐาน")
  #define MSG_FAN_SPEED                       _UxGT("ความเร็วพัดลม")
  #define MSG_EXTRA_FAN_SPEED                 _UxGT("ความเร็วพัดลมเพิ่มเติม")
  #define MSG_FLOW                            _UxGT("การไหล")
  #define MSG_CONTROL                         _UxGT("การควบตุม")
  #define MSG_LCD_ON                          _UxGT("เปิด")
  #define MSG_LCD_OFF                         _UxGT("ปิด")

#if IS_KINEMATIC

#else

#endif

  #define MSG_VELOCITY                        _UxGT("ความเร็ว")
  #define MSG_ACCELERATION                    _UxGT("ความเร่ง")

#if IS_KINEMATIC
  
#else
 
#endif

  #define MSG_TEMPERATURE                     _UxGT("อุณหภูมิ")
  #define MSG_MOTION                          _UxGT("การเคลื่อนที่")
  #define MSG_STORE_EEPROM                    _UxGT("บันทึกการตั้งค่า")
  #define MSG_LOAD_EEPROM                     _UxGT("ดึงข้อมูลการตั้งค่า")
  #define MSG_INIT_EEPROM                     _UxGT("เริ่มต้น EEPROM")
  #define MSG_WATCH                           _UxGT("หน้าแสดงข้อมูล")
  #define MSG_PREPARE                         _UxGT("เตรียมพร้อม")
  #define MSG_TUNE                            _UxGT("จูน")
  #define MSG_START_PRINT                     _UxGT("เริ่มปริ้น")
  #define MSG_BUTTON_NEXT                     _UxGT("ถัดไป")
  #define MSG_BUTTON_STOP                     _UxGT("หยุด")
  #define MSG_BUTTON_CANCEL                   _UxGT("ยกเลิก")
  #define MSG_BUTTON_DONE                     _UxGT("เสร็จสิ้น")
  #define MSG_PAUSE_PRINT                     _UxGT("หยุดชั่วคราว")
  #define MSG_RESUME_PRINT                    _UxGT("เริ่มต่อ")
  #define MSG_STOP_PRINT                      _UxGT("หยุด")
  #define MSG_OUTAGE_RECOVERY                 _UxGT("การกู้คืน")
  #define MSG_CARD_MENU                       _UxGT("ปริ้นจากการ์ด")
  #define MSG_NO_CARD                         _UxGT("ไม่พบ SD Card")
  #define MSG_USERWAIT                        _UxGT("กดเพื่อเริ่ม...")
  #define MSG_PRINT_PAUSED                    _UxGT("หยุดชั่วคราว")
  #define MSG_PRINTING                        _UxGT("กำลังทำงาน...")
  #define MSG_PRINT_ABORTED                   _UxGT("หยุดการทำงาน")
  #define MSG_CONTROL_RETRACT                 _UxGT("ดึงกลับ mm")
  #define MSG_CONTROL_RETRACTF                _UxGT("ดึงกลับ  V")
  #define MSG_FILAMENTCHANGE                  _UxGT("เปลี่ยน Filament")
  #define MSG_FILAMENTLOAD                    _UxGT("ดึง Filament")
  #define MSG_FILAMENTUNLOAD                  _UxGT("ถอน Filament")
  #define MSG_FILAMENTUNLOAD_ALL              _UxGT("ถอนทั้งหมด")
  #define MSG_INIT_SDCARD                     _UxGT("ค้นหา SD Card")
  #define MSG_CHANGE_SDCARD                   _UxGT("เปลี่ยน SD Card")
  #define MSG_RELEASE_SDCARD                  _UxGT("ปลด SD Card")
  #define MSG_BLTOUCH_MODE_CHANGE             _UxGT("อันตราย: การตั้งค่าสามารถสร้างความเสียหายได้! ต้องการดำเนินการต่อหรือไม่?")
  #define MSG_TOUCHMI_SAVE                    _UxGT("บันทึก")
  #define MSG_FIRST                           _UxGT("ก่อน")
  #define MSG_HEATING_FAILED_LCD              _UxGT("ไม่สามารถอุ่นได้")
  #define MSG_HEATING_FAILED_LCD_BED          _UxGT("ไม่สามารถอุ่นฐานได้")
  #define MSG_HALTED                          _UxGT("PRINTER ถูกยกเลิกการทำงาน")
  #define MSG_PLEASE_RESET                    _UxGT("กรุณา Reset")
  #define MSG_HEATING                         _UxGT("กำลังอุ่น...")
  #define MSG_COOLING                         _UxGT("กำลังเย็นตัว...")
  #define MSG_BED_HEATING                     _UxGT("กำลังอุ่นฐาน...")
  #define MSG_BED_COOLING                     _UxGT("ฐานกำลังเย็นตัว...")
  #define MSG_DELTA_HEIGHT                    _UxGT("ความสูง")
  #define MSG_DELTA_RADIUS                    _UxGT("รัศมี")
  #define MSG_INFO_MENU                       _UxGT("เกี่ยวกับ Printer")
  #define MSG_INFO_PRINTER_MENU               _UxGT("ข้อมูล Printer")


#if LCD_WIDTH >= 20
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("เสร็จสิ้น")
#else

#endif

  #define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("หยุดชั่วคราว")
  #define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("ดึง FILAMENT")
  #define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("ถอน FILAMENT")
  #define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("ฉีดเพิ่ม")
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("เริ่มต่อ")

#if LCD_HEIGHT >= 4
    #define MSG_ADVANCED_PAUSE_WAITING_1      _UxGT("กดปุ่ม")
    #define MSG_ADVANCED_PAUSE_WAITING_2      _UxGT("เพื่อเริ่มการทำงานต่อ")
    #define MSG_PAUSE_PRINT_INIT_1            _UxGT("กำลังจอด...")
    #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("รอ")
    #define MSG_FILAMENT_CHANGE_INIT_2        _UxGT("การเปลี่ยน Filament")
    #define MSG_FILAMENT_CHANGE_INIT_3        _UxGT("เพื่อเริ่ม")
    #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("กรุณาใส่ filament")
    #define MSG_FILAMENT_CHANGE_INSERT_2      _UxGT("แล้วกดปุ่ม")
    #define MSG_FILAMENT_CHANGE_INSERT_3      _UxGT("เพื่อเริ่มต่อ")
    #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("กดปุ่ม")
    #define MSG_FILAMENT_CHANGE_HEAT_2        _UxGT("เพื่ออุ่นหัวฉีด")
    #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("กำลังอุ่นหัวฉีด")
    #define MSG_FILAMENT_CHANGE_HEATING_2     _UxGT("กรุณารอ...")
    #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("รอ")
    #define MSG_FILAMENT_CHANGE_UNLOAD_2      _UxGT("ถอน Filament")
    #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("รอ")
    #define MSG_FILAMENT_CHANGE_LOAD_2        _UxGT("ดึง Filament")
    #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("รอ")
    #define MSG_FILAMENT_CHANGE_PURGE_2       _UxGT("Filament ฉีดทดสอบ")
    #define MSG_FILAMENT_CHANGE_CONT_PURGE_1  _UxGT("กดเพื่อสิ้นสุด")
    #define MSG_FILAMENT_CHANGE_CONT_PURGE_2  _UxGT("ฉีดทดสอบ Filament")
    #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("รอการทำงาน")
    #define MSG_FILAMENT_CHANGE_RESUME_2      _UxGT("เพื่อเริ่มต่อ...")
#else // LCD_HEIGHT < 4
    #define MSG_ADVANCED_PAUSE_WAITING_1      _UxGT("กดเพื่อเริ่มต่อ")
    #define MSG_PAUSE_PRINT_INIT_1            _UxGT("กำลังจอด...")
    #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("กรุณารอ...")
    #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("ใส่แล้วกดปุ่ม")
    #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("กดปุ่มเพื่ออุ่น")
    #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("กำลังอุ่น...")
    #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("กำลังถอน...")
    #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("กำลังดึง...")
    #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("กำลังฉีด...")
    #define MSG_FILAMENT_CHANGE_CONT_PURGE_1  _UxGT("กดเพื่อสิ้นสุด")
    #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("กำลังเริ่มต่อ...")
#endif // LCD_HEIGHT < 4

  #define MSG_TMC_CURRENT                     _UxGT("กระแส Driver")