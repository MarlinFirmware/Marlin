/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/anycubic_vyper/dgus_tft.cpp
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_VYPER)

#include "dgus_tft.h"
#include "Tunes.h"
#include "FileNavigator.h"

#include "../../../gcode/queue.h"
#include "../../../sd/cardreader.h"
#include "../../../libs/numtostr.h"
#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../module/stepper.h"
#include "../../../module/probe.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#define DEBUG_OUT ACDEBUGLEVEL
#include "../../../core/debug_out.h"

#include <string>

namespace Anycubic {

  const char MESSAGE_charu[]          = {0xB4, 0xE6, 0xB4, 0xA2, 0xBF, 0xA8, 0xD2, 0xD1, 0xB2, 0xE5, 0xC8, 0xEB, 0x00}; // '忙'垄驴篓脪脩虏氓脠毛
  const char MESSAGE_bachu[]          = {0xB4, 0xE6, 0xB4, 0xA2, 0xBF, 0xA8, 0xD2, 0xD1, 0xB0, 0xCE, 0xB3, 0xF6, 0x00};
  const char MESSAGE_wuka[]           = {0xCE, 0xDE, 0xB4, 0xE6, 0xB4, 0xA2, 0xBF, 0xA8, 0x00};
  const char MESSAGE_lianji[]         = {0xC1, 0xAA, 0xBB, 0xFA, 0xD6, 0xD0, 0x00};
  const char MESSAGE_tuoji[]          = {0xCD, 0xD1, 0xBB, 0xFA, 0xB4, 0xF2, 0xD3, 0xA1, 0xD6, 0xD0, 0x00};
  const char MESSAGE_zanting[]        = {0xB4, 0xF2, 0xD3, 0xA1, 0xD4, 0xDD, 0xCD, 0xA3, 0xD6, 0xD0, 0x00};
  const char MESSAGE_tingzhi[]        = {0xCD, 0xA3, 0xD6, 0xB9, 0xB4, 0xF2, 0xD3, 0xA1, 0x00};
  const char MESSAGE_wancheng[]       = {0xCD, 0xEA, 0xB3, 0xC9, 0xB4, 0xF2, 0xD3, 0xA1, 0x00};
  const char MESSAGE_hotend_heating[] = {0xB4, 0xF2, 0xD3, 0xA1, 0xCD, 0xB7, 0xD5, 0xFD, 0xD4, 0xDA, 0xBC, 0xD3, 0xC8, 0xC8, 0x00};
  const char MESSAGE_hotend_over[]    = {0xB4, 0xF2, 0xD3, 0xA1, 0xCD, 0xB7, 0xBC, 0xD3, 0xC8, 0xC8, 0xCD, 0xEA, 0xB3, 0xC9, 0x00};
  const char MESSAGE_bed_heating[]    = {0xC8, 0xC8, 0xB4, 0xB2, 0xD5, 0xFD, 0xD4, 0xDA, 0xBC, 0xD3, 0xC8, 0xC8, 0x00};
  const char MESSAGE_bed_over[]       = {0xC8, 0xC8, 0xB4, 0xB2, 0xBC, 0xD3, 0xC8, 0xC8, 0xCD, 0xEA, 0xB3, 0xC9, 0x00};
  const char MESSAGE_ready[]          = {0xD7, 0xBC, 0xB1, 0xB8, 0xBE, 0xCD, 0xD0, 0xF7, 0x00};
  const char MESSAGE_cold[]           = {0xB4, 0xF2, 0xD3, 0xA1, 0xCD, 0xB7, 0xCE, 0xC2, 0xB6, 0xC8, 0xB9, 0xFD, 0xB5, 0xCD, 0x00};

  const char *p_mesage[] = { MESSAGE_charu, MESSAGE_bachu, MESSAGE_wuka, MESSAGE_lianji, MESSAGE_tuoji, MESSAGE_zanting,
                             MESSAGE_tingzhi, MESSAGE_wancheng, MESSAGE_hotend_heating, MESSAGE_hotend_over, MESSAGE_bed_heating,
                             MESSAGE_bed_over, MESSAGE_ready, MESSAGE_cold };

  DgusTFT::p_fun fun_array[] = {
    DgusTFT::page1,  DgusTFT::page2,  DgusTFT::page3,  DgusTFT::page4,  DgusTFT::page5,  DgusTFT::page6,
    DgusTFT::page7,  DgusTFT::page8,  DgusTFT::page9,  DgusTFT::page10, DgusTFT::page11, DgusTFT::page12,
    DgusTFT::page13, DgusTFT::page14, DgusTFT::page15, DgusTFT::page16, DgusTFT::page17, DgusTFT::page18,
    DgusTFT::page19, DgusTFT::page20, DgusTFT::page21, DgusTFT::page22, DgusTFT::page23, DgusTFT::page24,
    DgusTFT::page25, DgusTFT::page26, DgusTFT::page27, DgusTFT::page28, DgusTFT::page29, DgusTFT::page30,
    DgusTFT::page31, DgusTFT::page32
    #if HAS_LEVELING
      , DgusTFT::page33 , DgusTFT::page34
    #endif
  };

  printer_state_t DgusTFT::printer_state;
  paused_state_t DgusTFT::pause_state;
  #if HAS_HOTEND
    heater_state_t DgusTFT::hotend_state;
  #endif
  #if HAS_HEATED_BED
    heater_state_t DgusTFT::hotbed_state;
  #endif
  char DgusTFT::selectedfile[MAX_PATH_LEN];
  char DgusTFT::panel_command[MAX_CMND_LEN];
  uint8_t DgusTFT::command_len;
  file_menu_t DgusTFT::file_menu;

  bool DgusTFT::data_received;
  uint8_t DgusTFT::data_buf[DATA_BUF_SIZE];
  uint8_t DgusTFT::data_index;
  uint16_t DgusTFT::page_index_now, DgusTFT::page_index_last, DgusTFT::page_index_last_2;
  uint8_t DgusTFT::message_index;
  uint8_t DgusTFT::pop_up_index;
  uint32_t DgusTFT::key_value;
  uint8_t DgusTFT::lcd_txtbox_index;
  uint8_t DgusTFT::lcd_txtbox_page;
  int16_t DgusTFT::feedrate_back;
  lcd_info_t DgusTFT::lcd_info, DgusTFT::lcd_info_back;
  language_t DgusTFT::ui_language;
  uint16_t page_index_saved;          // flags to keep from bombing the host display
  uint8_t pop_up_index_saved;
  uint32_t key_value_saved;

  void DEBUG_PRINT_PAUSED_STATE(FSTR_P const msg, paused_state_t state);
  void DEBUG_PRINT_PRINTER_STATE(FSTR_P const msg, printer_state_t state);
  void DEBUG_PRINT_TIMER_EVENT(FSTR_P const msg, timer_event_t event);
  void DEBUG_PRINT_MEDIA_EVENT(FSTR_P const msg, media_event_t event);

  DgusTFT Dgus;

  DgusTFT::DgusTFT() {
    data_buf[0] = '\0';
    message_index = 100;
    pop_up_index = 100;
    page_index_now = page_index_last = page_index_last_2 = 1;
    lcd_txtbox_index = 0;
    feedrate_back = -1;
  }

  void DgusTFT::Startup() {
    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("DgusTFT::Startup()");
    #endif
    selectedfile[0] = '\0';
    panel_command[0] = '\0';
    command_len = 0;
    printer_state = AC_printer_idle;
    pause_state = AC_paused_idle;
    TERN_(HAS_HOTEND, hotend_state = AC_heater_off);
    TERN_(HAS_HEATED_BED, hotbed_state = AC_heater_off);
    file_menu = AC_menu_file;
    set_language(ui_language); // use language stored in EEPROM

    // Filament runout is handled by Marlin settings in Configuration.h
    // opt_set    FIL_RUNOUT_STATE HIGH  // Pin state indicating that filament is NOT present.
    // opt_enable FIL_RUNOUT_PULLUP

    TFTSer.begin(115200);

    // Signal Board has reset
    SendtoTFTLN(AC_msg_main_board_has_reset);

    // Enable levelling and Disable end stops during print
    // as Z home places nozzle above the bed so we need to allow it past the end stops
    injectCommands_P(AC_cmnd_enable_levelling);

    // Startup tunes are defined in Tunes.h
    //PlayTune(BEEPER_PIN, Anycubic_PowerOn, 1);
    //PlayTune(BEEPER_PIN, GB_PowerOn, 1);
    #if ACDEBUGLEVEL
      DEBUG_ECHOLNPGM("Startup   AC Debug Level ", ACDEBUGLEVEL);
    #endif
    SendtoTFTLN(AC_msg_ready);
  }

  void DgusTFT::ParamInit() {

    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("DgusTFT::ParamInit()");
    #endif

    if (lcd_info.language == CHS)
      page_index_now = 1;
    else if (lcd_info.language == ENG)
      page_index_now = 121;

    LcdAudioSet(lcd_info.audio_on);

    #if ACDEBUG(AC_MARLIN)
      if (lcd_info.language == CHS)
        DEBUG_ECHOLNPGM("ParamInit   lcd language: CHS");
      else if (lcd_info.language == ENG)
        DEBUG_ECHOLNPGM("ParamInit   lcd language: ENG");

      if (lcd_info.audio_on)
        DEBUG_ECHOLNPGM("ParamInit   lcd audio: ON");
      else
        DEBUG_ECHOLNPGM("ParamInit   lcd audio: OFF");
    #endif

    RequestValueFromTFT(0x14);  // get page ID
  }

  void DgusTFT::IdleLoop() {
    if (ReadTFTCommand()) {
      ProcessPanelRequest();
      command_len = 0;
    }

    #if ACDEBUG(AC_MARLIN)
      if (key_value) {
        DEBUG_ECHOLNPGM("IdleLoop   page: ", page_index_now);
        DEBUG_ECHOLNPGM("key: ", key_value);
      }
    #endif

    switch (page_index_now) {
      case 115: page115(); break;
      case 117: page117(); break;
      //case 124: page124(); break;
      //case 125: page125(); break;
      case 170: page170(); break;

      #if ENABLED(POWER_LOSS_RECOVERY)
        case 171: page171(); break;
        case 173: page173(); break;
      #endif

      #if HAS_LEVELING
        case 175: page175(); break;
        case 176: page176(); break;
      #endif

      case 177 ... 198: {
        #if 0 // ACDEBUG(AC_MARLIN)
          DEBUG_ECHOLNPGM("line: ", __LINE__);
          DEBUG_ECHOLNPGM("func: ", page_index_now);
        #endif
        //page177_to_198();
      } break;

      case 199 ... 200: {
        #if 0 // ACDEBUG(AC_MARLIN)
          DEBUG_ECHOLNPGM("line: ", __LINE__);
          DEBUG_ECHOLNPGM("func: ", page_index_now);
        #endif
        page199_to_200();
      } break;

      case 201: case 204: page201(); break;
      case 202: case 205: page202(); break;
      case 203: case 206: page203(); break;

      default:
        if (lcd_info.language == CHS) {
          if (WITHIN(page_index_now, 1, 1 + COUNT(fun_array))) {
            fun_array[page_index_now - 1]();
          }
          else {
            #if ACDEBUG(AC_MARLIN)
              DEBUG_ECHOLNPGM("line: ", __LINE__);
              DEBUG_ECHOLNPGM("fun doesn't exist: ", page_index_now);
            #endif
          }
        }
        else if (lcd_info.language == ENG) {
          if (WITHIN(page_index_now, 121, 121 + COUNT(fun_array))) {
            fun_array[page_index_now - 121]();  // ENG page_index is 120 more than CHS
          }
          else {
            SERIAL_ECHOLNPGM("lcd function doesn't exist");
            SERIAL_ECHOLNPGM("page_index_last: ", page_index_last);
            SERIAL_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
          }
        }
        break;
    }

    pop_up_manager();
    key_value = 0;

    CheckHeaters();
  }

  uint8_t FSHlength(FSTR_P FSHinput) {
    PGM_P FSHinputPointer = reinterpret_cast<PGM_P>(FSHinput);
    uint8_t stringLength  = 0;
    while (pgm_read_byte(FSHinputPointer++)) stringLength++;
    return stringLength;
  }

  void DgusTFT::PrinterKilled(FSTR_P error_p, FSTR_P component_p) {

    // copy string in FLASH to RAM for strcmp_P

    uint8_t textLength = FSHlength(error_p);
    char error[FSHlength(error_p) + 1];
    memcpy_P(error, error_p, textLength + 1);  // +1 for the null terminator

    textLength = FSHlength(component_p);
    char component[FSHlength(component_p) + 1];
    memcpy_P(component, component_p, textLength + 1);  // +1 for the null terminator

    SendtoTFTLN(AC_msg_kill_lcd);
    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("PrinterKilled()\nerror: ", error, "\ncomponent: ", component);
    #endif

    if (strcmp_P(error, PSTR("Heating Failed")) == 0) {

      if (strcmp_P(component, PSTR("Bed")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_BED_HEATER);
        SERIAL_ECHOLNPGM("Check Bed heater");
      }
      else if (strcmp_P(component, PSTR("E1")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_HOTEND_HEATER);
        SERIAL_ECHOLNPGM("Check E1 heater");
      }

    }
    else if (strcmp_P(error, PSTR("Err: MINTEMP")) == 0) {

      if (strcmp_P(component, PSTR("Bed")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_BED_NTC);
        SERIAL_ECHOLNPGM("Check Bed thermistor");
      }
      else if (strcmp_P(component, PSTR("E1")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_HOTEND_NTC);
        SERIAL_ECHOLNPGM("Check E1 thermistor");
      }

    }
    else if (strcmp_P(error, PSTR("Err: MAXTEMP")) == 0) {

      if (strcmp_P(component, PSTR("Bed")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_BED_NTC);
        SERIAL_ECHOLNPGM("Check Bed thermistor");
      }
      else if (strcmp_P(component, PSTR("E1")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_HOTEND_NTC);
        SERIAL_ECHOLNPGM("Check E1 thermistor");
      }

    }
    else if (strcmp_P(error, PSTR("THERMAL RUNAWAY")) == 0) {

      if (strcmp_P(component, PSTR("Bed")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_BED_HEATER);
        SERIAL_ECHOLNPGM("Check Bed thermal runaway");
      }
      else if (strcmp_P(component, PSTR("E1")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_HOTEND_HEATER);
        SERIAL_ECHOLNPGM("Check E1 thermal runaway");
      }

    }
    else if (strcmp_P(error, PSTR("Homing Failed")) == 0) {

      if (strcmp_P(component, PSTR("X")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_X_ENDSTOP);
        SERIAL_ECHOLNPGM("Check X endstop");
      }
      else if (strcmp_P(component, PSTR("Y")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_Y_ENDSTOP);
        SERIAL_ECHOLNPGM("Check Y endstop");
      }
      else if (strcmp_P(component, PSTR("Z")) == 0) {
        ChangePageOfTFT(PAGE_CHS_ABNORMAL_Z_ENDSTOP);
        SERIAL_ECHOLNPGM("Check Z endstop");
      }

    }

  }

  void DgusTFT::set_descript_color(const uint16_t color, const uint8_t index/*=lcd_txtbox_index*/) {
    SendColorToTFT(color, TXT_DESCRIPT_0 + 0x30 * (index - 1));
  }

  void DgusTFT::MediaEvent(media_event_t event) {
    #if ACDEBUG(AC_MARLIN)
      DEBUG_PRINT_MEDIA_EVENT(F("ProcessMediaStatus() "), event);
    #endif
    switch (event) {
      case AC_media_inserted:

        filenavigator.reset();

        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          set_descript_color(COLOR_BLUE);
          lcd_txtbox_index = 0;
        }

        SendFileList(lcd_txtbox_index);

        break;

      case AC_media_removed:
        SendtoTFTLN(AC_msg_sd_card_removed);

        filenavigator.reset();

        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          set_descript_color(COLOR_BLUE);
          lcd_txtbox_index = 0;
        }

        SendFileList(lcd_txtbox_index);
        break;

      case AC_media_error:
        SendtoTFTLN(AC_msg_no_sd_card);
        break;
    }
  }

  void DgusTFT::TimerEvent(timer_event_t event) {

    #if ACDEBUG(AC_MARLIN)
      DEBUG_PRINT_TIMER_EVENT(F("TimerEvent() "), event);
      DEBUG_PRINT_PRINTER_STATE(F("Printer State: "), printer_state);
    #endif

    switch (event) {
      case AC_timer_started:
        setSoftEndstopState(false);  // disable endstops to print
        printer_state = AC_printer_printing;
        SendtoTFTLN(AC_msg_print_from_sd_card);
      break;

      case AC_timer_paused:
        //printer_state = AC_printer_paused;
        //pause_state = AC_paused_idle;
        SendtoTFTLN(AC_msg_paused);
        break;

      case AC_timer_stopped:
        if (printer_state != AC_printer_idle) {
          if (printer_state == AC_printer_stopping_from_media_remove) {
            ChangePageOfTFT(PAGE_NO_SD);
          }
          else {
            printer_state = AC_printer_stopping;

            // Get Printing Time
            uint32_t time = getProgress_seconds_elapsed() / 60;
            char str_buf[20];
            sprintf(str_buf, "%s H ", utostr3(time / 60));
            sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
            SendTxtToTFT(str_buf, TXT_FINISH_TIME);
            ChangePageOfTFT(PAGE_PRINT_FINISH);
            SendtoTFTLN(AC_msg_print_complete);
            pop_up_index = 100;
          }
        }
        setSoftEndstopState(true); // enable endstops
        break;
    }
  }

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)

    void DgusTFT::FilamentRunout() {
      #if ACDEBUG(AC_MARLIN)
        DEBUG_PRINT_PRINTER_STATE(F("FilamentRunout() printer_state "), printer_state);

        // 1 Signal filament out
        SendtoTFTLN(isPrintingFromMedia() ? AC_msg_filament_out_alert : AC_msg_filament_out_block);
        //printer_state = AC_printer_filament_out;

        DEBUG_ECHOLNPGM("getFilamentRunoutState: ", getFilamentRunoutState());
      #endif

      pop_up_index = 15;  // show filament lack.

      if (READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_STATE) {
        PlayTune(BEEPER_PIN, FilamentOut, 1);

        feedrate_back = getFeedrate_percent();

        if (isPrintingFromMedia()) {
          pausePrint();
          printer_state = AC_printer_pausing;
          pause_state   = AC_paused_filament_lack;
        }
      }
    }

  #endif // FILAMENT_RUNOUT_SENSOR

  void DgusTFT::ConfirmationRequest(const char * const msg) {
    // M108 continue
    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("HomingComplete, line: ", __LINE__);
      DEBUG_ECHOLNPGM("ConfirmationRequest() ", msg);
      DEBUG_PRINT_PRINTER_STATE(F("printer_state: " ), printer_state);
      DEBUG_PRINT_PAUSED_STATE(F("pause_state: "), pause_state);
    #endif

    switch (printer_state) {
      case AC_printer_pausing: {
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0 || strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
          if (pause_state != AC_paused_filament_lack)
            ChangePageOfTFT(PAGE_STATUS1);    // enable continue button
          printer_state = AC_printer_paused;
        }
      }
      break;

      #if ENABLED(POWER_LOSS_RECOVERY)
        case AC_printer_resuming_from_power_outage:
      #endif
      case AC_printer_printing:
      case AC_printer_paused:
        // Heater timout, send acknowledgement
        if (strcmp_P(msg, MARLIN_msg_heater_timeout) == 0) {
          pause_state = AC_paused_heater_timed_out;
          SendtoTFTLN(AC_msg_paused); // enable continue button
          PlayTune(BEEPER_PIN, Heater_Timedout, 1);
        }
        // Reheat finished, send acknowledgement
        else if (strcmp_P(msg, MARLIN_msg_reheat_done) == 0) {
          #if ACDEBUG(AC_MARLIN)
            DEBUG_ECHOLNPGM("send M108 ", __LINE__);
          #endif
          injectCommands(F("M108"));

          if (pause_state != AC_paused_filament_lack)
            pause_state = AC_paused_idle;

          SendtoTFTLN(AC_msg_paused); // enable continue button
        }
        // Filament Purging, send acknowledgement enter run mode
        else if (strcmp_P(msg, MARLIN_msg_filament_purging) == 0) {
          pause_state = AC_paused_purging_filament;
          SendtoTFTLN(AC_msg_paused); // enable continue button
        }
        else if (strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
          #if ACDEBUG(AC_MARLIN)
            DEBUG_ECHOLNPGM("send M108 ", __LINE__);
          #endif
          injectCommands(F("M108"));

          if (pause_state != AC_paused_filament_lack)
            pause_state = AC_paused_idle;
        }

        break;

      default: break;
    }
  }

  void DgusTFT::StatusChange(const char * const msg) {
    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("StatusChange() ", msg);
      DEBUG_PRINT_PRINTER_STATE(F("printer_state: "), printer_state);
      DEBUG_PRINT_PAUSED_STATE(F("pause_state: "), pause_state);
    #endif
    bool msg_matched = false;

    #if HAS_LEVELING
      static uint8_t probe_cnt = 0;
    #endif

    // The only way to get printer status is to parse messages
    // Use the state to minimise the work we do here.
    switch (printer_state) {
      #if HAS_LEVELING
        case AC_printer_probing:

          if (strncmp_P(msg, MARLIN_msg_probing_point, strlen_P(MARLIN_msg_probing_point)) == 0)
            probe_cnt++;

          // If probing completes ok save the mesh and park
          // Ignore the custom machine name
          if (strcmp_P(msg + strlen(MACHINE_NAME), MARLIN_msg_ready) == 0) {
            if (probe_cnt == GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y) {
              probe_cnt = 0;
              injectCommands(F("M500"));    // G27 park nozzle
              //ChangePageOfTFT(PAGE_PreLEVEL);
              FakeChangePageOfTFT(PAGE_PreLEVEL); // Prevent UI refreshing too quickly when probing is done
              printer_state = AC_printer_idle;
              msg_matched   = true;
            }
          }

          // If probing fails don't save the mesh raise the probe above the bad point
          if (strcmp_P(msg, MARLIN_msg_probing_failed) == 0) {
            PlayTune(BEEPER_PIN, BeepBeepBeeep, 1);
            injectCommands(F("G1 Z50 F500"));
            ChangePageOfTFT(PAGE_CHS_ABNORMAL_LEVELING_SENSOR);
            SendtoTFTLN(AC_msg_probing_complete);
            printer_state = AC_printer_idle;
            msg_matched   = true;
          }

          if (strcmp_P(msg, MARLIN_msg_probe_preheat_start) == 0)
            ChangePageOfTFT(PAGE_CHS_PROBE_PREHEATING);

          if (strcmp_P(msg, MARLIN_msg_probe_preheat_stop) == 0)
            ChangePageOfTFT(PAGE_LEVELING);

          break;
      #endif

      case AC_printer_printing:
        if (strcmp_P(msg, MARLIN_msg_reheating) == 0) {
          SendtoTFTLN(AC_msg_paused); // enable continue button
          ChangePageOfTFT(PAGE_STATUS2);
          msg_matched = true;
        }
        else if (strcmp_P(msg, MARLIN_msg_media_removed) == 0) {
          msg_matched   = true;
          printer_state = AC_printer_stopping_from_media_remove;
        }
        else {
          #if ENABLED(FILAMENT_RUNOUT_SENSOR)
            #if ACDEBUG(AC_MARLIN)
              DEBUG_ECHOLNPGM("setFilamentRunoutState: ", __LINE__);
            #endif
            setFilamentRunoutState(false);
          #endif
        }
        break;

      case AC_printer_pausing:
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {
          if (pause_state != AC_paused_filament_lack) {
            ChangePageOfTFT(PAGE_STATUS1);        // show resume
            pause_state = AC_paused_idle;
          }

          printer_state = AC_printer_paused;
          msg_matched = true;
        }
        break;

      case AC_printer_paused:
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {
          if (pause_state != AC_paused_filament_lack) {
            ChangePageOfTFT(PAGE_STATUS1);        // show resume
            pause_state = AC_paused_idle;
          }

          printer_state = AC_printer_paused;
          msg_matched = true;
        }
        break;

      case AC_printer_stopping:
        if (strcmp_P(msg, MARLIN_msg_print_aborted) == 0) {
          ChangePageOfTFT(PAGE_MAIN);
          printer_state = AC_printer_idle;
          msg_matched   = true;
        }
        break;

      default: break;
    }

    // If not matched earlier see if this was a heater message
    if (!msg_matched) {
      #if HAS_HOTEND
        if (strcmp_P(msg, MARLIN_msg_extruder_heating) == 0) {
          SendtoTFTLN(AC_msg_nozzle_heating);
          hotend_state = AC_heater_temp_set;
          return;
        }
      #endif
      #if HAS_HEATED_BED
        if (strcmp_P(msg, MARLIN_msg_bed_heating) == 0) {
          SendtoTFTLN(AC_msg_bed_heating);
          hotbed_state = AC_heater_temp_set;
        }
      #endif
    }
  }

  #if ENABLED(POWER_LOSS_RECOVERY)

    void DgusTFT::PowerLoss() {
      // On:  5A A5 05 82 00 82 00 00
      // Off: 5A A5 05 82 00 82 00 64
      uint8_t data[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x82, 0x00, uint8_t(recovery.enabled ? 0x00 : 0x64) };
      LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
    }

    void DgusTFT::PowerLossRecovery() {
      printer_state = AC_printer_resuming_from_power_outage; // Play tune to notify user we can recover.
    }

  #endif // POWER_LOSS_RECOVERY

  void DgusTFT::HomingStart() {
    if (!isPrintingFromMedia())
      ChangePageOfTFT(PAGE_CHS_HOMING);
  }

  void DgusTFT::HomingComplete() {
    if (lcd_info.language == ENG && page_index_last > 120)
      page_index_last -= 120;

    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("HomingComplete, line: ", __LINE__);
      DEBUG_ECHOLNPGM("page_index_last: ", page_index_last);
    #endif

    if (!isPrintingFromMedia())
      ChangePageOfTFT(page_index_last);
  }

  void DgusTFT::SendtoTFT(FSTR_P const fstr/*=nullptr*/) {  // A helper to print PROGMEM string to the panel
    #if ACDEBUG(AC_SOME)
      DEBUG_ECHOF(fstr);
    #endif
    PGM_P str = FTOP(fstr);
    while (const char c = pgm_read_byte(str++)) TFTSer.write(c);
  }

  void DgusTFT::SendtoTFTLN(FSTR_P const fstr/*=nullptr*/) {
    if (fstr) {
      #if ACDEBUG(AC_SOME)
        DEBUG_ECHOPGM("> ");
      #endif
      SendtoTFT(fstr);
      #if ACDEBUG(AC_SOME)
        SERIAL_EOL();
      #endif
    }
    TFTSer.println();
  }

  void DgusTFT::SendValueToTFT(const uint16_t value, const uint16_t address) {
    uint8_t data[] = { 0x5A, 0xA5, 0x05, 0x82, uint8_t(address >> 8), uint8_t(address & 0xFF), uint8_t(value >> 8), uint8_t(value & 0xFF) };
    LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
  }

  void DgusTFT::RequestValueFromTFT(const uint16_t address) {
    uint8_t data[] = { 0x5A, 0xA5, 0x04, 0x83, uint8_t(address >> 8), uint8_t(address & 0xFF), 0x01 };
    LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
  }

  void DgusTFT::SendTxtToTFT(const char *pdata, const uint16_t address) {
    uint8_t data_len = strlen(pdata);
    uint8_t data[] = { 0x5A, 0xA5, uint8_t(data_len + 5), 0x82, uint8_t(address >> 8), uint8_t(address & 0xFF) };
    LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
    LOOP_L_N(i, data_len) TFTSer.write(pdata[i]);
    TFTSer.write(0xFF); TFTSer.write(0xFF);
  }

  void DgusTFT::SendColorToTFT(const uint16_t color, const uint16_t address) {
    uint8_t data[] = { 0x5A, 0xA5, 0x05, 0x82, uint8_t(address >> 8), uint8_t(address & 0xFF), uint8_t(color >> 8), uint8_t(color & 0xFF) };
    LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
  }

  void DgusTFT::SendReadNumOfTxtToTFT(const uint8_t number, const uint16_t address) {
    uint8_t data[] = { 0x5A, 0xA5, 0x04, 0x83, uint8_t(address >> 8), uint8_t(address & 0xFF), number };
    LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
  }

  void DgusTFT::ChangePageOfTFT(const uint16_t page_index, const bool no_send/*=false*/) {
    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("ChangePageOfTFT: ", page_index);
    #endif

    uint32_t data_temp = 0;

    if (lcd_info.language == CHS) {
      data_temp = page_index;
    }
    else if (lcd_info.language == ENG) {
      switch (page_index) {
        case PAGE_OUTAGE_RECOVERY:
          data_temp = PAGE_ENG_OUTAGE_RECOVERY; break;
        case PAGE_CHS_PROBE_PREHEATING:
          data_temp = PAGE_ENG_PROBE_PREHEATING; break;
        case PAGE_CHS_HOMING ... PAGE_ENG_HOMING:
          data_temp = page_index + 12; break;
        case PAGE_CHS_PROBE_PRECHECK ... PAGE_CHS_PROBE_PRECHECK_FAILED:
          data_temp = page_index + 3; break;
        default:
          data_temp = page_index + 120; break;
      }
    }

    if (!no_send) {
      uint8_t data[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, uint8_t(data_temp >> 8), uint8_t(data_temp & 0xFF) };
      LOOP_L_N(i, COUNT(data)) TFTSer.write(data[i]);
    }

    page_index_last_2 = page_index_last;
    page_index_last   = page_index_now;
    page_index_now    = data_temp;

    #if ACDEBUG(AC_MARLIN)
      DEBUG_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
      DEBUG_ECHOLNPGM("page_index_last: ", page_index_last);
      DEBUG_ECHOLNPGM("page_index_now: ", page_index_now);
    #endif
  }

  void DgusTFT::FakeChangePageOfTFT(const uint16_t page_index) {
    #if ACDEBUG(AC_MARLIN)
      if (page_index_saved != page_index_now)
        DEBUG_ECHOLNPGM("FakeChangePageOfTFT: ", page_index);
    #endif
    ChangePageOfTFT(page_index, true);
  }

  void DgusTFT::LcdAudioSet(const bool audio_on) {
    // On:  5A A5 07 82 00 80 5A 00 00 1A
    // Off: 5A A5 07 82 00 80 5A 00 00 12
    uint8_t data[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, uint8_t(audio_on ? 0x1A : 0x12) };
    LOOP_L_N(i, 10) TFTSer.write(data[i]);
  }

  bool DgusTFT::ReadTFTCommand() {
    static uint8_t length = 0, cnt = 0, tft_receive_steps = 0;
    uint8_t data;

    if (0 == TFTSer.available() || data_received) return false;

    data = TFTSer.read();

    if (tft_receive_steps == 0) {
      if (data != 0x5A) {
        cnt = 0;
        length = 0;
        data_index = 0;
        data_received = false;
        return false;
      }

      while (!TFTSer.available()) TERN_(USE_WATCHDOG, hal.watchdog_refresh());

      data = TFTSer.read();
      // MYSERIAL.write(data );
      if (data == 0xA5) tft_receive_steps = 2;
    }
    else if (tft_receive_steps == 2) {
      length = data;
      tft_receive_steps = 3;
      data_index = 0;
      cnt = 0;
    }
    else if (tft_receive_steps == 3) {
      if (data_index >= (DATA_BUF_SIZE -1)) {
        #if ACDEBUG(AC_MARLIN)
          DEBUG_ECHOLNPGM("lcd uart buff overflow: ", data_index);
        #endif
        data_index = 0;
        data_received = false;
        return false;
      }
      data_buf[data_index++] = data;
      cnt++;
      if (cnt >= length) {   // Receive complete
        tft_receive_steps = 0;
        cnt = 0;
        data_index = 0;
        data_received = true;
        return true;
      }
    }

    return false;
  }

  #if 0
    {
      //SERIAL_ECHOLNPGM("ReadTFTCommand: ", millis());
      //return -1;

      bool command_ready = false;
      uint8_t data       = 0;

      while (TFTSer.available() > 0 && command_len < MAX_CMND_LEN) {
        data = TFTSer.read();
        if (0 == command_len) {
          // if
        }

        panel_command[command_len] =
          if (panel_command[command_len] == '\n') {
          command_ready = true;
          break;
        }
        command_len++;
      }

      if (command_ready) {
        panel_command[command_len] = 0x00;
        #if ACDEBUG(AC_ALL)
          DEBUG_ECHOLNPGM("< panel_command ", panel_command);
        #endif
        #if ACDEBUG(AC_SOME)
          // Ignore status request commands
          uint8_t req = atoi(&panel_command[1]);
          if (req > 7 && req != 20) {
            DEBUG_ECHOLNPGM("> ", panel_command);
            DEBUG_PRINT_PRINTER_STATE(F("printer_state: "), printer_state);
          }
        #endif
      }
      return command_ready;
    }
  #endif

  int8_t DgusTFT::Findcmndpos(const char * buff, const char q) {
    for (int8_t pos = 0; pos < MAX_CMND_LEN; ++pos)
      if (buff[pos] == q) return pos;
    return -1;
  }

  void DgusTFT::CheckHeaters() {
    static uint32_t time_last = 0;
    if (PENDING(millis(), time_last)) return;
    time_last = millis() + 500;

    float temp = 0;

    #if HAS_HOTEND
      // If the hotend temp is abnormal, confirm state before signalling panel
      static uint8_t faultE0Duration = 0;
      temp = getActualTemp_celsius(E0);
      if (!WITHIN(temp, HEATER_0_MINTEMP, HEATER_0_MAXTEMP)) {
        faultE0Duration++;
        if (faultE0Duration >= AC_HEATER_FAULT_VALIDATION_TIME) {
          SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
          #if ACDEBUG(AC_MARLIN)
            DEBUG_ECHOLNPGM("Extruder temp abnormal! : ", temp);
          #endif
          faultE0Duration = 0;
        }
      }
    #endif

    #if HAS_HEATED_BED
      static uint8_t faultBedDuration = 0;
      temp = getActualTemp_celsius(BED);
      if (!WITHIN(temp, BED_MINTEMP, BED_MAXTEMP)) {
        faultBedDuration++;
        if (faultBedDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
          SendtoTFTLN(AC_msg_bed_temp_abnormal);
          #if ACDEBUG(AC_MARLIN)
            DEBUG_ECHOLNPGM("Bed temp abnormal! : ", temp);
          #endif
          faultBedDuration = 0;
        }
      }
    #endif

    #if 0
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
    #endif
  }

  void DgusTFT::SendFileList(int8_t startindex) {
    // Respond to panel request for 4 files starting at index
    #if ACDEBUG(AC_INFO)
      DEBUG_ECHOLNPGM("## SendFileList ## ", startindex);
    #endif
    filenavigator.getFiles(startindex);
  }

  void DgusTFT::SelectFile() {
    strncpy(selectedfile, panel_command + 4, command_len - 4);
    selectedfile[command_len - 5] = '\0';
    #if ACDEBUG(AC_FILE)
      DEBUG_ECHOLNPGM(" Selected File: ", selectedfile);
    #endif
    switch (selectedfile[0]) {
      case '/':   // Valid file selected
        SendtoTFTLN(AC_msg_sd_file_open_success);
        break;
      case '<':   // .. (go up folder level)
        filenavigator.upDIR();
        SendtoTFTLN(AC_msg_sd_file_open_failed);
        SendFileList(0);
        break;
      default:   // enter sub folder
        filenavigator.changeDIR(selectedfile);
        SendtoTFTLN(AC_msg_sd_file_open_failed);
        SendFileList(0);
        break;
    }
  }

  void DgusTFT::ProcessPanelRequest() {
    uint16_t control_index = 0;
    uint32_t control_value;
    uint16_t temp;
    char str_buf[20];

    if (data_received) {
      data_received = false;

      if (0x83 == data_buf[0]) {
        control_index = uint16_t(data_buf[1] << 8) | uint16_t(data_buf[2]);
        if ((control_index & 0xF000) == KEY_ADDRESS) { // is KEY
          //key_index = control_index;
          key_value = (uint16_t(data_buf[4]) << 8) | uint16_t(data_buf[5]);
        }

        #if HAS_HOTEND
          else if (control_index == TXT_HOTEND_TARGET || control_index == TXT_ADJUST_HOTEND) { // hotend target temp
            control_value = (uint16_t(data_buf[4]) << 8) | uint16_t(data_buf[5]);
            temp = constrain(uint16_t(control_value), 0, HEATER_0_MAXTEMP);
            setTargetTemp_celsius(temp, E0);
            //sprintf(str_buf,"%u/%u", (uint16_t)thermalManager.degHotend(0), uint16_t(control_value));
            //SendTxtToTFT(str_buf, TXT_PRINT_HOTEND);
          }
        #endif

        #if HAS_HEATED_BED
          else if (control_index == TXT_BED_TARGET || control_index == TXT_ADJUST_BED) {// bed target temp
            control_value = (uint16_t(data_buf[4]) << 8) | uint16_t(data_buf[5]);
            temp = constrain(uint16_t(control_value), 0, BED_MAXTEMP);
            setTargetTemp_celsius(temp, BED);
            //sprintf(str_buf,"%u/%u", uint16_t(thermalManager.degBed()), uint16_t(control_value));
            //SendTxtToTFT(str_buf, TXT_PRINT_BED);
          }
        #endif

        #if HAS_FAN
          else if (control_index == TXT_FAN_SPEED_TARGET) { // fan speed
            control_value = (uint16_t(data_buf[4]) << 8) | uint16_t(data_buf[5]);
            temp = constrain(uint16_t(control_value), 0, 100);
            SendValueToTFT(temp, TXT_FAN_SPEED_NOW);
            SendValueToTFT(temp, TXT_FAN_SPEED_TARGET);
            setTargetFan_percent(temp, FAN0);
          }
        #endif

        else if (control_index == TXT_PRINT_SPEED_TARGET || control_index == TXT_ADJUST_SPEED) { // print speed
          control_value = (uint16_t(data_buf[4]) << 8) | uint16_t(data_buf[5]);
          const uint16_t feedrate = constrain(uint16_t(control_value), 40, 999);
          //feedrate_percentage=constrain(control_value,40,999);
          sprintf(str_buf, "%u", feedrate);
          SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
          SendValueToTFT(feedrate, TXT_PRINT_SPEED_NOW);
          SendValueToTFT(feedrate, TXT_PRINT_SPEED_TARGET);
          setFeedrate_percent(feedrate);
        }

        else if (control_index == REG_LCD_READY) {
          control_value = (uint32_t(data_buf[3]) << 16) | (uint32_t(data_buf[4]) << 8) | uint32_t(data_buf[5]);
          if (control_value == 0x010072) { // startup last gif
            LcdAudioSet(lcd_info.audio_on);

            SendValueToTFT(2, ADDRESS_MOVE_DISTANCE);

            #if ENABLED(CASE_LIGHT_ENABLE)
              SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
              SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS);
            #endif

            #if ENABLED(POWER_LOSS_RECOVERY)
              const bool is_outage = AC_printer_resuming_from_power_outage == printer_state;
              if (is_outage) {
                ChangePageOfTFT(PAGE_OUTAGE_RECOVERY);
                #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
                  char filename[64] = { '\0' };
                  card.getLongPath(filename, recovery.info.sd_filename);
                  SendTxtToTFT(filename, TXT_OUTAGE_RECOVERY_FILE);
                #else
                  SendTxtToTFT(recovery.info.sd_filename, TXT_OUTAGE_RECOVERY_FILE);
                #endif
                PlayTune(BEEPER_PIN, SOS, 1);
              }
            #else
              constexpr bool is_outage = false;
            #endif

            if (!is_outage) ChangePageOfTFT(PAGE_MAIN);

          }
          else if (control_value == 0x010000) {         // startup first gif
            PlayTune(BEEPER_PIN, Anycubic_PowerOn, 1);  // takes 3500 ms
          }
        }

        /*
        else if ((control_index & 0xF000) == 0x2000) { // is TXT ADDRESS
          tft_txt_index = control_index;
          j = 0;
          for (i = 4; ;i++) {
            tft_txt_buf[j] = data_buf[i];
            if (tft_txt_buf[j] == 0xFF) {
              tft_txt_buf[j] = 0;
              break;
            }
            j++;
          }
        }
        */
      }
      else if (0x82 == data_buf[0]) {
        // send_cmd_to_pc(cmd ,start );
      }
    }
  }

  #if 0
    {
      // Break these up into logical blocks // as its easier to navigate than one huge switch case!
      int8_t req = atoi(&panel_command[1]);

      // Information requests A0 - A8 and A33
      if (req <= 8 || req == 33) PanelInfo(req);

      // Simple Actions A9 - A28
      else if (req <= 28) PanelAction(req);

      // Process Initiation
      else if (req <= 34) PanelProcess(req);

      else SendtoTFTLN();
    }
  #endif

  void DgusTFT::set_language(language_t language) {
    lcd_info.language = ui_language = lcd_info_back.language = language;
  }

  void DgusTFT::toggle_language() {
    lcd_info.language = ui_language = (lcd_info.language == CHS ? ENG : CHS);
  }

  void DgusTFT::goto_system_page() {
    ChangePageOfTFT(lcd_info.language == CHS
      ? (lcd_info.audio_on ? PAGE_SYSTEM_CHS_AUDIO_ON : PAGE_SYSTEM_CHS_AUDIO_OFF)
      : (lcd_info.audio_on ? 11 : 50) // PAGE_SYSTEM_ENG_AUDIO_ON/OFF - 120
    );
  }

  void DgusTFT::toggle_audio() {
    lcd_info.audio_on = !lcd_info.audio_on;
    goto_system_page();
    LcdAudioSet(lcd_info.audio_on);
  }

  void DgusTFT::store_changes() {
    if (lcd_info_back.language != lcd_info.language || lcd_info_back.audio_on != lcd_info.audio_on) {
      lcd_info_back = lcd_info;
      injectCommands(F("M500"));
    }
  }

  #if HAS_HOTEND
    void DgusTFT::send_temperature_hotend(uint32_t addr) {
      char str_buf[16];
      sprintf(str_buf, "%u/%u", uint16_t(getActualTemp_celsius(E0)), uint16_t(getTargetTemp_celsius(E0)));
      SendTxtToTFT(str_buf, addr);
    }
  #endif

  #if HAS_HEATED_BED
    void DgusTFT::send_temperature_bed(uint32_t addr) {
      char str_buf[16];
      sprintf(str_buf, "%u/%u", uint16_t(getActualTemp_celsius(BED)), uint16_t(getTargetTemp_celsius(BED)));
      SendTxtToTFT(str_buf, addr);
    }
  #endif

  void DgusTFT::page1() {
    #if ACDEBUG(AC_ALL)
      if (page_index_saved != page_index_now || key_value_saved != key_value) {
        DEBUG_ECHOLNPGM("page1  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1: { // main page, print
        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          set_descript_color(COLOR_BLUE);
          lcd_txtbox_index = 0;
        }
        ChangePageOfTFT(PAGE_FILE);
        SendFileList(0);
      } break;

      case 2: { // tool
        ChangePageOfTFT(PAGE_TOOL);
        #if ENABLED(CASE_LIGHT_ENABLE)
          SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
        #endif
      } break;

      case 3: // prepare
        ChangePageOfTFT(PAGE_PREPARE);
        break;

      case 4: // system
        goto_system_page();
        break;
    }

    #if 0
      if (message_index < 30) {
        SendTxtToTFT(p_mesage[message_index], TXT_MAIN_MESSAGE);
        message_index = 30;
      }
    #endif

    #if HAS_HOTEND || HAS_HEATED_BED
      static millis_t flash_time = 0;
      const millis_t ms = millis();
      if (PENDING(ms, flash_time)) return;
      flash_time = ms + 1500;

      TERN_(HAS_HOTEND, send_temperature_hotend(TXT_PREHEAT_HOTEND));
      TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_PREHEAT_BED));
    #endif
  }

  void DgusTFT::page2() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page2  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    char file_index = 0;

    switch (key_value) {
      case 0: break;

      case 1: // return
        ChangePageOfTFT(PAGE_MAIN);
        set_descript_color(COLOR_BLUE);
        break;

      case 2: // page up
        if (lcd_txtbox_page > 0) {
          lcd_txtbox_page--;

          set_descript_color(COLOR_BLUE);
          lcd_txtbox_index = 0;

          SendFileList(lcd_txtbox_page * 5);
        }
        break;

      case 3: // page down
        if ((lcd_txtbox_page + 1) * 5 < filenavigator.getFileNum()) {
          lcd_txtbox_page++;

          set_descript_color(COLOR_BLUE);
          lcd_txtbox_index = 0;

          SendFileList(lcd_txtbox_page * 5);
        }
        break;

      case 4:   // page refresh
        if (!isMediaInserted()) safe_delay(500);

        filenavigator.reset();

        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          set_descript_color(COLOR_BLUE);
          lcd_txtbox_index = 0;
        }
        SendFileList(lcd_txtbox_index);
        break;

      case 5: // resume of outage(last power off)
        #if ACDEBUG(AC_MARLIN)
          DEBUG_PRINT_PRINTER_STATE(F("printer_state: "), printer_state);
        #endif
        if (lcd_txtbox_index > 0 && lcd_txtbox_index  < 6) {   // 1~5

          if (filenavigator.filelist.seek(lcd_txtbox_page * 5 + (lcd_txtbox_index - 1))) {

            set_descript_color(COLOR_BLUE);

            TERN_(CASE_LIGHT_ENABLE, setCaseLightState(true));

            char str_buf[20];
            strncpy_P(str_buf, filenavigator.filelist.longFilename(), 17);
            str_buf[17] = '\0';
            SendTxtToTFT(str_buf, TXT_PRINT_NAME);

            #if ENABLED(POWER_LOSS_RECOVERY)
              if (printer_state == AC_printer_resuming_from_power_outage) {
                // Need to home here to restore the Z position
                //injectCommands_P(AC_cmnd_power_loss_recovery);
                //SERIAL_ECHOLNPGM("start resuming from power outage: ", AC_cmnd_power_loss_recovery);
                ChangePageOfTFT(PAGE_STATUS2);    // show pause
                injectCommands(F("M1000"));       // home and start recovery
              }
            #endif
          }
        }
        break;

      case 6: // start print
        if (lcd_txtbox_index > 0 && lcd_txtbox_index  < 6) {    // 1~5

          if (filenavigator.filelist.seek(lcd_txtbox_page * 5 + lcd_txtbox_index - 1)) {
            #if 0
              SERIAL_ECHOLNPGM("start print: ", lcd_txtbox_page * 5 + (lcd_txtbox_index - 1));
              SERIAL_ECHOLNPGM("start print: ", filenavigator.filelist.shortFilename());
              SERIAL_ECHOLNPGM("start print: ", filenavigator.filelist.longFilename());
            #endif

            set_descript_color(COLOR_BLUE);

            // Allows printer to restart the job if we don't want to recover
            if (printer_state == AC_printer_resuming_from_power_outage) {
              injectCommands(F("M1000 C"));   // Cancel recovery
              printer_state = AC_printer_idle;
            }

            TERN_(CASE_LIGHT_ENABLE, setCaseLightState(true));
            printFile(filenavigator.filelist.shortFilename());

            char str_buf[20];
            strncpy_P(str_buf, filenavigator.filelist.longFilename(), 17);
            str_buf[17] = '\0';
            SendTxtToTFT(str_buf, TXT_PRINT_NAME);

            sprintf(str_buf, "%5.2f", getFeedrate_percent());
            SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

            sprintf(str_buf, "%u", uint16_t(getProgress_percent()));
            SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

            uint32_t time = 0;
            sprintf(str_buf, "%s H ", utostr3(time / 60));
            sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
            SendTxtToTFT(str_buf, TXT_PRINT_TIME);

            ChangePageOfTFT(PAGE_STATUS2);
          }
        }
        break;

      case 7: // txtbox 1 click
      case 8: // txtbox 2 click
      case 9: // txtbox 3 click
      case 10: // txtbox 4 click

      case 11: { // txtbox 5 click
        static uint8_t lcd_txtbox_index_last = 0;

        lcd_txtbox_index = key_value - 6;

        // lcd_txtbox_page 0~...
        // lcd_txtbox_index 1~5
        file_index = lcd_txtbox_page * 5 + (lcd_txtbox_index - 1);
        if (file_index < filenavigator.getFileNum()) {

          set_descript_color(COLOR_RED);

          if (lcd_txtbox_index_last && lcd_txtbox_index_last != lcd_txtbox_index)    // 1~5
            set_descript_color(COLOR_BLUE, lcd_txtbox_index_last);
          lcd_txtbox_index_last = lcd_txtbox_index;
        }
      } break;
    }
  }

  void DgusTFT::page3() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page3  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    char str_buf[20];
    static uint8_t progress_last = 0;

    switch (key_value) {
      case 0: break;

      case 1:    // return
        if (!isPrintingFromMedia()) // only idle status can return
          ChangePageOfTFT(PAGE_FILE);
        break;

      case 2:     // resume print
        #if ACDEBUG(AC_MARLIN)
          DEBUG_PRINT_PRINTER_STATE(F("printer_state: "), printer_state);
          DEBUG_PRINT_PAUSED_STATE(F("pause_state :"), pause_state);
        #endif
        if ( pause_state == AC_paused_idle
          || pause_state == AC_paused_filament_lack
          || printer_state == AC_printer_resuming_from_power_outage
        ) {
          printer_state = AC_printer_idle;
          pause_state = AC_paused_idle;
          resumePrint();
          ChangePageOfTFT(PAGE_STATUS2);        // show pause print
          flash_time = ms + 1500;
        }
        else
          setUserConfirmed();
        break;

      case 3:     // print stop
        if (isPrintingFromMedia())
          ChangePageOfTFT(PAGE_STOP_CONF);
        break;

      case 4:     // print change param
        ChangePageOfTFT(PAGE_ADJUST);
        TERN_(CASE_LIGHT_ENABLE, SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS));
        TERN_(HAS_HOTEND, SendValueToTFT(uint16_t(getTargetTemp_celsius(E0)), TXT_ADJUST_HOTEND));
        TERN_(HAS_HEATED_BED, SendValueToTFT(uint16_t(getTargetTemp_celsius(BED)), TXT_ADJUST_BED));
        feedrate_back = getFeedrate_percent();
        SendValueToTFT(uint16_t(feedrate_back), TXT_ADJUST_SPEED);
        flash_time = ms + 1500;
        break;
    }

    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1500;

    if (feedrate_back != getFeedrate_percent()) {
      if (getFeedrate_percent() != 0)
        sprintf(str_buf, "%5.2f", getFeedrate_percent());
      else
        sprintf(str_buf, "%d", feedrate_back);

      #if ACDEBUG(AC_MARLIN)
        DEBUG_ECHOLNPGM("print speed: ", str_buf);
        DEBUG_ECHOLNPGM("feedrate_back: ", feedrate_back);
      #endif
      SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
      feedrate_back = getFeedrate_percent();
    }

    if (progress_last != getProgress_percent()) {
      sprintf(str_buf, "%u", getProgress_percent());
      SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);
      progress_last = getProgress_percent();
    }

    // Get Printing Time
    uint32_t time = getProgress_seconds_elapsed() / 60;
    sprintf(str_buf, "%s H ", utostr3(time / 60));
    sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
    SendTxtToTFT(str_buf, TXT_PRINT_TIME);

    TERN_(HAS_HOTEND, send_temperature_hotend(TXT_PRINT_HOTEND));
    TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_PRINT_BED));
  }

  void DgusTFT::page4() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page4  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    char str_buf[20];
    static uint8_t progress_last = 0;

    switch (key_value) {
      case 0: break;

      case 1:   // return
        if (!isPrintingFromMedia()) // only is idle status can return
          ChangePageOfTFT(PAGE_FILE);
        break;

      case 2:    // print pause
        if (isPrintingFromMedia()) {
          pausePrint();
          printer_state = AC_printer_pausing;
          pause_state = AC_paused_idle;
          ChangePageOfTFT(PAGE_WAIT_PAUSE);
          //injectCommands(F("M108"));     // stop waiting temperature M109
        }
        break;

      case 3:   // print stop
        if (isPrintingFromMedia())
          ChangePageOfTFT(PAGE_STOP_CONF);
        break;

      case 4:   // print settings
        ChangePageOfTFT(PAGE_ADJUST);
        TERN_(CASE_LIGHT_ENABLE, SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS));
        TERN_(HAS_HOTEND, SendValueToTFT(uint16_t(getTargetTemp_celsius(E0)), TXT_ADJUST_HOTEND));
        TERN_(HAS_HEATED_BED, SendValueToTFT(uint16_t(getTargetTemp_celsius(BED)), TXT_ADJUST_BED));
        feedrate_back = getFeedrate_percent();
        SendValueToTFT((uint16_t)feedrate_back, TXT_ADJUST_SPEED);
        TERN_(HAS_FAN, SendValueToTFT(uint16_t(getActualFan_percent(FAN0)), TXT_FAN_SPEED_TARGET));
        str_buf[0] = 0;
        strcat(str_buf, ftostr52sprj(getZOffset_mm()) + 3);
        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
        //SendTxtToTFT(ftostr52sprj(getZOffset_mm()), TXT_LEVEL_OFFSET);
        RequestValueFromTFT(TXT_ADJUST_SPEED);  // attempt to make feedrate visible on visit to this page
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1500;

    if (feedrate_back != getFeedrate_percent()) {
      if (getFeedrate_percent() != 0)
        sprintf(str_buf, "%5.2f", getFeedrate_percent());
      else
        sprintf(str_buf, "%d", feedrate_back);

      SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
      feedrate_back = getFeedrate_percent();
    }

    if (progress_last != getProgress_percent()) {
      sprintf(str_buf, "%u", getProgress_percent());
      SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);
      progress_last = getProgress_percent();
    }

    uint32_t time = getProgress_seconds_elapsed() / 60;
    sprintf(str_buf, "%s H ", utostr3(time / 60));
    sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
    SendTxtToTFT(str_buf, TXT_PRINT_TIME);

    TERN_(HAS_HOTEND, send_temperature_hotend(TXT_PRINT_HOTEND));
    TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_PRINT_BED));
  }

  void DgusTFT::page5() {          // print settings
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page5  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    static bool z_change = false;

    switch (key_value) {
      case 0: break;

      case 1: // return
        if (AC_printer_printing == printer_state)
          ChangePageOfTFT(PAGE_STATUS2);  // show pause
        else if (AC_printer_paused == printer_state)
          ChangePageOfTFT(PAGE_STATUS1);  // show print
        break;

      #if ENABLED(MESH_EDIT_MENU)

        case 2: { // -
          float z_off = getZOffset_mm();
          //SERIAL_ECHOLNPGM("z_off: ", z_off);
          //setSoftEndstopState(false);
          if (z_off <= -5) return;
          z_off -= 0.05f;
          setZOffset_mm(z_off);

          char str_buf[10];
          str_buf[0] = 0;
          strcat(str_buf, ftostr52sprj(getZOffset_mm()) + 2);
          SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
          //SendTxtToTFT(ftostr52sprj(getZOffset_mm()), TXT_LEVEL_OFFSET);

          //if (isAxisPositionKnown(Z)) {  // Move Z axis
          //  SERIAL_ECHOLNPGM("Z now:", getAxisPosition_mm(Z));
          //  const float currZpos = getAxisPosition_mm(Z);
          //  setAxisPosition_mm(currZpos-0.05, Z);
          //  SERIAL_ECHOLNPGM("Z now:", getAxisPosition_mm(Z));
          //}

          #if ENABLED(BABYSTEPPING)
            int16_t steps = mmToWholeSteps(-0.05, Z);
            babystepAxis_steps(steps, Z);
          #endif

          GRID_LOOP(x, y) {
            const xy_uint8_t pos { x, y };
            const float currval = getMeshPoint(pos);
            #if ACDEBUG(AC_MARLIN)
              DEBUG_ECHOLNPGM("x: ", x, " y: ", y, " z: ", currval);
            #endif
            setMeshPoint(pos, constrain(currval - 0.05f, AC_LOWEST_MESHPOINT_VAL, 5));
          }

          z_change = true;

          //setSoftEndstopState(true);
        } break;

        case 3: { // +
          float z_off = getZOffset_mm();
          //SERIAL_ECHOLNPGM("z_off: ", z_off);
          //setSoftEndstopState(false);

          if (z_off >= 5) return;
          z_off += 0.05f;
          setZOffset_mm(z_off);

          char str_buf[10];
          str_buf[0] = '\0';
          strcat(str_buf, ftostr52sprj(getZOffset_mm()) + 2);
          SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
          //SendTxtToTFT(ftostr52sprj(getZOffset_mm()), TXT_LEVEL_OFFSET);

          //int16_t steps = mmToWholeSteps(constrain(Zshift,-0.05,0.05), Z);

          /*
          if (isAxisPositionKnown(Z)) {  // Move Z axis
            SERIAL_ECHOLNPGM("Z now:", getAxisPosition_mm(Z));
            const float currZpos = getAxisPosition_mm(Z);
            setAxisPosition_mm(currZpos-0.05, Z);
            SERIAL_ECHOLNPGM("Z now:", getAxisPosition_mm(Z));
          }
          */

          #if ENABLED(BABYSTEPPING)
            int16_t steps = mmToWholeSteps(0.05, Z);
            babystepAxis_steps(steps, Z);
          #endif

          GRID_LOOP(x, y) {
            const xy_uint8_t pos { x, y };
            const float currval = getMeshPoint(pos);
            //SERIAL_ECHOLNPGM("x: ", x, " y: ", y, " z: ", currval);
            setMeshPoint(pos, constrain(currval + 0.05f, AC_LOWEST_MESHPOINT_VAL, 5));
          }

          z_change = true;

          //setSoftEndstopState(true);
        } break;

      #endif // MESH_EDIT_MENU

      #if ENABLED(CASE_LIGHT_ENABLE)
        case 4: {   // light control
          const bool cls = !getCaseLightState();
          SendValueToTFT(cls, ADDRESS_PRINT_SETTING_LED_STATUS);
          setCaseLightState(cls);
        } break;
      #endif

      case 5:
        ChangePageOfTFT(PAGE_DONE);
        break;

      case 6: break;

      case 7:
        TERN_(HAS_HEATED_BED, RequestValueFromTFT(TXT_ADJUST_BED));
        RequestValueFromTFT(TXT_ADJUST_SPEED);
        TERN_(HAS_HOTEND, RequestValueFromTFT(TXT_ADJUST_HOTEND));
        TERN_(HAS_FAN, RequestValueFromTFT(TXT_FAN_SPEED_TARGET));

        if (z_change == true) {
          injectCommands(F("M500"));
          z_change = false;
        }

        if (AC_printer_printing == printer_state)
          ChangePageOfTFT(PAGE_STATUS2);    // show pause
        else if (AC_printer_paused == printer_state)
          ChangePageOfTFT(PAGE_STATUS1);    // show print

        break;
    }
  }

  void DgusTFT::page6() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page6  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;
      case 1: break;
    }
  }

  void DgusTFT::page7() { // tools
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page7  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;

      case 1:       // return
        ChangePageOfTFT(PAGE_MAIN);
        break;

      case 2:
        ChangePageOfTFT(PAGE_MOVE);
        break;

      case 3:       // set temperature
        ChangePageOfTFT(PAGE_TEMP);
        #if HAS_HOTEND
          SendValueToTFT(uint16_t(getActualTemp_celsius(E0)), TXT_HOTEND_NOW);
          SendValueToTFT(uint16_t(getTargetTemp_celsius(E0)), TXT_HOTEND_TARGET);
        #endif
        #if HAS_HEATED_BED
          SendValueToTFT(uint16_t(getActualTemp_celsius(BED)), TXT_BED_NOW);
          SendValueToTFT(uint16_t(getTargetTemp_celsius(BED)), TXT_BED_TARGET);
        #endif
        break;

      case 4:
        ChangePageOfTFT(PAGE_SPEED);
        #if HAS_FAN
          SendValueToTFT(uint16_t(getActualFan_percent(FAN0)), TXT_FAN_SPEED_NOW);
          SendValueToTFT(uint16_t(getTargetFan_percent(FAN0)), TXT_FAN_SPEED_TARGET);
        #endif
        SendValueToTFT(uint16_t(getFeedrate_percent()), TXT_PRINT_SPEED_NOW);
        SendValueToTFT(uint16_t(getFeedrate_percent()), TXT_PRINT_SPEED_TARGET);
        break;

      case 5:       // turn off the xyz motor
        if (!isMoving())
          stepper.disable_all_steppers();
        break;

      #if ENABLED(CASE_LIGHT_ENABLE)
        case 6: {   // light control
          const bool cls = !getCaseLightState();
          SendValueToTFT(cls, ADDRESS_SYSTEM_LED_STATUS);
          setCaseLightState(cls);
        } break;
      #endif
    }
  }

  void DgusTFT::page8() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page8  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    //static uint16_t movespeed = 50;
    static float move_dis = 1.0f;

    if (key_value == 2 || key_value == 4
        || key_value == 6 || key_value == 8
        || key_value == 10 || (key_value == 12 && !isMoving())
    ) {
      if (getAxisPosition_mm(Z) < 0) setAxisPosition_mm(0, Z, 8);
    }

    //  if (!planner.movesplanned())return;
    switch (key_value) {
      case 0:
        break;

      case 1:    // return
        ChangePageOfTFT(PAGE_TOOL);
        break;

      case 5:
        if (!isMoving())
          injectCommands(F("G28 X"));
        break;

      case 9:
        if (!isMoving())
          injectCommands(F("G28 Y"));
        break;

      case 13:
        if (!isMoving())
          injectCommands(F("G28 Z"));
        break;

      case 17:
        if (!isMoving())
          injectCommands(F("G28"));
        break;

      case 2:       // X-
        if (!isMoving())
          setAxisPosition_mm(getAxisPosition_mm(X) - move_dis, X, 50);
        break;

      case 4:       // X+
        if (!isMoving())
          setAxisPosition_mm(getAxisPosition_mm(X) + move_dis, X, 50);
        break;

      case 6:       // Y+
        if (!isMoving())
          setAxisPosition_mm(getAxisPosition_mm(Y) - move_dis, Y, 50);
        break;

      case 8:       // Y-
        if (!isMoving())
          setAxisPosition_mm(getAxisPosition_mm(Y) + move_dis, Y, 50);
        break;

      case 10:      // Z-
        if (!isMoving())
          setAxisPosition_mm(getAxisPosition_mm(Z) - move_dis, Z, 8);
        break;

      case 12:      // Z+
        if (!isMoving())
          setAxisPosition_mm(getAxisPosition_mm(Z) + move_dis, Z, 8);
        break;

      case 3:
        move_dis = 0.1f;
        SendValueToTFT(1, ADDRESS_MOVE_DISTANCE);
        break;

      case 7:
        move_dis = 1.0f;
        SendValueToTFT(2, ADDRESS_MOVE_DISTANCE);
        break;

      case 11:
        move_dis = 10.0f;
        SendValueToTFT(3, ADDRESS_MOVE_DISTANCE);
        break;

      //case 14:
      //  movespeed = 3000; //SERIAL_ECHOLN(movespeed);
      //  break;
      //
      //case 15:
      //  movespeed = 2000; //SERIAL_ECHOLN(movespeed);
      //  break;
      //
      //case 16:
      //  movespeed = 1000; //SERIAL_ECHOLN(movespeed);
      //  break;
    }
  }

  void DgusTFT::page9() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page9  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:    // return
        ChangePageOfTFT(PAGE_TOOL);
        break;

      case 2: break;
      case 3: break;
      case 4: break;
      case 5: break;

      case 6:     // cooling
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, BED);
        ChangePageOfTFT(PAGE_TOOL);
        break;

      case 7:     // send target temp
        RequestValueFromTFT(TXT_HOTEND_TARGET);
        RequestValueFromTFT(TXT_BED_TARGET);
        ChangePageOfTFT(PAGE_TOOL);
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1500;

    SendValueToTFT(uint16_t(getActualTemp_celsius(E0)), TXT_HOTEND_NOW);
    SendValueToTFT(uint16_t(getActualTemp_celsius(BED)), TXT_BED_NOW);
  }

  void DgusTFT::page10() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page10  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1:       // return
        ChangePageOfTFT(PAGE_TOOL);
        break;

      case 2: break;
      case 3: break;
      case 4: break;
      case 5: break;

      case 6:       // ok
        RequestValueFromTFT(TXT_FAN_SPEED_TARGET);
        RequestValueFromTFT(TXT_PRINT_SPEED_TARGET);
        ChangePageOfTFT(PAGE_TOOL);
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1500;

    SendValueToTFT(uint16_t(getActualFan_percent(FAN0)), TXT_FAN_SPEED_NOW);
    SendValueToTFT(uint16_t(getFeedrate_percent()), TXT_PRINT_SPEED_NOW);
  }

  void DgusTFT::page11() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page11  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;

      case 1:       // return
        ChangePageOfTFT(PAGE_MAIN);
        store_changes();
        break;

      case 2:       // language
        toggle_language();
        goto_system_page();
        break;

      case 3: break;

      case 4:       // audio
        toggle_audio();
        break;

      case 5: {      // about
        char str_ver[32];
        //sprintf(str_ver, "%04d-%02d-%02d %02d:%02d:%02d\n", BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
        sprintf(str_ver, MAIN_BOARD_FIRMWARE_VER);
        SendTxtToTFT(str_ver, TXT_VERSION);
        ChangePageOfTFT(PAGE_ABOUT);
      } break;

      case 6:
        ChangePageOfTFT(PAGE_RECORD);
        break;
    }
  }

  void DgusTFT::page12() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page12  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;
      case 1:        // return
        ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
        break;
    }
  }

  void DgusTFT::page13() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page13  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;

      case 1:    // return
        goto_system_page();
        break;

      case 2: break;
    }
  }

  void DgusTFT::page14() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page14  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;
      case 1: break; // return
      case 2: break;
      case 3: break;
      case 4: break;
    }
  }

  void DgusTFT::page15() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page15  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:        // return
        ChangePageOfTFT(PAGE_MAIN);
        break;

      case 2:
        ChangePageOfTFT(PAGE_PreLEVEL);
        break;

      #if HAS_HOTEND || HAS_HEATED_BED
        case 3: {
          ChangePageOfTFT(PAGE_PREHEAT);
          TERN_(HAS_HOTEND, send_temperature_hotend(TXT_PREHEAT_HOTEND));
          TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_PREHEAT_BED));
        } break;
      #endif

      #if HAS_EXTRUDERS
        case 4: {
          send_temperature_hotend(TXT_FILAMENT_TEMP);
          ChangePageOfTFT(PAGE_FILAMENT);
        } break;
      #endif
    }
  }

  void DgusTFT::page16() {    // AUTO LEVELING
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page16  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;
      case 1:            // return
        ChangePageOfTFT(PAGE_PREPARE);
        break;

      case 2:
        if (!isPrinting()) {
          //ChangePageOfTFT(PAGE_LEVEL_ENSURE);
          ChangePageOfTFT(PAGE_CHS_PROBE_PRECHECK);
        }
        break;

      case 3: {
        char str_buf[10];
        str_buf[0] = '\0';
        strcat(str_buf, ftostr52sprj(getZOffset_mm()) + 2);
        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
        //SendTxtToTFT(ftostr52sprj(getZOffset_mm()), TXT_LEVEL_OFFSET);
        ChangePageOfTFT(PAGE_LEVEL_ADVANCE);
      } break;

      case 4:
        ChangePageOfTFT(PAGE_AUTO_OFFSET);
        break;
    }
  }

  void DgusTFT::page17() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page17  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    float z_off;
    switch (key_value) {
      case 0: break;

      case 1:        // return
        ChangePageOfTFT(PAGE_PreLEVEL);
        break;

      case 2: {
        setSoftEndstopState(false);
        if (getZOffset_mm() <= -5) return;
        z_off = getZOffset_mm() - 0.01f;
        setZOffset_mm(z_off);

        char str_buf[10];
        strcat(str_buf, ftostr52sprj(getZOffset_mm()) + 2);
        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
        //SendTxtToTFT(ftostr52sprj(getZOffset_mm()), TXT_LEVEL_OFFSET);

        if (isAxisPositionKnown(Z)) {
          const float currZpos = getAxisPosition_mm(Z);
          setAxisPosition_mm(currZpos - 0.01f, Z);
        }

        setSoftEndstopState(true);
      } break;

      case 3: {
        setSoftEndstopState(false);
        if (getZOffset_mm() >= 5) return;
        z_off = getZOffset_mm() + 0.01f;
        setZOffset_mm(z_off);

        char str_buf[10];
        strcat(str_buf, ftostr52sprj(getZOffset_mm()) + 2);
        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
        //SendTxtToTFT(ftostr52sprj(getZOffset_mm()), TXT_LEVEL_OFFSET);

        if (isAxisPositionKnown(Z)) {          // Move Z axis
          const float currZpos = getAxisPosition_mm(Z);
          setAxisPosition_mm(currZpos + 0.01f, Z);
        }

        setSoftEndstopState(true);
      } break;

      case 4:
        #if ACDEBUG(AC_MARLIN)
          DEBUG_ECHOLNPGM("z off: ", ftostr52sprj(getZOffset_mm()));
        #endif
        #if HAS_LEVELING
          GRID_LOOP(x, y) {
            const xy_uint8_t pos { x, y };
            const float currval = getMeshPoint(pos);
            setMeshPoint(pos, constrain(currval + getZOffset_mm(), AC_LOWEST_MESHPOINT_VAL, 5));
          }
          injectCommands(F("M500"));
        #endif
        ChangePageOfTFT(PAGE_PREPARE);
        break;
    }
  }

  #if HAS_HOTEND || HAS_HEATED_BED

    void DgusTFT::page18() {     // preheat
      #if ACDEBUG(AC_ALL)
        if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
          DEBUG_ECHOLNPGM("page18  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
          page_index_saved = page_index_now;
          key_value_saved = key_value;
        }
      #endif

      switch (key_value) {
        case 0: break;

        case 1:         // return
          ChangePageOfTFT(PAGE_PREPARE);
          break;

        case 2:         // PLA
          TERN_(HAS_HOTEND, setTargetTemp_celsius(190, E0));
          TERN_(HAS_HEATED_BED, setTargetTemp_celsius(60, BED));
          ChangePageOfTFT(PAGE_PREHEAT);
          break;

        case 3:         // ABS
          TERN_(HAS_HOTEND, setTargetTemp_celsius(240, E0));
          TERN_(HAS_HEATED_BED, setTargetTemp_celsius(100, BED));
          ChangePageOfTFT(PAGE_PREHEAT);
          break;
      }

      static millis_t flash_time = 0;
      const millis_t ms = millis();
      if (PENDING(ms, flash_time)) return;
      flash_time = ms + 1500;

      TERN_(HAS_HOTEND, send_temperature_hotend(TXT_PREHEAT_HOTEND));
      TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_PREHEAT_BED));
    }

  #endif // HAS_HOTEND || HAS_HEATED_BED

  #if HAS_EXTRUDERS

    void DgusTFT::page19() {       // Filament
      #if ACDEBUG(AC_ALL)
        if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
          DEBUG_ECHOLNPGM("page19  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
          page_index_saved = page_index_now;
          key_value_saved = key_value;
        }
      #endif
      static char filament_status = 0;
      static millis_t flash_time  = 0;
      switch (key_value) {
        case 0: break;

        case 1:           // return
          filament_status = 0;
          injectCommands(F("G90"));
          ChangePageOfTFT(PAGE_PREPARE);
          break;

        case 2:           // Filament in
          if (getActualTemp_celsius(E0) < 220) {
            filament_status = 0;
            ChangePageOfTFT(PAGE_FILAMENT_HEAT);
          }
          else {
            filament_status = 1;
            injectCommands(F("G91"));
          }
          break;

        case 3:           // filament out
          if (getActualTemp_celsius(E0) < 220) {
            filament_status = 0;
            ChangePageOfTFT(PAGE_FILAMENT_HEAT);
          }
          else {
            filament_status = 2;
            injectCommands(F("G91"));
          }
          break;

        case 4:           // stop
          filament_status = 0;
          break;

      }

      const millis_t ms = millis();
      if (PENDING(ms, flash_time)) return;
      flash_time = ms + 1500;

      send_temperature_hotend(TXT_FILAMENT_TEMP);

      if (!isPrinting()) {
        if (filament_status == 1) {
          if (canMove(E0) && !commandsInQueue())
            injectCommands_P(AC_cmnd_manual_load_filament);
        }
        else if (filament_status == 2) {
          if (canMove(E0) && !commandsInQueue())
            injectCommands_P(AC_cmnd_manual_unload_filament);
        }
      }
    }

  #endif // HAS_EXTRUDERS

  void DgusTFT::page20() {       // confirm
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page20  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1:        // return
        ChangePageOfTFT(page_index_last);
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page21() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page21  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:        // return
        ChangePageOfTFT(page_index_last);
        break;

      case 2: break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page22() {       // print finish
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page22  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:          // OK to finish
        TERN_(CASE_LIGHT_ENABLE, setCaseLightState(false));
        ChangePageOfTFT(PAGE_MAIN);
        break;

      case 2: break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page23() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page23  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1: ChangePageOfTFT(page_index_last); break; // return
      case 2: ChangePageOfTFT(page_index_last); break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page24() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page24  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1: ChangePageOfTFT(page_index_last); break; // return
      case 2: ChangePageOfTFT(page_index_last); break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page25() {           // lack filament
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page25  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:             // return
        #if ACDEBUG(AC_MARLIN)
          DEBUG_PRINT_PRINTER_STATE(F("printer_state: "), printer_state);
          DEBUG_PRINT_PAUSED_STATE(F("pause_state: "), pause_state);
        #endif
        if (AC_printer_printing == printer_state)
          ChangePageOfTFT(PAGE_STATUS2);              // show pause
        else if (AC_printer_paused == printer_state) {
          //injectCommands(F("M108"));
          ChangePageOfTFT(PAGE_STATUS1);              // show resume
        }
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page26() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page26  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1: ChangePageOfTFT(page_index_last); break; // return
      case 2: break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page27() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page27  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:           // print stop confirmed
        if (isPrintingFromMedia()) {
          printer_state = AC_printer_stopping;
          stopPrint();
          message_index = 6;
          ChangePageOfTFT(PAGE_MAIN);
        }
        else {
          if (printer_state == AC_printer_resuming_from_power_outage)
            injectCommands(F("M1000 C"));         // Cancel recovery
          printer_state = AC_printer_idle;
        }
        break;

      case 2:           // return
        if (AC_printer_printing == printer_state)
          ChangePageOfTFT(PAGE_STATUS2);          // show pause
        else if (AC_printer_paused == printer_state)
          ChangePageOfTFT(PAGE_STATUS1);          // show print
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page28() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page28  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1: ChangePageOfTFT(page_index_last); break; // return
      case 2: break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page29() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page29  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:        // return
        TERN_(CASE_LIGHT_ENABLE, setCaseLightState(false));
        ChangePageOfTFT(PAGE_MAIN);
        break;

      case 2: break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page30() {       // Auto heat filament
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page30  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;

      case 1:           // return
        setTargetTemp_celsius(230, E0);
        ChangePageOfTFT(PAGE_FILAMENT);
        break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page31() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page31  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {
      case 0: break;
      case 1: break; // return
      case 2: break;
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page32() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page32  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  #if HAS_LEVELING

    void DgusTFT::page33() {
      #if ACDEBUG(AC_ALL)
        if (page_index_saved != page_index_now) {
          DEBUG_ECHOLNPGM("page33  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
          page_index_saved = page_index_now;
        }
      #endif

      switch (key_value) {
        case 0: break;

        case 1:         // auto leveling start
          injectCommands(F("G28\nG29"));
          printer_state = AC_printer_probing;

          // this will cause leveling->preheating->leveling
          #if 0
            #if ENABLED(PREHEAT_BEFORE_LEVELING)
              if (getTargetTemp_celsius(E0) < LEVELING_NOZZLE_TEMP
                 || getTargetTemp_celsius(BED) < LEVELING_BED_TEMP
              ) {
                setTargetTemp_celsius(LEVELING_NOZZLE_TEMP, E0);
                setTargetTemp_celsius(LEVELING_BED_TEMP, BED);
                ChangePageOfTFT(PAGE_CHS_PROBE_PREHEATING);
              }
              else
                ChangePageOfTFT(PAGE_LEVELING);

            #else
              ChangePageOfTFT(PAGE_LEVELING);
            #endif
          #endif

          ChangePageOfTFT(PAGE_LEVELING);
          break;

        case 2:
          ChangePageOfTFT(PAGE_PreLEVEL);
          break;
      }

      static millis_t flash_time = 0;
      const millis_t ms = millis();
      if (PENDING(ms, flash_time)) return;
      flash_time = ms + 1500;
    }

    void DgusTFT::page34() {
      #if ACDEBUG(AC_ALL)
        if ((page_index_saved != page_index_now) || (key_value_saved != key_value))  {
          DEBUG_ECHOLNPGM("page34  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
          page_index_saved = page_index_now;
          key_value_saved = key_value;
        }
      #endif

      #if HAS_HOTEND || HAS_HEATED_BED
        static millis_t flash_time = 0;
        const millis_t ms = millis();
        if (PENDING(ms, flash_time)) return;
        flash_time = ms + 1500;

        TERN_(HAS_HOTEND, send_temperature_hotend(TXT_MAIN_HOTEND));
        TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_MAIN_BED));
      #endif

      if (pop_up_index == 25) {
        pop_up_index = 100;
        ChangePageOfTFT(PAGE_PreLEVEL);
      }
    }

  #endif // HAS_LEVELING

  void DgusTFT::page115() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page115  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    switch (key_value) {

      case 0: break;
      case 1: ChangePageOfTFT(PAGE_PreLEVEL); break;

      case 2: {
        injectCommands(F("M1024 S3"));   // -1
        //char value[20]
        //sprintf_P(value, PSTR("G1 Z%iF%i")); enqueue_and_echo_command_now(value); }
      } break;

      case 3: injectCommands(F("M1024 S4")); break; // 1
      case 4: injectCommands(F("M1024 S1")); break; // -0.1
      case 5: injectCommands(F("M1024 S2")); break; // 0.1
      case 6: injectCommands(F("M1024 S0")); break; // prepare, move x y to center
      case 7: injectCommands(F("M1024 S5")); break; // 0.1
    }

    static millis_t flash_time = 0;
    const millis_t ms = millis();
    if (PENDING(ms, flash_time)) return;
    flash_time = ms + 1000;
  }

  void DgusTFT::page117() {  // Page CHS Mute handler
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page117  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;

      case 1:
        ChangePageOfTFT(PAGE_MAIN);
        store_changes();
        break;

      case 2:       // language
        toggle_language();
        goto_system_page();
        break;

      case 3: break;

      case 4:       // audio
        toggle_audio();
        break;

      case 5:       // about
        char str_ver[32];
        //sprintf(str_ver, "%04d-%02d-%02d %02d:%02d:%02d\n", BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
        sprintf(str_ver, MAIN_BOARD_FIRMWARE_VER);
        SendTxtToTFT(str_ver, TXT_VERSION);
        ChangePageOfTFT(PAGE_ABOUT);
        break;

      case 6:
        ChangePageOfTFT(PAGE_RECORD);
        break;
    }
  }

  void DgusTFT::page124() {  // first time into page 124 the feedrate percent is not set
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page124  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
        //DEBUG_ECHOLNPGM("update feedrate percent");
      }
    #endif
    SendValueToTFT(uint16_t(getFeedrate_percent()), TXT_PRINT_SPEED_NOW);
  }

  void DgusTFT::page125() {  // first time into page 125 the feedrate percent is not set
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page125  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
        //DEBUG_ECHOLNPGM("update feedrate percent");
      }
    #endif
    SendValueToTFT(uint16_t(getFeedrate_percent()), TXT_PRINT_SPEED_NOW);
  }

  void DgusTFT::page170() {  // ENG Mute handler
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page170  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 0: break;

      case 1:
        ChangePageOfTFT(PAGE_MAIN);
        store_changes();
        break;

      case 2:       // language
        toggle_language();
        goto_system_page();
        break;

      case 3: break;

      case 4:       // audio
        toggle_audio();
        break;

      case 5:       // about
        char str_ver[32];
        //sprintf(str_ver, "%04d-%02d-%02d %02d:%02d:%02d\n", BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
        sprintf(str_ver, MAIN_BOARD_FIRMWARE_VER);
        SendTxtToTFT(str_ver, TXT_VERSION);
        ChangePageOfTFT(PAGE_ABOUT);
        break;

      case 6:
        ChangePageOfTFT(PAGE_RECORD);
        break;
    }
  }

  #if ENABLED(POWER_LOSS_RECOVERY)

    void DgusTFT::page171() {  // CHS power outage resume handler
      #if ACDEBUG(AC_ALL)
        if (page_index_saved != page_index_now) {
          DEBUG_ECHOLNPGM("page171  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
          page_index_saved = page_index_now;
        }
      #endif
      #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
        char filename[64] = { '\0' };
      #endif

      switch (key_value) {
        case 0: break;

        case 1: {     // resume
          ChangePageOfTFT(PAGE_OUTAGE_RECOVERY);
          #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
            card.getLongPath(filename, recovery.info.sd_filename);
            filename[17] = '\0';
            SendTxtToTFT(filename, TXT_OUTAGE_RECOVERY_FILE);
          #else
            SendTxtToTFT(recovery.info.sd_filename, TXT_OUTAGE_RECOVERY_FILE);
          #endif

          char str_buf[20] = { '\0' };
          sprintf(str_buf, "%u", uint16_t(getFeedrate_percent()));
          SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

          sprintf(str_buf, "%u", uint16_t(getProgress_percent()));
          SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

          ChangePageOfTFT(PAGE_STATUS2);              // show pause
          injectCommands(F("M355 S1\nM1000"));        // case light on, home and start recovery
        } break;

        case 2:       // cancel
          printer_state = AC_printer_idle;
          ChangePageOfTFT(PAGE_MAIN);
          injectCommands(F("M355 S0\nM1000 C"));      // cancel recovery
          break;
      }
    }

    void DgusTFT::page173() {  // ENG power outage resume handler
      #if ACDEBUG(AC_ALL)
        if (page_index_saved != page_index_now) {
          DEBUG_ECHOLNPGM("page173  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
          page_index_saved = page_index_now;
        }
      #endif
      #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
        char filename[64] = { '\0' };
      #endif

      switch (key_value) {
        case 0: break;

        case 1: {     // resume
          ChangePageOfTFT(PAGE_OUTAGE_RECOVERY);
          #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
            card.getLongPath(filename, recovery.info.sd_filename);
            filename[17] = '\0';
            SendTxtToTFT(filename, TXT_OUTAGE_RECOVERY_FILE);
          #else
            SendTxtToTFT(recovery.info.sd_filename, TXT_OUTAGE_RECOVERY_FILE);
          #endif

          char str_buf[20] = { '\0' };
          sprintf(str_buf, "%u", uint16_t(getFeedrate_percent()));
          SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

          sprintf(str_buf, "%u", uint16_t(getProgress_percent()));
          SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

          ChangePageOfTFT(PAGE_STATUS2);          // show pause
          injectCommands(F("M355 S1\nM1000"));    // case light on, home and start recovery
        } break;

        case 2:       // cancel
          printer_state = AC_printer_idle;
          ChangePageOfTFT(PAGE_MAIN);
          injectCommands(F("M355 S0\nM1000 C"));  // cancel recovery
          break;
      }
    }

  #endif // POWER_LOSS_RECOVERY

  #if HAS_LEVELING

    void DgusTFT::page175() {     // CHS probe preheating handler
      #if ACDEBUG(AC_ALL)
        if (page_index_saved != page_index_now) {
          DEBUG_ECHOLNPGM("page175  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
          page_index_saved = page_index_now;
        }
      #endif

      #if HAS_HOTEND || HAS_HEATED_BED
        static millis_t flash_time = 0;
        const millis_t ms = millis();
        if (PENDING(ms, flash_time)) return;
        flash_time = ms + 1500;

        TERN_(HAS_HOTEND, send_temperature_hotend(TXT_MAIN_HOTEND));
        TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_MAIN_BED));
      #endif
    }

    void DgusTFT::page176() {     // ENG probe preheating handler
      #if ACDEBUG(AC_ALL)
        if (page_index_saved != page_index_now) {
          DEBUG_ECHOLNPGM("page176  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
          page_index_saved = page_index_now;
        }
      #endif

      #if HAS_HOTEND || HAS_HEATED_BED
        static millis_t flash_time = 0;
        const millis_t ms = millis();
        if (PENDING(ms, flash_time)) return;
        flash_time = ms + 1500;

        TERN_(HAS_HOTEND, send_temperature_hotend(TXT_MAIN_HOTEND));
        TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_MAIN_BED));
      #endif
    }

  #endif // HAS_LEVELING

  void DgusTFT::page177_to_198() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page177_to_198  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 1:       // return
        #if ACDEBUG(AC_MARLIN)
          //DEBUG_ECHOLNPGM("page_index_now: ", page_index_now);
          //DEBUG_ECHOLNPGM("page_index_last: ", page_index_last);
          //DEBUG_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
        #endif

        if ((WITHIN(page_index_now, PAGE_CHS_ABNORMAL_X_ENDSTOP, PAGE_CHS_ABNORMAL_Z_ENDSTOP))
         || (WITHIN(page_index_now, PAGE_ENG_ABNORMAL_X_ENDSTOP, PAGE_ENG_ABNORMAL_Z_ENDSTOP))
        ) {
          if (lcd_info.language == ENG) {
            if (page_index_last_2 > 120) page_index_last_2 -= 120;
            if (page_index_last > 120) page_index_last -= 120;
          }

          if (PAGE_STATUS1 == page_index_last_2 || PAGE_STATUS2 == page_index_last_2 || PAGE_PRINT_FINISH == page_index_last)
            ChangePageOfTFT(PAGE_MAIN);
          else
            ChangePageOfTFT(page_index_last_2);
        }
        else {
          if (lcd_info.language == ENG && page_index_last > 120)
            page_index_last -= 120;
          ChangePageOfTFT(page_index_last);
        }

        onSurviveInKilled();
        stepper.disable_all_steppers();
        break;

      default: break;
    }
  }

  #if 0
    void DgusTFT::page178_to_181_190_to_193() {  // temperature abnormal
      #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page178_to_181_190_to_193  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
      #endif
      switch (key_value) {
        case 1:     // return
          SERIAL_ECHOLNPGM("page_index_now: ", page_index_now);
          SERIAL_ECHOLNPGM("page_index_last: ", page_index_last);
          SERIAL_ECHOLNPGM("page_index_last_2: ", page_index_last_2);

          if (isPrinting() || isPrintingPaused() || isPrintingFromMedia()) {
            printer_state = AC_printer_stopping;
            stopPrint();
            ChangePageOfTFT(PAGE_MAIN);
          }
          else
            ChangePageOfTFT(page_index_last);

          onSurviveInKilled();
          break;

        default: break;
      }
    }
  #endif

  void DgusTFT::page199_to_200() {
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page199_to_200  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now, "  key: ", key_value);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    switch (key_value) {
      case 1:       // return
        #if ACDEBUG(AC_MARLIN)
          //DEBUG_ECHOLNPGM("page_index_now: ", page_index_now);
          //DEBUG_ECHOLNPGM("page_index_last: ", page_index_last);
          //DEBUG_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
        #endif
        onSurviveInKilled();
        ChangePageOfTFT(PAGE_PreLEVEL);
        break;

      default: break;
    }
  }

  inline void ProbeTare() {
    #if PIN_EXISTS(AUTO_LEVEL_TX)
      OUT_WRITE(AUTO_LEVEL_TX_PIN, LOW);
      delay(300);
      OUT_WRITE(AUTO_LEVEL_TX_PIN, HIGH);
      delay(100);
    #endif
  }

  inline bool getProbeState() { return PROBE_TRIGGERED(); }

  void DgusTFT::page201() {  // probe precheck
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page201  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    static millis_t probe_check_time   = 0;
    static millis_t temperature_time   = 0;
    static uint8_t probe_check_counter = 0;
    static uint8_t probe_state_last    = 0;
    static bool probe_tare_flag        = 0;

    if (!probe_tare_flag) {
      ProbeTare();

      delay(100);

      if (getProbeState()) {        // triggered too early
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_CHS_PROBE_PRECHECK_FAILED);
      }
      probe_tare_flag = 1;
    }

    switch (key_value) {
      case 1:     // cancel
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_PreLEVEL);
        break;

      default: break;
    }

    if (ELAPSED(millis(), probe_check_time)) {
      probe_check_time = millis() + 300;

      if (!probe_state_last && getProbeState()) {
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_CHS_PROBE_PRECHECK_OK);
      }

      probe_state_last = getProbeState();

      if (probe_check_counter++ >= 200) {         // waiting for 1 min
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_CHS_PROBE_PRECHECK_FAILED);
      }
    }

    if (ELAPSED(millis(), temperature_time)) {
      temperature_time = millis() + 1500;
      TERN_(HAS_HOTEND, send_temperature_hotend(TXT_MAIN_HOTEND));
      TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_MAIN_BED));
    }
  }

  void DgusTFT::page202() {  // probe precheck ok
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page202  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif

    //static millis_t flash_time = 0;
    //static millis_t probe_check_counter = 0;
    //static uint8_t probe_state_last = 0;

    delay(3000);

    injectCommands(F("G28\nG29"));
    printer_state = AC_printer_probing;
    ChangePageOfTFT(PAGE_LEVELING);
  }

  void DgusTFT::page203() {    // probe precheck failed
    #if ACDEBUG(AC_ALL)
      if ((page_index_saved != page_index_now) || (key_value_saved != key_value)) {
        DEBUG_ECHOLNPGM("page203  page_index_last_2: ", page_index_last_2,  "  page_index_last: ", page_index_last, "  page_index_now: ", page_index_now);
        page_index_saved = page_index_now;
        key_value_saved = key_value;
      }
    #endif
    //static millis_t probe_check_counter = 0;
    //static uint8_t probe_state_last = 0;

    #if HAS_HOTEND || HAS_HEATED_BED
      static millis_t flash_time = 0;
      const millis_t ms = millis();
      if (PENDING(ms, flash_time)) return;
      flash_time = ms + 1500;

      TERN_(HAS_HOTEND, send_temperature_hotend(TXT_MAIN_HOTEND));
      TERN_(HAS_HEATED_BED, send_temperature_bed(TXT_MAIN_BED));
    #endif
  }

  void DgusTFT::pop_up_manager() {
    #if ACDEBUG(AC_ALL)
      if (pop_up_index_saved != pop_up_index) {
        DEBUG_ECHOLNPGM("pop_up_manager  pop_up_index: ", pop_up_index);
        pop_up_index_saved = pop_up_index;
      }
    #endif

    switch (pop_up_index) {
      case 10:      // T0 error
        if (page_index_now != PAGE_ABNORMAL)
          ChangePageOfTFT(PAGE_ABNORMAL);
        pop_up_index = 100;
        break;

      case 15:      // filament lack
        if (page_index_now != PAGE_FILAMENT_LACK)
          ChangePageOfTFT(PAGE_FILAMENT_LACK);
        pop_up_index = 100;
        break;

      case 16:      // stop wait
        ChangePageOfTFT(PAGE_WAIT_STOP);
        pop_up_index = 100;
        break;

      case 18:
        ChangePageOfTFT(PAGE_STATUS1);
        pop_up_index = 100;
        break;

      case 23:      //
        if (page_index_now != PAGE_FILAMENT_LACK)
          ChangePageOfTFT(PAGE_FILAMENT_LACK);
        pop_up_index = 100;
        break;

      case 24: { //
        uint32_t time = getProgress_seconds_elapsed() / 60;
        char str_buf[20];
        sprintf(str_buf, "%s H ", utostr3(time / 60));
        sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
        SendTxtToTFT(str_buf, TXT_FINISH_TIME);
        ChangePageOfTFT(PAGE_PRINT_FINISH);
        //SendtoTFTLN(AC_msg_print_complete);   // no idea why this causes a compile error
        pop_up_index = 100;
      } break;

      case 25:  // LEVEL DONE
        ChangePageOfTFT(PAGE_PreLEVEL);
        pop_up_index = 100;
        break;
    }
  }

  void DEBUG_PRINT_PAUSED_STATE(FSTR_P const msg, paused_state_t state) {
    DEBUG_ECHOPGM(msg, state);
    switch (state) {
      case AC_paused_heater_timed_out:
        DEBUG_ECHOLNPGM("  AC_paused_heater_timed_out");
        break;
      case AC_paused_filament_lack:
        DEBUG_ECHOLNPGM("  AC_paused_filament_lack");
        break;
      case AC_paused_purging_filament:
        DEBUG_ECHOLNPGM("  AC_paused_purging_filament");
        break;
      case AC_paused_idle:
        DEBUG_ECHOLNPGM("  AC_paused_idle");
        break;
    }
  }

// routines to make the debug outputs human readable

  void DEBUG_PRINT_PRINTER_STATE(FSTR_P const msg, printer_state_t state) {
    DEBUG_ECHOPGM(msg, state);
    switch (state) {
      case AC_printer_idle:
        DEBUG_ECHOLNPGM("  AC_printer_idle");
        break;
      case AC_printer_probing:
        DEBUG_ECHOLNPGM("  AC_printer_probing");
        break;
      case AC_printer_printing:
        DEBUG_ECHOLNPGM("  AC_printer_printing");
        break;
      case AC_printer_pausing:
        DEBUG_ECHOLNPGM("  AC_printer_pausing");
        break;
      case AC_printer_paused:
        DEBUG_ECHOLNPGM("  AC_printer_paused");
        break;
      case AC_printer_stopping:
        DEBUG_ECHOLNPGM("  AC_printer_stopping");
        break;
      case AC_printer_stopping_from_media_remove:
        DEBUG_ECHOLNPGM("  AC_printer_stopping_from_media_remove");
        break;
      case AC_printer_resuming_from_power_outage:
        DEBUG_ECHOLNPGM("  AC_printer_resuming_from_power_outage");
        break;
    }
  }

  void DEBUG_PRINT_TIMER_EVENT(FSTR_P const msg, timer_event_t event) {
    DEBUG_ECHOPGM(msg, event);
    switch (event) {
      case AC_timer_started:
        DEBUG_ECHOLNPGM("  AC_timer_started");
        break;
      case AC_timer_paused:
        DEBUG_ECHOLNPGM("  AC_timer_paused");
        break;
      case AC_timer_stopped:
        DEBUG_ECHOLNPGM("  AC_timer_stopped");
        break;
    }
  }

  void DEBUG_PRINT_MEDIA_EVENT(FSTR_P const msg, media_event_t event) {
    DEBUG_ECHOPGM(msg, event);
    switch (event) {
      case AC_media_inserted:
        DEBUG_ECHOLNPGM("  AC_media_inserted");
        break;
      case AC_media_removed:
        DEBUG_ECHOLNPGM("  AC_media_removed");
        break;
      case AC_media_error:
        DEBUG_ECHOLNPGM("  AC_media_error");
        break;
    }
  }

} // namespace

#endif // ANYCUBIC_LCD_VYPER
