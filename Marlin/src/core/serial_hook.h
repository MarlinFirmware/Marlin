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

#include "serial_base.h"

// The most basic serial class: it dispatch to the base serial class with no hook whatsoever. This will compile to nothing but the base serial class
template <class SerialT>
struct BaseSerial : public SerialBase< BaseSerial<SerialT> >, public SerialT {
  typedef SerialBase< BaseSerial<SerialT> > BaseClassT;

  // It's required to implement a write method here to help compiler disambiguate what method to call
  using SerialT::write;
  using SerialT::flush;

  void msgDone() {}

  bool available(uint8_t index) { return index == 0 && SerialT::available(); }
  int read(uint8_t index)       { return index == 0 ? SerialT::read() : -1; }
  bool connected()              { return CALL_IF_EXISTS(bool, static_cast<SerialT*>(this), connected);; }
  // We have 2 implementation of the same method in both base class, let's say which one we want
  using SerialT::available;
  using SerialT::read;
  using SerialT::begin;
  using SerialT::end;

  using BaseClassT::print;
  using BaseClassT::println;

  BaseSerial(const bool e) : BaseClassT(e) {}

  // Forward constructor
  template <typename... Args>
  BaseSerial(const bool e, Args... args) : BaseClassT(e), SerialT(args...) {}
};

// A serial with a condition checked at runtime for its output
// A bit less efficient than static dispatching but since it's only used for ethernet's serial output right now, it's ok.
template <class SerialT>
struct ConditionalSerial : public SerialBase< ConditionalSerial<SerialT> > {
  typedef SerialBase< ConditionalSerial<SerialT> > BaseClassT;

  bool    & condition;
  SerialT & out;
  NO_INLINE size_t write(uint8_t c) { if (condition) return out.write(c); return 0; }
  void flush()            { if (condition) out.flush();  }
  void begin(long br)     { out.begin(br); }
  void end()              { out.end(); }

  void msgDone() {}
  bool connected()              { return CALL_IF_EXISTS(bool, &out, connected); }

  bool available(uint8_t index) { return index == 0 && out.available(); }
  int read(uint8_t index)       { return index == 0 ? out.read() : -1; }
  using BaseClassT::available;
  using BaseClassT::read;

  ConditionalSerial(bool & conditionVariable, SerialT & out, const bool e) : BaseClassT(e), condition(conditionVariable), out(out) {}
};

// A simple foward class that taking a reference to an existing serial instance (likely created in their respective framework)
template <class SerialT>
struct ForwardSerial : public SerialBase< ForwardSerial<SerialT> > {
  typedef SerialBase< ForwardSerial<SerialT> > BaseClassT;

  SerialT & out;
  NO_INLINE size_t write(uint8_t c) { return out.write(c); }
  void flush()            { out.flush();  }
  void begin(long br)     { out.begin(br); }
  void end()              { out.end(); }

  void msgDone() {}
  // Existing instances implement Arduino's operator bool, so use that if it's available
  bool connected()              { return Private::HasMember_connected<SerialT>::value ? CALL_IF_EXISTS(bool, &out, connected) : (bool)out; }

  bool available(uint8_t index) { return index == 0 && out.available(); }
  int read(uint8_t index)       { return index == 0 ? out.read() : -1; }
  bool available()              { return out.available(); }
  int read()                    { return out.read(); }

  ForwardSerial(const bool e, SerialT & out) : BaseClassT(e), out(out) {}
};

// A class that's can be hooked and unhooked at runtime, useful to capturing the output of the serial interface
template <class SerialT>
struct RuntimeSerial : public SerialBase< RuntimeSerial<SerialT> >, public SerialT {
  typedef SerialBase< RuntimeSerial<SerialT> > BaseClassT;
  typedef void (*WriteHook)(void * userPointer, uint8_t c);
  typedef void (*EndOfMessageHook)(void * userPointer);

  WriteHook        writeHook;
  EndOfMessageHook eofHook;
  void *           userPointer;

  NO_INLINE size_t write(uint8_t c) {
    if (writeHook) writeHook(userPointer, c);
    return SerialT::write(c);
  }

  NO_INLINE void msgDone() {
    if (eofHook) eofHook(userPointer);
  }

  bool available(uint8_t index) { return index == 0 && SerialT::available(); }
  int read(uint8_t index)       { return index == 0 ? SerialT::read() : -1; }
  using SerialT::available;
  using SerialT::read;
  using SerialT::flush;
  using SerialT::begin;
  using SerialT::end;

  using BaseClassT::print;
  using BaseClassT::println;

  // Underlying implementation might use Arduino's bool operator
  bool connected() { 
    return Private::HasMember_connected<SerialT>::value ? CALL_IF_EXISTS(bool, static_cast<SerialT*>(this), connected) : static_cast<SerialT*>(this)->operator bool(); 
  }  

  void setHook(WriteHook writeHook = 0, EndOfMessageHook eofHook = 0, void * userPointer = 0) {
    // Order is important here as serial code can be called inside interrupts
    // When setting a hook, the user pointer must be set first so if writeHook is called as soon as it's set, it'll be valid
    if (userPointer) this->userPointer = userPointer;
    this->writeHook = writeHook;
    this->eofHook = eofHook;
    // Order is important here because of asynchronous access here
    // When unsetting a hook, the user pointer must be unset last so that any pending writeHook is still using the old pointer
    if (!userPointer) this->userPointer = 0;
  }

  RuntimeSerial(const bool e) : BaseClassT(e), writeHook(0), eofHook(0), userPointer(0) {}

  // Forward constructor
  template <typename... Args>
  RuntimeSerial(const bool e, Args... args) : BaseClassT(e), SerialT(args...) {}
};

// A class that's duplicating its output conditionally to 2 serial interface
template <class Serial0T, class Serial1T, const uint8_t offset = 0>
struct MultiSerial : public SerialBase< MultiSerial<Serial0T, Serial1T, offset> > {
  typedef SerialBase< MultiSerial<Serial0T, Serial1T, offset> > BaseClassT;

  uint8_t    portMask;
  Serial0T & serial0;
  Serial1T & serial1;

  enum Masks {
    FirstOutputMask   =  (1 << offset),
    SecondOutputMask  =  (1 << (offset + 1)),
    AllMask           = FirstOutputMask | SecondOutputMask,
  };

  NO_INLINE size_t write(uint8_t c) {
    size_t ret = 0;
    if (portMask & FirstOutputMask)   ret = serial0.write(c);
    if (portMask & SecondOutputMask)  ret = serial1.write(c) | ret;
    return ret;
  }
  NO_INLINE void msgDone() {
    if (portMask & FirstOutputMask)   serial0.msgDone();
    if (portMask & SecondOutputMask)  serial1.msgDone();
  }
  bool available(uint8_t index) {
    switch(index) {
      case 0 + offset: return serial0.available();
      case 1 + offset: return serial1.available();
      default: return false;
    }
  }
  NO_INLINE int read(uint8_t index) {
    switch(index) {
      case 0 + offset: return serial0.read();
      case 1 + offset: return serial1.read();
      default: return -1;
    }
  }
  void begin(const long br) {
    if (portMask & FirstOutputMask)   serial0.begin(br);
    if (portMask & SecondOutputMask)  serial1.begin(br);
  }
  void end() {
    if (portMask & FirstOutputMask)   serial0.end();
    if (portMask & SecondOutputMask)  serial1.end();
  }
  bool connected() {
    bool ret = true;
    if (portMask & FirstOutputMask)   ret = CALL_IF_EXISTS(bool, &serial0, connected);
    if (portMask & SecondOutputMask)  ret = ret && CALL_IF_EXISTS(bool, &serial1, connected);
    return ret;
  }

  using BaseClassT::available;
  using BaseClassT::read;

  // Redirect flush
  NO_INLINE void flush()      {
    if (portMask & FirstOutputMask)   serial0.flush();
    if (portMask & SecondOutputMask)  serial1.flush();
  }
  NO_INLINE void flushTX()    {
    if (portMask & FirstOutputMask)   CALL_IF_EXISTS(void, &serial0, flushTX);
    if (portMask & SecondOutputMask)  CALL_IF_EXISTS(void, &serial1, flushTX);
  }

  MultiSerial(Serial0T & serial0, Serial1T & serial1, int8_t mask = AllMask, const bool e = false) :
    BaseClassT(e),
    portMask(mask), serial0(serial0), serial1(serial1) {}
};

// Build the actual serial object depending on current configuration
#define Serial0Type TERN(SERIAL_RUNTIME_HOOK, RuntimeSerial, BaseSerial)
#define ForwardSerial0Type TERN(SERIAL_RUNTIME_HOOK, RuntimeSerial, ForwardSerial)
#ifdef HAS_MULTI_SERIAL
  #define Serial1Type ConditionalSerial
#endif
