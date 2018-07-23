#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <inttypes.h>
#include "../core/macros.h"
#include "../../Configuration.h"
#include "planner.h"

//===========================================================================
//=============================public variables=============================
//===========================================================================
//float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int16_t feedrate_percentage = 100; // In percent: 100->1 200->2
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0};
#ifdef DELTA
float endstop_adj[3]={0,0,0};
#endif

double min_pos_extruded[3] = { std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
double max_pos_extruded[3] = { -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};
uint8_t active_extruder = 0;
bool junction_deviation = false; // This replaces the JUNCTION_DEVIATION constant.
bool s_curve_acceleration = true; // This replaces the S_CURVE_ACCELERATION constant.
double filament_diameter = DEFAULT_NOMINAL_FILAMENT_DIA;
bool volumetric_enabled = false;
//===========================================================================
//=============================private variables=============================
//===========================================================================
static double extrusion_length[MAX_EXTRUDERS] = {0,0,0,0,0};
static double calc_destination[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};
//static double offset[3] = {0.0, 0.0, 0.0};
//static bool home_all_axis = true;
static double feedrate = 1500.0;
long gcode_N, gcode_LastN;//, Stopped_gcode_LastN = 0;

static char cmdbuffer[MAX_CMD_SIZE];
//static bool fromsd[BUFSIZE];
//static int i = 0;
//static char serial_char;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

//static double tt = 0;
//static double bt = 0;

//Inactivity shutdown variables
//static unsigned long max_inactive_time = 0;
//static unsigned long stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME*1000l;

unsigned long starttime=0;
unsigned long stoptime=0;

//static uint8_t tmp_extruder;


bool Stopped=false;

#if NUM_SERVOS > 0
  Servo servos[NUM_SERVOS];
#endif

bool CooldownNoWait = true;
bool target_direction;


int fanSpeed = 0;

//void plan_buffer_line(const double &x, const float &y, const float &z, const float &e, float feed_rate, const uint8_t &extruder);

// Run this after any change in the printer's parameters, like max acceleration, jerk, etc.
void recalculate_rates() {
  Planner::refresh_positioning();
  // steps per sq second need to be updated to agree with the units per sq second
  Planner::reset_acceleration_rates();
  Planner::calculate_volumetric_multipliers();
}

void Config_ResetDefault()
{
    double tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
    double tmp2[]=DEFAULT_MAX_FEEDRATE;
    long tmp3[]=DEFAULT_MAX_ACCELERATION;
    for (short i=0;i<4;i++)
    {
      Planner::axis_steps_per_mm[i]=tmp1[i];
      Planner::steps_to_mm[i]=1/tmp1[i];
      Planner::max_feedrate_mm_s[i]=tmp2[i];
      Planner::max_acceleration_mm_per_s2[i]=tmp3[i];
    }

    Planner::acceleration=DEFAULT_ACCELERATION;
    Planner::travel_acceleration=DEFAULT_TRAVEL_ACCELERATION;
    Planner::retract_acceleration=DEFAULT_RETRACT_ACCELERATION;
    Planner::min_feedrate_mm_s=DEFAULT_MINIMUMFEEDRATE;
    Planner::min_segment_time_us=DEFAULT_MINSEGMENTTIME;
    Planner::min_travel_feedrate_mm_s=DEFAULT_MINTRAVELFEEDRATE;
    Planner::max_jerk[X_AXIS] = DEFAULT_XJERK;
    Planner::max_jerk[Y_AXIS] = DEFAULT_YJERK;
    Planner::max_jerk[Z_AXIS] = DEFAULT_ZJERK;
    Planner::max_jerk[E_AXIS] = DEFAULT_EJERK;
    Planner::junction_deviation_mm = JUNCTION_DEVIATION_MM;
    recalculate_rates();
}

double code_value() {
  return (strtod(&cmdbuffer[strchr_pointer - cmdbuffer + 1], NULL));
}

long code_value_long()
{
  return (strtol(&cmdbuffer[strchr_pointer - cmdbuffer + 1], NULL, 10));
}

bool code_seen(char code)
{
  strchr_pointer = strchr(cmdbuffer, code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}

double extruder_position = 0;

void prepare_move(const ExtraData& extra_data)
{
  Planner::buffer_line(calc_destination[X_AXIS], calc_destination[Y_AXIS], calc_destination[Z_AXIS], calc_destination[E_AXIS], MMS_SCALED(feedrate/60), active_extruder, 0.0, extra_data);

  bool moved = calc_destination[X_AXIS] != current_position[X_AXIS] ||
               calc_destination[Y_AXIS] != current_position[Y_AXIS] ||
               calc_destination[Z_AXIS] != current_position[Z_AXIS];
  double extruded = calc_destination[E_AXIS] - current_position[E_AXIS];
  extruder_position += extruded;
  if (extruded != 0 && moved) {
    for (int i = 0; i < NUM_AXIS - 1; i++) {
      min_pos_extruded[i] = min(min_pos_extruded[i], calc_destination[i]);
      max_pos_extruded[i] = max(max_pos_extruded[i], calc_destination[i]);
    }
    int target_extruder = active_extruder;
    if (code_seen('T')) {
      target_extruder = code_value();
    }
    extrusion_length[target_extruder] += extruded;
  }
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = calc_destination[i];
  }
}

void get_coordinates()
{
  for(int8_t i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i]))
    {
      calc_destination[i] = (double)code_value() + (axis_relative_modes[i])*current_position[i];
    }
    else calc_destination[i] = current_position[i]; //Are these else lines really needed?
  }
  if(code_seen('F')) {
    double next_feedrate = code_value();
    if(next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

std::string get_command(std::istream& in) {
  // Return a string with the new command in it.
  // Filters out all comments and line numbers and checksums.
  // Returns an empty string when there are no more lines.
  // Assume that we start at the beginning of a line always.
  std::string line;
  if (!std::getline(in, line)) {
    return line;
  }
  auto comment_start = line.find(';');
  if (comment_start != std::string::npos) {
    line.erase(comment_start);
  }
  auto first_char =
      std::find_if(line.begin(), line.end(), std::iswspace);
  if (first_char != line.end()) {
    // Found the first non-whitespace
    if (*first_char == 'N' || *first_char == 'n') {
      // Found a line number.
      std::string::iterator end_line_number =
          std::find_if_not(first_char+1, line.end(), static_cast<int(*)(int)>(std::isdigit));
      line.erase(line.begin(), end_line_number);
    }
  }
  // Remove the checksum, don't even check it.
  auto checksum_start = line.find('*');
  if (checksum_start != std::string::npos) {
    line.erase(checksum_start);
  }
  if (line.size() == 0) {
    return std::string(" "); // TODO: find a better way to return end of file
  }
  return line;
}

double total_time = 0;
int total_g=0, total_m=0;


void set_junction_deviation(bool new_value) {
  if (new_value == junction_deviation) {
    return;
  }
  junction_deviation = new_value;
  fprintf(stderr, "Junction deviation %s\n",
          junction_deviation ? "enabled" : "disabled");
}

// Return the axis from the axis code accounting for T codes that might
// temporarily change the target tool.  get_axis calls code_seen so be careful
// if you call it between code_seen and code_value.
int get_axis(int i) {
  if (i != E_AXIS) {
    return i;
  }
  if (code_seen('T')) {
    return E_AXIS + code_value();
  }
  return E_AXIS + active_extruder;
}

void process_commands(const std::string& command, const ExtraData& extra_data) {
  cmdbuffer[command.copy(cmdbuffer, MAX_CMD_SIZE-1)] = 0; // TODO: get rid of this ugliness
  unsigned long codenum; //throw away variable

  if(code_seen('G')) {
    total_g++;
    switch((int)code_value())
    {
    case 0: // G0 -> G1
    case 1: // G1
      if(Stopped == false) {
        get_coordinates(); // For X Y Z E F
        prepare_move(extra_data);
        //ClearToSend();
        return;
      } else fprintf(stderr, "STOPPED!!");
      //break;
    case 4: // G4 dwell
      codenum = 0;
      if(code_seen('P')) codenum = code_value(); // milliseconds to wait
      if(code_seen('S')) codenum = code_value() * 1000; // seconds to wait

	  total_time += codenum / 1000.0;
      break;
    case 28: //G28 Home all Axis one at a time
      total_time += 5; // 5 seconds to home
      break;
    case 90: // G90
      for (int i = 0; i < 4; i++) {
        axis_relative_modes[i] = false;
      }
      break;
    case 91: // G91
      for (int i = 0; i < 4; i++) {
        axis_relative_modes[i] = true;
      }
      break;
    case 92: // G92
      for(int8_t i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) {
          current_position[i] = code_value();
          Planner::set_position_mm(AxisEnum(i), current_position[i]);
        }
      }
      break;
	}
  } else if(code_seen('M')) {
    total_m++;
    switch((int)code_value()) {
      case 82: // M82: Set extruder to absolute mode
        axis_relative_modes[3] = false;
        break;
      case 83: // M83: Set extruder to relative mode
        axis_relative_modes[3] = true;
        break;
      case 92:
        {
          for (int i = 0; i < NUM_AXIS; i++) {
            int axis = get_axis(i);
            if (code_seen(axis_codes[i])) {
              Planner::axis_steps_per_mm[axis] = code_value();
            }
          }
        }
        break;
      case 200: // M200 - Set Filament Diameter
        {
          // The settings include this and then disable it if we're not in
          // volumetic mode, which is hopefully everyone!
          int target_extruder = active_extruder;
          if (code_seen('T')) {
            target_extruder = code_value();
          }
          if (code_seen('D')) {
            if (code_value() != 0) {
              filament_diameter = code_value();
              volumetric_enabled = true;  // Global for all extruders.
              planner.set_filament_size(target_extruder, code_value());
            } else {
              // "M200 D" or "M200 D0", both turn off volumetic extrusion but
              // we'll keep the filament_diamter that we learned.
              volumetric_enabled = false;
              planner.set_filament_size(target_extruder, code_value());
            }
          }
        }
      case 201: // M201
        {
          for(int i = 0; i < NUM_AXIS; i++) {
            int axis = get_axis(i);
            if(code_seen(axis_codes[i])) {
              Planner::max_acceleration_mm_per_s2[axis] = code_value();
            }
          }
        }
        break;
      case 203: // M203 max feedrate mm/sec
        {
          for(int i=0; i < NUM_AXIS; i++) {
            int axis = get_axis(i);
            if(code_seen(axis_codes[i])) {
              Planner::max_feedrate_mm_s[axis] = code_value();
            }
          }
        }
        break;
      case 204: // M204 acclereration S normal moves T filmanent only moves
        {
          if(code_seen('S')) {
            Planner::acceleration = code_value();
            Planner::travel_acceleration = code_value();
          }
          if(code_seen('P')) {
            Planner::acceleration = code_value();
          }
          if(code_seen('R')) {
            Planner::retract_acceleration = code_value();
          }
          if(code_seen('T')) {
            Planner::travel_acceleration = code_value();
          }
        }
        break;
      case 205: //M205 advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
        if(code_seen('B')) Planner::min_segment_time_us = code_value();
        if(code_seen('S')) Planner::min_feedrate_mm_s = code_value();
        if(code_seen('T')) Planner::min_travel_feedrate_mm_s = code_value();

        // jdev handling below taken from:
        // Marlin/Marlin/src/gcode/config/M200-M205.cpp
        for (unsigned int i=0; i < NUM_AXIS; i++) {
          if (code_seen(axis_codes[i])) {
            // Seeing any of XYZE implies that we have jdev disabled.
            set_junction_deviation(false);
            Planner::max_jerk[i] = code_value();
          }
        }
        if(code_seen('J')) {
          // Seeing junction deviation implies that we have jdev compiled.
          set_junction_deviation(true);
          const float junc_dev = code_value();
          if (WITHIN(junc_dev, 0.01f, 0.3f)) {
            Planner::junction_deviation_mm = junc_dev;
            planner.recalculate_max_e_jerk();
          }
        }
        break;
      case 220: // M220 S<factor in percent>- set speed factor override percentage
        {
          if(code_seen('S'))
          {
            feedrate_percentage = code_value();
          }
        }
        break;
      case 221: // M221 S<factor in percent>- set extrude factor override percentage
        {
          int target_extruder = active_extruder;
          if (code_seen('T')) {
            target_extruder = code_value();
          }
          if(code_seen('S')) {
            Planner::flow_percentage[target_extruder] = code_value();
            planner.refresh_e_factor(target_extruder);
          }
        }
        break;
    }
    recalculate_rates(); // M commands have the potential to affect planner calculations.
  } else if (code_seen('T')) {
    // Tool change
    active_extruder = code_value();
  }
}

int blocks = 0;

uint8_t last_direction_bits = 0;

// Returns true if we found a block.
bool idle2() {
  block_t *block = Planner::get_current_block();
  if (block == NULL) {
    return false;
  }
  blocks++;
  if(blocks % 100000 == 0) { fprintf(stderr, "."); }

  if (block->step_event_count > 0) {
    // Useful equations: https://quizlet.com/45763821/sat-physics-equations-2-flash-cards/
    double d = min(block->accelerate_until, 0); // Accelerate until this distance is past.
    d = max(d, 0);
    double tt1 = d * 2 / (block->cruise_rate + block->initial_rate);

    d = min(block->decelerate_after, block->step_event_count) - d;
    d = max(d, 0);
    double tt2 = d / block->cruise_rate;

    d = block->step_event_count - d;
    d = max(d, 0);
    double tt3 = d*2/(block->final_rate + block->cruise_rate);

    double tt4 = 0;
    if (block->direction_bits != last_direction_bits) {
      last_direction_bits = block->direction_bits;
      // MINIMUM_STEPPER_DIR_DELAY is in ns, described in Configuration_adv.h
      tt4 += double(MINIMUM_STEPPER_DIR_DELAY)/1000/1000/1000;
    }
    total_time += tt1+tt2+tt3+tt4;
    printf("Progress: %.17f, %.17f, %.17f\n", block->extra_data.filepos,
           block->extra_data.extruder_position,
           total_time);
  }
  Planner::discard_current_block();
  return true;
}


int main(int argc, char *argv[]) {
  double x=0, y=0, z=0, e=0, feed_rate=0;
  uint8_t extruder = 0;
  Config_ResetDefault();
  planner.init();
  {
    // Add an initial line.
    ExtraData extra_data;
    extra_data.filepos = 0;
    Planner::buffer_line(x,y,z,e,feed_rate,extruder,0.0, extra_data);
  }
  if (argc > 2) {
    // There are mcodes in the input.  Process them but they don't count
    // as bytes in the file.
    std::istringstream mcodes(argv[2]);
    std::string new_command = get_command(mcodes);
    while (new_command.size() > 0) {
      ExtraData extra_data; // Ignored.
      extra_data.filepos = 0;
      extra_data.extruder_position = 0;
      process_commands(new_command, extra_data);
      new_command = get_command(mcodes);
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second
  Planner::reset_acceleration_rates();

  std::ifstream in(argv[1], std::ifstream::ate | std::ifstream::binary);
  long total_file_size = in.tellg();
  in.seekg(0, in.beg);  // back to the start
  std::string new_command = get_command(in);
  while(new_command.size() > 0) {
    ExtraData extra_data;
    extra_data.filepos = (((double) in.tellg())/total_file_size);
    extra_data.extruder_position = extruder_position;
    process_commands(new_command, extra_data);
    new_command = get_command(in);
  }
  while(idle2())
    ; // Keep going.
  in.close();
  fprintf(stderr, "Processed %d Gcodes and %d Mcodes. %d blocks\n", total_g, total_m, blocks);
  fprintf(stderr, "Total time: %f\n", total_time);
  printf("analysis: {");
  printf("\"estimatedPrintTime\": %.17f, ", total_time);
  printf("\"printingArea\": ");
  printf("{\"maxX\": %.17f, \"maxY\": %.17f, \"maxZ\": %.17f,",
         max_pos_extruded[0], max_pos_extruded[1], max_pos_extruded[2]);
  printf(" \"minX\": %.17f, \"minY\": %.17f, \"minZ\": %.17f}",
         min_pos_extruded[0], min_pos_extruded[1], min_pos_extruded[2]);
  printf(", \"dimensions\": {\"width\": %.17f, \"depth\": %.17f, \"height\": %.17f}, ",
         max_pos_extruded[0] - min_pos_extruded[0],
         max_pos_extruded[1] - min_pos_extruded[1],
         max_pos_extruded[2] - min_pos_extruded[2]);
  printf("\"filament\": {");
  bool printed_tool = false;
  for (int i = 0; i < MAX_EXTRUDERS; i++) {
    if (extrusion_length[i]) {
      if (printed_tool) {
        printf(", ");
      }
      printf("\"tool%d\": {\"length\": %.17f, \"volume\": %.17f}", i,
             extrusion_length[i], // in mm
             extrusion_length[i] * filament_diameter/2 * filament_diameter/2 * M_PI / 1000 // in cm^3
             );
      printed_tool = true;
    }
  }
  printf("}}\n");
  return 0;
}
