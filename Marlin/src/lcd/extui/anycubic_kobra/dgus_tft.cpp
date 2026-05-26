/**
   Marlin 3D Printer Firmware
   Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]

   Based on Sprinter and grbl.
   Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm`

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/**
   lcd/extui/lib/dgus_tft.cpp

   Extensible_UI implementation for Anycubic Chiron
   Written By Nick Wells, 2020 [https://github.com/SwiftNick]
    (not affiliated with Anycubic, Ltd.)
*/

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_KOBRA)

  #include "../../../MarlinCore.h"
  #if HAS_FILAMENT_SENSOR
    #include "../../../feature/runout.h"
  #endif
  #include "../../../gcode/queue.h"
  #include "../../../libs/numtostr.h"
  #include "../../../module/printcounter.h"
  #include "../../../module/probe.h"
  #include "../../../module/motion.h"
  #include "../../../module/stepper.h"
  #include "../../../sd/cardreader.h"
  #include "dgus_FileNavigator.h"
  #include "dgus_Tunes.h"
  #include "dgus_tft.h"
  #include "dgus_tft_defs.h"
  #include "dgus_util.h"

namespace Anycubic {

  const uint8_t MESSAGE_charu[] = {0xB4, 0xE6, 0xB4, 0xA2, 0xBF, 0xA8, 0xD2,
                                   0xD1, 0xB2, 0xE5, 0xC8, 0xEB, 0x00};
  const uint8_t MESSAGE_bachu[] = {0xB4, 0xE6, 0xB4, 0xA2, 0xBF, 0xA8, 0xD2,
                                   0xD1, 0xB0, 0xCE, 0xB3, 0xF6, 0x00};
  const uint8_t MESSAGE_wuka[] = {0xCE, 0xDE, 0xB4, 0xE6, 0xB4,
                                  0xA2, 0xBF, 0xA8, 0x00};
  const uint8_t MESSAGE_lianji[] = {0xC1, 0xAA, 0xBB, 0xFA, 0xD6, 0xD0, 0x00};
  const uint8_t MESSAGE_tuoji[] = {0xCD, 0xD1, 0xBB, 0xFA, 0xB4, 0xF2,
                                   0xD3, 0xA1, 0xD6, 0xD0, 0x00};
  const uint8_t MESSAGE_zanting[] = {0xB4, 0xF2, 0xD3, 0xA1, 0xD4, 0xDD,
                                     0xCD, 0xA3, 0xD6, 0xD0, 0x00};
  const uint8_t MESSAGE_tingzhi[] = {0xCD, 0xA3, 0xD6, 0xB9, 0xB4,
                                     0xF2, 0xD3, 0xA1, 0x00};
  const uint8_t MESSAGE_wancheng[] = {0xCD, 0xEA, 0xB3, 0xC9, 0xB4,
                                      0xF2, 0xD3, 0xA1, 0x00};
  const uint8_t MESSAGE_hotend_heating[] = {0xB4, 0xF2, 0xD3, 0xA1, 0xCD,
                                            0xB7, 0xD5, 0xFD, 0xD4, 0xDA,
                                            0xBC, 0xD3, 0xC8, 0xC8, 0x00};
  const uint8_t MESSAGE_hotend_over[] = {0xB4, 0xF2, 0xD3, 0xA1, 0xCD,
                                         0xB7, 0xBC, 0xD3, 0xC8, 0xC8,
                                         0xCD, 0xEA, 0xB3, 0xC9, 0x00};
  const uint8_t MESSAGE_bed_heating[] = {0xC8, 0xC8, 0xB4, 0xB2, 0xD5,
                                         0xFD, 0xD4, 0xDA, 0xBC, 0xD3,
                                         0xC8, 0xC8, 0x00};
  const uint8_t MESSAGE_bed_over[] = {0xC8, 0xC8, 0xB4, 0xB2, 0xBC, 0xD3, 0xC8,
                                      0xC8, 0xCD, 0xEA, 0xB3, 0xC9, 0x00};
  const uint8_t MESSAGE_ready[] = {0xD7, 0xBC, 0xB1, 0xB8, 0xBE,
                                   0xCD, 0xD0, 0xF7, 0x00};
  const uint8_t MESSAGE_cold[] = {0xB4, 0xF2, 0xD3, 0xA1, 0xCD,
                                  0xB7, 0xCE, 0xC2, 0xB6, 0xC8,
                                  0xB9, 0xFD, 0xB5, 0xCD, 0x00};

  const uint8_t *p_mesage[] = {
      MESSAGE_charu,       MESSAGE_bachu,       MESSAGE_wuka,
      MESSAGE_lianji,      MESSAGE_tuoji,       MESSAGE_zanting,
      MESSAGE_tingzhi,     MESSAGE_wancheng,    MESSAGE_hotend_heating,
      MESSAGE_hotend_over, MESSAGE_bed_heating, MESSAGE_bed_over,
      MESSAGE_ready,       MESSAGE_cold};

  void nope() {}
  DgusTFT::p_fun fun_array[] = {
      DgusTFT::page121_handle, DgusTFT::page122_handle,
      DgusTFT::page123_handle, DgusTFT::page124_handle,
      DgusTFT::page125_handle, DgusTFT::page6_handle,
      DgusTFT::page127_handle, DgusTFT::page128_handle,
      DgusTFT::page129_handle, DgusTFT::page130_handle,
      DgusTFT::page131_handle, nope,
      DgusTFT::page133_handle, nope,
      DgusTFT::page135_handle, DgusTFT::page136_handle,
      DgusTFT::page137_handle, DgusTFT::page138_handle,
      DgusTFT::page139_handle, DgusTFT::page140_handle,
      DgusTFT::page141_handle, DgusTFT::page142_handle,
      DgusTFT::page143_handle, DgusTFT::page144_handle,
      DgusTFT::page145_handle, DgusTFT::page146_handle,
      DgusTFT::page147_handle, DgusTFT::page148_handle,
      DgusTFT::page149_handle, DgusTFT::page150_handle,
      DgusTFT::page151_handle, DgusTFT::page152_handle,
      DgusTFT::page153_handle, nope};

  printer_state_t DgusTFT::printer_state;
  paused_state_t DgusTFT::pause_state;
  heater_state_t DgusTFT::hotend_state;
  heater_state_t DgusTFT::hotbed_state;
  xy_uint8_t DgusTFT::selectedmeshpoint;
  char DgusTFT::selectedfile[MAX_PATH_LEN];
  char DgusTFT::panel_command[MAX_CMND_LEN];
  uint8_t DgusTFT::command_len;
  float DgusTFT::live_Zoffset;
  file_menu_t DgusTFT::file_menu;

  bool DgusTFT::data_received;
  uint8_t DgusTFT::data_buf[DATA_BUF_SIZE];
  uint8_t DgusTFT::data_index;
  uint32_t DgusTFT::page_index_last;
  uint32_t DgusTFT::page_index_last_2;
  uint32_t DgusTFT::page_index_now;
  uint8_t DgusTFT::message_index;
  uint8_t DgusTFT::pop_up_index;
  uint32_t DgusTFT::key_index;
  uint32_t DgusTFT::key_value;
  uint16_t DgusTFT::filenumber;
  uint16_t DgusTFT::filepage;
  uint8_t DgusTFT::lcd_txtbox_index;
  uint8_t DgusTFT::lcd_txtbox_page;
  uint16_t DgusTFT::change_color_index;
  uint8_t DgusTFT::TFTpausingFlag;
  uint8_t DgusTFT::TFTStatusFlag;
  uint8_t DgusTFT::TFTresumingflag;
  uint8_t DgusTFT::ready;
  int16_t DgusTFT::home_owner_page;
  bool DgusTFT::is_auto_leveling;
  lcd_info_t DgusTFT::lcd_info;
  lcd_info_t DgusTFT::lcd_info_back;  // back for changing on lcd, to save flash
  // lifecycle

  DgusTFT Dgus;
  uint8_t page_index_saved;

  bool LowerZOffset() {
    auto z_off = getZOffset_mm();
    char str_buf[16] = {0};

    if (z_off <= -5) {
      return false;
    }

    auto steps = mmToWholeSteps(-BABYSTEP_MULTIPLICATOR_Z, Z);
    babystepAxis_steps(steps, Z);

    z_off -= BABYSTEP_MULTIPLICATOR_Z;
    setZOffset_mm(z_off);

    strcat(str_buf, dgus_ftostr(getZOffset_mm()));
    Dgus.SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);

    return true;
  }

  bool RaiseZOffset() {
    auto z_off = getZOffset_mm();
    char str_buf[16] = {0};

    if (z_off >= 5) {
      return false;
    }

    auto steps = mmToWholeSteps(BABYSTEP_MULTIPLICATOR_Z, Z);
    babystepAxis_steps(steps, Z);

    z_off += BABYSTEP_MULTIPLICATOR_Z;
    setZOffset_mm(z_off);

    strcat(str_buf, dgus_ftostr(getZOffset_mm()));
    Dgus.SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);

    return true;
  }

  uint8_t FSHlength(FSTR_P FSHinput) {
    PGM_P FSHinputPointer = reinterpret_cast<PGM_P>(FSHinput);
    uint8_t stringLength = 0;
    while (pgm_read_byte(FSHinputPointer++)) stringLength++;
    return stringLength;
  }

  DgusTFT::DgusTFT() {
    data_buf[0] = '\0';
    message_index = 100;
    pop_up_index = POPUP_NONE;
    page_index_now = 1;
    page_index_last = 1;
    page_index_last_2 = 1;

    lcd_txtbox_index = 0;
  }

  void DgusTFT::Startup() {
    selectedfile[0] = '\0';
    panel_command[0] = '\0';
    command_len = 0;
    printer_state = AC_printer_idle;
    pause_state = AC_paused_idle;
    hotend_state = AC_heater_off;
    hotbed_state = AC_heater_off;
    live_Zoffset = 0.0;
    file_menu = AC_menu_file;

    TERN_(HAS_HOTEND, hotend_state = AC_heater_off);
    TERN_(HAS_HEATED_BED, hotbed_state = AC_heater_off);

    TFTSer.begin(115200);

    SendtoTFTLN(AC_msg_main_board_has_reset);

    injectCommands_P(AC_cmnd_enable_levelling);

  #if ACDEBUGLEVEL
    SERIAL_ECHOLNPGM("AC Debug Level ", ACDEBUGLEVEL);
  #endif
    SendtoTFTLN(AC_msg_ready);
  }

  void DgusTFT::IdleLoop() {
  #if ACDEBUG(AC_MARLIN)
    static uint16_t pageNotFound = 0;
  #endif
    if (ReadTFTCommand()) {
      ProcessPanelRequest();
      command_len = 0;
    }

  #if ACDEBUG(AC_MARLIN)
    if (key_value) {
      SERIAL_ECHOLNPGM("page: ", page_index_now);
      SERIAL_ECHOLNPGM("key: ", key_value);
    }
  #endif

    static uint32_t milli_last = 0;
    if (millis() - milli_last > 1500) {
      milli_last = millis();

      char str_buf[10];
      sprintf(str_buf, "%lu/%lu", (uint32_t)getActualTemp_celsius(E0),
              (uint32_t)getTargetTemp_celsius(E0));
      SendTxtToTFT(str_buf, TXT_MAIN_HOTEND);

      sprintf(str_buf, "%lu/%lu", (uint32_t)getActualTemp_celsius(BED),
              (uint32_t)getTargetTemp_celsius(BED));
      SendTxtToTFT(str_buf, TXT_MAIN_BED);
    }
    switch (page_index_now) {
      case 157 ... 159:
      case 161 ... 169:
        break;
      case 170:
        page170_handle();
        break;
      case 174:
        break;
      case 175:
      case 176:
        page175_176_handle();
        break;
      case 189 ... 196:
        break;
      case 200:
        page199_to_200_handle();
        break;
      case 204:
        page204_handle();
        break;
      case 205:
        page205_handle();
        break;
      case 206:
        page206_handle();
        break;
      case 208:
        break;
      case 209:
        page207_209_handle();
        break;
      case 210:
        break;
      case 212:
        page211_212_handle();
        break;
      case 213:
        page213_handle();
        break;
      case 214:
        break;
      case 120 ... 156:
        fun_array[page_index_now - 1 - 120]();
        break;
      default:
  #if ACDEBUG(AC_MARLIN)
        if (pageNotFound != page_index_now) {
          SERIAL_ECHOLN("LCD Function does not exist");
          SERIAL_ECHOLNPGM("page_index_now: ", page_index_now);
          SERIAL_ECHOLNPGM("page_index_last: ", page_index_last);
          SERIAL_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
          pageNotFound = page_index_now;
        }
  #endif
        break;
    }
    pop_up_manager();
    key_value = 0;

    CheckHeaters();
  }

  void DgusTFT::PrinterKilled(FSTR_P const error_p, FSTR_P const component_p) {
    uint8_t textLength = FSHlength(error_p);
    char err[FSHlength(error_p) + 1];
    memcpy_P(err, error_p, textLength + 1);  // +1 for the null terminator

    textLength = FSHlength(component_p);
    char component[FSHlength(component_p) + 1];
    memcpy_P(component, component_p,
             textLength + 1);  // +1 for the null terminator

  //    SendtoTFTLN(AC_msg_kill_lcd);
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("PrinterKilled()\nerror: ", err,
                     "\ncomponent: ", component);
  #endif

    if (strncmp(err, "Heating Failed", strlen("Heating Failed")) == 0) {
      if (strncmp(component, "Bed", strlen("Bed")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_BED_HEATER);
        SERIAL_ECHOLNPGM("Check Bed heater");
      } else if (strncmp(component, "E1", strlen("E1")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_HOTEND_HEATER);
        SERIAL_ECHOLNPGM("Check E1 heater");
      }

    } else if (strncmp(err, "Err: MINTEMP", strlen("Err: MINTEMP")) == 0) {
      if (strncmp(component, "Bed", strlen("Bed")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_BED_NTC);
        SERIAL_ECHOLNPGM("Check Bed thermistor");
      } else if (strncmp(component, "E1", strlen("E1")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_HOTEND_NTC);
        SERIAL_ECHOLNPGM("Check E1 thermistor");
      }
    } else if (strncmp(err, "Err: MAXTEMP", strlen("Err: MAXTEMP")) == 0) {
      if (strncmp(component, "Bed", strlen("Bed")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_BED_NTC);
        SERIAL_ECHOLNPGM("Check Bed thermistor");
      } else if (strncmp(component, "E1", strlen("E1")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_HOTEND_NTC);
        SERIAL_ECHOLNPGM("Check E1 thermistor");
      }
    } else if (strncmp(err, "THERMAL RUNAWAY", strlen("THERMAL RUNAWAY")) ==
               0) {
      if (strncmp(component, "Bed", strlen("Bed")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_BED_HEATER);
        SERIAL_ECHOLNPGM("Check Bed thermal runaway");
      } else if (strncmp(component, "E1", strlen("E1")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_HOTEND_HEATER);
        SERIAL_ECHOLNPGM("Check E1 thermal runaway");
      }

    } else if (strncmp(err, "Homing Failed", strlen("Homing Failed")) == 0) {
      if (strncmp(component, "X", strlen("X")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_X_ENDSTOP);
        SERIAL_ECHOLNPGM("Check X endstop");
      } else if (strncmp(component, "Y", strlen("Y")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_Y_ENDSTOP);
        SERIAL_ECHOLNPGM("Check Y endstop");
      } else if (strncmp(component, "Z", strlen("Z")) == 0) {
        ChangePageOfTFT(PAGE_ABNORMAL_Z_ENDSTOP);
        SERIAL_ECHOLNPGM("Check Z endstop");
      }
    }
  }
  void DgusTFT::SetDescriptColor(const uint16_t color, const uint8_t index) {
    SendColorToTFT(color, TXT_DESCRIPT_0 + 0x30 * (index - 1));
  }

  void DgusTFT::MediaEvent(media_event_t event) {
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("ProcessMediaStatus() ", event);
  #endif
    switch (event) {
      case AC_media_inserted:

        filenavigator.reset();
        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          SetDescriptColor(COLOR_WHITE);
          lcd_txtbox_index = 0;
        }
        SendFileList(lcd_txtbox_index);
        break;

      case AC_media_removed:
        // SendtoTFTLN(AC_msg_sd_card_removed);

        filenavigator.reset();
        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          SetDescriptColor(COLOR_WHITE);
          lcd_txtbox_index = 0;
        }

        SendFileList(lcd_txtbox_index);
        break;

      case AC_media_error:
        //        SendtoTFTLN(AC_msg_no_sd_card);
        break;
    }
  }

  void DgusTFT::TimerEvent(timer_event_t event) {
    char str_buf[20];

  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("TimerEvent() ", event);
    SERIAL_ECHOLNPGM("Printer State: ", printer_state);
  #endif

    switch (event) {
      case AC_timer_started: {
        live_Zoffset = 0.0;          // reset print offset
        setSoftEndstopState(false);  // disable endstops to print
        printer_state = AC_printer_printing;
      } break;

      case AC_timer_paused: {
      } break;

      case AC_timer_stopped: {
        if (printer_state != AC_printer_idle) {
          if (printer_state == AC_printer_stopping_from_media_remove) {
            ChangePageOfTFT(PAGE_NO_SD);
          } else {
            printer_state = AC_printer_stopping;

            // Get Printing Time
            uint32_t time = getProgress_seconds_elapsed() / 60;
            sprintf(str_buf, "%s H ", utostr3(time / 60));
            sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
            SendTxtToTFT(str_buf, TXT_FINISH_TIME);
            ChangePageOfTFT(PAGE_PRINT_FINISH);
          }
        }
        setSoftEndstopState(true);  // enable endstops
      } break;
    }
  }

  void DgusTFT::FilamentRunout() {
  #if HAS_FILAMENT_SENSOR
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("FilamentRunout() printer_state ", printer_state);
    SERIAL_ECHOLNPGM("getFilamentRunoutState: ", getFilamentRunoutState());
  #endif

    pop_up_index = POPUP_FILAMENT_LACK;  // show filament lack.

    if (getFilamentRunoutState()) {
      PlayTune(BEEPER_PIN, FilamentOut, 1);

      if (isPrintingFromMedia()) {
        pausePrint();
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, BED);
        printer_state = AC_printer_pausing;
        pause_state = AC_paused_filament_lack;
      }
    }
  #endif // HAS_FILAMENT_SENSOR
  }

  void DgusTFT::ConfirmationRequest(const char *const msg) {
  // M108 continue
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("ConfirmationRequest() ", msg);
    SERIAL_ECHOLNPGM("printer_state:", printer_state);
    SERIAL_ECHOLNPGM("pause_state:", pause_state);
  #endif

    switch (printer_state) {
      case AC_printer_pausing: {
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0 ||
            strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
          if (pause_state != AC_paused_filament_lack) {
            ChangePageOfTFT(PAGE_STATUS1);  // enable continue button
          }
          printer_state = AC_printer_paused;
        }
      } break;

      case AC_printer_resuming_from_power_outage:
      case AC_printer_printing:
      case AC_printer_paused: {
        // Heater timout, send acknowledgement
        if (strcmp_P(msg, MARLIN_msg_heater_timeout) == 0) {
          pause_state = AC_paused_heater_timed_out;
          //          SendtoTFTLN(AC_msg_paused); // enable continue button
          PlayTune(BEEPER_PIN, Heater_Timedout, 1);
        }
        // Reheat finished, send acknowledgement
        else if (strcmp_P(msg, MARLIN_msg_reheat_done) == 0) {
  #if ACDEBUG(AC_MARLIN)
          SERIAL_ECHOLNPGM("send M108 ", __LINE__);
  #endif
          injectCommands_P(PSTR("M108"));

          if (pause_state != AC_paused_filament_lack) {
            pause_state = AC_paused_idle;
          }

          //          SendtoTFTLN(AC_msg_paused); // enable continue button
        }
        // Filament Purging, send acknowledgement enter run mode
        else if (strcmp_P(msg, MARLIN_msg_filament_purging) == 0) {
          pause_state = AC_paused_purging_filament;
          //          SendtoTFTLN(AC_msg_paused); // enable continue button
        } else if (strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
  #if ACDEBUG(AC_MARLIN)
          SERIAL_ECHOLNPGM("send M108 ", __LINE__);
  #endif
          injectCommands_P(PSTR("M108"));

          if (pause_state != AC_paused_filament_lack) {
            pause_state = AC_paused_idle;
          }
        }
      } break;

      default:
        break;
    }
  }

  void DgusTFT::StatusChange(const char *const msg) {
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("StatusChange() ", msg);
    SERIAL_ECHOLNPGM("printer_state:", printer_state);
    SERIAL_ECHOLNPGM("pause_state:", pause_state);
  #endif
    bool msg_matched = false;
    // The only way to get printer status is to parse messages
    // Use the state to minimise the work we do here.
    switch (printer_state) {
      case AC_printer_probing: {
        // If probing fails dont save the mesh raise the probe above the bad
        // point
        if (strcmp_P(msg, MARLIN_msg_probing_failed) == 0) {
          PlayTune(BEEPER_PIN, BeepBeepBeeep, 1);
          injectCommands_P(PSTR("G1 Z50 F500"));
          ChangePageOfTFT(PAGE_ABNORMAL_LEVELING_SENSOR);
          printer_state = AC_printer_idle;
        }
        msg_matched = true;
      }
      case AC_printer_printing: {
        if (strcmp_P(msg, MARLIN_msg_reheating) == 0) {
          //          SendtoTFTLN(AC_msg_paused); // enable continue button
          ChangePageOfTFT(PAGE_STATUS2);
          msg_matched = true;
        } else if (strcmp_P(msg, MARLIN_msg_media_removed) == 0) {
          msg_matched = true;
          printer_state = AC_printer_stopping_from_media_remove;
        }
      } break;

      case AC_printer_pausing: {
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {
          if (pause_state != AC_paused_filament_lack) {
            ChangePageOfTFT(PAGE_STATUS1);  // show resume
            if (lcd_txtbox_index) {
              SetDescriptColor(COLOR_WHITE);
              lcd_txtbox_index = 0;
            }
            pause_state = AC_paused_idle;
          }

          printer_state = AC_printer_paused;

          msg_matched = true;
        }
      } break;

      case AC_printer_paused: {
        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {
          if (pause_state != AC_paused_filament_lack) {
            ChangePageOfTFT(PAGE_STATUS1);  // show resume
            pause_state = AC_paused_idle;
          }

          printer_state = AC_printer_paused;

          msg_matched = true;
        }
      } break;

      case AC_printer_stopping: {
        if (strcmp_P(msg, MARLIN_msg_print_aborted) == 0) {
          ChangePageOfTFT(PAGE_MAIN);
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
        //        SendtoTFTLN(AC_msg_nozzle_heating);
        hotend_state = AC_heater_temp_set;
      } else if (strcmp_P(msg, MARLIN_msg_bed_heating) == 0) {
        //        SendtoTFTLN(AC_msg_bed_heating);
        hotbed_state = AC_heater_temp_set;
      }
    }
  }

  void DgusTFT::PowerLoss() {
    // On:  5A A5 05 82 00 82 00 00
    // Off: 5A A5 05 82 00 82 00 64

    uint8_t data_buf[8] = {0x5A, 0xA5, 0x05, 0x82, 0x00, 0x82, 0x00, 0x00};

    for (uint8_t i = 0; i < 8; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  void DgusTFT::HomingStart() {
    if (!isPrintingFromMedia()) {
      ChangePageOfTFT(PAGE_HOMING);
    }
  }

  void DgusTFT::HomingComplete() {
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("HomingComplete, line: ", __LINE__);
    SERIAL_ECHOLNPGM("page_index_last: ", page_index_last);
  #endif

    if (!isPrintingFromMedia()) {
      if (home_owner_page > 120) {
        ChangePageOfTFT(home_owner_page);
        home_owner_page = -1;
      } else {
        ChangePageOfTFT(page_index_last);
      }
    }
  }

  void DgusTFT::LevelingStart() { is_auto_leveling = true; }

  void DgusTFT::LevelingDone() {
    injectCommands_P(PSTR("M500"));
    printer_state = AC_printer_idle;
    is_auto_leveling = false;
    pop_up_index = POPUP_LEVELING_DONE;
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    setTargetTemp_celsius(0, E0);
    setTargetTemp_celsius(0, BED);
  #endif
  }

  void DgusTFT::MeshUpdate(const int8_t xpos, const int8_t ypos,
                           const ExtUI::probe_state_t state) {
    if (state == ExtUI::G29_POINT_START) {
    }
  }

  // A helper to print PROGMEN string to the panel
  void DgusTFT::SendtoTFT(PGM_P str) {
  #if ACDEBUG(AC_SOME)
    SERIAL_ECHOLNPGM("Sending to TFT: ", str);
  #endif
    while (const uint8_t c = pgm_read_byte(str++)) TFTSer.print(c);
  }

  void DgusTFT::SendValueToTFT(uint32_t value, uint32_t address) {
    uint8_t data_buf[32] = {0};
    uint8_t data_index = 0;

    uint8_t *p_u8 = (uint8_t *)(&address) + 1;

    data_buf[data_index++] = 0x5A;
    data_buf[data_index++] = 0xA5;
    data_buf[data_index++] = 0x05;
    data_buf[data_index++] = 0x82;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;
    p_u8 = (uint8_t *)(&value) + 1;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;

    for (uint8_t i = 0; i < data_index; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  void DgusTFT::RequestValueFromTFT(uint32_t address) {
    uint8_t data_buf[20] = {0};
    uint8_t data_index = 0;

    uint8_t *p_u8 = (uint8_t *)(&address) + 1;

    data_buf[data_index++] = 0x5A;
    data_buf[data_index++] = 0xA5;
    data_buf[data_index++] = 0x04;
    data_buf[data_index++] = 0x83;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;
    data_buf[data_index++] = 0x01;

    for (uint8_t i = 0; i < data_index; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  void DgusTFT::SendTxtToTFT(const char *pdata, uint32_t address) {
    char data_buf[128] = {0};
    uint8_t data_index = 0;
    uint8_t data_len = 0;

    uint8_t *p_u8 = (uint8_t *)(&address) + 1;
    data_len = strlen(pdata);

    data_buf[data_index++] = 0x5A;
    data_buf[data_index++] = 0xA5;
    data_buf[data_index++] = data_len + 5;
    data_buf[data_index++] = 0x82;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;

    memcpy(&data_buf[data_index], pdata, data_len);
    data_index += data_len;

    data_buf[data_index++] = 0xFF;
    data_buf[data_index++] = 0xFF;

    for (uint8_t i = 0; i < data_index; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  void DgusTFT::SendColorToTFT(uint32_t color, uint32_t address) {
    uint8_t data_buf[32] = {0};
    uint8_t data_index = 0;

    uint8_t *p_u8 = (uint8_t *)(&address) + 1;
    address += 3;

    data_buf[data_index++] = 0x5A;
    data_buf[data_index++] = 0xA5;
    data_buf[data_index++] = 0x05;
    data_buf[data_index++] = 0x82;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;
    p_u8 = (uint8_t *)(&color) + 1;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;

    for (uint8_t i = 0; i < data_index; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  void DgusTFT::SendReadNumOfTxtToTFT(uint8_t number, uint32_t address) {
    uint8_t data_buf[32] = {0};
    uint8_t data_index = 0;

    uint8_t *p_u8 = (uint8_t *)(&address) + 1;

    data_buf[data_index++] = 0x5A;
    data_buf[data_index++] = 0xA5;
    data_buf[data_index++] = 0x04;  // frame length
    data_buf[data_index++] = 0x83;
    data_buf[data_index++] = *p_u8;
    p_u8--;
    data_buf[data_index++] = *p_u8;
    data_buf[data_index++] = number;  // how much bytes to read

    for (uint8_t i = 0; i < data_index; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  void DgusTFT::ChangePageOfTFT(uint32_t page_index, bool no_send /*false*/) {
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("ChangePageOfTFT: ", page_index);
  #endif

    uint32_t data_temp = 0;
    data_temp = page_index;

    if (!no_send) {
      uint8_t data[] = {0x5A,
                        0xA5,
                        0x07,
                        0x82,
                        0x00,
                        0x84,
                        0x5A,
                        0x01,
                        uint8_t(data_temp >> 8),
                        uint8_t(data_temp & 0xFF)};
      for (uint8_t i = 0; i < COUNT(data); ++i) TFTSer.write(data[i]);
    }

    page_index_last_2 = page_index_last;
    page_index_last = page_index_now;
    page_index_now = data_temp;

  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
    SERIAL_ECHOLNPGM("page_index_last: ", page_index_last);
    SERIAL_ECHOLNPGM("page_index_now: ", page_index_now);
  #endif
  }

  void DgusTFT::FakeChangePageOfTFT(uint32_t page_index) {
  #if ACDEBUG(AC_MARLIN)
    DEBUG_ECHOLNPGM("FakeChangePageOfTFT: ", page_index);
  #endif

    ChangePageOfTFT(page_index, true);
  }

  void DgusTFT::LcdAudioSet(ExtUI::audio_t audio) {
    // On:  5A A5 07 82 00 80 5A 00 00 1A
    // Off: 5A A5 07 82 00 80 5A 00 00 12
    uint8_t data_buf[20] = {0x5A, 0xA5, 0x07, 0x82, 0x00,
                            0x80, 0x5A, 0x00, 0x00};

    if (audio == ExtUI::ON) {
      data_buf[9] = 0x1A;
    } else if (audio == ExtUI::OFF) {
      data_buf[9] = 0x12;
    }

    for (uint8_t i = 0; i < 10; i++) {
      TFTSer.write(data_buf[i]);
    }
  }

  bool DgusTFT::ReadTFTCommand() {
    static uint32_t tft_last_check = 0;

    static uint8_t length = 0, cnt = 0, tft_receive_steps = 0;
    uint8_t data;

    if (0 == TFTSer.available() || data_received) {
      return false;
    }

    data = TFTSer.read();

    if (tft_receive_steps == 0) {
      if (data != 0x5A) {
        cnt = 0;
        length = 0;
        data_index = 0;
        data_received = false;
        return false;
      }

      tft_last_check = millis();
      while (!TFTSer.available()) {
        TERN_(USE_WATCHDOG, hal.watchdog_refresh());
        if (millis() - tft_last_check > 500) {
          data_index = 0;
          data_received = 0;
          printf("lcd ReadTFTCommand error\n");
          return false;
        }
      }

      data = TFTSer.read();
      // MYSERIAL.write(data );
      if (data == 0xa5) {
        tft_receive_steps = 2;
      }
    } else if (tft_receive_steps == 2) {
      length = data;
      tft_receive_steps = 3;
      data_index = 0;
      cnt = 0;
    } else if (tft_receive_steps == 3) {
      if (data_index >= DATA_BUF_SIZE - 1) {
  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLNPGM("lcd uart buff overflow: ", data_index);
  #endif
        data_index = 0;
        data_received = 0;
        return false;
      }
      data_buf[data_index++] = data;
      cnt++;
      if (cnt >= length) {  // Receive complete
        tft_receive_steps = 0;
        cnt = 0;
        data_index = 0;
        data_received = 1;
        return true;
      }
    }

    return false;
  }

  int8_t DgusTFT::Findcmndpos(const uint8_t *buff, uint8_t q) {
    int8_t pos = 0;
    do {
      if (buff[pos] == q) return pos;
    } while (++pos < MAX_CMND_LEN);
    return -1;
  }

  void DgusTFT::CheckHeaters() {
    static uint8_t faultE0Duration = 0, faultBedDuration = 0;
    float temp = 0;
    static uint32_t time_last = 0;

    if (millis() < time_last + 500) {
      return;
    }

    time_last = millis();

    // if the hotend temp is abnormal, confirm state before signalling panel
    temp = getActualTemp_celsius(E0);
    if (!WITHIN(temp, HEATER_0_MINTEMP, HEATER_0_MAXTEMP)) {
      faultE0Duration++;
      if (faultE0Duration >= AC_HEATER_FAULT_VALIDATION_TIME) {
        //          SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLNPGM("Extruder temp abnormal! : ", temp);
  #endif
        faultE0Duration = 0;
      }
    }

    temp = getActualTemp_celsius(BED);
    if (!WITHIN(temp, BED_MINTEMP, BED_MAXTEMP)) {
      faultBedDuration++;
      if (faultBedDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
        //            SendtoTFTLN(AC_msg_bed_temp_abnormal);
  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLNPGM("Bed temp abnormal! : ", temp);
  #endif
        faultBedDuration = 0;
      }
    }

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
    SERIAL_ECHOLNPGM("## SendFileList ## ", startindex);
  #endif
    filenavigator.getFiles(startindex);
  }

  void DgusTFT::SelectFile() {
    strncpy(selectedfile, panel_command + 4, command_len - 4);
    selectedfile[command_len - 5] = '\0';
  #if ACDEBUG(AC_FILE)
    SERIAL_ECHOLN(" Selected File: ", selectedfile);
  #endif
    switch (selectedfile[0]) {
      case '/':  // Valid file selected
        // SendtoTFTLN(AC_msg_sd_file_open_success);
        break;

      case '<':  // .. (go up folder level)
        filenavigator.upDIR();
        // SendtoTFTLN(AC_msg_sd_file_open_failed);
        SendFileList(0);
        break;
      default:  // enter sub folder
        filenavigator.changeDIR(selectedfile);
        // SendtoTFTLN(AC_msg_sd_file_open_failed);
        SendFileList(0);
        break;
    }
  }

  void DgusTFT::ProcessPanelRequest() {
    uint8_t *p_u8;
    unsigned int control_index = 0;
    unsigned int control_value;
    unsigned int temp;
    char str_buf[20];

    if (data_received) {
      data_received = false;

      if (0x83 == data_buf[0]) {
        p_u8 = (uint8_t *)(&control_index);  // get control address
        *p_u8 = data_buf[2];
        p_u8++;
        *p_u8 = data_buf[1];

        if ((control_index & 0xF000) == KEY_ADDRESS)  // is KEY
        {
          key_index = control_index;
          p_u8 = (uint8_t *)(&key_value);  // get key value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];
        } else if (control_index == TXT_HOTEND_TARGET ||
                   control_index == TXT_ADJUST_HOTEND)  // hotend target temp
        {
          p_u8 = (uint8_t *)(&control_value);  // get  value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];

          temp = constrain((uint16_t)control_value, 0, HEATER_0_MAXTEMP);
          setTargetTemp_celsius(temp, E0);
        } else if (control_index == TXT_BED_TARGET ||
                   control_index == TXT_ADJUST_BED)  // bed target temp
        {
          p_u8 = (uint8_t *)(&control_value);  // get value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];

          temp = constrain((uint16_t)control_value, 0, BED_MAXTEMP);
          setTargetTemp_celsius(temp, BED);
        } else if (control_index == TXT_FAN_SPEED_TARGET)  // fan speed
        {
          p_u8 = (uint8_t *)(&control_value);  // get value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];

          temp = constrain((uint16_t)control_value, 0, 100);
          SendValueToTFT((uint16_t)temp, TXT_FAN_SPEED_NOW);
          SendValueToTFT((uint16_t)temp, TXT_FAN_SPEED_TARGET);
          setTargetFan_percent(temp, FAN0);
        } else if (control_index == TXT_PRINT_SPEED_TARGET ||
                   control_index == TXT_ADJUST_SPEED)  // prinf speed
        {
          p_u8 = (uint8_t *)(&control_value);  // get  value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];

          float feedrate = constrain((uint16_t)control_value, 40, 999);
          //         feedrate_percentage=constrain(control_value,40,999);
          sprintf(str_buf, "%u", (uint16_t)feedrate);
          SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
          SendValueToTFT((uint16_t)feedrate, TXT_PRINT_SPEED_NOW);
          SendValueToTFT((uint16_t)feedrate, TXT_PRINT_SPEED_TARGET);
          setFeedrate_percent(feedrate);
        } else if (control_index == TXT_PREHEAT_HOTEND_INPUT) {
          p_u8 = (uint8_t *)(&control_value);  // get  value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];

          temp = constrain((uint16_t)control_value, 0, HEATER_0_MAXTEMP);
          setTargetTemp_celsius(temp, E0);
        } else if (control_index == TXT_PREHEAT_BED_INPUT) {
          p_u8 = (uint8_t *)(&control_value);  // get  value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];

          temp = constrain((uint16_t)control_value, 0, BED_MAXTEMP);
          setTargetTemp_celsius(temp, BED);
        } else if (control_index == REG_LCD_READY) {
          p_u8 = (uint8_t *)(&control_value);  // get  value
          *p_u8 = data_buf[5];
          p_u8++;
          *p_u8 = data_buf[4];
          p_u8++;
          *p_u8 = data_buf[3];
          if ((control_value & 0x00FFFFFF) == 0x010072) {  // startup last gif

            LcdAudioSet(lcd_info.audio);

            SendValueToTFT(2, ADDRESS_MOVE_DISTANCE);

  #if ENABLED(CASE_LIGHT_ENABLE)
            SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
            SendValueToTFT(getCaseLightState(),
                           ADDRESS_PRINT_SETTING_LED_STATUS);
  #endif
            ChangePageOfTFT(PAGE_MAIN);

          } else if ((control_value & 0x00FFFFFF) ==
                     0x010000) {                        // startup first gif
            PlayTune(BEEPER_PIN, Anycubic_PowerOn, 1);  // take 3500 ms
          }
        }
      } else if (data_buf[0] == 0x82) {
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
  else if ( req <= 28) PanelAction(req);

  // Process Initiation
  else if (req <= 34) PanelProcess(req);

  else SendtoTFTLN();
}
  #endif

  // MainPage
  void DgusTFT::page121_handle(void) {
    static millis_t flash_time = 0;
    //  char str_buf[20];

    switch (key_value) {
      case 0:
        break;

      case 1:  // Print
        lcd_txtbox_page = 0;

        if (lcd_txtbox_index) {
          SetDescriptColor(COLOR_WHITE);
          lcd_txtbox_index = 0;
        }
        ChangePageOfTFT(PAGE_FILE);
        SendFileList(0);
        break;

      case 2:  // Tool
  #if ENABLED(CASE_LIGHT_ENABLE)
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
        SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif
        break;

      case 3:  // Prepare
        ChangePageOfTFT(PAGE_PREPARE);
        break;

      case 4:  // System Settings
        if (lcd_info.audio == ExtUI::ON) {
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_ON);
        } else if (lcd_info.audio == ExtUI::OFF) {
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_OFF);
        }
        break;
    }

  #if 0
    if (message_index < 30) {
      SendTxtToTFT(p_mesage[message_index], TXT_MAIN_MESSAGE);
      message_index = 30;
    }
  #endif

    if (millis() < (flash_time + 1500)) {
      return;
    }
    flash_time = millis();
  }

  // File Listing for Printing
  void DgusTFT::page122_handle(void) {
    uint8_t file_index = 0;

    switch (key_value) {
      case 0:
        break;

      case 1:
        ChangePageOfTFT(PAGE_MAIN);
        SetDescriptColor(COLOR_WHITE);
        break;

      case 2:  // page up
      {
        if (lcd_txtbox_page > 0) {
          lcd_txtbox_page--;

          SetDescriptColor(COLOR_WHITE);
          lcd_txtbox_index = 0;

          SendFileList(lcd_txtbox_page * 5);
        }
        break;
      }

      case 3:  // page down
      {
        if ((lcd_txtbox_page + 1) * 5 < filenavigator.getFileNum()) {
          lcd_txtbox_page++;

          SetDescriptColor(COLOR_WHITE);
          lcd_txtbox_index = 0;

          SendFileList(lcd_txtbox_page * 5);
        }
        break;
      }

      case 4:  // page refresh
      {
        if (!isMediaMounted()) {
          safe_delay(500);
        }

        filenavigator.reset();

        lcd_txtbox_page = 0;
        if (lcd_txtbox_index) {
          SetDescriptColor(COLOR_WHITE);
          lcd_txtbox_index = 0;
        }

        SendFileList(lcd_txtbox_index);
      } break;

      case 5:  // Power Outage Recovery
        break;
      case 6:                                                // Start Print
        if (lcd_txtbox_index > 0 && lcd_txtbox_index < 6) {  // 1~5

          if (filenavigator.filelist.seek(lcd_txtbox_page * 5 +
                                          (lcd_txtbox_index - 1))) {
            SetDescriptColor(COLOR_WHITE);
            // Allows printer to restart the job if we dont want to recover
            if (printer_state == AC_printer_resuming_from_power_outage) {
              printer_state = AC_printer_idle;
            }

  #if ENABLED(CASE_LIGHT_ENABLE)
            setCaseLightState(1);
  #endif

            printFile(filenavigator.filelist.shortFilename());

            char str_buf[20];
            strncpy_P(str_buf, filenavigator.filelist.longFilename(), 17);
            str_buf[17] = '\0';
            SendTxtToTFT(str_buf, TXT_PRINT_NAME);

            sprintf(str_buf, "%d", (uint16_t)getFeedrate_percent());
            SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

            sprintf(str_buf, "%d", (uint16_t)getProgress_percent());
            SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

            uint32_t time = 0;
            sprintf(str_buf, "%s H ", utostr3(time / 60));
            sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
            SendTxtToTFT(str_buf, TXT_PRINT_TIME);

            ChangePageOfTFT(PAGE_STATUS2);
          }
        }
        break;

      case 7:   // txtbox 1 click
      case 8:   // txtbox 2 click
      case 9:   // txtbox 3 click
      case 10:  // txtbox 4 click
      case 11:  // txtbox 5 click

      {
        static uint8_t lcd_txtbox_index_last = 0;

        if ((lcd_txtbox_page * 5 + key_value - 6) <=
            filenavigator.getFileNum()) {
          lcd_txtbox_index = key_value - 6;
        } else {
          break;
        }

  #if ACDEBUG(AC_MARLIN)
        printf("getFileNum: %d\n", filenavigator.getFileNum());
        printf("file_index: %d\n", file_index);
        printf("lcd_txtbox_page: %d\n", lcd_txtbox_page);
        printf("lcd_txtbox_index: %d\n", lcd_txtbox_index);
        printf("lcd_txtbox_index_last: %d\n", lcd_txtbox_index_last);
  #endif

        // lcd_txtbox_page 0~...
        // lcd_txtbox_index 1~5
        file_index = lcd_txtbox_page * 5 + (lcd_txtbox_index - 1);
        if (file_index < filenavigator.getFileNum()) {
          SetDescriptColor(COLOR_RED);
          if (lcd_txtbox_index_last &&
              lcd_txtbox_index_last != lcd_txtbox_index) {  // 1~5
            SetDescriptColor(COLOR_WHITE);
          }
          lcd_txtbox_index_last = lcd_txtbox_index;
        }
        break;
      }
    }
  }

  void DgusTFT::Page123_124_Return() {
    if (!isPrintingFromMedia()) {
      ChangePageOfTFT(PAGE_FILE);
    }
  }

  void DgusTFT::Page123_124_Stop() {
  #if ACDEBUG(AC_MARLIN)
    SERIAL_ECHOLN("Stopping Print");
  #endif
    if (isPrintingFromMedia()) {
      ChangePageOfTFT(PAGE_STOP_CONF);
    }
  }

  void DgusTFT::Page123_124_Settings(uint16_t &feedrate_last) {
  #ifdef CASE_LIGHT_ENABLE
    ChangePageOfTFT(PAGE_PRINTING_SETTING);
    SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS);
  #else
    ChangePageOfTFT(PAGE_ADJUST);
  #endif

    SendValueToTFT((uint16_t)getTargetTemp_celsius(E0), TXT_ADJUST_HOTEND);
    SendValueToTFT((uint16_t)getTargetTemp_celsius(BED), TXT_ADJUST_BED);
    feedrate_last = (uint16_t)getFeedrate_percent();
    SendValueToTFT(feedrate_last, TXT_ADJUST_SPEED);
  }

  void DgusTFT::Page123_124_End(millis_t &flash_time, uint8_t &progress_last,
                                uint16_t &feedrate_last) {
    char str_buf[20];
    if (millis() < (flash_time + 1500)) {
      return;
    }
    flash_time = millis();

    if (feedrate_last != (uint16_t)getFeedrate_percent()) {
      feedrate_last = (uint16_t)getFeedrate_percent();
      sprintf(str_buf, "%d", feedrate_last);
      SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
    }

    if (progress_last != getProgress_percent()) {
      progress_last = getProgress_percent();
      sprintf(str_buf, "%u", progress_last);
      SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);
    }

    // Get Printing Time
    uint32_t time = getProgress_seconds_elapsed() / 60;
    sprintf(str_buf, "%s H ", utostr3(time / 60));
    sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
    SendTxtToTFT(str_buf, TXT_PRINT_TIME);
  }

  // Printing Page (Paused)
  void DgusTFT::page123_handle(void) {
    static millis_t flash_time = 0;
    static uint8_t progress_last = 0;
    static uint16_t feedrate_last = 0;

    switch (key_value) {
      case 0:

        break;

      case 1:  // Return
        Page123_124_Return();
        break;

      case 2:  // Resume Print
  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLN("Resuming Print");
        SERIAL_ECHOLNPGM("printer_state: ", printer_state);
        SERIAL_ECHOLNPGM("pause_state: ", pause_state);
  #endif
        if (pause_state == AC_paused_idle ||
            pause_state == AC_paused_filament_lack) {
          printer_state = AC_printer_printing;
          pause_state = AC_paused_idle;
          resumePrint();
  #if ACDEBUG(AC_MARLIN)
          SERIAL_ECHOLN("Resuming Print 2");
  #endif
          ChangePageOfTFT(PAGE_STATUS2);
          flash_time = millis();

        } else {
          setUserConfirmed();
        }
        break;

      case 3:  // Stop Print

        Page123_124_Stop();
        break;

      case 4:  // Print Settings
        Page123_124_Settings(feedrate_last);
        break;
    }

    Page123_124_End(flash_time, progress_last, feedrate_last);
  }

  // Printing Page (Printing)
  void DgusTFT::page124_handle(void) {
    static millis_t flash_time = 0;
    static uint8_t progress_last = 0;
    static uint16_t feedrate_last = 0;

    switch (key_value) {
      case 0:
        break;

      case 1:  // return
        Page123_124_Return();
        break;

      case 2:  // print pause
        if (isPrintingFromMedia()) {
          pausePrint();
          setTargetTemp_celsius(0, E0);
          setTargetTemp_celsius(0, BED);
          printer_state = AC_printer_pausing;
          pause_state = AC_paused_idle;
          ChangePageOfTFT(PAGE_WAIT_PAUSE);
        }
        break;

      case 3:  // print stop
        Page123_124_Stop();
        break;

      case 4:  // print settings
        Page123_124_Settings(feedrate_last);
        break;
    }
    Page123_124_End(flash_time, progress_last, feedrate_last);
  }

  void DgusTFT::page125_SaveAndBack(bool z_change) {
    RequestValueFromTFT(TXT_ADJUST_BED);
    RequestValueFromTFT(TXT_ADJUST_SPEED);
    RequestValueFromTFT(TXT_ADJUST_HOTEND);
    RequestValueFromTFT(TXT_FAN_SPEED_TARGET);

    if (z_change) {
      injectCommands_P(PSTR("M500"));
      z_change = false;
    }

    if (AC_printer_printing == printer_state) {
      ChangePageOfTFT(PAGE_STATUS2);  // Show Pause
    } else if (AC_printer_paused == printer_state) {
      ChangePageOfTFT(PAGE_STATUS1);  // Show Print
    }
  }

  // Inside Print Settings
  void DgusTFT::page125_handle(void) {
    static bool z_change = false;

    switch (key_value) {
      case 0:
        break;

      case 1:  // Back Button
        page125_SaveAndBack(z_change);
        break;

      case 2: {
        z_change = LowerZOffset();
      } break;

      case 3: {
        z_change = RaiseZOffset();
      } break;

      case 4:  // Light Control
  #if ENABLED(CASE_LIGHT_ENABLE)
        if (getCaseLightState()) {
          SendValueToTFT(0, ADDRESS_PRINT_SETTING_LED_STATUS);
          setCaseLightState(0);
        } else {
          SendValueToTFT(1, ADDRESS_PRINT_SETTING_LED_STATUS);
          setCaseLightState(1);
        }
  #endif
        break;

      case 5:
        ChangePageOfTFT(PAGE_DONE);
        break;

      case 6:

        break;

      case 7:
        page125_SaveAndBack(z_change);

        break;
    }
  }

  // Tools Page
  void DgusTFT::page127_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // Back Button
      {
        ChangePageOfTFT(PAGE_MAIN);
      } break;

      case 2: {
        ChangePageOfTFT(PAGE_MOVE);
      } break;

      case 3:  // Set Temperature
        ChangePageOfTFT(PAGE_TEMP);
        SendValueToTFT((uint16_t)getActualTemp_celsius(E0), TXT_HOTNED_NOW);
        SendValueToTFT((uint16_t)getTargetTemp_celsius(E0), TXT_HOTEND_TARGET);
        SendValueToTFT((uint16_t)getActualTemp_celsius(BED), TXT_BED_NOW);
        SendValueToTFT((uint16_t)getTargetTemp_celsius(BED), TXT_BED_TARGET);

        break;

      case 4:  // Change Speeds
        ChangePageOfTFT(PAGE_SPEED);
        SendValueToTFT((uint16_t)getActualFan_percent(FAN0), TXT_FAN_SPEED_NOW);
        SendValueToTFT((uint16_t)getTargetFan_percent(FAN0),
                       TXT_FAN_SPEED_TARGET);
        SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_NOW);
        SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_TARGET);
        break;

      case 5:  // Disable Steppers
        if (!isMoving()) {
          stepper.disable_all_steppers();
          motion.set_all_unhomed();
        }
        break;

      case 6:  // CaseLight Control
  #if ENABLED(CASE_LIGHT_ENABLE)
        if (getCaseLightState()) {
          setCaseLightState(0);
          SendValueToTFT(0, ADDRESS_SYSTEM_LED_STATUS);
        } else {
          setCaseLightState(1);
          SendValueToTFT(1, ADDRESS_SYSTEM_LED_STATUS);
        }
  #endif
        break;
    }
  }

  // Move Page
  void DgusTFT::page128_handle(void) {
    static float move_dis = 1.0f;

    if (key_value == 2 || key_value == 4 || key_value == 6 || key_value == 8 ||
        key_value == 10 || (key_value == 12 && !isMoving())) {
      if (getAxisPosition_mm(Z) < 0) {
        setAxisPosition_mm(0, Z, 8);
      }
    }

    switch (key_value) {
      case 0:
        break;

      case 1:  // Back Button

  #ifdef CASE_LIGHT_ENABLE
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif

        break;

      case 5:  // Home X
        if (!isMoving()) {
          injectCommands_P(PSTR("G28 X"));
        }
        break;

      case 9:  // Home Y
        if (!isMoving()) {
          injectCommands_P(PSTR("G28 Y"));
        }
        break;

      case 13:  // Home Z
        if (!isMoving()) {
          if (motion.axis_is_trusted(X_AXIS) && motion.axis_is_trusted(Y_AXIS)) {
            injectCommands_P(PSTR("G28 Z"));
          } else {
            injectCommands_P(PSTR("G28"));
          }
        }
        break;

      case 17:  // Home All
        if (!isMoving()) {
          injectCommands_P(PSTR("G28"));
        }
        break;

      case 2:  // X-
        if (!isMoving()) {
          setAxisPosition_mm(getAxisPosition_mm(X) - move_dis, X, 50);
        }
        break;

      case 4:  // X+
        if (!isMoving()) {
          setAxisPosition_mm(getAxisPosition_mm(X) + move_dis, X, 50);
        }
        break;

      case 6:  // Y+
        if (!isMoving()) {
          setAxisPosition_mm(getAxisPosition_mm(Y) + move_dis, Y, 50);
        }
        break;

      case 8:  // Y-
        if (!isMoving()) {
          setAxisPosition_mm(getAxisPosition_mm(Y) - move_dis, Y, 50);
        }
        break;

      case 10:  // Z-
        if (!isMoving()) {
          setAxisPosition_mm(getAxisPosition_mm(Z) - move_dis, Z, 8);
        }
        break;

      case 12:  // Z+
        if (!isMoving()) {
          setAxisPosition_mm(getAxisPosition_mm(Z) + move_dis, Z, 8);
        }
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
    }
  }

  // Temperature Page
  void DgusTFT::page129_handle(void) {
    static millis_t flash_time = 0;

    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
  #if ENABLED(CASE_LIGHT_ENABLE)
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
        SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif

      } break;
      case 2:
        break;
      case 3:

        break;
      case 4:
        break;

      case 5:
        break;

      case 6:  // cooling
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, BED);

  #if ENABLED(CASE_LIGHT_ENABLE)
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
        SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif

        break;

      case 7:  // send target temp
        RequestValueFromTFT(TXT_HOTEND_TARGET);
        RequestValueFromTFT(TXT_BED_TARGET);

  #if ENABLED(CASE_LIGHT_ENABLE)
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
        SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif

        break;
    }

    if (millis() < (flash_time + 1500)) {
      return;
    }
    flash_time = millis();

    SendValueToTFT((uint16_t)getActualTemp_celsius(E0), TXT_HOTNED_NOW);
    SendValueToTFT((uint16_t)getActualTemp_celsius(BED), TXT_BED_NOW);
  }

  // Speed Page
  void DgusTFT::page130_handle(void) {
    static millis_t flash_time = 0;
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
  #if ENABLED(CASE_LIGHT_ENABLE)
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
        SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif

      } break;

      case 2:
        break;

      case 3:
        break;

      case 4:
        break;

      case 5:
        break;

      case 6:  // ok
        RequestValueFromTFT(TXT_FAN_SPEED_TARGET);
        RequestValueFromTFT(TXT_PRINT_SPEED_TARGET);

  #if ENABLED(CASE_LIGHT_ENABLE)
        ChangePageOfTFT(PAGE_TOOL_CASELIGHT);
        SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
  #else
        ChangePageOfTFT(PAGE_TOOL);
  #endif

        break;
    }

    if (millis() < (flash_time + 1500)) {
      return;
    }
    flash_time = millis();

    SendValueToTFT((uint16_t)getActualFan_percent(FAN0), TXT_FAN_SPEED_NOW);
    SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_NOW);
  }

  // System Page
  void DgusTFT::page131_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // Back Button
      {
        ChangePageOfTFT(PAGE_MAIN);
        if (lcd_info_back.language != lcd_info.language ||
            lcd_info_back.audio != lcd_info.audio) {
          lcd_info_back.language = lcd_info.language;
          lcd_info_back.audio = lcd_info.audio;

          injectCommands_P(PSTR("M500"));
        }
      } break;

      case 2:  // Printer Statistics Page

        printerStatsToTFT();
        ChangePageOfTFT(PAGE_PRINTERSTATS);
        break;

      case 3:
        break;

      case 4:  // Audio ON/OFF
      {
        if (lcd_info.audio == ExtUI::ON) {
          lcd_info.audio = ExtUI::OFF;
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_OFF);

        } else if (lcd_info.audio == ExtUI::OFF) {
          lcd_info.audio = ExtUI::ON;
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_ON);
        }

        LcdAudioSet(lcd_info.audio);
      } break;

      case 5:  // About

        SendTxtToTFT(DEVICE_NAME, TXT_ABOUT_DEVICE_NAME);
        SendTxtToTFT(FIRMWARE_VER, TXT_ABOUT_FW_VERSION);
        SendTxtToTFT(BUILD_VOLUME, TXT_ABOUT_PRINT_VOLUMN);
        SendTxtToTFT(TECH_SUPPORT, TXT_ABOUT_TECH_SUPPORT);

        ChangePageOfTFT(PAGE_ABOUT);
        break;

      case 6:
        ChangePageOfTFT(PAGE_RECORD);
        break;
    }
  }

  // About Page
  void DgusTFT::page133_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
        if (lcd_info.audio == ExtUI::ON) {
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_ON);
        } else if (lcd_info.audio == ExtUI::OFF) {
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_OFF);
        }
      } break;

      case 2:
        break;
    }
  }

  // Prepare Page
  void DgusTFT::page135_handle(void) {
    char str_buf[16];

    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
        ChangePageOfTFT(PAGE_MAIN);

      } break;

      case 2: {
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);
      } break;

      case 3:
        ChangePageOfTFT(PAGE_PREHEAT);
        sprintf(str_buf, "%u/%u", (uint16_t)getActualTemp_celsius(E0),
                (uint16_t)getTargetTemp_celsius(E0));
        SendTxtToTFT(str_buf, TXT_PREHEAT_HOTEND);
        sprintf(str_buf, "%u/%u", (uint16_t)getActualTemp_celsius(BED),
                (uint16_t)getTargetTemp_celsius(BED));
        SendTxtToTFT(str_buf, TXT_PREHEAT_BED);
        break;

      case 4: {
        str_buf[0] = 0;
        sprintf(str_buf, "%u/%u", (uint16_t)getActualTemp_celsius(E0),
                (uint16_t)getTargetTemp_celsius(E0));
        SendTxtToTFT(str_buf, TXT_FILAMENT_TEMP);
        ChangePageOfTFT(PAGE_FILAMENT);
      } break;
    }
  }

  // AutoLeveling Page
  void DgusTFT::page136_handle(void) {
    switch (key_value) {
      case 0:
        break;
      case 1:  // return
      {
        ChangePageOfTFT(PAGE_PREPARE);
      } break;

      case 2:
        if (!isPrinting()) {
          ChangePageOfTFT(PAGE_LEVEL_ENSURE);
        }
        break;

      case 3:
        char str_buf[10];
        str_buf[0] = 0;
        strcat(str_buf, dgus_ftostr(getZOffset_mm()));
        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
        ChangePageOfTFT(PAGE_Z_OFFSET);
        break;

      case 4:
        break;
    }
  }

  // Z Offset Page
  void DgusTFT::page137_handle(void) {
    static bool z_change = false;

    switch (key_value) {
      case 0:
        break;

      case 1:  // return
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);
        break;

      case 2:
        z_change = LowerZOffset();
        break;

      case 3:
        z_change = RaiseZOffset();
        break;

      case 4:

  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLNPGM("z off: ", dgus_ftostr(getZOffset_mm()));
  #endif
        if (z_change) {
          z_change = false;
          injectCommands_P(PSTR("M500"));
        }

        ChangePageOfTFT(PAGE_PREPARE);
        break;
    }
  }

  // PreHeat Page
  void DgusTFT::page138_handle(void) {
    static millis_t flash_time = 0;
    char str_buf[16];

    switch (key_value) {
      case 0:
        break;

      case 1:  // return
        ChangePageOfTFT(PAGE_PREPARE);
        break;

      case 2:  // PLA
        setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, E0);
        setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
        ChangePageOfTFT(PAGE_PREHEAT);
        break;

      case 3:  // ABS
        setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, E0);
        setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
        ChangePageOfTFT(PAGE_PREHEAT);
        break;
    }

    if (millis() < (flash_time + 1500)) {
      return;
    }
    flash_time = millis();

    sprintf(str_buf, "%u/%u", (uint16_t)getActualTemp_celsius(E0),
            (uint16_t)getTargetTemp_celsius(E0));
    SendTxtToTFT(str_buf, TXT_PREHEAT_HOTEND);
    sprintf(str_buf, "%u/%u", (uint16_t)getActualTemp_celsius(BED),
            (uint16_t)getTargetTemp_celsius(BED));
    SendTxtToTFT(str_buf, TXT_PREHEAT_BED);
  }

  // Filament Change Page
  void DgusTFT::page139_handle(void) {
    typedef enum {
      FILA_NO_ACT = 0x00,
      FILA_IN = 0x01,
      FILA_OUT = 0x02,
    } filament_cmd_t;

    char str_buf[20];
    static filament_cmd_t filament_cmd = FILA_NO_ACT;
    static millis_t flash_time = 0;
    switch (key_value) {
      case 0:
        break;

      case 1:  // Back Button
      {
        filament_cmd = FILA_NO_ACT;
        injectCommands(F("G90"));
        ChangePageOfTFT(PAGE_PREPARE);
      } break;

      case 2:  // Filament in
        if (getActualTemp_celsius(E0) < 220) {
          filament_cmd = FILA_NO_ACT;
          ChangePageOfTFT(PAGE_FILAMENT_HEAT);
        } else {
          if (getTargetTemp_celsius(E0) < 230) {
            setTargetTemp_celsius(230, E0);
          }
          filament_cmd = FILA_IN;
          injectCommands(F("G91"));
        }
        break;

      case 3:  // filament out
        if (getActualTemp_celsius(E0) < 220) {
          filament_cmd = FILA_NO_ACT;
          ChangePageOfTFT(PAGE_FILAMENT_HEAT);
        } else {
          if (getTargetTemp_celsius(E0) < 230) {
            setTargetTemp_celsius(230, E0);
          }
          if (filament_cmd == FILA_NO_ACT) {
            injectCommands_P(AC_cmnd_manual_unload_filament_first_in);
          }
          filament_cmd = FILA_OUT;
          injectCommands(F("G91"));
        }
        break;

      case 4:                          // stop
        setTargetTemp_celsius(0, E0);  // kill the heating
        filament_cmd = FILA_NO_ACT;
        break;
    }

    if (millis() < (flash_time + 1000)) {
      return;
    }
    flash_time = millis();

    sprintf(str_buf, "%u/%u", (uint16_t)getActualTemp_celsius(E0),
            (uint16_t)getTargetTemp_celsius(E0));
    SendTxtToTFT(str_buf, TXT_FILAMENT_TEMP);

    if (!isPrinting()) {
      if (filament_cmd == FILA_IN) {
        if (canMove(E0) && !commandsInQueue()) {
          injectCommands_P(AC_cmnd_manual_load_filament);
        }
      } else if (filament_cmd == FILA_OUT) {
        if (canMove(E0) && !commandsInQueue()) {
          injectCommands_P(AC_cmnd_manual_unload_filament);
        }
      }
    }
  }

  // Operation Performed PopUp
  void DgusTFT::page140_handle(void) {
  #if ACDEBUG(AC_ALL)
    if ((page_index_saved != page_index_now)) {
      DEBUG_ECHOLNPGM("page20  page_index_last_2: ", page_index_last_2,
                      "  page_index_last: ", page_index_last,
                      "  page_index_now: ", page_index_now,
                      "  key: ", key_value);
      page_index_saved = page_index_now;
    }
  #endif
    switch (key_value) {
      case 0:
        break;
      case 1:  // return
      {
        ChangePageOfTFT(page_index_last);
      } break;
    }
  }

  // Nozzle Temperature Abnormal
  void DgusTFT::page141_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
        ChangePageOfTFT(page_index_last);
      } break;

      case 2: {
      } break;
    }
  }

  // Print Finish
  void DgusTFT::page142_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // OK to finish
      {
  #if ENABLED(CASE_LIGHT_ENABLE)
        setCaseLightState(0);
  #endif
        ChangePageOfTFT(PAGE_MAIN);
        setFeedrate_percent(100);  // resume print speed to 100
        print_job_timer.reset();   // clear print time elapsed
      } break;

      case 2: {
      } break;
    }
  }

  // Print Stopped
  void DgusTFT::page143_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1: {
        ChangePageOfTFT(page_index_last);
      } break;

      case 2: {
        ChangePageOfTFT(page_index_last);
      } break;
    }
  }

  // Confirm if Wants to stop
  void DgusTFT::page144_handle(void) {
    switch (key_value) {
      case 0:
        break;
      case 1:  // return
      {
        ChangePageOfTFT(page_index_last);
      } break;
      case 2: {
        ChangePageOfTFT(page_index_last);
      } break;
    }
  }

  // Filament RunOut Triggered
  void DgusTFT::page145_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLNPGM("printer_state: ", printer_state);
        SERIAL_ECHOLNPGM("pause_state: ", pause_state);
  #endif
        if (AC_printer_printing == printer_state) {
          ChangePageOfTFT(PAGE_STATUS2);  // show pause
        } else if (AC_printer_paused == printer_state) {
          ChangePageOfTFT(PAGE_STATUS1);  // show resume
        }
      } break;
    }
  }

  // Printer is Running, Try again Later
  void DgusTFT::page146_handle(void) {
    switch (key_value) {
      case 0:
        break;
      case 1:  // return
      {
        ChangePageOfTFT(page_index_last);

      } break;
      case 2: {
      } break;
    }
  }

  // Current Print will be Stopped
  void DgusTFT::page147_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // Confirm
      {
        if (isPrintingFromMedia()) {
          printer_state = AC_printer_stopping;
          stopPrint();
          message_index = 6;
          ChangePageOfTFT(PAGE_MAIN);
        } else {
          printer_state = AC_printer_idle;
        }

        setFeedrate_percent(100);
        print_job_timer.reset();
      } break;

      case 2:  // Cancel
      {
        if (AC_printer_printing == printer_state) {
          ChangePageOfTFT(PAGE_STATUS2);  // show pause
        } else if (AC_printer_paused == printer_state) {
          ChangePageOfTFT(PAGE_STATUS1);  // show print
        }
      }
    }
  }

  // Pause Failed
  void DgusTFT::page148_handle(void) {
    switch (key_value) {
      case 0:
        break;
      case 1:  // return
      {
        ChangePageOfTFT(page_index_last);

      } break;
      case 2: {
      } break;
    }
  }

  // No Sd Card Found
  void DgusTFT::page149_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
  #if ENABLED(CASE_LIGHT_ENABLE)
        setCaseLightState(0);
  #endif
        ChangePageOfTFT(PAGE_MAIN);
      } break;

      case 2: {
      } break;
    }
  }

  // Heat for Filament Change
  void DgusTFT::page150_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
        setTargetTemp_celsius(230, E0);
        ChangePageOfTFT(PAGE_FILAMENT);
      } break;
    }
  }

  // Stop Failed
  void DgusTFT::page151_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
      } break;

      case 2: {
      } break;
    }
  }

  // Paused waiting for command to complete
  void DgusTFT::page152_handle(void) {
    static millis_t flash_time = 0;

    if (millis() < (flash_time + 1000)) return;
    flash_time = millis();
  }

  // Auto-leveling Confirm page
  void DgusTFT::page153_handle(void) {
  #if ACDEBUG(AC_ALL)
    if (page_index_saved != page_index_now) {
      DEBUG_ECHOLNPGM("page153  page_index_last_2: ", page_index_last_2,
                      "  page_index_last: ", page_index_last,
                      "  page_index_now: ", page_index_now,
                      "  key: ", key_value);
      page_index_saved = page_index_now;
    }
  #endif

    switch (key_value) {
      case 0:
        break;

      case 1:  // auto leveling start
        printer_state = AC_printer_probing;

  #if ENABLED(PREHEAT_BEFORE_LEVELING)
        if (getTargetTemp_celsius(E0) < LEVELING_NOZZLE_TEMP ||
            getTargetTemp_celsius(BED) < LEVELING_BED_TEMP) {
          setTargetTemp_celsius(LEVELING_NOZZLE_TEMP, E0);
          setTargetTemp_celsius(LEVELING_BED_TEMP, BED);
          ChangePageOfTFT(PAGE_PROBE_PREHEATING);
        } else
          ChangePageOfTFT(PAGE_LEVELING);

  #else
        ChangePageOfTFT(PAGE_LEVELING);
  #endif

        break;

      case 2:
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);
        break;
    }
  }

  // Mute Handler
  void DgusTFT::page170_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1: {
        ChangePageOfTFT(PAGE_MAIN);
        if (lcd_info_back.audio != lcd_info.audio) {
          lcd_info_back.audio = lcd_info.audio;

          injectCommands_P(PSTR("M500"));
        }
      } break;

      case 2:                 // language, now print stats handler MEL_MOD
        printerStatsToTFT();  // MEL_MOD
        ChangePageOfTFT(PAGE_PRINTERSTATS);
        break;

      case 3:
        break;

      case 4:  // audio
      {
        if (lcd_info.audio == ExtUI::ON) {
          lcd_info.audio = ExtUI::OFF;
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_OFF);
        } else if (lcd_info.audio == ExtUI::OFF) {
          lcd_info.audio = ExtUI::ON;
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_ON);
        }

        LcdAudioSet(lcd_info.audio);
      } break;

      case 5:  // about

        SendTxtToTFT(DEVICE_NAME, TXT_ABOUT_DEVICE_NAME);
        SendTxtToTFT(FIRMWARE_VER, TXT_ABOUT_FW_VERSION);
        SendTxtToTFT(BUILD_VOLUME, TXT_ABOUT_PRINT_VOLUMN);
        SendTxtToTFT(TECH_SUPPORT, TXT_ABOUT_TECH_SUPPORT);

        ChangePageOfTFT(PAGE_ABOUT);
        break;

      case 6:
        ChangePageOfTFT(PAGE_RECORD);
        break;
    }
  }

  // Probe Preheating
  void DgusTFT::page175_176_handle(void) {
    static millis_t flash_time = 0;
    if (millis() < (flash_time + 500)) {
      return;
    }
    flash_time = millis();

    #if ENABLED(PREHEAT_BEFORE_LEVELING)
    if (abs(getActualTemp_celsius(E0) - LEVELING_NOZZLE_TEMP) < 2 &&
        abs(getActualTemp_celsius(BED) - LEVELING_BED_TEMP) < 2) {
      ChangePageOfTFT(PAGE_PROBE_PRECHECK);
    }
    #endif
  }

  // Auto Leveling Fail due to Nozzle too high from the bed
  void DgusTFT::page199_to_200_handle(void) {
    switch (key_value) {
      case 1:  // return

  #if ACDEBUG(AC_MARLIN)
        SERIAL_ECHOLNPGM("page_index_now: ", page_index_now);
        SERIAL_ECHOLNPGM("page_index_last: ", page_index_last);
        SERIAL_ECHOLNPGM("page_index_last_2: ", page_index_last_2);
  #endif
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);

        break;

      default:
        break;
    }
  }

  // Probe Pre Check
  void DgusTFT::page204_handle(void) {
    static millis_t probe_check_time = 0;
    static uint8_t probe_check_counter = 0;
    static uint8_t probe_state_last = 0;
    static bool probe_tare_flag = 0;

    if (!probe_tare_flag) {
      probe.tare();

      if (PROBE_TRIGGERED()) {  // triggered too early
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_PROBE_PRECHECK_FAILED);
      }
      probe_tare_flag = 1;
    }

    switch (key_value) {
      case 1:  // cancel

        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);
        break;

      default:
        break;
    }

    if (millis() >= (probe_check_time + 300)) {
      probe_check_time = millis();

      if (!probe_state_last && PROBE_TRIGGERED()) {
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_PROBE_PRECHECK_OK);
      }

      probe_state_last = PROBE_TRIGGERED();

      if (probe_check_counter++ >= 200) {  // waiting for 1 min
        probe_check_counter = 0;
        probe_tare_flag = 0;
        ChangePageOfTFT(PAGE_PROBE_PRECHECK_FAILED);
      }
    }
  }

  // Probe Check Ok
  void DgusTFT::page205_handle(void) {
    ChangePageOfTFT(PAGE_LEVELING);
    injectCommands_P(PSTR("G28\nG29"));
  }

  void DgusTFT::page206_handle(void)  // probe precheck failed
  {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, BED);
  #endif
      } break;
    }
  }

  void DgusTFT::page207_209_handle(void) {
    switch (key_value) {
      case 0:
        break;

      case 1:  // return
      {
        ChangePageOfTFT(PAGE_MAIN);
      } break;

      case 2: {
        ChangePageOfTFT(PAGE_MOVE);
      } break;

      case 3:  // set temperature
        ChangePageOfTFT(PAGE_TEMP);
        SendValueToTFT((uint16_t)getActualTemp_celsius(E0), TXT_HOTNED_NOW);
        SendValueToTFT((uint16_t)getTargetTemp_celsius(E0), TXT_HOTEND_TARGET);
        SendValueToTFT((uint16_t)getActualTemp_celsius(BED), TXT_BED_NOW);
        SendValueToTFT((uint16_t)getTargetTemp_celsius(BED), TXT_BED_TARGET);

        break;

      case 4:
        ChangePageOfTFT(PAGE_SPEED);
        SendValueToTFT((uint16_t)getActualFan_percent(FAN0), TXT_FAN_SPEED_NOW);
        SendValueToTFT((uint16_t)getTargetFan_percent(FAN0),
                       TXT_FAN_SPEED_TARGET);
        SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_NOW);
        SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_TARGET);
        break;

      case 5:  // turn off the xyz motor
        if (!isMoving()) {
          stepper.disable_all_steppers();
        }
        break;

      case 6:  // light control
  #if ENABLED(CASE_LIGHT_ENABLE)
        if (getCaseLightState()) {
          setCaseLightState(0);
          SendValueToTFT(0, ADDRESS_SYSTEM_LED_STATUS);
        } else {
          setCaseLightState(1);
          SendValueToTFT(1, ADDRESS_SYSTEM_LED_STATUS);
        }
  #endif
        break;
    }
  }

  // Print Settings
  void DgusTFT::page211_212_handle(void) {
    static bool z_change = false;

    switch (key_value) {
      case 0:
        break;

      case 1:  // return
        if (AC_printer_printing == printer_state) {
          ChangePageOfTFT(PAGE_STATUS2);  // show pause
        } else if (AC_printer_paused == printer_state) {
          ChangePageOfTFT(PAGE_STATUS1);  // show print
        }
        break;

      case 2:  // -
      {
        z_change = LowerZOffset();
      } break;

      case 3:  // +
      {
        z_change = RaiseZOffset();
      } break;

      case 4:  // light control
  #if ENABLED(CASE_LIGHT_ENABLE)
        if (getCaseLightState()) {
          SendValueToTFT(0, ADDRESS_PRINT_SETTING_LED_STATUS);
          setCaseLightState(0);
        } else {
          SendValueToTFT(1, ADDRESS_PRINT_SETTING_LED_STATUS);
          setCaseLightState(1);
        }
  #endif
        break;

      case 5:
        ChangePageOfTFT(PAGE_DONE);
        break;

      case 6:

        break;

      case 7:
        RequestValueFromTFT(TXT_ADJUST_BED);
        RequestValueFromTFT(TXT_ADJUST_SPEED);
        RequestValueFromTFT(TXT_ADJUST_HOTEND);
        RequestValueFromTFT(TXT_FAN_SPEED_TARGET);

        if (z_change) {
          injectCommands_P(PSTR("M500"));
          z_change = false;
        }

        if (AC_printer_printing == printer_state) {
          ChangePageOfTFT(PAGE_STATUS2);  // show pause
        } else if (AC_printer_paused == printer_state) {
          ChangePageOfTFT(PAGE_STATUS1);  // show print
        }

        break;
    }
  }

  // Printer Stats
  void DgusTFT::page213_handle(void) {
    switch (key_value) {
      case 0:
        break;
      case 0x0999:  // return
        if (lcd_info.audio == ExtUI::ON) {
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_ON);
        } else if (lcd_info.audio == ExtUI::OFF) {
          ChangePageOfTFT(PAGE_SYSTEM_AUDIO_OFF);
        }
        break;
    }
  }

  void DgusTFT::pop_up_manager(void) {
    char str_buf[20];

    switch (pop_up_index) {
      case POPUP_T0_ERROR:  // T0 error
        if (page_index_now != PAGE_ABNORMAL) ChangePageOfTFT(PAGE_ABNORMAL);
        pop_up_index = POPUP_NONE;
        break;

      case POPUP_FILAMENT_LACK:  // filament lack
        if (page_index_now != PAGE_FILAMENT_LACK)
          ChangePageOfTFT(PAGE_FILAMENT_LACK);
        pop_up_index = POPUP_NONE;
        break;

      case POPUP_STOP_WAIT:  // stop wait
        ChangePageOfTFT(PAGE_WAIT_STOP);
        pop_up_index = POPUP_NONE;
        break;

      case POPUP_FILAMENT_LACKING:  //
        if (page_index_now != PAGE_FILAMENT_LACK) {
          ChangePageOfTFT(PAGE_FILAMENT_LACK);
        }
        pop_up_index = POPUP_NONE;
        break;

      case POPUP_PRINT_FINISH:  //
      {
        uint32_t time = getProgress_seconds_elapsed() / 60;
        sprintf(str_buf, "%s H ", utostr3(time / 60));
        sprintf(str_buf + strlen(str_buf), "%s M", utostr3(time % 60));
        SendTxtToTFT(str_buf, TXT_FINISH_TIME);
        ChangePageOfTFT(PAGE_PRINT_FINISH);
        pop_up_index = POPUP_NONE;
      } break;

      case POPUP_LEVELING_DONE:  // LEVEL DONE
        ChangePageOfTFT(PAGE_LEVELING_SETTINGS);
        pop_up_index = POPUP_NONE;
        break;
    }
  }

  void DgusTFT::printerStatsToTFT(void) {  // MEL_MOD printer statistics
    #if ENABLED(PRINTCOUNTER)
      char str_buf[64];
      char buffer[32];
      int32_t metresUsed, metresRemainder;

      printStatistics stats = print_job_timer.getStats();  // returns raw data

      sprintf(str_buf, "Total: %d", stats.totalPrints);
      SendTxtToTFT(str_buf, TXT_STATS_PRINTS_TOTAL);

      sprintf(str_buf, "Finished: %d", stats.finishedPrints);
      SendTxtToTFT(str_buf, TXT_STATS_PRINTS_FINISHED);

      sprintf(str_buf, "Failed: %d", stats.totalPrints - stats.finishedPrints);
      SendTxtToTFT(str_buf, TXT_STATS_PRINTS_FAILED);

      sprintf(str_buf, "Time: %s", duration_t(stats.printTime).toString(buffer));
      SendTxtToTFT(str_buf, TXT_STATS_PRINTS_TIME);

      sprintf(str_buf, "Longest: %s", duration_t(stats.longestPrint).toString(buffer));
      SendTxtToTFT(str_buf, TXT_STATS_PRINTS_LONGEST);

      metresUsed      = int32_t(stats.filamentUsed / 1000);
      metresRemainder = int16_t(stats.filamentUsed / 100) % 10;
      sprintf(str_buf, "Filament Used: %ld.%ldm", metresUsed, metresRemainder);
      SendTxtToTFT(str_buf, TXT_STATS_PRINTS_FILAMENT);
    #else
      SendTxtToTFT("N/A", TXT_STATS_PRINTS_TOTAL);
    #endif
  }

  void DgusTFT::SendtoTFTLN(PGM_P fstr) {
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

}  // namespace Anycubic

#endif  // ANYCUBIC_LCD_KOBRA
