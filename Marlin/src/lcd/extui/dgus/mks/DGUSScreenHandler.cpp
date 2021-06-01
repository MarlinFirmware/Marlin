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

#if ENABLED(DGUS_LCD_UI_MKS)

#include "../DGUSScreenHandler.h"

#include "../../../../inc/MarlinConfig.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/settings.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../module/printcounter.h"

#include "../../../../gcode/gcode.h"

#if ENABLED(HAS_STEALTHCHOP)
  #include "../../../../module/stepper/trinamic.h"
  #include "../../../../module/stepper/indirection.h"
#endif
#include "../../../../module/probe.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(SDSUPPORT)
  static ExtUI::FileList filelist;
#endif

bool DGUSAutoTurnOff = false;
uint8_t mks_language_index; // Initialized by settings.load()

// endianness swap
uint32_t swap32(const uint32_t value) { return (value & 0x000000FFU) << 24U | (value & 0x0000FF00U) << 8U | (value & 0x00FF0000U) >> 8U | (value & 0xFF000000U) >> 24U; }

#if 0
void DGUSScreenHandler::sendinfoscreen_ch_mks(const uint16_t *line1, const uint16_t *line2, const uint16_t *line3, const uint16_t *line4) {
  dgusdisplay.WriteVariable(VP_MSGSTR1, line1, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR2, line2, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR3, line3, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR4, line4, 32, true);
}

void DGUSScreenHandler::sendinfoscreen_en_mks(const char *line1, const char *line2, const char *line3, const char *line4) {
  dgusdisplay.WriteVariable(VP_MSGSTR1, line1, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR2, line2, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR3, line3, 32, true);
  dgusdisplay.WriteVariable(VP_MSGSTR4, line4, 32, true);
}

void DGUSScreenHandler::sendinfoscreen_mks(const void *line1, const void *line2, const void *line3, const void *line4, uint16_t language) {
  if (language == MKS_English)
    DGUSScreenHandler::sendinfoscreen_en_mks((char *)line1, (char *)line2, (char *)line3, (char *)line4);
  else if (language == MKS_SimpleChinese)
    DGUSScreenHandler::sendinfoscreen_ch_mks((uint16_t *)line1, (uint16_t *)line2, (uint16_t *)line3, (uint16_t *)line4);
}

#endif

void DGUSScreenHandler::DGUSLCD_SendFanToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint16_t tmp = *(uint8_t *) var.memadr; // +1 -> avoid rounding issues for the display.
    // tmp = map(tmp, 0, 255, 0, 100);
    dgusdisplay.WriteVariable(var.VP, tmp);
  }
}

void DGUSScreenHandler::DGUSLCD_SendBabyStepToDisplay_MKS(DGUS_VP_Variable &var) {
  float value = current_position.z;
  DEBUG_ECHOLNPAIR_F(" >> ", value, 6);
  value *= cpow(10, 2);
  dgusdisplay.WriteVariable(VP_SD_Print_Baby, (uint16_t)value);
}

void DGUSScreenHandler::DGUSLCD_SendPrintTimeToDisplay_MKS(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();
  uint32_t time = elapsed.value;
  dgusdisplay.WriteVariable(VP_PrintTime_H, uint16_t(time / 3600));
  dgusdisplay.WriteVariable(VP_PrintTime_M, uint16_t(time % 3600 / 60));
  dgusdisplay.WriteVariable(VP_PrintTime_S, uint16_t((time % 3600) % 60));
}

void DGUSScreenHandler::DGUSLCD_SetUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    const uint16_t value = swap16(*(uint16_t*)val_ptr);
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

void DGUSScreenHandler::DGUSLCD_SendStringToDisplay_Language_MKS(DGUS_VP_Variable &var) {
  if (mks_language_index == MKS_English) {
    char *tmp = (char*) var.memadr;
    dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
  }
  else if (mks_language_index == MKS_SimpleChinese) {
    uint16_t *tmp = (uint16_t *)var.memadr;
    dgusdisplay.WriteVariable(var.VP, tmp, var.size, true);
  }
}

void DGUSScreenHandler::DGUSLCD_SendTMCStepValue(DGUS_VP_Variable &var) {
  #if ENABLED(SENSORLESS_HOMING)
    #if AXIS_HAS_STEALTHCHOP(X)
      tmc_step.x = stepperX.homing_threshold();
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      tmc_step.y = stepperY.homing_threshold();
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      tmc_step.z = stepperZ.homing_threshold();
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    #endif
  #endif
}

#if ENABLED(SDSUPPORT)

  void DGUSScreenHandler::DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)swap16(*(uint16_t*)val_ptr) + top_file;
    if (touched_nr != 0x0F && touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr) && touched_nr != 0x0F) return;

    if (touched_nr == 0x0F) {
      if (filelist.isAtRootDir())
        GotoScreen(DGUSLCD_SCREEN_MAIN);
      else
        filelist.upDir();
      return;
    }

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
    GotoScreen(MKSLCD_SCREEN_PRINT_CONFIRM);
  }

  void DGUSScreenHandler::DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    GotoScreen(MKSLCD_SCREEN_PRINT);
    z_offset_add = 0;
  }

  void DGUSScreenHandler::DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {

    if (!ExtUI::isPrintingFromMedia()) return; // avoid race condition when user stays in this menu and printer finishes.
    switch (swap16(*(uint16_t*)val_ptr)) {
      case 0: { // Resume

        auto cs = getCurrentScreen();
        if (runout_mks.runout_status != RUNOUT_WAITTING_STATUS && runout_mks.runout_status != UNRUNOUT_STATUS) {
          if (cs == MKSLCD_SCREEN_PRINT || cs == MKSLCD_SCREEN_PAUSE)
            GotoScreen(MKSLCD_SCREEN_PAUSE);
          return;
        }
        else
          runout_mks.runout_status = UNRUNOUT_STATUS;

        GotoScreen(MKSLCD_SCREEN_PRINT);

        if (ExtUI::isPrintingFromMediaPaused()) {
          nozzle_park_mks.print_pause_start_flag = 0;
          nozzle_park_mks.blstatus = true;
          ExtUI::resumePrint();
        }
      } break;

      case 1: // Pause

        GotoScreen(MKSLCD_SCREEN_PAUSE);
        if (!ExtUI::isPrintingFromMediaPaused()) {
          nozzle_park_mks.print_pause_start_flag = 1;
          nozzle_park_mks.blstatus = true;
          ExtUI::pausePrint();
          //ExtUI::mks_pausePrint();
        }
        break;
      case 2: // Abort
        HandleUserConfirmationPopUp(VP_SD_AbortPrintConfirmed, nullptr, PSTR("Abort printing"), filelist.filename(), PSTR("?"), true, true, false, true);
        break;
    }
  }

  void DGUSScreenHandler::DGUSLCD_SD_SendFilename(DGUS_VP_Variable& var) {
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line > DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;

    uint16_t dir_icon_val = 25;
    if (filelist.seek(top_file + target_line)) {
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0); // snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s"), filelist.filename());
      dir_icon_val = filelist.isDir() ? 0 : 1;
    }
    DGUSLCD_SendStringToDisplay(var);

    dgusdisplay.WriteVariable(VP_File_Pictutr0 + target_line * 2, dir_icon_val);
  }

  void DGUSScreenHandler::SDCardInserted() {
    top_file = 0;
    filelist.refresh();
    auto cs = getCurrentScreen();
    if (cs == DGUSLCD_SCREEN_MAIN || cs == DGUSLCD_SCREEN_STATUS)
      GotoScreen(MKSLCD_SCREEN_CHOOSE_FILE);
  }

  void DGUSScreenHandler::SDCardRemoved() {
    if (current_screen == DGUSLCD_SCREEN_SDFILELIST
        || (current_screen == DGUSLCD_SCREEN_CONFIRM && (ConfirmVP == VP_SD_AbortPrintConfirmed || ConfirmVP == VP_SD_FileSelectConfirm))
        || current_screen == DGUSLCD_SCREEN_SDPRINTMANIPULATION
    ) filelist.refresh();
  }

  void DGUSScreenHandler::SDPrintingFinished() {
    if (DGUSAutoTurnOff) {
      queue.exhaust();
      gcode.process_subcommands_now_P(PSTR("M81"));
    }
    GotoScreen(MKSLCD_SCREEN_PrintDone);
  }

#else
  void DGUSScreenHandler::PrintReturn(DGUS_VP_Variable& var, void *val_ptr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    if (value == 0x0F) GotoScreen(DGUSLCD_SCREEN_MAIN);
  }
#endif // SDSUPPORT

void DGUSScreenHandler::ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUSLCD_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUSLCD_Screens target = (DGUSLCD_Screens)tmp[1];

  DEBUG_ECHOLNPAIR("\n DEBUG target", target);

  // when the dgus had reboot, it will enter the DGUSLCD_SCREEN_MAIN page,
  // so user can change any page to use this function, an it will check
  // if robin nano is printing. when it is, dgus will enter the printing
  // page to continue print;
  //
  //if (printJobOngoing() || printingIsPaused()) {
  //  if (target == MKSLCD_PAUSE_SETTING_MOVE || target == MKSLCD_PAUSE_SETTING_EX
  //    || target == MKSLCD_SCREEN_PRINT || target == MKSLCD_SCREEN_PAUSE
  //  ) {
  //  }
  //  else
  //    GotoScreen(MKSLCD_SCREEN_PRINT);
  // return;
  //}

  if (target == DGUSLCD_SCREEN_POPUP) {
    SetupConfirmAction(ExtUI::setUserConfirmed);

    // Special handling for popup is to return to previous menu
    if (current_screen == DGUSLCD_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    PopToOldScreen();
    return;
  }

  UpdateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!DGUSLCD_FindScreenVPMapList(target)) DEBUG_ECHOLNPAIR("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandler::ScreenBackChange(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t target = swap16(*(uint16_t *)val_ptr);
  DEBUG_ECHOLNPAIR(" back = 0x%x", target);
  switch (target) {
  }
}

void DGUSScreenHandler::ZoffsetConfirm(DGUS_VP_Variable &var, void *val_ptr) {
  settings.save();
  if (printJobOngoing())
    GotoScreen(MKSLCD_SCREEN_PRINT);
  else if (print_job_timer.isPaused)
    GotoScreen(MKSLCD_SCREEN_PAUSE);
}

void DGUSScreenHandler::GetTurnOffCtrl(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("GetTurnOffCtrl\n");
  const uint16_t value = swap16(*(uint16_t *)val_ptr);
  switch (value) {
    case 0 ... 1: DGUSAutoTurnOff = (bool)value; break;
    default: break;
  }
}

void DGUSScreenHandler::GetMinExtrudeTemp(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("GetMinExtrudeTemp");
  const uint16_t value = swap16(*(uint16_t *)val_ptr);
  TERN_(PREVENT_COLD_EXTRUSION, thermalManager.extrude_min_temp = value);
  mks_min_extrusion_temp = value;
  settings.save();
}

void DGUSScreenHandler::GetZoffsetDistance(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("GetZoffsetDistance");
  const uint16_t value = swap16(*(uint16_t *)val_ptr);
  float val_distance = 0;
  switch (value) {
    case 0: val_distance = 0.01; break;
    case 1: val_distance = 0.1; break;
    case 2: val_distance = 0.5; break;
    case 3: val_distance = 1; break;
    default: val_distance = 0.01; break;
  }
  ZOffset_distance = val_distance;
}

void DGUSScreenHandler::GetManualMovestep(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("\nGetManualMovestep");
  *(uint16_t *)var.memadr = swap16(*(uint16_t *)val_ptr);
}

void DGUSScreenHandler::EEPROM_CTRL(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t eep_flag = swap16(*(uint16_t *)val_ptr);
  switch (eep_flag) {
    case 0:
      settings.save();
      settings.load(); // load eeprom data to check the data is right
      GotoScreen(MKSLCD_SCREEN_EEP_Config);
      break;

    case 1:
      settings.reset();
      GotoScreen(MKSLCD_SCREEN_EEP_Config);
      break;

    default: break;
  }
}

void DGUSScreenHandler::Z_offset_select(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t z_value = swap16(*(uint16_t *)val_ptr);
  switch (z_value) {
    case 0: Z_distance = 0.01; break;
    case 1: Z_distance = 0.1; break;
    case 2: Z_distance = 0.5; break;
    default: Z_distance = 1; break;
  }
}

void DGUSScreenHandler::GetOffsetValue(DGUS_VP_Variable &var, void *val_ptr) {

  #if ENABLED(HAS_BED_PROBE)
    int32_t value = swap32(*(int32_t *)val_ptr);
    float Offset = value / 100.0f;
    DEBUG_ECHOLNPAIR_F("\nget int6 offset >> ", value, 6);
  #endif

  switch (var.VP) {
    case VP_OFFSET_X: TERN_(HAS_BED_PROBE, probe.offset.x = Offset); break;
    case VP_OFFSET_Y: TERN_(HAS_BED_PROBE, probe.offset.y = Offset); break;
    case VP_OFFSET_Z: TERN_(HAS_BED_PROBE, probe.offset.z = Offset); break;
    default: break;
  }
  settings.save();
}

void DGUSScreenHandler::LanguageChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t lag_flag = swap16(*(uint16_t *)val_ptr);
  switch (lag_flag) {
    case MKS_SimpleChinese:
      DGUS_LanguageDisplay(MKS_SimpleChinese);
      mks_language_index = MKS_SimpleChinese;
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1, MKS_Language_Choose);
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2, MKS_Language_NoChoose);
      settings.save();
      break;
    case MKS_English:
      DGUS_LanguageDisplay(MKS_English);
      mks_language_index = MKS_English;
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1, MKS_Language_NoChoose);
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2, MKS_Language_Choose);
      settings.save();
      break;
    default: break;
  }
}

#if ENABLED(MESH_BED_LEVELING)
  uint8_t mesh_point_count = GRID_MAX_POINTS;
#endif

void DGUSScreenHandler::Level_Ctrl_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t lev_but = swap16(*(uint16_t *)val_ptr);
  #if ENABLED(MESH_BED_LEVELING)
    auto cs = getCurrentScreen();
  #endif
  switch (lev_but) {
    case 0:
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

        static uint8_t a_first_level = 1;
        if (a_first_level == 1) {
          a_first_level = 0;
          queue.enqueue_now_P(G28_STR);
        }
        queue.enqueue_now_P(PSTR("G29"));

      #elif ENABLED(MESH_BED_LEVELING)

        mesh_point_count = GRID_MAX_POINTS;

        if (mks_language_index == MKS_English) {
          const char level_buf_en[] = "Start Level";
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en, 32, true);
        }
        else if (mks_language_index == MKS_SimpleChinese) {
          const uint16_t level_buf_ch[] = {0xAABF, 0xBCCA, 0xF7B5, 0xBDC6, 0x2000};
          dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch, 32, true);
        }

        cs = getCurrentScreen();
        if (cs != MKSLCD_AUTO_LEVEL) GotoScreen(MKSLCD_AUTO_LEVEL);
      #else

        GotoScreen(MKSLCD_SCREEN_LEVEL);

      #endif
      break;

    case 1:
      soft_endstop._enabled = true;
      GotoScreen(MKSLCD_SCREEM_TOOL);
      break;

    default: break;
  }
}

void DGUSScreenHandler::MeshLevelDistanceConfig(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t mesh_dist = swap16(*(uint16_t *)val_ptr);
  switch (mesh_dist) {
    case 0: mesh_adj_distance = 0.01; break;
    case 1: mesh_adj_distance = 0.1; break;
    case 2: mesh_adj_distance = 1; break;
    default: mesh_adj_distance = 0.1; break;
  }
}

void DGUSScreenHandler::MeshLevel(DGUS_VP_Variable &var, void *val_ptr) {
  #if ENABLED(MESH_BED_LEVELING)
    const uint16_t mesh_value = swap16(*(uint16_t *)val_ptr);
    // static uint8_t a_first_level = 1;
    char cmd_buf[30];
    float offset = mesh_adj_distance;
    int16_t integer, Deci, Deci2;

    if (!queue.ring_buffer.empty()) return;

    switch (mesh_value) {
      case 0:
        offset = mesh_adj_distance;
        integer = offset; // get int
        Deci = (offset * 10);
        Deci = Deci % 10;
        Deci2 = offset * 100;
        Deci2 = Deci2 % 10;
        soft_endstop._enabled = false;
        queue.enqueue_now_P(PSTR("G91"));
        snprintf_P(cmd_buf, 30, PSTR("G1 Z%d.%d%d"), integer, Deci, Deci2);
        queue.enqueue_one_now(cmd_buf);
        queue.enqueue_now_P(PSTR("G90"));
        //soft_endstop._enabled = true;
        break;

      case 1:
        offset = mesh_adj_distance;
        integer = offset;       // get int
        Deci = (offset * 10);
        Deci = Deci % 10;
        Deci2 = offset * 100;
        Deci2 = Deci2 % 10;
        soft_endstop._enabled = false;
        queue.enqueue_now_P(PSTR("G91"));
        snprintf_P(cmd_buf, 30, PSTR("G1 Z-%d.%d%d"), integer, Deci, Deci2);
        queue.enqueue_one_now(cmd_buf);
        queue.enqueue_now_P(PSTR("G90"));
        break;

      case 2:
        if (mesh_point_count == GRID_MAX_POINTS) { // The first point

          queue.enqueue_now_P(PSTR("G28"));
          queue.enqueue_now_P(PSTR("G29S1"));
          mesh_point_count--;

          if (mks_language_index == MKS_English) {
            const char level_buf_en1[] = "Next Point";
            dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en1, 32, true);
          }
          else if (mks_language_index == MKS_SimpleChinese) {
            const uint16_t level_buf_ch1[] = {0xC2CF, 0xBBD2, 0xE3B5, 0x2000};
            dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch1, 32, true);
          }
        }
        else if (mesh_point_count > 1) {                              // 倒数第二个点
          queue.enqueue_now_P(PSTR("G29S2"));
          mesh_point_count--;
          if (mks_language_index == MKS_English) {
            const char level_buf_en2[] = "Next Point";
            dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en2, 32, true);
          }
          else if (mks_language_index == MKS_SimpleChinese) {
            const uint16_t level_buf_ch2[] = {0xC2CF, 0xBBD2, 0xE3B5, 0x2000};
            dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch2, 32, true);
          }
        }
        else if (mesh_point_count == 1) {
          queue.enqueue_now_P(PSTR("G29S2"));
          mesh_point_count--;
          if (mks_language_index == MKS_English) {
            const char level_buf_en2[] = "Level Finsh";
            dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_en2, 32, true);
          }
          else if (mks_language_index == MKS_SimpleChinese) {
            const uint16_t level_buf_ch2[] = {0xF7B5, 0xBDC6, 0xEACD, 0xC9B3, 0x2000};
            dgusdisplay.WriteVariable(VP_AutoLevel_1_Dis, level_buf_ch2, 32, true);
          }
          settings.save();
        }
        else if (mesh_point_count == 0) {

          mesh_point_count = GRID_MAX_POINTS;
          soft_endstop._enabled = true;
          settings.save();
          GotoScreen(MKSLCD_SCREEM_TOOL);
        }
        break;

      default:
        break;
    }
  #endif // MESH_BED_LEVELING
}

void DGUSScreenHandler::LCD_BLK_Adjust(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t lcd_value = swap16(*(uint16_t *)val_ptr);

  lcd_default_light = constrain(lcd_value, 10, 100);

  const uint16_t lcd_data[2] = { lcd_default_light, lcd_default_light };
  dgusdisplay.WriteVariable(0x0082, &lcd_data, 5, true);
}

void DGUSScreenHandler::ManualAssistLeveling(DGUS_VP_Variable &var, void *val_ptr) {
  const int16_t point_value = swap16(*(uint16_t *)val_ptr);

  // Insist on leveling first time at this screen
  static bool first_level_flag = false;
  if (!first_level_flag || point_value == 0x0001) {
    queue.enqueue_now_P(G28_STR);
    first_level_flag = true;
  }

  constexpr uint16_t level_speed = 1500;

  auto enqueue_corner_move = [](int16_t lx, int16_t ly, uint16_t fr) {
    char buf_level[32];
    sprintf_P(buf_level, "G0X%dY%dF%d", lx, ly, fr);
    queue.enqueue_one_now(buf_level);
  };

  if (WITHIN(point_value, 0x0001, 0x0005))
    queue.enqueue_now_P(PSTR("G1Z10"));

  switch (point_value) {
    case 0x0001:
      enqueue_corner_move(X_MIN_POS + abs(mks_corner_offsets[0].x),
                          Y_MIN_POS + abs(mks_corner_offsets[0].y), level_speed);
      queue.enqueue_now_P(PSTR("G28Z"));
      break;
    case 0x0002:
      enqueue_corner_move(X_MAX_POS - abs(mks_corner_offsets[1].x),
                          Y_MIN_POS + abs(mks_corner_offsets[1].y), level_speed);
      break;
    case 0x0003:
      enqueue_corner_move(X_MAX_POS - abs(mks_corner_offsets[2].x),
                          Y_MAX_POS - abs(mks_corner_offsets[2].y), level_speed);
      break;
    case 0x0004:
      enqueue_corner_move(X_MIN_POS + abs(mks_corner_offsets[3].x),
                          Y_MAX_POS - abs(mks_corner_offsets[3].y), level_speed);
      break;
    case 0x0005:
      enqueue_corner_move(abs(mks_corner_offsets[4].x),
                          abs(mks_corner_offsets[4].y), level_speed);
      break;
  }

  if (WITHIN(point_value, 0x0002, 0x0005)) {
    //queue.enqueue_now_P(PSTR("G28Z"));
    queue.enqueue_now_P(PSTR("G1Z-10"));
  }
}

#define mks_min(a, b) ((a) < (b)) ? (a) : (b)
#define mks_max(a, b) ((a) > (b)) ? (a) : (b)
void DGUSScreenHandler::TMC_ChangeConfig(DGUS_VP_Variable &var, void *val_ptr) {
  #if EITHER(HAS_TRINAMIC_CONFIG, HAS_STEALTHCHOP)
    const uint16_t tmc_value = swap16(*(uint16_t*)val_ptr);
  #endif

  switch (var.VP) {
    case VP_TMC_X_STEP:
      #if USE_SENSORLESS
        #if AXIS_HAS_STEALTHCHOP(X)
          stepperX.homing_threshold(mks_min(tmc_value, 255));
          settings.save();
          //tmc_step.x = stepperX.homing_threshold();
        #endif
      #endif
      break;
    case VP_TMC_Y_STEP:
      #if USE_SENSORLESS
        #if AXIS_HAS_STEALTHCHOP(Y)
          stepperY.homing_threshold(mks_min(tmc_value, 255));
          settings.save();
          //tmc_step.y = stepperY.homing_threshold();
        #endif
      #endif
      break;
    case VP_TMC_Z_STEP:
      #if USE_SENSORLESS
        #if AXIS_HAS_STEALTHCHOP(Z)
          stepperZ.homing_threshold(mks_min(tmc_value, 255));
          settings.save();
          //tmc_step.z = stepperZ.homing_threshold();
        #endif
      #endif
      break;
    case VP_TMC_X_Current:
      #if AXIS_IS_TMC(X)
        stepperX.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_X1_Current:
      #if AXIS_IS_TMC(X2)
        stepperX2.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_Y_Current:
      #if AXIS_IS_TMC(Y)
        stepperY.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_Y1_Current:
      #if AXIS_IS_TMC(X2)
        stepperY2.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_Z_Current:
      #if AXIS_IS_TMC(Z)
        stepperZ.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_Z1_Current:
      #if AXIS_IS_TMC(Z2)
        stepperZ2.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_E0_Current:
      #if AXIS_IS_TMC(E0)
        stepperE0.rms_current(tmc_value);
        settings.save();
      #endif
      break;
    case VP_TMC_E1_Current:
      #if AXIS_IS_TMC(E1)
        stepperE1.rms_current(tmc_value);
        settings.save();
      #endif
      break;

    default:
      break;
  }
  #if USE_SENSORLESS
    #if AXIS_HAS_STEALTHCHOP(X)
      tmc_step.x = stepperX.homing_threshold();
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      tmc_step.y = stepperY.homing_threshold();
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      tmc_step.z = stepperZ.homing_threshold();
    #endif
  #endif
}

void DGUSScreenHandler::HandleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualMove");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);

  // Choose Move distance
       if (manualMoveStep == 0x01) manualMoveStep =   10;
  else if (manualMoveStep == 0x02) manualMoveStep =  100;
  else if (manualMoveStep == 0x03) manualMoveStep = 1000;

  DEBUG_ECHOLNPAIR("QUEUE LEN:", queue.length);

  if (!print_job_timer.isPaused() && !queue.ring_buffer.empty())
    return;

  char axiscode;
  unsigned int speed = 1500; // FIXME: get default feedrate for manual moves, dont hardcode.

  switch (var.VP) { // switch X Y Z or Home
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
      movevalue = 5;
      axiscode  = '\0';
      // return ;
      break;

    case VP_HOME_ALL: // only used for homing
      DEBUG_ECHOLNPGM("Home all");
      axiscode  = '\0';
      movevalue = 0; // ignore value sent from display, this VP is _ONLY_ for homing.
      //return;
      break;

    case VP_X_HOME:
      DEBUG_ECHOLNPGM("X Home");
      axiscode  = 'X';
      movevalue = 0;
      break;

    case VP_Y_HOME:
      DEBUG_ECHOLNPGM("Y Home");
      axiscode  = 'Y';
      movevalue = 0;
      break;

    case VP_Z_HOME:
      DEBUG_ECHOLNPGM("Z Home");
      axiscode  = 'Z';
      movevalue = 0;
      break;
  }

  DEBUG_ECHOPAIR("movevalue = ", movevalue);
  if (movevalue != 0 && movevalue != 5) { // get move distance
    switch (movevalue) {
      case 0x0001: movevalue =  manualMoveStep; break;
      case 0x0002: movevalue = -manualMoveStep; break;
      default:     movevalue = 0; break;
    }
  }

  if (!movevalue) {
    // homing
    DEBUG_ECHOPAIR(" homing ", AS_CHAR(axiscode));
    // char buf[6] = "G28 X";
    // buf[4] = axiscode;

    char buf[6];
    sprintf(buf, "G28 %c", axiscode);
    //DEBUG_ECHOPAIR(" ", buf);
    queue.enqueue_one_now(buf);
    //DEBUG_ECHOLNPGM(" ✓");
    ForceCompleteUpdate();
    return;
  }
  else if (movevalue == 5) {
    DEBUG_ECHOPAIR("send M84");
    char buf[6];
    snprintf_P(buf,6,PSTR("M84 %c"), axiscode);
    queue.enqueue_one_now(buf);
    ForceCompleteUpdate();
    return;
  }
  else {
    // movement
    DEBUG_ECHOPAIR(" move ", AS_CHAR(axiscode));
    bool old_relative_mode = relative_mode;

    if (!relative_mode) {
      //DEBUG_ECHOPGM(" G91");
      queue.enqueue_now_P(PSTR("G91"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
    char buf[32]; // G1 X9999.99 F12345
    // unsigned int backup_speed = MMS_TO_MMM(feedrate_mm_s);
    char sign[] = "\0";
    int16_t value = movevalue / 100;
    if (movevalue < 0) { value = -value; sign[0] = '-'; }
    int16_t fraction = ABS(movevalue) % 100;
    snprintf_P(buf, 32, PSTR("G0 %c%s%d.%02d F%d"), axiscode, sign, value, fraction, speed);
    queue.enqueue_one_now(buf);

    //if (backup_speed != speed) {
    //  snprintf_P(buf, 32, PSTR("G0 F%d"), backup_speed);
    //  queue.enqueue_one_now(buf);
    //  //DEBUG_ECHOPAIR(" ", buf);
    //}

    //while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLNPGM(" ✓ ");
    if (!old_relative_mode) {
      //DEBUG_ECHOPGM("G90");
      //queue.enqueue_now_P(PSTR("G90"));
      queue.enqueue_now_P(PSTR("G90"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
  }

  ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("manmv done.");
  return;

  cannotmove:
    DEBUG_ECHOLNPAIR(" cannot move ", AS_CHAR(axiscode));
    return;
}

void DGUSScreenHandler::GetParkPos_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  const int16_t value_pos = swap16(*(int16_t*)val_ptr);

  switch (var.VP) {
    case VP_X_PARK_POS: mks_park_pos.x = value_pos; break;
    case VP_Y_PARK_POS: mks_park_pos.y = value_pos; break;
    case VP_Z_PARK_POS: mks_park_pos.z = value_pos; break;
    default: break;
  }
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleChangeLevelPoint_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleChangeLevelPoint_MKS");

  const int16_t value_raw = swap16(*(int16_t*)val_ptr);
  DEBUG_ECHOLNPAIR_F("value_raw:", value_raw);

  *(int16_t*)var.memadr = value_raw;

  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleStepPerMMChanged_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMChanged_MKS");

  const uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  const float value = (float)value_raw;

  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  DEBUG_ECHOLNPAIR_F("value:", value);

  ExtUI::axis_t axis;
  switch (var.VP) {
    default: return;
    case VP_X_STEP_PER_MM: axis = ExtUI::axis_t::X; break;
    case VP_Y_STEP_PER_MM: axis = ExtUI::axis_t::Y; break;
    case VP_Z_STEP_PER_MM: axis = ExtUI::axis_t::Z; break;
  }
  ExtUI::setAxisSteps_per_mm(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(axis));
  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleStepPerMMExtruderChanged_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMExtruderChanged_MKS");

  const uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  const float value = (float)value_raw;

  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  DEBUG_ECHOLNPAIR_F("value:", value);

  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HAS_HOTEND
      case VP_E0_STEP_PER_MM: extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HAS_MULTI_HOTEND
      case VP_E1_STEP_PER_MM: extruder = ExtUI::extruder_t::E1; break;
    #endif
  }
  ExtUI::setAxisSteps_per_mm(value, extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(extruder));
  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleMaxSpeedChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMaxSpeedChange_MKS");

  const uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  const float value = (float)value_raw;

  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  DEBUG_ECHOLNPAIR_F("value:", value);

  ExtUI::axis_t axis;
  switch (var.VP) {
    case VP_X_MAX_SPEED: axis = ExtUI::axis_t::X; break;
    case VP_Y_MAX_SPEED: axis = ExtUI::axis_t::Y; break;
    case VP_Z_MAX_SPEED: axis = ExtUI::axis_t::Z; break;
    default: return;
  }
  ExtUI::setAxisMaxFeedrate_mm_s(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxFeedrate_mm_s(axis));
  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleExtruderMaxSpeedChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleExtruderMaxSpeedChange_MKS");

  const uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  const float value = (float)value_raw;

  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  DEBUG_ECHOLNPAIR_F("value:", value);

  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
      #if HAS_HOTEND
        case VP_E0_MAX_SPEED: extruder = ExtUI::extruder_t::E0; break;
      #endif
      #if HAS_MULTI_HOTEND
      #endif
    case VP_E1_MAX_SPEED: extruder = ExtUI::extruder_t::E1; break;
  }
  ExtUI::setAxisMaxFeedrate_mm_s(value, extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxFeedrate_mm_s(extruder));
  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleMaxAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMaxAccChange_MKS");

  const uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  const float value = (float)value_raw;

  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  DEBUG_ECHOLNPAIR_F("value:", value);

  ExtUI::axis_t axis;
  switch (var.VP) {
    default: return;
    case VP_X_ACC_MAX_SPEED: axis = ExtUI::axis_t::X;  break;
    case VP_Y_ACC_MAX_SPEED: axis = ExtUI::axis_t::Y;  break;
    case VP_Z_ACC_MAX_SPEED: axis = ExtUI::axis_t::Z;  break;
  }
  ExtUI::setAxisMaxAcceleration_mm_s2(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxAcceleration_mm_s2(axis));
  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleExtruderAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleExtruderAccChange_MKS");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw;
  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HAS_HOTEND
      case VP_E0_ACC_MAX_SPEED: extruder = ExtUI::extruder_t::E0; settings.load(); break;
    #endif
    #if HAS_MULTI_HOTEND
      case VP_E1_ACC_MAX_SPEED: extruder = ExtUI::extruder_t::E1; settings.load(); break;
    #endif
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisMaxAcceleration_mm_s2(value, extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisMaxAcceleration_mm_s2(extruder));
  settings.save();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleTravelAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t value_travel = swap16(*(uint16_t*)val_ptr);
  planner.settings.travel_acceleration = (float)value_travel;
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleFeedRateMinChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t value_t = swap16(*(uint16_t*)val_ptr);
  planner.settings.min_feedrate_mm_s = (float)value_t;
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleMin_T_F_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t value_t_f = swap16(*(uint16_t*)val_ptr);
  planner.settings.min_travel_feedrate_mm_s = (float)value_t_f;
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleAccChange_MKS(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t value_acc = swap16(*(uint16_t*)val_ptr);
  planner.settings.acceleration = (float)value_acc;
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

#if ENABLED(PREVENT_COLD_EXTRUSION)
  void DGUSScreenHandler::HandleGetExMinTemp_MKS(DGUS_VP_Variable &var, void *val_ptr) {
    const uint16_t value_ex_min_temp = swap16(*(uint16_t*)val_ptr);
    thermalManager.extrude_min_temp = value_ex_min_temp;
    skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }
#endif

#if HAS_PID_HEATING
  void DGUSScreenHandler::HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr) {
    const uint16_t rawvalue = swap16(*(uint16_t*)val_ptr);
    DEBUG_ECHOLNPAIR("V1:", rawvalue);
    const float value = 1.0f * rawvalue;
    DEBUG_ECHOLNPAIR("V2:", value);
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

    DEBUG_ECHOLNPAIR_F("V3:", newvalue);
    *(float *)var.memadr = newvalue;

    settings.save();
    skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }
#endif // HAS_PID_HEATING

#if ENABLED(BABYSTEPPING)
  void DGUSScreenHandler::HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleLiveAdjustZ");
    char babystep_buf[30];
    float step = ZOffset_distance;

    uint16_t flag = swap16(*(uint16_t*)val_ptr);
    switch (flag) {
      case 0:
        if (step == 0.01)
          queue.inject_P(PSTR("M290 Z-0.01"));
        else if (step == 0.1)
          queue.inject_P(PSTR("M290 Z-0.1"));
        else if (step == 0.5)
          queue.inject_P(PSTR("M290 Z-0.5"));
        else if (step == 1)
          queue.inject_P(PSTR("M290 Z-1"));
        else
          queue.inject_P(PSTR("M290 Z-0.01"));

        z_offset_add = z_offset_add - ZOffset_distance;
        break;

      case 1:
        if (step == 0.01)
          queue.inject_P(PSTR("M290 Z0.01"));
        else if (step == 0.1)
          queue.inject_P(PSTR("M290 Z0.1"));
        else if (step == 0.5)
          queue.inject_P(PSTR("M290 Z0.5"));
        else if (step == 1)
          queue.inject_P(PSTR("M290 Z1"));
        else
          queue.inject_P(PSTR("M290 Z-0.01"));

        z_offset_add = z_offset_add + ZOffset_distance;
        break;

      default:
        break;
    }
    ForceCompleteUpdate();
  }
#endif // BABYSTEPPING

void DGUSScreenHandler::GetManualFilament(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("GetManualFilament");

  uint16_t value_len = swap16(*(uint16_t*)val_ptr);

  float value = (float)value_len;

  DEBUG_ECHOLNPAIR_F("Get Filament len value:", value);
  distanceFilament = value;

  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::GetManualFilamentSpeed(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("GetManualFilamentSpeed");

  uint16_t value_len = swap16(*(uint16_t*)val_ptr);

  DEBUG_ECHOLNPAIR_F("filamentSpeed_mm_s value:", value_len);

  filamentSpeed_mm_s = value_len;

  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::MKS_FilamentLoadUnload(DGUS_VP_Variable &var, void *val_ptr, const int filamentDir) {
  #if EITHER(HAS_MULTI_HOTEND, SINGLENOZZLE)
    uint8_t swap_tool = 0;
  #else
    constexpr uint8_t swap_tool = 1; // T0 (or none at all)
  #endif

  #if HAS_HOTEND
    uint8_t hotend_too_cold = 0;
  #endif

  if (!print_job_timer.isPaused() && !queue.ring_buffer.empty())
    return;

  const uint16_t val_t = swap16(*(uint16_t*)val_ptr);
  switch (val_t) {
    default: break;
    case 0:
      #if HAS_HOTEND
        if (thermalManager.tooColdToExtrude(0))
          hotend_too_cold = 1;
        else {
          #if EITHER(HAS_MULTI_HOTEND, SINGLENOZZLE)
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

  #if BOTH(HAS_HOTEND, PREVENT_COLD_EXTRUSION)
    if (hotend_too_cold) {
      if (thermalManager.targetTooColdToExtrude(hotend_too_cold - 1)) thermalManager.setTargetHotend(thermalManager.extrude_min_temp, hotend_too_cold - 1);
      sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please wait."), PSTR("Nozzle heating!"), true, true, true, true);
      SetupConfirmAction(nullptr);
      GotoScreen(DGUSLCD_SCREEN_POPUP);
    }
  #endif

  if (swap_tool) {
    char buf[30];
    snprintf_P(buf, 30
      #if EITHER(HAS_MULTI_HOTEND, SINGLENOZZLE)
        , PSTR("M1002T%cE%dF%d"), char('0' + swap_tool - 1)
      #else
        , PSTR("M1002E%dF%d")
      #endif
      , (int)distanceFilament * filamentDir, filamentSpeed_mm_s * 60
    );
    queue.inject(buf);
  }
}

/**
 * M1002: Do a tool-change and relative move for MKS_FilamentLoadUnload
 *        within the G-code execution window for best concurrency.
 */
void GcodeSuite::M1002() {
  #if EITHER(HAS_MULTI_HOTEND, SINGLENOZZLE)
  {
    char buf[3];
    sprintf_P(buf, PSTR("T%c"), char('0' + parser.intval('T')));
    process_subcommands_now(buf);
  }
  #endif

  const uint8_t old_axis_relative = axis_relative;
  set_e_relative(); // M83
  {
    char buf[20];
    snprintf_P(buf, 20, PSTR("G1E%dF%d"), parser.intval('E'), parser.intval('F'));
    process_subcommands_now(buf);
  }
  axis_relative = old_axis_relative;
}

void DGUSScreenHandler::MKS_FilamentLoad(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("MKS_FilamentLoad");
   MKS_FilamentLoadUnload(var, val_ptr, 1);
}

void DGUSScreenHandler::MKS_FilamentUnLoad(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("MKS_FilamentUnLoad");
  MKS_FilamentLoadUnload(var, val_ptr, -1);
}

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

  void DGUSScreenHandler::HandleFilamentOption(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleFilamentOption");

    uint8_t e_temp = 0;
    filament_data.heated = false;
    uint16_t preheat_option = swap16(*(uint16_t*)val_ptr);
    if (preheat_option >= 10) {     // Unload filament type
      preheat_option -= 10;
      filament_data.action = 2;
      filament_data.purge_length = DGUS_FILAMENT_PURGE_LENGTH;
    }
    else if (preheat_option <= 8)   // Load filament type
      filament_data.action = 1;
    else                            // Cancel filament operation
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
      #if HAS_HOTEND
        thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E0);
      #endif
      #if HAS_MULTI_HOTEND
        thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E1);
      #endif
      GotoScreen(DGUSLCD_SCREEN_UTILITY);
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
          filament_data.heated = true;
        }
        movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) + movevalue;
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
        else {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) - movevalue;
        }
      }
      ExtUI::setAxisPosition_mm(movevalue, filament_data.extruder);
    }
  }

#endif // DGUS_FILAMENT_LOADUNLOAD

bool DGUSScreenHandler::loop() {
  dgusdisplay.loop();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  static uint8_t language_times = 2;

  if (!IsScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    UpdateScreenVPData();
  }

  if (language_times != 0) {
    LanguagePInit();
    DGUS_LanguageDisplay(mks_language_index);
    language_times--;
  }

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool booted = false;
    if (!booted && ELAPSED(ms, TERN(USE_MKS_GREEN_UI, 1000, BOOTSCREEN_TIMEOUT))) {
      booted = true;
      #if USE_SENSORLESS
        #if AXIS_HAS_STEALTHCHOP(X)
          tmc_step.x = stepperX.homing_threshold();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          tmc_step.y = stepperY.homing_threshold();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          tmc_step.z = stepperZ.homing_threshold();
        #endif
      #endif

      #if ENABLED(PREVENT_COLD_EXTRUSION)
        if (mks_min_extrusion_temp != 0)
          thermalManager.extrude_min_temp = mks_min_extrusion_temp;
      #endif

      DGUS_ExtrudeLoadInit();

      TERN_(DGUS_MKS_RUNOUT_SENSOR, DGUS_RunoutInit());

      if (TERN0(POWER_LOSS_RECOVERY, recovery.valid()))
        GotoScreen(DGUSLCD_SCREEN_POWER_LOSS);
      else
        GotoScreen(DGUSLCD_SCREEN_MAIN);
    }

    #if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
      if (booted && printingIsActive()) DGUS_Runout_Idle();
    #endif
  #endif // SHOW_BOOTSCREEN

  return IsScreenComplete();
}

void DGUSScreenHandler::LanguagePInit() {
  switch (mks_language_index) {
    case MKS_SimpleChinese:
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1, MKS_Language_Choose);
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2, MKS_Language_NoChoose);
      break;
    case MKS_English:
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE1, MKS_Language_NoChoose);
      dgusdisplay.MKS_WriteVariable(VP_LANGUAGE_CHANGE2, MKS_Language_Choose);
      break;
    default:
      break;
  }
}

void DGUSScreenHandler::DGUS_ExtrudeLoadInit(void) {
  ex_filament.ex_length           = distanceFilament;
  ex_filament.ex_load_unload_flag = 0;
  ex_filament.ex_need_time        = filamentSpeed_mm_s;
  ex_filament.ex_speed            = 0;
  ex_filament.ex_status           = EX_NONE;
  ex_filament.ex_tick_end         = 0;
  ex_filament.ex_tick_start       = 0;
}

void DGUSScreenHandler::DGUS_RunoutInit(void) {
  #if PIN_EXISTS(MT_DET_1)
    SET_INPUT_PULLUP(MT_DET_1_PIN);
  #endif
  runout_mks.de_count      = 0;
  runout_mks.de_times      = 10;
  runout_mks.pin_status    = 1;
  runout_mks.runout_status = UNRUNOUT_STATUS;
}

void DGUSScreenHandler::DGUS_Runout_Idle(void) {
  #if ENABLED(DGUS_MKS_RUNOUT_SENSOR)
    // scanf runout pin
    switch (runout_mks.runout_status) {

      case RUNOUT_STATUS:
        runout_mks.runout_status = RUNOUT_BEGIN_STATUS;
        queue.inject_P(PSTR("M25"));
        GotoScreen(MKSLCD_SCREEN_PAUSE);

        sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("Please change filament!"), nullptr, true, true, true, true);
        //SetupConfirmAction(nullptr);
        GotoScreen(DGUSLCD_SCREEN_POPUP);
        break;

      case UNRUNOUT_STATUS:
        if (READ(MT_DET_1_PIN) == LOW)
          runout_mks.runout_status = RUNOUT_STATUS;
        break;

      case RUNOUT_BEGIN_STATUS:
        if (READ(MT_DET_1_PIN) == HIGH)
          runout_mks.runout_status = RUNOUT_WAITTING_STATUS;
        break;

      case RUNOUT_WAITTING_STATUS:
        if (READ(MT_DET_1_PIN) == LOW)
          runout_mks.runout_status = RUNOUT_BEGIN_STATUS;
        break;

      default: break;
    }
  #endif
}

void DGUSScreenHandler::DGUS_LanguageDisplay(uint8_t var) {
  if (var == MKS_English) {
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

    const char Filament_buf_en[] = "Extrude";
    dgusdisplay.WriteVariable(VP_Filament_Dis, Filament_buf_en, 32, true);

    const char Move_buf_en[] = "Move";
    dgusdisplay.WriteVariable(VP_Move_Dis, Move_buf_en, 32, true);

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
    dgusdisplay.WriteVariable(VP_Fan_Speed_Dis, FAN_Speed_buf_en, 32, true);

    const char Printing_buf_en[] = "Printing";
    dgusdisplay.WriteVariable(VP_Printing_Dis, Printing_buf_en, 32, true);

    const char Info_EEPROM_1_buf_en[] = "Store setting?";
    dgusdisplay.WriteVariable(VP_Info_EEPROM_1_Dis, Info_EEPROM_1_buf_en, 32, true);

    const char Info_EEPROM_2_buf_en[] = "Revert setting?";
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

    const char LCD_BLK_buf_en[] = "Backlight";
    dgusdisplay.WriteVariable(VP_LCD_BLK_Dis, LCD_BLK_buf_en, 32, true);
  }
  else if (var == MKS_SimpleChinese) {
    uint16_t home_buf_ch[] = { 0xF7D6, 0xB3D2 };
    dgusdisplay.WriteVariable(VP_HOME_Dis, home_buf_ch, 4, true);

    const uint16_t Setting_Dis[] = { 0xE8C9, 0xC3D6, 0x2000, 0x2000, 0x2000 };
    dgusdisplay.WriteVariable(VP_Setting_Dis, Setting_Dis, 7, true);

    const uint16_t Tool_Dis[] = { 0xA4B9, 0xDFBE };
    dgusdisplay.WriteVariable(VP_Tool_Dis, Tool_Dis, 4, true);

    const uint16_t Print_buf_ch[] = { 0xF2B4, 0xA1D3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Print_Dis, Print_buf_ch, 6, true);

    const uint16_t Language_buf_ch[] = { 0xEFD3, 0xD4D1, 0x2000, 0x2000 };
    dgusdisplay.WriteVariable(VP_Language_Dis, Language_buf_ch, 8, true);

    const uint16_t About_buf_ch[] = { 0xD8B9, 0xDAD3, 0x2000 };
    dgusdisplay.WriteVariable(VP_About_Dis, About_buf_ch, 6, true);

    const uint16_t Config_buf_ch[] = { 0xE4C5, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Config_Dis, Config_buf_ch, 6, true);

    const uint16_t MotorConfig_buf_ch[] = { 0xE7B5, 0xFABB, 0xE4C5, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_MotorConfig_Dis, MotorConfig_buf_ch, 12, true);

    const uint16_t LevelConfig_buf_ch[] = { 0xD6CA, 0xAFB6, 0xF7B5, 0xBDC6, 0xE8C9, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_LevelConfig_Dis, LevelConfig_buf_ch, 32, true);

    const uint16_t TemperatureConfig_buf_ch[] = { 0xC2CE, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_TemperatureConfig_Dis, TemperatureConfig_buf_ch, 11, true);

    const uint16_t Advance_buf_ch[] = { 0xDFB8, 0xB6BC, 0xE8C9, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Advance_Dis, Advance_buf_ch, 16, true);

    const uint16_t Filament_buf_ch[] = { 0xB7BC, 0xF6B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Filament_Dis, Filament_buf_ch, 8, true);

    const uint16_t Move_buf_ch[] = { 0xC6D2, 0xAFB6, 0x2000  };
    dgusdisplay.WriteVariable(VP_Move_Dis, Move_buf_ch, 4, true);

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      const uint16_t Level_buf_ch[] = { 0xD4D7, 0xAFB6, 0xF7B5, 0xBDC6, 0x2000 };
      dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_ch, 32, true);
    #elif ENABLED(MESH_BED_LEVELING)
      const uint16_t Level_buf_ch[] = { 0xF8CD, 0xF1B8, 0xF7B5, 0xBDC6, 0x2000 };
      dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_ch, 32, true);
    #else
      const uint16_t Level_buf_ch[] = { 0xD6CA, 0xAFB6, 0xF7B5, 0xBDC6, 0x2000 };
      dgusdisplay.WriteVariable(VP_Level_Dis, Level_buf_ch, 32, true);
    #endif

    const uint16_t MotorPluse_buf_ch[] = { 0xF6C2, 0xE5B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_MotorPluse_Dis, MotorPluse_buf_ch, 16, true);

    const uint16_t MotorMaxSpeed_buf_ch[] = { 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_MotorMaxSpeed_Dis, MotorMaxSpeed_buf_ch, 16, true);

    const uint16_t MotorMaxAcc_buf_ch[] = { 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_MotorMaxAcc_Dis, MotorMaxAcc_buf_ch, 16, true);

    const uint16_t TravelAcc_buf_ch[] = { 0xD5BF, 0xD0D0, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_TravelAcc_Dis, TravelAcc_buf_ch, 16, true);

    const uint16_t FeedRateMin_buf_ch[] = { 0xEED7, 0xA1D0, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_FeedRateMin_Dis, FeedRateMin_buf_ch, 12, true);

    const uint16_t TravelFeeRateMin_buf_ch[] = { 0xD5BF, 0xD0D0, 0xEED7, 0xA1D0, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_TravelFeeRateMin_Dis, TravelFeeRateMin_buf_ch, 24, true);

    const uint16_t Acc_buf_ch[] = { 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_ACC_Dis, Acc_buf_ch, 16, true);

    const uint16_t Point_One_buf_ch[] = { 0xDAB5, 0xBBD2, 0xE3B5, 0x2000 };
    dgusdisplay.WriteVariable(VP_Point_One_Dis, Point_One_buf_ch, 12, true);

    const uint16_t Point_Two_buf_ch[] = { 0xDAB5, 0xFEB6, 0xE3B5, 0x2000 };
    dgusdisplay.WriteVariable(VP_Point_Two_Dis, Point_Two_buf_ch, 12, true);

    const uint16_t Point_Three_buf_ch[] = { 0xDAB5, 0xFDC8, 0xE3B5, 0x2000 };
    dgusdisplay.WriteVariable(VP_Point_Three_Dis, Point_Three_buf_ch, 12, true);

    const uint16_t Point_Four_buf_ch[] = { 0xDAB5, 0xC4CB, 0xE3B5, 0x2000 };
    dgusdisplay.WriteVariable(VP_Point_Four_Dis, Point_Four_buf_ch, 12, true);

    const uint16_t Point_Five_buf_ch[] = { 0xDAB5, 0xE5CE, 0xE3B5, 0x2000 };
    dgusdisplay.WriteVariable(VP_Point_Five_Dis, Point_Five_buf_ch, 12, true);

    const uint16_t Extrusion_buf_ch[] = { 0xB7BC, 0xF6B3, 0xB7CD, 0x2000 };
    dgusdisplay.WriteVariable(VP_Extrusion_Dis, Extrusion_buf_ch, 12, true);

    const uint16_t HeatBed_buf_ch[] = { 0xC8C8, 0xB2B4, 0x2000 };
    dgusdisplay.WriteVariable(VP_HeatBed_Dis, HeatBed_buf_ch, 12, true);

    const uint16_t FactoryDefaults_buf_ch[] = { 0xD6BB, 0xB4B8, 0xF6B3, 0xA7B3, 0xE8C9, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_FactoryDefaults_Dis, FactoryDefaults_buf_ch, 16, true);

    const uint16_t StoreSetting_buf_ch[] = { 0xA3B1, 0xE6B4, 0xE8C9, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_StoreSetting_Dis, StoreSetting_buf_ch, 16, true);

    const uint16_t PrintPauseConfig_buf_ch[] = { 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_PrintPauseConfig_Dis, PrintPauseConfig_buf_ch, 32, true);

    const uint16_t X_Pluse_buf_ch[] = { 0x2058, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_X_Pluse_Dis, X_Pluse_buf_ch, 16, true);

    const uint16_t Y_Pluse_buf_ch[] = { 0x2059, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Y_Pluse_Dis, Y_Pluse_buf_ch, 16, true);

    const uint16_t Z_Pluse_buf_ch[] = { 0x205A, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Z_Pluse_Dis, Z_Pluse_buf_ch, 16, true);

    const uint16_t E0_Pluse_buf_ch[] = { 0x3045, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_E0_Pluse_Dis, E0_Pluse_buf_ch, 16, true);

    const uint16_t E1_Pluse_buf_ch[] = { 0x3145, 0xE1D6, 0xF6C2, 0xE5B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_E1_Pluse_Dis, E1_Pluse_buf_ch, 16, true);

    const uint16_t X_Max_Speed_buf_ch[] = { 0x2058, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_X_Max_Speed_Dis, X_Max_Speed_buf_ch, 16, true);

    const uint16_t Y_Max_Speed_buf_ch[] = { 0x2059, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Y_Max_Speed_Dis, Y_Max_Speed_buf_ch, 16, true);

    const uint16_t Z_Max_Speed_buf_ch[] = { 0x205A, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Z_Max_Speed_Dis, Z_Max_Speed_buf_ch, 16, true);

    const uint16_t E0_Max_Speed_buf_ch[] = { 0x3045, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_E0_Max_Speed_Dis, E0_Max_Speed_buf_ch, 16, true);

    const uint16_t E1_Max_Speed_buf_ch[] = { 0x3145, 0xEED7, 0xF3B4, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_E1_Max_Speed_Dis, E1_Max_Speed_buf_ch, 16, true);

    const uint16_t X_Max_Acc_Speed_buf_ch[] = { 0x2058, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_X_Max_Acc_Speed_Dis, X_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t Y_Max_Acc_Speed_buf_ch[] = { 0x2059, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Y_Max_Acc_Speed_Dis, Y_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t Z_Max_Acc_Speed_buf_ch[] = { 0x205A, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Z_Max_Acc_Speed_Dis, Z_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t E0_Max_Acc_Speed_buf_ch[] = { 0x3045, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_E0_Max_Acc_Speed_Dis, E0_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t E1_Max_Acc_Speed_buf_ch[] = { 0x3145, 0xEED7, 0xF3B4, 0xD3BC, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_E1_Max_Acc_Speed_Dis, E1_Max_Acc_Speed_buf_ch, 16, true);

    const uint16_t X_PARK_POS_buf_ch[] = { 0x2058, 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_X_PARK_POS_Dis, X_PARK_POS_buf_ch, 16, true);

    const uint16_t Y_PARK_POS_buf_ch[] = { 0x2059, 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Y_PARK_POS_Dis, Y_PARK_POS_buf_ch, 16, true);

    const uint16_t Z_PARK_POS_buf_ch[] = { 0x205A, 0xDDD4, 0xA3CD, 0xBBCE, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Z_PARK_POS_Dis, Z_PARK_POS_buf_ch, 16, true);

    const uint16_t Length_buf_ch[] = { 0xBDB2, 0xA4B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Length_Dis, Length_buf_ch, 8, true);

    const uint16_t Speed_buf_ch[] = { 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Speed_Dis, Speed_buf_ch, 8, true);

    const uint16_t InOut_buf_ch[] = { 0xF8BD, 0xF6B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_InOut_Dis, InOut_buf_ch, 8, true);

    const uint16_t PrintTimet_buf_en[] = { 0xF2B4, 0xA1D3, 0xB1CA, 0xE4BC, 0x2000 };
    dgusdisplay.WriteVariable(VP_PrintTime_Dis, PrintTimet_buf_en, 16, true);

    const uint16_t E0_Temp_buf_ch[] = { 0x3045, 0xC2CE, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_E0_Temp_Dis, E0_Temp_buf_ch, 16, true);

    const uint16_t E1_Temp_buf_ch[] = { 0x3145, 0xC2CE, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_E1_Temp_Dis, E1_Temp_buf_ch, 16, true);

    const uint16_t HB_Temp_buf_ch[] = { 0xC8C8, 0xB2B4, 0xC2CE, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_HB_Temp_Dis, HB_Temp_buf_ch, 16, true);

    const uint16_t Feedrate_buf_ch[] = { 0xB7BC, 0xF6B3, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Feedrate_Dis, Feedrate_buf_ch, 16, true);

    const uint16_t PrintAcc_buf_ch[] = { 0xF2B4, 0xA1D3, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_PrintAcc_Dis, PrintAcc_buf_ch, 16, true);

    const uint16_t FAN_Speed_buf_ch[] = { 0xE7B7, 0xC8C9, 0xD9CB, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Fan_Speed_Dis, FAN_Speed_buf_ch, 16, true);

    const uint16_t Printing_buf_ch[] = { 0xF2B4, 0xA1D3, 0xD0D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Printing_Dis, Printing_buf_ch, 16, true);

    const uint16_t Info_EEPROM_1_buf_ch[] = { 0xC7CA, 0xF1B7, 0xA3B1, 0xE6B4, 0xE8C9, 0xC3D6, 0xBFA3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Info_EEPROM_1_Dis, Info_EEPROM_1_buf_ch, 32, true);

    const uint16_t Info_EEPROM_2_buf_ch[] = { 0xC7CA, 0xF1B7, 0xD6BB, 0xB4B8, 0xF6B3, 0xA7B3, 0xE8C9, 0xC3D6, 0xBFA3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Info_EEPROM_2_Dis, Info_EEPROM_2_buf_ch, 32, true);

    const uint16_t TMC_X_Step_buf_ch[] = { 0x2058, 0xE9C1, 0xF4C3, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_X_Step_Dis, TMC_X_Step_buf_ch, 16, true);

    const uint16_t TMC_Y_Step_buf_ch[] = { 0x2059, 0xE9C1, 0xF4C3, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_Y_Step_Dis, TMC_Y_Step_buf_ch, 16, true);

    const uint16_t TMC_Z_Step_buf_ch[] = { 0x205A, 0xE9C1, 0xF4C3, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_Z_Step_Dis, TMC_Z_Step_buf_ch, 16, true);

    const uint16_t Info_PrinfFinsh_1_buf_ch[] = { 0xF2B4, 0xA1D3, 0xEACD, 0xC9B3, 0x2000 };
    dgusdisplay.WriteVariable(VP_Info_PrinfFinsh_1_Dis, Info_PrinfFinsh_1_buf_ch, 32, true);

    const uint16_t TMC_X_Current_buf_ch[] = { 0x2058, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_X_Current_Dis, TMC_X_Current_buf_ch, 16, true);

    const uint16_t TMC_Y_Current_buf_ch[] = { 0x2059, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_Y_Current_Dis, TMC_Y_Current_buf_ch, 16, true);

    const uint16_t TMC_Z_Current_buf_ch[] = { 0x205A, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_Z_Current_Dis, TMC_Z_Current_buf_ch, 16, true);

    const uint16_t TMC_E0_Current_buf_ch[] = { 0x3045, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_E0_Current_Dis, TMC_E0_Current_buf_ch, 16, true);

    const uint16_t TMC_X1_Current_buf_ch[] = { 0x3158, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_X1_Current_Dis, TMC_X1_Current_buf_ch, 16, true);

    const uint16_t TMC_Y1_Current_buf_ch[] = { 0x3159, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_Y1_Current_Dis, TMC_Y1_Current_buf_ch, 16, true);

    const uint16_t TMC_Z1_Current_buf_ch[] = { 0x315A, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_Z1_Current_Dis, TMC_Z1_Current_buf_ch, 16, true);

    const uint16_t TMC_E1_Current_buf_ch[] = { 0x3145, 0xE1D6, 0xE7B5, 0xF7C1, 0x2000 };
    dgusdisplay.WriteVariable(VP_TMC_E1_Current_Dis, TMC_E1_Current_buf_ch, 16, true);

    const uint16_t Min_Ex_Temp_buf_ch[] = { 0xEED7, 0xA1D0, 0xB7BC, 0xF6B3, 0xC2CE, 0xC8B6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Min_Ex_Temp_Dis, Min_Ex_Temp_buf_ch, 32, true);

    const uint16_t AutoLEVEL_INFO1_buf_ch[] = { 0xEBC7, 0xB4B0, 0xC2CF, 0xB4B0, 0xA5C5, 0x2000 };
    dgusdisplay.WriteVariable(VP_AutoLEVEL_INFO1, AutoLEVEL_INFO1_buf_ch, 32, true);

    const uint16_t EX_TEMP_INFO2_buf_ch[] = { 0xEBC7, 0xD4C9, 0xC8B5, 0x2000 };
    dgusdisplay.WriteVariable(VP_EX_TEMP_INFO2_Dis, EX_TEMP_INFO2_buf_ch, 32, true);

    const uint16_t EX_TEMP_INFO3_buf_ch[] = { 0xA1C8, 0xFBCF, 0xD3BC, 0xC8C8, 0x2000 };
    dgusdisplay.WriteVariable(VP_EX_TEMP_INFO3_Dis, EX_TEMP_INFO3_buf_ch, 32, true);

    const uint16_t PrintConfrim_Info_buf_ch[] = { 0xC7CA, 0xF1B7, 0xAABF, 0xBCCA, 0xF2B4, 0xA1D3, 0x2000 };
    dgusdisplay.WriteVariable(VP_PrintConfrim_Info_Dis, PrintConfrim_Info_buf_ch, 32, true);

    const uint16_t StopPrintConfrim_Info_buf_ch[] = { 0xC7CA, 0xF1B7, 0xA3CD, 0xB9D6, 0xF2B4, 0xA1D3, 0x2000 };
    dgusdisplay.WriteVariable(VP_StopPrintConfrim_Info_Dis, StopPrintConfrim_Info_buf_ch, 32, true);

    const uint16_t Printting_buf_ch[] = { 0xF2B4, 0xA1D3, 0xD0D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_Printting_Dis, Printting_buf_ch, 32, true);

    const uint16_t LCD_BLK_buf_ch[] = { 0xB3B1, 0xE2B9, 0xE8C9, 0xC3D6, 0x2000 };
    dgusdisplay.WriteVariable(VP_LCD_BLK_Dis, LCD_BLK_buf_ch, 32, true);
  }
}

#endif // DGUS_LCD_UI_MKS
