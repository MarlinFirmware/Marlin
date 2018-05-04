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

#ifndef _LCD_API_H_
#define _LCD_API_H_

namespace Marlin_LCD_API {
  typedef const __FlashStringHelper *progmem_str;

  enum axis_t {
    X,
    Y,
    Z,
    E0,
    E1
  };

  const uint8_t extruderCount = EXTRUDERS;
  const uint8_t fanCount      = 1;

  float getActualTemp_celsius(const uint8_t extruder);
  float getTargetTemp_celsius(const uint8_t extruder);
  float getFan_percent(const uint8_t fan);

  float getAxisPosition_mm(const axis_t axis);
  float getAxisSteps_per_mm(const axis_t axis);
  uint8_t getProgress_percent();
  uint32_t getProgress_seconds_elapsed();
  float getFeedRate_percent();
  float getZOffset_mm();
  bool isAxisPositionKnown(const axis_t axis);
  bool isMoving();

  progmem_str getFirmwareName();

  void setTargetTemp_celsius(const uint8_t extruder, float temp);
  void setFan_percent(const uint8_t fan, float percent);
  void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s);
  void setAxisSteps_per_mm(const axis_t axis, float steps_per_mm);
  void incrementZOffset_mm(const float z_offset);
  void setFeedrate_percent(const float percent);

  void runGCode(progmem_str gcode);

  float clamp(float value, float minimum, float maximum);

  void initMedia();
  void checkMedia();
  bool isPrintingFromMedia();
  bool isPrinting();
  bool isMediaInserted();
  void stopPrint();
  void pausePrint();
  void resumePrint();

  void onCardInserted();
  void onCardRemoved();
  void onPrinterKilled(const char* lcd_msg);
  void onPlayTone(const uint16_t frequency, const uint16_t duration);

  uint16_t getMediaFileCount();

  void printFromSDCard(const char *filename);
  void changeDir(const char *dirname);
  void upDir();
  bool isAtRootDir();

  class Media_Iterator {
    private:
      uint16_t index;
      uint16_t num_files;
    public:
      Media_Iterator(uint16_t start_index /* = 0*/);
      bool hasMore();
      void seek(uint16_t);
      void next();
      const char *longFilename();
      const char *shortFilename();
      const char *filename();
      uint16_t value() {return index;}
      uint16_t count() {return num_files;}
      bool isDirectory();
  };
};

#endif _LCD_API_H_