/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
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

#include "number_format.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/e_parser.h"
#endif

// Used in multiple places
// You can build it but not manipulate it.
// There are only few places where it's required to access the underlying member: GCodeQueue, SerialMask and MultiSerial
struct serial_index_t {
  // A signed index, where -1 is a special case meaning no action (neither output or input)
  int8_t  index;

  // Check if the index is within the range [a ... b]
  constexpr inline bool within(const int8_t a, const int8_t b) const { return WITHIN(index, a, b); }
  constexpr inline bool valid() const { return WITHIN(index, 0, 7); } // At most, 8 bits

  // Construction is either from an index
  constexpr serial_index_t(const int8_t index) : index(index) {}

  // Default to "no index"
  constexpr serial_index_t() : index(-1) {}
};

// A simple feature list enumeration
enum class SerialFeature {
  None                = 0x00,
  MeatPack            = 0x01,   //!< Enabled when Meatpack is present
  BinaryFileTransfer  = 0x02,   //!< Enabled for BinaryFile transfer support (in the future)
  Virtual             = 0x04,   //!< Enabled for virtual serial port (like Telnet / Websocket / ...)
  Hookable            = 0x08,   //!< Enabled if the serial class supports a setHook method
};
ENUM_FLAGS(SerialFeature);

// flushTX is not implemented in all HAL, so use SFINAE to call the method where it is.
CALL_IF_EXISTS_IMPL(void, flushTX);
CALL_IF_EXISTS_IMPL(bool, connected, true);
CALL_IF_EXISTS_IMPL(SerialFeature, features, SerialFeature::None);

// Using Curiously-Recurring Template Pattern here to avoid virtual table cost when compiling.
// Since the real serial class is known at compile time, this results in the compiler writing
// a completely efficient code.
template <class Child>
struct SerialBase : public NumberFormatter< SerialBase<Child> > {
  #if ENABLED(EMERGENCY_PARSER)
    const bool ep_enabled;
    EmergencyParser::State emergency_state;
    inline bool emergency_parser_enabled() { return ep_enabled; }
    SerialBase(bool ep_capable) : ep_enabled(ep_capable), emergency_state(EmergencyParser::State::EP_RESET) {}
  #else
    SerialBase(const bool) {}
  #endif

  // Used by NumberFormatter after each number formatting
  void writeDone() {}

  // Syntaxic sugar
  FORCE_INLINE Child * SerialChild() { return static_cast<Child*>(this); }
  FORCE_INLINE const Child * SerialChild() const { return static_cast<const Child*>(this); }


  // Static dispatch methods below:
  // The most important method here is where it all ends to:
  size_t write(uint8_t c)           { return SerialChild()->write(c); }

  // Called when the parser finished processing an instruction, usually build to nothing
  void msgDone() const              { SerialChild()->msgDone(); }

  // Called on initialization
  void begin(const long baudRate)   { SerialChild()->begin(baudRate); }

  // Called on destruction
  void end()                        { SerialChild()->end(); }

  /** Check for available data from the port
      @param index  The port index, usually 0 */
  int available(serial_index_t index=0) const { return SerialChild()->available(index); }

  /** Read a value from the port
      @param index  The port index, usually 0 */
  int read(serial_index_t index=0)        { return SerialChild()->read(index); }

  /** Combine the features of this serial instance and return it
      @param index  The port index, usually 0 */
  SerialFeature features(serial_index_t index=0) const { return SerialChild()->features(index);  }

  // Check if the serial port has a feature
  bool has_feature(serial_index_t index, SerialFeature flag) const { return (features(index) & flag) != SerialFeature::None; }

  // Check if the serial port is connected (usually bypassed)
  bool connected() const            { return SerialChild()->connected(); }

  // Redirect flush
  void flush()                      { SerialChild()->flush(); }

  // Not all implementation have a flushTX, so let's call them only if the child has the implementation
  void flushTX()                    { CALL_IF_EXISTS(void, SerialChild(), flushTX); }

  // Glue code here
  FORCE_INLINE void write(const char *str)                    { while (*str) write(*str++); }
  FORCE_INLINE void write(const uint8_t *buffer, size_t size) { while (size--) write(*buffer++); }
  FORCE_INLINE void print(const char *str)                    { write(str); }

  FORCE_INLINE void println(const char s[])                  { print(s); println(); }
  FORCE_INLINE void println(char c, PrintBase base)          { print(c, base); println(); }
  FORCE_INLINE void println(unsigned char c, PrintBase base) { print(c, base); println(); }
  FORCE_INLINE void println(int c, PrintBase base)           { print(c, base); println(); }
  FORCE_INLINE void println(unsigned int c, PrintBase base)  { print(c, base); println(); }
  FORCE_INLINE void println(long c, PrintBase base)          { print(c, base); println(); }
  FORCE_INLINE void println(unsigned long c, PrintBase base) { print(c, base); println(); }
  FORCE_INLINE void println(double c, int digits)            { print(c, digits); println(); }
  FORCE_INLINE void println()                                { write('\r'); write('\n'); }

  // Forward the call to the former's method
  FORCE_INLINE void println(char c)                { println(c, PrintBase::Dec); }
  FORCE_INLINE void println(unsigned char c)       { println(c, PrintBase::Dec); }
  FORCE_INLINE void println(int c)                 { println(c, PrintBase::Dec); }
  FORCE_INLINE void println(unsigned int c)        { println(c, PrintBase::Dec); }
  FORCE_INLINE void println(unsigned long c)       { println(c, PrintBase::Dec); }
  FORCE_INLINE void println(long c)                { println(c, PrintBase::Dec); }
  FORCE_INLINE void println(double c)              { println(c, 2); }

  // Inject print methods here
  using NumberFormatter< SerialBase<Child> >::print;
};

// All serial instances will be built by chaining the features required
// for the function in the form of a template type definition.
