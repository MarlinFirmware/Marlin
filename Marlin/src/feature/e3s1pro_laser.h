/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * feature/e3s1pro_laser.h
 */

#include "../inc/MarlinConfig.h"

#include "spindle_laser_types.h"

#if ALL(E3S1PRO_RTS, IIC_BL24CXX_EEPROM)
  #include "../libs/BL24CXX.h"
#endif

enum device_header {
  DEVICE_UNKNOWN = 0xFF,  // unknown device
  DEVICE_LASER = 1,       // laser device
  DEVICE_FDM,             // fdm device
};

enum laser_device_range {
  LASER_MIN_X = 0,
  LASER_MIN_Y,
  LASER_MAX_X,
  LASER_MAX_Y,
};

// #include "../HAL/STM32F1/timers.h"
#include HAL_PATH(.., timers.h)

class spindle_laser_soft_pwm {
private:
  device_header current_device = DEVICE_UNKNOWN;
  bool need_read_gcode_range = false;
  float laser_range[4] = { 0.0f };

public:
  const char laser_cmp_info[4][6] = { "MINX:", "MINY:", "MAXX:", "MAXY:" };
  xy_pos_t pause_before_position = { 0, 0 };
  bool remove_card_before_is_printing = false;
  bool already_show_warning = false;
  bool laser_printing = false;
  uint16_t remain_time = 0;
  double laser_z_axis_high = 0;
  uint8_t power = 0;
  bool is_run_range = false;

  void quick_stop() {
    cutter.set_enabled(false);
    cutter.cutter_mode = CUTTER_MODE_STANDARD;
    cutter.apply_power(0);
  }

  void get_device_from_eeprom() {
    uint8_t buff[2] = { 0 };
    BL24CXX::read(LASER_FDM_ADDR, &buff[0], 1);
    //SERIAL_ECHOLNPGM("get_device_from_eeprom", buff[0]);
    const device_header dh = (device_header)buff[0];
    if (dh == DEVICE_LASER || dh == DEVICE_FDM)
      current_device = dh;
    else
      current_device = DEVICE_UNKNOWN;
  }

  double get_z_axis_high_from_eeprom() {
    uint8_t buff[3] = { 0 };
    uint16_t data = 0;
    BL24CXX::read(LASER_Z_AXIS_HIGH_ADDR, &buff[0], 2);

    data = buff[0];
    data = (data << 8) | buff[1];

    NOMORE(data, LASER_Z_AXIS_HIGH_MAX * 100);

    laser_z_axis_high = data / 100.0f;

    return laser_z_axis_high;
  }

  void save_device_to_eeprom() {
    BL24CXX::write(LASER_FDM_ADDR, (uint8_t *)&current_device, 1);
    //SERIAL_ECHOLNPGM("save_device_to_eeprom", buff[0]);
  }

  void save_z_axis_high_to_eeprom(float data) {
    uint8_t buff[3] = { 0 };
    uint16_t high = _MIN(data * 100, LASER_Z_AXIS_HIGH_MAX * 100);

    buff[0] = (high >> 8) & 0xFF;
    buff[1] = high & 0xFF;

    BL24CXX::write(LASER_Z_AXIS_HIGH_ADDR, &buff[0], 2);
    laser_z_axis_high = data;
  }

  device_header get_current_device() {
    return current_device;
  }

  void set_current_device(device_header dev) {
    current_device = dev;
    save_device_to_eeprom();
  }

  bool is_laser_device() {
    return current_device == DEVICE_LASER;
  }

  bool is_unknown_device() {
    return current_device == DEVICE_UNKNOWN;
  }

  void set_read_gcode_range_on() {
    need_read_gcode_range = true;
  }

  void set_read_gcode_range_off() {
    need_read_gcode_range = false;
  }

  bool is_read_gcode_range_on() {
    return need_read_gcode_range==true;
  }

  void set_laser_range(laser_device_range index, float value) {
    laser_range[index] = value;
  }

  float get_laser_range(laser_device_range index) {
    return laser_range[index];
  }

  uint8_t power16_to_8(uint16_t power) {
    if (power == 0) return 0;
    uint8_t p = (uint8_t)((power / 1000.0f) * 255.0f);
    NOLESS(p, 1);
    return p;
  }

  void reset_data() {
    laser_range[LASER_MIN_X] = 0.0;
    laser_range[LASER_MIN_Y] = 0.0;
    laser_range[LASER_MAX_X] = 0.0;
    laser_range[LASER_MAX_Y] = 0.0;
  }

  void soft_pwm_init() {
    _SET_OUTPUT(LASER_SOFT_PWM_PIN);
    laser_timer_soft_pwm_init(LASER_TIMER_FREQUENCY);
  }

  void laser_power_start(const uint8_t power) { //0 - 255
    //SERIAL_ECHOLNPGM("_laser.h 424 =", power);
    laser_timer_soft_pwm_start(power);
  }

  void laser_power_stop() {
    laser_timer_soft_pwm_stop();
  }

  void laser_power_close() {
    laser_timer_soft_pwm_close();
  }

  void laser_power_open() {
    laser_power_start(1);
  }
};

extern class spindle_laser_soft_pwm laser_device;
