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
#include "fastio.h"
#include "Configuration.h"
#include "pins.h"
#include "Marlin.h"
#include "ultralcd.h"
#include "streaming.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"

#ifdef SIMPLE_LCD
  #include "Simplelcd.h"
#endif

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

char axis_codes[NUM_AXIS] = {
  'X', 'Y', 'Z', 'E'};
float destination[NUM_AXIS] = {
  0.0, 0.0, 0.0, 0.0};
float current_position[NUM_AXIS] = {
  0.0, 0.0, 0.0, 0.0};
bool home_all_axis = true;
float feedrate = 1500.0, next_feedrate, saved_feedrate;
long gcode_N, gcode_LastN;

float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;

bool relative_mode = false;  //Determines Absolute or Relative Coordinates
bool relative_mode_e = false;  //Determines Absolute or Relative E Codes while in Absolute Coordinates mode. E is always relative in Relative Coordinates mode.

uint8_t fanpwm=0;

volatile int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
volatile bool feedmultiplychanged=false;
// comm variables
#define MAX_CMD_SIZE 96
#define BUFSIZE 4
char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
bool fromsd[BUFSIZE];
int bufindr = 0;
int bufindw = 0;
int buflen = 0;
int i = 0;
char serial_char;
int serial_count = 0;
boolean comment_mode = false;
char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc
extern float HeaterPower;

#include "EEPROM.h"

const int sensitive_pins[] = SENSITIVE_PINS; // Sensitive pin list for M42

float tt = 0, bt = 0;
#ifdef WATCHPERIOD
int watch_raw = -1000;
unsigned long watchmillis = 0;
#endif //WATCHPERIOD

//Inactivity shutdown variables
unsigned long previous_millis_cmd = 0;
unsigned long max_inactive_time = 0;
unsigned long stepper_inactive_time = 0;

unsigned long starttime=0;
unsigned long stoptime=0;
#ifdef SDSUPPORT
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;
uint32_t filesize = 0;
uint32_t sdpos = 0;
bool sdmode = false;
bool sdactive = false;
bool savetosd = false;
int16_t n;
long autostart_atmillis=0;

void initsd(){
  sdactive = false;
#if SDSS >- 1
  if(root.isOpen())
    root.close();
  if (!card.init(SPI_FULL_SPEED,SDSS)){
    //if (!card.init(SPI_HALF_SPEED,SDSS))
    Serial.println("SD init fail");
  }
  else if (!volume.init(&card))
    Serial.println("volume.init failed");
  else if (!root.openRoot(&volume)) 
    Serial.println("openRoot failed");
  else 
	{
    sdactive = true;
		Serial.println("SD card ok");
	}
#endif //SDSS
}

void quickinitsd(){
	sdactive=false;
	autostart_atmillis=millis()+5000;
}

inline void write_command(char *buf){
  char* begin = buf;
  char* npos = 0;
  char* end = buf + strlen(buf) - 1;

  file.writeError = false;
  if((npos = strchr(buf, 'N')) != NULL){
    begin = strchr(npos, ' ') + 1;
    end = strchr(npos, '*') - 1;
  }
  end[1] = '\r';
  end[2] = '\n';
  end[3] = '\0';
  //Serial.println(begin);
  file.write(begin);
  if (file.writeError){
    Serial.println("error writing to file");
  }
}
#endif //SDSUPPORT


///adds an command to the main command buffer
void enquecommand(const char *cmd)
{
  if(buflen < BUFSIZE)
  {
    //this is dangerous if a mixing of serial and this happsens
    strcpy(&(cmdbuffer[bufindw][0]),cmd);
    Serial.print("en:");Serial.println(cmdbuffer[bufindw]);
    bufindw= (bufindw + 1)%BUFSIZE;
    buflen += 1;
  }
}

void setup()
{ 
	
  Serial.begin(BAUDRATE);
  ECHOLN("Marlin "<<version_string);
  Serial.println("start");
#if defined FANCY_LCD || defined SIMPLE_LCD
  lcd_init();
#endif
  for(int i = 0; i < BUFSIZE; i++){
    fromsd[i] = false;
  }
  
  RetrieveSettings(); // loads data from EEPROM if available


  for(int i=0; i < NUM_AXIS; i++){
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
	//checkautostart();
}

#ifdef SDSUPPORT
bool autostart_stilltocheck=true;


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
	for(int i=0;i<strlen(autoname);i++)
		autoname[i]=tolower(autoname[i]);
	dir_t p;

	root.rewind();
	char filename[11];
	int cnt=0;

	bool found=false;
	while (root.readDir(p) > 0) 
	{
		for(int i=0;i<strlen((char*)p.name);i++)
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
#else

inline void checkautostart(bool x)
{
}
#endif


void loop()
{
  if(buflen<3)
    get_command();
	checkautostart(false);
  if(buflen)
  {
#ifdef SDSUPPORT
    if(savetosd){
      if(strstr(cmdbuffer[bufindr],"M29") == NULL){
        write_command(cmdbuffer[bufindr]);
        Serial.println("ok");
      }
      else{
        file.sync();
        file.close();
        savetosd = false;
        Serial.println("Done saving file.");
      }
    }
    else{
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
        Serial.println("Done printing file");
				stoptime=millis();
				char time[30];
				unsigned long t=(stoptime-starttime)/1000;
				int sec,min;
				min=t/60;
				sec=t%60;
				sprintf(time,"%i min, %i sec",min,sec);
				Serial.println(time);
				LCD_MESSAGE(time);
				checkautostart(true);
      }
      if(!serial_count) return; //if empty line
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


inline float code_value() { 
  return (strtod(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL)); 
}
inline long code_value_long() { 
  return (strtol(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL, 10)); 
}
inline bool code_seen(char code_string[]) { 
  return (strstr(cmdbuffer[bufindr], code_string) != NULL); 
}  //Return True if the string was found

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

      if((home_all_axis) || (code_seen(axis_codes[X_AXIS]))) {
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
		case 30:
		{
			stoptime=millis();
				char time[30];
				unsigned long t=(stoptime-starttime)/1000;
				int sec,min;
				min=t/60;
				sec=t%60;
				sprintf(time,"%i min, %i sec",min,sec);
				Serial.println(time);
				LCD_MESSAGE(time);
		}
				break;
#endif //SDSUPPORT
      case 42: //M42 -Change pin status via gcode
        if (code_seen('S'))
        {
          int pin_status = code_value();
          if (code_seen('P') && pin_status >= 0 && pin_status <= 255)
          {
            int pin_number = code_value();
            for(int i = 0; i < sizeof(sensitive_pins); i++)
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
                if (code_seen('S')) target_raw[0] = temp2analog(code_value());
#ifdef PIDTEMP
                pid_setpoint = code_value();
#endif //PIDTEM
        #ifdef WATCHPERIOD
            if(target_raw[0] > current_raw[0]){
                watchmillis = max(1,millis());
                watch_raw = current_raw[0];
            }else{
                watchmillis = 0;
            }
        #endif
        break;
      case 140: // M140 set bed temp
                if (code_seen('S')) target_raw[1] = temp2analogBed(code_value());
        break;
      case 105: // M105
        #if (TEMP_0_PIN > -1) || defined (HEATER_USES_AD595)
                tt = analog2temp(current_raw[0]);
        #endif
        #if TEMP_1_PIN > -1
                bt = analog2tempBed(current_raw[1]);
        #endif
        #if (TEMP_0_PIN > -1) || defined (HEATER_USES_AD595)
            Serial.print("ok T:");
            Serial.print(tt); 
//            Serial.print(", raw:");
//            Serial.print(current_raw);       
          #if TEMP_1_PIN > -1 
#ifdef PIDTEMP
            Serial.print(" B:");
            #if TEMP_1_PIN > -1
            Serial.println(bt); 
            #else
            Serial.println(HeaterPower); 
            #endif
#else
            Serial.println();
#endif
          #else
            Serial.println();
          #endif
        #else
          Serial.println("No thermistors - no temp");
        #endif
        return;
        //break;
      case 109: {// M109 - Wait for extruder heater to reach target.
            LCD_MESSAGE("Heating...");
            if (code_seen('S')) target_raw[0] = temp2analog(code_value());
            #ifdef PIDTEMP
            pid_setpoint = code_value();
            #endif //PIDTEM
            #ifdef WATCHPERIOD
            if(target_raw[0]>current_raw[0]) {
              watchmillis = max(1,millis());
              watch_raw = current_raw[0];
            } else {
              watchmillis = 0;
            }
            #endif //WATCHPERIOD
            codenum = millis(); 
     
               /* See if we are heating up or cooling down */
              bool target_direction = (current_raw[0] < target_raw[0]); // true if heating, false if cooling

            #ifdef TEMP_RESIDENCY_TIME
            long residencyStart;
            residencyStart = -1;
            /* continue to loop until we have reached the target temp   
              _and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
            while((target_direction ? (current_raw[0] < target_raw[0]) : (current_raw[0] > target_raw[0])) ||
                    (residencyStart > -1 && (millis() - residencyStart) < TEMP_RESIDENCY_TIME*1000) ) {
            #else
            while ( target_direction ? (current_raw[0] < target_raw[0]) : (current_raw[0] > target_raw[0]) ) {
            #endif //TEMP_RESIDENCY_TIME
              if( (millis() - codenum) > 1000 ) { //Print Temp Reading every 1 second while heating up/cooling down
                Serial.print("T:");
                Serial.println( analog2temp(current_raw[0]) );
                codenum = millis();
              }
              manage_heater();
              LCD_STATUS;
              #ifdef TEMP_RESIDENCY_TIME
               /* start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
                  or when current temp falls outside the hysteresis after target temp was reached */
              if ((residencyStart == -1 &&  target_direction && current_raw[0] >= target_raw[0]) ||
                  (residencyStart == -1 && !target_direction && current_raw[0] <= target_raw[0]) ||
                  (residencyStart > -1 && labs(analog2temp(current_raw[0]) - analog2temp(target_raw[0])) > TEMP_HYSTERESIS) ) {
                residencyStart = millis();
              }
              #endif //TEMP_RESIDENCY_TIME
	    }
            LCD_MESSAGE("Marlin ready.");
          }
          break;
      case 190: // M190 - Wait bed for heater to reach target.
      #if TEMP_1_PIN > -1
          if (code_seen('S')) target_raw[1] = temp2analog(code_value());
        codenum = millis(); 
          while(current_raw[1] < target_raw[1]) 
                                {
          if( (millis()-codenum) > 1000 ) //Print Temp Reading every 1 second while heating up.
          {
            float tt=analog2temp(current_raw[0]);
            Serial.print("T:");
            Serial.println( tt );
            Serial.print("ok T:");
            Serial.print( tt ); 
            Serial.print(" B:");
            Serial.println( analog2temp(current_raw[1]) ); 
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
#endif
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
		case 18:
    case 84:
      if(code_seen('S')){ 
        stepper_inactive_time = code_value() * 1000; 
      }
      else{ 
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
      for(int i=0; i < NUM_AXIS; i++) {
        if(code_seen(axis_codes[i])) axis_steps_per_unit[i] = code_value();
      }

      break;
    case 115: // M115
      Serial.println("FIRMWARE_NAME:Sprinter/grbl mashup for gen6 FIRMWARE_URL:http://www.mendel-parts.com PROTOCOL_VERSION:1.0 MACHINE_TYPE:Mendel EXTRUDER_COUNT:1");
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
      for(int i=0; i < NUM_AXIS; i++) {
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
//      ECHOLN("Kp "<<_FLOAT(Kp,2));
//      ECHOLN("Ki "<<_FLOAT(Ki/PID_dT,2));
//      ECHOLN("Kd "<<_FLOAT(Kd*PID_dT,2));

//      temp_iState_min = 0.0;
//      if (Ki!=0) {
//      temp_iState_max = PID_INTEGRAL_DRIVE_MAX / (Ki/100.0);
//      }
//      else       temp_iState_max = 1.0e10;
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
  else{
    Serial.println("Unknown command:");
    Serial.println(cmdbuffer[bufindr]);
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
    else destination[i] = current_position[i];                                                       //Are these else lines really needed?
  }
  if(code_seen('F')) {
    next_feedrate = code_value();
    if(next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

void prepare_move()
{
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60.0/100.0);
  for(int i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}



#ifdef USE_WATCHDOG

#include  <avr/wdt.h>
#include  <avr/interrupt.h>

volatile uint8_t timeout_seconds=0;

void(* ctrlaltdelete) (void) = 0;

ISR(WDT_vect) { //Watchdog timer interrupt, called if main program blocks >1sec
  if(timeout_seconds++ >= WATCHDOG_TIMEOUT)
  {
   kill();
#ifdef RESET_MANUAL
    LCD_MESSAGE("Please Reset!");
    ECHOLN("echo_: Something is wrong, please turn off the printer.");
#else
    LCD_MESSAGE("Timeout, resetting!");
#endif 
    //disable watchdog, it will survife reboot.
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    WDTCSR = 0;
#ifdef RESET_MANUAL
    while(1); //wait for user or serial reset
#else
    ctrlaltdelete();
#endif
  }
}

/// intialise watch dog with a 1 sec interrupt time
void wd_init() {
  WDTCSR = (1<<WDCE )|(1<<WDE ); //allow changes
  WDTCSR = (1<<WDIF)|(1<<WDIE)| (1<<WDCE )|(1<<WDE )|  (1<<WDP2 )|(1<<WDP1)|(0<<WDP0);
}

/// reset watchdog. MUST be called every 1s after init or avr will reset.
void wd_reset() {
  wdt_reset();
  timeout_seconds=0; //reset counter for resets
}
#endif /* USE_WATCHDOG */


inline void kill()
{
  #if TEMP_0_PIN > -1
  target_raw[0]=0;
   #if HEATER_0_PIN > -1  
     WRITE(HEATER_0_PIN,LOW);
   #endif
  #endif
  #if TEMP_1_PIN > -1
  target_raw[1]=0;
  #if HEATER_1_PIN > -1 
    WRITE(HEATER_1_PIN,LOW);
  #endif
  #endif
  #if TEMP_2_PIN > -1
  target_raw[2]=0;
  #if HEATER_2_PIN > -1  
    WRITE(HEATER_2_PIN,LOW);
  #endif
  #endif
  disable_x();
  disable_y();
  disable_z();
  disable_e();
  
  if(PS_ON_PIN > -1) pinMode(PS_ON_PIN,INPUT);
  Serial.println("!! Printer halted. kill() called!!");
  while(1); // Wait for reset
}

void manage_inactivity(byte debug) { 
  if( (millis()-previous_millis_cmd) >  max_inactive_time ) if(max_inactive_time) kill(); 
  if( (millis()-previous_millis_cmd) >  stepper_inactive_time ) if(stepper_inactive_time) { 
    disable_x(); 
    disable_y(); 
    disable_z(); 
    disable_e(); 
  }
  check_axes_activity();
}
