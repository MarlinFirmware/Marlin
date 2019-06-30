/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(DGUS_LCD)

#include "DGUSDisplay.h"
#include "DGUSVPVariable.h"
#include "DGUSDisplayDefinition.h"

#include "../../ui_api.h"

#include "../../../../Marlin.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"

// Preamble... 2 Bytes, usually 0x5A 0xA5, but configurable
constexpr uint8_t DGUS_HEADER1 = 0x5A;
constexpr uint8_t DGUS_HEADER2 = 0xA5;

constexpr uint8_t DGUS_CMD_WRITEVAR = 0x82;
constexpr uint8_t DGUS_CMD_READVAR = 0x83;

#if ENABLED(DEBUG_DGUSLCD)
  bool dguslcd_local_debug; // = false;
#endif

uint16_t DGUSScreenVariableHandler::ConfirmVP;

#if ENABLED(SDSUPPORT)
  int16_t DGUSScreenVariableHandler::top_file = 0;
  int16_t DGUSScreenVariableHandler::file_to_print = 0;
  static ExtUI::FileList filelist;
#endif

void (*DGUSScreenVariableHandler::confirm_action_cb)() = nullptr;

//DGUSScreenVariableHandler ScreenHandler;

DGUSLCD_Screens DGUSScreenVariableHandler::current_screen;
DGUSLCD_Screens DGUSScreenVariableHandler::past_screens[NUM_PAST_SCREENS];
uint8_t DGUSScreenVariableHandler::update_ptr;
uint16_t DGUSScreenVariableHandler::skipVP;
bool DGUSScreenVariableHandler::ScreenComplete;

//DGUSDisplay dgusdisplay;

rx_datagram_state_t DGUSDisplay::rx_datagram_state = DGUS_IDLE;
uint8_t DGUSDisplay::rx_datagram_len = 0;
bool DGUSDisplay::Initialized = false;
bool DGUSDisplay::no_reentrance = false;

#if DGUS_RX_BUFFER_SIZE > 256
  typedef uint16_t r_ring_buffer_pos_t;
#else
  typedef uint8_t r_ring_buffer_pos_t;
#endif

#if DGUS_TX_BUFFER_SIZE > 256
  typedef uint16_t t_ring_buffer_pos_t;
#else
  typedef uint8_t t_ring_buffer_pos_t;
#endif

class DGUSSerial {
public:
  DGUSSerial();
  ~DGUSSerial();

  r_ring_buffer_pos_t available();
  t_ring_buffer_pos_t GetTxBufferFree();
  void write(const uint8_t c);

  int read();

  // ISR for Rx
  void store_rxd_char();
  // ISR for Tx (UDRE vector)
  void tx_udr_empty_irq(void);

  inline volatile bool is_rx_overrun() {
    return dgus_rx_overrun;
  }

  inline void reset_rx_overun() {
    dgus_rx_overrun = false;
  }

private:
  r_ring_buffer_pos_t atomic_read_rx_head();
  void atomic_set_rx_tail(r_ring_buffer_pos_t value);
  r_ring_buffer_pos_t atomic_read_rx_tail();

  volatile bool dgus_rx_overrun = false;

  struct ring_buffer_r {
    volatile r_ring_buffer_pos_t head, tail;
    unsigned char buffer[DGUS_RX_BUFFER_SIZE];
  } rx_buffer = { 0, 0, { 0 } };

  struct ring_buffer_t {
    volatile t_ring_buffer_pos_t head, tail;
    unsigned char buffer[DGUS_TX_BUFFER_SIZE];
  } tx_buffer = { 0, 0, { 0 } };

  #if DGUS_RX_BUFFER_SIZE > 256
    volatile bool rx_tail_value_not_stable = false;
    volatile uint16_t rx_tail_value_backup = 0;
  #endif

};

static DGUSSerial dgusserial;

// endianness swap
uint16_t swap16(const uint16_t value) { return (value & 0xffU) << 8U | (value >> 8U); }

bool populate_VPVar(const uint16_t VP, DGUS_VP_Variable * const ramcopy) {
  // DEBUG_ECHOPAIR("populate_VPVar ", VP);
  const DGUS_VP_Variable *pvp = DGUSLCD_FindVPVar(VP);
  // DEBUG_ECHOLNPAIR(" pvp ", (uint16_t )pvp);
  if (!pvp) return false;
  memcpy_P(ramcopy, pvp, sizeof(DGUS_VP_Variable));
  return true;
}

void DGUSScreenVariableHandler::sendinfoscreen(const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool l4inflash) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_MSGSTR1, &ramcopy)) {
    ramcopy.memadr = (void*) line1;
    l1inflash ? DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR2, &ramcopy)) {
    ramcopy.memadr = (void*) line2;
    l2inflash ? DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR3, &ramcopy)) {
    ramcopy.memadr = (void*) line3;
    l3inflash ? DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR4, &ramcopy)) {
    ramcopy.memadr = (void*) line4;
    l4inflash ? DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
}

void DGUSScreenVariableHandler::HandleUserConfirmationPopUp(uint16_t VP, const char* line1, const char* line2, const char* line3, const char* line4, bool l1, bool l2, bool l3, bool l4) {
  if (current_screen == DGUSLCD_SCREEN_CONFIRM) {
    // Already showing a pop up, so we need to cancel that first.
    PopToOldScreen();
  }

  ConfirmVP = VP;
  sendinfoscreen(line1, line2, line3, line4, l1, l2, l3, l4);
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_CONFIRM);
}

void DGUSScreenVariableHandler::setstatusmessage(const char *msg) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) msg;
    DGUSLCD_SendStringToDisplay(ramcopy);
  }
}

void DGUSScreenVariableHandler::setstatusmessagePGM(PGM_P const msg) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) msg;
    DGUSLCD_SendStringToDisplayPGM(ramcopy);
  }
}

// Send an 8 bit or 16 bit value to the display.
void DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint8_t *tmp = (uint8_t *) var.memadr;
    uint16_t data_to_send = (tmp[0] << 8);
    if (var.size >= 1) data_to_send |= tmp[1];
    dgusdisplay.WriteVariable(var.VP, data_to_send);
  }
}

// Send an uint8_t between 0 and 255 to the display, but scale to a percentage (0..100)
void DGUSScreenVariableHandler::DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint16_t tmp = *(uint8_t *) var.memadr +1 ; // +1 -> avoid rounding issues for the display.
    tmp = map(tmp, 0, 255, 0, 100);
    uint16_t data_to_send = swap16(tmp);
    dgusdisplay.WriteVariable(var.VP, data_to_send);
  }
}

// Send the current print time to the display.
// It is using a hex display for that: It expects BSD coded data in the format xxyyzz
void DGUSScreenVariableHandler::DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();

  uint8_t days  = elapsed.day(),
          hours = elapsed.hour() % 24,
          minutes = elapsed.minute() % 60,
          seconds = elapsed.second() % 60;

  char buf[14], *p = buf; // that two extra bytes saves us some flash...

  if (days) { *p++ = days / 10 + '0'; *p++ = days % 10 + '0'; *p++ = 'd'; }
  *p++ = hours / 10 + '0'; *p++ = hours % 10 + '0'; *p++ = 'h';
  *p++ = minutes / 10 + '0'; *p++ = minutes % 10 + '0'; *p++ = 'm';
  *p++ = seconds / 10 + '0'; *p++ = seconds % 10 + '0'; *p++ = 's';
  *p = '\0';

  dgusdisplay.WriteVariable(VP_PrintTime, buf, var.size, true);
}



// Send an uint8_t between 0 and 100 to a variable scale to 0..255
void DGUSScreenVariableHandler::DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    *(uint8_t*)var.memadr = map(constrain(value, 0, 100), 0, 100, 0, 255);
  }
}

// Sends a (RAM located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
}

// Sends a (flash located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariablePGM(var.VP, tmp, var.size, true);
}

#if ENABLED(SDSUPPORT)

  void DGUSScreenVariableHandler::ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr) {
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
       setstatusmessagePGM(PSTR("No SD Card"));
       return;
    }
    if (card.flag.abort_sd_printing) {
       setstatusmessagePGM(PSTR("Aborting..."));
       return;
    }
  }

  void DGUSScreenVariableHandler::DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable& var, void *val_ptr) {
    auto old_top = top_file;
    int16_t scroll = (int16_t)swap16(*(uint16_t*)val_ptr);
    if (scroll == 0) {
      if (!filelist.isAtRootDir()) {
        filelist.upDir();
        top_file = 0;
        ForceCompleteUpdate();
      }
    }
    else {
      top_file += scroll;
      DEBUG_ECHOPAIR("new topfile calculated:", top_file);
      if (top_file < 0) {
        top_file = 0;
        DEBUG_ECHOLN("Top of filelist reached");
      }
      else {
        int16_t max_top = filelist.count() -  DGUS_SD_FILESPERSCREEN;
        NOLESS(max_top, 0);
        NOMORE(top_file, max_top);
      }
      DEBUG_ECHOPAIR("new topfile adjusted:", top_file);
    }

    if (old_top != top_file) ForceCompleteUpdate();
  }

  void DGUSScreenVariableHandler::DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)swap16(*(uint16_t*)val_ptr) + top_file;
    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;
    if (filelist.isDir()) {
      filelist.changeDir(filelist.filename());
      top_file = 0;
      ForceCompleteUpdate();
      return;
    }

    // Setup Confirmation screen
    file_to_print = touched_nr;
    HandleUserConfirmationPopUp(VP_SD_FileSelectConfirm, nullptr, PSTR("Print file"), filelist.filename(), PSTR("from SD Card?"), true, true, false, true);
  }

  void DGUSScreenVariableHandler::DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if(!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.filename());
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_STATUS);
  }

  void DGUSScreenVariableHandler::DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {
    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    switch (swap16(*(uint16_t*)val_ptr)) {
      case 0:  // Resume
        if (ExtUI::isPrintingFromMediaPaused()) ExtUI::resumePrint();
        break;
      case 1:  // Pause
        if (!ExtUI::isPrintingFromMediaPaused()) ExtUI::pausePrint();
        break;
      case 2:  // Abort
        ScreenHandler.HandleUserConfirmationPopUp(VP_SD_AbortPrintConfirmed, nullptr, PSTR("Abort printing"), filelist.filename(), PSTR("?"), true, true, false, true);
        break;
    }
  }

  void DGUSScreenVariableHandler::DGUSLCD_SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr) {
    ExtUI::stopPrint();
    GotoScreen(DGUSLCD_SCREEN_MAIN);
  }

  void DGUSScreenVariableHandler::DGUSLCD_SD_SendFilename(DGUS_VP_Variable& var) {
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line > DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;
    if (filelist.seek(top_file + target_line))
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0);
    DGUSLCD_SendStringToDisplay(var);
  }


  void DGUSScreenVariableHandler::SDCardInserted() {
    top_file = 0;
    auto cs = ScreenHandler.getCurrentScreen();
    if (cs == DGUSLCD_SCREEN_MAIN || cs == DGUSLCD_SCREEN_STATUS)
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_SDFILELIST);
  }

  void DGUSScreenVariableHandler::SDCardRemoved() {
    if (current_screen == DGUSLCD_SCREEN_SDFILELIST
        || (current_screen == DGUSLCD_SCREEN_CONFIRM && (ConfirmVP == VP_SD_AbortPrintConfirmed || ConfirmVP == VP_SD_FileSelectConfirm))
        || current_screen == DGUSLCD_SCREEN_SDPRINTMANIPULATION
    ) ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
  }

  void DGUSScreenVariableHandler::SDCardError() {
    DGUSScreenVariableHandler::SDCardRemoved();
    ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("SD card error"), nullptr, true, true, true, true);
    ScreenHandler.SetupConfirmAction(nullptr);
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
  }

#endif // SDSUPPORT

void DGUSScreenVariableHandler::ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr) {
  DGUS_VP_Variable ramcopy;
  if (!populate_VPVar(ConfirmVP, &ramcopy)) return;
  if (ramcopy.set_by_display_handler) ramcopy.set_by_display_handler(ramcopy, val_ptr);
}

const uint16_t* DGUSLCD_FindScreenVPMapList(uint8_t screen) {
  const uint16_t *ret;
  const struct VPMapping *map = VPMap;
  while (ret = (uint16_t*) pgm_read_word(&(map->VPList))) {
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

void DGUSScreenVariableHandler::ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr) {
  if (!ExtUI::isPrinting()) {
    ScreenChangeHook(var, val_ptr);
    dgusdisplay.RequestScreen(current_screen);
  }
}

void DGUSScreenVariableHandler::ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUSLCD_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUSLCD_Screens target = (DGUSLCD_Screens)tmp[1];

  if (target == DGUSLCD_SCREEN_POPUP) {
    // special handling for popup is to return to previous menu
    if (current_screen == DGUSLCD_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    PopToOldScreen();
    return;
  }

  UpdateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!DGUSLCD_FindScreenVPMapList(target)) DEBUG_ECHOLNPAIR("WARNING: No screen Mapping found for ", x);
  #endif
}

void DGUSScreenVariableHandler::HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr) {
  thermalManager.disable_all_heaters();
  ScreenHandler.ForceCompleteUpdate(); // hint to send all data.
}

void DGUSScreenVariableHandler::HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);
  uint16_t acceptedvalue;

  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_T_E1_Set:
        thermalManager.setTargetHotend(newvalue, 0);
        acceptedvalue = thermalManager.temp_hotend[0].target;
        break;
    #endif
    #if HOTENDS >= 2
      case VP_T_E2_Set:
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

void DGUSScreenVariableHandler::HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);
  uint8_t target_extruder;
  switch (var.VP) {
    default: return;
    #if (HOTENDS >= 1)
      case VP_Flowrate_E1: target_extruder = 0; break;
    #endif
    #if (HOTENDS >= 2)
      case VP_Flowrate_E2: target_extruder = 1; break;
    #endif
  }

  planner.flow_percentage[target_extruder] = newvalue;
  planner.refresh_e_factor(target_extruder);
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenVariableHandler::HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualMove");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  float target = movevalue * 0.01f;
  ExtUI::extruder_t target_extruder;

  switch (var.VP) {
    #if HOTENDS >=1
      case VP_MOVE_E1: target_extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >=2
      case VP_MOVE_E2: target_extruder = ExtUI::extruder_t::E1; break
    #endif
    default: return;
  }

  target += ExtUI::getAxisPosition_mm(target_extruder);
  ExtUI::setAxisPosition_mm(target, target_extruder);
  skipVP = var.VP;
}

void DGUSScreenVariableHandler::HandleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualMove");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  char axiscode;
  unsigned int speed = 1500;  //FIXME: get default feedrate for manual moves, dont hardcode.

  switch (var.VP) {
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

    default: return;
  }

  if (!movevalue) {
    // homing
    DEBUG_ECHOPAIR(" homing ", axiscode);
    char buf[6] = "G28 X";
    buf[4] = axiscode;
    //DEBUG_ECHOPAIR(" ", buf);
    while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLN(" ✓");
    ScreenHandler.ForceCompleteUpdate();
    return;
  }
  else {
    //movement
    DEBUG_ECHOPAIR(" move ", axiscode);
    bool old_relative_mode = relative_mode;
    if (!relative_mode) {
      //DEBUG_ECHO(" G91");
      while (!enqueue_and_echo_command("G91")) idle();
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
    while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLN(" ✓ ");
    if (backup_speed != speed) {
      snprintf_P(buf, 32, PSTR("G0 F%d"), backup_speed);
      while (!enqueue_and_echo_command(buf)) idle();
      //DEBUG_ECHOPAIR(" ", buf);
    }
    //while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLN(" ✓ ");
    if (!old_relative_mode) {
      //DEBUG_ECHO("G90");
      while (!enqueue_and_echo_command("G90")) idle();
      //DEBUG_ECHO(" ✓ ");
    }
  }

  ScreenHandler.ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("manmv done.");
  return;

  cannotmove:
  DEBUG_ECHOLNPAIR(" cannot move ", axiscode);
  return;
}

void DGUSScreenVariableHandler::UpdateNewScreen(DGUSLCD_Screens newscreen, bool popup) {
  DEBUG_ECHOLNPAIR("SetNewScreen: ", newscreen);

  if (!popup) {
    memmove(&past_screens[1], &past_screens[0], sizeof(past_screens) - 1);
    past_screens[0] = current_screen;
  }

  current_screen = newscreen;
  skipVP = 0;
  ForceCompleteUpdate();
}

void DGUSScreenVariableHandler::PopToOldScreen() {
  DEBUG_ECHOLNPAIR("PopToOldScreen s=", past_screens[0]);
  GotoScreen(past_screens[0], true);
  memmove(&past_screens[0], &past_screens[1], sizeof(past_screens) - 1);
  past_screens[sizeof(past_screens) - 1] = DGUSLCD_SCREEN_MAIN;
}

void DGUSScreenVariableHandler::UpdateScreenVPData() {
  DEBUG_ECHOPAIR(" UpdateScreenVPData Screen: ", current_screen);

  const uint16_t *VPList = DGUSLCD_FindScreenVPMapList(current_screen);
  if (!VPList) {
    DEBUG_ECHOLNPAIR(" NO SCREEN FOR: ", current_screen);
    ScreenComplete = true;
    return;  // nothing to do, likely a bug or boring screen.
  }

  // Round-Robbin updating of all VPs.
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

    if (VP == skipVP) {
      skipVP = 0;
      continue;
    }

    DGUS_VP_Variable rcpy;
    if (populate_VPVar(VP, &rcpy)) {
      uint8_t expected_tx = 6 + rcpy.size;  // expected overhead is 6 bytes + payload.
      // Send the VP to the display, but try to avoid overruning the Tx Buffer.
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

void DGUSDisplay::loop() {
  // protection against recursion… ProcessRx() might call indirectly idle() when trying to injecting gcode commands if the queue is full.
  if (!no_reentrance) {
    no_reentrance = true;
    ProcessRx();
    no_reentrance = false;
  }
}

void DGUSDisplay::InitDisplay() {
  RequestScreen(
    #if ENABLED(SHOW_BOOTSCREEN)
      DGUSLCD_SCREEN_BOOT
    #else
      DGUSLCD_SCREEN_MAIN
    #endif
  );
}

void DGUSDisplay::WriteVariable(uint16_t adr, const void* values, uint8_t valueslen, bool isstr) {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = myvalues ? false : true;
  WriteHeader(adr, DGUS_CMD_WRITEVAR, valueslen);
  while (valueslen--) {
    char x;
    if (!strend) x = *myvalues++;
    if ((isstr && !x) || strend) {
      strend = true;
      x = ' ';
    }
    dgusserial.write(x);
  }
}

void DGUSDisplay::WriteVariablePGM(uint16_t adr, const void* values, uint8_t valueslen, bool isstr) {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = myvalues ? false : true;
  WriteHeader(adr, DGUS_CMD_WRITEVAR, valueslen);
  while (valueslen--) {
    char x;
    if (!strend) x = pgm_read_byte(myvalues++);
    if ((isstr && !x) || strend) {
      strend = true;
      x = ' ';
    }
    dgusserial.write(x);
  }
}

void DGUSScreenVariableHandler::GotoScreen(DGUSLCD_Screens screen, bool ispopup) {
  dgusdisplay.RequestScreen(screen);
  UpdateNewScreen(screen, ispopup);
}

bool DGUSScreenVariableHandler::loop() {
  dgusdisplay.loop();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (!IsScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    UpdateScreenVPData();
  }

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool booted = false;
    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;
      GotoScreen(DGUSLCD_SCREEN_MAIN);
    }
  #endif
  return IsScreenComplete();
}

void DGUSDisplay::RequestScreen(DGUSLCD_Screens screen) {
  DEBUG_ECHOLNPAIR("GotoScreen ", screen);
  const unsigned char gotoscreen[] = { 0x5A, 0x01, (unsigned char) (screen >> 8U), (unsigned char) (screen & 0xFFU) };
  WriteVariable(0x84, gotoscreen, sizeof(gotoscreen));
}

void DGUSDisplay::ProcessRx() {

  if (!dgusserial.available() && dgusserial.is_rx_overrun()) {
    // if we've got an overrun, but reset the flag only when we've emptied the buffer
    // We want to extract as many as valid datagrams possible...
    DEBUG_ECHOPGM("OVFL");
    rx_datagram_state = DGUS_IDLE;
    dgusserial.reset_rx_overun();
  }

  uint8_t receivedbyte;
  while (dgusserial.available()) {
    switch (rx_datagram_state) {

      case DGUS_IDLE: // Waiting for the first header byte
        receivedbyte = dgusserial.read();
        //DEBUG_ECHOPAIR("< ",x);
        if (DGUS_HEADER1 == receivedbyte) rx_datagram_state = DGUS_HEADER1_SEEN;
        break;

      case DGUS_HEADER1_SEEN: // Waiting for the second header byte
        receivedbyte = dgusserial.read();
        //DEBUG_ECHOPAIR(" ",x);
        rx_datagram_state = (DGUS_HEADER2 == receivedbyte) ? DGUS_HEADER2_SEEN : DGUS_IDLE;
        break;

      case DGUS_HEADER2_SEEN: // Waiting for the length byte
        rx_datagram_len = dgusserial.read();
        DEBUG_ECHOPAIR(" (", rx_datagram_len);
        DEBUG_ECHOPGM(") ");

        // Telegram min len is 3 (command and one word of payload)
        rx_datagram_state = WITHIN(rx_datagram_len, 3, DGUS_RX_BUFFER_SIZE) ? DGUS_WAIT_TELEGRAM : DGUS_IDLE;
        break;

      case DGUS_WAIT_TELEGRAM: // wait for complete datagram to arrive.
        if (dgusserial.available() < rx_datagram_len) return;

        Initialized = true; // We've talked to it, so we defined it as initialized.
        uint8_t command = dgusserial.read();

        DEBUG_ECHOPAIR("# ", command);

        uint8_t readlen = rx_datagram_len - 1;  // command is part of len.
        unsigned char tmp[rx_datagram_len - 1];
        unsigned char *ptmp = tmp;
        while (readlen--) {
          receivedbyte = dgusserial.read();
          DEBUG_ECHOPAIR(" ", receivedbyte);
          *ptmp++ = receivedbyte;
        }
        DEBUG_ECHOPGM(" # ");
        // mostly we'll get this: 5A A5 03 82 4F 4B -- ACK on 0x82, so discard it.
        if (command == DGUS_CMD_WRITEVAR && 'O' == tmp[0] && 'K' == tmp[1]) {
          DEBUG_ECHOLNPGM(">");
          rx_datagram_state = DGUS_IDLE;
          break;
        }

        /* AutoUpload, (and answer to) Command 0x83 :
        |      tmp[0  1  2  3  4 ... ]
        | Example 5A A5 06 83 20 01 01 78 01 ……
        |          / /  |  |   \ /   |  \     \
        |        Header |  |    |    |   \_____\_ DATA (Words!)
        |     DatagramLen  /  VPAdr  |
        |           Command          DataLen (in Words) */
        if (command == DGUS_CMD_READVAR) {
          const uint16_t vp = tmp[0] << 8 | tmp[1];
          const uint8_t dlen = tmp[2] << 1;  // Convert to Bytes. (Display works with words)
          //DEBUG_ECHOPAIR(" vp=", vp, " dlen=", dlen);
          DGUS_VP_Variable ramcopy;
          if (populate_VPVar(vp, &ramcopy)) {
            if (!(dlen == ramcopy.size || (dlen == 2 && ramcopy.size == 1)))
              DEBUG_ECHOLNPGM("SIZE MISMATCH");
            else if (ramcopy.set_by_display_handler) {
              ramcopy.set_by_display_handler(ramcopy, &tmp[3]);
            }
            else
              DEBUG_ECHOLNPGM(" VPVar found, no handler.");
          }
          else
            DEBUG_ECHOLNPAIR(" VPVar not found:", vp);

          rx_datagram_state = DGUS_IDLE;
          break;
        }

      // discard what we do not understand.
      rx_datagram_state = DGUS_IDLE;
    }
  }
}

size_t DGUSDisplay::GetFreeTxBuffer() { return dgusserial.GetTxBufferFree(); }

void DGUSDisplay::WriteHeader(uint16_t adr, uint8_t cmd, uint8_t payloadlen) {
  dgusserial.write(DGUS_HEADER1);
  dgusserial.write(DGUS_HEADER2);
  dgusserial.write(payloadlen + 3);
  dgusserial.write(cmd);
  dgusserial.write(adr >> 8);
  dgusserial.write(adr & 0xFF);
}

void DGUSDisplay::WritePGM(const char str[], uint8_t len) {
  while (len--) dgusserial.write(pgm_read_byte(str++));
}

// Serial implementation stolen from MarlinSerial.cpp -- but functinality reduced to our use case
// (no XON/XOFF, no Emergency Parser, no error statistics, no support to send from interrupts ...)

// Define all UART registers
#define _TNAME(X,Y,Z)                X##Y##Z
#define TNAME(X,Y,Z)                _TNAME(X,Y,Z)
#define DGUS_SERIAL_RX_VECT          TNAME(USART,DGUS_SER_PORT,_RX_vect)
#define DGUS_SERIAL_UDRE_VECT        TNAME(USART,DGUS_SER_PORT,_UDRE_vect)
#define DGUS_UCSRxA                  TNAME(UCSR,DGUS_SER_PORT,A)
#define DGUS_UCSRxB                  TNAME(UCSR,DGUS_SER_PORT,B)
#define DGUS_UCSRxC                  TNAME(UCSR,DGUS_SER_PORT,C)
#define DGUS_UBRRxH                  TNAME(UBRR,DGUS_SER_PORT,H)
#define DGUS_UBRRxL                  TNAME(UBRR,DGUS_SER_PORT,L)
#define DGUS_UDRx                    TNAME(UDR,DGUS_SER_PORT,)

#define U2Xx                    TNAME(U2X,DGUS_SER_PORT,)
#define RXENx                   TNAME(RXEN,DGUS_SER_PORT,)
#define TXENx                   TNAME(TXEN,DGUS_SER_PORT,)
#define TXCx                    TNAME(TXC,DGUS_SER_PORT,)
#define RXCIEx                  TNAME(RXCIE,DGUS_SER_PORT,)
#define UDRIEx                  TNAME(UDRIE,DGUS_SER_PORT,)
#define UDREx                   TNAME(UDRE,DGUS_SER_PORT,)

// A SW memory barrier, to ensure GCC does not overoptimize loops
#define sw_barrier() asm volatile("": : :"memory");

DGUSSerial::DGUSSerial() {
  // Initialize UART
  DGUS_UCSRxA = 1 << U2Xx;
  const uint16_t baud_setting = (F_CPU / 4 / DGUS_BAUDRATE - 1) / 2;
  DGUS_UBRRxH = baud_setting >> 8;
  DGUS_UBRRxL = baud_setting;
  DGUS_UCSRxC = 0x06;
  DGUS_UCSRxB = 1 << RXCIEx | 1 << TXENx | 1 << RXENx;  // Enable TX,RX and the RX interrupts.
}

DGUSSerial::~DGUSSerial() { DGUS_UCSRxB = 0; }

// "Atomically" read the RX head index value without disabling interrupts:
// This MUST be called with RX interrupts enabled, and CAN'T be called
// from the RX ISR itself!
FORCE_INLINE r_ring_buffer_pos_t DGUSSerial::atomic_read_rx_head() {
  #if RX_BUFFER_SIZE > 256
    // Keep reading until 2 consecutive reads return the same value,
    // meaning there was no update in-between caused by an interrupt.
    // This works because serial RX interrupts happen at a slower rate
    // than successive reads of a variable, so 2 consecutive reads with
    // the same value means no interrupt updated it.
    r_ring_buffer_pos_t vold, vnew = rx_buffer.head;
    sw_barrier();
    do {
      vold = vnew;
      vnew = rx_buffer.head;
      sw_barrier();
    } while (vold != vnew);
    return vnew;
  #else
    // With an 8bit index, reads are always atomic. No need for special handling
    return rx_buffer.head;
  #endif
}

// Set RX tail index, taking into account the RX ISR could interrupt
//  the write to this variable in the middle - So a backup strategy
//  is used to ensure reads of the correct values.
//    -Must NOT be called from the RX ISR -
FORCE_INLINE void DGUSSerial::atomic_set_rx_tail(r_ring_buffer_pos_t value) {
  #if RX_BUFFER_SIZE > 256
    // Store the new value in the backup
    rx_tail_value_backup = value;
    sw_barrier();
    // Flag we are about to change the true value
    rx_tail_value_not_stable = true;
    sw_barrier();
    // Store the new value
    rx_buffer.tail = value;
    sw_barrier();
    // Signal the new value is completely stored into the value
    rx_tail_value_not_stable = false;
    sw_barrier();
  #else
    rx_buffer.tail = value;
  #endif
}

// Get the RX tail index, taking into account the read could be
//  interrupting in the middle of the update of that index value
//    -Called from the RX ISR -
FORCE_INLINE r_ring_buffer_pos_t DGUSSerial::atomic_read_rx_tail() {
  #if RX_BUFFER_SIZE > 256
    // If the true index is being modified, return the backup value
    if (rx_tail_value_not_stable) return rx_tail_value_backup;
  #endif
  // The true index is stable, return it
  return rx_buffer.tail;
}

// (called with RX interrupts disabled)
FORCE_INLINE void DGUSSerial::store_rxd_char() {
  // Get the tail - Nothing can alter its value while this ISR is executing, but there's
  // a chance that this ISR interrupted the main process while it was updating the index.
  // The backup mechanism ensures the correct value is always returned.
  const r_ring_buffer_pos_t t = atomic_read_rx_tail();

  // Get the head pointer - This ISR is the only one that modifies its value, so it's safe to read here
  r_ring_buffer_pos_t h = rx_buffer.head;

  // Get the next element
  r_ring_buffer_pos_t i = (r_ring_buffer_pos_t) (h + 1) & (r_ring_buffer_pos_t) (DGUS_RX_BUFFER_SIZE - 1);

  // Read the character from the USART
  uint8_t c = DGUS_UDRx;

  // If the character is to be stored at the index just before the tail
  // (such that the head would advance to the current tail), the RX FIFO is
  // full, so don't write the character or advance the head.
  if (i != t) {
    rx_buffer.buffer[h] = c;
    h = i;
  }
  else
    dgus_rx_overrun = true;

  // Store the new head value - The main loop will retry until the value is stable
  rx_buffer.head = h;
}

// (called with TX irqs disabled)
FORCE_INLINE void DGUSSerial::tx_udr_empty_irq(void) {
  // Read positions
  uint8_t t = tx_buffer.tail;
  const uint8_t h = tx_buffer.head;
  // If nothing to transmit, just disable TX interrupts. This could
  // happen as the result of the non atomicity of the disabling of RX
  // interrupts that could end reenabling TX interrupts as a side effect.
  if (h == t) {
    CBI(DGUS_UCSRxB, UDRIEx); // (Non-atomic, could be reenabled by the main program, but eventually this will succeed)
    return;
  }

  // There is something to TX, Send the next byte
  const uint8_t c = tx_buffer.buffer[t];
  t = (t + 1) & (DGUS_TX_BUFFER_SIZE - 1);
  DGUS_UDRx = c;
  tx_buffer.tail = t;

  // Clear the TXC bit (by writing a one to its bit location).
  // Ensures flush() won't return until the bytes are actually written/
  SBI(DGUS_UCSRxA, TXCx);

  // Disable interrupts if there is nothing to transmit following this byte
  if (h == t) CBI(DGUS_UCSRxB, UDRIEx);
}

r_ring_buffer_pos_t DGUSSerial::available(void) {
  const r_ring_buffer_pos_t h = atomic_read_rx_head(), t = rx_buffer.tail;
  return (r_ring_buffer_pos_t) (DGUS_RX_BUFFER_SIZE + h - t) & (DGUS_RX_BUFFER_SIZE - 1);
}

int DGUSSerial::read() {
  const r_ring_buffer_pos_t h = atomic_read_rx_head();

  // Read the tail. Main thread owns it, so it is safe to directly read it
  r_ring_buffer_pos_t t = rx_buffer.tail;

  // If nothing to read, return now
  if (h == t) return -1;

  // Get the next char
  const int v = rx_buffer.buffer[t];
  t = (r_ring_buffer_pos_t) (t + 1) & (DGUS_RX_BUFFER_SIZE - 1);

  // Advance tail - Making sure the RX ISR will always get an stable value, even
  // if it interrupts the writing of the value of that variable in the middle.
  atomic_set_rx_tail(t);
  return v;
}

void DGUSSerial::write(const uint8_t c) {
  // are we currently tranmitting? If not, we can just place the byte in UDR.
  if (!TEST(DGUS_UCSRxB, UDRIEx) && TEST(DGUS_UCSRxA, UDREx)) {
    DGUS_UDRx = c;
    SBI(DGUS_UCSRxA, TXCx);
    return;
  }

  const uint8_t i = (tx_buffer.head + 1) & (DGUS_TX_BUFFER_SIZE - 1);
  while (i == tx_buffer.tail) sw_barrier();

  // Store new char. head is always safe to move
  tx_buffer.buffer[tx_buffer.head] = c;
  tx_buffer.head = i;
  SBI(DGUS_UCSRxB, UDRIEx);  // Enable Interrupts to finish off.
}

t_ring_buffer_pos_t DGUSSerial::GetTxBufferFree() {
  const t_ring_buffer_pos_t t = tx_buffer.tail,  // next byte to send.
                            h = tx_buffer.head;  // next pos for queue.
  int ret = t - h - 1;
  if (ret < 0) ret += DGUS_TX_BUFFER_SIZE + 1;
  return ret;
}

ISR(DGUS_SERIAL_UDRE_VECT) { dgusserial.tx_udr_empty_irq(); }
ISR(DGUS_SERIAL_RX_VECT) { dgusserial.store_rxd_char(); }

#endif // DGUS_LCD
