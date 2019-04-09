/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifdef ARDUINO_ARCH_ESP32

#include "WebSocketSerial.h"

#if ENABLED(WIFISUPPORT)

#include "wifi.h"
#include <ESPAsyncWebServer.h>

WebSocketSerial webSocketSerial;
AsyncWebSocket ws("/ws"); // TODO Move inside the class.

// DataBuffer impl
DataBuffer::DataBuffer(data_buffer_size_t size)
    : data(new uint8_t[size]),
      head(0),
      tail(0),
      size(size)
{}

void DataBuffer::add(uint8_t * const data, data_buffer_size_t len) {
  for (size_t i = 0; i < len; i++) {
    const data_buffer_size_t n = (head + 1) & (data_buffer_size_t)(size - 1);

    if (n != tail) {
      this->data[head] = data[i];
      head = n;
    }

    // TODO: buffer is full, handle?
  }
}


// Public Methods
WebSocketSerial::WebSocketSerial()
    : rx_buffer(DataBuffer(RX_BUFFER_SIZE)),
      tx_buffer(DataBuffer(TX_BUFFER_SIZE))
{}

WebSocketSerial::~WebSocketSerial() {
  delete[] rx_buffer.data;
  delete[] tx_buffer.data;
}

void WebSocketSerial::begin(const long baud_setting) {
  ws.onEvent([this](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
      switch (type) {
        case WS_EVT_CONNECT: client->ping(); break; // client connected
        case WS_EVT_DISCONNECT:                     // client disconnected
        case WS_EVT_ERROR:                          // error was received from the other end
        case WS_EVT_PONG: break;                    // pong message was received (in response to a ping request maybe)
        case WS_EVT_DATA: {                         // data packet
          AwsFrameInfo * info = (AwsFrameInfo*)arg;
          if (info->opcode == WS_TEXT || info->message_opcode == WS_TEXT)
            this->rx_buffer.add(data, len);
        }
      }
    });
  server.addHandler(&ws); // attach AsyncWebSocket
}

void WebSocketSerial::end() { }

int WebSocketSerial::peek(void) {
  const int v = rx_buffer.head == rx_buffer.tail ? -1 : rx_buffer.data[rx_buffer.tail];
  return v;
}

int WebSocketSerial::read(void) {
  const data_buffer_size_t h = rx_buffer.head, t = rx_buffer.tail;
  if (h == t) return -1;  // Nothing to read? Return now

  const int v = rx_buffer.data[t];

  rx_buffer.tail = (data_buffer_size_t)(t + 1) & (RX_BUFFER_SIZE - 1); // Advance tail

  return v;
}

bool WebSocketSerial::available(void) {
  const data_buffer_size_t h = rx_buffer.head, t = rx_buffer.tail;
  return (data_buffer_size_t)(RX_BUFFER_SIZE + h - t) & (RX_BUFFER_SIZE - 1);
}

void WebSocketSerial::flush(void) {
  rx_buffer.tail = rx_buffer.head;
}

void WebSocketSerial::write(const uint8_t c) {
  const uint8_t i = (tx_buffer.head + 1) & (TX_BUFFER_SIZE - 1);

  // Store new char. head is always safe to move
  tx_buffer.data[tx_buffer.head] = c;
  tx_buffer.head = i;

  if (c == '\n') {
    ws.textAll(tx_buffer.data, tx_buffer.head);
    tx_buffer.head = 0;
  }
}

void WebSocketSerial::flushTX(void) {
  ws.textAll(tx_buffer.data, tx_buffer.head);
  tx_buffer.head = 0;
}

/**
 * Imports from print.h
 */

void WebSocketSerial::print(char c, int base) { print((long)c, base); }
void WebSocketSerial::print(unsigned char b, int base) { print((unsigned long)b, base); }
void WebSocketSerial::print(int n, int base) { print((long)n, base); }
void WebSocketSerial::print(unsigned int n, int base) { print((unsigned long)n, base); }
void WebSocketSerial::print(long n, int base) {
  if (base == 0)
    write(n);
  else if (base == 10) {
    if (n < 0) { print('-'); n = -n; }
    printNumber(n, 10);
  }
  else
    printNumber(n, base);
}

void WebSocketSerial::print(unsigned long n, int base) {
  if (base == 0) write(n); else printNumber(n, base);
}

void WebSocketSerial::print(double n, int digits)         { printFloat(n, digits); }

void WebSocketSerial::println(void)                       { print('\r'); print('\n'); }
void WebSocketSerial::println(const String& s)            { print(s); println(); }
void WebSocketSerial::println(const char c[])             { print(c); println(); }
void WebSocketSerial::println(char c, int base)           { print(c, base); println(); }
void WebSocketSerial::println(unsigned char b, int base)  { print(b, base); println(); }
void WebSocketSerial::println(int n, int base)            { print(n, base); println(); }
void WebSocketSerial::println(unsigned int n, int base)   { print(n, base); println(); }
void WebSocketSerial::println(long n, int base)           { print(n, base); println(); }
void WebSocketSerial::println(unsigned long n, int base)  { print(n, base); println(); }
void WebSocketSerial::println(double n, int digits)       { print(n, digits); println(); }

// Private Methods
void WebSocketSerial::printNumber(unsigned long n, uint8_t base) {
  if (n) {
    unsigned char buf[8 * sizeof(long)]; // Enough space for base 2
    int8_t i = 0;
    while (n) {
      buf[i++] = n % base;
      n /= base;
    }
    while (i--)
      print((char)(buf[i] + (buf[i] < 10 ? '0' : 'A' - 10)));
  }
  else
    print('0');
}

void WebSocketSerial::printFloat(double number, uint8_t digits) {
  // Handle negative numbers
  if (number < 0.0) { print('-'); number = -number; }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  // Use a lookup table for performance
  constexpr double rounds[] = { 0.5, 0.05, 0.005, 0.0005, 0.00005, 0.000005, 0.0000005, 0.00000005 };
  number += rounds[digits];

  //number += pow(10, -(digits + 1)); // slower single-line equivalent

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  double remainder = number - (double)int_part;
  if (digits) {
    print('.');
    // Extract digits from the remainder one at a time
    while (digits--) {
      remainder *= 10.0;
      const int toPrint = int(remainder);
      print(toPrint);
      remainder -= toPrint;
    }
  }
}

#endif // WIFISUPPORT
#endif // ARDUINO_ARCH_ESP32
