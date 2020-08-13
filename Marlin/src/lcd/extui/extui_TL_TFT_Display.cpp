//
//
//
/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/


//#pragma message(__FILE__)
//IGHMC, included these based on MALYAN example and because Tenlog has modifiers in most of them then realised they need to come from UI_api.h
#include "src/inc/MarlinConfigPre.h"
#include "src/lcd/extui_TL_TFT_Display.h"
#include "src/lcd/extui/ui_api.h"
#include "../../libs/numtostr.h"
#include "src/module/printcounter.h"
#include "src/MarlinCore.h"
#include "src/gcode/queue.h"
#include "src/module/motion.h"
#include "src/lcd/fontutils.h"
#include "src/sd/cardreader.h"
#include <string.h>
#include <WString.h>

#include "src/module/temperature.h"
#include "src/module/planner.h"

#include <avr/pgmspace.h>
//#include "src/sd/cardreader.h"
// #include "src/gcode/queue.h"
// #include "src/libs/duration_t.h"
// #include "src/module/motion.h"

// #include "src/module/stepper.h"
// #include "src/module/temperature.h"

#if EITHER(EXTENSIBLE_UI, DWIN_CREALITY_LCD)
#define START_OF_UTF8_CHAR(C) (((C)&0xC0u) != 0x80u)
#endif
// ********************************************* IGHMC, top section from
// Ultralcd.h, MarlinUI for extensible */
byte languageID = TL_LANGUAGEID;
extern bool wait_for_user, wait_for_heatup;
#define MAX_MESSAGE_LENGTH 32

#if EITHER(HAS_SPI_LCD, EXTENSIBLE_UI)
uint8_t MarlinUI::alert_level; // = 0
char MarlinUI::status_message[MAX_MESSAGE_LENGTH + 1];
#endif

void MarlinUI::media_changed(const uint8_t old_status, const uint8_t status) {
  if (old_status == status) {
    TERN_(EXTENSIBLE_UI, ExtUI::onMediaError()); // Failed to mount/unmount
    return;
  }
}

void MarlinUI::status_printf_P(const uint8_t, PGM_P const message, ...) {
  TERN(HOST_PROMPT_SUPPORT, host_action_notify_P(message), UNUSED(message));
}

void MarlinUI::abort_print() { ExtUI::abort_print(); }

void MarlinUI::set_status(const char *const message, const bool) {
#if ENABLED(HOST_PROMPT_SUPPORT)
  host_action_notify(message);
#else
  UNUSED(message);
#endif
if(marlin_state==MF_STOPPED){
  ExtUI::TL_printdone(message);
}
}

void MarlinUI::set_alert_status_P(PGM_P const message) {
  set_status_P(message, 1);
  TERN_(HAS_LCD_MENU, return_to_status());
}

static uint8_t alert_level;
//void reset_alert_level(bool=0){};
bool MarlinUI::has_status() { return false; }
void MarlinUI::finish_status(bool) {}
void MarlinUI::reset_status() { NOOP; }

void MarlinUI::set_status_P(PGM_P const message, int8_t level) {
  if (level < 0)
    level = alert_level = 0;
  if (level < alert_level)
    return;
  alert_level = level;

  TERN_(HOST_PROMPT_SUPPORT, host_action_notify_P(message));

  // Since the message is encoded in UTF8 it must
  // only be cut on a character boundary.

  // Get a pointer to the null terminator
  PGM_P pend = message + strlen_P(message);

  // If length of supplied UTF8 string is greater than
  // the buffer size, start cutting whole UTF8 chars
  while ((pend - message) > MAX_MESSAGE_LENGTH) {
    --pend;
    while (!START_OF_UTF8_CHAR(pgm_read_byte(pend)))
      --pend;
  };

  // At this point, we have the proper cut point. Use it
  uint8_t maxLen = pend - message;
  strncpy_P(status_message, message, maxLen);
  status_message[maxLen] = '\0';

  finish_status(level > 0);
}
  void status_printf_P(const uint8_t, PGM_P const message, ...) {
#if ENABLED(HOST_PROMPT_SUPPORT)
  host_action_notify(message);
#else
  UNUSED(message);
#endif
}
#define LCD_SERIAL   customizeSerial3
                    // IGHMC, Serial2 is TL default and presumably wired, Marlin2
                   // supports it but LCD_SERIAL probably redundant

//************************************** IGHMC Extensible UI additions taken
// from Tenlog */
// variables defined from Tenlog then commented out as they are replaced with
// Marlin standard or changed to work only with TL_TFT
// IGHMC - definitions for Tenlog functions from Marlin.ino
// copied here while I figure out how to convert them to Marlin2 standard calls
// so they are called from the right places
// static char *strchr_pointer; // just
// a pointer to find chars in the cmd string like X, Y, Z, E, etc static long
// gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

//IGHMC, just a number, todo: make it smaller if possible//IGHMC - tenlog screen from Marlin_main.cpp
  static ExtUI::FileList filelist;

  namespace ExtUI { //IGHMC candidate for progmem?
  byte i_print_page_id = 0;
  millis_t TL_printStarted;
  millis_t TL_statusstart = millis();
  millis_t TL_statuswait = 1000;
  bool TL_screenready = true;

  const char chrEnd = 0xFF;
  //char*    itostr2(uint16_t& x);  IGHMC replaced with Marlin version
  void TenlogScreen_begin(unsigned long int boud) {
    customizedSerial3.begin(boud);
    }
    void onPrintTimerStarted() {
      // TL_printStarted = printTime;
    }
    void TenlogScreen_println(const char s[]) {
      customizedSerial3.print(s);
      customizedSerial3.write(chrEnd);
      customizedSerial3.write(chrEnd);
      customizedSerial3.write(chrEnd);
    }
    void TenlogScreen_print(const char s[]) { customizedSerial3.print(s); }
    void TenlogScreen_printend() {
      customizedSerial3.write(chrEnd);
      customizedSerial3.write(chrEnd);
      customizedSerial3.write(chrEnd);
    }
    void TenlogScreen_printEmptyend() {
      customizedSerial3.write(static_cast<byte>(0x00));
      customizedSerial3.write(chrEnd);
      customizedSerial3.write(chrEnd);
      customizedSerial3.write(chrEnd);
    }
    bool MSerial2_available()  {
      return customizedSerial3.available();
    }
    char MSerial2_read() {
      return customizedSerial3.read();
    }


    // To implement a new UI, complete the functions below and
    // read or update Marlin's state using the methods in the
    // ExtUI methods in "../ui_api.h"
    //
    // Although it may be possible to access other state
    // variables from Marlin, using the API here possibly
    // helps ensure future compatibility.


    void Init_TenlogScreen() {

      //IGHMC separating the TL variables that will be stored in EEPROM and used for TL Menu from Marlin 'normal' variables, todo: investigate just using the Marlin standard
      float tmp[XYZ][HOTENDS] = {HOTEND_OFFSET_X, HOTEND_OFFSET_Y, HOTEND_OFFSET_Z};
      float TL_axis_steps_per_unit[XYZE] = DEFAULT_AXIS_STEPS_PER_UNIT;
      float TL_X2_MAX_POS = X2_MAX_POS;
      float TL_Y2_OFFSET = tmp[1][1];
      float TL_Z2_OFFSET = tmp[2][1];
      byte TL_SLEEP_TIME = 0;
      SERIAL_ECHOLN("TL_TFT.CPP  127      IGHMC  starting  init  tenlog  screen"); //IGHMC  -  todo  remove  this  up
      _delay_ms(20);
      TenlogScreen_print("main.vLanguageID.val=");
      TenlogScreen_print(String(languageID).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      long iSend = TL_X2_MAX_POS * 100.0;
      TenlogScreen_print("setting.xX2.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = TL_Y2_OFFSET * 100.0;
      TenlogScreen_print("setting.xY2.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = TL_Z2_OFFSET * 100.0;
      TenlogScreen_print("setting.xZ2.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = TL_axis_steps_per_unit[0] * 100.0;
      TenlogScreen_print("setting.xXs.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = TL_axis_steps_per_unit[1] * 100.0;
      TenlogScreen_print("setting.xYs.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = TL_axis_steps_per_unit[2] * 100.0;
      TenlogScreen_print("setting.xZs.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = TL_axis_steps_per_unit[3] * 100.0;
      TenlogScreen_print("setting.xEs.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);
  //IGHMC - sort out fan control
#ifdef FAN2_CONTROL
      iSend = zyf_FAN2_VALUE;
      TenlogScreen_print("setting.nF2s.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
#else
      TenlogScreen_println("setting.nF2s.val=0");
#endif
      _delay_ms(20);

#ifdef FAN2_CONTROL
      iSend = zyf_FAN2_START_TEMP;
      TenlogScreen_print("setting.nF2t.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
#else
      TenlogScreen_println("setting.nF2t.val=0");
#endif
      _delay_ms(20);

      iSend = TL_X2_MAX_POS * 10.0;
      TenlogScreen_print("main.vXMax.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = Y_MAX_POS * 10.0;
      TenlogScreen_print("main.vYMax.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = Z_MAX_POS * 10.0;
      TenlogScreen_print("main.vZMax.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);

#ifdef POWER_LOSS_TRIGGER_BY_PIN
      TenlogScreen_println("main.vPFR.val=1");
      TenlogScreen_printend();
      _delay_ms(20);

      iSend = zyf_AUTO_OFF;
      TenlogScreen_print("setting.cAutoOff.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);
#endif //POWER_LOSS_TRIGGER_BY_PIN

      iSend = TL_SLEEP_TIME;
      TenlogScreen_print("setting.nSleep.val=");
      TenlogScreen_print(String(iSend).c_str());
      TenlogScreen_printend();
      _delay_ms(20);
      TenlogScreen_println("sleep=0");
      _delay_ms(20);

      SERIAL_ECHOLN("TL_TFT.CPP                            IGHMC  -  end  of  screen  init"); //IGHMC  remove  this  upon  release
    }
    /*********************** Tenlog helper functions *****************************/

    bool strISAscii(String str) {
      bool bOK = true;
      int iFNL = str.length();
      char cFN[iFNL];
      str.toCharArray(cFN, iFNL);
      for (int i = 0; i < iFNL - 1; i++) {
        if (!isAscii(cFN[i])) {
          // ZYF_DEBUG_PRINT_LN((int)cFN[i]);
          bOK = false;
          break;
        }
      }
      return bOK;
    }
    //Get Data From Commport
    String getSerial2Data() {
      String strSerialdata = "";
      while (MSerial2_available() > 0) {
        strSerialdata += char(MSerial2_read());
        delay(2);
      }
      return strSerialdata;
    }
    ///////////////////split by zyf  //IGHMC now only used for CSDI- can be removed by taking the actual string?
    String getSplitValue(String data, char separator, int index) {
      int found = 0;
      int strIndex[] = {0, -1};
      int maxIndex = data.length() - 1;

      for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
      }
      return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
    }

    float string2Float(String Value) {
      char floatbuf[32]; // make this at least big enough for the whole string
      Value.toCharArray(floatbuf, sizeof(floatbuf));
      float fRet = atof(floatbuf);
      return fRet;
    }
    float string2int(String Value) {
      if (Value.length() < 4) {
        char floatbuf[32]; // make this at least big enough for the whole string
        Value.toCharArray(floatbuf, sizeof(floatbuf));
        float fRet = atoi(floatbuf);
        return fRet;
      } else {
        return 0;
      }
    }


    void CSDI_TLS() {
      unsigned long lScreenStart = 0;
      lScreenStart = millis();
      TenlogScreen_printEmptyend();
      TenlogScreen_println("TL Screen connect");
      bool bCheckDone = false;

      while (!bCheckDone) {
        String strSerial2 = getSerial2Data();
        if (strSerial2 != "") {
          SERIAL_ECHOLN(strSerial2);
          strSerial2.replace("\r", "");
          strSerial2.replace("\n", "");
          String strDI = getSplitValue(strSerial2, ',', 5); //IGHMC replace with a constant?
          bCheckDone = true;
          TenlogScreen_print("loading.sDI.txt=\"");
          TenlogScreen_print(strDI.c_str());
          TenlogScreen_print("\"");
          TenlogScreen_printend();
          _delay_ms(20);
          TenlogScreen_println("click btA,0");
        } else {
          _delay_ms(10);
        }
        if (millis() - lScreenStart > 1000) {
          TenlogScreen_printEmptyend();
          TenlogScreen_println("connect");
          SERIAL_ECHOLN("            CTL_TFT:350  looping  to  attempt  "
                        "connection"); // IGHMC  remmove
          lScreenStart = millis();
        }
      }
    }
    /*********************** end Tenlog helper functions *****************************/
    void TL_sdcarddisplay() {
      uint16_t fileCnt = card.get_num_Files();
      card.getWorkDirName();

      if (i_print_page_id == 0) {
        TenlogScreen_println("vis btUp,0");
        TenlogScreen_println("vis picUp,0");
      } else {
        TenlogScreen_println("vis btUp,1");
        TenlogScreen_println("vis picUp,1");
      }

      int iFileID = 0;
      // Clear the boxlist
      for (int i = 1; i < 7; i++) {
        TenlogScreen_print("select_file.tL");
        TenlogScreen_print(String(i).c_str());
        TenlogScreen_print(".txt=\"\"");
        TenlogScreen_printend();

        TenlogScreen_print("select_file.sL");
        TenlogScreen_print(String(i).c_str());
        TenlogScreen_print(".txt=\"\"");
        TenlogScreen_printend();
      }

      for (uint16_t i = 0; i < fileCnt; i++) {
        card.getfilename_sorted(fileCnt - 1 - i);
        String strFN =
            String(card.filename); // + " | " + String(card.filename);
        if (!card.flag.filenameIsDir && strFN.length() > 0) {
          if (strISAscii(strFN)) {
            strFN = String(card.longFilename);
            strFN.toLowerCase();
            String strLFN = strFN;
            iFileID++;
            if (iFileID >= (i_print_page_id)*6 + 1 &&
                iFileID <= (i_print_page_id + 1) * 6) {
              strFN = String(card.filename);
              strFN.toLowerCase();
              if (strLFN == "")
                strLFN = strFN;

              int iFTemp = iFileID - (i_print_page_id)*6;
              TenlogScreen_print("select_file.tL");
              TenlogScreen_print(String(iFTemp).c_str());
              TenlogScreen_print(".txt=\"");
              strLFN.toLowerCase();
              TenlogScreen_print(strLFN.c_str());
              TenlogScreen_print("\"");
              TenlogScreen_printend();

              TenlogScreen_print("select_file.sL");
              TenlogScreen_print(String(iFTemp).c_str());
              TenlogScreen_print(".txt=\"");
              TenlogScreen_print(strFN.c_str());
              TenlogScreen_print("\"");
              TenlogScreen_printend();
              // ZYF_DEBUG_PRINT_LN(strFN);
            }
            // MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename,
            // card.longFilename);
          }
        }
      }
      TenlogScreen_print("select_file.vPageID.val=");
      TenlogScreen_print(String(i_print_page_id).c_str());
      TenlogScreen_printend();

      if ((i_print_page_id + 1) * 6 >= iFileID) {
        TenlogScreen_println("vis btDown,0");
        TenlogScreen_println("vis picDown,0");
      } else {
        TenlogScreen_println("vis btDown,1");
        TenlogScreen_println("vis picDown,1");
      }
    }

    bool TL_parameter_update()
    {
      String strAll = "main.sStatus.txt=\"";

      //IGHMC tenlog current_position[X_AXIS] replaced with ui calls

      long lN = getAxisPosition_mm(X) * 10.0; //1
      String sSend = String(lN);
      strAll = strAll + sSend + "|";

      lN = getAxisPosition_mm(Y) * 10.0; //2
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      lN = getAxisPosition_mm(Z) * 10.0; //3
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      // lN = getAxisPosition_mm(E0_AXIS) * 10.0; //4  IGHMC - why report this value when screen doesn't show it
      lN = 0; //4
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //IGHMC using Marlin UI calls instead of zyf specific variables
      //lN = int(degTargetHotend(0) + 0.5); //5
      lN = int(0.5 + getTargetTemp_celsius(E0)); //5
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //lN = int(degHotend(0) + 0.5); //6
      lN = int(0.5 + getActualTemp_celsius(E0)); //6
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      // lN = int(degTargetHotend(1) + 0.5); //7
      lN = int(0.5 + getTargetTemp_celsius(E1)); //7
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      // lN = int(degHotend(1) + 0.5); //8
      lN = int(0.5 + getActualTemp_celsius(E1)); //8
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //lN = int(degTargetBed() + 0.5);            //9
      lN = int(0.5 + getTargetTemp_celsius(BED)); //9
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //lN = int(degBed() + 0.5); //10
      lN = int(0.5 + getActualTemp_celsius(BED)); //10
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //lN = fanSpeed; //11
      lN = getActualFan_percent(FAN0);
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //lN = feedmultiply; //12               //IGHMC - changed from tenlog to Marlin variable (M220), NB: Tenlog also support M221 (extrude feed rate) like Marlin but has no screen output for it
      lN = getFeedrate_percent(); //12
      lN = 0.0;                   //12
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      if (TL_sdprinting()==1) { //13
        strAll = strAll + "1|";
        lN = card.percentDone();
        sSend = String(lN); //14
        strAll = strAll + sSend + "|";
      } else if (TL_sdprinting() == 0) {
        strAll = strAll + "0|0|";
      } else if (TL_sdprinting() == 2) {
        strAll = strAll + "2|0|";
      }
      //IGHMC changed tenlog active_extruder for ui, check that E0 will work!!
      lN = active_extruder; //15
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      lN = dual_x_carriage_mode; //16
      sSend = String(lN);
      strAll = strAll + sSend + "|";

      //lN=dual_x_carriage_mode;                //17 time
      //if (IS_SD_PRINTING) {    //IGHMC, clarify is_sd_printing vs ui.isprinting
      if (isPrinting())
      {
        uint16_t time = millis() / 60000 - TL_printStarted / 60000;
        sSend = String(ui16tostr5rj(time / 60)) + ":" + String(ui16tostr5rj(time % 60));
        strAll = strAll + sSend + "|";
      }
      else
      {
        strAll = strAll + "00:00|";
      }
      //if (card.isFileOpen()) {
      if (isPrintingFromMedia())
      {
        //18 is file open
        strAll = strAll + "1|";
      }
      else
      {
        strAll = strAll + "0|";
      }

      if (isHeaterIdle(H0))
      {
        //19 is heating nozzle 0
        strAll = strAll + "1|";
      }
      else
      {
        strAll = strAll + "0|";
      }

      if (isHeaterIdle(H1))
      {
        //20 is heating nozzle 1
        strAll = strAll + "1|";
      }
      else
      {
        strAll = strAll + "0|";
      }

      if (isHeaterIdle(BED))
      {
        //21 is heating Bed
        strAll = strAll + "1|";
      }
      else
      {
        strAll = strAll + "0|";
      }

      strAll = strAll + "\"";
      const char *strAll0 = strAll.c_str();
      TenlogScreen_println(strAll0);
      delay(50);
      TenlogScreen_println("click btReflush,0");
      SERIAL_ECHO("\\");
      return 1;
    }

    void TL_status_timer() { // IGHMC, Marlin might take care of this another way
      millis_t tl_temp = millis();
      if (tl_temp - TL_statusstart > TL_statuswait and TL_screenready) {
        TL_screenready = false;
        TL_screenready=TL_parameter_update();
        TL_statusstart = tl_temp;
        SERIAL_ECHO("/");
      }
      return;
    }

    void onStartup() {
      /* Initialize the display module here. The following
       * routines are available for access to the GPIO pins:
       *
       *   SET_OUTPUT(pin)
       *   SET_INPUT_PULLUP(pin)
       *   SET_INPUT(pin)
       *   WRITE(pin,value)
       *   READ(pin)
       */

      // IGHMC, this section from Marlin_main:: setup and assumed to be startup
      // routine for Tenlog TFT
      //
      _delay_ms(20);
      TenlogScreen_begin(BAUDRATE2);
      _delay_ms(100);
      TenlogScreen_println("sleep=0");
      _delay_ms(20);
      String strDate = __DATE__;
      TenlogScreen_print("about.tVer.txt=\"Firmware: ");
      TenlogScreen_print(strDate.c_str());
      TenlogScreen_println("\"");
      SERIAL_ECHO_MSG("firmware "); // IGHMC remmove
#ifdef POWER_LOSS_TRIGGER_BY_PIN
      TenlogScreen_println("main.vPFR.val=1");
      _delay_ms(20);
#endif
      TenlogScreen_println("page  0");
      _delay_ms(20);
      SERIAL_ECHOLN("IGHMC               tl_tft:  527 end  of  tl  startup");
    }

    void onStartup_post() { // IGHMC TFT screen seems to need a set procedure so
                       // breaking the initialisation to match original setup()
      CSDI_TLS();
      Init_TenlogScreen();
    }
    void TL_mainmenu_load() {
      TenlogScreen_println("sleep=0");
      TenlogScreen_println("page main");
    #ifdef POWER_LOSS_RECOVERY
      String sFileName = card.isPowerLoss();

      if (sFileName != "")
      {
        TenlogScreen_println("msgbox.vaFromPageID.val=1");
        TenlogScreen_println("msgbox.vaToPageID.val=6");
        TenlogScreen_println("msgbox.vtOKValue.txt=\"M1003\"");
        TenlogScreen_println("msgbox.vtCancelValue.txt=\"M1004\"");
        String strMessage = "";
        if (languageID == 0)
          strMessage = "Power  loss  detected,  Resume  print  " + sFileName + "?";
        else
          strMessage = "????????????" + sFileName + "??";
        strMessage = "msgbox.tMessage.txt=\"" + strMessage + "\"";
        const char *str0 = strMessage.c_str();
        TenlogScreen_println(str0);
        TenlogScreen_println("page  msgbox");
      }
    #endif
    }

    void TL_SDPauseupdate() {
      TenlogScreen_println("reload.vaFromPageID.val=6");
      String strMessage = "reload.sT1T2.txt=\"" + String(active_extruder + 1) + "\"";
      const char *str0 = strMessage.c_str();
      TenlogScreen_println(str0);
      strMessage = "reload.vaTargetTemp0.val=" + String(Temperature::degTargetHotend(0)) + "";
      str0 = strMessage.c_str();
      TenlogScreen_println(str0);
      strMessage = "reload.vaTargetTemp1.val=" + String(Temperature::degTargetHotend(0)) + "";
      str0 = strMessage.c_str();
      TenlogScreen_println(str0);
      strMessage = "reload.vaTargetBed.val=" + String(int(Temperature::degTargetBed() + 0.5)) + "";
      str0 = strMessage.c_str();
      TenlogScreen_println(str0);
      strMessage = "reload.vaMode.val=" + String(dual_x_carriage_mode) + "";
      str0 = strMessage.c_str();
      TenlogScreen_println(str0);

      if (duplicate_extruder_x_offset != DEFAULT_DUPLICATION_X_OFFSET) {
        strMessage = "reload.vaMode2Offset.val=" + String(duplicate_extruder_x_offset) + "";
        str0 = strMessage.c_str();
        TenlogScreen_println(str0);
      } else
        TenlogScreen_println("reload.vaMode2Offset.val=-1");
    } //TL_SDPauseupdate
  const uint8_t TLG PROGMEM = 0;
  const uint8_t TLM PROGMEM = 1;
  const uint8_t TLF PROGMEM = 2;
  const uint8_t TLX PROGMEM = 3;
  const uint8_t TLY PROGMEM = 4;
  const uint8_t TLZ PROGMEM = 5;
  const uint8_t TLn PROGMEM = 6;
  const uint8_t TLE PROGMEM = 7;
  const uint8_t TLS PROGMEM = 8;
  const uint8_t TLT PROGMEM = 9;
  const uint8_t TLR PROGMEM = 10;
  const uint8_t TLm PROGMEM = 11;

  uint8_t TL_switcher(char p) {
    switch (p) {
      case 'G':
        return TLG;
        break;
      case 'M':
        return TLM;
      case 'F':
        return TLF;
        break;
      case 'X':
        return TLX;
        break;
      case 'Y':
        return TLY;
        break;
      case 'Z':
        return TLZ;
        break;
      case 'E':
        return TLE;
        break;
      case 'S':
        return TLS;
        break;
      case 'T':
        return TLT;
        break;
      case 'R':
        return TLR;
        break;
      case 'm': //ighmc seperates the case where M is also a sub parameter
        return TLm;
        break;
      default:
        return 99;
        break;
    }
  }

  void TL_get_command_1() {
    #define ISEOL(C) ((C) == '\n' || (C) == '\r') //IGHMC
    char TL2_transcmd[36] = {"                                  \0"}; //translated command
    TL2_transcmd[0] = 0;
    char TL_buffer[128] = "";               //TL screen output
    TL_buffer[128] = 0;

    bool TL_seenpar[12];
    float TL_valuepar[12];
    char TL_subcdpar[12];
    for (uint8_t i = TLG; i < TLm; i++) {
      TL_seenpar[i] = false;
      TL_valuepar[i] = 0.0;
      TL_subcdpar[i] = ' ';
    }
    String TL_trimval = "";
    TL_trimval.reserve(10);

    while (MSerial2_available() > 0) {
      strncpy(TL_buffer, ExtUI::getSerial2Data().c_str(), sizeof(TL_buffer));
      SERIAL_ECHOLNPAIR("L931 get command >>",TL_buffer);
      if(ISEOL(TL_buffer[0])){
      return;
      } else {
        SERIAL_ECHOLNPAIR("buffer has data >> ", String(strlen(TL_buffer)));
      }
      char *TL_start = TL_buffer;
      char *TL_sub;
      uint8_t TL_pCode = 0;

      while ((TL_sub = strtok_r(TL_start, "\n", &TL_start))) {
        // IGHMC deal with all the standard codes that aren't modified by the tenlog screen
        SERIAL_ECHOLNPAIR("tl_sub: ", TL_sub);
        uint16_t TL_stdcodetest = atoi(&TL_sub[1]);
        if ((1000 > TL_stdcodetest && !((TL_stdcodetest == 19) || (TL_stdcodetest == 92) || (TL_stdcodetest == 1))) || TL_sub[0] == 'T') {
          queue.inject(TL_sub);
          queue.advance();
          break;
        }
        //and translate the rest into standard
        char *TL_param = TL_sub;
        char *TL_parsub;

        while ((TL_parsub = strtok_r(TL_param, " ", &TL_param))) {
          SERIAL_ECHOLNPAIR("tl_par: ", TL_parsub);
          TL_trimval = "";
          TL_pCode = TL_switcher(TL_parsub[0]);
          if (TL_pCode == 99) {
            SERIAL_ECHOLN("broken switch");
            break;
          }
          if ((TL_seenpar[TLM] || TL_seenpar[TLG]) && (TL_pCode == TLM)) {
            TL_pCode = TLm; //If  M or G  has already been set then make it sub param not a new M code
            SERIAL_ECHOLN("m detected");
          } // Tenlog 'm' changes  relative/absolute for G1 possibly for more. Marlin sets bool relative_mode by calling G90/91

          TL_seenpar[TL_pCode] = true;
          TL_subcdpar[TL_pCode] = TL_parsub[0];
          TL_valuepar[TL_pCode] = atof(&TL_parsub[1]);

          if (TL_pCode == TLR) {  //convert the R rate from screen to a reciprocal for easier math
            TL_trimval = TL_parsub[1];
            10 == atoi(&TL_parsub[1]) ? TL_valuepar[TL_pCode] = .1 : TL_valuepar[TL_pCode] = .01;
          }
        }  //parsub
        SERIAL_ECHOLN("injecty");
        SERIAL_ECHOLN("************************ \n");

        if (TL_pCode == 99) {
          SERIAL_ECHO("missing sub-code: ");
          SERIAL_ECHOLN(TL_subcdpar[0]);
          break;
        }
        if (!TL_seenpar[TLG] && !TL_seenpar[TLM] && !TL_seenpar[TLT]) {
          SERIAL_ECHOLN(" no M or G or T code found");
          break;
        }
        if (TL_seenpar[TLR]) {
          TL_valuepar[TLX] = TL_valuepar[TLX] * TL_valuepar[TLR];
          TL_valuepar[TLY] = TL_valuepar[TLY] * TL_valuepar[TLR];
          TL_valuepar[TLZ] = TL_valuepar[TLZ] * TL_valuepar[TLR];
          TL_valuepar[TLE] = TL_valuepar[TLE] * TL_valuepar[TLR];
          TL_valuepar[TLS] = TL_valuepar[TLS] * TL_valuepar[TLR];
          TL_seenpar[TLR] = false; //we never put the R figure out from the screen
        }
        // for (uint8_t i = TLG; i < TLm; i++) {
        //   SERIAL_ECHO(String(i));
        //   SERIAL_ECHO(" :");
        //   SERIAL_ECHOLNPAIR("seenpar:   ", TL_seenpar[i]);
        //   SERIAL_ECHOLNPAIR("   seencode:  ", TL_subcdpar[i]);
        //   SERIAL_ECHOLNPAIR("   seenvalue: ", TL_valuepar[i]);
        // }
        if (TL_seenpar[TLG] || (TL_seenpar[TLM]) || (TL_seenpar[TLT])) {
          int8_t TL_done = strlen(TL2_transcmd);
          if (TL_seenpar[TLG]) { // G1
            // SERIAL_ECHOLNPAIR("found G? :", TL_subcdpar[TLG]);
            if (TL_subcdpar[TLG] == 'G') {
              // SERIAL_ECHOLNPAIR("found G1? :", TL_valuepar[TLG]);
              bool old_relative_mode = relative_mode;
              if (TL_seenpar[TLm] && (TL_valuepar[TLm] == 1)) {
                if (!relative_mode) {
                  // SERIAL_ECHOPGM(" G91 ");
                  queue.enqueue_now_P(PSTR("G91"));
                  queue.advance();
                  // SERIAL_ECHOPGM(" > ");
                }
              }
              //TL does an update to Main.sStatus.txt here but only x axis
              for (int i = TLG; i <= TLT; i++) {
                if (TL_seenpar[i]) {
                  TL_trimval = TL_subcdpar[i];
                  strcat(TL2_transcmd, TL_trimval.c_str());
                  if ((i < TLX) || (i == TLT)) { //G,M and T are all integers, others could be float or integer
                    TL_trimval = ui16tostr4rj((uint16_t)(TL_valuepar[i]));
                    TL_trimval.trim();
                    strcat(TL2_transcmd, TL_trimval.c_str());
                  } else {
                    TL_trimval = ftostr42_52(TL_valuepar[i]);
                    TL_trimval.trim();
                    strcat(TL2_transcmd, TL_trimval.c_str());
                  }
                  if ((int8_t)strlen(TL2_transcmd) > TL_done) {
                    strcat(TL2_transcmd, " ");
                    TL_done = strlen(TL2_transcmd);
                  }
                  // SERIAL_ECHOLNPAIR("building tl2 : ", TL2_transcmd);
                }
              }
              // SERIAL_ECHOLNPAIR("tl2 for G1: ", TL2_transcmd);
              queue.inject(&TL2_transcmd[0]);
              queue.advance();
              if (!old_relative_mode) {
                // SERIAL_ECHOPGM("G90 ");
                queue.enqueue_now_P(PSTR("G90"));
                queue.advance();
                // SERIAL_ECHOPGM(" > ");
              }
              ZERO(TL2_transcmd);
              TL_trimval = "";
              for (uint8_t i = TLG; i <= TLm; i++) {
                TL_seenpar[i] = false;
                TL_valuepar[i] = 0.0;
                TL_subcdpar[i] = ' ';
              }
            } else {
              SERIAL_ECHOLN("lost in translation found par 0 with no G");
              break;
            }
          // SERIAL_ECHOLN("END OF G1   ");
          } //G1

          if (TL_seenpar[TLT] && !(TL_seenpar[TLG] || TL_seenpar[TLM])) { //Change tool
            // SERIAL_ECHOLNPAIR("change tool",TL_sub);
            (TL_valuepar[TLT] == 0) ? queue.inject_P(PSTR("T1")) : queue.inject_P(PSTR("T0"));

            break;
            }
          uint16_t TL_tempry = 0;
          // SERIAL_ECHOLNPAIR("END OF T    ", &TL_parsub);
          if (TL_seenpar[TLM]) { //M main tranlsation processing
            switch ((uint16_t)TL_valuepar[TLM]) {
              case 19: {
                if (TL_seenpar[TLS]) {
                  i_print_page_id = TL_valuepar[TLS];
                }
                TL_sdcarddisplay();
                break;
              }
              case 92: {
                //already translated by dealing with R100

                break;
              }

              case 1001: {
                //ighmc screen language only, no affect on Marlin
                languageID == 0 ? languageID = 1 : languageID = 0;
                break;
              }
              case 1002: {
                //this is listed as 'calibrate' but has no action in tenlog code
                //however its sent at the end of a print so maybe use it for print end?
                  SERIAL_ECHOLN("from TLTFT: M31");
                  queue.enqueue_one_P(PSTR("M31"));
                  ZERO(TL2_transcmd); //clear any stragglers
                  TL_trimval = "";
                  for (uint8_t i = TLG; i <= TLm; i++) {
                    TL_seenpar[i] = false;
                    TL_valuepar[i] = 0.0;
                    TL_subcdpar[i] = ' ';
                  }
                  break;
              }
              case 1003: {
                //IGHMC Power loss recovery, no hardware support in place but review after first iteration
              }
              case  1004: {
                //IGHMC cancel Power loss recovery, see M1003
              }
              case 1011: {
                //set X2_max offset by translating M1011 to M218 (I hope) change s to 'axis', adding a Tool id
                TL_valuepar[TLM] = 218;
                TL_subcdpar[TLS] = 'X';
                TL_seenpar[TLT] = true;
                TL_subcdpar[TLT] = 'T';
                TL_valuepar[TLT] = 1;
                break;
              }
              case 1012: { //as above for y offset
                TL_valuepar[TLM] = 218;
                TL_subcdpar[TLS] = 'Y';
                TL_seenpar[TLT] = true;
                TL_subcdpar[TLT] = 'T';
                TL_valuepar[TLT] = 1;
                break;
              }
              case 1013: { //...and z
                TL_valuepar[TLM] = 218;
                TL_subcdpar[TLS] = 'Z';
                TL_seenpar[TLT] = true;
                TL_subcdpar[TLT] = 'T';
                TL_valuepar[TLT] = 1;
                break;
              }
              case 1014: {  //fan 2 speed, uses the 'm' place to put 'p' for port
                TL_valuepar[TLM] = 106; //big m for command
                TL_subcdpar[TLm] = 'P';
                TL_seenpar[TLm] = true;
                TL_valuepar[TLm] = 1;
                break;
              }
              case 1015: { //'m' for port and replace the S param with a T for temp
                TL_valuepar[TLM] = 106; //big m for command
                TL_subcdpar[TLm] = 'P';
                TL_seenpar[TLm] = true;
                TL_valuepar[TLm] = 1;
                TL_subcdpar[TLS] = 'T';
                break;
              }
              case 1016: {
                //auto off power loss by pin. no hardware support on this printer
                break;
              }
              case 1017: {  //screen sleep time, no marlin impact, commented out until display updates done
                break;
                // TL_SLEEP_TIME =TL_valuepar[TLS]
                // Config_StoreSettings();
              }
              case 1021: {            //preheat pla/abs/cooldown, Marlin does some trixy stuff with menus and presets but whatevs.
                TL_valuepar[TLM] = 104; // set extruder temp. IGHMC this is all a bit dodgy and needs to be re-written at some point
                TL_tempry = uint16_t(TL_valuepar[TLS]);
                switch (TL_tempry) {
                  case 0: {                  //ABS preheat
                    TL_seenpar[TLS] = false; //hide the S param
                    TL_seenpar[TLF] = true;  //new s param
                    TL_subcdpar[TLF] = 'S';
                    TL_valuepar[TLF] = PREHEAT_2_TEMP_HOTEND; // preset from config. TL screen has ABS first
                    TL_seenpar[TLn] = true;                   // give a carriage return
                    TL_seenpar[TLE] = true;                   //new M for bed heat
                    TL_subcdpar[TLE] = 'M';
                    TL_valuepar[TLE] = 140;
                    TL_seenpar[TLT] = true; //new M for bed heat
                    TL_subcdpar[TLT] = 'S';
                    TL_valuepar[TLT] = PREHEAT_2_TEMP_BED;
                    break;

                    // char ubl_lcd_gcode[16];
                    // const int ind = ubl_height_amount > 0 ? 9 : 10;
                    // strcpy_P(ubl_lcd_gcode, PSTR("G29 P6 C -"));
                    // sprintf_P(&ubl_lcd_gcode[ind], PSTR(".%i"), ABS(ubl_height_amount));
                    // queue.inject(ubl_lcd_gcode);
                  }
                  case 1: {                  //PLA preheat
                    TL_seenpar[TLS] = false; //hide the S param
                    TL_seenpar[TLF] = true;  //new s param
                    TL_subcdpar[TLF] = 'S';
                    TL_valuepar[TLF] = PREHEAT_1_TEMP_HOTEND; // preset from config
                    TL_seenpar[TLn] = true;                   // give a carriage return
                    TL_seenpar[TLE] = true;                   //new M for bed heat
                    TL_subcdpar[TLE] = 'M';
                    TL_valuepar[TLE] = 140;
                    TL_seenpar[TLT] = true; //new M for bed heat
                    TL_subcdpar[TLT] = 'S';
                    TL_valuepar[TLT] = PREHEAT_1_TEMP_BED;
                    break;
                  }
                  case 2: { //cooldown
                    thermalManager.disable_all_heaters();
                    // SERIAL_ECHOLNPAIR("called marlin to kill heaters ", TL_tempry);
                    ZERO(TL2_transcmd);  //clear any stragglers
                    TL_trimval = "";
                    for (uint8_t i = TLG; i <= TLm; i++) {
                      TL_seenpar[i] = false;
                      TL_valuepar[i] = 0.0;
                      TL_subcdpar[i] = ' ';
                    }

                    // #define USER_DESC_2 "Preheat for " PREHEAT_1_LABEL
                    // #define USER_GCODE_2 "M140 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND)

                    // #define USER_DESC_3 "Preheat for " PREHEAT_2_LABEL
                    // #define USER_GCODE_3 "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND)

                    // #define USER_DESC_4 "Heat Bed/Home/Level"
                    // #define USER_GCODE_4 "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nG28\nG29"
                    break;
                  }
                  case 3: { //cooldown
                    thermalManager.disable_all_heaters();
                    disable_all_steppers();
                    // SERIAL_ECHOLNPAIR("called marlin to kill heaters & steppers ", TL_tempry);
                    ZERO(TL2_transcmd); //clear any stragglers
                    TL_trimval = "";
                    for (uint8_t i = TLG; i <= TLm; i++) {
                      TL_seenpar[i] = false;
                      TL_valuepar[i] = 0.0;
                      TL_subcdpar[i] = ' ';
                    }

                    // #define USER_DESC_2 "Preheat for " PREHEAT_1_LABEL
                    // #define USER_GCODE_2 "M140 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND)

                    // #define USER_DESC_3 "Preheat for " PREHEAT_2_LABEL
                    // #define USER_GCODE_3 "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND)

                    // #define USER_DESC_4 "Heat Bed/Home/Level"
                    // #define USER_GCODE_4 "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nG28\nG29"
                    break;
                  }
                  default: {
                    // SERIAL_ECHOLNPAIR("unknown preheat ", TL_tempry);
                    break;
                  }
                }//TMPRY
                break;
              }//m1021
              case 1022: {
                //bool bChanged = false;
                int iTempE = active_extruder;
                //float fX = current_position[X_AXIS];
                int TValue = TL_valuepar[TLT];
                if (TValue != iTempE && TValue != -1) {
                  // command_T(TValue);
                  // SERIAL_ECHOLNPAIR("T ", TValue);
                  (TValue = 0) ? queue.enqueue_now_P(PSTR("T0")) : queue.enqueue_now_P(PSTR("T1"));
                  //queue.advance();
                  // SERIAL_ECHOPGM(" > ");
                  //bChanged = true;
                }
                SERIAL_ECHOLNPAIR("marling planner ", TL_valuepar[TLS]);
                if (TL_valuepar[TLS] == 0) {
                  current_position[E_AXIS] += 90;
                  Planner::buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 10, active_extruder); //20
                  current_position[E_AXIS] += 20;
                  Planner::buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2, active_extruder); //20
                } else if (TL_valuepar[TLS] == 1) {
                  current_position[E_AXIS] += 30;
                  Planner::buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2, active_extruder); //20
                  current_position[E_AXIS] -= 120;
                  Planner::buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 50, active_extruder); //20
                }
                ZERO(TL2_transcmd); //clear any stragglers
                TL_trimval = "";
                for (uint8_t i = TLG; i <= TLm; i++) {
                  TL_seenpar[i] = false;
                  TL_valuepar[i] = 0.0;
                  TL_subcdpar[i] = ' ';
                }
                break;
              }
              case 1031: { //pause
              TL_valuepar[TLM] = 25; //m25
              TL_SDPauseupdate();    //TL screen update for pause dialog
              break;
              }
              case 1032: { //resume
              TL_valuepar[TLM] = 24; //m24
              TL_seenpar[TLT] = 0; // TL does a lot of checking about temp/tool changes during pause
              //also has some new variables I and H which need to be investigated
              //for first iteration I'm assuming Marlin takes care of all that
              break;
              }
              case 1033: { //stop - marlin abort print
                TL_valuepar[TLM] = 524;
                break;
              }
              case 1040: { //set z height to resume from, leave to next iteration
                break;
              }
              default : {
                SERIAL_ECHOLNPAIR("unknown M ", TL_valuepar[TLM]);
              }
            }

            for (int i = TLG; i <= TLT; i++) {
              if (TL_seenpar[i]) {
                SERIAL_ECHOLN(String(i));
                // SERIAL_ECHOLNPAIR("cdpar: ", TL_subcdpar[i]);
                TL_trimval = TL_subcdpar[i];
                strcat(TL2_transcmd, TL_trimval.c_str());
                if ((i < TLX) || (i == TLT)) {
                  TL_trimval = ui16tostr4rj((uint16_t)(TL_valuepar[i]));
                  TL_trimval.trim();
                  strcat(TL2_transcmd, TL_trimval.c_str());
                } else if (i == TLn && TL_seenpar[TLn]) {
                  SERIAL_ECHOLN("-");
                  TL_trimval = "\n";
                  TL_done -= 1;
                  strcat(TL2_transcmd, TL_trimval.c_str());
                }else {
                  SERIAL_ECHO("+");
                  TL_trimval = ftostr42_52(TL_valuepar[i]);
                  TL_trimval.trim();
                  strcat(TL2_transcmd, TL_trimval.c_str());
                }
                if((int8_t)strlen(TL2_transcmd)>TL_done) {
                  strcat(TL2_transcmd, " ");
                  TL_done = strlen(TL2_transcmd);
                }
                SERIAL_ECHOLNPAIR("tl2 building main: ", TL2_transcmd);
              }
            }
            if (TL_seenpar[TLm]) {
              TL_trimval = TL_subcdpar[TLm];
              TL_trimval.trim();
              SERIAL_ECHOLNPAIR("true tlm? :", TL_trimval.c_str());
              strcat(TL2_transcmd, TL_trimval.c_str());
              TL_trimval = ui16tostr4rj((uint16_t)(TL_valuepar[TLm]));
              TL_trimval.trim();
              strcat(TL2_transcmd, TL_trimval.c_str());
            }
            SERIAL_ECHOLNPAIR("tl2 len: ", strlen(TL2_transcmd));
            SERIAL_ECHOLNPAIR("before / n: ", TL2_transcmd);
            TL_trimval = "\n";
            strcat(TL2_transcmd, TL_trimval.c_str());
            //ighmc got a complete line to process
            SERIAL_ECHOLNPAIR("tl2 len: ", strlen(TL2_transcmd));
            SERIAL_ECHOLNPAIR("inject me : ", TL2_transcmd);
            queue.inject(&TL2_transcmd[0]);
            //queue.advance();
          } // end of M processing
          // SERIAL_ECHOLNPAIR("END OF M    ", &TL_parsub);
        } else {
          SERIAL_ECHOLN("lost in translation no g m or t");
          break;
        } //end of single command
        ZERO(TL2_transcmd);
        TL_trimval = "";
        for (uint8_t i = TLG; i <= TLm; i++) {
          TL_seenpar[i] = false;
          TL_valuepar[i] = 0.0;
          TL_subcdpar[i] = ' ';
        }
        // SERIAL_ECHOLNPAIR("TL2      ", TL2_transcmd);
      } // ighmc TLsub param loop
      // SERIAL_ECHOLNPAIR("NEXT CMD   ", TL_buffer);
    } //ighmc serialdata loop
  }   //tl get command

    // IGHMC these, presumably, are used to manage Marlin menus but we can't
    // access Tenlog menus so ignore incoming interactions and just update Tenlog screen
    void onIdle() {
      TL_status_timer();
      ExtUI::TL_get_command_1();
    }

    void onPrinterKilled(PGM_P const error, PGM_P const component) {}

    void onMediaInserted() {}

    void onMediaError() {}

    void onMediaRemoved() {}

    // void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
    // //IGHMC no buzzer on Tenlog

    void onPrintTimerPaused() {}

    void onPrintTimerStopped() {}

    // void onFilamentRunout(const extruder_t extruder) {} //IGHMC no buzzer on
    // Tenlog
    void onStatusChanged(const char *const msg) {}

    void onFactoryReset() {}

    void onUserConfirmRequired(const char *const msg) {
      TenlogScreen_println("sleep=0");
      TenlogScreen_print("main.vLanguageID.val=0");
      TenlogScreen_println("msgbox.vaFromPageID.val=1");
      TenlogScreen_println("msgbox.vaToPageID.val=1");
      TenlogScreen_println(
          "msgbox.vtOKValue.txt=\"\""); // ighmc  -  need  to  tell  screen what
                                        // to  do  when  ok  is  pressed

      String strMessage = "msgbox.tMessage.txt=\"\"";

      char encoded_message[MAX_MESSAGE_LENGTH];
      uint8_t message_length = _MIN(strlen_P(msg), sizeof(encoded_message));

      LOOP_L_N(i, message_length)
      encoded_message[i] = pgm_read_byte(&msg[i]) | 0x80;

      LOOP_L_N(i, message_length)
      strMessage += encoded_message[i];

      strMessage += "\"";
      const char *str0 = strMessage.c_str();
      TenlogScreen_println(str0);
      TenlogScreen_println("page msgbox");
    }

    void onStoreSettings(char *buff) {
      // Called when saving to EEPROM (i.e. M500). If the ExtUI needs
      // permanent data to be stored, it can write up to eeprom_data_size bytes
      // into buff.

      // Example:
      //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
      //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
    }

    void onLoadSettings(const char *buff) {
      // Called while loading settings from EEPROM. If the ExtUI
      // needs to retrieve data, it should copy up to eeprom_data_size bytes
      // from buff

      // Example:
      //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
      //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
    }

    void onConfigurationStoreWritten(bool success) {
      // Called after the entire EEPROM has been written,
      // whether successful or not.
    }

    void onConfigurationStoreRead(bool success) {
      // Called after the entire EEPROM has been read,
      // whether successful or not.
    }

    // IGHMC, no bed levelling or filament runout
    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      // Called when any mesh points are updated
    }
    //void onFilamentRunout(const extruder_t extruder){}
#if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
    }
#endif

#if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
    }
#endif

    //IGHMC no kill screen useful without power control
    // void kill_screen(PGM_P lcd_error, PGM_P lcd_component) {
    //     init();
    //     ui.status_printf_P(1, PSTR(S_FMT ": " S_FMT), lcd_error, lcd_component);
    //     // RED ALERT. RED ALERT.
    //     // ui.draw_kill_screen();          //IGHMC find the tenlog kill screen
    // }

    void abort_print() {
      wait_for_heatup = wait_for_user = false;
      card.flag.abort_sd_printing = true;
#ifdef ACTION_ON_CANCEL
    host_action_cancel();
#endif
#if ENABLED(HOST_PROMPT_SUPPORT)
    host_prompt_open(PROMPT_INFO, PSTR("UI Aborted"), DISMISS_STR);
#endif
    print_job_timer.stop();
    ui.set_status_P(GET_TEXT(MSG_PRINT_ABORTED));
#if HAS_LCD_MENU
    return_to_status();
#endif
  }

    void TL_printdone(const char *message) {
    TenlogScreen_println("sleep=0");
    TenlogScreen_println("msgbox.vaFromPageID.val=1");
    TenlogScreen_println("msgbox.vaToPageID.val=1");
    TenlogScreen_println("msgbox.vtOKValue.txt=\"\"");
    String strMessage = "";
    bool bAutoOff = false;
    if (languageID == 0) {
      strMessage = "Print finished, " + String(message) + "\r\n";
    } else {
      strMessage = "��ӡ��ɣ�������" + String(message) + "�֡�";
#ifdef POWER_LOSS_TRIGGER_BY_PIN
    if (zyf_AUTO_OFF == 1) {
      if (languageID == 0)
        strMessage = strMessage + "Power off in 10 seconds.";
      else
        strMessage = strMessage + "10���ػ�";
      bAutoOff = true;
    }
#endif
    }
    strMessage = "msgbox.tMessage.txt=\"" + strMessage + "\"";
    const char *str0 = strMessage.c_str();
    TenlogScreen_println(str0);
    TenlogScreen_println("page msgbox");
  }

} // namespace ExtUI
