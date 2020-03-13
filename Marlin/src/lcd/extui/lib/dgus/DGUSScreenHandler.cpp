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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_DGUS_LCD

#if HOTENDS > 2
  #error "More than 2 hotends not implemented on the Display UI design."
#endif

#include "DGUSScreenHandler.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/power_loss_recovery.h"
#endif

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
  typedef struct {
    ExtUI::extruder_t extruder;     // which extruder to operate
    uint8_t           action;       // load or unload
    bool              heated;       // heating done ?
    float             purge_length; // the length to extrude before unload, prevent filament jam
  } filament_data_t;

  static filament_data_t filament_data;
#endif

uint16_t DGUSScreenHandler::confirm_vp;

#if ENABLED(SDSUPPORT)
  int16_t DGUSScreenHandler::top_file = 0;
  int16_t DGUSScreenHandler::file_to_print = 0;
  static ExtUI::FileList filelist;
#endif

void (*DGUSScreenHandler::confirm_action_cb)() = nullptr;

DGUS_Screen DGUSScreenHandler::current_screen;
DGUS_Screen DGUSScreenHandler::past_screens[NUM_PAST_SCREENS];
uint8_t DGUSScreenHandler::update_index;
uint16_t DGUSScreenHandler::skip_vp;
bool DGUSScreenHandler::screen_complete;

// endianness swap
uint16_t swap16(const uint16_t value) {
  return (value & 0xffU) << 8U | (value >> 8U);
}

void DGUSScreenHandler::Init() {
  dgus_display.Init();

  if (true
    #if ENABLED(POWER_LOSS_RECOVERY)
      && !recovery.valid()
    #endif
  ) {
    dgus_display.SwitchScreen(
      #if ENABLED(SHOW_BOOTSCREEN)
        DGUS_SCREEN_BOOT
      #else
        DGUS_SCREEN_MAIN
      #endif
    );
  }

  UpdateScreenVPData();
}

bool DGUSScreenHandler::Loop() {
  dgus_display.Loop();

  const millis_t ms = ExtUI::safe_millis();
  static millis_t next_event_ms = 0;

  if (!IsScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    UpdateScreenVPData();
  }

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool booted = false;
    #if ENABLED(POWER_LOSS_RECOVERY)
      if (!booted && recovery.valid()) booted = true;
    #endif
    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;
      GotoScreen(DGUS_SCREEN_MAIN);
    }
  #endif
  return IsScreenComplete();
}

void DGUSScreenHandler::PrinterKilled(PGM_P error, PGM_P component) {
  SendInfoScreen(GET_TEXT(MSG_HALTED), true, error, true, NUL_STR, true, GET_TEXT(MSG_PLEASE_RESET), true);
  GotoScreen(DGUS_SCREEN_KILL);
  while (!Loop());  // Wait while anything is left to be sent
}

void DGUSScreenHandler::UserConfirmRequired(const char * const msg) {
  if (msg) {
    SendInfoScreen(PSTR("Please confirm."), true, NUL_STR, true, msg, false, NUL_STR, true);
    SetupConfirmAction(ExtUI::setUserConfirmed);
    GotoScreen(DGUS_SCREEN_POPUP);
  }
  else if (GetCurrentScreen() == DGUS_SCREEN_POPUP) {
    SetupConfirmAction(nullptr);
    PopToOldScreen();
  }
}

#if ENABLED(SDSUPPORT)

  void DGUSScreenHandler::SDCardInserted() {
    top_file = 0;
    auto cs = GetCurrentScreen();
    if (cs == DGUS_SCREEN_MAIN || cs == DGUS_SCREEN_STATUS)
      GotoScreen(DGUS_SCREEN_SDFILELIST);
  }

  void DGUSScreenHandler::SDCardRemoved() {
    if (current_screen == DGUS_SCREEN_SDFILELIST
        || (current_screen == DGUS_SCREEN_CONFIRM && (confirm_vp == VP_SD_AbortPrintConfirmed || confirm_vp == VP_SD_FileSelectConfirm))
        || current_screen == DGUS_SCREEN_SDPRINTMANIPULATION
    ) GotoScreen(DGUS_SCREEN_MAIN);
  }

  void DGUSScreenHandler::SDCardError() {
    SDCardRemoved();
    SendInfoScreen(PSTR("NOTICE"), true, NUL_STR, true, PSTR("SD card error"), true, NUL_STR, true);
    SetupConfirmAction(nullptr);
    GotoScreen(DGUS_SCREEN_POPUP);
  }

#endif // SDSUPPORT

#if ENABLED(POWER_LOSS_RECOVERY)

  void DGUSScreenHandler::PowerLossResume() {
    GotoScreen(DGUS_SCREEN_POWER_LOSS);
  }

#endif // POWER_LOSS_RECOVERY

#if HAS_PID_HEATING

  void DGUSScreenHandler::PidTuning(const ExtUI::result_t rst) {
    SERIAL_ECHOLNPAIR("OnPidTuning:", rst);
    switch(rst) {
      case ExtUI::PID_BAD_EXTRUDER_NUM:
        SetStatusMessagePGM(PSTR(STR_PID_BAD_EXTRUDER_NUM));
        break;
      case ExtUI::PID_TEMP_TOO_HIGH:
        SetStatusMessagePGM(PSTR(STR_PID_TEMP_TOO_HIGH));
        break;
      case ExtUI::PID_TUNING_TIMEOUT:
        SetStatusMessagePGM(PSTR(STR_PID_TIMEOUT));
        break;
      case ExtUI::PID_DONE:
        SetStatusMessagePGM(PSTR(STR_PID_AUTOTUNE_FINISHED));
        break;
    }
    GotoScreen(DGUS_SCREEN_MAIN);
  }

#endif // HAS_PID_HEATING

void DGUSScreenHandler::SendInfoScreen(const char* l1, bool l1inflash, const char* l2, bool l2inflash, const char* l3, bool l3inflash, const char* l4, bool l4inflash) {
  DGUS_VP_Variable ramcopy;
  if (DGUS_PopulateVPVar(VP_MSGSTR1, &ramcopy)) {
    ramcopy.memadr = (void*)l1;
    l1inflash ? SendStringToDisplayPGM(ramcopy) : SendStringToDisplay(ramcopy);
  }
  if (DGUS_PopulateVPVar(VP_MSGSTR2, &ramcopy)) {
    ramcopy.memadr = (void*)l2;
    l2inflash ? SendStringToDisplayPGM(ramcopy) : SendStringToDisplay(ramcopy);
  }
  if (DGUS_PopulateVPVar(VP_MSGSTR3, &ramcopy)) {
    ramcopy.memadr = (void*)l3;
    l3inflash ? SendStringToDisplayPGM(ramcopy) : SendStringToDisplay(ramcopy);
  }
  if (DGUS_PopulateVPVar(VP_MSGSTR4, &ramcopy)) {
    ramcopy.memadr = (void*)l4;
    l4inflash ? SendStringToDisplayPGM(ramcopy) : SendStringToDisplay(ramcopy);
  }
}

void DGUSScreenHandler::HandleUserConfirmationPopUp(uint16_t VP, const char* l1, bool l1inflash, const char* l2, bool l2inflash, const char* l3, bool l3inflash, const char* l4, bool l4inflash) {
  if (current_screen == DGUS_SCREEN_CONFIRM) {
    // Already showing a pop up, so we need to cancel that first.
    PopToOldScreen();
  }

  confirm_vp = VP;
  SendInfoScreen(l1, l1inflash, l2, l2inflash, l3, l3inflash, l4, l4inflash);
  GotoScreen(DGUS_SCREEN_CONFIRM);
}

void DGUSScreenHandler::SetStatusMessage(const char *msg) {
  DGUS_VP_Variable ramcopy;
  if (DGUS_PopulateVPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) msg;
    SendStringToDisplay(ramcopy);
  }
}

void DGUSScreenHandler::SetStatusMessagePGM(PGM_P const msg) {
  DGUS_VP_Variable ramcopy;
  if (DGUS_PopulateVPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) msg;
    SendStringToDisplayPGM(ramcopy);
  }
}

// Send an 8 bit or 16 bit value to the display.
void DGUSScreenHandler::SendWordValueToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint8_t *tmp = (uint8_t *) var.memadr;
    uint16_t data_to_send = (tmp[0] << 8);
    if (var.size >= 2) data_to_send |= tmp[1];
    dgus_display.WriteVariable(var.VP, data_to_send);
  }
}

// Send an uint8_t between 0 and 255 to the display, but scale to a percentage (0..100)
void DGUSScreenHandler::SendPercentageToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint16_t tmp = *(uint8_t *) var.memadr + 1; // +1 -> avoid rounding issues for the display.
    tmp = map(tmp, 0, 255, 0, 100);
    dgus_display.WriteVariable(var.VP, swap16(tmp));
  }
}

// Send the current print progress to the display.
void DGUSScreenHandler::SendPrintProgressToDisplay(DGUS_VP_Variable &var) {
  //DEBUG_ECHOPAIR(" SendPrintProgressToDisplay ", var.VP);
  uint16_t tmp = ExtUI::getProgress_percent();
  //DEBUG_ECHOLNPAIR(" data ", tmp);
  dgus_display.WriteVariable(var.VP, swap16(tmp));
}

// Send the current print time to the display.
// It is using a hex display for that: It expects BSD coded data in the format xxyyzz
void DGUSScreenHandler::SendPrintTimeToDisplay(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();
  char buf[32];
  elapsed.toString(buf);
  dgus_display.WriteVariable(VP_PrintTime, buf, var.size, true);
}

// Send an uint8_t between 0 and 100 to a variable scale to 0..255
void DGUSScreenHandler::PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    *(uint8_t*)var.memadr = map(constrain(value, 0, 100), 0, 100, 0, 255);
  }
}

// Sends a (RAM located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.
// var.size has the display buffer size!
void DGUSScreenHandler::SendStringToDisplay(DGUS_VP_Variable &var) {
  dgus_display.WriteVariable(var.VP, (char*)var.memadr, var.size, true);
}

// Sends a (flash located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.
// var.size has the display buffer size!
void DGUSScreenHandler::SendStringToDisplayPGM(DGUS_VP_Variable &var) {
  dgus_display.WriteVariablePGM(var.VP, (char*)var.memadr, var.size, true);
}

#if HAS_PID_HEATING

  void DGUSScreenHandler::SendTemperaturePID(DGUS_VP_Variable &var) {
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
    dgus_display.WriteVariable(var.VP, tmp, 2);
  }

#endif // HAS_PID_HEATING

#if ENABLED(PRINTCOUNTER)

  // Send the accumulate print time to the display.
  // It is using a hex display for that: It expects BSD coded data in the format xxyyzz
  void DGUSScreenHandler::SendPrintAccTimeToDisplay(DGUS_VP_Variable &var) {
    printStatistics state = print_job_timer.getStats();
    char buf[21];
    duration_t elapsed = state.printTime;
    elapsed.toString(buf);
    dgus_display.WriteVariable(VP_PrintAccTime, buf, var.size, true);
  }

  void DGUSScreenHandler::SendPrintsTotalToDisplay(DGUS_VP_Variable &var) {
    printStatistics state = print_job_timer.getStats();
    char buf[21];
    sprintf_P(buf, PSTR("%u"), state.totalPrints);
    dgus_display.WriteVariable(VP_PrintsTotal, buf, var.size, true);
  }

#endif // PRINTCOUNTER

// Send fan status value to the display.
#if FAN_COUNT > 0

  void DGUSScreenHandler::SendFanStatusToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      DEBUG_ECHOPAIR(" SendFanStatusToDisplay ", var.VP);
      DEBUG_ECHOLNPAIR(" data ", *(uint8_t *)var.memadr);
      uint16_t data_to_send = 0;
      if (*(uint8_t *) var.memadr) data_to_send = 1;
      dgus_display.WriteVariable(var.VP, swap16(data_to_send));
    }
  }

#endif // FAN_COUNT > 0

// Send heater status value to the display.
void DGUSScreenHandler::SendHeaterStatusToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    DEBUG_ECHOPAIR(" SendHeaterStatusToDisplay ", var.VP);
    DEBUG_ECHOLNPAIR(" data ", *(int16_t *)var.memadr);
    uint16_t data_to_send = 0;
    if (*(int16_t *) var.memadr) data_to_send = 1;
    dgus_display.WriteVariable(var.VP, swap16(data_to_send));
  }
}

#if ENABLED(DGUS_UI_WAITING)

  void DGUSScreenHandler::SendWaitingStatusToDisplay(DGUS_VP_Variable &var) {
    // In FYSETC UI design there are 10 statuses to loop
    static uint16_t period = 0;
    static uint16_t index = 0;
    //DEBUG_ECHOPAIR(" SendWaitingStatusToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", swap16(index));
    if (period++ > DGUS_UI_WAITING_STATUS_PERIOD) {
      dgus_display.WriteVariable(var.VP, swap16(index));
      //DEBUG_ECHOLNPAIR(" data ", swap16(index));
      if (++index >= DGUS_UI_WAITING_STATUS) index = 0;
      period = 0;
    }
  }

#endif // DGUS_UI_WAITING

#if ENABLED(SDSUPPORT)

  void DGUSScreenHandler::ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr) {
    // default action executed when there is a SD card, but not printing
    if (ExtUI::isMediaInserted() && !ExtUI::isPrintingFromMedia()) {
      ScreenChangeHook(var, val_ptr);
      dgus_display.SwitchScreen(current_screen);
      return;
    }

    // if we are printing, we jump to two screens after the requested one.
    // This should host e.g a print pause / print abort / print resume dialog.
    // This concept allows to recycle this hook for other file
    if (ExtUI::isPrintingFromMedia() && !card.flag.abort_sd_printing) {
      GotoScreen(DGUS_SCREEN_SDPRINTMANIPULATION);
      return;
    }

    // Don't let the user in the dark why there is no reaction.
    if (!ExtUI::isMediaInserted()) {
      SetStatusMessagePGM(GET_TEXT(MSG_NO_MEDIA));
      return;
    }

    if (card.flag.abort_sd_printing) {
      SetStatusMessagePGM(GET_TEXT(MSG_MEDIA_ABORTING));
      return;
    }
  }

  void DGUSScreenHandler::SD_ScrollFilelist(DGUS_VP_Variable& var, void *val_ptr) {
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
      filelist.upDir();
      top_file = 0;
      ForceCompleteUpdate();
    }

    if (old_top != top_file) ForceCompleteUpdate();
  }

  void DGUSScreenHandler::SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)swap16(*(uint16_t*)val_ptr) + top_file;
    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;
    if (filelist.isDir()) {
      filelist.changeDir(filelist.filename());
      top_file = 0;
      ForceCompleteUpdate();
      return;
    }

    #if ENABLED(DGUS_PRINT_FILENAME)
      // Send print filename
      dgus_display.WriteVariable(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN, true);
    #endif

    // Setup Confirmation screen
    file_to_print = touched_nr;
    HandleUserConfirmationPopUp(VP_SD_FileSelectConfirm, NUL_STR, true, PSTR("Print file"), true, filelist.filename(), false, PSTR("from SD Card?"), true);
  }

  void DGUSScreenHandler::SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    GotoScreen(
      #if ENABLED(DGUS_LCD_UI_ORIGIN)
        DGUS_SCREEN_STATUS
      #else
        DGUS_SCREEN_SDPRINTMANIPULATION
      #endif
    );
  }

  void DGUSScreenHandler::SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {
    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    switch (swap16(*(uint16_t*)val_ptr)) {
      case 0:  // Resume
        if (ExtUI::isPrintingFromMediaPaused()) ExtUI::resumePrint();
        break;

      case 1:  // Pause
        if (!ExtUI::isPrintingFromMediaPaused()) ExtUI::pausePrint();
        break;

      case 2:  // Abort
        HandleUserConfirmationPopUp(VP_SD_AbortPrintConfirmed, NUL_STR, true, PSTR("Abort printing"), true, filelist.filename(), false, PSTR("?"), true);
        break;
    }
  }

  void DGUSScreenHandler::SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr) {
    ExtUI::stopPrint();
    GotoScreen(DGUS_SCREEN_MAIN);
  }

  void DGUSScreenHandler::SD_PrintTune(DGUS_VP_Variable &var, void *val_ptr) {
    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    GotoScreen(DGUS_SCREEN_SDPRINTTUNE);
  }

  void DGUSScreenHandler::SD_SendFilename(DGUS_VP_Variable& var) {
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line > DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;
    if (filelist.seek(top_file + target_line))
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0);
    SendStringToDisplay(var);
  }

#endif // SDSUPPORT

void DGUSScreenHandler::ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr) {
  DGUS_VP_Variable ramcopy;
  if (!DGUS_PopulateVPVar(confirm_vp, &ramcopy)) return;
  if (ramcopy.set_by_display_handler) ramcopy.set_by_display_handler(ramcopy, val_ptr);
}

void DGUSScreenHandler::ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr) {
  if (!ExtUI::isPrinting()) {
    ScreenChangeHook(var, val_ptr);
    dgus_display.SwitchScreen(current_screen);
  }
}

void DGUSScreenHandler::ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUS_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUS_Screen target = (DGUS_Screen)tmp[1];

  if (target == DGUS_SCREEN_POPUP) {
    // special handling for popup is to return to previous menu
    if (current_screen == DGUS_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    PopToOldScreen();
    return;
  }

  UpdateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!DGUS_FindScreenVPMapList(target)) DEBUG_ECHOLNPAIR("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandler::HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr) {
  thermalManager.disable_all_heaters();
  ForceCompleteUpdate(); // hint to send all data.
}

void DGUSScreenHandler::HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);
  uint16_t acceptedvalue;

  switch (var.VP) {
    default: return;

    #if HOTENDS >= 1
      case VP_T_E0_Set:
        thermalManager.setTargetHotend(newvalue, ExtUI::extruder_t::E0);
        acceptedvalue = thermalManager.temp_hotend[ExtUI::extruder_t::E0].target;
        break;
    #endif

    #if HOTENDS >= 2
      case VP_T_E1_Set:
        thermalManager.setTargetHotend(newvalue, ExtUI::extruder_t::E1);
        acceptedvalue = thermalManager.temp_hotend[ExtUI::extruder_t::E1].target;
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
  skip_vp = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
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

    planner.flow_percentage[target_extruder] = newvalue;
    planner.refresh_e_factor(target_extruder);
    skip_vp = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
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
  skip_vp = var.VP;
}

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)

  void DGUSScreenHandler::HandleManualMoveOption(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleManualMoveOption");
    *(uint16_t*)var.memadr = swap16(*(uint16_t*)val_ptr);
  }

#endif // DGUS_UI_MOVE_DIS_OPTION

void DGUSScreenHandler::HandleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualMove");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);

  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    const uint16_t choice = *(uint16_t*)var.memadr;
    movevalue = movevalue > 0 ? choice : -choice;
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
    ForceCompleteUpdate();
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
    char sign[] = "\0";
    int16_t value = movevalue / 100;
    if (movevalue < 0) {
      value = -value;
      sign[0] = '-';
    }
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

  ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("manmv done.");
  return;

  cannotmove:
  DEBUG_ECHOLNPAIR(" cannot move ", axiscode);
  return;
}

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
      GotoScreen(DGUS_SCREEN_SDPRINTMANIPULATION);
    }
    else {
      recovery.cancel();
      GotoScreen(DGUS_SCREEN_STATUS);
    }
  }

#endif // POWER_LOSS_RECOVERY

void DGUSScreenHandler::HandleSettings(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleSettings");
  uint16_t value = swap16(*(uint16_t*)val_ptr);

  switch (value) {
    default: break;
    case 1:
      #if ENABLED(PRINTCOUNTER)
        print_job_timer.initStats();
      #endif
      queue.enqueue_now_P(PSTR("M502\nM500"));
      break;
    case 2: queue.enqueue_now_P(PSTR("M501")); break;
    case 3: queue.enqueue_now_P(PSTR("M500")); break;
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
  skip_vp = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

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
  ExtUI::setAxisSteps_per_mm(value, extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(extruder));
  skip_vp = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
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
    skip_vp = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
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
      SendInfoScreen(PSTR("PID is autotuning"), true, PSTR("please wait"), true, NUL_STR, true, NUL_STR, true);
      GotoScreen(DGUS_SCREEN_WAITING);
    #endif
  }

#endif // HAS_PID_HEATING

#if HAS_BED_PROBE

  void DGUSScreenHandler::HandleProbeOffsetZChanged(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleProbeOffsetZChanged");

    const float offset = float(swap16(*(uint16_t*)val_ptr)) / 100.0f;
    ExtUI::setZOffset_mm(offset);
    skip_vp = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
    return;
  }

#endif // HAS_BED_PROBE

#if ENABLED(BABYSTEPPING)

  void DGUSScreenHandler::HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleLiveAdjustZ");

    int16_t flag = swap16(*(uint16_t*)val_ptr);
    int16_t steps = flag ? -20 : 20;
    ExtUI::smartAdjustAxis_steps(steps, ExtUI::axis_t::Z, true);
    ForceCompleteUpdate();
    return;
  }

#endif // BABYSTEPPING

#if FAN_COUNT > 0

  void DGUSScreenHandler::HandleFanControl(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleFanControl");
    *(uint8_t*)var.memadr = *(uint8_t*)var.memadr > 0 ? 0 : 255;
  }

#endif // FAN_COUNT > 0

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
    uint8_t bed_temp = 0;
    const uint16_t preheat_option = swap16(*(uint16_t*)val_ptr);
    switch (preheat_option) {
      case 0: // Preheat PLA
        #if defined(PREHEAT_1_TEMP_HOTEND) && defined(PREHEAT_1_TEMP_BED)
          e_temp = PREHEAT_1_TEMP_HOTEND;
          bed_temp = PREHEAT_1_TEMP_BED;
        #endif
        break;
      case 1: // Preheat ABS
        #if defined(PREHEAT_2_TEMP_HOTEND) && defined(PREHEAT_2_TEMP_BED)
          e_temp = PREHEAT_2_TEMP_HOTEND;
          bed_temp = PREHEAT_2_TEMP_BED;
        #endif
        break;
      case 2: // Preheat PET
        #if defined(PREHEAT_3_TEMP_HOTEND) && defined(PREHEAT_3_TEMP_BED)
          e_temp = PREHEAT_3_TEMP_HOTEND;
          bed_temp = PREHEAT_3_TEMP_BED;
        #endif
        break;
      case 3: // Preheat FLEX
        #if defined(PREHEAT_4_TEMP_HOTEND) && defined(PREHEAT_4_TEMP_BED)
          e_temp = PREHEAT_4_TEMP_HOTEND;
          bed_temp = PREHEAT_4_TEMP_BED;
        #endif
        break;
      case 7: // Custom preheat
        break;
      case 9: // Cool down
        e_temp = 0;
        bed_temp = 0;
        break;
      default:
        #if defined(PREHEAT_1_TEMP_HOTEND) && defined(PREHEAT_1_TEMP_BED)
          e_temp = PREHEAT_1_TEMP_HOTEND;
          bed_temp = PREHEAT_1_TEMP_BED;
        #endif
        break;
    }

    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_E0_BED_PREHEAT:
          thermalManager.setTargetHotend(e_temp, 0);
          #if HAS_HEATED_BED
            thermalManager.setTargetBed(bed_temp);
          #endif
          break;
      #endif
      #if HOTENDS >= 2
        case VP_E1_BED_PREHEAT:
          thermalManager.setTargetHotend(e_temp, 1);
          #if HAS_HEATED_BED
            thermalManager.setTargetBed(bed_temp);
          #endif
        break;
      #endif
    }

    // Go to the preheat screen to show the heating progress
    GotoScreen(DGUS_SCREEN_PREHEAT);
  }

#endif // DGUS_PREHEAT_UI

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

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
        #if ENABLED(PREHEAT_2_TEMP_HOTEND)
          e_temp = PREHEAT_2_TEMP_HOTEND;
        #endif
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
      GotoScreen(DGUS_SCREEN_UTILITY);
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
      GotoScreen(DGUS_SCREEN_FILAMENT_HEATING);
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
          GotoScreen(DGUS_SCREEN_FILAMENT_LOADING);
          filament_data.heated = true;
        }
        movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder)+movevalue;
      }
      else { // unload filament
        if (!filament_data.heated) {
          GotoScreen(DGUS_SCREEN_FILAMENT_UNLOADING);
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

#endif // DGUS_FILAMENT_LOADUNLOAD

void DGUSScreenHandler::UpdateNewScreen(DGUS_Screen newscreen, bool popup) {
  DEBUG_ECHOLNPAIR("SetNewScreen: ", newscreen);

  if (!popup) {
    memmove(&past_screens[1], &past_screens[0], sizeof(past_screens) - 1);
    past_screens[0] = current_screen;
  }

  current_screen = newscreen;
  skip_vp = 0;
  ForceCompleteUpdate();
}

void DGUSScreenHandler::PopToOldScreen() {
  DEBUG_ECHOLNPAIR("PopToOldScreen s=", past_screens[0]);
  GotoScreen(past_screens[0], true);
  memmove(&past_screens[0], &past_screens[1], sizeof(past_screens) - 1);
  past_screens[sizeof(past_screens) - 1] = DGUS_SCREEN_MAIN;
}

void DGUSScreenHandler::UpdateScreenVPData() {
  DEBUG_ECHOPAIR(" UpdateScreenVPData Screen: ", current_screen);

  const uint16_t *VPList = DGUS_FindScreenVPMapList(current_screen);
  if (!VPList) {
    DEBUG_ECHOLNPAIR(" NO SCREEN FOR: ", current_screen);
    screen_complete = true;
    return;  // nothing to do, likely a bug or boring screen.
  }

  // Round-robin updating of all VPs.
  VPList += update_index;

  bool sent_one = false;
  do {
    uint16_t VP = pgm_read_word(VPList);
    DEBUG_ECHOPAIR(" VP: ", VP);
    if (!VP) {
      update_index = 0;
      DEBUG_ECHOLNPGM(" UpdateScreenVPData done");
      screen_complete = true;
      return;  // Screen completed.
    }
    if (VP == skip_vp) {
      skip_vp = 0;
      continue;
    }

    DGUS_VP_Variable rcpy;
    if (DGUS_PopulateVPVar(VP, &rcpy)) {
      uint8_t expected_tx = 6 + rcpy.size;  // expected overhead is 6 bytes + payload.
      // Send the VP to the display, but try to avoid overrunning the Tx Buffer.
      // But send at least one VP, to avoid getting stalled.
      if (rcpy.send_to_display_handler && (!sent_one || expected_tx <= dgus_display.GetFreeTxBuffer())) {
        //DEBUG_ECHOPAIR(" calling handler for ", rcpy.VP);
        sent_one = true;
        rcpy.send_to_display_handler(rcpy);
      }
      else {
        //auto x=dgus_display.GetFreeTxBuffer();
        //DEBUG_ECHOLNPAIR(" tx almost full: ", x);
        //DEBUG_ECHOPAIR(" update_index ", update_index);
        screen_complete = false;
        return;  // please call again!
      }
    }

  } while (++update_index, ++VPList, true);
}

void DGUSScreenHandler::GotoScreen(DGUS_Screen screen, bool ispopup) {
  dgus_display.SwitchScreen(screen);
  UpdateNewScreen(screen, ispopup);
}

#endif // HAS_DGUS_LCD
