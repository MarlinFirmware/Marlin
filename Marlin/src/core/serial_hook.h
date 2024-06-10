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

// A mask containing a bitmap of the serial port to act upon
// This is written to ensure a serial index is never used as a serial mask
class SerialMask {
  uint8_t mask;

  // This constructor is private to ensure you can't convert an index to a mask
  // The compiler will stop here if you are mixing index and mask in your code.
  // If you need to, you'll have to use the explicit static "from" method here
  SerialMask(const serial_index_t);

public:
  inline constexpr bool enabled(const SerialMask PortMask) const    { return mask & PortMask.mask; }
  inline constexpr SerialMask combine(const SerialMask other) const { return SerialMask(mask | other.mask); }
  inline constexpr SerialMask operator<< (const int offset) const   { return SerialMask(mask << offset); }
  static SerialMask from(const serial_index_t index) {
    if (index.valid()) return SerialMask(_BV(index.index));
    return SerialMask(0); // A invalid index mean no output
  }

  constexpr SerialMask(const uint8_t mask) : mask(mask) {}
  constexpr SerialMask(const SerialMask &rs) : mask(rs.mask) {} // Can't use = default here since not all frameworks support this

  SerialMask& operator=(const SerialMask &rs) { mask = rs.mask; return *this; }

  static constexpr uint8_t All = 0xFF;
};

// The most basic serial class: it dispatch to the base serial class with no hook whatsoever. This will compile to nothing but the base serial class
template <class SerialT>
struct BaseSerial : public SerialBase< BaseSerial<SerialT> >, public SerialT {
  typedef SerialBase< BaseSerial<SerialT> > BaseClassT;

  // It's required to implement a write method here to help compiler disambiguate what method to call
  using SerialT::write;
  using SerialT::flush;

  void msgDone() {}

  // We don't care about indices here, since if one can call us, it's the right index anyway
  int available(serial_index_t) { return (int)SerialT::available(); }
  int read(serial_index_t)      { return (int)SerialT::read(); }
  bool connected()              { return CALL_IF_EXISTS(bool, static_cast<SerialT*>(this), connected);; }
  void flushTX()                { CALL_IF_EXISTS(void, static_cast<SerialT*>(this), flushTX); }

  SerialFeature features(serial_index_t index) const { return CALL_IF_EXISTS(SerialFeature, static_cast<const SerialT*>(this), features, index);  }

  // Two implementations of the same method exist in both base classes so indicate the right one
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
  void flush()                      { if (condition) out.flush();  }
  void begin(long br)               { out.begin(br); }
  void end()                        { out.end(); }

  void msgDone() {}
  bool connected()          { return CALL_IF_EXISTS(bool, &out, connected); }
  void flushTX()            { CALL_IF_EXISTS(void, &out, flushTX); }

  int available(serial_index_t)   { return (int)out.available(); }
  int read(serial_index_t)        { return (int)out.read(); }
  int available()                 { return (int)out.available(); }
  int read()                      { return (int)out.read(); }
  SerialFeature features(serial_index_t index) const  { return CALL_IF_EXISTS(SerialFeature, &out, features, index);  }

  ConditionalSerial(bool & conditionVariable, SerialT & out, const bool e) : BaseClassT(e), condition(conditionVariable), out(out) {}
};

// A simple forward class that taking a reference to an existing serial instance (likely created in their respective framework)
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
  void flushTX()                { CALL_IF_EXISTS(void, &out, flushTX); }

  int available(serial_index_t) { return (int)out.available(); }
  int read(serial_index_t)      { return (int)out.read(); }
  int available()               { return (int)out.available(); }
  int read()                    { return (int)out.read(); }
  SerialFeature features(serial_index_t index) const  { return CALL_IF_EXISTS(SerialFeature, &out, features, index);  }

  ForwardSerial(const bool e, SerialT & out) : BaseClassT(e), out(out) {}
};

// A class that can be hooked and unhooked at runtime, useful to capture the output of the serial interface
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

  int available(serial_index_t)  { return (int)SerialT::available(); }
  int read(serial_index_t)       { return (int)SerialT::read(); }
  using SerialT::available;
  using SerialT::read;
  using SerialT::flush;
  using SerialT::begin;
  using SerialT::end;

  using BaseClassT::print;
  using BaseClassT::println;

  // Underlying implementation might use Arduino's bool operator
  bool connected() {
    return Private::HasMember_connected<SerialT>::value
      ? CALL_IF_EXISTS(bool, static_cast<SerialT*>(this), connected)
      : static_cast<SerialT*>(this)->operator bool();
  }

  void flushTX() { CALL_IF_EXISTS(void, static_cast<SerialT*>(this), flushTX); }

  // Append Hookable for this class
  SerialFeature features(serial_index_t index) const  { return SerialFeature::Hookable | CALL_IF_EXISTS(SerialFeature, static_cast<const SerialT*>(this), features, index);  }

  void setHook(WriteHook writeHook=0, EndOfMessageHook eofHook=0, void * userPointer=0) {
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
  RuntimeSerial(const bool e, Args... args) : BaseClassT(e), SerialT(args...), writeHook(0), eofHook(0), userPointer(0) {}
};

#define _S_CLASS(N) class Serial##N##T,
#define _S_NAME(N) Serial##N##T,

template < REPEAT(NUM_SERIAL, _S_CLASS) const uint8_t offset=0, const uint8_t step=1 >
struct MultiSerial : public SerialBase< MultiSerial< REPEAT(NUM_SERIAL, _S_NAME) offset, step > > {
  typedef SerialBase< MultiSerial< REPEAT(NUM_SERIAL, _S_NAME) offset, step > > BaseClassT;

  #undef _S_CLASS
  #undef _S_NAME

  SerialMask portMask;

  #define _S_DECLARE(N) Serial##N##T & serial##N;
  REPEAT(NUM_SERIAL, _S_DECLARE);
  #undef _S_DECLARE

  static constexpr uint8_t Usage = _BV(step) - 1; // A bit mask containing 'step' bits

  #define _OUT_PORT(N) (Usage << (offset + (step * N))),
  static constexpr uint8_t output[] = { REPEAT(NUM_SERIAL, _OUT_PORT) };
  #undef _OUT_PORT

  #define _OUT_MASK(N) | output[N]
  static constexpr uint8_t ALL = 0 REPEAT(NUM_SERIAL, _OUT_MASK);
  #undef _OUT_MASK

  NO_INLINE void write(uint8_t c) {
    #define _S_WRITE(N) if (portMask.enabled(output[N])) serial##N.write(c);
    REPEAT(NUM_SERIAL, _S_WRITE);
    #undef _S_WRITE
  }
  NO_INLINE void msgDone() {
    #define _S_DONE(N) if (portMask.enabled(output[N])) serial##N.msgDone();
    REPEAT(NUM_SERIAL, _S_DONE);
    #undef _S_DONE
  }
  int available(serial_index_t index) {
    uint8_t pos = offset;
    #define _S_AVAILABLE(N) if (index.within(pos, pos + step - 1)) return serial##N.available(index); else pos += step;
    REPEAT(NUM_SERIAL, _S_AVAILABLE);
    #undef _S_AVAILABLE
    return false;
  }
  int read(serial_index_t index) {
    uint8_t pos = offset;
    #define _S_READ(N) if (index.within(pos, pos + step - 1)) return serial##N.read(index); else pos += step;
    REPEAT(NUM_SERIAL, _S_READ);
    #undef _S_READ
    return -1;
  }
  void begin(const long br) {
    #define _S_BEGIN(N) if (portMask.enabled(output[N])) serial##N.begin(br);
    REPEAT(NUM_SERIAL, _S_BEGIN);
    #undef _S_BEGIN
  }
  void end() {
    #define _S_END(N) if (portMask.enabled(output[N])) serial##N.end();
    REPEAT(NUM_SERIAL, _S_END);
    #undef _S_END
  }
  bool connected() {
    bool ret = true;
    #define _S_CONNECTED(N) if (portMask.enabled(output[N]) && !CALL_IF_EXISTS(bool, &serial##N, connected)) ret = false;
    REPEAT(NUM_SERIAL, _S_CONNECTED);
    #undef _S_CONNECTED
    return ret;
  }

  using BaseClassT::available;
  using BaseClassT::read;

  // Redirect flush
  NO_INLINE void flush() {
    #define _S_FLUSH(N) if (portMask.enabled(output[N])) serial##N.flush();
    REPEAT(NUM_SERIAL, _S_FLUSH);
    #undef _S_FLUSH
  }
  NO_INLINE void flushTX() {
    #define _S_FLUSHTX(N) if (portMask.enabled(output[N])) CALL_IF_EXISTS(void, &serial0, flushTX);
    REPEAT(NUM_SERIAL, _S_FLUSHTX);
    #undef _S_FLUSHTX
  }

  // Forward feature queries
  SerialFeature features(serial_index_t index) const {
    uint8_t pos = offset;
    #define _S_FEATURES(N) if (index.within(pos, pos + step - 1)) return serial##N.features(index); else pos += step;
    REPEAT(NUM_SERIAL, _S_FEATURES);
    #undef _S_FEATURES
    return SerialFeature::None;
  }

  #define _S_REFS(N) Serial##N##T & serial##N,
  #define _S_INIT(N) ,serial##N (serial##N)

  MultiSerial(REPEAT(NUM_SERIAL, _S_REFS) const SerialMask mask=ALL, const bool e=false)
    : BaseClassT(e), portMask(mask) REPEAT(NUM_SERIAL, _S_INIT) {}

};

// Build the actual serial object depending on current configuration
#define Serial1Class TERN(SERIAL_RUNTIME_HOOK, RuntimeSerial, BaseSerial)
#define ForwardSerial1Class TERN(SERIAL_RUNTIME_HOOK, RuntimeSerial, ForwardSerial)
#if HAS_MULTI_SERIAL
  #define Serial2Class ConditionalSerial
  #if NUM_SERIAL >= 3
    #define Serial3Class ConditionalSerial
  #endif
#endif
