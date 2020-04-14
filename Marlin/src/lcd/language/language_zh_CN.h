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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Simplified Chinese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
namespace Language_zh_CN {
  using namespace Language_en;  // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 3;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Simplified Chinese");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT("已就绪.");     //" ready."
  PROGMEM Language_Str MSG_BACK                            = _UxGT("返回");     // ”Back“
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("存储卡已插入");     //"Card inserted"
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("存储卡被拔出");     //"Card removed"
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("挡块");     //"Endstops" // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("主菜单");     //"Main"
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("自动开始");     //"Autostart"
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("关闭步进电机");     //"Disable steppers"
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("调试菜单");     // "Debug Menu"
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("进度条测试");     // "Progress Bar Test"
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("回原点");     //"Auto home"
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("回X原位");     //"Home X"
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("回Y原位");     //"Home Y"
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("回Z原位");     //"Home Z"
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("平台调平XYZ归原位");     //"Homing XYZ"
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("单击开始热床调平");     //"Click to Begin"
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("下个热床调平点");     //"Next Point"
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("完成热床调平");     //"Leveling Done!"
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("淡出高度");     // "Fade Height"
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("设置原点偏移");     //"Set home offsets"
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("偏移已启用");     //"Offsets applied"
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("设置原点");     //"Set origin"
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("预热 ") PREHEAT_1_LABEL;     //"Preheat PREHEAT_2_LABEL"
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("预热 ") PREHEAT_1_LABEL " ~";     //"Preheat PREHEAT_2_LABEL"
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("预热 ") PREHEAT_1_LABEL _UxGT(" 喷嘴");     //MSG_PREHEAT_1 " "
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("预热 ") PREHEAT_1_LABEL _UxGT(" 喷嘴 ~");     //MSG_PREHEAT_1 " "
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("预热 ") PREHEAT_1_LABEL _UxGT(" 全部");     //MSG_PREHEAT_1 " All"
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("预热 ") PREHEAT_1_LABEL _UxGT(" 热床");     //MSG_PREHEAT_1 " Bed"
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("预热 ") PREHEAT_1_LABEL _UxGT(" 设置");     //MSG_PREHEAT_1 " conf"
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("预热 ") PREHEAT_2_LABEL;     //"Preheat PREHEAT_2_LABEL"
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("预热 ") PREHEAT_2_LABEL " ~";     //"Preheat PREHEAT_2_LABEL"
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("预热 ") PREHEAT_2_LABEL _UxGT(" 喷嘴");     //MSG_PREHEAT_2 " "
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("预热 ") PREHEAT_2_LABEL _UxGT(" 喷嘴 ~");     //MSG_PREHEAT_2 " "
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("预热 ") PREHEAT_2_LABEL _UxGT(" 全部");     //MSG_PREHEAT_2 " All"
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("预热 ") PREHEAT_2_LABEL _UxGT(" 热床");     //MSG_PREHEAT_2 " Bed"
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("预热 ") PREHEAT_2_LABEL _UxGT(" 设置");     //MSG_PREHEAT_2 " conf"
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("降温");     //"Cooldown"
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("电源打开");     //"Switch power on"
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("电源关闭");     //"Switch power off"
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("挤出");     //"Extrude"
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("回抽");     //"Retract"
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("移动轴");     //"Move axis"
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("调平热床");     //"Bed leveling"
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("调平热床");     //"Level bed"
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("调平边角");     // "Level corners"

  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("下个边角");     // "Next corner"
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("编辑网格");     // "Edit Mesh"
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("网格编辑已停止");     // "Mesh Editing Stopped"
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("定制命令");     // "Custom Commands"

  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("执行G29");     // "Doing G29"
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL工具");     // "UBL Tools"
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("统一热床调平(UBL)");     // "Unified Bed Leveling"
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("手工创设网格");     // "Manually Build Mesh"
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("放置垫片并测量");     // "Place shim & measure"
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("测量");     // "Measure"
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("移除并测量热床");     // "Remove & measure bed"
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("移动到下一个");     // "Moving to next"
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("激活UBL");     // "Activate UBL"
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("关闭UBL");     // "Deactivate UBL"
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("设置热床温度");     // "Bed Temp"
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("热端温度");     // "Hotend Temp"
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("网格编辑");     // "Mesh Edit"
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("编辑客户网格");     // "Edit Custom Mesh"
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("细调网格");     // "Fine Tuning Mesh"
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("完成编辑网格");     // "Done Editing Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("创设客户网格");     // "Build Custom Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("创设网格");     // "Build Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = _UxGT("创设 ") PREHEAT_1_LABEL _UxGT(" 网格");     // "Build PREHEAT_1_LABEL Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = _UxGT("创设 ") PREHEAT_2_LABEL _UxGT(" 网格");     // "Build PREHEAT_2_LABEL Mesh"
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("创设冷网格");     // "Build Cold Mesh"
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("调整网格高度");     // "Adjust Mesh Height"
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("高度合计");     // "Height Amount"
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("批准网格");     // "Validate Mesh"
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = _UxGT("批准 ") PREHEAT_1_LABEL _UxGT(" 网格");     // "Validate PREHEAT_1_LABEL Mesh"
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = _UxGT("批准 ") PREHEAT_2_LABEL _UxGT(" 网格");     // "Validate PREHEAT_2_LABEL Mesh"
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("批准客户网格");     // "Validate Custom Mesh"
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("继续热床网格");     // "Continue Bed Mesh"
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("网格调平");     // "Mesh Leveling"
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("三点调平");     // "3-Point Leveling"
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("格子网格调平");     // "Grid Mesh Leveling"
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("调平网格");     // "Level Mesh"
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("边点");     // "Side Points"
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("图类型");     // "Map Type"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("输出网格图");     // "Output Mesh Map"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("输出到主机");     // "Output for Host"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("输出到CSV");     // "Output for CSV"
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("输出到备份");     // "Off Printer Backup"
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("输出UBL信息");     // "Output UBL Info"
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("填充合计");     // "Fill-in Amount"
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("手工填充");     // "Manual Fill-in"
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("聪明填充");     // "Smart Fill-in"
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("填充网格");     // "Fill-in Mesh"
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("作废所有的");     // "Invalidate All"
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("作废最近的");     // "Invalidate Closest"
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("细调所有的");     // "Fine Tune All"
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("细调最近的");     // "Fine Tune Closest"
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("网格存储");     // "Mesh Storage"
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("存储槽");     // "Memory Slot"
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("装载热床网格");     // "Load Bed Mesh"
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("保存热床网格");     // "Save Bed Mesh"
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 网格 %i 已装载");     // "Mesh %i loaded"
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 网格 %i 已保存");     // "Mesh %i saved"
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("没有存储");     // "No storage"
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("错误: UBL保存");     // "Err: UBL Save"
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("错误: UBL还原");     // "Err: UBL Restore"
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z偏移已停止");     // "Z-Offset Stopped"
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("一步步UBL");     // "Step-By-Step UBL"
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. 创设冷网格");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. 聪明填充");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. 批准网格");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. 细调所有的");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. 批准网格");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. 细调所有的");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. 保存热床网格");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("灯管控制");     // "LED Control")
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("灯");     // "Lights")
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("灯预置");     // "Light Presets")
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("红");     // "Red")
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("橙");     // "Orange")
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("黄");     // "Yellow")
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("绿");     // "Green")
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("蓝");     // "Blue")
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("青");     // "Indigo")
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("紫");     // "Violet")
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("白");     // "White")
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("缺省");     // "Default")
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("定制灯");     // "Custom Lights")
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("红饱和度");     // "Red Intensity")
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("绿饱和度");     // "Green Intensity")
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("蓝饱和度");     // "Blue Intensity")
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("白饱和度");     // "White Intensity")
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("亮度");     // "Brightness")

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("移动 ...");     // "Moving...")
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("释放 XY");     // "Free XY")
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("移动X");     //"Move X"
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("移动Y");     //"Move Y"
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("移动Z");     //"Move Z"
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("挤出机");     //"Extruder"
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("挤出机 *");     //"Extruder"
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("移动 %s mm");     //"Move 0.025mm"
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("移动 0.1 mm");     //"Move 0.1mm"
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("移动 1 mm");     //"Move 1mm"
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("移动 10 mm");     //"Move 10mm"
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("速率");     //"Speed"
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("热床Z");     //"Bed Z"
  PROGMEM Language_Str MSG_NOZZLE                          = " " LCD_STR_THERMOMETER _UxGT(" 喷嘴");     //"Nozzle" 噴嘴
  PROGMEM Language_Str MSG_NOZZLE_N                        = " " LCD_STR_THERMOMETER _UxGT(" 喷嘴 ~");     //"Nozzle" 噴嘴
  PROGMEM Language_Str MSG_BED                             = " " LCD_STR_THERMOMETER _UxGT(" 热床");     //"Bed"
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("风扇速率");     //"Fan speed"
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("风扇速率 ~");     //"Fan speed"
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("额外风扇速率");     // "Extra fan speed"
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("额外风扇速率 ~");     // "Extra fan speed"
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("挤出速率");     //"Flow"
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("挤出速率 ~");     //"Flow"
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("控制");     //"Control"
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" 最小");     //" " LCD_STR_THERMOMETER " Min"
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" 最大");     //" " LCD_STR_THERMOMETER " Max"
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" 因数");     //" " LCD_STR_THERMOMETER " Fact"
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("自动控温");     //"Autotemp"
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("开");     //"On"
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("关");     //"Off"
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("选择");     //"Select"
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("选择 *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("加速度");     //"Accel" acceleration
  PROGMEM Language_Str MSG_JERK                            = _UxGT("抖动速率");     // "Jerk"
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("轴抖动速率") LCD_STR_A;     //"Va-jerk"
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("轴抖动速率") LCD_STR_B;     //"Vb-jerk"
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("轴抖动速率") LCD_STR_C;     //"Vc-jerk"
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("挤出机抖动速率");     //"Ve-jerk"
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("速度");     // "Velocity"
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("最大进料速率") LCD_STR_A;     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("最大进料速率") LCD_STR_B;     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("最大进料速率") LCD_STR_C;     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("最大进料速率") LCD_STR_E;     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("最大进料速率 *");     //"Vmax " max_feedrate_mm_s
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("最小进料速率");     //"Vmin"  min_feedrate_mm_s
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("最小移动速率");     //"VTrav min" min_travel_feedrate_mm_s, (target) speed of the move
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("加速度");     // "Acceleration"
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("最大打印加速度") LCD_STR_A;     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("最大打印加速度") LCD_STR_B;     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("最大打印加速度") LCD_STR_C;     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("最大打印加速度") LCD_STR_E;     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("最大打印加速度 *");     //"Amax " max_acceleration_mm_per_s2, acceleration in units/s^2 for print moves
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("收进加速度");     //"A-retract" retract_acceleration, E acceleration in mm/s^2 for retracts
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("非打印移动加速度");     //"A-travel" travel_acceleration, X, Y, Z acceleration in mm/s^2 for travel (non printing) moves
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("轴步数/mm");     //"Steps/mm" axis_steps_per_mm, axis steps-per-unit G92
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("轴步数/mm");     //"Asteps/mm"
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("轴步数/mm");     //"Bsteps/mm"
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("轴步数/mm");     //"Csteps/mm"
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("挤出机步数/mm");     //"Esteps/mm"
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("挤出机~步数/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("温度");     //"Temperature"
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("运动");     //"Motion"
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("丝料测容");     //"Filament" menu_advanced_filament
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("测容积mm³");     //"E in mm3" volumetric_enabled
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("丝料直径");     //"Fil. Dia."
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("丝料直径 *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("卸载 mm");     // "Unload mm"
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("装载 mm");     // "Load mm"
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD对比度");     //"LCD contrast"
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("保存设置");     //"Store memory"
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("装载设置");     //"Load memory"
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("恢复安全值");     //"Restore Defaults"
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("初始化设置");     // "Initialize EEPROM"
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("刷新");     //"Refresh"
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("信息屏");     //"Info screen"
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("准备");     //"Prepare"
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("调整");     //"Tune"
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("暂停打印");     //"Pause print"
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("恢复打印");     //"Resume print"
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("停止打印");     //"Stop print"
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("从存储卡上打印");     //"Print from SD"
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("无存储卡");     //"No SD card"
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("休眠中 ...");     //"Sleep..."
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("点击继续 ...");     //"Click to resume..."
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("暫停打印");     // "Print paused"
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("已取消打印");     //"Print aborted"
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("无移动");     //"No move."
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("已杀掉");     //"KILLED. "
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("已停止");     //"STOPPED. "
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("回抽长度mm");     //"Retract mm" retract_length, retract length (positive mm)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("换手回抽长度mm");     //"Swap Re.mm" swap_retract_length, swap retract length (positive mm), for extruder change
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("回抽速率mm/s");     //"Retract  V" retract_feedrate_mm_s, feedrate for retracting (mm/s)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Hop mm");     //"Hop mm" retract_zraise, retract Z-lift
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("回抽恢复长度mm");     //"UnRet +mm" retract_recover_extra, additional recover length (mm, added to retract length when recovering)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("换手回抽恢复长度mm");     //"S UnRet+mm" swap_retract_recover_extra, additional swap recover length (mm, added to retract length when recovering from extruder change)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("回抽恢复后进料速率mm/s");     //"UnRet  V" retract_recover_feedrate_mm_s, feedrate for recovering from retraction (mm/s)
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");     // "S UnRet V"
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("自动抽回");     //"AutoRetr." autoretract_enabled,
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("更换丝料");     //"Change filament"
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("更换丝料 *");     //"Change filament"
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("装载丝料");     // "Load filament"
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("装载丝料 *");     // "Load filament"
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("卸载丝料");     // "Unload filament"
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("卸载丝料 *");     // "Unload filament"
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("卸载全部");     // "Unload All"
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("初始化存储卡");     //"Init. SD card"
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("更换存储卡");     //"Change SD card"
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z探针在热床之外");     //"Z probe out. bed" Z probe is not within the physical limits
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("偏斜因数");     // "Skew Factor"
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");     // "BLTouch"
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch 自检");     // "BLTouch Self-Test"
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("重置BLTouch");     // "Reset BLTouch"
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("部署BLTouch");     // "Deploy BLTouch"
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("装载BLTouch");     // "Stow BLTouch"
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("归位 %s%s%s 先");     //"Home ... first"
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Z偏移");     //"Z Offset"
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("微量调整X轴");     //"Babystep X" lcd_babystep_x, Babystepping enables the user to control the axis in tiny amounts
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("微量调整Y轴");     //"Babystep Y"
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("微量调整Z轴");     //"Babystep Z"
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("挡块终止");     //"Endstop abort"
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("加热失败");     //"Heating failed"
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("错误：REDUNDANT TEMP");     //"Err: REDUNDANT TEMP"
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("温控失控");     //"THERMAL RUNAWAY"
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("错误：最高温度");     //"Err: MAXTEMP"
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("错误：最低温度");     //"Err: MINTEMP"
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("错误：最高热床温度");     //"Err: MAXTEMP BED"
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("错误：最低热床温度");     //"Err: MINTEMP BED"
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("归位 XY 先");     // "Home XY First"
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("打印停机");     //"PRINTER HALTED"
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("请重置");     //"Please reset"
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("天");     //"d" // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("时");     //"h" // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("分");     //"m" // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("加热中 ...");     //"Heating..."
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("加热热床中 ...");     //"Bed Heating..."
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("⊿校准");     //"Delta Calibration"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("⊿校准X");     //"Calibrate X"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("⊿校准Y");     //"Calibrate Y"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("⊿校准Z");     //"Calibrate Z"
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("⊿校准中心");     //"Calibrate Center"
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("⊿设置");     // "Delta Settings"
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("⊿自动校准");     // "Auto Calibration"
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("设置⊿高度");     // "Set Delta Height"
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("⊿斜柱");     // "Diag Rod"
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("⊿高度");     // "Height"
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("⊿半径");     // "Radius"
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("关于打印机");     //"About Printer"
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("打印机信息");     //"Printer Info"
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("三点调平");     // "3-Point Leveling"
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("线性调平");     // "Linear Leveling"
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("双线性调平");     // "Bilinear Leveling"
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("统一热床调平(UBL)");     // "Unified Bed Leveling"
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("网格调平");     // "Mesh Leveling"
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("打印机统计");     //"Printer Stats"
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("主板信息");     //"Board Info"
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("温度计");     //"Thermistors"
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("挤出机");     //"Extruders"
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("波特率");     //"Baud"
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("协议");     //"Protocol"
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("外壳灯");     // "Case light"
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("灯亮度");     // "Light BRIGHTNESS"

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("打印机不正确");     // "The printer is incorrect"

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("打印计数");     //"Print Count"
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("完成了");     //"Completed"
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("总打印时间");     //"Total print time"
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("最长工作时间");     //"Longest job time"
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("总计挤出");     //"Extruded total"
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("打印数");     //"Prints"
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("完成");     //"Completed"
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("总共");     //"Total"
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("最长");     //"Longest"
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("已挤出");     //"Extruded"
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("最低温度");     //"Min Temp"
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("最高温度");     //"Max Temp"
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("电源供应");     //"Power Supply"
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("驱动力度");     // "Drive Strength"
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X 驱动 %");     // "X Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y 驱动 %");     // "Y Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z 驱动 %");     // "Z Driver %"
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E 驱动 %");     // "E Driver %"
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("保存驱动设置");     // "DAC EEPROM Write"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("打印已暂停");     // "PRINT PAUSED"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("装载丝料");     // "LOAD FILAMENT"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("卸载丝料");     // "UNLOAD FILAMENT"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("恢复选项:");     // "RESUME OPTIONS:"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("清除更多");     // "Purge more"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("恢复打印");     //"Resume print"
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  喷嘴: ");     // "  Nozzle: "
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("归原位失败");     // "Homing failed"
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("探针探测失败");     // "Probing failed"
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: 太凉");     // "M600: Too cold"

  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("等待开始", "丝料", "变更"));     // "Wait for start of the filament change"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("等待", "卸下丝料"));     // "Wait for filament unload"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("插入丝料", "并按键", "来继续 ..."));     // "Insert filament and press button to continue..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("按下按钮来", "加热喷嘴."));     // "Press button to heat nozzle."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("加热喷嘴", "请等待 ..."));     // "Heating nozzle Please wait..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("等待", "进料"));     // "Wait for filament load"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("等待", "丝料清除"));     // "Wait for filament purge"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_2_LINE("等待打印", "恢复"));     // "Wait for print to resume"
  #else
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("请等待 ..."));     //"Please wait..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("退出中 ..."));     //"Ejecting..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("插入并单击"));     //"Insert and Click"
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("加热中 ..."));     // "Heating..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("装载中 ..."));     //"Loading..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("清除中 ..."));     // "Purging..."
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("恢复中 ..."));     //"Resuming..."
  #endif
}
