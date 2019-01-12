/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 *  The monitor_L6470_driver routines are a close copy of the TMC code
 */

#include "../../inc/MarlinConfig.h"

#if HAS_DRIVER(ST_L6470)

#include "L6470_Marlin.h"
#include "../stepper_indirection.h"
#include "../../gcode/gcode.h"
#include "../planner.h"


uint8_t L6470_Transfer(uint8_t data, uint8_t Axis, uint8_t* chain_position_array);

#if HAS_L6470_EXTRUDER
  uint8_t L6470_E_dir_commands[6];  // array to hold direction command for each extruder
#endif

char L6470_index_to_Axis[MAX_L6470][3] = { "X ",
                                           "Y ",
                                           "Z ",
                                           "X2",
                                           "Y2",
                                           "Z2",
                                           "Z3",
                                           "E0",
                                           "E1",
                                           "E2",
                                           "E3",
                                           "E4",
                                           "E5",
                                         };

bool L6470_index_to_DIR[MAX_L6470] =  { INVERT_X_DIR                        ,  // 0 X
                                        INVERT_Y_DIR                        ,  // 1 Y
                                        INVERT_Z_DIR                        ,  // 2 Z
#if ENABLED(X_DUAL_STEPPER_DRIVERS)
                                        INVERT_X_DIR ^ INVERT_X2_VS_X_DIR   ,  // 3 X2
#else
                                        INVERT_X_DIR                        ,  // 3 X2
#endif
#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
                                        INVERT_Y_DIR ^ INVERT_Y2_VS_Y_DIR   ,  // 4 52
#else
                                        INVERT_Y_DIR                        ,  // 4 Y2
#endif
                                        INVERT_Z_DIR                        ,  // 5 Z2
                                        INVERT_Z_DIR                        ,  // 6 Z3
                                        INVERT_E0_DIR                       ,  // 7 E0
                                        INVERT_E1_DIR                       ,  // 8 E1
                                        INVERT_E2_DIR                       ,  // 9 E2
                                        INVERT_E3_DIR                       ,  //10 E3
                                        INVERT_E4_DIR                       ,  //11 E4
                                        INVERT_E5_DIR                       ,  //12 E5
                                      };


uint8_t L6470_axis_xref[MAX_L6470] = {
    (AxisEnum)X_AXIS  ,   // X
    (AxisEnum)Y_AXIS  ,   // Y
    (AxisEnum)Z_AXIS  ,   // Z
    (AxisEnum)X_AXIS  ,   // X2
    (AxisEnum)Y_AXIS  ,   // Y2
    (AxisEnum)Z_AXIS  ,   // Z2
    (AxisEnum)Z_AXIS  ,   // Z3
    (AxisEnum)E_AXIS  ,   // E0
    (AxisEnum)E_AXIS  ,   // E1
    (AxisEnum)E_AXIS  ,   // E2
    (AxisEnum)E_AXIS  ,   // E3
    (AxisEnum)E_AXIS  ,   // E4
    (AxisEnum)E_AXIS  ,   // E5
};


///////////////////////////////////////////////////////////////////////////////////////////////

void L6470_init_to_defaults();
void L6470_populate_chain_array();
void L6470_SPI_init();

void L6470_init() {          // setup SPI and then init chips
  L6470_populate_chain_array();  // setup array to control where in the SPI transfer sequence a particular stepper's data goes
  L6470_SPI_init();              // setup L6470 soft SPI pins
  L6470_init_to_defaults();      // init the chips
}

uint16_t L6470_get_status(uint8_t axis) {

  #define GET_L6470_STATUS(Q) stepper##Q.getStatus()

  switch (axis) {
    #if AXIS_DRIVER_TYPE_X(ST_L6470)
      case 0: return  GET_L6470_STATUS(X);
    #endif
    #if AXIS_DRIVER_TYPE_Y(ST_L6470)
      case 1: return  GET_L6470_STATUS(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Z(ST_L6470)
      case 2: return  GET_L6470_STATUS(Z);
    #endif
    #if AXIS_DRIVER_TYPE_X2(ST_L6470)
      case 3: return  GET_L6470_STATUS(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
      case 4: return  GET_L6470_STATUS(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
      case 5: return  GET_L6470_STATUS(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
      case 6: return  GET_L6470_STATUS(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(ST_L6470)
      case 7: return  GET_L6470_STATUS(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(ST_L6470)
      case 8: return  GET_L6470_STATUS(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(ST_L6470)
      case 9: return  GET_L6470_STATUS(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(ST_L6470)
      case 10: return  GET_L6470_STATUS(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(ST_L6470)
      case 11: return  GET_L6470_STATUS(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(ST_L6470)
      case 12: return  GET_L6470_STATUS(E5);
    #endif
  }

  return 0 ; // not needed but kills a compiler warning
}



uint32_t L6470_get_param(uint8_t axis, uint8_t param) {

  #define GET_L6470_PARAM(Q) stepper##Q.GetParam(param)

  switch (axis) {
    #if AXIS_DRIVER_TYPE_X(ST_L6470)
      case 0: return  GET_L6470_PARAM(X);
    #endif
    #if AXIS_DRIVER_TYPE_Y(ST_L6470)
      case 1: return  GET_L6470_PARAM(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Z(ST_L6470)
      case 2: return  GET_L6470_PARAM(Z);
    #endif
    #if AXIS_DRIVER_TYPE_X2(ST_L6470)
      case 3: return  GET_L6470_PARAM(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
      case 4: return  GET_L6470_PARAM(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
      case 5: return  GET_L6470_PARAM(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
      case 6: return  GET_L6470_PARAM(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(ST_L6470)
      case 7: return  GET_L6470_PARAM(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(ST_L6470)
      case 8: return  GET_L6470_PARAM(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(ST_L6470)
      case 9: return  GET_L6470_PARAM(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(ST_L6470)
      case 10: return  GET_L6470_PARAM(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(ST_L6470)
      case 11: return  GET_L6470_PARAM(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(ST_L6470)
      case 12: return  GET_L6470_PARAM(E5);
    #endif
  }

  return 0 ; // not needed but kills a compiler warning
}


void L6470_set_param(uint8_t axis, uint8_t param, uint32_t value) {

  #define SET_L6470_PARAM(Q) stepper##Q.SetParam(param, value)

  switch (axis) {
    #if AXIS_DRIVER_TYPE_X(ST_L6470)
      case 0: SET_L6470_PARAM(X);
    #endif
    #if AXIS_DRIVER_TYPE_Y(ST_L6470)
      case 1: SET_L6470_PARAM(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Z(ST_L6470)
      case 2: SET_L6470_PARAM(Z);
    #endif
    #if AXIS_DRIVER_TYPE_X2(ST_L6470)
      case 3: SET_L6470_PARAM(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
      case 4: SET_L6470_PARAM(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
      case 5: SET_L6470_PARAM(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
      case 6: SET_L6470_PARAM(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(ST_L6470)
      case 7: SET_L6470_PARAM(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(ST_L6470)
      case 8: SET_L6470_PARAM(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(ST_L6470)
      case 9: SET_L6470_PARAM(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(ST_L6470)
      case 10: SET_L6470_PARAM(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(ST_L6470)
      case 11: SET_L6470_PARAM(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(ST_L6470)
      case 12: SET_L6470_PARAM(E5);
    #endif
  }
}


bool L6470_get_user_input(  uint8_t * p_driver_count, uint8_t axis_index[3], char axis_mon[3][3],
                       float * p_position_max, float * p_position_min, float * p_final_feedrate, uint8_t * p_kval_hold,
                       bool over_current_flag, uint8_t * p_OCD_TH_val/*=NULL*/, uint8_t * p_STALL_TH_val/*=NULL*/, uint16_t * p_over_current_threshold/*=NULL*/ ) {

  // returns TRUE if the calling routine needs to abort/kill


  #define driver_count            *p_driver_count
  #define position_max            *p_position_max
  #define position_min            *p_position_min
  #define final_feedrate          *p_final_feedrate
  #define kval_hold               *p_kval_hold
  #define OCD_TH_val              *p_OCD_TH_val
  #define STALL_TH_val            *p_STALL_TH_val
  #define over_current_threshold  *p_over_current_threshold

  uint16_t displacement = 0;  // " = 0" to eliminate compiler warning
  uint8_t j;   // general purpose counter


  if (!all_axes_homed()) {
    SERIAL_ECHOLNPGM("ERROR - must home all axis before running this command");
    //    return true;
  }



  LOOP_XYZE(i) if ( uint16_t _displacement = parser.intval(axis_codes[i]) ) {
    displacement = _displacement;
    uint8_t axis_offset = parser.byteval('J');
    axis_mon[0][0] = axis_codes[i];   // axis ASCII value (target character)
    if (axis_offset >=2) {                      // single axis
      axis_mon[0][1] = axis_offset + 0x30;
    }
    else if (axis_mon[0][0] == 'E') {
      axis_mon[0][1] = axis_offset + 0x30;      // E0 or E1
    }
    else if (axis_offset == 0) {               // one or more axis
      uint8_t driver_count_local = 0;         // can't use "driver_count" directly as a subscript because it's passed by reference
      for (j = 0; j < MAX_L6470; j++)         // see how many drivers on this axis
        if (axis_mon[0][0] == L6470_index_to_Axis[j][0]) {
          axis_mon[driver_count_local][0] = axis_mon[0][0];
          axis_mon[driver_count_local][1] = L6470_index_to_Axis[j][1];
          axis_mon[driver_count_local][2] = L6470_index_to_Axis[j][2];   // append end of string
          axis_index[driver_count_local] = j;                           // set axis index
          driver_count_local++;
        }
      driver_count = driver_count_local;
    }
    break;  // only take first axis found
  }


  //
  // position calcs & checks
  //

  float X_center = LOGICAL_X_POSITION(current_position[X_AXIS]);
  float Y_center = LOGICAL_Y_POSITION(current_position[Y_AXIS]);
  float Z_center = LOGICAL_Z_POSITION(current_position[Z_AXIS]);
  float E_center = current_position[E_AXIS];
  position_max = 0;
  position_min = 0;

  switch (axis_mon[0][0]) {
    case 'X': { position_min = X_center - displacement;
                position_max = X_center + displacement;
                SERIAL_ECHOLNPAIR(" X_min =  ", position_min);
                SERIAL_ECHOLNPAIR(" X_max =  ", position_max);
                if (
                      #ifdef X_MIN_POS
                        X_MIN_POS > position_min
                      #endif
                      #ifdef X_MAX_POS
                        || X_MAX_POS  < position_max
                      #endif
                   ) {
                  SERIAL_ECHOLNPGM("ERROR - motion outside known endstop position");
                  return true;
                }
                break;
              }
    case 'Y': { position_min = Y_center - displacement;
                position_max = Y_center + displacement;
                SERIAL_ECHOLNPAIR(" Y_min =  ", position_min);
                SERIAL_ECHOLNPAIR(" Y_max =  ", position_max);
                if (
                      #ifdef Y_MIN_POS
                        Y_MIN_POS > position_min
                      #endif
                      #ifdef Y_MAX_POS
                        || Y_MAX_POS  < position_max
                      #endif
                   ) {
                  SERIAL_ECHOLNPGM("ERROR - motion outside known endstop position");
                  return true;
                }
                break;
              }
    case 'Z': { position_min = Z_center - displacement;
                position_max = Z_center + displacement;
                SERIAL_ECHOLNPAIR(" Z_min =  ", position_min);
                SERIAL_ECHOLNPAIR(" Z_max =  ", position_max);;
                if (
                      #ifdef Z_MIN_POS
                        Z_MIN_POS > position_min
                      #endif
                      #ifdef Z_MAX_POS
                        || Z_MAX_POS  < position_max
                      #endif
                   ) {
                  SERIAL_ECHOLNPGM("ERROR - motion outside known endstop position");
                  return true;
                }
                break;
              }
    case 'E': { position_min = E_center - displacement;
                position_max = E_center + displacement;
                SERIAL_ECHOLNPAIR(" E_min =  ", position_min);
                SERIAL_ECHOLNPAIR(" E_max =  ", position_max);
                break;
              }
  }  // position calcs


  //
  // now work on the drivers
  //

  for (uint8_t k = 0; k < driver_count; k++) {
    bool not_found = true;
    for (j = 1; j <= L6470_chain[0]; j++)
      if ( (L6470_index_to_Axis[L6470_chain[j]][0] == axis_mon[k][0]) &&
           (L6470_index_to_Axis[L6470_chain[j]][1] == axis_mon[k][1])) {   // see if a L6470 driver
        not_found = false;
      }
    if (not_found) {
      driver_count = k;
      axis_mon[k][0] = ' ';  // mark this entry  as invalid
      break;
    }
  }
  if (driver_count == 0) {
    SERIAL_ECHOLNPGM("ERROR - not a L6470 axis");
    return true;
  }

  SERIAL_ECHOPGM("Moniitoring:");
  for (j = 0; j < driver_count; j++)
    SERIAL_ECHOPAIR("  ", &axis_mon[j][0]);
  SERIAL_EOL();

  // now have a list of driver(s) to monitor


  //
  //kVAL_HOLD checks & settings
  //

  kval_hold = parser.byteval('K');

  if (kval_hold) {
    SERIAL_ECHOLNPAIR("kval_hold = ", kval_hold);

    for (j = 0; j < driver_count; j++)
      L6470_set_param(axis_index[j], L6470_KVAL_HOLD, kval_hold);
  }
  else {
    // only print the KVAL_HOLD from one of the drivers
    kval_hold = L6470_get_param(axis_index[0], L6470_KVAL_HOLD);
    SERIAL_ECHOLNPAIR("KVAL_HOLD = ", kval_hold);
  }


  //
  // over_current checks & settings
  //

  if (over_current_flag) {

    uint8_t OCD_TH_val_local = 0;       // compiler thinks OCD_TH_val is unused if use it directly
    uint8_t STALL_TH_val_local = 0;     // just in case ...

    over_current_threshold = parser.intval('I');


    if (over_current_threshold) {

      OCD_TH_val_local = over_current_threshold/375;
      LIMIT(OCD_TH_val_local, 0, 15);
      STALL_TH_val_local = over_current_threshold/31.25;
      LIMIT(STALL_TH_val_local, 0, 127);
      uint16_t OCD_TH_actual = (OCD_TH_val_local + 1) * 375;
      uint16_t STALL_TH_actual = (STALL_TH_val_local + 1) * 31.25;
      if (OCD_TH_actual < STALL_TH_actual) {
        OCD_TH_val_local++;
        OCD_TH_actual = (OCD_TH_val_local + 1) * 375;
      }

      SERIAL_ECHOLNPAIR("over_current_threshold specified: ", over_current_threshold);
      SERIAL_ECHOPAIR("over_current_threshold used     : ", STALL_TH_actual);
      SERIAL_ECHOLNPGM("  (Stall threshold)");
      SERIAL_ECHOPAIR("over_current_threshold used     : ", OCD_TH_actual);
      SERIAL_ECHOLNPGM("  (OCD threshold)");

      #define SET_OVER_CURRENT(Q) do { stepper##Q.SetParam(L6470_STALL_TH, STALL_TH_val_local); stepper##Q.SetParam(L6470_OCD_TH, OCD_TH_val_local);} while (0)

      for (j = 0; j < driver_count; j++) {
        L6470_set_param(axis_index[j], L6470_STALL_TH, STALL_TH_val_local);
        L6470_set_param(axis_index[j], L6470_OCD_TH, OCD_TH_val_local);
      }
    }
    else {
      // only get & print the OVER_CURRENT values from one of the drivers
      STALL_TH_val_local = L6470_get_param(axis_index[0], L6470_STALL_TH);
      OCD_TH_val_local = L6470_get_param(axis_index[0], L6470_OCD_TH);

      SERIAL_ECHOPAIR("over_current_threshold used     : ", (STALL_TH_val_local + 1) * 31.25);
      SERIAL_ECHOLNPGM("  (Stall threshold)");
      SERIAL_ECHOPAIR("over_current_threshold used     : ", (OCD_TH_val_local + 1) * 375);
      SERIAL_ECHOLNPGM("  (OCD threshold)");
    } // over_current_threshold

    for (j = 0; j < driver_count; j++) {                 // set all drivers on axis the same
      L6470_set_param(axis_index[j], L6470_STALL_TH, STALL_TH_val_local);
      L6470_set_param(axis_index[j], L6470_OCD_TH, OCD_TH_val_local);
    }

    OCD_TH_val = OCD_TH_val_local;        // force compiler to update the main routine's copy
    STALL_TH_val = STALL_TH_val_local;    // force compiler to update the main routine's copy
  }  // end of overcurrent


  //
  // feedrate
  //

  final_feedrate = parser.floatval('F');
  if (final_feedrate == 0) {
    float default_max_feedrate[] = DEFAULT_MAX_FEEDRATE;
    final_feedrate = 0;
    uint8_t num_feedrates = COUNT(default_max_feedrate);
    for (j = 0; j < num_feedrates; j++)
      if (axis_codes[j] == axis_mon[0][0]) {
        final_feedrate =  default_max_feedrate[j];
        break;
      }
    if ( (j == 3) &&  (j < (num_feedrates -1))) {   // have more than one extruder feedrate
      uint8_t extruder_num = axis_mon[0][1] - 0x30;
      if (j + extruder_num <= num_feedrates ) {   // have a feedrate specifically for this extruder
        final_feedrate =  default_max_feedrate[j + extruder_num];
      }
      else {
         final_feedrate =  default_max_feedrate[3];  // use E0 feedrate for this extruder
      }
    }
    final_feedrate *= 60;  //convert to mm/minute

  }  // end feedrate

  return false;   // returns FALSE if there are no user input problems
}  // end L6470_get_user_input



void L6470_error_status_decode(uint16_t status, uint8_t axis) {  // assumes status bits have been inverted
  char temp_buf[10];
  SERIAL_ECHOPAIR("AXIS: ", L6470_index_to_Axis[axis][0]);
  SERIAL_ECHO(L6470_index_to_Axis[axis][1]);
  sprintf(temp_buf,"  %4x   ", status);
  SERIAL_ECHO(temp_buf);
  print_bin(status);
  SERIAL_ECHOPAIR("  THERMAL: ", (status & STATUS_TH_SD) ? "SHUTDOWN" : (status & STATUS_TH_WRN) ? "WARNING " :"OK      ") ;
  SERIAL_ECHOPAIR("   OVERCURRENT: ", (status & STATUS_OCD) ? "YES" : "NO ") ;
  SERIAL_ECHOLNPAIR("   STALL: ", ((status & STATUS_STEP_LOSS_A) | (status & STATUS_STEP_LOSS_B))  ? "YES" : "NO ") ;
}


void print_bin(uint16_t val) {
  char temp_buf[30];
  for (int8_t i = 15; i > -1; i--) {
    sprintf(temp_buf,"%d", (val & (1 << i) ? 1 : 0));
    SERIAL_ECHO(temp_buf);
    if ((i % 4) == 0) {sprintf(temp_buf," "); SERIAL_ECHO(temp_buf);}
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
////
////  monitor_L6470_driver routines
////
//////////////////////////////////////////////////////////////////////////////////////////////////


#if ENABLED(MONITOR_L6470_DRIVER_STATUS)

  struct L6470_driver_data {
    uint8_t driver_index;
    uint32_t driver_status;
    bool is_otw;
    uint8_t otw_counter;
    bool is_ot;
    bool is_hi_Z;
    uint8_t com_counter;
  };

  L6470_driver_data driver_L6470_data[] = {
    #if AXIS_DRIVER_TYPE_X(ST_L6470)
      {0, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_Y(ST_L6470)
      {1, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_Z(ST_L6470)
      {2, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_X2(ST_L6470)
      {3, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
      {4, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
      {5, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
      {6, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_E0(ST_L6470)
      {7, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_E1(ST_L6470)
      {8, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_E2(ST_L6470)
      {9, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_E3(ST_L6470)
      {10, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_E4(ST_L6470)
      {11, 0, 0, 0, 0, 0, 0},
    #endif
    #if AXIS_DRIVER_TYPE_E5(ST_L6470)
      {12, 0, 0, 0, 0, 0, 0},
    #endif
  }  ;

  void L6470_monitor_update(uint8_t stepper_index , uint16_t status) {
    uint8_t kval_hold;
    char temp_buf[120];
    char* p = &temp_buf[0];
    uint8_t j;
    for (j = 0; j < L6470_chain[0]; j++) // find the table for this stepper
      if ( driver_L6470_data[j].driver_index == stepper_index ) break;

    driver_L6470_data[j].driver_status = status;
    uint16_t _status = ~status;     // all error bits are active low

    if ( (status == 0) || (status == 0xffff) ) {                                              // com problem
      if (driver_L6470_data[j].com_counter == 0 ) {     // warn user when it first happens
        driver_L6470_data[j].com_counter++;
        p += sprintf(p, "Stepper %c%c ", (char)L6470_index_to_Axis[stepper_index][0], (char)L6470_index_to_Axis[stepper_index][1]);     //print out the axis/driver
        p += sprintf(p, " - communications lost\n");
        SERIAL_ECHO(temp_buf);
      }
      else {
        driver_L6470_data[j].com_counter++;
        if (driver_L6470_data[j].com_counter > 240) {  // remind of com problem about every 2 minutes
          driver_L6470_data[j].com_counter = 1;
          p += sprintf(p, "Stepper %c%c ", (char)L6470_index_to_Axis[stepper_index][0], (char)L6470_index_to_Axis[stepper_index][1]);    //print out the axis/driver
          p += sprintf(p, " - still no communications\n");
          SERIAL_ECHO(temp_buf);
        }
      }
    }
    else {
      if (driver_L6470_data[j].com_counter) {   // comms re-established
        driver_L6470_data[j].com_counter = 0;
        p += sprintf(p, "Stepper %c%c ", (char)L6470_index_to_Axis[stepper_index][0], (char)L6470_index_to_Axis[stepper_index][1]);     //print out the axis/driver
        p += sprintf(p, " - communications re-established\n");
        p += sprintf(p, ".. setting all drivers to default values\n");
        SERIAL_ECHO(temp_buf);
        L6470_init_to_defaults();
      }
      else {
        // no com problems - do the usual checks
        if ( _status & L6470_ERROR_MASK) {
          p += sprintf(p, "Stepper %c%c ", (char)L6470_index_to_Axis[stepper_index][0], (char)L6470_index_to_Axis[stepper_index][1]);   //print out the axis/driver

          if ( _status & STATUS_HIZ) {                         // the driver has shut down
            driver_L6470_data[j].is_hi_Z = true;
            p += sprintf(p, " IS SHUT DOWN");
            if ( _status & STATUS_TH_SD) {
              p += sprintf(p, " due to over temperature");
              driver_L6470_data[j].is_ot = true;
              kval_hold = L6470_get_param(stepper_index, L6470_KVAL_HOLD) - 2 * KVAL_HOLD_STEP_DOWN;
              L6470_set_param(stepper_index, L6470_KVAL_HOLD, kval_hold );     // reduce KVAL_HOLD
              p += sprintf(p," - KVAL_HOLD reduced by %d", 2 * KVAL_HOLD_STEP_DOWN);   // let user know
              p += sprintf(p," to %d", kval_hold);   // let user know
            }
            else {
              driver_L6470_data[j].is_ot = false;
            }
          }
          else {
            driver_L6470_data[j].is_hi_Z = false;
          }

          if ( _status & STATUS_TH_WRN) {     // have an over temperature warning
            driver_L6470_data[j].is_otw = true;
            driver_L6470_data[j].otw_counter++;
            if (driver_L6470_data[j].otw_counter > 4) {  // otw present for 2 - 2.5 seconds, reduce KVAL_HOLD
              kval_hold = L6470_get_param(stepper_index, L6470_KVAL_HOLD) - KVAL_HOLD_STEP_DOWN;
              L6470_set_param(stepper_index, L6470_KVAL_HOLD, kval_hold );     // reduce KVAL_HOLD
              p += sprintf(p," - KVAL_HOLD reduced by %d", KVAL_HOLD_STEP_DOWN);   // let user know
              p += sprintf(p," to %d", kval_hold);   // let user know
              driver_L6470_data[j].otw_counter = 0;
              driver_L6470_data[j].is_otw = true;
            }
            else {
              if (driver_L6470_data[j].otw_counter) {
                p += sprintf(p," - thermal warning %d", kval_hold);   // warn user
              }
            }
          }
          else {
            driver_L6470_data[j].otw_counter = 0;   //clear out warning indicators
            driver_L6470_data[j].is_otw = false;
          }

          if ( _status & STATUS_OCD)
            p += sprintf(p, " overcurrent");

          if ( _status & (STATUS_STEP_LOSS_A |STATUS_STEP_LOSS_B ))
            p += sprintf(p, " stall");

          if ( _status & STATUS_UVLO)
            p += sprintf(p, " under voltage lock out");

          p += sprintf(p,"\n");


          #ifdef L6470_STOP_ON_ERROR
            if ( _status & (STATUS_UVLO | STATUS_TH_WRN | STATUS_TH_SD))
              kill(temp_buf);
          #endif

          SERIAL_ECHO(temp_buf);
          SERIAL_EOL();
        }  //end usual checks
      }
    } //comms re-established
  } // end L6470_monitor_update()


  #define MONITOR_L6470_DRIVE(Q)  L6470_monitor_update(Q , stepper##Q.getStatus())

  void monitor_L6470_driver() {
    static millis_t next_cOT = 0;
    if (ELAPSED(millis(), next_cOT)) {
      next_cOT = millis() + 500;

      #if AXIS_DRIVER_TYPE_X(ST_L6470)
        MONITOR_L6470_DRIVE(X);
      #endif
      #if AXIS_DRIVER_TYPE_Y(ST_L6470)
        MONITOR_L6470_DRIVE(Y);
      #endif
      #if AXIS_DRIVER_TYPE_Z(ST_L6470)
        MONITOR_L6470_DRIVE(Z);
      #endif
      #if AXIS_DRIVER_TYPE_X2(ST_L6470)
        MONITOR_L6470_DRIVE(X2);
      #endif
      #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
        MONITOR_L6470_DRIVE(Y2);
      #endif
      #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
        MONITOR_L6470_DRIVE(Z2);
      #endif
      #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
        MONITOR_L6470_DRIVE(Z3);
      #endif
      #if AXIS_DRIVER_TYPE_E0(ST_L6470)
        MONITOR_L6470_DRIVE(E0);
      #endif
      #if AXIS_DRIVER_TYPE_E1(ST_L6470)
        MONITOR_L6470_DRIVE(E1);
      #endif
      #if AXIS_DRIVER_TYPE_E2(ST_L6470)
        MONITOR_L6470_DRIVE(E2);
      #endif
      #if AXIS_DRIVER_TYPE_E3(ST_L6470)
        MONITOR_L6470_DRIVE(E3);
      #endif
      #if AXIS_DRIVER_TYPE_E4(ST_L6470)
        MONITOR_L6470_DRIVE(E4);
      #endif
      #if AXIS_DRIVER_TYPE_E5(ST_L6470)
        MONITOR_L6470_DRIVE(E5);
      #endif


      #if ENABLED(L6470_DEBUG)
        if (report_L6470_status) SERIAL_EOL();
      #endif
    }
  }

#endif //monitor status

#endif // HAS_DRIVER(ST_L6470)
