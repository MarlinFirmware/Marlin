/**
 * anycubic_i3mega_lcd.cpp  --- Support for Anycubic i3 Mega TFT
 * Created by Christian Hopp on 09.12.17.
 * Improved by David Ramiro
 * Converted to ext_iu by John BouAntoun 21 June 2020
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_I3MEGA)

#include "anycubic_i3mega_lcd.h"

#include "../../../../inc/MarlinConfig.h"
#include "../../ui_api.h"
#include "../../../../MarlinCore.h"     // for quickstop_stepper and disable_steppers
#include "../../../../module/motion.h"  // for A20 read printing speed feedrate_percentage

// command sending macro's with debugging capability
#define SEND_PGM(x)                                 send_P(PSTR(x))
#define SENDLINE_PGM(x)                             sendLine_P(PSTR(x))
#define SEND_PGM_VAL(x,y)                           (send_P(PSTR(x)), sendLine(itostr3(y)))
#define SEND(x)                                     send(x)
#define SENDLINE(x)                                 sendLine(x)
#if ENABLED(ANYCUBIC_LCD_DEBUG)
  #define SENDLINE_DBG_PGM(x,y)                     (sendLine_P(PSTR(x)), SERIAL_ECHOLNPGM(y))
  #define SENDLINE_DBG_PGM_VAL(x,y,z)               (sendLine_P(PSTR(x)), SERIAL_ECHOPGM(y), SERIAL_ECHOLN(z))
#else
  #define SENDLINE_DBG_PGM(x,y)                     sendLine_P(PSTR(x))
  #define SENDLINE_DBG_PGM_VAL(x,y,z)               sendLine_P(PSTR(x))
#endif


AnycubicTFTClass AnycubicTFT;

char _conv[8];

char *itostr2(const uint8_t &x) {
  // sprintf(conv,"%5.1f",x);
  int xx = x;
  _conv[0] = (xx / 10) % 10 + '0';
  _conv[1] = (xx) % 10 + '0';
  _conv[2] = 0;
  return _conv;
}

static void sendNewLine(void) {
  LCD_SERIAL.write('\r');
  LCD_SERIAL.write('\n');
}

static void send(const char *str) {
  LCD_SERIAL.print(str);
}

static void sendLine(const char *str) {
  send(str);
  sendNewLine();
}

static void send_P(PGM_P str) {
  while (const char c = pgm_read_byte(str++))
    LCD_SERIAL.write(c);
}

static void sendLine_P(PGM_P str) {
  send_P(str);
  sendNewLine();
}

#ifndef ULTRA_LCD
  #define DIGIT(n) ('0' + (n))
  #define DIGIMOD(n, f) DIGIT((n) / (f) % 10)
  #define RJDIGIT(n, f) ((n) >= (f) ? DIGIMOD(n, f) : ' ')
  #define MINUSOR(n, alt) (n >= 0 ? (alt) : (n = -n, '-'))

  char* itostr3(const int x) {
    int xx = x;
    _conv[4] = MINUSOR(xx, RJDIGIT(xx, 100));
    _conv[5] = RJDIGIT(xx, 10);
    _conv[6] = DIGIMOD(xx, 1);
    return &_conv[4];
  }

// Convert signed float to fixed-length string with 023.45 / -23.45 format
  char *ftostr32(const float &x) {
    long xx = x * 100;
    _conv[1] = MINUSOR(xx, DIGIMOD(xx, 10000));
    _conv[2] = DIGIMOD(xx, 1000);
    _conv[3] = DIGIMOD(xx, 100);
    _conv[4] = '.';
    _conv[5] = DIGIMOD(xx, 10);
    _conv[6] = DIGIMOD(xx, 1);
    return &_conv[1];
  }

#endif

AnycubicTFTClass::AnycubicTFTClass() {}

void AnycubicTFTClass::OnSetup() {
  #ifndef LCD_BAUDRATE
    #define LCD_BAUDRATE 115200
  #endif
  LCD_SERIAL.begin(LCD_BAUDRATE);

  SENDLINE_DBG_PGM("J17", "TFT Serial Debug: Main board reset... J17"); // J17 Main board reset
  ExtUI::delay_ms(10);

  // initialise the state of the key pins running on the tft
  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    SET_INPUT_PULLUP(FIL_RUNOUT_PIN);
  #endif

  mediaPrintingState = AMPRINTSTATE_NOT_PRINTING;
  mediaPauseState = AMPAUSESTATE_NOT_PAUSED;

  // DoSDCardStateCheck();
  SENDLINE_DBG_PGM("J12", "TFT Serial Debug: Ready... J12"); // J12 Ready
  ExtUI::delay_ms(10);

  DoFilamentRunoutCheck();
  SelectedFile[0] = 0;

  #if ENABLED(STARTUP_CHIME)
    ExtUI::injectCommands_P(PSTR("M300 P250 S554\nM300 P250 S554\nM300 P250 S740\nM300 P250 S554\nM300 P250 S740\nM300 P250 S554\nM300 P500 S831"));
  #endif
  #if ENABLED(ANYCUBIC_LCD_DEBUG)
    SERIAL_ECHOLNPGM("TFT Serial Debug: Finished startup");
  #endif
}

void AnycubicTFTClass::OnCommandScan() {
  static millis_t nextStopCheck = 0; // used to slow the stopped print check down to reasonable times
  const millis_t ms = millis();
  if (ELAPSED(ms, nextStopCheck)) {
    nextStopCheck = ms + 1000UL;
    if (mediaPrintingState == AMPRINTSTATE_STOP_REQUESTED && IsNozzleHomed()) {
      #if ENABLED(ANYCUBIC_LCD_DEBUG)
        SERIAL_ECHOLNPGM("TFT Serial Debug: Finished stopping print, releasing motors ...");
      #endif
      mediaPrintingState = AMPRINTSTATE_NOT_PRINTING;
      mediaPauseState = AMPAUSESTATE_NOT_PAUSED;
      ExtUI::injectCommands_P(PSTR("M84\nM27")); // disable stepper motors and force report of SD status
      ExtUI::delay_ms(200);
      // tell printer to release resources of print to indicate it is done
      SENDLINE_DBG_PGM("J14", "TFT Serial Debug: SD Print Stopped... J14");
    }
  }

  if (TFTbuflen < (TFTBUFSIZE - 1))
    GetCommandFromTFT();

  if (TFTbuflen) {
    TFTbuflen  = (TFTbuflen - 1);
    TFTbufindr = (TFTbufindr + 1) % TFTBUFSIZE;
  }
}

void AnycubicTFTClass::OnKillTFT() {
  SENDLINE_DBG_PGM("J11", "TFT Serial Debug: Kill command... J11");
}

void AnycubicTFTClass::OnSDCardStateChange(bool isInserted) {
  #if ENABLED(ANYCUBIC_LCD_DEBUG)
    SERIAL_ECHOPGM("TFT Serial Debug: OnSDCardStateChange event triggered...");
    SERIAL_ECHO(itostr2(isInserted));
    SERIAL_EOL();
  #endif
  DoSDCardStateCheck();
}

void AnycubicTFTClass::OnSDCardError() {
  #if ENABLED(ANYCUBIC_LCD_DEBUG)
    SERIAL_ECHOLNPGM("TFT Serial Debug: OnSDCardError event triggered...");
  #endif
  SENDLINE_DBG_PGM("J21", "TFT Serial Debug: On SD Card Error ... J21");
}

void AnycubicTFTClass::OnFilamentRunout() {
  #if ENABLED(ANYCUBIC_LCD_DEBUG)
    SERIAL_ECHOLNPGM("TFT Serial Debug: FilamentRunout triggered...");
  #endif
  DoFilamentRunoutCheck();
}

void AnycubicTFTClass::OnUserConfirmRequired(const char * const msg) {
  #if ENABLED(ANYCUBIC_LCD_DEBUG)
    SERIAL_ECHOPGM("TFT Serial Debug: OnUserConfirmRequired triggered... ");
    SERIAL_ECHOLN(msg);
  #endif

  #if ENABLED(SDSUPPORT)
    /**
     * Need to handle the process of following states
     * "Nozzle Parked"
     * "Load Filament"
     * "Filament Purging..."
     * "HeaterTimeout"
     * "Reheat finished."
     *
     * NOTE:  The only way to handle these states is strcmp_P with the msg unfortunately (very expensive)
     */
    if (strcmp_P(msg, PSTR("Nozzle Parked")) == 0) {
      mediaPrintingState = AMPRINTSTATE_PAUSED;
      mediaPauseState    = AMPAUSESTATE_PARKED;
      // enable continue button
      SENDLINE_DBG_PGM("J18", "TFT Serial Debug: UserConfirm SD print paused done... J18");
    }
    else if (strcmp_P(msg, PSTR("Load Filament")) == 0) {
      mediaPrintingState = AMPRINTSTATE_PAUSED;
      mediaPauseState    = AMPAUSESTATE_FILAMENT_OUT;
      // enable continue button
      SENDLINE_DBG_PGM("J18", "TFT Serial Debug: UserConfirm Filament is out... J18");
      SENDLINE_DBG_PGM("J23", "TFT Serial Debug: UserConfirm Blocking filament prompt... J23");
    }
    else if (strcmp_P(msg, PSTR("Filament Purging...")) == 0) {
      mediaPrintingState = AMPRINTSTATE_PAUSED;
      mediaPauseState    = AMPAUSESTATE_PARKING;
      // TODO: JBA I don't think J05 just disables the continue button, i think it injects a rogue M25. So taking this out
      // disable continue button
      // SENDLINE_DBG_PGM("J05", "TFT Serial Debug: UserConfirm SD Filament Purging... J05"); // J05 printing pause

      // enable continue button
      SENDLINE_DBG_PGM("J18", "TFT Serial Debug: UserConfirm Filament is purging... J18");
    }
    else if (strcmp_P(msg, PSTR("HeaterTimeout")) == 0) {
      mediaPrintingState = AMPRINTSTATE_PAUSED;
      mediaPauseState    = AMPAUSESTATE_HEATER_TIMEOUT;
      // enable continue button
      SENDLINE_DBG_PGM("J18", "TFT Serial Debug: UserConfirm SD Heater timeout... J18");
    }
    else if (strcmp_P(msg, PSTR("Reheat finished.")) == 0) {
      mediaPrintingState = AMPRINTSTATE_PAUSED;
      mediaPauseState    = AMPAUSESTATE_REHEAT_FINISHED;
      // enable continue button
      SENDLINE_DBG_PGM("J18", "TFT Serial Debug: UserConfirm SD Reheat done... J18");
    }
  #endif
}

float AnycubicTFTClass::CodeValue() {
  return (strtod(&TFTcmdbuffer[TFTbufindr][TFTstrchr_pointer - TFTcmdbuffer[TFTbufindr] + 1], NULL));
}

bool AnycubicTFTClass::CodeSeen(char code) {
  TFTstrchr_pointer = strchr(TFTcmdbuffer[TFTbufindr], code);
  return (TFTstrchr_pointer != NULL); // Return True if a character was found
}

bool AnycubicTFTClass::IsNozzleHomed() {
  const float xPosition = ExtUI::getAxisPosition_mm((ExtUI::axis_t) ExtUI::X);
  const float yPosition = ExtUI::getAxisPosition_mm((ExtUI::axis_t) ExtUI::Y);
  return WITHIN(xPosition, X_MIN_POS - 0.1, X_MIN_POS + 0.1) &&
         WITHIN(yPosition, Y_MIN_POS - 0.1, Y_MIN_POS + 0.1);
}

void AnycubicTFTClass::HandleSpecialMenu() {
  /**
   * NOTE: that the file selection command actual lowercases the entire selected file/foldername, so charracter comparisons need to be lowercase.
   */
  if (SelectedDirectory[0] == '<') {
    switch (SelectedDirectory[1]) {
      case 'e': // "<exit>"
        SpecialMenu = false;
        return;
        break;

        #if ENABLED(PROBE_MANUALLY)
          case '0':
            switch (SelectedDirectory[2]) {
              case '1': // "<01ZUp0.1>"
                SERIAL_ECHOLNPGM("Special Menu: Z Up 0.1");
                ExtUI::injectCommands_P(PSTR("G91\nG1 Z+0.1\nG90"));
                break;

              case '2': // "<02ZUp0.02>"
                SERIAL_ECHOLNPGM("Special Menu: Z Up 0.02");
                ExtUI::injectCommands_P(PSTR("G91\nG1 Z+0.02\nG90"));
                break;

              case '3': // "<03ZDn0.02>"
                SERIAL_ECHOLNPGM("Special Menu: Z Down 0.02");
                ExtUI::injectCommands_P(PSTR("G91\nG1 Z-0.02\nG90"));
                break;

              case '4': // "<04ZDn0.1>"
                SERIAL_ECHOLNPGM("Special Menu: Z Down 0.1");
                ExtUI::injectCommands_P(PSTR("G91\nG1 Z-0.1\nG90"));
                break;

              case '5': // "<05PrehtBed>"
                SERIAL_ECHOLNPGM("Special Menu: Preheat Bed");
                ExtUI::injectCommands_P(PSTR("M140 S65"));
                break;

              case '6': // "<06SMeshLvl>"
                SERIAL_ECHOLNPGM("Special Menu: Start Mesh Leveling");
                ExtUI::injectCommands_P(PSTR("G29 S1"));
                break;

              case '7': // "<07MeshNPnt>"
                SERIAL_ECHOLNPGM("Special Menu: Next Mesh Point");
                ExtUI::injectCommands_P(PSTR("G29 S2"));
                break;

              case '8': // "<08HtEndPID>"
                SERIAL_ECHOLNPGM("Special Menu: Auto Tune Hotend PID");
                // need to dwell for half a second to give the fan a chance to start before the pid tuning starts
                ExtUI::injectCommands_P(PSTR("M106 S204\nG4 P500\nM303 E0 S215 C15 U1"));
                break;

              case '9': // "<09HtBedPID>"
                SERIAL_ECHOLNPGM("Special Menu: Auto Tune Hotbed Pid");
                ExtUI::injectCommands_P(PSTR("M303 E-1 S65 C6 U1"));
                break;

              default:
                break;
            }
            break;

          case '1':
            switch (SelectedDirectory[2]) {
              case '0': // "<10FWDeflts>"
                SERIAL_ECHOLNPGM("Special Menu: Load FW Defaults");
                ExtUI::injectCommands_P(PSTR("M502\nM300 P105 S1661\nM300 P210 S1108"));
                break;

              case '1': // "<11SvEEPROM>"
                SERIAL_ECHOLNPGM("Special Menu: Save EEPROM");
                ExtUI::injectCommands_P(PSTR("M500\nM300 P105 S1108\nM300 P210 S1661"));
                break;

              default:
                break;
            }
            break;
        #else // if ENABLED(PROBE_MANUALLY)
          case '0':
            switch (SelectedDirectory[2]) {
              case '1': // "<01PrehtBed>"
                SERIAL_ECHOLNPGM("Special Menu: Preheat Bed");
                ExtUI::injectCommands_P(PSTR("M140 S65"));
                break;

              case '2': // "<02ABL>"
                SERIAL_ECHOLNPGM("Special Menu: Auto Bed Leveling");
                ExtUI::injectCommands_P(PSTR("G28\nG29"));
                break;

              case '3': // "<03HtendPID>"
                SERIAL_ECHOLNPGM("Special Menu: Auto Tune Hotend PID");
                // need to dwell for half a second to give the fan a chance to start before the pid tuning starts
                ExtUI::injectCommands_P(PSTR("M106 S204\nG4 P500\nM303 E0 S215 C15 U1"));
                break;

              case '4': // "<04HtbedPID>"
                SERIAL_ECHOLNPGM("Special Menu: Auto Tune Hotbed Pid");
                ExtUI::injectCommands_P(PSTR("M303 E-1 S65 C6 U1"));
                break;

              case '5': // "<05FWDeflts>"
                SERIAL_ECHOLNPGM("Special Menu: Load FW Defaults");
                ExtUI::injectCommands_P(PSTR("M502\nM300 P105 S1661\nM300 P210 S1108"));
                break;

              case '6': // "<06SvEEPROM>"
                SERIAL_ECHOLNPGM("Special Menu: Save EEPROM");
                ExtUI::injectCommands_P(PSTR("M500\nM300 P105 S1108\nM300 P210 S1661"));
                break;

              case '7': // <07SendM108>
                SERIAL_ECHOLNPGM("Special Menu: Send User Confirmation");
                ExtUI::injectCommands_P(PSTR("M108"));
                break;

              default:
                break;
            }
            break;
            #endif  // PROBE_MANUALLY

          default:
            break;
    }
    #if ENABLED(ANYCUBIC_LCD_DEBUG)
  }
  else {
    SERIAL_ECHOPGM("TFT Serial Debug: Attempted to HandleSpecialMenu on non-special menu... ");
    SERIAL_ECHOLN(SelectedDirectory);
    #endif
  }
}

void AnycubicTFTClass::RenderCurrentFileList() {
  #if ENABLED(SDSUPPORT)
    uint16_t selectedNumber = 0;
    SelectedDirectory[0] = 0;
    SelectedFile[0] = 0;
    ExtUI::FileList currentFileList;

    SENDLINE_PGM("FN "); // Filelist start

    if (!ExtUI::isMediaInserted() && !SpecialMenu) {
      SENDLINE_DBG_PGM("J02", "TFT Serial Debug: No SD Card mounted to render Current File List... J02");

      SENDLINE_PGM("<Special_Menu>");
      SENDLINE_PGM("<Special_Menu>");
    }
    else {
      if (CodeSeen('S'))
        selectedNumber = CodeValue();

      if (SpecialMenu)
        RenderSpecialMenu(selectedNumber);
      else if (selectedNumber <= currentFileList.count())
        RenderCurrentFolder(selectedNumber);
    }
    SENDLINE_PGM("END"); // Filelist stop
  #endif // SDSUPPORT
}

void AnycubicTFTClass::RenderSpecialMenu(uint16_t selectedNumber) {
  switch (selectedNumber) {
    #if ENABLED(PROBE_MANUALLY)
      case 0: // First Page
        SENDLINE_PGM("<01ZUp0.1>");
        SENDLINE_PGM("<Z Up 0.1>");
        SENDLINE_PGM("<02ZUp0.02>");
        SENDLINE_PGM("<Z Up 0.02>");
        SENDLINE_PGM("<03ZDn0.02>");
        SENDLINE_PGM("<Z Down 0.02>");
        SENDLINE_PGM("<04ZDn0.1>");
        SENDLINE_PGM("<Z Down 0.1>");
        break;

      case 4: // Second Page
        SENDLINE_PGM("<05PrehtBed>");
        SENDLINE_PGM("<Preheat bed>");
        SENDLINE_PGM("<06SMeshLvl>");
        SENDLINE_PGM("<Start Mesh Leveling>");
        SENDLINE_PGM("<07MeshNPnt>");
        SENDLINE_PGM("<Next Mesh Point>");
        SENDLINE_PGM("<08HtEndPID>");
        SENDLINE_PGM("<Auto Tune Hotend PID>");
        break;

      case 8: // Third Page
        SENDLINE_PGM("<09HtBedPID>");
        SENDLINE_PGM("<Auto Tune Hotbed PID>");
        SENDLINE_PGM("<10FWDeflts>");
        SENDLINE_PGM("<Load FW Defaults>");
        SENDLINE_PGM("<11SvEEPROM>");
        SENDLINE_PGM("<Save EEPROM>");
        SENDLINE_PGM("<Exit>");
        SENDLINE_PGM("<Exit>");
        break;
    #else
      case 0: // First Page
        SENDLINE_PGM("<01PrehtBed>");
        SENDLINE_PGM("<Preheat bed>");
        SENDLINE_PGM("<02ABL>");
        SENDLINE_PGM("<Auto Bed Leveling>");
        SENDLINE_PGM("<03HtEndPID>");
        SENDLINE_PGM("<Auto Tune Hotend PID>");
        SENDLINE_PGM("<04HtBedPID>");
        SENDLINE_PGM("<Auto Tune Hotbed PID>");
        break;

      case 4: // Second Page
        SENDLINE_PGM("<05FWDeflts>");
        SENDLINE_PGM("<Load FW Defaults>");
        SENDLINE_PGM("<06SvEEPROM>");
        SENDLINE_PGM("<Save EEPROM>");
        SENDLINE_PGM("<07SendM108>");
        SENDLINE_PGM("<Send User Confirmation>");
        SENDLINE_PGM("<Exit>");
        SENDLINE_PGM("<Exit>");
        break;

        #endif // PROBE_MANUALLY

      default:
        break;
  }
}

void AnycubicTFTClass::RenderCurrentFolder(uint16_t selectedNumber) {
  ExtUI::FileList currentFileList;
  uint16_t cnt = selectedNumber;
  uint16_t max_files;
  uint16_t dir_files = currentFileList.count();

  if ((dir_files - selectedNumber) < 4)
    max_files = dir_files;
  else
    max_files = selectedNumber + 3;

  for (cnt = selectedNumber; cnt <= max_files; cnt++) {
    if (cnt == 0) { // Special Entry
      if (currentFileList.isAtRootDir()) {
        SENDLINE_PGM("<specialmnu>");
        SENDLINE_PGM("<Special Menu>");
      }
      else {
        SENDLINE_PGM("/..");
        SENDLINE_PGM("/..");
      }
    }
    else {
      currentFileList.seek(cnt - 1, false);

      #if ENABLED(ANYCUBIC_LCD_DEBUG)
        SERIAL_ECHOLN(currentFileList.filename());
      #endif
      if (currentFileList.isDir()) {
        SEND_PGM("/");
        SENDLINE(currentFileList.shortFilename());
        SEND_PGM("/");
        SENDLINE(currentFileList.longFilename());

      }
      else {
        SENDLINE(currentFileList.shortFilename());
        SENDLINE(currentFileList.longFilename());
      }
    }
  }
}

void AnycubicTFTClass::OnPrintTimerStarted() {
  #if ENABLED(SDSUPPORT)
    if (mediaPrintingState == AMPRINTSTATE_PRINTING)
      SENDLINE_DBG_PGM("J04", "TFT Serial Debug: Starting SD Print... J04"); // J04 Starting Print

  #endif
}

void AnycubicTFTClass::OnPrintTimerPaused() {
  #if ENABLED(SDSUPPORT)
    if (ExtUI::isPrintingFromMedia()) {
      mediaPrintingState = AMPRINTSTATE_PAUSED;
      mediaPauseState    = AMPAUSESTATE_PARKING;
    }
  #endif
}

void AnycubicTFTClass::OnPrintTimerStopped() {
  #if ENABLED(SDSUPPORT)
    if (mediaPrintingState == AMPRINTSTATE_PRINTING) {
      mediaPrintingState = AMPRINTSTATE_NOT_PRINTING;
      mediaPauseState    = AMPAUSESTATE_NOT_PAUSED;
      SENDLINE_DBG_PGM("J14", "TFT Serial Debug: SD Print Completed... J14");
    }
    // otherwise it was stopped by the printer so don't send print completed signal to TFT
  #endif
}

void AnycubicTFTClass::GetCommandFromTFT() {
  char *starpos = NULL;
  while (LCD_SERIAL.available() > 0  && TFTbuflen < TFTBUFSIZE) {
    serial3_char = LCD_SERIAL.read();
    if (serial3_char == '\n' ||
        serial3_char == '\r' ||
        serial3_char == ':'  ||
        serial3_count >= (TFT_MAX_CMD_SIZE - 1)
    ) {

      if (!serial3_count) return; // if empty line

      TFTcmdbuffer[TFTbufindw][serial3_count] = 0; // terminate string

      if ((strchr(TFTcmdbuffer[TFTbufindw], 'A') != NULL)) {
        int16_t a_command;
        TFTstrchr_pointer = strchr(TFTcmdbuffer[TFTbufindw], 'A');
        a_command = ((int)((strtod(&TFTcmdbuffer[TFTbufindw][TFTstrchr_pointer - TFTcmdbuffer[TFTbufindw] + 1], NULL))));

        #if ENABLED(ANYCUBIC_LCD_DEBUG)
          if ((a_command > 7) && (a_command != 20)) { // No debugging of status polls, please!
            SERIAL_ECHOPGM("TFT Serial Command: ");
            SERIAL_ECHOLN(TFTcmdbuffer[TFTbufindw]);
          }
        #endif

        switch (a_command) {
          case 0: { // A0 GET HOTEND TEMP
            float hotendActualTemp = ExtUI::getActualTemp_celsius((ExtUI::extruder_t) (ExtUI::extruder_t) ExtUI::E0);
            SEND_PGM_VAL("A0V ", int(hotendActualTemp + 0.5));
          }
          break;

          case 1: { // A1  GET HOTEND TARGET TEMP
            float hotendTargetTemp = ExtUI::getTargetTemp_celsius((ExtUI::extruder_t) (ExtUI::extruder_t) ExtUI::E0);
            SEND_PGM_VAL("A1V ", int(hotendTargetTemp + 0.5));
          }
          break;

          case 2: { // A2 GET HOTBED TEMP
            float heatedBedActualTemp = ExtUI::getActualTemp_celsius((ExtUI::heater_t) ExtUI::BED);
            SEND_PGM_VAL("A2V ", int(heatedBedActualTemp + 0.5));
          }
          break;

          case 3: { // A3 GET HOTBED TARGET TEMP
            float heatedBedTargetTemp = ExtUI::getTargetTemp_celsius((ExtUI::heater_t) ExtUI::BED);
            SEND_PGM_VAL("A3V ", int(heatedBedTargetTemp + 0.5));
          }
          break;

          case 4: // A4 GET FAN SPEED
          {
            float fanPercent = ExtUI::getActualFan_percent(ExtUI::FAN0);
            fanPercent = constrain(fanPercent, 0, 100);
            SEND_PGM_VAL("A4V ", int(fanPercent));
          }
          break;

          case 5: // A5 GET CURRENT COORDINATE
          {
            float xPostition = ExtUI::getAxisPosition_mm(ExtUI::X);
            float yPostition = ExtUI::getAxisPosition_mm(ExtUI::Y);
            float zPostition = ExtUI::getAxisPosition_mm(ExtUI::Z);
            SEND_PGM("A5V X: ");
            LCD_SERIAL.print(xPostition);
            SEND_PGM(" Y: ");
            LCD_SERIAL.print(yPostition);
            SEND_PGM(" Z: ");
            LCD_SERIAL.print(zPostition);
            SENDLINE_PGM("");
          }
          break;

          case 6: // A6 GET SD CARD PRINTING STATUS
            #if ENABLED(SDSUPPORT)
              if (ExtUI::isPrintingFromMedia()) {
                SEND_PGM("A6V ");
                if (ExtUI::isMediaInserted()) {
                  SENDLINE(itostr3(int(ExtUI::getProgress_percent())));
                }
                else {
                  SENDLINE_DBG_PGM("J02", "TFT Serial Debug: No SD Card mounted to return printing status... J02");
                }
              }
              else {
                SENDLINE_PGM("A6V ---");
              }
            #endif
            break;

          case 7: { // A7 GET PRINTING TIME
            uint32_t elapsedSeconds = ExtUI::getProgress_seconds_elapsed();
            SEND_PGM("A7V ");
            if (elapsedSeconds != 0) {  // print time
              uint32_t elapsedMinutes = elapsedSeconds / 60;
              SEND(itostr2(elapsedMinutes / 60));
              SEND_PGM(" H ");
              SEND(itostr2(elapsedMinutes % 60));
              SENDLINE_PGM(" M");
            }
            else {
              SENDLINE_PGM(" 999:999");
            }
          }
          break;

          case 8: // A8 GET  SD LIST
            #if ENABLED(SDSUPPORT)
              SelectedFile[0] = 0;
              RenderCurrentFileList();
            #endif
            break;

          case 9: // A9 pause sd print
            #if ENABLED(SDSUPPORT)
              if (ExtUI::isPrintingFromMedia())
                PausePrint();

            #endif
            break;

          case 10: // A10 resume sd print
            #if ENABLED(SDSUPPORT)
              if (ExtUI::isPrintingFromMediaPaused())
                ResumePrint();

            #endif
            break;

          case 11: // A11 STOP SD PRINT
            #if ENABLED(SDSUPPORT)
              StopPrint();
            #endif
            break;

          case 12: // A12 kill
            kill(PSTR(STR_ERR_KILLED));
            break;

          case 13: // A13 SELECTION FILE
            #if ENABLED(SDSUPPORT)
              if (ExtUI::isMediaInserted()) {
                starpos = (strchr(TFTstrchr_pointer + 4, '*'));
                if (TFTstrchr_pointer[4] == '/') {
                  strcpy(SelectedDirectory, TFTstrchr_pointer + 5);
                  SelectedFile[0] = 0;
                  SENDLINE_DBG_PGM("J21", "TFT Serial Debug: Clear file selection... J21 "); // J21 Not File Selected
                  SENDLINE_PGM("");
                }
                else if (TFTstrchr_pointer[4] == '<') {
                  strcpy(SelectedDirectory, TFTstrchr_pointer + 4);
                  SpecialMenu = true;
                  SelectedFile[0] = 0;
                  SENDLINE_DBG_PGM("J21", "TFT Serial Debug: Clear file selection... J21 "); // J21 Not File Selected
                  SENDLINE_PGM("");
                }
                else {
                  SelectedDirectory[0] = 0;

                  if (starpos != NULL)
                    *(starpos - 1) = '\0';

                  strcpy(SelectedFile, TFTstrchr_pointer + 4);
                  SENDLINE_DBG_PGM_VAL("J20", "TFT Serial Debug: File Selected... J20 ", SelectedFile); // J20 File Selected
                }
              }
            #endif
            break;

          case 14: // A14 START PRINTING
            #if ENABLED(SDSUPPORT)
              if (!ExtUI::isPrinting() && strlen(SelectedFile) > 0)
                StartPrint();

            #endif
            break;

          case 15: // A15 RESUMING FROM OUTAGE
            // TODO: JBA implement resume form outage
            break;

          case 16: { // A16 set hotend temp
            unsigned int tempvalue;
            if (CodeSeen('S')) {
              tempvalue = constrain(CodeValue(), 0, 275);
              ExtUI::setTargetTemp_celsius(tempvalue, (ExtUI::extruder_t) ExtUI::E0);
            }
            else if (CodeSeen('C') && !ExtUI::isPrinting()) {
              if (ExtUI::getAxisPosition_mm(ExtUI::Z) < 10)
                ExtUI::injectCommands_P(PSTR("G1 Z10")); // RASE Z AXIS
              tempvalue = constrain(CodeValue(), 0, 275);
              ExtUI::setTargetTemp_celsius(tempvalue, (ExtUI::extruder_t) ExtUI::E0);
            }
          }
          break;

          case 17:// A17 set heated bed temp
          {
            unsigned int tempbed;
            if (CodeSeen('S')) {
              tempbed = constrain(CodeValue(), 0, 100);
              ExtUI::setTargetTemp_celsius(tempbed, (ExtUI::heater_t)ExtUI::BED);
            }
          }
          break;

          case 18:// A18 set fan speed
          {
            float fanPercent;
            if (CodeSeen('S')) {
              fanPercent = CodeValue();
              fanPercent = constrain(fanPercent, 0, 100);
              ExtUI::setTargetFan_percent(fanPercent, ExtUI::FAN0);
            }
            else {
              fanPercent = 100;
            }
            ExtUI::setTargetFan_percent(fanPercent, ExtUI::FAN0);

            SENDLINE_PGM("");
          }
          break;

          case 19: // A19 stop stepper drivers - sent on stop extrude command and on turn motors off command
            if (!ExtUI::isPrinting()) {
              quickstop_stepper();
              disable_all_steppers();
            }

            SENDLINE_PGM("");
            break;

          case 20: { // A20 read printing speed

            if (CodeSeen('S'))
              feedrate_percentage = constrain(CodeValue(), 40, 999);
            else
              SEND_PGM_VAL("A20V ", feedrate_percentage);
          }
            break;

          case 21: // A21 all home
            if (!ExtUI::isPrinting() && !ExtUI::isPrintingFromMediaPaused()) {
              if (CodeSeen('X') || CodeSeen('Y') || CodeSeen('Z')) {
                if (CodeSeen('X'))
                  ExtUI::injectCommands_P(PSTR("G28 X"));
                if (CodeSeen('Y'))
                  ExtUI::injectCommands_P(PSTR("G28 Y"));
                if (CodeSeen('Z'))
                  ExtUI::injectCommands_P(PSTR("G28 Z"));
              }
              else if (CodeSeen('C')) {
                ExtUI::injectCommands_P(PSTR("G28"));
              }
            }
            break;

          case 22: // A22 move X/Y/Z or extrude
            if (!ExtUI::isPrinting()) {
              float coorvalue;
              unsigned int movespeed = 0;
              char commandStr[30];
              char fullCommandStr[38];

              commandStr[0] = 0; // empty string
              if (CodeSeen('F'))  // Set feedrate
                movespeed = CodeValue();

              if (CodeSeen('X')) { // Move in X direction
                coorvalue = CodeValue();
                if ((coorvalue <= 0.2) && coorvalue > 0)
                  sprintf_P(commandStr, PSTR("G1 X0.1F%i"), movespeed);
                else if ((coorvalue <= -0.1) && coorvalue > -1)
                  sprintf_P(commandStr, PSTR("G1 X-0.1F%i"), movespeed);
                else
                  sprintf_P(commandStr, PSTR("G1 X%iF%i"), int(coorvalue), movespeed);
              }
              else if (CodeSeen('Y')) {  // Move in Y direction
                coorvalue = CodeValue();
                if ((coorvalue <= 0.2) && coorvalue > 0)
                  sprintf_P(commandStr, PSTR("G1 Y0.1F%i"), movespeed);
                else if ((coorvalue <= -0.1) && coorvalue > -1)
                  sprintf_P(commandStr, PSTR("G1 Y-0.1F%i"), movespeed);
                else
                  sprintf_P(commandStr, PSTR("G1 Y%iF%i"), int(coorvalue), movespeed);
              }
              else if (CodeSeen('Z')) {  // Move in Z direction
                coorvalue = CodeValue();
                if ((coorvalue <= 0.2) && coorvalue > 0)
                  sprintf_P(commandStr, PSTR("G1 Z0.1F%i"), movespeed);
                else if ((coorvalue <= -0.1) && coorvalue > -1)
                  sprintf_P(commandStr, PSTR("G1 Z-0.1F%i"), movespeed);
                else
                  sprintf_P(commandStr, PSTR("G1 Z%iF%i"), int(coorvalue), movespeed);
              }
              else if (CodeSeen('E')) { // Extrude
                coorvalue = CodeValue();
                if ((coorvalue <= 0.2) && coorvalue > 0)
                  sprintf_P(commandStr, PSTR("G1 E0.1F%i"), movespeed);
                else if ((coorvalue <= -0.1) && coorvalue > -1)
                  sprintf_P(commandStr, PSTR("G1 E-0.1F%i"), movespeed);
                else
                  sprintf_P(commandStr, PSTR("G1 E%iF500"), int(coorvalue));
              }

              if (strlen(commandStr) > 0) {
                sprintf_P(fullCommandStr, PSTR("G91\n%s\nG90"), commandStr);
                #if ENABLED(ANYCUBIC_LCD_DEBUG)
                  SERIAL_ECHOPGM("TFT Serial Debug: A22 Move final request with gcode... ");
                  SERIAL_ECHOLN(fullCommandStr);
                #endif
                ExtUI::injectCommands(fullCommandStr);
              }
            }
            SENDLINE_PGM("");
            break;

          case 23: // A23 preheat pla
            if (!ExtUI::isPrinting()) {
              if (ExtUI::getAxisPosition_mm(ExtUI::Z) < 10)
                ExtUI::injectCommands_P(PSTR("G1 Z10")); // RASE Z AXIS

              ExtUI::setTargetTemp_celsius(PREHEAT_1_TEMP_BED, (ExtUI::heater_t) ExtUI::BED);
              ExtUI::setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, (ExtUI::extruder_t) ExtUI::E0);
              SENDLINE_PGM("OK");
            }
            break;

          case 24:// A24 preheat abs
            if (!ExtUI::isPrinting()) {
              if (ExtUI::getAxisPosition_mm(ExtUI::Z) < 10)
                ExtUI::injectCommands_P(PSTR("G1 Z10")); // RASE Z AXIS

              ExtUI::setTargetTemp_celsius(PREHEAT_2_TEMP_BED, (ExtUI::heater_t) ExtUI::BED);
              ExtUI::setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, (ExtUI::extruder_t) ExtUI::E0);
              SENDLINE_PGM("OK");
            }
            break;

          case 25: // A25 cool down
            if (!ExtUI::isPrinting()) {
              ExtUI::setTargetTemp_celsius(0, (ExtUI::heater_t) ExtUI::BED);
              ExtUI::setTargetTemp_celsius(0, (ExtUI::extruder_t) ExtUI::E0);

              SENDLINE_DBG_PGM("J12", "TFT Serial Debug: Cooling down... J12"); // J12 cool down
            }
            break;

          case 26: // A26 refresh SD
            #if ENABLED(SDSUPPORT)
              if (ExtUI::isMediaInserted()) {
                if (strlen(SelectedDirectory) > 0) {
                  ExtUI::FileList currentFileList;
                  if ((SelectedDirectory[0] == '.') && (SelectedDirectory[1] == '.')) {
                    currentFileList.upDir();
                  }
                  else {
                    if (SelectedDirectory[0] == '<')
                      HandleSpecialMenu();
                    else
                      currentFileList.changeDir(SelectedDirectory);
                  }
                }
              }
              else {
                SENDLINE_DBG_PGM("J02", "TFT Serial Debug: No SD Card mounted to refresh SD A26... J02");
              }

              SelectedDirectory[0] = 0;
            #endif
            break;

            #if ENABLED(SERVO_ENDSTOPS)
              case 27: break; // A27 servos angles adjust
            #endif

          case 28: // A28 filament test
            if (CodeSeen('O'))
              NOOP;
            else if (CodeSeen('C'))
              NOOP;
            SENDLINE_PGM("");
            break;

          case 33: // A33 get version info
            SEND_PGM("J33 ");
            SENDLINE_PGM(DETAILED_BUILD_VERSION);
            break;

          default:
            break;
        }
      }

      TFTbufindw = (TFTbufindw + 1) % TFTBUFSIZE;
      TFTbuflen += 1;
      serial3_count = 0; // clear buffer
    }
    else {
      TFTcmdbuffer[TFTbufindw][serial3_count++] = serial3_char;
    }
  }
}

void AnycubicTFTClass::DoSDCardStateCheck() {
  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    bool isInserted = ExtUI::isMediaInserted();
    if (isInserted)
      SENDLINE_DBG_PGM("J00", "TFT Serial Debug: SD card state changed... isInserted");
    else
      SENDLINE_DBG_PGM("J01", "TFT Serial Debug: SD card state changed... !isInserted");

  #endif
}

void AnycubicTFTClass::DoFilamentRunoutCheck() {
  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    // NOTE: ExtUI::getFilamentRunoutState() only returns the runout state if the job is printing
    // we want to actually check the status of the pin here, regardless of printstate
    if (READ(FIL_RUNOUT_PIN)) {
      if (mediaPrintingState == AMPRINTSTATE_PRINTING || mediaPrintingState == AMPRINTSTATE_PAUSED || mediaPrintingState == AMPRINTSTATE_PAUSE_REQUESTED) {
        // play tone to indicate filament is out
        ExtUI::injectCommands_P(PSTR("\nM300 P200 S1567\nM300 P200 S1174\nM300 P200 S1567\nM300 P200 S1174\nM300 P2000 S1567"));

        // tell the user that the filament has run out and wait
        SENDLINE_DBG_PGM("J23", "TFT Serial Debug: Blocking filament prompt... J23");
      }
      else {
        SENDLINE_DBG_PGM("J15", "TFT Serial Debug: Non blocking filament runout... J15");
      }
    }
  #endif // FILAMENT_RUNOUT_SENSOR
}

void AnycubicTFTClass::StartPrint() {
  #if ENABLED(SDSUPPORT)
    if (!ExtUI::isPrinting() && strlen(SelectedFile) > 0) {
      #if ENABLED(ANYCUBIC_LCD_DEBUG)
        SERIAL_ECHOPGM("TFT Serial Debug: About to print file ... ");
        SERIAL_ECHO(ExtUI::isPrinting());
        SERIAL_ECHOPGM(" ");
        SERIAL_ECHOLN(SelectedFile);
      #endif
      mediaPrintingState = AMPRINTSTATE_PRINTING;
      mediaPauseState    = AMPAUSESTATE_NOT_PAUSED;
      ExtUI::printFile(SelectedFile);
    }
  #endif // SDUPPORT
}

void AnycubicTFTClass::PausePrint() {
  #if ENABLED(SDSUPPORT)
    if (ExtUI::isPrintingFromMedia() && mediaPrintingState != AMPRINTSTATE_STOP_REQUESTED && mediaPauseState == AMPAUSESTATE_NOT_PAUSED) {
      mediaPrintingState = AMPRINTSTATE_PAUSE_REQUESTED;
      mediaPauseState    = AMPAUSESTATE_NOT_PAUSED; // need the userconfirm method to update pause state
      SENDLINE_DBG_PGM("J05", "TFT Serial Debug: SD print pause started... J05"); // J05 printing pause

      // for some reason pausing the print doesn't retract the extruder so force a manual one here
      ExtUI::injectCommands_P(PSTR("G91\nG1 E-2 F1800\nG90"));
      ExtUI::pausePrint();
    }
  #endif
}

void AnycubicTFTClass::ResumePrint() {
  #if ENABLED(SDSUPPORT)
    #if ENABLED(FILAMENT_RUNOUT_SENSOR)
      if (READ(FIL_RUNOUT_PIN)) {
        #if ENABLED(ANYCUBIC_LCD_DEBUG)
          SERIAL_ECHOLNPGM("TFT Serial Debug: Resume Print with filament sensor still tripped... ");
        #endif

        // trigger the user message box
        DoFilamentRunoutCheck();

        // re-enable the continue button
        SENDLINE_DBG_PGM("J18", "TFT Serial Debug: Resume Print with filament sensor still tripped... J18");
        return;
      }
    #endif

    if (mediaPauseState == AMPAUSESTATE_HEATER_TIMEOUT) {
      mediaPauseState = AMPAUSESTATE_REHEATING;
      // TODO: JBA I don't think J05 just disables the continue button, i think it injects a rogue M25. So taking this out
      // // disable the continue button
      // SENDLINE_DBG_PGM("J05", "TFT Serial Debug: Resume called with heater timeout... J05"); // J05 printing pause

      // reheat the nozzle
      ExtUI::setUserConfirmed();
    }
    else {
      mediaPrintingState = AMPRINTSTATE_PRINTING;
      mediaPauseState    = AMPAUSESTATE_NOT_PAUSED;

      SENDLINE_DBG_PGM("J04", "TFT Serial Debug: SD print resumed... J04"); // J04 printing form sd card now
      ExtUI::resumePrint();
    }
  #endif
}

void AnycubicTFTClass::StopPrint() {
  #if ENABLED(SDSUPPORT)
    mediaPrintingState = AMPRINTSTATE_STOP_REQUESTED;
    mediaPauseState    = AMPAUSESTATE_NOT_PAUSED;
    SENDLINE_DBG_PGM("J16", "TFT Serial Debug: SD print stop called... J16");

    // for some reason stopping the print doesn't retract the extruder so force a manual one here
    ExtUI::injectCommands_P(PSTR("G91\nG1 E-2 F1800\nG90"));
    ExtUI::stopPrint();
  #endif
}

#endif // ANYCUBIC_LCD_I3MEGA
