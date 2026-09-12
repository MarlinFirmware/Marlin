/*
  SoftwareSerial.cpp - AT32 implementation
  Copyright (c) 2024 Arduino. All rights reserved.
  Based on STM32 version
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "timer.h"

#define NC 0xFFFFFFFF

// Static member definitions
SoftwareSerial *SoftwareSerial::_active_object = nullptr;
bool SoftwareSerial::_timer_enabled = false;

SoftwareSerial::SoftwareSerial(uint32_t receivePin, uint32_t transmitPin, bool inverse_logic) :
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

SoftwareSerial::~SoftwareSerial()
{
  end();
}

void SoftwareSerial::setTX(uint32_t tx)
{
  if (tx != _transmitPin) {
    if (_transmitPin != NC) {
      pinMode(_transmitPin, INPUT);
    }
    _transmitPin = tx;
    
    if (tx != NC) {
      pinMode(tx, OUTPUT);
      digitalWrite(tx, !_inverse_logic ? HIGH : LOW);
    }
  }
}

void SoftwareSerial::setRX(uint32_t rx)
{
  if (rx != _receivePin) {
    if (_receivePin != NC) {
      detachInterrupt(_receivePin);
    }
    _receivePin = rx;
    
    if (rx != NC) {
      pinMode(rx, INPUT);
      if (!_inverse_logic) {
        attachInterrupt(rx, handle_interrupt, CHANGE);
      } else {
        attachInterrupt(rx, handle_interrupt, CHANGE);
      }
    }
  }
}

void SoftwareSerial::configure_timer()
{
  // AT32 için timer yapılandırma
  // Burada genel bir timer başlatılır (örneğin TIM6)
  // Gerçek implementasyon donanıma bağlı olarak değişebilir
  
  crm_clocks_freq_type crm_clocks_freq_struct = {0};
  crm_periph_clock_enable(CRM_TMR6_PERIPH_CLOCK, TRUE);

  crm_clocks_freq_get(&crm_clocks_freq_struct);
  /* tmr1 configuration */
  /* time base configuration */
  /* systemclock/24000/100 = 100hz */
  //tmr_base_init(TMR6, 99, (crm_clocks_freq_struct.ahb_freq / 10000) - 1);
  tmr_base_init(TMR6, 99, (crm_clocks_freq_struct.ahb_freq / 10000) - 1);
  tmr_cnt_dir_set(TMR6, TMR_COUNT_UP);

  /* overflow interrupt enable */
  tmr_interrupt_enable(TMR6, TMR_OVF_INT, TRUE);

  /* tmr1 overflow interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(TMR6_GLOBAL_IRQn, 0, 0);
  tmr_counter_enable(TMR6, TRUE);
}

void SoftwareSerial::start_timer()
{
  if (!_timer_enabled) {
    configure_timer();
     tmr_counter_enable(TIM6, TRUE);
    _timer_enabled = true;
  }
}

void SoftwareSerial::stop_timer()
{
  if (_timer_enabled) {
     tmr_counter_enable(TIM6, FALSE);
    _timer_enabled = false;
  }
}

void SoftwareSerial::begin(uint32_t baud)
{
  // Zamanlama gecikmelerini hesapla
  uint32_t cycles_per_bit = (F_CPU / baud);
  
  // 16x oversampling için gecikmeler
  _tx_delay = cycles_per_bit - 12; // Düzeltme faktörü
  _rx_delay_centering = (cycles_per_bit / 2) + (cycles_per_bit / 16);
  _rx_delay_intrabit = cycles_per_bit - 5;
  _rx_delay_stopbit = cycles_per_bit * 3 / 2;
  
  if (_receivePin != NC) {
    // Alıcı arabelleğini temizle
    _receive_buffer_head = _receive_buffer_tail = 0;
    
    // Aktif nesneyi ayarla
    _active_object = this;
    
    // Timer'ı başlat
    start_timer();
    
    // Kesme ayarları
    if (!_inverse_logic) {
      attachInterrupt(_receivePin, handle_interrupt, FALLING);
    } else {
      attachInterrupt(_receivePin, handle_interrupt, RISING);
    }
  }
}

void SoftwareSerial::end()
{
  if (_receivePin != NC) {
    detachInterrupt(_receivePin);
  }
  
  if (_active_object == this) {
    _active_object = nullptr;
  }
  
  // Tüm SoftwareSerial örnekleri kapandıysa timer'ı durdur
  stop_timer();
}

int SoftwareSerial::read()
{
  if (!available()) {
    return -1;
  }
  
  uint8_t d = _receive_buffer[_receive_buffer_tail];
  _receive_buffer_tail = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int SoftwareSerial::available()
{
  return (_receive_buffer_head + _SS_MAX_RX_BUFF - _receive_buffer_tail) % _SS_MAX_RX_BUFF;
}

size_t SoftwareSerial::write(uint8_t b)
{
  if (_tx_delay == 0) {
    setWriteError();
    return 0;
  }
  
  // Kritik bölge - kesmeleri devre dışı bırak
  uint32_t old_primask = __get_PRIMASK();
  __disable_irq();
  
  digitalWrite(_transmitPin, _inverse_logic ? HIGH : LOW);
  delayMicroseconds(_tx_delay);
  
  // Start bit
  digitalWrite(_transmitPin, _inverse_logic ? LOW : HIGH);
  delayMicroseconds(_tx_delay);
  
  // Data bits (LSB first)
  for (uint8_t i = 8; i > 0; --i) {
    if (b & 1) {
      digitalWrite(_transmitPin, _inverse_logic ? LOW : HIGH);
    } else {
      digitalWrite(_transmitPin, _inverse_logic ? HIGH : LOW);
    }
    delayMicroseconds(_tx_delay);
    b >>= 1;
  }
  
  // Stop bit
  digitalWrite(_transmitPin, _inverse_logic ? HIGH : LOW);
  delayMicroseconds(_tx_delay);
  
  // İşaretleyiciyi geri yükle
  __set_PRIMASK(old_primask);
  
  return 1;
}

void SoftwareSerial::flush()
{
  // RX buffer'ı temizle
  uint32_t old_primask = __get_PRIMASK();
  __disable_irq();
  _receive_buffer_head = _receive_buffer_tail = 0;
  __set_PRIMASK(old_primask);
}

int SoftwareSerial::peek()
{
  if (!available()) {
    return -1;
  }
  return _receive_buffer[_receive_buffer_tail];
}

void SoftwareSerial::recv()
{
  uint8_t d = 0;
  
  // Start bit'ini bekle
  uint32_t wait = micros();
  while (digitalRead(_receivePin) == (_inverse_logic ? HIGH : LOW)) {
    if (micros() - wait > 1000) {
      return; // Timeout
    }
  }
  
  // Yarım bit süresi bekle (centering)
  delayMicroseconds(_rx_delay_centering);
  
  // 8 data bitini oku
  for (uint8_t i = 8; i > 0; --i) {
    delayMicroseconds(_rx_delay_intrabit);
    d >>= 1;
    if (digitalRead(_receivePin) == (_inverse_logic ? LOW : HIGH)) {
      d |= 0x80;
    }
  }
  
  // Stop bitini bekle
  delayMicroseconds(_rx_delay_stopbit);
  
  // Buffer'a kaydet
  uint16_t next = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  if (next != _receive_buffer_tail) {
    _receive_buffer[_receive_buffer_head] = d;
    _receive_buffer_head = next;
  } else {
    _buffer_overflow = true;
  }
}

void SoftwareSerial::handle_interrupt()
{
  if (_active_object) {
    _active_object->recv();
  }
}

void SoftwareSerial::timer_isr()
{
  // AT32 timer interrupt handler
  // Bu fonksiyon TIM6_IRQHandler içinden çağrılmalı
  if (tmr_flag_get(TIM6, TMR_OVF_FLAG) != RESET) {
    tmr_flag_clear(TIM6, TMR_OVF_FLAG);
    
    // Timer overflow işlemleri burada yapılabilir
    if (_active_object) {
      // Gerekli timer işlemleri
    }
  }
}

// AT32 timer interrupt handler fonksiyonu

void TMR6_GLOBAL_IRQHandler(void)
{
  SoftwareSerial::timer_isr();
}


void SoftwareSerial::enableRx(bool enable)
{
  if (_receivePin != NC) {
    if (enable) {
      if (!_inverse_logic) {
        attachInterrupt(_receivePin, handle_interrupt, FALLING);
      } else {
        attachInterrupt(_receivePin, handle_interrupt, RISING);
      }
    } else {
      detachInterrupt(_receivePin);
    }
  }
}