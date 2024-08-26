/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(BD_SENSOR)

#include "../../../MarlinCore.h"
#include "../../../gcode/gcode.h"
#include "../../../module/settings.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"
#include "../../../module/probe.h"
#include "../../../module/temperature.h"
#include "../../../module/endstops.h"
#include "../../babystep.h"

// I2C software Master library for segment bed heating and bed distance sensor
#include <Panda_segmentBed_I2C.h>

#include "bdl.h"
BDS_Leveling bdl;

//#define DEBUG_OUT_BD

// M102 S-5   Read raw Calibrate data
// M102 S-6   Start Calibrate
// M102 S4    Set the adjustable Z height value (e.g., 'M102 S4' means it will do adjusting while the Z height <= 0.4mm , disable with 'M102 S0'.)
// M102 S-1   Read sensor information

#define MAX_BD_HEIGHT                 4.0f
#define CMD_START_READ_CALIBRATE_DATA 1017
#define CMD_END_READ_CALIBRATE_DATA   1018
#define CMD_START_CALIBRATE           1019
#define CMD_END_CALIBRATE             1021
#define CMD_READ_VERSION  1016

I2C_SegmentBED BD_I2C_SENSOR;

#define BD_SENSOR_I2C_ADDR            0x3C

int8_t BDS_Leveling::config_state;
uint8_t BDS_Leveling::homing;

void BDS_Leveling::echo_name() { SERIAL_ECHOPGM("Bed Distance Leveling"); }

void BDS_Leveling::init(uint8_t _sda, uint8_t _scl, uint16_t delay_s) {
  int ret = BD_I2C_SENSOR.i2c_init(_sda, _scl, BD_SENSOR_I2C_ADDR, delay_s);
  if (ret != 1) SERIAL_ECHOLNPGM("BD_I2C_SENSOR Init Fail return code:", ret);
  config_state = 0;
}

float BDS_Leveling::read() {
  const uint16_t tmp = BD_I2C_SENSOR.BD_i2c_read();
  float BD_z = NAN;
  if (BD_I2C_SENSOR.BD_Check_OddEven(tmp) && (tmp & 0x3FF) < 1020)
    BD_z = (tmp & 0x3FF) / 100.0f;
  return BD_z;
}

void BDS_Leveling::process() {
 //if (config_state == 0) return;
 static millis_t next_check_ms = 0; // starting at T=0
 static float z_pose = 0.0f;
 const millis_t ms = millis();
 if (ELAPSED(ms, next_check_ms)) { // timed out (or first run)
    next_check_ms = ms + (config_state < 0 ? 1000 : 100);   // check at 1Hz or 10Hz

    unsigned short tmp = 0;
    const float cur_z = planner.get_axis_position_mm(Z_AXIS); //current_position.z
    static float old_cur_z = cur_z,
                 old_buf_z = current_position.z;

    tmp = BD_I2C_SENSOR.BD_i2c_read();
    if (BD_I2C_SENSOR.BD_Check_OddEven(tmp) && (tmp & 0x3FF) < 1020) {
      const float z_sensor = (tmp & 0x3FF) / 100.0f;
      if (cur_z < 0) config_state = 0;
      //float abs_z = current_position.z > cur_z ? (current_position.z - cur_z) : (cur_z - current_position.z);
      #if ENABLED(BABYSTEPPING)
        if (cur_z < config_state * 0.1f
          && config_state > 0
          && old_cur_z == cur_z
          && old_buf_z == current_position.z
          && z_sensor < (MAX_BD_HEIGHT)
        ) {
          babystep.set_mm(Z_AXIS, cur_z - z_sensor);
          #if ENABLED(DEBUG_OUT_BD)
            SERIAL_ECHOLNPGM("BD:", z_sensor, ", Z:", cur_z, "|", current_position.z);
          #endif
        }
        else {
          babystep.set_mm(Z_AXIS, 0);          //if (old_cur_z <= cur_z) Z_DIR_WRITE(!INVERT_Z_DIR);
          stepper.set_directions();
        }
      #endif
      old_cur_z = cur_z;
      old_buf_z = current_position.z;
      endstops.bdp_state_update(z_sensor <= 0.01f);
      //endstops.update();
    }
    else
      stepper.set_directions();

    #if ENABLED(DEBUG_OUT_BD)
      SERIAL_ECHOLNPGM("BD:", tmp & 0x3FF, ", Z:", cur_z, "|", current_position.z);
      if (BD_I2C_SENSOR.BD_Check_OddEven(tmp) == 0) SERIAL_ECHOLNPGM("errorCRC");
    #endif

    if ((tmp & 0x3FF) > 1020) {
      BD_I2C_SENSOR.BD_i2c_stop();
      safe_delay(10);
    }

    // read raw calibrate data
    if (config_state == -5) {
      BD_I2C_SENSOR.BD_i2c_write(CMD_START_READ_CALIBRATE_DATA);
      safe_delay(1000);

      for (int i = 0; i < MAX_BD_HEIGHT * 10; i++) {
        tmp = BD_I2C_SENSOR.BD_i2c_read();
        SERIAL_ECHOLNPGM("Calibrate data:", i, ",", tmp & 0x3FF, ", check:", BD_I2C_SENSOR.BD_Check_OddEven(tmp));
        safe_delay(500);
      }
      config_state = 0;
      BD_I2C_SENSOR.BD_i2c_write(CMD_END_READ_CALIBRATE_DATA);
      safe_delay(500);
    }
    else if (config_state <= -6) {   // Start Calibrate
      safe_delay(100);
      if (config_state == -6) {
        //BD_I2C_SENSOR.BD_i2c_write(1019); // begin calibrate
        //delay(1000);
        gcode.stepper_inactive_time = SEC_TO_MS(60 * 5);
        gcode.process_subcommands_now(F("M17 Z"));
        gcode.process_subcommands_now(F("G1 Z0.0"));
        z_pose = 0;
        safe_delay(1000);
        BD_I2C_SENSOR.BD_i2c_write(CMD_START_CALIBRATE); // Begin calibrate
        SERIAL_ECHOLNPGM("Begin calibrate");
        safe_delay(2000);
        config_state = -7;
      }
      else if (planner.get_axis_position_mm(Z_AXIS) < 10.0f) {
        if (z_pose >= MAX_BD_HEIGHT) {
          BD_I2C_SENSOR.BD_i2c_write(CMD_END_CALIBRATE); // End calibrate
          SERIAL_ECHOLNPGM("End calibrate data");
          z_pose = 7;
          config_state = 0;
          safe_delay(1000);
        }
        else {
          float tmp_k = 0;
          char tmp_1[30];
          sprintf_P(tmp_1, PSTR("G1 Z%d.%d"), int(z_pose), int(int(z_pose * 10) % 10));
          gcode.process_subcommands_now(tmp_1);

          SERIAL_ECHO(tmp_1);
          SERIAL_ECHOLNPGM(" ,Z:", current_position.z);

          while (tmp_k < (z_pose - 0.1f)) {
            tmp_k = planner.get_axis_position_mm(Z_AXIS);
            safe_delay(1);
          }
          safe_delay(800);
          tmp = (z_pose + 0.0001f) * 10;
          BD_I2C_SENSOR.BD_i2c_write(tmp);
          SERIAL_ECHOLNPGM("w:", tmp, ",Zpose:", z_pose);
          z_pose += 0.1001f;
          //queue.enqueue_now_P(PSTR("G90"));
        }
      }
    }
  }
}

#endif // BD_SENSOR
