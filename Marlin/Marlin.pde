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
#include "streaming.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "motion_control.h"


char version_string[] = "1.0.0 Alpha 1";

#ifdef SDSUPPORT
  #include "SdFat.h"
#endif //SDSUPPORT


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
// M500 - stores paramters in EEPROM
// M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).  D
// M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.

//Stepper Movement Variables

//===========================================================================
//=============================imported variables============================
//===========================================================================
extern float HeaterPower;

//public variables
float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
volatile int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
volatile bool feedmultiplychanged=false;

//===========================================================================
//=============================private variables=============================
//===========================================================================
const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};
static float destination[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};
static float current_position[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};
static float offset[3] = {0.0, 0.0, 0.0};
static bool home_all_axis = true;
static float feedrate = 1500.0, next_feedrate, saved_feedrate;
static long gcode_N, gcode_LastN;



static bool relative_mode = false;  //Determines Absolute or Relative Coordinates
static bool relative_mode_e = false;  //Determines Absolute or Relative E Codes while in Absolute Coordinates mode. E is always relative in Relative Coordinates mode.

static uint8_t fanpwm=0;


// comm variables

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

#ifdef SDSUPPORT
  static Sd2Card card;
  static SdVolume volume;
  static SdFile root;
  static SdFile file;
  static uint32_t filesize = 0;
  static uint32_t sdpos = 0;
  static bool sdmode = false;
  static bool sdactive = false;
  static bool savetosd = false;
  static int16_t n;
  static unsigned long autostart_atmillis=0;
  
  static bool autostart_stilltocheck=true; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.
#endif //SDSUPPORT

//===========================================================================
//=============================ROUTINES=============================
//===========================================================================
#ifdef SDSUPPORT
  void initsd()
  {
    sdactive = false;
    #if SDSS >- 1
      if(root.isOpen())
	root.close();
      if (!card.init(SPI_FULL_SPEED,SDSS))
      {
	//if (!card.init(SPI_HALF_SPEED,SDSS))
	SERIAL_ECHOLN("SD init fail");
      }
      else if (!volume.init(&card))
      {
	SERIAL_ERRORLN("volume.init failed");
      }
      else if (!root.openRoot(&volume)) 
      {
	SERIAL_ERRORLN("openRoot failed");
      }
      else 
      {
	sdactive = true;
	SERIAL_ECHOLN("SD card ok");
      }
    #endif //SDSS
  }

  void quickinitsd()
  {
    sdactive=false;
    autostart_atmillis=millis()+5000;
  }

  inline void write_command(char *buf)
  {
    char* begin = buf;
    char* npos = 0;
    char* end = buf + strlen(buf) - 1;

    file.writeError = false;
    if((npos = strchr(buf, 'N')) != NULL)
    {
      begin = strchr(npos, ' ') + 1;
      end = strchr(npos, '*') - 1;
    }
    end[1] = '\r';
    end[2] = '\n';
    end[3] = '\0';
    file.write(begin);
    if (file.writeError)
    {
      SERIAL_ERRORLN("error writing to file");
    }
  }
  

  void checkautostart(bool force)
  {
  //this is to delay autostart and hence the initialisaiton of the sd card to some seconds after the normal init, so the device is available quick after a reset
    if(!force)
    {
      if(!autostart_stilltocheck)
        return;
      if(autostart_atmillis<millis())
        return;
    }
    autostart_stilltocheck=false;
    if(!sdactive)
    {
      initsd();
      if(!sdactive) //fail
        return;
    }
    static int lastnr=0;
    char autoname[30];
    sprintf(autoname,"auto%i.g",lastnr);
    for(int i=0;i<(int)strlen(autoname);i++)
      autoname[i]=tolower(autoname[i]);
    dir_t p;

    root.rewind();
    
    bool found=false;
    while (root.readDir(p) > 0) 
    {
      for(int i=0;i<(int)strlen((char*)p.name);i++)
      p.name[i]=tolower(p.name[i]);
      //Serial.print((char*)p.name);
      //Serial.print(" ");
      //Serial.println(autoname);
      if(p.name[9]!='~') //skip safety copies
      if(strncmp((char*)p.name,autoname,5)==0)
      {
        char cmd[30];

        sprintf(cmd,"M23 %s",autoname);
        //sprintf(cmd,"M115");
        //enquecommand("G92 Z0");
        //enquecommand("G1 Z10 F2000");
        //enquecommand("G28 X-105 Y-105");
        enquecommand(cmd);
        enquecommand("M24");
        found=true;
      }
    }
    if(!found)
      lastnr=-1;
    else
      lastnr++;
  }
#else  //NO SD SUPORT
  inline void checkautostart(bool x){};
  
#endif //SDSUPPORT


//adds an command to the main command buffer
//thats really done in a non-safe way.
//needs overworking someday
void enquecommand(const char *cmd)
{
  if(buflen < BUFSIZE)
  {
    //this is dangerous if a mixing of serial and this happsens
    strcpy(&(cmdbuffer[bufindw][0]),cmd);
    SERIAL_ECHOLN("enqueing \""<<cmdbuffer[bufindw]<<"\"");
    bufindw= (bufindw + 1)%BUFSIZE;
    buflen += 1;
  }
}

void setup()
{ 
  Serial.begin(BAUDRATE);
  SERIAL_ECHOLN("Marlin "<<version_string);
  Serial.println("start");
  for(int i = 0; i < BUFSIZE; i++)
  {
    fromsd[i] = false;
  }
  
  RetrieveSettings(); // loads data from EEPROM if available

  for(int i=0; i < NUM_AXIS; i++)
  {
    axis_steps_per_sqr_second[i] = max_acceleration_units_per_sq_second[i] * axis_steps_per_unit[i];
  }

  #ifdef SDSUPPORT
    //power to SD reader
    #if SDPOWER > -1
      SET_OUTPUT(SDPOWER); 
      WRITE(SDPOWER,HIGH);
    #endif //SDPOWER
    quickinitsd();
  #endif //SDSUPPORT
  
  plan_init();  // Initialize planner;
  st_init();    // Initialize stepper;
  tp_init();    // Initialize temperature loop
}


void loop()
{
  if(buflen<3)
    get_command();
  checkautostart(false);
  if(buflen)
  {
    #ifdef SDSUPPORT
      if(savetosd)
      {
	if(strstr(cmdbuffer[bufindr],"M29") == NULL)
	{
	  write_command(cmdbuffer[bufindr]);
	  Serial.println("ok");
	}
	else
	{
	  file.sync();
	  file.close();
	  savetosd = false;
	  Serial.println("Done saving file.");
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
            Serial.print("Serial Error: Line Number is not Last Line Number+1, Last Line:");
            Serial.println(gcode_LastN);
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
              Serial.print("Error: checksum mismatch, Last Line:");
              Serial.println(gcode_LastN);
              FlushSerialRequestResend();
              serial_count = 0;
              return;
            }
            //if no errors, continue parsing
          }
          else 
          {
            Serial.print("Error: No Checksum with line number, Last Line:");
            Serial.println(gcode_LastN);
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
            Serial.print("Error: No Line Number with checksum, Last Line:");
            Serial.println(gcode_LastN);
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
            if(savetosd)
              break;
	    #endif //SDSUPPORT
            Serial.println("ok"); 
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
  if(!sdmode || serial_count!=0){
    return;
  }
  while( filesize > sdpos  && buflen < BUFSIZE) {
    n = file.read();
    serial_char = (char)n;
    if(serial_char == '\n' || serial_char == '\r' || serial_char == ':' || serial_count >= (MAX_CMD_SIZE - 1) || n == -1) 
    {
      sdpos = file.curPosition();
      if(sdpos >= filesize){
        sdmode = false;
        Serial.println("echo: Done printing file");
        stoptime=millis();
        char time[30];
        unsigned long t=(stoptime-starttime)/1000;
        int sec,min;
        min=t/60;
        sec=t%60;
        sprintf(time,"echo: %i min, %i sec",min,sec);
        Serial.println(time);
        LCD_MESSAGE(time);
        checkautostart(true);
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
      
      for(int i=0; i < NUM_AXIS; i++) {
        destination[i] = current_position[i];
      }
      feedrate = 0.0;
      home_all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2])));

      if((home_all_axis) || (code_seen(axis_codes[X_AXIS]))) 
      {
        if ((X_MIN_PIN > -1 && X_HOME_DIR==-1) || (X_MAX_PIN > -1 && X_HOME_DIR==1)){
//          st_synchronize();
          current_position[X_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[X_AXIS] = 1.5 * X_MAX_LENGTH * X_HOME_DIR;
          feedrate = homing_feedrate[X_AXIS];
          prepare_move();
          
//          st_synchronize();        
          current_position[X_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[X_AXIS] = -5 * X_HOME_DIR;
          prepare_move();
          
//          st_synchronize();         
          destination[X_AXIS] = 10 * X_HOME_DIR;
          feedrate = homing_feedrate[X_AXIS]/2 ;
          prepare_move();
          
//          st_synchronize();
          current_position[X_AXIS] = (X_HOME_DIR == -1) ? 0 : X_MAX_LENGTH;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[X_AXIS] = current_position[X_AXIS];
          feedrate = 0.0;
        }
      }

      if((home_all_axis) || (code_seen(axis_codes[Y_AXIS]))) {
        if ((Y_MIN_PIN > -1 && Y_HOME_DIR==-1) || (Y_MAX_PIN > -1 && Y_HOME_DIR==1)){
          current_position[Y_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Y_AXIS] = 1.5 * Y_MAX_LENGTH * Y_HOME_DIR;
          feedrate = homing_feedrate[Y_AXIS];
          prepare_move();
//          st_synchronize();

          current_position[Y_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Y_AXIS] = -5 * Y_HOME_DIR;
          prepare_move();
//          st_synchronize();

          destination[Y_AXIS] = 10 * Y_HOME_DIR;
          feedrate = homing_feedrate[Y_AXIS]/2;
          prepare_move();
//          st_synchronize();

          current_position[Y_AXIS] = (Y_HOME_DIR == -1) ? 0 : Y_MAX_LENGTH;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Y_AXIS] = current_position[Y_AXIS];
          feedrate = 0.0;
        }
      }

      if((home_all_axis) || (code_seen(axis_codes[Z_AXIS]))) {
        if ((Z_MIN_PIN > -1 && Z_HOME_DIR==-1) || (Z_MAX_PIN > -1 && Z_HOME_DIR==1)){
          current_position[Z_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Z_AXIS] = 1.5 * Z_MAX_LENGTH * Z_HOME_DIR;
          feedrate = homing_feedrate[Z_AXIS];
          prepare_move();
//          st_synchronize();

          current_position[Z_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Z_AXIS] = -2 * Z_HOME_DIR;
          prepare_move();
//          st_synchronize();

          destination[Z_AXIS] = 3 * Z_HOME_DIR;
          feedrate = homing_feedrate[Z_AXIS]/2;
          prepare_move();
//          st_synchronize();

          current_position[Z_AXIS] = (Z_HOME_DIR == -1) ? 0 : Z_MAX_LENGTH;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Z_AXIS] = current_position[Z_AXIS];
          feedrate = 0.0;         
        }
      }       
      feedrate = saved_feedrate;
      feedmultiply = saved_feedmultiply;
      previous_millis_cmd = millis();
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
      for(int i=0; i < NUM_AXIS; i++) {
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
      Serial.println("Begin file list");
      root.ls();
      Serial.println("End file list");
      break;
    case 21: // M21 - init SD card
      sdmode = false;
      initsd();
      break;
    case 22: //M22 - release SD card
      sdmode = false;
      sdactive = false;
      break;
    case 23: //M23 - Select file
      if(sdactive){
        sdmode = false;
        file.close();
        starpos = (strchr(strchr_pointer + 4,'*'));
        if(starpos!=NULL)
          *(starpos-1)='\0';
        if (file.open(&root, strchr_pointer + 4, O_READ)) {
          Serial.print("File opened:");
          Serial.print(strchr_pointer + 4);
          Serial.print(" Size:");
          Serial.println(file.fileSize());
          sdpos = 0;
          filesize = file.fileSize();
          Serial.println("File selected");
        }
        else{
          Serial.println("file.open failed");
        }
      }
      break;
    case 24: //M24 - Start SD print
      if(sdactive){
        sdmode = true;
	starttime=millis();
      }
      break;
    case 25: //M25 - Pause SD print
      if(sdmode){
        sdmode = false;
      }
      break;
    case 26: //M26 - Set SD index
      if(sdactive && code_seen('S')){
        sdpos = code_value_long();
        file.seekSet(sdpos);
      }
      break;
    case 27: //M27 - Get SD status
      if(sdactive){
        Serial.print("SD printing byte ");
        Serial.print(sdpos);
        Serial.print("/");
        Serial.println(filesize);
      }
      else{
        Serial.println("Not SD printing");
      }
      break;
    case 28: //M28 - Start SD write
      if(sdactive){
        char* npos = 0;
        file.close();
        sdmode = false;
        starpos = (strchr(strchr_pointer + 4,'*'));
        if(starpos != NULL){
          npos = strchr(cmdbuffer[bufindr], 'N');
          strchr_pointer = strchr(npos,' ') + 1;
          *(starpos-1) = '\0';
        }
        if (!file.open(&root, strchr_pointer+4, O_CREAT | O_APPEND | O_WRITE | O_TRUNC))
        {
          Serial.print("open failed, File: ");
          Serial.print(strchr_pointer + 4);
          Serial.print(".");
        }
        else{
          savetosd = true;
          Serial.print("Writing to file: ");
          Serial.println(strchr_pointer + 4);
        }
      }
      break;
    case 29: //M29 - Stop SD write
      //processed in write to file routine above
      //savetosd = false;
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
      sprintf(time,"echo: time needed %i min, %i sec",min,sec);
      Serial.println(time);
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
          for(int i = 0; i < (int)sizeof(sensitive_pins); i++)
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
      #if (TEMP_0_PIN > -1) || defined (HEATER_USES_AD595)
        tt = degHotend0();
      #endif
      #if TEMP_1_PIN > -1
          bt = degBed();
      #endif
      #if (TEMP_0_PIN > -1) || defined (HEATER_USES_AD595)
        Serial.print("ok T:");
        Serial.print(tt); 
        #if TEMP_1_PIN > -1 
          #ifdef PIDTEMP
            Serial.print(" B:");
            #if TEMP_1_PIN > -1
              Serial.println(bt); 
            #else
              Serial.println(HeaterPower); 
            #endif
          #else //not PIDTEMP
            Serial.println();
           #endif //PIDTEMP
         #else
            Serial.println();
          #endif //TEMP_1_PIN
        #else
          Serial.println("echo: No thermistors - no temp");
      #endif
      return;
      break;
    case 109: 
    {// M109 - Wait for extruder heater to reach target.
        LCD_MESSAGE("Heating...");
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
          Serial.print("T:");
        Serial.println( degHotend0() ); 
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
        LCD_MESSAGE("Heating done.");
        starttime=millis();
      }
      break;
    case 190: // M190 - Wait bed for heater to reach target.
    #if TEMP_1_PIN > -1
        if (code_seen('S')) setTargetBed(code_value());
        codenum = millis(); 
        while(isHeatingBed()) 
        {
          if( (millis()-codenum) > 1000 ) //Print Temp Reading every 1 second while heating up.
          {
            float tt=degHotend0();
            Serial.print("T:");
            Serial.println( tt );
            Serial.print("ok T:");
            Serial.print( tt ); 
            Serial.print(" B:");
            Serial.println( degBed() ); 
            codenum = millis(); 
          }
          manage_heater();
        }
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
      for(int i=0; i < NUM_AXIS; i++) 
      {
        if(code_seen(axis_codes[i])) 
          axis_steps_per_unit[i] = code_value();
      }
      break;
    case 115: // M115
      Serial.println("FIRMWARE_NAME:Marlin; Sprinter/grbl mashup for gen6 FIRMWARE_URL:http://www.mendel-parts.com PROTOCOL_VERSION:1.0 MACHINE_TYPE:Mendel EXTRUDER_COUNT:1");
      break;
    case 114: // M114
      Serial.print("X:");
      Serial.print(current_position[X_AXIS]);
      Serial.print("Y:");
      Serial.print(current_position[Y_AXIS]);
      Serial.print("Z:");
      Serial.print(current_position[Z_AXIS]);
      Serial.print("E:");      
      Serial.print(current_position[E_AXIS]);
      #ifdef DEBUG_STEPS
        Serial.print(" Count X:");
        Serial.print(float(count_position[X_AXIS])/axis_steps_per_unit[X_AXIS]);
        Serial.print("Y:");
        Serial.print(float(count_position[Y_AXIS])/axis_steps_per_unit[Y_AXIS]);
        Serial.print("Z:");
        Serial.println(float(count_position[Z_AXIS])/axis_steps_per_unit[Z_AXIS]);
      #endif
      Serial.println("");
      break;
    case 119: // M119
      #if (X_MIN_PIN > -1)
        Serial.print("x_min:");
        Serial.print((READ(X_MIN_PIN)^ENDSTOPS_INVERTING)?"H ":"L ");
      #endif
      #if (X_MAX_PIN > -1)
        Serial.print("x_max:");
        Serial.print((READ(X_MAX_PIN)^ENDSTOPS_INVERTING)?"H ":"L ");
      #endif
      #if (Y_MIN_PIN > -1)
        Serial.print("y_min:");
        Serial.print((READ(Y_MIN_PIN)^ENDSTOPS_INVERTING)?"H ":"L ");
      #endif
      #if (Y_MAX_PIN > -1)
        Serial.print("y_max:");
        Serial.print((READ(Y_MAX_PIN)^ENDSTOPS_INVERTING)?"H ":"L ");
      #endif
      #if (Z_MIN_PIN > -1)
        Serial.print("z_min:");
        Serial.print((READ(Z_MIN_PIN)^ENDSTOPS_INVERTING)?"H ":"L ");
      #endif
      #if (Z_MAX_PIN > -1)
        Serial.print("z_max:");
        Serial.print((READ(Z_MAX_PIN)^ENDSTOPS_INVERTING)?"H ":"L ");
      #endif
      Serial.println("");
      break;
      //TODO: update for all axis, use for loop
    case 201: // M201
      for(int i=0; i < NUM_AXIS; i++) 
      {
        if(code_seen(axis_codes[i])) axis_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
      }
      break;
    #if 0 // Not used for Sprinter/grbl gen6
    case 202: // M202
      for(int i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
      }
      break;
    #endif
    case 203: // M203 max feedrate mm/sec
      for(int i=0; i < NUM_AXIS; i++) {
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
      if(code_seen('P')) Kp = code_value();
      if(code_seen('I')) Ki = code_value()*PID_dT;
      if(code_seen('D')) Kd = code_value()/PID_dT;
      break;
    #endif //PIDTEMP
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
    Serial.print("echo: Unknown command:\"");
    Serial.print(cmdbuffer[bufindr]);
    Serial.println("\"");
  }

  ClearToSend();
}

void FlushSerialRequestResend()
{
  //char cmdbuffer[bufindr][100]="Resend:";
  Serial.flush();
  Serial.print("Resend:");
  Serial.println(gcode_LastN + 1);
  ClearToSend();
}

void ClearToSend()
{
  previous_millis_cmd = millis();
  #ifdef SDSUPPORT
  if(fromsd[bufindr])
    return;
  #endif //SDSUPPORT
  Serial.println("ok"); 
}

inline void get_coordinates()
{
  for(int i=0; i < NUM_AXIS; i++) {
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
   if(code_seen("I")) offset[0] = code_value();
   if(code_seen("J")) offset[1] = code_value();
}

void prepare_move()
{
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60.0/100.0);
  for(int i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}

void prepare_arc_move(char isclockwise) {
#if 0
  if (radius_mode) {
    /* 
      We need to calculate the center of the circle that has the designated radius and passes
      through both the current position and the target position. This method calculates the following
      set of equations where [x,y] is the vector from current to target position, d == magnitude of 
      that vector, h == hypotenuse of the triangle formed by the radius of the circle, the distance to
      the center of the travel vector. A vector perpendicular to the travel vector [-y,x] is scaled to the 
      length of h [-y/d*h, x/d*h] and added to the center of the travel vector [x/2,y/2] to form the new point 
      [i,j] at [x/2-y/d*h, y/2+x/d*h] which will be the center of our arc.
      
      d^2 == x^2 + y^2
      h^2 == r^2 - (d/2)^2
      i == x/2 - y/d*h
      j == y/2 + x/d*h
      
                                                           O <- [i,j]
                                                        -  |
                                              r      -     |
                                                  -        |
                                               -           | h
                                            -              |
                              [0,0] ->  C -----------------+--------------- T  <- [x,y]
                                        | <------ d/2 ---->|
                
      C - Current position
      T - Target position
      O - center of circle that pass through both C and T
      d - distance from C to T
      r - designated radius
      h - distance from center of CT to O
      
      Expanding the equations:

      d -> sqrt(x^2 + y^2)
      h -> sqrt(4 * r^2 - x^2 - y^2)/2
      i -> (x - (y * sqrt(4 * r^2 - x^2 - y^2)) / sqrt(x^2 + y^2)) / 2 
      j -> (y + (x * sqrt(4 * r^2 - x^2 - y^2)) / sqrt(x^2 + y^2)) / 2
     
      Which can be written:
      
      i -> (x - (y * sqrt(4 * r^2 - x^2 - y^2))/sqrt(x^2 + y^2))/2
      j -> (y + (x * sqrt(4 * r^2 - x^2 - y^2))/sqrt(x^2 + y^2))/2
      
      Which we for size and speed reasons optimize to:

      h_x2_div_d = sqrt(4 * r^2 - x^2 - y^2)/sqrt(x^2 + y^2)
      i = (x - (y * h_x2_div_d))/2
      j = (y + (x * h_x2_div_d))/2
      
    */
    
    // Calculate the change in position along each selected axis
    double x = target[gc.plane_axis_0]-gc.position[gc.plane_axis_0];
    double y = target[gc.plane_axis_1]-gc.position[gc.plane_axis_1];
    
    clear_vector(offset);
    double h_x2_div_d = -sqrt(4 * r*r - x*x - y*y)/hypot(x,y); // == -(h * 2 / d)
    // If r is smaller than d, the arc is now traversing the complex plane beyond the reach of any
    // real CNC, and thus - for practical reasons - we will terminate promptly:
    if(isnan(h_x2_div_d)) { FAIL(STATUS_FLOATING_POINT_ERROR); return(gc.status_code); }
    // Invert the sign of h_x2_div_d if the circle is counter clockwise (see sketch below)
    if (gc.motion_mode == MOTION_MODE_CCW_ARC) { h_x2_div_d = -h_x2_div_d; }
    
    /* The counter clockwise circle lies to the left of the target direction. When offset is positive,
       the left hand circle will be generated - when it is negative the right hand circle is generated.
       
       
                                                         T  <-- Target position
                                                         
                                                         ^ 
              Clockwise circles with this center         |          Clockwise circles with this center will have
              will have > 180 deg of angular travel      |          < 180 deg of angular travel, which is a good thing!
                                               \         |          /   
  center of arc when h_x2_div_d is positive ->  x <----- | -----> x <- center of arc when h_x2_div_d is negative
                                                         |
                                                         |
                                                         
                                                         C  <-- Current position                                 */
                

    // Negative R is g-code-alese for "I want a circle with more than 180 degrees of travel" (go figure!), 
    // even though it is advised against ever generating such circles in a single line of g-code. By 
    // inverting the sign of h_x2_div_d the center of the circles is placed on the opposite side of the line of
    // travel and thus we get the unadvisably long arcs as prescribed.
    if (r < 0) { 
        h_x2_div_d = -h_x2_div_d; 
        r = -r; // Finished with r. Set to positive for mc_arc
    }        
    // Complete the operation by calculating the actual center of the arc
    offset[gc.plane_axis_0] = 0.5*(x-(y*h_x2_div_d));
    offset[gc.plane_axis_1] = 0.5*(y+(x*h_x2_div_d));

  } else { // Offset mode specific computations
#endif
    float r = hypot(offset[X_AXIS], offset[Y_AXIS]); // Compute arc radius for mc_arc

//  }
  
  // Set clockwise/counter-clockwise sign for mc_arc computations
//  uint8_t isclockwise = false;
//  if (gc.motion_mode == MOTION_MODE_CW_ARC) { isclockwise = true; }

  // Trace the arc
  mc_arc(current_position, destination, offset, X_AXIS, Y_AXIS, Z_AXIS, feedrate*feedmultiply/60.0/100.0, r, isclockwise);
    
//  }
  
  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  for(int ii=0; ii < NUM_AXIS; ii++) {
    current_position[ii] = destination[ii];
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
  SERIAL_ERRORLN("Printer halted. kill() called !!");
  while(1); // Wait for reset
}


