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

/**
 * @brief Circular Queue class
 * @details Implementation of the classic ring buffer data structure
 */
template<typename T, int N>
class CircularQueue {
  private:

    /**
     * @brief Buffer structure
     * @details This structure consolidates all the overhead required to handle
     * a circular queue such as the pointers and the buffer vector.
     */
    struct buffer_t {
      uint8_t head;
      uint8_t tail;
      uint8_t size;
      uint8_t length;
      T queue[N];
    } buffer;

  public:
    /**
     * @brief Class constructor
     * @details This class requires two template parameters, T defines the type
     * of the items this queue will handle and N defines the maximum number of
     * items that can be stored on the queue.
     */
    CircularQueue<T, N>() {
      this->buffer.length = N;
      this->buffer.size = this->buffer.head = this->buffer.tail = 0;
    }

    /**
     * @brief Removes and returns a item from the queue
     * @details Removes the oldest item on the queue which is pointed by the
     * buffer_t head variable, this item is then returned to the caller.
     * @return type T item
     */
    T dequeue() {
      if (this->isEmpty()) return T();

      T const item = this->buffer.queue[this->buffer.head++];
      --this->buffer.size;

      if (this->buffer.head == this->buffer.length)
        this->buffer.head = 0;

      return item;
    }

    /**
     * @brief Adds an item to the queue
     * @details Adds a item to the queue on the location pointed by the buffer_t
     * tail vairable, will return false if there is no queue space available.
     *
     * @param item Item to be added to the queue
     * @return true if the operation was successfull
     */
    bool enqueue(T const &item) {
      if (this->isFull()) return false;

      this->buffer.queue[this->buffer.tail++] = item;
      ++this->buffer.size;

      if (this->buffer.tail == this->buffer.length)
        this->buffer.tail = 0;

      return true;
    }

    /**
     * @brief Checks if the queue has no items
     * @details Returns true if there are no items on the queue, false otherwise.
     * @return true if queue is empty
     */
    bool isEmpty() {
      return this->buffer.size == 0;
    }

    /**
     * @brief Checks if the queue is full
     * @details Returns true if the queue is full, false otherwise.
     * @return true if queue is full
     */
    bool isFull() {
      return this->buffer.size == this->buffer.length;
    }

    /**
     * @brief Gets the queue size
     * @details Returns the maximum number of items a queue can have.
     * @return the queue lenght
     */
    uint8_t length() {
      return this->buffer.length;
    }

    /**
     * @brief Gets the next item from the queue without removing it
     * @details Returns the next item on the queue but the item is not removed
     * from the queue nor the pointers updated.
     * @return the queue size
     */
    uint8_t peek() {
      return this->buffer.queue[this->buffer.head];
    }

    /**
     * @brief Gets the number of items on the queue
     * @details Returns the current number of items stored on the queue.
     * @return type T item
     */
    uint8_t size() {
      return this->buffer.size;
    }
};

#endif
