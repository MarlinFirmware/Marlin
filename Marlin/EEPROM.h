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

#define EEPROM_VERSION "V03"  // IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
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
  char ver2[4]=EEPROM_VERSION;
  i=EEPROM_OFFSET;
  EEPROM_writeAnything(i,ver2); // validate data
 //  Serial.println("Settings Stored");

}

void RetrieveSettings(bool def=false){  // if def=true, the default values will be used
  int i=EEPROM_OFFSET;
  char stored_ver[4];
  char ver[4]=EEPROM_VERSION;
  EEPROM_readAnything(i,stored_ver); //read stored version
//  Serial.print("Version: [");Serial.print(ver);Serial.print("] Stored version: [");Serial.print(stored_ver);Serial.println("]");
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
   //   Serial.println("Stored settings retreived:");
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
    minimumfeedrate=DEFAULT_MINIMUMFEEDRATE
    minsegmenttime=DEFAULT_MINSEGMENTTIME;       
    mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE
    max_xy_jerk=DEFAULT_XYJERK;
    max_z_jerk=DEFAULT_ZJERK;
 //   Serial.println("Using Default settings:");
  }
 /*
  Serial.println("Steps per unit:");
  Serial.print("   M92");
  Serial.print(" X");Serial.print(axis_steps_per_unit[0]);
  Serial.print(" Y");Serial.print(axis_steps_per_unit[1]);
  Serial.print(" Z");Serial.print(axis_steps_per_unit[2]);
  Serial.print(" E");Serial.println(axis_steps_per_unit[3]);
  Serial.println("Maximum feedrates (mm/s):");
  Serial.print ("   M203");
  Serial.print(" X");Serial.print(max_feedrate[0]/60);
  Serial.print(" Y");Serial.print(max_feedrate[1]/60);
  Serial.print(" Z");Serial.print(max_feedrate[2]/60);
  Serial.print(" E");Serial.println(max_feedrate[3]/60);
  Serial.println("Maximum Acceleration (mm/s2):");
  Serial.print("   M201");
  Serial.print(" X");Serial.print(max_acceleration_units_per_sq_second[0]);
  Serial.print(" Y");Serial.print(max_acceleration_units_per_sq_second[1]);
  Serial.print(" Z");Serial.print(max_acceleration_units_per_sq_second[2]);
  Serial.print(" E");Serial.println(max_acceleration_units_per_sq_second[3]);
  Serial.println("Acceleration: S=acceleration, T=retract acceleration");
  Serial.print("   M204");
  Serial.print(" S");Serial.print(acceleration);
  Serial.print(" T");Serial.println(retract_acceleration);
  Serial.println("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum xY jerk (mm/s),  Z=maximum Z jerk (mm/s)");
  Serial.print("   M205");
  Serial.print(" S");Serial.print(minimumfeedrate/60);
  Serial.print(" T");Serial.print(mintravelfeedrate/60);
  Serial.print(" B");Serial.print(minsegmenttime);
  Serial.print(" X");Serial.print(max_xy_jerk/60);
  Serial.print(" Z");Serial.println(max_z_jerk/60);
  */
}  


