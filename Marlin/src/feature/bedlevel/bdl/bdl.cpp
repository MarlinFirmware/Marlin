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
#include "../../../lcd/marlinui.h"

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
#define BD_SENSOR_I2C_ADDR            0x3C
I2C_SegmentBED BD_I2C_SENSOR;
float BDS_Leveling::pos_zero_offset;
int8_t BDS_Leveling::config_state;
uint8_t BDS_Leveling::homing;

void BDS_Leveling::echo_name() { SERIAL_ECHOPGM("Bed Distance Leveling"); }

void BDS_Leveling::init(uint8_t _sda, uint8_t _scl, uint16_t delay_s) {
  int ret = BD_I2C_SENSOR.i2c_init(_sda, _scl, BD_SENSOR_I2C_ADDR, delay_s);
  if (ret != 1) SERIAL_ECHOLNPGM("BD_I2C_SENSOR Init Fail return code:", ret);
  config_state = 0;
  sync_plan_position();
  pos_zero_offset=planner.get_axis_position_mm(Z_AXIS)-current_position.z;
  SERIAL_ECHOLNPGM("BD_I2C_SENSOR pos_zero_offset:", pos_zero_offset);
}

float BDS_Leveling::read() {
  const uint16_t tmp = BD_I2C_SENSOR.BD_i2c_read();
  float BD_z = NAN;
  if(BD_I2C_SENSOR.BD_Check_OddEven(tmp) == 0)
    SERIAL_ECHOLNPGM("Read Error.");
  else if((tmp & 0x3FF) >= 1015)
    SERIAL_ECHOLNPGM("Invalid data,please calibrate.");
  else if((tmp & 0x3FF) >= (MAX_BD_HEIGHT*100-10))
    SERIAL_ECHOLNPGM("Out of Range.");  
  BD_z = (tmp & 0x3FF) / 100.0f;
  return BD_z;
}

void BDS_Leveling::process() {
 //if (config_state == 0) return;
 static millis_t next_check_ms = 0; // starting at T=0
 static float z_pose = 0.0f;
 const millis_t ms = millis();
 if (ELAPSED(ms, next_check_ms)) { // timed out (or first run)
    next_check_ms = ms + (config_state < 0 ? 200 : 50);   // check at 5Hz or 20Hz

    uint16_t tmp = 0;
    const float cur_z = planner.get_axis_position_mm(Z_AXIS)-pos_zero_offset; //current_position.z
    static float old_cur_z = cur_z,
                 old_buf_z = current_position.z;
    tmp = BD_I2C_SENSOR.BD_i2c_read();
    if (BD_I2C_SENSOR.BD_Check_OddEven(tmp) && (tmp & 0x3FF) < 1016) {
      const float z_sensor = (tmp & 0x3FF) / 100.0f;
      if (cur_z < -0.5f) config_state = 0;
      //float abs_z = current_position.z > cur_z ? (current_position.z - cur_z) : (cur_z - current_position.z);
      #if ENABLED(BABYSTEPPING)
        if (cur_z < config_state * 0.1f
          && config_state > 0
          && old_cur_z == cur_z
          && old_buf_z == current_position.z
          && z_sensor < (MAX_BD_HEIGHT) - 0.1f
        ) {
          babystep.set_mm(Z_AXIS, cur_z - z_sensor);
          #if ENABLED(DEBUG_OUT_BD)
            SERIAL_ECHOLNPGM("BD:", z_sensor, ", Z:", cur_z, "|", current_position.z);
          #endif
        }
        else {
          if(config_state>0){
            babystep.set_mm(Z_AXIS, 0);
            stepper.set_directions();
          }
        }
      #endif
      old_cur_z = cur_z;
      old_buf_z = current_position.z;
      endstops.bdp_state_update(z_sensor <= 0.01f);
      //endstops.update();
      #if HAS_STATUS_MESSAGE
        static float old_z_sensor = 0;
        if (old_z_sensor != z_sensor) {
          old_z_sensor = z_sensor;
          char tmp_1[32];
          sprintf_P(tmp_1, PSTR("BD:%d.%02dmm"), int(z_sensor), int(z_sensor * 100) % 100);
          //SERIAL_ECHOLNPGM("Bed Dis:", z_sensor,"mm");
          ui.set_status(tmp_1, true);
        }
      #endif
    }
    else
      stepper.set_directions();

    #if ENABLED(DEBUG_OUT_BD)
      SERIAL_ECHOLNPGM("BD:", tmp & 0x3FF, ", Z:", cur_z, "|", current_position.z);
      if (BD_I2C_SENSOR.BD_Check_OddEven(tmp) == 0) SERIAL_ECHOLNPGM("errorCRC");
    #endif

    if ((tmp & 0x3FF) > 1016) {
      BD_I2C_SENSOR.BD_i2c_stop();
      safe_delay(10);
    }
    // Read version and Usually used as a connection check
    if (config_state == -1) {
      BD_I2C_SENSOR.BD_i2c_write(CMD_READ_VERSION);
      safe_delay(100);
      char tmp_1[21];
      for (int i = 0; i < 19; i++) {
        tmp_1[i] = BD_I2C_SENSOR.BD_i2c_read() & 0xFF;
        safe_delay(50);
      }
      config_state = 0;
      BD_I2C_SENSOR.BD_i2c_write(CMD_END_READ_CALIBRATE_DATA);
      SERIAL_ECHOLNPGM("BDsensor version:", tmp_1);
      if(tmp_1[0] != 'V')
        SERIAL_ECHOLNPGM("Read Error,check connection or delay value.");
      safe_delay(50);
    }
    // read raw calibrate data
    else if (config_state == -5) {
      BD_I2C_SENSOR.BD_i2c_write(CMD_START_READ_CALIBRATE_DATA);
      safe_delay(100);

      for (int i = 0; i < MAX_BD_HEIGHT * 10; i++) {
        tmp = BD_I2C_SENSOR.BD_i2c_read();
        SERIAL_ECHOLNPGM("Calibrate data:", i, ",", tmp & 0x3FF, ", check:", BD_I2C_SENSOR.BD_Check_OddEven(tmp));
        if(BD_I2C_SENSOR.BD_Check_OddEven(tmp) == 0)
          SERIAL_ECHOLNPGM("Read Error");
        else if((tmp & 0x3FF) >= 1015)
          SERIAL_ECHOLNPGM("Invalid data!");
        else if(((tmp & 0x3FF) > 550) && i == 0)
          SERIAL_ECHOLNPGM("BDsensor mounted too high!");
        safe_delay(50);
      }
      config_state = 0;
      BD_I2C_SENSOR.BD_i2c_write(CMD_END_READ_CALIBRATE_DATA);
      safe_delay(50);
    }
    else if (config_state <= -6) {   // Start Calibrate
      safe_delay(10);
      if (config_state == -6) {
        const millis_t old_inactive_time = gcode.stepper_inactive_time;
        gcode.stepper_inactive_time = SEC_TO_MS(60 * 5);
        SERIAL_ECHOLNPGM("c_z0:", planner.get_axis_position_mm(Z_AXIS),"-",pos_zero_offset);
        //gcode.process_subcommands_now(F("M17 Z"));
        ////////////move the z axis instead of enable the z axis with M17 
        current_position.z = 0; 
        sync_plan_position();
        gcode.process_subcommands_now(F("G90\nG1Z0.05"));
        safe_delay(1000);
        gcode.process_subcommands_now(F("G1Z0.00"));
        safe_delay(1000);
        //current_position.z = 0.05; 
        //sync_plan_position();
       // gcode.process_subcommands_now(F("G92Z0\nG1Z0.05\nG92Z0"));
        current_position.z = 0; 
        sync_plan_position();
        ////////////////
        //safe_delay(1000);
        while ((planner.get_axis_position_mm(Z_AXIS)-pos_zero_offset) > 0.00001f) {
            safe_delay(200);
            SERIAL_ECHOLNPGM("c_z6:", planner.get_axis_position_mm(Z_AXIS));
        }
        z_pose = 0.00001f;
        safe_delay(100);
        BD_I2C_SENSOR.BD_i2c_write(CMD_START_CALIBRATE); // Begin calibrate
        SERIAL_ECHOLNPGM("Begin calibrate");
        safe_delay(200);
        gcode.stepper_inactive_time = old_inactive_time;
        config_state = -7;
      }
      else if ((planner.get_axis_position_mm(Z_AXIS)-pos_zero_offset) < 10.0f) {
        if (z_pose >= MAX_BD_HEIGHT) {
          BD_I2C_SENSOR.BD_i2c_write(CMD_END_CALIBRATE); // End calibrate
          SERIAL_ECHOLNPGM("End calibrate data");
          z_pose = 7;
          config_state = 0;
          safe_delay(1000);
        }
        else {
          float tmp_k = 0;
          char tmp_1[32];
          sprintf_P(tmp_1, PSTR("G1Z%d.%d"), int(z_pose), int(z_pose * 10) % 10);
          gcode.process_subcommands_now(tmp_1);

          SERIAL_ECHO(tmp_1);
          SERIAL_ECHOLNPGM(" ,Z:", current_position.z);

          while (0.004f < abs(z_pose - tmp_k)) {
            tmp_k = planner.get_axis_position_mm(Z_AXIS)-pos_zero_offset;
            safe_delay(10);
          }
          safe_delay(100);
          if (z_pose <= 0.4f) safe_delay(500);
          tmp = (z_pose + 0.00001f) * 10;
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
