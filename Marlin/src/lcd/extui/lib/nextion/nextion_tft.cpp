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

/* ****************************************
 * lcd/extui/lib/nextion/nextion_tft.h
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(NEXTION_TFT)

  #include "../../../../MarlinCore.h"
  #include "../../../../feature/pause.h"
  #include "../../../../gcode/queue.h"
  #include "../../../../libs/numtostr.h"
  #include "../../../../sd/cardreader.h"
  #include "FileNavigator.h"
  #include "nextion_tft.h"

namespace Nextion {

  char NextionTFT::selectedfile[MAX_PATH_LEN];
  char NextionTFT::nextion_command[MAX_CMND_LEN];
  uint8_t NextionTFT::command_len;

  bool last_homed = 0, last_homedX = 0, last_homedY = 0, last_homedZ = 0;
  float last_degBed = 999, last_degHotend0 = 999, last_degHotend1 = 999, last_degTargetBed = 999, last_degTargetHotend0 = 999, last_degTargetHotend1 = 999;
  float last_get_axis_position_mmX = 999, last_get_axis_position_mmY = 999, last_get_axis_position_mmZ = 999;
  float last_extruder_advance_K = 999;
  uint8_t last_active_extruder = 99, last_fan_speed = 99, last_print_speed = 99, last_flow_speed = 99, last_progress = 99;
  uint8_t last_printer_state = 99, last_IDEX_Mode = 99;
  uint32_t layer = 0, last_layer = 99;

  NextionTFT nextion;

  NextionTFT::NextionTFT() {}

  void NextionTFT::Startup() {
    selectedfile[0] = '\0';
    nextion_command[0] = '\0';
    command_len = 0;
    LCD_SERIAL.begin(115200);

    SEND_VAL("tmppage.connected", 0);
    delay_ms(100);
    SEND_VAL("tmppage.connected", 1);

    SEND_VALasTXT("tmppage.marlin", SHORT_BUILD_VERSION);
    SEND_VALasTXT("tmppage.compiled", __DATE__ " / " __TIME__);
    SEND_VALasTXT("tmppage.extruder", EXTRUDERS);
    SEND_VALasTXT("tmppage.printer", MACHINE_NAME);
    SEND_VALasTXT("tmppage.author", STRING_CONFIG_H_AUTHOR);
    SEND_VALasTXT("tmppage.released", STRING_DISTRIBUTION_DATE);
    SEND_VALasTXT("tmppage.bedx", X_BED_SIZE);
    SEND_VALasTXT("tmppage.bedy", Y_BED_SIZE);
    SEND_VALasTXT("tmppage.bedz", Z_MAX_POS);

  #if NEXDEBUGLEVEL
    SERIAL_ECHOLNPAIR("Nextion Debug Level ", NEXDEBUGLEVEL);
  #endif
  }

  void NextionTFT::IdleLoop() {
    if (ReadTFTCommand()) {
      ProcessPanelRequest();
      command_len = 0;
    }
    UpdateOnChange();
  }

  void NextionTFT::PrinterKilled(PGM_P error, PGM_P component) {
    SEND_TXT_END("page error");
    SEND_TXT("t3", "Error");
    SEND_TXT_P("t4", component);
    SEND_TXT_P("t5", error);
    SEND_TXT("t6", "Need reset");
  }

  void NextionTFT::PrintFinished() {
    SEND_TXT_END("page printfinished");
    setTargetTemp_celsius(0, E0);
    setTargetTemp_celsius(0, E1);
    setTargetTemp_celsius(0, BED);
    setTargetFan_percent(0, FAN0);
    // injectCommands_P(PSTR("G28 X Y"));
  }

  void NextionTFT::ConfirmationRequest(const char *const msg) {
    SEND_VALasTXT("tmppage.M117", msg);
  #if NEXDEBUG(N_MARLIN)
    SERIAL_ECHOLNPAIR("ConfirmationRequest() ", msg, " printer_state:", printer_state);
  #endif
  }

  void NextionTFT::StatusChange(const char *const msg) {
  #if NEXDEBUG(N_MARLIN)
    SERIAL_ECHOLNPAIR("StatusChange() ", msg);
    SERIAL_ECHOLNPAIR("printer_state:", printer_state);
  #endif
    SEND_VALasTXT("tmppage.M117", msg);
  }

  void NextionTFT::SendtoTFT(PGM_P str) { // A helper to print PROGMEN string to the panel
  #if NEXDEBUG(N_SOME)
    serialprintPGM(str);
  #endif
    while (const char c = pgm_read_byte(str++))
      LCD_SERIAL.write(c);
  }

  bool NextionTFT::ReadTFTCommand() {
    bool command_ready = false;
    while ((LCD_SERIAL.available() > 0) && (command_len < MAX_CMND_LEN)) {
      nextion_command[command_len] = LCD_SERIAL.read();
      if (nextion_command[command_len] == 10) {
        command_ready = true;
        break;
      }
      command_len++;
    }

    if (command_ready) {
      nextion_command[command_len] = 0x00;
      if (nextion_command[0] == 'G' || nextion_command[0] == 'M' || nextion_command[0] == 'T')
        injectCommands(nextion_command);
  #if NEXDEBUG(N_ALL)
      SERIAL_ECHOLNPAIR("< ", nextion_command);
  #endif
  #if NEXDEBUG(N_SOME)
      uint8_t req = atoi(&nextion_command[1]);
      if (req > 7 && req != 20) {
        SERIAL_ECHOLNPAIR("> ", nextion_command[0]);
        SERIAL_ECHOLNPAIR("> ", nextion_command[1]);
        SERIAL_ECHOLNPAIR("> ", nextion_command[2]);
        SERIAL_ECHOLNPAIR("> ", nextion_command[3]);
        SERIAL_ECHOLNPAIR("printer_state:", printer_state);
      }
  #endif
    }
    return command_ready;
  }

  void NextionTFT::SendFileList(int8_t startindex) {
  // respond to panel request for 7 files starting at index
  #if NEXDEBUG(N_INFO)
    SERIAL_ECHOLNPAIR("## SendFileList ## ", startindex);
  #endif
    filenavigator.getFiles(startindex);
  }

  void NextionTFT::SelectFile() {
    strncpy(selectedfile, nextion_command + 4, command_len - 4);
    selectedfile[command_len - 5] = '\0';
  #if NEXDEBUG(N_FILE)
    SERIAL_ECHOLNPAIR_F(" Selected File: ", selectedfile);
  #endif
    switch (selectedfile[0]) {
    case '/': // Valid file selected
              // SEND_TXT("tmppage.M117", msg_sd_file_open_success);
      break;
    case '<': // .. (go up folder level)
      filenavigator.upDIR();
      SendFileList(0);
      break;
    default: // enter sub folder
      filenavigator.changeDIR(selectedfile);
      SendFileList(0);
      break;
    }
  }

  void NextionTFT::_format_time(char *outstr, uint32_t time) {
    const uint8_t hrs = time / 3600,
                  min = (time / 60) % 60,
                  sec = time % 60;
    if (hrs)
      sprintf_P(outstr, PSTR("%02d:%02dm"), hrs, min);
    else
      sprintf_P(outstr, PSTR("%02d:%02ds"), min, sec);
  }

  void NextionTFT::ProcessPanelRequest() {
    // Break these up into logical blocks as its easier to navigate than one huge switch case!
    if (nextion_command[0] == 'X') {
      int8_t req = atoi(&nextion_command[1]);

      // Information requests
      if (req <= 49)
        PanelInfo(req);

      // Simple Actions
      else if (req >= 50)
        PanelAction(req);
    }
  }

  void NextionTFT::PanelInfo(uint8_t req) {
    switch (req) {
    case 0: //

      break;

    case 1: // Get SD Card list
      if (!isPrinting()) {
        if (!isMediaInserted())
          safe_delay(500);
        if (!isMediaInserted()) { // Make sure the card is removed
                                  // SEND_TXT("tmppage.M117", msg_no_sd_card);
        } else if (nextion_command[3] == 'S')
          SendFileList(atoi(&nextion_command[4]));
      }
      break;

    case 2: // Printer Info

      if (!isPrinting()) {
        SEND_VAL("tmppage.connected", 1);
        SEND_VALasTXT("tmppage.marlin", SHORT_BUILD_VERSION);
        SEND_VALasTXT("tmppage.compiled", __DATE__ " / " __TIME__);
        SEND_VALasTXT("tmppage.extruder", EXTRUDERS);
        SEND_VALasTXT("tmppage.printer", MACHINE_NAME);
        SEND_VALasTXT("tmppage.author", STRING_CONFIG_H_AUTHOR);
        SEND_VALasTXT("tmppage.released", STRING_DISTRIBUTION_DATE);
        SEND_VALasTXT("tmppage.bedx", X_BED_SIZE);
        SEND_VALasTXT("tmppage.bedy", Y_BED_SIZE);
        SEND_VALasTXT("tmppage.bedz", Z_MAX_POS);
        SEND_TEMP("tmppage.t0", ui8tostr3rj(getActualTemp_celsius(E0)), " / ", ui8tostr3rj(getTargetTemp_celsius(E0)));
        SEND_TEMP("tmppage.t1", ui8tostr3rj(getActualTemp_celsius(E1)), " / ", ui8tostr3rj(getTargetTemp_celsius(E1)));
        SEND_TEMP("tmppage.t2", ui8tostr3rj(getActualTemp_celsius(BED)), " / ", ui8tostr3rj(getTargetTemp_celsius(BED)));
        SEND_VALasTXT("tmppage.tool", getActiveTool());
        SEND_VALasTXT("tmppage.fan", ui8tostr3rj(getActualFan_percent(FAN0)));
        SEND_VALasTXT("tmppage.speed", getFeedrate_percent());
        SEND_VALasTXT("tmppage.flow", getFlowPercentage(getActiveTool()));
        SEND_VALasTXT("tmppage.progress", ui8tostr3rj(getProgress_percent()));
        SEND_VALasTXT("tmppage.layer", layer);
        SEND_VALasTXT("tmppage.x", getAxisPosition_mm(X));
        SEND_VALasTXT("tmppage.y", getAxisPosition_mm(Y));
        SEND_VALasTXT("tmppage.z", getAxisPosition_mm(Z));
        SEND_VAL("tmppage.homed", isPositionKnown());
        SEND_VAL("tmppage.homedx", isAxisPositionKnown(X));
        SEND_VAL("tmppage.homedy", isAxisPositionKnown(Y));
        SEND_VAL("tmppage.homedz", isAxisPositionKnown(Z));
  #if ENABLED(DUAL_X_CARRIAGE)
        SEND_VAL("tmppage.idexmode", getIDEX_Mode());
  #endif
        SEND_TXT("tmppage.M117", msg_welcome);
      }
      break;

    case 23: // Linear Advance
  #if ENABLED(LIN_ADVANCE)
      SEND_VALasTXT("linadvance", getLinearAdvance_mm_mm_s(getActiveTool()));
  #else
      SEND_TXT("linadvance", "n/a");
  #endif
      break;

    case 24: // TMC Motor Current
  #if HAS_TRINAMIC_CONFIG
      SEND_VALasTXT("x", getAxisCurrent_mA(X));
      SEND_VALasTXT("x2", getAxisCurrent_mA(X2));
      SEND_VALasTXT("y", getAxisCurrent_mA(Y));
      SEND_VALasTXT("y2", getAxisCurrent_mA(Y2));
      SEND_VALasTXT("z", getAxisCurrent_mA(Z));
      SEND_VALasTXT("z2", getAxisCurrent_mA(Z2));
      SEND_VALasTXT("e", getAxisCurrent_mA(E0));
      SEND_VALasTXT("e1", getAxisCurrent_mA(E1));
  #else
      SEND_TXT("x", "n/a");
      SEND_TXT("x2", "n/a");
      SEND_TXT("y", "n/a");
      SEND_TXT("y2", "n/a");
      SEND_TXT("z", "n/a");
      SEND_TXT("z2", "n/a");
      SEND_TXT("e", "n/a");
      SEND_TXT("e1", "n/a");
  #endif
      break;

    case 25: // TMC Bump Sensitivity
  #if HAS_TRINAMIC_CONFIG
      SEND_VALasTXT("x", getTMCBumpSensitivity(X));
      SEND_VALasTXT("x2", getTMCBumpSensitivity(X2));
      SEND_VALasTXT("y", getTMCBumpSensitivity(Y));
      SEND_VALasTXT("y2", getTMCBumpSensitivity(Y2));
      SEND_VALasTXT("z", getTMCBumpSensitivity(Z));
      SEND_VALasTXT("z2", getTMCBumpSensitivity(Z2));
  #else
      SEND_TXT("x", "n/a");
      SEND_TXT("x2", "n/a");
      SEND_TXT("y", "n/a");
      SEND_TXT("y2", "n/a");
      SEND_TXT("z", "n/a");
      SEND_TXT("z2", "n/a");

  #endif
      break;

    case 26: // TMC Hybrid Threshold Speed
  #if HAS_TRINAMIC_CONFIG
      SEND_VALasTXT("x", getAxisCurrent_mA(X));
      SEND_VALasTXT("x2", getAxisCurrent_mA(X2));
      SEND_VALasTXT("y", getAxisCurrent_mA(Y));
      SEND_VALasTXT("y2", getAxisCurrent_mA(Y2));
      SEND_VALasTXT("z", getAxisCurrent_mA(Z));
      SEND_VALasTXT("z2", getAxisCurrent_mA(Z2));
      SEND_VALasTXT("e", getAxisCurrent_mA(E0));
      SEND_VALasTXT("e1", getAxisCurrent_mA(E1));
  #else
      SEND_TXT("x", "n/a");
      SEND_TXT("x2", "n/a");
      SEND_TXT("y", "n/a");
      SEND_TXT("y2", "n/a");
      SEND_TXT("z", "n/a");
      SEND_TXT("z2", "n/a");
      SEND_TXT("e", "n/a");
      SEND_TXT("e1", "n/a");
  #endif
      break;

    case 27: // Printcounter
  #ifdef PRINTCOUNTER
      char buffer[21];
      SEND_VALasTXT("t5", getTotalPrints_str(buffer));
      SEND_VALasTXT("t3", getFinishedPrints_str(buffer));
      SEND_VALasTXT("t4", getFailedPrints_str(buffer));
      SEND_VALasTXT("t6", getTotalPrintTime_str(buffer));
      SEND_VALasTXT("t7", getLongestPrint_str(buffer));
      SEND_VALasTXT("t8", getFilamentUsed_str(buffer));
  #else
      SEND_TXT("t5", "n/a");
      SEND_TXT("t3", "n/a");
      SEND_TXT("t4", "n/a");
      SEND_TXT("t6", "n/a");
      SEND_TXT("t7", "n/a");
      SEND_TXT("t8", "n/a");
  #endif
      break;

    case 28: // Filament laod/unload
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
      SEND_VALasTXT("filamentin", fc_settings[getActiveTool()].load_length);
      SEND_VALasTXT("filamentout", fc_settings[getActiveTool()].unload_length);
  #else
      SEND_TXT("filamentin", "n/a");
      SEND_TXT("filamentout", "n/a");
  #endif
      break;

    case 29: // Preheat
  #if PREHEAT_COUNT
      if (!isPrinting()) {

        // Preheat PLA
        if (nextion_command[4] == 'P') {
          SEND_VALasTXT("pe", getMaterial_preset_E(0));
          #if HAS_HEATED_BED
          SEND_VALasTXT("pb", getMaterial_preset_B(0));
          #endif
        }

        // Preheat ABS
        if (nextion_command[4] == 'A') {
          SEND_VALasTXT("ae", getMaterial_preset_E(1));
          #if HAS_HEATED_BED
          SEND_VALasTXT("ab", getMaterial_preset_B(1));
          #endif
        }

        // Preheat PETG
        if (nextion_command[4] == 'G') {
    #ifdef PREHEAT_3_TEMP_HOTEND
          SEND_VALasTXT("ge", getMaterial_preset_E(2));
          #if HAS_HEATED_BED
          SEND_VALasTXT("gb", getMaterial_preset_B(2));
          #endif
    #endif
        }
      }
  #endif
      break;

    case 30: // velocity
      SEND_VALasTXT("x", getAxisMaxFeedrate_mm_s(X));
      SEND_VALasTXT("y", getAxisMaxFeedrate_mm_s(Y));
      SEND_VALasTXT("z", getAxisMaxFeedrate_mm_s(Z));
      SEND_VALasTXT("e", getAxisMaxFeedrate_mm_s(getActiveTool()));
      SEND_VALasTXT("min", getMinFeedrate_mm_s());
      SEND_VALasTXT("tmin", getMinTravelFeedrate_mm_s());
      break;

    case 31: // jerk
  #if HAS_JUNCTION_DEVIATION
        // SEND_VALasTXT("x", getJunctionDeviation_mm());
      SEND_TXT("tmppage.M117", "classic Jerk not enabled");
      SEND_TXT("x", "n/a");
      SEND_TXT("y", "n/a");
      SEND_TXT("z", "n/a");
      SEND_TXT("e", "n/a");
  #else
      SEND_VALasTXT("x", getAxisMaxJerk_mm_s(X));
      SEND_VALasTXT("y", getAxisMaxJerk_mm_s(Y));
      SEND_VALasTXT("z", getAxisMaxJerk_mm_s(Z));
      SEND_VALasTXT("e", getAxisMaxJerk_mm_s(getActiveTool()));
  #endif
      break;

    case 32: // stepsmm
      SEND_VALasTXT("x", getAxisSteps_per_mm(X));
      SEND_VALasTXT("y", getAxisSteps_per_mm(Y));
      SEND_VALasTXT("z", getAxisSteps_per_mm(Z));
      SEND_VALasTXT("e0", getAxisSteps_per_mm(E0));
      SEND_VALasTXT("e1", getAxisSteps_per_mm(E1));
      break;

    case 33: // acceleration
      SEND_VALasTXT("x", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(X)));
      SEND_VALasTXT("y", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(Y)));
      SEND_VALasTXT("z", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(Z)));
      SEND_VALasTXT("e", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(getActiveTool())));
      SEND_VALasTXT("print", ui16tostr5rj(getPrintingAcceleration_mm_s2()));
      SEND_VALasTXT("retract", ui16tostr5rj(getRetractAcceleration_mm_s2()));
      SEND_VALasTXT("travel", ui16tostr5rj(getTravelAcceleration_mm_s2()));
      break;

    case 34: // tool offset
  #ifdef DUAL_X_CARRIAGE
      SEND_VALasTXT("x", getNozzleOffset_mm(X, getActiveTool()));
      SEND_VALasTXT("y", getNozzleOffset_mm(Y, getActiveTool()));
      SEND_VALasTXT("z", getNozzleOffset_mm(Z, getActiveTool()));
  #else
      SEND_TXT("x", "n/a");
      SEND_TXT("y", "n/a");
      SEND_TXT("z", "n/a");
  #endif
      break;

    case 35: // z probe offset
  #if HAS_PROBE_XY_OFFSET
      SEND_VALasTXT("x", getProbeOffset_mm(X));
      SEND_VALasTXT("y", getProbeOffset_mm(Y));
  #else
      SEND_TXT("x", "n/a");
      SEND_TXT("y", "n/a");
  #endif
      SEND_VALasTXT("z", getZOffset_mm());
      break;

    case 36: // Endstop Info
  #if HAS_X_MIN
      SEND_VALasTXT("x1", READ(X_MIN_PIN) != X_MIN_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_X_MAX
      SEND_VALasTXT("x2", READ(X_MAX_PIN) != X_MAX_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_Y_MIN
      SEND_VALasTXT("y1", READ(Y_MIN_PIN) != Y_MIN_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_Z_MIN
      SEND_VALasTXT("z1", READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_Z_MAX
      SEND_VALasTXT("z2", READ(Z_MAX_PIN) != Z_MAX_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_Z2_MIN
      SEND_VALasTXT("z2", READ(Z2_MIN_PIN) != Z2_MIN_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_Z2_MAX
      SEND_VALasTXT("z2", READ(Z2_MAX_PIN) != Z2_MAX_ENDSTOP_INVERTING ? "triggered" : "open");
  #endif
  #if HAS_BED_PROBE
        //  SEND_VALasTXT("bltouch", READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING ? "triggered" : "open");
  #else
      SEND_TXT("bltouch", "n/a");
  #endif
      break;

    case 37: // PID
      SEND_VALasTXT("p0", getPIDValues_Kp(E0));
      SEND_VALasTXT("i0", getPIDValues_Ki(E0));
      SEND_VALasTXT("d0", getPIDValues_Kd(E0));
  #if (EXTRUDERS == 2)
      SEND_VALasTXT("p1", getPIDValues_Kp(E1));
      SEND_VALasTXT("i1", getPIDValues_Ki(E1));
      SEND_VALasTXT("d1", getPIDValues_Kd(E1));
  #else
      SEND_TXT("p1", "n/a");
      SEND_TXT("i1", "n/a");
      SEND_TXT("d1", "n/a");
  #endif
  #if ENABLED(PIDTEMPBED)
      SEND_VALasTXT("hbp", getBedPIDValues_Kp());
      SEND_VALasTXT("hbi", getBedPIDValues_Ki());
      SEND_VALasTXT("hbd", getBedPIDValues_Kd());
  #else
      SEND_TXT("hbp", "n/a");
      SEND_TXT("hbi", "n/a");
      SEND_TXT("hbd", "n/a");
  #endif
      break;
    }
  }

  void NextionTFT::PanelAction(uint8_t req) {
    switch (req) {

    case 50: // Pause SD print
             //if (isPrintingFromMedia()) {
      //SEND_TXT("tmppage.M117", "Paused");
      pausePrint();
      SEND_TXT_END("qpause.picc=29");
      //}
      break;

    case 51: // Resume SD Print
      resumePrint();
      SEND_TXT_END("qpause.picc=28");
      break;

    case 52: // Stop SD print
             // if (isPrintingFromMedia()) {
      stopPrint();
      SEND_TXT_END("page prepare");
      // }
      break;

    case 54: // A13 Select file
      SelectFile();
      break;

    case 65: // Cool Down
      if (!isPrinting()) {
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, E1);
        setTargetTemp_celsius(0, BED);
        setTargetFan_percent(0, FAN0);
      }
      break;

    case 66: // Refresh SD
      if (!isPrinting()) {
        injectCommands_P(PSTR("M21"));
        filenavigator.reset();
      }
      break;

    case 56: // Set Fan, Flow, Print Speed
      if (nextion_command[4] == 'S') {
        setTargetFan_percent(atof(&nextion_command[5]), FAN0);
      }

      if (nextion_command[4] == 'P') {
        setFeedrate_percent(atoi(&nextion_command[5]));
      }

      if (nextion_command[4] == 'F') {
        setFlow_percent(atoi(&nextion_command[5]), getActiveTool());
      }

      break;

    case 57: // disable Motors
      if (!isPrinting()) {
        disable_all_steppers(); // from marlincore.h
        SEND_TXT("tmppage.M117", "Motors disabled");
      }
      break;

    case 58: // Load/Unload Filament
      if (canMove(getActiveTool())) {
        if (nextion_command[4] == 'L') {
          injectCommands_P(PSTR("M701"));
        }
        if (nextion_command[4] == 'U') {
          injectCommands_P(PSTR("M702"));
        }
      } else {
        SEND_TXT("tmppage.M117", "Preheat first");
        SEND_TXT_END("page preheat");
      }
      break;

    case 63: // Preheat // Temps defined in configuration.h
  #if PREHEAT_COUNT
      if (!isPrinting()) {

        // Preheat PLA
        if (nextion_command[4] == 'P') {
          #if HAS_HEATED_BED
          setTargetTemp_celsius(getMaterial_preset_B(0), BED);
          #endif
          setTargetTemp_celsius(getMaterial_preset_E(0), getActiveTool());
        }
        // Preheat ABS
        if (nextion_command[4] == 'A') {
          #if HAS_HEATED_BED
          setTargetTemp_celsius(getMaterial_preset_B(1), BED);
          #endif
          setTargetTemp_celsius(getMaterial_preset_E(1), getActiveTool());
        }
        // Preheat PETG
        if (nextion_command[4] == 'G') {
          #if HAS_HEATED_BED
          setTargetTemp_celsius(getMaterial_preset_B(2), BED);
          #endif
          setTargetTemp_celsius(getMaterial_preset_E(2), getActiveTool());
        }
      }
  #endif
      break;
    }
  }

  void NextionTFT::UpdateOnChange() {
    const millis_t ms = millis();
    static millis_t next_event_ms = 0;
    // tmppage Temperature
    if (!WITHIN(last_degHotend0 - getActualTemp_celsius(E0), -0.2, 0.2) || !WITHIN(last_degTargetHotend0 - getTargetTemp_celsius(E0), -0.5, 0.5)) {
      SEND_TEMP("tmppage.t0", ui8tostr3rj(getActualTemp_celsius(E0)), " / ", ui8tostr3rj(getTargetTemp_celsius(E0)));
      last_degHotend0 = getActualTemp_celsius(E0);
      last_degTargetHotend0 = getTargetTemp_celsius(E0);
    }

    if (!WITHIN(last_degHotend1 - getActualTemp_celsius(E1), -0.2, 0.2) || !WITHIN(last_degTargetHotend1 - getTargetTemp_celsius(E1), -0.5, 0.5)) {
      SEND_TEMP("tmppage.t1", ui8tostr3rj(getActualTemp_celsius(E1)), " / ", ui8tostr3rj(getTargetTemp_celsius(E1)));
      last_degHotend1 = getActualTemp_celsius(E1);
      last_degTargetHotend1 = getTargetTemp_celsius(E1);
    }

    if (!WITHIN(last_degBed - getActualTemp_celsius(BED), -0.2, 0.2) || !WITHIN(last_degTargetBed - getTargetTemp_celsius(BED), -0.5, 0.5)) {
      SEND_TEMP("tmppage.t2", ui8tostr3rj(getActualTemp_celsius(BED)), " / ", ui8tostr3rj(getTargetTemp_celsius(BED)));
      last_degBed = getActualTemp_celsius(BED);
      last_degTargetBed = getTargetTemp_celsius(BED);
    }

    // tmppage Tool
    if (last_active_extruder != getActiveTool()) {
      SEND_VALasTXT("tmppage.tool", getActiveTool());
      last_active_extruder = getActiveTool();
    }

    // tmppage Fan Speed
    if (last_fan_speed != getActualFan_percent(FAN0)) {
      SEND_VALasTXT("tmppage.fan", ui8tostr3rj(getActualFan_percent(FAN0)));
      last_fan_speed = getActualFan_percent(FAN0);
    }

    // tmppage Print Speed
    if (last_print_speed != getFeedrate_percent()) {
      SEND_VALasTXT("tmppage.speed", ui8tostr3rj(getFeedrate_percent()));
      last_print_speed = getFeedrate_percent();
    }

    // tmppage Flow
    if (last_flow_speed != getFlowPercentage(getActiveTool())) {
      SEND_VALasTXT("tmppage.flow", getFlowPercentage(getActiveTool()));
      last_flow_speed = getFlowPercentage(getActiveTool());
    }

    // tmppage Progress + Layer + Time
    if (isPrinting()) {

      if (ELAPSED(ms, next_event_ms)) {
        next_event_ms = ms + 1000;
  #if ENABLED(SHOW_REMAINING_TIME)
        const uint32_t remaining = getProgress_seconds_remaining();
        char remaining_str[10];
        _format_time(remaining_str, remaining);
        SEND_VALasTXT("tmppage.remaining", remaining_str);
  #endif
        const uint32_t elapsed = getProgress_seconds_elapsed();
        char elapsed_str[10];
        _format_time(elapsed_str, elapsed);
        SEND_VALasTXT("tmppage.elapsed", elapsed_str);
      }

      if (last_progress != getProgress_percent()) {
        SEND_VALasTXT("tmppage.progress", ui8tostr3rj(getProgress_percent()));
        last_progress = getProgress_percent();
      }

      if (last_get_axis_position_mmZ < getAxisPosition_mm(Z)) {
        layer++;
        SEND_VALasTXT("tmppage.layer", layer);
      }

      if (last_get_axis_position_mmZ > getAxisPosition_mm(Z)) {
        layer--;
        SEND_VALasTXT("tmppage.layer", layer);
      }
    }

    // tmppage Axis
    if (!WITHIN(last_get_axis_position_mmX - getAxisPosition_mm(X), -0.1, 0.1)) {
      if (ELAPSED(ms, next_event_ms)) {
        next_event_ms = ms + 30;
        SEND_VALasTXT("tmppage.x", getAxisPosition_mm(X));
        last_get_axis_position_mmX = getAxisPosition_mm(X);
      }
    }

    if (!WITHIN(last_get_axis_position_mmY - getAxisPosition_mm(Y), -0.1, 0.1)) {
      if (ELAPSED(ms, next_event_ms)) {
        next_event_ms = ms + 30;
        SEND_VALasTXT("tmppage.y", getAxisPosition_mm(Y));
        last_get_axis_position_mmY = getAxisPosition_mm(Y);
      }
    }

    if (!WITHIN(last_get_axis_position_mmZ - getAxisPosition_mm(Z), -0.1, 0.1)) {
      SEND_VALasTXT("tmppage.z", getAxisPosition_mm(Z));
      last_get_axis_position_mmZ = getAxisPosition_mm(Z);
    }

    // tmppage homed
    if (last_homed != isPositionKnown()) {
      SEND_VAL("tmppage.homed", isPositionKnown());
      last_homed = isPositionKnown();
    }
    if (last_homedX != isAxisPositionKnown(X)) {
      SEND_VAL("tmppage.homedx", isAxisPositionKnown(X));
      last_homedX = isAxisPositionKnown(X);
    }
    if (last_homedY != isAxisPositionKnown(Y)) {
      SEND_VAL("tmppage.homedy", isAxisPositionKnown(Y));
      last_homedY = isAxisPositionKnown(Y);
    }
    if (last_homedZ != isAxisPositionKnown(Z)) {
      SEND_VAL("tmppage.homedz", isAxisPositionKnown(Z));
      last_homedZ = isAxisPositionKnown(Z);
    }

  // tmppage IDEX Mode
  #if ENABLED(DUAL_X_CARRIAGE)
    if (last_IDEX_Mode != getIDEX_Mode()) {
      SEND_VAL("tmppage.idexmode", getIDEX_Mode());
      last_IDEX_Mode = getIDEX_Mode();
    }
  #endif
  }
} // namespace Nextion

#endif // NEXTION_TFT
