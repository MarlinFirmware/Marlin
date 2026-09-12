/*
  SoftwareSerial.h - AT32 implementation
  Copyright (c) 2024 Arduino. All rights reserved.
*/

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>
#include "at32f403a_407.h"
#include "at32f403a_407_crm.h"
#include "at32f403a_407_tmr.h"

#define _SS_MAX_RX_BUFF 64

class SoftwareSerial : public Stream
{
private:
  // Per object data
  uint32_t _rx_delay_centering;
  uint32_t _rx_delay_intrabit;
  uint32_t _rx_delay_stopbit;
  uint32_t _tx_delay;
  uint32_t _receivePin;
  uint32_t _transmitPin;
  uint8_t _receive_buffer[_SS_MAX_RX_BUFF];
  volatile uint8_t _receive_buffer_head;
  volatile uint8_t _receive_buffer_tail;
  bool _buffer_overflow;
  bool _inverse_logic;
  
  void recv();
  void setTX(uint32_t transmitPin);
  void setRX(uint32_t receivePin);
  
  // Statics
  static SoftwareSerial *_active_object;
  static bool _timer_enabled;
  
  static void handle_interrupt();
  
  // AT32 için timer yapılandırma
  static void configure_timer();
  static void start_timer();
  static void stop_timer();
  
public:
  SoftwareSerial(uint32_t receivePin, uint32_t transmitPin, bool inverse_logic = false);
  ~SoftwareSerial();
    static void timer_isr();
  void begin(uint32_t baud);
  void end();
  int available();
  int read();
  int peek();
  void flush();
  size_t write(uint8_t byte);
  void enableRx(bool enable);
  
  using Print::write;
  
  operator bool() { return true; }
};

#endif