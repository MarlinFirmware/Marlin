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

 This firmware is optimized for gen6 electronics.
 */

#include "fastio.h"
#include "Configuration.h"
#include "pins.h"
#include "Marlin.h"
#include "speed_lookuptable.h"

char version_string[] = "0.9.10";

#ifdef SDSUPPORT
#include "SdFat.h"
#endif //SDSUPPORT

#ifndef CRITICAL_SECTION_START
#define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli()
#define CRITICAL_SECTION_END    SREG = _sreg
#endif //CRITICAL_SECTION_START

// look here for descriptions of gcodes: http://linuxcnc.org/handbook/gcode/g-code.html
// http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes

//Implemented Codes
//-------------------
// G0  -> G1
// G1  - Coordinated Movement X Y Z E
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
// M80  - Turn on Power Supply
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
// M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
// M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000)
// M301 - Set PID parameters P I and D

//Stepper Movement Variables

char axis_codes[NUM_AXIS] = {
  'X', 'Y', 'Z', 'E'};
float destination[NUM_AXIS] = {
  0.0, 0.0, 0.0, 0.0};
float current_position[NUM_AXIS] = {
  0.0, 0.0, 0.0, 0.0};
bool home_all_axis = true;
long feedrate = 1500, next_feedrate, saved_feedrate;
long gcode_N, gcode_LastN;
bool relative_mode = false;  //Determines Absolute or Relative Coordinates
bool relative_mode_e = false;  //Determines Absolute or Relative E Codes while in Absolute Coordinates mode. E is always relative in Relative Coordinates mode.
unsigned long axis_steps_per_sqr_second[NUM_AXIS];

// comm variables
#define MAX_CMD_SIZE 96
#define BUFSIZE 8
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

// Manage heater variables.

int target_raw = 0;
int current_raw = 0;
unsigned char temp_meas_ready = false;

#ifdef PIDTEMP
  double temp_iState = 0;
  double temp_dState = 0;
  double pTerm;
  double iTerm;
  double dTerm;
      //int output;
  double pid_error;
  double temp_iState_min;
  double temp_iState_max;
  double pid_setpoint = 0.0;
  double pid_input;
  double pid_output;
  bool pid_reset;
#endif //PIDTEMP

#ifdef WATCHPERIOD
int watch_raw = -1000;
unsigned long watchmillis = 0;
#endif //WATCHPERIOD
#ifdef MINTEMP
int minttemp = temp2analogh(MINTEMP);
#endif //MINTEMP
#ifdef MAXTEMP
int maxttemp = temp2analogh(MAXTEMP);
#endif //MAXTEMP

//Inactivity shutdown variables
unsigned long previous_millis_cmd = 0;
unsigned long max_inactive_time = 0;
unsigned long stepper_inactive_time = 0;

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
    sdactive = true;
#endif //SDSS
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


void setup()
{ 
  Serial.begin(BAUDRATE);
  Serial.print("Marlin ");
  Serial.println(version_string);
  Serial.println("start");

  for(int i = 0; i < BUFSIZE; i++){
    fromsd[i] = false;
  }

  //Initialize Dir Pins
#if X_DIR_PIN > -1
  SET_OUTPUT(X_DIR_PIN);
#endif
#if Y_DIR_PIN > -1 
  SET_OUTPUT(Y_DIR_PIN);
#endif
#if Z_DIR_PIN > -1 
  SET_OUTPUT(Z_DIR_PIN);
#endif
#if E_DIR_PIN > -1 
  SET_OUTPUT(E_DIR_PIN);
#endif

  //Initialize Enable Pins - steppers default to disabled.

#if (X_ENABLE_PIN > -1)
  SET_OUTPUT(X_ENABLE_PIN);
  if(!X_ENABLE_ON) WRITE(X_ENABLE_PIN,HIGH);
#endif
#if (Y_ENABLE_PIN > -1)
  SET_OUTPUT(Y_ENABLE_PIN);
  if(!Y_ENABLE_ON) WRITE(Y_ENABLE_PIN,HIGH);
#endif
#if (Z_ENABLE_PIN > -1)
  SET_OUTPUT(Z_ENABLE_PIN);
  if(!Z_ENABLE_ON) WRITE(Z_ENABLE_PIN,HIGH);
#endif
#if (E_ENABLE_PIN > -1)
  SET_OUTPUT(E_ENABLE_PIN);
  if(!E_ENABLE_ON) WRITE(E_ENABLE_PIN,HIGH);
#endif

  //endstops and pullups
#ifdef ENDSTOPPULLUPS
#if X_MIN_PIN > -1
  SET_INPUT(X_MIN_PIN); 
  WRITE(X_MIN_PIN,HIGH);
#endif
#if X_MAX_PIN > -1
  SET_INPUT(X_MAX_PIN); 
  WRITE(X_MAX_PIN,HIGH);
#endif
#if Y_MIN_PIN > -1
  SET_INPUT(Y_MIN_PIN); 
  WRITE(Y_MIN_PIN,HIGH);
#endif
#if Y_MAX_PIN > -1
  SET_INPUT(Y_MAX_PIN); 
  WRITE(Y_MAX_PIN,HIGH);
#endif
#if Z_MIN_PIN > -1
  SET_INPUT(Z_MIN_PIN); 
  WRITE(Z_MIN_PIN,HIGH);
#endif
#if Z_MAX_PIN > -1
  SET_INPUT(Z_MAX_PIN); 
  WRITE(Z_MAX_PIN,HIGH);
#endif
#else //ENDSTOPPULLUPS
#if X_MIN_PIN > -1
  SET_INPUT(X_MIN_PIN); 
#endif
#if X_MAX_PIN > -1
  SET_INPUT(X_MAX_PIN); 
#endif
#if Y_MIN_PIN > -1
  SET_INPUT(Y_MIN_PIN); 
#endif
#if Y_MAX_PIN > -1
  SET_INPUT(Y_MAX_PIN); 
#endif
#if Z_MIN_PIN > -1
  SET_INPUT(Z_MIN_PIN); 
#endif
#if Z_MAX_PIN > -1
  SET_INPUT(Z_MAX_PIN); 
#endif
#endif //ENDSTOPPULLUPS

#if (HEATER_0_PIN > -1) 
  SET_OUTPUT(HEATER_0_PIN);
#endif  
#if (HEATER_1_PIN > -1) 
  SET_OUTPUT(HEATER_1_PIN);
#endif  

  //Initialize Step Pins
#if (X_STEP_PIN > -1) 
  SET_OUTPUT(X_STEP_PIN);
#endif  
#if (Y_STEP_PIN > -1) 
  SET_OUTPUT(Y_STEP_PIN);
#endif  
#if (Z_STEP_PIN > -1) 
  SET_OUTPUT(Z_STEP_PIN);
#endif  
#if (E_STEP_PIN > -1) 
  SET_OUTPUT(E_STEP_PIN);
#endif  
  for(int i=0; i < NUM_AXIS; i++){
    axis_steps_per_sqr_second[i] = max_acceleration_units_per_sq_second[i] * axis_steps_per_unit[i];
  }

#ifdef PIDTEMP
  temp_iState_min = 0.0;
  temp_iState_max = PID_INTEGRAL_DRIVE_MAX / Ki;
#endif //PIDTEMP

#ifdef SDSUPPORT
  //power to SD reader
#if SDPOWER > -1
  SET_OUTPUT(SDPOWER); 
  WRITE(SDPOWER,HIGH);
#endif //SDPOWER
  initsd();

#endif //SDSUPPORT
  plan_init();  // Initialize planner;
  st_init();    // Initialize stepper;
  tp_init();    // Initialize temperature loop
}


void loop()
{
  if(buflen<3)
    get_command();

  if(buflen){
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
      for(int i=0; i < NUM_AXIS; i++) {
        destination[i] = current_position[i];
      }
      feedrate = 0;

      home_all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2])));

      if((home_all_axis) || (code_seen(axis_codes[X_AXIS]))) {
        if ((X_MIN_PIN > -1 && X_HOME_DIR==-1) || (X_MAX_PIN > -1 && X_HOME_DIR==1)){
          st_synchronize();
          current_position[X_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[X_AXIS] = 1.5 * X_MAX_LENGTH * X_HOME_DIR;
          feedrate = homing_feedrate[X_AXIS];
          prepare_move();

          st_synchronize();        
          current_position[X_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[X_AXIS] = -5 * X_HOME_DIR;
          prepare_move();

          st_synchronize();         
          destination[X_AXIS] = 10 * X_HOME_DIR;
          feedrate = homing_feedrate[X_AXIS]/2 ;
          prepare_move();
          st_synchronize();

          current_position[X_AXIS] = (X_HOME_DIR == -1) ? 0 : X_MAX_LENGTH;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[X_AXIS] = current_position[X_AXIS];
          feedrate = 0;
        }
      }

      if((home_all_axis) || (code_seen(axis_codes[Y_AXIS]))) {
        if ((Y_MIN_PIN > -1 && Y_HOME_DIR==-1) || (Y_MAX_PIN > -1 && Y_HOME_DIR==1)){
          current_position[Y_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Y_AXIS] = 1.5 * Y_MAX_LENGTH * Y_HOME_DIR;
          feedrate = homing_feedrate[Y_AXIS];
          prepare_move();
          st_synchronize();

          current_position[Y_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Y_AXIS] = -5 * Y_HOME_DIR;
          prepare_move();
          st_synchronize();

          destination[Y_AXIS] = 10 * Y_HOME_DIR;
          feedrate = homing_feedrate[Y_AXIS]/2;
          prepare_move();
          st_synchronize();

          current_position[Y_AXIS] = (Y_HOME_DIR == -1) ? 0 : Y_MAX_LENGTH;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Y_AXIS] = current_position[Y_AXIS];
          feedrate = 0;
        }
      }

      if((home_all_axis) || (code_seen(axis_codes[Z_AXIS]))) {
        if ((Z_MIN_PIN > -1 && Z_HOME_DIR==-1) || (Z_MAX_PIN > -1 && Z_HOME_DIR==1)){
          current_position[Z_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Z_AXIS] = 1.5 * Z_MAX_LENGTH * Z_HOME_DIR;
          feedrate = homing_feedrate[Z_AXIS];
          prepare_move();
          st_synchronize();

          current_position[Z_AXIS] = 0;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Z_AXIS] = -2 * Z_HOME_DIR;
          prepare_move();
          st_synchronize();

          destination[Z_AXIS] = 3 * Z_HOME_DIR;
          feedrate = homing_feedrate[Z_AXIS]/2;
          prepare_move();
          st_synchronize();

          current_position[Z_AXIS] = (Z_HOME_DIR == -1) ? 0 : Z_MAX_LENGTH;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          destination[Z_AXIS] = current_position[Z_AXIS];
          feedrate = 0;         
        }
      }       
      feedrate = saved_feedrate;
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
    case 104: // M104
#ifdef PID_OPENLOOP
      if (code_seen('S')) PidTemp_Output = code_value() * (PID_MAX/100.0);
      if(pid_output > PID_MAX) pid_output = PID_MAX;
      if(pid_output < 0) pid_output = 0;
#else //PID_OPENLOOP
      if (code_seen('S')) {
        target_raw = temp2analogh(code_value());
#ifdef PIDTEMP
        pid_setpoint = code_value();
#endif //PIDTEMP
      }
#ifdef WATCHPERIOD
      if(target_raw > current_raw){
        watchmillis = max(1,millis());
        watch_raw = current_raw;
      }
      else{
        watchmillis = 0;
      }
#endif //WATCHPERIOD
#endif //PID_OPENLOOP
      break;
    case 105: // M105
      Serial.print("ok T:");
      Serial.println(analog2temp(current_raw)); 
      return;
      //break;
    case 109: // M109 - Wait for extruder heater to reach target.
      if (code_seen('S')) {
        target_raw = temp2analogh(code_value());
#ifdef PIDTEMP
        pid_setpoint = code_value();
#endif //PIDTEMP
      }
#ifdef WATCHPERIOD
      if(target_raw>current_raw){
        watchmillis = max(1,millis());
        watch_raw = current_raw;
      }
      else{
        watchmillis = 0;
      }
#endif //WATCHERPERIOD
      codenum = millis(); 
      while(current_raw < target_raw) {
        if( (millis() - codenum) > 1000 ) //Print Temp Reading every 1 second while heating up.
        {
          Serial.print("T:");
          Serial.println( analog2temp(current_raw)); 
          codenum = millis(); 
        }
        manage_heater();
      }
      break;
    case 190:
      break;
    case 82:
      axis_relative_modes[3] = false;
      break;
    case 83:
      axis_relative_modes[3] = true;
      break;
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
      Serial.println(current_position[E_AXIS]);
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
#ifdef PIDTEMP
    case 301: // M301
      if(code_seen('P')) Kp = code_value();
      if(code_seen('I')) Ki = code_value()*PID_dT;
      if(code_seen('D')) Kd = code_value()/PID_dT;
      Serial.print("Kp ");Serial.println(Kp);
      Serial.print("Ki ");Serial.println(Ki/PID_dT);
      Serial.print("Kd ");Serial.println(Kd*PID_dT);
      temp_iState_min = 0.0;
      temp_iState_max = PID_INTEGRAL_DRIVE_MAX / Ki;
      break;
#endif //PIDTEMP
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
  if (min_software_endstops) {
    if (destination[X_AXIS] < 0) destination[X_AXIS] = 0.0;
    if (destination[Y_AXIS] < 0) destination[Y_AXIS] = 0.0;
    if (destination[Z_AXIS] < 0) destination[Z_AXIS] = 0.0;
  }

  if (max_software_endstops) {
    if (destination[X_AXIS] > X_MAX_LENGTH) destination[X_AXIS] = X_MAX_LENGTH;
    if (destination[Y_AXIS] > Y_MAX_LENGTH) destination[Y_AXIS] = Y_MAX_LENGTH;
    if (destination[Z_AXIS] > Z_MAX_LENGTH) destination[Z_AXIS] = Z_MAX_LENGTH;
  }

  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60.0);
  for(int i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}

void manage_heater()
{
  float pid_input;
  float pid_output;
  if(temp_meas_ready != true)
    return;

CRITICAL_SECTION_START;
  temp_meas_ready = false;
CRITICAL_SECTION_END;

#ifdef PIDTEMP
  pid_input = analog2temp(current_raw);

#ifndef PID_OPENLOOP
  pid_error = pid_setpoint - pid_input;
  if(pid_error > 10){
    pid_output = PID_MAX;
    pid_reset = true;
  }
  else if(pid_error < -10) {
    pid_output = 0;
    pid_reset = true;
  }
  else {
    if(pid_reset == true) {
      temp_iState = 0.0;
      pid_reset = false;
    }
    pTerm = Kp * pid_error;
    temp_iState += pid_error;
    temp_iState = constrain(temp_iState, temp_iState_min, temp_iState_max);
    iTerm = Ki * temp_iState;
    #define K1 0.8
    #define K2 (1.0-K1)
    dTerm = (Kd * (pid_input - temp_dState))*K2 + (K1 * dTerm);
    temp_dState = pid_input;
    pid_output = constrain(pTerm + iTerm - dTerm, 0, PID_MAX);
  }
#endif //PID_OPENLOOP
#ifdef PID_DEBUG
   Serial.print(" Input ");
   Serial.print(pid_input);
   Serial.print(" Output ");
   Serial.print(pid_output);    
   Serial.print(" pTerm ");
   Serial.print(pTerm); 
   Serial.print(" iTerm ");
   Serial.print(iTerm); 
   Serial.print(" dTerm ");
   Serial.print(dTerm); 
   Serial.println();
#endif //PID_DEBUG
  OCR2B = pid_output;
#endif //PIDTEMP
}


int temp2analogu(int celsius, const short table[][2], int numtemps) {
  int raw = 0;
  byte i;

  for (i=1; i<numtemps; i++) {
    if (table[i][1] < celsius) {
      raw = table[i-1][0] + 
           (celsius - table[i-1][1]) * 
           (table[i][0] - table[i-1][0]) /
           (table[i][1] - table[i-1][1]);

    break;
    }
  }
  // Overflow: Set to last value in the table
  if (i == numtemps) raw = table[i-1][0];

  return 16383 - raw;
}

float analog2tempu(int raw,const short table[][2], int numtemps) {
  float celsius = 0.0;
  byte i;

  raw = 16383 - raw;
  for (i=1; i<numtemps; i++) {
    if (table[i][0] > raw) {
       celsius  = (float)table[i-1][1] + 
                  (float)(raw - table[i-1][0]) * 
                  (float)(table[i][1] - table[i-1][1]) /
                 (float)(table[i][0] - table[i-1][0]);

      break;
    }
  }
  // Overflow: Set to last value in the table
  if (i == numtemps) celsius = table[i-1][1];

  return celsius;
}


inline void kill()
{
  target_raw=0;
#ifdef PIDTEMP
  pid_setpoint = 0.0;
#endif //PIDTEMP
  OCR2B = 0;
  WRITE(HEATER_0_PIN,LOW);

  disable_x();
  disable_y();
  disable_z();
  disable_e();

}

inline void manage_inactivity(byte debug) { 
  if( (millis()-previous_millis_cmd) >  max_inactive_time ) if(max_inactive_time) kill(); 
  if( (millis()-previous_millis_cmd) >  stepper_inactive_time ) if(stepper_inactive_time) { 
    disable_x(); 
    disable_y(); 
    disable_z(); 
    disable_e(); 
  }
  check_axes_activity();
}

// Planner

/*  
 Reasoning behind the mathematics in this module (in the key of 'Mathematica'):
 
 s == speed, a == acceleration, t == time, d == distance
 
 Basic definitions:
 
 Speed[s_, a_, t_] := s + (a*t) 
 Travel[s_, a_, t_] := Integrate[Speed[s, a, t], t]
 
 Distance to reach a specific speed with a constant acceleration:
 
 Solve[{Speed[s, a, t] == m, Travel[s, a, t] == d}, d, t]
 d -> (m^2 - s^2)/(2 a) --> estimate_acceleration_distance()
 
 Speed after a given distance of travel with constant acceleration:
 
 Solve[{Speed[s, a, t] == m, Travel[s, a, t] == d}, m, t]
 m -> Sqrt[2 a d + s^2]    
 
 DestinationSpeed[s_, a_, d_] := Sqrt[2 a d + s^2]
 
 When to start braking (di) to reach a specified destionation speed (s2) after accelerating
 from initial speed s1 without ever stopping at a plateau:
 
 Solve[{DestinationSpeed[s1, a, di] == DestinationSpeed[s2, a, d - di]}, di]
 di -> (2 a d - s1^2 + s2^2)/(4 a) --> intersection_distance()
 
 IntersectionDistance[s1_, s2_, a_, d_] := (2 a d - s1^2 + s2^2)/(4 a)
 */


// The number of linear motions that can be in the plan at any give time
#define BLOCK_BUFFER_SIZE 16
#define BLOCK_BUFFER_MASK 0x0f

static block_t block_buffer[BLOCK_BUFFER_SIZE];            // A ring buffer for motion instructions
static volatile unsigned char block_buffer_head;           // Index of the next block to be pushed
static volatile unsigned char block_buffer_tail;           // Index of the block to process now

// The current position of the tool in absolute steps
static long position[4];   

#define ONE_MINUTE_OF_MICROSECONDS 60000000.0

// Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the 
// given acceleration:
inline long estimate_acceleration_distance(long initial_rate, long target_rate, long acceleration) {
  return(
  (target_rate*target_rate-initial_rate*initial_rate)/
    (2L*acceleration)
    );
}

// This function gives you the point at which you must start braking (at the rate of -acceleration) if 
// you started at speed initial_rate and accelerated until this point and want to end at the final_rate after
// a total travel of distance. This can be used to compute the intersection point between acceleration and
// deceleration in the cases where the trapezoid has no plateau (i.e. never reaches maximum speed)

inline long intersection_distance(long initial_rate, long final_rate, long acceleration, long distance) {
  return(
  (2*acceleration*distance-initial_rate*initial_rate+final_rate*final_rate)/
    (4*acceleration)
    );
}

// Calculates trapezoid parameters so that the entry- and exit-speed is compensated by the provided factors.

void calculate_trapezoid_for_block(block_t *block, float entry_speed, float exit_speed) {
  if(block->busy == true) return; // If block is busy then bail out.
  float entry_factor = entry_speed / block->nominal_speed;
  float exit_factor = exit_speed / block->nominal_speed;
  long initial_rate = ceil(block->nominal_rate*entry_factor);
  long final_rate = ceil(block->nominal_rate*exit_factor);
  
#ifdef ADVANCE
  long initial_advance = block->advance*entry_factor*entry_factor;
  long final_advance = block->advance*exit_factor*exit_factor;
#endif // ADVANCE

  // Limit minimal step rate (Otherwise the timer will overflow.)
  if(initial_rate <120) initial_rate=120;
  if(final_rate < 120) final_rate=120;
  
  // Calculate the acceleration steps
  long acceleration = block->acceleration_st;
  long accelerate_steps = estimate_acceleration_distance(initial_rate, block->nominal_rate, acceleration);
  long decelerate_steps = estimate_acceleration_distance(final_rate, block->nominal_rate, acceleration);
  // Calculate the size of Plateau of Nominal Rate. 
  long plateau_steps = block->step_event_count-accelerate_steps-decelerate_steps;

  // Is the Plateau of Nominal Rate smaller than nothing? That means no cruising, and we will
  // have to use intersection_distance() to calculate when to abort acceleration and start braking 
  // in order to reach the final_rate exactly at the end of this block.
  if (plateau_steps < 0) {  
    accelerate_steps = intersection_distance(initial_rate, final_rate, acceleration, block->step_event_count);
    plateau_steps = 0;
  }  

  long decelerate_after = accelerate_steps+plateau_steps;
  long acceleration_rate = (long)((float)acceleration * 8.388608);

  CRITICAL_SECTION_START;  // Fill variables used by the stepper in a critical section
  if(block->busy == false) { // Don't update variables if block is busy.
    block->accelerate_until = accelerate_steps;
    block->decelerate_after = decelerate_after;
    block->acceleration_rate = acceleration_rate;
    block->initial_rate = initial_rate;
    block->final_rate = final_rate;
#ifdef ADVANCE
    block->initial_advance = initial_advance;
    block->final_advance = final_advance;
#endif ADVANCE
  }
  CRITICAL_SECTION_END;
}                    

// Calculates the maximum allowable speed at this point when you must be able to reach target_velocity using the 
// acceleration within the allotted distance.
inline float max_allowable_speed(float acceleration, float target_velocity, float distance) {
  return(
  sqrt(target_velocity*target_velocity-2*acceleration*60*60*distance)
    );
}

// "Junction jerk" in this context is the immediate change in speed at the junction of two blocks.
// This method will calculate the junction jerk as the euclidean distance between the nominal 
// velocities of the respective blocks.
inline float junction_jerk(block_t *before, block_t *after) {
  return(sqrt(
    pow((before->speed_x-after->speed_x), 2)+
    pow((before->speed_y-after->speed_y), 2)));
}

// Return the safe speed which is max_jerk/2, e.g. the 
// speed under which you cannot exceed max_jerk no matter what you do.
float safe_speed(block_t *block) {
  float safe_speed;
  safe_speed = max_xy_jerk/2;  
  if(abs(block->speed_z) > max_z_jerk/2) safe_speed = max_z_jerk/2;
  if (safe_speed > block->nominal_speed) safe_speed = block->nominal_speed;
  return safe_speed;  
}

// The kernel called by planner_recalculate() when scanning the plan from last to first entry.
void planner_reverse_pass_kernel(block_t *previous, block_t *current, block_t *next) {
  if(!current) { 
    return; 
  }

  float entry_speed = current->nominal_speed;
  float exit_factor;
  float exit_speed;
  if (next) {
    exit_speed = next->entry_speed;
  } 
  else {
    exit_speed = safe_speed(current);
  }

  // Calculate the entry_factor for the current block. 
  if (previous) {
    // Reduce speed so that junction_jerk is within the maximum allowed
    float jerk = junction_jerk(previous, current);
    if((previous->steps_x == 0) && (previous->steps_y == 0)) {
      entry_speed = safe_speed(current);
    }
    else if (jerk > max_xy_jerk) {
      entry_speed = (max_xy_jerk/jerk) * entry_speed;
    } 
    if(abs(previous->speed_z - current->speed_z) > max_z_jerk) {
      entry_speed = (max_z_jerk/abs(previous->speed_z - current->speed_z)) * entry_speed;
    }
    // If the required deceleration across the block is too rapid, reduce the entry_factor accordingly.
    if (entry_speed > exit_speed) {
      float max_entry_speed = max_allowable_speed(-current->acceleration,exit_speed, current->millimeters);
      if (max_entry_speed < entry_speed) {
        entry_speed = max_entry_speed;
      }
    }    
  } 
  else {
    entry_speed = safe_speed(current);
  }
  // Store result
  current->entry_speed = entry_speed;
}

// planner_recalculate() needs to go over the current plan twice. Once in reverse and once forward. This 
// implements the reverse pass.
void planner_reverse_pass() {
  char block_index = block_buffer_head;
  block_index--;
  block_t *block[3] = { NULL, NULL, NULL };
  while(block_index != block_buffer_tail) {  
    block_index--;
    if(block_index < 0) block_index = BLOCK_BUFFER_SIZE-1;  
    block[2]= block[1];
    block[1]= block[0];
    block[0] = &block_buffer[block_index];
    planner_reverse_pass_kernel(block[0], block[1], block[2]);
  }
  planner_reverse_pass_kernel(NULL, block[0], block[1]);
}

// The kernel called by planner_recalculate() when scanning the plan from first to last entry.
void planner_forward_pass_kernel(block_t *previous, block_t *current, block_t *next) {
  if(!current) { 
    return; 
  }
  if(previous) {
    // If the previous block is an acceleration block, but it is not long enough to 
    // complete the full speed change within the block, we need to adjust out entry
    // speed accordingly. Remember current->entry_factor equals the exit factor of 
    // the previous block.
    if(previous->entry_speed < current->entry_speed) {
      float max_entry_speed = max_allowable_speed(-previous->acceleration, previous->entry_speed, previous->millimeters);
      if (max_entry_speed < current->entry_speed) {
        current->entry_speed = max_entry_speed;
      }
    }
  }
}

// planner_recalculate() needs to go over the current plan twice. Once in reverse and once forward. This 
// implements the forward pass.
void planner_forward_pass() {
  char block_index = block_buffer_tail;
  block_t *block[3] = {
    NULL, NULL, NULL  };

  while(block_index != block_buffer_head) {
    block[0] = block[1];
    block[1] = block[2];
    block[2] = &block_buffer[block_index];
    planner_forward_pass_kernel(block[0],block[1],block[2]);
    block_index = (block_index+1) & BLOCK_BUFFER_MASK;
  }
  planner_forward_pass_kernel(block[1], block[2], NULL);
}

// Recalculates the trapezoid speed profiles for all blocks in the plan according to the 
// entry_factor for each junction. Must be called by planner_recalculate() after 
// updating the blocks.
void planner_recalculate_trapezoids() {
  char block_index = block_buffer_tail;
  block_t *current;
  block_t *next = NULL;
  while(block_index != block_buffer_head) {
    current = next;
    next = &block_buffer[block_index];
    if (current) {
      calculate_trapezoid_for_block(current, current->entry_speed, next->entry_speed);      
    }
    block_index = (block_index+1) & BLOCK_BUFFER_MASK;
  }
  calculate_trapezoid_for_block(next, next->entry_speed, safe_speed(next));
}

// Recalculates the motion plan according to the following algorithm:
//
//   1. Go over every block in reverse order and calculate a junction speed reduction (i.e. block_t.entry_factor) 
//      so that:
//     a. The junction jerk is within the set limit
//     b. No speed reduction within one block requires faster deceleration than the one, true constant 
//        acceleration.
//   2. Go over every block in chronological order and dial down junction speed reduction values if 
//     a. The speed increase within one block would require faster accelleration than the one, true 
//        constant acceleration.
//
// When these stages are complete all blocks have an entry_factor that will allow all speed changes to 
// be performed using only the one, true constant acceleration, and where no junction jerk is jerkier than 
// the set limit. Finally it will:
//
//   3. Recalculate trapezoids for all blocks.

void planner_recalculate() {   
  planner_reverse_pass();
  planner_forward_pass();
  planner_recalculate_trapezoids();
}

void plan_init() {
  block_buffer_head = 0;
  block_buffer_tail = 0;
  memset(position, 0, sizeof(position)); // clear position
}


inline void plan_discard_current_block() {
  if (block_buffer_head != block_buffer_tail) {
    block_buffer_tail = (block_buffer_tail + 1) & BLOCK_BUFFER_MASK;  
  }
}

inline block_t *plan_get_current_block() {
  if (block_buffer_head == block_buffer_tail) { 
    return(NULL); 
  }
  block_t *block = &block_buffer[block_buffer_tail];
  block->busy = true;
  return(block);
}

void check_axes_activity() {
  unsigned char x_active = 0;
  unsigned char y_active = 0;  
  unsigned char z_active = 0;
  unsigned char e_active = 0;
  block_t *block;

  if(block_buffer_tail != block_buffer_head) {
    char block_index = block_buffer_tail;
    while(block_index != block_buffer_head) {
      block = &block_buffer[block_index];
      if(block->steps_x != 0) x_active++;
      if(block->steps_y != 0) y_active++;
      if(block->steps_z != 0) z_active++;
      if(block->steps_e != 0) e_active++;
      block_index = (block_index+1) & BLOCK_BUFFER_MASK;
    }
  }
  if((DISABLE_X) && (x_active == 0)) disable_x();
  if((DISABLE_Y) && (y_active == 0)) disable_y();
  if((DISABLE_Z) && (z_active == 0)) disable_z();
  if((DISABLE_E) && (e_active == 0)) disable_e();
}

// Add a new linear movement to the buffer. steps_x, _y and _z is the absolute position in 
// mm. Microseconds specify how many microseconds the move should take to perform. To aid acceleration
// calculation the caller must also provide the physical length of the line in millimeters.
void plan_buffer_line(float x, float y, float z, float e, float feed_rate) {
  // The target position of the tool in absolute steps
  // Calculate target position in absolute steps
  long target[4];
  target[X_AXIS] = lround(x*axis_steps_per_unit[X_AXIS]);
  target[Y_AXIS] = lround(y*axis_steps_per_unit[Y_AXIS]);
  target[Z_AXIS] = lround(z*axis_steps_per_unit[Z_AXIS]);     
  target[E_AXIS] = lround(e*axis_steps_per_unit[E_AXIS]);     
  
  // Calculate the buffer head after we push this byte
  int next_buffer_head = (block_buffer_head + 1) & BLOCK_BUFFER_MASK;	

  // If the buffer is full: good! That means we are well ahead of the robot. 
  // Rest here until there is room in the buffer.
  while(block_buffer_tail == next_buffer_head) { 
    manage_heater(); 
    manage_inactivity(1); 
  }

  // Prepare to set up new block
  block_t *block = &block_buffer[block_buffer_head];
  
  // Mark block as not busy (Not executed by the stepper interrupt)
  block->busy = false;

  // Number of steps for each axis
  block->steps_x = labs(target[X_AXIS]-position[X_AXIS]);
  block->steps_y = labs(target[Y_AXIS]-position[Y_AXIS]);
  block->steps_z = labs(target[Z_AXIS]-position[Z_AXIS]);
  block->steps_e = labs(target[E_AXIS]-position[E_AXIS]);
  block->step_event_count = max(block->steps_x, max(block->steps_y, max(block->steps_z, block->steps_e)));

  // Bail if this is a zero-length block
  if (block->step_event_count == 0) { 
    return; 
  };
  
  //enable active axes
  if(block->steps_x != 0) enable_x();
  if(block->steps_y != 0) enable_y();
  if(block->steps_z != 0) enable_z();
  if(block->steps_e != 0) enable_e();

  float delta_x_mm = (target[X_AXIS]-position[X_AXIS])/axis_steps_per_unit[X_AXIS];
  float delta_y_mm = (target[Y_AXIS]-position[Y_AXIS])/axis_steps_per_unit[Y_AXIS];
  float delta_z_mm = (target[Z_AXIS]-position[Z_AXIS])/axis_steps_per_unit[Z_AXIS];
  float delta_e_mm = (target[E_AXIS]-position[E_AXIS])/axis_steps_per_unit[E_AXIS];
  block->millimeters = sqrt(square(delta_x_mm) + square(delta_y_mm) + square(delta_z_mm) + square(delta_e_mm));

  unsigned long microseconds;
  microseconds = lround((block->millimeters/feed_rate)*1000000);
  
  // Calculate speed in mm/minute for each axis
  float multiplier = 60.0*1000000.0/microseconds;
  block->speed_z = delta_z_mm * multiplier; 
  block->speed_x = delta_x_mm * multiplier;
  block->speed_y = delta_y_mm * multiplier;
  block->speed_e = delta_e_mm * multiplier; 

  // Limit speed per axis
  float speed_factor = 1;
  float tmp_speed_factor;
  if(abs(block->speed_x) > max_feedrate[X_AXIS]) {
    speed_factor = max_feedrate[X_AXIS] / abs(block->speed_x);
  }
  if(abs(block->speed_y) > max_feedrate[Y_AXIS]){
    tmp_speed_factor = max_feedrate[Y_AXIS] / abs(block->speed_y);
    if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  if(abs(block->speed_z) > max_feedrate[Z_AXIS]){
    tmp_speed_factor = max_feedrate[Z_AXIS] / abs(block->speed_z);
    if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  if(abs(block->speed_e) > max_feedrate[E_AXIS]){
    tmp_speed_factor = max_feedrate[E_AXIS] / abs(block->speed_e);
    if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  multiplier = multiplier * speed_factor;
  block->speed_z = delta_z_mm * multiplier; 
  block->speed_x = delta_x_mm * multiplier;
  block->speed_y = delta_y_mm * multiplier;
  block->speed_e = delta_e_mm * multiplier; 
  block->nominal_speed = block->millimeters * multiplier;
  block->nominal_rate = ceil(block->step_event_count * multiplier / 60);  
  
  if(block->nominal_rate < 120) block->nominal_rate = 120;
  block->entry_speed = safe_speed(block);

  // Compute the acceleration rate for the trapezoid generator. 
  float travel_per_step = block->millimeters/block->step_event_count;
  if(block->steps_x == 0 && block->steps_y == 0 && block->steps_z == 0) {
    block->acceleration_st = ceil( (retract_acceleration)/travel_per_step); // convert to: acceleration steps/sec^2
  }
  else {
    block->acceleration_st = ceil( (acceleration)/travel_per_step);      // convert to: acceleration steps/sec^2
    // Limit acceleration per axis
    if((block->acceleration_st * block->steps_x / block->step_event_count) > axis_steps_per_sqr_second[X_AXIS])
      block->acceleration_st = axis_steps_per_sqr_second[X_AXIS];
    if((block->acceleration_st * block->steps_y / block->step_event_count) > axis_steps_per_sqr_second[Y_AXIS])
      block->acceleration_st = axis_steps_per_sqr_second[Y_AXIS];
    if((block->acceleration_st * block->steps_e / block->step_event_count) > axis_steps_per_sqr_second[E_AXIS])
      block->acceleration_st = axis_steps_per_sqr_second[E_AXIS];
    if(((block->acceleration_st / block->step_event_count) * block->steps_z ) > axis_steps_per_sqr_second[Z_AXIS])
      block->acceleration_st = axis_steps_per_sqr_second[Z_AXIS];
  }
  block->acceleration = block->acceleration_st * travel_per_step;
  
#ifdef ADVANCE
  // Calculate advance rate
  if((block->steps_e == 0) || (block->steps_x == 0 && block->steps_y == 0 && block->steps_z == 0)) {
    block->advance_rate = 0;
    block->advance = 0;
  }
  else {
    long acc_dist = estimate_acceleration_distance(0, block->nominal_rate, block->acceleration_st);
    float advance = (STEPS_PER_CUBIC_MM_E * EXTRUDER_ADVANCE_K) * 
      (block->speed_e * block->speed_e * EXTRUTION_AREA * EXTRUTION_AREA / 3600.0)*65536;
    block->advance = advance;
    if(acc_dist == 0) {
      block->advance_rate = 0;
    } 
    else {
      block->advance_rate = advance / (float)acc_dist;
    }
  }

#endif // ADVANCE

  // compute a preliminary conservative acceleration trapezoid
  float safespeed = safe_speed(block);
  calculate_trapezoid_for_block(block, safespeed, safespeed); 

  // Compute direction bits for this block
  block->direction_bits = 0;
  if (target[X_AXIS] < position[X_AXIS]) { 
    block->direction_bits |= (1<<X_AXIS); 
  }
  if (target[Y_AXIS] < position[Y_AXIS]) { 
    block->direction_bits |= (1<<Y_AXIS); 
  }
  if (target[Z_AXIS] < position[Z_AXIS]) { 
    block->direction_bits |= (1<<Z_AXIS); 
  }
  if (target[E_AXIS] < position[E_AXIS]) { 
    block->direction_bits |= (1<<E_AXIS); 
  }

  // Move buffer head
  block_buffer_head = next_buffer_head;     

  // Update position 
  memcpy(position, target, sizeof(target)); // position[] = target[]

  planner_recalculate();
  st_wake_up();
}

void plan_set_position(float x, float y, float z, float e)
{
  position[X_AXIS] = lround(x*axis_steps_per_unit[X_AXIS]);
  position[Y_AXIS] = lround(y*axis_steps_per_unit[Y_AXIS]);
  position[Z_AXIS] = lround(z*axis_steps_per_unit[Z_AXIS]);     
  position[E_AXIS] = lround(e*axis_steps_per_unit[E_AXIS]);     
}

// Stepper

// intRes = intIn1 * intIn2 >> 16
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 24 bit result
#define MultiU16X8toH16(intRes, charIn1, intIn2) \
asm volatile ( \
"clr r26 \n\t" \
"mul %A1, %B2 \n\t" \
"movw %A0, r0 \n\t" \
"mul %A1, %A2 \n\t" \
"add %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"lsr r0 \n\t" \
"adc %A0, r26 \n\t" \
"adc %B0, r26 \n\t" \
"clr r1 \n\t" \
: \
"=&r" (intRes) \
: \
"d" (charIn1), \
"d" (intIn2) \
: \
"r26" \
)

// intRes = longIn1 * longIn2 >> 24
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 48bit result
#define MultiU24X24toH16(intRes, longIn1, longIn2) \
asm volatile ( \
"clr r26 \n\t" \
"mul %A1, %B2 \n\t" \
"mov r27, r1 \n\t" \
"mul %B1, %C2 \n\t" \
"movw %A0, r0 \n\t" \
"mul %C1, %C2 \n\t" \
"add %B0, r0 \n\t" \
"mul %C1, %B2 \n\t" \
"add %A0, r0 \n\t" \
"adc %B0, r1 \n\t" \
"mul %A1, %C2 \n\t" \
"add r27, r0 \n\t" \
"adc %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"mul %B1, %B2 \n\t" \
"add r27, r0 \n\t" \
"adc %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"mul %C1, %A2 \n\t" \
"add r27, r0 \n\t" \
"adc %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"mul %B1, %A2 \n\t" \
"add r27, r1 \n\t" \
"adc %A0, r26 \n\t" \
"adc %B0, r26 \n\t" \
"lsr r27 \n\t" \
"adc %A0, r26 \n\t" \
"adc %B0, r26 \n\t" \
"clr r1 \n\t" \
: \
"=&r" (intRes) \
: \
"d" (longIn1), \
"d" (longIn2) \
: \
"r26" , "r27" \
)

// Some useful constants

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  TIMSK1 |= (1<<OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() TIMSK1 &= ~(1<<OCIE1A)

static block_t *current_block;  // A pointer to the block currently being traced

// Variables used by The Stepper Driver Interrupt
static unsigned char out_bits;        // The next stepping-bits to be output
static long counter_x,       // Counter variables for the bresenham line tracer
            counter_y, 
            counter_z,       
            counter_e;
static unsigned long step_events_completed; // The number of step events executed in the current block
static long advance_rate, advance, final_advance = 0;
static short old_advance = 0;
static short e_steps;
static unsigned char busy = false; // TRUE when SIG_OUTPUT_COMPARE1A is being serviced. Used to avoid retriggering that handler.
static long acceleration_time, deceleration_time;
static long accelerate_until, decelerate_after, acceleration_rate, initial_rate, final_rate, nominal_rate;
static unsigned short acc_step_rate; // needed for deccelaration start point



//         __________________________
//        /|                        |\     _________________         ^
//       / |                        | \   /|               |\        |
//      /  |                        |  \ / |               | \       s
//     /   |                        |   |  |               |  \      p
//    /    |                        |   |  |               |   \     e
//   +-----+------------------------+---+--+---------------+----+    e
//   |               BLOCK 1            |      BLOCK 2          |    d
//
//                           time ----->
// 
//  The trapezoid is the shape the speed curve over time. It starts at block->initial_rate, accelerates 
//  first block->accelerate_until step_events_completed, then keeps going at constant speed until 
//  step_events_completed reaches block->decelerate_after after which it decelerates until the trapezoid generator is reset.
//  The slope of acceleration is calculated with the leib ramp alghorithm.

void st_wake_up() {
  //  TCNT1 = 0;
  ENABLE_STEPPER_DRIVER_INTERRUPT();  
}

inline unsigned short calc_timer(unsigned short step_rate) {
  unsigned short timer;
  if(step_rate < 32) step_rate = 32;
  step_rate -= 32; // Correct for minimal speed
  if(step_rate >= (8*256)){ // higher step rate 
    unsigned short table_address = (unsigned short)&speed_lookuptable_fast[(unsigned char)(step_rate>>8)][0];
    unsigned char tmp_step_rate = (step_rate & 0x00ff);
    unsigned short gain = (unsigned short)pgm_read_word_near(table_address+2);
    MultiU16X8toH16(timer, tmp_step_rate, gain);
    timer = (unsigned short)pgm_read_word_near(table_address) - timer;
  }
  else { // lower step rates
    unsigned short table_address = (unsigned short)&speed_lookuptable_slow[0][0];
    table_address += ((step_rate)>>1) & 0xfffc;
    timer = (unsigned short)pgm_read_word_near(table_address);
    timer -= (((unsigned short)pgm_read_word_near(table_address+2) * (unsigned char)(step_rate & 0x0007))>>3);
  }
  if(timer < 100) timer = 100;
  return timer;
}

// Initializes the trapezoid generator from the current block. Called whenever a new 
// block begins.
inline void trapezoid_generator_reset() {
  accelerate_until = current_block->accelerate_until;
  decelerate_after = current_block->decelerate_after;
  acceleration_rate = current_block->acceleration_rate;
  initial_rate = current_block->initial_rate;
  final_rate = current_block->final_rate;
  nominal_rate = current_block->nominal_rate;
  advance = current_block->initial_advance;
  final_advance = current_block->final_advance;
  deceleration_time = 0;
  advance_rate = current_block->advance_rate;
  
  // step_rate to timer interval
  acc_step_rate = initial_rate;
  acceleration_time = calc_timer(acc_step_rate);
  OCR1A = acceleration_time;
}

// "The Stepper Driver Interrupt" - This timer interrupt is the workhorse.  
// It pops blocks from the block_buffer and executes them by pulsing the stepper pins appropriately. 
ISR(TIMER1_COMPA_vect)
{        
  if(busy){ /*Serial.println("BUSY")*/; 
    return; 
  } // The busy-flag is used to avoid reentering this interrupt

  busy = true;
  sei(); // Re enable interrupts (normally disabled while inside an interrupt handler)

  // If there is no current block, attempt to pop one from the buffer
  if (current_block == NULL) {
    // Anything in the buffer?
    current_block = plan_get_current_block();
    if (current_block != NULL) {
      trapezoid_generator_reset();
      counter_x = -(current_block->step_event_count >> 1);
      counter_y = counter_x;
      counter_z = counter_x;
      counter_e = counter_x;
      step_events_completed = 0;
      e_steps = 0;
    } 
    else {
      DISABLE_STEPPER_DRIVER_INTERRUPT();
    }    
  } 

  if (current_block != NULL) {
    // Set directions TO DO This should be done once during init of trapezoid. Endstops -> interrupt
    out_bits = current_block->direction_bits;

#ifdef ADVANCE
    // Calculate E early.
    counter_e += current_block->steps_e;
    if (counter_e > 0) {
      counter_e -= current_block->step_event_count;
      if ((out_bits & (1<<E_AXIS)) != 0) { // - direction
        CRITICAL_SECTION_START;
        e_steps--;
        CRITICAL_SECTION_END;
      }
      else {
        CRITICAL_SECTION_START;
        e_steps++;
        CRITICAL_SECTION_END;
      }
    }    
    // Do E steps + advance steps
    CRITICAL_SECTION_START;
    e_steps += ((advance >> 16) - old_advance);
    CRITICAL_SECTION_END;
    old_advance = advance >> 16;  
#endif //ADVANCE

    // Set direction en check limit switches
    if ((out_bits & (1<<X_AXIS)) != 0) {   // -direction
      WRITE(X_DIR_PIN, INVERT_X_DIR);
      if(READ(X_MIN_PIN) != ENDSTOPS_INVERTING) {
        step_events_completed = current_block->step_event_count;
      }
    }
    else // +direction
    WRITE(X_DIR_PIN,!INVERT_X_DIR);

    if ((out_bits & (1<<Y_AXIS)) != 0) {   // -direction
      WRITE(Y_DIR_PIN,INVERT_Y_DIR);
      if(READ(Y_MIN_PIN) != ENDSTOPS_INVERTING) {
        step_events_completed = current_block->step_event_count;
      }
    }
    else // +direction
    WRITE(Y_DIR_PIN,!INVERT_Y_DIR);

    if ((out_bits & (1<<Z_AXIS)) != 0) {   // -direction
      WRITE(Z_DIR_PIN,INVERT_Z_DIR);
      if(READ(Z_MIN_PIN) != ENDSTOPS_INVERTING) {
        step_events_completed = current_block->step_event_count;
      }
    }
    else // +direction
    WRITE(Z_DIR_PIN,!INVERT_Z_DIR);

#ifndef ADVANCE
    if ((out_bits & (1<<E_AXIS)) != 0)   // -direction
      WRITE(E_DIR_PIN,INVERT_E_DIR);
    else // +direction
      WRITE(E_DIR_PIN,!INVERT_E_DIR);
#endif //!ADVANCE

    counter_x += current_block->steps_x;
    if (counter_x > 0) {
      WRITE(X_STEP_PIN, HIGH);
      counter_x -= current_block->step_event_count;
      WRITE(X_STEP_PIN, LOW);
    }

    counter_y += current_block->steps_y;
    if (counter_y > 0) {
      WRITE(Y_STEP_PIN, HIGH);
      counter_y -= current_block->step_event_count;
      WRITE(Y_STEP_PIN, LOW);
    }

    counter_z += current_block->steps_z;
    if (counter_z > 0) {
      WRITE(Z_STEP_PIN, HIGH);
      counter_z -= current_block->step_event_count;
      WRITE(Z_STEP_PIN, LOW);
    }

#ifndef ADVANCE
    counter_e += current_block->steps_e;
    if (counter_e > 0) {
      WRITE(E_STEP_PIN, HIGH);
      counter_e -= current_block->step_event_count;
      WRITE(E_STEP_PIN, LOW);
    }
#endif //!ADVANCE

    // Calculare new timer value
    unsigned short timer;
    unsigned short step_rate;
    if (step_events_completed < accelerate_until) {
      MultiU24X24toH16(acc_step_rate, acceleration_time, acceleration_rate);
      acc_step_rate += initial_rate;
      
      // upper limit
      if(acc_step_rate > nominal_rate)
        acc_step_rate = nominal_rate;

      // step_rate to timer interval
      timer = calc_timer(acc_step_rate);
      advance += advance_rate;
      acceleration_time += timer;
      OCR1A = timer;
    } 
    else if (step_events_completed >= decelerate_after) {   
      MultiU24X24toH16(step_rate, deceleration_time, acceleration_rate);
      
      if(step_rate > acc_step_rate) { // Check step_rate stays positive
        step_rate = final_rate;
      }
      else {
        step_rate = acc_step_rate - step_rate; // Decelerate from aceleration end point.
      }

      // lower limit
      if(step_rate < final_rate)
        step_rate = final_rate;

      // step_rate to timer interval
      timer = calc_timer(step_rate);
#ifdef ADVANCE
      advance -= advance_rate;
      if(advance < final_advance)
        advance = final_advance;
#endif //ADVANCE
      deceleration_time += timer;
      OCR1A = timer;
    }       
    // If current block is finished, reset pointer 
    step_events_completed += 1;  
    if (step_events_completed >= current_block->step_event_count) {
      current_block = NULL;
      plan_discard_current_block();
    }   
  } 
  busy=false;
}

#ifdef ADVANCE

unsigned char old_OCR0A;
// Timer interrupt for E. e_steps is set in the main routine;
// Timer 0 is shared with millies
ISR(TIMER0_COMPA_vect)
{
  // Critical section needed because Timer 1 interrupt has higher priority. 
  // The pin set functions are placed on trategic position to comply with the stepper driver timing.
  WRITE(E_STEP_PIN, LOW);
  // Set E direction (Depends on E direction + advance)
  if (e_steps < 0) {
    WRITE(E_DIR_PIN,INVERT_E_DIR);    
    e_steps++;
    WRITE(E_STEP_PIN, HIGH);
  } 
  if (e_steps > 0) {
    WRITE(E_DIR_PIN,!INVERT_E_DIR);
    e_steps--;
    WRITE(E_STEP_PIN, HIGH);
  }
  old_OCR0A += 25; // 10kHz interrupt
  OCR0A = old_OCR0A;
}
#endif // ADVANCE

void st_init()
{
  // waveform generation = 0100 = CTC
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |=  (1<<WGM12);
  TCCR1A &= ~(1<<WGM11); 
  TCCR1A &= ~(1<<WGM10);

  // output mode = 00 (disconnected)
  TCCR1A &= ~(3<<COM1A0); 
  TCCR1A &= ~(3<<COM1B0); 
  TCCR1B = (TCCR1B & ~(0x07<<CS10)) | (2<<CS10); // 2MHz timer

  OCR1A = 0x4000;
  DISABLE_STEPPER_DRIVER_INTERRUPT();  

#ifdef ADVANCE
  e_steps = 0;
  TIMSK0 |= (1<<OCIE0A);
#endif //ADVANCE
  sei();
}

// Block until all buffered steps are executed
void st_synchronize()
{
  while(plan_get_current_block()) {
    manage_heater();
    manage_inactivity(1);
  }   
}

// Temperature loop

void tp_init()
{
  DIDR0 = 1<<5; // TEMP_0_PIN for GEN6
  ADMUX = ((1 << REFS0) | (5 & 0x07));
  ADCSRA = 1<<ADEN | 1<<ADSC | 1<<ADIF | 0x07; // ADC enable, Clear interrupt, 1/128 prescaler.
  TCCR2B = 0;     //Stop timer in case of running

#ifdef PIDTEMP
  TCCR2A = 0x23;  //OC2A disable; FastPWM noninverting; FastPWM mode 7
#else
  TCCR2A = 0x03;  //OC2A disable; FastPWM noninverting; FastPWM mode 7
#endif //PIDTEMP
  OCR2A = 156;    //Period is ~10ms
  OCR2B = 0;      //Duty Cycle for heater pin is 0 (startup)
  TIMSK2 = 0x01;  //Enable overflow interrupt
  TCCR2B = 0x0F;  //1/1024 prescaler, start
}

static unsigned char temp_count = 0;
static unsigned long raw_temp_value = 0;

ISR(TIMER2_OVF_vect)
{
  //  uint8_t low, high;

  //  low = ADCL;
  //  high = ADCH;
  raw_temp_value += ADC;
  //  raw_temp_value = (ADCH <<8) | ADCL;
  ADCSRA = 1<<ADEN | 1<<ADSC | 1<<ADIF | 0x07; // ADC enable, Clear interrupt, Enable Interrupt, 1/128 prescaler.
  //  raw_temp_value += (high <<8) | low;
  temp_count++;

  if(temp_count >= 16)
  {
    current_raw = 16383 - raw_temp_value;
    temp_meas_ready = true;
    temp_count = 0;
    raw_temp_value = 0;
#ifdef MAXTEMP
    if(current_raw >= maxttemp) {
      target_raw = 0;
#ifdef PIDTEMP
      OCR2B = 0;
#else
      WRITE(HEATER_0_PIN,LOW);
#endif //PIDTEMP
    }
#endif //MAXTEMP
#ifdef MINTEMP
    if(current_raw <= minttemp) {
      target_raw = 0;
#ifdef PIDTEMP
      OCR2B = 0;
#else
      WRITE(HEATER_0_PIN,LOW);
#endif //PIDTEMP
    }
#endif //MAXTEMP
#ifndef PIDTEMP
    if(current_raw >= target_raw)
    {
      WRITE(HEATER_0_PIN,LOW);
    }
    else 
    {
      WRITE(HEATER_0_PIN,HIGH);
    }
#endif //PIDTEMP
  }
}


