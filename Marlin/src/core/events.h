/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2021 X-Ryl669 boite.pour.spam@gmail.com
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


#if MoreThan32OrDynamicEvents

  /** The event types */
  typedef unsigned int EventType;
  constexpr size_t EventTypeCount = 34;

  /** An event listener. */
  class EventListener
  {
    /** The next listener in the chained list */
    EventListener * next;
    #if ENABLED(MARLIN_DEV_MODE)
      /** The listener name, used for debugging purpose */
      const char * name;
    #endif

    // Interface
  public:
    /** The event listener unique callback that's called upon registered events are broadcasted */
    virtual bool on(EventType event, void * parameter) = 0;

    EventListener(const char * name = nullptr) : next(0)
      #if ENABLED(MARLIN_DEV_MODE)
        , name(name)
      #endif
    {}
    virtual ~EventListener() {}

    // Helpers
  private:
    /** Append another listener in the chain */
    void linkWith(EventListener * listener) { EventListener * n = this; while (n->next) n = n->next; n->next = listener; }

    friend struct EventBroadcaster;
  };


  /** The broadcaster singleton */
  struct EventBroadcaster
  {
    // Register a listener for an event
    void registerListener(EventType event, EventListener * listener) {
      if (!listeners[event]) listeners[event] = listener;
      else listeners[event]->linkWith(listener);
    }
    // Emit an event
    static void emit(EventType event, void * parameter = nullptr) {
      EventListener * listener = listeners[event];
      while (listener) {
        if (!listener->on(event, parameter)) break;
        listener = listener->next;
      }
    }
    #if ENABLED(MARLIN_DEV_MODE)
      void dumpListeners() {
        for (size_t i = 0; i < EventType::Count; i++) {
          SERIAL_ECHOPAIR("Event: ", i);
          EventListener * listener = listeners[i];
          while (listener) {
            SERIAL_ECHOPAIR(listener->name, ", ");
            listener = listener->next;
          }
          SERIAL_ECHOLN();
        }
      }
    #endif

  private:
    // First possibility: A table of possible listener
    EventListener * listeners[EventTypeCount];
  };

  EventBroadcaster events;
#elif EventTypeIsBitMask
  /** The possible event to listen upon. */
  enum class EventType
  {
    PrinterHalted     = 0x00000001,
    PrinterConnected  = 0x00000002,
    PrinterBurning    = 0x00000004,
  };

  ENUM_FLAGS(EventType); // See #21318, this adds the | & and other bit manipulation operators so we can treat the enum as an integer without casting

  /** An event listener. */
  class EventListener
  {
    /** The next listener in the chained list */
    EventListener * next;
    /** The event this listener is interested in */
    uint32          eventMask;
    #if ENABLED(MARLIN_DEV_MODE)
      /** The listener name, used for debugging purpose */
      const char * name;
    #endif

    // Interface
  public:
    /** The event listener unique callback that's called upon registered events are broadcasted */
    virtual bool on(EventType event, void * parameter) = 0;

    EventListener(const char * name = nullptr) : next(0), eventMask(0)
      #if ENABLED(MARLIN_DEV_MODE)
        , name(name)
      #endif
    {}
    virtual ~EventListener() {}

    // Helpers
  private:
    /** Append another listener in the chain */
    void linkWith(EventListener * listener) { EventListener * n = this; while (n->next) n = n->next; n->next = listener; }

    friend struct EventBroadcaster;
  };


  /** The broadcaster singleton */
  struct EventBroadcaster
  {
    // Register a listener for an event
    void registerListener(EventType event, EventListener * listener) {
      listener->eventMask |= event;

      if (!head) head = listener;
      else head->linkWith(listener);
    }
    // Emit an event
    static void emit(EventType event, void * parameter = nullptr) {
      EventListener * listener = head;
      while (listener) {
        if ((listener->eventMask & event) && !listener->on(event, parameter)) break;
        listener = listener->next;
      }
    }

  private:
    // First possibility: A table of possible listener
    EventListener * head;
  };

  EventBroadcaster events;
#endif
