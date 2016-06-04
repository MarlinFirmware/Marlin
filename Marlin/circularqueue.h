/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __CIRCULARQUEUE_H__
#define __CIRCULARQUEUE_H__

#include <Arduino.h>

template<typename T> class CircularQueue {
  private:

    struct buffer_t {
      uint8_t head;
      uint8_t tail;
      uint8_t size;
      uint8_t length;
      T* queue;
    } buffer;

  public:
    CircularQueue<T>(uint8_t const &length) {
      this->buffer.length = length;
      this->buffer.queue = new T[sizeof(T) * length];
      this->buffer.size = this->buffer.head = this->buffer.tail = 0;
    }

    bool isEmpty() {
      return this->buffer.size == 0 ? true : false;
    }

    bool isFull() {
      return this->buffer.size == this->buffer.length ? true : false;
    }

    T dequeue() {
      if (this->isEmpty()) return T();

      T const item = this->buffer.queue[this->buffer.head];
      ++this->buffer.head;
      --this->buffer.size;

      if (this->buffer.head == this->buffer.length)
        this->buffer.head = 0;

      return item;
    }

    uint8_t peek() {
      return this->buffer.queue[buffer.head];
    }

    bool enqueue(T const &item) {
      if (this->isFull()) return false;

      this->buffer.queue[this->buffer.tail] = item;
      ++this->buffer.size;
      ++this->buffer.tail;

      if (this->buffer.tail == this->buffer.length)
        this->buffer.tail = 0;

      return true;
    }
};

#endif
