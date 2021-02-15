/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
/**
 * @file
 * @brief Fast Digital Pin functions
 *
 * @defgroup digitalPin Fast Pin I/O
 * @details  Fast Digital I/O functions and template class.
 * @{
 */
#ifndef DigitalPin_h
#define DigitalPin_h
#if defined(__AVR__) || defined(DOXYGEN)
#include <avr/io.h>
/** GpioPinMap type */
struct GpioPinMap_t {
  volatile uint8_t* pin;   /**< address of PIN for this pin */
  volatile uint8_t* ddr;   /**< address of DDR for this pin */
  volatile uint8_t* port;  /**< address of PORT for this pin */
  uint8_t mask;            /**< bit mask for this pin */
};

/** Initializer macro. */
#define GPIO_PIN(reg, bit) {&PIN##reg, &DDR##reg, &PORT##reg, 1 << bit}

// Include pin map for current board.
#include "boards/GpioPinMap.h"
//------------------------------------------------------------------------------
/** generate bad pin number error */
void badPinNumber(void)
  __attribute__((error("Pin number is too large or not a constant")));
//------------------------------------------------------------------------------
/** Check for valid pin number
 * @param[in] pin Number of pin to be checked.
 */
static inline __attribute__((always_inline))
void badPinCheck(uint8_t pin) {
  if (!__builtin_constant_p(pin) || pin >= NUM_DIGITAL_PINS) {
     badPinNumber();
  }
}
//------------------------------------------------------------------------------
/** DDR register address
 * @param[in] pin Arduino pin number
 * @return register address
 */
static inline __attribute__((always_inline))
volatile uint8_t* ddrReg(uint8_t pin) {
  badPinCheck(pin);
  return GpioPinMap[pin].ddr;
}
//------------------------------------------------------------------------------
/** Bit mask for pin
 * @param[in] pin Arduino pin number
 * @return mask
 */
static inline __attribute__((always_inline))
uint8_t pinMask(uint8_t pin) {
  badPinCheck(pin);
  return GpioPinMap[pin].mask;
}
//------------------------------------------------------------------------------
/** PIN register address
 * @param[in] pin Arduino pin number
 * @return register address
 */
static inline __attribute__((always_inline))
volatile uint8_t* pinReg(uint8_t pin) {
  badPinCheck(pin);
  return GpioPinMap[pin].pin;
}
//------------------------------------------------------------------------------
/** PORT register address
 * @param[in] pin Arduino pin number
 * @return register address
 */
static inline __attribute__((always_inline))
volatile uint8_t* portReg(uint8_t pin) {
  badPinCheck(pin);
  return GpioPinMap[pin].port;
}
//------------------------------------------------------------------------------
/** Fast write helper.
 * @param[in] address I/O register address
 * @param[in] mask bit mask for pin
 * @param[in] level value for bit
 */
static inline __attribute__((always_inline))
void fastBitWriteSafe(volatile uint8_t* address, uint8_t mask, bool level) {
  uint8_t s;
  if (address > reinterpret_cast<uint8_t*>(0X3F)) {
    s = SREG;
    cli();
  }
  if (level) {
    *address |= mask;
  } else {
    *address &= ~mask;
  }
  if (address > reinterpret_cast<uint8_t*>(0X3F)) {
    SREG = s;
  }
}
//------------------------------------------------------------------------------
/** Read pin value.
 * @param[in] pin Arduino pin number
 * @return value read
 */
static inline __attribute__((always_inline))
bool fastDigitalRead(uint8_t pin) {
  return *pinReg(pin) & pinMask(pin);
}
//------------------------------------------------------------------------------
/** Toggle a pin.
 * @param[in] pin Arduino pin number
 *
 * If the pin is in output mode toggle the pin level.
 * If the pin is in input mode toggle the state of the 20K pullup.
 */
static inline __attribute__((always_inline))
void fastDigitalToggle(uint8_t pin) {
    if (pinReg(pin) > reinterpret_cast<uint8_t*>(0X3F)) {
      // must write bit to high address port
      *pinReg(pin) = pinMask(pin);
    } else {
      // will compile to sbi and PIN register will not be read.
      *pinReg(pin) |= pinMask(pin);
    }
}
//------------------------------------------------------------------------------
/** Set pin value.
 * @param[in] pin Arduino pin number
 * @param[in] level value to write
 */
static inline __attribute__((always_inline))
void fastDigitalWrite(uint8_t pin, bool level) {
  fastBitWriteSafe(portReg(pin), pinMask(pin), level);
}
//------------------------------------------------------------------------------
/** Write the DDR register.
 * @param[in] pin Arduino pin number
 * @param[in] level value to write
 */
static inline __attribute__((always_inline))
void fastDdrWrite(uint8_t pin, bool level) {
  fastBitWriteSafe(ddrReg(pin), pinMask(pin), level);
}
//------------------------------------------------------------------------------
/** Set pin mode.
 * @param[in] pin Arduino pin number
 * @param[in] mode INPUT, OUTPUT, or INPUT_PULLUP.
 *
 * The internal pullup resistors will be enabled if mode is INPUT_PULLUP
 * and disabled if the mode is INPUT.
 */
static inline __attribute__((always_inline))
void fastPinMode(uint8_t pin, uint8_t mode) {
  fastDdrWrite(pin, mode == OUTPUT);
  if (mode != OUTPUT) {
    fastDigitalWrite(pin, mode == INPUT_PULLUP);
  }
}
#else  // defined(__AVR__)
#if defined(CORE_TEENSY)
//------------------------------------------------------------------------------
/** read pin value
 * @param[in] pin Arduino pin number
 * @return value read
 */
static inline __attribute__((always_inline))
bool fastDigitalRead(uint8_t pin) {
  return *portInputRegister(pin);
}
//------------------------------------------------------------------------------
/** Set pin value
 * @param[in] pin Arduino pin number
 * @param[in] level value to write
 */
static inline __attribute__((always_inline))
void fastDigitalWrite(uint8_t pin, bool value) {
  if (value) {
    *portSetRegister(pin) = 1;
  } else {
    *portClearRegister(pin) = 1;
  }
}
#elif defined(__SAM3X8E__) || defined(__SAM3X8H__)
//------------------------------------------------------------------------------
/** read pin value
 * @param[in] pin Arduino pin number
 * @return value read
 */
static inline __attribute__((always_inline))
bool fastDigitalRead(uint8_t pin) {
  return g_APinDescription[pin].pPort->PIO_PDSR & g_APinDescription[pin].ulPin;
}
//------------------------------------------------------------------------------
/** Set pin value
 * @param[in] pin Arduino pin number
 * @param[in] level value to write
 */
static inline __attribute__((always_inline))
void fastDigitalWrite(uint8_t pin, bool value) {
  if (value) {
    g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  } else {
    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
  }
}
#elif defined(ESP8266)
//------------------------------------------------------------------------------
/** Set pin value
 * @param[in] pin Arduino pin number
 * @param[in] val value to write
 */
static inline __attribute__((always_inline))
void fastDigitalWrite(uint8_t pin, uint8_t val) {
  if (pin < 16) {
    if (val) {
      GPOS = (1 << pin);
    } else {
      GPOC = (1 << pin);
    }
  } else if (pin == 16) {
    if (val) {
      GP16O |= 1;
    } else {
      GP16O &= ~1;
    }
  }
}
//------------------------------------------------------------------------------
/** Read pin value
 * @param[in] pin Arduino pin number
 * @return value read
 */
static inline __attribute__((always_inline))
bool fastDigitalRead(uint8_t pin) {
  if (pin < 16) {
    return GPIP(pin);
  } else if (pin == 16) {
    return GP16I & 0x01;
  }
  return 0;
}
#else  // CORE_TEENSY
//------------------------------------------------------------------------------
inline void fastDigitalWrite(uint8_t pin, bool value) {
  digitalWrite(pin, value);
}
//------------------------------------------------------------------------------
inline bool fastDigitalRead(uint8_t pin) {
  return digitalRead(pin);
}
#endif  // CORE_TEENSY
//------------------------------------------------------------------------------
inline void fastDigitalToggle(uint8_t pin) {
  fastDigitalWrite(pin, !fastDigitalRead(pin));
}
//------------------------------------------------------------------------------
inline void fastPinMode(uint8_t pin, uint8_t mode) {
  pinMode(pin, mode);
}
#endif  // __AVR__
//------------------------------------------------------------------------------
/** set pin configuration
 * @param[in] pin Arduino pin number
 * @param[in] mode mode INPUT or OUTPUT.
 * @param[in] level If mode is output, set level high/low.
 *                  If mode is input, enable or disable the pin's 20K pullup.
 */
#define fastPinConfig(pin, mode, level)\
  {fastPinMode(pin, mode); fastDigitalWrite(pin, level);}
//==============================================================================
/**
 * @class DigitalPin
 * @brief Fast digital port I/O
 */
template<uint8_t PinNumber>
class DigitalPin {
 public:
  //----------------------------------------------------------------------------
  /** Constructor */
  DigitalPin() {}
  //----------------------------------------------------------------------------
  /** Asignment operator.
   * @param[in] value If true set the pin's level high else set the
   *  pin's level low.
   *
   * @return This DigitalPin instance.
   */
  inline DigitalPin & operator = (bool value) __attribute__((always_inline)) {
    write(value);
    return *this;
  }
  //----------------------------------------------------------------------------
  /** Parenthesis operator.
   * @return Pin's level
   */
  inline operator bool () const __attribute__((always_inline)) {
    return read();
  }
  //----------------------------------------------------------------------------
  /** Set pin configuration.
   * @param[in] mode: INPUT or OUTPUT.
   * @param[in] level If mode is OUTPUT, set level high/low.
   *                  If mode is INPUT, enable or disable the pin's 20K pullup.
   */
  inline __attribute__((always_inline))
  void config(uint8_t mode, bool level) {
    fastPinConfig(PinNumber, mode, level);
  }
  //----------------------------------------------------------------------------
  /**
   * Set pin level high if output mode or enable 20K pullup if input mode.
   */
  inline __attribute__((always_inline))
  void high() {write(true);}
  //----------------------------------------------------------------------------
  /**
   * Set pin level low if output mode or disable 20K pullup if input mode.
   */
  inline __attribute__((always_inline))
  void low() {write(false);}
  //----------------------------------------------------------------------------
  /**
   * Set pin mode.
   * @param[in] mode: INPUT, OUTPUT, or INPUT_PULLUP.
   *
   * The internal pullup resistors will be enabled if mode is INPUT_PULLUP
   * and disabled if the mode is INPUT.
   */
  inline __attribute__((always_inline))
  void mode(uint8_t mode) {
    fastPinMode(PinNumber, mode);
  }
  //----------------------------------------------------------------------------
  /** @return Pin's level. */
  inline __attribute__((always_inline))
  bool read() const {
    return fastDigitalRead(PinNumber);
  }
  //----------------------------------------------------------------------------
  /** Toggle a pin.
   *
   * If the pin is in output mode toggle the pin's level.
   * If the pin is in input mode toggle the state of the 20K pullup.
   */
  inline __attribute__((always_inline))
  void toggle() {
    fastDigitalToggle(PinNumber);
  }
  //----------------------------------------------------------------------------
  /** Write the pin's level.
   * @param[in] value If true set the pin's level high else set the
   *  pin's level low.
   */
  inline __attribute__((always_inline))
  void write(bool value) {
    fastDigitalWrite(PinNumber, value);
  }
};
#endif  // DigitalPin_h
/** @} */
