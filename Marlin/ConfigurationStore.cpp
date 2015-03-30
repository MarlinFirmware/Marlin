/**
 * ConfigurationStore.cpp
 *
 * Configuration and EEPROM storage
 *
 * V16 EEPROM Layout:
 *
 *  ver
 *  axis_steps_per_unit (x4)
 *  max_feedrate (x4)
 *  max_acceleration_units_per_sq_second (x4)
 *  acceleration
 *  retract_acceleration
 *  travel_acceleration
 *  minimumfeedrate
 *  mintravelfeedrate
 *  minsegmenttime
 *  max_xy_jerk
 *  max_z_jerk
 *  max_e_jerk
 *  home_offset (x3)
 *
 * Mesh bed leveling:
 *  active
 *  mesh_num_x
 *  mesh_num_y
 *  z_values[][]
 *  zprobe_zoffset
 *
 * DELTA:
 *  endstop_adj (x3)
 *  delta_radius
 *  delta_diagonal_rod
 *  delta_segments_per_second
 *
 * ULTIPANEL:
 *  plaPreheatHotendTemp
 *  plaPreheatHPBTemp
 *  plaPreheatFanSpeed
 *  absPreheatHotendTemp
 *  absPreheatHPBTemp
 *  absPreheatFanSpeed
 *
 * PIDTEMP:
 *  Kp[0], Ki[0], Kd[0], Kc[0]
 *  Kp[1], Ki[1], Kd[1], Kc[1]
 *  Kp[2], Ki[2], Kd[2], Kc[2]
 *  Kp[3], Ki[3], Kd[3], Kc[3]
 *
 * DOGLCD:
 *  lcd_contrast
 *
 * SCARA:
 *  axis_scaling (x3)
 *
 * FWRETRACT:
 *  autoretract_enabled
 *  retract_length
 *  retract_length_swap
 *  retract_feedrate
 *  retract_zlift
 *  retract_recover_length
 *  retract_recover_length_swap
 *  retract_recover_feedrate
 *
 *  volumetric_enabled
 *
 *  filament_size (x4)
 *
 * Z_DUAL_ENDSTOPS
 *  z_endstop_adj
 *
 */
#include "Marlin.h"
#include "language.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "ConfigurationStore.h"

#if defined(MESH_BED_LEVELING)
   #include "mesh_bed_leveling.h"
#endif  // MESH_BED_LEVELING

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size) {
  uint8_t c;
  while(size--) {
    eeprom_write_byte((unsigned char*)pos, *value);
    c = eeprom_read_byte((unsigned char*)pos);
    if (c != *value) {
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
    }
    pos++;
    value++;
  };
}
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size) {
  do {
    *value = eeprom_read_byte((unsigned char*)pos);
    pos++;
    value++;
  } while (--size);
}
#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))

//======================================================================================

#define DUMMY_PID_VALUE 3000.0f

#define EEPROM_OFFSET 100


// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.

#define EEPROM_VERSION "V18"

#ifdef EEPROM_SETTINGS

void Config_StoreSettings()  {
  float dummy = 0.0f;
  char ver[4] = "000";
  int i = EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i, ver); // invalidate data first
  EEPROM_WRITE_VAR(i, axis_steps_per_unit);
  EEPROM_WRITE_VAR(i, max_feedrate);
  EEPROM_WRITE_VAR(i, max_acceleration_units_per_sq_second);
  EEPROM_WRITE_VAR(i, acceleration);
  EEPROM_WRITE_VAR(i, retract_acceleration);
  EEPROM_WRITE_VAR(i, travel_acceleration);
  EEPROM_WRITE_VAR(i, minimumfeedrate);
  EEPROM_WRITE_VAR(i, mintravelfeedrate);
  EEPROM_WRITE_VAR(i, minsegmenttime);
  EEPROM_WRITE_VAR(i, max_xy_jerk);
  EEPROM_WRITE_VAR(i, max_z_jerk);
  EEPROM_WRITE_VAR(i, max_e_jerk);
  EEPROM_WRITE_VAR(i, home_offset);

  uint8_t mesh_num_x = 3;
  uint8_t mesh_num_y = 3;
  #ifdef MESH_BED_LEVELING
    // Compile time test that sizeof(mbl.z_values) is as expected
    typedef char c_assert[(sizeof(mbl.z_values) == MESH_NUM_X_POINTS*MESH_NUM_Y_POINTS*sizeof(dummy)) ? 1 : -1];
    mesh_num_x = MESH_NUM_X_POINTS;
    mesh_num_y = MESH_NUM_Y_POINTS;
    EEPROM_WRITE_VAR(i, mbl.active);
    EEPROM_WRITE_VAR(i, mesh_num_x);
    EEPROM_WRITE_VAR(i, mesh_num_y);
    EEPROM_WRITE_VAR(i, mbl.z_values);
  #else
    uint8_t dummy_uint8 = 0;
    EEPROM_WRITE_VAR(i, dummy_uint8);
    EEPROM_WRITE_VAR(i, mesh_num_x);
    EEPROM_WRITE_VAR(i, mesh_num_y);
    dummy = 0.0f;
    for (int q=0; q<mesh_num_x*mesh_num_y; q++) {
      EEPROM_WRITE_VAR(i, dummy);
    }
  #endif // MESH_BED_LEVELING

  #ifndef ENABLE_AUTO_BED_LEVELING
    float zprobe_zoffset = 0;
  #endif
  EEPROM_WRITE_VAR(i, zprobe_zoffset);

  #ifdef DELTA
    EEPROM_WRITE_VAR(i, endstop_adj);               // 3 floats
    EEPROM_WRITE_VAR(i, delta_radius);              // 1 float
    EEPROM_WRITE_VAR(i, delta_diagonal_rod);        // 1 float
    EEPROM_WRITE_VAR(i, delta_segments_per_second); // 1 float
  #elif defined(Z_DUAL_ENDSTOPS)
    EEPROM_WRITE_VAR(i, z_endstop_adj);            // 1 floats
    dummy = 0.0f;
    for (int q=5; q--;) EEPROM_WRITE_VAR(i, dummy);
  #else
    dummy = 0.0f;
    for (int q=6; q--;) EEPROM_WRITE_VAR(i, dummy);
  #endif

  #ifndef ULTIPANEL
    int plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP, plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP, plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED,
        absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP, absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP, absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif // !ULTIPANEL

  EEPROM_WRITE_VAR(i, plaPreheatHotendTemp);
  EEPROM_WRITE_VAR(i, plaPreheatHPBTemp);
  EEPROM_WRITE_VAR(i, plaPreheatFanSpeed);
  EEPROM_WRITE_VAR(i, absPreheatHotendTemp);
  EEPROM_WRITE_VAR(i, absPreheatHPBTemp);
  EEPROM_WRITE_VAR(i, absPreheatFanSpeed);


  for (int e = 0; e < 4; e++) {

    #ifdef PIDTEMP
      if (e < EXTRUDERS) {
        EEPROM_WRITE_VAR(i, PID_PARAM(Kp, e));
        EEPROM_WRITE_VAR(i, PID_PARAM(Ki, e));
        EEPROM_WRITE_VAR(i, PID_PARAM(Kd, e));
        #ifdef PID_ADD_EXTRUSION_RATE
          EEPROM_WRITE_VAR(i, PID_PARAM(Kc, e));
        #else
          dummy = 1.0f; // 1.0 = default kc
          EEPROM_WRITE_VAR(i, dummy);
        #endif
      }
      else {
    #else // !PIDTEMP
      {
    #endif // !PIDTEMP

        dummy = DUMMY_PID_VALUE;
        EEPROM_WRITE_VAR(i, dummy);
        dummy = 0.0f;
        for (int q = 3; q--;) EEPROM_WRITE_VAR(i, dummy);
      }

  } // Extruders Loop

  #ifndef DOGLCD
    int lcd_contrast = 32;
  #endif
  EEPROM_WRITE_VAR(i, lcd_contrast);

  #ifdef SCARA
    EEPROM_WRITE_VAR(i, axis_scaling); // 3 floats
  #else
    dummy = 1.0f;
    EEPROM_WRITE_VAR(i, dummy);
  #endif

  #ifdef FWRETRACT
    EEPROM_WRITE_VAR(i, autoretract_enabled);
    EEPROM_WRITE_VAR(i, retract_length);
    #if EXTRUDERS > 1
      EEPROM_WRITE_VAR(i, retract_length_swap);
    #else
      dummy = 0.0f;
      EEPROM_WRITE_VAR(i, dummy);
    #endif
    EEPROM_WRITE_VAR(i, retract_feedrate);
    EEPROM_WRITE_VAR(i, retract_zlift);
    EEPROM_WRITE_VAR(i, retract_recover_length);
    #if EXTRUDERS > 1
      EEPROM_WRITE_VAR(i, retract_recover_length_swap);
    #else
      dummy = 0.0f;
      EEPROM_WRITE_VAR(i, dummy);
    #endif
    EEPROM_WRITE_VAR(i, retract_recover_feedrate);
  #endif // FWRETRACT

  EEPROM_WRITE_VAR(i, volumetric_enabled);

  // Save filament sizes
  for (int q = 0; q < 4; q++) {
    if (q < EXTRUDERS) dummy = filament_size[q];
    EEPROM_WRITE_VAR(i, dummy);
  }

  char ver2[4] = EEPROM_VERSION;
  int j = EEPROM_OFFSET;
  EEPROM_WRITE_VAR(j, ver2); // validate data

  // Report storage size
  SERIAL_ECHO_START;
  SERIAL_ECHOPAIR("Settings Stored (", (unsigned long)i);
  SERIAL_ECHOLNPGM(" bytes)");
}

void Config_RetrieveSettings() {

  int i = EEPROM_OFFSET;
  char stored_ver[4];
  char ver[4] = EEPROM_VERSION;
  EEPROM_READ_VAR(i, stored_ver); //read stored version
  //  SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");

  if (strncmp(ver, stored_ver, 3) != 0) {
    Config_ResetDefault();
  }
  else {
    float dummy = 0;

    // version number match
    EEPROM_READ_VAR(i, axis_steps_per_unit);
    EEPROM_READ_VAR(i, max_feedrate);
    EEPROM_READ_VAR(i, max_acceleration_units_per_sq_second);

    // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
    reset_acceleration_rates();

    EEPROM_READ_VAR(i, acceleration);
    EEPROM_READ_VAR(i, retract_acceleration);
    EEPROM_READ_VAR(i, travel_acceleration);
    EEPROM_READ_VAR(i, minimumfeedrate);
    EEPROM_READ_VAR(i, mintravelfeedrate);
    EEPROM_READ_VAR(i, minsegmenttime);
    EEPROM_READ_VAR(i, max_xy_jerk);
    EEPROM_READ_VAR(i, max_z_jerk);
    EEPROM_READ_VAR(i, max_e_jerk);
    EEPROM_READ_VAR(i, home_offset);

    uint8_t mesh_num_x = 0;
    uint8_t mesh_num_y = 0;
    #if defined(MESH_BED_LEVELING)
      EEPROM_READ_VAR(i, mbl.active);
      EEPROM_READ_VAR(i, mesh_num_x);
      EEPROM_READ_VAR(i, mesh_num_y);
      if (mesh_num_x != MESH_NUM_X_POINTS ||
          mesh_num_y != MESH_NUM_Y_POINTS) {
        mbl.reset();
        for (int q=0; q<mesh_num_x*mesh_num_y; q++) {
          EEPROM_READ_VAR(i, dummy);
        }
      } else {
        EEPROM_READ_VAR(i, mbl.z_values);
      }
    #else
      uint8_t dummy_uint8 = 0;
      EEPROM_READ_VAR(i, dummy_uint8);
      EEPROM_READ_VAR(i, mesh_num_x);
      EEPROM_READ_VAR(i, mesh_num_y);
      for (int q=0; q<mesh_num_x*mesh_num_y; q++) {
        EEPROM_READ_VAR(i, dummy);
      }
    #endif  // MESH_BED_LEVELING

    #ifndef ENABLE_AUTO_BED_LEVELING
      float zprobe_zoffset = 0;
    #endif
    EEPROM_READ_VAR(i, zprobe_zoffset);

    #ifdef DELTA
      EEPROM_READ_VAR(i, endstop_adj);                // 3 floats
      EEPROM_READ_VAR(i, delta_radius);               // 1 float
      EEPROM_READ_VAR(i, delta_diagonal_rod);         // 1 float
      EEPROM_READ_VAR(i, delta_segments_per_second);  // 1 float
    #elif defined(Z_DUAL_ENDSTOPS)
      EEPROM_READ_VAR(i, z_endstop_adj);
      dummy = 0.0f;
      for (int q=5; q--;) EEPROM_READ_VAR(i, dummy);
    #else
      dummy = 0.0f;
      for (int q=6; q--;) EEPROM_READ_VAR(i, dummy);
    #endif

    #ifndef ULTIPANEL
      int plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed,
          absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed;
    #endif

    EEPROM_READ_VAR(i, plaPreheatHotendTemp);
    EEPROM_READ_VAR(i, plaPreheatHPBTemp);
    EEPROM_READ_VAR(i, plaPreheatFanSpeed);
    EEPROM_READ_VAR(i, absPreheatHotendTemp);
    EEPROM_READ_VAR(i, absPreheatHPBTemp);
    EEPROM_READ_VAR(i, absPreheatFanSpeed);

    #ifdef PIDTEMP
      for (int e = 0; e < 4; e++) { // 4 = max extruders currently supported by Marlin
        EEPROM_READ_VAR(i, dummy);
        if (e < EXTRUDERS && dummy != DUMMY_PID_VALUE) {
          // do not need to scale PID values as the values in EEPROM are already scaled
          PID_PARAM(Kp, e) = dummy;
          EEPROM_READ_VAR(i, PID_PARAM(Ki, e));
          EEPROM_READ_VAR(i, PID_PARAM(Kd, e));
          #ifdef PID_ADD_EXTRUSION_RATE
            EEPROM_READ_VAR(i, PID_PARAM(Kc, e));
          #else
            EEPROM_READ_VAR(i, dummy);
          #endif
        }
        else {
          for (int q=3; q--;) EEPROM_READ_VAR(i, dummy); // Ki, Kd, Kc
        }
      }
    #else // !PIDTEMP
      // 4 x 4 = 16 slots for PID parameters
      for (int q=16; q--;) EEPROM_READ_VAR(i, dummy);  // 4x Kp, Ki, Kd, Kc
    #endif // !PIDTEMP

    #ifndef DOGLCD
      int lcd_contrast;
    #endif
    EEPROM_READ_VAR(i, lcd_contrast);

    #ifdef SCARA
      EEPROM_READ_VAR(i, axis_scaling);  // 3 floats
    #else
      EEPROM_READ_VAR(i, dummy);
    #endif

    #ifdef FWRETRACT
      EEPROM_READ_VAR(i, autoretract_enabled);
      EEPROM_READ_VAR(i, retract_length);
      #if EXTRUDERS > 1
        EEPROM_READ_VAR(i, retract_length_swap);
      #else
        EEPROM_READ_VAR(i, dummy);
      #endif
      EEPROM_READ_VAR(i, retract_feedrate);
      EEPROM_READ_VAR(i, retract_zlift);
      EEPROM_READ_VAR(i, retract_recover_length);
      #if EXTRUDERS > 1
        EEPROM_READ_VAR(i, retract_recover_length_swap);
      #else
        EEPROM_READ_VAR(i, dummy);
      #endif
      EEPROM_READ_VAR(i, retract_recover_feedrate);
    #endif // FWRETRACT

    EEPROM_READ_VAR(i, volumetric_enabled);

    for (int q = 0; q < 4; q++) {
      EEPROM_READ_VAR(i, dummy);
      if (q < EXTRUDERS) filament_size[q] = dummy;
    }

    calculate_volumetric_multipliers();
    // Call updatePID (similar to when we have processed M301)
    updatePID();

    // Report settings retrieved and length
    SERIAL_ECHO_START;
    SERIAL_ECHO(ver);
    SERIAL_ECHOPAIR(" stored settings retrieved (", (unsigned long)i);
    SERIAL_ECHOLNPGM(" bytes)");
  }

  #ifdef EEPROM_CHITCHAT
    Config_PrintSettings();
  #endif
}

#endif // EEPROM_SETTINGS

void Config_ResetDefault() {
  float tmp1[] = DEFAULT_AXIS_STEPS_PER_UNIT;
  float tmp2[] = DEFAULT_MAX_FEEDRATE;
  long tmp3[] = DEFAULT_MAX_ACCELERATION;
  for (uint16_t i = 0; i < NUM_AXIS; i++) {
    axis_steps_per_unit[i] = tmp1[i];
    max_feedrate[i] = tmp2[i];
    max_acceleration_units_per_sq_second[i] = tmp3[i];
    #ifdef SCARA
      if (i < sizeof(axis_scaling) / sizeof(*axis_scaling))
        axis_scaling[i] = 1;
    #endif
  }

  // steps per sq second need to be updated to agree with the units per sq second
  reset_acceleration_rates();

  acceleration = DEFAULT_ACCELERATION;
  retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  minimumfeedrate = DEFAULT_MINIMUMFEEDRATE;
  minsegmenttime = DEFAULT_MINSEGMENTTIME;
  mintravelfeedrate = DEFAULT_MINTRAVELFEEDRATE;
  max_xy_jerk = DEFAULT_XYJERK;
  max_z_jerk = DEFAULT_ZJERK;
  max_e_jerk = DEFAULT_EJERK;
  home_offset[X_AXIS] = home_offset[Y_AXIS] = home_offset[Z_AXIS] = 0;

  #ifdef MESH_BED_LEVELING
    mbl.active = 0;
  #endif

  #ifdef ENABLE_AUTO_BED_LEVELING
    zprobe_zoffset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
  #endif

  #ifdef DELTA
    endstop_adj[X_AXIS] = endstop_adj[Y_AXIS] = endstop_adj[Z_AXIS] = 0;
    delta_radius =  DELTA_RADIUS;
    delta_diagonal_rod =  DELTA_DIAGONAL_ROD;
    delta_segments_per_second =  DELTA_SEGMENTS_PER_SECOND;
    recalc_delta_settings(delta_radius, delta_diagonal_rod);
  #elif defined(Z_DUAL_ENDSTOPS)
    z_endstop_adj = 0;
  #endif

  #ifdef ULTIPANEL
    plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP;
    plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP;
    plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
    absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP;
    absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP;
    absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif

  #ifdef DOGLCD
    lcd_contrast = DEFAULT_LCD_CONTRAST;
  #endif

  #ifdef PIDTEMP
    #ifdef PID_PARAMS_PER_EXTRUDER
      for (int e = 0; e < EXTRUDERS; e++)
    #else
      int e = 0; // only need to write once
    #endif
    {
      PID_PARAM(Kp, e) = DEFAULT_Kp;
      PID_PARAM(Ki, e) = scalePID_i(DEFAULT_Ki);
      PID_PARAM(Kd, e) = scalePID_d(DEFAULT_Kd);
      #ifdef PID_ADD_EXTRUSION_RATE
        PID_PARAM(Kc, e) = DEFAULT_Kc;
      #endif
    }
    // call updatePID (similar to when we have processed M301)
    updatePID();
  #endif // PIDTEMP

  #ifdef FWRETRACT
    autoretract_enabled = false;
    retract_length = RETRACT_LENGTH;
    #if EXTRUDERS > 1
      retract_length_swap = RETRACT_LENGTH_SWAP;
    #endif
    retract_feedrate = RETRACT_FEEDRATE;
    retract_zlift = RETRACT_ZLIFT;
    retract_recover_length = RETRACT_RECOVER_LENGTH;
    #if EXTRUDERS > 1
      retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
    #endif
    retract_recover_feedrate = RETRACT_RECOVER_FEEDRATE;
  #endif

  volumetric_enabled = false;
  filament_size[0] = DEFAULT_NOMINAL_FILAMENT_DIA;
  #if EXTRUDERS > 1
    filament_size[1] = DEFAULT_NOMINAL_FILAMENT_DIA;
    #if EXTRUDERS > 2
      filament_size[2] = DEFAULT_NOMINAL_FILAMENT_DIA;
      #if EXTRUDERS > 3
        filament_size[3] = DEFAULT_NOMINAL_FILAMENT_DIA;
      #endif
    #endif
  #endif
  calculate_volumetric_multipliers();

  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");
}

#ifndef DISABLE_M503

void Config_PrintSettings(bool forReplay) {
  // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown

  SERIAL_ECHO_START;

  if (!forReplay) {
    SERIAL_ECHOLNPGM("Steps per unit:");
    SERIAL_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M92 X", axis_steps_per_unit[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", axis_steps_per_unit[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", axis_steps_per_unit[Z_AXIS]);
  SERIAL_ECHOPAIR(" E", axis_steps_per_unit[E_AXIS]);
  SERIAL_EOL;

  SERIAL_ECHO_START;

  #ifdef SCARA
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Scaling factors:");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M365 X", axis_scaling[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", axis_scaling[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z", axis_scaling[Z_AXIS]);
    SERIAL_EOL;
    SERIAL_ECHO_START;
  #endif // SCARA

  if (!forReplay) {
    SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
    SERIAL_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M203 X", max_feedrate[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", max_feedrate[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", max_feedrate[Z_AXIS]);
  SERIAL_ECHOPAIR(" E", max_feedrate[E_AXIS]);
  SERIAL_EOL;

  SERIAL_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
    SERIAL_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M201 X", max_acceleration_units_per_sq_second[X_AXIS] );
  SERIAL_ECHOPAIR(" Y", max_acceleration_units_per_sq_second[Y_AXIS] );
  SERIAL_ECHOPAIR(" Z", max_acceleration_units_per_sq_second[Z_AXIS] );
  SERIAL_ECHOPAIR(" E", max_acceleration_units_per_sq_second[E_AXIS]);
  SERIAL_EOL;
  SERIAL_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Accelerations: P=printing, R=retract and T=travel");
    SERIAL_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M204 P", acceleration );
  SERIAL_ECHOPAIR(" R", retract_acceleration);
  SERIAL_ECHOPAIR(" T", travel_acceleration);
  SERIAL_EOL;

  SERIAL_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum XY jerk (mm/s),  Z=maximum Z jerk (mm/s),  E=maximum E jerk (mm/s)");
    SERIAL_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M205 S", minimumfeedrate );
  SERIAL_ECHOPAIR(" T", mintravelfeedrate );
  SERIAL_ECHOPAIR(" B", minsegmenttime );
  SERIAL_ECHOPAIR(" X", max_xy_jerk );
  SERIAL_ECHOPAIR(" Z", max_z_jerk);
  SERIAL_ECHOPAIR(" E", max_e_jerk);
  SERIAL_EOL;

  SERIAL_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Home offset (mm):");
    SERIAL_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M206 X", home_offset[X_AXIS] );
  SERIAL_ECHOPAIR(" Y", home_offset[Y_AXIS] );
  SERIAL_ECHOPAIR(" Z", home_offset[Z_AXIS] );
  SERIAL_EOL;

  #ifdef DELTA
    SERIAL_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Endstop adjustement (mm):");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M666 X", endstop_adj[X_AXIS] );
    SERIAL_ECHOPAIR(" Y", endstop_adj[Y_AXIS] );
    SERIAL_ECHOPAIR(" Z", endstop_adj[Z_AXIS] );
    SERIAL_EOL;
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Delta settings: L=delta_diagonal_rod, R=delta_radius, S=delta_segments_per_second");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M665 L", delta_diagonal_rod );
    SERIAL_ECHOPAIR(" R", delta_radius );
    SERIAL_ECHOPAIR(" S", delta_segments_per_second );
    SERIAL_EOL;
  #elif defined(Z_DUAL_ENDSTOPS)
    SERIAL_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Z2 Endstop adjustement (mm):");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M666 Z", z_endstop_adj );
    SERIAL_EOL;  
  #endif // DELTA

  #ifdef PIDTEMP
    SERIAL_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("PID settings:");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("   M301 P", PID_PARAM(Kp, 0)); // for compatibility with hosts, only echos values for E0
    SERIAL_ECHOPAIR(" I", unscalePID_i(PID_PARAM(Ki, 0)));
    SERIAL_ECHOPAIR(" D", unscalePID_d(PID_PARAM(Kd, 0)));
    SERIAL_EOL;
  #endif // PIDTEMP

  #ifdef FWRETRACT

    SERIAL_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Retract: S=Length (mm) F:Speed (mm/m) Z: ZLift (mm)");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("   M207 S", retract_length);
    SERIAL_ECHOPAIR(" F", retract_feedrate*60);
    SERIAL_ECHOPAIR(" Z", retract_zlift);
    SERIAL_EOL;
    SERIAL_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Recover: S=Extra length (mm) F:Speed (mm/m)");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("   M208 S", retract_recover_length);
    SERIAL_ECHOPAIR(" F", retract_recover_feedrate*60);
    SERIAL_EOL;
    SERIAL_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Auto-Retract: S=0 to disable, 1 to interpret extrude-only moves as retracts or recoveries");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("   M209 S", (unsigned long)(autoretract_enabled ? 1 : 0));
    SERIAL_EOL;

    #if EXTRUDERS > 1
      if (!forReplay) {
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Multi-extruder settings:");
        SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   Swap retract length (mm):    ", retract_length_swap);
        SERIAL_EOL;
        SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   Swap rec. addl. length (mm): ", retract_recover_length_swap);
        SERIAL_EOL;
      }
    #endif // EXTRUDERS > 1

  #endif // FWRETRACT

  SERIAL_ECHO_START;
  if (volumetric_enabled) {
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Filament settings:");
      SERIAL_ECHO_START;
    }
    SERIAL_ECHOPAIR("   M200 D", filament_size[0]);
    SERIAL_EOL;

    #if EXTRUDERS > 1
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("   M200 T1 D", filament_size[1]);
      SERIAL_EOL;
      #if EXTRUDERS > 2
        SERIAL_ECHO_START;
        SERIAL_ECHOPAIR("   M200 T2 D", filament_size[2]);
        SERIAL_EOL;
        #if EXTRUDERS > 3
          SERIAL_ECHO_START;
          SERIAL_ECHOPAIR("   M200 T3 D", filament_size[3]);
          SERIAL_EOL;
        #endif
      #endif
    #endif

  } else {
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Filament settings: Disabled");
    }
  }

  #ifdef ENABLE_AUTO_BED_LEVELING
    SERIAL_ECHO_START;
    #ifdef CUSTOM_M_CODES
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Z-Probe Offset (mm):");
        SERIAL_ECHO_START;
      }
      SERIAL_ECHOPAIR("   M", (unsigned long)CUSTOM_M_CODE_SET_Z_PROBE_OFFSET);
      SERIAL_ECHOPAIR(" Z", -zprobe_zoffset);
    #else
      if (!forReplay) {
        SERIAL_ECHOPAIR("Z-Probe Offset (mm):", -zprobe_zoffset);
      }
    #endif
    SERIAL_EOL;
  #endif
}

#endif // !DISABLE_M503
