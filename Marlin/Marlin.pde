/*
    Reprap firmware based on Sprinter and grbl.
 Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 This firmware is a mashup between Sprinter and grbl.
  (https://github.com/kliment/Sprinter)
  (https://github.com/simen/grbl/tree)
 
 It has preliminary support for Matthew Roberts advance algorithm 
    http://reprap.org/pipermail/reprap-dev/2011-May/003323.html
 */

#include "Marlin.h"

#include "ultralcd.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "motion_control.h"
#include "cardreader.h"
#include "watchdog.h"
#include "EEPROMwrite.h"
#include "language.h"

#define VERSION_STRING  "1.0.0 RC2"

// look here for descriptions of gcodes: http://linuxcnc.org/handbook/gcode/g-code.html
// http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes

//Implemented Codes
//-------------------
// G0  -> G1
// G1  - Coordinated Movement X Y Z E
// G2  - CW ARC
// G3  - CCW ARC
// G4  - Dwell S<seconds> or P<milliseconds>
// G28 - Home all Axis
// G90 - Use Absolute Coordinates
// G91 - Use Relative Coordinates
// G92 - Set current position to cordinates given

//RepRap M Codes
// M104 - Set extruder target temp
// M105 - Read current temp
// M106 - Fan on
// M107 - Fan off
// M109 - Wait for extruder current temp to reach target temp.
// M114 - Display current position

//Custom M Codes
// M17  - Enable/Power all stepper motors
// M18  - Disable all stepper motors; same as M84
// M20  - List SD card
// M21  - Init SD card
// M22  - Release SD card
// M23  - Select SD file (M23 filename.g)
// M24  - Start/resume SD print
// M25  - Pause SD print
// M26  - Set SD position in bytes (M26 S12345)
// M27  - Report SD print status
// M28  - Start SD write (M28 filename.g)
// M29  - Stop SD write
// M30  - Delete file from SD (M30 filename.g)
// M31  - Output time since last M109 or SD card start to serial
// M42  - Change pin status via gcode
// M80  - Turn on Power Supply
// M81  - Turn off Power Supply
// M82  - Set E codes absolute (default)
// M83  - Set E codes relative while in Absolute Coordinates (G90) mode
// M84  - Disable steppers until next move, 
//        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
// M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
// M92  - Set axis_steps_per_unit - same syntax as G92
// M114 - Output current position to serial port 
// M115	- Capabilities string
// M117 - display message
// M119 - Output Endstop status to serial port
// M140 - Set bed target temp
// M190 - Wait for bed current temp to reach target temp.
// M200 - Set filament diameter
// M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
// M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in Marlin!!
// M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
// M204 - Set default acceleration: S normal moves T filament only moves (M204 S3000 T7000) im mm/sec^2  also sets minimum segment time in ms (B20000) to prevent buffer underruns and M20 minimum feedrate
// M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
// M206 - set additional homeing offset
// M220 S<factor in percent>- set speed factor override percentage
// M221 S<factor in percent>- set extrude factor override percentage
// M240 - Trigger a camera to take a photograph
// M301 - Set PID parameters P I and D
// M302 - Allow cold extrudes
// M400 - Finish all moves
// M500 - stores paramters in EEPROM
// M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).  
// M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
// M503 - print the current settings (from memory not from eeprom)
// M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)

//Stepper Movement Variables

//===========================================================================
//=============================imported variables============================
//===========================================================================


//===========================================================================
//=============================public variables=============================
//===========================================================================
#ifdef SDSUPPORT
CardReader card;
#endif
float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
volatile int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
volatile bool feedmultiplychanged=false;
volatile int extrudemultiply=100; //100->1 200->2
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
float add_homeing[3]={0,0,0};
uint8_t active_extruder = 0;
unsigned char FanSpeed=0;

//===========================================================================
//=============================private variables=============================
//===========================================================================
const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};
static float destination[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};
static float offset[3] = {0.0, 0.0, 0.0};
static bool home_all_axis = true;
static float feedrate = 1500.0, next_feedrate, saved_feedrate;
static long gcode_N, gcode_LastN;

static bool relative_mode = false;  //Determines Absolute or Relative Coordinates
static bool relative_mode_e = false;  //Determines Absolute or Relative E Codes while in Absolute Coordinates mode. E is always relative in Relative Coordinates mode.

static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
static bool fromsd[BUFSIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;
//static int i = 0;
static char serial_char;
static int serial_count = 0;
static boolean comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

const int sensitive_pins[] = SENSITIVE_PINS; // Sensitive pin list for M42

//static float tt = 0;
//static float bt = 0;

//Inactivity shutdown variables
static unsigned long previous_millis_cmd = 0;
static unsigned long max_inactive_time = 0;
static unsigned long stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME*1000l;

static unsigned long starttime=0;
static unsigned long stoptime=0;

static uint8_t tmp_extruder;


//===========================================================================
//=============================ROUTINES=============================
//===========================================================================

void get_arc_coordinates();

extern "C"{
  extern unsigned int __bss_end;
  extern unsigned int __heap_start;
  extern void *__brkval;

  int freeMemory() {
    int free_memory;

    if((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);

    return free_memory;
  }
}

//adds an command to the main command buffer
//thats really done in a non-safe way.
//needs overworking someday
void enquecommand(const char *cmd)
{
  if(buflen < BUFSIZE)
  {
    //this is dangerous if a mixing of serial and this happsens
    strcpy(&(cmdbuffer[bufindw][0]),cmd);
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM("enqueing \"");
    SERIAL_ECHO(cmdbuffer[bufindw]);
    SERIAL_ECHOLNPGM("\"");
    bufindw= (bufindw + 1)%BUFSIZE;
    buflen += 1;
  }
}

void setup_photpin()
{
  #ifdef PHOTOGRAPH_PIN
    #if (PHOTOGRAPH_PIN > -1)
    SET_OUTPUT(PHOTOGRAPH_PIN);
    WRITE(PHOTOGRAPH_PIN, LOW);
    #endif
  #endif 
}

void setup_powerhold()
{
 #ifdef SUICIDE_PIN
   #if (SUICIDE_PIN> -1)
      SET_OUTPUT(SUICIDE_PIN);
      WRITE(SUICIDE_PIN, HIGH);
   #endif
 #endif
}

void suicide()
{
 #ifdef SUICIDE_PIN
    #if (SUICIDE_PIN> -1) 
      SET_OUTPUT(SUICIDE_PIN);
      WRITE(SUICIDE_PIN, LOW);
    #endif
  #endif
}

void setup()
{ 
  setup_powerhold();
  MYSERIAL.begin(BAUDRATE);
  SERIAL_PROTOCOLLNPGM("start");
  SERIAL_ECHO_START;

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = MCUSR;
  if(mcu & 1) SERIAL_ECHOLNPGM(MSG_POWERUP);
  if(mcu & 2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
  if(mcu & 4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
  if(mcu & 8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
  if(mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
  MCUSR=0;

  SERIAL_ECHOPGM(MSG_MARLIN);
  SERIAL_ECHOLNPGM(VERSION_STRING);
  #ifdef STRING_VERSION_CONFIG_H
    #ifdef STRING_CONFIG_H_AUTHOR
      SERIAL_ECHO_START;
      SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
      SERIAL_ECHOPGM(STRING_VERSION_CONFIG_H);
      SERIAL_ECHOPGM(MSG_AUTHOR);
      SERIAL_ECHOLNPGM(STRING_CONFIG_H_AUTHOR);
    #endif
  #endif
  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_FREE_MEMORY);
  SERIAL_ECHO(freeMemory());
  SERIAL_ECHOPGM(MSG_PLANNER_BUFFER_BYTES);
  SERIAL_ECHOLN((int)sizeof(block_t)*BLOCK_BUFFER_SIZE);
  for(int8_t i = 0; i < BUFSIZE; i++)
  {
    fromsd[i] = false;
  }
  
  EEPROM_RetrieveSettings(); // loads data from EEPROM if available

  for(int8_t i=0; i < NUM_AXIS; i++)
  {
    axis_steps_per_sqr_second[i] = max_acceleration_units_per_sq_second[i] * axis_steps_per_unit[i];
  }


  tp_init();    // Initialize temperature loop 
  plan_init();  // Initialize planner;
  st_init();    // Initialize stepper;
  wd_init();
  setup_photpin();
}


void loop()
{
  if(buflen < (BUFSIZE-1))
    get_command();
  #ifdef SDSUPPORT
  card.checkautostart(false);
  #endif
  if(buflen)
  {
    #ifdef SDSUPPORT
      if(card.saving)
      {
	if(strstr(cmdbuffer[bufindr],"M29") == NULL)
	{
	  card.write_command(cmdbuffer[bufindr]);
	  SERIAL_PROTOCOLLNPGM(MSG_OK);
	}
	else
	{
	  card.closefile();
	  SERIAL_PROTOCOLLNPGM(MSG_FILE_SAVED);
	}
      }
      else
      {
	process_commands();
      }
    #else
      process_commands();
    #endif //SDSUPPORT
    buflen = (buflen-1);
    bufindr = (bufindr + 1)%BUFSIZE;
  }
  //check heater every n milliseconds
  manage_heater();
  manage_inactivity(1);
  checkHitEndstops();
  LCD_STATUS;
}

void get_command() 
{ 
  while( MYSERIAL.available() > 0  && buflen < BUFSIZE) {
    serial_char = MYSERIAL.read();
    if(serial_char == '\n' || serial_char == '\r' || serial_char == ':' || serial_count >= (MAX_CMD_SIZE - 1) ) 
    {
      if(!serial_count) { //if empty line
        comment_mode = false; //for new command
        return;
      }
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
      if(!comment_mode){
        comment_mode = false; //for new command
        fromsd[bufindw] = false;
        if(strstr(cmdbuffer[bufindw], "N") != NULL)
        {
          strchr_pointer = strchr(cmdbuffer[bufindw], 'N');
          gcode_N = (strtol(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL, 10));
          if(gcode_N != gcode_LastN+1 && (strstr(cmdbuffer[bufindw], "M110") == NULL) ) {
            SERIAL_ERROR_START;
            SERIAL_ERRORPGM(MSG_ERR_LINE_NO);
            SERIAL_ERRORLN(gcode_LastN);
            //Serial.println(gcode_N);
            FlushSerialRequestResend();
            serial_count = 0;
            return;
          }

          if(strstr(cmdbuffer[bufindw], "*") != NULL)
          {
            byte checksum = 0;
            byte count = 0;
            while(cmdbuffer[bufindw][count] != '*') checksum = checksum^cmdbuffer[bufindw][count++];
            strchr_pointer = strchr(cmdbuffer[bufindw], '*');

            if( (int)(strtod(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL)) != checksum) {
              SERIAL_ERROR_START;
              SERIAL_ERRORPGM(MSG_ERR_CHECKSUM_MISMATCH);
              SERIAL_ERRORLN(gcode_LastN);
              FlushSerialRequestResend();
              serial_count = 0;
              return;
            }
            //if no errors, continue parsing
          }
          else 
          {
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
        else  // if we don't receive 'N' but still see '*'
        {
          if((strstr(cmdbuffer[bufindw], "*") != NULL))
          {
            SERIAL_ERROR_START;
            SERIAL_ERRORPGM(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM);
            SERIAL_ERRORLN(gcode_LastN);
            serial_count = 0;
            return;
          }
        }
        if((strstr(cmdbuffer[bufindw], "G") != NULL)){
          strchr_pointer = strchr(cmdbuffer[bufindw], 'G');
          switch((int)((strtod(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL)))){
          case 0:
          case 1:
          case 2:
          case 3:
	    #ifdef SDSUPPORT
            if(card.saving)
              break;
	    #endif //SDSUPPORT
            SERIAL_PROTOCOLLNPGM(MSG_OK); 
            break;
          default:
            break;
          }

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
  #ifdef SDSUPPORT
  if(!card.sdprinting || serial_count!=0){
    return;
  }
  while( !card.eof()  && buflen < BUFSIZE) {
    int16_t n=card.get();
    serial_char = (char)n;
    if(serial_char == '\n' || serial_char == '\r' || serial_char == ':' || serial_count >= (MAX_CMD_SIZE - 1)||n==-1) 
    {
      if(card.eof()){
        SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
        stoptime=millis();
        char time[30];
        unsigned long t=(stoptime-starttime)/1000;
        int sec,min;
        min=t/60;
        sec=t%60;
        sprintf(time,"%i min, %i sec",min,sec);
        SERIAL_ECHO_START;
        SERIAL_ECHOLN(time);
        LCD_MESSAGE(time);
        card.printingHasFinished();
        card.checkautostart(true);
        
      }
      if(!serial_count)
      {
        comment_mode = false; //for new command
        return; //if empty line
      }
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
//      if(!comment_mode){
        fromsd[bufindw] = true;
        buflen += 1;
        bufindw = (bufindw + 1)%BUFSIZE;
//      }     
      comment_mode = false; //for new command
      serial_count = 0; //clear buffer
    }
    else
    {
      if(serial_char == ';') comment_mode = true;
      if(!comment_mode) cmdbuffer[bufindw][serial_count++] = serial_char;
    }
  }
  
  #endif //SDSUPPORT

}


float code_value() 
{ 
  return (strtod(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL)); 
}

long code_value_long() 
{ 
  return (strtol(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL, 10)); 
}

bool code_seen(char code_string[]) //Return True if the string was found
{ 
  return (strstr(cmdbuffer[bufindr], code_string) != NULL); 
}  

bool code_seen(char code)
{
  strchr_pointer = strchr(cmdbuffer[bufindr], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}

#define HOMEAXIS(LETTER) \
  if ((LETTER##_MIN_PIN > -1 && LETTER##_HOME_DIR==-1) || (LETTER##_MAX_PIN > -1 && LETTER##_HOME_DIR==1))\
    { \
    current_position[LETTER##_AXIS] = 0; \
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]); \
    destination[LETTER##_AXIS] = 1.5 * LETTER##_MAX_LENGTH * LETTER##_HOME_DIR; \
    feedrate = homing_feedrate[LETTER##_AXIS]; \
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder); \
    st_synchronize();\
    \
    current_position[LETTER##_AXIS] = 0;\
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);\
    destination[LETTER##_AXIS] = -LETTER##_HOME_RETRACT_MM * LETTER##_HOME_DIR;\
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder); \
    st_synchronize();\
    \
    destination[LETTER##_AXIS] = 2*LETTER##_HOME_RETRACT_MM * LETTER##_HOME_DIR;\
    feedrate = homing_feedrate[LETTER##_AXIS]/2 ;  \
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder); \
    st_synchronize();\
    \
    current_position[LETTER##_AXIS] = (LETTER##_HOME_DIR == -1) ? LETTER##_HOME_POS : LETTER##_MAX_LENGTH;\
    destination[LETTER##_AXIS] = current_position[LETTER##_AXIS];\
    feedrate = 0.0;\
    endstops_hit_on_purpose();\
  }

void process_commands()
{
  unsigned long codenum; //throw away variable
  char *starpos = NULL;

  if(code_seen('G'))
  {
    switch((int)code_value())
    {
    case 0: // G0 -> G1
    case 1: // G1
      get_coordinates(); // For X Y Z E F
      prepare_move();
      //ClearToSend();
      return;
      //break;
    case 2: // G2  - CW ARC
      get_arc_coordinates();
      prepare_arc_move(true);
      return;
    case 3: // G3  - CCW ARC
      get_arc_coordinates();
      prepare_arc_move(false);
      return;
    case 4: // G4 dwell
      LCD_MESSAGEPGM(MSG_DWELL);
      codenum = 0;
      if(code_seen('P')) codenum = code_value(); // milliseconds to wait
      if(code_seen('S')) codenum = code_value() * 1000; // seconds to wait
      
      st_synchronize();
      codenum += millis();  // keep track of when we started waiting
      previous_millis_cmd = millis();
      while(millis()  < codenum ){
        manage_heater();
        manage_inactivity(1);
      }
      break;
    case 28: //G28 Home all Axis one at a time
      saved_feedrate = feedrate;
      saved_feedmultiply = feedmultiply;
      feedmultiply = 100;
      previous_millis_cmd = millis();
      
      enable_endstops(true);
      
      for(int8_t i=0; i < NUM_AXIS; i++) {
        destination[i] = current_position[i];
      }
      feedrate = 0.0;
      home_all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2])));
      #ifdef QUICK_HOME
      if( code_seen(axis_codes[X_AXIS]) && code_seen(axis_codes[Y_AXIS]) )  //first diagonal move
      {
        current_position[X_AXIS] = 0;current_position[Y_AXIS] = 0;  

        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]); 
        destination[X_AXIS] = 1.5 * X_MAX_LENGTH * X_HOME_DIR;destination[Y_AXIS] = 1.5 * Y_MAX_LENGTH * Y_HOME_DIR;  
        feedrate = homing_feedrate[X_AXIS]; 
        if(homing_feedrate[Y_AXIS]<feedrate)
          feedrate =homing_feedrate[Y_AXIS]; 
        plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
        st_synchronize();
    
        current_position[X_AXIS] = (X_HOME_DIR == -1) ? X_HOME_POS : X_MAX_LENGTH;
        current_position[Y_AXIS] = (Y_HOME_DIR == -1) ? Y_HOME_POS : Y_MAX_LENGTH;
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        destination[X_AXIS] = current_position[X_AXIS];
        destination[Y_AXIS] = current_position[Y_AXIS];
        plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
        feedrate = 0.0;
        st_synchronize();
        endstops_hit_on_purpose();
      }
      #endif
      
      if((home_all_axis) || (code_seen(axis_codes[X_AXIS]))) 
      {
        HOMEAXIS(X);
      }

      if((home_all_axis) || (code_seen(axis_codes[Y_AXIS]))) {
       HOMEAXIS(Y);
      }
      
      if((home_all_axis) || (code_seen(axis_codes[Z_AXIS]))) {
        HOMEAXIS(Z);
      }
      
      if(code_seen(axis_codes[X_AXIS])) 
      {
        if(code_value_long() != 0) {
          current_position[X_AXIS]=code_value()+add_homeing[0];
        }
      }

      if(code_seen(axis_codes[Y_AXIS])) {
        if(code_value_long() != 0) {
          current_position[Y_AXIS]=code_value()+add_homeing[1];
        }
      }

      if(code_seen(axis_codes[Z_AXIS])) {
        if(code_value_long() != 0) {
          current_position[Z_AXIS]=code_value()+add_homeing[2];
        }
      }
      plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      
      #ifdef ENDSTOPS_ONLY_FOR_HOMING
        enable_endstops(false);
      #endif
      
      feedrate = saved_feedrate;
      feedmultiply = saved_feedmultiply;
      previous_millis_cmd = millis();
      endstops_hit_on_purpose();
      break;
    case 90: // G90
      relative_mode = false;
      break;
    case 91: // G91
      relative_mode = true;
      break;
    case 92: // G92
      if(!code_seen(axis_codes[E_AXIS]))
        st_synchronize();
      for(int8_t i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) { 
           current_position[i] = code_value()+add_homeing[i];  
           if(i == E_AXIS) {
             current_position[i] = code_value();  
             plan_set_e_position(current_position[E_AXIS]);
           }
           else {
             current_position[i] = code_value()+add_homeing[i];  
             plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
           }
        }
      }
      break;
    }
  }

  else if(code_seen('M'))
  {
    switch( (int)code_value() ) 
    {
    case 17:
        LCD_MESSAGEPGM(MSG_NO_MOVE);
        enable_x(); 
        enable_y(); 
        enable_z(); 
        enable_e0(); 
        enable_e1(); 
        enable_e2(); 
      break;

#ifdef SDSUPPORT
    case 20: // M20 - list SD card
      SERIAL_PROTOCOLLNPGM(MSG_BEGIN_FILE_LIST);
      card.ls();
      SERIAL_PROTOCOLLNPGM(MSG_END_FILE_LIST);
      break;
    case 21: // M21 - init SD card
      
      card.initsd();
      
      break;
    case 22: //M22 - release SD card
      card.release();

      break;
    case 23: //M23 - Select file
      starpos = (strchr(strchr_pointer + 4,'*'));
      if(starpos!=NULL)
        *(starpos-1)='\0';
      card.openFile(strchr_pointer + 4,true);
      break;
    case 24: //M24 - Start SD print
      card.startFileprint();
      starttime=millis();
      break;
    case 25: //M25 - Pause SD print
      card.pauseSDPrint();
      break;
    case 26: //M26 - Set SD index
      if(card.cardOK && code_seen('S')) {
        card.setIndex(code_value_long());
      }
      break;
    case 27: //M27 - Get SD status
      card.getStatus();
      break;
    case 28: //M28 - Start SD write
      starpos = (strchr(strchr_pointer + 4,'*'));
      if(starpos != NULL){
        char* npos = strchr(cmdbuffer[bufindr], 'N');
        strchr_pointer = strchr(npos,' ') + 1;
        *(starpos-1) = '\0';
      }
      card.openFile(strchr_pointer+4,false);
      break;
    case 29: //M29 - Stop SD write
      //processed in write to file routine above
      //card,saving = false;
      break;
    case 30: //M30 <filename> Delete File 
	if (card.cardOK){
		card.closefile();
		starpos = (strchr(strchr_pointer + 4,'*'));
                if(starpos != NULL){
                char* npos = strchr(cmdbuffer[bufindr], 'N');
                strchr_pointer = strchr(npos,' ') + 1;
                *(starpos-1) = '\0';
         }
	 card.removeFile(strchr_pointer + 4);
	}
	break;
	
#endif //SDSUPPORT

    case 31: //M31 take time since the start of the SD print or an M109 command
      {
      stoptime=millis();
      char time[30];
      unsigned long t=(stoptime-starttime)/1000;
      int sec,min;
      min=t/60;
      sec=t%60;
      sprintf(time,"%i min, %i sec",min,sec);
      SERIAL_ECHO_START;
      SERIAL_ECHOLN(time);
      LCD_MESSAGE(time);
      autotempShutdown();
      }
      break;
    case 42: //M42 -Change pin status via gcode
      if (code_seen('S'))
      {
        int pin_status = code_value();
        if (code_seen('P') && pin_status >= 0 && pin_status <= 255)
        {
          int pin_number = code_value();
          for(int8_t i = 0; i < (int8_t)sizeof(sensitive_pins); i++)
          {
            if (sensitive_pins[i] == pin_number)
            {
              pin_number = -1;
              break;
            }
          }
          
          if (pin_number > -1)
          {              
            pinMode(pin_number, OUTPUT);
            digitalWrite(pin_number, pin_status);
            analogWrite(pin_number, pin_status);
          }
        }
      }
     break;
    case 104: // M104
      tmp_extruder = active_extruder;
      if(code_seen('T')) {
        tmp_extruder = code_value();
        if(tmp_extruder >= EXTRUDERS) {
          SERIAL_ECHO_START;
          SERIAL_ECHO(MSG_M104_INVALID_EXTRUDER);
          SERIAL_ECHOLN(tmp_extruder);
          break;
        }
      }
      if (code_seen('S')) setTargetHotend(code_value(), tmp_extruder);
      setWatch();
      break;
    case 140: // M140 set bed temp
      if (code_seen('S')) setTargetBed(code_value());
      break;
    case 105 : // M105
      tmp_extruder = active_extruder;
      if(code_seen('T')) {
        tmp_extruder = code_value();
        if(tmp_extruder >= EXTRUDERS) {
          SERIAL_ECHO_START;
          SERIAL_ECHO(MSG_M105_INVALID_EXTRUDER);
          SERIAL_ECHOLN(tmp_extruder);
          break;
        }
      }
      #if (TEMP_0_PIN > -1)
        SERIAL_PROTOCOLPGM("ok T:");
        SERIAL_PROTOCOL(degHotend(tmp_extruder)); 
        #if TEMP_BED_PIN > -1
          SERIAL_PROTOCOLPGM(" B:");  
          SERIAL_PROTOCOL(degBed());
        #endif //TEMP_BED_PIN
      #else
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM(MSG_ERR_NO_THERMISTORS);
      #endif
      #ifdef PIDTEMP
        SERIAL_PROTOCOLPGM(" @:");
        SERIAL_PROTOCOL(getHeaterPower(tmp_extruder));  
      #endif
        SERIAL_PROTOCOLLN("");
      return;
      break;
    case 109: 
    {// M109 - Wait for extruder heater to reach target.
      tmp_extruder = active_extruder;
      if(code_seen('T')) {
        tmp_extruder = code_value();
        if(tmp_extruder >= EXTRUDERS) {
          SERIAL_ECHO_START;
          SERIAL_ECHO(MSG_M109_INVALID_EXTRUDER);
          SERIAL_ECHOLN(tmp_extruder);
          break;
        }
      }
      LCD_MESSAGEPGM(MSG_HEATING);   
      #ifdef AUTOTEMP
        autotemp_enabled=false;
      #endif
      if (code_seen('S')) setTargetHotend(code_value(), tmp_extruder);
      #ifdef AUTOTEMP
        if (code_seen('S')) autotemp_min=code_value();
        if (code_seen('G')) autotemp_max=code_value();
        if (code_seen('F')) 
        {
          autotemp_factor=code_value();
          autotemp_enabled=true;
        }
      #endif
      
      setWatch();
      codenum = millis(); 

      /* See if we are heating up or cooling down */
      bool target_direction = isHeatingHotend(tmp_extruder); // true if heating, false if cooling

      #ifdef TEMP_RESIDENCY_TIME
        long residencyStart;
        residencyStart = -1;
        /* continue to loop until we have reached the target temp   
          _and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
        while((residencyStart == -1) ||
              (residencyStart >= 0 && (((unsigned int) (millis() - residencyStart)) < (TEMP_RESIDENCY_TIME * 1000UL))) ) {
      #else
        while ( target_direction ? (isHeatingHotend(tmp_extruder)) : (isCoolingHotend(tmp_extruder)&&(CooldownNoWait==false)) ) {
      #endif //TEMP_RESIDENCY_TIME
          if( (millis() - codenum) > 1000UL )
          { //Print Temp Reading and remaining time every 1 second while heating up/cooling down
            SERIAL_PROTOCOLPGM("T:");
            SERIAL_PROTOCOL( degHotend(tmp_extruder) ); 
            SERIAL_PROTOCOLPGM(" E:");
            SERIAL_PROTOCOL( (int)tmp_extruder ); 
            #ifdef TEMP_RESIDENCY_TIME
              SERIAL_PROTOCOLPGM(" W:");
              if(residencyStart > -1)
              {
                 codenum = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residencyStart)) / 1000UL;
                 SERIAL_PROTOCOLLN( codenum );
              }
              else 
              {
                 SERIAL_PROTOCOLLN( "?" );
              }
            #else
              SERIAL_PROTOCOLLN("");
            #endif
            codenum = millis();
          }
          manage_heater();
          manage_inactivity(1);
          LCD_STATUS;
        #ifdef TEMP_RESIDENCY_TIME
            /* start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
              or when current temp falls outside the hysteresis after target temp was reached */
          if ((residencyStart == -1 &&  target_direction && !isHeatingHotend(tmp_extruder)) ||
              (residencyStart == -1 && !target_direction && !isCoolingHotend(tmp_extruder)) ||
              (residencyStart > -1 && labs(degHotend(tmp_extruder) - degTargetHotend(tmp_extruder)) > TEMP_HYSTERESIS) ) 
          {
            residencyStart = millis();
          }
        #endif //TEMP_RESIDENCY_TIME
        }
        LCD_MESSAGEPGM(MSG_HEATING_COMPLETE);
        starttime=millis();
        previous_millis_cmd = millis();
      }
      break;
    case 190: // M190 - Wait for bed heater to reach target.
    #if TEMP_BED_PIN > -1
        LCD_MESSAGEPGM(MSG_BED_HEATING);
        if (code_seen('S')) setTargetBed(code_value());
        codenum = millis(); 
        while(isHeatingBed()) 
        {
          if(( millis() - codenum) > 1000 ) //Print Temp Reading every 1 second while heating up.
          {
            float tt=degHotend(active_extruder);
            SERIAL_PROTOCOLPGM("T:");
            SERIAL_PROTOCOL(tt);
            SERIAL_PROTOCOLPGM(" E:");
            SERIAL_PROTOCOL( (int)active_extruder ); 
            SERIAL_PROTOCOLPGM(" B:");
            SERIAL_PROTOCOLLN(degBed()); 
            codenum = millis(); 
          }
          manage_heater();
          manage_inactivity(1);
          LCD_STATUS;
        }
        LCD_MESSAGEPGM(MSG_BED_DONE);
        previous_millis_cmd = millis();
    #endif
        break;

    #if FAN_PIN > -1
      case 106: //M106 Fan On
        if (code_seen('S')){
           FanSpeed=constrain(code_value(),0,255);
        }
        else {
          FanSpeed=255;			
        }
        break;
      case 107: //M107 Fan Off
        FanSpeed = 0;
        break;
    #endif //FAN_PIN

    #if (PS_ON_PIN > -1)
      case 80: // M80 - ATX Power On
        SET_OUTPUT(PS_ON_PIN); //GND
        break;
      #endif
      
      case 81: // M81 - ATX Power Off
      
      #if defined SUICIDE_PIN && SUICIDE_PIN > -1
        st_synchronize();
        suicide();
      #elif (PS_ON_PIN > -1)
        SET_INPUT(PS_ON_PIN); //Floating
      #endif
		break;
        
    case 82:
      axis_relative_modes[3] = false;
      break;
    case 83:
      axis_relative_modes[3] = true;
      break;
    case 18: //compatibility
    case 84: // M84
      if(code_seen('S')){ 
        stepper_inactive_time = code_value() * 1000; 
      }
      else
      { 
        bool all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2]))|| (code_seen(axis_codes[3])));
        if(all_axis)
        {
          st_synchronize();
          disable_e0();
          disable_e1();
          disable_e2();
          finishAndDisableSteppers();
        }
        else
        {
          st_synchronize();
          if(code_seen('X')) disable_x();
          if(code_seen('Y')) disable_y();
          if(code_seen('Z')) disable_z();
          #if ((E0_ENABLE_PIN != X_ENABLE_PIN) && (E1_ENABLE_PIN != Y_ENABLE_PIN)) // Only enable on boards that have seperate ENABLE_PINS
            if(code_seen('E')) {
              disable_e0();
              disable_e1();
              disable_e2();
            }
          #endif 
          LCD_MESSAGEPGM(MSG_PART_RELEASE);
        }
      }
      break;
    case 85: // M85
      code_seen('S');
      max_inactive_time = code_value() * 1000; 
      break;
    case 92: // M92
      for(int8_t i=0; i < NUM_AXIS; i++) 
      {
        if(code_seen(axis_codes[i])) 
          axis_steps_per_unit[i] = code_value();
      }
      break;
    case 115: // M115
      SerialprintPGM(MSG_M115_REPORT);
      break;
    case 117: // M117 display message
      LCD_MESSAGE(cmdbuffer[bufindr]+5);
      break;
    case 114: // M114
      SERIAL_PROTOCOLPGM("X:");
      SERIAL_PROTOCOL(current_position[X_AXIS]);
      SERIAL_PROTOCOLPGM("Y:");
      SERIAL_PROTOCOL(current_position[Y_AXIS]);
      SERIAL_PROTOCOLPGM("Z:");
      SERIAL_PROTOCOL(current_position[Z_AXIS]);
      SERIAL_PROTOCOLPGM("E:");      
      SERIAL_PROTOCOL(current_position[E_AXIS]);
      
      SERIAL_PROTOCOLPGM(MSG_COUNT_X);
      SERIAL_PROTOCOL(float(st_get_position(X_AXIS))/axis_steps_per_unit[X_AXIS]);
      SERIAL_PROTOCOLPGM("Y:");
      SERIAL_PROTOCOL(float(st_get_position(Y_AXIS))/axis_steps_per_unit[Y_AXIS]);
      SERIAL_PROTOCOLPGM("Z:");
      SERIAL_PROTOCOL(float(st_get_position(Z_AXIS))/axis_steps_per_unit[Z_AXIS]);
      
      SERIAL_PROTOCOLLN("");
      break;
    case 119: // M119
      #if (X_MIN_PIN > -1)
        SERIAL_PROTOCOLPGM(MSG_X_MIN);
        SERIAL_PROTOCOL(((READ(X_MIN_PIN)^X_ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (X_MAX_PIN > -1)
        SERIAL_PROTOCOLPGM(MSG_X_MAX);
        SERIAL_PROTOCOL(((READ(X_MAX_PIN)^X_ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Y_MIN_PIN > -1)
        SERIAL_PROTOCOLPGM(MSG_Y_MIN);
        SERIAL_PROTOCOL(((READ(Y_MIN_PIN)^Y_ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Y_MAX_PIN > -1)
        SERIAL_PROTOCOLPGM(MSG_Y_MAX);
        SERIAL_PROTOCOL(((READ(Y_MAX_PIN)^Y_ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Z_MIN_PIN > -1)
        SERIAL_PROTOCOLPGM(MSG_Z_MIN);
        SERIAL_PROTOCOL(((READ(Z_MIN_PIN)^Z_ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Z_MAX_PIN > -1)
        SERIAL_PROTOCOLPGM(MSG_Z_MAX);
        SERIAL_PROTOCOL(((READ(Z_MAX_PIN)^Z_ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      SERIAL_PROTOCOLLN("");
      break;
      //TODO: update for all axis, use for loop
    case 201: // M201
      for(int8_t i=0; i < NUM_AXIS; i++) 
      {
        if(code_seen(axis_codes[i]))
        {
          max_acceleration_units_per_sq_second[i] = code_value();
          axis_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
        }
      }
      break;
    #if 0 // Not used for Sprinter/grbl gen6
    case 202: // M202
      for(int8_t i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
      }
      break;
    #endif
    case 203: // M203 max feedrate mm/sec
      for(int8_t i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) max_feedrate[i] = code_value();
      }
      break;
    case 204: // M204 acclereration S normal moves T filmanent only moves
      {
        if(code_seen('S')) acceleration = code_value() ;
        if(code_seen('T')) retract_acceleration = code_value() ;
      }
      break;
    case 205: //M205 advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
    {
      if(code_seen('S')) minimumfeedrate = code_value();
      if(code_seen('T')) mintravelfeedrate = code_value();
      if(code_seen('B')) minsegmenttime = code_value() ;
      if(code_seen('X')) max_xy_jerk = code_value() ;
      if(code_seen('Z')) max_z_jerk = code_value() ;
      if(code_seen('E')) max_e_jerk = code_value() ;
    }
    break;
    case 206: // M206 additional homeing offset
      for(int8_t i=0; i < 3; i++) 
      {
        if(code_seen(axis_codes[i])) add_homeing[i] = code_value();
      }
      break;
    case 220: // M220 S<factor in percent>- set speed factor override percentage
    {
      if(code_seen('S')) 
      {
        feedmultiply = code_value() ;
        feedmultiplychanged=true;
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

    #ifdef PIDTEMP
    case 301: // M301
      {
        if(code_seen('P')) Kp = code_value();
        if(code_seen('I')) Ki = code_value()*PID_dT;
        if(code_seen('D')) Kd = code_value()/PID_dT;
        #ifdef PID_ADD_EXTRUSION_RATE
        if(code_seen('C')) Kc = code_value();
        #endif
        updatePID();
        SERIAL_PROTOCOL(MSG_OK);
		SERIAL_PROTOCOL(" p:");
        SERIAL_PROTOCOL(Kp);
        SERIAL_PROTOCOL(" i:");
        SERIAL_PROTOCOL(Ki/PID_dT);
        SERIAL_PROTOCOL(" d:");
        SERIAL_PROTOCOL(Kd*PID_dT);
        #ifdef PID_ADD_EXTRUSION_RATE
        SERIAL_PROTOCOL(" c:");
        SERIAL_PROTOCOL(Kc*PID_dT);
        #endif
        SERIAL_PROTOCOLLN("");
      }
      break;
    #endif //PIDTEMP
    case 240: // M240  Triggers a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
     {
      #ifdef PHOTOGRAPH_PIN
        #if (PHOTOGRAPH_PIN > -1)
        const uint8_t NUM_PULSES=16;
        const float PULSE_LENGTH=0.01524;
        for(int i=0; i < NUM_PULSES; i++) {
          WRITE(PHOTOGRAPH_PIN, HIGH);
          _delay_ms(PULSE_LENGTH);
          WRITE(PHOTOGRAPH_PIN, LOW);
          _delay_ms(PULSE_LENGTH);
        }
        delay(7.33);
        for(int i=0; i < NUM_PULSES; i++) {
          WRITE(PHOTOGRAPH_PIN, HIGH);
          _delay_ms(PULSE_LENGTH);
          WRITE(PHOTOGRAPH_PIN, LOW);
          _delay_ms(PULSE_LENGTH);
        }
        #endif
      #endif
     }
    break;
      
    case 302: // finish all moves
    {
      allow_cold_extrudes(true);
    }
    break;
    case 303: // M303 PID autotune
    {
      float temp = 150.0;
      if (code_seen('S')) temp=code_value();
      PID_autotune(temp);
    }
    break;
    case 400: // finish all moves
    {
      st_synchronize();
    }
    break;
    case 500: // Store settings in EEPROM
    {
        EEPROM_StoreSettings();
    }
    break;
    case 501: // Read settings from EEPROM
    {
      EEPROM_RetrieveSettings();
    }
    break;
    case 502: // Revert to default settings
    {
      EEPROM_RetrieveSettings(true);
    }
    break;
    case 503: // print settings currently in memory
    {
      EEPROM_printSettings();
    }
    break;

    }
  }

  else if(code_seen('T')) 
  {
    tmp_extruder = code_value();
    if(tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      SERIAL_ECHO("T");
      SERIAL_ECHO(tmp_extruder);
      SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
    }
    else {
      active_extruder = tmp_extruder;
      SERIAL_ECHO_START;
      SERIAL_ECHO(MSG_ACTIVE_EXTRUDER);
      SERIAL_PROTOCOLLN((int)active_extruder);
    }
  }

  else
  {
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
    SERIAL_ECHO(cmdbuffer[bufindr]);
    SERIAL_ECHOLNPGM("\"");
  }

  ClearToSend();
}

void FlushSerialRequestResend()
{
  //char cmdbuffer[bufindr][100]="Resend:";
  MYSERIAL.flush();
  SERIAL_PROTOCOLPGM(MSG_RESEND);
  SERIAL_PROTOCOLLN(gcode_LastN + 1);
  ClearToSend();
}

void ClearToSend()
{
  previous_millis_cmd = millis();
  #ifdef SDSUPPORT
  if(fromsd[bufindr])
    return;
  #endif //SDSUPPORT
  SERIAL_PROTOCOLLNPGM(MSG_OK); 
}

void get_coordinates()
{
  for(int8_t i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i])) destination[i] = (float)code_value() + (axis_relative_modes[i] || relative_mode)*current_position[i];
    else destination[i] = current_position[i]; //Are these else lines really needed?
  }
  if(code_seen('F')) {
    next_feedrate = code_value();
    if(next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

void get_arc_coordinates()
{
   get_coordinates();
   if(code_seen('I')) offset[0] = code_value();
   if(code_seen('J')) offset[1] = code_value();
}

void prepare_move()
{
  if (min_software_endstops) {
    if (destination[X_AXIS] < X_HOME_POS) destination[X_AXIS] = X_HOME_POS;
    if (destination[Y_AXIS] < Y_HOME_POS) destination[Y_AXIS] = Y_HOME_POS;
    if (destination[Z_AXIS] < Z_HOME_POS) destination[Z_AXIS] = Z_HOME_POS;
  }

  if (max_software_endstops) {
    if (destination[X_AXIS] > X_MAX_LENGTH) destination[X_AXIS] = X_MAX_LENGTH;
    if (destination[Y_AXIS] > Y_MAX_LENGTH) destination[Y_AXIS] = Y_MAX_LENGTH;
    if (destination[Z_AXIS] > Z_MAX_LENGTH) destination[Z_AXIS] = Z_MAX_LENGTH;
  }
  previous_millis_cmd = millis();  
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60/100.0, active_extruder);
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}

void prepare_arc_move(char isclockwise) {
  float r = hypot(offset[X_AXIS], offset[Y_AXIS]); // Compute arc radius for mc_arc

  // Trace the arc
  mc_arc(current_position, destination, offset, X_AXIS, Y_AXIS, Z_AXIS, feedrate*feedmultiply/60/100.0, r, isclockwise, active_extruder);
  
  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
  previous_millis_cmd = millis();
}

#ifdef CONTROLLERFAN_PIN
unsigned long lastMotor = 0; //Save the time for when a motor was turned on last
unsigned long lastMotorCheck = 0;

void controllerFan()
{
  if ((millis() - lastMotorCheck) >= 2500) //Not a time critical function, so we only check every 2500ms
  {
    lastMotorCheck = millis();
    
    if(!READ(X_ENABLE_PIN) || !READ(Y_ENABLE_PIN) || !READ(Z_ENABLE_PIN)
    #if EXTRUDERS > 2
       || !READ(E2_ENABLE_PIN)
    #endif
    #if EXTRUDER > 1
       || !READ(E2_ENABLE_PIN)
    #endif
       || !READ(E0_ENABLE_PIN)) //If any of the drivers are enabled...    
    {
      lastMotor = millis(); //... set time to NOW so the fan will turn on
    }
    
    if ((millis() - lastMotor) >= (CONTROLLERFAN_SEC*1000UL) || lastMotor == 0) //If the last time any driver was enabled, is longer since than CONTROLLERSEC...   
    {
      WRITE(CONTROLLERFAN_PIN, LOW); //... turn the fan off
    }
    else
    {
      WRITE(CONTROLLERFAN_PIN, HIGH); //... turn the fan on
    }
  }
}
#endif

void manage_inactivity(byte debug) 
{ 
  if( (millis() - previous_millis_cmd) >  max_inactive_time ) 
    if(max_inactive_time) 
      kill(); 
  if(stepper_inactive_time)  {
    if( (millis() - previous_millis_cmd) >  stepper_inactive_time ) 
    {
      if(blocks_queued() == false) {
        disable_x();
        disable_y();
        disable_z();
        disable_e0();
        disable_e1();
        disable_e2();
      }
    }
  }
  #ifdef CONTROLLERFAN_PIN
    controllerFan(); //Check if fan should be turned on to cool stepper drivers down
  #endif
  #ifdef EXTRUDER_RUNOUT_PREVENT
    if( (millis() - previous_millis_cmd) >  EXTRUDER_RUNOUT_SECONDS*1000 ) 
    if(degHotend(active_extruder)>EXTRUDER_RUNOUT_MINTEMP)
    {
     bool oldstatus=READ(E0_ENABLE_PIN);
     enable_e0();
     float oldepos=current_position[E_AXIS];
     float oldedes=destination[E_AXIS];
     plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], 
                      current_position[E_AXIS]+EXTRUDER_RUNOUT_EXTRUDE*EXTRUDER_RUNOUT_ESTEPS/axis_steps_per_unit[E_AXIS], 
                      EXTRUDER_RUNOUT_SPEED/60.*EXTRUDER_RUNOUT_ESTEPS/axis_steps_per_unit[E_AXIS], active_extruder);
     current_position[E_AXIS]=oldepos;
     destination[E_AXIS]=oldedes;
     plan_set_e_position(oldepos);
     previous_millis_cmd=millis();
     st_synchronize();
     WRITE(E0_ENABLE_PIN,oldstatus);
    }
  #endif
  check_axes_activity();
}

void kill()
{
  cli(); // Stop interrupts
  disable_heater();

  disable_x();
  disable_y();
  disable_z();
  disable_e0();
  disable_e1();
  disable_e2();
  
  if(PS_ON_PIN > -1) pinMode(PS_ON_PIN,INPUT);
  SERIAL_ERROR_START;
  SERIAL_ERRORLNPGM(MSG_ERR_KILLED);
  LCD_MESSAGEPGM(MSG_KILLED);
  suicide();
  while(1); // Wait for reset
}


