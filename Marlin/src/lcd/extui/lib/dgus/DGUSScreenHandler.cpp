/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_DGUS_LCD

#include "DGUSScreenHandler.h"
#include "DGUSDisplay.h"
#include "DGUSVPVariable.h"
#include "DGUSDisplayDef.h"

#include "../../ui_api.h"
#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"
#if ENABLED(DGUS_LCD_UI_MKS)
  #include "src/gcode/gcode.h"
  #include "../../../../pins/pins.h"
  #include "../../../../../src/libs/nozzle.h"
  #if ENABLED(HAS_STEALTHCHOP)
    #include "../../../../src/module/stepper/trinamic.h"
    #include "../../../../src/module/stepper/indirection.h"
  #endif 
  #include "../../../../module/probe.h"
#endif 

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

uint16_t DGUSScreenHandler::ConfirmVP;

#if ENABLED(SDSUPPORT)
  int16_t DGUSScreenHandler::top_file = 0;
  int16_t DGUSScreenHandler::file_to_print = 0;
  static ExtUI::FileList filelist;
#endif

void (*DGUSScreenHandler::confirm_action_cb)() = nullptr;

//DGUSScreenHandler ScreenHandler;

DGUSLCD_Screens DGUSScreenHandler::current_screen;
DGUSLCD_Screens DGUSScreenHandler::past_screens[NUM_PAST_SCREENS];
uint8_t DGUSScreenHandler::update_ptr;
uint16_t DGUSScreenHandler::skipVP;
bool DGUSScreenHandler::ScreenComplete;


#if ENABLED(DGUS_LCD_UI_MKS)


uint16_t DGUSLanguageSwitch = 0;   // switch language for MKS DGUS
uint16_t DGUSAutoTurnOff = 0;     
#endif

//DGUSDisplay dgusdisplay;

// endianness swap
uint16_t swap16(const uint16_t value) { return (value & 0xffU) << 8U | (value >> 8U); }

#if ENABLED(DGUS_LCD_UI_MKS)
uint32_t swap32(const uint32_t value) { return (value & 0x000000ffU) << 24U | (value & 0x0000ff00U) << 8U  |  (value & 0x00ff0000U) >> 8U  | (value& 0xff000000U) >> 24U; }
#endif

void DGUSScreenHandler::sendinfoscreen(const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool l4inflash) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_MSGSTR1, &ramcopy)) {
    ramcopy.memadr = (void*) line1;
    l1inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR2, &ramcopy)) {
    ramcopy.memadr = (void*) line2;
    l2inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR3, &ramcopy)) {
    ramcopy.memadr = (void*) line3;
    l3inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR4, &ramcopy)) {
    ramcopy.memadr = (void*) line4;
    l4inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
}

#if ENABLED(DGUS_LCD_UI_MKS)

void DGUSScreenHandler::sendinfoscreen_ch_mks(const uint16_t* line1, const uint16_t* line2, const uint16_t* line3, const uint16_t* line4) {

  dgusdisplay.WriteVariable(VP_MSGSTR1, line1, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR2, line2, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR3, line3, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR4, line4, 32, true);

}

void DGUSScreenHandler::sendinfoscreen_en_mks(const char* line1, const char* line2, const char* line3, const char* line4) {

  dgusdisplay.WriteVariable(VP_MSGSTR1, line1, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR2, line2, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR3, line3, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR4, line4, 32, true);
}

void DGUSScreenHandler::sendinfoscreen_mks(const void* line1, const void* line2, const void* line3, const void* line4,uint16_t language) {
  
  if(language == MKS_English) {

    DGUSScreenHandler::sendinfoscreen_en_mks((char *)line1,(char *)line2,(char *)line3,(char *)line4);
  }
  else if (language == MKS_SimpleChinese) {

    DGUSScreenHandler::sendinfoscreen_ch_mks((uint16_t *)line1,(uint16_t *)line2,(uint16_t *)line3,(uint16_t *)line4);
  }
}

#endif

void DGUSScreenHandler::HandleUserConfirmationPopUp(uint16_t VP, const char* line1, const char* line2, const char* line3, const char* line4, bool l1, bool l2, bool l3, bool l4) {
  if (current_screen == DGUSLCD_SCREEN_CONFIRM) {
    // Already showing a pop up, so we need to cancel that first.
    PopToOldScreen();
  }

  ConfirmVP = VP;
  sendinfoscreen(line1, line2, line3, line4, l1, l2, l3, l4);
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_CONFIRM);
}

void DGUSScreenHandler::setstatusmessage(const char *msg) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) msg;
    DGUSLCD_SendStringToDisplay(ramcopy);
  }
}

void DGUSScreenHandler::setstatusmessagePGM(PGM_P const msg) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) msg;
    DGUSLCD_SendStringToDisplayPGM(ramcopy);
  }
}

// Send an 8 bit or 16 bit value to the display.
void DGUSScreenHandler::DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    if (var.size > 1)
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    else
      dgusdisplay.WriteVariable(var.VP, *(int8_t*)var.memadr);
  }
}

// Send an uint8_t between 0 and 255 to the display, but scale to a percentage (0..100)
void DGUSScreenHandler::DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint16_t tmp = *(uint8_t *) var.memadr +1 ; // +1 -> avoid rounding issues for the display.
    tmp = map(tmp, 0, 255, 0, 100);
    dgusdisplay.WriteVariable(var.VP, tmp);
  }
}

#if ENABLED(DGUS_LCD_UI_MKS)

void DGUSScreenHandler::DGUSLCD_SendFloatByStringToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    float tmp = *(float *) var.memadr;
    char buf[20];

    sprintf(buf,"%.2f",tmp);
    dgusdisplay.WriteVariable(var.VP, buf, 8, true);
  }
}


void DGUSScreenHandler::DGUSLCD_SendFanToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint16_t tmp = *(uint8_t *) var.memadr; // +1 -> avoid rounding issues for the display.
    //tmp = map(tmp, 0, 255, 0, 100);
    dgusdisplay.WriteVariable(var.VP, tmp);
  }
}
#endif

// Send the current print progress to the display.
void DGUSScreenHandler::DGUSLCD_SendPrintProgressToDisplay(DGUS_VP_Variable &var) {
  //DEBUG_ECHOPAIR(" DGUSLCD_SendPrintProgressToDisplay ", var.VP);
  uint16_t tmp = ExtUI::getProgress_percent();
  //DEBUG_ECHOLNPAIR(" data ", tmp);
  dgusdisplay.WriteVariable(var.VP, tmp);
}

// Send the current print time to the display.
// It is using a hex display for that: It expects BSD coded data in the format xxyyzz
void DGUSScreenHandler::DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();
  char buf[32];
  elapsed.toString(buf);
  dgusdisplay.WriteVariable(VP_PrintTime, buf, var.size, true);
}

#if ENABLED(DGUS_LCD_UI_MKS)

void DGUSScreenHandler::DGUSLCD_SendBabyStepToDisplay_MKS(DGUS_VP_Variable &var)
{
  float value = current_position.z;
  DEBUG_ECHOLNPAIR_F(" >> ", value, 6);
  value *= cpow(10, 2);
  dgusdisplay.WriteVariable(VP_SD_Print_Baby, (uint16_t)value);
}


void DGUSScreenHandler::DGUSLCD_SendPrintTimeToDisplay_MKS(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();

  uint32_t time = elapsed.value;
  uint16_t time_h;
  uint16_t time_m;
  uint16_t time_s;
  time_h = time / 3600;
  time_m = time %3600 / 60;
  time_s = (time % 3600) % 60;

  dgusdisplay.WriteVariable(VP_PrintTime_H, (uint16_t)time_h);
  dgusdisplay.WriteVariable(VP_PrintTime_M, (uint16_t)time_m);
  dgusdisplay.WriteVariable(VP_PrintTime_S, (uint16_t)time_s);
}

void DGUSScreenHandler::DGUSLCD_SetUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    DEBUG_ECHOLNPAIR("FAN value get:", value);
    *(uint8_t*)var.memadr = map(constrain(value, 0, 255), 0, 255, 0, 255);
    DEBUG_ECHOLNPAIR("FAN value change:", *(uint8_t*)var.memadr);
  }
}

void DGUSScreenHandler::DGUSLCD_SendGbkToDisplay(DGUS_VP_Variable &var) {

  DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
  uint16_t *tmp = (uint16_t*) var.memadr;
  dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
}

#endif

// Send an uint8_t between 0 and 100 to a variable scale to 0..255
void DGUSScreenHandler::DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    DEBUG_ECHOLNPAIR("FAN value get:", value);
    *(uint8_t*)var.memadr = map(constrain(value, 0, 100), 0, 100, 0, 255);
    DEBUG_ECHOLNPAIR("FAN value change:", *(uint8_t*)var.memadr);
  }
}

// Sends a (RAM located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenHandler::DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
}

#if ENABLED(DGUS_LCD_UI_MKS)

void DGUSScreenHandler::DGUSLCD_SendStringToDisplay_Ch_MKS(DGUS_VP_Variable &var) {
  uint16_t *tmp = (uint16_t *)var.memadr;  
  dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
}

void DGUSScreenHandler::DGUSLCD_SendStringToDisplay_Language_MKS(DGUS_VP_Variable &var) {

  if(DGUSLanguageSwitch == MKS_English) {
    char *tmp = (char*) var.memadr;
    dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
  }
  else if (DGUSLanguageSwitch == MKS_SimpleChinese) {
    uint16_t *tmp = (uint16_t *)var.memadr;  
    dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
  }
}

void DGUSScreenHandler::DGUSLCD_SendTMCStepValue(DGUS_VP_Variable &var) {

  #if ENABLED(SENSORLESS_HOMING)
    #if AXIS_HAS_STEALTHCHOP(X)
      tmc_x_step = stepperX.homing_threshold();
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    #endif

    #if AXIS_HAS_STEALTHCHOP(Y)
      tmc_y_step = stepperY.homing_threshold();
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    #endif

    #if AXIS_HAS_STEALTHCHOP(Z)
      tmc_z_step = stepperZ.homing_threshold();
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    #endif
  #endif
}

#endif 

// Sends a (flash located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariablePGM(var.VP, tmp, var.size, true);
}

#if HAS_PID_HEATING
  void DGUSScreenHandler::DGUSLCD_SendTemperaturePID(DGUS_VP_Variable &var) {
    float value = *(float *)var.memadr;
    float valuesend = 0;
    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_E0_PID_P: valuesend = value; break;
        case VP_E0_PID_I: valuesend = unscalePID_i(value); break;
        case VP_E0_PID_D: valuesend = unscalePID_d(value); break;
      #endif
      #if HOTENDS >= 2
        case VP_E1_PID_P: valuesend = value; break;
        case VP_E1_PID_I: valuesend = unscalePID_i(value); break;
        case VP_E1_PID_D: valuesend = unscalePID_d(value); break;
      #endif
      #if HAS_HEATED_BED
        case VP_BED_PID_P: valuesend = value; break;
        case VP_BED_PID_I: valuesend = unscalePID_i(value); break;
        case VP_BED_PID_D: valuesend = unscalePID_d(value); break;
      #endif
    }

    valuesend *= cpow(10, 1);
    union { int16_t i; char lb[2]; } endian;

    char tmp[2];
    endian.i = valuesend;
    tmp[0] = endian.lb[1];
    tmp[1] = endian.lb[0];
    dgusdisplay.WriteVariable(var.VP, tmp, 2);
  }
#endif

#if ENABLED(PRINTCOUNTER)

  // Send the accumulate print time to the display.
  // It is using a hex display for that: It expects BSD coded data in the format xxyyzz
  void DGUSScreenHandler::DGUSLCD_SendPrintAccTimeToDisplay(DGUS_VP_Variable &var) {
    printStatistics state = print_job_timer.getStats();
    char buf[22];
    duration_t elapsed = state.printTime;
    elapsed.toString(buf);
    dgusdisplay.WriteVariable(VP_PrintAccTime, buf, var.size, true);
  }

  void DGUSScreenHandler::DGUSLCD_SendPrintsTotalToDisplay(DGUS_VP_Variable &var) {
    printStatistics state = print_job_timer.getStats();
    char buf[10];
    sprintf_P(buf, PSTR("%u"), state.totalPrints);
    dgusdisplay.WriteVariable(VP_PrintsTotal, buf, var.size, true);
  }

#endif

// Send fan status value to the display.
#if HAS_FAN
  void DGUSScreenHandler::DGUSLCD_SendFanStatusToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      DEBUG_ECHOPAIR(" DGUSLCD_SendFanStatusToDisplay ", var.VP);
      DEBUG_ECHOLNPAIR(" data ", *(uint8_t *)var.memadr);
      uint16_t data_to_send = 0;
      if (*(uint8_t *) var.memadr) data_to_send = 1;
      dgusdisplay.WriteVariable(var.VP, data_to_send);
    }
  }
#endif

// Send heater status value to the display.
void DGUSScreenHandler::DGUSLCD_SendHeaterStatusToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    DEBUG_ECHOPAIR(" DGUSLCD_SendHeaterStatusToDisplay ", var.VP);
    DEBUG_ECHOLNPAIR(" data ", *(int16_t *)var.memadr);
    uint16_t data_to_send = 0;
    if (*(int16_t *) var.memadr) data_to_send = 1;
    dgusdisplay.WriteVariable(var.VP, data_to_send);
  }
}

#if ENABLED(DGUS_UI_WAITING)
  void DGUSScreenHandler::DGUSLCD_SendWaitingStatusToDisplay(DGUS_VP_Variable &var) {
    // In FYSETC UI design there are 10 statuses to loop
    static uint16_t period = 0;
    static uint16_t index = 0;
    //DEBUG_ECHOPAIR(" DGUSLCD_SendWaitingStatusToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", swap16(index));
    if (period++ > DGUS_UI_WAITING_STATUS_PERIOD) {
      dgusdisplay.WriteVariable(var.VP, index);
      //DEBUG_ECHOLNPAIR(" data ", swap16(index));
      if (++index >= DGUS_UI_WAITING_STATUS) index = 0;
      period = 0;
    }
  }
#endif

#if ENABLED(SDSUPPORT)

  void DGUSScreenHandler::ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr) {
    // default action executed when there is a SD card, but not printing
    if (ExtUI::isMediaInserted() && !ExtUI::isPrintingFromMedia()) {
      ScreenChangeHook(var, val_ptr);
      dgusdisplay.RequestScreen(current_screen);
      return;
    }

    // if we are printing, we jump to two screens after the requested one.
    // This should host e.g a print pause / print abort / print resume dialog.
    // This concept allows to recycle this hook for other file
    if (ExtUI::isPrintingFromMedia() && !card.flag.abort_sd_printing) {
      GotoScreen(DGUSLCD_SCREEN_SDPRINTMANIPULATION);
      return;
    }

    // Don't let the user in the dark why there is no reaction.
    if (!ExtUI::isMediaInserted()) {
      setstatusmessagePGM(GET_TEXT(MSG_NO_MEDIA));
      return;
    }
    if (card.flag.abort_sd_printing) {
      setstatusmessagePGM(GET_TEXT(MSG_MEDIA_ABORTING));
      return;
    }
  }

  void DGUSScreenHandler::DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable& var, void *val_ptr) {
    auto old_top = top_file;
    const int16_t scroll = (int16_t)swap16(*(uint16_t*)val_ptr);
    if (scroll) {
      top_file += scroll;
      DEBUG_ECHOPAIR("new topfile calculated:", top_file);
      if (top_file < 0) {
        top_file = 0;
        DEBUG_ECHOLNPGM("Top of filelist reached");
      }
      else {
        int16_t max_top = filelist.count() -  DGUS_SD_FILESPERSCREEN;
        NOLESS(max_top, 0);
        NOMORE(top_file, max_top);
      }
      DEBUG_ECHOPAIR("new topfile adjusted:", top_file);
    }
    else if (!filelist.isAtRootDir()) {
      #if !ENABLED(DGUS_LCD_UI_MKS)
        filelist.upDir();
      #endif
      top_file = 0;
      ForceCompleteUpdate();
    }

    if (old_top != top_file) ForceCompleteUpdate();
  }

  void DGUSScreenHandler::DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)swap16(*(uint16_t*)val_ptr) + top_file;
    #if !ENABLED(DGUS_LCD_UI_MKS)
    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;
    #else
    if ((touched_nr != 0x0F) && (touched_nr > filelist.count())) return;
    if ((!filelist.seek(touched_nr)) && (touched_nr !=0x0f)) return;
    #endif

    #if ENABLED(DGUS_LCD_UI_MKS)
      if (touched_nr == 0X0F)
      { 
        if(filelist.isAtRootDir())
          GotoScreen(DGUSLCD_SCREEN_MAIN);
        else 
          filelist.upDir();
        return ;
      }
    #endif 

    if (filelist.isDir()) {
      filelist.changeDir(filelist.filename());
      top_file = 0;
      ForceCompleteUpdate();
      return;
    }

    #if ENABLED(DGUS_PRINT_FILENAME)
      // Send print filename
      dgusdisplay.WriteVariable(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN, true);
    #endif

    // Setup Confirmation screen
    file_to_print = touched_nr;
    #if ENABLED(DGUS_LCD_UI_MKS)
      GotoScreen(MKSLCD_SCREEN_PRINT_CONFIRM);
    #endif

    #if !ENABLED(DGUS_LCD_UI_MKS)
    HandleUserConfirmationPopUp(VP_SD_FileSelectConfirm, nullptr, PSTR("Print file"), filelist.filename(), PSTR("from SD Card?"), true, true, false, true);
    #endif
  }

  void DGUSScreenHandler::DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    ScreenHandler.GotoScreen(
      #if ENABLED(DGUS_LCD_UI_ORIGIN)
        DGUSLCD_SCREEN_STATUS
      #elif ENABLED(DGUS_LCD_UI_MKS)    
        MKSLCD_SCREEN_PRINT  
        
      #else
        DGUSLCD_SCREEN_SDPRINTMANIPULATION
      #endif
    );
    #if ENABLED(DGUS_LCD_UI_MKS)
      z_offset_add = 0;
    #endif
  }

  void DGUSScreenHandler::DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {
    
    #if ENABLED(DGUS_LCD_UI_MKS)
      auto cs = getCurrentScreen();
    #endif

    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    switch (swap16(*(uint16_t*)val_ptr)) {
      case 0:  // Resume
      
        #if ENABLED(DGUS_LCD_UI_MKS)
          
          if((runout_mks.runout_status != RUNOUT_WAITTING_STATUS) 
          && (runout_mks.runout_status != UNRUNOUT_STATUS)) {

            if((cs == MKSLCD_SCREEN_PRINT) || (cs == MKSLCD_SCREEN_PAUSE)) {
              ScreenHandler.GotoScreen(MKSLCD_SCREEN_PAUSE);  
            }else{
              return;
            }
            return; 
          }else {
            runout_mks.runout_status = UNRUNOUT_STATUS;
          }
        #endif
          #if ENABLED(DGUS_LCD_UI_MKS)
            ScreenHandler.GotoScreen(MKSLCD_SCREEN_PRINT);
          #endif

          if (ExtUI::isPrintingFromMediaPaused()) {
            #if ENABLED(DGUS_LCD_UI_MKS)
              nozzle_park_mks.print_pause_start_flag = 0;
              nozzle_park_mks.blstatus = true;
            #endif
            ExtUI::resumePrint();
          }
      break;

      case 1:  // Pause

        ScreenHandler.GotoScreen(MKSLCD_SCREEN_PAUSE); 
        if (!ExtUI::isPrintingFromMediaPaused()) {
          #if ENABLED(DGUS_LCD_UI_MKS)
            nozzle_park_mks.print_pause_start_flag = 1;
            nozzle_park_mks.blstatus = true;
          #endif
          ExtUI::pausePrint();
          // ExtUI::mks_pausePrint();
        }
        break;
      case 2:  // Abort
        #if !ENABLED(DGUS_LCD_UI_MKS)
          ScreenHandler.HandleUserConfirmationPopUp(VP_SD_AbortPrintConfirmed, nullptr, PSTR("Abort printing"), filelist.filename(), PSTR("?"), true, true, false, true);
        #endif
      break;
    }
  }

  void DGUSScreenHandler::DGUSLCD_SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr) {
    ExtUI::stopPrint();
    GotoScreen(DGUSLCD_SCREEN_MAIN);
  }

  void DGUSScreenHandler::DGUSLCD_SD_PrintTune(DGUS_VP_Variable &var, void *val_ptr) {
    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    GotoScreen(DGUSLCD_SCREEN_SDPRINTTUNE);
  }

  void DGUSScreenHandler::DGUSLCD_SD_SendFilename(DGUS_VP_Variable& var) {
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line > DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;

    #if ENABLED(DGUS_LCD_UI_MKS)
    uint16_t dir_icon_val = 25;

    if (filelist.seek(top_file + target_line)) {
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0);// snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s"), filelist.filename());
      if(filelist.isDir()) {
        dir_icon_val = 0;
      }
      else {
        dir_icon_val = 1;
      }
    }
    #else
    if (filelist.seek(top_file + target_line))
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0);
    #endif
    DGUSLCD_SendStringToDisplay(var);

    #if ENABLED(DGUS_LCD_UI_MKS)
      dgusdisplay.WriteVariable(VP_File_Pictutr0 + target_line * 2, dir_icon_val);
    #endif
  }

  void DGUSScreenHandler::SDCardInserted() {
    top_file = 0;
    filelist.refresh();
    auto cs = ScreenHandler.getCurrentScreen();
    if (cs == DGUSLCD_SCREEN_MAIN || cs == DGUSLCD_SCREEN_STATUS)
    {
        #if ENABLED(DGUS_LCD_UI_MKS)
          ScreenHandler.GotoScreen(MKSLCD_SCREEN_CHOOSE_FILE);
        #else 
          ScreenHandler.GotoScreen(DGUSLCD_SCREEN_SDFILELIST);
        #endif 
    }
      
  }

  void DGUSScreenHandler::SDCardRemoved() {
    if (current_screen == DGUSLCD_SCREEN_SDFILELIST
        || (current_screen == DGUSLCD_SCREEN_CONFIRM && (ConfirmVP == VP_SD_AbortPrintConfirmed || ConfirmVP == VP_SD_FileSelectConfirm))
        || current_screen == DGUSLCD_SCREEN_SDPRINTMANIPULATION
    ) 
    {
      #if ENABLED(DGUS_LCD_UI_MKS)
        filelist.refresh();
      #else 
        ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
      #endif
    }
  }

  void DGUSScreenHandler::SDCardError() {
    DGUSScreenHandler::SDCardRemoved();
    ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("SD card error"), nullptr, true, true, true, true);
    ScreenHandler.SetupConfirmAction(nullptr);
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
  }

#endif // SDSUPPORT

void DGUSScreenHandler::ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr) {
  DGUS_VP_Variable ramcopy;
  if (!populate_VPVar(ConfirmVP, &ramcopy)) return;
  if (ramcopy.set_by_display_handler) ramcopy.set_by_display_handler(ramcopy, val_ptr);
}

const uint16_t* DGUSLCD_FindScreenVPMapList(uint8_t screen) {
  const uint16_t *ret;
  const struct VPMapping *map = VPMap;
  while ((ret = (uint16_t*) pgm_read_ptr(&(map->VPList)))) {
    if (pgm_read_byte(&(map->screen)) == screen) return ret;
    map++;
  }
  return nullptr;
}

const DGUS_VP_Variable* DGUSLCD_FindVPVar(const uint16_t vp) {
  const DGUS_VP_Variable *ret = ListOfVP;
  do {
    const uint16_t vpcheck = pgm_read_word(&(ret->VP));
    if (vpcheck == 0) break;
    if (vpcheck == vp) return ret;
    ++ret;
  } while (1);

  DEBUG_ECHOLNPAIR("FindVPVar NOT FOUND ", vp);
  return nullptr;
}

void DGUSScreenHandler::ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr) {
  if (!ExtUI::isPrinting()) {
    ScreenChangeHook(var, val_ptr);
    dgusdisplay.RequestScreen(current_screen);
  }
}

void DGUSScreenHandler::ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUSLCD_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUSLCD_Screens target = (DGUSLCD_Screens)tmp[1];

  DEBUG_ECHOLNPAIR("\n DEBUG target",target);

  // #if ENABLED(DGUS_LCD_UI_MKS)
  // when the dgus had reboot,it will enter the DGUSLCD_SCREEN_MAIN page,
  // so user can change any page to use this function, an it will check 
  // if robin nano is printting? when it is, dgus will enter the printting  
  // page to continue print;  
  
  // if(print_job_timer.isRunning() || print_job_timer.isPaused())
  // {                                                   
  //     if ( (target = MKSLCD_PAUSE_SETTING_MOVE)
  //       || (target = MKSLCD_PAUSE_SETTING_EX)
  //       || (target = MKSLCD_SCREEN_PRINT)
  //       || (target = MKSLCD_SCREEN_PAUSE))
  //     {
  //     }
  //     else 
  //     {
  //         GotoScreen(MKSLCD_SCREEN_PRINT);  
  //     }                                                                        
  //     return ;                                        
  // } 
  // #endif 

  if (target == DGUSLCD_SCREEN_POPUP) {

    #if ENABLED(DGUS_LCD_UI_MKS)
      ScreenHandler.SetupConfirmAction(ExtUI::setUserConfirmed);
    #endif
    // special handling for popup is to return to previous menu
    if (current_screen == DGUSLCD_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    PopToOldScreen();
    return;
  }

  UpdateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!DGUSLCD_FindScreenVPMapList(target)) DEBUG_ECHOLNPAIR("WARNING: No screen Mapping found for ", target);
  #endif
}


#if ENABLED(DGUS_LCD_UI_MKS)

void DGUSScreenHandler::ScreenBackChange(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t target = swap16(*(uint16_t *)val_ptr);

  DEBUG_ECHOLNPAIR(" back = 0x%x", target);
  switch(target)
  {

  }
}

#endif


void DGUSScreenHandler::HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr) {
  thermalManager.disable_all_heaters();
  ScreenHandler.ForceCompleteUpdate(); // hint to send all data.
}

void DGUSScreenHandler::HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);
  uint16_t acceptedvalue;

  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_T_E0_Set:
        thermalManager.setTargetHotend(newvalue, 0);
        acceptedvalue = thermalManager.temp_hotend[0].target;
        break;
    #endif
    #if HOTENDS >= 2
      case VP_T_E1_Set:
        thermalManager.setTargetHotend(newvalue, 1);
        acceptedvalue = thermalManager.temp_hotend[1].target;
      break;
    #endif
    #if HAS_HEATED_BED
      case VP_T_Bed_Set:
        thermalManager.setTargetBed(newvalue);
        acceptedvalue = thermalManager.temp_bed.target;
        break;
    #endif
  }

  // reply to display the new value to update the view if the new value was rejected by the Thermal Manager.
  if (newvalue != acceptedvalue && var.send_to_display_handler) var.send_to_display_handler(var);
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr) {
  #if EXTRUDERS
    uint16_t newvalue = swap16(*(uint16_t*)val_ptr);
    uint8_t target_extruder;
    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_Flowrate_E0: target_extruder = 0; break;
      #endif
      #if HOTENDS >= 2
        case VP_Flowrate_E1: target_extruder = 1; break;
      #endif
    }

    planner.set_flow(target_extruder, newvalue);
    ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  #else
    UNUSED(var); UNUSED(val_ptr);
  #endif
}

void DGUSScreenHandler::HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualExtrude");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  float target = movevalue * 0.01f;
  ExtUI::extruder_t target_extruder;

  switch (var.VP) {
    #if HOTENDS >= 1
      case VP_MOVE_E0: target_extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >= 2
      case VP_MOVE_E1: target_extruder = ExtUI::extruder_t::E1; break;
    #endif
    default: return;
  }

  target += ExtUI::getAxisPosition_mm(target_extruder);
  ExtUI::setAxisPosition_mm(target, target_extruder);
  skipVP = var.VP;
}

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  void DGUSScreenHandler::HandleManualMoveOption(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleManualMoveOption");
    *(uint16_t*)var.memadr = swap16(*(uint16_t*)val_ptr);
  }
#endif

#if ENABLED(DGUS_LCD_UI_MKS)

  void DGUSScreenHandler::ZoffsetConfirm(DGUS_VP_Variable &var, void *val_ptr) {
    
    // gcode.process_subcommands_now_P(PSTR("M500"));
    gcode.process_subcommands_now_P(PSTR("M500"));

    if(print_job_timer.isRunning())
    {
      ScreenHandler.GotoScreen(MKSLCD_SCREEN_PRINT);
    }
    else if(print_job_timer.isPaused)
    {
      ScreenHandler.GotoScreen(MKSLCD_SCREEN_PAUSE);
    }
  }

  void DGUSScreenHandler::GetTurnOffCtrl(DGUS_VP_Variable &var, void *val_ptr) {

    DEBUG_ECHOLNPGM("ctrl turn off\n");
    uint16_t value = swap16(*(uint16_t *)val_ptr);

    switch(value)
    {
      case 0:
        DGUSAutoTurnOff = value;
      break;

      case 1:
        DGUSAutoTurnOff = value;
      break;

      default:
      break;
    }
  }

  void DGUSScreenHandler::GetMinExtrudeTemp(DGUS_VP_Variable &var, void *val_ptr) {
    
    DEBUG_ECHOLNPGM("MinExtrudeTempChange DistanceChange");

    uint16_t value = swap16(*(uint16_t *)val_ptr);
    thermalManager.extrude_min_temp = value;
    min_ex_temp = value;
    gcode.process_subcommands_now_P(PSTR("M500"));
  }


  void DGUSScreenHandler::GetZoffsetDistance(DGUS_VP_Variable &var, void *val_ptr)
  {
    DEBUG_ECHOLNPGM("Zoffset DistanceChange");
    uint16_t value = swap16(*(uint16_t *)val_ptr);
    float val_distance = 0;
    switch(value)
    {
      case 0: val_distance = 0.01; break;
      case 1: val_distance = 0.1; break;
      case 2: val_distance = 0.5; break;
      case 3: val_distance = 1;break;
      default:
        val_distance = 0.01;
      break;
    }
    ZOffset_distance = val_distance;
  }


  void DGUSScreenHandler::GetManualMovestep(DGUS_VP_Variable &var, void *val_ptr)
  {
    DEBUG_ECHOLNPGM("\nMove DistanceChange");
    *(uint16_t *)var.memadr = swap16(*(uint16_t *)val_ptr);
  }


  void DGUSScreenHandler::EEPROM_CTRL(DGUS_VP_Variable &var, void *val_ptr)
  {
    uint16_t eep_flag = swap16(*(uint16_t *)val_ptr);

    switch(eep_flag)
    {
      case 0:
        
        gcode.process_subcommands_now_P(PSTR("M500"));
        gcode.process_subcommands_now_P(PSTR("M501"));
        // gcode.process_subcommands_now_P(PSTR("M500"));
        // gcode.process_subcommands_now_P(PSTR("M501"));
        ScreenHandler.GotoScreen(MKSLCD_SCREEN_EEP_Config);
      break;

      case 1:
        // gcode.mks_m502();
        gcode.process_subcommands_now_P(PSTR("M502"));
        ScreenHandler.GotoScreen(MKSLCD_SCREEN_EEP_Config);
      break;

      default:
      break;
    }
  }


  void DGUSScreenHandler::Z_offset_select(DGUS_VP_Variable &var, void *val_ptr) {

    uint16_t z_value = swap16(*(uint16_t *)val_ptr);

    switch(z_value) {

      case 0:
        Z_distance = 0.01;
      break;
      
      case 1:
        Z_distance = 0.1;
      break;

      case 2:
        Z_distance = 0.5;
      break;

      default:
        Z_distance = 1;
      break;
    }
  }


  void DGUSScreenHandler::GetOffsetValue(DGUS_VP_Variable &var, void *val_ptr) {

    // uint16_t Offset = swap16(*(uint16_t *)val_ptr);

    int32_t value = swap32(*(int32_t *)val_ptr);
    float Offset;
    DEBUG_ECHOLNPAIR_F("\nget int6 offset >> ", value, 6);
  
    Offset = value;
    Offset = Offset / (float)100;

    DEBUG_ECHOLNPAIR_F(" get float offset >> ", Offset, 6);
    
    switch (var.VP)
    {
    case VP_OFFSET_X:
      #if BOTH(DGUS_LCD_UI_MKS,HAS_BED_PROBE)
        probe.offset.x = Offset;
      #endif
      break;
    case VP_OFFSET_Y:
      #if BOTH(DGUS_LCD_UI_MKS,HAS_BED_PROBE)
        probe.offset.y = Offset;
      #endif
      break;
    case VP_OFFSET_Z:
      #if BOTH(DGUS_LCD_UI_MKS,HAS_BED_PROBE)
        probe.offset.z = Offset;
      #endif
      break;

    default:
      break;
    }
    queue.inject_P(PSTR("M500"));
  }

  void DGUSScreenHandler::LanguageChange_MKS(DGUS_VP_Variable &var, void *val_ptr)
  {
    uint16_t lag_flag = swap16(*(uint16_t *)val_ptr);

    switch(lag_flag)
    {
      case MKS_SimpleChinese:
        DGUS_LanguageDisplay(MKS_SimpleChinese);
        DGUSLanguageSwitch = MKS_SimpleChinese;
        dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1,MKS_Language_Choose);
        dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2,MKS_Language_NoChoose);
        gcode.process_subcommands_now_P(PSTR("M500"));
      break;

      case MKS_English:
        DGUS_LanguageDisplay(MKS_English);
        DGUSLanguageSwitch = MKS_English;
        dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1,MKS_Language_NoChoose);
        dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2,MKS_Language_Choose);
        gcode.process_subcommands_now_P(PSTR("M500"));
      break;
      default:
      break;
    }
  }

#if ENABLED(MESH_BED_LEVELING)
uint8_t mesh_point_count = GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y;
#endif

void DGUSScreenHandler::Level_Ctrl_MKS(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t lev_but = swap16(*(uint16_t *)val_ptr);
  #if ENABLED(MESH_BED_LEVELING)
    auto cs = ScreenHandler.getCurrentScreen();
  #endif
  switch(lev_but)
  {
    case 0:
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        char cmd_buf[10];
        static uint8_t a_first_level = 1;
      
        if(a_first_level == 1)
        {
          a_first_level = 0;

          sprintf(cmd_buf,"G28\n");
          queue.enqueue_one_now(cmd_buf);
        }

        sprintf(cmd_buf,"G29\n");
        queue.enqueue_one_now(cmd_buf);
      #elif ENABLED(MESH_BED_LEVELING)
        mesh_point_count = GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y;

        if(DGUSLanguageSwitch == MKS_English) {
          const char level_buf_en[] = "Start Level";
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en, 32, true);
        }
        else if(DGUSLanguageSwitch == MKS_SimpleChinese){
          const uint16_t level_buf_ch[] = {0xAABF,0XBCCA,0XF7B5,0XBDC6,0X2000};
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch, 32, true);
        }

        cs = ScreenHandler.getCurrentScreen();
        if(cs != MKSLCD_AUTO_LEVEL ) ScreenHandler.GotoScreen(MKSLCD_AUTO_LEVEL);

      #else
        ScreenHandler.GotoScreen(MKSLCD_SCREEN_LEVEL); 
      #endif
    break;

    case 1:
      soft_endstop._enabled = true;
      ScreenHandler.GotoScreen(MKSLCD_SCREEM_TOOL);
    break;

    default:
    break;
  }
}

void DGUSScreenHandler::MeshLevelDistanceConfig(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t mesh_dist = swap16(*(uint16_t *)val_ptr);

  switch(mesh_dist)
  {
    case 0:
      mesh_adj_distance = 0.01;
    break;

    case 1:
      mesh_adj_distance = 0.1;
    break;

    case 2:
      mesh_adj_distance = 1;
    break;

    default:
      mesh_adj_distance = 0.1;
    break;
  }
}

void DGUSScreenHandler::MeshLevel(DGUS_VP_Variable &var, void *val_ptr)
{
  #if ENABLED(MESH_BED_LEVELING)
  uint16_t mesh_value = swap16(*(uint16_t *)val_ptr);
  // static uint8_t a_first_level = 1;
  char cmd_buf[30];
  float offset = mesh_adj_distance;
  
  // float f3 = current_position.z;
  // float f4 = current_position.z;
  switch(mesh_value)
  {
    case 0:
      offset = mesh_adj_distance;
      soft_endstop._enabled = false;
      sprintf(cmd_buf,"G91\n");
      queue.enqueue_one_now(cmd_buf);
      sprintf(cmd_buf,"G1 Z%.2f\n",offset);
      queue.enqueue_one_now(cmd_buf);
      sprintf(cmd_buf,"G90\n");
      queue.enqueue_one_now(cmd_buf);
      // soft_endstop._enabled = true;
    break;

    case 1:
      offset = mesh_adj_distance;

      soft_endstop._enabled = false;

      sprintf(cmd_buf,"G91\n");
      queue.enqueue_one_now(cmd_buf);

      sprintf(cmd_buf,"G1 Z%.2f\n",-offset);
      queue.enqueue_one_now(cmd_buf);

      sprintf(cmd_buf,"G90\n");
      queue.enqueue_one_now(cmd_buf);
    break;

    case 2:
      if(mesh_point_count == (GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y))    //第1个点
      {
        sprintf(cmd_buf,"G29 S1\n");
        queue.enqueue_one_now(cmd_buf);
        mesh_point_count--;
        
        if(DGUSLanguageSwitch == MKS_English)
        {
          const char level_buf_en1[] = "Next Point";
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en1, 32, true);
        }
        else if(DGUSLanguageSwitch == MKS_SimpleChinese)
        {
          const uint16_t level_buf_ch1[] = {0XC2CF,0XBBD2,0XE3B5,0X2000};
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch1, 32, true);
        }
      }
      else if(mesh_point_count > 1)                                      // 倒数第二个点
      {
        sprintf(cmd_buf,"G29 S2\n");
        queue.enqueue_one_now(cmd_buf);
        mesh_point_count--;
        if(DGUSLanguageSwitch == MKS_English)
        {
          const char level_buf_en2[] = "Next Point";
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en2, 32, true);
        }
        else if(DGUSLanguageSwitch == MKS_SimpleChinese)
        {
          const uint16_t level_buf_ch2[] = {0XC2CF,0XBBD2,0XE3B5,0X2000};
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch2, 32, true);
        }
      }
      else if (mesh_point_count == 1) {
        sprintf(cmd_buf,"G29 S2\n");
        queue.enqueue_one_now(cmd_buf);
        mesh_point_count--;
        if(DGUSLanguageSwitch == MKS_English)
        {
          const char level_buf_en2[] = "Level Finsh";
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en2, 32, true);
        }
        else if(DGUSLanguageSwitch == MKS_SimpleChinese)
        {
          const uint16_t level_buf_ch2[] = {0xF7B5,0XBDC6,0XEACD,0XC9B3,0X2000};
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch2, 32, true);
        }

        queue.inject_P(PSTR("M500"));
      }
      else if ( mesh_point_count == 0 ) {
      
        mesh_point_count = GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y;
        soft_endstop._enabled = true;
        queue.inject_P(PSTR("M500"));
        ScreenHandler.GotoScreen(MKSLCD_SCREEM_TOOL);
      }
    break;

    default:
    break;
  }
  #endif
}

void DGUSScreenHandler::ManualAssistLeveling(DGUS_VP_Variable &var, void *val_ptr)
{
  int16_t point_value = swap16(*(uint16_t *)val_ptr);

  int16_t level_x_pos, level_y_pos;
  char buf_level[32] = {0};
  unsigned int level_speed = 1500;
  static uint8_t first_level_flag = 1;


  if(first_level_flag == 1)
  {
    sprintf(buf_level, PSTR("G28"));
    queue.enqueue_one_now(buf_level);
  }

  switch(point_value)
  {
    case 0x0001:
      if(first_level_flag != 1)
      {
        sprintf(buf_level, PSTR("G28"));
        queue.enqueue_one_now(buf_level);
      }
      sprintf(buf_level, PSTR("G1 Z10"));
      queue.enqueue_one_now(buf_level);
      // level_x_pos = X_MIN_POS + 20;
      // level_y_pos = Y_MIN_POS + 20;
      level_x_pos = X_MIN_POS + abs(level_1_x_point);
      level_y_pos = Y_MIN_POS + abs(level_1_y_point);

      memset(buf_level, 0, sizeof(buf_level));
      sprintf_P(buf_level, "G0 X%d Y%d F%d", level_x_pos, level_y_pos, level_speed);
      queue.enqueue_one_now(buf_level);
      sprintf(buf_level, PSTR("G28 Z"));
      queue.enqueue_one_now(buf_level); 
      break;
    case 0x0002:
    sprintf(buf_level, PSTR("G1 Z10"));
    queue.enqueue_one_now(buf_level);

    // level_x_pos = X_MAX_POS - 20;
    // level_y_pos = Y_MIN_POS + 20;

    level_x_pos = X_MAX_POS - abs(level_2_x_point);
    level_y_pos = Y_MIN_POS + abs(level_2_y_point);

    sprintf_P(buf_level, "G0 X%d Y%d F%d", level_x_pos, level_y_pos, level_speed);
    queue.enqueue_one_now(buf_level);
    // sprintf(buf_level, PSTR("G28 Z"));
    // queue.enqueue_one_now(buf_level);
    sprintf(buf_level, PSTR("G1 Z-10"));
    queue.enqueue_one_now(buf_level);
    break;
   case 0x0003:
    sprintf(buf_level, PSTR("G1 Z10"));
    queue.enqueue_one_now(buf_level);

    // level_x_pos = X_MAX_POS - 20;
    // level_y_pos = Y_MAX_POS - 20;

    level_x_pos = X_MAX_POS - abs(level_3_x_point);
    level_y_pos = Y_MAX_POS - abs(level_3_y_point); 

    sprintf_P(buf_level, "G0 X%d Y%d F%d", level_x_pos, level_y_pos, level_speed);
    queue.enqueue_one_now(buf_level);
    // sprintf(buf_level, PSTR("G28 Z"));
    sprintf(buf_level, PSTR("G1 Z-10"));
    queue.enqueue_one_now(buf_level);
    break;
   case 0x0004:
    sprintf(buf_level, PSTR("G1 Z10"));
    queue.enqueue_one_now(buf_level);

    // level_x_pos = X_MIN_POS + 20;
    // level_y_pos = Y_MAX_POS - 20;
    level_x_pos = X_MIN_POS + abs(level_4_x_point);
    level_y_pos = Y_MAX_POS - abs(level_4_y_point);  

    sprintf_P(buf_level, "G0 X%d Y%d F%d", level_x_pos, level_y_pos, level_speed);
    queue.enqueue_one_now(buf_level);
    // sprintf(buf_level, PSTR("G28 Z"));
    sprintf(buf_level, PSTR("G1 Z-10"));
    queue.enqueue_one_now(buf_level);
    break;
   case 0x0005:
    sprintf(buf_level, PSTR("G1 Z10"));
    queue.enqueue_one_now(buf_level);
    // level_x_pos = (uint16_t)(X_MAX_POS / 2);
    // level_y_pos = (uint16_t)(Y_MAX_POS / 2);
    level_x_pos = abs(level_5_x_point);
    level_y_pos = abs(level_5_y_point); 

    sprintf_P(buf_level, "G0 X%d Y%d F%d", level_x_pos, level_y_pos, level_speed);
    queue.enqueue_one_now(buf_level);
    // sprintf(buf_level, PSTR("G28 Z"));
    sprintf(buf_level, PSTR("G1 Z-10"));
    queue.enqueue_one_now(buf_level);
    break;
  }

    /* Only once */
    if(first_level_flag == 1)
      first_level_flag = 0;
}

#define mks_min(a,b) ((a)<(b)) ? (a):(b)
#define mks_max(a,b) ((a)>(b)) ? (a):(b)
void DGUSScreenHandler::TMC_ChangeConfig(DGUS_VP_Variable &var, void *val_ptr)
{
    #if EITHER(HAS_TRINAMIC_CONFIG,HAS_STEALTHCHOP)  
    uint16_t tmc_value = swap16(*(uint16_t*)val_ptr);
    #endif

    switch(var.VP)
    {
      case VP_TMC_X_STEP:
        #if AXIS_HAS_STEALTHCHOP(X)
          stepperX.homing_threshold(mks_min(tmc_value,255));
          gcode.process_subcommands_now_P(PSTR("M500"));
          // tmc_x_step = stepperX.homing_threshold();
        #endif
      break;
      case VP_TMC_Y_STEP :
        #if AXIS_HAS_STEALTHCHOP(Y)
          stepperY.homing_threshold(mks_min(tmc_value,255));
          gcode.process_subcommands_now_P(PSTR("M500"));
          // tmc_y_step = stepperY.homing_threshold();
        #endif
      break;   
      case VP_TMC_Z_STEP :
        #if AXIS_HAS_STEALTHCHOP(Z)
          stepperZ.homing_threshold(mks_min(tmc_value,255));
          gcode.process_subcommands_now_P(PSTR("M500"));
          // tmc_z_step = stepperZ.homing_threshold();
        #endif
      break;
      case VP_TMC_X_Current:
        #if AXIS_IS_TMC(X)
          stepperX.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_X1_Current:
        #if AXIS_IS_TMC(X2)
          stepperX2.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_Y_Current:
        #if AXIS_IS_TMC(Y)
          stepperY.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_Y1_Current:
          #if AXIS_IS_TMC(X2)
          stepperY2.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_Z_Current:
        #if AXIS_IS_TMC(Z)
          stepperZ.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_Z1_Current:
        #if AXIS_IS_TMC(Z2)
          stepperZ2.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_E0_Current:
        #if AXIS_IS_TMC(E0)
          stepperE0.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;
      case VP_TMC_E1_Current:
        #if AXIS_IS_TMC(E1)
          stepperE1.rms_current(tmc_value);
          gcode.process_subcommands_now_P(PSTR("M500"));
        #endif
      break;

      default:
      break;
    }
    #if AXIS_HAS_STEALTHCHOP(X)
      tmc_x_step = stepperX.homing_threshold();
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      tmc_y_step = stepperY.homing_threshold();
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      tmc_z_step = stepperZ.homing_threshold();
    #endif
}
#endif 

#if ENABLED(DGUS_LCD_UI_MKS)
void DGUSScreenHandler::HandleManualMove(DGUS_VP_Variable &var, void *val_ptr) {

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);

  DEBUG_ECHOLNPGM("HandleManualMove");
  char axiscode;
  unsigned int speed = 1500;  //FIXME: get default feedrate for manual moves, dont hardcode.
  /* Choose Move distance */

  if (distanceMove == 0x01)
    distanceMove = 10;
  else if (distanceMove == 0x02)
    distanceMove = 100;
  else if (distanceMove == 0x03)
    distanceMove = 1000;

  DEBUG_ECHOLNPAIR("QUEUE LEN:", queue.length);

  if(!print_job_timer.isPaused()) {
    if(queue.length >= BUFSIZE) {  return ;}
  }
  
  switch (var.VP) {     // switch X Y Z or Home
    default: return;
    case VP_MOVE_X:
      DEBUG_ECHOLNPGM("X Move");
      axiscode = 'X';
      if (!ExtUI::canMove(ExtUI::axis_t::X)) goto cannotmove;
      break;

    case VP_MOVE_Y:
      DEBUG_ECHOLNPGM("Y Move");
      axiscode = 'Y';
      if (!ExtUI::canMove(ExtUI::axis_t::Y)) goto cannotmove;
      break;

    case VP_MOVE_Z:
      DEBUG_ECHOLNPGM("Z Move");
      axiscode = 'Z';
      speed = 300; // default to 5mm/s
      if (!ExtUI::canMove(ExtUI::axis_t::Z)) goto cannotmove;
    break;

    case VP_MOTOR_LOCK_UNLOK:
      DEBUG_ECHOLNPGM("Motor Unlock");
      // gcode.motor_unlock();
      movevalue = 5;
      axiscode = '\0';
      // return ;
    break;

    case VP_HOME_ALL: // only used for homing
      DEBUG_ECHOLNPGM("Home all");
      axiscode = '\0';
      movevalue = 0; // ignore value sent from display, this VP is _ONLY_ for homing.
        // gcode.motor_all_back();
        // return ;
      break;
                       
    case VP_X_HOME:
      DEBUG_ECHOLNPGM("X Home");
      axiscode = 'X';
      movevalue = 0;
    break;

    case VP_Y_HOME:
      DEBUG_ECHOLNPGM("Y Home");
      axiscode = 'Y';
      movevalue = 0;
    break;

    case VP_Z_HOME:
      DEBUG_ECHOLNPGM("Z Home");
      axiscode = 'Z';
      movevalue = 0;
    break;           
  }
  
  DEBUG_ECHOPAIR("movevalue = ",movevalue);
  if ((movevalue != 0) && (movevalue != 5))   // get move distance
  {
    switch (movevalue)
    {
      case 0x0001: movevalue = 0 + distanceMove;  
      break;
      case 0x0002: movevalue = 0 - distanceMove;
      break;
      
      default:
          movevalue = 0 ;
      break;
    }
  }

  if (!movevalue) {
    // homing
    DEBUG_ECHOPAIR(" homing ", axiscode);
    // char buf[6] = "G28 X";
    // buf[4] = axiscode;

    char buf[6];
    sprintf(buf,"G28 %c",axiscode);
    //DEBUG_ECHOPAIR(" ", buf);
    // queue.enqueue_one_now(buf);
    gcode.process_subcommands_now_P(buf);
    //DEBUG_ECHOLNPGM(" ✓");
    ScreenHandler.ForceCompleteUpdate();
    return;
  }
  else if(movevalue == 5)
  {
    DEBUG_ECHOPAIR("send M84");
    char buf[6];
    sprintf(buf,"M84 %c",axiscode);
    // queue.enqueue_one_now(buf);
    gcode.process_subcommands_now_P(buf);
    ScreenHandler.ForceCompleteUpdate();
    return ;
  }
  else {
    //movement
    DEBUG_ECHOPAIR(" move ", axiscode);
    bool old_relative_mode = relative_mode;

    if (!relative_mode) {
      //DEBUG_ECHOPGM(" G91");
      // queue.enqueue_now_P(PSTR("G91"));
      gcode.process_subcommands_now_P(PSTR("G91"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
    char buf[32];  // G1 X9999.99 F12345
    // unsigned int backup_speed = MMS_TO_MMM(feedrate_mm_s);
    char sign[]="\0";
    int16_t value = movevalue / 100;
    if (movevalue < 0) { value = -value; sign[0] = '-'; }
    int16_t fraction = ABS(movevalue) % 100;
    snprintf_P(buf, 32, PSTR("G0 %c%s%d.%02d F%d"), axiscode, sign, value, fraction, speed);

    // queue.enqueue_one_now(buf);
    gcode.process_subcommands_now_P(buf);


    // if (backup_speed != speed) {
    //   snprintf_P(buf, 32, PSTR("G0 F%d"), backup_speed);
    //   queue.enqueue_one_now(buf);
    //   //DEBUG_ECHOPAIR(" ", buf);
    // }

    //while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLNPGM(" ✓ ");
    if (!old_relative_mode) {
      //DEBUG_ECHOPGM("G90");
      // queue.enqueue_now_P(PSTR("G90"));
      gcode.process_subcommands_now_P(PSTR("G90"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
  }

  ScreenHandler.ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("manmv done.");
  return;
  cannotmove:
  DEBUG_ECHOLNPAIR(" cannot move ", axiscode);
  return;
}
#else
void DGUSScreenHandler::HandleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualMove");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    if (movevalue) {
      const uint16_t choice = *(uint16_t*)var.memadr;
      movevalue = movevalue < 0 ? -choice : choice;
    }
  #endif
  char axiscode;
  unsigned int speed = 1500;  //FIXME: get default feedrate for manual moves, dont hardcode.

  switch (var.VP) {
    default: return;

    case VP_MOVE_X:
      axiscode = 'X';
      if (!ExtUI::canMove(ExtUI::axis_t::X)) goto cannotmove;
      break;

    case VP_MOVE_Y:
      axiscode = 'Y';
      if (!ExtUI::canMove(ExtUI::axis_t::Y)) goto cannotmove;
      break;

    case VP_MOVE_Z:
      axiscode = 'Z';
      speed = 300; // default to 5mm/s
      if (!ExtUI::canMove(ExtUI::axis_t::Z)) goto cannotmove;
      break;

    case VP_HOME_ALL: // only used for homing
      axiscode = '\0';
      movevalue = 0; // ignore value sent from display, this VP is _ONLY_ for homing.
      break;
  }

  if (!movevalue) {
    // homing
    DEBUG_ECHOPAIR(" homing ", axiscode);
    char buf[6] = "G28 X";
    buf[4] = axiscode;
    //DEBUG_ECHOPAIR(" ", buf);
    queue.enqueue_one_now(buf);
    //DEBUG_ECHOLNPGM(" ✓");
    ScreenHandler.ForceCompleteUpdate();
    return;
  }
  else {
    //movement
    DEBUG_ECHOPAIR(" move ", axiscode);
    bool old_relative_mode = relative_mode;
    if (!relative_mode) {
      //DEBUG_ECHOPGM(" G91");
      queue.enqueue_now_P(PSTR("G91"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
    char buf[32];  // G1 X9999.99 F12345
    unsigned int backup_speed = MMS_TO_MMM(feedrate_mm_s);
    char sign[]="\0";
    int16_t value = movevalue / 100;
    if (movevalue < 0) { value = -value; sign[0] = '-'; }
    int16_t fraction = ABS(movevalue) % 100;
    snprintf_P(buf, 32, PSTR("G0 %c%s%d.%02d F%d"), axiscode, sign, value, fraction, speed);
    //DEBUG_ECHOPAIR(" ", buf);
    queue.enqueue_one_now(buf);
    //DEBUG_ECHOLNPGM(" ✓ ");
    if (backup_speed != speed) {
      snprintf_P(buf, 32, PSTR("G0 F%d"), backup_speed);
      queue.enqueue_one_now(buf);
      //DEBUG_ECHOPAIR(" ", buf);
    }
    //while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLNPGM(" ✓ ");
    if (!old_relative_mode) {
      //DEBUG_ECHOPGM("G90");
      queue.enqueue_now_P(PSTR("G90"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
  }

  ScreenHandler.ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("manmv done.");
  return;

  cannotmove:
  DEBUG_ECHOLNPAIR(" cannot move ", axiscode);
  return;
}
#endif

void DGUSScreenHandler::HandleMotorLockUnlock(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMotorLockUnlock");

  char buf[4];
  const int16_t lock = swap16(*(uint16_t*)val_ptr);
  strcpy_P(buf, lock ? PSTR("M18") : PSTR("M17"));

  //DEBUG_ECHOPAIR(" ", buf);
  queue.enqueue_one_now(buf);
}

#if ENABLED(POWER_LOSS_RECOVERY)

  void DGUSScreenHandler::HandlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    if (value) {
      queue.inject_P(PSTR("M1000"));
      #if DISABLED(DGUS_LCD_UI_MKS)
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_SDPRINTMANIPULATION);
      #else
      // dgusdisplay.WriteVariable(VP_SD_Print_Filename,recovery.info.sd_filename, 32, true);
      dgusdisplay.WriteVariable(VP_SD_Print_Filename, filelist.filename(), 32, true);
      ScreenHandler.GotoScreen(MKSLCD_SCREEN_PRINT);
      #endif 
    }
    else {
      recovery.cancel();
      
      #if DISABLED(DGUS_LCD_UI_MKS)
        queue.inject_P(PSTR("G28"));
        ScreenHandler.GotoScreen(DGUSLCD_SCREEN_STATUS);
      #else 
        ScreenHandler.GotoScreen(MKSLCD_SCREEN_HOME);
      #endif 
    }
  }

#endif

void DGUSScreenHandler::HandleSettings(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleSettings");
  uint16_t value = swap16(*(uint16_t*)val_ptr);
  switch (value) {
    default: break;
    case 1:
      TERN_(PRINTCOUNTER, print_job_timer.initStats());
      queue.inject_P(PSTR("M502\nM500"));
      break;
    case 2: queue.inject_P(PSTR("M501")); break;
    case 3: queue.inject_P(PSTR("M500")); break;
  }
}

void DGUSScreenHandler::HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMChanged");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw/10;
  ExtUI::axis_t axis;
  switch (var.VP) {
    case VP_X_STEP_PER_MM: axis = ExtUI::axis_t::X; break;
    case VP_Y_STEP_PER_MM: axis = ExtUI::axis_t::Y; break;
    case VP_Z_STEP_PER_MM: axis = ExtUI::axis_t::Z; break;
    default: return;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisSteps_per_mm(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(axis));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

#if ENABLED(DGUS_LCD_UI_MKS)

void DGUSScreenHandler::GetParkPos_MKS(DGUS_VP_Variable &var, void *val_ptr) 
{
  int16_t value_pos = swap16(*(int16_t*)val_ptr);

  switch(var.VP)
  {
    case VP_X_PARK_POS:
      x_park_pos = value_pos;
    break;
    case VP_Y_PARK_POS:
      y_park_pos = value_pos;
    break;
    case VP_Z_PARK_POS:
      z_park_pos = value_pos;
    break;
    default:
    break;
  }
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return ;
}

void DGUSScreenHandler::HandleChangeLevelPoint_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMChanged");

  int16_t value_raw = swap16(*(int16_t*)val_ptr);

  DEBUG_ECHOLNPAIR_F("value:", value_raw);

  *(int16_t*)var.memadr = value_raw;

  gcode.process_subcommands_now_P(PSTR("M500"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}
  
void DGUSScreenHandler::HandleStepPerMMChanged_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMChanged");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);

  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::axis_t axis;
  switch (var.VP) {
    case VP_X_STEP_PER_MM: axis = ExtUI::axis_t::X; break;
    case VP_Y_STEP_PER_MM: axis = ExtUI::axis_t::Y; break;
    case VP_Z_STEP_PER_MM: axis = ExtUI::axis_t::Z; break;
    default: return;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisSteps_per_mm(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(axis));
  gcode.process_subcommands_now_P(PSTR("M500")); 
  gcode.process_subcommands_now_P(PSTR("M501"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleStepPerMMExtruderChanged_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMExtruderChanged");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_E0_STEP_PER_MM: extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >= 2
    #endif
      case VP_E1_STEP_PER_MM: extruder = ExtUI::extruder_t::E1; break;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisSteps_per_mm(value,extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(extruder));
  gcode.process_subcommands_now_P(PSTR("M500")); 
  gcode.process_subcommands_now_P(PSTR("M501"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleMaxSpeedChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMaxSpeedChange_MKS");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::axis_t axis;
  switch (var.VP) {
    case VP_X_MAX_SPEED: axis = ExtUI::axis_t::X; break;
    case VP_Y_MAX_SPEED: axis = ExtUI::axis_t::Y; break;
    case VP_Z_MAX_SPEED: axis = ExtUI::axis_t::Z; break;
    default : return;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  // ExtUI::setAxisSteps_per_mm(value,extruder);
  ExtUI::setAxisMaxFeedrate_mm_s(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxFeedrate_mm_s(axis)); 
  gcode.process_subcommands_now_P(PSTR("M500")); 
  gcode.process_subcommands_now_P(PSTR("M501"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleExtruderMaxSpeedChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMaxSpeedChange_MKS");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_E0_MAX_SPEED: extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >= 2
    #endif
      case VP_E1_MAX_SPEED: extruder = ExtUI::extruder_t::E1; break;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  // ExtUI::setAxisSteps_per_mm(value,extruder);
  ExtUI::setAxisMaxFeedrate_mm_s(value, extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxFeedrate_mm_s(extruder)); 
  gcode.process_subcommands_now_P(PSTR("M500")); 
  gcode.process_subcommands_now_P(PSTR("M501"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}


void DGUSScreenHandler::HandleMaxAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMaxSpeedChange_MKS");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::axis_t axis;
  switch (var.VP) {
    case VP_X_ACC_MAX_SPEED: axis = ExtUI::axis_t::X;  break;
    case VP_Y_ACC_MAX_SPEED: axis = ExtUI::axis_t::Y;  break;
    case VP_Z_ACC_MAX_SPEED: axis = ExtUI::axis_t::Z;  break;
    default : return;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisMaxAcceleration_mm_s2(value,axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxAcceleration_mm_s2(axis)); 
  gcode.process_subcommands_now_P(PSTR("M500")); 
  gcode.process_subcommands_now_P(PSTR("M501"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleExtruderAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMaxSpeedChange_MKS");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_E0_ACC_MAX_SPEED: extruder = ExtUI::extruder_t::E0; gcode.process_subcommands_now_P(PSTR("M501")); break;
    #endif
    #if HOTENDS >= 2
    #endif
      case VP_E1_ACC_MAX_SPEED: extruder = ExtUI::extruder_t::E1; gcode.process_subcommands_now_P(PSTR("M501")); break;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  // ExtUI::setAxisSteps_per_mm(value,extruder);
  ExtUI::setAxisMaxAcceleration_mm_s2(value, extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxAcceleration_mm_s2(extruder)); 
  gcode.process_subcommands_now_P(PSTR("M500")); 
  gcode.process_subcommands_now_P(PSTR("M501"));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleTravelAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t value_travel = swap16(*(uint16_t*)val_ptr);
  float value = (float)value_travel;
  planner.settings.travel_acceleration = value;

  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleFeedRateMinChange_MKS(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t value_t = swap16(*(uint16_t*)val_ptr);
  float value = (float)value_t;
  planner.settings.min_feedrate_mm_s = value;
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleMin_T_F_MKS(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t value_t_f = swap16(*(uint16_t*)val_ptr);
  float value = (float)value_t_f;
  planner.settings.min_travel_feedrate_mm_s = value;
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t value_acc = swap16(*(uint16_t*)val_ptr);
  float value = (float)value_acc;
  planner.settings.acceleration = value;
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleGetExMinTemp_MKS(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t value_ex_min_temp = swap16(*(uint16_t*)val_ptr);

  thermalManager.extrude_min_temp = value_ex_min_temp;

  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

#endif

void DGUSScreenHandler::HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMExtruderChanged");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw/10;
  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_E0_STEP_PER_MM: extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >= 2
      case VP_E1_STEP_PER_MM: extruder = ExtUI::extruder_t::E1; break;
    #endif
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisSteps_per_mm(value,extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(extruder));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

#if HAS_PID_HEATING
  void DGUSScreenHandler::HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t rawvalue = swap16(*(uint16_t*)val_ptr);
    DEBUG_ECHOLNPAIR("V1:", rawvalue);
    float value = (float)rawvalue / 10;
    DEBUG_ECHOLNPAIR("V2:", value);
    float newvalue = 0;

    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_E0_PID_P: newvalue = value; break;
        case VP_E0_PID_I: newvalue = scalePID_i(value); break;
        case VP_E0_PID_D: newvalue = scalePID_d(value); break;
      #endif
      #if HOTENDS >= 2
        case VP_E1_PID_P: newvalue = value; break;
        case VP_E1_PID_I: newvalue = scalePID_i(value); break;
        case VP_E1_PID_D: newvalue = scalePID_d(value); break;
      #endif
      #if HAS_HEATED_BED
        case VP_BED_PID_P: newvalue = value; break;
        case VP_BED_PID_I: newvalue = scalePID_i(value); break;
        case VP_BED_PID_D: newvalue = scalePID_d(value); break;
      #endif
    }

    DEBUG_ECHOLNPAIR_F("V3:", newvalue);
    *(float *)var.memadr = newvalue;

    #if ENABLED(DGUS_LCD_UI_MKS)
      queue.inject_P(PSTR("M500"));
    #endif
    ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }

  void DGUSScreenHandler::HandlePIDAutotune(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandlePIDAutotune");

    char buf[32] = {0};

    switch (var.VP) {
      default: break;
      #if ENABLED(PIDTEMP)
        #if HOTENDS >= 1
          case VP_PID_AUTOTUNE_E0: // Autotune Extruder 0
            sprintf(buf, "M303 E%d C5 S210 U1", ExtUI::extruder_t::E0);
            break;
        #endif
        #if HOTENDS >= 2
          case VP_PID_AUTOTUNE_E1:
            sprintf(buf, "M303 E%d C5 S210 U1", ExtUI::extruder_t::E1);
            break;
        #endif
      #endif
      #if ENABLED(PIDTEMPBED)
        case VP_PID_AUTOTUNE_BED:
          sprintf(buf, "M303 E-1 C5 S70 U1");
          break;
      #endif
    }

    if (buf[0]) queue.enqueue_one_now(buf);

    #if ENABLED(DGUS_UI_WAITING)
      sendinfoscreen(PSTR("PID is autotuning"), PSTR("please wait"), NUL_STR, NUL_STR, true, true, true, true);
      GotoScreen(DGUSLCD_SCREEN_WAITING);
    #endif
  }
#endif

#if HAS_BED_PROBE
  void DGUSScreenHandler::HandleProbeOffsetZChanged(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleProbeOffsetZChanged");

    const float offset = float(int16_t(swap16(*(uint16_t*)val_ptr))) / 100.0f;
    ExtUI::setZOffset_mm(offset);
    ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
    return;
  }
#endif

#if ENABLED(BABYSTEPPING)
  void DGUSScreenHandler::HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleLiveAdjustZ");

    #if ENABLED(DGUS_LCD_UI_MKS)
      uint16_t flag = swap16(*(uint16_t*)val_ptr);
      char babystep_buf[30];

        switch(flag)
        {
          case 0:
            DEBUG_ECHOLNPGM("babystep down");
            sprintf(babystep_buf,"M290 Z%.3f",-ZOffset_distance);
            z_offset_add = z_offset_add - ZOffset_distance;
          break;

          case 1:
            DEBUG_ECHOLNPGM("babystep up");
            sprintf(babystep_buf,"M290 Z%.3f",ZOffset_distance);
            z_offset_add = z_offset_add + ZOffset_distance;
          break;

          default:
          break;
        }
      gcode.process_subcommands_now_P(babystep_buf);

    #else 
      int16_t flag = swap16(*(uint16_t*)val_ptr);
      int16_t steps = flag ? -20 : 20;
      ExtUI::smartAdjustAxis_steps(steps, ExtUI::axis_t::Z, true);
    #endif
    ScreenHandler.ForceCompleteUpdate();
    return;
  }
#endif

#if HAS_FAN
  void DGUSScreenHandler::HandleFanControl(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleFanControl");
    *(uint8_t*)var.memadr = *(uint8_t*)var.memadr > 0 ? 0 : 255;
  }
#endif

void DGUSScreenHandler::HandleHeaterControl(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleHeaterControl");

  uint8_t preheat_temp = 0;
  switch (var.VP) {
    #if HOTENDS >= 1
      case VP_E0_CONTROL:
    #endif
    #if HOTENDS >= 2
      case VP_E1_CONTROL:
    #endif
    #if HOTENDS >= 3
      case VP_E2_CONTROL:
    #endif
      preheat_temp = PREHEAT_1_TEMP_HOTEND;
      break;

    case VP_BED_CONTROL:
      preheat_temp = PREHEAT_1_TEMP_BED;
      break;
  }

  *(int16_t*)var.memadr = *(int16_t*)var.memadr > 0 ? 0 : preheat_temp;
}

#if ENABLED(DGUS_PREHEAT_UI)

  void DGUSScreenHandler::HandlePreheat(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandlePreheat");

    uint8_t e_temp = 0;
    TERN_(HAS_HEATED_BED, uint8_t bed_temp = 0);
    const uint16_t preheat_option = swap16(*(uint16_t*)val_ptr);
    switch (preheat_option) {
      default:
      case 0: // Preheat PLA
        #if defined(PREHEAT_1_TEMP_HOTEND) && defined(PREHEAT_1_TEMP_BED)
          e_temp = PREHEAT_1_TEMP_HOTEND;
          TERN_(HAS_HEATED_BED, bed_temp = PREHEAT_1_TEMP_BED);
        #endif
        break;
      case 1: // Preheat ABS
        #if defined(PREHEAT_2_TEMP_HOTEND) && defined(PREHEAT_2_TEMP_BED)
          e_temp = PREHEAT_2_TEMP_HOTEND;
          TERN_(HAS_HEATED_BED, bed_temp = PREHEAT_2_TEMP_BED);
        #endif
        break;
      case 2: // Preheat PET
        #if defined(PREHEAT_3_TEMP_HOTEND) && defined(PREHEAT_3_TEMP_BED)
          e_temp = PREHEAT_3_TEMP_HOTEND;
          TERN_(HAS_HEATED_BED, bed_temp = PREHEAT_3_TEMP_BED);
        #endif
        break;
      case 3: // Preheat FLEX
        #if defined(PREHEAT_4_TEMP_HOTEND) && defined(PREHEAT_4_TEMP_BED)
          e_temp = PREHEAT_4_TEMP_HOTEND;
          TERN_(HAS_HEATED_BED, bed_temp = PREHEAT_4_TEMP_BED);
        #endif
        break;
      case 7: break; // Custom preheat
      case 9: break; // Cool down
    }

    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_E0_BED_PREHEAT:
          thermalManager.setTargetHotend(e_temp, 0);
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(bed_temp));
          break;
      #endif
      #if HOTENDS >= 2
        case VP_E1_BED_PREHEAT:
          thermalManager.setTargetHotend(e_temp, 1);
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(bed_temp));
        break;
      #endif
    }

    // Go to the preheat screen to show the heating progress
    GotoScreen(DGUSLCD_SCREEN_PREHEAT);
  }

#endif


#if ENABLED(DGUS_LCD_UI_MKS)
void DGUSScreenHandler::GetManualFilament(DGUS_VP_Variable &var, void *val_ptr)
{
  DEBUG_ECHOLNPGM("HandleGetFilament");

  uint16_t value_len = swap16(*(uint16_t*)val_ptr);

  float value = (float)value_len;

  DEBUG_ECHOLNPAIR_F("Get Filament len value:", value);
  distanceFilament = value;

  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::GetManualFilamentSpeed(DGUS_VP_Variable &var, void *val_ptr)
{
  DEBUG_ECHOLNPGM("HandleGetFilamentSpeed");

  uint16_t value_len = swap16(*(uint16_t*)val_ptr);

  DEBUG_ECHOLNPAIR_F("FilamentSpeed value:", value_len);

  FilamentSpeed = value_len;

  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}


  void DGUSScreenHandler::MKS_FilamentLoad(DGUS_VP_Variable &var, void *val_ptr)
  {
    DEBUG_ECHOLNPGM("Load Filament");
    char buf[40];

    uint16_t val_t = swap16(*(uint16_t*)val_ptr);

    switch(val_t) {
      case 0:
        #if HOTENDS >= 1
          if(thermalManager.temp_hotend[0].celsius < thermalManager.extrude_min_temp) {
            if ((thermalManager.temp_hotend[0].target == 0) || 
                (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)) {
              thermalManager.setTargetHotend(thermalManager.extrude_min_temp, 0);
            }
            ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait nozzle"), PSTR("heat!"), true, true, true, true);
            ScreenHandler.SetupConfirmAction(nullptr);
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
          }
          else {
            sprintf(buf,"T0\nG91\nG1 E%d F%d\nG90",(int)distanceFilament,FilamentSpeed * 60);
            gcode.process_subcommands_now_P(buf);
          }
        #endif
      break;

      case 1:
        #if HOTENDS >= 2
          if(thermalManager.temp_hotend[1].celsius < thermalManager.extrude_min_temp) {
            if ((thermalManager.temp_hotend[1].target == 0) || 
                (thermalManager.temp_hotend[1].target < thermalManager.extrude_min_temp)) {
              thermalManager.setTargetHotend(thermalManager.extrude_min_temp, 1);
            }
            ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait nozzle"), PSTR("heat!"), true, true, true, true);
            ScreenHandler.SetupConfirmAction(nullptr);
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
          }
          else {
            sprintf(buf,"T1\nG91\nG1 E%d F%d\nG90",(int)distanceFilament,FilamentSpeed * 60);
            gcode.process_subcommands_now_P(buf);
          }
        #endif
        #if ENABLED(SINGLENOZZLE)
          if(thermalManager.temp_hotend[0].celsius < thermalManager.extrude_min_temp) {
            if ((thermalManager.temp_hotend[0].target == 0) || 
                (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)) {
              thermalManager.setTargetHotend(thermalManager.extrude_min_temp, 0);
            }
            ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait nozzle"), PSTR("heat!"), true, true, true, true);
            ScreenHandler.SetupConfirmAction(nullptr);
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
          }
          else {
            sprintf(buf,"T0\nG91\nG1 E%d F%d\nG90",(int)distanceFilament,FilamentSpeed * 60);
            gcode.process_subcommands_now_P(buf);
          }
        #endif
      break;

      default:
      break;
    }
  }

  void DGUSScreenHandler::MKS_FilamentUnLoad(DGUS_VP_Variable &var, void *val_ptr)
  {
    DEBUG_ECHOLNPGM("UnLoad Filament");
    char buf[40];
    uint16_t val_t = swap16(*(uint16_t*)val_ptr);

    switch(val_t) {
      case 0:
        #if HOTENDS >= 1
          if(thermalManager.temp_hotend[0].celsius < thermalManager.extrude_min_temp) {
            if ((thermalManager.temp_hotend[0].target == 0) || 
                (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)) {
              thermalManager.setTargetHotend(thermalManager.extrude_min_temp, 0);
            }
            ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait nozzle"), PSTR("heat!"), true, true, true, true);
            ScreenHandler.SetupConfirmAction(nullptr);
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
          }
          else {
            sprintf(buf,"T0\nG91\nG1 E-%d F%d\nG90",(int)distanceFilament,FilamentSpeed * 60);
            gcode.process_subcommands_now_P(buf);
          }
        #endif
      break;
      case 1:
        #if HOTENDS >= 2
          if(thermalManager.temp_hotend[1].celsius < thermalManager.extrude_min_temp) {
            if ((thermalManager.temp_hotend[1].target == 0) || 
                (thermalManager.temp_hotend[1].target < thermalManager.extrude_min_temp)) {
              thermalManager.setTargetHotend(thermalManager.extrude_min_temp, 1);
            }
            ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait nozzle"), PSTR("heat!"), true, true, true, true);
            ScreenHandler.SetupConfirmAction(nullptr);
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
          }
          else {
            sprintf(buf,"T1\nG91\nG1 E-%d F%d\nG90",(int)distanceFilament,FilamentSpeed * 60);
            gcode.process_subcommands_now_P(buf);
          }
        #endif

        #if ENABLED(SINGLENOZZLE)
          if(thermalManager.temp_hotend[0].celsius < thermalManager.extrude_min_temp) {
            if ((thermalManager.temp_hotend[0].target == 0) || 
                (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)) {
              thermalManager.setTargetHotend(thermalManager.extrude_min_temp, 0);
            }
            ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait nozzle"), PSTR("heat!"), true, true, true, true);
            ScreenHandler.SetupConfirmAction(nullptr);
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
          }
          else {
            sprintf(buf,"T1\nG91\nG1 E-%d F%d\nG90",(int)distanceFilament,FilamentSpeed * 60);
            gcode.process_subcommands_now_P(buf);
          }
        #endif
      break;
    }
  }
#endif


#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

  typedef struct  {
    ExtUI::extruder_t extruder; // which extruder to operate
    uint8_t action; // load or unload
    bool heated; // heating done ?
    float purge_length; // the length to extrude before unload, prevent filament jam
  } filament_data_t;

  static filament_data_t filament_data;

  void DGUSScreenHandler::HandleFilamentOption(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleFilamentOption");

    uint8_t e_temp = 0;
    filament_data.heated = false;
    uint16_t preheat_option = swap16(*(uint16_t*)val_ptr);
    if (preheat_option <= 8)          // Load filament type
      filament_data.action = 1;
    else if (preheat_option >= 10) {  // Unload filament type
      preheat_option -= 10;
      filament_data.action = 2;
      filament_data.purge_length = DGUS_FILAMENT_PURGE_LENGTH;
    }
    else                              // Cancel filament operation
      filament_data.action = 0;

    switch (preheat_option) {
      case 0: // Load PLA
        #ifdef PREHEAT_1_TEMP_HOTEND
          e_temp = PREHEAT_1_TEMP_HOTEND;
        #endif
        break;
      case 1: // Load ABS
        TERN_(PREHEAT_2_TEMP_HOTEND, e_temp = PREHEAT_2_TEMP_HOTEND);
        break;
      case 2: // Load PET
        #ifdef PREHEAT_3_TEMP_HOTEND
          e_temp = PREHEAT_3_TEMP_HOTEND;
        #endif
        break;
      case 3: // Load FLEX
        #ifdef PREHEAT_4_TEMP_HOTEND
          e_temp = PREHEAT_4_TEMP_HOTEND;
        #endif
        break;
      case 9: // Cool down
      default:
        e_temp = 0;
        break;
    }

    if (filament_data.action == 0) { // Go back to utility screen
      #if HOTENDS >= 1
        thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E0);
      #endif
      #if HOTENDS >= 2
        thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E1);
      #endif
      GotoScreen(DGUSLCD_SCREEN_UTILITY);
    }
    else { // Go to the preheat screen to show the heating progress
      switch (var.VP) {
        default: return;
        #if HOTENDS >= 1
          case VP_E0_FILAMENT_LOAD_UNLOAD:
            filament_data.extruder = ExtUI::extruder_t::E0;
            thermalManager.setTargetHotend(e_temp, filament_data.extruder);
            break;
        #endif
        #if HOTENDS >= 2
          case VP_E1_FILAMENT_LOAD_UNLOAD:
            filament_data.extruder = ExtUI::extruder_t::E1;
            thermalManager.setTargetHotend(e_temp, filament_data.extruder);
          break;
        #endif
      }
      #if !ENABLED(DGUS_LCD_UI_MKS)
        GotoScreen(DGUSLCD_SCREEN_FILAMENT_HEATING);
      #endif
    }
  }

  void DGUSScreenHandler::HandleFilamentLoadUnload(DGUS_VP_Variable &var) {
    DEBUG_ECHOLNPGM("HandleFilamentLoadUnload");
    if (filament_data.action <= 0) return;

    // If we close to the target temperature, we can start load or unload the filament
    if (thermalManager.hotEnoughToExtrude(filament_data.extruder) && \
       thermalManager.targetHotEnoughToExtrude(filament_data.extruder)) {
      float movevalue = DGUS_FILAMENT_LOAD_LENGTH_PER_TIME;

      if (filament_data.action == 1) { // load filament
        if (!filament_data.heated) {
          #if !ENABLED(DGUS_LCD_UI_MKS)
            // GotoScreen(DGUSLCD_SCREEN_FILAMENT_LOADING);
          #endif
          filament_data.heated = true;
        }
        movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder)+movevalue;
      }
      else { // unload filament
        if (!filament_data.heated) {
          GotoScreen(DGUSLCD_SCREEN_FILAMENT_UNLOADING);
          filament_data.heated = true;
        }
        // Before unloading extrude to prevent jamming
        if (filament_data.purge_length >= 0) {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) + movevalue;
          filament_data.purge_length -= movevalue;
        }
        else
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) - movevalue;
      }
      ExtUI::setAxisPosition_mm(movevalue, filament_data.extruder);
    }
  }
#endif

void DGUSScreenHandler::UpdateNewScreen(DGUSLCD_Screens newscreen, bool popup) {
  DEBUG_ECHOLNPAIR("SetNewScreen: ", newscreen);

  if (!popup) {
    memmove(&past_screens[1], &past_screens[0], sizeof(past_screens) - 1);
    past_screens[0] = current_screen;
  }

  current_screen = newscreen;
  skipVP = 0;
  ForceCompleteUpdate();
}

void DGUSScreenHandler::PopToOldScreen() {
  DEBUG_ECHOLNPAIR("PopToOldScreen s=", past_screens[0]);
  GotoScreen(past_screens[0], true);
  memmove(&past_screens[0], &past_screens[1], sizeof(past_screens) - 1);
  past_screens[sizeof(past_screens) - 1] = DGUSLCD_SCREEN_MAIN;
}

void DGUSScreenHandler::UpdateScreenVPData() {
  DEBUG_ECHOPAIR(" UpdateScreenVPData Screen: ", current_screen);

  const uint16_t *VPList = DGUSLCD_FindScreenVPMapList(current_screen);
  if (!VPList) {
    DEBUG_ECHOLNPAIR(" NO SCREEN FOR: ", current_screen);
    ScreenComplete = true;
    return;  // nothing to do, likely a bug or boring screen.
  }

  // Round-robin updating of all VPs.
  VPList += update_ptr;

  bool sent_one = false;
  do {
    uint16_t VP = pgm_read_word(VPList);
    DEBUG_ECHOPAIR(" VP: ", VP);
    if (!VP) {
      update_ptr = 0;
      DEBUG_ECHOLNPGM(" UpdateScreenVPData done");
      ScreenComplete = true;
      return;  // Screen completed.
    }

    if (VP == skipVP) { skipVP = 0; continue; }

    DGUS_VP_Variable rcpy;
    if (populate_VPVar(VP, &rcpy)) {
      uint8_t expected_tx = 6 + rcpy.size;  // expected overhead is 6 bytes + payload.
      // Send the VP to the display, but try to avoid overrunning the Tx Buffer.
      // But send at least one VP, to avoid getting stalled.
      if (rcpy.send_to_display_handler && (!sent_one || expected_tx <= dgusdisplay.GetFreeTxBuffer())) {
        //DEBUG_ECHOPAIR(" calling handler for ", rcpy.VP);
        sent_one = true;
        rcpy.send_to_display_handler(rcpy);
      }
      else {
        //auto x=dgusdisplay.GetFreeTxBuffer();
        //DEBUG_ECHOLNPAIR(" tx almost full: ", x);
        //DEBUG_ECHOPAIR(" update_ptr ", update_ptr);
        ScreenComplete = false;
        return;  // please call again!
      }
    }

  } while (++update_ptr, ++VPList, true);
}

void DGUSScreenHandler::GotoScreen(DGUSLCD_Screens screen, bool ispopup) {
  dgusdisplay.RequestScreen(screen);
  UpdateNewScreen(screen, ispopup);
}

bool DGUSScreenHandler::loop() {
  dgusdisplay.loop();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  #if ENABLED(DGUS_LCD_UI_MKS)
    static uint8_t language_times = 2;
  #endif

  if (!IsScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    UpdateScreenVPData();
  }

  #if ENABLED(DGUS_LCD_UI_MKS)
    if (language_times != 0) {
      ScreenHandler.LanguagePInit();
      ScreenHandler.DGUS_LanguageDisplay(DGUSLanguageSwitch);
      language_times--;
    }
  #endif
  
  #if ENABLED(SHOW_BOOTSCREEN)
    static bool booted = false;

  #if !ENABLED(DGUS_LCD_UI_MKS)
    if (!booted && TERN0(POWER_LOSS_RECOVERY, recovery.valid())) 
      booted = true;
    #endif

    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;

      #if ENABLED(DGUS_LCD_UI_MKS)
        #if ANY_AXIS_HAS(STEALTHCHOP)
          tmc_x_step = stepperX.homing_threshold();
          tmc_y_step = stepperY.homing_threshold();
          tmc_z_step = stepperZ.homing_threshold();
        #endif

        if(min_ex_temp != 0) {
          thermalManager.extrude_min_temp = min_ex_temp;
        }

        DGUS_ExturdeLoadInit();

        #if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
          DGUS_RunoutInit();
        #endif

        if(TERN0(POWER_LOSS_RECOVERY, recovery.valid())) {
          ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POWER_LOSS);
        }else {
          GotoScreen(DGUSLCD_SCREEN_MAIN);
        }
      #endif
    }

    #if ENABLED(DGUS_LCD_UI_MKS)
      if(booted) {
        if(IS_SD_PRINTING() || IS_SD_PAUSED()) {
          #if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
            DGUS_Runout_Idle();
          #endif
        }
      }
    #endif
  #endif
  return IsScreenComplete();
}

void DGUSDisplay::RequestScreen(DGUSLCD_Screens screen) {
  DEBUG_ECHOLNPAIR("GotoScreen ", screen);
  const unsigned char gotoscreen[] = { 0x5A, 0x01, (unsigned char) (screen >> 8U), (unsigned char) (screen & 0xFFU) };
  WriteVariable(0x84, gotoscreen, sizeof(gotoscreen));
}


#if ENABLED(DGUS_LCD_UI_MKS)
void DGUSScreenHandler::LanguagePInit()
{
  switch (DGUSLanguageSwitch)
  {
    case MKS_SimpleChinese:
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1,MKS_Language_Choose);
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2,MKS_Language_NoChoose);
    break;
    case MKS_English:
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1,MKS_Language_NoChoose);
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2,MKS_Language_Choose);
    break;
    default:
      break;
  }
}

void DGUSScreenHandler::DGUS_ExturdeLoadInit(void) {

  ex_filament.ex_length = distanceFilament;
  ex_filament.ex_load_unload_flag = 0;
  ex_filament.ex_need_time = FilamentSpeed;
  ex_filament.ex_speed = 0;
  ex_filament.ex_status = EX_NONE;
  ex_filament.ex_tick_end = 0;
  ex_filament.ex_tick_start = 0;
}

void DGUSScreenHandler::DGUS_RunoutInit(void)  {

#if PIN_EXISTS(MT_DET_1)
  pinMode(MT_DET_1_PIN,INPUT_PULLUP);
#endif
  runout_mks.de_count = 0;
  runout_mks.de_times = 10;
  runout_mks.pin_status = 1;
  runout_mks.runout_status = UNRUNOUT_STATUS;
}

void DGUSScreenHandler::DGUS_Runout_Idle(void) {
#if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
  // scanf runout pin
  switch(runout_mks.runout_status) {

    case RUNOUT_STATUS:
      runout_mks.runout_status = RUNOUT_BEGIN_STATUS;
      queue.inject_P(PSTR("M25"));
      ScreenHandler.GotoScreen(MKSLCD_SCREEN_PAUSE);

      ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please change filament!"), nullptr, true, true, true, true);
      // ScreenHandler.SetupConfirmAction(nullptr);
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
    break;

    case UNRUNOUT_STATUS:
      if (READ(MT_DET_1_PIN) == LOW) { 
        runout_mks.runout_status = RUNOUT_STATUS;   
      }
    break;

    case RUNOUT_BEGIN_STATUS:
      if (READ(MT_DET_1_PIN) == HIGH) {
        runout_mks.runout_status = RUNOUT_WAITTING_STATUS;
      }
    break;

    case RUNOUT_WAITTING_STATUS:
      if (READ(MT_DET_1_PIN) == LOW) {
        runout_mks.runout_status = RUNOUT_BEGIN_STATUS;
      }
    break;

    default:
    break;
  }
#endif
}


void DGUSScreenHandler::DGUS_LanguageDisplay(uint8_t var)
{
  if (var == MKS_English)       
  {
    const char home_buf_en[] = "Home";
    dgusdisplay.WriteVariable(VP_HOME_Dis, home_buf_en, 32, true);

    const char setting_buf_en[] = "Setting";
    dgusdisplay.WriteVariable(VP_Setting_Dis, setting_buf_en, 32, true);

    const char Tool_buf_en[] = "Tool";
    dgusdisplay.WriteVariable(VP_Tool_Dis, Tool_buf_en, 32, true);

    const char Print_buf_en[] = "Print";
    dgusdisplay.WriteVariable(VP_Print_Dis, Print_buf_en, 32, true);

    const char Language_buf_en[] = "Language";
    dgusdisplay.WriteVariable(VP_Language_Dis, Language_buf_en, 32, true);

    const char About_buf_en[] = "About";
    dgusdisplay.WriteVariable(VP_About_Dis, About_buf_en, 32, true);

    const char Config_buf_en[] = "Config";
    dgusdisplay.WriteVariable(VP_Config_Dis, Config_buf_en, 32, true);

    const char MotorConfig_buf_en[] = "MotorConfig";
    dgusdisplay.WriteVariable(VP_MotorConfig_Dis, MotorConfig_buf_en, 32, true); 

    const char LevelConfig_buf_en[] = "LevelConfig";
    dgusdisplay.WriteVariable(VP_LevelConfig_Dis, LevelConfig_buf_en, 32, true);

    const char TemperatureConfig_buf_en[] = "Temperature";
    dgusdisplay.WriteVariable(VP_TemperatureConfig_Dis, TemperatureConfig_buf_en, 32, true);

    const char Advance_buf_en[] = "Advance";
    dgusdisplay.WriteVariable(VP_Advance_Dis, Advance_buf_en, 32, true);

    const char Filament_buf_en[] = "Exturde";
    dgusdisplay.WriteVariable(VP_Filament_Dis, Filament_buf_en,32, true);

    const char Move_buf_en[] = "Move";
    dgusdisplay.WriteVariable(VP_Move_Dis, Move_buf_en,32, true);

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    const char Level_buf_en[] = "AutoLevel";
    dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_en, 32, true);
    #elif ENABLED(MESH_BED_LEVELING)
    const char Level_buf_en[] = "MeshLevel";
    dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_en, 32, true);
    #else
    const char Level_buf_en[] = "Level";
    dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_en, 32, true);
    #endif

    const char MotorPluse_buf_en[] = "MotorPluse";
    dgusdisplay.WriteVariable(VP_MotorPluse_Dis, MotorPluse_buf_en, 32, true);

    const char MotorMaxSpeed_buf_en[] = "MotorMaxSpeed";
    dgusdisplay.WriteVariable(VP_MotorMaxSpeed_Dis, MotorMaxSpeed_buf_en, 32, true);

    const char MotorMaxAcc_buf_en[] = "MotorAcc";
    dgusdisplay.WriteVariable(VP_MotorMaxAcc_Dis, MotorMaxAcc_buf_en, 32, true);

    const char TravelAcc_buf_en[] = "TravelAcc";
    dgusdisplay.WriteVariable(VP_TravelAcc_Dis, TravelAcc_buf_en, 32, true);

    const char FeedRateMin_buf_en[] = "FeedRateMin";
    dgusdisplay.WriteVariable(VP_FeedRateMin_Dis, FeedRateMin_buf_en, 32, true);

    const char TravelFeeRateMin_buf_en[] = "TravelFeedRateMin";
    dgusdisplay.WriteVariable(VP_TravelFeeRateMin_Dis, TravelFeeRateMin_buf_en, 32, true);

    const char Acc_buf_en[] = "Acc";
    dgusdisplay.WriteVariable(VP_ACC_Dis, Acc_buf_en, 32, true);

    const char Point_One_buf_en[] = "Point_First";
    dgusdisplay.WriteVariable(VP_Point_One_Dis, Point_One_buf_en, 32, true);

    const char Point_Two_buf_en[] = "Point_Second";
    dgusdisplay.WriteVariable(VP_Point_Two_Dis, Point_Two_buf_en, 32, true);

    const char Point_Three_buf_en[] = "Point_Third";
    dgusdisplay.WriteVariable(VP_Point_Three_Dis, Point_Three_buf_en, 32, true);

    const char Point_Four_buf_en[] = "Point_Fourth";
    dgusdisplay.WriteVariable(VP_Point_Four_Dis, Point_Four_buf_en, 32, true);

    const char Point_Five_buf_en[] = "Point_Fifth";
    dgusdisplay.WriteVariable(VP_Point_Five_Dis, Point_Five_buf_en, 32, true);

    const char Extrusion_buf_en[] = "Extrusion";
    dgusdisplay.WriteVariable(VP_Extrusion_Dis, Extrusion_buf_en, 32, true);

    const char HeatBed_buf_en[] = "HeatBed";
    dgusdisplay.WriteVariable(VP_HeatBed_Dis, HeatBed_buf_en, 32, true);

    const char FactoryDefaults_buf_en[] = "FactoryDefaults";
    dgusdisplay.WriteVariable(VP_FactoryDefaults_Dis, FactoryDefaults_buf_en, 32, true);

    const char StoreSetting_buf_en[] = "StoreSetting";
    dgusdisplay.WriteVariable(VP_StoreSetting_Dis, StoreSetting_buf_en, 32, true);

    const char PrintPauseConfig_buf_en[] = "PrintPauseConfig";
    dgusdisplay.WriteVariable(VP_PrintPauseConfig_Dis, PrintPauseConfig_buf_en, 32, true);

    const char X_Pluse_buf_en[] = "X_Pluse";
    dgusdisplay.WriteVariable(VP_X_Pluse_Dis, X_Pluse_buf_en, 32, true);

    const char Y_Pluse_buf_en[] = "Y_Pluse";
    dgusdisplay.WriteVariable(VP_Y_Pluse_Dis, Y_Pluse_buf_en, 32, true);

    const char Z_Pluse_buf_en[] = "Z_Pluse";
    dgusdisplay.WriteVariable(VP_Z_Pluse_Dis, Z_Pluse_buf_en, 32, true);

    const char E0_Pluse_buf_en[] = "E0_Pluse";
    dgusdisplay.WriteVariable(VP_E0_Pluse_Dis, E0_Pluse_buf_en, 32, true);

    const char E1_Pluse_buf_en[] = "E1_Pluse";
    dgusdisplay.WriteVariable(VP_E1_Pluse_Dis, E1_Pluse_buf_en, 32, true);

    const char X_Max_Speed_buf_en[] = "X_Max_Speed";
    dgusdisplay.WriteVariable(VP_X_Max_Speed_Dis, X_Max_Speed_buf_en, 32, true);

    const char Y_Max_Speed_buf_en[] = "Y_Max_Speed";
    dgusdisplay.WriteVariable(VP_Y_Max_Speed_Dis, Y_Max_Speed_buf_en, 32, true);

    const char Z_Max_Speed_buf_en[] = "Z_Max_Speed";
    dgusdisplay.WriteVariable(VP_Z_Max_Speed_Dis, Z_Max_Speed_buf_en, 32, true);

    const char E0_Max_Speed_buf_en[] = "E0_Max_Speed";
    dgusdisplay.WriteVariable(VP_E0_Max_Speed_Dis, E0_Max_Speed_buf_en, 32, true);

    const char E1_Max_Speed_buf_en[] = "E1_Max_Speed";
    dgusdisplay.WriteVariable(VP_E1_Max_Speed_Dis, E1_Max_Speed_buf_en, 32, true);

    const char X_Max_Acc_Speed_buf_en[] = "X_Max_Acc_Speed";
    dgusdisplay.WriteVariable(VP_X_Max_Acc_Speed_Dis, X_Max_Acc_Speed_buf_en, 32, true);

    const char Y_Max_Acc_Speed_buf_en[] = "Y_Max_Acc_Speed";
    dgusdisplay.WriteVariable(VP_Y_Max_Acc_Speed_Dis, Y_Max_Acc_Speed_buf_en, 32, true);

    const char Z_Max_Acc_Speed_buf_en[] = "Z_Max_Acc_Speed";
    dgusdisplay.WriteVariable(VP_Z_Max_Acc_Speed_Dis, Z_Max_Acc_Speed_buf_en, 32, true);

    const char E0_Max_Acc_Speed_buf_en[] = "E0_Max_Acc_Speed";
    dgusdisplay.WriteVariable(VP_E0_Max_Acc_Speed_Dis, E0_Max_Acc_Speed_buf_en, 32, true);

    const char E1_Max_Acc_Speed_buf_en[] = "E1_Max_Acc_Speed";
    dgusdisplay.WriteVariable(VP_E1_Max_Acc_Speed_Dis, E1_Max_Acc_Speed_buf_en, 32, true);

    const char X_PARK_POS_buf_en[] = "X_PARK_POS";
    dgusdisplay.WriteVariable(VP_X_PARK_POS_Dis, X_PARK_POS_buf_en, 32, true);

    const char Y_PARK_POS_buf_en[] = "Y_PARK_POS";
    dgusdisplay.WriteVariable(VP_Y_PARK_POS_Dis, Y_PARK_POS_buf_en, 32, true);

    const char Z_PARK_POS_buf_en[] = "Z_PARK_POS";
    dgusdisplay.WriteVariable(VP_Z_PARK_POS_Dis, Z_PARK_POS_buf_en, 32, true);

    const char Length_buf_en[] = "Length";
    dgusdisplay.WriteVariable(VP_Length_Dis, Length_buf_en, 32, true);

    const char Speed_buf_en[] = "Speed";
    dgusdisplay.WriteVariable(VP_Speed_Dis, Speed_buf_en, 32, true);

    const char InOut_buf_en[] = "InOut";
    dgusdisplay.WriteVariable(VP_InOut_Dis, InOut_buf_en, 32, true);

    const char PrintTimet_buf_en[] = "PrintTime";
    dgusdisplay.WriteVariable(VP_PrintTime_Dis, PrintTimet_buf_en, 32, true);

    const char E0_Temp_buf_en[] = "E0_Temp";
    dgusdisplay.WriteVariable(VP_E0_Temp_Dis, E0_Temp_buf_en, 32, true);

    const char E1_Temp_buf_en[] = "E1_Temp";
    dgusdisplay.WriteVariable(VP_E1_Temp_Dis, E1_Temp_buf_en, 32, true);

    const char HB_Temp_buf_en[] = "HB_Temp";
    dgusdisplay.WriteVariable(VP_HB_Temp_Dis, HB_Temp_buf_en, 32, true);

    const char Feedrate_buf_en[] = "Feedrate";
    dgusdisplay.WriteVariable(VP_Feedrate_Dis, Feedrate_buf_en, 32, true);

    const char PrintAcc_buf_en[] = "PrintSpeed";
    dgusdisplay.WriteVariable(VP_PrintAcc_Dis, PrintAcc_buf_en, 32, true);

    const char FAN_Speed_buf_en[] = "FAN_Speed";
    dgusdisplay.WriteVariable(VP_FAN_Speed_Dis, FAN_Speed_buf_en, 32, true);

    const char Printing_buf_en[] = "Printing";
    dgusdisplay.WriteVariable(VP_Printing_Dis, Printing_buf_en, 32, true);

    const char Info_EEPROM_1_buf_en[] = "Store setting？";
    dgusdisplay.WriteVariable(VP_Info_EEPROM_1_Dis, Info_EEPROM_1_buf_en, 32, true);

    const char Info_EEPROM_2_buf_en[] = "Revert setting？";
    dgusdisplay.WriteVariable(VP_Info_EEPROM_2_Dis, Info_EEPROM_2_buf_en, 32, true);

    const char Info_PrinfFinsh_1_buf_en[] = "Print Done";
    dgusdisplay.WriteVariable(VP_Info_PrinfFinsh_1_Dis, Info_PrinfFinsh_1_buf_en, 32, true);

    const char TMC_X_Step_buf_en[] = "X_SenSitivity";
    dgusdisplay.WriteVariable(VP_TMC_X_Step_Dis, TMC_X_Step_buf_en, 32, true);

    const char TMC_Y_Step_buf_en[] = "Y_SenSitivity";
    dgusdisplay.WriteVariable(VP_TMC_Y_Step_Dis, TMC_Y_Step_buf_en, 32, true);

    const char TMC_Z_Step_buf_en[] = "Z_SenSitivity";
    dgusdisplay.WriteVariable(VP_TMC_Z_Step_Dis, TMC_Z_Step_buf_en, 32, true);

    const char TMC_X_Current_buf_en[] = "X_Current";
    dgusdisplay.WriteVariable(VP_TMC_X_Current_Dis, TMC_X_Current_buf_en, 32, true);

    const char TMC_Y_Current_buf_en[] = "Y_Current";
    dgusdisplay.WriteVariable(VP_TMC_Y_Current_Dis, TMC_Y_Current_buf_en, 32, true);

    const char TMC_Z_Current_buf_en[] = "Z_Current";
    dgusdisplay.WriteVariable(VP_TMC_Z_Current_Dis, TMC_Z_Current_buf_en, 32, true);

    const char TMC_E0_Current_buf_en[] = "E0_Current";
    dgusdisplay.WriteVariable(VP_TMC_E0_Current_Dis, TMC_E0_Current_buf_en, 32, true);

    const char TMC_X1_Current_buf_en[] = "X1_Current";
    dgusdisplay.WriteVariable(VP_TMC_X1_Current_Dis, TMC_X1_Current_buf_en, 32, true);

    const char TMC_Y1_Current_buf_en[] = "Y1_Current";
    dgusdisplay.WriteVariable(VP_TMC_Y1_Current_Dis, TMC_Y1_Current_buf_en, 32, true);

    const char TMC_Z1_Current_buf_en[] = "Z1_Current";
    dgusdisplay.WriteVariable(VP_TMC_Z1_Current_Dis, TMC_Z1_Current_buf_en, 32, true);

    const char TMC_E1_Current_buf_en[] = "E1_Current";
    dgusdisplay.WriteVariable(VP_TMC_E1_Current_Dis, TMC_E1_Current_buf_en, 32, true);

    const char Min_Ex_Temp_buf_en[] = "Min_Ex_Temp";
    dgusdisplay.WriteVariable(VP_Min_Ex_Temp_Dis, Min_Ex_Temp_buf_en, 32, true);

    const char AutoLEVEL_INFO1_buf_en[] = "Please Press Button!";
    dgusdisplay.WriteVariable(VP_AutoLEVEL_INFO1, AutoLEVEL_INFO1_buf_en, 32, true);

    const char EX_TEMP_INFO2_buf_en[] = "Please wait a monent";
    dgusdisplay.WriteVariable(VP_EX_TEMP_INFO2_Dis, EX_TEMP_INFO2_buf_en, 32, true);

    const char EX_TEMP_INFO3_buf_en[] = "Cancle";
    dgusdisplay.WriteVariable(VP_EX_TEMP_INFO3_Dis, EX_TEMP_INFO3_buf_en, 32, true);

    const char PrintConfrim_Info_buf_en[] = "Start Print?";
    dgusdisplay.WriteVariable(VP_PrintConfrim_Info_Dis, PrintConfrim_Info_buf_en, 32, true);

    const char StopPrintConfrim_Info_buf_en[] = "Stop Print?";
    dgusdisplay.WriteVariable(VP_StopPrintConfrim_Info_Dis, StopPrintConfrim_Info_buf_en, 32, true);

    const char Printting_buf_en[] = "Printing";
    dgusdisplay.WriteVariable(VP_Printting_Dis, Printting_buf_en, 32, true);

    

    

    
  }
  else if (var == MKS_SimpleChinese)
  {
    uint16_t home_buf_ch[] = {0xF7D6,0xB3D2}; 
    dgusdisplay.WriteVariable(VP_HOME_Dis, home_buf_ch, 4, true);

    const uint16_t Setting_Dis[] PROGMEM = {0xE8C9,0xC3D6,0x2000,0x2000,0x2000};
    dgusdisplay.WriteVariable(VP_Setting_Dis, Setting_Dis, 7, true);

    const uint16_t Tool_Dis[] PROGMEM = {0xA4B9,0xDFBE};  
    dgusdisplay.WriteVariable(VP_Tool_Dis, Tool_Dis, 4, true);
    
    const uint16_t Print_buf_ch[] PROGMEM = {0xF2B4,0xA1D3,0x2000};  
    dgusdisplay.WriteVariable(VP_Print_Dis, Print_buf_ch, 6, true);

    const uint16_t Language_buf_ch[] = {0xEFD3,0xD4D1,0x2000,0x2000};
    dgusdisplay.WriteVariable(VP_Language_Dis, Language_buf_ch, 8, true);

    const uint16_t About_buf_ch[] = {0xD8B9,0xDAD3,0x2000};
    dgusdisplay.WriteVariable(VP_About_Dis, About_buf_ch, 6, true);

    const uint16_t Config_buf_ch[] = {0xE4C5,0xC3D6,0x2000};
    dgusdisplay.WriteVariable(VP_Config_Dis, Config_buf_ch, 6, true);

    const uint16_t MotorConfig_buf_ch[] = {0xE7B5,0xFABB,0xE4C5,0xC3D6,0x2000};
    dgusdisplay.WriteVariable(VP_MotorConfig_Dis, MotorConfig_buf_ch, 12, true);

    const uint16_t LevelConfig_buf_ch[] = {0xD6CA,0XAFB6,0XF7B5,0XBDC6,0XE8C9,0XC3D6,0x2000};
    dgusdisplay.WriteVariable(VP_LevelConfig_Dis, LevelConfig_buf_ch, 32, true);

    const uint16_t TemperatureConfig_buf_ch[] = {0xC2CE,0XC8B6,0x2000};
    dgusdisplay.WriteVariable(VP_TemperatureConfig_Dis, TemperatureConfig_buf_ch, 11, true);

    const uint16_t Advance_buf_ch[] = {0xdfb8,0xB6BC,0XE8C9,0XC3D6,0x2000};
    dgusdisplay.WriteVariable(VP_Advance_Dis, Advance_buf_ch, 16, true);

    const uint16_t Filament_buf_ch[] = {0xB7BC,0xF6B3,0X2000};
    dgusdisplay.WriteVariable(VP_Filament_Dis, Filament_buf_ch, 8, true);

    const uint16_t Move_buf_ch[] = {0xC6D2,0xAFB6,0x2000 }; 
    dgusdisplay.WriteVariable(VP_Move_Dis, Move_buf_ch, 4, true);

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    const uint16_t Level_buf_ch[] = {0xD4D7,0xAFB6,0xf7b5,0xbdc6,0x2000};
    dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_ch, 32, true);
    #elif ENABLED(MESH_BED_LEVELING)
    const uint16_t Level_buf_ch[] = {0xF8CD,0XF1B8,0XF7B5,0XBDC6,0x2000};
    dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_ch, 32, true);
    #else
    const uint16_t Level_buf_ch[] = {0XD6CA,0XAFB6,0XF7B5,0XBDC6,0x2000};
    dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_ch, 32, true);
    #endif

    const uint16_t MotorPluse_buf_ch[] = {0XF6C2,0XE5B3,0X2000};
    dgusdisplay.WriteVariable(VP_MotorPluse_Dis, MotorPluse_buf_ch, 16, true);

    const uint16_t MotorMaxSpeed_buf_ch[] = {0XEED7,0XF3B4,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_MotorMaxSpeed_Dis, MotorMaxSpeed_buf_ch, 16, true);

    const uint16_t MotorMaxAcc_buf_ch[] ={0XEED7,0XF3B4,0XD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_MotorMaxAcc_Dis, MotorMaxAcc_buf_ch, 16, true);

    const uint16_t TravelAcc_buf_ch[] = {0XD5BF,0XD0D0,0XD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_TravelAcc_Dis, TravelAcc_buf_ch, 16, true);

    const uint16_t FeedRateMin_buf_ch[] = {0XEED7,0XA1D0,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_FeedRateMin_Dis, FeedRateMin_buf_ch, 12, true);

    const uint16_t TravelFeeRateMin_buf_ch[] = {0XD5BF,0XD0D0,0XEED7,0XA1D0,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_TravelFeeRateMin_Dis, TravelFeeRateMin_buf_ch, 24, true);

    const uint16_t Acc_buf_ch[] = {0XD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_ACC_Dis, Acc_buf_ch, 16, true);

    const uint16_t Point_One_buf_ch[] = {0XDAB5,0XBBD2,0XE3B5,0X2000};
    dgusdisplay.WriteVariable(VP_Point_One_Dis, Point_One_buf_ch, 12, true);

    const uint16_t Point_Two_buf_ch[] = {0XDAB5,0XFEB6,0XE3B5,0X2000};
    dgusdisplay.WriteVariable(VP_Point_Two_Dis, Point_Two_buf_ch, 12, true);

    const uint16_t Point_Three_buf_ch[] = {0XDAB5,0XFDC8,0XE3B5,0X2000};
    dgusdisplay.WriteVariable(VP_Point_Three_Dis, Point_Three_buf_ch, 12, true);

    const uint16_t Point_Four_buf_ch[] = {0XDAB5,0XC4CB,0XE3B5,0X2000};
    dgusdisplay.WriteVariable(VP_Point_Four_Dis, Point_Four_buf_ch, 12, true);

    const uint16_t Point_Five_buf_ch[] = {0XDAB5,0XE5CE,0XE3B5,0X2000};
    dgusdisplay.WriteVariable(VP_Point_Five_Dis, Point_Five_buf_ch, 12, true);

    const uint16_t Extrusion_buf_ch[] = {0xB7BC,0XF6B3,0XB7CD,0X2000};
    dgusdisplay.WriteVariable(VP_Extrusion_Dis, Extrusion_buf_ch, 12, true);

    const uint16_t HeatBed_buf_ch[] = {0XC8C8,0XB2B4,0X2000};
    dgusdisplay.WriteVariable(VP_HeatBed_Dis, HeatBed_buf_ch, 12, true);

    const uint16_t FactoryDefaults_buf_ch[] = {0xD6BB,0XB4B8,0XF6B3,0XA7B3,0XE8C9,0XC3D6,0X2000};
    dgusdisplay.WriteVariable(VP_FactoryDefaults_Dis, FactoryDefaults_buf_ch, 16, true);

    const uint16_t StoreSetting_buf_ch[] = {0XA3B1,0XE6B4,0XE8C9,0XC3D6,0X2000};
    dgusdisplay.WriteVariable(VP_StoreSetting_Dis, StoreSetting_buf_ch, 16, true);

    const uint16_t PrintPauseConfig_buf_ch[] = {0XDDD4,0XA3CD,0XBBCE,0XC3D6,0X2000};
    dgusdisplay.WriteVariable(VP_PrintPauseConfig_Dis, PrintPauseConfig_buf_ch, 32, true);

    const uint16_t X_Pluse_buf_ch[] = {0x2058,0xE1D6,0XF6C2,0XE5B3,0x2000};
    dgusdisplay.WriteVariable(VP_X_Pluse_Dis, X_Pluse_buf_ch, 16, true);

    const uint16_t Y_Pluse_buf_ch[] = {0x2059,0xE1D6,0XF6C2,0XE5B3,0x2000};
    dgusdisplay.WriteVariable(VP_Y_Pluse_Dis, Y_Pluse_buf_ch, 16, true);

    const uint16_t Z_Pluse_buf_ch[] = {0x205A,0xE1D6,0XF6C2,0XE5B3,0x2000};
    dgusdisplay.WriteVariable(VP_Z_Pluse_Dis, Z_Pluse_buf_ch, 16, true);

    const uint16_t E0_Pluse_buf_ch[] = {0x3045,0xE1D6,0XF6C2,0XE5B3,0x2000};
    dgusdisplay.WriteVariable(VP_E0_Pluse_Dis, E0_Pluse_buf_ch, 16, true);

    const uint16_t E1_Pluse_buf_ch[] = {0x3145,0xE1D6,0XF6C2,0XE5B3,0x2000};
    dgusdisplay.WriteVariable(VP_E1_Pluse_Dis, E1_Pluse_buf_ch, 16, true);

    const uint16_t X_Max_Speed_buf_ch[] = {0x2058,0xEED7,0XF3B4,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_X_Max_Speed_Dis, X_Max_Speed_buf_ch, 16, true);

    const uint16_t Y_Max_Speed_buf_ch[] = {0x2059,0xEED7,0XF3B4,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Y_Max_Speed_Dis, Y_Max_Speed_buf_ch, 16, true);

    const uint16_t Z_Max_Speed_buf_ch[] = {0x205A,0xEED7,0XF3B4,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Z_Max_Speed_Dis, Z_Max_Speed_buf_ch, 16, true);

    const uint16_t E0_Max_Speed_buf_ch[] = {0x3045,0xEED7,0XF3B4,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_E0_Max_Speed_Dis, E0_Max_Speed_buf_ch, 16, true);

    const uint16_t E1_Max_Speed_buf_ch[] = {0x3145,0xEED7,0XF3B4,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_E1_Max_Speed_Dis, E1_Max_Speed_buf_ch, 16, true);

    const uint16_t X_Max_Acc_Speed_buf_ch[] = {0x2058,0xEED7,0xF3B4,0xD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_X_Max_Acc_Speed_Dis, X_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t Y_Max_Acc_Speed_buf_ch[] = {0x2059,0xEED7,0xF3B4,0xD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Y_Max_Acc_Speed_Dis, Y_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t Z_Max_Acc_Speed_buf_ch[] = {0x205A,0xEED7,0xF3B4,0xD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Z_Max_Acc_Speed_Dis, Z_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t E0_Max_Acc_Speed_buf_ch[] ={0x3045,0xEED7,0xF3B4,0xD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_E0_Max_Acc_Speed_Dis, E0_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t E1_Max_Acc_Speed_buf_ch[] = {0x3145,0xEED7,0xF3B4,0xD3BC,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_E1_Max_Acc_Speed_Dis, E1_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t X_PARK_POS_buf_ch[] = {0X2058,0XDDD4,0XA3CD,0XBBCE,0XC3D6,0X2000};
    dgusdisplay.WriteVariable(VP_X_PARK_POS_Dis, X_PARK_POS_buf_ch, 16, true);

    const uint16_t Y_PARK_POS_buf_ch[] = {0X2059,0XDDD4,0XA3CD,0XBBCE,0XC3D6,0X2000};
    dgusdisplay.WriteVariable(VP_Y_PARK_POS_Dis, Y_PARK_POS_buf_ch, 16, true);

    const uint16_t Z_PARK_POS_buf_ch[] = {0X205A,0XDDD4,0XA3CD,0XBBCE,0XC3D6,0X2000};
    dgusdisplay.WriteVariable(VP_Z_PARK_POS_Dis, Z_PARK_POS_buf_ch, 16, true);

    const uint16_t Length_buf_ch[] = {0XBDB2,0XA4B3,0X2000};
    dgusdisplay.WriteVariable(VP_Length_Dis, Length_buf_ch, 8, true);

    const uint16_t Speed_buf_ch[] = {0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Speed_Dis, Speed_buf_ch, 8, true);

    const uint16_t InOut_buf_ch[] = {0XF8BD,0XF6B3,0X2000};
    dgusdisplay.WriteVariable(VP_InOut_Dis, InOut_buf_ch, 8, true);

    const uint16_t PrintTimet_buf_en[] = {0XF2B4,0XA1D3,0XB1CA,0XE4BC,0X2000};
    dgusdisplay.WriteVariable(VP_PrintTime_Dis, PrintTimet_buf_en, 16, true);

    const uint16_t E0_Temp_buf_ch[] = {0x3045,0XC2CE,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_E0_Temp_Dis, E0_Temp_buf_ch, 16, true);

    const uint16_t E1_Temp_buf_ch[] = {0x3145,0XC2CE,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_E1_Temp_Dis, E1_Temp_buf_ch, 16, true);

    const uint16_t HB_Temp_buf_ch[] = {0XC8C8,0XB2B4,0XC2CE,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_HB_Temp_Dis, HB_Temp_buf_ch, 16, true);

    const uint16_t Feedrate_buf_ch[] = {0XB7BC,0XF6B3,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Feedrate_Dis, Feedrate_buf_ch, 16, true);

    const uint16_t PrintAcc_buf_ch[] = {0xF2B4,0XA1D3,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_PrintAcc_Dis, PrintAcc_buf_ch, 16, true);

    const uint16_t FAN_Speed_buf_ch[] = {0XE7B7,0XC8C9,0XD9CB,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_FAN_Speed_Dis, FAN_Speed_buf_ch, 16, true);

    const uint16_t Printing_buf_ch[] = {0XF2B4,0XA1D3,0XD0D6,0X2000};
    dgusdisplay.WriteVariable(VP_Printing_Dis, Printing_buf_ch, 16, true);

    const uint16_t Info_EEPROM_1_buf_ch[] = {0XC7CA,0XF1B7,0XA3B1,0XE6B4,0XE8C9,0XC3D6,0XBFA3,0X2000};
    dgusdisplay.WriteVariable(VP_Info_EEPROM_1_Dis, Info_EEPROM_1_buf_ch, 32, true);

    const uint16_t Info_EEPROM_2_buf_ch[] = {0xC7CA,0XF1B7,0XD6BB,0XB4B8,0XF6B3,0XA7B3,0XE8C9,0XC3D6,0x2000};
    dgusdisplay.WriteVariable(VP_Info_EEPROM_2_Dis, Info_EEPROM_2_buf_ch, 32, true);
    
    const uint16_t TMC_X_Step_buf_ch[] = {0X2058,0XE9C1,0XF4C3,0XC8B6,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_X_Step_Dis, TMC_X_Step_buf_ch, 16, true);

    const uint16_t TMC_Y_Step_buf_ch[] = {0X2059,0XE9C1,0XF4C3,0XC8B6,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_Y_Step_Dis, TMC_Y_Step_buf_ch, 16, true);

    const uint16_t TMC_Z_Step_buf_ch[] = {0X205A,0XE9C1,0XF4C3,0XC8B6,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_Z_Step_Dis, TMC_Z_Step_buf_ch, 16, true);

    const uint16_t Info_PrinfFinsh_1_buf_ch[] = {0XF2B4,0XA1D3,0XEACD,0XC9B3,0X2000};
    dgusdisplay.WriteVariable(VP_Info_PrinfFinsh_1_Dis, Info_PrinfFinsh_1_buf_ch, 32, true);

    const uint16_t TMC_X_Current_buf_ch[] = {0X2058,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_X_Current_Dis, TMC_X_Current_buf_ch, 16, true);

    const uint16_t TMC_Y_Current_buf_ch[] = {0X2059,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_Y_Current_Dis, TMC_Y_Current_buf_ch, 16, true);

    const uint16_t TMC_Z_Current_buf_ch[] = {0X205A,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_Z_Current_Dis, TMC_Z_Current_buf_ch, 16, true);

    const uint16_t TMC_E0_Current_buf_ch[] = {0X3045,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_E0_Current_Dis, TMC_E0_Current_buf_ch, 16, true);

    const uint16_t TMC_X1_Current_buf_ch[] = {0X3158,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_X1_Current_Dis, TMC_X1_Current_buf_ch, 16, true);

    const uint16_t TMC_Y1_Current_buf_ch[] = {0X3159,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_Y1_Current_Dis, TMC_Y1_Current_buf_ch, 16, true);

    const uint16_t TMC_Z1_Current_buf_ch[] = {0X315A,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_Z1_Current_Dis, TMC_Z1_Current_buf_ch, 16, true);

    const uint16_t TMC_E1_Current_buf_ch[] = {0X3145,0XE1D6,0XE7B5,0XF7C1,0x2000};
    dgusdisplay.WriteVariable(VP_TMC_E1_Current_Dis, TMC_E1_Current_buf_ch, 16, true);

    const uint16_t Min_Ex_Temp_buf_ch[] = {0xEED7,0XA1D0,0XB7BC,0XF6B3,0XC2CE,0XC8B6,0X2000};
    dgusdisplay.WriteVariable(VP_Min_Ex_Temp_Dis, Min_Ex_Temp_buf_ch, 32, true);

    const uint16_t AutoLEVEL_INFO1_buf_ch[] = {0xEBC7,0XB4B0,0XC2CF,0XB4B0,0XA5C5,0X2000};
    dgusdisplay.WriteVariable(VP_AutoLEVEL_INFO1, AutoLEVEL_INFO1_buf_ch, 32, true);

    const uint16_t EX_TEMP_INFO2_buf_ch[] = {0xEBC7,0XD4C9,0XC8B5,0X2000};
    dgusdisplay.WriteVariable(VP_EX_TEMP_INFO2_Dis, EX_TEMP_INFO2_buf_ch, 32, true);

    const uint16_t EX_TEMP_INFO3_buf_ch[] = {0xA1C8,0XFBCF,0XD3BC,0XC8C8,0X2000};
    dgusdisplay.WriteVariable(VP_EX_TEMP_INFO3_Dis, EX_TEMP_INFO3_buf_ch, 32, true);

    const uint16_t PrintConfrim_Info_buf_ch[] = {0xC7CA,0XF1B7,0XAABF,0XBCCA,0XF2B4,0XA1D3,0X2000};
    dgusdisplay.WriteVariable(VP_PrintConfrim_Info_Dis, PrintConfrim_Info_buf_ch, 32, true);

    const uint16_t StopPrintConfrim_Info_buf_ch[] = {0xC7CA,0XF1B7,0XA3CD,0XB9D6,0XF2B4,0XA1D3,0X2000};
    dgusdisplay.WriteVariable(VP_StopPrintConfrim_Info_Dis, StopPrintConfrim_Info_buf_ch, 32, true);

    const uint16_t Printting_buf_ch[] = {0xF2B4,0XA1D3,0XD0D6,0X2000};
    dgusdisplay.WriteVariable(VP_Printting_Dis, Printting_buf_ch, 32, true);
  }
}
#endif 

#endif // HAS_DGUS_LCD
