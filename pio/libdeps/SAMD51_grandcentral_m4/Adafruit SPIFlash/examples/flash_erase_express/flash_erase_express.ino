// Adafruit SPI Flash Total Erase
// Authors: Tony DiCola, Dan Halbert
//
// This example will perform a complete erase of ALL data on the SPI
// flash.  This is handy to reset the flash into a known empty state
// and fix potential filesystem or other corruption issues.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!  NOTE: YOU WILL ERASE ALL DATA BY RUNNING THIS SKETCH!  !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Usage:
// - Modify the pins and type of fatfs object in the config
//   section below if necessary (usually not necessary).
// - Upload this sketch to your M0 express board.
// - Upon starting the board Neopixel will be blue
// - About 13 seconds later, the Neopixel should starting flashing
//   green once per second. This indicates the SPI flash has been
//   erased and all is well.
// - If the Nexopixel starts flashing red two or three times a second,
//   an error has occurred.

#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_NeoPixel.h>

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

Adafruit_SPIFlash flash(&flashTransport);

// On-board status Neopixel.
#define NEOPIN         40       // neopixel pin
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, NEOPIN, NEO_GRB + NEO_KHZ800);
uint32_t BLUE = pixel.Color(0, 0, 100);
uint32_t GREEN = pixel.Color(0, 100, 0);
uint32_t RED = pixel.Color(100, 0, 0);
uint32_t OFF = pixel.Color(0, 0, 0);

void setup() {
  // Start with a blue pixel.
  pixel.begin();
  pixel.setBrightness(30);
  pixel.setPixelColor(0, BLUE);
  pixel.show();

  // Initialize flash library and check its chip ID.
  if (!flash.begin()) {
    // blink red
    blink(2, RED);
  }
  
  if (!flash.eraseChip()) {
    blink(3, RED);
  }

  flash.waitUntilReady();
  blink(1, GREEN);
}

void loop() {
  // Nothing to do in the loop.
  delay(100);
}

void blink(int times, uint32_t color) {
  while (1) {
    for (int i = 0; i < times; i++) {
      pixel.setPixelColor(0, color);
      pixel.show();
      delay(100);
      pixel.setPixelColor(0, OFF);
      pixel.show();
      delay(100);
    }
    delay(1000);
  }

}
