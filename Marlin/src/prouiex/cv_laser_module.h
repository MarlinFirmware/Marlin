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
 * Support for Creality CV Laser module
 * Includes code from CrealityOfficial laser support repository:
 * https://github.com/CrealityOfficial/Ender-3S1/tree/ender-3s1-lasermodel
 *
 * Adapted for Pro UI by Miguel A. Risco-Castillo (MRISCOC)
 * Version: 1.2.3
 * Date: 2023/05/04
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(CV_LASER_MODULE) && DISABLED(PROUI_EX)
  #error "CV_LASER_MODULE requires PROUI_EX."
#endif

#if HAS_MEDIA
  #include "file_header.h"
  #define LASER_XMIN fileprop.minx
  #define LASER_XMAX fileprop.maxx
  #define LASER_YMIN fileprop.miny
  #define LASER_YMAX fileprop.maxy
#endif

class spindle_laser_soft_pwm
{
  public:
    static xyz_pos_t homepos;
    static bool laser_enabled;
    // float pause_before_position_x=0, pause_before_position_y=0;
    static uint8_t save_power;// 激光功率
    static void init_device();
    // 快速停止激光 等同命令 "M5 I"
    static void quick_stop();
    static bool is_laser_device();
    static bool is_fdm_device();
    //0-1000 比例转换为0-255
    static uint8_t power16_to_8(const_float_t power);
    // 设置并启动timer3 的pwm
    static void laser_power_start(const uint8_t power);
    // 将pwm占空比设置为1，以最弱的激光输出
    static void laser_power_stop();
    // 关闭激光PWM输出， 暂停timer3中断，无激光输出
    static void laser_power_close();
    //打开激光pwm，以最弱的激光输出
    static void laser_power_open();
    #if HAS_MEDIA
      static void laser_range();
    #endif
    static void laser_home();
    static void laser_set(const bool turn_on);
};

extern class spindle_laser_soft_pwm laser_device;
