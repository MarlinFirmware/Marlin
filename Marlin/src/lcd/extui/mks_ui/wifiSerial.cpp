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
#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "tft_lvgl_configuration.h"

#if ENABLED(MKS_WIFI_MODULE)

#include "draw_ui.h"
#include "wifiSerial.h"

#ifdef __STM32F1__

  #include <libmaple/libmaple.h>
  #include <libmaple/gpio.h>
  #include <libmaple/timer.h>
  #include <libmaple/usart.h>
  #include <libmaple/ring_buffer.h>

  #include "../../../MarlinCore.h"

  DEFINE_WFSERIAL(WifiSerial1, 1);

  WifiSerial::WifiSerial(usart_dev *usart_device, uint8 tx_pin, uint8 rx_pin) {
    this->usart_device = usart_device;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
  }

  /**
   * Set up / tear down
   */
  #if STM32_MCU_SERIES == STM32_SERIES_F1
    /* F1 MCUs have no GPIO_AFR[HL], so turn off PWM if there's a conflict
    * on this GPIO bit. */
    static void disable_timer_if_necessary(timer_dev *dev, uint8 ch) {
      if (dev) timer_set_mode(dev, ch, TIMER_DISABLED);
    }
    static void usart_enable_no_irq(usart_dev *usart_device, bool with_irq) {
      if (with_irq) usart_enable(usart_device);
      else {
        usart_reg_map *regs = usart_device->regs;
        regs->CR1 |= (USART_CR1_TE | USART_CR1_RE);// don't change the word length etc, and 'or' in the patten not overwrite |USART_CR1_M_8N1);
        regs->CR1 |= USART_CR1_UE;
      }
    }

  #elif STM32_MCU_SERIES == STM32_SERIES_F2 || STM32_MCU_SERIES == STM32_SERIES_F4
    #define disable_timer_if_necessary(dev, ch) ((void)0)

    static void usart_enable_no_irq(usart_dev *usart_device, bool with_irq) {
      if (with_irq) usart_enable(usart_device);
      else {
        usart_reg_map *regs = usart_device->regs;
        regs->CR1 |= (USART_CR1_TE | USART_CR1_RE);// don't change the word length etc, and 'or' in the patten not overwrite |USART_CR1_M_8N1);
        regs->CR1 |= USART_CR1_UE;
      }
    }
  #else
    #warning "Unsupported STM32 series; timer conflicts are possible"
    #define usart_enable_no_irq(X, Y) usart_enable(X)
  #endif

  void WifiSerial::begin(uint32 baud) { begin(baud, SERIAL_8N1); }

  /**
   * Roger Clark.
   * Note. The config parameter is not currently used. This is a work in progress.
   * Code needs to be written to set the config of the hardware serial control register in question.
   */

  void WifiSerial::begin(uint32 baud, uint8_t config) {
    //ASSERT(baud <= this->usart_device->max_baud); // Roger Clark. Assert doesn't do anything useful, we may as well save the space in flash and ram etc

    if (baud > this->usart_device->max_baud) return;

    const stm32_pin_info *txi = &PIN_MAP[this->tx_pin],
                        *rxi = &PIN_MAP[this->rx_pin];

    disable_timer_if_necessary(txi->timer_device, txi->timer_channel);

    usart_init(this->usart_device);

    // Reinitialize the receive buffer, mks_esp8266 fixed data frame length is 1k bytes
    rb_init(this->usart_device->rb, WIFI_RX_BUF_SIZE, wifiRxBuf);

    usart_config_gpios_async(this->usart_device,
                            rxi->gpio_device, rxi->gpio_bit,
                            txi->gpio_device, txi->gpio_bit,
                            config);
    usart_set_baud_rate(this->usart_device, USART_USE_PCLK, baud);
    usart_enable_no_irq(this->usart_device, baud == WIFI_BAUDRATE);
  }

  void WifiSerial::end() {
    usart_disable(this->usart_device);
  }

  int WifiSerial::available() {
    return usart_data_available(this->usart_device);
  }

  //
  // I/O
  //

  int WifiSerial::read() {
    if (usart_data_available(usart_device) <= 0) return -1;
    return usart_getc(usart_device);
  }

  int WifiSerial::write(unsigned char ch) {
    usart_putc(this->usart_device, ch);
    return 1;
  }

  int WifiSerial::wifi_rb_is_full() {
    return rb_is_full(this->usart_device->rb);
  }

#else

  WifiSerial WifiSerial1(USART1);

  void WifiSerial::setRx(uint32_t _rx)
  {
    _serial.pin_rx = digitalPinToPinName(_rx);
  }

  void WifiSerial::setTx(uint32_t _tx)
  {
    _serial.pin_tx = digitalPinToPinName(_tx);
  }

  void WifiSerial::setRx(PinName _rx)
  {
    _serial.pin_rx = _rx;
  }

  void WifiSerial::setTx(PinName _tx)
  {
    _serial.pin_tx = _tx;
  }

  void WifiSerial::init(PinName _rx, PinName _tx)
  {
    if (_rx == _tx) {
      _serial.pin_rx = NC;
    } else {
      _serial.pin_rx = _rx;
    }
    _serial.pin_tx = _tx;
    _serial.rx_buff = wifiRxBuf;
    _serial.rx_head = 0;
    _serial.rx_tail = 0;
    _serial.tx_buff = wifiTxBuf;
    _serial.tx_head = 0;
    _serial.tx_tail = 0;
  }

  WifiSerial::WifiSerial(void *peripheral)
  {
    // If PIN_SERIALy_RX is not defined assume half-duplex
    _serial.pin_rx = NC;
    // If Serial is defined in variant set
    // the Rx/Tx pins for com port if defined
  #if defined(Serial) && defined(PIN_SERIAL_TX)
    if ((void *)this == (void *)&Serial) {
  #if defined(PIN_SERIAL_RX)
      setRx(PIN_SERIAL_RX);
  #endif
      setTx(PIN_SERIAL_TX);
    } else
  #endif
  #if defined(PIN_SERIAL1_TX) && defined(USART1_BASE)
      if (peripheral == USART1) {
  #if defined(PIN_SERIAL1_RX)
        setRx(PIN_SERIAL1_RX);
  #endif
        setTx(PIN_SERIAL1_TX);
      } else
  #endif
  #if defined(PIN_SERIAL2_TX) && defined(USART2_BASE)
        if (peripheral == USART2) {
  #if defined(PIN_SERIAL2_RX)
          setRx(PIN_SERIAL2_RX);
  #endif
          setTx(PIN_SERIAL2_TX);
        } else
  #endif
  #if defined(PIN_SERIAL3_TX) && defined(USART3_BASE)
          if (peripheral == USART3) {
  #if defined(PIN_SERIAL3_RX)
            setRx(PIN_SERIAL3_RX);
  #endif
            setTx(PIN_SERIAL3_TX);
          } else
  #endif
  #if defined(PIN_SERIAL4_TX) &&\
    (defined(USART4_BASE) || defined(UART4_BASE))
  #if defined(USART4_BASE)
            if (peripheral == USART4)
  #elif defined(UART4_BASE)
            if (peripheral == UART4)
  #endif
            {
  #if defined(PIN_SERIAL4_RX)
              setRx(PIN_SERIAL4_RX);
  #endif
              setTx(PIN_SERIAL4_TX);
            } else
  #endif
  #if defined(PIN_SERIAL5_TX) &&\
    (defined(USART5_BASE) || defined(UART5_BASE))
  #if defined(USART5_BASE)
              if (peripheral == USART5)
  #elif defined(UART5_BASE)
              if (peripheral == UART5)
  #endif
              {
  #if defined(PIN_SERIAL5_RX)
                setRx(PIN_SERIAL5_RX);
  #endif
                setTx(PIN_SERIAL5_TX);
              } else
  #endif
  #if defined(PIN_SERIAL6_TX) && defined(USART6_BASE)
                if (peripheral == USART6) {
  #if defined(PIN_SERIAL6_RX)
                  setRx(PIN_SERIAL6_RX);
  #endif
                  setTx(PIN_SERIAL6_TX);
                } else
  #endif
  #if defined(PIN_SERIAL7_TX) &&\
    (defined(USART7_BASE) || defined(UART7_BASE))
  #if defined(USART7_BASE)
                  if (peripheral == USART7)
  #elif defined(UART7_BASE)
                  if (peripheral == UART7)
  #endif
                  {
  #if defined(PIN_SERIAL7_RX)
                    setRx(PIN_SERIAL7_RX);
  #endif
                    setTx(PIN_SERIAL7_TX);
                  } else
  #endif
  #if defined(PIN_SERIAL8_TX) &&\
    (defined(USART8_BASE) || defined(UART8_BASE))
  #if defined(USART8_BASE)
                    if (peripheral == USART8)
  #elif defined(UART8_BASE)
                    if (peripheral == UART8)
  #endif
                    {
  #if defined(PIN_SERIAL8_RX)
                      setRx(PIN_SERIAL8_RX);
  #endif
                      setTx(PIN_SERIAL8_TX);
                    } else
  #endif
  #if defined(PIN_SERIAL9_TX) && defined(UART9_BASE)
                      if (peripheral == UART9) {
  #if defined(PIN_SERIAL9_RX)
                        setRx(PIN_SERIAL9_RX);
  #endif
                        setTx(PIN_SERIAL9_TX);
                      } else
  #endif
  #if defined(PIN_SERIAL10_TX) &&\
    (defined(USART10_BASE) || defined(UART10_BASE))
  #if defined(USART10_BASE)
                        if (peripheral == USART10)
  #elif defined(UART10_BASE)
                        if (peripheral == UART10)
  #endif
                        {
  #if defined(PIN_SERIAL10_RX)
                          setRx(PIN_SERIAL10_RX);
  #endif
                          setTx(PIN_SERIAL10_TX);
                        } else
  #endif
  #if defined(PIN_SERIALLP1_TX) && defined(LPUART1_BASE)
                          if (peripheral == LPUART1) {
  #if defined(PIN_SERIALLP1_RX)
                            setRx(PIN_SERIALLP1_RX);
  #endif
                            setTx(PIN_SERIALLP1_TX);
                          } else
  #endif
                            // else get the pins of the first peripheral occurence in PinMap
                          {
                            _serial.pin_rx = pinmap_pin(peripheral, PinMap_UART_RX);
                            _serial.pin_tx = pinmap_pin(peripheral, PinMap_UART_TX);
                          }
    // if (halfDuplex == HALF_DUPLEX_ENABLED) {
    //   _serial.pin_rx = NC;
    // }
    init(_serial.pin_rx, _serial.pin_tx);
  }

  void WifiSerial::flush()
  {
    // If we have never written a byte, no need to flush. This special
    // case is needed since there is no way to force the TXC (transmit
    // complete) bit to 1 during initialization
    if (!_written) {
      return;
    }

    while ((_serial.tx_head != _serial.tx_tail)) {
      // nop, the interrupt handler will free up space for us
    }
    // If we get here, nothing is queued anymore (DRIE is disabled) and
    // the hardware finished tranmission (TXC is set).
  }

  bool WifiSerial::isHalfDuplex(void) const
  {
    return _serial.pin_rx == NC;
  }

  void WifiSerial::enableHalfDuplexRx(void)
  {
    if (isHalfDuplex()) {
      // In half-duplex mode we have to wait for all TX characters to
      // be transmitted before we can receive data.
      flush();
      if (!_rx_enabled) {
        _rx_enabled = true;
        uart_enable_rx(&_serial);
      }
    }
  }

  // Actual interrupt handlers //////////////////////////////////////////////////////////////

  void WifiSerial::_rx_complete_irq(serial_t *obj)
  {
    // No Parity error, read byte and store it in the buffer if there is room
    unsigned char c;

    if (uart_getc(obj, &c) == 0) {

      WRITE(WIFI_IO1_PIN, HIGH);

      rx_buffer_index_t i = (unsigned int)(obj->rx_head + 1) % WIFI_RX_BUF_SIZE;

      // if we should be storing the received character into the location
      // just before the tail (meaning that the head would advance to the
      // current location of the tail), we're about to overflow the buffer
      // and so we don't write the character or advance the head.
      if (i != obj->rx_tail) {
        obj->rx_buff[obj->rx_head] = c;
        obj->rx_head = i;
      }
    }
  }

  // Actual interrupt handlers //////////////////////////////////////////////////////////////

  int WifiSerial::_tx_complete_irq(serial_t *obj)
  {
    // If interrupts are enabled, there must be more data in the output
    // buffer. Send the next byte
    obj->tx_tail = (obj->tx_tail + 1) % WIFI_TX_BUF_SIZE;

    if (obj->tx_head == obj->tx_tail) {
      return -1;
    }

    return 0;
  }

  void WifiSerial::begin(unsigned long baud) { begin(baud, SERIAL_8N1); }

  void WifiSerial::begin(unsigned long baud, byte config) {
    uint32_t databits = 0;
    uint32_t stopbits = 0;
    uint32_t parity = 0;

    _baud = baud;
    _config = config;

    // Manage databits
    switch (config & 0x07) {
      case 0x02:
        databits = 6;
        break;
      case 0x04:
        databits = 7;
        break;
      case 0x06:
        databits = 8;
        break;
      default:
        databits = 0;
        break;
    }

    if ((config & 0x30) == 0x30) {
      parity = UART_PARITY_ODD;
      databits++;
    } else if ((config & 0x20) == 0x20) {
      parity = UART_PARITY_EVEN;
      databits++;
    } else {
      parity = UART_PARITY_NONE;
    }

    if ((config & 0x08) == 0x08) {
      stopbits = UART_STOPBITS_2;
    } else {
      stopbits = UART_STOPBITS_1;
    }

    switch (databits) {
    #ifdef UART_WORDLENGTH_7B
      case 7:
        databits = UART_WORDLENGTH_7B;
        break;
    #endif
      case 8:
        databits = UART_WORDLENGTH_8B;
        break;
      case 9:
        databits = UART_WORDLENGTH_9B;
        break;
      default:
      case 0:
        Error_Handler();
        break;
    }

    uart_init(&_serial, (uint32_t)baud, databits, parity, stopbits);
    enableHalfDuplexRx();
    if (baud == WIFI_BAUDRATE) uart_attach_rx_callback(&_serial, _rx_complete_irq);
    else {
      USART1->CR1 |= (USART_CR1_RE);// don't change the word length etc, and 'or' in the patten not overwrite |USART_CR1_M_8N1);
    }
  }

  void WifiSerial::end(void)
  {
    // wait for transmission of outgoing data
    flush();

    uart_deinit(&_serial);

    // clear any received data
    _serial.rx_head = _serial.rx_tail;
  }

  int WifiSerial::available(void) {
    return ((unsigned int)(WIFI_RX_BUF_SIZE + _serial.rx_head - _serial.rx_tail)) % WIFI_RX_BUF_SIZE;
  }

  //
  // I/O
  //
  int WifiSerial::read(void)
  {
    enableHalfDuplexRx();
    // if the head isn't ahead of the tail, we don't have any characters
    if (_serial.rx_head == _serial.rx_tail) {
      return -1;
    } else {
      unsigned char c = _serial.rx_buff[_serial.rx_tail];
      _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % WIFI_RX_BUF_SIZE;
      return c;
    }
  }

  int WifiSerial::write(uint8_t c)
  {
    _written = true;
    if (isHalfDuplex()) {
      if (_rx_enabled) {
        _rx_enabled = false;
        uart_enable_tx(&_serial);
      }
    }

    tx_buffer_index_t i = (_serial.tx_head + 1) % WIFI_TX_BUF_SIZE;

    // If the output buffer is full, there's nothing for it other than to
    // wait for the interrupt handler to empty it a bit
    while (i == _serial.tx_tail) {
      // nop, the interrupt handler will free up space for us
    }

    _serial.tx_buff[_serial.tx_head] = c;
    _serial.tx_head = i;

    if (!serial_tx_active(&_serial)) {
      uart_attach_tx_callback(&_serial, _tx_complete_irq);
    }

    return 1;
  }
#endif // __STM32F1__
#endif // MKS_WIFI_MODULE
#endif // HAS_TFT_LVGL_UI
