/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * lcd/extui/elegoo_neptune_3/neptune3_tft.h
 *
 * Elegoo Neptune 3 Pro / Plus / Max TJC touch screen
 * Compatible with the stock screen firmware (3D30_20221121.HMI).
 */

#include "../ui_api.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

class Neptune3TFT {
  public:
    static void startup();
    static void idleLoop();

    static void printerKilled(FSTR_P const error);
    static void heatingError(const heater_id_t heater);
    static void minTempError(const heater_id_t heater);
    static void maxTempError(const heater_id_t heater);

    static void mediaMounted();
    static void mediaRemoved();
    static void mediaError();

    static void printTimerStarted();
    static void printDone();
    static void statusChanged(const char * const msg);
    static void userConfirmRequired();
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      static void pauseMode(const PauseMessage message);
    #endif

    static void homingStart();
    static void homingDone();
    static void levelingStart();
    static void levelingDone();
    static void meshUpdate(const int8_t xpos, const int8_t ypos, const float zval);

    #if ENABLED(POWER_LOSS_RECOVERY)
      static void powerLossResume();
      static void setPowerLoss(const bool onoff);
    #endif

    static void factoryReset();
    static void storeSettings(char *buff);
    static void loadSettings(const char *buff);
    static void postprocessSettings();

  private:
    static void sendRaw(FSTR_P const fstr);
    static void send(FSTR_P const fstr);
    static void sendf(PGM_P const fmt, ...);
    static void sendEnd();

    static void readData();
    static void processFrame();
    static void handleKey(const uint16_t addr, const uint16_t value);

    static void periodicUpdate();
    static void checkWaitState();
    static void sendModel();
    static void sendMesh();
    static void sendZOffset(FSTR_P const obj);
    static void sendPreTemps(const bool nozzle, const bool bed);
    static void sendPrintInfo();
    static void sendSpeedValue();
    static void sendAdvancedValues();
    static void sendMaterialValues();

    static void refreshFileList();
    static void clearFileList();
    static void startPrint(const int8_t index);
    static void stopPrint();
    static void clearPreview();
    static void startPreview(const int8_t index);
    static void previewStep();

    static void adjustZOffset(const float delta);
    static void moveAxis(const ExtUI::axis_t axis, const bool positive);
    static void moveExtruder(const float distance);
    static bool filamentPresent();
    static void toggleCaseLight();
};

extern Neptune3TFT neptune3;
