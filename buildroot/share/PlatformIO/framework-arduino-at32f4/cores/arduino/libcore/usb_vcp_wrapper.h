#ifndef USB_VCP_WRAPPER_H
#define USB_VCP_WRAPPER_H

#include <Arduino.h>
#include <string.h>
#include <stdarg.h>
#include "itoa.h"
#include "Print.h"          // ← bunu ekle (cores/arduino/Print.h'den gelir)
// #include "../../system/Drivers/Firmware/inc/at32f403a_407_usb.h"
#include "../../system/Drivers/usbd_class/cdc/cdc_class.h"

#ifdef __cplusplus
extern "C" {
#endif


char *dtostrf (double val, signed char width, unsigned char prec, char *sout);
char *dtostrnf(double val, signed char width, unsigned char prec, char *sout, size_t sout_size);


extern usbd_core_type usb_core_dev;



class VcpSerial : public Print {   // ← Print'ten miras al
public:
    VcpSerial();                   // constructor varsa

    void begin(uint32_t baud = 115200);   // baud sembolik olabilir USB'de

    // En önemli kısım: write() fonksiyonunu implement et
    size_t write(uint8_t c) override;     // ← tek byte gönder (USB CDC'ye)

    // İstersen birden fazla byte için de override edebilirsin (daha hızlı olur)
    size_t write(const uint8_t *buffer, size_t size) override;

    int available();   // okuma varsa
    int read();        // okuma fonksiyonu
    void flush();      // tamponu boşalt vs.

    // Stream özelliklerini de ekleyebilirsin ama şimdilik Print yeter
};

#ifdef AT32_USB_CDC
extern VcpSerial Serial;
#endif
#ifdef __cplusplus
}
#endif
#endif