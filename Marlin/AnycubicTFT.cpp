/*
  AnycubicTFT.cpp  --- Support for Anycubic i3 Mega TFT
  Created by Christian Hopp on 09.12.17.
    
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
        
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
                
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "MarlinConfig.h"
#include "Marlin.h"
#include "cardreader.h"
#include "planner.h"
#include "temperature.h"
#include "language.h"
#include "stepper.h"
#include "serial.h"

#ifdef ANYCUBIC_TFT_MODEL
#include "AnycubicTFT.h"
#include "AnycubicSerial.h"

char _conv[8];

char *itostr2(const uint8_t &x)
{
  //sprintf(conv,"%5.1f",x);
  int xx=x;
  _conv[0]=(xx/10)%10+'0';
  _conv[1]=(xx)%10+'0';
  _conv[2]=0;
  return _conv;
}

#ifndef ULTRA_LCD
#define DIGIT(n) ('0' + (n))
#define DIGIMOD(n, f) DIGIT((n)/(f) % 10)
#define RJDIGIT(n, f) ((n) >= (f) ? DIGIMOD(n, f) : ' ')
#define MINUSOR(n, alt) (n >= 0 ? (alt) : (n = -n, '-'))


char* itostr3(const int x) {
  int xx = x;
  _conv[4] = MINUSOR(xx, RJDIGIT(xx, 100));
  _conv[5] = RJDIGIT(xx, 10);
  _conv[6] = DIGIMOD(xx, 1);
  return &_conv[4];
}

// Convert signed float to fixed-length string with 023.45 / -23.45 format
char *ftostr32(const float &x) {
  long xx = x * 100;
  _conv[1] = MINUSOR(xx, DIGIMOD(xx, 10000));
  _conv[2] = DIGIMOD(xx, 1000);
  _conv[3] = DIGIMOD(xx, 100);
  _conv[4] = '.';
  _conv[5] = DIGIMOD(xx, 10);
  _conv[6] = DIGIMOD(xx, 1);
  return &_conv[1];
}
#endif

AnycubicTFTClass::AnycubicTFTClass() {
}

void AnycubicTFTClass::Setup() {
  AnycubicSerial.begin(115200);
  //ANYCUBIC_SERIAL_START();
  ANYCUBIC_SERIAL_PROTOCOLPGM("J17"); // J17 Main board reset
  ANYCUBIC_SERIAL_ENTER();
  delay(10);
  ANYCUBIC_SERIAL_PROTOCOLPGM("J12"); // J12 Ready
  ANYCUBIC_SERIAL_ENTER();

#if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
  pinMode(SD_DETECT_PIN, INPUT);
  WRITE(SD_DETECT_PIN, HIGH);
#endif
  
#if ENABLED(ANYCUBIC_FILAMENT_RUNOUT_SENSOR)
  pinMode(FIL_RUNOUT_PIN,INPUT);
  WRITE(FIL_RUNOUT_PIN,HIGH);
  if(READ(FIL_RUNOUT_PIN)==true)
  {
    ANYCUBIC_SERIAL_PROTOCOLPGM("J15"); //J15 FILAMENT LACK
    ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
    SERIAL_ECHOLNPGM("TFT Serial Debug: Filament runout... J15");
#endif
  }
#endif
  
  SelectedDirectory[0]=0;
  SpecialMenu=false;
}

void AnycubicTFTClass::WriteOutageEEPromData() {
  int pos=E2END-256;
  
}

void AnycubicTFTClass::ReadOutageEEPromData() {
  int pos=E2END-256;

}

void AnycubicTFTClass::KillTFT()
{
  ANYCUBIC_SERIAL_PROTOCOLPGM("J11"); // J11 Kill
  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
  SERIAL_ECHOLNPGM("TFT Serial Debug: Kill command... J11");
#endif
}


void AnycubicTFTClass::StartPrint(){
  if (TFTstate==ANYCUBIC_TFT_STATE_SDPAUSE) {
#ifndef ADVANCED_PAUSE_FEATURE
    enqueue_and_echo_commands_P(PSTR("G91\nG1 Z-10 F240\nG90"));
#endif
  }
  starttime=millis();
  card.startFileprint();
  TFTstate=ANYCUBIC_TFT_STATE_SDPRINT;
}

void AnycubicTFTClass::PausePrint(){
  card.pauseSDPrint();
  TFTstate=ANYCUBIC_TFT_STATE_SDPAUSE_REQ;
#ifdef ANYCUBIC_FILAMENT_RUNOUT_SENSOR  
  if(FilamentTestStatus) {
    ANYCUBIC_SERIAL_PROTOCOLPGM("J05");// J05 pausing
    ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
    SERIAL_ECHOLNPGM("TFT Serial Debug: SD print paused... J05");
#endif
  } else {
    // Pause because of "out of filament"
    ANYCUBIC_SERIAL_PROTOCOLPGM("J23"); //J23 FILAMENT LACK with the prompt box don't disappear
    ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
    SERIAL_ECHOLNPGM("TFT Serial Debug: Filament runout while printing... J23");
#endif
  }
#endif
}

void AnycubicTFTClass::StopPrint(){
  card.stopSDPrint();
  clear_command_queue();
  quickstop_stepper();
  print_job_timer.stop();
  thermalManager.disable_all_heaters();
  #if FAN_COUNT > 0
    for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
  #endif
  TFTstate=ANYCUBIC_TFT_STATE_SDSTOP_REQ;
}


float AnycubicTFTClass::CodeValue()
{
  return (strtod(&TFTcmdbuffer[TFTbufindr][TFTstrchr_pointer - TFTcmdbuffer[TFTbufindr] + 1], NULL));
}

bool AnycubicTFTClass::CodeSeen(char code)
{
  TFTstrchr_pointer = strchr(TFTcmdbuffer[TFTbufindr], code);
  return (TFTstrchr_pointer != NULL);  //Return True if a character was found
}

void AnycubicTFTClass::HandleSpecialMenu()
{
  if(strcmp(SelectedDirectory, "<special menu>")==0) {
    SpecialMenu=true;
  } else if (strcmp(SelectedDirectory, "<auto tune hotend pid>")==0) {
    SERIAL_PROTOCOLLNPGM("Special Menu: Auto Tune PID");
    enqueue_and_echo_commands_P(PSTR("M303 C8 S200"));
  } else if (strcmp(SelectedDirectory, "<auto bed leveling>")==0) {
    SERIAL_PROTOCOLLNPGM("Special Menu: Auto Bed Leveling");
    enqueue_and_echo_commands_P(PSTR("G28\nG29"));
  } else if (strcmp(SelectedDirectory, "<save eeprom>")==0) {
    SERIAL_PROTOCOLLNPGM("Special Menu: Save EEPROM");
    enqueue_and_echo_commands_P(PSTR("M500"));
  } else if (strcmp(SelectedDirectory, "<read eeprom>")==0) {
    SERIAL_PROTOCOLLNPGM("Special Menu: Read EEPROM");
    enqueue_and_echo_commands_P(PSTR("M501"));
  } else if (strcmp(SelectedDirectory, "<exit>")==0) {
    SpecialMenu=false;
  }
}

void AnycubicTFTClass::Ls()
{
  if (SpecialMenu) {
    switch (filenumber) {
      case 0: // First Page
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Exit>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Exit>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Auto Tune Hotend PID>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Auto Tune Hotend PID>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Auto Bed Leveling>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Auto Bed Leveling>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Save EEPROM>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Save EEPROM>");
        break;

      case 4: // Second Page
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Read EEPROM>");
        ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Read EEPROM>");
        break;
        
      default:
        break;
    }
  }
  else if(card.cardOK)
  {
    uint16_t cnt=filenumber;
    uint16_t max_files;
    uint16_t dir_files=card.getnrfilenames();
    
    if((dir_files-filenumber)<4)
    {
      max_files=dir_files;
    } else {
      max_files=filenumber+3;
    }
      
    for(cnt=filenumber; cnt<=max_files; cnt++)
    {
      if (cnt==0) // Special Entry
      {
        if(strcmp(card.getWorkDirName(), "/") == 0) {
          ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Special Menu>");
          ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Special Menu>");
          SERIAL_PROTOCOL(cnt);
          SERIAL_PROTOCOLLNPGM("<Special_Menu>");
        } else {
          ANYCUBIC_SERIAL_PROTOCOLLNPGM("/..");
          ANYCUBIC_SERIAL_PROTOCOLLNPGM("/..");
          SERIAL_PROTOCOL(cnt);
          SERIAL_PROTOCOLLNPGM("/..");
        }
      } else {
        card.getfilename(cnt-1);
//      card.getfilename(cnt);
      
        if(card.filenameIsDir) {
          ANYCUBIC_SERIAL_PROTOCOLPGM("/");
          ANYCUBIC_SERIAL_PROTOCOLLN(card.filename);
          ANYCUBIC_SERIAL_PROTOCOLPGM("/");
          ANYCUBIC_SERIAL_PROTOCOLLN(card.longFilename);
          SERIAL_PROTOCOL(cnt);
          SERIAL_PROTOCOLPGM("/");
          SERIAL_PROTOCOLLN(card.longFilename);
        } else {
          ANYCUBIC_SERIAL_PROTOCOLLN(card.filename);
          ANYCUBIC_SERIAL_PROTOCOLLN(card.longFilename);
          SERIAL_PROTOCOL(cnt);
          SERIAL_PROTOCOLLN(card.longFilename);
        }
      }
    }
  } else {
    ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Special_Menu>");
    ANYCUBIC_SERIAL_PROTOCOLLNPGM("<Special_Menu>");
  }
}

void AnycubicTFTClass::CheckSDCardChange()
{
  if (LastSDstatus != IS_SD_INSERTED)
  {
    LastSDstatus = IS_SD_INSERTED;
    
    if (LastSDstatus)
    {
      card.initsd();
      ANYCUBIC_SERIAL_PROTOCOLPGM("J00"); // J00 SD Card inserted
      ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
      SERIAL_ECHOLNPGM("TFT Serial Debug: SD card inserted... J00");
#endif
    }
    else
    {
      ANYCUBIC_SERIAL_PROTOCOLPGM("J01"); // J01 SD Card removed
      ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
      SERIAL_ECHOLNPGM("TFT Serial Debug: SD card removed... J01");
#endif

    }
  }
}

void AnycubicTFTClass::CheckHeaterError()
{
  if ((thermalManager.degHotend(0) < 5) || (thermalManager.degHotend(0) > 290))
  {
    if (HeaterCheckCount > 60000)
    {
      HeaterCheckCount = 0;
      ANYCUBIC_SERIAL_PROTOCOLPGM("J10"); // J10 Hotend temperature abnormal
      ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
      SERIAL_ECHOLNPGM("TFT Serial Debug: Hotend temperature abnormal... J20");
#endif

    }
    else
    {
      HeaterCheckCount++;
    }
  }
  else
  {
    HeaterCheckCount = 0;
  }
}

void AnycubicTFTClass::StateHandler()
{
  switch (TFTstate) {
  case ANYCUBIC_TFT_STATE_IDLE:
    if(card.sdprinting){
      TFTstate=ANYCUBIC_TFT_STATE_SDPRINT;
      starttime=millis();

      // --> Send print info to display... most probably print started via gcode
    }
    break;
  case ANYCUBIC_TFT_STATE_SDPRINT:
      if(!card.sdprinting){
        // It seems that we are to printing anymore... pause or stopped?
        if (card.isFileOpen()){
          // File is still open --> paused
          TFTstate=ANYCUBIC_TFT_STATE_SDPAUSE;          
        } else {
          // File is closed --> stopped
          TFTstate=ANYCUBIC_TFT_STATE_IDLE;
          ANYCUBIC_SERIAL_PROTOCOLPGM("J14");// J14 print done
          ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
          SERIAL_ECHOLNPGM("TFT Serial Debug: SD print done... J14");
#endif
        }
      }
      break;
  case ANYCUBIC_TFT_STATE_SDPAUSE:
    break;
  case ANYCUBIC_TFT_STATE_SDPAUSE_OOF:
#ifdef ANYCUBIC_FILAMENT_RUNOUT_SENSOR
    if(!FilamentTestStatus) {
      // We got filament again
      TFTstate=ANYCUBIC_TFT_STATE_SDPAUSE;
    }
#endif
    break;
  case ANYCUBIC_TFT_STATE_SDPAUSE_REQ:
    if((!card.sdprinting) && (!planner.movesplanned())){
      // We have to wait until the sd card printing has been settled
#ifndef ADVANCED_PAUSE_FEATURE
      enqueue_and_echo_commands_P(PSTR("G91\nG1 Z10 F240\nG90"));
#endif
#ifdef ANYCUBIC_FILAMENT_RUNOUT_SENSOR
      if(FilamentTestStatus) {
        TFTstate=ANYCUBIC_TFT_STATE_SDPAUSE;
      } else {
        // Pause because of "out of filament"
        TFTstate=ANYCUBIC_TFT_STATE_SDPAUSE_OOF;
      }
#endif      
      ANYCUBIC_SERIAL_PROTOCOLPGM("J18");// J18 pausing print done
      ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
      SERIAL_ECHOLNPGM("TFT Serial Debug: SD print paused done... J18");
#endif
      
    }
    break;
  case ANYCUBIC_TFT_STATE_SDSTOP_REQ:
    if((!card.sdprinting) && (!planner.movesplanned())){
      ANYCUBIC_SERIAL_PROTOCOLPGM("J16");// J16 stop print
      ANYCUBIC_SERIAL_ENTER();
      TFTstate=ANYCUBIC_TFT_STATE_IDLE;
#ifdef ANYCUBIC_TFT_DEBUG
      SERIAL_ECHOLNPGM("TFT Serial Debug: SD print stopped... J16");
#endif
      enqueue_and_echo_commands_P(PSTR("M84"));
    }
    break;
  default:
    break;
  }
}

void AnycubicTFTClass::FilamentRunout()
{
#if ENABLED(ANYCUBIC_FILAMENT_RUNOUT_SENSOR)
  FilamentTestStatus=READ(FIL_RUNOUT_PIN)&0xff;
  
  if(FilamentTestStatus>FilamentTestLastStatus)
  {
    FilamentRunoutCounter++;
    if(FilamentRunoutCounter>=15800)
    {
      FilamentRunoutCounter=0;
      if((card.sdprinting==true))
      {
        PausePrint();
      }
      else if((card.sdprinting==false))
      {
        ANYCUBIC_SERIAL_PROTOCOLPGM("J15"); //J15 FILAMENT LACK
        ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
        SERIAL_ECHOLNPGM("TFT Serial Debug: Filament runout... J15");
#endif
      }
      FilamentTestLastStatus=FilamentTestStatus;
    }
  }
  else if(FilamentTestStatus!=FilamentTestLastStatus)
  {
    FilamentRunoutCounter=0;
    FilamentTestLastStatus=FilamentTestStatus;
#ifdef ANYCUBIC_TFT_DEBUG
    SERIAL_ECHOLNPGM("TFT Serial Debug: Filament runout recovered");
#endif
  }
#endif
}

void AnycubicTFTClass::GetCommandFromTFT()
{
  char *starpos = NULL;
  while( AnycubicSerial.available() > 0  && TFTbuflen < TFTBUFSIZE)
  {
    serial3_char = AnycubicSerial.read();
    if(serial3_char == '\n' ||
       serial3_char == '\r' ||
       serial3_char == ':'  ||
       serial3_count >= (TFT_MAX_CMD_SIZE - 1) )
    {
      if(!serial3_count) { //if empty line
        return;
      }
      
      TFTcmdbuffer[TFTbufindw][serial3_count] = 0; //terminate string
      
      if((strchr(TFTcmdbuffer[TFTbufindw], 'A') != NULL)){
        int16_t a_command;
        TFTstrchr_pointer = strchr(TFTcmdbuffer[TFTbufindw], 'A');
        a_command=((int)((strtod(&TFTcmdbuffer[TFTbufindw][TFTstrchr_pointer - TFTcmdbuffer[TFTbufindw] + 1], NULL))));
        
#ifdef ANYCUBIC_TFT_DEBUG
        if ((a_command>7) && (a_command != 20)) // No debugging of status polls, please!
          SERIAL_ECHOLNPAIR("TFT Serial Command: ", TFTcmdbuffer[TFTbufindw]);
#endif
        
        switch(a_command){
            
          case 0: //A0 GET HOTEND TEMP
            ANYCUBIC_SERIAL_PROTOCOLPGM("A0V ");
            ANYCUBIC_SERIAL_PROTOCOL(itostr3(int(thermalManager.degHotend(0) + 0.5)));
            ANYCUBIC_SERIAL_ENTER();
            break;
            
          case 1: //A1  GET HOTEND TARGET TEMP
            ANYCUBIC_SERIAL_PROTOCOLPGM("A1V ");
            ANYCUBIC_SERIAL_PROTOCOL(itostr3(int(thermalManager.degTargetHotend(0) + 0.5)));
            ANYCUBIC_SERIAL_ENTER();
            break;
            
          case 2: //A2 GET HOTBED TEMP
            ANYCUBIC_SERIAL_PROTOCOLPGM("A2V ");
            ANYCUBIC_SERIAL_PROTOCOL(itostr3(int(thermalManager.degBed() + 0.5)));
            ANYCUBIC_SERIAL_ENTER();
            break;
            
          case 3: //A3 GET HOTBED TARGET TEMP
            ANYCUBIC_SERIAL_PROTOCOLPGM("A3V ");
            ANYCUBIC_SERIAL_PROTOCOL(itostr3(int(thermalManager.degTargetBed() + 0.5)));
            ANYCUBIC_SERIAL_ENTER();
            break;
            
          case 4://A4 GET FAN SPEED
          {
            unsigned int temp;
            
            temp=((fanSpeeds[0]*100)/255);
            temp=constrain(temp,0,100);
            
            ANYCUBIC_SERIAL_PROTOCOLPGM("A4V ");
            ANYCUBIC_SERIAL_PROTOCOL(temp);
            ANYCUBIC_SERIAL_ENTER();
          }
            break;
          case 5:// A5 GET CURRENT COORDINATE
            ANYCUBIC_SERIAL_PROTOCOLPGM("A5V");
            ANYCUBIC_SERIAL_SPACE();
            ANYCUBIC_SERIAL_PROTOCOLPGM("X: ");
            ANYCUBIC_SERIAL_PROTOCOL(current_position[X_AXIS]);
            ANYCUBIC_SERIAL_SPACE();
            ANYCUBIC_SERIAL_PROTOCOLPGM("Y: ");
            ANYCUBIC_SERIAL_PROTOCOL(current_position[Y_AXIS]);
            ANYCUBIC_SERIAL_SPACE();
            ANYCUBIC_SERIAL_PROTOCOLPGM("Z: ");
            ANYCUBIC_SERIAL_PROTOCOL(current_position[Z_AXIS]);
            ANYCUBIC_SERIAL_SPACE();
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 6: //A6 GET SD CARD PRINTING STATUS
            if(card.sdprinting){
              ANYCUBIC_SERIAL_PROTOCOLPGM("A6V ");
              if(card.cardOK)
              {
                ANYCUBIC_SERIAL_PROTOCOL(itostr3(card.percentDone()));
              }
              else
              {
                ANYCUBIC_SERIAL_PROTOCOLPGM("J02");
              }
            }
            else
              ANYCUBIC_SERIAL_PROTOCOLPGM("A6V ---");
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 7://A7 GET PRINTING TIME
          {
            ANYCUBIC_SERIAL_PROTOCOLPGM("A7V ");
            if(starttime != 0) // print time
            {
              uint16_t time = millis()/60000 - starttime/60000;
              ANYCUBIC_SERIAL_PROTOCOL(itostr2(time/60));
              ANYCUBIC_SERIAL_SPACE();
              ANYCUBIC_SERIAL_PROTOCOLPGM("H");
              ANYCUBIC_SERIAL_SPACE();
              ANYCUBIC_SERIAL_PROTOCOL(itostr2(time%60));
              ANYCUBIC_SERIAL_SPACE();
              ANYCUBIC_SERIAL_PROTOCOLPGM("M");
            }else{
              ANYCUBIC_SERIAL_SPACE();
              ANYCUBIC_SERIAL_PROTOCOLPGM("999:999");
            }
            ANYCUBIC_SERIAL_ENTER();
            
            break;
          }
          case 8: // A8 GET  SD LIST
            SelectedDirectory[0]=0;
            if(!IS_SD_INSERTED)
            {
              ANYCUBIC_SERIAL_PROTOCOLPGM("J02");
              ANYCUBIC_SERIAL_ENTER();
            }
            else
            {
              if(CodeSeen('S'))
                filenumber=CodeValue();
              
              ANYCUBIC_SERIAL_PROTOCOLPGM("FN "); // Filelist start
              ANYCUBIC_SERIAL_ENTER();
              Ls();
              ANYCUBIC_SERIAL_PROTOCOLPGM("END"); // Filelist stop
              ANYCUBIC_SERIAL_ENTER();
            }
            break;
          case 9: // A9 pause sd print
            if(card.sdprinting)
            {
              PausePrint();
            }
            else
            {
              StopPrint();
            }
            break;
          case 10: // A10 resume sd print
            if((TFTstate==ANYCUBIC_TFT_STATE_SDPAUSE) || (TFTstate==ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              StartPrint();
              ANYCUBIC_SERIAL_PROTOCOLPGM("J04");// J04 printing form sd card now
              ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Serial Debug: SD print started... J04");
#endif
            }
            break;
          case 11: // A11 STOP SD PRINT
            if((card.sdprinting) || (TFTstate==ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              StopPrint();
            }
            break;
          case 12: // A12 kill
            kill(PSTR(MSG_KILLED));
            break;
          case 13: // A13 SELECTION FILE
            if((!planner.movesplanned()) && (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              starpos = (strchr(TFTstrchr_pointer + 4,'*'));
              if (TFTstrchr_pointer[4] == '/') {
                strcpy(SelectedDirectory, TFTstrchr_pointer+5);
              } else if (TFTstrchr_pointer[4] == '<') {
                strcpy(SelectedDirectory, TFTstrchr_pointer+4);
              } else {
                SelectedDirectory[0]=0;
                
                if(starpos!=NULL)
                  *(starpos-1)='\0';
                card.openFile(TFTstrchr_pointer + 4,true);
                if (card.isFileOpen()) {
                  ANYCUBIC_SERIAL_PROTOCOLPGM("J20"); // J20 Open successful
                  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
                  SERIAL_ECHOLNPGM("TFT Serial Debug: File open successful... J20");
#endif
                } else {
                  ANYCUBIC_SERIAL_PROTOCOLPGM("J21"); // J21 Open failed
                  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
                  SERIAL_ECHOLNPGM("TFT Serial Debug: File open failed... J21");
#endif
                }
              }
              ANYCUBIC_SERIAL_ENTER();
            }
            break;
          case 14: // A14 START PRINTING
            if((!planner.movesplanned()) && (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE) && (card.isFileOpen()))
            {
              StartPrint();
              ANYCUBIC_SERIAL_PROTOCOLPGM("J04"); // J04 Starting Print
              ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Serial Debug: Starting SD Print... J04");
#endif
            }
            break;
          case 15: // A15 RESUMING FROM OUTAGE
            //                    			if((!planner.movesplanned())&&(!TFTresumingflag))
            //                          {
            //                                if(card.cardOK)
            //                                FlagResumFromOutage=true;
            //                                ResumingFlag=1;
            //                                card.startFileprint();
            //                                starttime=millis();
            //                                ANYCUBIC_SERIAL_SUCC_START;
            //                          }
            //                          ANYCUBIC_SERIAL_ENTER();
            break;
          case 16: // A16 set hotend temp
            {
              unsigned int tempvalue;
              if(CodeSeen('S'))
              {
                tempvalue=constrain(CodeValue(),0,275);
                thermalManager.setTargetHotend(tempvalue,0);
              }
              else if((CodeSeen('C'))&&(!planner.movesplanned()))
              {
                if((current_position[Z_AXIS]<10))
                  enqueue_and_echo_commands_P(PSTR("G1 Z10")); //RASE Z AXIS
                tempvalue=constrain(CodeValue(),0,275);
                thermalManager.setTargetHotend(tempvalue,0);
              }
            }
            //  ANYCUBIC_SERIAL_ENTER();
            break;
          case 17:// A17 set heated bed temp
            {
              unsigned int tempbed;
              if(CodeSeen('S')){tempbed=constrain(CodeValue(),0,150);
                thermalManager.setTargetBed(tempbed);
              }
            }
            //  ANYCUBIC_SERIAL_ENTER();
            break;
          case 18:// A18 set fan speed
            unsigned int temp;
            if (CodeSeen('S'))
            {
              temp=(CodeValue()*255/100);
              temp=constrain(temp,0,255);
              fanSpeeds[0]=temp;
            }
            else fanSpeeds[0]=255;
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 19: // A19 stop stepper drivers
            if((!planner.movesplanned())&&(!card.sdprinting))
            {
              quickstop_stepper();
              disable_X();
              disable_Y();
              disable_Z();
              disable_E0();
            }
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 20:// A20 read printing speed
          {
            if(CodeSeen('S')){
              feedrate_percentage=constrain(CodeValue(),40,999);}
            else{
              ANYCUBIC_SERIAL_PROTOCOLPGM("A20V ");
              ANYCUBIC_SERIAL_PROTOCOL(feedrate_percentage);
              ANYCUBIC_SERIAL_ENTER();
            }
          }
            break;
          case 21: // A21 all home
            if((!planner.movesplanned()) && (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              if(CodeSeen('X')||CodeSeen('Y')||CodeSeen('Z'))
              {
                if(CodeSeen('X'))enqueue_and_echo_commands_P(PSTR("G28 X"));
                if(CodeSeen('Y')) enqueue_and_echo_commands_P(PSTR("G28 Y"));
                if(CodeSeen('Z')) enqueue_and_echo_commands_P(PSTR("G28 Z"));
              }
              else if(CodeSeen('C'))enqueue_and_echo_commands_P(PSTR("G28"));
            }
            break;
          case 22: // A22 move X/Y/Z or extrude
            if((!planner.movesplanned()) && (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              float coorvalue;
              unsigned int movespeed=0;
              char value[30];
              if(CodeSeen('F')) // Set feedrate
                movespeed = CodeValue();
              
              enqueue_and_echo_commands_P(PSTR("G91"));  // relative coordinates
              
              if(CodeSeen('X')) // Move in X direction
              {
                coorvalue=CodeValue();
                if((coorvalue<=0.2)&&coorvalue>0){sprintf_P(value,PSTR("G1 X0.1F%i"),movespeed);}
                else if((coorvalue<=-0.1)&&coorvalue>-1){sprintf_P(value,PSTR("G1 X-0.1F%i"),movespeed);}
                else {sprintf_P(value,PSTR("G1 X%iF%i"),int(coorvalue),movespeed);}
                enqueue_and_echo_command(value);
              }
              else if(CodeSeen('Y')) // Move in Y direction
              {
                coorvalue=CodeValue();
                if((coorvalue<=0.2)&&coorvalue>0){sprintf_P(value,PSTR("G1 Y0.1F%i"),movespeed);}
                else if((coorvalue<=-0.1)&&coorvalue>-1){sprintf_P(value,PSTR("G1 Y-0.1F%i"),movespeed);}
                else {sprintf_P(value,PSTR("G1 Y%iF%i"),int(coorvalue),movespeed);}
                enqueue_and_echo_command(value);
              }
              else if(CodeSeen('Z')) // Move in Z direction
              {
                coorvalue=CodeValue();
                if((coorvalue<=0.2)&&coorvalue>0){sprintf_P(value,PSTR("G1 Z0.1F%i"),movespeed);}
                else if((coorvalue<=-0.1)&&coorvalue>-1){sprintf_P(value,PSTR("G1 Z-0.1F%i"),movespeed);}
                else {sprintf_P(value,PSTR("G1 Z%iF%i"),int(coorvalue),movespeed);}
                enqueue_and_echo_command(value);
              }
              else if(CodeSeen('E')) // Extrude
              {
                coorvalue=CodeValue();
                if((coorvalue<=0.2)&&coorvalue>0){sprintf_P(value,PSTR("G1 E0.1F%i"),movespeed);}
                else if((coorvalue<=-0.1)&&coorvalue>-1){sprintf_P(value,PSTR("G1 E-0.1F%i"),movespeed);}
                else {sprintf_P(value,PSTR("G1 E%iF500"),int(coorvalue)); }
                enqueue_and_echo_command(value);
              }
              enqueue_and_echo_commands_P(PSTR("G90"));  // absolute coordinates
            }
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 23: // A23 preheat pla
            if((!planner.movesplanned())&& (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              if((current_position[Z_AXIS]<10)) enqueue_and_echo_commands_P(PSTR("G1 Z10")); // RAISE Z AXIS
              thermalManager.setTargetBed(50);
              thermalManager.setTargetHotend(200, 0);
              ANYCUBIC_SERIAL_SUCC_START;
              ANYCUBIC_SERIAL_ENTER();
            }
            break;
          case 24:// A24 preheat abs
            if((!planner.movesplanned()) && (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              if((current_position[Z_AXIS]<10)) enqueue_and_echo_commands_P(PSTR("G1 Z10")); //RAISE Z AXIS
              thermalManager.setTargetBed(80);
              thermalManager.setTargetHotend(240, 0);
              
              ANYCUBIC_SERIAL_SUCC_START;
              ANYCUBIC_SERIAL_ENTER();
            }
            break;
          case 25: // A25 cool down
            if((!planner.movesplanned())&& (TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
              thermalManager.setTargetHotend(0,0);
              thermalManager.setTargetBed(0);
              ANYCUBIC_SERIAL_PROTOCOLPGM("J12"); // J12 cool down
              ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Serial Debug: Cooling down... J12");
#endif
            }
            break;
          case 26: // A26 refresh SD
            if (SelectedDirectory[0]==0) {
              card.initsd();
            } else {
              if ((SelectedDirectory[0] == '.') && (SelectedDirectory[1] == '.')) {
                card.updir();
              } else {
                if (SelectedDirectory[0] == '<') {
                  HandleSpecialMenu();
                } else {
                  card.chdir(SelectedDirectory);
                }
              }
            }

            SelectedDirectory[0]=0;
            
            if(!IS_SD_INSERTED)
            {
              ANYCUBIC_SERIAL_PROTOCOLPGM("J02"); // J02 SD Card initilized
              ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Serial Debug: SD card initialized... J02");
#endif
            }
            break;
#ifdef SERVO_ENDSTOPS
          case 27: // A27 servos angles  adjust
            break;
#endif
          case 28: // A28 filament test
          {
            if(CodeSeen('O'));
            else if(CodeSeen('C'));
          }
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 29: // A29 Z PROBE OFFESET SET
            break;
            
          case 30: // A30 assist leveling, the original function was canceled
            if(CodeSeen('S')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Entering level menue...");
#endif
            } else if(CodeSeen('O')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Leveling started and movint to front left...");
#endif
              enqueue_and_echo_commands_P(PSTR("G91\nG1 Z10 F240\nG90\nG28\nG29\nG1 X20 Y20 F6000\nG1 Z0 F240"));
            } else if(CodeSeen('T')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Level checkpoint front right...");
#endif
              enqueue_and_echo_commands_P(PSTR("G1 Z5 F240\nG1 X190 Y20 F6000\nG1 Z0 F240"));
            } else if(CodeSeen('C')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Level checkpoint back right...");
#endif
              enqueue_and_echo_commands_P(PSTR("G1 Z5 F240\nG1 X190 Y190 F6000\nG1 Z0 F240"));
            } else if(CodeSeen('Q')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Level checkpoint back right...");
#endif
              enqueue_and_echo_commands_P(PSTR("G1 Z5 F240\nG1 X190 Y20 F6000\nG1 Z0 F240"));
            } else if(CodeSeen('H')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Level check no heating...");
#endif
              //                            enqueue_and_echo_commands_P(PSTR("... TBD ..."));
              ANYCUBIC_SERIAL_PROTOCOLPGM("J22"); // J22 Test print done
              ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Serial Debug: Leveling print test done... J22");
#endif
            } else if(CodeSeen('L')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Level check heating...");
#endif
              //                            enqueue_and_echo_commands_P(PSTR("... TBD ..."));
              ANYCUBIC_SERIAL_PROTOCOLPGM("J22"); // J22 Test print done
              ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Serial Debug: Leveling print test with heating done... J22");
#endif
            }
            ANYCUBIC_SERIAL_SUCC_START;
            ANYCUBIC_SERIAL_ENTER();
            
            break;
          case 31: // A31 zoffset
            if((!planner.movesplanned())&&(TFTstate!=ANYCUBIC_TFT_STATE_SDPAUSE) && (TFTstate!=ANYCUBIC_TFT_STATE_SDOUTAGE))
            {
#if HAS_BED_PROBE
              char value[30];
              char *s_zoffset;
              //if((current_position[Z_AXIS]<10))
              //  z_offset_auto_test();

              if(CodeSeen('S')){
                ANYCUBIC_SERIAL_PROTOCOLPGM("A9V ");
                ANYCUBIC_SERIAL_PROTOCOL(itostr3(int(zprobe_zoffset*100.00 + 0.5)));
                ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
                SERIAL_ECHOPGM("TFT sending current z-probe offset data... <");
                SERIAL_ECHOPGM("A9V ");
                SERIAL_ECHO(itostr3(int(zprobe_zoffset*100.00 + 0.5)));
                SERIAL_ECHOLNPGM(">");
#endif
              }
              if(CodeSeen('D'))
              {
                s_zoffset=ftostr32(float(CodeValue())/100.0);
                sprintf_P(value,PSTR("M851 Z"));
                strcat(value,s_zoffset);
                enqueue_and_echo_command(value); // Apply Z-Probe offset
                enqueue_and_echo_commands_P(PSTR("M500")); // Save to EEPROM
              }            
#endif
			}
            ANYCUBIC_SERIAL_ENTER();
            break;
          case 32: // A32 clean leveling beep flag
            if(CodeSeen('S')) {
#ifdef ANYCUBIC_TFT_DEBUG
              SERIAL_ECHOLNPGM("TFT Level saving data...");
#endif
              enqueue_and_echo_commands_P(PSTR("M500\nM420 S1\nG1 Z10 F240\nG1 X0 Y0 F6000"));
              ANYCUBIC_SERIAL_SUCC_START;
              ANYCUBIC_SERIAL_ENTER();
            }
            break;
          case 33: // A33 get version info
          {
            ANYCUBIC_SERIAL_PROTOCOLPGM("J33 ");
            ANYCUBIC_SERIAL_PROTOCOLPGM(MSG_MY_VERSION);
            ANYCUBIC_SERIAL_ENTER();
          }
            break;
          default: break;
        }
      }
      TFTbufindw = (TFTbufindw + 1)%TFTBUFSIZE;
      TFTbuflen += 1;
      serial3_count = 0; //clear buffer
    }
    else
    {
      TFTcmdbuffer[TFTbufindw][serial3_count++] = serial3_char;
    }
  }
}

void AnycubicTFTClass::CommandScan()
{
  CheckHeaterError();
  CheckSDCardChange();
  StateHandler();
  
  if(TFTbuflen<(TFTBUFSIZE-1))
    GetCommandFromTFT();
  if(TFTbuflen)
  {
    TFTbuflen = (TFTbuflen-1);
    TFTbufindr = (TFTbufindr + 1)%TFTBUFSIZE;
  }
}

void AnycubicTFTClass::HeatingStart()
{
  ANYCUBIC_SERIAL_PROTOCOLPGM("J06"); // J07 hotend heating start
  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
  SERIAL_ECHOLNPGM("TFT Serial Debug: Nozzle is heating... J06");
#endif
}

void AnycubicTFTClass::HeatingDone()
{
  ANYCUBIC_SERIAL_PROTOCOLPGM("J07"); // J07 hotend heating done
  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
  SERIAL_ECHOLNPGM("TFT Serial Debug: Nozzle heating is done... J07");
#endif

  if(TFTstate==ANYCUBIC_TFT_STATE_SDPRINT)
  {
    ANYCUBIC_SERIAL_PROTOCOLPGM("J04"); // J04 printing from sd card
    ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
    SERIAL_ECHOLNPGM("TFT Serial Debug: Continuing SD print after heating... J04");
#endif
  }
}

void AnycubicTFTClass::BedHeatingStart()
{
  ANYCUBIC_SERIAL_PROTOCOLPGM("J08"); // J08 hotbed heating start
  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
  SERIAL_ECHOLNPGM("TFT Serial Debug: Bed is heating... J08");
#endif
}

void AnycubicTFTClass::BedHeatingDone()
{
  ANYCUBIC_SERIAL_PROTOCOLPGM("J09"); // J09 hotbed heating done
  ANYCUBIC_SERIAL_ENTER();
#ifdef ANYCUBIC_TFT_DEBUG
  SERIAL_ECHOLNPGM("TFT Serial Debug: Bed heating is done... J09");
#endif
}


AnycubicTFTClass AnycubicTFT;
#endif
