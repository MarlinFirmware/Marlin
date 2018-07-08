#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../core/macros.h"
#include "../../Configuration.h"
#include "planner.h"

//===========================================================================
//=============================public variables=============================
//===========================================================================
//float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
int extrudemultiply=100; //100->1 200->2
int extruder_multiply[EXTRUDERS] = {100
  #if EXTRUDERS > 1
    , 100
    #if EXTRUDERS > 2
      , 100
    #endif
  #endif
};
float volumetric_multiplier[EXTRUDERS] = {1.0
  #if EXTRUDERS > 1
    , 1.0
    #if EXTRUDERS > 2
      , 1.0
    #endif
  #endif
};
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
float add_homing[3]={0,0,0};
#ifdef DELTA
float endstop_adj[3]={0,0,0};
#endif

double min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
double max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
uint8_t active_extruder = 0;
//===========================================================================
//=============================private variables=============================
//===========================================================================
//const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};
static double calc_destination[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};
//static double offset[3] = {0.0, 0.0, 0.0};
//static bool home_all_axis = true;
static double feedrate = 1500.0, next_feedrate;//, saved_feedrate;
long gcode_N, gcode_LastN;//, Stopped_gcode_LastN = 0;

static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
//static bool fromsd[BUFSIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;
//static int i = 0;
//static char serial_char;
static int serial_count = 0;
static bool comment_mode = false;
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
    
    // steps per sq second need to be updated to agree with the units per sq second
    Planner::reset_acceleration_rates();
    
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
}

double code_value() 
{ 
  return (strtod(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL)); 
}

long code_value_long() 
{ 
  return (strtol(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL, 10)); 
}

bool code_seen(char code)
{
  strchr_pointer = strchr(cmdbuffer[bufindr], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}

void prepare_move()
{
  //clamp_to_software_endstops(destination);

  // Do not use feedmultiply for E or Z only moves
  if( (current_position[X_AXIS] == calc_destination [X_AXIS]) && (current_position[Y_AXIS] == calc_destination [Y_AXIS])) {
    Planner::buffer_line(calc_destination[X_AXIS], calc_destination[Y_AXIS], calc_destination[Z_AXIS], calc_destination[E_AXIS], feedrate/60, active_extruder);
  }
  else {
    Planner::buffer_line(calc_destination[X_AXIS], calc_destination[Y_AXIS], calc_destination[Z_AXIS], calc_destination[E_AXIS], feedrate*feedmultiply/60/100.0, active_extruder);
  }
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = calc_destination[i];
  }
}

void get_coordinates()
{
  bool seen[4]={false,false,false,false};
  for(int8_t i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i])) 
    {
      calc_destination[i] = (double)code_value() + (axis_relative_modes[i])*current_position[i];
      seen[i]=true;
    }
    else calc_destination[i] = current_position[i]; //Are these else lines really needed?
  }
  if(code_seen('F')) {
    next_feedrate = code_value();
    if(next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

FILE *fp;

void get_command() {
while(!feof(fp) && buflen < BUFSIZE) {
	char serial_char;
	fread(&serial_char, 1, 1, fp);

    if(serial_char == '\n' || 
       serial_char == '\r' || 
       (serial_char == ':' && comment_mode == false) || 
       serial_count >= (MAX_CMD_SIZE - 1) ) 
    {
      if(!serial_count) { //if empty line
        comment_mode = false; //for new command
        return;
      }
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
      if(!comment_mode){
        comment_mode = false; //for new command
        if(strchr(cmdbuffer[bufindw], 'N') != NULL)
        {
          strchr_pointer = strchr(cmdbuffer[bufindw], 'N');
          gcode_N = (strtol(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL, 10));

          if(strchr(cmdbuffer[bufindw], '*') != NULL)
          {
            byte checksum = 0;
            byte count = 0;
            while(cmdbuffer[bufindw][count] != '*') checksum = checksum^cmdbuffer[bufindw][count++];
            strchr_pointer = strchr(cmdbuffer[bufindw], '*');
          }

          gcode_LastN = gcode_N;
          //if no errors, continue parsing
        }
        if((strchr(cmdbuffer[bufindw], 'G') != NULL)){
          strchr_pointer = strchr(cmdbuffer[bufindw], 'G');
        }
        bufindw = (bufindw + 1)%BUFSIZE;
        buflen += 1;
      }
      serial_count = 0; //clear buffer
    }
    else
    {
      if(serial_char == ';') comment_mode = true;
      if(!comment_mode) cmdbuffer[bufindw][serial_count++] = serial_char;
    }
  }
}

double total_time = 0;
int total_g=0, total_m=0;

		
void process_commands() {
  unsigned long codenum; //throw away variable

  if(code_seen('G'))
  {
	total_g++;
    switch((int)code_value())
    {
    case 0: // G0 -> G1
    case 1: // G1
      if(Stopped == false) {
        get_coordinates(); // For X Y Z E F
        prepare_move();
        //ClearToSend();
        return;
      } else printf("STOPPED!!");
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
           if(i == E_AXIS) {
             current_position[i] = code_value();  
             Planner::set_e_position_mm(current_position[E_AXIS]);
           }
           else {
             current_position[i] = code_value()+add_homing[i];  
             Planner::set_position_mm(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
           }
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
    case 201: // M201
      for(int8_t i=0; i < NUM_AXIS; i++) 
      {
        if(code_seen(axis_codes[i]))
        {
          Planner::max_acceleration_mm_per_s2[i] = code_value();
          //Planner::axis_steps_per_sqr_second[i] = code_value() * axis_steps_per_mm[i];
        }
      }
      break;
    #if 0 // Not used for Sprinter/grbl gen6
    case 202: // M202
      for(int8_t i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = code_value() * axis_steps_per_mm[i];
      }
      break;
    #endif
    case 203: // M203 max feedrate mm/sec
      for(int8_t i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) Planner::max_feedrate_mm_s[i] = code_value();
      }
      break;
    case 204: // M204 acclereration S normal moves T filmanent only moves
      {
        if(code_seen('S')) Planner::acceleration = code_value() ;
        if(code_seen('T')) Planner::retract_acceleration = code_value() ;
      }
      break;
    case 205: //M205 advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
    {
      if(code_seen('S')) Planner::min_feedrate_mm_s = code_value();
      if(code_seen('T')) Planner::min_travel_feedrate_mm_s = code_value();
      if(code_seen('B')) Planner::min_segment_time_us = code_value() ;
      if(code_seen('X')) Planner::max_jerk[X_AXIS] = code_value() ;
      if(code_seen('Y')) Planner::max_jerk[Y_AXIS] = code_value() ;
      if(code_seen('Z')) Planner::max_jerk[Z_AXIS] = code_value() ;
      if(code_seen('E')) Planner::max_jerk[E_AXIS] = code_value() ;
    }
    break;
    case 220: // M220 S<factor in percent>- set speed factor override percentage
    {
      if(code_seen('S')) 
      {
        feedmultiply = code_value() ;
      }
    }
    break;
    case 221: // M221 S<factor in percent>- set extrude factor override percentage
    {
      if(code_seen('S')) 
      {
        extrudemultiply = code_value() ;
      }
    }
    break;

   }
  }

}

int blocks = 0;

void idle2() {
  block_t *block = Planner::get_current_block();
	if (block != NULL) {
		blocks++;
		if(blocks % 100000 == 0) { fprintf(stderr, "."); }
//		if(blocks > 10) exit(0);
//		printf("S: %lu in: %lu nom: %lu out: %lu ac: %lu\n", block->step_event_count, block->initial_rate, block->nominal_rate, block->final_rate, block->acceleration_st);
		// calculate block len

		double tt1, tt2, tt3;
                if (block->step_event_count > 0) {
                  double vi, vf, v, d, a, t1, t2;
                  vi = block->initial_rate;
                  a = block->acceleration_steps_per_s2;
                  d = block->accelerate_until;
                  t1 = -1 * (sqrt(2*a*d+vi*vi) + vi) / a;
                  t2 =      (sqrt(2*a*d+vi*vi) - vi) / a;
                  //printf("D: %f A: %f T: %f / %f \n", d, a, t1, t2);
                  tt1 = max(t1, t2);
                  vf = vi + a * tt1;

                  if (block->decelerate_after > block->accelerate_until) {
                    tt2 = (block->decelerate_after - block->accelerate_until) / vf;
                  } else {
                    tt2 = 0;
                  }

                  vi = vf;
                  if (block->step_event_count > block->decelerate_after) {
                    d = block->step_event_count - block->decelerate_after;
                  } else {
                    d = 0;
                  }
                  a = -a;
                  vf = block->final_rate;
                  v = (vi+vf)/2;
                  tt3 = d / v;

                  total_time += tt1+tt2+tt3;
                }
                Planner::discard_current_block();
	}
}


int main(int argc, char *argv[]) {
	double x=0, y=0, z=0, e=0, feed_rate=0;
    uint8_t extruder = 0;
	Config_ResetDefault();
        planner.init();
        Planner::buffer_line(x,y,z,e,feed_rate,extruder);
	fp = fopen(argv[1], "r");
	while(!feof(fp)) {
		get_command();
		while(buflen) { 
			process_commands();
		    buflen = (buflen-1);
			bufindr = (bufindr + 1)%BUFSIZE;
		}

	}
	fclose(fp);
	fprintf(stderr, "Processed %d Gcodes and %d Mcodes. %d blocks\n", total_g, total_m, blocks);
	fprintf(stderr, "Total time: %f\n", total_time);
	printf("%f\n", total_time);
	return 0;
}
