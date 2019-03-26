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

#if HAS_L64XX

#include "L6470_Marlin.h"

L64XX_Marlin L64xx_MARLIN;

#include "../../module/stepper_indirection.h"
#include "../../gcode/gcode.h"
#include "../../module/planner.h"

const char * const L64XX_Marlin::index_to_axis[MAX_L6470] = { "X ", "Y ", "Z ", "X2", "Y2", "Z2", "Z3", "E0", "E1", "E2", "E3", "E4", "E5" };

#define DEBUG_OUT ENABLED(L6470_CHITCHAT)
#include "../../core/debug_out.h"

uint8_t L64XX_Marlin::dir_commands[MAX_L6470];  // array to hold direction command for each driver

bool L64XX_Marlin::index_to_dir[MAX_L6470] =  { INVERT_X_DIR                        ,  // 0 X
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
                                                INVERT_E5_DIR                          //12 E5
                                              };

volatile bool L64XX_Marlin::spi_abort = false;
bool L64XX_Marlin::spi_active = false;

L64XX_Marlin::L64XX_shadow_t L64XX_Marlin::shadow;

//uint32_t UVLO_ADC = 0x0400; // ADC undervoltage event

void L6470_populate_chain_array() {

  #define _L6470_INIT_SPI(Q)  do{ stepper##Q.set_chain_info(Q, Q##_CHAIN_POS); }while(0)

  #if AXIS_IS_L64XX(X)
    _L6470_INIT_SPI(X);
  #endif
  #if AXIS_IS_L64XX(X2)
    _L6470_INIT_SPI(X2);
  #endif
  #if AXIS_IS_L64XX(Y)
    _L6470_INIT_SPI(Y);
  #endif
  #if AXIS_IS_L64XX(Y2)
    _L6470_INIT_SPI(Y2);
  #endif
  #if AXIS_IS_L64XX(Z)
    _L6470_INIT_SPI(Z);
  #endif
  #if AXIS_IS_L64XX(Z2)
    _L6470_INIT_SPI(Z2);
  #endif
  #if AXIS_IS_L64XX(Z3)
    _L6470_INIT_SPI(Z3);
  #endif
  #if AXIS_IS_L64XX(E0)
    _L6470_INIT_SPI(E0);
  #endif
  #if AXIS_IS_L64XX(E1)
    _L6470_INIT_SPI(E1);
  #endif
  #if AXIS_IS_L64XX(E2)
    _L6470_INIT_SPI(E2);
  #endif
  #if AXIS_IS_L64XX(E3)
    _L6470_INIT_SPI(E3);
  #endif
  #if AXIS_IS_L64XX(E4)
    _L6470_INIT_SPI(E4);
  #endif
  #if AXIS_IS_L64XX(E5)
    _L6470_INIT_SPI(E5);
  #endif
}

void L64XX_Marlin::init() {               // Set up SPI and then init chips
  #ifdef L6470_RESET_CHAIN_PIN
    OUT_WRITE(L6470_RESET_CHAIN_PIN,0);   // hardware reset of drivers
    delay(1);
    OUT_WRITE(L6470_RESET_CHAIN_PIN,1);
    delay(1);                             // need about 650uS for the chip to fully start up
  #endif
  L6470_populate_chain_array();           // Set up array to control where in the SPI transfer sequence a particular stepper's data goes

  spi_init();                             // Since L64XX SPI pins are unset we must init SPI here

  init_to_defaults();                     // init the chips
}

/**
 * Some status bit positions & definitions change depending on the driver chip.  Copy all status info into known locations
 * so check/display logic is simplified
 *   1. copy stepper status
 *   2. copy status bit definitions
 *   3. copy status layout
 *   4. make all error bits active low (if needed)
 */
uint16_t L64XX_Marlin::get_stepper_status(L64XX &st) {
  shadow.STATUS_AXIS_RAW           = st.getStatus();
  shadow.STATUS_AXIS               = shadow.STATUS_AXIS_RAW;
  shadow.STATUS_AXIS_LAYOUT        = st.L6470_status_layout;
  shadow.AXIS_OCD_TH_MAX           = st.OCD_TH_MAX;
  shadow.AXIS_STALL_TH_MAX         = st.STALL_TH_MAX;
  shadow.AXIS_OCD_CURRENT_CONSTANT_INV   = st.OCD_CURRENT_CONSTANT_INV;
  shadow.AXIS_STALL_CURRENT_CONSTANT_INV = st.STALL_CURRENT_CONSTANT_INV;
  shadow.L6470_AXIS_CONFIG         = st.L64XX_CONFIG;
  shadow.L6470_AXIS_STATUS         = st.L64XX_STATUS;
  shadow.STATUS_AXIS_OCD           = st.STATUS_OCD;
  shadow.STATUS_AXIS_SCK_MOD       = st.STATUS_SCK_MOD;
  shadow.STATUS_AXIS_STEP_LOSS_A   = st.STATUS_STEP_LOSS_A;
  shadow.STATUS_AXIS_STEP_LOSS_B   = st.STATUS_STEP_LOSS_B;
  shadow.STATUS_AXIS_TH_SD         = st.STATUS_TH_SD;
  shadow.STATUS_AXIS_TH_WRN        = st.STATUS_TH_WRN;
  shadow.STATUS_AXIS_UVLO          = st.STATUS_UVLO;
  shadow.STATUS_AXIS_WRONG_CMD     = st.STATUS_WRONG_CMD;
  shadow.STATUS_AXIS_CMD_ERR       = st.STATUS_CMD_ERR;
  shadow.STATUS_AXIS_NOTPERF_CMD   = st.STATUS_NOTPERF_CMD;
  shadow.L6470_ERROR_MASK          = shadow.STATUS_AXIS_UVLO | shadow.STATUS_AXIS_TH_WRN | shadow.STATUS_AXIS_TH_SD | shadow.STATUS_AXIS_OCD | shadow.STATUS_AXIS_STEP_LOSS_A | shadow.STATUS_AXIS_STEP_LOSS_B;
  shadow.STATUS_AXIS_LAYOUT        = st.L6470_status_layout;
  if (!shadow.STATUS_AXIS_LAYOUT) shadow.STATUS_AXIS ^= (shadow.STATUS_AXIS_TH_SD | shadow.STATUS_AXIS_TH_WRN);
  return shadow.STATUS_AXIS;
}

uint16_t L64XX_Marlin::get_status(const L6470_axis_t axis) {

  #define STATUS_L6470(Q) get_stepper_status(stepper##Q)

  switch (axis) {
    default: break;
    #if AXIS_IS_L64XX(X)
      case X : return STATUS_L6470(X);
    #endif
    #if AXIS_IS_L64XX(Y)
      case Y : return STATUS_L6470(Y);
    #endif
    #if AXIS_IS_L64XX(Z)
      case Z : return STATUS_L6470(Z);
    #endif
    #if AXIS_IS_L64XX(X2)
      case X2: return STATUS_L6470(X2);
    #endif
    #if AXIS_IS_L64XX(Y2)
      case Y2: return STATUS_L6470(Y2);
    #endif
    #if AXIS_IS_L64XX(Z2)
      case Z2: return STATUS_L6470(Z2);
    #endif
    #if AXIS_IS_L64XX(Z3)
      case Z3: return STATUS_L6470(Z3);
    #endif
    #if AXIS_IS_L64XX(E0)
      case E0: return STATUS_L6470(E0);
    #endif
    #if AXIS_IS_L64XX(E1)
      case E1: return STATUS_L6470(E1);
    #endif
    #if AXIS_IS_L64XX(E2)
      case E2: return STATUS_L6470(E2);
    #endif
    #if AXIS_IS_L64XX(E3)
      case E3: return STATUS_L6470(E3);
    #endif
    #if AXIS_IS_L64XX(E4)
      case E4: return STATUS_L6470(E4);
    #endif
    #if AXIS_IS_L64XX(E5)
      case E5: return STATUS_L6470(E5);
    #endif
  }

  return 0; // Not needed but kills a compiler warning
}

uint32_t L64XX_Marlin::get_param(const L6470_axis_t axis, const uint8_t param) {

  #define GET_L6470_PARAM(Q) L6470_GETPARAM(param,Q)

  switch (axis) {
    default: break;
    #if AXIS_IS_L64XX(X)
      case X : return GET_L6470_PARAM(X);
    #endif
    #if AXIS_IS_L64XX(Y)
      case Y : return GET_L6470_PARAM(Y);
    #endif
    #if AXIS_IS_L64XX(Z)
      case Z : return GET_L6470_PARAM(Z);
    #endif
    #if AXIS_IS_L64XX(X2)
      case X2: return GET_L6470_PARAM(X2);
    #endif
    #if AXIS_IS_L64XX(Y2)
      case Y2: return GET_L6470_PARAM(Y2);
    #endif
    #if AXIS_IS_L64XX(Z2)
      case Z2: return GET_L6470_PARAM(Z2);
    #endif
    #if AXIS_IS_L64XX(Z3)
      case Z3: return GET_L6470_PARAM(Z3);
    #endif
    #if AXIS_IS_L64XX(E0)
      case E0: return GET_L6470_PARAM(E0);
    #endif
    #if AXIS_IS_L64XX(E1)
      case E1: return GET_L6470_PARAM(E1);
    #endif
    #if AXIS_IS_L64XX(E2)
      case E2: return GET_L6470_PARAM(E2);
    #endif
    #if AXIS_IS_L64XX(E3)
      case E3: return GET_L6470_PARAM(E3);
    #endif
    #if AXIS_IS_L64XX(E4)
      case E4: return GET_L6470_PARAM(E4);
    #endif
    #if AXIS_IS_L64XX(E5)
      case E5: return GET_L6470_PARAM(E5);
    #endif
  }

  return 0 ; // not needed but kills a compiler warning
}

void L64XX_Marlin::set_param(const L6470_axis_t axis, const uint8_t param, const uint32_t value) {

  #define SET_L6470_PARAM(Q) stepper##Q.SetParam(param, value)

  switch (axis) {
    default: break;
    #if AXIS_IS_L64XX(X)
      case X : SET_L6470_PARAM(X);
    #endif
    #if AXIS_IS_L64XX(Y)
      case Y : SET_L6470_PARAM(Y);
    #endif
    #if AXIS_IS_L64XX(Z)
      case Z : SET_L6470_PARAM(Z);
    #endif
    #if AXIS_IS_L64XX(X2)
      case X2: SET_L6470_PARAM(X2);
    #endif
    #if AXIS_IS_L64XX(Y2)
      case Y2: SET_L6470_PARAM(Y2);
    #endif
    #if AXIS_IS_L64XX(Z2)
      case Z2: SET_L6470_PARAM(Z2);
    #endif
    #if AXIS_IS_L64XX(Z3)
      case Z3: SET_L6470_PARAM(Z3);
    #endif
    #if AXIS_IS_L64XX(E0)
      case E0: SET_L6470_PARAM(E0);
    #endif
    #if AXIS_IS_L64XX(E1)
      case E1: SET_L6470_PARAM(E1);
    #endif
    #if AXIS_IS_L64XX(E2)
      case E2: SET_L6470_PARAM(E2);
    #endif
    #if AXIS_IS_L64XX(E3)
      case E3: SET_L6470_PARAM(E3);
    #endif
    #if AXIS_IS_L64XX(E4)
      case E4: SET_L6470_PARAM(E4);
    #endif
    #if AXIS_IS_L64XX(E5)
      case E5: SET_L6470_PARAM(E5);
    #endif
  }
}

inline void echo_min_max(const char a, const float &min, const float &max) {
  DEBUG_CHAR(' '); DEBUG_CHAR(a);
  DEBUG_ECHOPAIR(" min = ", min);
  DEBUG_ECHOLNPAIR("  max = ", max);
}
inline void echo_oct_used(const float &oct, const bool stall) {
  DEBUG_ECHOPAIR("over_current_threshold used     : ", oct);
  serialprintPGM(stall ? PSTR("  (Stall") : PSTR("  (OCD"));
  DEBUG_ECHOLNPGM(" threshold)");
}
inline void err_out_of_bounds() { DEBUG_ECHOLNPGM("ERROR - motion out of bounds"); }

bool L64XX_Marlin::get_user_input(uint8_t &driver_count, L6470_axis_t axis_index[3],  char axis_mon[3][3],
                                 float &position_max, float &position_min, float &final_feedrate, uint8_t &kval_hold,
                                 bool over_current_flag, uint8_t &OCD_TH_val, uint8_t &STALL_TH_val, uint16_t &over_current_threshold
) {
  // Return TRUE if the calling routine needs to abort/kill

  uint16_t displacement = 0;  // " = 0" to eliminate compiler warning
  uint8_t j;   // general purpose counter

  if (!all_axes_homed()) {
    DEBUG_ECHOLNPGM("ERROR - home all before running this command");
    //return true;
  }

  LOOP_XYZE(i) if (uint16_t _displacement = parser.intval(axis_codes[i])) {
    displacement = _displacement;
    uint8_t axis_offset = parser.byteval('J');
    axis_mon[0][0] = axis_codes[i];   // axis ASCII value (target character)
    if (axis_offset >= 2 || axis_mon[0][0] == 'E')  // Single axis, E0, or E1
      axis_mon[0][1] = axis_offset + '0';
    else if (axis_offset == 0) {              // One or more axes
      uint8_t driver_count_local = 0;         // Can't use "driver_count" directly as a subscript because it's passed by reference
      for (j = 0; j < MAX_L6470; j++) {       // See how many drivers on this axis
        const char * const str = index_to_axis[j];
        if (axis_mon[0][0] == str[0]) {
          char * const mon = axis_mon[driver_count_local];
          mon[0] = str[0];
          mon[1] = str[1];
          mon[2] = str[2];   // append end of string
          axis_index[driver_count_local] = (L6470_axis_t)j;        // set axis index
          driver_count_local++;
        }
      }
      driver_count = driver_count_local;
    }
    break; // only take first axis found
  }

  //
  // Position calcs & checks
  //

  const float X_center = LOGICAL_X_POSITION(current_position[X_AXIS]),
              Y_center = LOGICAL_Y_POSITION(current_position[Y_AXIS]),
              Z_center = LOGICAL_Z_POSITION(current_position[Z_AXIS]),
              E_center = current_position[E_AXIS];

  switch (axis_mon[0][0]) {
    default: position_max = position_min = 0; break;

    case 'X': {
      position_min = X_center - displacement;
      position_max = X_center + displacement;
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
      position_min = Y_center - displacement;
      position_max = Y_center + displacement;
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
      position_min = Z_center - displacement;
      position_max = Z_center + displacement;
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
      position_min = E_center - displacement;
      position_max = E_center + displacement;
      echo_min_max('E', position_min, position_max);
    } break;
  }

  //
  // Work on the drivers
  //
  for (uint8_t k = 0; k < driver_count; k++) {
    bool not_found = true;
    for (j = 1; j <= L64XX::chain[0]; j++) {
      const char * const ind_axis = index_to_axis[L64XX::chain[j]];
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
    DEBUG_ECHOLNPGM("ERROR - not a L6470 axis");
    return true;
  }

  DEBUG_ECHOPGM("Monitoring:");
  for (j = 0; j < driver_count; j++) DEBUG_ECHOPAIR("  ", axis_mon[j]);
  DEBUG_EOL();

  // now have a list of driver(s) to monitor

  //
  // kVAL_HOLD checks & settings
  //

  kval_hold = parser.byteval('K');
  if (kval_hold) {
    DEBUG_ECHOLNPAIR("kval_hold = ", kval_hold);
    for (j = 0; j < driver_count; j++)
      set_param(axis_index[j], L6470_KVAL_HOLD, kval_hold);
  }
  else {
    // only print the KVAL_HOLD from one of the drivers
    kval_hold = get_param(axis_index[0], L6470_KVAL_HOLD);
    DEBUG_ECHOLNPAIR("KVAL_HOLD = ", kval_hold);
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

      DEBUG_ECHOLNPAIR("over_current_threshold specified: ", over_current_threshold);
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

/*
char* L64XX_Marlin::index_to_axis(const uint8_t index) {
  static PGM_P const _axis_string[MAX_L6470] = PROGMEM {
    PSTR("X "), PSTR("Y "), PSTR("Z "),
    PSTR("X2"), PSTR("Y2"), PSTR("Z2"), PSTR("Z3"),
    PSTR("E0"), PSTR("E1"), PSTR("E2"), PSTR("E3"), PSTR("E4"), PSTR("E5")
  };
  return (char*)pgm_read_ptr(&_axis_string[index]);
}
*/

void L64XX_Marlin::say_axis(const L6470_axis_t axis, const bool label/*=true*/) {
  if (label) SERIAL_ECHOPGM("AXIS:");
  const char * const str = index_to_axis[axis];
  SERIAL_CHAR(' ');
  SERIAL_CHAR(str[0]);
  SERIAL_CHAR(str[1]);
  SERIAL_CHAR(' ');
}

void L64XX_Marlin::error_status_decode(const uint16_t status, const L6470_axis_t axis,
      const uint16_t _status_axis_th_sd, const uint16_t _status_axis_th_wrn,
      const uint16_t _status_axis_step_loss_a, const uint16_t _status_axis_step_loss_b,
      const uint16_t _status_axis_ocd) {  // assumes status bits have been inverted
  #if ENABLED(L6470_CHITCHAT)
    char temp_buf[10];
    L64xx_MARLIN.say_axis(axis);
    sprintf_P(temp_buf, PSTR("  %4x   "), status);
    DEBUG_ECHO(temp_buf);
    print_bin(status);
    DEBUG_ECHOPGM("  THERMAL: ");
    serialprintPGM((status & _status_axis_th_sd) ? PSTR("SHUTDOWN") : (status & _status_axis_th_wrn) ? PSTR("WARNING ") : PSTR("OK      "));
    DEBUG_ECHOPGM("   OVERCURRENT: ");
    echo_yes_no(status & _status_axis_ocd);
    DEBUG_ECHOPGM("   STALL: ");
    echo_yes_no(status & (_status_axis_step_loss_a | _status_axis_step_loss_b));
    DEBUG_EOL();
  #else
    UNUSED(status); UNUSED(axis);
  #endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////
////  MONITOR_L6470_DRIVER_STATUS routines
////
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MONITOR_L6470_DRIVER_STATUS

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
    #if AXIS_IS_L64XX(X)
      {  0, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(Y)
      {  1, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(Z)
      {  2, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(X2)
      {  3, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(Y2)
      {  4, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(Z2)
      {  5, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(Z3)
      {  6, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(E0)
      {  7, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(E1)
      {  8, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(E2)
      {  9, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(E3)
      { 10, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(E4)
      { 11, 0, 0, 0, 0, 0, 0 },
    #endif
    #if AXIS_IS_L64XX(E5)
      { 12, 0, 0, 0, 0, 0, 0 }
    #endif
  };

  inline void append_stepper_err(char * &p, const uint8_t stepper_index, const char * const err=nullptr) {
    p += sprintf_P(p, PSTR("Stepper %c%c "), char(index_to_axis[stepper_index][0]), char(index_to_axis[stepper_index][1]));
    if (err) p += sprintf_P(p, err);
  }

  void L64XX_Marlin::monitor_update(uint8_t stepper_index, uint16_t status) {
    if (spi_abort) return;  // don't do anything if set_directions() has occurred
    uint8_t kval_hold;
    char temp_buf[120];
    char* p = &temp_buf[0];
    uint8_t j;
    for (j = 0; j < L64XX::chain[0]; j++) // find the table for this stepper
      if (driver_L6470_data[j].driver_index == stepper_index) break;

    driver_L6470_data[j].driver_status = status;
    uint16_t _status = ~status;     // all error bits are active low

    if (status == 0 || status == 0xFFFF) {              // com problem
      if (driver_L6470_data[j].com_counter == 0) {      // warn user when it first happens
        driver_L6470_data[j].com_counter++;
        append_stepper_err(p, stepper_index, PSTR(" - communications lost\n"));
        DEBUG_ECHO(temp_buf);
      }
      else {
        driver_L6470_data[j].com_counter++;
        if (driver_L6470_data[j].com_counter > 240) {  // remind of com problem about every 2 minutes
          driver_L6470_data[j].com_counter = 1;
          append_stepper_err(p, stepper_index, PSTR(" - still no communications\n"));
          DEBUG_ECHO(temp_buf);
        }
      }
    }
    else {
      if (driver_L6470_data[j].com_counter) {   // comms re-established
        driver_L6470_data[j].com_counter = 0;
        append_stepper_err(p, stepper_index, PSTR(" - communications re-established\n.. setting all drivers to default values\n"));
        DEBUG_ECHO(temp_buf);
        init_to_defaults();
      }
      else {
        // no com problems - do the usual checks
        if (_status & shadow.L6470_ERROR_MASK) {
          append_stepper_err(p, stepper_index);

          if (status & STATUS_HIZ) {                         // The driver has shut down. HiZ is active high
            driver_L6470_data[j].is_hi_Z = true;
            p += sprintf_P(p, PSTR("%cIS SHUT DOWN"), ' ');
            //if (_status & shadow.STATUS_AXIS_TH_SD) { // strange - TH_SD never seems to go active, must be implied by the HiZ and TH_WRN
            if (_status & shadow.STATUS_AXIS_TH_WRN) {                    // over current shutdown
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

            if (_status & shadow.STATUS_AXIS_TH_WRN) {     // have an over temperature warning
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

          #if ENABLED(L6470_STOP_ON_ERROR)
            if (_status & (shadow.STATUS_AXIS_UVLO | shadow.STATUS_AXIS_TH_WRN | shadow.STATUS_AXIS_TH_SD))
              kill(temp_buf);
          #endif

          #if ENABLED(L6470_CHITCHAT)
            if (_status & shadow.STATUS_AXIS_OCD)
              p += sprintf_P(p, PSTR("%c  over current"), ' ');

            if (_status & (shadow.STATUS_AXIS_STEP_LOSS_A | shadow.STATUS_AXIS_STEP_LOSS_B))
              p += sprintf_P(p, PSTR("%c  stall"), ' ');

            if (_status & shadow.STATUS_AXIS_UVLO)
              p += sprintf_P(p, PSTR("%c  under voltage lock out"), ' ');

            p += sprintf_P(p, PSTR("%c\n"), ' ');
          #endif

          DEBUG_ECHOLN(temp_buf);  // print the error message
        }
        else {
          driver_L6470_data[j].is_ot = false;
          driver_L6470_data[j].otw_counter = 0;   //clear out warning indicators
          driver_L6470_data[j].is_otw = false;
        } // end usual checks

      }// comms established but have errors
    } // comms re-established
  } // end monitor_update()

  void L64XX_Marlin::monitor_driver() {
    static millis_t next_cOT = 0;
    if (ELAPSED(millis(), next_cOT)) {
      next_cOT = millis() + 500;

      spi_active = true;    // let set_directions() know we're in the middle of a series of SPI transfers

      #define MONITOR_L6470_DRIVE(Q) monitor_update(Q, stepper##Q.getStatus())

      #if AXIS_IS_L64XX(X)
        MONITOR_L6470_DRIVE(X);
      #endif
      #if AXIS_IS_L64XX(Y)
        MONITOR_L6470_DRIVE(Y);
      #endif
      #if AXIS_IS_L64XX(Z)
        MONITOR_L6470_DRIVE(Z);
      #endif
      #if AXIS_IS_L64XX(X2)
        MONITOR_L6470_DRIVE(X2);
      #endif
      #if AXIS_IS_L64XX(Y2)
        MONITOR_L6470_DRIVE(Y2);
      #endif
      #if AXIS_IS_L64XX(Z2)
        MONITOR_L6470_DRIVE(Z2);
      #endif
      #if AXIS_IS_L64XX(Z3)
        MONITOR_L6470_DRIVE(Z3);
      #endif
      #if AXIS_IS_L64XX(E0)
        MONITOR_L6470_DRIVE(E0);
      #endif
      #if AXIS_IS_L64XX(E1)
        MONITOR_L6470_DRIVE(E1);
      #endif
      #if AXIS_IS_L64XX(E2)
        MONITOR_L6470_DRIVE(E2);
      #endif
      #if AXIS_IS_L64XX(E3)
        MONITOR_L6470_DRIVE(E3);
      #endif
      #if AXIS_IS_L64XX(E4)
        MONITOR_L6470_DRIVE(E4);
      #endif
      #if AXIS_IS_L64XX(E5)
        MONITOR_L6470_DRIVE(E5);
      #endif

      #if ENABLED(L6470_DEBUG)
        if (report_L6470_status) DEBUG_EOL();
      #endif

      spi_active = false;   // done with all SPI transfers - clear handshake flags
      spi_abort = false;
    }
  }

#endif // MONITOR_L6470_DRIVER_STATUS

#endif // HAS_L64XX
