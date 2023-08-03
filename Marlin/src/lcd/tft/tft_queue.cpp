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

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_TFT

#include "tft_queue.h"
#include "tft.h"
#include "tft_image.h"

uint8_t TFT_Queue::queue[];
uint8_t *TFT_Queue::end_of_queue = queue;
uint8_t *TFT_Queue::current_task = nullptr;
uint8_t *TFT_Queue::last_task = nullptr;
uint8_t *TFT_Queue::last_parameter = nullptr;

void TFT_Queue::reset() {
  tft.abort();

  end_of_queue = queue;
  current_task = nullptr;
  last_task = nullptr;
  last_parameter = nullptr;
}

void TFT_Queue::async() {
  if (!current_task) return;
  queueTask_t *task = (queueTask_t *)current_task;

  // Check IO busy status
  if (tft.is_busy()) return;

  if (task->state == TASK_STATE_COMPLETED) {
    task = (queueTask_t *)task->nextTask;
    current_task = (uint8_t *)task;
  }

  finish_sketch();

  switch (task->type) {
    case TASK_END_OF_QUEUE: reset();      break;
    case TASK_FILL:         fill(task);   break;
    case TASK_CANVAS:       canvas(task); break;
  }
}

void TFT_Queue::finish_sketch() {
  if (!last_task) return;
  queueTask_t *task = (queueTask_t *)last_task;

  if (task->state == TASK_STATE_SKETCH) {
    *end_of_queue = TASK_END_OF_QUEUE;
    task->nextTask = end_of_queue;
    task->state = TASK_STATE_READY;

    if (!current_task) current_task = (uint8_t *)task;
  }
}

void TFT_Queue::fill(queueTask_t *task) {
  uint16_t count;
  parametersFill_t *task_parameters = (parametersFill_t *)(((uint8_t *)task) + sizeof(queueTask_t));

  if (task->state == TASK_STATE_READY) {
    tft.set_window(task_parameters->x, task_parameters->y, task_parameters->x + task_parameters->width - 1, task_parameters->y + task_parameters->height - 1);
    task->state = TASK_STATE_IN_PROGRESS;
  }

  if (task_parameters->count > DMA_MAX_WORDS) {
    count = DMA_MAX_WORDS;
    task_parameters->count -= DMA_MAX_WORDS;
  }
  else {
    count = task_parameters->count;
    task_parameters->count = 0;
    task->state = TASK_STATE_COMPLETED;
  }

  tft.write_multiple(task_parameters->color, count);
}

void TFT_Queue::canvas(queueTask_t *task) {
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)task) + sizeof(queueTask_t));

  uint16_t i;
  uint8_t *item = ((uint8_t *)task_parameters) + sizeof(parametersCanvas_t);

  if (task->state == TASK_STATE_READY) {
    task->state = TASK_STATE_IN_PROGRESS;
    tftCanvas.instantiate(task_parameters->x, task_parameters->y, task_parameters->width, task_parameters->height);
  }
  tftCanvas.next();

  for (i = 0; i < task_parameters->count; i++) {
    switch (*item) {
      case CANVAS_SET_BACKGROUND:
        tftCanvas.setBackground(((parametersCanvasBackground_t *)item)->color);
        break;
      case CANVAS_ADD_TEXT:
        tftCanvas.addText(((parametersCanvasText_t *)item)->x, ((parametersCanvasText_t *)item)->y, ((parametersCanvasText_t *)item)->color, (uint16_t*)(item + sizeof(parametersCanvasText_t)), ((parametersCanvasText_t *)item)->maxWidth);
        break;

      case CANVAS_ADD_IMAGE:
        MarlinImage image;
        uint16_t *colors;

        image = ((parametersCanvasImage_t *)item)->image;
        colors = (uint16_t *)(item + sizeof(parametersCanvasImage_t));
        tftCanvas.addImage(((parametersCanvasImage_t *)item)->x, ((parametersCanvasImage_t *)item)->y, image, colors);
        break;

      case CANVAS_ADD_BAR:
        tftCanvas.addBar(((parametersCanvasBar_t *)item)->x, ((parametersCanvasBar_t *)item)->y, ((parametersCanvasBar_t *)item)->width, ((parametersCanvasBar_t *)item)->height, ((parametersCanvasBar_t *)item)->color);
        break;
      case CANVAS_ADD_RECT:
        tftCanvas.addRect(((parametersCanvasRectangle_t *)item)->x, ((parametersCanvasRectangle_t *)item)->y, ((parametersCanvasRectangle_t *)item)->width, ((parametersCanvasRectangle_t *)item)->height, ((parametersCanvasRectangle_t *)item)->color);
        break;
    }
    item = ((parametersCanvasBackground_t *)item)->nextParameter;
  }

  if (tftCanvas.toScreen()) task->state = TASK_STATE_COMPLETED;
}

void TFT_Queue::fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  finish_sketch();

  queueTask_t *task = (queueTask_t *)end_of_queue;
  last_task = (uint8_t *)task;

  end_of_queue += sizeof(queueTask_t);
  parametersFill_t *task_parameters = (parametersFill_t *)end_of_queue;
  end_of_queue += sizeof(parametersFill_t);

  last_parameter = end_of_queue;
  task_parameters->x = x;
  task_parameters->y = y;
  task_parameters->width = width;
  task_parameters->height = height;
  task_parameters->color = ENDIAN_COLOR(color);
  task_parameters->count = width * height;

  *end_of_queue = TASK_END_OF_QUEUE;
  task->nextTask = end_of_queue;
  task->state = TASK_STATE_READY;
  task->type = TASK_FILL;

  if (!current_task) current_task = (uint8_t *)task;
}

void TFT_Queue::canvas(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  finish_sketch();

  queueTask_t *task = (queueTask_t *)end_of_queue;
  last_task = (uint8_t *) task;

  task->state = TASK_STATE_SKETCH;
  task->type = TASK_CANVAS;
  task->nextTask = nullptr;

  end_of_queue += sizeof(queueTask_t);
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)end_of_queue;
  end_of_queue += sizeof(parametersCanvas_t);

  last_parameter = end_of_queue;
  task_parameters->x = x;
  task_parameters->y = y;
  task_parameters->width = width;
  task_parameters->height = height;
  task_parameters->count = 0;

  if (!current_task) current_task = (uint8_t *)task;
}

void TFT_Queue::set_background(uint16_t color) {
  handle_queue_overflow(sizeof(parametersCanvasBackground_t));
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)last_task) + sizeof(queueTask_t));
  parametersCanvasBackground_t *parameters = (parametersCanvasBackground_t *)end_of_queue;
  last_parameter = end_of_queue;

  parameters->type = CANVAS_SET_BACKGROUND;
  parameters->color = ENDIAN_COLOR(color);

  end_of_queue += sizeof(parametersCanvasBackground_t);
  task_parameters->count++;
  parameters->nextParameter = end_of_queue;
}

#define QUEUE_SAFETY_FREE_SPACE 100

void TFT_Queue::handle_queue_overflow(uint16_t sizeNeeded) {
  if (uintptr_t(end_of_queue) + sizeNeeded + (QUEUE_SAFETY_FREE_SPACE) - uintptr_t(queue) >= TFT_QUEUE_SIZE) {
    end_of_queue = queue;
    ((parametersCanvasText_t *)last_parameter)->nextParameter = end_of_queue;
  }
}

void TFT_Queue::add_text(uint16_t x, uint16_t y, uint16_t color, const uint8_t *string, uint16_t maxWidth) {
  handle_queue_overflow(sizeof(parametersCanvasText_t) + maxWidth);
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)last_task) + sizeof(queueTask_t));
  parametersCanvasText_t *parameters = (parametersCanvasText_t *)end_of_queue;
  last_parameter = end_of_queue;

  const uint8_t *pointer = string;

  parameters->type = CANVAS_ADD_TEXT;
  parameters->x = x;
  parameters->y = y;
  parameters->color = ENDIAN_COLOR(color);
  parameters->stringLength = 0;
  parameters->maxWidth = maxWidth;

  end_of_queue += sizeof(parametersCanvasText_t);

  uint16_t *character = (uint16_t *)end_of_queue;

  lchar_t wc;
  for (;;) {
    pointer = get_utf8_value_cb(pointer, read_byte_ram, wc);
    *character++ = uint16_t(wc);
    if (uint16_t(wc) == 0) break;
    parameters->stringLength++;
  }
  end_of_queue = (uint8_t*)character;

  parameters->nextParameter = end_of_queue;
  task_parameters->count++;
}

void TFT_Queue::add_text(uint16_t x, uint16_t y, uint16_t color, const uint16_t *string, uint16_t maxWidth) {
  handle_queue_overflow(sizeof(parametersCanvasText_t) + maxWidth);
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)last_task) + sizeof(queueTask_t));
  parametersCanvasText_t *parameters = (parametersCanvasText_t *)end_of_queue;
  last_parameter = end_of_queue;

  const uint16_t *pointer = string;

  parameters->type = CANVAS_ADD_TEXT;
  parameters->x = x;
  parameters->y = y;
  parameters->color = ENDIAN_COLOR(color);
  parameters->stringLength = 0;
  parameters->maxWidth = maxWidth;

  end_of_queue += sizeof(parametersCanvasText_t);

  uint16_t *character = (uint16_t *)end_of_queue;
  // TODO: Deal with maxWidth
  while ((*character++ = *pointer++) != 0);
  end_of_queue = (uint8_t *)character;

  parameters->nextParameter = end_of_queue;
  parameters->stringLength = pointer - string;
  task_parameters->count++;
}

void TFT_Queue::add_image(int16_t x, int16_t y, MarlinImage image, uint16_t *colors) {
  handle_queue_overflow(sizeof(parametersCanvasImage_t));
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)last_task) + sizeof(queueTask_t));
  parametersCanvasImage_t *parameters = (parametersCanvasImage_t *)end_of_queue;
  last_parameter = end_of_queue;

  parameters->type = CANVAS_ADD_IMAGE;
  parameters->x = x;
  parameters->y = y;
  parameters->image = image;

  end_of_queue += sizeof(parametersCanvasImage_t);
  task_parameters->count++;
  parameters->nextParameter = end_of_queue;

  colorMode_t color_mode = images[image].colorMode;

  if (color_mode == HIGHCOLOR) return;

  uint16_t *color = (uint16_t *)end_of_queue;
  uint8_t color_count = 0;

  switch (color_mode) {
    case GREYSCALE1: color_count =  1; break;
    case GREYSCALE2: color_count =  3; break;
    case GREYSCALE4: color_count = 15; break;
    default: break;
  }

  uint16_t tmp;
  while (color_count--) {
    tmp = *colors++;
    *color++ = ENDIAN_COLOR(tmp);
  }

  end_of_queue = (uint8_t *)color;
  parameters->nextParameter = end_of_queue;
}

uint16_t gradient(uint16_t colorA, uint16_t colorB, uint16_t factor) {
  uint16_t red, green, blue;

  red   = (  RED(colorA) * factor +   RED(colorB) * (256 - factor)) >> 8;
  green = (GREEN(colorA) * factor + GREEN(colorB) * (256 - factor)) >> 8;
  blue  = ( BLUE(colorA) * factor +  BLUE(colorB) * (256 - factor)) >> 8;

  return RGB(red, green, blue);
}

void TFT_Queue::add_image(int16_t x, int16_t y, MarlinImage image, uint16_t color_main, uint16_t color_background, uint16_t color_shadow) {
  uint16_t colors[16];
  colorMode_t color_mode = images[image].colorMode;
  uint16_t i;

  switch (color_mode) {
    case GREYSCALE1:
      colors[1] = color_main;
      break;
    case GREYSCALE2:
      for (i = 1; i < 4; i++)
        colors[i] = gradient(color_main, color_background, (i << 8) / 3);
      break;
    case GREYSCALE4:
      for (i = 1; i < 8; i++)
        colors[i] = gradient(color_background, color_shadow, i << 5);
      for (i = 8; i < 16; i++)
        colors[i] = gradient(color_main, color_background, ((i - 8) << 8) / 7);
      break;
    default:
      break;
  }

  add_image(x, y, image, colors + 1);
}

void TFT_Queue::add_bar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  handle_queue_overflow(sizeof(parametersCanvasBar_t));
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)last_task) + sizeof(queueTask_t));
  parametersCanvasBar_t *parameters = (parametersCanvasBar_t *)end_of_queue;
  last_parameter = end_of_queue;

  parameters->type = CANVAS_ADD_BAR;
  parameters->x = x;
  parameters->y = y;
  parameters->width = width;
  parameters->height = height;
  parameters->color = ENDIAN_COLOR(color);

  end_of_queue += sizeof(parametersCanvasBar_t);
  task_parameters->count++;
  parameters->nextParameter = end_of_queue;
}

void TFT_Queue::add_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  handle_queue_overflow(sizeof(parametersCanvasRectangle_t));
  parametersCanvas_t *task_parameters = (parametersCanvas_t *)(((uint8_t *)last_task) + sizeof(queueTask_t));
  parametersCanvasRectangle_t *parameters = (parametersCanvasRectangle_t *)end_of_queue;
  last_parameter = end_of_queue;

  parameters->type = CANVAS_ADD_RECT;
  parameters->x = x;
  parameters->y = y;
  parameters->width = width;
  parameters->height = height;
  parameters->color = ENDIAN_COLOR(color);

  end_of_queue += sizeof(parametersCanvasRectangle_t);
  task_parameters->count++;
  parameters->nextParameter = end_of_queue;
}

#endif // HAS_GRAPHICAL_TFT
