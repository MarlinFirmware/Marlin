#include "Marlin.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#ifdef HYSTERESIS_PATCH
  #include "Hysteresis.h"
#endif
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

#define EEPROM_VERSION "X13"

#ifdef EEPROM_SETTINGS
void Config_StoreSettings() 
{
  char ver[4]= "000";
  int i=EEPROM_OFFSET;
  float dummyfloat = 0.0f;
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
  #else
    for (int n = 0; n < 6; n++)
    {
  	  EEPROM_WRITE_VAR(i,dummyfloat);
    }
  #endif
  #ifndef ULTIPANEL
	int plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP, plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP, plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
	int absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP, absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP, absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif
  EEPROM_WRITE_VAR(i,plaPreheatHotendTemp);
  EEPROM_WRITE_VAR(i,plaPreheatHPBTemp);
  EEPROM_WRITE_VAR(i,plaPreheatFanSpeed);
  EEPROM_WRITE_VAR(i,absPreheatHotendTemp);
  EEPROM_WRITE_VAR(i,absPreheatHPBTemp);
  EEPROM_WRITE_VAR(i,absPreheatFanSpeed);
  EEPROM_WRITE_VAR(i,zprobe_zoffset);
  #ifdef PIDTEMP
	EEPROM_WRITE_VAR(i,Kp);
	EEPROM_WRITE_VAR(i,Ki);
	EEPROM_WRITE_VAR(i,Kd);
  #else
	EEPROM_WRITE_VAR(i,dummyfloat);
	EEPROM_WRITE_VAR(i,dummyfloat);
	EEPROM_WRITE_VAR(i,dummyfloat);
  #endif
  #ifdef PIDTEMPBED
	EEPROM_WRITE_VAR(i, bedKp);
	EEPROM_WRITE_VAR(i, bedKi);
	EEPROM_WRITE_VAR(i, bedKd);
  #else
	EEPROM_WRITE_VAR(i, dummy);
	EEPROM_WRITE_VAR(i, dummy);
	EEPROM_WRITE_VAR(i, dummy);
  #endif
  #ifndef DOGLCD
    int lcd_contrast = 32;
  #endif
  EEPROM_WRITE_VAR(i,lcd_contrast);
  #ifdef SCARA
    EEPROM_WRITE_VAR(i,axis_scaling);        // Add scaling for SCARA
  #else
	EEPROM_WRITE_VAR(i,dummyfloat);
	EEPROM_WRITE_VAR(i,dummyfloat);
	EEPROM_WRITE_VAR(i,dummyfloat);
  #endif
  #ifdef HYSTERESIS_PATCH
    float h = hysteresis.GetAxis(X_AXIS);
    EEPROM_WRITE_VAR(i, h);
    h = hysteresis.GetAxis(Y_AXIS);
    EEPROM_WRITE_VAR(i, h);
    h = hysteresis.GetAxis(Z_AXIS);
    EEPROM_WRITE_VAR(i, h);
    h = hysteresis.GetAxis(E_AXIS);
    EEPROM_WRITE_VAR(i, h);
  #else
    EEPROM_WRITE_VAR(i, dummyfloat);
    EEPROM_WRITE_VAR(i, dummyfloat);
    EEPROM_WRITE_VAR(i, dummyfloat);
    EEPROM_WRITE_VAR(i, dummyfloat);
  #endif
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
#endif
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
#endif
#ifdef PIDTEMP
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("PID settings:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M301 P",Kp); 
    SERIAL_ECHOPAIR(" I" ,unscalePID_i(Ki)); 
    SERIAL_ECHOPAIR(" D" ,unscalePID_d(Kd));
    SERIAL_ECHOLN(""); 
#endif
#ifdef PIDTEMPBED
	SERIAL_ECHO_START;
	SERIAL_ECHOLNPGM("PID Bed settings:");
	SERIAL_ECHO_START;
	SERIAL_ECHOPAIR("   M304 P", bedKp);
	SERIAL_ECHOPAIR(" I", unscalePID_i(bedKi));
	SERIAL_ECHOPAIR(" D", unscalePID_d(bedKd));
	SERIAL_ECHOLN("");
#endif
#ifdef HYSTERESIS_PATCH
	SERIAL_ECHO_START;
	SERIAL_ECHOLNPGM("Hysteresis Settings:");
	hysteresis.ReportToSerial();
#endif
} 
#endif


#ifdef EEPROM_SETTINGS
void Config_RetrieveSettings()
{
    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    EEPROM_READ_VAR(i,stored_ver); //read stored version
    //  SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
    if (strncmp(ver,stored_ver,3) == 0) // version number match
    {
		// dummy variable for dumping unused parameters
		float dummyfloat = 0.0f;
		unsigned int dummyint = 0;
	
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
		#else
			EEPROM_READ_VAR(i,dummyfloat);
			EEPROM_READ_VAR(i,dummyfloat);
			EEPROM_READ_VAR(i,dummyfloat);
			EEPROM_READ_VAR(i,dummyfloat);
			EEPROM_READ_VAR(i,dummyfloat);
			EEPROM_READ_VAR(i,dummyfloat);
        #endif
        #ifdef ULTIPANEL
			EEPROM_READ_VAR(i, plaPreheatHotendTemp);
			EEPROM_READ_VAR(i, plaPreheatHPBTemp);
			EEPROM_READ_VAR(i, plaPreheatFanSpeed);
			EEPROM_READ_VAR(i, absPreheatHotendTemp);
			EEPROM_READ_VAR(i, absPreheatHPBTemp);
			EEPROM_READ_VAR(i, absPreheatFanSpeed);
			EEPROM_READ_VAR(i, zprobe_zoffset);
		#else
			EEPROM_READ_VAR(i, dummyint);
			EEPROM_READ_VAR(i, dummyint);
			EEPROM_READ_VAR(i, dummyint);
			EEPROM_READ_VAR(i, dummyint);
			EEPROM_READ_VAR(i, dummyint);
			EEPROM_READ_VAR(i, dummyint);
			EEPROM_READ_VAR(i, dummyfloat);
        #endif

        #ifdef PIDTEMP
			// do not need to scale PID values as the values in EEPROM are already scaled
			EEPROM_READ_VAR(i, Kp);
			EEPROM_READ_VAR(i, Ki);
			EEPROM_READ_VAR(i, Kd);
        #else
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
		#endif
		#ifdef PIDTEMPBED
			EEPROM_READ_VAR(i, bedKp);
			EEPROM_READ_VAR(i, bedKi);
			EEPROM_READ_VAR(i, bedKd);
		#else
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
		#endif
        #ifndef DOGLCD
        int lcd_contrast;
        #endif
        EEPROM_READ_VAR(i,lcd_contrast);
		#ifdef SCARA
			EEPROM_READ_VAR(i,axis_scaling);
		#else
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
		#endif
		#ifdef HYSTERESIS_PATCH
			float h = 0.0f;
			EEPROM_READ_VAR(i,h);
			hysteresis.SetAxis(X_AXIS, h);
			EEPROM_READ_VAR(i,h);
			hysteresis.SetAxis(Y_AXIS, h);
			EEPROM_READ_VAR(i,h);
			hysteresis.SetAxis(Z_AXIS, h);
			EEPROM_READ_VAR(i,h);
			hysteresis.SetAxis(E_AXIS, h);   
		#else
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
			EEPROM_READ_VAR(i, dummyfloat);
		#endif
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
    #endif
}
#endif

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
		#endif
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
#endif
#ifdef ULTIPANEL
    plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP;
    plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP;
    plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
    absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP;
    absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP;
    absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
#endif
#ifdef ENABLE_AUTO_BED_LEVELING
    zprobe_zoffset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
#endif
#ifdef DOGLCD
    lcd_contrast = DEFAULT_LCD_CONTRAST;
#endif
#ifdef PIDTEMP
    Kp = DEFAULT_Kp;
    Ki = scalePID_i(DEFAULT_Ki);
    Kd = scalePID_d(DEFAULT_Kd);
    
    // call updatePID (similar to when we have processed M301)
    updatePID();
    
#ifdef PID_ADD_EXTRUSION_RATE
    Kc = DEFAULT_Kc;
#endif//PID_ADD_EXTRUSION_RATE
#endif//PIDTEMP
#ifdef PIDTEMPBED
	bedKp = DEFAULT_bedKp;
	bedKi = DEFAULT_bedKi;
	bedKd = DEFAULT_bedKd;
#endif
#ifdef HYSTERESIS_PATCH
	hysteresis.Set(0.0f, 0.0f, 0.0f, 0.0f); // zero is the default
#endif
SERIAL_ECHO_START;
SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");

}
