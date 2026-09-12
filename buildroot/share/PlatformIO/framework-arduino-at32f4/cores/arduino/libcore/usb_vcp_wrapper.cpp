/*
  usb_vcp_wrapper.cpp - AT32F403A için USB VCP wrapper (VcpSerial)
  snprintf, sprintf, dtostrf, itoa, ltoa, utoa kullanıldı
  Float ve integer yazdırma overload'ları eklendi
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>     // itoa, ltoa, utoa için
#include <string.h>
#include <math.h>
#include "Print.h" 
#include "usb_vcp_wrapper.h"
#include "libcore/config/mcu_config.h"

#ifdef AT32_USB_CDC
// Global nesne
//VcpSerial SerialVCP;
VcpSerial Serial;

// Yardımcı: delayMicroseconds
#define delayMicroseconds(us) delay_us(us)

VcpSerial::VcpSerial() {}

// Begin (USB zaten bootloader/core tarafından başlatılmış)
void VcpSerial::begin(uint32_t baud) {
  // USB VCP baud rate USB sabit (genellikle 12Mbps), baud parametresi kullanılmaz
  // Ekstra init gerekmez
}

// Tek byte yazma
size_t VcpSerial::write(uint8_t c) {
  return write(&c, 1);
}

// Çoklu byte yazma (DMA/USB VCP ile)
size_t VcpSerial::write(const uint8_t* buffer, size_t size) {
  if (size == 0) return 0;

  size_t sent = 0;
  while (sent < size) {
    uint16_t chunk = (64 < (size - sent)) ? 64 : (size - sent);
    error_status res;
    int timeout = 1000; // ~1 sn timeout
    do {
      res = usb_vcp_send_data(&usb_core_dev, (uint8_t*)buffer + sent, chunk);
      if (res == SUCCESS) break;
      //delayMicroseconds(50);
    } while (--timeout > 0);
    if (timeout <= 0 || res == ERROR) break;
    sent += chunk;
    //delayMicroseconds(50);
  }
  return sent;
}

// Okunabilir byte sayısı
int VcpSerial::available() {
  uint8_t dummy[1];
  uint16_t len = usb_vcp_get_rxdata(&usb_core_dev, dummy);
  return (len > 0) ? len : 0;
}

// Tek byte oku
int VcpSerial::read() {
  uint8_t data;
  uint16_t len = usb_vcp_get_rxdata(&usb_core_dev, &data);
  if (len > 0){
    return data;
  }
  return -1;
}

// Buffer temizle (okunabilir veriyi at)
void VcpSerial::flush() {
  uint8_t dummy[64];
  while (usb_vcp_get_rxdata(&usb_core_dev, dummy) > 0) {}
}
#endif



// // printf (vsnprintf ile)
// void VcpSerial::printf(const char *format, ...) {
//   char buf[256];  // Güvenli boyut
//   va_list args;
//   va_start(args, format);
//   int len = vsnprintf(buf, sizeof(buf), format, args);
//   va_end(args);
//   if (len > 0) write((uint8_t*)buf, len);
// }

// // String yazma
// size_t VcpSerial::print(const char* str) {
//   if (!str) return 0;
//   return write((const uint8_t*)str, strlen(str));
// }

// // println (string + \r\n)
// size_t VcpSerial::println(const char* str) {
//   size_t n = print(str ? str : "");
//   n += write((const uint8_t*)"\r\n", 2);
//   return n;
// }

// // Integer (int) - itoa ile
// size_t VcpSerial::print(int num, int base) {
//   char buf[16];
//   itoa(num, buf, base);
//   return write((const uint8_t*)buf, strlen(buf));
// }

// // Unsigned int - utoa ile
// size_t VcpSerial::print(unsigned int num, int base) {
//   char buf[16];
//   utoa(num, buf, base);
//   return write((const uint8_t*)buf, strlen(buf));
// }

// // Long - ltoa ile
// size_t VcpSerial::print(long num, int base) {
//   char buf[32];
//   ltoa(num, buf, base);
//   return write((const uint8_t*)buf, strlen(buf));
// }

// // Unsigned long - utoa ile
// size_t VcpSerial::print(unsigned long num, int base) {
//   char buf[32];
//   utoa(num, buf, base);
//   return write((const uint8_t*)buf, strlen(buf));
// }

// // Float (dtostrf ile)
// size_t VcpSerial::print(float num, int decimalPlaces) {
//   char buf[32];
//   dtostrf(num, 10, decimalPlaces, buf);  // 10 genişlik, decimalPlaces ondalık
//   return write((const uint8_t*)buf, strlen(buf));
// }

// // Double (dtostrf ile)
// size_t VcpSerial::print(double num, int decimalPlaces) {
//   char buf[48];
//   dtostrf(num, 10, decimalPlaces, buf);
//   return write((const uint8_t*)buf, strlen(buf));
// }

// // Bool
// size_t VcpSerial::print(bool value) {
//   return print(value ? "true" : "false");
// }

// // Arduino String
// size_t VcpSerial::print(const String &str) {
//   return write((const uint8_t*)str.c_str(), str.length());
// }

// // println overload'ları
// size_t VcpSerial::println() {
//   return write((const uint8_t*)"\r\n", 2);
// }

// size_t VcpSerial::println(int num, int base) {
//   size_t n = print(num, base);
//   return n + println();
// }

// size_t VcpSerial::println(unsigned int num, int base) {
//   size_t n = print(num, base);
//   return n + println();
// }

// size_t VcpSerial::println(long num, int base) {
//   size_t n = print(num, base);
//   return n + println();
// }

// size_t VcpSerial::println(unsigned long num, int base) {
//   size_t n = print(num, base);
//   return n + println();
// }

// size_t VcpSerial::println(float num, int decimalPlaces) {
//   size_t n = print(num, decimalPlaces);
//   return n + println();
// }

// size_t VcpSerial::println(double num, int decimalPlaces) {
//   size_t n = print(num, decimalPlaces);
//   return n + println();
// }

// size_t VcpSerial::println(bool value) {
//   size_t n = print(value);
//   return n + println();
// }

// size_t VcpSerial::println(const String &str) {
//   size_t n = print(str);
//   return n + println();
// }

// Global nesne
