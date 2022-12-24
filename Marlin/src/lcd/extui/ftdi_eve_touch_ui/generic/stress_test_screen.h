/************************
 * stress_test_screen.h *
 ************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#define FTDI_STRESS_TEST_SCREEN
#define FTDI_STRESS_TEST_SCREEN_CLASS StressTestScreen

struct StressTestScreenData {
  uint32_t next_watchdog_trigger;
  const char*  message;
};

class StressTestScreen : public BaseScreen, public UncachedScreen {
  private:
    static void drawDots(uint16_t x, uint16_t y, uint16_t h, uint16_t v);
    static bool watchDogTestNow();
    static void recursiveLockup();
    static void iterativeLockup();
    static void runTestOnBootup(bool enable);

  public:
    static void startupCheck();

    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};
