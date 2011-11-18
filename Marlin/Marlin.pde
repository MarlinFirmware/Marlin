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

#include <EEPROM.h>
#include "EEPROMwrite.h"
#include "fastio.h"
#include "Configuration.h"
#include "pins.h"
#include "Marlin.h"
#include "ultralcd.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "motion_control.h"
#include "cardreader.h"


#define VERSION_STRING  "1.0.0 Alpha 1"




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
// M30  - Output time since last M109 or SD card start to serial
// M42  - Change pin status via gcode
// M80  - Turn on Power Supply
// M81  - Turn off Power Supply
// M82  - Set E codes absolute (default)
// M83  - Set E codes relative while in Absolute Coordinates (G90) mode
// M84  - Disable steppers until next move, 
//        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
// M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
// M92  - Set axis_steps_per_unit - same syntax as G92
// M115	- Capabilities string
// M140 - Set bed target temp
// M190 - Wait for bed current temp to reach target temp.
// M200 - Set filament diameter
// M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
// M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in Marlin!!
// M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
// M204 - Set default acceleration: S normal moves T filament only moves (M204 S3000 T7000) im mm/sec^2  also sets minimum segment time in ms (B20000) to prevent buffer underruns and M20 minimum feedrate
// M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
// M220 - set speed factor override percentage S:factor in percent
// M301 - Set PID parameters P I and D
// M400 - Finish all moves
// M500 - stores paramters in EEPROM
// M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).  
// M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.

//Stepper Movement Variables

//===========================================================================
//=============================imported variables============================
//===========================================================================
extern float HeaterPower;


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
float current_position[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};


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

static uint8_t fanpwm=0;



static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
static bool fromsd[BUFSIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;
static int i = 0;
static char serial_char;
static int serial_count = 0;
static boolean comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

const int sensitive_pins[] = SENSITIVE_PINS; // Sensitive pin list for M42

static float tt = 0, bt = 0;

//Inactivity shutdown variables
static unsigned long previous_millis_cmd = 0;
static unsigned long max_inactive_time = 0;
static unsigned long stepper_inactive_time = 0;

static unsigned long starttime=0;
static unsigned long stoptime=0;



//===========================================================================
//=============================ROUTINES=============================
//===========================================================================


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

void setup()
{ 
  Serial.begin(BAUDRATE);
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM(VERSION_STRING);
  SERIAL_PROTOCOLLNPGM("start");
  SERIAL_ECHO_START;
  SERIAL_ECHOPGM("Free Memory:");
  SERIAL_ECHOLN(freeMemory());
  for(int8_t i = 0; i < BUFSIZE; i++)
  {
    fromsd[i] = false;
  }
  
  RetrieveSettings(); // loads data from EEPROM if available

  for(int8_t i=0; i < NUM_AXIS; i++)
  {
    axis_steps_per_sqr_second[i] = max_acceleration_units_per_sq_second[i] * axis_steps_per_unit[i];
  }

  tp_init();    // Initialize temperature loop 
  plan_init();  // Initialize planner;
  st_init();    // Initialize stepper;
}


void loop()
{
  if(buflen<3)
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
	  SERIAL_PROTOCOLLNPGM("ok");
	}
	else
	{
	  card.closefile();
	  SERIAL_PROTOCOLLNPGM("Done saving file.");
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


inline void get_command() 
{ 
  while( Serial.available() > 0  && buflen < BUFSIZE) {
    serial_char = Serial.read();
    if(serial_char == '\n' || serial_char == '\r' || serial_char == ':' || serial_count >= (MAX_CMD_SIZE - 1) ) 
    {
      if(!serial_count) return; //if empty line
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
      if(!comment_mode){
        fromsd[bufindw] = false;
        if(strstr(cmdbuffer[bufindw], "N") != NULL)
        {
          strchr_pointer = strchr(cmdbuffer[bufindw], 'N');
          gcode_N = (strtol(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL, 10));
          if(gcode_N != gcode_LastN+1 && (strstr(cmdbuffer[bufindw], "M110") == NULL) ) {
            SERIAL_ERROR_START;
            SERIAL_ERRORPGM("Line Number is not Last Line Number+1, Last Line:");
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
              SERIAL_ERRORPGM("checksum mismatch, Last Line:");
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
            SERIAL_ERRORPGM("No Checksum with line number, Last Line:");
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
            SERIAL_ERRORPGM("No Line Number with checksum, Last Line:");
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
            SERIAL_PROTOCOLLNPGM("ok"); 
            break;
          default:
            break;
          }

        }
        bufindw = (bufindw + 1)%BUFSIZE;
        buflen += 1;

      }
      comment_mode = false; //for new command
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
        card.sdprinting = false;
        SERIAL_PROTOCOLLNPGM("Done printing file");
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
        card.checkautostart(true);
      }
      if(!serial_count) 
	return; //if empty line
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
      if(!comment_mode){
        fromsd[bufindw] = true;
        buflen += 1;
        bufindw = (bufindw + 1)%BUFSIZE;
      }
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


inline float code_value() 
{ 
  return (strtod(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL)); 
}
inline long code_value_long() 
{ 
  return (strtol(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL, 10)); 
}
inline bool code_seen(char code_string[]) //Return True if the string was found
{ 
  return (strstr(cmdbuffer[bufindr], code_string) != NULL); 
}  

inline bool code_seen(char code)
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
    prepare_move(); \
    st_synchronize();\
    \
    current_position[LETTER##_AXIS] = 0;\
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);\
    destination[LETTER##_AXIS] = -5 * LETTER##_HOME_DIR;\
    prepare_move(); \
    st_synchronize();\
    \
    destination[LETTER##_AXIS] = 10 * LETTER##_HOME_DIR;\
    feedrate = homing_feedrate[LETTER##_AXIS]/2 ;  \
    prepare_move(); \
    st_synchronize();\
    \
    current_position[LETTER##_AXIS] = (LETTER##_HOME_DIR == -1) ? 0 : LETTER##_MAX_LENGTH;\
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);\
    destination[LETTER##_AXIS] = current_position[LETTER##_AXIS];\
    feedrate = 0.0;\
    st_synchronize();\
    endstops_hit_on_purpose();\
  }

inline void process_commands()
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
      previous_millis_cmd = millis();
      //ClearToSend();
      return;
      //break;
    case 2: // G2  - CW ARC
      get_arc_coordinates();
      prepare_arc_move(true);
      previous_millis_cmd = millis();
      return;
    case 3: // G3  - CCW ARC
      get_arc_coordinates();
      prepare_arc_move(false);
      previous_millis_cmd = millis();
      return;
    case 4: // G4 dwell
      LCD_MESSAGEPGM("DWELL...");
      codenum = 0;
      if(code_seen('P')) codenum = code_value(); // milliseconds to wait
      if(code_seen('S')) codenum = code_value() * 1000; // seconds to wait
      codenum += millis();  // keep track of when we started waiting
      while(millis()  < codenum ){
        manage_heater();
      }
      break;
    case 28: //G28 Home all Axis one at a time
      saved_feedrate = feedrate;
      saved_feedmultiply = feedmultiply;
      feedmultiply = 100;
      
      for(int8_t i=0; i < NUM_AXIS; i++) {
        destination[i] = current_position[i];
      }
      feedrate = 0.0;
      home_all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2])));

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
        if(code_seen(axis_codes[i])) current_position[i] = code_value();  
      }
      plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      break;
    }
  }

  else if(code_seen('M'))
  {

    switch( (int)code_value() ) 
    {
    #ifdef SDSUPPORT

    case 20: // M20 - list SD card
      SERIAL_PROTOCOLLNPGM("Begin file list");
      card.ls();
      SERIAL_PROTOCOLLNPGM("End file list");
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
      card.selectFile(strchr_pointer + 4);
      break;
    case 24: //M24 - Start SD print
      card.startFileprint();
      starttime=millis();
      break;
    case 25: //M25 - Pause SD print
      card.pauseSDPrint();
      break;
    case 26: //M26 - Set SD index
      if(card.cardOK && code_seen('S')){
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
      card.startFilewrite(strchr_pointer+4);
      
      break;
    case 29: //M29 - Stop SD write
      //processed in write to file routine above
      //card,saving = false;
      break;
    #endif //SDSUPPORT

    case 30: //M30 take time since the start of the SD print or an M109 command
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
      if (code_seen('S')) setTargetHotend0(code_value());
      setWatch();
      break;
    case 140: // M140 set bed temp
      if (code_seen('S')) setTargetBed(code_value());
      break;
    case 105: // M105
      //SERIAL_ECHOLN(freeMemory());
          
      #if (TEMP_0_PIN > -1) || defined (HEATER_USES_AD595)
        SERIAL_PROTOCOLPGM("ok T:");
        SERIAL_PROTOCOL( degHotend0()); 
        #if TEMP_1_PIN > -1 
          SERIAL_PROTOCOLPGM(" B:");  
          SERIAL_PROTOCOL(degBed());
        #endif //TEMP_1_PIN
      #else
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM("No thermistors - no temp");
      #endif
      #ifdef PIDTEMP
        SERIAL_PROTOCOLPGM(" @:");
        SERIAL_PROTOCOL( HeaterPower); 
       
      #endif
        SERIAL_PROTOCOLLN("");
      return;
      break;
    case 109: 
    {// M109 - Wait for extruder heater to reach target.
        LCD_MESSAGEPGM("Heating...");
        if (code_seen('S')) setTargetHotend0(code_value());
        
        setWatch();
        codenum = millis(); 

        /* See if we are heating up or cooling down */
        bool target_direction = isHeatingHotend0(); // true if heating, false if cooling

        #ifdef TEMP_RESIDENCY_TIME
          long residencyStart;
          residencyStart = -1;
          /* continue to loop until we have reached the target temp   
            _and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
          while((target_direction ? (isHeatingHotend0()) : (isCoolingHotend0()) ||
                  (residencyStart > -1 && (millis() - residencyStart) < TEMP_RESIDENCY_TIME*1000) ) {
        #else
          while ( target_direction ? (isHeatingHotend0()) : (isCoolingHotend0()) ) {
        #endif //TEMP_RESIDENCY_TIME
        if( (millis() - codenum) > 1000 ) 
        { //Print Temp Reading every 1 second while heating up/cooling down
          SERIAL_PROTOCOLPGM("T:");
          SERIAL_PROTOCOLLN( degHotend0() ); 
          codenum = millis();
        }
        manage_heater();
        LCD_STATUS;
        #ifdef TEMP_RESIDENCY_TIME
            /* start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
              or when current temp falls outside the hysteresis after target temp was reached */
          if ((residencyStart == -1 &&  target_direction && !isHeatingHotend0()) ||
              (residencyStart == -1 && !target_direction && !isCoolingHotend0()) ||
              (residencyStart > -1 && labs(degHotend0() - degTargetHotend0()) > TEMP_HYSTERESIS) ) 
          {
            residencyStart = millis();
          }
        #endif //TEMP_RESIDENCY_TIME
        }
        LCD_MESSAGEPGM("Heating done.");
        starttime=millis();
      }
      break;
    case 190: // M190 - Wait bed for heater to reach target.
    #if TEMP_1_PIN > -1
        LCD_MESSAGEPGM("Bed Heating.");
        if (code_seen('S')) setTargetBed(code_value());
        codenum = millis(); 
        while(isHeatingBed()) 
        {
          if( (millis()-codenum) > 1000 ) //Print Temp Reading every 1 second while heating up.
          {
            float tt=degHotend0();
            SERIAL_PROTOCOLPGM("T:");
            SERIAL_PROTOCOLLN(tt );
            SERIAL_PROTOCOLPGM("ok T:");
            SERIAL_PROTOCOL(tt );
            SERIAL_PROTOCOLPGM(" B:");
            SERIAL_PROTOCOLLN(degBed() ); 
            codenum = millis(); 
          }
          manage_heater();
        }
        LCD_MESSAGEPGM("Bed done.");
    #endif
    break;

    #if FAN_PIN > -1
      case 106: //M106 Fan On
        if (code_seen('S')){
            WRITE(FAN_PIN,HIGH);
            fanpwm=constrain(code_value(),0,255);
            analogWrite(FAN_PIN,  fanpwm);
        }
        else {
          WRITE(FAN_PIN,HIGH);
          fanpwm=255;
          analogWrite(FAN_PIN, fanpwm);			
        }
        break;
      case 107: //M107 Fan Off
        WRITE(FAN_PIN,LOW);
        analogWrite(FAN_PIN, 0);
        break;
    #endif //FAN_PIN

    #if (PS_ON_PIN > -1)
      case 80: // M80 - ATX Power On
        SET_OUTPUT(PS_ON_PIN); //GND
        break;
      case 81: // M81 - ATX Power Off
        SET_INPUT(PS_ON_PIN); //Floating
        break;
    #endif
    case 82:
      axis_relative_modes[3] = false;
      break;
    case 83:
      axis_relative_modes[3] = true;
      break;
    case 18: //compatibility
    case 84:
      if(code_seen('S')){ 
        stepper_inactive_time = code_value() * 1000; 
      }
      else
      { 
        st_synchronize(); 
        LCD_MESSAGEPGM("Free move.");
        disable_x(); 
        disable_y(); 
        disable_z(); 
        disable_e(); 
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
      SerialprintPGM("FIRMWARE_NAME:Marlin; Sprinter/grbl mashup for gen6 FIRMWARE_URL:http://www.mendel-parts.com PROTOCOL_VERSION:1.0 MACHINE_TYPE:Mendel EXTRUDER_COUNT:1");
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
      #ifdef DEBUG_STEPS
        SERIAL_PROTOCOLPGM(" Count X:");
        SERIAL_PROTOCOL(float(count_position[X_AXIS])/axis_steps_per_unit[X_AXIS]);
        SERIAL_PROTOCOLPGM("Y:");
        SERIAL_PROTOCOL(float(count_position[Y_AXIS])/axis_steps_per_unit[Y_AXIS]);
        SERIAL_PROTOCOLPGM("Z:");
        SERIAL_PROTOCOL(float(count_position[Z_AXIS])/axis_steps_per_unit[Z_AXIS]);
      #endif
      SERIAL_PROTOCOLLN("");
      break;
    case 119: // M119
      #if (X_MIN_PIN > -1)
        SERIAL_PROTOCOLPGM("x_min:");
        SERIAL_PROTOCOL(((READ(X_MIN_PIN)^ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (X_MAX_PIN > -1)
        SERIAL_PROTOCOLPGM("x_max:");
        SERIAL_PROTOCOL(((READ(X_MAX_PIN)^ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Y_MIN_PIN > -1)
        SERIAL_PROTOCOLPGM("y_min:");
        SERIAL_PROTOCOL(((READ(Y_MIN_PIN)^ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Y_MAX_PIN > -1)
        SERIAL_PROTOCOLPGM("y_max:");
        SERIAL_PROTOCOL(((READ(Y_MAX_PIN)^ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Z_MIN_PIN > -1)
        SERIAL_PROTOCOLPGM("z_min:");
        SERIAL_PROTOCOL(((READ(Z_MIN_PIN)^ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      #if (Z_MAX_PIN > -1)
        SERIAL_PROTOCOLPGM("z_max:");
        SERIAL_PROTOCOL(((READ(Z_MAX_PIN)^ENDSTOPS_INVERTING)?"H ":"L "));
      #endif
      SERIAL_PROTOCOLLN("");
      break;
      //TODO: update for all axis, use for loop
    case 201: // M201
      for(int8_t i=0; i < NUM_AXIS; i++) 
      {
        if(code_seen(axis_codes[i])) axis_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
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
        if(code_seen(axis_codes[i])) max_feedrate[i] = code_value()*60 ;
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
      if(code_seen('S')) minimumfeedrate = code_value()*60 ;
      if(code_seen('T')) mintravelfeedrate = code_value()*60 ;
      if(code_seen('B')) minsegmenttime = code_value() ;
      if(code_seen('X')) max_xy_jerk = code_value()*60 ;
      if(code_seen('Z')) max_z_jerk = code_value()*60 ;
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
        SERIAL_PROTOCOL("ok p:");
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
    case 400: // finish all moves
    {
      st_synchronize();
    }
    break;
    case 500: // Store settings in EEPROM
    {
        StoreSettings();
    }
    break;
    case 501: // Read settings from EEPROM
    {
      RetrieveSettings();
    }
    break;
    case 502: // Revert to default settings
    {
      RetrieveSettings(true);
    }
    break;

    }
  }
  else
  {
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM("Unknown command:\"");
    SERIAL_ECHO(cmdbuffer[bufindr]);
    SERIAL_ECHOLNPGM("\"");
  }

  ClearToSend();
}

void FlushSerialRequestResend()
{
  //char cmdbuffer[bufindr][100]="Resend:";
  Serial.flush();
  SERIAL_PROTOCOLPGM("Resend:");
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
  SERIAL_PROTOCOLLNPGM("ok"); 
}

inline void get_coordinates()
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

inline void get_arc_coordinates()
{
   get_coordinates();
   if(code_seen('I')) offset[0] = code_value();
   if(code_seen('J')) offset[1] = code_value();
}

void prepare_move()
{
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60/100.0);
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}

void prepare_arc_move(char isclockwise) {
  float r = hypot(offset[X_AXIS], offset[Y_AXIS]); // Compute arc radius for mc_arc

  // Trace the arc
  mc_arc(current_position, destination, offset, X_AXIS, Y_AXIS, Z_AXIS, feedrate*feedmultiply/60/100.0, r, isclockwise);
  
  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}





void manage_inactivity(byte debug) 
{ 
  if( (millis()-previous_millis_cmd) >  max_inactive_time ) 
    if(max_inactive_time) 
      kill(); 
  if( (millis()-previous_millis_cmd) >  stepper_inactive_time ) 
    if(stepper_inactive_time) 
    { 
      disable_x(); 
      disable_y(); 
      disable_z(); 
      disable_e(); 
    }
  check_axes_activity();
}

void kill()
{
  disable_heater();

  disable_x();
  disable_y();
  disable_z();
  disable_e();
  
  if(PS_ON_PIN > -1) pinMode(PS_ON_PIN,INPUT);
  SERIAL_ERROR_START;
  SERIAL_ERRORLNPGM("Printer halted. kill() called !!");
  LCD_MESSAGEPGM("KILLED. ");
  while(1); // Wait for reset
}


