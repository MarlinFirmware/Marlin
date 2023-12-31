/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/nextion/nextion_tft.h
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(NEXTION_TFT)

#include "../../../MarlinCore.h"
#include "../../../feature/pause.h"
#include "../../../module/stepper.h"
#include "../../../gcode/queue.h"
#include "../../../libs/numtostr.h"
#include "../../../sd/cardreader.h"
#include "FileNavigator.h"
#include "nextion_tft.h"

#define DEBUG_OUT NEXDEBUGLEVEL
#include "../../../core/debug_out.h"

char NextionTFT::selectedfile[MAX_PATH_LEN];
char NextionTFT::nextion_command[MAX_CMND_LEN];
uint8_t NextionTFT::command_len;

uint32_t layer = 0;

NextionTFT nextion;

NextionTFT::NextionTFT() {}

void NextionTFT::startup() {
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

  DEBUG_ECHOLNPGM("Nextion Debug Level ", NEXDEBUGLEVEL);
}

void NextionTFT::idleLoop() {
  if (readTFTCommand()) {
    processPanelRequest();
    command_len = 0;
  }
  UpdateOnChange();
}

void NextionTFT::printerKilled(FSTR_P const error, FSTR_P const component) {
  SEND_TXT_END("page error");
  SEND_TXT_F("t3", F("Error"));
  SEND_TXT_F("t4", component);
  SEND_TXT_F("t5", error);
  SEND_TXT_F("t6", F("Need reset"));
}

void NextionTFT::PrintFinished() {
  SEND_TXT_END("page printfinished");
}

void NextionTFT::confirmationRequest(const char * const msg) {
  SEND_VALasTXT("tmppage.M117", msg);
  #if NEXDEBUG(N_MARLIN)
    DEBUG_ECHOLNPGM("confirmationRequest() ", msg, " printer_state:", printer_state);
  #endif
}

void NextionTFT::statusChange(const char * const msg) {
  #if NEXDEBUG(N_MARLIN)
    DEBUG_ECHOLNPGM("statusChange() ", msg, "\nprinter_state:", printer_state);
  #endif
  SEND_VALasTXT("tmppage.M117", msg);
}

void NextionTFT::tftSend(FSTR_P const fstr/*=nullptr*/) { // A helper to print PROGMEM string to the panel
  #if NEXDEBUG(N_SOME)
    DEBUG_ECHO(fstr);
  #endif
  PGM_P str = FTOP(fstr);
  while (const char c = pgm_read_byte(str++)) LCD_SERIAL.write(c);
}

bool NextionTFT::readTFTCommand() {
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
      DEBUG_ECHOLNPGM("< ", nextion_command);
    #endif
    #if NEXDEBUG(N_SOME)
      uint8_t req = atoi(&nextion_command[1]);
      if (req > 7 && req != 20)
        DEBUG_ECHOLNPGM(  "> ", C(nextion_command[0]),
                         "\n> ", C(nextion_command[1]),
                         "\n> ", C(nextion_command[2]),
                         "\n> ", C(nextion_command[3]),
                         "\nprinter_state:", printer_state);
    #endif
  }
  return command_ready;
}

void NextionTFT::sendFileList(int8_t startindex) {
  // respond to panel request for 7 files starting at index
  #if NEXDEBUG(N_INFO)
    DEBUG_ECHOLNPGM("## sendFileList ## ", startindex);
  #endif
  filenavigator.getFiles(startindex);
}

void NextionTFT::selectFile() {
  strlcpy(selectedfile, nextion_command + 4, command_len - 3);
  #if NEXDEBUG(N_FILE)
    DEBUG_ECHOLNPGM(" Selected File: ", selectedfile);
  #endif
  switch (selectedfile[0]) {
    case '/': // Valid file selected
      //SEND_TXT("tmppage.M117", msg_sd_file_open_success);
      break;
    case '<': // .. (go up folder level)
      filenavigator.upDIR();
      sendFileList(0);
      break;
    default: // enter sub folder
      filenavigator.changeDIR(selectedfile);
      sendFileList(0);
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

void NextionTFT::processPanelRequest() {
  // Break these up into logical blocks as its easier to navigate than one huge switch case!
  if (nextion_command[0] == 'X') {
    int8_t req = atoi(&nextion_command[1]);

    // Information requests
    if (req <= 49)
      panelInfo(req);

    // Simple Actions
    else if (req >= 50)
      panelAction(req);
  }
}

#define SEND_NA(A) SEND_TXT(A, "n/a")

void NextionTFT::panelInfo(uint8_t req) {
  switch (req) {
  case 0: break;

  case 1: // Get SD Card list
    if (!isPrinting()) {
      if (!isMediaInserted()) safe_delay(500);
      if (!isMediaInserted()) { // Make sure the card is removed
        //SEND_TXT("tmppage.M117", msg_no_sd_card);
      }
      else if (nextion_command[3] == 'S')
        sendFileList(atoi(&nextion_command[4]));
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
      SEND_VALasTXT("tmppage.flow", getFlow_percent(getActiveTool()));
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
      SEND_NA("linadvance");
    #endif
    break;

  case 24: // TMC Motor Current
    #if HAS_TRINAMIC_CONFIG
      #define SEND_TRINAMIC_CURR(A, B) SEND_VALasTXT(A, getAxisCurrent_mA(B))
    #else
      #define SEND_TRINAMIC_CURR(A, B) SEND_NA(A)
    #endif
    SEND_TRINAMIC_CURR("x",  X);
    SEND_TRINAMIC_CURR("x2", X2);
    SEND_TRINAMIC_CURR("y",  Y);
    SEND_TRINAMIC_CURR("y2", Y2);
    SEND_TRINAMIC_CURR("z",  Z);
    SEND_TRINAMIC_CURR("z2", Z2);
    SEND_TRINAMIC_CURR("e",  E0);
    SEND_TRINAMIC_CURR("e1", E1);
    break;

  case 25: // TMC Bump Sensitivity
    #if HAS_TRINAMIC_CONFIG
      #define SEND_TRINAMIC_BUMP(A, B) SEND_VALasTXT(A, getTMCBumpSensitivity(B))
    #else
      #define SEND_TRINAMIC_BUMP(A, B) SEND_NA(A)
    #endif
    SEND_TRINAMIC_BUMP("x",  X);
    SEND_TRINAMIC_BUMP("x2", X2);
    SEND_TRINAMIC_BUMP("y",  Y);
    SEND_TRINAMIC_BUMP("y2", Y2);
    SEND_TRINAMIC_BUMP("z",  Z);
    SEND_TRINAMIC_BUMP("z2", Z2);
    break;

  case 26: // TMC Hybrid Threshold Speed
    #if 0 && ALL(HAS_TRINAMIC_CONFIG, HYBRID_THRESHOLD)
      #define SEND_TRINAMIC_THRS(A, B) SEND_VALasTXT(A, getAxisPWMthrs(B))
    #else
      #define SEND_TRINAMIC_THRS(A, B) SEND_NA(A)
    #endif
    SEND_TRINAMIC_THRS("x",  X);
    SEND_TRINAMIC_THRS("x2", X2);
    SEND_TRINAMIC_THRS("y",  Y);
    SEND_TRINAMIC_THRS("y2", Y2);
    SEND_TRINAMIC_THRS("z",  Z);
    SEND_TRINAMIC_THRS("z2", Z2);
    SEND_TRINAMIC_THRS("e",  E0);
    SEND_TRINAMIC_THRS("e1", E1);
    break;

  case 27: // Printcounter
    #if ENABLED(PRINTCOUNTER)
      char buffer[21];
      #define SEND_PRINT_INFO(A, B) SEND_VALasTXT(A, B(buffer))
    #else
      #define SEND_PRINT_INFO(A, B) SEND_NA(A)
    #endif
    SEND_PRINT_INFO("t5", getTotalPrints_str);
    SEND_PRINT_INFO("t3", getFinishedPrints_str);
    SEND_PRINT_INFO("t4", getFailedPrints_str);
    SEND_PRINT_INFO("t6", getTotalPrintTime_str);
    SEND_PRINT_INFO("t7", getLongestPrint_str);
    SEND_PRINT_INFO("t8", getFilamentUsed_str);
    break;

  case 28: // Filament laod/unload
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      #define SEND_PAUSE_INFO(A, B) SEND_VALasTXT(A, fc_settings[getActiveTool()].B)
    #else
      #define SEND_PAUSE_INFO(A, B) SEND_NA(A)
    #endif
    SEND_PAUSE_INFO("filamentin", load_length);
    SEND_PAUSE_INFO("filamentout", unload_length);
    break;

  case 29: // Preheat
    #if HAS_PREHEAT
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

  case 30: // Velocity
    SEND_VALasTXT("x",    getAxisMaxFeedrate_mm_s(X));
    SEND_VALasTXT("y",    getAxisMaxFeedrate_mm_s(Y));
    SEND_VALasTXT("z",    getAxisMaxFeedrate_mm_s(Z));
    SEND_VALasTXT("e",    getAxisMaxFeedrate_mm_s(getActiveTool()));
    SEND_VALasTXT("min",  getMinFeedrate_mm_s());
    SEND_VALasTXT("tmin", getMinTravelFeedrate_mm_s());
    break;

  case 31: // Jerk
    #if ENABLED(CLASSIC_JERK)
      #define SEND_JERK_INFO(A, B) SEND_VALasTXT(A, getAxisMaxJerk_mm_s(B))
    #else
      #define SEND_JERK_INFO(A, B) SEND_NA(A)
      //SEND_VALasTXT("x", getJunctionDeviation_mm());
      SEND_TXT("tmppage.M117", "classic Jerk not enabled");
    #endif
    SEND_JERK_INFO("x", X);
    SEND_JERK_INFO("y", Y);
    SEND_JERK_INFO("z", Z);
    SEND_JERK_INFO("e", getActiveTool());
    break;

  case 32: // Steps-per-mm
    SEND_VALasTXT("x",  getAxisSteps_per_mm(X));
    SEND_VALasTXT("y",  getAxisSteps_per_mm(Y));
    SEND_VALasTXT("z",  getAxisSteps_per_mm(Z));
    SEND_VALasTXT("e0", getAxisSteps_per_mm(E0));
    SEND_VALasTXT("e1", getAxisSteps_per_mm(E1));
    break;

  case 33: // Acceleration
    SEND_VALasTXT("x", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(X)));
    SEND_VALasTXT("y", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(Y)));
    SEND_VALasTXT("z", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(Z)));
    SEND_VALasTXT("e", ui16tostr5rj(getAxisMaxAcceleration_mm_s2(getActiveTool())));
    SEND_VALasTXT("print",   ui16tostr5rj(getPrintingAcceleration_mm_s2()));
    SEND_VALasTXT("retract", ui16tostr5rj(getRetractAcceleration_mm_s2()));
    SEND_VALasTXT("travel",  ui16tostr5rj(getTravelAcceleration_mm_s2()));
    break;

  case 34: // Dual X carriage offset
    #if ENABLED(DUAL_X_CARRIAGE)
      #define SEND_IDEX_INFO(A, B) SEND_VALasTXT(A, getNozzleOffset_mm(B, getActiveTool()))
    #else
      #define SEND_IDEX_INFO(A, B) SEND_NA(A)
    #endif
    SEND_IDEX_INFO("x", X);
    SEND_IDEX_INFO("y", Y);
    SEND_IDEX_INFO("z", Z);
    break;

  case 35: // Probe offset
    #if HAS_PROBE_XY_OFFSET
      #define SEND_PROBE_INFO(A, B) SEND_VALasTXT(A, getProbeOffset_mm(B))
    #else
      #define SEND_PROBE_INFO(A, B) SEND_NA(A)
    #endif
    SEND_PROBE_INFO("x", X);
    SEND_PROBE_INFO("y", Y);
    SEND_VALasTXT("z", getZOffset_mm());
    break;

  case 36: // Endstop Info
    #if X_HOME_TO_MIN
      SEND_VALasTXT("x1", READ(X_MIN_PIN) == X_MIN_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #elif X_HOME_TO_MAX
      SEND_VALasTXT("x2", READ(X_MAX_PIN) == X_MAX_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #endif
    #if Y_HOME_TO_MIN
      SEND_VALasTXT("y1", READ(Y_MIN_PIN) == Y_MIN_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #elif Y_HOME_TO_MAX
      SEND_VALasTXT("y2", READ(X_MAX_PIN) == Y_MAX_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #endif
    #if Z_HOME_TO_MIN
      SEND_VALasTXT("z1", READ(Z_MIN_PIN) == Z_MIN_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #elif Z_HOME_TO_MAX
      SEND_VALasTXT("z2", READ(Z_MAX_PIN) == Z_MAX_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #endif
    #if USE_Z2_MIN
      SEND_VALasTXT("z2", READ(Z2_MIN_PIN) == Z2_MIN_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #elif USE_Z2_MAX
      SEND_VALasTXT("z2", READ(Z2_MAX_PIN) == Z2_MAX_ENDSTOP_HIT_STATE ? "triggered" : "open");
    #endif
    #if HAS_BED_PROBE
      //SEND_VALasTXT("bltouch", PROBE_TRIGGERED() ? "triggered" : "open");
    #else
      SEND_NA("bltouch");
    #endif
    break;

  case 37: // PID
    #if ENABLED(PIDTEMP)
      #define SEND_PID_INFO_0(A, B) SEND_VALasTXT(A, getPID_K##B(E0))
    #else
      #define SEND_PID_INFO_0(A, B) SEND_NA(A)
    #endif
    #if ALL(PIDTEMP, HAS_MULTI_EXTRUDER)
      #define SEND_PID_INFO_1(A, B) SEND_VALasTXT(A, getPID_K##B(E1))
    #else
      #define SEND_PID_INFO_1(A, B) SEND_NA(A)
    #endif
    #if ENABLED(PIDTEMPBED)
      #define SEND_PID_INFO_BED(A, B) SEND_VALasTXT(A, getBedPID_K##B())
    #else
      #define SEND_PID_INFO_BED(A, B) SEND_NA(A)
    #endif
    SEND_PID_INFO_0("p0", p);
    SEND_PID_INFO_0("i0", i);
    SEND_PID_INFO_0("d0", d);

    SEND_PID_INFO_1("p1", p);
    SEND_PID_INFO_1("i1", i);
    SEND_PID_INFO_1("d1", d);

    SEND_PID_INFO_BED("hbp", p);
    SEND_PID_INFO_BED("hbi", i);
    SEND_PID_INFO_BED("hbd", d);
    break;
  }
}

void NextionTFT::panelAction(uint8_t req) {
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
      //if (isPrintingFromMedia()) {
        stopPrint();
        SEND_TXT_END("page prepare");
      //}
      break;

    case 54: // A13 Select file
      selectFile();
      break;

    case 65: // Cool Down
      if (!isPrinting()) coolDown();
      break;

    case 66: // Refresh SD
      if (!isPrinting()) {
        injectCommands(F("M21"));
        filenavigator.reset();
      }
      break;

    case 56: // Set Fan, Flow, Print Speed
      switch (nextion_command[4]) {
        case 'S': setTargetFan_percent(atof(&nextion_command[5]), FAN0); break;
        case 'P': setFeedrate_percent(atoi(&nextion_command[5])); break;
        case 'F': setFlow_percent(atoi(&nextion_command[5]), getActiveTool()); break;
      }
      break;

    case 57: // Disable Motors
      if (!isPrinting()) {
        stepper.disable_all_steppers();
        SEND_TXT("tmppage.M117", "Motors disabled");
      }
      break;

    case 58: // Load/Unload Filament
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        if (canMove(getActiveTool())) {
          switch (nextion_command[4]) {
            case 'L': injectCommands(F("M701")); break;
            case 'U': injectCommands(F("M702")); break;
          }
        }
        else {
          SEND_TXT("tmppage.M117", "Preheat first");
          SEND_TXT_END("page preheat");
        }
      #else
        SEND_TXT("tmppage.M117", "Filament loading disabled");
      #endif
      break;

    case 63: // Preheat // Temps defined in configuration.h
      #if HAS_PREHEAT
        if (!isPrinting()) switch (nextion_command[4]) {
          // Preheat PLA
          case 'P':
            #if HAS_HEATED_BED
              setTargetTemp_celsius(getMaterial_preset_B(0), BED);
            #endif
            setTargetTemp_celsius(getMaterial_preset_E(0), getActiveTool());
            break;

          // Preheat ABS
          case 'A':
            #if HAS_HEATED_BED
              setTargetTemp_celsius(getMaterial_preset_B(1), BED);
            #endif
            setTargetTemp_celsius(getMaterial_preset_E(1), getActiveTool());
            break;

          // Preheat PETG
          case 'G':
            #if HAS_HEATED_BED
              setTargetTemp_celsius(getMaterial_preset_B(2), BED);
            #endif
            setTargetTemp_celsius(getMaterial_preset_E(2), getActiveTool());
            break;
        }
      #else
        SEND_TXT("tmppage.M117", "Preheat disabled");
      #endif
      break;
  }
}

void NextionTFT::UpdateOnChange() {
  const millis_t ms = millis();
  static millis_t next_event_ms = 0;
  static celsius_float_t last_degBed = 999, last_degHotend0 = 999, last_degHotend1 = 999,
                         last_degTargetBed = 999, last_degTargetHotend0 = 999, last_degTargetHotend1 = 999;

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
  static uint8_t last_active_extruder = 99;
  if (last_active_extruder != getActiveTool()) {
    SEND_VALasTXT("tmppage.tool", getActiveTool());
    last_active_extruder = getActiveTool();
  }

  // tmppage Fan Speed
  static uint8_t last_fan_speed = 99;
  if (last_fan_speed != getActualFan_percent(FAN0)) {
    SEND_VALasTXT("tmppage.fan", ui8tostr3rj(getActualFan_percent(FAN0)));
    last_fan_speed = getActualFan_percent(FAN0);
  }

  // tmppage Print Speed
  static uint8_t last_print_speed = 99;
  if (last_print_speed != getFeedrate_percent()) {
    SEND_VALasTXT("tmppage.speed", ui8tostr3rj(getFeedrate_percent()));
    last_print_speed = getFeedrate_percent();
  }

  // tmppage Flow
  static uint8_t last_flow_speed = 99;
  if (last_flow_speed != getFlow_percent(getActiveTool())) {
    SEND_VALasTXT("tmppage.flow", getFlow_percent(getActiveTool()));
    last_flow_speed = getFlow_percent(getActiveTool());
  }

  // tmppage Axis
  static float last_get_axis_position_mmX = 999, last_get_axis_position_mmY = 999, last_get_axis_position_mmZ = 999;

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

    static uint8_t last_progress = 99;
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
  static bool last_homed = false, last_homedX = false, last_homedY = false, last_homedZ = false;

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

  #if ENABLED(DUAL_X_CARRIAGE)
    // tmppage IDEX Mode
    static uint8_t last_IDEX_Mode = 99;
    if (last_IDEX_Mode != getIDEX_Mode()) {
      SEND_VAL("tmppage.idexmode", getIDEX_Mode());
      last_IDEX_Mode = getIDEX_Mode();
    }
  #endif
}

#endif // NEXTION_TFT
