#include "../../inc/MarlinConfig.h"

#if HAS_RPGFABI_MFU
#include "../../feature/mfu/mfu.h"
#include "../../gcode/gcode.h"
#include "../../libs/buzzer.h"
#include "../../module/temperature.h"
#include "../../MarlinCore.h"

MFU mfu;
#define MFU_BAUDRATE 115200


#define DEBUG_OUT ENABLED(MFU_DEBUG)
#include "../../core/debug_out.h"

inline void mfu_e_move(const float &dist, const feedRate_t fr_mm_s, const bool sync =true){
  current_position.e += dist /planner.e_factor[active_extruder];
  line_to_current_position(fr_mm_s);
  if(sync) planner.synchronize();
}

MFU::MFU(){
  rx_buffer[0] = '\0';
};

MFU::init(){
  MFU_SERIAL.begin(MFU_BAUDRATE);
  extruder = MFU_NO_TOOL;

  rx_buffer[0] = '\0';
  state = -1;   
  _enabled = false;
}

/*
  Handle tool Change
*/
MFU:: tool_change(const uint8_t index){
  if(_enabled) return;

  if(index != extruder){
    set_runout_valid(false);  // Disable Runout Sensor

    // Unload from Extruder
    mfu_e_move(-MFU_UNLOAD_GEARS_MM, MMM_TO_MMS(MFU_UNLOAD_FEEDRATE));

    // Handle Change
    stepper.disable_extruder();
    setCommand(MFU_CMD_FIRSTTOOL + index);
    // Wait for response
    manage_response(true, true);

    // Load Into Extruder
    extruder = index;
    active_extruder = 0;
    stepper.enable_extruder();

    mfu_e_move(MFU_UNLOAD_GEARS_MM, MMM_TO_MMS(MFU_UNLOAD_FEEDRATE));
    setCommand(MFU_CMD_LOADTOOL);
    // Wait for response
    manage_response(true, true);

    set_runout_valid(true); // Enable Runout Sensor
  }
}

/**
* Handle special T?/Tx/Tc commands
*
* T? Gcode to extrude shouldn't have to follow, load to extruder wheels is done automatically
* Tx Same as T?, except nozzle doesn't have to be preheated. Tc must be placed after extruder nozzle is preheated to finish filament load.
* Tc Load to nozzle after filament was prepared by Tx and extruder nozzle is already heated.
*/
void MFU::tool_change(const char *special) {
  if(!_enabled) return false;

}

/**
 * Wait for response from MFU
 */
bool MFU::get_response() {
  while (cmd != MFU_CMD_NOCMD) idle();

  while (!ready) {
    idle();
    if (state != 3) break;
  }

  const bool ret = ready;
  ready = false;

  return ret;
}

/**
 * Wait for response and deal with timeout if necessary
 */
void MFU::manage_response(const bool move_axes, const bool turn_off_nozzle) {

  constexpr xyz_pos_t park_point = NOZZLE_PARK_POINT;
  bool response = false, mfu_print_saved = false;
  xyz_pos_t resume_position;
  celsius_t resume_hotend_temp = thermalManager.degTargetHotend(active_extruder);

  KEEPALIVE_STATE(PAUSED_FOR_USER);

  while (!response) {

    response = get_response(); // wait for "ok" from mfu

    if (!response) {          // No "ok" was received in reserved time frame, user will fix the issue on mfu unit
      if (!mfu_print_saved) { // First occurrence. Save current position, park print head, disable nozzle heater.

        planner.synchronize();

        mfu_print_saved = true;

        SERIAL_ECHOLNPGM("MFU not responding");

        resume_hotend_temp = thermalManager.degTargetHotend(active_extruder);
        resume_position = current_position;

        if (move_axes && all_axes_homed()) nozzle.park(0, park_point);

        if (turn_off_nozzle) thermalManager.setTargetHotend(0, active_extruder);

        // Handle no MFU responding
      }
    }
    else if (mfu_print_saved) {
      SERIAL_ECHOLNPGM("\nMFU starts responding");

      if (turn_off_nozzle && resume_hotend_temp) {
        thermalManager.setTargetHotend(resume_hotend_temp, active_extruder);
        LCD_MESSAGE(MSG_HEATING);
        ERR_BUZZ();
        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(1000);
      }

      //LCD_MESSAGE(MSG_MMU2_RESUMING);
      //mmu2_attn_buzz(true);

      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

      if (move_axes && all_axes_homed()) {
        // Move XY to starting position, then Z
        do_blocking_move_to_xy(resume_position, feedRate_t(NOZZLE_PARK_XY_FEEDRATE));
        // Move Z_AXIS to saved position
        do_blocking_move_to_z(resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));
      }

      #pragma GCC diagnostic pop
    }
  }
}




MFU::loop(){
  switch(state){
    case 0: break;

    case -1: // NOT HOMED
      #if defined MFU_USE_FILAMENTSENSOR
        if(runout.filament_ran_out){
          // Filament not loaded => Home without Retract
          while (!thermalManager.wait_for_hotend(active_extruder,false)) safe_delay(100); // Wait for Headup
          MFU_SEND("H0");
        }
        else{
          // Filament in Sensor => Home with Retract
          MFU_SEND("H0");
        }
      #else
        // Home with retract
        MFU_SEND("H1");
      #endif 
      state = -2; // set to Homing
      break;
    
    case -2:  // Is Homing, wait for "ok"
      if(MFU_RECV("ok")){
        _enabled = true;
        state = 0;  // Homed
      }
      break;

    case 0: // Handle Commands
      if(cmd){
        if(WITHIN(cmd, MFU_CMD_FIRSTTOOL, MFU_CMD_FIRSTTOOL + MFU_TOOLCOUNT -1)){
          // Toolchange
          const int toolIndex = cmd - MFU_CMD_FIRSTTOOL;

          MFU_SEND(F("T%d"), toolIndex);
          state = 1;
        }
        else if(cmd == MFU_CMD_UNLOADTTOOL){
          // Unload current tool
          
          MFU_SEND("U");
          state = 2;
        }
        else if(cmd == MFU_CMD_LOADTOOL){
          // Load the Filament 
          MFU_SEND("L");
          state = 2;
        }
      }
      break;
    
    case 1: // Waiting for Toolchange
      if(MFU_RECV("ok")){
        // Preloaded 
        // Enable Extruder for Primingdistance

        set_runout_valid();
      }
      break;
    
    case 2: // Wait for 'ok'
      last_cmd = MFU_NOCMD;
      state = 0;
      break;
  }
}

bool MFU::unload(){
  if(!_enabled) return false;

  if(thermalManager.tooColdToExtrude(active_extruder)){
    LCD_ALERTMESSAGE(MSG_HOTEND_TOO_COLD);
    return false;
  }

  // Unload Filament
  setCommand(MFU_CMD_UNLOADTOOL);
  manage_response(true,true);

  // Unloaded
  extruder = MFU_NO_TOOL;
  set_runout_valid(false);
  return true;
}

void MFU::clear_rx_buffer() {
  while (MFU_SERIAL.available()) MFU_SERIAL.read();
  rx_buffer[0] = '\0';
}

void MFU::rx_str(){
  PGM_P pstr = FTOP(fstr);

  uint8_t i = strlen(rx_buffer);

  while (MFU_Serial.available()) {
    rx_buffer[i++] = MFU_SERIAL.read();

    if (i == sizeof(rx_buffer) - 1) {
      DEBUG_ECHOLNPGM("rx buffer overrun");
      break;
    }
  }
  rx_buffer[i] = '\0';

  uint8_t len = strlen_P(pstr);

  if (i < len) return false;

  pstr += len;

  while (len--) {
    char c0 = pgm_read_byte(pstr--), c1 = rx_buffer[i--];
    if (c0 == c1) continue;
    if (c0 == '\r' && c1 == '\n') continue;  // match cr as lf
    if (c0 == '\n' && c1 == '\r') continue;  // match lf as cr
    return false;
  }
  return true;
}

static void MFU::setCommand(const uint newCommand){
  cmd = newCommand;
  isReady = false;
}

/**
 * Transfer data to MFU, no argument
 */
void MFU::tx_str(FSTR_P fstr) {
  clear_rx_buffer();
  PGM_P pstr = FTOP(fstr);
  while (const char c = pgm_read_byte(pstr)) { MFU_SERIAL.write(c); pstr++; }
  prev_request = millis();
}

/**
 * Transfer data to MFU, one argument
 */
void MFU::tx_printf(FSTR_P format, int argument = -1) {
  clear_rx_buffer();
  const uint8_t len = sprintf_P(tx_buffer, FTOP(format), argument);
  for (uint8_t i = 0; i < len; ++i) MFU_SERIAL.write(tx_buffer[i]);
  prev_request = millis();
}

/**
 * Transfer data to MFU, two arguments
 */
void MFU::tx_printf(FSTR_P format, int argument1, int argument2) {
  clear_rx_buffer();
  const uint8_t len = sprintf_P(tx_buffer, FTOP(format), argument1, argument2);
  for (uint8_t i = 0; i < len; ++i) MFU_SERIAL.write(tx_buffer[i]);
  prev_request = millis();
}
#endif