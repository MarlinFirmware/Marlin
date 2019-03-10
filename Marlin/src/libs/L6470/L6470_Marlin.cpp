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
 *  The monitor_driver routines are a close copy of the TMC code
 */

#include "../../inc/MarlinConfig.h"

#if HAS_DRIVER(L6470)

#include "L6470_Marlin.h"

L6470_Marlin L6470;

#include "../stepper_indirection.h"
#include "../../gcode/gcode.h"
#include "../planner.h"

uint8_t L6470_Marlin::dir_commands[MAX_L6470];  // array to hold direction command for each driver

char L6470_Marlin::index_to_axis[MAX_L6470][3] = { "X ", "Y ", "Z ", "X2", "Y2", "Z2", "Z3", "E0", "E1", "E2", "E3", "E4", "E5" };

bool L6470_Marlin::index_to_dir[MAX_L6470] =  {
  INVERT_X_DIR                        ,  // 0 X
  INVERT_Y_DIR                        ,  // 1 Y
  INVERT_Z_DIR                        ,  // 2 Z
  #if ENABLED(X_DUAL_STEPPER_DRIVERS)
    INVERT_X_DIR ^ INVERT_X2_VS_X_DIR ,  // 3 X2
  #else
    INVERT_X_DIR                      ,  // 3 X2
  #endif
  #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
    INVERT_Y_DIR ^ INVERT_Y2_VS_Y_DIR ,  // 4 Y2
  #else
    INVERT_Y_DIR                      ,  // 4 Y2
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

uint8_t L6470_Marlin::axis_xref[MAX_L6470] = {
  AxisEnum(X_AXIS), // X
  AxisEnum(Y_AXIS), // Y
  AxisEnum(Z_AXIS), // Z
  AxisEnum(X_AXIS), // X2
  AxisEnum(Y_AXIS), // Y2
  AxisEnum(Z_AXIS), // Z2
  AxisEnum(Z_AXIS), // Z3
  AxisEnum(E_AXIS), // E0
  AxisEnum(E_AXIS), // E1
  AxisEnum(E_AXIS), // E2
  AxisEnum(E_AXIS), // E3
  AxisEnum(E_AXIS), // E4
  AxisEnum(E_AXIS)  // E5
};

volatile bool L6470_Marlin::spi_abort = false;
bool L6470_Marlin::spi_active = false;

void L6470_Marlin::populate_chain_array() {

  #define _L6470_INIT_SPI(Q)  do{ stepper##Q.set_chain_info(Q, Q##_CHAIN_POS); }while(0)

  #if AXIS_DRIVER_TYPE_X(L6470)
    _L6470_INIT_SPI(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(L6470)
    _L6470_INIT_SPI(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(L6470)
    _L6470_INIT_SPI(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(L6470)
    _L6470_INIT_SPI(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(L6470)
    _L6470_INIT_SPI(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(L6470)
    _L6470_INIT_SPI(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(L6470)
    _L6470_INIT_SPI(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_E0(L6470)
    _L6470_INIT_SPI(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(L6470)
    _L6470_INIT_SPI(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(L6470)
    _L6470_INIT_SPI(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(L6470)
    _L6470_INIT_SPI(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(L6470)
    _L6470_INIT_SPI(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(L6470)
    _L6470_INIT_SPI(E5);
  #endif
}

void L6470_Marlin::init() {               // Set up SPI and then init chips
  #if PIN_EXISTS(L6470_RESET_CHAIN)
    OUT_WRITE(L6470_RESET_CHAIN_PIN, LOW);  // hardware reset of drivers
    delay(1);
    OUT_WRITE(L6470_RESET_CHAIN_PIN, HIGH);
    delay(1);                     // need about 650uS for the chip to fully start up
  #endif
  populate_chain_array();   // Set up array to control where in the SPI transfer sequence a particular stepper's data goes
  L6470_spi_init();               // Set up L6470 soft SPI pins
  init_to_defaults();             // init the chips
}

uint16_t L6470_Marlin::get_status(const uint8_t axis) {

  #define GET_L6470_STATUS(Q) stepper##Q.getStatus()

  switch (axis) {
    #if AXIS_DRIVER_TYPE_X(L6470)
      case  0: return GET_L6470_STATUS(X);
    #endif
    #if AXIS_DRIVER_TYPE_Y(L6470)
      case  1: return GET_L6470_STATUS(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Z(L6470)
      case  2: return GET_L6470_STATUS(Z);
    #endif
    #if AXIS_DRIVER_TYPE_X2(L6470)
      case  3: return GET_L6470_STATUS(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(L6470)
      case  4: return GET_L6470_STATUS(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(L6470)
      case  5: return GET_L6470_STATUS(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(L6470)
      case  6: return GET_L6470_STATUS(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(L6470)
      case  7: return GET_L6470_STATUS(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(L6470)
      case  8: return GET_L6470_STATUS(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(L6470)
      case  9: return GET_L6470_STATUS(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(L6470)
      case 10: return GET_L6470_STATUS(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(L6470)
      case 11: return GET_L6470_STATUS(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(L6470)
      case 12: return GET_L6470_STATUS(E5);
    #endif
  }

  return 0; // Not needed but kills a compiler warning
}

uint32_t L6470_Marlin::get_param(uint8_t axis, uint8_t param) {

  #define GET_L6470_PARAM(Q) L6470_GETPARAM(param,Q)

  switch (axis) {
    #if AXIS_DRIVER_TYPE_X(L6470)
      case  0: return GET_L6470_PARAM(X);
    #endif
    #if AXIS_DRIVER_TYPE_Y(L6470)
      case  1: return GET_L6470_PARAM(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Z(L6470)
      case  2: return GET_L6470_PARAM(Z);
    #endif
    #if AXIS_DRIVER_TYPE_X2(L6470)
      case  3: return GET_L6470_PARAM(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(L6470)
      case  4: return GET_L6470_PARAM(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(L6470)
      case  5: return GET_L6470_PARAM(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(L6470)
      case  6: return GET_L6470_PARAM(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(L6470)
      case  7: return GET_L6470_PARAM(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(L6470)
      case  8: return GET_L6470_PARAM(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(L6470)
      case  9: return GET_L6470_PARAM(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(L6470)
      case 10: return GET_L6470_PARAM(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(L6470)
      case 11: return GET_L6470_PARAM(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(L6470)
      case 12: return GET_L6470_PARAM(E5);
    #endif
  }

  return 0 ; // not needed but kills a compiler warning
}

void L6470_Marlin::set_param(uint8_t axis, uint8_t param, uint32_t value) {

  #define SET_L6470_PARAM(Q) stepper##Q.SetParam(param, value)

  switch (axis) {
    #if AXIS_DRIVER_TYPE_X(L6470)
      case  0: SET_L6470_PARAM(X);
    #endif
    #if AXIS_DRIVER_TYPE_Y(L6470)
      case  1: SET_L6470_PARAM(Y);
    #endif
    #if AXIS_DRIVER_TYPE_Z(L6470)
      case  2: SET_L6470_PARAM(Z);
    #endif
    #if AXIS_DRIVER_TYPE_X2(L6470)
      case  3: SET_L6470_PARAM(X2);
    #endif
    #if AXIS_DRIVER_TYPE_Y2(L6470)
      case  4: SET_L6470_PARAM(Y2);
    #endif
    #if AXIS_DRIVER_TYPE_Z2(L6470)
      case  5: SET_L6470_PARAM(Z2);
    #endif
    #if AXIS_DRIVER_TYPE_Z3(L6470)
      case  6: SET_L6470_PARAM(Z3);
    #endif
    #if AXIS_DRIVER_TYPE_E0(L6470)
      case  7: SET_L6470_PARAM(E0);
    #endif
    #if AXIS_DRIVER_TYPE_E1(L6470)
      case  8: SET_L6470_PARAM(E1);
    #endif
    #if AXIS_DRIVER_TYPE_E2(L6470)
      case  9: SET_L6470_PARAM(E2);
    #endif
    #if AXIS_DRIVER_TYPE_E3(L6470)
      case 10: SET_L6470_PARAM(E3);
    #endif
    #if AXIS_DRIVER_TYPE_E4(L6470)
      case 11: SET_L6470_PARAM(E4);
    #endif
    #if AXIS_DRIVER_TYPE_E5(L6470)
      case 12: SET_L6470_PARAM(E5);
    #endif
  }
}

inline void echo_min_max(const char a, const float &min, const float &max) {
  L6470_CHAR(' '); L6470_CHAR(a);
  L6470_ECHOPAIR(" min = ", min);
  L6470_ECHOLNPAIR("  max = ", max);
}
inline void echo_oct_used(const float &oct, const bool stall) {
  L6470_ECHOPAIR("over_current_threshold used     : ", oct);
  serialprintPGM(stall ? PSTR("  (Stall") : PSTR("  (OCD"));
  L6470_ECHOLNPGM(" threshold)");
}
inline void err_out_of_bounds() { L6470_ECHOLNPGM("ERROR - motion out of bounds"); }

bool L6470_Marlin::get_user_input(uint8_t &driver_count, uint8_t axis_index[3], char axis_mon[3][3],
                          float &position_max, float &position_min, float &final_feedrate, uint8_t &kval_hold,
                          bool over_current_flag, uint8_t &OCD_TH_val, uint8_t &STALL_TH_val, uint16_t &over_current_threshold
) {
  // Return TRUE if the calling routine needs to abort/kill

  uint16_t displacement = 0;  // " = 0" to eliminate compiler warning
  uint8_t j;   // general purpose counter

  if (!all_axes_homed()) {
    L6470_ECHOLNPGM("ERROR - home all before running this command");
    //return true;
  }

  LOOP_XYZE(i) if (uint16_t _displacement = parser.intval(axis_codes[i])) {
    displacement = _displacement;
    uint8_t axis_offset = parser.byteval('J');
    axis_mon[0][0] = axis_codes[i];   // axis ASCII value (target character)
    if (axis_offset >= 2 || axis_mon[0][0] == 'E')  // Single axis, E0, or E1
      axis_mon[0][1] = axis_offset + '0';
    else if (axis_offset == 0) {              // one or more axes
      uint8_t driver_count_local = 0;         // can't use "driver_count" directly as a subscript because it's passed by reference
      for (j = 0; j < MAX_L6470; j++)         // see how many drivers on this axis
        if (axis_mon[0][0] == index_to_axis[j][0]) {
          axis_mon[driver_count_local][0] = axis_mon[0][0];
          axis_mon[driver_count_local][1] = index_to_axis[j][1];
          axis_mon[driver_count_local][2] = index_to_axis[j][2];   // append end of string
          axis_index[driver_count_local] = j;                      // set axis index
          driver_count_local++;
        }
      driver_count = driver_count_local;
    }
    break; // only take first axis found
  }

  //
  // Position calcs & checks
  //

  const float center[] = {
    LOGICAL_X_POSITION(current_position[X_AXIS]),
    LOGICAL_Y_POSITION(current_position[Y_AXIS]),
    LOGICAL_Z_POSITION(current_position[Z_AXIS]),
    current_position[E_AXIS]
  };

  switch (axis_mon[0][0]) {
    default: position_max = position_min = 0; break;

    case 'X': {
      position_min = center[X_AXIS] - displacement;
      position_max = center[X_AXIS] + displacement;
      echo_min_max('X', position_min, position_max);
      if (false
        #ifdef X_MIN_POS
          || position_min < (X_MIN_POS)
        #endif
        #ifdef X_MAX_POS
          || position_max > (X_MAX_POS)
        #endif
      ) {
        err_out_of_bounds();
        return true;
      }
    } break;

    case 'Y': {
      position_min = center[Y_AXIS] - displacement;
      position_max = center[Y_AXIS] + displacement;
      echo_min_max('Y', position_min, position_max);
      if (false
        #ifdef Y_MIN_POS
          || position_min < (Y_MIN_POS)
        #endif
        #ifdef Y_MAX_POS
          || position_max > (Y_MAX_POS)
        #endif
      ) {
        err_out_of_bounds();
        return true;
      }
    } break;

    case 'Z': {
      position_min = center[E_AXIS] - displacement;
      position_max = center[E_AXIS] + displacement;
      echo_min_max('Z', position_min, position_max);
      if (false
        #ifdef Z_MIN_POS
          || position_min < (Z_MIN_POS)
        #endif
        #ifdef Z_MAX_POS
          || position_max > (Z_MAX_POS)
        #endif
      ) {
        err_out_of_bounds();
        return true;
      }
    } break;

    case 'E': {
      position_min = center[E_AXIS] - displacement;
      position_max = center[E_AXIS] + displacement;
      echo_min_max('E', position_min, position_max);
    } break;
  }

  //
  // Work on the drivers
  //
  for (uint8_t k = 0; k < driver_count; k++) {
    bool not_found = true;
    for (j = 1; j <= L6470::chain[0]; j++) {
      const char * const ind_axis = index_to_axis[L6470::chain[j]];
      if (ind_axis[0] == axis_mon[k][0] && ind_axis[1] == axis_mon[k][1]) { // See if a L6470 driver
        not_found = false;
        break;
      }
    }
    if (not_found) {
      driver_count = k;
      axis_mon[k][0] = ' ';  // mark this entry invalid
      break;
    }
  }

  if (driver_count == 0) {
    L6470_ECHOLNPGM("ERROR - not a L6470 axis");
    return true;
  }

  L6470_ECHOPGM("Monitoring:");
  for (j = 0; j < driver_count; j++) L6470_ECHOPAIR("  ", axis_mon[j]);
  L6470_EOL();

  // now have a list of driver(s) to monitor

  //
  // kVAL_HOLD checks & settings
  //

  kval_hold = parser.byteval('K');
  if (kval_hold) {
    L6470_ECHOLNPAIR("kval_hold = ", kval_hold);
    for (j = 0; j < driver_count; j++)
      set_param(axis_index[j], L6470_KVAL_HOLD, kval_hold);
  }
  else {
    // only print the KVAL_HOLD from one of the drivers
    kval_hold = get_param(axis_index[0], L6470_KVAL_HOLD);
    L6470_ECHOLNPAIR("KVAL_HOLD = ", kval_hold);
  }

  //
  // Overcurrent checks & settings
  //

  if (over_current_flag) {

    uint8_t OCD_TH_val_local = 0,       // compiler thinks OCD_TH_val is unused if use it directly
            STALL_TH_val_local = 0;     // just in case ...

    over_current_threshold = parser.intval('I');

    if (over_current_threshold) {

      OCD_TH_val_local = over_current_threshold/375;
      LIMIT(OCD_TH_val_local, 0, 15);
      STALL_TH_val_local = over_current_threshold/31.25;
      LIMIT(STALL_TH_val_local, 0, 127);
      uint16_t OCD_TH_actual = (OCD_TH_val_local + 1) * 375,
               STALL_TH_actual = (STALL_TH_val_local + 1) * 31.25;
      if (OCD_TH_actual < STALL_TH_actual) {
        OCD_TH_val_local++;
        OCD_TH_actual = (OCD_TH_val_local + 1) * 375;
      }

      L6470_ECHOLNPAIR("over_current_threshold specified: ", over_current_threshold);
      echo_oct_used(STALL_TH_actual, true);
      echo_oct_used(OCD_TH_actual, false);

      #define SET_OVER_CURRENT(Q) do { stepper##Q.SetParam(L6470_STALL_TH, STALL_TH_val_local); stepper##Q.SetParam(L6470_OCD_TH, OCD_TH_val_local);} while (0)

      for (j = 0; j < driver_count; j++) {
        set_param(axis_index[j], L6470_STALL_TH, STALL_TH_val_local);
        set_param(axis_index[j], L6470_OCD_TH, OCD_TH_val_local);
      }
    }
    else {
      // only get & print the OVER_CURRENT values from one of the drivers
      STALL_TH_val_local = get_param(axis_index[0], L6470_STALL_TH);
      OCD_TH_val_local = get_param(axis_index[0], L6470_OCD_TH);

      echo_oct_used((STALL_TH_val_local + 1) * 31.25, true);
      echo_oct_used((OCD_TH_val_local + 1) * 375, false);
    } // over_current_threshold

    for (j = 0; j < driver_count; j++) {                 // set all drivers on axis the same
      set_param(axis_index[j], L6470_STALL_TH, STALL_TH_val_local);
      set_param(axis_index[j], L6470_OCD_TH, OCD_TH_val_local);
    }

    OCD_TH_val = OCD_TH_val_local;        // force compiler to update the main routine's copy
    STALL_TH_val = STALL_TH_val_local;    // force compiler to update the main routine's copy
  } // end of overcurrent

  //
  // Feedrate
  //

  final_feedrate = parser.floatval('F');
  if (final_feedrate == 0) {
    static constexpr float default_max_feedrate[] = DEFAULT_MAX_FEEDRATE;
    const uint8_t num_feedrates = COUNT(default_max_feedrate);
    for (j = 0; j < num_feedrates; j++) {
      if (axis_codes[j] == axis_mon[0][0]) {
        final_feedrate = default_max_feedrate[j];
        break;
      }
    }
    if (j == 3 && num_feedrates > 4) {   // have more than one extruder feedrate
      uint8_t extruder_num = axis_mon[0][1] - '0';
      if (j <= num_feedrates - extruder_num)     // have a feedrate specifically for this extruder
        final_feedrate = default_max_feedrate[j + extruder_num];
      else
        final_feedrate = default_max_feedrate[3];  // use E0 feedrate for this extruder
    }
    final_feedrate *= 60;  // convert to mm/minute
  } // end of feedrate

  return false;   // FALSE indicates no user input problems
}

#if ENABLED(L6470_CHITCHAT)
  inline void echo_yes_no(const bool yes) { serialprintPGM(yes ? PSTR("YES") : PSTR("NO ")); }
#endif

void L6470_Marlin::say_axis(const uint8_t axis, const bool label/*=true*/) {
  if (label) SERIAL_ECHOPGM("AXIS:");
  SERIAL_CHAR(' ');
  SERIAL_CHAR(index_to_axis[axis][0]);
  SERIAL_CHAR(index_to_axis[axis][1]);
  SERIAL_CHAR(' ');
}

void L6470_Marlin::error_status_decode(const uint16_t status, const uint8_t axis) {  // assumes status bits have been inverted
  #if ENABLED(L6470_CHITCHAT)
    char temp_buf[10];
    say_axis(axis);
    sprintf_P(temp_buf, PSTR("  %4x   "), status);
    L6470_ECHO(temp_buf);
    print_bin(status);
    L6470_ECHOPGM("  THERMAL: ");
    serialprintPGM((status & STATUS_TH_SD) ? PSTR("SHUTDOWN") : (status & STATUS_TH_WRN) ? PSTR("WARNING ") : PSTR("OK      "));
    L6470_ECHOPGM("   OVERCURRENT: ");
    echo_yes_no(status & STATUS_OCD);
    L6470_ECHOPGM("   STALL: ");
    echo_yes_no(status & (STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B));
    L6470_EOL();
  #else
    UNUSED(status); UNUSED(axis);
  #endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////
////  MONITOR_L6470_DRIVER_STATUS routines
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
    #if AXIS_DRIVER_TYPE_X(L6470)
      {  0, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_Y(L6470)
      {  1, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_Z(L6470)
      {  2, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_X2(L6470)
      {  3, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_Y2(L6470)
      {  4, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_Z2(L6470)
      {  5, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_Z3(L6470)
      {  6, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_E0(L6470)
      {  7, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_E1(L6470)
      {  8, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_E2(L6470)
      {  9, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_E3(L6470)
      { 10, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_E4(L6470)
      { 11, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_DRIVER_TYPE_E5(L6470)
      { 12, 0, 0, 0, 0, 0, 0 }
    #endif
  };

  inline void append_stepper_err(char * &p, const uint8_t stepper_index, const char * const err=NULL) {
    p += sprintf_P(p, PSTR("Stepper %c%c "), char(index_to_axis[stepper_index][0]), char(index_to_axis[stepper_index][1]));
    if (err) p += sprintf_P(p, err);
  }

  void L6470_monitor_update(uint8_t stepper_index, uint16_t status) {
    if (spi_abort) return;  // don't do anything if set_directions() has occurred
    uint8_t kval_hold;
    char temp_buf[120];
    char* p = &temp_buf[0];
    uint8_t j;
    for (j = 0; j < L6470::chain[0]; j++) // find the table for this stepper
      if (driver_L6470_data[j].driver_index == stepper_index) break;

    driver_L6470_data[j].driver_status = status;
    uint16_t _status = ~status;     // all error bits are active low

    if (status == 0 || status == 0xFFFF) {              // com problem
      if (driver_L6470_data[j].com_counter == 0) {      // warn user when it first happens
        driver_L6470_data[j].com_counter++;
        append_stepper_err(p, stepper_index, PSTR(" - communications lost\n"));
        L6470_ECHO(temp_buf);
      }
      else {
        driver_L6470_data[j].com_counter++;
        if (driver_L6470_data[j].com_counter > 240) {  // remind of com problem about every 2 minutes
          driver_L6470_data[j].com_counter = 1;
          append_stepper_err(p, stepper_index, PSTR(" - still no communications\n"));
          L6470_ECHO(temp_buf);
        }
      }
    }
    else {
      if (driver_L6470_data[j].com_counter) {   // comms re-established
        driver_L6470_data[j].com_counter = 0;
        append_stepper_err(p, stepper_index, PSTR(" - communications re-established\n.. setting all drivers to default values\n"));
        L6470_ECHO(temp_buf);
        init_to_defaults();
      }
      else {
        // no com problems - do the usual checks
        if (_status & L6470_ERROR_MASK) {
          append_stepper_err(p, stepper_index);

          if (status & STATUS_HIZ) {                         // the driver has shut down  HiZ is active high
            driver_L6470_data[j].is_hi_Z = true;
            p += sprintf_P(p, PSTR("%cIS SHUT DOWN"), ' ');
            //         if (_status & STATUS_TH_SD) {                     // strange - TH_SD never seems to go active, must be implied by the HiZ and TH_WRN
            if (_status & STATUS_TH_WRN) {                    // over current shutdown
              p += sprintf_P(p, PSTR("%cdue to over temperature"), ' ');
              driver_L6470_data[j].is_ot = true;
              kval_hold = get_param(stepper_index, L6470_KVAL_HOLD) - 2 * KVAL_HOLD_STEP_DOWN;
              set_param(stepper_index, L6470_KVAL_HOLD, kval_hold);     // reduce KVAL_HOLD
              p += sprintf_P(p, PSTR(" - KVAL_HOLD reduced by %d to %d"), 2 * KVAL_HOLD_STEP_DOWN, kval_hold);   // let user know
            }
            else
              driver_L6470_data[j].is_ot = false;
          }
          else {
            driver_L6470_data[j].is_hi_Z = false;

            if (_status & STATUS_TH_WRN) {     // have an over temperature warning
              driver_L6470_data[j].is_otw = true;
              driver_L6470_data[j].otw_counter++;
              kval_hold = get_param(stepper_index, L6470_KVAL_HOLD);
              if (driver_L6470_data[j].otw_counter > 4) {  // otw present for 2 - 2.5 seconds, reduce KVAL_HOLD
                kval_hold -= KVAL_HOLD_STEP_DOWN;
                set_param(stepper_index, L6470_KVAL_HOLD, kval_hold);     // reduce KVAL_HOLD
                p += sprintf_P(p, PSTR(" - KVAL_HOLD reduced by %d to %d"), KVAL_HOLD_STEP_DOWN, kval_hold);   // let user know
                driver_L6470_data[j].otw_counter = 0;
                driver_L6470_data[j].is_otw = true;
              }
              else if (driver_L6470_data[j].otw_counter)
                p += sprintf_P(p, PSTR("%c- thermal warning"), ' ');   // warn user
            }
          }

          #ifdef L6470_STOP_ON_ERROR
            if (_status & (STATUS_UVLO | STATUS_TH_WRN | STATUS_TH_SD))
            kill(temp_buf);
          #endif


          #if ENABLED(L6470_CHITCHAT)

            if (_status & STATUS_OCD)
            p += sprintf_P(p, PSTR("%c  over current"), ' ');

            if (_status & (STATUS_STEP_LOSS_A | STATUS_STEP_LOSS_B))
            p += sprintf_P(p, PSTR("%c  stall"), ' ');

            if (_status & STATUS_UVLO)
            p += sprintf_P(p, PSTR("%c  under voltage lock out"), ' ');

            p += sprintf_P(p, PSTR("%c\n"), ' ');
          #endif

          L6470_ECHOLN(temp_buf);  // print the error message
        }
        else {
          driver_L6470_data[j].is_ot = false;
          driver_L6470_data[j].otw_counter = 0;   //clear out warning indicators
          driver_L6470_data[j].is_otw = false;

        } // end usual checks
      } // comms established but have errors
    } // comms re-established
  } // end L6470_monitor_update()

  #define MONITOR_L6470_DRIVE(Q) L6470_monitor_update(Q, stepper##Q.getStatus())

  void L6470_Marlin::monitor_driver() {
    static millis_t next_cOT = 0;
    if (ELAPSED(millis(), next_cOT)) {
      next_cOT = millis() + 500;

      spi_active = true;    // let set_directions() know we're in the middle of a series of SPI transfers

      #if AXIS_DRIVER_TYPE_X(L6470)
        MONITOR_L6470_DRIVE(X);
      #endif
      #if AXIS_DRIVER_TYPE_Y(L6470)
        MONITOR_L6470_DRIVE(Y);
      #endif
      #if AXIS_DRIVER_TYPE_Z(L6470)
        MONITOR_L6470_DRIVE(Z);
      #endif
      #if AXIS_DRIVER_TYPE_X2(L6470)
        MONITOR_L6470_DRIVE(X2);
      #endif
      #if AXIS_DRIVER_TYPE_Y2(L6470)
        MONITOR_L6470_DRIVE(Y2);
      #endif
      #if AXIS_DRIVER_TYPE_Z2(L6470)
        MONITOR_L6470_DRIVE(Z2);
      #endif
      #if AXIS_DRIVER_TYPE_Z3(L6470)
        MONITOR_L6470_DRIVE(Z3);
      #endif
      #if AXIS_DRIVER_TYPE_E0(L6470)
        MONITOR_L6470_DRIVE(E0);
      #endif
      #if AXIS_DRIVER_TYPE_E1(L6470)
        MONITOR_L6470_DRIVE(E1);
      #endif
      #if AXIS_DRIVER_TYPE_E2(L6470)
        MONITOR_L6470_DRIVE(E2);
      #endif
      #if AXIS_DRIVER_TYPE_E3(L6470)
        MONITOR_L6470_DRIVE(E3);
      #endif
      #if AXIS_DRIVER_TYPE_E4(L6470)
        MONITOR_L6470_DRIVE(E4);
      #endif
      #if AXIS_DRIVER_TYPE_E5(L6470)
        MONITOR_L6470_DRIVE(E5);
      #endif

      #if ENABLED(L6470_DEBUG)
        if (report_L6470_status) L6470_EOL();
      #endif

      spi_active = false;   // done with all SPI transfers - clear handshake flags
      spi_abort = false;
    }
  }

#endif // MONITOR_L6470_DRIVER_STATUS

#endif // HAS_DRIVER(L6470)
