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
 * extui_anycubic_chiron_lcd.cpp
 *
 * Anycubic Chiron TFT support for Marlin
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_CHIRON)

#include "extui/ui_api.h"

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  #if GRID_MAX_POINTS_X != 5 || GRID_MAX_POINTS_Y != 5
    #error ANYCUBIC CHIRON LCD requires a 5x5 bed leveling grid (GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y)
  #endif
#else
  #error ANYCUBIC CHIRON LCD requires AUTO_BED_LEVELING_BILINEAR enabled
#endif

#if DISABLED(FILAMENT_RUNOUT_SENSOR)
  #error ANYCUBIC CHIRON LCD requires FILAMENT_RUNOUT_SENSOR enabled
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #error ANYCUBIC CHIRON LCD does not currently support POWER_LOSS_RECOVERY
#endif

static bool is_auto_leveling = false;
static bool is_printing_from_sd = false;
static bool is_out_of_filament = false;

static void sendNewLine(void) {
  ANYCUBIC_LCD_SERIAL.write('\r');
  ANYCUBIC_LCD_SERIAL.write('\n');
}

static void send(const char *str) {
  ANYCUBIC_LCD_SERIAL.print(str);
}

static void sendLine(const char *str) {
  send(str);
  sendNewLine();
}

static void send_P(PGM_P str) {
  while (const char c = pgm_read_byte(str++))
    ANYCUBIC_LCD_SERIAL.write(c);
}

static void sendLine_P(PGM_P str) {
  send_P(str);
  sendNewLine();
}

static void sendValue_P(PGM_P prefix, int value) {
  send_P(prefix);
  ANYCUBIC_LCD_SERIAL.print(value);
}

static void sendValue_P(PGM_P prefix, float value) {
  send_P(prefix);
  ANYCUBIC_LCD_SERIAL.print(value);
}

static void sendValueLine_P(PGM_P prefix, int value) {
  send_P(prefix);
  ANYCUBIC_LCD_SERIAL.print(value);
  sendNewLine();
}

static void sendValueLine_P(PGM_P prefix, float value) {
  send_P(prefix);
  ANYCUBIC_LCD_SERIAL.print(value);
  sendNewLine();
}

static int parseIntArgument(const char *buffer, char letterId) {
  char *p = strchr(buffer, letterId);
  if (!p)
    return -1;
  return atoi(p+1);
}

static float parseFloatArgument(const char *buffer, char letterId) {
  char *p = strchr(buffer, letterId);
  if (!p)
    return NAN;
  return strtof(p+1, nullptr);
}

static int mmToHundredths(float x) {
  // Round
  if (x >= 0)
    x += 0.005f;
  else
    x -= 0.005f;
  return (int)(x * 100.0f);
}

static float hundredthsToMm(int x) {
  return x / 100.0f;
}

#define SEND_PGM(str)                           send_P(PSTR(str))
#define SENDLINE_PGM(str)                       sendLine_P(PSTR(str))
#define SENDVALUE_PGM(prefix, value)            sendValue_P(PSTR(prefix), value)
#define SENDVALUELINE_PGM(prefix, value)        sendValueLine_P(PSTR(prefix), value)

namespace ExtUI {

  static void moveAxis(float delta, feedRate_t feedrate, axis_t axis) {
    float pos = getAxisPosition_mm(axis);
    pos += delta;
    setAxisPosition_mm(pos, axis, feedrate);
  }

  static void handleCmd(const char *rx) {
    static FileList fileList;
    static char selectedFileShortName[8+1+3+1];

    if (rx[0] != 'A') {
      SERIAL_ECHOPGM("Unexpected RX: ");
      SERIAL_ECHOLN(rx);

      return;
    }

    const int cmd = atoi(&rx[1]);

    // Uncomment for debugging RX
    //if (cmd > 7 && cmd != 20) {
    //  SERIAL_ECHOPGM("RX: ");
    //  SERIAL_ECHOLN(rx);
    //}

    switch (cmd) {
      case 0: // Get Hotend Actual Temperature
        SENDVALUELINE_PGM("A0V ", (int)getActualTemp_celsius(E0));
        break;
      case 1: // Get Hotend Target Temperature
        SENDVALUELINE_PGM("A1V ", (int)getTargetTemp_celsius(E0));
        break;
      case 2: // Get Bed Actual Temperature
        SENDVALUELINE_PGM("A2V ", (int)getActualTemp_celsius(BED));
        break;
      case 3: // Get Bed Target Temperature
        SENDVALUELINE_PGM("A3V ", (int)getTargetTemp_celsius(BED));
        break;
      case 4: // Get Fan Speed
        SENDVALUELINE_PGM("A4V ", (int)getTargetFan_percent(FAN0));
        break;
      case 5: // Get Current Coordinates
        SENDVALUE_PGM("A5V X: ", getAxisPosition_mm(X));
        SENDVALUE_PGM(" Y: ", getAxisPosition_mm(Y));
        SENDVALUE_PGM(" Z: ", getAxisPosition_mm(Z));
        sendNewLine();
        break;
      case 6: // Get SD Card Print Status
        if (isPrintingFromMedia())
          SENDVALUELINE_PGM("A6V ", (int)getProgress_percent());
        else
          SENDLINE_PGM("A6V ---");
        break;
      case 7: // Get Printing Time
        if (isPrinting()) {
          const int totalMinutes = getProgress_seconds_elapsed() / 60;
          SENDVALUE_PGM("A7V ", (int)(totalMinutes/60));
          SENDVALUE_PGM(" H ", (int)(totalMinutes%60));
          SENDLINE_PGM(" M");
        } else {
          SENDLINE_PGM("A7V 999:999");
        }
        break;
      case 8: // Get SD Card File List
        if (isMediaInserted()) {
          const int startIndex = parseIntArgument(rx, 'S');
          SENDLINE_PGM("FN ");
          for (int i = 0, fileIndex = 0, numFiles = 0; i < (int)fileList.count() && numFiles < 4; i++) {
            fileList.seek(i);
            if (!fileList.isDir()) {
              if (fileIndex >= startIndex) {
                sendLine(fileList.shortFilename());
                sendLine(fileList.longFilename());
                numFiles++;
              }
              fileIndex++;
            }
          }
          SENDLINE_PGM("END");
        } else {
          SENDLINE_PGM("J02");
        }
        break;
      case 9: // Pause SD Card Print
        if (isPrintingFromMedia()) {
          pausePrint();
          is_printing_from_sd = false;
          SENDLINE_PGM("J05");
        } else {
          SENDLINE_PGM("J16"); // Print stopped
        }
        break;
      case 10: // Resume SD Card Print
        if (is_out_of_filament) {
          is_out_of_filament = false;
          // Filament change did eject the old filament automatically,
          // now continue and load the new one
          setUserConfirmed();
          SENDLINE_PGM("J04"); // Printing from SD card
        } else if (isPrintingFromMediaPaused()) {
          resumePrint();
          SENDLINE_PGM("J04"); // Printing from SD card
        }
        break;
      case 11: // Stop SD Card Print
        if (isPrintingFromMedia()) {
          stopPrint();
          is_printing_from_sd = false;
          SENDLINE_PGM("J16"); // Print stopped
        }
        break;
      //case 12: // Kill
      //  break;
      case 13: // Select File
        if (!isPrinting()) {
          // Store selected file name
          char *p = strchr(rx, ' ');
          if (p != nullptr && strlen(p+1) < sizeof(selectedFileShortName)) {
            strcpy(selectedFileShortName, p+1);
            SENDLINE_PGM("J20"); // Open succeeded
          }
          else
            SENDLINE_PGM("J21"); // Open failed
        }
        break;
      case 14: // Start Print
        if (!isPrinting() && strcmp(selectedFileShortName, "") != 0) {
          printFile(selectedFileShortName);
          is_printing_from_sd = true;
          SENDLINE_PGM("J04"); // Printing from SD card
        }
        break;
      case 15: // Resume from power outage
        // This is not supported, just report print as completed
        SENDLINE_PGM("J16"); // Print stopped
        break;
      case 16: // Set Hotend Target Temperature
        {
          int temp = parseIntArgument(rx, 'S');
          if (temp >= 0)
            setTargetTemp_celsius(temp, E0);
        }
        break;
      case 17: // Set Bed Target Temperature
        {
          int temp = parseIntArgument(rx, 'S');
          if (temp >= 0)
            setTargetTemp_celsius(temp, BED);
        }
        break;
      case 18: // Set Fan Speed
        {
          int temp = parseIntArgument(rx, 'S');
          if (temp >= 0)
            setTargetFan_percent(temp, FAN0);
        }
        break;
      case 19: // Disable Motors
        injectCommands_P(PSTR("M84"));
        break;
      case 20: // Get/Set Printing Speed
        {
          int newPerc = parseIntArgument(rx, 'S');
          if (newPerc >= 0)
            setFeedrate_percent(newPerc);
          else
            SENDVALUELINE_PGM("A20V ", (int)getFeedrate_percent());
        }
        break;
      case 21: // Home axes
        if (!isPrinting()) {
          const bool hasX = strchr(rx, 'X') != nullptr,
                     hasY = strchr(rx, 'Y') != nullptr,
                     hasZ = strchr(rx, 'Z') != nullptr,
                     hasC = strchr(rx, 'C') != nullptr;
          if (hasX || hasY || hasZ) {
            if (hasX) injectCommands_P(PSTR("G28 X"));
            if (hasY) injectCommands_P(PSTR("G28 Y"));
            if (hasZ) injectCommands_P(PSTR("G28 Z"));
          } else if (hasC) {
            injectCommands_P(PSTR("G28"));
          }
        }
        break;
      case 22: // Move axes
        if (!isPrinting()) {
          const int feedrate = parseIntArgument(rx, 'F') / 60;
          float delta;
          if (!isnan(delta = parseFloatArgument(rx, 'X')))
            moveAxis(delta, feedrate, X);
          else if (!isnan(delta = parseFloatArgument(rx, 'Y')))
            moveAxis(delta, feedrate, Y);
          else if (!isnan(delta = parseFloatArgument(rx, 'Z')))
            moveAxis(delta, feedrate, Z);
        }
        break;
      case 23: // Preheat PLA
        setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, E0);
        setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
        SENDLINE_PGM("OK");
        break;
      case 24: // Preheat ABS
        setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, E0);
        setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
        SENDLINE_PGM("OK");
        break;
      case 25: // Cool down
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, BED);
        SENDLINE_PGM("J12");
        break;
      case 26: // Refresh SD Card
        fileList.refresh();
        break;
      //case 27: // Adjust Servo Angles
      //  break;
      //case 28: // Filament Test
      //  break;
      case 29: // Get Bed Autolevel Grid
        {
          int x = parseIntArgument(rx, 'X'),
              y = parseIntArgument(rx, 'Y');
          if (x != -1 && y != -1) {
            xy_uint8_t coord;
            coord.set(x, y);
            const int value = mmToHundredths(getMeshPoint(coord));
            SENDVALUELINE_PGM("A29V ", value);
          }
        }
        break;
      case 30: // Autolevel
        if (strchr(rx, 'S')) { // Autoleveling started by clicking "PROBE" and then "OK"
          // Note:
          //  We check for completion by monitoring the command queue.
          //  Since it will become empty *while* processing the last injected command,
          //  we enqueue an extra 10ms delay so we can the determine when all the others
          //  have completed.
          if (isMachineHomed())
            injectCommands_P(PSTR("G29\nG4 P10"));
          else
            injectCommands_P(PSTR("G28\nG29\nG4 P10"));
          is_auto_leveling = true;
        } else { // Entering Autoleveling screen
          if (isPrinting())
            SENDLINE_PGM("J24"); // Disallow autoleveling
          else
            SENDLINE_PGM("J26"); // Allow autoleveling
        }
        break;
      case 31: // Set Bed Autolevel Z offset
        if (strchr(rx, 'G')) { // Get
          SENDVALUELINE_PGM("A31V ", getZOffset_mm());
        } else if (strchr(rx, 'S')) { // Set
          float delta = parseFloatArgument(rx, 'S');
          delta = constrain(delta, -1.0, 1.0);
          setZOffset_mm(getZOffset_mm() + delta);

          SENDVALUELINE_PGM("A31V ", getZOffset_mm());
        } else if (strchr(rx, 'D')) { // Save
          injectCommands_P(PSTR("M500"));
        }
        break;
      //case 32: // ?
      //  break;
      case 33: // Get Version Info
        SENDLINE_PGM("J33 " SHORT_BUILD_VERSION);
        break;
      case 34: // Set Bed Autolevel Grid
        {
          int x = parseIntArgument(rx, 'X'),
              y = parseIntArgument(rx, 'Y'),
              v = parseIntArgument(rx, 'V');
          if (x != -1 && y != -1 && v != -1) { // Set new value
            float value = hundredthsToMm(v);
            value = constrain(value, -10, 10);

            xy_uint8_t coord;
            coord.set(x, y);
            setMeshPoint(coord, value);
          } else if (strchr(rx, 'S')) { // Save (apply new values)
            injectCommands_P(PSTR("M500"));
          } else if (strchr(rx, 'C')) { // Cancel (discard new values)
            injectCommands_P(PSTR("M501"));
          }
        }
        break;
    }
  }

  #define RX_LEN_MAX 63
  static void parseSerialRx() {
    static char rxBuffer[RX_LEN_MAX+1];
    static uint8_t rxLen = 0;

    while (ANYCUBIC_LCD_SERIAL.available()) {
      const char c = ANYCUBIC_LCD_SERIAL.read();
      switch (c) {
        case '\r': case '\n':
          if (rxLen > 0 && rxLen <= RX_LEN_MAX) {
            rxBuffer[rxLen] = '\0'; // Terminate string
            handleCmd(rxBuffer);
          }
          rxLen = 0;
          break;
        default:
          if (rxLen < RX_LEN_MAX)
            rxBuffer[rxLen++] = c;
          else {
            rxLen = 0xFF; // Overrun
            SERIAL_ECHOPGM("Warning: dropping long received line");
          }
          break;
      }
    }
  }

  static void detectPrintFromSdCompletion() {
    // Note: printFile() queues some commands that actually start the print, so isPrintingFromMedia()
    //       initially returns false
    if (is_printing_from_sd && !commandsInQueue() && !isPrintingFromMedia()) {
      is_printing_from_sd = false;
      SENDLINE_PGM("J14"); // Print done
    }
  }

  static void detectAutolevelingCompletion() {
    if (is_auto_leveling && !commandsInQueue()) {
      is_auto_leveling = false;
      injectCommands_P(PSTR("M500"));
      SENDLINE_PGM("J25"); // Autoleveling done
    }
  }

  void onStartup() {
    ANYCUBIC_LCD_SERIAL.begin(115200);
    sendNewLine();
    SENDLINE_PGM("J17"); // Reset
    delay_ms(10);
    SENDLINE_PGM("J12"); // Ready
  }

  void onIdle() {
    parseSerialRx();
    detectAutolevelingCompletion();
    detectPrintFromSdCompletion();
  }

  void onPrinterKilled(PGM_P const error, PGM_P const component) { }

  void onMediaInserted() {
    SENDLINE_PGM("J00"); // SD Inserted
  }

  void onMediaError() { }

  void onMediaRemoved() {
    SENDLINE_PGM("J01"); // SD Removed
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    tone(BEEPER_PIN, frequency, duration);
  }

  void onPrintTimerStarted() { }

  void onPrintTimerPaused() { }

  void onPrintTimerStopped() { }

  void onFilamentRunout(const extruder_t extruder) {
    is_out_of_filament = true;
    SENDLINE_PGM("J23"); // Filament runout
    SENDLINE_PGM("J18"); // Print paused
    // Note: printer will unload filament automatically
  }

  void onUserConfirmRequired(const char * const msg) { }

  void onStatusChanged(const char * const msg) { }

  void onFactoryReset() { }

  void onStoreSettings(char *buff) { }

  void onLoadSettings(const char *buff) { }

  void onConfigurationStoreWritten(bool success) { }

  void onConfigurationStoreRead(bool success) { }

  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) { }

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() { }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) { }
  #endif
}

#endif // ANYCUBIC_LCD_CHIRON
