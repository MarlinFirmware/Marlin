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
#pragma once

/**
 * Traditional Chinese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define CHARSIZE 3

#define WELCOME_MSG                         MACHINE_NAME _UxGT("已就緒.")  //" ready."
#define MSG_BACK                            _UxGT("返回")         // ”Back“
#define MSG_SD_INSERTED                     _UxGT("記憶卡已插入")  //"Card inserted"
#define MSG_SD_REMOVED                      _UxGT("記憶卡被拔出")  //"Card removed"
#define MSG_LCD_ENDSTOPS                    _UxGT("擋塊")  //"Endstops" // Max length 8 characters
#define MSG_MAIN                            _UxGT("主選單")  //"Main"
#define MSG_AUTOSTART                       _UxGT("自動開始")  //"Autostart"
#define MSG_DISABLE_STEPPERS                _UxGT("關閉步進馬達")  //"Disable steppers"
#define MSG_DEBUG_MENU                      _UxGT("除錯選單")    // "Debug Menu"
#define MSG_PROGRESS_BAR_TEST               _UxGT("進度條測試")  // "Progress Bar Test"
#define MSG_AUTO_HOME                       _UxGT("自動回原點")  //"Auto home"
#define MSG_AUTO_HOME_X                     _UxGT("回X原點")  //"Home X"
#define MSG_AUTO_HOME_Y                     _UxGT("回Y原點")  //"Home Y"
#define MSG_AUTO_HOME_Z                     _UxGT("回Z原點")  //"Home Z"
#define MSG_TMC_Z_CALIBRATION               _UxGT("⊿校準Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("平台調平XYZ歸原點")  //"Homing XYZ"
#define MSG_LEVEL_BED_WAITING               _UxGT("單擊開始熱床調平")  //"Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("下個熱床調平點")  //"Next Point"
#define MSG_LEVEL_BED_DONE                  _UxGT("完成熱床調平")  //"Leveling Done!"
#define MSG_Z_FADE_HEIGHT                   _UxGT("淡出高度")     // "Fade Height"
#define MSG_SET_HOME_OFFSETS                _UxGT("設置原點偏移")  //"Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("偏移已啟用")  //"Offsets applied"
#define MSG_SET_ORIGIN                      _UxGT("設置原點")  //"Set origin"
#define MSG_PREHEAT_1                       _UxGT("預熱" PREHEAT_1_LABEL)  //"Preheat PREHEAT_1_LABEL"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")  //MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" 全部")  //MSG_PREHEAT_1 " All"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" 熱床")  //MSG_PREHEAT_1 " Bed"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" 設置")  //MSG_PREHEAT_1 " conf"
#define MSG_PREHEAT_2                       _UxGT("預熱" PREHEAT_2_LABEL)  //"Preheat PREHEAT_2_LABEL"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")  //MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" 全部")  //MSG_PREHEAT_2 " All"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" 熱床")  //MSG_PREHEAT_2 " Bed"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" 設置")  //MSG_PREHEAT_2 " conf"
#define MSG_COOLDOWN                        _UxGT("降溫")  //"Cooldown"
#define MSG_SWITCH_PS_ON                    _UxGT("電源打開")  //"Switch power on"
#define MSG_SWITCH_PS_OFF                   _UxGT("電源關閉")  //"Switch power off"
#define MSG_EXTRUDE                         _UxGT("擠出")  //"Extrude"
#define MSG_RETRACT                         _UxGT("回縮")  //"Retract"
#define MSG_MOVE_AXIS                       _UxGT("移動軸")  //"Move axis"
#define MSG_BED_LEVELING                    _UxGT("調平熱床")  //"Bed leveling"
#define MSG_LEVEL_BED                       _UxGT("調平熱床")  //"Level bed"
#define MSG_LEVEL_CORNERS                   _UxGT("調平邊角") // "Level corners"

#define MSG_NEXT_CORNER                     _UxGT("下个邊角") // "Next corner"
#define MSG_EDITING_STOPPED                 _UxGT("網格編輯已停止") // "Mesh Editing Stopped"
#define MSG_USER_MENU                       _UxGT("客制命令") // "Custom Commands"

#define MSG_UBL_DOING_G29                   _UxGT("执行G29") // "Doing G29"
#define MSG_UBL_UNHOMED                     _UxGT("先回XYZ原點") // "Home XYZ first"
#define MSG_UBL_TOOLS                       _UxGT("UBL工具") // "UBL Tools"
#define MSG_UBL_LEVEL_BED                   _UxGT("統一熱床調平(UBL)") // "Unified Bed Leveling"
#define MSG_UBL_MANUAL_MESH                 _UxGT("手工建网") // "Manually Build Mesh"

#define MSG_UBL_BC_INSERT                   _UxGT("放置墊片並測量") // "Place shim & measure"
#define MSG_UBL_BC_INSERT2                  _UxGT("測量") // "Measure"
#define MSG_UBL_BC_REMOVE                   _UxGT("移除並測量熱床") // "Remove & measure bed"
#define MSG_UBL_MOVING_TO_NEXT              _UxGT("移動到下一個") // "Moving to next"
#define MSG_UBL_ACTIVATE_MESH               _UxGT("激活UBL") // "Activate UBL"
#define MSG_UBL_DEACTIVATE_MESH             _UxGT("關閉UBL") // "Deactivate UBL"
#define MSG_UBL_SET_TEMP_BED                _UxGT("設置熱床溫度") // "Bed Temp"
#define MSG_UBL_SET_TEMP_HOTEND             _UxGT("熱端溫度") // "Hotend Temp"
#define MSG_UBL_MESH_EDIT                   _UxGT("網格編輯") // "Mesh Edit"
#define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("編輯客戶網格") // "Edit Custom Mesh"
#define MSG_UBL_FINE_TUNE_MESH              _UxGT("細調網格") // "Fine Tuning Mesh"
#define MSG_UBL_DONE_EDITING_MESH           _UxGT("完成編輯網格") // "Done Editing Mesh"
#define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("創設客戶網格") // "Build Custom Mesh"
#define MSG_UBL_BUILD_MESH_MENU             _UxGT("創設網格") // "Build Mesh"
#define MSG_UBL_BUILD_MESH_M1               _UxGT("創設" PREHEAT_1_LABEL "網格") // "Build PREHEAT_1_LABEL Mesh"
#define MSG_UBL_BUILD_MESH_M2               _UxGT("創設" PREHEAT_2_LABEL "網格") // "Build PREHEAT_2_LABEL Mesh"
#define MSG_UBL_BUILD_COLD_MESH             _UxGT("創設冷網格") // "Build Cold Mesh"
#define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("調整網格高度") // "Adjust Mesh Height"
#define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("高度合計") // "Height Amount"
#define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("批准網格") // "Validate Mesh"
#define MSG_UBL_VALIDATE_MESH_M1            _UxGT("批准" PREHEAT_1_LABEL "網格") // "Validate PREHEAT_1_LABEL Mesh"
#define MSG_UBL_VALIDATE_MESH_M2            _UxGT("批准" PREHEAT_2_LABEL "網格") // "Validate PREHEAT_2_LABEL Mesh"
#define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("批准客戶網格") // "Validate Custom Mesh"
#define MSG_UBL_CONTINUE_MESH               _UxGT("繼續熱床網格") // "Continue Bed Mesh"
#define MSG_UBL_MESH_LEVELING               _UxGT("網格調平") // "Mesh Leveling"
#define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("三點調平") // "3-Point Leveling"
#define MSG_UBL_GRID_MESH_LEVELING          _UxGT("格子網格調平") // "Grid Mesh Leveling"
#define MSG_UBL_MESH_LEVEL                  _UxGT("調平網格") // "Level Mesh"
#define MSG_UBL_SIDE_POINTS                 _UxGT("邊點") // "Side Points"
#define MSG_UBL_MAP_TYPE                    _UxGT("圖類型") // "Map Type"
#define MSG_UBL_OUTPUT_MAP                  _UxGT("輸出網格圖") // "Output Mesh Map"
#define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("輸出到主機") // "Output for Host"
#define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("輸出到CSV") // "Output for CSV"
#define MSG_UBL_OUTPUT_MAP_BACKUP           _UxGT("輸出到備份") // "Off Printer Backup"
#define MSG_UBL_INFO_UBL                    _UxGT("輸出UBL信息") // "Output UBL Info"
#define MSG_EDIT_MESH                       _UxGT("編輯網格") // "Edit Mesh"
#define MSG_UBL_FILLIN_AMOUNT               _UxGT("填充合計") // "Fill-in Amount"
#define MSG_UBL_MANUAL_FILLIN               _UxGT("手工填充") // "Manual Fill-in"
#define MSG_UBL_SMART_FILLIN                _UxGT("聰明填充") // "Smart Fill-in"
#define MSG_UBL_FILLIN_MESH                 _UxGT("填充網格") // "Fill-in Mesh"
#define MSG_UBL_INVALIDATE_ALL              _UxGT("作廢所有的") // "Invalidate All"
#define MSG_UBL_INVALIDATE_CLOSEST          _UxGT("作廢最近的") // "Invalidate Closest"
#define MSG_UBL_FINE_TUNE_ALL               _UxGT("細調所有的") // "Fine Tune All"
#define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("細調最近的") // "Fine Tune Closest"
#define MSG_UBL_STORAGE_MESH_MENU           _UxGT("網格存儲") // "Mesh Storage"
#define MSG_UBL_STORAGE_SLOT                _UxGT("存儲槽") // "Memory Slot"
#define MSG_UBL_LOAD_MESH                   _UxGT("裝載熱床網格") // "Load Bed Mesh"
#define MSG_UBL_SAVE_MESH                   _UxGT("保存熱床網格") // "Save Bed Mesh"
#define MSG_MESH_LOADED                     _UxGT("網格 %i 已裝載") // "Mesh %i loaded"
#define MSG_MESH_SAVED                      _UxGT("網格 %i 已保存") // "Mesh %i saved"
#define MSG_NO_STORAGE                      _UxGT("沒有存儲") // "No storage"
#define MSG_UBL_SAVE_ERROR                  _UxGT("錯誤: UBL保存") // "Err: UBL Save"
#define MSG_UBL_RESTORE_ERROR               _UxGT("錯誤: UBL還原") // "Err: UBL Restore"
#define MSG_UBL_Z_OFFSET_STOPPED            _UxGT("Z偏移已停止") // "Z-Offset Stopped"
#define MSG_UBL_STEP_BY_STEP_MENU           _UxGT("一步步UBL") // "Step-By-Step UBL"

#define MSG_LED_CONTROL                     _UxGT("灯管控制") // "LED Control")
#define MSG_LEDS                            _UxGT("灯") // "Lights")
#define MSG_LED_PRESETS                     _UxGT("灯预置") // "Light Presets")
#define MSG_SET_LEDS_RED                    _UxGT("红") // "Red")
#define MSG_SET_LEDS_ORANGE                 _UxGT("橙") // "Orange")
#define MSG_SET_LEDS_YELLOW                 _UxGT("黄") // "Yellow")
#define MSG_SET_LEDS_GREEN                  _UxGT("绿") // "Green")
#define MSG_SET_LEDS_BLUE                   _UxGT("蓝") // "Blue")
#define MSG_SET_LEDS_INDIGO                 _UxGT("青") // "Indigo")
#define MSG_SET_LEDS_VIOLET                 _UxGT("紫") // "Violet")
#define MSG_SET_LEDS_WHITE                  _UxGT("白") // "White")
#define MSG_SET_LEDS_DEFAULT                _UxGT("缺省") // "Default")
#define MSG_CUSTOM_LEDS                     _UxGT("定制灯") // "Custom Lights")
#define MSG_INTENSITY_R                     _UxGT("红飽和度") // "Red Intensity")
#define MSG_INTENSITY_G                     _UxGT("绿飽和度") // "Green Intensity")
#define MSG_INTENSITY_B                     _UxGT("蓝飽和度") // "Blue Intensity")
#define MSG_INTENSITY_W                     _UxGT("白飽和度") // "White Intensity")
#define MSG_LED_BRIGHTNESS                  _UxGT("亮度") // "Brightness")

#define MSG_MOVING                          _UxGT("移动 ...") // "Moving...")
#define MSG_FREE_XY                         _UxGT("释放 XY") // "Free XY")
#define MSG_MOVE_X                          _UxGT("移動X")  //"Move X"
#define MSG_MOVE_Y                          _UxGT("移動Y")  //"Move Y"
#define MSG_MOVE_Z                          _UxGT("移動Z")  //"Move Z"
#define MSG_MOVE_E                          _UxGT("擠出機")  //"Extruder"
#define MSG_MOVE_01MM                       _UxGT("移動 0.1 mm")  //"Move 0.1mm"
#define MSG_MOVE_1MM                        _UxGT("移動 1 mm")  //"Move 1mm"
#define MSG_MOVE_10MM                       _UxGT("移動 10 mm")  //"Move 10mm"
#define MSG_SPEED                           _UxGT("速率")  //"Speed"
#define MSG_BED_Z                           _UxGT("熱床Z")  //"Bed Z"
#define MSG_NOZZLE                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" 噴嘴")  //"Nozzle" 噴嘴
#define MSG_BED                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" 熱床")  //"Bed"
#define MSG_FAN_SPEED                       _UxGT("風扇速率")  //"Fan speed"
#define MSG_EXTRA_FAN_SPEED                 _UxGT("額外風扇速率") // "Extra fan speed"
#define MSG_FLOW                            _UxGT("擠出速率")  //"Flow"
#define MSG_CONTROL                         _UxGT("控制")  //"Control"
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" 最小") //" " LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" 最大")  //" " LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" 系數")  //" " LCD_STR_THERMOMETER " Fact"
#define MSG_AUTOTEMP                        _UxGT("自動控溫")  //"Autotemp"
#define MSG_ON                              _UxGT("開 ")  //"On "
#define MSG_OFF                             _UxGT("關 ")  //"Off"
#define MSG_PID_P                           _UxGT("PID-P")  //"PID-P"
#define MSG_PID_I                           _UxGT("PID-I")  //"PID-I"
#define MSG_PID_D                           _UxGT("PID-D")  //"PID-D"
#define MSG_PID_C                           _UxGT("PID-C")  //"PID-C"
#define MSG_SELECT                          _UxGT("選擇")  //"Select"
#define MSG_ACC                             _UxGT("加速度")  //"Accel" acceleration
#define MSG_JERK                            _UxGT("抖動速率")  //"Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("A軸抖動速率")  //"Va-jerk"
  #define MSG_VB_JERK                       _UxGT("B軸抖動速率")  //"Vb-jerk"
  #define MSG_VC_JERK                       _UxGT("C軸抖動速率")  //"Vc-jerk"
#else
  #define MSG_VA_JERK                       _UxGT("X軸抖動速率")  //"Vx-jerk"
  #define MSG_VB_JERK                       _UxGT("Y軸抖動速率")  //"Vy-jerk"
  #define MSG_VC_JERK                       _UxGT("Z軸抖動速率")  //"Vz-jerk"
#endif
#define MSG_VE_JERK                         _UxGT("擠出機抖動速率")  //"Ve-jerk"
#define MSG_VELOCITY                        _UxGT("速度")          // "Velocity"
#define MSG_VMAX                            _UxGT("最大進料速率")  //"Vmax " max_feedrate_mm_s
#define MSG_VMIN                            _UxGT("最小進料速率")  //"Vmin"  min_feedrate_mm_s
#define MSG_VTRAV_MIN                       _UxGT("最小移動速率")  //"VTrav min" min_travel_feedrate_mm_s, (target) speed of the move
#define MSG_ACCELERATION                    _UxGT("加速度")       // "Acceleration"
#define MSG_AMAX                            _UxGT("最大列印加速度")  //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
#define MSG_A_RETRACT                       _UxGT("回縮加速度")  //"A-retract" retract_acceleration, E acceleration in mm/s^2 for retracts
#define MSG_A_TRAVEL                        _UxGT("非列印移動加速度")  //"A-travel" travel_acceleration, X, Y, Z acceleration in mm/s^2 for travel (non printing) moves
#define MSG_STEPS_PER_MM                    _UxGT("軸步數/mm")  //"Steps/mm" axis_steps_per_mm, axis steps-per-unit G92
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("A軸步數/mm")  //"Asteps/mm" axis_steps_per_mm, axis steps-per-unit G92
  #define MSG_BSTEPS                        _UxGT("B軸步數/mm")  //"Bsteps/mm"
  #define MSG_CSTEPS                        _UxGT("C軸步數/mm")  //"Csteps/mm"
#else
  #define MSG_ASTEPS                        _UxGT("X軸步數/mm")  //"Xsteps/mm" axis_steps_per_mm, axis steps-per-unit G92
  #define MSG_BSTEPS                        _UxGT("Y軸步數/mm")  //"Ysteps/mm"
  #define MSG_CSTEPS                        _UxGT("Z軸步數/mm")  //"Zsteps/mm"
#endif
#define MSG_ESTEPS                          _UxGT("擠出機步數/mm")  //"Esteps/mm"
#define MSG_E1STEPS                         _UxGT("擠出機1步數/mm") //"E1steps/mm"
#define MSG_E2STEPS                         _UxGT("擠出機2步數/mm") //"E2steps/mm"
#define MSG_E3STEPS                         _UxGT("擠出機3步數/mm") //"E3steps/mm"
#define MSG_E4STEPS                         _UxGT("擠出機4步數/mm") //"E4steps/mm"
#define MSG_E5STEPS                         _UxGT("擠出機5步數/mm") //"E5steps/mm"
#define MSG_E6STEPS                         _UxGT("擠出機6步數/mm") //"E6steps/mm"
#define MSG_TEMPERATURE                     _UxGT("溫度")  //"Temperature"
#define MSG_MOTION                          _UxGT("運作")  //"Motion"
#define MSG_FILAMENT                        _UxGT("絲料測容")  //"Filament" menu_control_volumetric
#define MSG_VOLUMETRIC_ENABLED              _UxGT("測容積mm³")  //"E in mm3" volumetric_enabled
#define MSG_FILAMENT_DIAM                   _UxGT("絲料直徑")  //"Fil. Dia."
#define MSG_FILAMENT_UNLOAD                 _UxGT("卸載 mm") // "Unload mm"
#define MSG_FILAMENT_LOAD                   _UxGT("装載 mm")   // "Load mm"
#define MSG_ADVANCE_K                       _UxGT("Advance K") // "Advance K"
#define MSG_CONTRAST                        _UxGT("LCD對比度")  //"LCD contrast"
#define MSG_STORE_EEPROM                    _UxGT("保存設置")  //"Store memory"
#define MSG_LOAD_EEPROM                     _UxGT("載入設置")  //"Load memory"
#define MSG_RESTORE_FAILSAFE                _UxGT("恢復安全值")  //"Restore failsafe"
#define MSG_INIT_EEPROM                     _UxGT("初始化設置") // "Initialize EEPROM"
#define MSG_REFRESH                         _UxGT("刷新")  //"Refresh"
#define MSG_WATCH                           _UxGT("資訊界面")  //"Info screen"
#define MSG_PREPARE                         _UxGT("準備")  //"Prepare"
#define MSG_TUNE                            _UxGT("調整")  //"Tune"
#define MSG_PAUSE_PRINT                     _UxGT("暫停列印")  //"Pause print"
#define MSG_RESUME_PRINT                    _UxGT("恢復列印")  //"Resume print"
#define MSG_STOP_PRINT                      _UxGT("停止列印")  //"Stop print"
#define MSG_CARD_MENU                       _UxGT("從記憶卡上列印")  //"Print from SD"
#define MSG_NO_CARD                         _UxGT("無記憶卡")  //"No SD card"
#define MSG_DWELL                           _UxGT("休眠 ...")  //"Sleep..."
#define MSG_USERWAIT                        _UxGT("點擊繼續 ...")  //"Click to resume..."
#define MSG_PRINT_PAUSED                    _UxGT("列印已暫停") // "Print paused"
#define MSG_PRINT_ABORTED                   _UxGT("已取消列印")  //"Print aborted"
#define MSG_NO_MOVE                         _UxGT("無移動")  //"No move."
#define MSG_KILLED                          _UxGT("已砍掉")  //"KILLED. "
#define MSG_STOPPED                         _UxGT("已停止")  //"STOPPED. "
#define MSG_CONTROL_RETRACT                 _UxGT("回縮長度mm")  //"Retract mm" retract_length, retract length (positive mm)
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("換手回抽長度mm")  //"Swap Re.mm" swap_retract_length, swap retract length (positive mm), for extruder change
#define MSG_CONTROL_RETRACTF                _UxGT("回縮速率mm/s")  //"Retract  V" retract_feedrate_mm_s, feedrate for retracting (mm/s)
#define MSG_CONTROL_RETRACT_ZHOP            _UxGT("Hop mm")  //"Hop mm" retract_zraise, retract Z-lift
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("回縮恢復長度mm")  //"UnRet +mm" retract_recover_length, additional recover length (mm, added to retract length when recovering)
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("換手回縮恢復長度mm")  //"S UnRet+mm" swap_retract_recover_length, additional swap recover length (mm, added to retract length when recovering from extruder change)
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("回縮恢復後進料速率mm/s")  //"UnRet V" retract_recover_feedrate_mm_s, feedrate for recovering from retraction (mm/s)
#define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("S UnRet V") // "S UnRet V"
#define MSG_AUTORETRACT                     _UxGT("自動回縮")  //"AutoRetr." autoretract_enabled,
#define MSG_FILAMENTCHANGE                  _UxGT("更換絲料")  //"Change filament"
#define MSG_FILAMENTLOAD                    _UxGT("裝載絲料") // "Load filament"
#define MSG_FILAMENTUNLOAD                  _UxGT("卸載絲料") // "Unload filament"
#define MSG_FILAMENTUNLOAD_ALL              _UxGT("卸載全部") // "Unload All"
#define MSG_INIT_SDCARD                     _UxGT("初始化記憶卡")  //"Init. SD card"
#define MSG_CHANGE_SDCARD                   _UxGT("更換記憶卡")  //"Change SD card"
#define MSG_ZPROBE_OUT                      _UxGT("Z探針在熱床之外")  //"Z probe out. bed" Z probe is not within the physical limits
#define MSG_SKEW_FACTOR                     _UxGT("偏斜因數") // "Skew Factor"
#define MSG_BLTOUCH                         _UxGT("BLTouch")     // "BLTouch"
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch 自檢") // "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   _UxGT("重置BLTouch")  // "Reset BLTouch"
#define MSG_BLTOUCH_DEPLOY                  _UxGT("部署BLTouch") // "Deploy BLTouch"
#define MSG_BLTOUCH_STOW                    _UxGT("裝載BLTouch")   // "Stow BLTouch"
#define MSG_HOME                            _UxGT("歸位")  //"Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("先")  //"first"
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z偏移")  //"Z Offset"
#define MSG_BABYSTEP_X                      _UxGT("微量調整X軸")  //"Babystep X" lcd_babystep_x, Babystepping enables the user to control the axis in tiny amounts
#define MSG_BABYSTEP_Y                      _UxGT("微量調整Y軸")  //"Babystep Y"
#define MSG_BABYSTEP_Z                      _UxGT("微量調整Z軸")  //"Babystep Z"
#define MSG_ENDSTOP_ABORT                   _UxGT("擋塊終止")  //"Endstop abort"
#define MSG_HEATING_FAILED_LCD              _UxGT("加熱失敗")  //"Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("錯誤：REDUNDANT TEMP")  //"Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 _UxGT("溫控失控")  //"THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     _UxGT("錯誤：最高溫度")  //"Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     _UxGT("錯誤：最低溫度")  //"Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 _UxGT("錯誤：最高熱床溫度")  //"Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 _UxGT("錯誤：最低熱床溫度")  //"Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("印表機停機")  //"PRINTER HALTED"
#define MSG_PLEASE_RESET                    _UxGT("請重置")  //"Please reset"
#define MSG_SHORT_DAY                       _UxGT("天")  //"d" // One character only
#define MSG_SHORT_HOUR                      _UxGT("時")  //"h" // One character only
#define MSG_SHORT_MINUTE                    _UxGT("分")  //"m" // One character only
#define MSG_HEATING                         _UxGT("加熱中 ...")  //"Heating..."
#define MSG_BED_HEATING                     _UxGT("加熱熱床中 ...")  //"Bed Heating..."
#define MSG_DELTA_CALIBRATE                 _UxGT("⊿校準")  //"Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               _UxGT("⊿校準X")  //"Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               _UxGT("⊿校準Y")  //"Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               _UxGT("⊿校準Z")  //"Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("⊿校準中心")  //"Calibrate Center"
#define MSG_DELTA_SETTINGS                  _UxGT("⊿設置") // "Delta Settings"
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("⊿自動校準") // "Auto Calibration"
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("設置⊿高度") // "Set Delta Height"
#define MSG_DELTA_DIAG_ROD                  _UxGT("⊿斜柱") // "Diag Rod"
#define MSG_DELTA_HEIGHT                    _UxGT("⊿高度") // "Height"
#define MSG_DELTA_RADIUS                    _UxGT("⊿半徑") // "Radius"
#define MSG_INFO_MENU                       _UxGT("關於印表機")  //"About Printer"
#define MSG_INFO_PRINTER_MENU               _UxGT("印表機訊息")  //"Printer Info"
#define MSG_3POINT_LEVELING                 _UxGT("三點調平") // "3-Point Leveling"
#define MSG_LINEAR_LEVELING                 _UxGT("線性調平") // "Linear Leveling"
#define MSG_BILINEAR_LEVELING               _UxGT("雙線性調平") // "Bilinear Leveling"
#define MSG_UBL_LEVELING                    _UxGT("統一熱床調平(UBL)") // "Unified Bed Leveling"
#define MSG_MESH_LEVELING                   _UxGT("網格調平") // "Mesh Leveling"
#define MSG_INFO_STATS_MENU                 _UxGT("印表機統計")  //"Printer Stats"
#define MSG_INFO_BOARD_MENU                 _UxGT("主板訊息")  //"Board Info"
#define MSG_INFO_THERMISTOR_MENU            _UxGT("溫度計")  //"Thermistors"
#define MSG_INFO_EXTRUDERS                  _UxGT("擠出機")  //"Extruders"
#define MSG_INFO_BAUDRATE                   _UxGT("傳輸率")  //"Baud"
#define MSG_INFO_PROTOCOL                   _UxGT("協議")  //"Protocol"
#define MSG_CASE_LIGHT                      _UxGT("外殼燈") // "Case light"
#define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("燈亮度") // "Light BRIGHTNESS"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("列印計數")  //"Print Count"
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("已完成")  //"Completed"
  #define MSG_INFO_PRINT_TIME               _UxGT("總列印時間")  //"Total print time"
  #define MSG_INFO_PRINT_LONGEST            _UxGT("最長工作時間")  //"Longest job time"
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("總計擠出")  //"Extruded total"
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("列印數")  //"Prints"
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("完成")  //"Completed"
  #define MSG_INFO_PRINT_TIME               _UxGT("總共")  //"Total"
  #define MSG_INFO_PRINT_LONGEST            _UxGT("最長")  //"Longest"
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("已擠出")  //"Extruded"
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("最低溫度")  //"Min Temp"
#define MSG_INFO_MAX_TEMP                   _UxGT("最高溫度")  //"Max Temp"
#define MSG_INFO_PSU                        _UxGT("電源供應")  //"Power Supply"
#define MSG_DRIVE_STRENGTH                  _UxGT("驅動力度") // "Drive Strength"
#define MSG_DAC_PERCENT                     _UxGT("驅動 %") // "Driver %"
#define MSG_DAC_EEPROM_WRITE                _UxGT("保存驅動設置") // "DAC EEPROM Write"
#define MSG_FILAMENT_CHANGE_HEADER_PAUSE    _UxGT("列印已暫停") // "PRINT PAUSED"
#define MSG_FILAMENT_CHANGE_HEADER_LOAD     _UxGT("裝載絲料") // "LOAD FILAMENT"
#define MSG_FILAMENT_CHANGE_HEADER_UNLOAD   _UxGT("卸載絲料") // "UNLOAD FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("恢複選項:") // "RESUME OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_PURGE    _UxGT("清除更多") // "Purge more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("恢復列印")  //"Resume print"
#define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  噴嘴: ") // "  Nozzle: "
#define MSG_ERR_HOMING_FAILED               _UxGT("歸原位失敗") // "Homing failed"
#define MSG_ERR_PROBING_FAILED              _UxGT("探針探測失敗") // "Probing failed"
#define MSG_M600_TOO_COLD                   _UxGT("M600: 太涼") // "M600: Too cold"

#if LCD_HEIGHT >= 4
  #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("等待開始")  //"Wait for start"
  #define MSG_FILAMENT_CHANGE_INIT_2        _UxGT("絲料")  //"of the filament"
  #define MSG_FILAMENT_CHANGE_INIT_3        _UxGT("變更")  //"change"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("等待")  //"Wait for"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2      _UxGT("卸下絲料")  //"filament unload"
  #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("插入絲料")  //"Insert filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2      _UxGT("並按鍵")  //"and press button"
  #define MSG_FILAMENT_CHANGE_INSERT_3      _UxGT("繼續 ...")  //"to continue..."
  #define MSG_FILAMENT_CHANGE_HEAT_1        _UxGT("按下按鈕來") // "Press button to"
  #define MSG_FILAMENT_CHANGE_HEAT_2        _UxGT("加熱噴嘴.") // "heat nozzle."
  #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("加熱噴嘴") // "Heating nozzle"
  #define MSG_FILAMENT_CHANGE_HEATING_2     _UxGT("請等待 ...") // "Please wait..."
  #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("等待")  //"Wait for"
  #define MSG_FILAMENT_CHANGE_LOAD_2        _UxGT("進料")  //"filament load"
  #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("等待") // "Wait for"
  #define MSG_FILAMENT_CHANGE_PURGE_2       _UxGT("絲料清除") // "filament purge"
  #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("等待列印")  //"Wait for print"
  #define MSG_FILAMENT_CHANGE_RESUME_2      _UxGT("恢復")  //"to resume"
#else // LCD_HEIGHT < 4
  #define MSG_FILAMENT_CHANGE_INIT_1        _UxGT("請等待 ...")  //"Please wait..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1      _UxGT("退出中 ...")  //"Ejecting..."
  #define MSG_FILAMENT_CHANGE_INSERT_1      _UxGT("插入並點擊")  //"Insert and Click"
  #define MSG_FILAMENT_CHANGE_HEATING_1     _UxGT("加熱中 ...") // "Heating..."
  #define MSG_FILAMENT_CHANGE_LOAD_1        _UxGT("載入中 ...")  //"Loading..."
  #define MSG_FILAMENT_CHANGE_PURGE_1       _UxGT("清除中 ...") // "Purging..."
  #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("恢復中 ...")  //"Resuming..."
#endif // LCD_HEIGHT < 4
