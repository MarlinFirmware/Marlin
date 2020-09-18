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
 * Traditional Chinese
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */
namespace Language_zh_TW {
  using namespace Language_en;  // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 3;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Traditional Chinese");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT("已就緒.");     //" ready."
  PROGMEM Language_Str MSG_MARLIN                          = _UxGT("Marlin");
  PROGMEM Language_Str MSG_YES                             = _UxGT("是");  //"YES"
  PROGMEM Language_Str MSG_NO                              = _UxGT("否");  //"NO"
  PROGMEM Language_Str MSG_BACK                            = _UxGT("返回");     // "Back"
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("正在中止...");   //"Aborting..."
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("記憶卡已插入");     //"Card inserted"
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("記憶卡被拔出");     //"Card removed"
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("等待記憶卡");    //"Waiting for media"
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("記憶卡讀取錯誤"); //"Media read error"
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB裝置已移除");   //"USB device removed"
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("USB啟動失敗");  //"USB start failed"
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("擋塊");     //"Endstops" // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("軟體擋塊");    //"Soft Endstops"
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("主選單");     //"Main"
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("進階設置");   //"Advanced Settings"
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("設置");    //Configuration
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("自動開始");     //"Autostart"
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("關閉步進馬達");     //"Disable steppers"
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("除錯選單");     // "Debug Menu"
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("進度條測試");     // "Progress Bar Test"
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("自動回原點");     //"Auto home"
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("回X原點");     //"Home X"
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("回Y原點");     //"Home Y"
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("回Z原點");     //"Home Z"
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("自動Z對齊");   //"Auto Z-Align"
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("平台調平XYZ歸原點");     //"Homing XYZ"
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("單擊開始熱床調平");     //"Click to Begin"
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("下個熱床調平點");     //"Next Point"
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("完成熱床調平");     //"Leveling Done!"
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("淡出高度");     // "Fade Height"
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("設置原點偏移");     //"Set home offsets"
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("偏移已啟用");     //"Offsets applied"
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("設置原點");     //"Set origin"
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("預熱 ") PREHEAT_1_LABEL;     //"Preheat PREHEAT_1_LABEL"
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("預熱 ") PREHEAT_1_LABEL " ~";     //"Preheat PREHEAT_1_LABEL"
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("預熱 ") PREHEAT_1_LABEL _UxGT(" 噴嘴");     //MSG_PREHEAT_1 " "
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("預熱 ") PREHEAT_1_LABEL _UxGT(" 噴嘴 ~");   //MSG_PREHEAT_1 " "
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("預熱 ") PREHEAT_1_LABEL _UxGT(" 全部");     //MSG_PREHEAT_1 " All"
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("預熱 ") PREHEAT_1_LABEL _UxGT(" 熱床");     //MSG_PREHEAT_1 " Bed"
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("預熱 ") PREHEAT_1_LABEL _UxGT(" 設置");     //MSG_PREHEAT_1 " conf"

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("預熱 $");     //"Preheat PREHEAT_1_LABEL"
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("預熱 $ ~");     //"Preheat PREHEAT_1_LABEL"
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("預熱 $ 噴嘴");     //MSG_PREHEAT_1 " "
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("預熱 $ 噴嘴 ~");   //MSG_PREHEAT_1 " "
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("預熱 $ 全部");     //MSG_PREHEAT_1 " All"
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("預熱 $ 熱床");     //MSG_PREHEAT_1 " Bed"
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("預熱 $ 設置");     //MSG_PREHEAT_1 " conf"
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("自定預熱");   //"Preheat Custom"
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("降溫");     //"Cooldown"
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("激光控制");    //"Laser Control"
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("激光 關");    //"Laser Off"
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("激光 開");   //"Laser On"
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("激光電源");    //"Laser Power"
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("主軸控告制");    //"Spindle Control"
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("主軸 關");    //"Spindle Off"
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("主軸 開");   //"Spindle On"
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("主軸電源");    //"Spindle Power"
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("主軸反轉");  //"Spindle Reverse"
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("電源打開");     //"Switch power on"
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("電源關閉");     //"Switch power off"
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("擠出");     //"Extrude"
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("回縮");     //"Retract"
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("移動軸");     //"Move axis"
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("調平熱床");     //"Bed leveling"
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("調平熱床");     //"Level bed"
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("調平邊角");     // "Level corners"
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("下個邊角");     // "Next corner"
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("網格編輯器");    //"Mesh Editor"
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("編輯網格");     // "Edit Mesh"
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("網格編輯已停止");     // "Mesh Editing Stopped"
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("探測點");   //"Probing Point"
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("索引 X");    //"Index X"
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("索引 Y");    //"Index Y"
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Z 值");    //"Z Value"
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("自定命令");     // "Custom Commands"
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 探測測試");   //"M48 Probe Test"
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 探測點");    //"M48 Point"
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("偏差");    //"Deviation"
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX Mode");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Tool Offsets");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Park");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplication");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Mirrored Copy");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Full Control");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2nd Nozzle X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2nd Nozzle Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2nd Nozzle Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("執行G29");     // "Doing G29"
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL工具");     // "UBL Tools"
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("統一熱床調平(UBL)");     // "Unified Bed Leveling"
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("傾斜點");  //"Tilting Point"
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("手工建網");     // "Manually Build Mesh"
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("放置墊片並測量");     // "Place shim & measure"
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("測量");     // "Measure"
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("移除並測量熱床");     // "Remove & measure bed"
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("移動到下一個");     // "Moving to next"
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("啟動UBL");     // "Activate UBL"
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("關閉UBL");     // "Deactivate UBL"
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("置設熱床溫度");     // "Bed Temp"
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("置設熱床溫度");    //"Bed Temp")
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("置設噴嘴溫度");     // "Hotend Temp"
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("熱端溫度");    //"Hotend Temp"
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("網格編輯");     // "Mesh Edit"
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("編輯客戶網格");     // "Edit Custom Mesh"
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("細調網格");     // "Fine Tuning Mesh"
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("完成編輯網格");     // "Done Editing Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("創設客戶網格");     // "Build Custom Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("創設網格");     // "Build Mesh"
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("創設 $ 網格");     // "Build PREHEAT_1_LABEL Mesh"
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("批准 $ 網格");     // "Validate PREHEAT_1_LABEL Mesh"
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("創設冷網格");     // "Build Cold Mesh"
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("調整網格高度");     // "Adjust Mesh Height"
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("高度合計");     // "Height Amount"
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("批准網格");     // "Validate Mesh"
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("批准客戶網格");     // "Validate Custom Mesh"
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 加熱熱床");    //"G26 Heating Bed"
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 加熱噴嘴"); //"G26 Heating Nozzle"
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("手動填裝");  //"Manual priming..."
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("固定距離填裝");   //"Fixed Length Prime"
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("完成填裝");   //"Done Priming"
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26已取消");   //"G26 Canceled"
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("離開 G26");  //"Leaving G26"
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("繼續熱床網格");     // "Continue Bed Mesh"
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("網格調平");     // "Mesh Leveling"
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("三點調平");     // "3-Point Leveling"
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("格子網格調平");     // "Grid Mesh Leveling"
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("調平網格");     // "Level Mesh"
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("邊點");     // "Side Points"
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("圖類型");     // "Map Type"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("輸出網格圖");     // "Output Mesh Map"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("輸出到主機");     // "Output for Host"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("輸出到CSV");     // "Output for CSV"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("輸出到備份");     // "Off Printer Backup"
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("輸出UBL信息");     // "Output UBL Info"
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("填充合計");     // "Fill-in Amount"
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("手工填充");     // "Manual Fill-in"
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("聰明填充");     // "Smart Fill-in"
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("填充網格");     // "Fill-in Mesh"
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("作廢所有的");     // "Invalidate All"
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("作廢最近的");     // "Invalidate Closest"
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("細調所有的");     // "Fine Tune All"
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("細調最近的");     // "Fine Tune Closest"
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("網格存儲");     // "Mesh Storage"
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("存儲槽");     // "Memory Slot"
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("裝載熱床網格");     // "Load Bed Mesh"
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("保存熱床網格");     // "Save Bed Mesh"
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("網格 %i 已裝載");     // "Mesh %i loaded"
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("網格 %i 已保存");     // "Mesh %i saved"
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("沒有存儲");     // "No storage"
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("錯誤: UBL保存");     // "Err: UBL Save"
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("錯誤: UBL還原");     // "Err: UBL Restore"
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-偏移:");   //"Z-Offset: "
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z偏移已停止");     // "Z-Offset Stopped"
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("一步步UBL");     // "Step-By-Step UBL"
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. 創設冷網格");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. 聰明填充");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. 批准網格");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. 細調所有的");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. 批准網格");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. 細調所有的");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. 保存熱床網格");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("燈管控制");     // "LED Control")
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("燈");     // "Lights")
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("燈預置");     // "Light Presets")
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("红");     // "Red")
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("橙");     // "Orange")
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("黃");     // "Yellow")
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("綠");     // "Green")
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("藍");     // "Blue")
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("青");     // "Indigo")
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("紫");     // "Violet")
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("白");     // "White")
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("復歸");     // "Default")
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("定制燈");     // "Custom Lights")
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("紅飽和度");     // "Red Intensity")
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("綠飽和度");     // "Green Intensity")
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("藍飽和度");     // "Blue Intensity")
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("白飽和度");     // "White Intensity")
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("亮度");     // "Brightness")

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("移動 ...");     // "Moving...")
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("釋放 XY");     // "Free XY")
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("移動X");     //"Move X"
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("移動Y");     //"Move Y"
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("移動Z");     //"Move Z"
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("擠出機");     //"Extruder"
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("擠出機 *");       //"Extruder *"
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("噴嘴溫度不夠");  //"Hotend too cold"
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("移動 %s mm");     //"Move 0.025mm"
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("移動 0.1 mm");     //"Move 0.1mm"
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("移動 1 mm");     //"Move 1mm"
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("移動 10 mm");     //"Move 10mm"
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("速率");     //"Speed"
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("熱床Z");     //"Bed Z"
  PROGMEM Language_Str MSG_NOZZLE                          = " " LCD_STR_THERMOMETER _UxGT(" 噴嘴");     //"Nozzle" 噴嘴
  PROGMEM Language_Str MSG_NOZZLE_N                        = " " LCD_STR_THERMOMETER _UxGT(" 噴嘴 ~");
  PROGMEM Language_Str MSG_BED                             = " " LCD_STR_THERMOMETER _UxGT(" 熱床");     //"Bed"
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Enclosure");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("風扇速率");     //"Fan speed"
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("風扇速率 =");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Stored Fan =");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("額外風扇速率");     // "Extra fan speed"
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("額外風扇速率 =");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("擠出速率");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("擠出速率 ~");     //"Flow"
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("控制");     //"Control"
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" 最小");     //" " LCD_STR_THERMOMETER " Min"
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" 最大");     //" " LCD_STR_THERMOMETER " Max"
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" 系數");     //" " LCD_STR_THERMOMETER " Fact"
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("自動控溫");     //"Autotemp"
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("開 ");     //"On"
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("關 ");     //"Off"

  PROGMEM Language_Str MSG_SELECT                          = _UxGT("選擇");     //"Select"
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("選擇 *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("加速度");     //"Accel" acceleration
  PROGMEM Language_Str MSG_JERK                            = _UxGT("抖動速率");     //"Jerk"
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("軸抖動速率") LCD_STR_A;     //"Va-jerk"
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("軸抖動速率") LCD_STR_B;     //"Vb-jerk"
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("軸抖動速率") LCD_STR_C;     //"Vc-jerk"
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("擠出機抖動速率");     //"Ve-jerk"

  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("速度");     // "Velocity"
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("最大進料速率") LCD_STR_A;     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("最大進料速率") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("最大進料速率") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("最大進料速率") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("最大進料速率 *");     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("最小進料速率");     //"Vmin"  min_feedrate_mm_s
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("最小移動速率");     //"VTrav min" min_travel_feedrate_mm_s, (target) speed of the move
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("加速度");     // "Acceleration"
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("最大列印加速度") LCD_STR_A;     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("最大列印加速度") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("最大列印加速度") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("最大列印加速度") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("最大列印加速度 *");     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("回縮加速度");     //"A-retract" retract_acceleration, E acceleration in mm/s^2 for retracts
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("非列印移動加速度");     //"A-travel" travel_acceleration, X, Y, Z acceleration in mm/s^2 for travel (non printing) moves
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("軸步數/mm");     //"Steps/mm" axis_steps_per_mm, axis steps-per-unit G92
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("軸步數/mm");     //"Asteps/mm" axis_steps_per_mm, axis steps-per-unit G92
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("軸步數/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("軸步數/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("擠出機步數/mm");     //"Esteps/mm"
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("擠出機~步數/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("溫度");     //"Temperature"
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("運作");     //"Motion"
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("絲料測容");     //"Filament" menu_control_volumetric
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("測容積mm³");     //"E in mm3" volumetric_enabled
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("絲料直徑");     //"Fil. Dia."
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("絲料直徑 *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("卸載 mm");     // "Unload mm"
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("装載 mm");     // "Load mm"
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Advance K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Advance K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD對比度");     //"LCD contrast"
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("保存設置");     //"Store memory"
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("載入設置");     //"Load memory"
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("恢復安全值");     //"Restore failsafe"
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("初始化設置");     // "Initialize EEPROM"
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("錯誤: EEPROM CRC");    //"Err: EEPROM CRC"
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("錯誤: EEPROM Index");    //"Err: EEPROM Index"
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("錯誤: EEPROM Version");    //"EEPROM Version"
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("媒體更新");    //"Media Update"
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("重置打印機");    //"Reset Printer
  PROGMEM Language_Str MSG_REFRESH                         = _UxGT("刷新");     //"Refresh"
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("資訊界面");     //"Info screen"
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("準備");     //"Prepare"
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("調整");     //"Tune"
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("開始列印");   //"Start Print"
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("下一個");   //"Next"
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("初始  ");   //"Init"
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("停止  ");   //"Stop"
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("列印  ");   //"Print"
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("復歸  ");   //"Reset"
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("放棄  ");   //"Cancel"
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("確認  ");   //"Done"
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("返回  ");   //"Back"
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("繼續  ");   //"Proceed"
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("暫停列印");     //"Pause print"
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("恢復列印");     //"Resume print"
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("停止列印");     //"Stop print"
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("列印物件");   //"Printing Object"
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("中止物件");   //"Cancel Object"
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("中止物件 =");   //"Cancel Object ="
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("中斷恢復");   //"Outage Recovery"
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("從記憶卡上列印");     //"Print from SD"
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("無記憶卡");     //"No SD card"
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("休眠 ...");     //"Sleep..."
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("點擊繼續 ...");     //"Click to resume..."
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("列印已暫停");     // "Print paused"
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("列印中 ...");   //"Printing..."
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("已取消列印");     //"Print aborted"
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("無移動");     //"No move."
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("已砍掉");     //"KILLED. "
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("已停止");     //"STOPPED. "
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("回縮長度mm");     //"Retract mm" retract_length, retract length (positive mm)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("換手回抽長度mm");     //"Swap Re.mm" swap_retract_length, swap retract length (positive mm), for extruder change
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("回縮速率mm/s");     //"Retract  V" retract_feedrate_mm_s, feedrate for retracting (mm/s)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Hop mm");     //"Hop mm" retract_zraise, retract Z-lift
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("回縮恢復長度mm");     //"UnRet +mm" retract_recover_extra, additional recover length (mm, added to retract length when recovering)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("換手回縮恢復長度mm");     //"S UnRet+mm" swap_retract_recover_extra, additional swap recover length (mm, added to retract length when recovering from extruder change)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("回縮恢復後進料速率mm/s");     //"UnRet V" retract_recover_feedrate_mm_s, feedrate for recovering from retraction (mm/s)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");     // "S UnRet V"
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("自動回縮");     //"AutoRetr." autoretract_enabled,
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("交換長度");    //"Swap Length"
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("清除長度");   //"Purge Length"
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("交換工具"); //"Tool Change"
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z軸提昇");    //"Z Raise"
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("最高速度");    //"Prime Speed"
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("收回速度");  //"Retract Speed"
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("噴嘴待機"); //"Nozzle Standby"
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("更換絲料");     //"Change filament"
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("更換絲料 *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("裝載絲料");     // "Load filament"
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("裝載絲料 *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("卸載絲料");     // "Unload filament"
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("卸載絲料 *");     // "Unload filament"
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("卸載全部");     // "Unload All"
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("初始化記憶卡");     //"Init. SD card"
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("連接記憶卡");     //"Attach Media
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("更換記憶卡");     //"Change SD card"
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("釋放媒體");   //"Release Media"
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z探針在熱床之外");     //"Z probe out. bed" Z probe is not within the physical limits
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("偏斜因數");     // "Skew Factor"

  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch 自檢");     // "BLTouch Self-Test"
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("重置BLTouch");     // "Reset BLTouch"
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("裝載BLTouch");     // "Stow BLTouch"
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("部署BLTouch");     // "Deploy BLTouch"

  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("歸位 %s%s%s 先");     //"Home ... first"
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("探針偏移");   //Probe Offsets
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("探針X偏移量");   //Probe X Offset
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("探針Y偏移量");   //Probe Y Offset
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("探針Z偏移量");   //Probe Z Offset
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("微量調整X軸");     //"Babystep X" lcd_babystep_x, Babystepping enables the user to control the axis in tiny amounts
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("微量調整Y軸");     //"Babystep Y"
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("微量調整Z軸");     //"Babystep Z"
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("總計");    //"Total"
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("擋塊終止");     //"Endstop abort"
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("加熱失敗");     //"Heating failed"
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("錯誤：冗餘溫度");     //"Err: REDUNDANT TEMP"
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("溫度失控");    //"THERMAL RUNAWAY"
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("熱床溫度失控");    //"BED THERMAL RUNAWAY"
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("機箱溫度失控");   //"CHAMBER T. RUNAWAY"
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("錯誤：最高溫度");     //"Err: MAXTEMP"
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("錯誤：最低溫度");     //"Err: MINTEMP"
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("印表機停機");     //"PRINTER HALTED"
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("請重置");     //"Please reset"
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("天");     //"d" // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("時");     //"h" // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("分");     //"m" // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("加熱中 ...");     //"Heating..."
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("冷卻中 ...");   //"Cooling..."
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("加熱熱床中 ...");     //"Bed Heating..."
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("熱床冷卻中 ...");   //"Bed Cooling..."
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("機箱加熱中 ..");   //"Chamber Heating..."
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("機箱冷卻中 ...");   //Chamber Cooling...
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("⊿校準");     //"Delta Calibration"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("⊿校準X");     //"Calibrate X"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("⊿校準Y");     //"Calibrate Y"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("⊿校準Z");     //"Calibrate Z"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("⊿校準中心");     //"Calibrate Center"
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("⊿設置");     // "Delta Settings"
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("⊿自動校準");     // "Auto Calibration"
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("設置⊿高度");     // "Set Delta Height"
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Z偏移");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("⊿斜柱");     // "Diag Rod"
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("⊿高度");     // "Height"
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("⊿半徑");     // "Radius"
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("關於印表機");     //"About Printer"
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("印表機訊息");     //"Printer Info"
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("三點調平");     // "3-Point Leveling"
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("線性調平");     // "Linear Leveling"
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("      雙線性調平");     // "Bilinear Leveling"
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("統一熱床調平(UBL)");     // "Unified Bed Leveling"
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("網格調平");     // "Mesh Leveling"
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("印表機統計");     //"Printer Stats"
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("主板訊息");     //"Board Info"
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("溫度計");     //"Thermistors"
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("      擠出機");     //"Extruders"
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("傳輸率");     //"Baud"
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("協議");     //"Protocol"
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("監測溫度失控:關");   //"Runaway Watch: OFF"
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("監測溫度失控:開");    //"Runaway Watch: ON"

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("外殼燈");     // "Case light"
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("燈亮度");     // "Light BRIGHTNESS"
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("打印機不正確");     // "The printer is incorrect"

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("列印計數");     //"Print Count"
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("已完成");     //"Completed"
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("總列印時間");     //"Total print time"
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("最長工作時間");     //"Longest job time"
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("總計擠出");     //"Extruded total"
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("列印數");     //"Prints"
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("完成");     //"Completed"
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("總共");     //"Total"
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("最長");     //"Longest"
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("已擠出");     //"Extruded"
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("最低溫度");     //"Min Temp"
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("最高溫度");     //"Max Temp"
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("電源供應");     //"Power Supply"
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("驅動力度");     // "Drive Strength"
  PROGMEM Language_Str MSG_DAC_PERCENT                     = _UxGT("驅動 %");     // "Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X 驅動 %");    //X Driver %
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y 驅動 %");    //Y Driver %
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z 驅動 %");    //Z Driver %
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E 驅動 %");    //E Driver %
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC連接錯誤");   //"TMC CONNECTION ERROR"
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("保存驅動設置");     // "DAC EEPROM Write"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("更換絲料");   //"FILAMENT CHANGE"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("列印已暫停");     // "PRINT PAUSED"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("裝載絲料");     // "LOAD FILAMENT"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("卸載絲料");     // "UNLOAD FILAMENT"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("恢複選項:");     // "RESUME OPTIONS:"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("清除更多");     // "Purge more"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("恢復列印");     //"Resume print"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  噴嘴: ");     // "  Nozzle: "
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("斷絲偵測");    //"Runout Sensor"
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("絲距離mm");   //"Runout Dist mm"
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("歸原位失敗");     // "Homing failed"
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("探針探測失敗");     // "Probing failed"

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_2_LINE("按下按鈕", "恢復列印")); //"Press Button to resume print"
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("停車中 ..."));   //"Parking..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("等待開始", "絲料", "變更"));     //"Wait for start of the filament change"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("等待", "卸下絲料"));     //"Wait for filament unload"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("插入絲料", "並按鍵", "繼續 ..."));     //"Insert filament and press button to continue..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("按下按鈕", "加熱噴嘴."));     // "Press button to heat nozzle."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("加熱噴嘴", "請等待 ..."));     // "Heating nozzle Please wait..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("等待", "進料"));     //"Wait for filament load"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("等待", "絲料清除"));     // "Wait for filament purge"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_2_LINE("按下完成","絲料清除")); //"Press button to filament purge"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("等待列印", "恢復"));     //"Wait for print to resume"
  #else // LCD_HEIGHT < 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("按下繼續.."));   //"Click to continue"
    PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING           = _UxGT(MSG_1_LINE("停車中 ..."));     //"Parking..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("請等待 ..."));     //"Please wait..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("插入並點擊"));     //"Insert and Click"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("按下加熱.."));     //"Click to heat"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("加熱中 ..."));     //"Heating..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("退出中 ..."));     //"Ejecting..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("載入中 ..."));     //"Loading..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("清除中 ..."));     //"Purging..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("按下完成.."));     //"Click to finish"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("恢復中 ..."));     //"Resuming..."
  #endif // LCD_HEIGHT < 4
}

#if FAN_COUNT == 1
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED
  #define MSG_FIRST_EXTRA_FAN_SPEED MSG_EXTRA_FAN_SPEED
#else
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED_N
  #define MSG_FIRST_EXTRA_FAN_SPEED MSG_EXTRA_FAN_SPEED_N
#endif
