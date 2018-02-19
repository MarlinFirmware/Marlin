/*
 * SoftwareSerial.cpp (formerly NewSoftSerial.cpp)
 *
 * Multi-instance software serial library for Arduino/Wiring
 * -- Interrupt-driven receive and other improvements by ladyada
 *    (http://ladyada.net)
 * -- Tuning, circular buffer, derivation from class Print/Stream,
 *    multi-instance support, porting to 8MHz processors,
 *    various optimizations, PROGMEM delay tables, inverse logic and
 *    direct port writing by Mikal Hart (http://www.arduiniana.org)
 * -- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
 * -- 20MHz processor support by Garrett Mace (http://www.macetech.com)
 * -- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * The latest version of this library can always be found at
 * http://arduiniana.org.
 */

#ifdef TARGET_LPC1768

//
// Includes
//
//#include <WInterrupts.h>
#include "../../inc/MarlinConfig.h"
#include <stdint.h>
#include <stdarg.h>
#include "include/Arduino.h"
#include "pinmapping.h"
#include "fastio.h"
#include "SoftwareSerial.h"

void GpioEnableInt(uint32_t port, uint32_t pin, uint32_t mode);
void GpioDisableInt(uint32_t port, uint32_t pin);
//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;
unsigned char SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF];
volatile uint8_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint8_t SoftwareSerial::_receive_buffer_head = 0;

typedef struct _DELAY_TABLE
{
  long baud;
  uint16_t rx_delay_centering;
  uint16_t rx_delay_intrabit;
  uint16_t rx_delay_stopbit;
  uint16_t tx_delay;
} DELAY_TABLE;

// rough delay estimation
static const DELAY_TABLE table[] =
{
  //baud    |rxcenter|rxintra |rxstop  |tx
  { 250000,   2,      4,       4,       4,   }, //Done but not good due to instruction cycle error
  { 115200,   4,      8,       8,       8,   }, //Done but not good due to instruction cycle error
  //{ 74880,   69,       139,       62,      162,  }, // estimation
//  { 57600,   100,       185,      1,       208,  }, // Done but not good due to instruction cycle error
  //{ 38400,   13,      26,      26,      26,  }, // Done
  //{ 19200,   26,      52,      52,      52,  }, // Done
  { 9600,    52,      104,     104,     104, }, // Done
  //{ 4800,    104,     208,     208,     208, },
  //{ 2400,    208,     417,     417,     417, },
  //{ 1200,    416,    833,      833,     833,},
};

//
// Private methods
//

#if 0
/* static */
inline void SoftwareSerial::tunedDelay(uint32_t count) {

  asm volatile(

    "mov r3, %[loopsPerMicrosecond] \n\t" //load the initial loop counter
    "1: \n\t"
    "sub r3, r3, #1 \n\t"
    "bne 1b \n\t"

    ://empty output list
    :[loopsPerMicrosecond] "r" (count)
    :"r3", "cc" //clobber list
  );

}
#else
inline void SoftwareSerial::tunedDelay(uint32_t count) {
  delayMicroseconds(count);
}
#endif

// This function sets the current object as the "listening"
// one and returns true if it replaces another
bool SoftwareSerial::listen()
{
  if (!_rx_delay_stopbit)
    return false;

  if (active_object != this)
  {
    if (active_object)
      active_object->stopListening();

    _buffer_overflow = false;
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;

    setRxIntMsk(true);
    return true;
  }

  return false;
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{
  if (active_object == this)
  {
    setRxIntMsk(false);
    active_object = NULL;
    return true;
  }
  return false;
}

//
// The receive routine called by the interrupt handler
//
void SoftwareSerial::recv()
{
  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? rx_pin_read() : !rx_pin_read())
  {
    // Disable further interrupts during reception, this prevents
    // triggering another interrupt directly after we return, which can
    // cause problems at higher baudrates.
    setRxIntMsk(false);//__disable_irq();//

    // Wait approximately 1/2 of a bit width to "center" the sample
    tunedDelay(_rx_delay_centering);
    // Read each of the 8 bits
    for (uint8_t i=8; i > 0; --i)
    {
    tunedDelay(_rx_delay_intrabit);
      d >>= 1;
      if (rx_pin_read())
        d |= 0x80;
    }

    if (_inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    uint8_t next = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    if (next != _receive_buffer_head)
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = next;
    }
    else
    {
      _buffer_overflow = true;
    }
  tunedDelay(_rx_delay_stopbit);
    // Re-enable interrupts when we're sure to be inside the stop bit
  setRxIntMsk(true);//__enable_irq();//

  }
}

uint32_t SoftwareSerial::rx_pin_read()
{
  return digitalRead(_receivePin);
}

//
// Interrupt handling
//

/* static */
inline void SoftwareSerial::handle_interrupt()
{
  if (active_object)
  {
    active_object->recv();
  }
}
extern "C" void intWrapper() {
  SoftwareSerial::handle_interrupt();
}
//
// Constructor
//
SoftwareSerial::SoftwareSerial(pin_t receivePin, pin_t transmitPin, bool inverse_logic /* = false */) :
  _rx_delay_centering(0),
  _rx_delay_intrabit(0),
  _rx_delay_stopbit(0),
  _tx_delay(0),
  _buffer_overflow(false),
  _inverse_logic(inverse_logic)
{
  setTX(transmitPin);
  setRX(receivePin);

}

//
// Destructor
//
SoftwareSerial::~SoftwareSerial()
{
  end();
}

void SoftwareSerial::setTX(pin_t tx)
{
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output hihg. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.

  digitalWrite(tx, _inverse_logic ? LOW : HIGH);
  pinMode(tx,OUTPUT);
  _transmitPin = tx;

}

void SoftwareSerial::setRX(pin_t rx)
{
  pinMode(rx, INPUT_PULLUP); // pullup for normal logic!
  //if (!_inverse_logic)
  // digitalWrite(rx, HIGH);
  _receivePin = rx;
  _receivePort = LPC1768_PIN_PORT(rx);
  _receivePortPin = LPC1768_PIN_PIN(rx);
/*  GPIO_T * rxPort = digitalPinToPort(rx);
  _receivePortRegister = portInputRegister(rxPort);
  _receiveBitMask = digitalPinToBitMask(rx);*/

}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
  _rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;

  for(uint8_t i = 0; i < sizeof(table)/sizeof(table[0]); ++i)
  {
    long baud = table[i].baud;
    if(baud == speed)
    {
      _rx_delay_centering = table[i].rx_delay_centering;
      _rx_delay_intrabit = table[i].rx_delay_intrabit;
      _rx_delay_stopbit = table[i].rx_delay_stopbit;
      _tx_delay = table[i].tx_delay;
      break;
    }
  }

  attachInterrupt(_receivePin, intWrapper, CHANGE); //this->handle_interrupt, CHANGE);

  listen();
  tunedDelay(_tx_delay);

}

void SoftwareSerial::setRxIntMsk(bool enable)
{
  if (enable)
    GpioEnableInt(_receivePort,_receivePin,CHANGE);
  else
    GpioDisableInt(_receivePort,_receivePin);
}

void SoftwareSerial::end()
{
  stopListening();
}


// Read data from buffer
int SoftwareSerial::read()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int SoftwareSerial::available()
{
  if (!isListening())
    return 0;

  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}

size_t SoftwareSerial::write(uint8_t b)
{
  // By declaring these as local variables, the compiler will put them
  // in registers _before_ disabling interrupts and entering the
  // critical timing sections below, which makes it a lot easier to
  // verify the cycle timings

  bool inv = _inverse_logic;
  uint16_t delay = _tx_delay;

  if(inv)
    b = ~b;

  cli();  // turn off interrupts for a clean txmit

  // Write the start bit
  if (inv)
    digitalWrite(_transmitPin, 1);
  else
    digitalWrite(_transmitPin, 0);

  tunedDelay(delay);

  // Write each of the 8 bits
  for (uint8_t i = 8; i > 0; --i)
  {
    if (b & 1) // choose bit
      digitalWrite(_transmitPin, 1); // send 1 //(GPIO_Desc[_transmitPin].P)->DOUT |= GPIO_Desc[_transmitPin].bit;
    else
      digitalWrite(_transmitPin, 0); // send 0 //(GPIO_Desc[_transmitPin].P)->DOUT &= ~GPIO_Desc[_transmitPin].bit;

    tunedDelay(delay);
    b >>= 1;
  }

  // restore pin to natural state
  if (inv)
    digitalWrite(_transmitPin, 0);
  else
    digitalWrite(_transmitPin, 1);

  sei(); // turn interrupts back on
  tunedDelay(delay);

  return 1;
}

void SoftwareSerial::flush()
{
  if (!isListening())
    return;

  cli();
  _receive_buffer_head = _receive_buffer_tail = 0;
  sei();
}

int SoftwareSerial::peek()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}

#endif // TARGET_LPC1768
