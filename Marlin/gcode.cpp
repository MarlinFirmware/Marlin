/**
 * gcode.cpp
 */

#include "Marlin.h"
#include "stepper.h"
#include "temperature.h"
#include "ultralcd.h"
#include "cardreader.h"
#include "language.h"
#include "pins_arduino.h"
#include "configuration_store.h"

#include "gcode.h"

const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};

bool relative_mode = false;    //!< Absolute or Relative Coordinates
bool comment_mode = false;
bool target_direction;
uint8_t target_extruder;
bool no_wait_for_cooling = true;
bool cancel_heatup = false;

char serial_char;
int serial_count = 0;

char *strchr_pointer; //!< Pointer to find chars in the command string (X, Y, Z, E, etc.)
const char* queued_commands_P = NULL; //!< Pointer to the current line in the active sequence of commands, or NULL when none
int cmd_queue_index_r = 0, cmd_queue_index_w = 0, commands_in_queue = 0;
char command_queue[BUFSIZE][MAX_CMD_SIZE];

long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

const int sensitive_pins[] = SENSITIVE_PINS; //!< Sensitive pin list for M42 and M226

// static int cmd_queue_index_r = 0;
// static int cmd_queue_index_w = 0;
// static int commands_in_queue = 0;
// static char command_queue[BUFSIZE][MAX_CMD_SIZE];

#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

inline void set_destination_to_current() { memcpy(destination, current_position, sizeof(destination)); }

inline void sync_plan_position() {
  plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
#if defined(DELTA) || defined(SCARA)
  inline void sync_plan_position_delta() {
    calculate_delta(current_position);
    plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
  }
#endif

#ifdef SDSUPPORT
  bool fromsd[BUFSIZE];
#endif

void init_gcode() {
  #ifdef SDSUPPORT
    for (int8_t i = 0; i < BUFSIZE; i++) fromsd[i] = false;
  #endif
}

/**
 * Inject the next command from the command queue, when possible
 * Return false only if no command was pending
 */
bool drain_queued_commands_P() {
  if (!queued_commands_P) return false;

  // Get the next 30 chars from the sequence of gcodes to run
  char cmd[30];
  strncpy_P(cmd, queued_commands_P, sizeof(cmd) - 1);
  cmd[sizeof(cmd) - 1] = '\0';

  // Look for the end of line, or the end of sequence
  size_t i = 0;
  char c;
  while((c = cmd[i]) && c != '\n') i++; // find the end of this gcode command
  cmd[i] = '\0';
  if (enqueuecommand(cmd)) {      // buffer was not full (else we will retry later)
    if (c)
      queued_commands_P += i + 1; // move to next command
    else
      queued_commands_P = NULL;   // will have no more commands in the sequence
  }
  return true;
}

/**
 * Record one or many commands to run from program memory.
 * Aborts the current queue, if any.
 * Note: drain_queued_commands_P() must be called repeatedly to drain the commands afterwards
 */
void enqueuecommands_P(const char* pgcode) {
  queued_commands_P = pgcode;
  drain_queued_commands_P(); // first command executed asap (when possible)
}

/**
 * Copy a command directly into the main command buffer, from RAM.
 *
 * This is done in a non-safe way and needs a rework someday.
 * Returns false if it doesn't add any command
 */
bool enqueuecommand(const char *cmd) {

  if (*cmd == ';' || commands_in_queue >= BUFSIZE) return false;

  // This is dangerous if a mixing of serial and this happens
  char *command = command_queue[cmd_queue_index_w];
  strcpy(command, cmd);
  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_Enqueueing);
  SERIAL_ECHO(command);
  SERIAL_ECHOLNPGM("\"");
  cmd_queue_index_w = (cmd_queue_index_w + 1) % BUFSIZE;
  commands_in_queue++;
  return true;
}

void ClearToSend() {
  refresh_cmd_timeout();
  #ifdef SDSUPPORT
    if (fromsd[cmd_queue_index_r]) return;
  #endif
  SERIAL_PROTOCOLLNPGM(MSG_OK);
}

void FlushSerialRequestResend() {
  //char command_queue[cmd_queue_index_r][100]="Resend:";
  MYSERIAL.flush();
  SERIAL_PROTOCOLPGM(MSG_RESEND);
  SERIAL_PROTOCOLLN(gcode_LastN + 1);
  ClearToSend();
}

/**
 * Add to the circular command queue the next command from:
 *  - The command-injection queue (queued_commands_P)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void get_command() {

  if (drain_queued_commands_P()) return; // priority is given to non-serial commands
  
  while (MYSERIAL.available() > 0 && commands_in_queue < BUFSIZE) {

    serial_char = MYSERIAL.read();

    if (serial_char == '\n' || serial_char == '\r' ||
       serial_count >= (MAX_CMD_SIZE - 1)
    ) {
      // end of line == end of comment
      comment_mode = false;

      if (!serial_count) return; // shortcut for empty lines

      char *command = command_queue[cmd_queue_index_w];
      command[serial_count] = 0; // terminate string

      #ifdef SDSUPPORT
        fromsd[cmd_queue_index_w] = false;
      #endif

      if (strchr(command, 'N') != NULL) {
        strchr_pointer = strchr(command, 'N');
        gcode_N = (strtol(strchr_pointer + 1, NULL, 10));
        if (gcode_N != gcode_LastN + 1 && strstr_P(command, PSTR("M110")) == NULL) {
          SERIAL_ERROR_START;
          SERIAL_ERRORPGM(MSG_ERR_LINE_NO1);
          SERIAL_ERROR(gcode_LastN + 1);
          SERIAL_ERRORPGM(MSG_ERR_LINE_NO2);
          SERIAL_ERRORLN(gcode_N);
          FlushSerialRequestResend();
          serial_count = 0;
          return;
        }

        if (strchr(command, '*') != NULL) {
          byte checksum = 0;
          byte count = 0;
          while (command[count] != '*') checksum ^= command[count++];
          strchr_pointer = strchr(command, '*');

          if (strtol(strchr_pointer + 1, NULL, 10) != checksum) {
            SERIAL_ERROR_START;
            SERIAL_ERRORPGM(MSG_ERR_CHECKSUM_MISMATCH);
            SERIAL_ERRORLN(gcode_LastN);
            FlushSerialRequestResend();
            serial_count = 0;
            return;
          }
          //if no errors, continue parsing
        }
        else {
          SERIAL_ERROR_START;
          SERIAL_ERRORPGM(MSG_ERR_NO_CHECKSUM);
          SERIAL_ERRORLN(gcode_LastN);
          FlushSerialRequestResend();
          serial_count = 0;
          return;
        }

        gcode_LastN = gcode_N;
        //if no errors, continue parsing
      }
      else {  // if we don't receive 'N' but still see '*'
        if ((strchr(command, '*') != NULL)) {
          SERIAL_ERROR_START;
          SERIAL_ERRORPGM(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM);
          SERIAL_ERRORLN(gcode_LastN);
          serial_count = 0;
          return;
        }
      }

      if (strchr(command, 'G') != NULL) {
        strchr_pointer = strchr(command, 'G');
        switch (strtol(strchr_pointer + 1, NULL, 10)) {
          case 0:
          case 1:
          case 2:
          case 3:
            if (IsStopped()) {
              SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
              LCD_MESSAGEPGM(MSG_STOPPED);
            }
            break;
          default:
            break;
        }
      }

      // If command was e-stop process now
      if (strcmp(command, "M112") == 0) kill();

      cmd_queue_index_w = (cmd_queue_index_w + 1) % BUFSIZE;
      commands_in_queue += 1;

      serial_count = 0; //clear buffer
    }
    else if (serial_char == '\\') {  // Handle escapes
      if (MYSERIAL.available() > 0  && commands_in_queue < BUFSIZE) {
        // if we have one more character, copy it over
        serial_char = MYSERIAL.read();
        command_queue[cmd_queue_index_w][serial_count++] = serial_char;
      }
      // otherwise do nothing
    }
    else { // its not a newline, carriage return or escape char
      if (serial_char == ';') comment_mode = true;
      if (!comment_mode) command_queue[cmd_queue_index_w][serial_count++] = serial_char;
    }
  }

  #ifdef SDSUPPORT

    if (!card.sdprinting || serial_count) return;

    // '#' stops reading from SD to the buffer prematurely, so procedural macro calls are possible
    // if it occurs, stop_buffering is triggered and the buffer is ran dry.
    // this character _can_ occur in serial com, due to checksums. however, no checksums are used in SD printing

    static bool stop_buffering = false;
    if (commands_in_queue == 0) stop_buffering = false;

    while (!card.eof() && commands_in_queue < BUFSIZE && !stop_buffering) {
      int16_t n = card.get();
      serial_char = (char)n;
      if (serial_char == '\n' || serial_char == '\r' ||
          ((serial_char == '#' || serial_char == ':') && !comment_mode) ||
          serial_count >= (MAX_CMD_SIZE - 1) || n == -1
      ) {
        if (card.eof()) {
          SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
          print_job_stop_ms = millis();
          char time[30];
          millis_t t = (print_job_stop_ms - print_job_start_ms) / 1000;
          int hours = t / 60 / 60, minutes = (t / 60) % 60;
          sprintf_P(time, PSTR("%i " MSG_END_HOUR " %i " MSG_END_MINUTE), hours, minutes);
          SERIAL_ECHO_START;
          SERIAL_ECHOLN(time);
          lcd_setstatus(time, true);
          card.printingHasFinished();
          card.checkautostart(true);
        }
        if (serial_char == '#') stop_buffering = true;

        if (!serial_count) {
          comment_mode = false; //for new command
          return; //if empty line
        }
        command_queue[cmd_queue_index_w][serial_count] = 0; //terminate string
        // if (!comment_mode) {
        fromsd[cmd_queue_index_w] = true;
        commands_in_queue += 1;
        cmd_queue_index_w = (cmd_queue_index_w + 1) % BUFSIZE;
        // }
        comment_mode = false; //for new command
        serial_count = 0; //clear buffer
      }
      else {
        if (serial_char == ';') comment_mode = true;
        if (!comment_mode) command_queue[cmd_queue_index_w][serial_count++] = serial_char;
      }
    }

  #endif // SDSUPPORT
}

/**
 * Helpers for code values
 */
bool code_seen(char code) {
  strchr_pointer = strchr(command_queue[cmd_queue_index_r], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}

bool code_has_value() {
  int i = 1;
  char c = strchr_pointer[i];
  if (c == '-' || c == '+') c = strchr_pointer[++i];
  if (c == '.') c = strchr_pointer[++i];
  return (c >= '0' && c <= '9');
}

float code_value() {
  float ret;
  char *e = strchr(strchr_pointer, 'E');
  if (e) {
    *e = 0;
    ret = strtod(strchr_pointer+1, NULL);
    *e = 'E';
  }
  else
    ret = strtod(strchr_pointer+1, NULL);
  return ret;
}

long code_value_long() { return strtol(strchr_pointer + 1, NULL, 10); }

int16_t code_value_short() { return (int16_t)strtol(strchr_pointer + 1, NULL, 10); }

/**
 * Get coordinates from X Y Z E, and feedrate from F
 */
void get_coordinates() {
  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i]))
      destination[i] = code_value() + (axis_relative_modes[i] || relative_mode ? current_position[i] : 0);
    else
      destination[i] = current_position[i];
  }
  if (code_seen('F')) {
    float fr = code_value();
    if (fr > 0.0) feedrate = fr;
  }
}

/**
 * Get arc coordinates from I and J parameters
 */
void get_arc_coordinates() {
  #ifdef SF_ARC_FIX
    bool relative_mode_backup = relative_mode;
    relative_mode = true;
  #endif
    get_coordinates();
  #ifdef SF_ARC_FIX
    relative_mode = relative_mode_backup;
  #endif

  offset[0] = code_seen('I') ? code_value() : 0;
  offset[1] = code_seen('J') ? code_value() : 0;
}

/**
 * Set the targeted hotend for a command
 * Use the T parameter or the active extruder
 */
bool setTargetedHotend(int code){
  target_extruder = active_extruder;
  if (code_seen('T')) {
    target_extruder = code_value_short();
    if (target_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      switch(code){
        case 104:
          SERIAL_ECHO(MSG_M104_INVALID_EXTRUDER);
          break;
        case 105:
          SERIAL_ECHO(MSG_M105_INVALID_EXTRUDER);
          break;
        case 109:
          SERIAL_ECHO(MSG_M109_INVALID_EXTRUDER);
          break;
        case 218:
          SERIAL_ECHO(MSG_M218_INVALID_EXTRUDER);
          break;
        case 221:
          SERIAL_ECHO(MSG_M221_INVALID_EXTRUDER);
          break;
      }
      SERIAL_ECHOLN(target_extruder);
      return true;
    }
  }
  return false;
}

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
inline void gcode_G0_G1() {
  if (IsRunning()) {
    get_coordinates(); // For X Y Z E F

    #ifdef FWRETRACT

      if (autoretract_enabled && !(code_seen('X') || code_seen('Y') || code_seen('Z')) && code_seen('E')) {
        float echange = destination[E_AXIS] - current_position[E_AXIS];
        // Is this move an attempt to retract or recover?
        if ((echange < -MIN_RETRACT && !retracted[active_extruder]) || (echange > MIN_RETRACT && retracted[active_extruder])) {
          current_position[E_AXIS] = destination[E_AXIS]; // hide the slicer-generated retract/recover from calculations
          plan_set_e_position(current_position[E_AXIS]);  // AND from the planner
          retract(!retracted[active_extruder]);
          return;
        }
      }

    #endif //FWRETRACT

    prepare_move();
    //ClearToSend();
  }
}

/**
 * G2: Clockwise Arc
 * G3: Counterclockwise Arc
 */
inline void gcode_G2_G3(bool clockwise) {
  if (IsRunning()) {
    get_arc_coordinates();
    prepare_arc_move(clockwise);
  }
}

/**
 * G4: Dwell S<seconds> or P<milliseconds>
 */
inline void gcode_G4() {
  millis_t codenum = 0;

  if (code_seen('P')) codenum = code_value_long(); // milliseconds to wait
  if (code_seen('S')) codenum = code_value_long() * 1000; // seconds to wait

  st_synchronize();
  refresh_cmd_timeout();
  codenum += previous_cmd_ms;  // keep track of when we started waiting

  if (!lcd_hasstatus()) LCD_MESSAGEPGM(MSG_DWELL);

  while (millis() < codenum) {
    manage_heater();
    manage_inactivity();
    lcd_update();
  }
}

#ifdef FWRETRACT

  /**
   * G10 - Retract filament according to settings of M207
   * G11 - Recover filament according to settings of M208
   */
  inline void gcode_G10_G11(bool doRetract=false) {
    #if EXTRUDERS > 1
      if (doRetract) {
        retracted_swap[active_extruder] = (code_seen('S') && code_value_short() == 1); // checks for swap retract argument
      }
    #endif
    retract(doRetract
     #if EXTRUDERS > 1
      , retracted_swap[active_extruder]
     #endif
    );
  }

#endif //FWRETRACT

/**
 * G28: Home all axes according to settings
 *
 * Parameters
 *
 *  None  Home to all axes with no parameters.
 *        With QUICK_HOME enabled XY will home together, then Z.
 *
 * Cartesian parameters
 *
 *  X   Home to the X endstop
 *  Y   Home to the Y endstop
 *  Z   Home to the Z endstop
 *
 */
inline void gcode_G28() {

  // For auto bed leveling, clear the level matrix
  #ifdef ENABLE_AUTO_BED_LEVELING
    plan_bed_level_matrix.set_to_identity();
    #ifdef DELTA
      reset_bed_level();
    #endif
  #endif

  // For manual bed leveling deactivate the matrix temporarily
  #ifdef MESH_BED_LEVELING
    uint8_t mbl_was_active = mbl.active;
    mbl.active = 0;
  #endif

  float tmp_feedrate = feedrate;
  int tmp_feedrate_multiplier = feedrate_multiplier;

  feedrate_multiplier = 100;
  refresh_cmd_timeout();

  enable_endstops(true);

  set_destination_to_current();

  feedrate = 0.0;

  #ifdef DELTA
    // A delta can only safely home all axis at the same time
    // all axis have to home at the same time

    // Pretend the current position is 0,0,0
    for (int i = X_AXIS; i <= Z_AXIS; i++) current_position[i] = 0;
    sync_plan_position();

    // Move all carriages up together until the first endstop is hit.
    for (int i = X_AXIS; i <= Z_AXIS; i++) destination[i] = 3 * Z_MAX_LENGTH;
    feedrate = 1.732 * homing_feedrate[X_AXIS];
    line_to_destination();
    st_synchronize();
    endstops_hit_on_purpose(); // clear endstop hit flags

    // Destination reached
    for (int i = X_AXIS; i <= Z_AXIS; i++) current_position[i] = destination[i];

    // take care of back off and rehome now we are all at the top
    HOMEAXIS(X);
    HOMEAXIS(Y);
    HOMEAXIS(Z);

    sync_plan_position_delta();

  #else // NOT DELTA

    bool  homeX = code_seen(axis_codes[X_AXIS]),
          homeY = code_seen(axis_codes[Y_AXIS]),
          homeZ = code_seen(axis_codes[Z_AXIS]);

    bool home_all_axis = (!homeX && !homeY && !homeZ) || (homeX && homeY && homeZ);

    if (home_all_axis || homeZ) {

      #if Z_HOME_DIR > 0  // If homing away from BED do Z first

        HOMEAXIS(Z);

      #elif !defined(Z_SAFE_HOMING) && defined(Z_RAISE_BEFORE_HOMING) && Z_RAISE_BEFORE_HOMING > 0

        // Raise Z before homing any other axes
        // (Does this need to be "negative home direction?" Why not just use Z_RAISE_BEFORE_HOMING?)
        destination[Z_AXIS] = -Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS);
        feedrate = max_feedrate[Z_AXIS] * 60;
        line_to_destination();
        st_synchronize();

      #endif

    } // home_all_axis || homeZ

    #ifdef QUICK_HOME

      if (home_all_axis || (homeX && homeY)) {  // First diagonal move

        current_position[X_AXIS] = current_position[Y_AXIS] = 0;

        #ifdef DUAL_X_CARRIAGE
          int x_axis_home_dir = x_home_dir(active_extruder);
          extruder_duplication_enabled = false;
        #else
          int x_axis_home_dir = home_dir(X_AXIS);
        #endif

        sync_plan_position();

        float mlx = max_length(X_AXIS), mly = max_length(Y_AXIS),
              mlratio = mlx>mly ? mly/mlx : mlx/mly;

        destination[X_AXIS] = 1.5 * mlx * x_axis_home_dir;
        destination[Y_AXIS] = 1.5 * mly * home_dir(Y_AXIS);
        feedrate = min(homing_feedrate[X_AXIS], homing_feedrate[Y_AXIS]) * sqrt(mlratio * mlratio + 1);
        line_to_destination();
        st_synchronize();

        axis_is_at_home(X_AXIS);
        axis_is_at_home(Y_AXIS);
        sync_plan_position();

        destination[X_AXIS] = current_position[X_AXIS];
        destination[Y_AXIS] = current_position[Y_AXIS];
        line_to_destination();
        feedrate = 0.0;
        st_synchronize();
        endstops_hit_on_purpose(); // clear endstop hit flags

        current_position[X_AXIS] = destination[X_AXIS];
        current_position[Y_AXIS] = destination[Y_AXIS];
        #ifndef SCARA
          current_position[Z_AXIS] = destination[Z_AXIS];
        #endif
      }

    #endif // QUICK_HOME

    // Home X
    if (home_all_axis || homeX) {
      #ifdef DUAL_X_CARRIAGE
        int tmp_extruder = active_extruder;
        extruder_duplication_enabled = false;
        active_extruder = !active_extruder;
        HOMEAXIS(X);
        inactive_extruder_x_pos = current_position[X_AXIS];
        active_extruder = tmp_extruder;
        HOMEAXIS(X);
        // reset state used by the different modes
        memcpy(raised_parked_position, current_position, sizeof(raised_parked_position));
        delayed_move_time = 0;
        active_extruder_parked = true;
      #else
        HOMEAXIS(X);
      #endif
    }

    // Home Y
    if (home_all_axis || homeY) HOMEAXIS(Y);

    // Home Z last if homing towards the bed
    #if Z_HOME_DIR < 0

      if (home_all_axis || homeZ) {

        #ifdef Z_SAFE_HOMING

          if (home_all_axis) {

            current_position[Z_AXIS] = 0;
            sync_plan_position();

            //
            // Set the probe (or just the nozzle) destination to the safe homing point
            //
            // NOTE: If current_position[X_AXIS] or current_position[Y_AXIS] were set above
            // then this may not work as expected.
            destination[X_AXIS] = round(Z_SAFE_HOMING_X_POINT - X_PROBE_OFFSET_FROM_EXTRUDER);
            destination[Y_AXIS] = round(Z_SAFE_HOMING_Y_POINT - Y_PROBE_OFFSET_FROM_EXTRUDER);
            destination[Z_AXIS] = -Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS);    // Set destination away from bed
            feedrate = XY_TRAVEL_SPEED;
            // This could potentially move X, Y, Z all together
            line_to_destination();
            st_synchronize();

            // Set current X, Y is the Z_SAFE_HOMING_POINT minus PROBE_OFFSET_FROM_EXTRUDER
            current_position[X_AXIS] = destination[X_AXIS];
            current_position[Y_AXIS] = destination[Y_AXIS];

            // Home the Z axis
            HOMEAXIS(Z);
          }

          else if (homeZ) { // Don't need to Home Z twice

            // Let's see if X and Y are homed
            if (axis_known_position[X_AXIS] && axis_known_position[Y_AXIS]) {

              // Make sure the probe is within the physical limits
              // NOTE: This doesn't necessarily ensure the probe is also within the bed!
              float cpx = current_position[X_AXIS], cpy = current_position[Y_AXIS];
              if (   cpx >= X_MIN_POS - X_PROBE_OFFSET_FROM_EXTRUDER
                  && cpx <= X_MAX_POS - X_PROBE_OFFSET_FROM_EXTRUDER
                  && cpy >= Y_MIN_POS - Y_PROBE_OFFSET_FROM_EXTRUDER
                  && cpy <= Y_MAX_POS - Y_PROBE_OFFSET_FROM_EXTRUDER) {
                // Set the plan current position to X, Y, 0
                current_position[Z_AXIS] = 0;
                plan_set_position(cpx, cpy, 0, current_position[E_AXIS]); // = sync_plan_position

                // Set Z destination away from bed and raise the axis
                // NOTE: This should always just be Z_RAISE_BEFORE_HOMING unless...???
                destination[Z_AXIS] = -Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS);
                feedrate = max_feedrate[Z_AXIS] * 60;  // feedrate (mm/m) = max_feedrate (mm/s)
                line_to_destination();
                st_synchronize();

                // Home the Z axis
                HOMEAXIS(Z);
              }
              else {
                LCD_MESSAGEPGM(MSG_ZPROBE_OUT);
                SERIAL_ECHO_START;
                SERIAL_ECHOLNPGM(MSG_ZPROBE_OUT);
              }
            }
            else {
              LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
              SERIAL_ECHO_START;
              SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
            }

          } // !home_all_axes && homeZ

        #else // !Z_SAFE_HOMING

          HOMEAXIS(Z);

        #endif // !Z_SAFE_HOMING

      } // home_all_axis || homeZ

    #endif // Z_HOME_DIR < 0

    sync_plan_position();

  #endif // else DELTA

  #ifdef SCARA
    sync_plan_position_delta();
  #endif

  #ifdef ENDSTOPS_ONLY_FOR_HOMING
    enable_endstops(false);
  #endif

  // For manual leveling move back to 0,0
  #ifdef MESH_BED_LEVELING
    if (mbl_was_active) {
      current_position[X_AXIS] = mbl.get_x(0);
      current_position[Y_AXIS] = mbl.get_y(0);
      set_destination_to_current();
      feedrate = homing_feedrate[X_AXIS];
      line_to_destination();
      st_synchronize();
      current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
      sync_plan_position();
      mbl.active = 1;
    }
  #endif

  feedrate = tmp_feedrate;
  feedrate_multiplier = tmp_feedrate_multiplier;
  refresh_cmd_timeout();
  endstops_hit_on_purpose(); // clear endstop hit flags
}

#ifdef MESH_BED_LEVELING

  enum MeshLevelingState { MeshReport, MeshStart, MeshNext, MeshSet };

  /**
   * G29: Mesh-based Z-Probe, probes a grid and produces a
   *      mesh to compensate for variable bed height
   *
   * Parameters With MESH_BED_LEVELING:
   *
   *  S0              Produce a mesh report
   *  S1              Start probing mesh points
   *  S2              Probe the next mesh point
   *  S3 Xn Yn Zn.nn  Manually modify a single point
   *
   * The S0 report the points as below
   *
   *  +----> X-axis
   *  |
   *  |
   *  v Y-axis
   *  
   */
  inline void gcode_G29() {

    static int probe_point = -1;
    MeshLevelingState state = code_seen('S') || code_seen('s') ? (MeshLevelingState)code_value_short() : MeshReport;
    if (state < 0 || state > 3) {
      SERIAL_PROTOCOLLNPGM("S out of range (0-3).");
      return;
    }

    int ix, iy;
    float z;

    switch(state) {
      case MeshReport:
        if (mbl.active) {
          SERIAL_PROTOCOLPGM("Num X,Y: ");
          SERIAL_PROTOCOL(MESH_NUM_X_POINTS);
          SERIAL_PROTOCOLCHAR(',');
          SERIAL_PROTOCOL(MESH_NUM_Y_POINTS);
          SERIAL_PROTOCOLPGM("\nZ search height: ");
          SERIAL_PROTOCOL(MESH_HOME_SEARCH_Z);
          SERIAL_PROTOCOLLNPGM("\nMeasured points:");
          for (int y = 0; y < MESH_NUM_Y_POINTS; y++) {
            for (int x = 0; x < MESH_NUM_X_POINTS; x++) {
              SERIAL_PROTOCOLPGM("  ");
              SERIAL_PROTOCOL_F(mbl.z_values[y][x], 5);
            }
            SERIAL_EOL;
          }
        }
        else
          SERIAL_PROTOCOLLNPGM("Mesh bed leveling not active.");
        break;

      case MeshStart:
        mbl.reset();
        probe_point = 0;
        enqueuecommands_P(PSTR("G28\nG29 S2"));
        break;

      case MeshNext:
        if (probe_point < 0) {
          SERIAL_PROTOCOLLNPGM("Start mesh probing with \"G29 S1\" first.");
          return;
        }
        if (probe_point == 0) {
          // Set Z to a positive value before recording the first Z.
          current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
          sync_plan_position();
        }
        else {
          // For others, save the Z of the previous point, then raise Z again.
          ix = (probe_point - 1) % MESH_NUM_X_POINTS;
          iy = (probe_point - 1) / MESH_NUM_X_POINTS;
          if (iy & 1) ix = (MESH_NUM_X_POINTS - 1) - ix; // zig-zag
          mbl.set_z(ix, iy, current_position[Z_AXIS]);
          current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
          plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], homing_feedrate[X_AXIS]/60, active_extruder);
          st_synchronize();
        }
        // Is there another point to sample? Move there.
        if (probe_point < MESH_NUM_X_POINTS * MESH_NUM_Y_POINTS) {
          ix = probe_point % MESH_NUM_X_POINTS;
          iy = probe_point / MESH_NUM_X_POINTS;
          if (iy & 1) ix = (MESH_NUM_X_POINTS - 1) - ix; // zig-zag
          current_position[X_AXIS] = mbl.get_x(ix);
          current_position[Y_AXIS] = mbl.get_y(iy);
          plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], homing_feedrate[X_AXIS]/60, active_extruder);
          st_synchronize();
          probe_point++;
        }
        else {
          // After recording the last point, activate the mbl and home
          SERIAL_PROTOCOLLNPGM("Mesh probing done.");
          probe_point = -1;
          mbl.active = 1;
          enqueuecommands_P(PSTR("G28"));
        }
        break;

      case MeshSet:
        if (code_seen('X') || code_seen('x')) {
          ix = code_value_long()-1;
          if (ix < 0 || ix >= MESH_NUM_X_POINTS) {
            SERIAL_PROTOCOLPGM("X out of range (1-" STRINGIFY(MESH_NUM_X_POINTS) ").\n");
            return;
          }
        } else {
            SERIAL_PROTOCOLPGM("X not entered.\n");
            return;
        }
        if (code_seen('Y') || code_seen('y')) {
          iy = code_value_long()-1;
          if (iy < 0 || iy >= MESH_NUM_Y_POINTS) {
            SERIAL_PROTOCOLPGM("Y out of range (1-" STRINGIFY(MESH_NUM_Y_POINTS) ").\n");
            return;
          }
        } else {
            SERIAL_PROTOCOLPGM("Y not entered.\n");
            return;
        }
        if (code_seen('Z') || code_seen('z')) {
          z = code_value();
        } else {
          SERIAL_PROTOCOLPGM("Z not entered.\n");
          return;
        }
        mbl.z_values[iy][ix] = z;

    } // switch(state)
  }

#elif defined(ENABLE_AUTO_BED_LEVELING)

  /**
   * G29: Detailed Z-Probe, probes the bed at 3 or more points.
   *      Will fail if the printer has not been homed with G28.
   *
   * Enhanced G29 Auto Bed Leveling Probe Routine
   * 
   * Parameters With AUTO_BED_LEVELING_GRID:
   *
   *  P  Set the size of the grid that will be probed (P x P points).
   *     Not supported by non-linear delta printer bed leveling.
   *     Example: "G29 P4"
   *
   *  S  Set the XY travel speed between probe points (in mm/min)
   *
   *  D  Dry-Run mode. Just evaluate the bed Topology - Don't apply
   *     or clean the rotation Matrix. Useful to check the topology
   *     after a first run of G29.
   *
   *  V  Set the verbose level (0-4). Example: "G29 V3"
   *
   *  T  Generate a Bed Topology Report. Example: "G29 P5 T" for a detailed report.
   *     This is useful for manual bed leveling and finding flaws in the bed (to
   *     assist with part placement).
   *     Not supported by non-linear delta printer bed leveling.
   *
   *  F  Set the Front limit of the probing grid
   *  B  Set the Back limit of the probing grid
   *  L  Set the Left limit of the probing grid
   *  R  Set the Right limit of the probing grid
   *
   * Global Parameters:
   *
   * E/e By default G29 will engage the probe, test the bed, then disengage.
   *     Include "E" to engage/disengage the probe for each sample.
   *     There's no extra effect if you have a fixed probe.
   *     Usage: "G29 E" or "G29 e"
   *
   */
  inline void gcode_G29() {

    // Don't allow auto-leveling without homing first
    if (!axis_known_position[X_AXIS] || !axis_known_position[Y_AXIS]) {
      LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
      return;
    }

    int verbose_level = code_seen('V') || code_seen('v') ? code_value_short() : 1;
    if (verbose_level < 0 || verbose_level > 4) {
      SERIAL_ECHOLNPGM("?(V)erbose Level is implausible (0-4).");
      return;
    }

    bool dryrun = code_seen('D') || code_seen('d'),
         deploy_probe_for_each_reading = code_seen('E') || code_seen('e');

    #ifdef AUTO_BED_LEVELING_GRID

      #ifndef DELTA
        bool do_topography_map = verbose_level > 2 || code_seen('T') || code_seen('t');
      #endif

      if (verbose_level > 0) {
        SERIAL_PROTOCOLPGM("G29 Auto Bed Leveling\n");
        if (dryrun) SERIAL_ECHOLNPGM("Running in DRY-RUN mode");
      }

      int auto_bed_leveling_grid_points = AUTO_BED_LEVELING_GRID_POINTS;
      #ifndef DELTA
        if (code_seen('P')) auto_bed_leveling_grid_points = code_value_short();
        if (auto_bed_leveling_grid_points < 2) {
          SERIAL_PROTOCOLPGM("?Number of probed (P)oints is implausible (2 minimum).\n");
          return;
        }
      #endif

      xy_travel_speed = code_seen('S') ? code_value_short() : XY_TRAVEL_SPEED;

      int left_probe_bed_position = code_seen('L') ? code_value_short() : LEFT_PROBE_BED_POSITION,
          right_probe_bed_position = code_seen('R') ? code_value_short() : RIGHT_PROBE_BED_POSITION,
          front_probe_bed_position = code_seen('F') ? code_value_short() : FRONT_PROBE_BED_POSITION,
          back_probe_bed_position = code_seen('B') ? code_value_short() : BACK_PROBE_BED_POSITION;

      bool left_out_l = left_probe_bed_position < MIN_PROBE_X,
           left_out = left_out_l || left_probe_bed_position > right_probe_bed_position - MIN_PROBE_EDGE,
           right_out_r = right_probe_bed_position > MAX_PROBE_X,
           right_out = right_out_r || right_probe_bed_position < left_probe_bed_position + MIN_PROBE_EDGE,
           front_out_f = front_probe_bed_position < MIN_PROBE_Y,
           front_out = front_out_f || front_probe_bed_position > back_probe_bed_position - MIN_PROBE_EDGE,
           back_out_b = back_probe_bed_position > MAX_PROBE_Y,
           back_out = back_out_b || back_probe_bed_position < front_probe_bed_position + MIN_PROBE_EDGE;

      if (left_out || right_out || front_out || back_out) {
        if (left_out) {
          SERIAL_PROTOCOLPGM("?Probe (L)eft position out of range.\n");
          left_probe_bed_position = left_out_l ? MIN_PROBE_X : right_probe_bed_position - MIN_PROBE_EDGE;
        }
        if (right_out) {
          SERIAL_PROTOCOLPGM("?Probe (R)ight position out of range.\n");
          right_probe_bed_position = right_out_r ? MAX_PROBE_X : left_probe_bed_position + MIN_PROBE_EDGE;
        }
        if (front_out) {
          SERIAL_PROTOCOLPGM("?Probe (F)ront position out of range.\n");
          front_probe_bed_position = front_out_f ? MIN_PROBE_Y : back_probe_bed_position - MIN_PROBE_EDGE;
        }
        if (back_out) {
          SERIAL_PROTOCOLPGM("?Probe (B)ack position out of range.\n");
          back_probe_bed_position = back_out_b ? MAX_PROBE_Y : front_probe_bed_position + MIN_PROBE_EDGE;
        }
        return;
      }

    #endif // AUTO_BED_LEVELING_GRID

    #ifdef Z_PROBE_SLED
      dock_sled(false); // engage (un-dock) the probe
    #elif defined(Z_PROBE_ALLEN_KEY) //|| defined(SERVO_LEVELING)
      deploy_z_probe();
    #endif

    st_synchronize();

    if (!dryrun) {
      // make sure the bed_level_rotation_matrix is identity or the planner will get it wrong
      plan_bed_level_matrix.set_to_identity();

      #ifdef DELTA
        reset_bed_level();
      #else //!DELTA
        //vector_3 corrected_position = plan_get_position_mm();
        //corrected_position.debug("position before G29");
        vector_3 uncorrected_position = plan_get_position();
        //uncorrected_position.debug("position during G29");
        current_position[X_AXIS] = uncorrected_position.x;
        current_position[Y_AXIS] = uncorrected_position.y;
        current_position[Z_AXIS] = uncorrected_position.z;
        sync_plan_position();
      #endif // !DELTA
    }

    setup_for_endstop_move();

    feedrate = homing_feedrate[Z_AXIS];

    #ifdef AUTO_BED_LEVELING_GRID

      // probe at the points of a lattice grid
      const int xGridSpacing = (right_probe_bed_position - left_probe_bed_position) / (auto_bed_leveling_grid_points - 1),
                yGridSpacing = (back_probe_bed_position - front_probe_bed_position) / (auto_bed_leveling_grid_points - 1);

      #ifdef DELTA
        delta_grid_spacing[0] = xGridSpacing;
        delta_grid_spacing[1] = yGridSpacing;
        float z_offset = Z_PROBE_OFFSET_FROM_EXTRUDER;
        if (code_seen(axis_codes[Z_AXIS])) z_offset += code_value();
      #else // !DELTA
        // solve the plane equation ax + by + d = z
        // A is the matrix with rows [x y 1] for all the probed points
        // B is the vector of the Z positions
        // the normal vector to the plane is formed by the coefficients of the plane equation in the standard form, which is Vx*x+Vy*y+Vz*z+d = 0
        // so Vx = -a Vy = -b Vz = 1 (we want the vector facing towards positive Z

        int abl2 = auto_bed_leveling_grid_points * auto_bed_leveling_grid_points;

        double eqnAMatrix[abl2 * 3], // "A" matrix of the linear system of equations
               eqnBVector[abl2],     // "B" vector of Z points
               mean = 0.0;
      #endif // !DELTA

      int probePointCounter = 0;
      bool zig = true;

      for (int yCount = 0; yCount < auto_bed_leveling_grid_points; yCount++) {
        double yProbe = front_probe_bed_position + yGridSpacing * yCount;
        int xStart, xStop, xInc;

        if (zig) {
          xStart = 0;
          xStop = auto_bed_leveling_grid_points;
          xInc = 1;
        }
        else {
          xStart = auto_bed_leveling_grid_points - 1;
          xStop = -1;
          xInc = -1;
        }

        #ifndef DELTA
          // If do_topography_map is set then don't zig-zag. Just scan in one direction.
          // This gets the probe points in more readable order.
          if (!do_topography_map) zig = !zig;
        #endif

        for (int xCount = xStart; xCount != xStop; xCount += xInc) {
          double xProbe = left_probe_bed_position + xGridSpacing * xCount;

          // raise extruder
          float measured_z,
                z_before = probePointCounter ? Z_RAISE_BETWEEN_PROBINGS + current_position[Z_AXIS] : Z_RAISE_BEFORE_PROBING;

          #ifdef DELTA
            // Avoid probing the corners (outside the round or hexagon print surface) on a delta printer.
            float distance_from_center = sqrt(xProbe*xProbe + yProbe*yProbe);
            if (distance_from_center > DELTA_PROBABLE_RADIUS) continue;
          #endif //DELTA

          ProbeAction act;
          if (deploy_probe_for_each_reading) // G29 E - Stow between probes
            act = ProbeDeployAndStow;
          else if (yCount == 0 && xCount == xStart)
            act = ProbeDeploy;
          else if (yCount == auto_bed_leveling_grid_points - 1 && xCount == xStop - xInc)
            act = ProbeStow;
          else
            act = ProbeStay;

          measured_z = probe_pt(xProbe, yProbe, z_before, act, verbose_level);

          #ifndef DELTA
            mean += measured_z;

            eqnBVector[probePointCounter] = measured_z;
            eqnAMatrix[probePointCounter + 0 * abl2] = xProbe;
            eqnAMatrix[probePointCounter + 1 * abl2] = yProbe;
            eqnAMatrix[probePointCounter + 2 * abl2] = 1;
          #else
            bed_level[xCount][yCount] = measured_z + z_offset;
          #endif

          probePointCounter++;

          manage_heater();
          manage_inactivity();
          lcd_update();

        } //xProbe
      } //yProbe

      clean_up_after_endstop_move();

      #ifdef DELTA

        if (!dryrun) extrapolate_unprobed_bed_level();
        print_bed_level();

      #else // !DELTA

        // solve lsq problem
        double *plane_equation_coefficients = qr_solve(abl2, 3, eqnAMatrix, eqnBVector);

        mean /= abl2;

        if (verbose_level) {
          SERIAL_PROTOCOLPGM("Eqn coefficients: a: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[0], 8);
          SERIAL_PROTOCOLPGM(" b: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[1], 8);
          SERIAL_PROTOCOLPGM(" d: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[2], 8);
          SERIAL_EOL;
          if (verbose_level > 2) {
            SERIAL_PROTOCOLPGM("Mean of sampled points: ");
            SERIAL_PROTOCOL_F(mean, 8);
            SERIAL_EOL;
          }
        }

        // Show the Topography map if enabled
        if (do_topography_map) {

          SERIAL_PROTOCOLPGM(" \nBed Height Topography: \n");
          SERIAL_PROTOCOLPGM("+-----------+\n");
          SERIAL_PROTOCOLPGM("|...Back....|\n");
          SERIAL_PROTOCOLPGM("|Left..Right|\n");
          SERIAL_PROTOCOLPGM("|...Front...|\n");
          SERIAL_PROTOCOLPGM("+-----------+\n");

          for (int yy = auto_bed_leveling_grid_points - 1; yy >= 0; yy--) {
            for (int xx = 0; xx < auto_bed_leveling_grid_points; xx++) {
              int ind = yy * auto_bed_leveling_grid_points + xx;
              float diff = eqnBVector[ind] - mean;
              if (diff >= 0.0)
                SERIAL_PROTOCOLPGM(" +");   // Include + for column alignment
              else
                SERIAL_PROTOCOLCHAR(' ');
              SERIAL_PROTOCOL_F(diff, 5);
            } // xx
            SERIAL_EOL;
          } // yy
          SERIAL_EOL;

        } //do_topography_map


        if (!dryrun) set_bed_level_equation_lsq(plane_equation_coefficients);
        free(plane_equation_coefficients);

      #endif //!DELTA

    #else // !AUTO_BED_LEVELING_GRID

      // Actions for each probe
      ProbeAction p1, p2, p3;
      if (deploy_probe_for_each_reading)
        p1 = p2 = p3 = ProbeDeployAndStow;
      else
        p1 = ProbeDeploy, p2 = ProbeStay, p3 = ProbeStow;

      // Probe at 3 arbitrary points
      float z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING, p1, verbose_level),
            z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, p2, verbose_level),
            z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, p3, verbose_level);
      clean_up_after_endstop_move();
      if (!dryrun) set_bed_level_equation_3pts(z_at_pt_1, z_at_pt_2, z_at_pt_3);

    #endif // !AUTO_BED_LEVELING_GRID

    #ifndef DELTA
      if (verbose_level > 0)
        plan_bed_level_matrix.debug(" \n\nBed Level Correction Matrix:");

      if (!dryrun) {
        // Correct the Z height difference from z-probe position and hotend tip position.
        // The Z height on homing is measured by Z-Probe, but the probe is quite far from the hotend.
        // When the bed is uneven, this height must be corrected.
        float x_tmp = current_position[X_AXIS] + X_PROBE_OFFSET_FROM_EXTRUDER,
              y_tmp = current_position[Y_AXIS] + Y_PROBE_OFFSET_FROM_EXTRUDER,
              z_tmp = current_position[Z_AXIS],
              real_z = (float)st_get_position(Z_AXIS) / axis_steps_per_unit[Z_AXIS];  //get the real Z (since the auto bed leveling is already correcting the plane)

        apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);         //Apply the correction sending the probe offset
        current_position[Z_AXIS] = z_tmp - real_z + current_position[Z_AXIS];   //The difference is added to current position and sent to planner.
        sync_plan_position();
      }
    #endif // !DELTA

    #ifdef Z_PROBE_SLED
      dock_sled(true, -SLED_DOCKING_OFFSET); // dock the probe, correcting for over-travel
    #elif defined(Z_PROBE_ALLEN_KEY) //|| defined(SERVO_LEVELING)
      stow_z_probe();
    #endif

    #ifdef Z_PROBE_END_SCRIPT
      enqueuecommands_P(PSTR(Z_PROBE_END_SCRIPT));
      st_synchronize();
    #endif
  }

  #ifndef Z_PROBE_SLED

    inline void gcode_G30() {
      deploy_z_probe(); // Engage Z Servo endstop if available
      st_synchronize();
      // TODO: make sure the bed_level_rotation_matrix is identity or the planner will get set incorectly
      setup_for_endstop_move();

      feedrate = homing_feedrate[Z_AXIS];

      run_z_probe();
      SERIAL_PROTOCOLPGM("Bed");
      SERIAL_PROTOCOLPGM(" X: ");
      SERIAL_PROTOCOL(current_position[X_AXIS] + 0.0001);
      SERIAL_PROTOCOLPGM(" Y: ");
      SERIAL_PROTOCOL(current_position[Y_AXIS] + 0.0001);
      SERIAL_PROTOCOLPGM(" Z: ");
      SERIAL_PROTOCOL(current_position[Z_AXIS] + 0.0001);
      SERIAL_EOL;

      clean_up_after_endstop_move();
      stow_z_probe(); // Retract Z Servo endstop if available
    }

  #endif //!Z_PROBE_SLED

#endif //ENABLE_AUTO_BED_LEVELING

/**
 * G92: Set current position to given X Y Z E
 */
inline void gcode_G92() {
  if (!code_seen(axis_codes[E_AXIS]))
    st_synchronize();

  bool didXYZ = false;
  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      float v = current_position[i] = code_value();
      if (i == E_AXIS)
        plan_set_e_position(v);
      else
        didXYZ = true;
    }
  }
  if (didXYZ) sync_plan_position();
}

#ifdef ULTIPANEL

  /**
   * M0: // M0 - Unconditional stop - Wait for user button press on LCD
   * M1: // M1 - Conditional stop - Wait for user button press on LCD
   */
  inline void gcode_M0_M1() {
    char *src = strchr_pointer + 2;

    millis_t codenum = 0;
    bool hasP = false, hasS = false;
    if (code_seen('P')) {
      codenum = code_value_short(); // milliseconds to wait
      hasP = codenum > 0;
    }
    if (code_seen('S')) {
      codenum = code_value_short() * 1000UL; // seconds to wait
      hasS = codenum > 0;
    }
    char* starpos = strchr(src, '*');
    if (starpos != NULL) *(starpos) = '\0';
    while (*src == ' ') ++src;
    if (!hasP && !hasS && *src != '\0')
      lcd_setstatus(src, true);
    else {
      LCD_MESSAGEPGM(MSG_USERWAIT);
      #if defined(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
        dontExpireStatus();
      #endif
    }

    lcd_ignore_click();
    st_synchronize();
    refresh_cmd_timeout();
    if (codenum > 0) {
      codenum += previous_cmd_ms;  // keep track of when we started waiting
      while(millis() < codenum && !lcd_clicked()) {
        manage_heater();
        manage_inactivity();
        lcd_update();
      }
      lcd_ignore_click(false);
    }
    else {
      if (!lcd_detected()) return;
      while (!lcd_clicked()) {
        manage_heater();
        manage_inactivity();
        lcd_update();
      }
    }
    if (IS_SD_PRINTING)
      LCD_MESSAGEPGM(MSG_RESUMING);
    else
      LCD_MESSAGEPGM(WELCOME_MSG);
  }

#endif // ULTIPANEL

/**
 * M17: Enable power on all stepper motors
 */
inline void gcode_M17() {
  LCD_MESSAGEPGM(MSG_NO_MOVE);
  enable_all_steppers();
}

#ifdef SDSUPPORT

  /**
   * M20: List SD card to serial output
   */
  inline void gcode_M20() {
    SERIAL_PROTOCOLLNPGM(MSG_BEGIN_FILE_LIST);
    card.ls();
    SERIAL_PROTOCOLLNPGM(MSG_END_FILE_LIST);
  }

  /**
   * M21: Init SD Card
   */
  inline void gcode_M21() {
    card.initsd();
  }

  /**
   * M22: Release SD Card
   */
  inline void gcode_M22() {
    card.release();
  }

  /**
   * M23: Select a file
   */
  inline void gcode_M23() {
    char* codepos = strchr_pointer + 4;
    char* starpos = strchr(codepos, '*');
    if (starpos) *starpos = '\0';
    card.openFile(codepos, true);
  }

  /**
   * M24: Start SD Print
   */
  inline void gcode_M24() {
    card.startFileprint();
    print_job_start_ms = millis();
  }

  /**
   * M25: Pause SD Print
   */
  inline void gcode_M25() {
    card.pauseSDPrint();
  }

  /**
   * M26: Set SD Card file index
   */
  inline void gcode_M26() {
    if (card.cardOK && code_seen('S'))
      card.setIndex(code_value_short());
  }

  /**
   * M27: Get SD Card status
   */
  inline void gcode_M27() {
    card.getStatus();
  }

  /**
   * M28: Start SD Write
   */
  inline void gcode_M28() {
    char* codepos = strchr_pointer + 4;
    char* starpos = strchr(codepos, '*');
    if (starpos) {
      char* npos = strchr(command_queue[cmd_queue_index_r], 'N');
      strchr_pointer = strchr(npos, ' ') + 1;
      *(starpos) = '\0';
    }
    card.openFile(codepos, false);
  }

  /**
   * M29: Stop SD Write
   * Processed in write to file routine above
   */
  inline void gcode_M29() {
    // card.saving = false;
  }

  /**
   * M30 <filename>: Delete SD Card file
   */
  inline void gcode_M30() {
    if (card.cardOK) {
      card.closefile();
      char* starpos = strchr(strchr_pointer + 4, '*');
      if (starpos) {
        char* npos = strchr(command_queue[cmd_queue_index_r], 'N');
        strchr_pointer = strchr(npos, ' ') + 1;
        *(starpos) = '\0';
      }
      card.removeFile(strchr_pointer + 4);
    }
  }

#endif

/**
 * M31: Get the time since the start of SD Print (or last M109)
 */
inline void gcode_M31() {
  print_job_stop_ms = millis();
  millis_t t = (print_job_stop_ms - print_job_start_ms) / 1000;
  int min = t / 60, sec = t % 60;
  char time[30];
  sprintf_P(time, PSTR("%i min, %i sec"), min, sec);
  SERIAL_ECHO_START;
  SERIAL_ECHOLN(time);
  lcd_setstatus(time);
  autotempShutdown();
}

#ifdef SDSUPPORT

  /**
   * M32: Select file and start SD Print
   */
  inline void gcode_M32() {
    if (card.sdprinting)
      st_synchronize();

    char* codepos = strchr_pointer + 4;

    char* namestartpos = strchr(codepos, '!');   //find ! to indicate filename string start.
    if (! namestartpos)
      namestartpos = codepos; //default name position, 4 letters after the M
    else
      namestartpos++; //to skip the '!'

    char* starpos = strchr(codepos, '*');
    if (starpos) *(starpos) = '\0';

    bool call_procedure = code_seen('P') && (strchr_pointer < namestartpos);

    if (card.cardOK) {
      card.openFile(namestartpos, true, !call_procedure);

      if (code_seen('S') && strchr_pointer < namestartpos) // "S" (must occur _before_ the filename!)
        card.setIndex(code_value_short());

      card.startFileprint();
      if (!call_procedure)
        print_job_start_ms = millis(); //procedure calls count as normal print time.
    }
  }

  /**
   * M928: Start SD Write
   */
  inline void gcode_M928() {
    char* starpos = strchr(strchr_pointer + 5, '*');
    if (starpos) {
      char* npos = strchr(command_queue[cmd_queue_index_r], 'N');
      strchr_pointer = strchr(npos, ' ') + 1;
      *(starpos) = '\0';
    }
    card.openLogFile(strchr_pointer + 5);
  }

#endif // SDSUPPORT

/**
 * M42: Change pin status via GCode
 */
inline void gcode_M42() {
  if (code_seen('S')) {
    int pin_status = code_value_short(),
        pin_number = LED_PIN;

    if (code_seen('P') && pin_status >= 0 && pin_status <= 255)
      pin_number = code_value_short();

    for (int8_t i = 0; i < (int8_t)(sizeof(sensitive_pins) / sizeof(*sensitive_pins)); i++) {
      if (sensitive_pins[i] == pin_number) {
        pin_number = -1;
        break;
      }
    }

    #if HAS_FAN
      if (pin_number == FAN_PIN) fanSpeed = pin_status;
    #endif

    if (pin_number > -1) {
      pinMode(pin_number, OUTPUT);
      digitalWrite(pin_number, pin_status);
      analogWrite(pin_number, pin_status);
    }
  } // code_seen('S')
}

#if defined(ENABLE_AUTO_BED_LEVELING) && defined(Z_PROBE_REPEATABILITY_TEST)

  // This is redundant since the SanityCheck.h already checks for a valid Z_PROBE_PIN, but here for clarity.
  #ifdef Z_PROBE_ENDSTOP
    #if !HAS_Z_PROBE
      #error You must define Z_PROBE_PIN to enable Z-Probe repeatability calculation.
    #endif
  #elif !HAS_Z_MIN
    #error You must define Z_MIN_PIN to enable Z-Probe repeatability calculation.
  #endif

  /**
   * M48: Z-Probe repeatability measurement function.
   *
   * Usage:
   *   M48 <P#> <X#> <Y#> <V#> <E> <L#>
   *     P = Number of sampled points (4-50, default 10)
   *     X = Sample X position
   *     Y = Sample Y position
   *     V = Verbose level (0-4, default=1)
   *     E = Engage probe for each reading
   *     L = Number of legs of movement before probe
   *  
   * This function assumes the bed has been homed.  Specifically, that a G28 command
   * as been issued prior to invoking the M48 Z-Probe repeatability measurement function.
   * Any information generated by a prior G29 Bed leveling command will be lost and need to be
   * regenerated.
   */
  inline void gcode_M48() {

    double sum = 0.0, mean = 0.0, sigma = 0.0, sample_set[50];
    uint8_t verbose_level = 1, n_samples = 10, n_legs = 0;

    if (code_seen('V') || code_seen('v')) {
      verbose_level = code_value_short();
      if (verbose_level < 0 || verbose_level > 4 ) {
        SERIAL_PROTOCOLPGM("?Verbose Level not plausible (0-4).\n");
        return;
      }
    }

    if (verbose_level > 0)
      SERIAL_PROTOCOLPGM("M48 Z-Probe Repeatability test\n");

    if (code_seen('P') || code_seen('p')) {
      n_samples = code_value_short();
      if (n_samples < 4 || n_samples > 50) {
        SERIAL_PROTOCOLPGM("?Sample size not plausible (4-50).\n");
        return;
      }
    }

    double X_current = st_get_position_mm(X_AXIS),
           Y_current = st_get_position_mm(Y_AXIS),
           Z_current = st_get_position_mm(Z_AXIS),
           E_current = st_get_position_mm(E_AXIS),
           X_probe_location = X_current, Y_probe_location = Y_current,
           Z_start_location = Z_current + Z_RAISE_BEFORE_PROBING;

    bool deploy_probe_for_each_reading = code_seen('E') || code_seen('e');

    if (code_seen('X') || code_seen('x')) {
      X_probe_location = code_value() - X_PROBE_OFFSET_FROM_EXTRUDER;
      if (X_probe_location < X_MIN_POS || X_probe_location > X_MAX_POS) {
        SERIAL_PROTOCOLPGM("?X position out of range.\n");
        return;
      }
    }

    if (code_seen('Y') || code_seen('y')) {
      Y_probe_location = code_value() -  Y_PROBE_OFFSET_FROM_EXTRUDER;
      if (Y_probe_location < Y_MIN_POS || Y_probe_location > Y_MAX_POS) {
        SERIAL_PROTOCOLPGM("?Y position out of range.\n");
        return;
      }
    }

    if (code_seen('L') || code_seen('l')) {
      n_legs = code_value_short();
      if (n_legs == 1) n_legs = 2;
      if (n_legs < 0 || n_legs > 15) {
        SERIAL_PROTOCOLPGM("?Number of legs in movement not plausible (0-15).\n");
        return;
      }
    }

    //
    // Do all the preliminary setup work.   First raise the probe.
    //

    st_synchronize();
    plan_bed_level_matrix.set_to_identity();
    plan_buffer_line(X_current, Y_current, Z_start_location, E_current, homing_feedrate[Z_AXIS] / 60, active_extruder);
    st_synchronize();

    //
    // Now get everything to the specified probe point So we can safely do a probe to
    // get us close to the bed.  If the Z-Axis is far from the bed, we don't want to 
    // use that as a starting point for each probe.
    //
    if (verbose_level > 2)
      SERIAL_PROTOCOLPGM("Positioning the probe...\n");

    plan_buffer_line( X_probe_location, Y_probe_location, Z_start_location,
        E_current,
        homing_feedrate[X_AXIS]/60,
        active_extruder);
    st_synchronize();

    current_position[X_AXIS] = X_current = st_get_position_mm(X_AXIS);
    current_position[Y_AXIS] = Y_current = st_get_position_mm(Y_AXIS);
    current_position[Z_AXIS] = Z_current = st_get_position_mm(Z_AXIS);
    current_position[E_AXIS] = E_current = st_get_position_mm(E_AXIS);

    // 
    // OK, do the inital probe to get us close to the bed.
    // Then retrace the right amount and use that in subsequent probes
    //

    deploy_z_probe();

    setup_for_endstop_move();
    run_z_probe();

    current_position[Z_AXIS] = Z_current = st_get_position_mm(Z_AXIS);
    Z_start_location = st_get_position_mm(Z_AXIS) + Z_RAISE_BEFORE_PROBING;

    plan_buffer_line( X_probe_location, Y_probe_location, Z_start_location,
        E_current,
        homing_feedrate[X_AXIS]/60,
        active_extruder);
    st_synchronize();
    current_position[Z_AXIS] = Z_current = st_get_position_mm(Z_AXIS);

    if (deploy_probe_for_each_reading) stow_z_probe();

    for (uint8_t n=0; n < n_samples; n++) {
      // Make sure we are at the probe location
      do_blocking_move_to(X_probe_location, Y_probe_location, Z_start_location); // this also updates current_position

      if (n_legs) {
        millis_t ms = millis();
        double radius = ms % (X_MAX_LENGTH / 4),       // limit how far out to go
               theta = RADIANS(ms % 360L);
        float dir = (ms & 0x0001) ? 1 : -1;            // clockwise or counter clockwise

        //SERIAL_ECHOPAIR("starting radius: ",radius);
        //SERIAL_ECHOPAIR("   theta: ",theta);
        //SERIAL_ECHOPAIR("   direction: ",dir);
        //SERIAL_EOL;

        for (uint8_t l = 0; l < n_legs - 1; l++) {
          ms = millis();
          theta += RADIANS(dir * (ms % 20L));
          radius += (ms % 10L) - 5L;
          if (radius < 0.0) radius = -radius;

          X_current = X_probe_location + cos(theta) * radius;
          X_current = constrain(X_current, X_MIN_POS, X_MAX_POS);
          Y_current = Y_probe_location + sin(theta) * radius;
          Y_current = constrain(Y_current, Y_MIN_POS, Y_MAX_POS);

          if (verbose_level > 3) {
            SERIAL_ECHOPAIR("x: ", X_current);
            SERIAL_ECHOPAIR("y: ", Y_current);
            SERIAL_EOL;
          }

          do_blocking_move_to(X_current, Y_current, Z_current); // this also updates current_position

        } // n_legs loop

        // Go back to the probe location
        do_blocking_move_to(X_probe_location, Y_probe_location, Z_start_location); // this also updates current_position

      } // n_legs

      if (deploy_probe_for_each_reading)  {
        deploy_z_probe(); 
        delay(1000);
      }

      setup_for_endstop_move();
      run_z_probe();

      sample_set[n] = current_position[Z_AXIS];

      //
      // Get the current mean for the data points we have so far
      //
      sum = 0.0;
      for (uint8_t j = 0; j <= n; j++) sum += sample_set[j];
      mean = sum / (n + 1);

      //
      // Now, use that mean to calculate the standard deviation for the
      // data points we have so far
      //
      sum = 0.0;
      for (uint8_t j = 0; j <= n; j++) {
        float ss = sample_set[j] - mean;
        sum += ss * ss;
      }
      sigma = sqrt(sum / (n + 1));

      if (verbose_level > 1) {
        SERIAL_PROTOCOL(n+1);
        SERIAL_PROTOCOLPGM(" of ");
        SERIAL_PROTOCOL(n_samples);
        SERIAL_PROTOCOLPGM("   z: ");
        SERIAL_PROTOCOL_F(current_position[Z_AXIS], 6);
        if (verbose_level > 2) {
          SERIAL_PROTOCOLPGM(" mean: ");
          SERIAL_PROTOCOL_F(mean,6);
          SERIAL_PROTOCOLPGM("   sigma: ");
          SERIAL_PROTOCOL_F(sigma,6);
        }
      }

      if (verbose_level > 0) SERIAL_EOL;

      plan_buffer_line(X_probe_location, Y_probe_location, Z_start_location, current_position[E_AXIS], homing_feedrate[Z_AXIS]/60, active_extruder);
      st_synchronize();

      if (deploy_probe_for_each_reading) {
        stow_z_probe();
        delay(1000);
      }
    }

    if (!deploy_probe_for_each_reading) {
      stow_z_probe();
      delay(1000);
    }

    clean_up_after_endstop_move();

    // enable_endstops(true);

    if (verbose_level > 0) {
      SERIAL_PROTOCOLPGM("Mean: ");
      SERIAL_PROTOCOL_F(mean, 6);
      SERIAL_EOL;
    }

    SERIAL_PROTOCOLPGM("Standard Deviation: ");
    SERIAL_PROTOCOL_F(sigma, 6);
    SERIAL_EOL; SERIAL_EOL;
  }

#endif // ENABLE_AUTO_BED_LEVELING && Z_PROBE_REPEATABILITY_TEST

/**
 * M104: Set hot end temperature
 */
inline void gcode_M104() {
  if (setTargetedHotend(104)) return;

  if (code_seen('S')) {
    float temp = code_value();
    setTargetHotend(temp, target_extruder);
    #ifdef DUAL_X_CARRIAGE
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        setTargetHotend1(temp == 0.0 ? 0.0 : temp + duplicate_extruder_temp_offset);
    #endif
    setWatch();
  }
}

/**
 * M105: Read hot end and bed temperature
 */
inline void gcode_M105() {
  if (setTargetedHotend(105)) return;

  #if HAS_TEMP_0 || HAS_TEMP_BED || defined(HEATER_0_USES_MAX6675)
    SERIAL_PROTOCOLPGM("ok");
    #if HAS_TEMP_0
      SERIAL_PROTOCOLPGM(" T:");
      SERIAL_PROTOCOL_F(degHotend(target_extruder), 1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetHotend(target_extruder), 1);
    #endif
    #if HAS_TEMP_BED
      SERIAL_PROTOCOLPGM(" B:");
      SERIAL_PROTOCOL_F(degBed(), 1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetBed(), 1);
    #endif
    for (int8_t e = 0; e < EXTRUDERS; ++e) {
      SERIAL_PROTOCOLPGM(" T");
      SERIAL_PROTOCOL(e);
      SERIAL_PROTOCOLCHAR(':');
      SERIAL_PROTOCOL_F(degHotend(e), 1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetHotend(e), 1);
    }
  #else // !HAS_TEMP_0 && !HAS_TEMP_BED
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM(MSG_ERR_NO_THERMISTORS);
  #endif

  SERIAL_PROTOCOLPGM(" @:");
  #ifdef EXTRUDER_WATTS
    SERIAL_PROTOCOL((EXTRUDER_WATTS * getHeaterPower(target_extruder))/127);
    SERIAL_PROTOCOLCHAR('W');
  #else
    SERIAL_PROTOCOL(getHeaterPower(target_extruder));
  #endif

  SERIAL_PROTOCOLPGM(" B@:");
  #ifdef BED_WATTS
    SERIAL_PROTOCOL((BED_WATTS * getHeaterPower(-1))/127);
    SERIAL_PROTOCOLCHAR('W');
  #else
    SERIAL_PROTOCOL(getHeaterPower(-1));
  #endif

  #ifdef SHOW_TEMP_ADC_VALUES
    #if HAS_TEMP_BED
      SERIAL_PROTOCOLPGM("    ADC B:");
      SERIAL_PROTOCOL_F(degBed(),1);
      SERIAL_PROTOCOLPGM("C->");
      SERIAL_PROTOCOL_F(rawBedTemp()/OVERSAMPLENR,0);
    #endif
    for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder) {
      SERIAL_PROTOCOLPGM("  T");
      SERIAL_PROTOCOL(cur_extruder);
      SERIAL_PROTOCOLCHAR(':');
      SERIAL_PROTOCOL_F(degHotend(cur_extruder),1);
      SERIAL_PROTOCOLPGM("C->");
      SERIAL_PROTOCOL_F(rawHotendTemp(cur_extruder)/OVERSAMPLENR,0);
    }
  #endif

  SERIAL_EOL;
}

#if HAS_FAN

  /**
   * M106: Set Fan Speed
   */
  inline void gcode_M106() { fanSpeed = code_seen('S') ? constrain(code_value_short(), 0, 255) : 255; }

  /**
   * M107: Fan Off
   */
  inline void gcode_M107() { fanSpeed = 0; }

#endif // HAS_FAN

/**
 * M109: Wait for extruder(s) to reach temperature
 */
inline void gcode_M109() {
  if (setTargetedHotend(109)) return;

  LCD_MESSAGEPGM(MSG_HEATING);

  no_wait_for_cooling = code_seen('S');
  if (no_wait_for_cooling || code_seen('R')) {
    float temp = code_value();
    setTargetHotend(temp, target_extruder);
    #ifdef DUAL_X_CARRIAGE
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        setTargetHotend1(temp == 0.0 ? 0.0 : temp + duplicate_extruder_temp_offset);
    #endif
  }

  #ifdef AUTOTEMP
    autotemp_enabled = code_seen('F');
    if (autotemp_enabled) autotemp_factor = code_value();
    if (code_seen('S')) autotemp_min = code_value();
    if (code_seen('B')) autotemp_max = code_value();
  #endif

  setWatch();

  millis_t temp_ms = millis();

  /* See if we are heating up or cooling down */
  target_direction = isHeatingHotend(target_extruder); // true if heating, false if cooling

  cancel_heatup = false;

  #ifdef TEMP_RESIDENCY_TIME
    long residency_start_ms = -1;
    /* continue to loop until we have reached the target temp
      _and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
    while (!cancel_heatup && (residency_start_ms == -1 || (residency_start_ms >= 0 && millis() < residency_start_ms + TEMP_RESIDENCY_TIME * 1000UL)))
  #else
    while (target_direction ? isHeatingHotend(target_extruder) : isCoolingHotend(target_extruder) && !no_wait_for_cooling)
  #endif //TEMP_RESIDENCY_TIME

    { // while loop
      if (millis() > temp_ms + 1000UL) { //Print temp & remaining time every 1s while waiting
        SERIAL_PROTOCOLPGM("T:");
        SERIAL_PROTOCOL_F(degHotend(target_extruder),1);
        SERIAL_PROTOCOLPGM(" E:");
        SERIAL_PROTOCOL((int)target_extruder);
        #ifdef TEMP_RESIDENCY_TIME
          SERIAL_PROTOCOLPGM(" W:");
          if (residency_start_ms > -1) {
            temp_ms = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residency_start_ms)) / 1000UL;
            SERIAL_PROTOCOLLN(temp_ms);
          }
          else {
            SERIAL_PROTOCOLLNPGM("?");
          }
        #else
          SERIAL_EOL;
        #endif
        temp_ms = millis();
      }
      manage_heater();
      manage_inactivity();
      lcd_update();
      #ifdef TEMP_RESIDENCY_TIME
        // start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
        // or when current temp falls outside the hysteresis after target temp was reached
        if ((residency_start_ms == -1 &&  target_direction && (degHotend(target_extruder) >= (degTargetHotend(target_extruder)-TEMP_WINDOW))) ||
            (residency_start_ms == -1 && !target_direction && (degHotend(target_extruder) <= (degTargetHotend(target_extruder)+TEMP_WINDOW))) ||
            (residency_start_ms > -1 && labs(degHotend(target_extruder) - degTargetHotend(target_extruder)) > TEMP_HYSTERESIS) )
        {
          residency_start_ms = millis();
        }
      #endif //TEMP_RESIDENCY_TIME
    }

  LCD_MESSAGEPGM(MSG_HEATING_COMPLETE);
  refresh_cmd_timeout();
  print_job_start_ms = previous_cmd_ms;
}

#if HAS_TEMP_BED

  /**
   * M190: Sxxx Wait for bed current temp to reach target temp. Waits only when heating
   *       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
   */
  inline void gcode_M190() {
    LCD_MESSAGEPGM(MSG_BED_HEATING);
    no_wait_for_cooling = code_seen('S');
    if (no_wait_for_cooling || code_seen('R'))
      setTargetBed(code_value());

    millis_t temp_ms = millis();
    
    cancel_heatup = false;
    target_direction = isHeatingBed(); // true if heating, false if cooling

    while ((target_direction && !cancel_heatup) ? isHeatingBed() : isCoolingBed() && !no_wait_for_cooling) {
      millis_t ms = millis();
      if (ms > temp_ms + 1000UL) { //Print Temp Reading every 1 second while heating up.
        temp_ms = ms;
        float tt = degHotend(active_extruder);
        SERIAL_PROTOCOLPGM("T:");
        SERIAL_PROTOCOL(tt);
        SERIAL_PROTOCOLPGM(" E:");
        SERIAL_PROTOCOL((int)active_extruder);
        SERIAL_PROTOCOLPGM(" B:");
        SERIAL_PROTOCOL_F(degBed(), 1);
        SERIAL_EOL;
      }
      manage_heater();
      manage_inactivity();
      lcd_update();
    }
    LCD_MESSAGEPGM(MSG_BED_DONE);
    refresh_cmd_timeout();
  }

#endif // HAS_TEMP_BED

/**
 * M111: Set the debug level
 */
inline void gcode_M111() {
  marlin_debug_flags = code_seen('S') ? code_value_short() : DEBUG_INFO|DEBUG_ERRORS;
}

/**
 * M112: Emergency Stop
 */
inline void gcode_M112() { kill(); }

#ifdef BARICUDA

  #if HAS_HEATER_1
    /**
     * M126: Heater 1 valve open
     */
    inline void gcode_M126() { ValvePressure = code_seen('S') ? constrain(code_value(), 0, 255) : 255; }
    /**
     * M127: Heater 1 valve close
     */
    inline void gcode_M127() { ValvePressure = 0; }
  #endif

  #if HAS_HEATER_2
    /**
     * M128: Heater 2 valve open
     */
    inline void gcode_M128() { EtoPPressure = code_seen('S') ? constrain(code_value(), 0, 255) : 255; }
    /**
     * M129: Heater 2 valve close
     */
    inline void gcode_M129() { EtoPPressure = 0; }
  #endif

#endif //BARICUDA

/**
 * M140: Set bed temperature
 */
inline void gcode_M140() {
  if (code_seen('S')) setTargetBed(code_value());
}

#if HAS_POWER_SWITCH

  /**
   * M80: Turn on Power Supply
   */
  inline void gcode_M80() {
    OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE); //GND

    // If you have a switch on suicide pin, this is useful
    // if you want to start another print with suicide feature after
    // a print without suicide...
    #if HAS_SUICIDE
      OUT_WRITE(SUICIDE_PIN, HIGH);
    #endif

    #ifdef ULTIPANEL
      powersupply = true;
      LCD_MESSAGEPGM(WELCOME_MSG);
      lcd_update();
    #endif
  }

#endif // HAS_POWER_SWITCH

/**
 * M81: Turn off Power, including Power Supply, if there is one.
 *
 *      This code should ALWAYS be available for EMERGENCY SHUTDOWN!
 */
inline void gcode_M81() {
  disable_all_heaters();
  st_synchronize();
  disable_e0();
  disable_e1();
  disable_e2();
  disable_e3();
  finishAndDisableSteppers();
  fanSpeed = 0;
  delay(1000); // Wait 1 second before switching off
  #if HAS_SUICIDE
    st_synchronize();
    suicide();
  #elif HAS_POWER_SWITCH
    OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
  #endif
  #ifdef ULTIPANEL
    #if HAS_POWER_SWITCH
      powersupply = false;
    #endif
    LCD_MESSAGEPGM(MACHINE_NAME " " MSG_OFF ".");
    lcd_update();
  #endif
}


/**
 * M82: Set E codes absolute (default)
 */
inline void gcode_M82() { axis_relative_modes[E_AXIS] = false; }

/**
 * M82: Set E codes relative while in Absolute Coordinates (G90) mode
 */
inline void gcode_M83() { axis_relative_modes[E_AXIS] = true; }

/**
 * M18, M84: Disable all stepper motors
 */
inline void gcode_M18_M84() {
  if (code_seen('S')) {
    stepper_inactive_time = code_value() * 1000;
  }
  else {
    bool all_axis = !((code_seen(axis_codes[X_AXIS])) || (code_seen(axis_codes[Y_AXIS])) || (code_seen(axis_codes[Z_AXIS]))|| (code_seen(axis_codes[E_AXIS])));
    if (all_axis) {
      st_synchronize();
      disable_e0();
      disable_e1();
      disable_e2();
      disable_e3();
      finishAndDisableSteppers();
    }
    else {
      st_synchronize();
      if (code_seen('X')) disable_x();
      if (code_seen('Y')) disable_y();
      if (code_seen('Z')) disable_z();
      #if ((E0_ENABLE_PIN != X_ENABLE_PIN) && (E1_ENABLE_PIN != Y_ENABLE_PIN)) // Only enable on boards that have seperate ENABLE_PINS
        if (code_seen('E')) {
          disable_e0();
          disable_e1();
          disable_e2();
          disable_e3();
        }
      #endif
    }
  }
}

/**
 * M85: Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 */
inline void gcode_M85() {
  if (code_seen('S')) max_inactive_time = code_value() * 1000;
}

/**
 * M92: Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 */
inline void gcode_M92() {
  for(int8_t i=0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      if (i == E_AXIS) {
        float value = code_value();
        if (value < 20.0) {
          float factor = axis_steps_per_unit[i] / value; // increase e constants if M92 E14 is given for netfab.
          max_e_jerk *= factor;
          max_feedrate[i] *= factor;
          axis_steps_per_sqr_second[i] *= factor;
        }
        axis_steps_per_unit[i] = value;
      }
      else {
        axis_steps_per_unit[i] = code_value();
      }
    }
  }
}

/**
 * M114: Output current position to serial port
 */
inline void gcode_M114() {
  SERIAL_PROTOCOLPGM("X:");
  SERIAL_PROTOCOL(current_position[X_AXIS]);
  SERIAL_PROTOCOLPGM(" Y:");
  SERIAL_PROTOCOL(current_position[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" Z:");
  SERIAL_PROTOCOL(current_position[Z_AXIS]);
  SERIAL_PROTOCOLPGM(" E:");
  SERIAL_PROTOCOL(current_position[E_AXIS]);

  SERIAL_PROTOCOLPGM(MSG_COUNT_X);
  SERIAL_PROTOCOL(float(st_get_position(X_AXIS))/axis_steps_per_unit[X_AXIS]);
  SERIAL_PROTOCOLPGM(" Y:");
  SERIAL_PROTOCOL(float(st_get_position(Y_AXIS))/axis_steps_per_unit[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" Z:");
  SERIAL_PROTOCOL(float(st_get_position(Z_AXIS))/axis_steps_per_unit[Z_AXIS]);

  SERIAL_EOL;

  #ifdef SCARA
    SERIAL_PROTOCOLPGM("SCARA Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta:");
    SERIAL_PROTOCOL(delta[Y_AXIS]);
    SERIAL_EOL;
    
    SERIAL_PROTOCOLPGM("SCARA Cal - Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]+home_offset[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta (90):");
    SERIAL_PROTOCOL(delta[Y_AXIS]-delta[X_AXIS]-90+home_offset[Y_AXIS]);
    SERIAL_EOL;
    
    SERIAL_PROTOCOLPGM("SCARA step Cal - Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]/90*axis_steps_per_unit[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta:");
    SERIAL_PROTOCOL((delta[Y_AXIS]-delta[X_AXIS])/90*axis_steps_per_unit[Y_AXIS]);
    SERIAL_EOL; SERIAL_EOL;
  #endif
}

/**
 * M115: Capabilities string
 */
inline void gcode_M115() {
  SERIAL_PROTOCOLPGM(MSG_M115_REPORT);
}

/**
 * M117: Set LCD Status Message
 */
inline void gcode_M117() {
  char* codepos = strchr_pointer + 5;
  char* starpos = strchr(codepos, '*');
  if (starpos) *starpos = '\0';
  lcd_setstatus(codepos);
}

/**
 * M119: Output endstop states to serial output
 */
inline void gcode_M119() {
  SERIAL_PROTOCOLLN(MSG_M119_REPORT);
  #if HAS_X_MIN
    SERIAL_PROTOCOLPGM(MSG_X_MIN);
    SERIAL_PROTOCOLLN(((READ(X_MIN_PIN)^X_MIN_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_X_MAX
    SERIAL_PROTOCOLPGM(MSG_X_MAX);
    SERIAL_PROTOCOLLN(((READ(X_MAX_PIN)^X_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Y_MIN
    SERIAL_PROTOCOLPGM(MSG_Y_MIN);
    SERIAL_PROTOCOLLN(((READ(Y_MIN_PIN)^Y_MIN_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Y_MAX
    SERIAL_PROTOCOLPGM(MSG_Y_MAX);
    SERIAL_PROTOCOLLN(((READ(Y_MAX_PIN)^Y_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_MIN
    SERIAL_PROTOCOLPGM(MSG_Z_MIN);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PIN)^Z_MIN_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_MAX
    SERIAL_PROTOCOLPGM(MSG_Z_MAX);
    SERIAL_PROTOCOLLN(((READ(Z_MAX_PIN)^Z_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z2_MAX
    SERIAL_PROTOCOLPGM(MSG_Z2_MAX);
    SERIAL_PROTOCOLLN(((READ(Z2_MAX_PIN)^Z2_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_PROBE
    SERIAL_PROTOCOLPGM(MSG_Z_PROBE);
    SERIAL_PROTOCOLLN(((READ(Z_PROBE_PIN)^Z_PROBE_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
}

/**
 * M120: Enable endstops
 */
inline void gcode_M120() { enable_endstops(false); }

/**
 * M121: Disable endstops
 */
inline void gcode_M121() { enable_endstops(true); }

#ifdef BLINKM

  /**
   * M150: Set Status LED Color - Use R-U-B for R-G-B
   */
  inline void gcode_M150() {
    SendColors(
      code_seen('R') ? (byte)code_value_short() : 0,
      code_seen('U') ? (byte)code_value_short() : 0,
      code_seen('B') ? (byte)code_value_short() : 0
    );
  }

#endif // BLINKM

/**
 * M200: Set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
 *       T<extruder>
 *       D<millimeters>
 */
inline void gcode_M200() {
  int tmp_extruder = active_extruder;
  if (code_seen('T')) {
    tmp_extruder = code_value_short();
    if (tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      SERIAL_ECHO(MSG_M200_INVALID_EXTRUDER);
      return;
    }
  }

  if (code_seen('D')) {
    float diameter = code_value();
    // setting any extruder filament size disables volumetric on the assumption that
    // slicers either generate in extruder values as cubic mm or as as filament feeds
    // for all extruders
    volumetric_enabled = (diameter != 0.0);
    if (volumetric_enabled) {
      filament_size[tmp_extruder] = diameter;
      // make sure all extruders have some sane value for the filament size
      for (int i=0; i<EXTRUDERS; i++)
        if (! filament_size[i]) filament_size[i] = DEFAULT_NOMINAL_FILAMENT_DIA;
    }
  }
  else {
    //reserved for setting filament diameter via UFID or filament measuring device
    return;
  }
  calculate_volumetric_multipliers();
}

/**
 * M201: Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 */
inline void gcode_M201() {
  for (int8_t i=0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      max_acceleration_units_per_sq_second[i] = code_value();
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
  reset_acceleration_rates();
}

#if 0 // Not used for Sprinter/grbl gen6
  inline void gcode_M202() {
    for(int8_t i=0; i < NUM_AXIS; i++) {
      if(code_seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
    }
  }
#endif


/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
 */
inline void gcode_M203() {
  for (int8_t i=0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      max_feedrate[i] = code_value();
    }
  }
}

/**
 * M204: Set Accelerations in mm/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 *
 *  Also sets minimum segment time in ms (B20000) to prevent buffer under-runs and M20 minimum feedrate
 */
inline void gcode_M204() {
  if (code_seen('S')) {  // Kept for legacy compatibility. Should NOT BE USED for new developments.
    acceleration = code_value();
    travel_acceleration = acceleration;
    SERIAL_ECHOPAIR("Setting Print and Travel Acceleration: ", acceleration );
    SERIAL_EOL;
  }
  if (code_seen('P')) {
    acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Print Acceleration: ", acceleration );
    SERIAL_EOL;
  }
  if (code_seen('R')) {
    retract_acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Retract Acceleration: ", retract_acceleration );
    SERIAL_EOL;
  }
  if (code_seen('T')) {
    travel_acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Travel Acceleration: ", travel_acceleration );
    SERIAL_EOL;
  }
  
}

/**
 * M205: Set Advanced Settings
 *
 *    S = Min Feed Rate (mm/s)
 *    T = Min Travel Feed Rate (mm/s)
 *    B = Min Segment Time (µs)
 *    X = Max XY Jerk (mm/s/s)
 *    Z = Max Z Jerk (mm/s/s)
 *    E = Max E Jerk (mm/s/s)
 */
inline void gcode_M205() {
  if (code_seen('S')) minimumfeedrate = code_value();
  if (code_seen('T')) mintravelfeedrate = code_value();
  if (code_seen('B')) minsegmenttime = code_value();
  if (code_seen('X')) max_xy_jerk = code_value();
  if (code_seen('Z')) max_z_jerk = code_value();
  if (code_seen('E')) max_e_jerk = code_value();
}

/**
 * M206: Set Additional Homing Offset (X Y Z). SCARA aliases T=X, P=Y
 */
inline void gcode_M206() {
  for (int8_t i=X_AXIS; i <= Z_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      home_offset[i] = code_value();
    }
  }
  #ifdef SCARA
    if (code_seen('T')) home_offset[X_AXIS] = code_value(); // Theta
    if (code_seen('P')) home_offset[Y_AXIS] = code_value(); // Psi
  #endif
}

#ifdef DELTA
  /**
   * M665: Set delta configurations
   *
   *    L = diagonal rod
   *    R = delta radius
   *    S = segments per second
   */
  inline void gcode_M665() {
    if (code_seen('L')) delta_diagonal_rod = code_value();
    if (code_seen('R')) delta_radius = code_value();
    if (code_seen('S')) delta_segments_per_second = code_value();
    recalc_delta_settings(delta_radius, delta_diagonal_rod);
  }
  /**
   * M666: Set delta endstop adjustment
   */
  inline void gcode_M666() {
    for (int8_t i = X_AXIS; i <= Z_AXIS; i++) {
      if (code_seen(axis_codes[i])) {
        endstop_adj[i] = code_value();
      }
    }
  }
#elif defined(Z_DUAL_ENDSTOPS) // !DELTA && defined(Z_DUAL_ENDSTOPS)
  /**
   * M666: For Z Dual Endstop setup, set z axis offset to the z2 axis.
   */
  inline void gcode_M666() {
    if (code_seen('Z')) z_endstop_adj = code_value();
    SERIAL_ECHOPAIR("Z Endstop Adjustment set to (mm):", z_endstop_adj);
    SERIAL_EOL;
  }
  
#endif // !DELTA && defined(Z_DUAL_ENDSTOPS)

#ifdef FWRETRACT

  /**
   * M207: Set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop]
   */
  inline void gcode_M207() {
    if (code_seen('S')) retract_length = code_value();
    if (code_seen('F')) retract_feedrate = code_value() / 60;
    if (code_seen('Z')) retract_zlift = code_value();
  }

  /**
   * M208: Set retract recover length S[positive mm surplus to the M207 S*] F[feedrate mm/min]
   */
  inline void gcode_M208() {
    if (code_seen('S')) retract_recover_length = code_value();
    if (code_seen('F')) retract_recover_feedrate = code_value() / 60;
  }

  /**
   * M209: Enable automatic retract (M209 S1)
   *       detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
   */
  inline void gcode_M209() {
    if (code_seen('S')) {
      int t = code_value_short();
      switch(t) {
        case 0:
          autoretract_enabled = false;
          break;
        case 1:
          autoretract_enabled = true;
          break;
        default:
          SERIAL_ECHO_START;
          SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
          SERIAL_ECHO(command_queue[cmd_queue_index_r]);
          SERIAL_ECHOLNPGM("\"");
          return;
      }
      for (int i=0; i<EXTRUDERS; i++) retracted[i] = false;
    }
  }

#endif // FWRETRACT

#if EXTRUDERS > 1

  /**
   * M218 - set hotend offset (in mm), T<extruder_number> X<offset_on_X> Y<offset_on_Y>
   */
  inline void gcode_M218() {
    if (setTargetedHotend(218)) return;

    if (code_seen('X')) extruder_offset[X_AXIS][target_extruder] = code_value();
    if (code_seen('Y')) extruder_offset[Y_AXIS][target_extruder] = code_value();

    #ifdef DUAL_X_CARRIAGE
      if (code_seen('Z')) extruder_offset[Z_AXIS][target_extruder] = code_value();
    #endif

    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
    for (int e = 0; e < EXTRUDERS; e++) {
      SERIAL_CHAR(' ');
      SERIAL_ECHO(extruder_offset[X_AXIS][e]);
      SERIAL_CHAR(',');
      SERIAL_ECHO(extruder_offset[Y_AXIS][e]);
      #ifdef DUAL_X_CARRIAGE
        SERIAL_CHAR(',');
        SERIAL_ECHO(extruder_offset[Z_AXIS][e]);
      #endif
    }
    SERIAL_EOL;
  }

#endif // EXTRUDERS > 1

/**
 * M220: Set speed percentage factor, aka "Feed Rate" (M220 S95)
 */
inline void gcode_M220() {
  if (code_seen('S')) feedrate_multiplier = code_value();
}

/**
 * M221: Set extrusion percentage (M221 T0 S95)
 */
inline void gcode_M221() {
  if (code_seen('S')) {
    int sval = code_value();
    if (code_seen('T')) {
      if (setTargetedHotend(221)) return;
      extruder_multiply[target_extruder] = sval;
    }
    else {
      extruder_multiply[active_extruder] = sval;
    }
  }
}

/**
 * M226: Wait until the specified pin reaches the state required (M226 P<pin> S<state>)
 */
inline void gcode_M226() {
  if (code_seen('P')) {
    int pin_number = code_value();

    int pin_state = code_seen('S') ? code_value() : -1; // required pin state - default is inverted

    if (pin_state >= -1 && pin_state <= 1) {

      for (int8_t i = 0; i < (int8_t)(sizeof(sensitive_pins)/sizeof(*sensitive_pins)); i++) {
        if (sensitive_pins[i] == pin_number) {
          pin_number = -1;
          break;
        }
      }

      if (pin_number > -1) {
        int target = LOW;

        st_synchronize();

        pinMode(pin_number, INPUT);

        switch(pin_state){
          case 1:
            target = HIGH;
            break;

          case 0:
            target = LOW;
            break;

          case -1:
            target = !digitalRead(pin_number);
            break;
        }

        while(digitalRead(pin_number) != target) {
          manage_heater();
          manage_inactivity();
          lcd_update();
        }

      } // pin_number > -1
    } // pin_state -1 0 1
  } // code_seen('P')
}

#if NUM_SERVOS > 0

  /**
   * M280: Set servo position absolute. P: servo index, S: angle or microseconds
   */
  inline void gcode_M280() {
    int servo_index = code_seen('P') ? code_value() : -1;
    int servo_position = 0;
    if (code_seen('S')) {
      servo_position = code_value();
      if ((servo_index >= 0) && (servo_index < NUM_SERVOS)) {
        #if SERVO_LEVELING
          servo[servo_index].attach(0);
        #endif
        servo[servo_index].write(servo_position);
        #if SERVO_LEVELING
          delay(PROBE_SERVO_DEACTIVATION_DELAY);
          servo[servo_index].detach();
        #endif
      }
      else {
        SERIAL_ECHO_START;
        SERIAL_ECHO("Servo ");
        SERIAL_ECHO(servo_index);
        SERIAL_ECHOLN(" out of range");
      }
    }
    else if (servo_index >= 0) {
      SERIAL_PROTOCOL(MSG_OK);
      SERIAL_PROTOCOL(" Servo ");
      SERIAL_PROTOCOL(servo_index);
      SERIAL_PROTOCOL(": ");
      SERIAL_PROTOCOL(servo[servo_index].read());
      SERIAL_EOL;
    }
  }

#endif // NUM_SERVOS > 0

#if BEEPER > 0 || defined(ULTRALCD) || defined(LCD_USE_I2C_BUZZER)

  /**
   * M300: Play beep sound S<frequency Hz> P<duration ms>
   */
  inline void gcode_M300() {
    uint16_t beepS = code_seen('S') ? code_value_short() : 110;
    uint32_t beepP = code_seen('P') ? code_value_long() : 1000;
    if (beepP > 5000) beepP = 5000; // limit to 5 seconds
    lcd_buzz(beepP, beepS);
  }

#endif // BEEPER>0 || ULTRALCD || LCD_USE_I2C_BUZZER

#ifdef PIDTEMP

  /**
   * M301: Set PID parameters P I D (and optionally C)
   */
  inline void gcode_M301() {

    // multi-extruder PID patch: M301 updates or prints a single extruder's PID values
    // default behaviour (omitting E parameter) is to update for extruder 0 only
    int e = code_seen('E') ? code_value() : 0; // extruder being updated

    if (e < EXTRUDERS) { // catch bad input value
      if (code_seen('P')) PID_PARAM(Kp, e) = code_value();
      if (code_seen('I')) PID_PARAM(Ki, e) = scalePID_i(code_value());
      if (code_seen('D')) PID_PARAM(Kd, e) = scalePID_d(code_value());
      #ifdef PID_ADD_EXTRUSION_RATE
        if (code_seen('C')) PID_PARAM(Kc, e) = code_value();
      #endif      

      updatePID();
      SERIAL_PROTOCOL(MSG_OK);
      #ifdef PID_PARAMS_PER_EXTRUDER
        SERIAL_PROTOCOL(" e:"); // specify extruder in serial output
        SERIAL_PROTOCOL(e);
      #endif // PID_PARAMS_PER_EXTRUDER
      SERIAL_PROTOCOL(" p:");
      SERIAL_PROTOCOL(PID_PARAM(Kp, e));
      SERIAL_PROTOCOL(" i:");
      SERIAL_PROTOCOL(unscalePID_i(PID_PARAM(Ki, e)));
      SERIAL_PROTOCOL(" d:");
      SERIAL_PROTOCOL(unscalePID_d(PID_PARAM(Kd, e)));
      #ifdef PID_ADD_EXTRUSION_RATE
        SERIAL_PROTOCOL(" c:");
        //Kc does not have scaling applied above, or in resetting defaults
        SERIAL_PROTOCOL(PID_PARAM(Kc, e));
      #endif
      SERIAL_EOL;    
    }
    else {
      SERIAL_ECHO_START;
      SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
    }
  }

#endif // PIDTEMP

#ifdef PIDTEMPBED

  inline void gcode_M304() {
    if (code_seen('P')) bedKp = code_value();
    if (code_seen('I')) bedKi = scalePID_i(code_value());
    if (code_seen('D')) bedKd = scalePID_d(code_value());

    updatePID();
    SERIAL_PROTOCOL(MSG_OK);
    SERIAL_PROTOCOL(" p:");
    SERIAL_PROTOCOL(bedKp);
    SERIAL_PROTOCOL(" i:");
    SERIAL_PROTOCOL(unscalePID_i(bedKi));
    SERIAL_PROTOCOL(" d:");
    SERIAL_PROTOCOL(unscalePID_d(bedKd));
    SERIAL_EOL;
  }

#endif // PIDTEMPBED

#if defined(CHDK) || HAS_PHOTOGRAPH

  /**
   * M240: Trigger a camera by emulating a Canon RC-1
   *       See http://www.doc-diy.net/photo/rc-1_hacked/
   */
  inline void gcode_M240() {
    #ifdef CHDK
     
       OUT_WRITE(CHDK, HIGH);
       chdkHigh = millis();
       chdkActive = true;
     
    #elif HAS_PHOTOGRAPH

      const uint8_t NUM_PULSES = 16;
      const float PULSE_LENGTH = 0.01524;
      for (int i = 0; i < NUM_PULSES; i++) {
        WRITE(PHOTOGRAPH_PIN, HIGH);
        _delay_ms(PULSE_LENGTH);
        WRITE(PHOTOGRAPH_PIN, LOW);
        _delay_ms(PULSE_LENGTH);
      }
      delay(7.33);
      for (int i = 0; i < NUM_PULSES; i++) {
        WRITE(PHOTOGRAPH_PIN, HIGH);
        _delay_ms(PULSE_LENGTH);
        WRITE(PHOTOGRAPH_PIN, LOW);
        _delay_ms(PULSE_LENGTH);
      }

    #endif // !CHDK && HAS_PHOTOGRAPH
  }

#endif // CHDK || PHOTOGRAPH_PIN

#ifdef HAS_LCD_CONTRAST

  /**
   * M250: Read and optionally set the LCD contrast
   */
  inline void gcode_M250() {
    if (code_seen('C')) lcd_setcontrast(code_value_short() & 0x3F);
    SERIAL_PROTOCOLPGM("lcd contrast value: ");
    SERIAL_PROTOCOL(lcd_contrast);
    SERIAL_EOL;
  }

#endif // HAS_LCD_CONTRAST

#ifdef PREVENT_DANGEROUS_EXTRUDE

  void set_extrude_min_temp(float temp) { extrude_min_temp = temp; }

  /**
   * M302: Allow cold extrudes, or set the minimum extrude S<temperature>.
   */
  inline void gcode_M302() {
    set_extrude_min_temp(code_seen('S') ? code_value() : 0);
  }

#endif // PREVENT_DANGEROUS_EXTRUDE

/**
 * M303: PID relay autotune
 *       S<temperature> sets the target temperature. (default target temperature = 150C)
 *       E<extruder> (-1 for the bed)
 *       C<cycles>
 */
inline void gcode_M303() {
  int e = code_seen('E') ? code_value_short() : 0;
  int c = code_seen('C') ? code_value_short() : 5;
  float temp = code_seen('S') ? code_value() : (e < 0 ? 70.0 : 150.0);
  PID_autotune(temp, e, c);
}

#ifdef SCARA
  bool SCARA_move_to_cal(uint8_t delta_x, uint8_t delta_y) {
    //SoftEndsEnabled = false;              // Ignore soft endstops during calibration
    //SERIAL_ECHOLN(" Soft endstops disabled ");
    if (IsRunning()) {
      //get_coordinates(); // For X Y Z E F
      delta[X_AXIS] = delta_x;
      delta[Y_AXIS] = delta_y;
      calculate_SCARA_forward_Transform(delta);
      destination[X_AXIS] = delta[X_AXIS]/axis_scaling[X_AXIS];
      destination[Y_AXIS] = delta[Y_AXIS]/axis_scaling[Y_AXIS];
      prepare_move();
      //ClearToSend();
      return true;
    }
    return false;
  }

  /**
   * M360: SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
   */
  inline bool gcode_M360() {
    SERIAL_ECHOLN(" Cal: Theta 0 ");
    return SCARA_move_to_cal(0, 120);
  }

  /**
   * M361: SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
   */
  inline bool gcode_M361() {
    SERIAL_ECHOLN(" Cal: Theta 90 ");
    return SCARA_move_to_cal(90, 130);
  }

  /**
   * M362: SCARA calibration: Move to cal-position PsiA (0 deg calibration)
   */
  inline bool gcode_M362() {
    SERIAL_ECHOLN(" Cal: Psi 0 ");
    return SCARA_move_to_cal(60, 180);
  }

  /**
   * M363: SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
   */
  inline bool gcode_M363() {
    SERIAL_ECHOLN(" Cal: Psi 90 ");
    return SCARA_move_to_cal(50, 90);
  }

  /**
   * M364: SCARA calibration: Move to cal-position PSIC (90 deg to Theta calibration position)
   */
  inline bool gcode_M364() {
    SERIAL_ECHOLN(" Cal: Theta-Psi 90 ");
    return SCARA_move_to_cal(45, 135);
  }

  /**
   * M365: SCARA calibration: Scaling factor, X, Y, Z axis
   */
  inline void gcode_M365() {
    for (int8_t i = X_AXIS; i <= Z_AXIS; i++) {
      if (code_seen(axis_codes[i])) {
        axis_scaling[i] = code_value();
      }
    }
  }

#endif // SCARA

#ifdef EXT_SOLENOID

  void enable_solenoid(uint8_t num) {
    switch(num) {
      case 0:
        OUT_WRITE(SOL0_PIN, HIGH);
        break;
        #if HAS_SOLENOID_1
          case 1:
            OUT_WRITE(SOL1_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_2
          case 2:
            OUT_WRITE(SOL2_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_3
          case 3:
            OUT_WRITE(SOL3_PIN, HIGH);
            break;
        #endif
      default:
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM(MSG_INVALID_SOLENOID);
        break;
    }
  }

  void enable_solenoid_on_active_extruder() { enable_solenoid(active_extruder); }

  void disable_all_solenoids() {
    OUT_WRITE(SOL0_PIN, LOW);
    OUT_WRITE(SOL1_PIN, LOW);
    OUT_WRITE(SOL2_PIN, LOW);
    OUT_WRITE(SOL3_PIN, LOW);
  }

  /**
   * M380: Enable solenoid on the active extruder
   */
  inline void gcode_M380() { enable_solenoid_on_active_extruder(); }

  /**
   * M381: Disable all solenoids
   */
  inline void gcode_M381() { disable_all_solenoids(); }

#endif // EXT_SOLENOID

/**
 * M400: Finish all moves
 */
inline void gcode_M400() { st_synchronize(); }

#if defined(ENABLE_AUTO_BED_LEVELING) && (defined(SERVO_ENDSTOPS) || defined(Z_PROBE_ALLEN_KEY)) && not defined(Z_PROBE_SLED)

  #ifdef SERVO_ENDSTOPS
    void raise_z_for_servo() {
      float zpos = current_position[Z_AXIS], z_dest = Z_RAISE_BEFORE_HOMING;
      if (!axis_known_position[Z_AXIS]) z_dest += zpos;
      if (zpos < z_dest)
        do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], z_dest); // also updates current_position
    }
  #endif

  /**
   * M401: Engage Z Servo endstop if available
   */
  inline void gcode_M401() {
    #ifdef SERVO_ENDSTOPS
      raise_z_for_servo();
    #endif
    deploy_z_probe();
  }

  /**
   * M402: Retract Z Servo endstop if enabled
   */
  inline void gcode_M402() {
    #ifdef SERVO_ENDSTOPS
      raise_z_for_servo();
    #endif
    stow_z_probe();
  }

#endif

#ifdef FILAMENT_SENSOR

  /**
   * M404: Display or set the nominal filament width (3mm, 1.75mm ) W<3.0>
   */
  inline void gcode_M404() {
    #if HAS_FILWIDTH
      if (code_seen('W')) {
        filament_width_nominal = code_value();
      }
      else {
        SERIAL_PROTOCOLPGM("Filament dia (nominal mm):");
        SERIAL_PROTOCOLLN(filament_width_nominal);
      }
    #endif
  }
    
  /**
   * M405: Turn on filament sensor for control
   */
  inline void gcode_M405() {
    if (code_seen('D')) meas_delay_cm = code_value();
    if (meas_delay_cm > MAX_MEASUREMENT_DELAY) meas_delay_cm = MAX_MEASUREMENT_DELAY;

    if (delay_index2 == -1) { //initialize the ring buffer if it has not been done since startup
      int temp_ratio = widthFil_to_size_ratio();

      for (delay_index1 = 0; delay_index1 < MAX_MEASUREMENT_DELAY + 1; ++delay_index1)
        measurement_delay[delay_index1] = temp_ratio - 100;  //subtract 100 to scale within a signed byte

      delay_index1 = delay_index2 = 0;
    }

    filament_sensor = true;

    //SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
    //SERIAL_PROTOCOL(filament_width_meas);
    //SERIAL_PROTOCOLPGM("Extrusion ratio(%):");
    //SERIAL_PROTOCOL(extruder_multiply[active_extruder]);
  }

  /**
   * M406: Turn off filament sensor for control
   */
  inline void gcode_M406() { filament_sensor = false; }
  
  /**
   * M407: Get measured filament diameter on serial output
   */
  inline void gcode_M407() {
    SERIAL_PROTOCOLPGM("Filament dia (measured mm):"); 
    SERIAL_PROTOCOLLN(filament_width_meas);   
  }

#endif // FILAMENT_SENSOR

/**
 * M410: Quickstop - Abort all planned moves
 *
 * This will stop the carriages mid-move, so most likely they
 * will be out of sync with the stepper position after this.
 */
inline void gcode_M410() { quickStop(); }

/**
 * M500: Store settings in EEPROM
 */
inline void gcode_M500() {
  Config_StoreSettings();
}

/**
 * M501: Read settings from EEPROM
 */
inline void gcode_M501() {
  Config_RetrieveSettings();
}

/**
 * M502: Revert to default settings
 */
inline void gcode_M502() {
  Config_ResetDefault();
}

/**
 * M503: print settings currently in memory
 */
inline void gcode_M503() {
  Config_PrintSettings(code_seen('S') && code_value() == 0);
}

#ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

  /**
   * M540: Set whether SD card print should abort on endstop hit (M540 S<0|1>)
   */
  inline void gcode_M540() {
    if (code_seen('S')) abort_on_endstop_hit = (code_value() > 0);
  }

#endif // ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

#ifdef CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

  inline void gcode_SET_Z_PROBE_OFFSET() {
    float value;
    if (code_seen('Z')) {
      value = code_value();
      if (Z_PROBE_OFFSET_RANGE_MIN <= value && value <= Z_PROBE_OFFSET_RANGE_MAX) {
        zprobe_zoffset = -value; // compare w/ line 278 of configuration_store.cpp
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM(MSG_ZPROBE_ZOFFSET " " MSG_OK);
        SERIAL_EOL;
      }
      else {
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM(MSG_ZPROBE_ZOFFSET);
        SERIAL_ECHOPGM(MSG_Z_MIN);
        SERIAL_ECHO(Z_PROBE_OFFSET_RANGE_MIN);
        SERIAL_ECHOPGM(MSG_Z_MAX);
        SERIAL_ECHO(Z_PROBE_OFFSET_RANGE_MAX);
        SERIAL_EOL;
      }
    }
    else {
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_ZPROBE_ZOFFSET " : ");
      SERIAL_ECHO(-zprobe_zoffset);
      SERIAL_EOL;
    }
  }

#endif // CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

#ifdef FILAMENTCHANGEENABLE

  /**
   * M600: Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
   */
  inline void gcode_M600() {
    float target[NUM_AXIS], lastpos[NUM_AXIS], fr60 = feedrate / 60;
    for (int i=0; i<NUM_AXIS; i++)
      target[i] = lastpos[i] = current_position[i];

    #define BASICPLAN plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], fr60, active_extruder);
    #ifdef DELTA
      #define RUNPLAN calculate_delta(target); BASICPLAN
    #else
      #define RUNPLAN BASICPLAN
    #endif

    //retract by E
    if (code_seen('E')) target[E_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_FIRSTRETRACT
      else target[E_AXIS] += FILAMENTCHANGE_FIRSTRETRACT;
    #endif

    RUNPLAN;

    //lift Z
    if (code_seen('Z')) target[Z_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_ZADD
      else target[Z_AXIS] += FILAMENTCHANGE_ZADD;
    #endif

    RUNPLAN;

    //move xy
    if (code_seen('X')) target[X_AXIS] = code_value();
    #ifdef FILAMENTCHANGE_XPOS
      else target[X_AXIS] = FILAMENTCHANGE_XPOS;
    #endif

    if (code_seen('Y')) target[Y_AXIS] = code_value();
    #ifdef FILAMENTCHANGE_YPOS
      else target[Y_AXIS] = FILAMENTCHANGE_YPOS;
    #endif

    RUNPLAN;

    if (code_seen('L')) target[E_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_FINALRETRACT
      else target[E_AXIS] += FILAMENTCHANGE_FINALRETRACT;
    #endif

    RUNPLAN;

    //finish moves
    st_synchronize();
    //disable extruder steppers so filament can be removed
    disable_e0();
    disable_e1();
    disable_e2();
    disable_e3();
    delay(100);
    LCD_ALERTMESSAGEPGM(MSG_FILAMENTCHANGE);
    uint8_t cnt = 0;
    while (!lcd_clicked()) {
      if (++cnt == 0) lcd_quick_feedback(); // every 256th frame till the lcd is clicked
      manage_heater();
      manage_inactivity(true);
      lcd_update();
    } // while(!lcd_clicked)

    //return to normal
    if (code_seen('L')) target[E_AXIS] -= code_value();
    #ifdef FILAMENTCHANGE_FINALRETRACT
      else target[E_AXIS] -= FILAMENTCHANGE_FINALRETRACT;
    #endif

    current_position[E_AXIS] = target[E_AXIS]; //the long retract of L is compensated by manual filament feeding
    plan_set_e_position(current_position[E_AXIS]);

    RUNPLAN; //should do nothing

    lcd_reset_alert_level();

    #ifdef DELTA
      calculate_delta(lastpos);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], target[E_AXIS], fr60, active_extruder); //move xyz back
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], lastpos[E_AXIS], fr60, active_extruder); //final untretract
    #else
      plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], target[Z_AXIS], target[E_AXIS], fr60, active_extruder); //move xy back
      plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], target[E_AXIS], fr60, active_extruder); //move z back
      plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], fr60, active_extruder); //final untretract
    #endif        

    #ifdef FILAMENT_RUNOUT_SENSOR
      filrunoutEnqueued = false;
    #endif
    
  }

#endif // FILAMENTCHANGEENABLE

#ifdef DUAL_X_CARRIAGE

  /**
   * M605: Set dual x-carriage movement mode
   *
   *    M605 S0: Full control mode. The slicer has full control over x-carriage movement
   *    M605 S1: Auto-park mode. The inactive head will auto park/unpark without slicer involvement
   *    M605 S2 [Xnnn] [Rmmm]: Duplication mode. The second extruder will duplicate the first with nnn
   *                         millimeters x-offset and an optional differential hotend temperature of
   *                         mmm degrees. E.g., with "M605 S2 X100 R2" the second extruder will duplicate
   *                         the first with a spacing of 100mm in the x direction and 2 degrees hotter.
   *
   *    Note: the X axis should be homed after changing dual x-carriage mode.
   */
  inline void gcode_M605() {
    st_synchronize();
    if (code_seen('S')) dual_x_carriage_mode = code_value();
    switch(dual_x_carriage_mode) {
      case DXC_DUPLICATION_MODE:
        if (code_seen('X')) duplicate_extruder_x_offset = max(code_value(), X2_MIN_POS - x_home_pos(0));
        if (code_seen('R')) duplicate_extruder_temp_offset = code_value();
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(extruder_offset[X_AXIS][0]);
        SERIAL_CHAR(',');
        SERIAL_ECHO(extruder_offset[Y_AXIS][0]);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(duplicate_extruder_x_offset);
        SERIAL_CHAR(',');
        SERIAL_ECHOLN(extruder_offset[Y_AXIS][1]);
        break;
      case DXC_FULL_CONTROL_MODE:
      case DXC_AUTO_PARK_MODE:
        break;
      default:
        dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
        break;
    }
    active_extruder_parked = false;
    extruder_duplication_enabled = false;
    delayed_move_time = 0;
  }

#endif // DUAL_X_CARRIAGE

/**
 * M907: Set digital trimpot motor current using axis codes X, Y, Z, E, B, S
 */
inline void gcode_M907() {
  #if HAS_DIGIPOTSS
    for (int i=0;i<NUM_AXIS;i++)
      if (code_seen(axis_codes[i])) digipot_current(i, code_value());
    if (code_seen('B')) digipot_current(4, code_value());
    if (code_seen('S')) for (int i=0; i<=4; i++) digipot_current(i, code_value());
  #endif
  #ifdef MOTOR_CURRENT_PWM_XY_PIN
    if (code_seen('X')) digipot_current(0, code_value());
  #endif
  #ifdef MOTOR_CURRENT_PWM_Z_PIN
    if (code_seen('Z')) digipot_current(1, code_value());
  #endif
  #ifdef MOTOR_CURRENT_PWM_E_PIN
    if (code_seen('E')) digipot_current(2, code_value());
  #endif
  #ifdef DIGIPOT_I2C
    // this one uses actual amps in floating point
    for (int i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) digipot_i2c_set_current(i, code_value());
    // for each additional extruder (named B,C,D,E..., channels 4,5,6,7...)
    for (int i=NUM_AXIS;i<DIGIPOT_I2C_NUM_CHANNELS;i++) if(code_seen('B'+i-NUM_AXIS)) digipot_i2c_set_current(i, code_value());
  #endif
}

#if HAS_DIGIPOTSS

  /**
   * M908: Control digital trimpot directly (M908 P<pin> S<current>)
   */
  inline void gcode_M908() {
      digitalPotWrite(
        code_seen('P') ? code_value() : 0,
        code_seen('S') ? code_value() : 0
      );
  }

#endif // HAS_DIGIPOTSS

#if HAS_MICROSTEPS

  // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
  inline void gcode_M350() {
    if(code_seen('S')) for(int i=0;i<=4;i++) microstep_mode(i,code_value());
    for(int i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) microstep_mode(i,(uint8_t)code_value());
    if(code_seen('B')) microstep_mode(4,code_value());
    microstep_readings();
  }

  /**
   * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
   *       S# determines MS1 or MS2, X# sets the pin high/low.
   */
  inline void gcode_M351() {
    if (code_seen('S')) switch(code_value_short()) {
      case 1:
        for(int i=0;i<NUM_AXIS;i++) if (code_seen(axis_codes[i])) microstep_ms(i, code_value(), -1);
        if (code_seen('B')) microstep_ms(4, code_value(), -1);
        break;
      case 2:
        for(int i=0;i<NUM_AXIS;i++) if (code_seen(axis_codes[i])) microstep_ms(i, -1, code_value());
        if (code_seen('B')) microstep_ms(4, -1, code_value());
        break;
    }
    microstep_readings();
  }

#endif // HAS_MICROSTEPS

/**
 * M999: Restart after being stopped
 */
inline void gcode_M999() {
  Running = true;
  lcd_reset_alert_level();
  gcode_LastN = Stopped_gcode_LastN;
  FlushSerialRequestResend();
}

/**
 * T0-T3: Switch tool, usually switching extruders
 */
inline void gcode_T() {
  int tmp_extruder = code_value();
  if (tmp_extruder >= EXTRUDERS) {
    SERIAL_ECHO_START;
    SERIAL_CHAR('T');
    SERIAL_ECHO(tmp_extruder);
    SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
  }
  else {
    target_extruder = tmp_extruder;

    #if EXTRUDERS > 1
      bool make_move = false;
    #endif

    if (code_seen('F')) {

      #if EXTRUDERS > 1
        make_move = true;
      #endif

      float fr = code_value();
      if (fr > 0.0) feedrate = fr;
    }
    #if EXTRUDERS > 1
      if (tmp_extruder != active_extruder) {
        // Save current position to return to after applying extruder offset
        set_destination_to_current();
        #ifdef DUAL_X_CARRIAGE
          if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE && IsRunning() &&
                (delayed_move_time != 0 || current_position[X_AXIS] != x_home_pos(active_extruder))) {
            // Park old head: 1) raise 2) move to park position 3) lower
            plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] + TOOLCHANGE_PARK_ZLIFT,
                  current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
            plan_buffer_line(x_home_pos(active_extruder), current_position[Y_AXIS], current_position[Z_AXIS] + TOOLCHANGE_PARK_ZLIFT,
                  current_position[E_AXIS], max_feedrate[X_AXIS], active_extruder);
            plan_buffer_line(x_home_pos(active_extruder), current_position[Y_AXIS], current_position[Z_AXIS],
                  current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
            st_synchronize();
          }

          // apply Y & Z extruder offset (x offset is already used in determining home pos)
          current_position[Y_AXIS] = current_position[Y_AXIS] -
                       extruder_offset[Y_AXIS][active_extruder] +
                       extruder_offset[Y_AXIS][tmp_extruder];
          current_position[Z_AXIS] = current_position[Z_AXIS] -
                       extruder_offset[Z_AXIS][active_extruder] +
                       extruder_offset[Z_AXIS][tmp_extruder];

          active_extruder = tmp_extruder;

          // This function resets the max/min values - the current position may be overwritten below.
          axis_is_at_home(X_AXIS);

          if (dual_x_carriage_mode == DXC_FULL_CONTROL_MODE) {
            current_position[X_AXIS] = inactive_extruder_x_pos;
            inactive_extruder_x_pos = destination[X_AXIS];
          }
          else if (dual_x_carriage_mode == DXC_DUPLICATION_MODE) {
            active_extruder_parked = (active_extruder == 0); // this triggers the second extruder to move into the duplication position
            if (active_extruder == 0 || active_extruder_parked)
              current_position[X_AXIS] = inactive_extruder_x_pos;
            else
              current_position[X_AXIS] = destination[X_AXIS] + duplicate_extruder_x_offset;
            inactive_extruder_x_pos = destination[X_AXIS];
            extruder_duplication_enabled = false;
          }
          else {
            // record raised toolhead position for use by unpark
            memcpy(raised_parked_position, current_position, sizeof(raised_parked_position));
            raised_parked_position[Z_AXIS] += TOOLCHANGE_UNPARK_ZLIFT;
            active_extruder_parked = true;
            delayed_move_time = 0;
          }
        #else // !DUAL_X_CARRIAGE
          // Offset extruder (only by XY)
          for (int i=X_AXIS; i<=Y_AXIS; i++)
            current_position[i] += extruder_offset[i][tmp_extruder] - extruder_offset[i][active_extruder];
          // Set the new active extruder and position
          active_extruder = tmp_extruder;
        #endif // !DUAL_X_CARRIAGE
        #ifdef DELTA
          sync_plan_position_delta();
        #else
          sync_plan_position();
        #endif
        // Move to the old position if 'F' was in the parameters
        if (make_move && IsRunning()) prepare_move();
      }

      #ifdef EXT_SOLENOID
        st_synchronize();
        disable_all_solenoids();
        enable_solenoid_on_active_extruder();
      #endif // EXT_SOLENOID

    #endif // EXTRUDERS > 1
    SERIAL_ECHO_START;
    SERIAL_ECHO(MSG_ACTIVE_EXTRUDER);
    SERIAL_PROTOCOLLN((int)active_extruder);
  }
}

/**
 * Process Commands and dispatch them to handlers
 * This is called from the main loop()
 */
void process_commands() {

  if ((marlin_debug_flags & DEBUG_ECHO)) {
    SERIAL_ECHO_START;
    SERIAL_ECHOLN(command_queue[cmd_queue_index_r]);
  }

  if (code_seen('G')) {

    int gCode = code_value_short();

    switch(gCode) {

    // G0, G1
    case 0:
    case 1:
      gcode_G0_G1();
      break;

    // G2, G3
    #ifndef SCARA
      case 2: // G2  - CW ARC
      case 3: // G3  - CCW ARC
        gcode_G2_G3(gCode == 2);
        break;
    #endif

    // G4 Dwell
    case 4:
      gcode_G4();
      break;

    #ifdef FWRETRACT

      case 10: // G10: retract
      case 11: // G11: retract_recover
        gcode_G10_G11(gCode == 10);
        break;

    #endif //FWRETRACT

    case 28: // G28: Home all axes, one at a time
      gcode_G28();
      break;

    #if defined(ENABLE_AUTO_BED_LEVELING) || defined(MESH_BED_LEVELING)
      case 29: // G29 Detailed Z-Probe, probes the bed at 3 or more points.
        gcode_G29();
        break;
    #endif

    #ifdef ENABLE_AUTO_BED_LEVELING

      #ifndef Z_PROBE_SLED

        case 30: // G30 Single Z Probe
          gcode_G30();
          break;

      #else // Z_PROBE_SLED

          case 31: // G31: dock the sled
          case 32: // G32: undock the sled
            dock_sled(gCode == 31);
            break;

      #endif // Z_PROBE_SLED

    #endif // ENABLE_AUTO_BED_LEVELING

    case 90: // G90
      relative_mode = false;
      break;
    case 91: // G91
      relative_mode = true;
      break;

    case 92: // G92
      gcode_G92();
      break;
    }
  }

  else if (code_seen('M')) {
    switch(code_value_short()) {
      #ifdef ULTIPANEL
        case 0: // M0 - Unconditional stop - Wait for user button press on LCD
        case 1: // M1 - Conditional stop - Wait for user button press on LCD
          gcode_M0_M1();
          break;
      #endif // ULTIPANEL

      case 17:
        gcode_M17();
        break;

      #ifdef SDSUPPORT

        case 20: // M20 - list SD card
          gcode_M20(); break;
        case 21: // M21 - init SD card
          gcode_M21(); break;
        case 22: //M22 - release SD card
          gcode_M22(); break;
        case 23: //M23 - Select file
          gcode_M23(); break;
        case 24: //M24 - Start SD print
          gcode_M24(); break;
        case 25: //M25 - Pause SD print
          gcode_M25(); break;
        case 26: //M26 - Set SD index
          gcode_M26(); break;
        case 27: //M27 - Get SD status
          gcode_M27(); break;
        case 28: //M28 - Start SD write
          gcode_M28(); break;
        case 29: //M29 - Stop SD write
          gcode_M29(); break;
        case 30: //M30 <filename> Delete File
          gcode_M30(); break;
        case 32: //M32 - Select file and start SD print
          gcode_M32(); break;
        case 928: //M928 - Start SD write
          gcode_M928(); break;

      #endif //SDSUPPORT

      case 31: //M31 take time since the start of the SD print or an M109 command
        gcode_M31();
        break;

      case 42: //M42 -Change pin status via gcode
        gcode_M42();
        break;

      #if defined(ENABLE_AUTO_BED_LEVELING) && defined(Z_PROBE_REPEATABILITY_TEST)
        case 48: // M48 Z-Probe repeatability
          gcode_M48();
          break;
      #endif // ENABLE_AUTO_BED_LEVELING && Z_PROBE_REPEATABILITY_TEST

      case 104: // M104
        gcode_M104();
        break;

      case 111: //  M111: Set debug level
        gcode_M111();
        break;

      case 112: //  M112: Emergency Stop
        gcode_M112();
        break;

      case 140: // M140: Set bed temp
        gcode_M140();
        break;

      case 105: // M105: Read current temperature
        gcode_M105();
        return;
        break;

      case 109: // M109: Wait for temperature
        gcode_M109();
        break;

      #if HAS_TEMP_BED
        case 190: // M190: Wait for bed heater to reach target
          gcode_M190();
          break;
      #endif // HAS_TEMP_BED

      #if HAS_FAN
        case 106: // M106: Fan On
          gcode_M106();
          break;
        case 107: // M107: Fan Off
          gcode_M107();
          break;
      #endif // HAS_FAN

      #ifdef BARICUDA
        // PWM for HEATER_1_PIN
        #if HAS_HEATER_1
          case 126: // M126: valve open
            gcode_M126();
            break;
          case 127: // M127: valve closed
            gcode_M127();
            break;
        #endif // HAS_HEATER_1

        // PWM for HEATER_2_PIN
        #if HAS_HEATER_2
          case 128: // M128: valve open
            gcode_M128();
            break;
          case 129: // M129: valve closed
            gcode_M129();
            break;
        #endif // HAS_HEATER_2
      #endif // BARICUDA

      #if HAS_POWER_SWITCH

        case 80: // M80: Turn on Power Supply
          gcode_M80();
          break;

      #endif // HAS_POWER_SWITCH

      case 81: // M81: Turn off Power, including Power Supply, if possible
        gcode_M81();
        break;

      case 82:
        gcode_M82();
        break;
      case 83:
        gcode_M83();
        break;
      case 18: // (for compatibility)
      case 84: // M84
        gcode_M18_M84();
        break;
      case 85: // M85
        gcode_M85();
        break;
      case 92: // M92
        gcode_M92();
        break;
      case 115: // M115
        gcode_M115();
        break;
      case 117: // M117 display message
        gcode_M117();
        break;
      case 114: // M114
        gcode_M114();
        break;
      case 120: // M120
        gcode_M120();
        break;
      case 121: // M121
        gcode_M121();
        break;
      case 119: // M119
        gcode_M119();
        break;
        //TODO: update for all axis, use for loop

      #ifdef BLINKM

        case 150: // M150
          gcode_M150();
          break;

      #endif //BLINKM

      case 200: // M200 D<millimeters> set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
        gcode_M200();
        break;
      case 201: // M201
        gcode_M201();
        break;
      #if 0 // Not used for Sprinter/grbl gen6
      case 202: // M202
        gcode_M202();
        break;
      #endif
      case 203: // M203 max feedrate mm/sec
        gcode_M203();
        break;
      case 204: // M204 acclereration S normal moves T filmanent only moves
        gcode_M204();
        break;
      case 205: //M205 advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
        gcode_M205();
        break;
      case 206: // M206 additional homing offset
        gcode_M206();
        break;

      #ifdef DELTA
        case 665: // M665 set delta configurations L<diagonal_rod> R<delta_radius> S<segments_per_sec>
          gcode_M665();
          break;
      #endif

      #if defined(DELTA) || defined(Z_DUAL_ENDSTOPS)
        case 666: // M666 set delta / dual endstop adjustment
          gcode_M666();
          break;
      #endif

      #ifdef FWRETRACT
        case 207: //M207 - set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop]
          gcode_M207();
          break;
        case 208: // M208 - set retract recover length S[positive mm surplus to the M207 S*] F[feedrate mm/min]
          gcode_M208();
          break;
        case 209: // M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
          gcode_M209();
          break;
      #endif // FWRETRACT

      #if EXTRUDERS > 1
        case 218: // M218 - set hotend offset (in mm), T<extruder_number> X<offset_on_X> Y<offset_on_Y>
          gcode_M218();
          break;
      #endif

      case 220: // M220 S<factor in percent>- set speed factor override percentage
        gcode_M220();
        break;

      case 221: // M221 S<factor in percent>- set extrude factor override percentage
        gcode_M221();
        break;

      case 226: // M226 P<pin number> S<pin state>- Wait until the specified pin reaches the state required
        gcode_M226();
        break;

      #if NUM_SERVOS > 0
        case 280: // M280 - set servo position absolute. P: servo index, S: angle or microseconds
          gcode_M280();
          break;
      #endif // NUM_SERVOS > 0

      #if BEEPER > 0 || defined(ULTRALCD) || defined(LCD_USE_I2C_BUZZER)
        case 300: // M300 - Play beep tone
          gcode_M300();
          break;
      #endif // BEEPER > 0 || ULTRALCD || LCD_USE_I2C_BUZZER

      #ifdef PIDTEMP
        case 301: // M301
          gcode_M301();
          break;
      #endif // PIDTEMP

      #ifdef PIDTEMPBED
        case 304: // M304
          gcode_M304();
          break;
      #endif // PIDTEMPBED

      #if defined(CHDK) || HAS_PHOTOGRAPH
        case 240: // M240  Triggers a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
          gcode_M240();
          break;
      #endif // CHDK || PHOTOGRAPH_PIN

      #ifdef HAS_LCD_CONTRAST
        case 250: // M250  Set LCD contrast value: C<value> (value 0..63)
          gcode_M250();
          break;
      #endif // HAS_LCD_CONTRAST

      #ifdef PREVENT_DANGEROUS_EXTRUDE
        case 302: // allow cold extrudes, or set the minimum extrude temperature
          gcode_M302();
          break;
      #endif // PREVENT_DANGEROUS_EXTRUDE

      case 303: // M303 PID autotune
        gcode_M303();
        break;

      #ifdef SCARA
        case 360:  // M360 SCARA Theta pos1
          if (gcode_M360()) return;
          break;
        case 361:  // M361 SCARA Theta pos2
          if (gcode_M361()) return;
          break;
        case 362:  // M362 SCARA Psi pos1
          if (gcode_M362()) return;
          break;
        case 363:  // M363 SCARA Psi pos2
          if (gcode_M363()) return;
          break;
        case 364:  // M364 SCARA Psi pos3 (90 deg to Theta)
          if (gcode_M364()) return;
          break;
        case 365: // M365 Set SCARA scaling for X Y Z
          gcode_M365();
          break;
      #endif // SCARA

      case 400: // M400 finish all moves
        gcode_M400();
        break;

      #if defined(ENABLE_AUTO_BED_LEVELING) && (defined(SERVO_ENDSTOPS) || defined(Z_PROBE_ALLEN_KEY)) && not defined(Z_PROBE_SLED)
        case 401:
          gcode_M401();
          break;
        case 402:
          gcode_M402();
          break;
      #endif

      #ifdef FILAMENT_SENSOR
        case 404:  //M404 Enter the nominal filament width (3mm, 1.75mm ) N<3.0> or display nominal filament width
          gcode_M404();
          break;
        case 405:  //M405 Turn on filament sensor for control
          gcode_M405();
          break;
        case 406:  //M406 Turn off filament sensor for control
          gcode_M406();
          break;
        case 407:   //M407 Display measured filament diameter
          gcode_M407();
          break;
      #endif // FILAMENT_SENSOR

      case 410: // M410 quickstop - Abort all the planned moves.
        gcode_M410();
        break;

      case 500: // M500 Store settings in EEPROM
        gcode_M500();
        break;
      case 501: // M501 Read settings from EEPROM
        gcode_M501();
        break;
      case 502: // M502 Revert to default settings
        gcode_M502();
        break;
      case 503: // M503 print settings currently in memory
        gcode_M503();
        break;

      #ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED
        case 540:
          gcode_M540();
          break;
      #endif

      #ifdef CUSTOM_M_CODE_SET_Z_PROBE_OFFSET
        case CUSTOM_M_CODE_SET_Z_PROBE_OFFSET:
          gcode_SET_Z_PROBE_OFFSET();
          break;
      #endif // CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

      #ifdef FILAMENTCHANGEENABLE
        case 600: //Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
          gcode_M600();
          break;
      #endif // FILAMENTCHANGEENABLE

      #ifdef DUAL_X_CARRIAGE
        case 605:
          gcode_M605();
          break;
      #endif // DUAL_X_CARRIAGE

      case 907: // M907 Set digital trimpot motor current using axis codes.
        gcode_M907();
        break;

      #if HAS_DIGIPOTSS
        case 908: // M908 Control digital trimpot directly.
          gcode_M908();
          break;
      #endif // HAS_DIGIPOTSS

      #if HAS_MICROSTEPS

        case 350: // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
          gcode_M350();
          break;

        case 351: // M351 Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
          gcode_M351();
          break;

      #endif // HAS_MICROSTEPS

      case 999: // M999: Restart after being Stopped
        gcode_M999();
        break;
    }
  }

  else if (code_seen('T')) {
    gcode_T();
  }

  else {
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
    SERIAL_ECHO(command_queue[cmd_queue_index_r]);
    SERIAL_ECHOLNPGM("\"");
  }

  ClearToSend();
}
