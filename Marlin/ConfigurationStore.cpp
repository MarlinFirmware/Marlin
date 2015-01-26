#include "Marlin.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "ConfigurationStore.h"

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size)
{
    do
    {
        eeprom_write_byte((unsigned char*)pos, *value);
        pos++;
        value++;
    }while(--size);
}
#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size)
{
    do
    {
        *value = eeprom_read_byte((unsigned char*)pos);
        pos++;
        value++;
    }while(--size);
}
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))
//======================================================================================




#define EEPROM_OFFSET 100


// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.

#define EEPROM_VERSION "V14"

#ifdef EEPROM_SETTINGS
void Config_StoreSettings() 
{
  char ver[4]= "000";
  int i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver); // invalidate data first 
  EEPROM_WRITE_VAR(i,axis_steps_per_unit);
  EEPROM_WRITE_VAR(i,max_feedrate);  
  EEPROM_WRITE_VAR(i,max_acceleration_units_per_sq_second);
  EEPROM_WRITE_VAR(i,acceleration);
  EEPROM_WRITE_VAR(i,retract_acceleration);
  EEPROM_WRITE_VAR(i,minimumfeedrate);
  EEPROM_WRITE_VAR(i,mintravelfeedrate);
  EEPROM_WRITE_VAR(i,minsegmenttime);
  EEPROM_WRITE_VAR(i,max_xy_jerk);
  EEPROM_WRITE_VAR(i,max_z_jerk);
  EEPROM_WRITE_VAR(i,max_e_jerk);
  EEPROM_WRITE_VAR(i,add_homing);
  #ifdef DELTA
  EEPROM_WRITE_VAR(i,endstop_adj);
  EEPROM_WRITE_VAR(i,delta_radius);
  EEPROM_WRITE_VAR(i,delta_diagonal_rod);
  EEPROM_WRITE_VAR(i,delta_segments_per_second);
  #endif//DELTA
  #ifndef ULTIPANEL
  int plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP, plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP, plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
  int absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP, absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP, absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif//ULTIPANEL
  EEPROM_WRITE_VAR(i,plaPreheatHotendTemp);
  EEPROM_WRITE_VAR(i,plaPreheatHPBTemp);
  EEPROM_WRITE_VAR(i,plaPreheatFanSpeed);
  EEPROM_WRITE_VAR(i,absPreheatHotendTemp);
  EEPROM_WRITE_VAR(i,absPreheatHPBTemp);
  EEPROM_WRITE_VAR(i,absPreheatFanSpeed);
  EEPROM_WRITE_VAR(i,zprobe_zoffset);
  #ifdef PIDTEMP
    float dummy = 0.0f;
    for (int e = 0; e < 4; e++)
	{
	  if (e < EXTRUDERS)
	  {
        EEPROM_WRITE_VAR(i,PID_PARAM(Kp,e));
        EEPROM_WRITE_VAR(i,PID_PARAM(Ki,e));
        EEPROM_WRITE_VAR(i,PID_PARAM(Kd,e));
        #ifdef PID_ADD_EXTRUSION_RATE
        EEPROM_WRITE_VAR(i,PID_PARAM(Kc,e));
        #else//PID_ADD_EXTRUSION_RATE
		dummy = 1.0f; // 1.0 = default kc
	    EEPROM_WRITE_VAR(dummmy);
        #endif//PID_ADD_EXTRUSION_RATE
	  }
	  else
	  {
		dummy = 3000.0f;
	    EEPROM_WRITE_VAR(i, dummy);
		dummy = 0.0f;
        EEPROM_WRITE_VAR(i,dummy);
        EEPROM_WRITE_VAR(i,dummy);
	  }
	}
  #else//PIDTEMP
		float dummy = 3000.0f;
    EEPROM_WRITE_VAR(i,dummy);
		dummy = 0.0f;
    EEPROM_WRITE_VAR(i,dummy);
    EEPROM_WRITE_VAR(i,dummy);
  #endif//PIDTEMP
  #ifndef DOGLCD
    int lcd_contrast = 32;
  #endif//DOGLCD
  EEPROM_WRITE_VAR(i,lcd_contrast);
  #ifdef SCARA
  EEPROM_WRITE_VAR(i,axis_scaling);        // Add scaling for SCARA
  #endif//SCARA
  #ifdef FWRETRACT
  EEPROM_WRITE_VAR(i,autoretract_enabled);
  EEPROM_WRITE_VAR(i,retract_length);
  #if EXTRUDERS > 1
  EEPROM_WRITE_VAR(i,retract_length_swap);
  #endif//EXTRUDERS > 1
  EEPROM_WRITE_VAR(i,retract_feedrate);
  EEPROM_WRITE_VAR(i,retract_zlift);
  EEPROM_WRITE_VAR(i,retract_recover_length);
  #if EXTRUDERS > 1
  EEPROM_WRITE_VAR(i,retract_recover_length_swap);
  #endif//EXTRUDERS > 1
  EEPROM_WRITE_VAR(i,retract_recover_feedrate);
  #endif//FWRETRACT

  // Save filament sizes
  EEPROM_WRITE_VAR(i, volumetric_enabled);
  EEPROM_WRITE_VAR(i, filament_size[0]);
#if EXTRUDERS > 1
  EEPROM_WRITE_VAR(i, filament_size[1]);
#if EXTRUDERS > 2
  EEPROM_WRITE_VAR(i, filament_size[2]);
#if EXTRUDERS > 3
  EEPROM_WRITE_VAR(i, filament_size[3]);
#endif //EXTRUDERS > 3
#endif //EXTRUDERS > 2
#endif //EXTRUDERS > 1
  
  char ver2[4]=EEPROM_VERSION;
  i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver2); // validate data
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Settings Stored");
}
#endif //EEPROM_SETTINGS


#ifndef DISABLE_M503
void Config_PrintSettings()
{  // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Steps per unit:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M92 X",axis_steps_per_unit[X_AXIS]);
    SERIAL_ECHOPAIR(" Y",axis_steps_per_unit[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z",axis_steps_per_unit[Z_AXIS]);
    SERIAL_ECHOPAIR(" E",axis_steps_per_unit[E_AXIS]);
    SERIAL_ECHOLN("");
      
    SERIAL_ECHO_START;
#ifdef SCARA
SERIAL_ECHOLNPGM("Scaling factors:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M365 X",axis_scaling[X_AXIS]);
    SERIAL_ECHOPAIR(" Y",axis_scaling[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z",axis_scaling[Z_AXIS]);
    SERIAL_ECHOLN("");
      
    SERIAL_ECHO_START;
#endif//SCARA
    SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M203 X", max_feedrate[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", max_feedrate[Y_AXIS]); 
    SERIAL_ECHOPAIR(" Z", max_feedrate[Z_AXIS]); 
    SERIAL_ECHOPAIR(" E", max_feedrate[E_AXIS]);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M201 X" ,max_acceleration_units_per_sq_second[X_AXIS] ); 
    SERIAL_ECHOPAIR(" Y" , max_acceleration_units_per_sq_second[Y_AXIS] ); 
    SERIAL_ECHOPAIR(" Z" ,max_acceleration_units_per_sq_second[Z_AXIS] );
    SERIAL_ECHOPAIR(" E" ,max_acceleration_units_per_sq_second[E_AXIS]);
    SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Acceleration: S=acceleration, T=retract acceleration");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M204 S",acceleration ); 
    SERIAL_ECHOPAIR(" T" ,retract_acceleration);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum XY jerk (mm/s),  Z=maximum Z jerk (mm/s),  E=maximum E jerk (mm/s)");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M205 S",minimumfeedrate ); 
    SERIAL_ECHOPAIR(" T" ,mintravelfeedrate ); 
    SERIAL_ECHOPAIR(" B" ,minsegmenttime ); 
    SERIAL_ECHOPAIR(" X" ,max_xy_jerk ); 
    SERIAL_ECHOPAIR(" Z" ,max_z_jerk);
    SERIAL_ECHOPAIR(" E" ,max_e_jerk);
    SERIAL_ECHOLN(""); 

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Home offset (mm):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M206 X",add_homing[X_AXIS] );
    SERIAL_ECHOPAIR(" Y" ,add_homing[Y_AXIS] );
    SERIAL_ECHOPAIR(" Z" ,add_homing[Z_AXIS] );
    SERIAL_ECHOLN("");
#ifdef DELTA
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Endstop adjustement (mm):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M666 X",endstop_adj[X_AXIS] );
    SERIAL_ECHOPAIR(" Y" ,endstop_adj[Y_AXIS] );
    SERIAL_ECHOPAIR(" Z" ,endstop_adj[Z_AXIS] );
	SERIAL_ECHOLN("");
	SERIAL_ECHO_START;
	SERIAL_ECHOLNPGM("Delta settings: L=delta_diagonal_rod, R=delta_radius, S=delta_segments_per_second");
	SERIAL_ECHO_START;
	SERIAL_ECHOPAIR("  M665 L",delta_diagonal_rod );
	SERIAL_ECHOPAIR(" R" ,delta_radius );
	SERIAL_ECHOPAIR(" S" ,delta_segments_per_second );
	SERIAL_ECHOLN("");
#endif//DELTA
#ifdef PIDTEMP
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("PID settings:");
	SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M301 P", PID_PARAM(Kp,0)); // for compatibility with hosts, only echos values for E0
	SERIAL_ECHOPAIR(" I", unscalePID_i(PID_PARAM(Ki, 0)));
	SERIAL_ECHOPAIR(" D", unscalePID_d(PID_PARAM(Kd, 0)));
    SERIAL_ECHOLN(""); 
#endif//PIDTEMP
#ifdef FWRETRACT
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Retract: S=Length (mm) F:Speed (mm/m) Z: ZLift (mm)");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M207 S",retract_length); 
    SERIAL_ECHOPAIR(" F" ,retract_feedrate*60); 
    SERIAL_ECHOPAIR(" Z" ,retract_zlift);
    SERIAL_ECHOLN(""); 
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Recover: S=Extra length (mm) F:Speed (mm/m)");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M208 S",retract_recover_length);
    SERIAL_ECHOPAIR(" F", retract_recover_feedrate*60);
	SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Auto-Retract: S=0 to disable, 1 to interpret extrude-only moves as retracts or recoveries");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M209 S", (unsigned long)(autoretract_enabled ? 1 : 0));
    SERIAL_ECHOLN("");
#if EXTRUDERS > 1
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Multi-extruder settings:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   Swap retract length (mm):    ", retract_length_swap);
    SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   Swap rec. addl. length (mm): ", retract_recover_length_swap);
    SERIAL_ECHOLN("");
#endif//EXTRUDERS > 1
    SERIAL_ECHO_START;
    if (volumetric_enabled) {
        SERIAL_ECHOLNPGM("Filament settings:");
        SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   M200 D", filament_size[0]);
        SERIAL_ECHOLN(""); 
#if EXTRUDERS > 1
		SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   M200 T1 D", filament_size[1]);
        SERIAL_ECHOLN(""); 
#if EXTRUDERS > 2
		SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   M200 T2 D", filament_size[2]);
		SERIAL_ECHOLN("");
#if EXTRUDERS > 3
        SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   M200 T3 D", filament_size[3]);
        SERIAL_ECHOLN("");
#endif //EXTRUDERS > 3
#endif //EXTRUDERS > 2
#endif //EXTRUDERS > 1
    } else {
        SERIAL_ECHOLNPGM("Filament settings: Disabled");
    }
#endif//FWRETRACT
#ifdef CUSTOM_M_CODES
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Z-Probe Offset (mm):");
  SERIAL_ECHO_START;
  SERIAL_ECHO("   M");
  SERIAL_ECHO(CUSTOM_M_CODE_SET_Z_PROBE_OFFSET);
  SERIAL_ECHOPAIR(" Z",-zprobe_zoffset);
  SERIAL_ECHOLN("");
#endif
}
#endif//DISABLE_M503


#ifdef EEPROM_SETTINGS
void Config_RetrieveSettings()
{
    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    EEPROM_READ_VAR(i,stored_ver); //read stored version
    //  SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
    if (strncmp(ver,stored_ver,3) == 0)
    {
        // version number match
        EEPROM_READ_VAR(i,axis_steps_per_unit);
        EEPROM_READ_VAR(i,max_feedrate);  
        EEPROM_READ_VAR(i,max_acceleration_units_per_sq_second);
        
        // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
		reset_acceleration_rates();
        
        EEPROM_READ_VAR(i,acceleration);
        EEPROM_READ_VAR(i,retract_acceleration);
        EEPROM_READ_VAR(i,minimumfeedrate);
        EEPROM_READ_VAR(i,mintravelfeedrate);
        EEPROM_READ_VAR(i,minsegmenttime);
        EEPROM_READ_VAR(i,max_xy_jerk);
        EEPROM_READ_VAR(i,max_z_jerk);
        EEPROM_READ_VAR(i,max_e_jerk);
        EEPROM_READ_VAR(i,add_homing);
        #ifdef DELTA
		EEPROM_READ_VAR(i,endstop_adj);
		EEPROM_READ_VAR(i,delta_radius);
		EEPROM_READ_VAR(i,delta_diagonal_rod);
		EEPROM_READ_VAR(i,delta_segments_per_second);
        #endif//DELTA
        #ifndef ULTIPANEL
        int plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed;
        int absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed;
        #endif//ULTIPANEL
        EEPROM_READ_VAR(i,plaPreheatHotendTemp);
        EEPROM_READ_VAR(i,plaPreheatHPBTemp);
        EEPROM_READ_VAR(i,plaPreheatFanSpeed);
        EEPROM_READ_VAR(i,absPreheatHotendTemp);
        EEPROM_READ_VAR(i,absPreheatHPBTemp);
        EEPROM_READ_VAR(i,absPreheatFanSpeed);
        EEPROM_READ_VAR(i,zprobe_zoffset);
        #ifdef PIDTEMP
		float dummy = 0.0f;
		for (int e = 0; e < 4; e++) // 4 = max extruders supported by marlin
		{
		  if (e < EXTRUDERS)
		  {
		    // do not need to scale PID values as the values in EEPROM are already scaled			  
            EEPROM_READ_VAR(i,PID_PARAM(Kp,e));
            EEPROM_READ_VAR(i,PID_PARAM(Ki,e));
		    EEPROM_READ_VAR(i,PID_PARAM(Kd,e));
#ifdef PID_ADD_EXTRUSION_RATE
            EEPROM_READ_VAR(i,PID_PARAM(Kc,e));
#else//PID_ADD_EXTRUSION_RATE
	        EEPROM_READ_VAR(i,dummy);
#endif//PID_ADD_EXTRUSION_RATE
		  }
		  else
		  {
			EEPROM_READ_VAR(i,dummy);
			EEPROM_READ_VAR(i,dummy);
			EEPROM_READ_VAR(i,dummy);
			EEPROM_READ_VAR(i,dummy);
		  }
		}
		#else//PIDTEMP
		// 4 x 3 = 12 slots for PID parameters
		float dummy = 0.0f;
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);
		EEPROM_READ_VAR(i,dummy);			
		#endif//PIDTEMP
        #ifndef DOGLCD
        int lcd_contrast;
        #endif//DOGLCD
        EEPROM_READ_VAR(i,lcd_contrast);
		#ifdef SCARA
		EEPROM_READ_VAR(i,axis_scaling);
		#endif//SCARA

		#ifdef FWRETRACT
		EEPROM_READ_VAR(i,autoretract_enabled);
		EEPROM_READ_VAR(i,retract_length);
		#if EXTRUDERS > 1
		EEPROM_READ_VAR(i,retract_length_swap);
		#endif//EXTRUDERS > 1
		EEPROM_READ_VAR(i,retract_feedrate);
		EEPROM_READ_VAR(i,retract_zlift);
		EEPROM_READ_VAR(i,retract_recover_length);
		#if EXTRUDERS > 1
		EEPROM_READ_VAR(i,retract_recover_length_swap);
		#endif//EXTRUDERS > 1
		EEPROM_READ_VAR(i,retract_recover_feedrate);
		#endif//FWRETRACT

		EEPROM_READ_VAR(i, volumetric_enabled);
		EEPROM_READ_VAR(i, filament_size[0]);
#if EXTRUDERS > 1
		EEPROM_READ_VAR(i, filament_size[1]);
#if EXTRUDERS > 2
		EEPROM_READ_VAR(i, filament_size[2]);
#if EXTRUDERS > 3
    EEPROM_READ_VAR(i, filament_size[3]);
#endif //EXTRUDERS > 3
#endif //EXTRUDERS > 2
#endif //EXTRUDERS > 1
		calculate_volumetric_multipliers();
		// Call updatePID (similar to when we have processed M301)
		updatePID();
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Stored settings retrieved");
    }
    else
    {
        Config_ResetDefault();
    }
    #ifdef EEPROM_CHITCHAT
      Config_PrintSettings();
    #endif//EEPROM_CHITCHAT
}
#endif//EEPROM_SETTINGS

void Config_ResetDefault()
{
    float tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
    float tmp2[]=DEFAULT_MAX_FEEDRATE;
    long tmp3[]=DEFAULT_MAX_ACCELERATION;
    for (short i=0;i<4;i++) 
    {
        axis_steps_per_unit[i]=tmp1[i];  
        max_feedrate[i]=tmp2[i];  
        max_acceleration_units_per_sq_second[i]=tmp3[i];
		#ifdef SCARA
		axis_scaling[i]=1;
		#endif//SCARA
    }
    
    // steps per sq second need to be updated to agree with the units per sq second
    reset_acceleration_rates();
    
    acceleration=DEFAULT_ACCELERATION;
    retract_acceleration=DEFAULT_RETRACT_ACCELERATION;
    minimumfeedrate=DEFAULT_MINIMUMFEEDRATE;
    minsegmenttime=DEFAULT_MINSEGMENTTIME;       
    mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE;
    max_xy_jerk=DEFAULT_XYJERK;
    max_z_jerk=DEFAULT_ZJERK;
    max_e_jerk=DEFAULT_EJERK;
    add_homing[X_AXIS] = add_homing[Y_AXIS] = add_homing[Z_AXIS] = 0;
#ifdef DELTA
	endstop_adj[X_AXIS] = endstop_adj[Y_AXIS] = endstop_adj[Z_AXIS] = 0;
	delta_radius= DELTA_RADIUS;
	delta_diagonal_rod= DELTA_DIAGONAL_ROD;
	delta_segments_per_second= DELTA_SEGMENTS_PER_SECOND;
	recalc_delta_settings(delta_radius, delta_diagonal_rod);
#endif//DELTA
#ifdef ULTIPANEL
    plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP;
    plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP;
    plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
    absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP;
    absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP;
    absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
#endif//ULTIPANEL
#ifdef ENABLE_AUTO_BED_LEVELING
    zprobe_zoffset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
#endif//ENABLE_AUTO_BED_LEVELING
#ifdef DOGLCD
    lcd_contrast = DEFAULT_LCD_CONTRAST;
#endif//DOGLCD
#ifdef PIDTEMP
#ifdef PID_PARAMS_PER_EXTRUDER
	for (int e = 0; e < EXTRUDERS; e++)
#else // PID_PARAMS_PER_EXTRUDER
	int e = 0; // only need to write once
#endif // PID_PARAMS_PER_EXTRUDER
	{
      PID_PARAM(Kp,e) = DEFAULT_Kp;
      PID_PARAM(Ki,e) = scalePID_i(DEFAULT_Ki);
      PID_PARAM(Kd,e) = scalePID_d(DEFAULT_Kd);
      #ifdef PID_ADD_EXTRUSION_RATE
        PID_PARAM(Kc,e) = DEFAULT_Kc;
      #endif//PID_ADD_EXTRUSION_RATE
    }
    // call updatePID (similar to when we have processed M301)
    updatePID();
#endif//PIDTEMP

#ifdef FWRETRACT
	autoretract_enabled = false;
	retract_length = RETRACT_LENGTH;
#if EXTRUDERS > 1
	retract_length_swap = RETRACT_LENGTH_SWAP;
#endif//EXTRUDERS > 1
	retract_feedrate = RETRACT_FEEDRATE;
	retract_zlift = RETRACT_ZLIFT;
	retract_recover_length = RETRACT_RECOVER_LENGTH;
#if EXTRUDERS > 1
	retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
#endif//EXTRUDERS > 1
	retract_recover_feedrate = RETRACT_RECOVER_FEEDRATE;
#endif//FWRETRACT

	volumetric_enabled = false;
	filament_size[0] = DEFAULT_NOMINAL_FILAMENT_DIA;
#if EXTRUDERS > 1
	filament_size[1] = DEFAULT_NOMINAL_FILAMENT_DIA;
#if EXTRUDERS > 2
	filament_size[2] = DEFAULT_NOMINAL_FILAMENT_DIA;
#if EXTRUDERS > 3
  filament_size[3] = DEFAULT_NOMINAL_FILAMENT_DIA;
#endif //EXTRUDERS > 3
#endif //EXTRUDERS > 2
#endif //EXTRUDERS > 1
	calculate_volumetric_multipliers();

SERIAL_ECHO_START;
SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");

}
