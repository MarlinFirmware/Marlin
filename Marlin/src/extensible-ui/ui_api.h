/************
 * ui_api.h *
 ************/

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

#ifndef _UI_API_H_
#define _UI_API_H_

#include "../inc/MarlinConfig.h"

#if defined(EXTENSIBLE_UI)
  typedef const __FlashStringHelper *progmem_str;

  namespace Extensible_UI_API {

    enum axis_t {
      X,
      Y,
      Z,
      E0,
      E1
    };

    const uint8_t extruderCount = EXTRUDERS;
    const uint8_t fanCount      = 1;

    // The following methods should be used by the extension module to
    // query or change Marlin's state.

    progmem_str getFirmwareName();

    bool isAxisPositionKnown(const axis_t axis);
    bool isMoving();

    float getActualTemp_celsius(const uint8_t extruder);
    float getTargetTemp_celsius(const uint8_t extruder);
    float getFan_percent(const uint8_t fan);
    float getAxisPosition_mm(const axis_t axis);
    float getAxisSteps_per_mm(const axis_t axis);
    uint8_t getProgress_percent();
    uint32_t getProgress_seconds_elapsed();
    float getFeedRate_percent();

    void setTargetTemp_celsius(const uint8_t extruder, float temp);
    void setFan_percent(const uint8_t fan, float percent);
    void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s);
    void setAxisSteps_per_mm(const axis_t axis, float steps_per_mm);
    void setFeedrate_percent(const float percent);

    #if HAS_BED_PROBE
      float getZOffset_mm();
      void incrementZOffset_mm(const float z_offset);
    #endif

    void enqueueCommands(progmem_str gcode);

    bool isPrintingFromMedia();
    bool isPrinting();
    void stopPrint();
    void pausePrint();
    void resumePrint();

    bool isMediaInserted();

    void printFile(const char *filename);
    void changeDir(const char *dirname);
    void upDir();
    bool isAtRootDir();
    uint16_t getFileCount();

    class Media_Iterator {
      private:
        uint16_t index;
        uint16_t num_files;
      public:
        Media_Iterator(uint16_t start_index /* = 0*/);

        uint16_t value() {return index;}
        uint16_t count() {return num_files;}
        bool hasMore();
        void seek(uint16_t);
        void next();
        const char *longFilename();
        const char *shortFilename();
        const char *filename();
        bool isDirectory();
    };

    // The following event handlers are to be declared by the extension
    // module and may be called by Marlin.

    void onStartup();
    void onIdle();
    void onUpdate();
    void onMediaInserted();
    void onMediaRemoved();
    void onPlayTone(const uint16_t frequency, const uint16_t duration);
    void onPrinterKilled(const char* msg);
    void onStatusChanged(const char* msg);
    void onStatusChanged(progmem_str msg);
  };
#endif // EXTENSIBLE_UI

#endif _UI_API_H_