/*********************
 * extui_example.cpp *
 *********************/

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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_CHIRON)

#include "ui_api.h"
#include "lib/anycubic_chiron/chiron_tft.h"

using namespace Anycubic;

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

static void sendValue_P(PGM_P prefix, int value) {
  send_P(prefix);
  LCD_SERIAL.print(value);
}

static void sendValue_P(PGM_P prefix, float value) {
  send_P(prefix);
  LCD_SERIAL.print(value);
}

static void sendValueLine_P(PGM_P prefix, int value) {
  send_P(prefix);
  LCD_SERIAL.print(value);
  sendNewLine();
}

static void sendValueLine_P(PGM_P prefix, float value) {
  send_P(prefix);
  LCD_SERIAL.print(value);
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
  void onStartup() {
    Chiron.Startup();
  }
  void onIdle() {
    Chiron.IdleLoop();
  }
  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    Chiron.PrinterKilled(error,component);

  #define RX_LEN_MAX 63
  static void parseSerialRx() {
    static char rxBuffer[RX_LEN_MAX+1];
    static uint8_t rxLen = 0;

    while (LCD_SERIAL.available()) {
      const char c = LCD_SERIAL.read();
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
  void onMediaInserted() {
    Chiron.MediaEvent(AC_media_inserted);
  };
  void onMediaError() {
    Chiron.MediaEvent(AC_media_error);
  };
  void onMediaRemoved() {
    Chiron.MediaEvent(AC_media_removed);
  };
  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    #if ENABLED(SPEAKER)
      tone(BEEPER_PIN,frequency,duration);  // from marlincore.h
    #endif
  }
  void onPrintTimerStarted() {
    Chiron.TimerEvent(AC_timer_started);
  }
  void onPrintTimerPaused() {
    Chiron.TimerEvent(AC_timer_paused);
  }

  void onStartup() {
    #ifndef LCD_BAUDRATE
      #define LCD_BAUDRATE 115200
    #endif
    LCD_SERIAL.begin(LCD_BAUDRATE);
    sendNewLine();
    SENDLINE_PGM("J17"); // Reset
    delay_ms(10);
    SENDLINE_PGM("J12"); // Ready
  }
  void onPrintTimerStopped() {
    Chiron.TimerEvent(AC_timer_stopped);
  }
  void onFilamentRunout(const extruder_t extruder) {
    Chiron.FilamentRunout();
  }
  void onUserConfirmRequired(const char * const msg) {
    Chiron.ConfirmationRequest(msg);
  }
  void onStatusChanged(const char * const msg) {
    Chiron.StatusChange(msg);
  }
  void onFactoryReset() {}

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

  #if HAS_MESH
    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      // Called when any mesh points are updated
      //_SELP_6_P(PSTR("onMeshUpdate() x:"),xpos,PSTR(" y:"),ypos,PSTR(" z:"),zval);
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const ExtUI::probe_state_t state) {
      // Called to indicate a special condition
      //_SELP_6_P(PSTR("onMeshUpdate() x:"),xpos,PSTR(" y:"),ypos,PSTR(" state:"),state);
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
      Chiron.PowerLossRecovery();
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
    }
  #endif
}

#endif // ANYCUBIC_LCD_CHIRON
