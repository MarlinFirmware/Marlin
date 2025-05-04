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

#if DGUS_LCD_UI_MKS

#include "../DGUSScreenHandler.h"

#include "../../../../inc/MarlinConfig.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/settings.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../module/printcounter.h"

#include "../../../../gcode/gcode.h"

#if HAS_STEALTHCHOP
  #include "../../../../module/stepper/trinamic.h"
  #include "../../../../module/stepper/indirection.h"
#endif
#include "../../../../module/probe.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
  #define FILAMENT_IS_OUT(N...) (READ(FIL_RUNOUT##N##_PIN) == FIL_RUNOUT##N##_STATE)
#endif

#if HAS_MEDIA
  extern ExtUI::FileList filelist;
#endif

bool DGUSAutoTurnOff = false;
MKS_Language mks_language_index; // Initialized by settings.load

void DGUSScreenHandlerMKS::sendInfoScreen(const uint16_t *line1, const uint16_t *line2, const uint16_t *line3, const uint16_t *line4) {
  dgus.writeStringVar(VP_MSGSTR1, line1);
  dgus.writeStringVar(VP_MSGSTR2, line2);
  dgus.writeStringVar(VP_MSGSTR3, line3);
  dgus.writeStringVar(VP_MSGSTR4, line4);
}

void DGUSScreenHandlerMKS::sendInfoScreen(const char *line1, const char *line2, const char *line3, const char *line4) {
  dgus.writeStringVar(VP_MSGSTR1, line1);
  dgus.writeStringVar(VP_MSGSTR2, line2);
  dgus.writeStringVar(VP_MSGSTR3, line3);
  dgus.writeStringVar(VP_MSGSTR4, line4);
}

void DGUSScreenHandlerMKS::sendInfoScreen_P(PGM_P const line1, PGM_P const line2, PGM_P const line3, PGM_P const line4) {
  dgus.writeStringVar_P(VP_MSGSTR1, line1);
  dgus.writeStringVar_P(VP_MSGSTR2, line2);
  dgus.writeStringVar_P(VP_MSGSTR3, line3);
  dgus.writeStringVar_P(VP_MSGSTR4, line4);
}

void DGUSScreenHandlerMKS::sendInfoScreenMKS(const void *line1, const void *line2, const void *line3, const void *line4, const MKS_Language language) {
  if (language == MKS_English)
    DGUSScreenHandlerMKS::sendInfoScreen((char *)line1, (char *)line2, (char *)line3, (char *)line4);
  else if (language == MKS_SimpleChinese)
    DGUSScreenHandlerMKS::sendInfoScreen((uint16_t *)line1, (uint16_t *)line2, (uint16_t *)line3, (uint16_t *)line4);
}

void DGUSScreenHandlerMKS::sendFanToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    uint16_t tmp = *(uint8_t *) var.memadr; // +1 -> avoid rounding issues for the display.
    //tmp = map(constrain(tmp, 0, 255), 0, 255, 0, 100);
    dgus.writeVariable(var.VP, tmp);
  }
}

void DGUSScreenHandlerMKS::sendBabyStepToDisplay(DGUS_VP_Variable &var) {
  float value = current_position.z;
  value *= 100; //cpow(10, 2);
  dgus.writeVariable(VP_SD_Print_Baby, (uint16_t)value);
}

void DGUSScreenHandlerMKS::sendPrintTimeToDisplay(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();
  uint32_t time = elapsed.value;
  dgus.writeVariable(VP_PrintTime_H, uint16_t(time / 3600));
  dgus.writeVariable(VP_PrintTime_M, uint16_t(time % 3600 / 60));
  dgus.writeVariable(VP_PrintTime_S, uint16_t((time % 3600) % 60));
}

void DGUSScreenHandlerMKS::setUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    const uint16_t value = BE16_P(val_ptr);
    *(uint8_t*)var.memadr = map(constrain(value, 0, 255), 0, 255, 0, 255);
  }
}

void DGUSScreenHandlerMKS::sendGbkToDisplay(DGUS_VP_Variable &var) {
  uint16_t *tmp = (uint16_t*) var.memadr;
  dgus.writeStringVar(var.VP, tmp, var.size);
}

void DGUSScreenHandlerMKS::sendStringToDisplay_Language(DGUS_VP_Variable &var) {
  switch (mks_language_index) {
    default:
    case MKS_English: {
      char *tmp = (char*) var.memadr;
      dgus.writeStringVar(var.VP, tmp, var.size);
    } break;
    case MKS_SimpleChinese: {
      uint16_t *tmp = (uint16_t *)var.memadr;
      dgus.writeStringVar(var.VP, tmp, var.size);
    } break;
  }
}

void DGUSScreenHandlerMKS::sendTMCSensValue(DGUS_VP_Variable &var) {
  #if ENABLED(SENSORLESS_HOMING)
    #if X_HAS_STEALTHCHOP
      tmc_stall_sens.x = stepperX.homing_threshold();
      dgus.writeVariable(var.VP, *(int16_t*)var.memadr);
    #endif
    #if Y_HAS_STEALTHCHOP
      tmc_stall_sens.y = stepperY.homing_threshold();
      dgus.writeVariable(var.VP, *(int16_t*)var.memadr);
    #endif
    #if Z_HAS_STEALTHCHOP
      tmc_stall_sens.z = stepperZ.homing_threshold();
      dgus.writeVariable(var.VP, *(int16_t*)var.memadr);
    #endif
  #endif
}

#if HAS_MEDIA

  void DGUSScreenHandler::sdFileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)BE16_P(val_ptr) + top_file;
    if (touched_nr != 0x0F && touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr) && touched_nr != 0x0F) return;

    if (touched_nr == 0x0F) {
      if (filelist.isAtRootDir())
        gotoScreen(DGUS_SCREEN_MAIN);
      else
        filelist.upDir();
      return;
    }

    if (filelist.isDir()) {
      filelist.changeDir(filelist.filename());
      top_file = 0;
      forceCompleteUpdate();
      return;
    }

    #if ENABLED(DGUS_PRINT_FILENAME)
      // Send print filename
      dgus.writeStringVar(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN);
    #endif

    // Setup Confirmation screen
    file_to_print = touched_nr;
    gotoScreen(MKSLCD_SCREEN_PRINT_CONFIRM);
  }

  void DGUSScreenHandler::sdStartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    gotoScreen(MKSLCD_SCREEN_PRINT);
    z_offset_add = 0;
  }

  void DGUSScreenHandler::sdResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {

    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    switch (BE16_P(val_ptr)) {
      case 0: { // Resume
        auto cs = getCurrentScreen();
        if (runout_mks.runout_status != RUNOUT_WAITING_STATUS && runout_mks.runout_status != UNRUNOUT_STATUS) {
          if (cs == MKSLCD_SCREEN_PRINT || cs == MKSLCD_SCREEN_PAUSE)
            gotoScreen(MKSLCD_SCREEN_PAUSE);
          return;
        }
        else
          runout_mks.runout_status = UNRUNOUT_STATUS;

        gotoScreen(MKSLCD_SCREEN_PRINT);

        if (ExtUI::isPrintingFromMediaPaused()) {
          nozzle_park_mks.print_pause_start_flag = 0;
          nozzle_park_mks.blstatus = true;
          ExtUI::resumePrint();
        }
      } break;

      case 1: // Pause
        gotoScreen(MKSLCD_SCREEN_PAUSE);
        if (!ExtUI::isPrintingFromMediaPaused()) {
          nozzle_park_mks.print_pause_start_flag = 1;
          nozzle_park_mks.blstatus = true;
          ExtUI::pausePrint();
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

    uint16_t dir_icon_val = 25;
    if (filelist.seek(top_file + target_line)) {
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0); // snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s"), filelist.filename());
      dir_icon_val = filelist.isDir() ? 0 : 1;
    }
    sendStringToDisplay(var);

    dgus.writeVariable(VP_File_Picture0 + target_line * 2, dir_icon_val);
  }

  void DGUSScreenHandler::sdCardInserted() {
    top_file = 0;
    filelist.refresh();
    auto cs = getCurrentScreen();
    if (cs == DGUS_SCREEN_MAIN || cs == DGUS_SCREEN_STATUS)
      gotoScreen(MKSLCD_SCREEN_CHOOSE_FILE);
  }

  void DGUSScreenHandler::sdCardRemoved() {
    if (current_screenID == DGUS_SCREEN_SDFILELIST
      || (current_screenID == DGUS_SCREEN_CONFIRM && (confirmVP == VP_SD_AbortPrintConfirmed || confirmVP == VP_SD_FileSelectConfirm))
      || current_screenID == DGUS_SCREEN_SDPRINTMANIPULATION
    ) filelist.refresh();
  }

  void DGUSScreenHandlerMKS::sdPrintingFinished() {
    if (DGUSAutoTurnOff) {
      queue.exhaust();
      gcode.process_subcommands_now(F("M81"));
    }
    gotoScreen(MKSLCD_SCREEN_PrintDone);
  }

#else // !HAS_MEDIA

  void DGUSScreenHandlerMKS::printReturn(DGUS_VP_Variable& var, void *val_ptr) {
    if (BE16_P(val_ptr) == 0x0F) gotoScreen(DGUS_SCREEN_MAIN);
  }

#endif

void DGUSScreenHandler::screenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUS_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUS_ScreenID target = (DGUS_ScreenID)tmp[1];

  // When the DGUS reboots it enters the DGUS_SCREEN_MAIN page so the user
  // can change any page to use this function and it will check whether a print
  // job is active. If so DGUS will go to the printing page to continue the job.
  //
  //if (printJobOngoing() || printingIsPaused()) {
  //  if (target == MKSLCD_PAUSE_SETTING_MOVE || target == MKSLCD_PAUSE_SETTING_EX
  //    || target == MKSLCD_SCREEN_PRINT || target == MKSLCD_SCREEN_PAUSE
  //  ) {
  //  }
  //  else
  //    gotoScreen(MKSLCD_SCREEN_PRINT);
  //  return;
  //}

  if (target == DGUS_SCREEN_POPUP) {
    setupConfirmAction(ExtUI::setUserConfirmed);

    // Special handling for popup is to return to previous menu
    if (current_screenID == DGUS_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    popToOldScreen();
    return;
  }

  updateNewScreen(target);

  #if ENABLED(DEBUG_DGUSLCD)
    if (!findScreenVPMapList(target)) DEBUG_ECHOLNPGM("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandlerMKS::screenBackChange(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t target = BE16_P(val_ptr);
  switch (target) {
  }
}

void DGUSScreenHandlerMKS::zOffsetConfirm(DGUS_VP_Variable &var, void *val_ptr) {
  settings.save();
  if (printJobOngoing())
    gotoScreen(MKSLCD_SCREEN_PRINT);
  else if (print_job_timer.isPaused)
    gotoScreen(MKSLCD_SCREEN_PAUSE);
}

void DGUSScreenHandlerMKS::getTurnOffCtrl(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t value = BE16_P(val_ptr);
  if (value < 2) DGUSAutoTurnOff = (bool)value;
}

void DGUSScreenHandlerMKS::getMinExtrudeTemp(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t value = BE16_P(val_ptr);
  TERN_(PREVENT_COLD_EXTRUSION, thermalManager.extrude_min_temp = value);
  mks_min_extrusion_temp = value;
  settings.save();
}

void DGUSScreenHandlerMKS::getZoffsetDistance(DGUS_VP_Variable &var, void *val_ptr) {
  float val_distance = 0;
  switch (BE16_P(val_ptr)) {
    default:
    case 0: val_distance = 0.01f; break;
    case 1: val_distance = 0.10f; break;
    case 2: val_distance = 0.50f; break;
    case 3: val_distance = 1.00f; break;
  }
  ZOffset_distance = val_distance;
}

void DGUSScreenHandlerMKS::getManualMovestep(DGUS_VP_Variable &var, void *val_ptr) {
  *(uint16_t *)var.memadr = BE16_P(val_ptr);
}

void DGUSScreenHandlerMKS::eepromControl(DGUS_VP_Variable &var, void *val_ptr) {
  switch (BE16_P(val_ptr)) {
    case 0:
      settings.save();
      //settings.load(); // Load EEPROM to validate the data
      gotoScreen(MKSLCD_SCREEN_EEP_Config);
      break;

    case 1:
      settings.reset();
      gotoScreen(MKSLCD_SCREEN_EEP_Config);
      break;
  }
}

void DGUSScreenHandlerMKS::zOffsetSelect(DGUS_VP_Variable &var, void *val_ptr) {
  switch (BE16_P(val_ptr)) {
    case 0:  Z_distance = 0.01f; break;
    case 1:  Z_distance = 0.10f; break;
    case 2:  Z_distance = 0.50f; break;
    case 3:
    default: Z_distance = 1.00f; break;
  }
}

void DGUSScreenHandlerMKS::getOffsetValue(DGUS_VP_Variable &var, void *val_ptr) {
  #if HAS_BED_PROBE
    const float offset = BE32_P(val_ptr) / 100.0f;
    switch (var.VP) {
      default: break;
      case VP_OFFSET_X: probe.offset.x = offset; break;
      case VP_OFFSET_Y: probe.offset.y = offset; break;
      case VP_OFFSET_Z: probe.offset.z = offset; break;
    }
    settings.save();
  #endif
}

void DGUSScreenHandlerMKS::languageChange(DGUS_VP_Variable &var, void *val_ptr) {
  const MKS_Language lang = (MKS_Language)BE16_P(val_ptr);
  if (lang != MKS_SimpleChinese && lang != MKS_English) return;
  mks_language_index = lang;
  updateDisplayLanguage();
  languagePInit();
  settings.save();
}

#if ENABLED(MESH_BED_LEVELING)
  grid_count_t mesh_point_count = GRID_MAX_POINTS;
#endif

void DGUSScreenHandlerMKS::levelControl(DGUS_VP_Variable &var, void *val_ptr) {
  #if ENABLED(MESH_BED_LEVELING)
    auto cs = getCurrentScreen();
  #endif

  switch (BE16_P(val_ptr)) {
    case 0:
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

        static uint8_t a_first_level = 1;
        if (a_first_level == 1) {
          a_first_level = 0;
          queue.enqueue_now_P(G28_STR);
        }
        queue.enqueue_now(F("G29"));

      #elif ENABLED(MESH_BED_LEVELING)

        mesh_point_count = GRID_MAX_POINTS;

        switch (mks_language_index) {
          default:
          case MKS_English: {
            const char level_buf_en[] = "Start Leveling";
            dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_en);
          } break;
          case MKS_SimpleChinese: {
            const uint16_t level_buf_ch[] = { 0xAABF, 0xBCCA, 0xF7B5, 0xBDC6, 0x2000 };
            dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_ch);
          } break;
        }

        cs = getCurrentScreen();
        if (cs != MKSLCD_AUTO_LEVEL) gotoScreen(MKSLCD_AUTO_LEVEL);

      #else

        gotoScreen(MKSLCD_SCREEN_LEVEL);

      #endif

      break;

    case 1:
      soft_endstop._enabled = true;
      gotoScreen(MKSLCD_SCREEM_TOOL);
      break;

    default: break;
  }
}

void DGUSScreenHandlerMKS::meshLevelDistanceConfig(DGUS_VP_Variable &var, void *val_ptr) {
  switch (BE16_P(val_ptr)) {
    case 0:  mesh_adj_distance = 0.01f; break;
    case 1:  mesh_adj_distance = 0.10f; break;
    case 2:  mesh_adj_distance = 1.00f; break;
    default: mesh_adj_distance = 0.10f; break;
  }
}

void DGUSScreenHandlerMKS::meshLevel(DGUS_VP_Variable &var, void *val_ptr) {
  #if ENABLED(MESH_BED_LEVELING)
    char cmd_buf[30];
    float offset = mesh_adj_distance;
    int16_t integer, Deci, Deci2;

    if (!queue.ring_buffer.empty()) return;

    switch (BE16_P(val_ptr)) {
      case 0:
        offset = mesh_adj_distance;
        integer = offset; // get int
        Deci = (offset * 10) % 10;
        Deci2 = (offset * 100) % 10;
        soft_endstop._enabled = false;
        queue.enqueue_now(F("G91"));
        snprintf_P(cmd_buf, 30, PSTR("G1 Z%d.%d%d"), integer, Deci, Deci2);
        queue.enqueue_one_now(cmd_buf);
        queue.enqueue_now(F("G90"));
        break;

      case 1:
        offset = mesh_adj_distance;
        integer = offset;       // get int
        Deci = (offset * 10) % 10;
        Deci2 = (offset * 100) % 10;
        soft_endstop._enabled = false;
        queue.enqueue_now(F("G91"));
        snprintf_P(cmd_buf, 30, PSTR("G1 Z-%d.%d%d"), integer, Deci, Deci2);
        queue.enqueue_one_now(cmd_buf);
        queue.enqueue_now(F("G90"));
        break;

      case 2:
        if (mesh_point_count == GRID_MAX_POINTS) { // The first point
          queue.enqueue_now(F("G28\nG29S1"));
          mesh_point_count--;

          switch (mks_language_index) {
            default:
            case MKS_English: {
              const char level_buf_en1[] = "Next Point";
              dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_en1);
            } break;
            case MKS_SimpleChinese: {
              const uint16_t level_buf_ch1[] = { 0xC2CF, 0xBBD2, 0xE3B5, 0x2000 };
              dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_ch1);
            } break;
          }
        }
        else if (mesh_point_count > 1) {
          queue.enqueue_now(F("G29S2"));
          mesh_point_count--;
          switch (mks_language_index) {
            default:
            case MKS_English: {
              const char level_buf_en2[] = "Next Point";
              dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_en2);
            } break;
            case MKS_SimpleChinese: {
              const uint16_t level_buf_ch2[] = { 0xC2CF, 0xBBD2, 0xE3B5, 0x2000 };
              dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_ch2);
            } break;
          }
        }
        else if (mesh_point_count == 1) {
          queue.enqueue_now(F("G29S2"));
          mesh_point_count--;
          switch (mks_language_index) {
            default:
            case MKS_English: {
              const char level_buf_en2[] = "Leveling Done";
              dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_en2);
            } break;
            case MKS_SimpleChinese: {
              const uint16_t level_buf_ch2[] = { 0xF7B5, 0xBDC6, 0xEACD, 0xC9B3, 0x2000 };
              dgus.writeStringVar(VP_AutoLevel_1_Dis, level_buf_ch2);
            } break;
          }
          settings.save();
        }
        else if (mesh_point_count == 0) {
          mesh_point_count = GRID_MAX_POINTS;
          soft_endstop._enabled = true;
          settings.save();
          gotoScreen(MKSLCD_SCREEM_TOOL);
        }
        break;

      default: break;
    }
  #endif // MESH_BED_LEVELING
}

void DGUSScreenHandlerMKS::sdFileBack(DGUS_VP_Variable&, void*) {
  gotoScreen(MKSLCD_SCREEN_HOME);
}

void DGUSScreenHandlerMKS::lcdBLKAdjust(DGUS_VP_Variable &var, void *val_ptr) {
  lcd_default_light = constrain(BE16_P(val_ptr), 10, 100);

  const uint16_t lcd_data[2] = { lcd_default_light, lcd_default_light };
  dgus.writeVariable(0x0082, &lcd_data, 5, true);
}

void DGUSScreenHandlerMKS::manualAssistLeveling(DGUS_VP_Variable &var, void *val_ptr) {
  const int16_t point_val = BE16_P(val_ptr);

  // Insist on leveling first time at this screen
  static bool first_level_flag = false;
  if (!first_level_flag || point_val == 0x0001) {
    queue.enqueue_now_P(G28_STR);
    first_level_flag = true;
  }

  constexpr uint16_t level_speed = 1500;

  auto enqueue_corner_move = [](int16_t lx, int16_t ly, uint16_t fr) {
    char buf_level[32];
    sprintf_P(buf_level, "G0X%dY%dF%d", lx, ly, fr);
    queue.enqueue_one_now(buf_level);
  };

  if (WITHIN(point_val, 0x0001, 0x0005))
    queue.enqueue_now(F("G1Z10"));

  switch (point_val) {
    case 0x0001:
      enqueue_corner_move(X_MIN_POS + ABS(mks_corner_offsets[0].x),
                          Y_MIN_POS + ABS(mks_corner_offsets[0].y), level_speed);
      queue.enqueue_now(F("G28Z"));
      break;
    case 0x0002:
      enqueue_corner_move(X_MAX_POS - ABS(mks_corner_offsets[1].x),
                          Y_MIN_POS + ABS(mks_corner_offsets[1].y), level_speed);
      break;
    case 0x0003:
      enqueue_corner_move(X_MAX_POS - ABS(mks_corner_offsets[2].x),
                          Y_MAX_POS - ABS(mks_corner_offsets[2].y), level_speed);
      break;
    case 0x0004:
      enqueue_corner_move(X_MIN_POS + ABS(mks_corner_offsets[3].x),
                          Y_MAX_POS - ABS(mks_corner_offsets[3].y), level_speed);
      break;
    case 0x0005:
      enqueue_corner_move(ABS(mks_corner_offsets[4].x),
                          ABS(mks_corner_offsets[4].y), level_speed);
      break;
  }

  if (WITHIN(point_val, 0x0002, 0x0005))
    queue.enqueue_now(F("G1Z-10"));
}

#if ANY(HAS_TRINAMIC_CONFIG, HAS_STEALTHCHOP)

  void DGUSScreenHandlerMKS::tmcChangeConfig(DGUS_VP_Variable &var, void *val_ptr) {
    const uint16_t tmc_val = BE16_P(val_ptr);
    switch (var.VP) {
      case VP_TMC_X_SENS:
        #if USE_SENSORLESS && X_HAS_STEALTHCHOP
          stepperX.homing_threshold(_MIN(tmc_val, 255));
          settings.save();
          tmc_stall_sens.x = stepperX.homing_threshold();
        #endif
        break;
      case VP_TMC_Y_SENS:
        #if USE_SENSORLESS && Y_HAS_STEALTHCHOP
          stepperY.homing_threshold(_MIN(tmc_val, 255));
          settings.save();
          tmc_stall_sens.y = stepperY.homing_threshold();
        #endif
        break;
      case VP_TMC_Z_SENS:
        #if USE_SENSORLESS && Z_HAS_STEALTHCHOP
          stepperZ.homing_threshold(_MIN(tmc_val, 255));
          settings.save();
          tmc_stall_sens.z = stepperZ.homing_threshold();
        #endif
        break;
      case VP_TMC_X_Current:
        #if X_IS_TRINAMIC
          stepperX.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_X1_Current:
        #if X2_IS_TRINAMIC
          stepperX2.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_Y_Current:
        #if Y_IS_TRINAMIC
          stepperY.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_Y1_Current:
        #if Y2_IS_TRINAMIC
          stepperY2.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_Z_Current:
        #if Z_IS_TRINAMIC
          stepperZ.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_Z1_Current:
        #if Z2_IS_TRINAMIC
          stepperZ2.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_E0_Current:
        #if E0_IS_TRINAMIC
          stepperE0.rms_current(tmc_val);
          settings.save();
        #endif
        break;
      case VP_TMC_E1_Current:
        #if E1_IS_TRINAMIC
          stepperE1.rms_current(tmc_val);
          settings.save();
        #endif
        break;

      default: break;
    }
  }

#endif // HAS_TRINAMIC_CONFIG || HAS_STEALTHCHOP

void DGUSScreenHandler::handleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  int16_t movevalue = BE16_P(val_ptr);

  // Choose Move distance
       if (manualMoveStep == 0x01) manualMoveStep =   10;
  else if (manualMoveStep == 0x02) manualMoveStep =  100;
  else if (manualMoveStep == 0x03) manualMoveStep = 1000;

  if (!print_job_timer.isPaused() && !queue.ring_buffer.empty())
    return;

  char axiscode = '\0';
  uint16_t speed = 0;

  switch (var.VP) { // switch X Y Z or Home
    #if HAS_X_AXIS
      case VP_MOVE_X:
        if (!ExtUI::canMove(ExtUI::axis_t::X)) return;
        axiscode = 'X'; speed = manual_feedrate_mm_m.x;
        break;
    #endif

    #if HAS_Y_AXIS
      case VP_MOVE_Y:
        if (!ExtUI::canMove(ExtUI::axis_t::Y)) return;
        axiscode = 'Y'; speed = manual_feedrate_mm_m.y;
        break;
    #endif

    #if HAS_Z_AXIS
      case VP_MOVE_Z:
        if (!ExtUI::canMove(ExtUI::axis_t::Z)) return;
        axiscode = 'Z'; speed = manual_feedrate_mm_m.z;
        break;
    #endif

    case VP_MOTOR_LOCK_UNLOCK: movevalue = 5; break;

    // Ignore value sent from display, this VP is _ONLY_ for homing.
    case VP_HOME_ALL: axiscode = '\0'; movevalue = 0; break;

    #if HAS_X_AXIS
      case VP_X_HOME: axiscode = 'X'; movevalue = 0; break;
    #endif
    #if HAS_Y_AXIS
      case VP_Y_HOME: axiscode = 'Y'; movevalue = 0; break;
    #endif
    #if HAS_Z_AXIS
      case VP_Z_HOME: axiscode = 'Z'; movevalue = 0; break;
    #endif
  }

  if WITHIN(movevalue, 1, 4)  // get move distance
    movevalue == 1 ? movevalue =  manualMoveStep : movevalue = -manualMoveStep;

  if (!movevalue) {
    queue.enqueue_one_now(TS(F("G28"), axiscode));
    forceCompleteUpdate();
    return;
  }

  if (movevalue == 5) {
    queue.enqueue_one_now(TS(F("M84"), axiscode));
    forceCompleteUpdate();
    return;
  }

  // Movement
  const bool old_relative_mode = relative_mode;
  if (!relative_mode) queue.enqueue_now(F("G91"));

  // TODO: Use MString / TS() ...

  char buf[32]; // G1 X9999.99 F12345
  char sign[] = "\0";
  int16_t value = movevalue / 100;
  if (movevalue < 0) { value = -value; sign[0] = '-'; }
  const int16_t fraction = ABS(movevalue) % 100;
  snprintf_P(buf, 32, PSTR("G0 %c%s%d.%02d F%d"), axiscode, sign, value, fraction, speed);
  queue.enqueue_one_now(buf);

  if (!old_relative_mode) queue.enqueue_now(F("G90"));

  forceCompleteUpdate();
}

void DGUSScreenHandlerMKS::getParkPos(DGUS_VP_Variable &var, void *val_ptr) {
  const int16_t pos = BE16_P(val_ptr);
  switch (var.VP) {
    case VP_X_PARK_POS: mks_park_pos.x = pos; break;
    case VP_Y_PARK_POS: mks_park_pos.y = pos; break;
    case VP_Z_PARK_POS: mks_park_pos.z = pos; break;
  }
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::handleChangeLevelPoint(DGUS_VP_Variable &var, void *val_ptr) {
  *(int16_t*)var.memadr = BE16_P(val_ptr);
  settings.save();
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

#if ENABLED(EDITABLE_STEPS_PER_UNIT)

  void DGUSScreenHandlerMKS::handleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr) {
    const float value = (float)BE16_P(val_ptr);
    switch (var.VP) {
      case VP_X_STEP_PER_MM: ExtUI::setAxisSteps_per_mm(value, ExtUI::axis_t::X); break;
      case VP_Y_STEP_PER_MM: ExtUI::setAxisSteps_per_mm(value, ExtUI::axis_t::Y); break;
      case VP_Z_STEP_PER_MM: ExtUI::setAxisSteps_per_mm(value, ExtUI::axis_t::Z); break;
      #if HAS_HOTEND
        case VP_E0_STEP_PER_MM: ExtUI::setAxisSteps_per_mm(value, ExtUI::extruder_t::E0); break;
      #endif
      #if HAS_MULTI_HOTEND
        case VP_E1_STEP_PER_MM: ExtUI::setAxisSteps_per_mm(value, ExtUI::extruder_t::E1); break;
      #endif
    }
    settings.save();
    skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
  }

#endif // EDITABLE_STEPS_PER_UNIT

void DGUSScreenHandlerMKS::handleMaxSpeedChange(DGUS_VP_Variable &var, void *val_ptr) {
  const float value = (float)BE16_P(val_ptr);
  switch (var.VP) {
    case VP_X_MAX_SPEED: ExtUI::setAxisMaxFeedrate_mm_s(value, ExtUI::axis_t::X); break;
    case VP_Y_MAX_SPEED: ExtUI::setAxisMaxFeedrate_mm_s(value, ExtUI::axis_t::Y); break;
    case VP_Z_MAX_SPEED: ExtUI::setAxisMaxFeedrate_mm_s(value, ExtUI::axis_t::Z); break;
    #if HAS_HOTEND
      case VP_E0_MAX_SPEED: ExtUI::setAxisMaxFeedrate_mm_s(value, ExtUI::extruder_t::E0); break;
    #endif
    #if HAS_MULTI_HOTEND
      case VP_E1_MAX_SPEED: ExtUI::setAxisMaxFeedrate_mm_s(value, ExtUI::extruder_t::E1); break;
    #endif
  }
  settings.save();
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::handleMaxAccChange(DGUS_VP_Variable &var, void *val_ptr) {
  const float value = (float)BE16_P(val_ptr);
  switch (var.VP) {
    default: return;
    case VP_X_MAX_ACC: ExtUI::setAxisMaxAcceleration_mm_s2(value, ExtUI::axis_t::X); break;
    case VP_Y_MAX_ACC: ExtUI::setAxisMaxAcceleration_mm_s2(value, ExtUI::axis_t::Y); break;
    case VP_Z_MAX_ACC: ExtUI::setAxisMaxAcceleration_mm_s2(value, ExtUI::axis_t::Z); break;
    #if HAS_HOTEND
      case VP_E0_MAX_ACC: ExtUI::setAxisMaxAcceleration_mm_s2(value, ExtUI::extruder_t::E0); break;
    #endif
    #if HAS_MULTI_HOTEND
      case VP_E1_MAX_ACC: ExtUI::setAxisMaxAcceleration_mm_s2(value, ExtUI::extruder_t::E1); break;
    #endif
  }
  settings.save();
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::handleTravelAccChange(DGUS_VP_Variable &var, void *val_ptr) {
  planner.settings.travel_acceleration = (float)BE16_P(val_ptr);
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::handleFeedRateMinChange(DGUS_VP_Variable &var, void *val_ptr) {
  planner.settings.min_feedrate_mm_s = (float)BE16_P(val_ptr);
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::handleMin_T_F(DGUS_VP_Variable &var, void *val_ptr) {
  planner.settings.min_travel_feedrate_mm_s = (float)BE16_P(val_ptr);
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::handleAccChange(DGUS_VP_Variable &var, void *val_ptr) {
  planner.settings.acceleration = (float)BE16_P(val_ptr);
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

#if ENABLED(PREVENT_COLD_EXTRUSION)
  void DGUSScreenHandlerMKS::handleGetExMinTemp(DGUS_VP_Variable &var, void *val_ptr) {
    thermalManager.extrude_min_temp = BE16_P(val_ptr);
    skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
  }
#endif

#if HAS_PID_HEATING

  void DGUSScreenHandler::handleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr) {
    const float value = (float)BE16_P(val_ptr);
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

    settings.save();
    skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
  }

#endif // HAS_PID_HEATING

#if ENABLED(BABYSTEPPING)

  void DGUSScreenHandler::handleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr) {
    switch (BE16_P(val_ptr)) {
      case 0:
        queue.inject(TS(F("M290 Z"), -ZOffset_distance));
        z_offset_add -= ZOffset_distance;
        break;

      case 1:
        queue.inject(TS(F("M290 Z"), ZOffset_distance));
        z_offset_add += ZOffset_distance;
        break;

      default: break;
    }
    forceCompleteUpdate();
  }

#endif // BABYSTEPPING

void DGUSScreenHandlerMKS::getManualFilament(DGUS_VP_Variable &var, void *val_ptr) {
  distanceFilament = (float)BE16_P(val_ptr);
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::getManualFilamentSpeed(DGUS_VP_Variable &var, void *val_ptr) {
  filamentSpeed_mm_s = BE16_P(val_ptr);
  skipVP = var.VP; // Don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerMKS::filamentLoadUnload(DGUS_VP_Variable &var, void *val_ptr, const int filamentDir) {
  #if ANY(HAS_MULTI_HOTEND, SINGLENOZZLE)
    uint8_t swap_tool = 0;
  #else
    constexpr uint8_t swap_tool = 1; // T0 (or none at all)
  #endif

  #if HAS_HOTEND
    uint8_t hotend_too_cold = 0;
  #endif

  if (!print_job_timer.isPaused() && !queue.ring_buffer.empty())
    return;

  const uint16_t val_t = BE16_P(val_ptr);
  switch (val_t) {
    default: break;
    case 0:
      #if HAS_HOTEND
        if (thermalManager.tooColdToExtrude(0))
          hotend_too_cold = 1;
        else {
          #if ANY(HAS_MULTI_HOTEND, SINGLENOZZLE)
            swap_tool = 1;
          #endif
        }
      #endif
      break;
    case 1:
      #if HAS_MULTI_HOTEND
        if (thermalManager.tooColdToExtrude(1)) hotend_too_cold = 2; else swap_tool = 2;
      #elif ENABLED(SINGLENOZZLE)
        if (thermalManager.tooColdToExtrude(0)) hotend_too_cold = 1; else swap_tool = 2;
      #endif
      break;
  }

  #if ALL(HAS_HOTEND, PREVENT_COLD_EXTRUSION)
    if (hotend_too_cold) {
      if (thermalManager.targetTooColdToExtrude(hotend_too_cold - 1))
        thermalManager.setTargetHotend(thermalManager.extrude_min_temp, hotend_too_cold - 1);
      sendInfoScreenMKS(F("NOTICE"), nullptr, F("Please wait."), F("Nozzle heating!"), MKS_English);
      setupConfirmAction(nullptr);
      gotoScreen(DGUS_SCREEN_POPUP);
    }
  #endif

  if (swap_tool) {
    char buf[30]; // TODO: Use MString / TS()
    snprintf_P(buf, 30,
      #if ANY(HAS_MULTI_HOTEND, SINGLENOZZLE)
        PSTR("M1002T%cE%dF%d"), char('0' + swap_tool - 1)
      #else
        PSTR("M1002E%dF%d")
      #endif
      , (int)distanceFilament * filamentDir, filamentSpeed_mm_s * 60
    );
    queue.inject(buf);
  }
}

/**
 * M1002: Do a tool-change and relative move for filamentLoadUnload
 *        within the G-code execution window for best concurrency.
 */
void GcodeSuite::M1002() {
  #if ANY(HAS_MULTI_HOTEND, SINGLENOZZLE)
  {
    char buf[3]; // TODO: Use MString / TS()
    sprintf_P(buf, PSTR("T%c"), char('0' + parser.intval('T')));
    process_subcommands_now(buf);
  }
  #endif

  const uint8_t old_axis_relative = axis_relative;
  set_e_relative(); // M83

  {
    char buf[20]; // TODO: Use MString / TS()
    snprintf_P(buf, 20, PSTR("G1E%dF%d"), parser.intval('E'), parser.intval('F'));
    process_subcommands_now(buf);
  }

  axis_relative = old_axis_relative;
}

void DGUSScreenHandlerMKS::filamentLoad(DGUS_VP_Variable &var, void *val_ptr) {
  filamentLoadUnload(var, val_ptr, 1);
}

void DGUSScreenHandlerMKS::filamentUnload(DGUS_VP_Variable &var, void *val_ptr) {
  filamentLoadUnload(var, val_ptr, -1);
}

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

  void DGUSScreenHandler::handleFilamentOption(DGUS_VP_Variable &var, void *val_ptr) {
    filament_data.heated = false;
    uint16_t preheat_option = BE16_P(val_ptr);
    if (preheat_option >= 10) {     // Unload filament type
      preheat_option -= 10;
      filament_data.action = 2;
      filament_data.purge_length = DGUS_FILAMENT_PURGE_LENGTH;
    }
    else if (preheat_option <= 8)   // Load filament type
      filament_data.action = 1;
    else                            // Cancel filament operation
      filament_data.action = 0;

    uint8_t e_temp = 0;
    switch (preheat_option) {
      case 0: // Load PLA
        #ifdef PREHEAT_1_TEMP_HOTEND
          e_temp = PREHEAT_1_TEMP_HOTEND;
        #endif
        break;
      case 1: // Load ABS
        #ifdef PREHEAT_2_TEMP_HOTEND
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
        e_temp = 0; break;
    }

    if (filament_data.action == 0) { // Go back to utility screen
      TERN_(HAS_EXTRUDERS, thermalManager.setTargetHotend(e_temp, 0));
      TERN_(HAS_MULTI_EXTRUDER, thermalManager.setTargetHotend(e_temp, 1));
      gotoScreen(DGUS_SCREEN_UTILITY);
      return;
    }

    // Go to the preheat screen to show the heating progress
    switch (var.VP) {
      default: return;
      #if HAS_EXTRUDERS
        case VP_E0_FILAMENT_LOAD_UNLOAD:
          filament_data.extruder = 0;
          thermalManager.setTargetHotend(e_temp, filament_data.extruder);
          break;
      #endif
      #if HAS_MULTI_EXTRUDER
        case VP_E1_FILAMENT_LOAD_UNLOAD:
          filament_data.extruder = 1;
          thermalManager.setTargetHotend(e_temp, filament_data.extruder);
          break;
      #endif
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
          filament_data.heated = true;
        }
        movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder ? ExtUI::extruder_t::E1 : ExtUI::extruder_t::E0) + movevalue;
      }
      else { // unload filament
        if (!filament_data.heated) {
          gotoScreen(DGUS_SCREEN_FILAMENT_UNLOADING);
          filament_data.heated = true;
        }
        // Before unloading extrude to prevent jamming
        if (filament_data.purge_length >= 0) {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder ? ExtUI::extruder_t::E1 : ExtUI::extruder_t::E0) + movevalue;
          filament_data.purge_length -= movevalue;
        }
        else {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder ? ExtUI::extruder_t::E1 : ExtUI::extruder_t::E0) - movevalue;
        }
      }
      ExtUI::setAxisPosition_mm(movevalue, filament_data.extruder ? ExtUI::extruder_t::E1 : ExtUI::extruder_t::E0);
    }
  }

#endif // DGUS_FILAMENT_LOADUNLOAD

bool DGUSScreenHandlerMKS::loop() {
  dgus.loop();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  static uint8_t language_times = 2;

  if (!isScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    updateScreenVPData();
  }

  if (language_times != 0) {
    languagePInit();
    updateDisplayLanguage();
    language_times--;
  }

  #if ENABLED(SHOW_BOOTSCREEN)

    static bool booted = false;
    if (!booted && ELAPSED(ms, TERN(USE_MKS_GREEN_UI, 1000, BOOTSCREEN_TIMEOUT))) {
      booted = true;
      #if USE_SENSORLESS
        TERN_(X_HAS_STEALTHCHOP, tmc_stall_sens.x = stepperX.homing_threshold());
        TERN_(Y_HAS_STEALTHCHOP, tmc_stall_sens.y = stepperY.homing_threshold());
        TERN_(Z_HAS_STEALTHCHOP, tmc_stall_sens.z = stepperZ.homing_threshold());
      #endif

      #if ENABLED(PREVENT_COLD_EXTRUSION)
        if (mks_min_extrusion_temp != 0)
          thermalManager.extrude_min_temp = mks_min_extrusion_temp;
      #endif

      extrudeLoadInit();

      TERN_(DGUS_MKS_RUNOUT_SENSOR, runoutInit());

      if (TERN0(POWER_LOSS_RECOVERY, recovery.valid()))
        gotoScreen(DGUS_SCREEN_POWER_LOSS);
      else
        gotoScreen(DGUS_SCREEN_MAIN);
    }

    #if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
      if (booted && printingIsActive()) runoutIdle();
    #endif

  #endif // SHOW_BOOTSCREEN

  return isScreenComplete();
}

void DGUSScreenHandlerMKS::languagePInit() {
  dgus.writeVariable(VP_LANGUAGE_CHANGE1, (uint8_t)(mks_language_index == MKS_English ? MKS_Language_NoChoose : MKS_Language_Choose));
  dgus.writeVariable(VP_LANGUAGE_CHANGE2, (uint8_t)(mks_language_index == MKS_English ? MKS_Language_Choose : MKS_Language_NoChoose));
}

void DGUSScreenHandlerMKS::extrudeLoadInit() {
  ex_filament.ex_length           = distanceFilament;
  ex_filament.ex_load_unload_flag = 0;
  ex_filament.ex_need_time        = filamentSpeed_mm_s;
  ex_filament.ex_speed            = 0;
  ex_filament.ex_status           = EX_NONE;
  ex_filament.ex_tick_end         = 0;
  ex_filament.ex_tick_start       = 0;
}

void DGUSScreenHandlerMKS::runoutInit() {
  #if ENABLED(DGUS_MKS_RUNOUT_SENSOR) && PIN_EXISTS(FIL_RUNOUT)
    SET_INPUT_PULLUP(FIL_RUNOUT_PIN);
  #endif
  runout_mks.de_count      = 0;
  runout_mks.de_times      = 10;
  runout_mks.pin_status    = 1;
  runout_mks.runout_status = UNRUNOUT_STATUS;
}

void DGUSScreenHandlerMKS::runoutIdle() {
  #if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
    // scanf runout pin
    switch (runout_mks.runout_status) {

      case RUNOUT_STATUS:
        runout_mks.runout_status = RUNOUT_BEGIN_STATUS;
        queue.inject(F("M25"));
        gotoScreen(MKSLCD_SCREEN_PAUSE);

        sendInfoScreenMKS(F("NOTICE"), nullptr, F("Please change filament!"), nullptr, MKS_English);
        //setupConfirmAction(nullptr);
        gotoScreen(DGUS_SCREEN_POPUP);
        break;

      case UNRUNOUT_STATUS:
        if (FILAMENT_IS_OUT()) runout_mks.runout_status = RUNOUT_STATUS;
        break;

      case RUNOUT_BEGIN_STATUS:
        if (!FILAMENT_IS_OUT()) runout_mks.runout_status = RUNOUT_WAITING_STATUS;
        break;

      case RUNOUT_WAITING_STATUS:
        if (FILAMENT_IS_OUT()) runout_mks.runout_status = RUNOUT_BEGIN_STATUS;
        break;

      default: break;
    }
  #endif // DGUS_MKS_RUNOUT_SENSOR
}

void DGUSScreenHandlerMKS::updateDisplayLanguage() {
  switch (mks_language_index) {
    case MKS_English : {
      const char home_buf_en[] = "Home";
      dgus.writeStringVar(VP_HOME_Dis, home_buf_en);

      const char setting_buf_en[] = "Settings";
      dgus.writeStringVar(VP_Setting_Dis, setting_buf_en);

      const char Tool_buf_en[] = "Tools";
      dgus.writeStringVar(VP_Tool_Dis, Tool_buf_en);

      const char Print_buf_en[] = "Print";
      dgus.writeStringVar(VP_Print_Dis, Print_buf_en);

      const char Language_buf_en[] = "Language";
      dgus.writeStringVar(VP_Language_Dis, Language_buf_en);

      const char About_buf_en[] = "About";
      dgus.writeStringVar(VP_About_Dis, About_buf_en);

      const char Config_buf_en[] = "Config";
      dgus.writeStringVar(VP_Config_Dis, Config_buf_en);

      const char MotorConfig_buf_en[] = "Motion Config";
      dgus.writeStringVar(VP_MotorConfig_Dis, MotorConfig_buf_en);

      const char LevelConfig_buf_en[] = "Level Config";
      dgus.writeStringVar(VP_LevelConfig_Dis, LevelConfig_buf_en);

      const char TemperatureConfig_buf_en[] = "Temperature";
      dgus.writeStringVar(VP_TemperatureConfig_Dis, TemperatureConfig_buf_en);

      const char Probe_Offset_buf_en[] = "Probe Offset";
      dgus.writeStringVar(VP_Probe_Offset_Dis, Probe_Offset_buf_en);

      const char Advance_buf_en[] = "Advanced";
      dgus.writeStringVar(VP_Advance_Dis, Advance_buf_en);

      const char Filament_buf_en[] = "Extrude";
      dgus.writeStringVar(VP_Filament_Dis, Filament_buf_en);

      const char Move_buf_en[] = "Move";
      dgus.writeStringVar(VP_Move_Dis, Move_buf_en);

      const char Level_buf_en[] =
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          "Auto"
        #elif ENABLED(MESH_BED_LEVELING)
          "Mesh"
        #endif
        "Level"
      ;
      dgus.writeStringVar(VP_Level_Dis, Level_buf_en);

      const char AxisRes_buf_en[] = "Axis Resolution";
      dgus.writeStringVar(VP_AxisRes_Dis, AxisRes_buf_en);

      const char AxisMaxSpeed_buf_en[] = "Axis Max Speed";
      dgus.writeStringVar(VP_AxisMaxSpeed_Dis, AxisMaxSpeed_buf_en);

      const char AxisMaxAcc_buf_en[] = "Axis Max Acc.";
      dgus.writeStringVar(VP_AxisMaxAcc_Dis, AxisMaxAcc_buf_en);

      const char TravelAcc_buf_en[] = "Travel Acc.";
      dgus.writeStringVar(VP_TravelAcc_Dis, TravelAcc_buf_en);

      const char FeedRateMin_buf_en[] = "Min FeedRate";
      dgus.writeStringVar(VP_FeedRateMin_Dis, FeedRateMin_buf_en);

      const char TravelFeeRateMin_buf_en[] = "Travel Min FeedRate";
      dgus.writeStringVar(VP_TravelFeeRateMin_Dis, TravelFeeRateMin_buf_en);

      const char Acc_buf_en[] = "Acceleration";
      dgus.writeStringVar(VP_ACC_Dis, Acc_buf_en);

      const char Point_One_buf_en[] = "Point 1";
      dgus.writeStringVar(VP_Point_One_Dis, Point_One_buf_en);

      const char Point_Two_buf_en[] = "Point 2";
      dgus.writeStringVar(VP_Point_Two_Dis, Point_Two_buf_en);

      const char Point_Three_buf_en[] = "Point 3";
      dgus.writeStringVar(VP_Point_Three_Dis, Point_Three_buf_en);

      const char Point_Four_buf_en[] = "Point 4";
      dgus.writeStringVar(VP_Point_Four_Dis, Point_Four_buf_en);

      const char Point_Five_buf_en[] = "Point 5";
      dgus.writeStringVar(VP_Point_Five_Dis, Point_Five_buf_en);

      const char Extrusion_buf_en[] = "Extrusion";
      dgus.writeStringVar(VP_Extrusion_Dis, Extrusion_buf_en);

      const char HeatBed_buf_en[] = "HeatBed";
      dgus.writeStringVar(VP_HeatBed_Dis, HeatBed_buf_en);

      const char FactoryDefaults_buf_en[] = "Factory Defaults";
      dgus.writeStringVar(VP_FactoryDefaults_Dis, FactoryDefaults_buf_en);

      const char StoreSetting_buf_en[] = "Store Settings";
      dgus.writeStringVar(VP_StoreSetting_Dis, StoreSetting_buf_en);

      const char PrintPauseConfig_buf_en[] = "PrintPause Config";
      dgus.writeStringVar(VP_PrintPauseConfig_Dis, PrintPauseConfig_buf_en);

      const char X_Steps_mm_buf_en[] = "X steps/mm";
      dgus.writeStringVar(VP_X_Steps_mm_Dis, X_Steps_mm_buf_en);

      const char Y_Steps_mm_buf_en[] = "Y steps/mm";
      dgus.writeStringVar(VP_Y_Steps_mm_Dis, Y_Steps_mm_buf_en);

      const char Z_Steps_mm_buf_en[] = "Z steps/mm";
      dgus.writeStringVar(VP_Z_Steps_mm_Dis, Z_Steps_mm_buf_en);

      const char E0_Steps_mm_buf_en[] = "E0 steps/mm";
      dgus.writeStringVar(VP_E0_Steps_mm_Dis, E0_Steps_mm_buf_en);

      const char E1_Steps_mm_buf_en[] = "E1 steps/mm";
      dgus.writeStringVar(VP_E1_Steps_mm_Dis, E1_Steps_mm_buf_en);

      const char X_Max_Speed_buf_en[] = "X Max Speed";
      dgus.writeStringVar(VP_X_Max_Speed_Dis, X_Max_Speed_buf_en);

      const char Y_Max_Speed_buf_en[] = "Y Max Speed";
      dgus.writeStringVar(VP_Y_Max_Speed_Dis, Y_Max_Speed_buf_en);

      const char Z_Max_Speed_buf_en[] = "Z Max Speed";
      dgus.writeStringVar(VP_Z_Max_Speed_Dis, Z_Max_Speed_buf_en);

      const char E0_Max_Speed_buf_en[] = "E0 Max Speed";
      dgus.writeStringVar(VP_E0_Max_Speed_Dis, E0_Max_Speed_buf_en);

      const char E1_Max_Speed_buf_en[] = "E1 Max Speed";
      dgus.writeStringVar(VP_E1_Max_Speed_Dis, E1_Max_Speed_buf_en);

      const char X_Max_Acc_Speed_buf_en[] = "X Max Acc";
      dgus.writeStringVar(VP_X_Max_Acc_Dis, X_Max_Acc_Speed_buf_en);

      const char Y_Max_Acc_Speed_buf_en[] = "Y Max Acc";
      dgus.writeStringVar(VP_Y_Max_Acc_Dis, Y_Max_Acc_Speed_buf_en);

      const char Z_Max_Acc_Speed_buf_en[] = "Z Max Acc";
      dgus.writeStringVar(VP_Z_Max_Acc_Dis, Z_Max_Acc_Speed_buf_en);

      const char E0_Max_Acc_Speed_buf_en[] = "E0 Max Acc";
      dgus.writeStringVar(VP_E0_Max_Acc_Dis, E0_Max_Acc_Speed_buf_en);

      const char E1_Max_Acc_Speed_buf_en[] = "E1 Max Acc";
      dgus.writeStringVar(VP_E1_Max_Acc_Dis, E1_Max_Acc_Speed_buf_en);

      const char X_PARK_POS_buf_en[] = "X Park Pos";
      dgus.writeStringVar(VP_X_PARK_POS_Dis, X_PARK_POS_buf_en);

      const char Y_PARK_POS_buf_en[] = "Y Park Pos";
      dgus.writeStringVar(VP_Y_PARK_POS_Dis, Y_PARK_POS_buf_en);

      const char Z_PARK_POS_buf_en[] = "Z Park Pos";
      dgus.writeStringVar(VP_Z_PARK_POS_Dis, Z_PARK_POS_buf_en);

      const char Length_buf_en[] = "Length";
      dgus.writeStringVar(VP_Length_Dis, Length_buf_en);

      const char Speed_buf_en[] = "Speed";
      dgus.writeStringVar(VP_Speed_Dis, Speed_buf_en);

      const char InOut_buf_en[] = "In/Out";
      dgus.writeStringVar(VP_InOut_Dis, InOut_buf_en);

      const char PrintTimet_buf_en[] = "Print Time";
      dgus.writeStringVar(VP_PrintTime_Dis, PrintTimet_buf_en);

      const char E0_Temp_buf_en[] = "E0 Temp";
      dgus.writeStringVar(VP_E0_Temp_Dis, E0_Temp_buf_en);

      const char E1_Temp_buf_en[] = "E1 Temp";
      dgus.writeStringVar(VP_E1_Temp_Dis, E1_Temp_buf_en);

      const char HB_Temp_buf_en[] = "HB Temp";
      dgus.writeStringVar(VP_HB_Temp_Dis, HB_Temp_buf_en);

      const char Feedrate_buf_en[] = "Feedrate";
      dgus.writeStringVar(VP_Feedrate_Dis, Feedrate_buf_en);

      const char PrintAcc_buf_en[] = "Print Speed";
      dgus.writeStringVar(VP_PrintAcc_Dis, PrintAcc_buf_en);

      const char FAN_Speed_buf_en[] = "FAN Speed";
      dgus.writeStringVar(VP_Fan_Speed_Dis, FAN_Speed_buf_en);

      const char Printing_buf_en[] = "Printing";
      dgus.writeStringVar(VP_Printing_Dis, Printing_buf_en);

      const char Info_EEPROM_1_buf_en[] = "Store Settings?";
      dgus.writeStringVar(VP_Info_EEPROM_1_Dis, Info_EEPROM_1_buf_en);

      const char Info_EEPROM_2_buf_en[] = "Revert Settings?";
      dgus.writeStringVar(VP_Info_EEPROM_2_Dis, Info_EEPROM_2_buf_en);

      const char Info_PrintFinish_1_buf_en[] = "Print Done";
      dgus.writeStringVar(VP_Info_PrintFinish_1_Dis, Info_PrintFinish_1_buf_en);

      const char TMC_X_Step_buf_en[] = "X Sensitivity";
      dgus.writeStringVar(VP_TMC_X_SENS_Dis, TMC_X_Step_buf_en);

      const char TMC_Y_Step_buf_en[] = "Y Sensitivity";
      dgus.writeStringVar(VP_TMC_Y_SENS_Dis, TMC_Y_Step_buf_en);

      const char TMC_Z_Step_buf_en[] = "Z Sensitivity";
      dgus.writeStringVar(VP_TMC_Z_SENS_Dis, TMC_Z_Step_buf_en);

      const char TMC_X_Current_buf_en[] = "X Current";
      dgus.writeStringVar(VP_TMC_X_Current_Dis, TMC_X_Current_buf_en);

      const char TMC_Y_Current_buf_en[] = "Y Current";
      dgus.writeStringVar(VP_TMC_Y_Current_Dis, TMC_Y_Current_buf_en);

      const char TMC_Z_Current_buf_en[] = "Z Current";
      dgus.writeStringVar(VP_TMC_Z_Current_Dis, TMC_Z_Current_buf_en);

      const char TMC_E0_Current_buf_en[] = "E0 Current";
      dgus.writeStringVar(VP_TMC_E0_Current_Dis, TMC_E0_Current_buf_en);

      const char TMC_X1_Current_buf_en[] = "X2 Current";
      dgus.writeStringVar(VP_TMC_X1_Current_Dis, TMC_X1_Current_buf_en);

      const char TMC_Y1_Current_buf_en[] = "Y2 Current";
      dgus.writeStringVar(VP_TMC_Y1_Current_Dis, TMC_Y1_Current_buf_en);

      const char TMC_Z1_Current_buf_en[] = "Z2 Current";
      dgus.writeStringVar(VP_TMC_Z1_Current_Dis, TMC_Z1_Current_buf_en);

      const char TMC_E1_Current_buf_en[] = "E1 Current";
      dgus.writeStringVar(VP_TMC_E1_Current_Dis, TMC_E1_Current_buf_en);

      const char Min_Ex_Temp_buf_en[] = "Min Extrude Temp";
      dgus.writeStringVar(VP_Min_Ex_Temp_Dis, Min_Ex_Temp_buf_en);

      const char X_Offset_buf_en[] = "X Offset";
      dgus.writeStringVar(VP_X_Offset_Dis, X_Offset_buf_en);

      const char Y_Offset_buf_en[] = "Y Offset";
      dgus.writeStringVar(VP_Y_Offset_Dis, Y_Offset_buf_en);

      const char Z_Offset_buf_en[] = "Z Offset";
      dgus.writeStringVar(VP_Z_Offset_Dis, Z_Offset_buf_en);

      const char AutoLEVEL_INFO1_buf_en[] = "Please Press Button!";
      dgus.writeStringVar(VP_AutoLEVEL_INFO1, AutoLEVEL_INFO1_buf_en);

      const char EX_TEMP_INFO2_buf_en[] = "Please wait a moment";
      dgus.writeStringVar(VP_EX_TEMP_INFO2_Dis, EX_TEMP_INFO2_buf_en);

      const char EX_TEMP_INFO3_buf_en[] = "Cancel";
      dgus.writeStringVar(VP_EX_TEMP_INFO3_Dis, EX_TEMP_INFO3_buf_en);

      const char PrintConfirm_Info_buf_en[] = "Start Print?";
      dgus.writeStringVar(VP_PrintConfirm_Info_Dis, PrintConfirm_Info_buf_en);

      const char StopPrintConfirm_Info_buf_en[] = "Stop Print?";
      dgus.writeStringVar(VP_StopPrintConfirm_Info_Dis, StopPrintConfirm_Info_buf_en);

      const char LCD_BLK_buf_en[] = "Backlight";
      dgus.writeStringVar(VP_LCD_BLK_Dis, LCD_BLK_buf_en);

    } break; // MKS_English

    case MKS_SimpleChinese: {
      const uint16_t home_buf_ch[] = { 0xF7D6, 0xB3D2 };
      dgus.writeStringVar(VP_HOME_Dis, home_buf_ch, 4);

      const uint16_t Setting_Dis[] = { 0xE8C9, 0xC3D6, 0x2000, 0x2000, 0x2000 };
      dgus.writeStringVar(VP_Setting_Dis, Setting_Dis, 7);

      const uint16_t Tool_Dis[] = { 0xA4B9, 0xDFBE };
      dgus.writeStringVar(VP_Tool_Dis, Tool_Dis, 4);

      const uint16_t Print_buf_ch[] = { 0xF2B4, 0xA1D3, 0x2000 };
      dgus.writeStringVar(VP_Print_Dis, Print_buf_ch, 6);

      const uint16_t Language_buf_ch[] = { 0xEFD3, 0xD4D1, 0x2000, 0x2000 };
      dgus.writeStringVar(VP_Language_Dis, Language_buf_ch, 8);

      const uint16_t About_buf_ch[] = { 0xD8B9, 0xDAD3, 0x2000 };
      dgus.writeStringVar(VP_About_Dis, About_buf_ch, 6);

      const uint16_t Config_buf_ch[] = { 0xE4C5, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_Config_Dis, Config_buf_ch, 6);

      const uint16_t MotorConfig_buf_ch[] = { 0xE7B5, 0xFABB, 0xE4C5, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_MotorConfig_Dis, MotorConfig_buf_ch, 12);

      const uint16_t LevelConfig_buf_ch[] = { 0xD6CA, 0xAFB6, 0xF7B5, 0xBDC6, 0xE8C9, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_LevelConfig_Dis, LevelConfig_buf_ch, 32);

      const uint16_t TemperatureConfig_buf_ch[] = { 0xC2CE, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_TemperatureConfig_Dis, TemperatureConfig_buf_ch, 11);

      const uint16_t Advance_buf_ch[] = { 0xDFB8, 0xB6BC, 0xE8C9, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_Advance_Dis, Advance_buf_ch, 32);

      const uint16_t Filament_buf_ch[] = { 0xB7BC, 0xF6B3, 0x2000 };
      dgus.writeStringVar(VP_Filament_Dis, Filament_buf_ch, 8);

      const uint16_t Move_buf_ch[] = { 0xC6D2, 0xAFB6, 0x2000  };
      dgus.writeStringVar(VP_Move_Dis, Move_buf_ch, 4);

      const uint16_t Level_buf_ch[] = {
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          0xD4D7, 0xAFB6
        #elif ENABLED(MESH_BED_LEVELING)
          0xF8CD, 0xF1B8
        #else
          0xD6CA, 0xAFB6
        #endif
        , 0xF7B5, 0xBDC6, 0x2000
      };
      dgus.writeStringVar(VP_Level_Dis, Level_buf_ch, 32);

      const uint16_t AxisRes_buf_ch[] = { 0xF6C2, 0xE5B3, 0x2000 };
      dgus.writeStringVar(VP_AxisRes_Dis, AxisRes_buf_ch);

      const uint16_t AxisMaxSpeed_buf_ch[] = { 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_AxisMaxSpeed_Dis, AxisMaxSpeed_buf_ch);

      const uint16_t AxisMaxAcc_buf_ch[] = { 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_AxisMaxAcc_Dis, AxisMaxAcc_buf_ch);

      const uint16_t TravelAcc_buf_ch[] = { 0xD5BF, 0xD0D0, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_TravelAcc_Dis, TravelAcc_buf_ch);

      const uint16_t FeedRateMin_buf_ch[] = { 0xEED7, 0xA1D0, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_FeedRateMin_Dis, FeedRateMin_buf_ch, 12);

      const uint16_t TravelFeeRateMin_buf_ch[] = { 0xD5BF, 0xD0D0, 0xEED7, 0xA1D0, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_TravelFeeRateMin_Dis, TravelFeeRateMin_buf_ch, 24);

      const uint16_t Acc_buf_ch[] = { 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_ACC_Dis, Acc_buf_ch);

      const uint16_t Point_One_buf_ch[] = { 0xDAB5, 0xBBD2, 0xE3B5, 0x2000 };
      dgus.writeStringVar(VP_Point_One_Dis, Point_One_buf_ch, 12);

      const uint16_t Point_Two_buf_ch[] = { 0xDAB5, 0xFEB6, 0xE3B5, 0x2000 };
      dgus.writeStringVar(VP_Point_Two_Dis, Point_Two_buf_ch, 12);

      const uint16_t Point_Three_buf_ch[] = { 0xDAB5, 0xFDC8, 0xE3B5, 0x2000 };
      dgus.writeStringVar(VP_Point_Three_Dis, Point_Three_buf_ch, 12);

      const uint16_t Point_Four_buf_ch[] = { 0xDAB5, 0xC4CB, 0xE3B5, 0x2000 };
      dgus.writeStringVar(VP_Point_Four_Dis, Point_Four_buf_ch, 12);

      const uint16_t Point_Five_buf_ch[] = { 0xDAB5, 0xE5CE, 0xE3B5, 0x2000 };
      dgus.writeStringVar(VP_Point_Five_Dis, Point_Five_buf_ch, 12);

      const uint16_t Extrusion_buf_ch[] = { 0xB7BC, 0xF6B3, 0xB7CD, 0x2000 };
      dgus.writeStringVar(VP_Extrusion_Dis, Extrusion_buf_ch, 12);

      const uint16_t HeatBed_buf_ch[] = { 0xC8C8, 0xB2B4, 0x2000 };
      dgus.writeStringVar(VP_HeatBed_Dis, HeatBed_buf_ch, 12);

      const uint16_t FactoryDefaults_buf_ch[] = { 0xD6BB, 0xB4B8, 0xF6B3, 0xA7B3, 0xE8C9, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_FactoryDefaults_Dis, FactoryDefaults_buf_ch);

      const uint16_t StoreSetting_buf_ch[] = { 0xA3B1, 0xE6B4, 0xE8C9, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_StoreSetting_Dis, StoreSetting_buf_ch);

      const uint16_t PrintPauseConfig_buf_ch[] = { 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_PrintPauseConfig_Dis, PrintPauseConfig_buf_ch, 32);

      const uint16_t X_Steps_mm_buf_ch[] = { 0x2058, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
      dgus.writeStringVar(VP_X_Steps_mm_Dis, X_Steps_mm_buf_ch);

      const uint16_t Y_Steps_mm_buf_ch[] = { 0x2059, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
      dgus.writeStringVar(VP_Y_Steps_mm_Dis, Y_Steps_mm_buf_ch);

      const uint16_t Z_Steps_mm_buf_ch[] = { 0x205A, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
      dgus.writeStringVar(VP_Z_Steps_mm_Dis, Z_Steps_mm_buf_ch);

      const uint16_t E0_Steps_mm_buf_ch[] = { 0x3045, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
      dgus.writeStringVar(VP_E0_Steps_mm_Dis, E0_Steps_mm_buf_ch);

      const uint16_t E1_Steps_mm_buf_ch[] = { 0x3145, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
      dgus.writeStringVar(VP_E1_Steps_mm_Dis, E1_Steps_mm_buf_ch);

      const uint16_t X_Max_Speed_buf_ch[] = { 0x2058, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_X_Max_Speed_Dis, X_Max_Speed_buf_ch);

      const uint16_t Y_Max_Speed_buf_ch[] = { 0x2059, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Y_Max_Speed_Dis, Y_Max_Speed_buf_ch);

      const uint16_t Z_Max_Speed_buf_ch[] = { 0x205A, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Z_Max_Speed_Dis, Z_Max_Speed_buf_ch);

      const uint16_t E0_Max_Speed_buf_ch[] = { 0x3045, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_E0_Max_Speed_Dis, E0_Max_Speed_buf_ch);

      const uint16_t E1_Max_Speed_buf_ch[] = { 0x3145, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_E1_Max_Speed_Dis, E1_Max_Speed_buf_ch);

      const uint16_t X_Max_Acc_Speed_buf_ch[] = { 0x2058, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_X_Max_Acc_Dis, X_Max_Acc_Speed_buf_ch);

      const uint16_t Y_Max_Acc_Speed_buf_ch[] = { 0x2059, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Y_Max_Acc_Dis, Y_Max_Acc_Speed_buf_ch);

      const uint16_t Z_Max_Acc_Speed_buf_ch[] = { 0x205A, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Z_Max_Acc_Dis, Z_Max_Acc_Speed_buf_ch);

      const uint16_t E0_Max_Acc_Speed_buf_ch[] = { 0x3045, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_E0_Max_Acc_Dis, E0_Max_Acc_Speed_buf_ch);

      const uint16_t E1_Max_Acc_Speed_buf_ch[] = { 0x3145, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_E1_Max_Acc_Dis, E1_Max_Acc_Speed_buf_ch);

      const uint16_t X_PARK_POS_buf_ch[] = { 0x2058, 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_X_PARK_POS_Dis, X_PARK_POS_buf_ch);

      const uint16_t Y_PARK_POS_buf_ch[] = { 0x2059, 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_Y_PARK_POS_Dis, Y_PARK_POS_buf_ch);

      const uint16_t Z_PARK_POS_buf_ch[] = { 0x205A, 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_Z_PARK_POS_Dis, Z_PARK_POS_buf_ch);

      const uint16_t Length_buf_ch[] = { 0xBDB2, 0xA4B3, 0x2000 };
      dgus.writeStringVar(VP_Length_Dis, Length_buf_ch, 8);

      const uint16_t Speed_buf_ch[] = { 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Speed_Dis, Speed_buf_ch, 8);

      const uint16_t InOut_buf_ch[] = { 0xF8BD, 0xF6B3, 0x2000 };
      dgus.writeStringVar(VP_InOut_Dis, InOut_buf_ch, 8);

      const uint16_t PrintTimet_buf_ch[] = { 0xF2B4, 0xA1D3, 0xB1CA, 0xE4BC, 0x2000 };
      dgus.writeStringVar(VP_PrintTime_Dis, PrintTimet_buf_ch);

      const uint16_t E0_Temp_buf_ch[] = { 0x3045, 0xC2CE, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_E0_Temp_Dis, E0_Temp_buf_ch);

      const uint16_t E1_Temp_buf_ch[] = { 0x3145, 0xC2CE, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_E1_Temp_Dis, E1_Temp_buf_ch);

      const uint16_t HB_Temp_buf_ch[] = { 0xC8C8, 0xB2B4, 0xC2CE, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_HB_Temp_Dis, HB_Temp_buf_ch);

      const uint16_t Feedrate_buf_ch[] = { 0xB7BC, 0xF6B3, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Feedrate_Dis, Feedrate_buf_ch);

      const uint16_t PrintAcc_buf_ch[] = { 0xF2B4, 0xA1D3, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_PrintAcc_Dis, PrintAcc_buf_ch);

      const uint16_t FAN_Speed_buf_ch[] = { 0xE7B7, 0xC8C9, 0xD9CB, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Fan_Speed_Dis, FAN_Speed_buf_ch);

      const uint16_t Printing_buf_ch[] = { 0xF2B4, 0xA1D3, 0xD0D6, 0x2000 };
      dgus.writeStringVar(VP_Printing_Dis, Printing_buf_ch);

      const uint16_t Info_EEPROM_1_buf_ch[] = { 0xC7CA, 0xF1B7, 0xA3B1, 0xE6B4, 0xE8C9, 0xC3D6, 0xBFA3, 0x2000 };
      dgus.writeStringVar(VP_Info_EEPROM_1_Dis, Info_EEPROM_1_buf_ch, 32);

      const uint16_t Info_EEPROM_2_buf_ch[] = { 0xC7CA, 0xF1B7, 0xD6BB, 0xB4B8, 0xF6B3, 0xA7B3, 0xE8C9, 0xC3D6, 0xBFA3, 0x2000 };
      dgus.writeStringVar(VP_Info_EEPROM_2_Dis, Info_EEPROM_2_buf_ch, 32);

      const uint16_t TMC_X_Step_buf_ch[] = { 0x2058, 0xE9C1, 0xF4C3, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_TMC_X_SENS_Dis, TMC_X_Step_buf_ch);

      const uint16_t TMC_Y_Step_buf_ch[] = { 0x2059, 0xE9C1, 0xF4C3, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_TMC_Y_SENS_Dis, TMC_Y_Step_buf_ch);

      const uint16_t TMC_Z_Step_buf_ch[] = { 0x205A, 0xE9C1, 0xF4C3, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_TMC_Z_SENS_Dis, TMC_Z_Step_buf_ch);

      const uint16_t Info_PrintFinish_1_buf_ch[] = { 0xF2B4, 0xA1D3, 0xEACD, 0xC9B3, 0x2000 };
      dgus.writeStringVar(VP_Info_PrintFinish_1_Dis, Info_PrintFinish_1_buf_ch, 32);

      const uint16_t TMC_X_Current_buf_ch[] = { 0x2058, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_X_Current_Dis, TMC_X_Current_buf_ch);

      const uint16_t TMC_Y_Current_buf_ch[] = { 0x2059, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_Y_Current_Dis, TMC_Y_Current_buf_ch);

      const uint16_t TMC_Z_Current_buf_ch[] = { 0x205A, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_Z_Current_Dis, TMC_Z_Current_buf_ch);

      const uint16_t TMC_E0_Current_buf_ch[] = { 0x3045, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_E0_Current_Dis, TMC_E0_Current_buf_ch);

      const uint16_t TMC_X1_Current_buf_ch[] = { 0x3158, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_X1_Current_Dis, TMC_X1_Current_buf_ch);

      const uint16_t TMC_Y1_Current_buf_ch[] = { 0x3159, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_Y1_Current_Dis, TMC_Y1_Current_buf_ch);

      const uint16_t TMC_Z1_Current_buf_ch[] = { 0x315A, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_Z1_Current_Dis, TMC_Z1_Current_buf_ch);

      const uint16_t TMC_E1_Current_buf_ch[] = { 0x3145, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
      dgus.writeStringVar(VP_TMC_E1_Current_Dis, TMC_E1_Current_buf_ch);

      const uint16_t Min_Ex_Temp_buf_ch[] = { 0xEED7, 0xA1D0, 0xB7BC, 0xF6B3, 0xC2CE, 0xC8B6, 0x2000 };
      dgus.writeStringVar(VP_Min_Ex_Temp_Dis, Min_Ex_Temp_buf_ch, 32);

      const uint16_t AutoLEVEL_INFO1_buf_ch[] = { 0xEBC7, 0xB4B0, 0xC2CF, 0xB4B0, 0xA5C5, 0x2000 };
      dgus.writeStringVar(VP_AutoLEVEL_INFO1, AutoLEVEL_INFO1_buf_ch, 32);

      const uint16_t EX_TEMP_INFO2_buf_ch[] = { 0xEBC7, 0xD4C9, 0xC8B5, 0x2000 };
      dgus.writeStringVar(VP_EX_TEMP_INFO2_Dis, EX_TEMP_INFO2_buf_ch, 32);

      const uint16_t EX_TEMP_INFO3_buf_ch[] = { 0xA1C8, 0xFBCF, 0xD3BC, 0xC8C8, 0x2000 };
      dgus.writeStringVar(VP_EX_TEMP_INFO3_Dis, EX_TEMP_INFO3_buf_ch, 32);

      const uint16_t PrintConfirm_Info_buf_ch[] = { 0xC7CA, 0xF1B7, 0xAABF, 0xBCCA, 0xF2B4, 0xA1D3, 0x2000 };
      dgus.writeStringVar(VP_PrintConfirm_Info_Dis, PrintConfirm_Info_buf_ch, 32);

      const uint16_t StopPrintConfirm_Info_buf_ch[] = { 0xC7CA, 0xF1B7, 0xA3CD, 0xB9D6, 0xF2B4, 0xA1D3, 0x2000 };
      dgus.writeStringVar(VP_StopPrintConfirm_Info_Dis, StopPrintConfirm_Info_buf_ch, 32);

      const uint16_t LCD_BLK_buf_ch[] = { 0xB3B1, 0xE2B9, 0xE8C9, 0xC3D6, 0x2000 };
      dgus.writeStringVar(VP_LCD_BLK_Dis, LCD_BLK_buf_ch, 32);

    } break; // MKS_SimpleChinese
  } // switch
}

#endif // DGUS_LCD_UI_MKS
