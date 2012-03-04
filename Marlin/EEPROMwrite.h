#ifndef __EEPROMH
#define __EEPROMH

#include "Marlin.h"
#include "planner.h"
#include "temperature.h"
//#include <EEPROM.h>



template <class T> int EEPROM_writeAnything(int &ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  int i;
  for (i = 0; i < (int)sizeof(value); i++)
    eeprom_write_byte((unsigned char *)ee++, *p++);
  return i;
}

template <class T> int EEPROM_readAnything(int &ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  int i;
  for (i = 0; i < (int)sizeof(value); i++)
    *p++ = eeprom_read_byte((unsigned char *)ee++);
  return i;
}
//======================================================================================




#define EEPROM_OFFSET 100


// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.
#define EEPROM_VERSION "V05"  

inline void EEPROM_StoreSettings() 
{
#ifdef EEPROM_SETTINGS
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
  EEPROM_writeAnything(i,max_e_jerk);
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
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Settings Stored");
#endif //EEPROM_SETTINGS
}


inline void EEPROM_printSettings()
{  // if def=true, the default values will be used
  #ifdef EEPROM_SETTINGS  
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("Steps per unit:");
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("  M92 X",axis_steps_per_unit[0]);
      SERIAL_ECHOPAIR(" Y",axis_steps_per_unit[1]);
      SERIAL_ECHOPAIR(" Z",axis_steps_per_unit[2]);
      SERIAL_ECHOPAIR(" E",axis_steps_per_unit[3]);
      SERIAL_ECHOLN("");
      
    SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("  M203 X",max_feedrate[0]);
      SERIAL_ECHOPAIR(" Y",max_feedrate[1] ); 
      SERIAL_ECHOPAIR(" Z", max_feedrate[2] ); 
      SERIAL_ECHOPAIR(" E", max_feedrate[3]);
      SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("  M201 X" ,max_acceleration_units_per_sq_second[0] ); 
      SERIAL_ECHOPAIR(" Y" , max_acceleration_units_per_sq_second[1] ); 
      SERIAL_ECHOPAIR(" Z" ,max_acceleration_units_per_sq_second[2] );
      SERIAL_ECHOPAIR(" E" ,max_acceleration_units_per_sq_second[3]);
      SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("Acceleration: S=acceleration, T=retract acceleration");
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("  M204 S",acceleration ); 
      SERIAL_ECHOPAIR(" T" ,retract_acceleration);
      SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum xY jerk (mm/s),  Z=maximum Z jerk (mm/s)");
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("  M205 S",minimumfeedrate ); 
      SERIAL_ECHOPAIR(" T" ,mintravelfeedrate ); 
      SERIAL_ECHOPAIR(" B" ,minsegmenttime ); 
      SERIAL_ECHOPAIR(" X" ,max_xy_jerk ); 
      SERIAL_ECHOPAIR(" Z" ,max_z_jerk);
      SERIAL_ECHOPAIR(" E" ,max_e_jerk);
      SERIAL_ECHOLN(""); 
    #ifdef PIDTEMP
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("PID settings:");
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("   M301 P",Kp); 
      SERIAL_ECHOPAIR(" I" ,Ki/PID_dT); 
      SERIAL_ECHOPAIR(" D" ,Kd*PID_dT);
      SERIAL_ECHOLN(""); 
    #endif
  #endif
} 


inline void EEPROM_RetrieveSettings(bool def=false)
{  // if def=true, the default values will be used
  #ifdef EEPROM_SETTINGS
    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    EEPROM_readAnything(i,stored_ver); //read stored version
    //  SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
    if ((!def)&&(strncmp(ver,stored_ver,3)==0)) 
    {   // version number match
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
      EEPROM_readAnything(i,max_e_jerk);
      #ifndef PIDTEMP
        float Kp,Ki,Kd;
      #endif
      EEPROM_readAnything(i,Kp);
      EEPROM_readAnything(i,Ki);
      EEPROM_readAnything(i,Kd);

      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM("Stored settings retreived:");
    }
    else 
  #endif
    {
      float tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
      float tmp2[]=DEFAULT_MAX_FEEDRATE;
      long tmp3[]=DEFAULT_MAX_ACCELERATION;
      for (short i=0;i<4;i++) 
      {
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
      max_e_jerk=DEFAULT_EJERK;
      SERIAL_ECHO_START;
      SERIAL_ECHOLN("Using Default settings:");
    }
  #ifdef EEPROM_CHITCHAT
    EEPROM_printSettings();
  #endif
}  

#endif


