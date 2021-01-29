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

/**
 * lcd/extui/lib/chiron_tft.cpp
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_CHIRON)

#include "chiron_tft.h"
#include "Tunes.h"
#include "FileNavigator.h"

#include "../../../../gcode/queue.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/numtostr.h"
#include "../../../../MarlinCore.h"

namespace Anycubic {

  printer_state_t  ChironTFT::printer_state;
  paused_state_t   ChironTFT::pause_state;
  heater_state_t   ChironTFT::hotend_state;
  heater_state_t   ChironTFT::hotbed_state;
  xy_uint8_t       ChironTFT::selectedmeshpoint;
  char             ChironTFT::selectedfile[MAX_PATH_LEN];
  char             ChironTFT::panel_command[MAX_CMND_LEN];
  uint8_t          ChironTFT::command_len;
  float            ChironTFT::live_Zoffset;
  file_menu_t      ChironTFT::file_menu;

  ChironTFT Chiron;

  ChironTFT::ChironTFT(){}

  void ChironTFT::Startup() {
    selectedfile[0]   = '\0';
    panel_command[0]  = '\0';
    command_len       = 0;
    printer_state     = AC_printer_idle;
    pause_state       = AC_paused_idle;
    hotend_state      = AC_heater_off;
    hotbed_state      = AC_heater_off;
    live_Zoffset      = 0.0;
    file_menu         = AC_menu_file;

    // Setup pins for powerloss detection
    // Two IO pins are connected on the Trigorilla Board
    // On a power interruption the OUTAGECON_PIN goes low.

    #if ENABLED(POWER_LOSS_RECOVERY)
      OUT_WRITE(OUTAGECON_PIN, HIGH);
    #endif

    // Filament runout is handled by Marlin settings in Configuration.h
    // opt_set    FIL_RUNOUT_STATE HIGH  // Pin state indicating that filament is NOT present.
    // opt_enable FIL_RUNOUT_PULLUP

    TFTSer.begin(115200);

    // Signal Board has reset
    SendtoTFTLN(AC_msg_main_board_has_reset);

    safe_delay(200);

    // Enable leveling and Disable end stops during print
    // as Z home places nozzle above the bed so we need to allow it past the end stops
    injectCommands_P(AC_cmnd_enable_leveling);

    // Startup tunes are defined in Tunes.h
    //PlayTune(BEEPER_PIN, Anycubic_PowerOn, 1);
    PlayTune(BEEPER_PIN, GB_PowerOn, 1);
    #if ACDEBUGLEVEL
      SERIAL_ECHOLNPAIR("AC Debug Level ", ACDEBUGLEVEL);
    #endif
    SendtoTFTLN(AC_msg_ready);
  }

  void ChironTFT::IdleLoop()  {
    if (ReadTFTCommand()) {
      ProcessPanelRequest();
      command_len = 0;
    }
    CheckHeaters();
  }

  void ChironTFT::PrinterKilled(PGM_P error,PGM_P component)  {
    SendtoTFTLN(AC_msg_kill_lcd);
    #if ACDEBUG(AC_MARLIN)
      SERIAL_ECHOLNPAIR("PrinterKilled()\nerror: ", error , "\ncomponent: ", component);
    #endif
  }

  void ChironTFT::MediaEvent(media_event_t event)  {
    #if ACDEBUG(AC_MARLIN)
      SERIAL_ECHOLNPAIR("ProcessMediaStatus() ", event);
    #endif
    switch (event) {
      case AC_media_inserted:
        SendtoTFTLN(AC_msg_sd_card_inserted);
      break;

      case AC_media_removed:
        SendtoTFTLN(AC_msg_sd_card_removed);
      break;

      case AC_media_error:
        SendtoTFTLN(AC_msg_no_sd_card);
      break;
    }
  }

  void ChironTFT::TimerEvent(timer_event_t event)  {
    #if ACDEBUG(AC_MARLIN)
      SERIAL_ECHOLNPAIR("TimerEvent() ", event);
      SERIAL_ECHOLNPAIR("Printer State: ", printer_state);
    #endif

    switch (event) {
      case AC_timer_started: {
        live_Zoffset = 0.0; // reset print offset
        setSoftEndstopState(false);  // disable endstops to print
        printer_state = AC_printer_printing;
        SendtoTFTLN(AC_msg_print_from_sd_card);
      } break;

      case AC_timer_paused: {
        printer_state = AC_printer_paused;
        pause_state   = AC_paused_idle;
        SendtoTFTLN(AC_msg_paused);
      } break;

      case AC_timer_stopped: {
        if (printer_state != AC_printer_idle) {
          printer_state = AC_printer_stopping;
          SendtoTFTLN(AC_msg_print_complete);
        }
        setSoftEndstopState(true); // enable endstops
      } break;
    }
  }

  void ChironTFT::FilamentRunout()  {
    #if ACDEBUG(AC_MARLIN)
      SERIAL_ECHOLNPAIR("FilamentRunout() printer_state ", printer_state);
    #endif
    // 1 Signal filament out
    SendtoTFTLN(isPrintingFromMedia() ? AC_msg_filament_out_alert : AC_msg_filament_out_block);
    //printer_state = AC_printer_filament_out;
    PlayTune(BEEPER_PIN, FilamentOut, 1);
  }

  void ChironTFT::ConfirmationRequest(const char * const msg)  {
    // M108 continue
    #if ACDEBUG(AC_MARLIN)
      SERIAL_ECHOLNPAIR("ConfirmationRequest() ", msg, " printer_state:", printer_state);
    #endif
    switch (printer_state) {
      case AC_printer_pausing: {
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0 || strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
          SendtoTFTLN(AC_msg_paused); // enable continue button
          printer_state = AC_printer_paused;
        }
      } break;

      case AC_printer_resuming_from_power_outage:
      case AC_printer_printing:
      case AC_printer_paused: {
        // Heater timout, send acknowledgement
        if (strcmp_P(msg, MARLIN_msg_heater_timeout) == 0) {
          pause_state = AC_paused_heater_timed_out;
          SendtoTFTLN(AC_msg_paused); // enable continue button
          PlayTune(BEEPER_PIN,Heater_Timedout,1);
        }
        // Reheat finished, send acknowledgement
        else if (strcmp_P(msg, MARLIN_msg_reheat_done) == 0) {
          pause_state = AC_paused_idle;
          SendtoTFTLN(AC_msg_paused); // enable continue button
        }
        // Filament Purging, send acknowledgement enter run mode
        else if (strcmp_P(msg, MARLIN_msg_filament_purging) == 0) {
          pause_state = AC_paused_purging_filament;
          SendtoTFTLN(AC_msg_paused); // enable continue button
        }
      } break;
      default:
      break;
    }
  }

  void ChironTFT::StatusChange(const char * const msg)  {
    #if ACDEBUG(AC_MARLIN)
      SERIAL_ECHOLNPAIR("StatusChange() ", msg);
      SERIAL_ECHOLNPAIR("printer_state:", printer_state);
    #endif
    bool msg_matched = false;
    // The only way to get printer status is to parse messages
    // Use the state to minimise the work we do here.
    switch (printer_state) {
      case AC_printer_probing: {
        // If probing completes ok save the mesh and park
        // Ignore the custom machine name
        if (strcmp_P(msg + strlen(CUSTOM_MACHINE_NAME), MARLIN_msg_ready) == 0) {
          injectCommands_P(PSTR("M500\nG27"));
          SendtoTFTLN(AC_msg_probing_complete);
          printer_state = AC_printer_idle;
          msg_matched = true;
        }
        // If probing fails dont save the mesh raise the probe above the bad point
        if (strcmp_P(msg, MARLIN_msg_probing_failed) == 0) {
          PlayTune(BEEPER_PIN, BeepBeepBeeep, 1);
          injectCommands_P(PSTR("G1 Z50 F500"));
          SendtoTFTLN(AC_msg_probing_complete);
          printer_state = AC_printer_idle;
          msg_matched = true;
        }
      } break;

      case AC_printer_printing: {
        if (strcmp_P(msg, MARLIN_msg_reheating) == 0) {
          SendtoTFTLN(AC_msg_paused); // enable continue button
          msg_matched = true;
         }
      } break;

      case AC_printer_pausing: {
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {
          SendtoTFTLN(AC_msg_paused);
          printer_state = AC_printer_paused;
          pause_state = AC_paused_idle;
          msg_matched = true;
         }
      } break;

      case AC_printer_stopping: {
        if (strcmp_P(msg, MARLIN_msg_print_aborted) == 0) {
          SendtoTFTLN(AC_msg_stop);
          printer_state = AC_printer_idle;
          msg_matched = true;
        }
      } break;
      default:
      break;
    }

    // If not matched earlier see if this was a heater message
    if (!msg_matched) {
      if (strcmp_P(msg, MARLIN_msg_extruder_heating) == 0) {
        SendtoTFTLN(AC_msg_nozzle_heating);
        hotend_state = AC_heater_temp_set;
      }
      else if (strcmp_P(msg, MARLIN_msg_bed_heating) == 0) {
        SendtoTFTLN(AC_msg_bed_heating);
        hotbed_state = AC_heater_temp_set;
      }
    }
  }

  void ChironTFT::PowerLossRecovery()  {
    printer_state = AC_printer_resuming_from_power_outage; // Play tune to notify user we can recover.
    PlayTune(BEEPER_PIN, SOS, 1);
    SERIAL_ECHOLNPGM("Resuming from power outage...");
    SERIAL_ECHOLNPGM("Select SD file then press resume");
  }

  void ChironTFT::SendtoTFT(PGM_P str) {  // A helper to print PROGMEN string to the panel
    #if ACDEBUG(AC_SOME)
      serialprintPGM(str);
    #endif
    while (const char c = pgm_read_byte(str++)) TFTSer.print(c);
  }

  void ChironTFT::SendtoTFTLN(PGM_P str = nullptr) {
    if (str) {
      #if ACDEBUG(AC_SOME)
        SERIAL_ECHOPGM("> ");
      #endif
      SendtoTFT(str);
      #if ACDEBUG(AC_SOME)
        SERIAL_EOL();
      #endif
    }
    TFTSer.println("");
  }

  bool ChironTFT::ReadTFTCommand() {
    bool command_ready = false;
    while(TFTSer.available() > 0 && command_len < MAX_CMND_LEN) {
      panel_command[command_len] = TFTSer.read();
      if (panel_command[command_len] == '\n') {
        command_ready = true;
        break;
      }
      command_len++;
    }

    if (command_ready) {
      panel_command[command_len] = 0x00;
      #if ACDEBUG(AC_ALL)
        SERIAL_ECHOLNPAIR("< ", panel_command);
      #endif
      #if ACDEBUG(AC_SOME)
        // Ignore status request commands
        uint8_t req = atoi(&panel_command[1]);
        if (req > 7 && req != 20) {
          SERIAL_ECHOLNPAIR("> ", panel_command);
          SERIAL_ECHOLNPAIR("printer_state:", printer_state);
        }
      #endif
    }
    return command_ready;
  }

  int8_t ChironTFT::Findcmndpos(const char * buff, char q) {
    int8_t pos = 0;
    do { if (buff[pos] == q) return pos; } while(++pos < MAX_CMND_LEN);
    return -1;
  }

  void ChironTFT::CheckHeaters() {
    uint8_t faultDuration = 0;
    float temp = 0;

    // if the hotend temp is abnormal, confirm state before signalling panel
    temp = getActualTemp_celsius(E0);
    while (!WITHIN(temp, HEATER_0_MINTEMP, HEATER_0_MAXTEMP)) {
      faultDuration++;
      if (faultDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
        SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
        SERIAL_ECHOLNPAIR("Extruder temp abnormal! : ", temp);
        break;
      }
      delay_ms(500);
      temp = getActualTemp_celsius(E0);
    }

    // If the hotbed temp is abnormal, confirm state before signaling panel
    faultDuration = 0;
    temp = getActualTemp_celsius(BED);
    while (!WITHIN(temp, BED_MINTEMP, BED_MAXTEMP)) {
      faultDuration++;
      if (faultDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
        SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
        SERIAL_ECHOLNPAIR("Bed temp abnormal! : ", temp);
        break;
      }
      delay_ms(500);
      temp = getActualTemp_celsius(E0);
    }

    // Update panel with hotend heater status
    if (hotend_state != AC_heater_temp_reached) {
      if (WITHIN(getActualTemp_celsius(E0) - getTargetTemp_celsius(E0), -1, 1)) {
        SendtoTFTLN(AC_msg_nozzle_heating_done);
        hotend_state = AC_heater_temp_reached;
      }
    }

    // Update panel with bed heater status
    if (hotbed_state != AC_heater_temp_reached) {
      if (WITHIN(getActualTemp_celsius(BED) - getTargetTemp_celsius(BED), -0.5, 0.5)) {
        SendtoTFTLN(AC_msg_bed_heating_done);
        hotbed_state = AC_heater_temp_reached;
      }
    }
  }

  void ChironTFT::SendFileList(int8_t startindex) {
    // Respond to panel request for 4 files starting at index
    #if ACDEBUG(AC_INFO)
      SERIAL_ECHOLNPAIR("## SendFileList ## ", startindex);
    #endif
    SendtoTFTLN(PSTR("FN "));
    filenavigator.getFiles(startindex);
    SendtoTFTLN(PSTR("END"));
  }

  void ChironTFT::SelectFile() {
    strncpy(selectedfile, panel_command + 4, command_len - 4);
    selectedfile[command_len - 5] = '\0';
    #if ACDEBUG(AC_FILE)
      SERIAL_ECHOLNPAIR_F(" Selected File: ",selectedfile);
    #endif
    switch (selectedfile[0]) {
      case '/':   // Valid file selected
        SendtoTFTLN(AC_msg_sd_file_open_success);
        break;

      case '<':   // .. (go up folder level)
        filenavigator.upDIR();
        SendtoTFTLN(AC_msg_sd_file_open_failed);
        SendFileList( 0 );
        break;
      default:   // enter sub folder
        filenavigator.changeDIR(selectedfile);
        SendtoTFTLN(AC_msg_sd_file_open_failed);
        SendFileList( 0 );
        break;
    }
  }

  void ChironTFT::InjectCommandandWait(PGM_P cmd) {
    //injectCommands_P(cmnd); queue.enqueue_now_P(cmd);
    //SERIAL_ECHOLN(PSTR("Inject>"));
  }

  void ChironTFT::ProcessPanelRequest() {
    // Break these up into logical blocks // as its easier to navigate than one huge switch case!
    int8_t req = atoi(&panel_command[1]);

    // Information requests A0 - A8 and A33
    if (req <= 8 || req == 33) PanelInfo(req);

    // Simple Actions A9 - A28
    else if ( req <= 28) PanelAction(req);

    // Process Initiation
    else if (req <= 34) PanelProcess(req);

    else SendtoTFTLN();
  }

  void ChironTFT::PanelInfo(uint8_t req) {
    // information requests A0-A8 and A33
    switch (req) {
      case 0:   // A0 Get HOTEND Temp
        SendtoTFT(PSTR("A0V "));
        TFTSer.println(getActualTemp_celsius(E0));
        break;

      case 1:   // A1 Get HOTEND Target Temp
        SendtoTFT(PSTR("A1V "));
        TFTSer.println(getTargetTemp_celsius(E0));
        break;

      case 2:   // A2 Get BED Temp
        SendtoTFT(PSTR("A2V "));
        TFTSer.println(getActualTemp_celsius(BED));
        break;

      case 3:   // A3 Get BED Target Temp
        SendtoTFT(PSTR("A3V "));
        TFTSer.println(getTargetTemp_celsius(BED));
        break;

      case 4:   // A4 Get FAN Speed
        SendtoTFT(PSTR("A4V "));
        TFTSer.println(getActualFan_percent(FAN0));
        break;

      case 5:   // A5 Get Current Coordinates
        SendtoTFT(PSTR("A5V X: "));
        TFTSer.print(getAxisPosition_mm(X));
        SendtoTFT(PSTR(" Y: "));
        TFTSer.print(getAxisPosition_mm(Y));
        SendtoTFT(PSTR(" Z: "));
        TFTSer.println(getAxisPosition_mm(Z));
        break;

      case 6:   // A6 Get printing progress
        if (isPrintingFromMedia()) {
          SendtoTFT(PSTR("A6V "));
          TFTSer.println(ui8tostr2(getProgress_percent()));
        }
        else
          SendtoTFTLN(PSTR("A6V ---"));
        break;

      case 7: { // A7 Get Printing Time
        uint32_t time = getProgress_seconds_elapsed() / 60;
        SendtoTFT(PSTR("A7V "));
        TFTSer.print(ui8tostr2(time / 60));
        SendtoTFT(PSTR(" H "));
        TFTSer.print(ui8tostr2(time % 60));
        SendtoTFT(PSTR(" M"));
        #if ACDEBUG(AC_ALL)
          SERIAL_ECHOLNPAIR("Print time ", ui8tostr2(time / 60), ":", ui8tostr2(time % 60));
        #endif
      } break;

      case 8:   // A8 Get SD Card list A8 S0
        if (!isMediaInserted()) safe_delay(500);
        if (!isMediaInserted())   // Make sure the card is removed
          SendtoTFTLN(AC_msg_no_sd_card);
        else if (panel_command[3] == 'S')
          SendFileList( atoi( &panel_command[4] ) );
        break;

      case 33:   // A33 Get firmware info
        SendtoTFT(PSTR("J33 "));
        SendtoTFTLN(PSTR(SHORT_BUILD_VERSION));
        break;
    }
  }

  void ChironTFT::PanelAction(uint8_t req) {
    switch (req) {
      case  9:   // A9 Pause SD print
        if (isPrintingFromMedia()) {
          SendtoTFTLN(AC_msg_pause);
          pausePrint();
          printer_state = AC_printer_pausing;
        }
        else
          SendtoTFTLN(AC_msg_stop);
        break;

      case 10: // A10 Resume SD Print
        if (pause_state == AC_paused_idle || printer_state == AC_printer_resuming_from_power_outage)
          resumePrint();
        else
          setUserConfirmed();
        break;

      case 11:   // A11 Stop SD print
        if (isPrintingFromMedia()) {
          printer_state = AC_printer_stopping;
          stopPrint();
        }
        else {
          if (printer_state == AC_printer_resuming_from_power_outage)
            injectCommands_P(PSTR("M1000 C")); // Cancel recovery
          SendtoTFTLN(AC_msg_stop);
          printer_state = AC_printer_idle;
        }
        break;

      case 12:   // A12 Kill printer
        kill();  // from marlincore.h
        break;

      case 13:   // A13 Select file
        SelectFile();
        break;

      case 14: { // A14 Start Printing
        // Allows printer to restart the job if we dont want to recover
        if (printer_state == AC_printer_resuming_from_power_outage) {
          injectCommands_P(PSTR("M1000 C")); // Cancel recovery
          printer_state = AC_printer_idle;
        }
        #if ACDebugLevel >= 1
          SERIAL_ECHOLNPAIR_F("Print: ", selectedfile);
        #endif
        // the card library needs a path starting // but the File api doesn't...
        char file[MAX_PATH_LEN];
        file[0] = '/';
        strcpy(file + 1, selectedfile);
        printFile(file);
        SendtoTFTLN(AC_msg_print_from_sd_card);
      } break;

      case 15:   // A15 Resuming from outage
        if (printer_state == AC_printer_resuming_from_power_outage) {
          // Need to home here to restore the Z position
          injectCommands(AC_cmnd_power_loss_recovery);
          injectCommands("M1000");  // home and start recovery
        }
        break;

      case 16: { // A16 Set HotEnd temp  A17 S170
        const float set_Htemp = atof(&panel_command[5]);
        hotend_state = set_Htemp ? AC_heater_temp_set : AC_heater_off;
        switch ((char)panel_command[4]) {
          // Set Temp
          case 'S': case 'C': setTargetTemp_celsius(set_Htemp, E0);
        }
      } break;

      case 17: { // A17 Set bed temp
        const float set_Btemp = atof(&panel_command[5]);
        hotbed_state = set_Btemp ? AC_heater_temp_set : AC_heater_off;
        if (panel_command[4] == 'S')
          setTargetTemp_celsius(set_Btemp, BED);
      } break;

      case 18:   // A18 Set Fan Speed
        if (panel_command[4] == 'S')
          setTargetFan_percent(atof(&panel_command[5]), FAN0);
        break;

      case 19:   // A19 Motors off
        if (!isPrinting()) {
          disable_all_steppers(); // from marlincore.h
          SendtoTFTLN(AC_msg_ready);
        }
        break;

      case 20:   // A20 Read/write print speed
        if (panel_command[4] == 'S')
          setFeedrate_percent(atoi(&panel_command[5]));
        else {
          SendtoTFT(PSTR("A20V "));
          TFTSer.println(getFeedrate_percent());
        }
        break;

      case 21:   // A21 Home Axis  A21 X
        if (!isPrinting()) {
          switch ((char)panel_command[4]) {
            case 'X': injectCommands_P(PSTR("G28X")); break;
            case 'Y': injectCommands_P(PSTR("G28Y")); break;
            case 'Z': injectCommands_P(PSTR("G28Z")); break;
            case 'C': injectCommands_P(G28_STR); break;
          }
        }
        break;

      case 22:   // A22 Move Axis  A22 Y +10F3000
        // Ignore request if printing
        if (!isPrinting()) {
          // setAxisPosition_mm() uses pre defined manual feedrates so ignore the feedrate from the panel
          setSoftEndstopState(true);  // enable endstops
          float newposition = atof(&panel_command[6]);

          #if ACDEBUG(AC_ACTION)
            SERIAL_ECHOLNPAIR("Nudge ", panel_command[4], " axis ", newposition);
          #endif

          switch (panel_command[4]) {
            case 'X': setAxisPosition_mm(getAxisPosition_mm(X) + newposition, X); break;
            case 'Y': setAxisPosition_mm(getAxisPosition_mm(Y) + newposition, Y); break;
            case 'Z': setAxisPosition_mm(getAxisPosition_mm(Z) + newposition, Z); break;
            case 'E':   // The only time we get this command is from the filament load/unload menu
                        // the standard movement is too slow so we will use the load unlod GCode to speed it up a bit
              if (canMove(E0) && !commandsInQueue())
                injectCommands_P(newposition > 0 ? AC_cmnd_manual_load_filament : AC_cmnd_manual_unload_filament);
              break;
          }
        }
        break;

      case 23:   // A23 Preheat PLA
        // Ignore request if printing
        if (!isPrinting()) {
          // Temps defined in configuration.h
          setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
          setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, E0);
          SendtoTFTLN();
          hotbed_state = AC_heater_temp_set;
          hotend_state = AC_heater_temp_set;
        }
        break;

      case 24:   // A24 Preheat ABS
        // Ignore request if printing
        if (!isPrinting()) {
          setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
          setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, E0);
          SendtoTFTLN();
          hotbed_state = AC_heater_temp_set;
          hotend_state = AC_heater_temp_set;
        }
        break;

      case 25:   // A25 Cool Down
        // Ignore request if printing
        if (!isPrinting()) {
          setTargetTemp_celsius(0, E0);
          setTargetTemp_celsius(0, BED);
          SendtoTFTLN(AC_msg_ready);
          hotbed_state = AC_heater_off;
          hotend_state = AC_heater_off;
        }
        break;

      case 26:   // A26 Refresh SD
        // M22 M21 maybe needed here to reset sd card
        filenavigator.reset();
        break;

      case 27:   // A27 Servo Angles adjust
        break;

      case 28:   // A28 Filament set A28 O/C
        // Ignore request if printing
        if (isPrinting()) break;
        SendtoTFTLN();
        break;
    }
  }

  void ChironTFT::PanelProcess(uint8_t req) {
    switch (req) {
      case 29: { // A29 Read Mesh Point A29 X1 Y1
        xy_uint8_t pos;
        float pos_z;
        pos.x = atoi(&panel_command[5]);
        pos.y = atoi(&panel_command[8]);
        pos_z = getMeshPoint(pos);

        SendtoTFT(PSTR("A29V "));
        TFTSer.println(pos_z * 100);
        if (!isPrinting()) {
          setSoftEndstopState(true);  // disable endstops
          // If the same meshpoint is selected twice in a row, move the head to that ready for adjustment
          if ((selectedmeshpoint.x == pos.x) && (selectedmeshpoint.y == pos.y)) {
            if (!isPositionKnown())
              injectCommands_P(G28_STR); // home

            if (isPositionKnown()) {
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPAIR("Moving to mesh point at x: ", pos.x, " y: ", pos.y, " z: ", pos_z);
              #endif
              // Go up before moving
              setAxisPosition_mm(3.0,Z);

              setAxisPosition_mm(17 + (93 * pos.x), X);
              setAxisPosition_mm(20 + (93 * pos.y), Y);
              setAxisPosition_mm(0.0, Z);
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPAIR("Current Z: ", getAxisPosition_mm(Z));
              #endif
            }
          }
          selectedmeshpoint.x = pos.x;
          selectedmeshpoint.y = pos.y;
        }
      } break;

      case 30: {  // A30 Auto leveling
        if (panel_command[3] == 'S') { // Start probing
          // Ignore request if printing
          if (isPrinting())
            SendtoTFTLN(AC_msg_probing_not_allowed); // forbid auto leveling
          else {
            injectCommands_P(PSTR("G28O\nG29"));
            printer_state = AC_printer_probing;
            SendtoTFTLN(AC_msg_start_probing);
          }
        }
        else SendtoTFTLN(AC_msg_start_probing);
      }  break;

      case 31: { // A31 Adjust all Probe Points
        switch (panel_command[3]) {
          case 'C':   // Restore and apply original offsets
            if (!isPrinting()) {
              injectCommands_P(PSTR("M501\nM420 S1"));
              selectedmeshpoint.x = selectedmeshpoint.y = 99;
            }
          break;
          case 'D':   // Save Z Offset tables and restore leveling state
            if (!isPrinting()) {
              setAxisPosition_mm(1.0,Z);
              injectCommands_P(PSTR("M500"));
              selectedmeshpoint.x = selectedmeshpoint.y = 99;
            }
          break;
          case 'G':   // Get current offset
            SendtoTFT(PSTR("A31V "));
            // When printing use the live z Offset position
            // we will use babystepping to move the print head
            if (isPrinting())
              TFTSer.println(live_Zoffset);
            else {
              TFTSer.println(getZOffset_mm());
              selectedmeshpoint.x = selectedmeshpoint.y = 99;
            }
          break;
          case 'S': { // Set offset (adjusts all points by value)
            float Zshift = atof(&panel_command[4]);
            setSoftEndstopState(false);  // disable endstops
            // Allow temporary Z position nudging during print
            // From the leveling panel use the all points UI to adjust the print pos.
            if (isPrinting()) {
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPAIR("Change Zoffset from:", live_Zoffset, " to ", live_Zoffset + Zshift);
              #endif
              if (isAxisPositionKnown(Z)) {
                #if ACDEBUG(AC_INFO)
                  const float currZpos = getAxisPosition_mm(Z);
                  SERIAL_ECHOLNPAIR("Nudge Z pos from ", currZpos, " to ", currZpos + constrain(Zshift, -0.05, 0.05));
                #endif
                // Use babystepping to adjust the head position
                int16_t steps = mmToWholeSteps(constrain(Zshift,-0.05,0.05), Z);
                #if ACDEBUG(AC_INFO)
                  SERIAL_ECHOLNPAIR("Steps to move Z: ", steps);
                #endif
                babystepAxis_steps(steps, Z);
                live_Zoffset += Zshift;
              }
              SendtoTFT(PSTR("A31V "));
              TFTSer.println(live_Zoffset);
            }
            else {
              GRID_LOOP(x, y) {
                const xy_uint8_t pos { x, y };
                const float currval = getMeshPoint(pos);
                setMeshPoint(pos, constrain(currval + Zshift, AC_LOWEST_MESHPOINT_VAL, 2));
              }
              const float currZOffset = getZOffset_mm();
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPAIR("Change probe offset from ", currZOffset, " to  ", currZOffset + Zshift);
              #endif

              setZOffset_mm(currZOffset + Zshift);
              SendtoTFT(PSTR("A31V "));
              TFTSer.println(getZOffset_mm());

              if (isAxisPositionKnown(Z)) {
                // Move Z axis
                const float currZpos = getAxisPosition_mm(Z);
                #if ACDEBUG(AC_INFO)
                  SERIAL_ECHOLNPAIR("Move Z pos from ", currZpos, " to ", currZpos + constrain(Zshift, -0.05, 0.05));
                #endif
                setAxisPosition_mm(currZpos+constrain(Zshift,-0.05,0.05),Z);
              }
            }
          } break;
        } // end switch
      } break;

      case 32: { // A32 clean leveling beep flag
        // Ignore request if printing
        //if (isPrinting()) break;
        //injectCommands_P(PSTR("M500\nM420 S1\nG1 Z10 F240\nG1 X0 Y0 F6000"));
        //TFTSer.println("");
      } break;

      // A33 firmware info request seet PanelInfo()

      case 34: {  // A34 Adjust single mesh point A34 C/S X1 Y1 V123
        if (panel_command[3] == 'C') { // Restore original offsets
          injectCommands_P(PSTR("M501\nM420 S1"));
          selectedmeshpoint.x = selectedmeshpoint.y = 99;
          //printer_state = AC_printer_idle;
        }
        else {
          xy_uint8_t pos;
          pos.x = atoi(&panel_command[5]);
          pos.y = atoi(&panel_command[8]);

          float currmesh = getMeshPoint(pos);
          float newval   = atof(&panel_command[11])/100;
          #if ACDEBUG(AC_INFO)
            SERIAL_ECHOLNPAIR("Change mesh point x:", pos.x, " y:", pos.y);
            SERIAL_ECHOLNPAIR("from ", currmesh, " to ", newval);
          #endif
          // Update Meshpoint
          setMeshPoint(pos,newval);
          if (printer_state == AC_printer_idle || printer_state == AC_printer_probing /*!isPrinting()*/) {
            // if we are at the current mesh point indicated on the panel Move Z pos +/- 0.05mm
            // (The panel changes the mesh value by +/- 0.05mm on each button press)
            if (selectedmeshpoint.x == pos.x && selectedmeshpoint.y == pos.y) {
              setSoftEndstopState(false);
              float currZpos = getAxisPosition_mm(Z);
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPAIR("Move Z pos from ", currZpos, " to ", currZpos + constrain(newval - currmesh, -0.05, 0.05));
              #endif
              setAxisPosition_mm(currZpos + constrain(newval - currmesh, -0.05, 0.05), Z);
            }
          }
        }
      }  break;
    }
  }
} // namespace

#endif // ANYCUBIC_LCD_CHIRON
