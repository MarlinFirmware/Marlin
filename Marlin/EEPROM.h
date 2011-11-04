
#include "planner.h"
#include "temperature.h"

//======================================================================================
template <class T> int EEPROM_writeAnything(int &ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
	  EEPROM.write(ee++, *p++);
    return i;
}
//======================================================================================
template <class T> int EEPROM_readAnything(int &ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
	  *p++ = EEPROM.read(ee++);
    return i;
}
//======================================================================================

#define EEPROM_OFFSET 100

#define EEPROM_VERSION "V04"  // IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
                              // in the functions below, also increment the version number. This makes sure that
                              // the default values are used whenever there is a change to the data, to prevent
                              // wrong data being written to the variables.
                              // ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.
void StoreSettings() {
  char ver[4]= "000";
  int i=EEPROM_OFFSET;
  EEPROM_writeAnything(i,ver); // invalidate data first 
  EEPROM_writeAnything(i,axis_steps_per_unit);  
  EEPROM_writeAnything(i,max_feedrate);  
  EEPROM_writeAnything(i,max_acceleration_units_per_sq_second);
  EEPROM_writeAnything(i,acceleration);
  EEPROM_writeAnything(i,retract_acceleration);
  EEPROM_writeAnything(i,minimumfeedrate);
  EEPROM_writeAnything(i,mintravelfeedrate);
  EEPROM_writeAnything(i,minsegmenttime);
  EEPROM_writeAnything(i,max_xy_jerk);
  EEPROM_writeAnything(i,max_z_jerk);
  #ifdef PIDTEMP
  EEPROM_writeAnything(i,Kp);
  EEPROM_writeAnything(i,Ki);
  EEPROM_writeAnything(i,Kd);
#else
  EEPROM_writeAnything(i,3000);
  EEPROM_writeAnything(i,0);
  EEPROM_writeAnything(i,0);
#endif
  char ver2[4]=EEPROM_VERSION;
  i=EEPROM_OFFSET;
  EEPROM_writeAnything(i,ver2); // validate data
   ECHOLN("Settings Stored");

}

void RetrieveSettings(bool def=false){  // if def=true, the default values will be used
  int i=EEPROM_OFFSET;
  char stored_ver[4];
  char ver[4]=EEPROM_VERSION;
  EEPROM_readAnything(i,stored_ver); //read stored version
//  ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
  if ((!def)&&(strncmp(ver,stored_ver,3)==0)) {   // version number match
      EEPROM_readAnything(i,axis_steps_per_unit);  
      EEPROM_readAnything(i,max_feedrate);  
      EEPROM_readAnything(i,max_acceleration_units_per_sq_second);
      EEPROM_readAnything(i,acceleration);
      EEPROM_readAnything(i,retract_acceleration);
      EEPROM_readAnything(i,minimumfeedrate);
      EEPROM_readAnything(i,mintravelfeedrate);
      EEPROM_readAnything(i,minsegmenttime);
      EEPROM_readAnything(i,max_xy_jerk);
      EEPROM_readAnything(i,max_z_jerk);
#ifndef PIDTEMP
      float Kp,Ki,Kd;
#endif
      EEPROM_readAnything(i,Kp);
      EEPROM_readAnything(i,Ki);
      EEPROM_readAnything(i,Kd);

      ECHOLN("Stored settings retreived:");
  }
  else {
    float tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
    float tmp2[]=DEFAULT_MAX_FEEDRATE;
    long tmp3[]=DEFAULT_MAX_ACCELERATION;
    for (int i=0;i<4;i++) {
      axis_steps_per_unit[i]=tmp1[i];  
      max_feedrate[i]=tmp2[i];  
      max_acceleration_units_per_sq_second[i]=tmp3[i];
    }
    acceleration=DEFAULT_ACCELERATION;
    retract_acceleration=DEFAULT_RETRACT_ACCELERATION;
    minimumfeedrate=DEFAULT_MINIMUMFEEDRATE;
    minsegmenttime=DEFAULT_MINSEGMENTTIME;       
    mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE;
    max_xy_jerk=DEFAULT_XYJERK;
    max_z_jerk=DEFAULT_ZJERK;
    ECHOLN("Using Default settings:");
  }
  ECHOLN("Steps per unit:");
  ECHOLN("   M92 X"   <<_FLOAT(axis_steps_per_unit[0],3) << " Y" <<  _FLOAT(axis_steps_per_unit[1],3) << " Z" << _FLOAT(axis_steps_per_unit[2],3) << " E" << _FLOAT(axis_steps_per_unit[3],3));
  ECHOLN("Maximum feedrates (mm/s):");
  ECHOLN("   M203 X"  <<_FLOAT(max_feedrate[0]/60,2)<<" Y" << _FLOAT(max_feedrate[1]/60,2) << " Z" << _FLOAT(max_feedrate[2]/60,2) << " E" << _FLOAT(max_feedrate[3]/60,2));
  ECHOLN("Maximum Acceleration (mm/s2):");
  ECHOLN("   M201 X"  <<_FLOAT(max_acceleration_units_per_sq_second[0],0) << " Y" << _FLOAT(max_acceleration_units_per_sq_second[1],0) << " Z" << _FLOAT(max_acceleration_units_per_sq_second[2],0) << " E" << _FLOAT(max_acceleration_units_per_sq_second[3],0));
  ECHOLN("Acceleration: S=acceleration, T=retract acceleration");
  ECHOLN("   M204 S"  <<_FLOAT(acceleration,2) << " T" << _FLOAT(retract_acceleration,2));
  ECHOLN("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum xY jerk (mm/s),  Z=maximum Z jerk (mm/s)");
  ECHOLN("   M205 S"  <<_FLOAT(minimumfeedrate/60,2) << " T" << _FLOAT(mintravelfeedrate/60,2) << " B" << _FLOAT(minsegmenttime,2) << " X" << _FLOAT(max_xy_jerk/60,2) << " Z" << _FLOAT(max_z_jerk/60,2));
#ifdef PIDTEMP
  ECHOLN("PID settings:");
  ECHOLN("   M301 P"  << _FLOAT(Kp,3) << " I" << _FLOAT(Ki,3) << " D" << _FLOAT(Kd,3));  
#endif
  
}  


