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

//*************简体中文***********************//
#define TOOL_TEXT_CN                "工具"
#define PREHEAT_TEXT_CN             "预热"
#define MOVE_TEXT_CN                "移动"
#define HOME_TEXT_CN                "回零"
#define PRINT_TEXT_CN               "打印"
#define EXTRUDE_TEXT_CN             "挤出"
#define LEVELING_TEXT_CN            "调平"
#define AUTO_LEVELING_TEXT_CN       "自动调平"
#define SET_TEXT_CN                 "设置"
#define MORE_TEXT_CN                "更多"

#define ADD_TEXT_CN                 "增加"
#define DEC_TEXT_CN                 "减少"
#define EXTRUDER_1_TEXT_CN          "喷头1"
#define EXTRUDER_2_TEXT_CN          "喷头2"
#define HEATBED_TEXT_CN             "热床"
#define TEXT_1C_CN                  "1℃"
#define TEXT_5C_CN                  "5℃"
#define TEXT_10C_CN                 "10℃"
#define CLOSE_TEXT_CN               "关闭"

#define BACK_TEXT_CN                "返回"

#define TOOL_PREHEAT_CN             "预热"
#define TOOL_EXTRUDE_CN             "挤出"
#define TOOL_MOVE_CN                "移动"
#define TOOL_HOME_CN                "回零"
#define TOOL_LEVELING_CN            "调平"
#define TOOL_AUTO_LEVELING_CN       "自动调平"
#define TOOL_FILAMENT_CN            "换料"
#define TOOL_MORE_CN                "更多"

#define AXIS_X_ADD_TEXT_CN          "X+"
#define AXIS_X_DEC_TEXT_CN          "X-"
#define AXIS_Y_ADD_TEXT_CN          "Y+"
#define AXIS_Y_DEC_TEXT_CN          "Y-"
#define AXIS_Z_ADD_TEXT_CN          "Z+"
#define AXIS_Z_DEC_TEXT_CN          "Z-"
#define TEXT_01MM_CN                "0.1mm"
#define TEXT_1MM_CN                 "1mm"
#define TEXT_10MM_CN                "10mm"

#define HOME_X_TEXT_CN              "X"
#define HOME_Y_TEXT_CN              "Y"
#define HOME_Z_TEXT_CN              "Z"
#define HOME_ALL_TEXT_CN            "回零"
#define HOME_STOPMOVE_CN            "急停"

#define PAGE_UP_TEXT_CN             "上一页"
#define PAGE_DOWN_TEXT_CN           "下一页"

#define EXTRUDER_IN_TEXT_CN         "进料"
#define EXTRUDER_OUT_TEXT_CN        "退料"
#define EXTRUDE_1MM_TEXT_CN         "1mm"
#define EXTRUDE_5MM_TEXT_CN         "5mm"
#define EXTRUDE_10MM_TEXT_CN        "10mm"
#define EXTRUDE_LOW_SPEED_TEXT_CN   "低速"
#define EXTRUDE_MEDIUM_SPEED_TEXT_CN "常速"
#define EXTRUDE_HIGH_SPEED_TEXT_CN  "高速"

#define LEVELING_POINT1_TEXT_CN   "第一点"
#define LEVELING_POINT2_TEXT_CN   "第二点"
#define LEVELING_POINT3_TEXT_CN   "第三点"
#define LEVELING_POINT4_TEXT_CN   "第四点"
#define LEVELING_POINT5_TEXT_CN   "第五点"

#define FILESYS_TEXT_CN           "文件系统"
#define WIFI_TEXT_CN              "WIFI"
#define FAN_TEXT_CN               "风扇"
#define ABOUT_TEXT_CN             "关于"
#define BREAK_POINT_TEXT_CN       "断点续打"
#define FILAMENT_TEXT_CN          "换料"
#define LANGUAGE_TEXT_CN          "语言"
#define MOTOR_OFF_TEXT_CN         "关闭电机"
#define MOTOR_OFF_XY_TEXT_CN      "关闭XY"
#define SHUTDOWN_TEXT_CN          "关机"

#define U_DISK_TEXT_CN            "U盘"
#define SD_CARD_TEXT_CN           "SD卡"
#define WIFI_NAME_TEXT_CN         "无线网络:"
#define WIFI_KEY_TEXT_CN          "密码: "
#define WIFI_IP_TEXT_CN           "IP: "
#define WIFI_AP_TEXT_CN           "状态: AP"
#define WIFI_STA_TEXT_CN          "状态: STA"
#define WIFI_CONNECTED_TEXT_CN    "已连接"
#define WIFI_DISCONNECTED_TEXT_CN "未连接"
#define WIFI_EXCEPTION_TEXT_CN    "模块异常"
#define CLOUD_TEXT_CN             "云服务"
#define CLOUD_BIND_CN             "已绑定"
#define CLOUD_UNBIND_CN           "解绑"
#define CLOUD_UNBINDING_CN        "解绑中"
#define CLOUD_DISCONNECTED_CN     "未连接"
#define CLOUD_UNBINDED_CN         "未绑定"
#define CLOUD_BINDED_CN           "已绑定"
#define CLOUD_DISABLE_CN          "已禁用"

#define FAN_ADD_TEXT_CN           "增加"
#define FAN_DEC_TEXT_CN           "减少"
#define FAN_OPEN_TEXT_CN          "100%"
#define FAN_HALF_TEXT_CN          "50%"
#define FAN_CLOSE_TEXT_CN         "关闭"
#define FAN_TIPS1_TEXT_CN         "风扇"
#define FAN_TIPS2_TEXT_CN         "FAN\nClose"

#define FILAMENT_IN_TEXT_CN       "进料"
#define FILAMENT_OUT_TEXT_CN      "退料"
#define FILAMENT_EXT0_TEXT_CN     "喷头1"
#define FILAMENT_EXT1_TEXT_CN     "喷头2"
#define FILAMENT_HEAT_TEXT_CN     "预热"
#define FILAMENT_STOP_TEXT_CN     "停止"
#if 0
  #define FILAMENT_REPLAYS_IDLE_TEXT_CN "请按<进料>或<退料>进行换料!"
  #define FILAMENT_CHANGE_TEXT_CN   "待打印机暂停后,请按<进料>或<退料>进行换料!"
#else
  #define FILAMENT_CHANGE_TEXT_CN   "待打印机暂停后,\n请按<进料>或<退料>"
#endif

#define FILAMENT_DIALOG_LOAD_HEAT_TIPS_CN       "准备进料,正在加热,请稍等!"
#define FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_CN     "准备退料,正在加热,请稍等!"
#define FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_CN   "加热完成,请装载耗材后,按<确定>开始进料!"
#define FILAMENT_DIALOG_LOAD_CONFIRM2_TIPS_CN   "请装载耗材,按<确定>开始进料!"
#define FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_CN  "加热完成,请按<确定>开始退料!"
#define FILAMENT_DIALOG_LOADING_TIPS_CN         "正在进料,请等待耗材加载完成!"
#define FILAMENT_DIALOG_UNLOADING_TIPS_CN       "正在退料,请等待耗材卸载完成!"
#define FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_CN   "进料完成,请按<确定>返回"
#define FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_CN "退料完成,请按<确定>返回"

#define FILAMENT_TIPS3_TEXT_CN      "正在进料"
#define FILAMENT_TIPS4_TEXT_CN      "正在退料"
#define FILAMENT_TIPS5_TEXT_CN      "温度太低,请先预热"
#define FILAMENT_TIPS6_TEXT_CN      "换料完成"

#define PRE_HEAT_EXT_TEXT_CN        "喷头"
#define PRE_HEAT_BED_TEXT_CN        "热床"

#define FILE_LOADING_CN             "正在载入......"
#define NO_FILE_AND_CHECK_CN        "无文件!请插入sd卡或u盘!"
#define NO_FILE_CN                  "无文件!"

#define EXTRUDER_TEMP_TEXT_CN       "温度"
#define EXTRUDER_E_LENGTH1_TEXT_CN  "喷头"
#define EXTRUDER_E_LENGTH2_TEXT_CN  "喷头"
#define EXTRUDER_E_LENGTH3_TEXT_CN  "喷头"

#define ABOUT_TYPE_TEXT_CN          "Type: "
#define ABOUT_VERSION_TEXT_CN       "Firmware: "
#define ABOUT_WIFI_TEXT_CN          "Wifi: "

#define PRINTING_OPERATION_CN       "操作"
#define PRINTING_PAUSE_CN           "暂停"
#define PRINTING_TEMP_CN            "温度"
#define PRINTING_CHANGESPEED_CN     "变速"
#define PRINTING_RESUME_CN          "恢复"
#define PRINTING_STOP_CN            "停止"
#define PRINTING_MORE_CN            "更多"
#define PRINTING_EXTRUDER_CN        "挤出"
#define PRINTING_MOVE_CN            "移动"

#define EXTRUDER_SPEED_CN           "挤出"
#define MOVE_SPEED_CN               "移动"
#define EXTRUDER_SPEED_STATE_CN     "挤出速度"
#define MOVE_SPEED_STATE_CN         "移动速度"
#define STEP_1PERCENT_CN            "1%"
#define STEP_5PERCENT_CN            "5%"
#define STEP_10PERCENT_CN           "10%"

#define ZOFFSET_CN                  "Z Offset"
#define ZOFFSET_INC_CN              "增加"
#define ZOFFSET_DEC_CN              "减少"

#define TITLE_READYPRINT_CN         "准备打印"
#define TITLE_PREHEAT_CN            "预热"
#define TITLE_MOVE_CN               "移动"
#define TITLE_HOME_CN               "回零"
#define TITLE_EXTRUDE_CN            "挤出"
#define TITLE_LEVELING_CN           "调平"
#define TITLE_SET_CN                "设置"
#define TITLE_MORE_CN               "更多"
#define TITLE_CHOOSEFILE_CN         "选择文件"
#define TITLE_PRINTING_CN           "正在打印"
#define TITLE_OPERATION_CN          "操作"
#define TITLE_ADJUST_CN             "调整"
#define TITLE_WIRELESS_CN           "无线网络"
#define TITLE_FILAMENT_CN           "换料"
#define TITLE_ABOUT_CN              "关于"
#define TITLE_FAN_CN                "风扇"
#define TITLE_LANGUAGE_CN           "语言"
#define TITLE_PAUSE_CN              "暂停"
#define TITLE_CHANGESPEED_CN        "变速"
#define TITLE_CLOUD_TEXT_CN         "云服务"
#define TITLE_DIALOG_CONFIRM_CN     "确认"
#define TITLE_FILESYS_CN            "文件系统"
#define TITLE_ZOFFSET_CN            "Z Offset"

#define AUTO_SHUTDOWN_CN            "自动关机"
#define MANUAL_SHUTDOWN_CN          "手动关机"

#define DIALOG_CONFIRM_CN           "确定"
#define DIALOG_CANCLE_CN            "取消"
#define DIALOG_OK_CN                "确认"
#define DIALOG_RESET_CN             "重置"
#define DIALOG_DISABLE_CN           "禁用"
#define DIALOG_PRINT_MODEL_CN       "打印模型?"
#define DIALOG_CANCEL_PRINT_CN      "停止打印?"
#define DIALOG_RETRY_CN             "重试"
#define DIALOG_STOP_CN              "停止"
#define DIALOG_REPRINT_FROM_BREAKPOINT_CN   "从断点续打?"
//#define DIALOG_UNBIND_PRINTER_CN  "解除绑定 ?"
#define DIALOG_ERROR_TIPS1_CN       "错误:找不到文件,请插入sd卡/u盘!"
#define DIALOG_ERROR_TIPS2_CN       "错误:通信失败,请检查波特率或主板硬件!"
#define DIALOG_ERROR_TIPS3_CN       "错误:文件名或文件路径太长 !"
#define DIALOG_CLOSE_MACHINE_CN     "正在关机......"
#define DIALOG_UNBIND_PRINTER_CN    "解除绑定?"
#define DIALOG_FILAMENT_NO_PRESS_CN "请先装载耗材!"
#define DIALOG_PRINT_FINISH_CN      "打印完成!"
#define DIALOG_PRINT_TIME_CN        "打印时间: "
#define DIALOG_REPRINT_CN           "再打印一次"
#define DIALOG_WIFI_ENABLE_TIPS_CN  "wifi模块正在配置中,请稍等......"

#define TEXT_VALUE_CN               "%d℃/%d℃"
#define EXTRUDE_TEXT_VALUE_T_CN     ": %d℃"
#define WIFI_RECONNECT_TEXT_CN      "重新连接"

#define PRINTING_GBK                "正在打印"
#define PRINTING_OPERATION_GBK      "操作"
#define PRINTING_PAUSE_GBK          "暂停"

#define MESSEGE_PAUSING_CN          "暂停中..."
#define MESSEGE_CHANGING_CN         "等待换料开始..."
#define MESSEGE_UNLOAD_CN           "退料中,请稍等..."
#define MESSEGE_WAITING_CN          "点击按钮恢复打印"
#define MESSEGE_INSERT_CN           "装载耗材后,点击按钮开始打印"
#define MESSEGE_LOAD_CN             "进料中,请稍等..."
#define MESSEGE_PURGE_CN            "等待挤出..."
#define MESSEGE_RESUME_CN           "等待恢复打印..."
#define MESSEGE_HEAT_CN             "按下按钮,加热喷头"
#define MESSEGE_HEATING_CN          "喷头加热中,请等待..."
#define MESSEGE_OPTION_CN           "挤出更多还是继续打印?"
#define MESSEGE_PURGE_MORE_CN       "挤出"
#define MESSEGE_CONTINUE_PRINT_CN   "打印"
