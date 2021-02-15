// The MIT License (MIT)
// Copyright (c) 2019 Ha Thach for Adafruit Industries

#include "SdFat.h"
#include "Adafruit_SPIFlash.h"

// Uncomment to run example with FRAM
// #define FRAM_CS   A5
// #define FRAM_SPI  SPI

#if defined(FRAM_CS) && defined(FRAM_SPI)
  Adafruit_FlashTransport_SPI flashTransport(FRAM_CS, FRAM_SPI);

#else
  // On-board external flash (QSPI or SPI) macros should already
  // defined in your board variant if supported
  // - EXTERNAL_FLASH_USE_QSPI
  // - EXTERNAL_FLASH_USE_CS/EXTERNAL_FLASH_USE_SPI
  #if defined(EXTERNAL_FLASH_USE_QSPI)
    Adafruit_FlashTransport_QSPI flashTransport;

  #elif defined(EXTERNAL_FLASH_USE_SPI)
    Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);

  #else
    #error No QSPI/SPI flash are defined on your board variant.h !
  #endif
#endif

Adafruit_SPIFlash flash(&flashTransport);

#define BUFSIZE   4096

// 4 byte aligned buffer has best result with nRF QSPI
uint8_t bufwrite[BUFSIZE] __attribute__ ((aligned(4)));
uint8_t bufread[BUFSIZE] __attribute__ ((aligned(4)));

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(100);   // wait for native usb

  flash.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  flash.setIndicator(LED_BUILTIN, true);

  Serial.println("Adafruit Serial Flash Speed Test example");
  Serial.print("JEDEC ID: "); Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: "); Serial.println(flash.size());
  Serial.flush();

  write_and_compare(0xAA);
  write_and_compare(0x55);

  Serial.println("Speed test is completed.");
  Serial.flush();
}

void print_speed(const char* text, uint32_t count, uint32_t ms)
{
  Serial.print(text);
  Serial.print(count);
  Serial.print(" bytes in ");
  Serial.print(ms / 1000.0F, 2);
  Serial.println(" seconds.");

  Serial.print("Speed : ");
  Serial.print( (count / 1000.0F) / (ms / 1000.0F), 2);
  Serial.println(" KB/s.\r\n");
}

bool write_and_compare(uint8_t pattern)
{
  uint32_t ms;

  Serial.println("Erase chip");
  Serial.flush();

#define TEST_WHOLE_CHIP

#ifdef TEST_WHOLE_CHIP
  uint32_t const flash_sz = flash.size();
  flash.eraseChip();
#else
  uint32_t const flash_sz = 4096;
  flash.eraseSector(0);
#endif

  flash.waitUntilReady();

  // write all
  memset(bufwrite, (int) pattern, sizeof(bufwrite));
  Serial.printf("Write flash with 0x%02X\n", pattern);
  Serial.flush();
  ms = millis();

  for(uint32_t addr = 0; addr < flash_sz; addr += sizeof(bufwrite))
  {
    flash.writeBuffer(addr, bufwrite, sizeof(bufwrite));
  }

  uint32_t ms_write = millis() - ms;
  print_speed("Write ", flash_sz, ms_write);
  Serial.flush();

  // read and compare
  Serial.println("Read flash and compare");
  Serial.flush();
  uint32_t ms_read = 0;
  for(uint32_t addr = 0; addr < flash_sz; addr += sizeof(bufread))
  {
    memset(bufread, 0, sizeof(bufread));

    ms = millis();
    flash.readBuffer(addr, bufread, sizeof(bufread));
    ms_read += millis() - ms;

    if ( memcmp(bufwrite, bufread, BUFSIZE) )
    {
      Serial.printf("Error: flash contents mismatched at address 0x%08X!!!", addr);
      for(uint32_t i=0; i<sizeof(bufread); i++)
      {
        if ( i != 0 ) Serial.print(' ');
        if ( (i%16 == 0) )
        {
          Serial.println();
          Serial.printf("%03X: ", i);
        }

        Serial.printf("%02X", bufread[i]);
      }

      Serial.println();
      return false;
    }
  }

  print_speed("Read  ", flash_sz, ms_read);
  Serial.flush();

  return true;
}

void loop()
{
  // nothing to do
}
