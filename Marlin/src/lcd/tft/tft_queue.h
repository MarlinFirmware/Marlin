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
#pragma once

#include "../../inc/MarlinConfig.h"
#include "tft_string.h"
#include "tft_image.h"

#ifndef TFT_QUEUE_SIZE
  #define TFT_QUEUE_SIZE              8192
#endif

enum QueueTaskType : uint8_t {
  TASK_END_OF_QUEUE = 0x00,
  TASK_FILL,
  TASK_CANVAS,
};

enum QueueTaskState : uint8_t {
  TASK_STATE_READY = 0x00,
  TASK_STATE_IN_PROGRESS,
  TASK_STATE_COMPLETED,
  TASK_STATE_SKETCH = 0xFF,
};

enum CanvasSubtype : uint8_t {
  CANVAS_SET_BACKGROUND = 0x00,
  CANVAS_ADD_TEXT,
  CANVAS_ADD_IMAGE,
  CANVAS_ADD_BAR,
  CANVAS_ADD_RECTANGLE,
};

typedef struct __attribute__((__packed__)) {
  QueueTaskType type;
  QueueTaskState state;
  uint8_t *nextTask;
} queueTask_t;

typedef struct __attribute__((__packed__)) {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint16_t color;
  uint32_t count;
} parametersFill_t;

typedef struct __attribute__((__packed__)) {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint32_t count;
} parametersCanvas_t;

typedef struct __attribute__((__packed__)) {
  CanvasSubtype type;
  uint8_t *nextParameter;
  uint16_t color;
} parametersCanvasBackground_t;

typedef struct __attribute__((__packed__)) {
  CanvasSubtype type;
  uint8_t *nextParameter;
  uint16_t x;
  uint16_t y;
  uint16_t color;
  uint32_t count;
  uint16_t maxWidth;
  uint16_t stringLength;
} parametersCanvasText_t;

typedef struct __attribute__((__packed__)) {
  CanvasSubtype type;
  uint8_t *nextParameter;
  int16_t x;
  int16_t y;
  MarlinImage image;
} parametersCanvasImage_t;

typedef struct __attribute__((__packed__)) {
  CanvasSubtype type;
  uint8_t *nextParameter;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint16_t color;
} parametersCanvasBar_t;

typedef struct __attribute__((__packed__)) {
  CanvasSubtype type;
  uint8_t *nextParameter;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint16_t color;
} parametersCanvasRectangle_t;

class TFT_Queue {
  private:
    static uint8_t queue[TFT_QUEUE_SIZE];
    static uint8_t *end_of_queue;
    static uint8_t *current_task;
    static uint8_t *last_task;
    static uint8_t *last_parameter;

    static void finish_sketch();
    static void fill(queueTask_t *task);
    static void canvas(queueTask_t *task);
    static void handle_queue_overflow(uint16_t sizeNeeded);

  public:
    static void reset();
    static void async();
    static void sync() { while (current_task != nullptr) async(); }

    static void fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    static void canvas(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    static void set_background(uint16_t color);
    static void add_text(uint16_t x, uint16_t y, uint16_t color, const uint8_t *string, uint16_t maxWidth);
    static void add_text(uint16_t x, uint16_t y, uint16_t color, const char *string, uint16_t maxWidth) {
      add_text(x, y, color, (uint8_t *)string, maxWidth);
    }

    static void add_image(int16_t x, int16_t y, MarlinImage image, uint16_t *colors);
    static void add_image(int16_t x, int16_t y, MarlinImage image, uint16_t color_main, uint16_t color_background, uint16_t color_shadow);

    static void add_bar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    static void add_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
};
