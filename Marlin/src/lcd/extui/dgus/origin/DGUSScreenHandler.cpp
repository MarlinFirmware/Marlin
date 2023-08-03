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

#if DGUS_LCD_UI_ORIGIN

#include "../DGUSScreenHandler.h"

#include "../../../../MarlinCore.h"
#include "../../../../gcode/queue.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/settings.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../module/printcounter.h"
#include "../../../../sd/cardreader.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if HAS_MEDIA

  extern ExtUI::FileList filelist;

  void DGUSScreenHandler::sdFileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)swap16(*(uint16_t*)val_ptr) + top_file;
    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;

    if (filelist.isDir()) {
      filelist.changeDir(filelist.filename());
      top_file = 0;
      forceCompleteUpdate();
      return;
    }

    #if ENABLED(DGUS_PRINT_FILENAME)
      // Send print filename
      dgus.writeVariable(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN, true);
    #endif

    // Setup Confirmation screen
    file_to_print = touched_nr;

    handleUserConfirmationPopUp(VP_SD_FileSelectConfirm, nullptr, PSTR("Print file"), filelist.filename(), PSTR("from SD Card?"), true, true, false, true);
  }

  void DGUSScreenHandler::sdStartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    gotoScreen(DGUS_SCREEN_STATUS);
  }

  void DGUSScreenHandler::sdResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {

    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    switch (swap16(*(uint16_t*)val_ptr)) {
      case 0: { // Resume
        if (ExtUI::isPrintingFromMediaPaused()) {
          ExtUI::resumePrint();
        }
      } break;

      case 1: // Pause

        gotoScreen(DGUS_SCREEN_SDPRINTMANIPULATION);
        if (!ExtUI::isPrintingFromMediaPaused()) {
          ExtUI::pausePrint();
          //ExtUI::mks_pausePrint();
        }
        break;
      case 2: // Abort
        handleUserConfirmationPopUp(VP_SD_AbortPrintConfirmed, nullptr, PSTR("Abort printing"), filelist.filename(), PSTR("?"), true, true, false, true);
        break;
    }
  }

  void DGUSScreenHandler::sdSendFilename(DGUS_VP_Variable& var) {
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line > DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;

    if (filelist.seek(top_file + target_line)) {
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0); // snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s"), filelist.filename());
    }
    sendStringToDisplay(var);
  }

  void DGUSScreenHandler::sdCardInserted() {
    top_file = 0;
    filelist.refresh();
    auto cs = getCurrentScreen();
    if (cs == DGUS_SCREEN_MAIN || cs == DGUS_SCREEN_STATUS)
      gotoScreen(DGUS_SCREEN_SDFILELIST);
  }

  void DGUSScreenHandler::sdCardRemoved() {
    if (current_screenID == DGUS_SCREEN_SDFILELIST
        || (current_screenID == DGUS_SCREEN_CONFIRM && (confirmVP == VP_SD_AbortPrintConfirmed || confirmVP == VP_SD_FileSelectConfirm))
        || current_screenID == DGUS_SCREEN_SDPRINTMANIPULATION
    ) gotoScreen(DGUS_SCREEN_MAIN);
  }

#endif // HAS_MEDIA

void DGUSScreenHandler::screenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUS_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUS_ScreenID target = (DGUS_ScreenID)tmp[1];

  if (target == DGUS_SCREEN_POPUP) {
    // Special handling for popup is to return to previous menu
    if (current_screenID == DGUS_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    popToOldScreen();
    return;
  }

  updateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!findScreenVPMapList(target)) DEBUG_ECHOLNPGM("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandler::handleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    if (movevalue) {
      const uint16_t choice = *(uint16_t*)var.memadr;
      movevalue = movevalue < 0 ? -choice : choice;
    }
  #endif
  char axiscode;
  uint16_t speed = manual_feedrate_mm_m.x; // Default feedrate for manual moves

  switch (var.VP) {
    default: return;

    #if HAS_X_AXIS
      case VP_MOVE_X:
        axiscode = 'X';
        if (!ExtUI::canMove(ExtUI::axis_t::X)) goto cannotmove;
        break;
    #endif

    #if HAS_Y_AXIS
      case VP_MOVE_Y:
        axiscode = 'Y';
        speed = manual_feedrate_mm_m.y;
        if (!ExtUI::canMove(ExtUI::axis_t::Y)) goto cannotmove;
        break;
    #endif

    #if HAS_Z_AXIS
      case VP_MOVE_Z:
        axiscode = 'Z';
        speed = manual_feedrate_mm_m.z;
        if (!ExtUI::canMove(ExtUI::axis_t::Z)) goto cannotmove;
        break;
    #endif

    case VP_HOME_ALL: // only used for homing
      axiscode  = '\0';
      movevalue = 0; // ignore value sent from display, this VP is _ONLY_ for homing.
      break;
  }

  if (!movevalue) {
    // homing
    char buf[6] = "G28 X";
    buf[4] = axiscode;
    queue.enqueue_one_now(buf);
    forceCompleteUpdate();
    return;
  }
  else {
    // movement
    const bool old_relative_mode = relative_mode;
    if (!relative_mode) queue.enqueue_now(F("G91"));
    char buf[32]; // G1 X9999.99 F12345
    const uint16_t backup_speed = MMS_TO_MMM(feedrate_mm_s);
    char sign[] = "\0";
    int16_t value = movevalue / 100;
    if (movevalue < 0) { value = -value; sign[0] = '-'; }
    int16_t fraction = ABS(movevalue) % 100;
    snprintf_P(buf, 32, PSTR("G0 %c%s%d.%02d F%d"), axiscode, sign, value, fraction, speed);
    queue.enqueue_one_now(buf);
    if (backup_speed != speed) {
      snprintf_P(buf, 32, PSTR("G0 F%d"), backup_speed);
      queue.enqueue_one_now(buf);
    }
    //while (!enqueue_and_echo_command(buf)) idle();
    if (!old_relative_mode) queue.enqueue_now(F("G90"));
  }

  forceCompleteUpdate();

  cannotmove:
    return;
}

#if HAS_PID_HEATING
  void DGUSScreenHandler::handleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t rawvalue = swap16(*(uint16_t*)val_ptr);
    float value = (float)rawvalue / 10;
    float newvalue = 0;

    switch (var.VP) {
      default: return;
        #if HAS_HOTEND
          case VP_E0_PID_P: newvalue = value; break;
          case VP_E0_PID_I: newvalue = scalePID_i(value); break;
          case VP_E0_PID_D: newvalue = scalePID_d(value); break;
        #endif
        #if HAS_MULTI_HOTEND
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

    *(float *)var.memadr = newvalue;

    skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }
#endif // HAS_PID_HEATING

#if ENABLED(BABYSTEPPING)
  void DGUSScreenHandler::handleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr) {
    int16_t flag  = swap16(*(uint16_t*)val_ptr),
            steps = flag ? -20 : 20;
    ExtUI::smartAdjustAxis_steps(steps, ExtUI::axis_t::Z, true);
    forceCompleteUpdate();
  }
#endif

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

  void DGUSScreenHandler::handleFilamentOption(DGUS_VP_Variable &var, void *val_ptr) {
    uint8_t e_temp = 0;
    filament_data.heated = false;
    uint16_t preheat_option = swap16(*(uint16_t*)val_ptr);
    if (preheat_option <= 8) {      // Load filament type
      filament_data.action = 1;
    }
    else if (preheat_option >= 10) { // Unload filament type
      preheat_option -= 10;
      filament_data.action = 2;
      filament_data.purge_length = DGUS_FILAMENT_PURGE_LENGTH;
    }
    else {                          // Cancel filament operation
      filament_data.action = 0;
    }

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
      #if HAS_HOTEND
        thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E0);
        #if HAS_MULTI_HOTEND
          thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E1);
        #endif
      #endif
      gotoScreen(DGUS_SCREEN_UTILITY);
    }
    else { // Go to the preheat screen to show the heating progress
      switch (var.VP) {
        default: return;
          #if HAS_HOTEND
            case VP_E0_FILAMENT_LOAD_UNLOAD:
              filament_data.extruder = ExtUI::extruder_t::E0;
              thermalManager.setTargetHotend(e_temp, filament_data.extruder);
              break;
          #endif
          #if HAS_MULTI_HOTEND
            case VP_E1_FILAMENT_LOAD_UNLOAD:
              filament_data.extruder = ExtUI::extruder_t::E1;
              thermalManager.setTargetHotend(e_temp, filament_data.extruder);
              break;
          #endif
      }
      gotoScreen(DGUS_SCREEN_FILAMENT_HEATING);
    }
  }

  void DGUSScreenHandler::handleFilamentLoadUnload(DGUS_VP_Variable &var) {
    if (filament_data.action <= 0) return;

    // If we close to the target temperature, we can start load or unload the filament
    if (thermalManager.hotEnoughToExtrude(filament_data.extruder) && \
        thermalManager.targetHotEnoughToExtrude(filament_data.extruder)) {
      float movevalue = DGUS_FILAMENT_LOAD_LENGTH_PER_TIME;

      if (filament_data.action == 1) { // load filament
        if (!filament_data.heated) {
          //gotoScreen(DGUS_SCREEN_FILAMENT_LOADING);
          filament_data.heated = true;
        }
        movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) + movevalue;
      }
      else { // unload filament
        if (!filament_data.heated) {
          gotoScreen(DGUS_SCREEN_FILAMENT_UNLOADING);
          filament_data.heated = true;
        }
        // Before unloading extrude to prevent jamming
        if (filament_data.purge_length >= 0) {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) + movevalue;
          filament_data.purge_length -= movevalue;
        }
        else {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) - movevalue;
        }
      }
      ExtUI::setAxisPosition_mm(movevalue, filament_data.extruder);
    }
  }
#endif // DGUS_FILAMENT_LOADUNLOAD

bool DGUSScreenHandler::loop() {
  dgus.loop();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (!isScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    updateScreenVPData();
  }

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool booted = false;

    if (!booted && TERN0(POWER_LOSS_RECOVERY, recovery.valid()))
      booted = true;

    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;
      gotoScreen(TERN0(POWER_LOSS_RECOVERY, recovery.valid()) ? DGUS_SCREEN_POWER_LOSS : DGUS_SCREEN_MAIN);
    }
  #endif

  return isScreenComplete();
}

#endif // DGUS_LCD_UI_ORIGIN
