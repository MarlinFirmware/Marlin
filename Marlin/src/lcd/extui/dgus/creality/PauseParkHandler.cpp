/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * lcd/extui/dgus/creality/PauseParkHandler.cpp
 *
 * Pause:  M25 (or the same without the modal M125), then (once the queue is empty) retract and G27.
 * Resume: reheat if needed, un-retract, move back to X Y, then Z, restore the
 *         E position, and finish with M24.
 * Each stage is queued and the next one starts when the queue is done, because
 * only a short string can be injected at a time.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "PauseParkHandler.h"

#if CREALITY_PARK_ON_PAUSE

#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"
#include "../../../../module/motion.h"
#include "../../../../module/temperature.h"
#include "../../../../module/printcounter.h"
#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../../feature/host_actions.h"
#endif
#include "../../../../sd/cardreader.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#ifdef PAUSE_PARK_RETRACT_LENGTH
  #define PARK_RETRACT_LENGTH PAUSE_PARK_RETRACT_LENGTH
#else
  #define PARK_RETRACT_LENGTH 2
#endif
#ifdef PAUSE_PARK_RETRACT_FEEDRATE
  #define PARK_RETRACT_FEEDRATE PAUSE_PARK_RETRACT_FEEDRATE
#else
  #define PARK_RETRACT_FEEDRATE 60
#endif

bool PauseParkHandler::working, PauseParkHandler::parked, PauseParkHandler::retracted, PauseParkHandler::modes_saved;
float PauseParkHandler::saved_x, PauseParkHandler::saved_y, PauseParkHandler::saved_z, PauseParkHandler::saved_e;
celsius_t PauseParkHandler::saved_temp;
relative_t PauseParkHandler::saved_axis_relative;
feedRate_t PauseParkHandler::saved_feedrate;

// The queue is empty when these are called, so the G-code modes can be changed now
void PauseParkHandler::enterMoveModes() {
  modes_saved = true;
  saved_axis_relative = gcode.axis_relative;
  saved_feedrate = motion.feedrate_mm_s;
  gcode.set_relative_mode(false);
  gcode.set_e_relative();
}

void PauseParkHandler::leaveMoveModes() {
  modes_saved = false;
  gcode.axis_relative = saved_axis_relative;
  motion.feedrate_mm_s = saved_feedrate;
}

void PauseParkHandler::reset() {
  if (working) {
    screen.runWhenQueueDone(nullptr);
    if (modes_saved) leaveMoveModes();
  }
  working = parked = retracted = false;
}

bool PauseParkHandler::pause() {
  if (working || parked || !ExtUI::isPrintingFromMedia() || ExtUI::isPrintingPaused()) return false;

  working = true;
  screen.setStatusMessage(F("Pausing print - please wait..."));

  // Let the buffered commands finish before parking
  #if ENABLED(PARK_HEAD_ON_PAUSE)
    // M25 would run the modal M125, so pause the media directly, as M25 does otherwise
    if (card.isStillPrinting()) card.pauseSDPrint();
    #if ENABLED(POWER_LOSS_RECOVERY)
      if (recovery.enabled) recovery.save(true);
    #endif
    print_job_timer.pause();
    ui.reset_status();
    #if ENABLED(HOST_ACTION_COMMANDS)
      TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_PAUSE_RESUME, F("Pause SD"), F("Resume")));
      #ifdef ACTION_ON_PAUSE
        hostui.pause();
      #endif
    #endif
  #else
    ExtUI::injectCommands(F("M25"));
  #endif
  screen.runWhenQueueDone(onPaused);
  return true;
}

void PauseParkHandler::onPaused() {
  if (!working) return;

  saved_x = ExtUI::getAxisPosition_mm(ExtUI::X);
  saved_y = ExtUI::getAxisPosition_mm(ExtUI::Y);
  saved_z = ExtUI::getAxisPosition_mm(ExtUI::Z);
  saved_e = ExtUI::getAxisPosition_mm(ExtUI::E0);
  saved_temp = ExtUI::getTargetTemp_celsius(ExtUI::E0);

  enterMoveModes();

  // Retract (unless the nozzle is too cold) and park with G27
  retracted = !thermalManager.tooColdToExtrude(0);
  MString<32> cmd;
  if (retracted)
    cmd.set(F("G1 E-"), p_float_t(float(PARK_RETRACT_LENGTH), 1), F(" F"), int(MMS_TO_MMM(PARK_RETRACT_FEEDRATE)), F("\nG27"));
  else
    cmd.set(F("G27"));
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(onParked);
}

void PauseParkHandler::onParked() {
  if (!working) return;
  leaveMoveModes();
  parked = true;
  working = false;
  screen.setStatusMessage(F("Print paused"));
}

// M24 (from G-code or a host "Resume" prompt) unparks first. While moving, it is ignored.
bool PauseParkHandler::onResumeRequest() {
  if (working) return true;
  return resume();
}

bool PauseParkHandler::resume() {
  if (working || !parked) return false;

  working = true;
  screen.setStatusMessage(F("Resuming print - please wait..."));

  enterMoveModes();

  // The nozzle may have been cooled in the meantime
  if (saved_temp && ExtUI::getTargetTemp_celsius(ExtUI::E0) != saved_temp) {
    MString<16> cmd;
    cmd.setf(F("M109 S%i"), int(saved_temp));
    ExtUI::injectCommands(cmd);
    screen.runWhenQueueDone(resumeHeated);
  }
  else
    resumeHeated();
  return true;
}

// Un-retract and move back to the print position, X and Y first
void PauseParkHandler::resumeHeated() {
  if (!working) return;

  MString<64> cmd;
  if (retracted)
    cmd.set(F("G1 E"), p_float_t(float(PARK_RETRACT_LENGTH), 1), F(" F"), int(MMS_TO_MMM(PARK_RETRACT_FEEDRATE)), F("\n"));
  cmd.append(F("G1 X"), p_float_t(saved_x, 2), F(" Y"), p_float_t(saved_y, 2), F(" F"), int(MMS_TO_MMM(NOZZLE_PARK_XY_FEEDRATE)));
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(resumeMoved);
}

// Then lower Z, and restore E, which Load / Unload may have changed
void PauseParkHandler::resumeMoved() {
  if (!working) return;

  MString<64> cmd;
  cmd.set(F("G1 Z"), p_float_t(saved_z, 2), F(" F"), int(MMS_TO_MMM(NOZZLE_PARK_Z_FEEDRATE)), F("\nG92 E"), p_float_t(saved_e, 3));
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(onResumed);
}

void PauseParkHandler::onResumed() {
  if (!working) return;
  leaveMoveModes();
  parked = retracted = false;
  working = false;
  screen.gotoScreen(DGUS_SCREEN_PRINT_RUNNING); // Also when the host resumed
  ExtUI::resumePrint();
}

#endif // CREALITY_PARK_ON_PAUSE
#endif // DGUS_LCD_UI_CREALITY_TOUCH
