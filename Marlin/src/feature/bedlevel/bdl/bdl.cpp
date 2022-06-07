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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(BD_SENSOR)

#include "../../../MarlinCore.h"
#include "../../../gcode/gcode.h"
#include "../../../module/settings.h"
#include "../../../module/planner.h"
#include "../../../module/motion.h"
#include "../../../module/probe.h"
#include "../../../module/temperature.h"
#include "../../../module/endstops.h"
#include "../../babystep.h"
#include "../../../module/stepper/indirection.h"
#include "../../../module/stepper.h"

//I2C software Master library for segment bed heating and bed distance sensor
#include <Panda_segmentBed_I2C.h>

#include "bdl.h"
#define DEBUG_OUT_BD 1
Bed_Distance_sensor_level BD_Level; 

//M102   S-5     Read raw Calibrate data
//M102   S-6     Start Calibrate 
//M102   S4      Set the adjustable Z height value,e.g. M102 S4  means it will do adjusting while the Z height <=0.4mm , disable it by M102 S0.
//M102   S-1     Read sensor information

#define MAX_BD_HEIGHT 6.9
#define CMD_START_READ_CALIBRATE_DATA   1017
#define CMD_END_READ_CALIBRATE_DATA   1018
#define CMD_START_CALIBRATE 1019
#define CMD_END_CALIBRATE 1021  



I2C_SegmentBED BD_I2C_SENSOR;


void Bed_Distance_sensor_level::echo_name() { SERIAL_ECHOPGM("unified bed leveling"); }

void Bed_Distance_sensor_level::init(unsigned char _sda,unsigned char _scl,int delay_s) {

  int ret=BD_I2C_SENSOR.i2c_init(_sda,_scl,0x3C,delay_s);
  if(ret!=1)
    SERIAL_ECHOLNPGM("BD_I2C_SENSOR Init Fail return code:",ret);
  BDsensor_config=0;
}

void Bed_Distance_sensor_level::BD_sensor_process(void){
 static millis_t timeout_auto=0;
 static float z_pose=0.0;
 int timeout_y=100;
 float z_sensor=0; 
 //if(BDsensor_config==0)
 // return;
 if(BDsensor_config<0)
    timeout_y=1000;
 if((millis()-timeout_auto)>timeout_y){
    unsigned short tmp=0;
    char tmp_1[50];
    float cur_z=planner.get_axis_position_mm(Z_AXIS);//current_position.z
    static float old_cur_z=cur_z;
    static float old_buf_z=current_position.z;
   
    tmp=BD_I2C_SENSOR.BD_i2c_read();      
    if(BD_I2C_SENSOR.BD_Check_OddEven(tmp)&&(tmp&0x3ff)<1020){
      z_sensor=(tmp&0x3ff)/100.0;
      if(cur_z<0)
        BDsensor_config=0;
      //float abs_z=current_position.z>cur_z?(current_position.z-cur_z):(cur_z-current_position.z);
      if(cur_z<(BDsensor_config/10.0)&&(BDsensor_config>0)
        &&(old_cur_z==cur_z)&&(old_buf_z==current_position.z)&&(z_sensor<(MAX_BD_HEIGHT))){
          
          babystep.set_mm(Z_AXIS,(cur_z-z_sensor));
#if DEBUG_OUT_BD          
          SERIAL_ECHOLNPGM("BD:",z_sensor,", Z:",cur_z,"|",current_position.z);
#endif          
      }
      else{
        babystep.set_mm(Z_AXIS,0);
      //  if(old_cur_z<=cur_z)
        //   Z_DIR_WRITE(!INVERT_Z_DIR);
        stepper.set_directions();
      }
      old_cur_z=cur_z;
      old_buf_z=current_position.z;
      endstops.BD_Zaxis_update(z_sensor<=0.01);
      //endstops.update();
      
    }
    else
      stepper.set_directions();
#if DEBUG_OUT_BD      
    SERIAL_ECHOLNPGM("BD:",tmp&0x3ff,", Z:",cur_z,"|",current_position.z);
    if(BD_I2C_SENSOR.BD_Check_OddEven(tmp)==0)
      SERIAL_ECHOLNPGM("errorCRC");        
#endif    
    if((tmp&0x3ff)>1020){
      BD_I2C_SENSOR.BD_i2c_stop();
      safe_delay(10);
    }
    // read raw calibrate data
    if(BDsensor_config==-5){
      BD_I2C_SENSOR.BD_i2c_write(CMD_START_READ_CALIBRATE_DATA);
      safe_delay(1000);
         
      for(int i=0;i<MAX_BD_HEIGHT*10;i++){
        tmp=BD_I2C_SENSOR.BD_i2c_read();    
        SERIAL_ECHOLNPGM("Calibrate data:",i,",",tmp&0x3ff,", check:",BD_I2C_SENSOR.BD_Check_OddEven(tmp));
        safe_delay(500);
      }
      BDsensor_config=0; 
      BD_I2C_SENSOR.BD_i2c_write(CMD_END_READ_CALIBRATE_DATA);//
        safe_delay(500);
    }
    // start Calibrate
    else if(BDsensor_config<=-6){
      safe_delay(100);     
      if(BDsensor_config==-6){
       
       // BD_I2C_SENSOR.BD_i2c_write(1019);// begain calibrate
       // delay(1000);
        gcode.stepper_inactive_time = SEC_TO_MS(60*5);
        sprintf_P(tmp_1,  PSTR("M17 Z"));
        parser.parse(tmp_1);
        gcode.process_parsed_command();
        sprintf_P(tmp_1,  PSTR("G1 Z0.0"));
        parser.parse(tmp_1);
        gcode.process_parsed_command();        
        z_pose=0;
        safe_delay(1000);
        BD_I2C_SENSOR.BD_i2c_write(CMD_START_CALIBRATE);// begain calibrate //
        SERIAL_ECHOLNPGM("Begain calibrate");
        safe_delay(2000);
        BDsensor_config=-7;
      }
      else if(planner.get_axis_position_mm(Z_AXIS)<10.0){        
        if(z_pose>=MAX_BD_HEIGHT){
            BD_I2C_SENSOR.BD_i2c_write(CMD_END_CALIBRATE); // end calibrate  //
            SERIAL_ECHOLNPGM("End Calibrate data");
            z_pose=7;
            BDsensor_config=0;
            safe_delay(1000);
        }
        else{
          float tmp_k=0;
          sprintf_P(tmp_1,  PSTR("G1 Z%d.%d"), (int)z_pose,(int)((int)(z_pose*10)%10));
          parser.parse(tmp_1);
          gcode.process_parsed_command();
        //  current_position.z
          SERIAL_ECHOPGM(tmp_1);
          SERIAL_ECHOLNPGM(" ,Z:",current_position.z);
          while((tmp_k+0.1)<z_pose){
            tmp_k=planner.get_axis_position_mm(Z_AXIS);
            safe_delay(1);
          }
          safe_delay(800);
          tmp=(z_pose+0.0001)*10;
          BD_I2C_SENSOR.BD_i2c_write(tmp);
          SERIAL_ECHOLNPGM("w:",tmp,",Zpose:",z_pose);
          z_pose+=0.1001;
         // queue.enqueue_now_P(PSTR("G90"));
        }
      }
       
    }
   timeout_auto=millis();  
 }
}

#endif // BD_SENSOR
