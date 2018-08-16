/*------------------------------------------------------------------------
  This file is part of the Adafruit Dot Star library.

  Adafruit Dot Star is free software: you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  Adafruit Dot Star is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with DotStar.  If not, see <http://www.gnu.org/licenses/>.
  ------------------------------------------------------------------------*/

#ifndef _ADAFRUIT_DOT_STAR_H_
#define _ADAFRUIT_DOT_STAR_H_

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

// Color-order flag for LED pixels (optional extra parameter to constructor):
// Bits 0,1 = R index (0-2), bits 2,3 = G index, bits 4,5 = B index
#define DOTSTAR_RGB (0 | (1 << 2) | (2 << 4))
#define DOTSTAR_RBG (0 | (2 << 2) | (1 << 4))
#define DOTSTAR_GRB (1 | (0 << 2) | (2 << 4))
#define DOTSTAR_GBR (2 | (0 << 2) | (1 << 4))
#define DOTSTAR_BRG (1 | (2 << 2) | (0 << 4))
#define DOTSTAR_BGR (2 | (1 << 2) | (0 << 4))
#define DOTSTAR_MONO 0 // Single-color strip WIP DO NOT USE YET

class Adafruit_DotStar {

 public:

    Adafruit_DotStar(uint16_t n, uint8_t o=DOTSTAR_BRG);
    Adafruit_DotStar(uint16_t n, uint8_t d, uint8_t c, uint8_t o=DOTSTAR_BRG);
   ~Adafruit_DotStar(void);
  void
    begin(void),                            // Prime pins/SPI for output
    clear(),                                // Set all pixel data to zero
    setBrightness(uint8_t),                 // Set global brightness 0-255
    setPixelColor(uint16_t n, uint32_t c),
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
    show(void),                             // Issue color data to strip
    updatePins(void),                       // Change pin assignments (HW)
    updatePins(uint8_t d, uint8_t c),       // Change pin assignments (SW)
    updateLength(uint16_t n);               // Change length
  uint32_t
    Color(uint8_t r, uint8_t g, uint8_t b), // R,G,B to 32-bit color
    getPixelColor(uint16_t n) const;        // Return 32-bit pixel color
  uint16_t
    numPixels(void);                        // Return number of pixels
  uint8_t
    getBrightness(void) const,              // Return global brightness
   *getPixels(void) const,                  // Return pixel data pointer
    sine8(uint8_t) const,
    gamma8(uint8_t) const;

 private:

  uint16_t
    numLEDs;                                // Number of pixels
  uint8_t
    dataPin,                                // If soft SPI, data pin #
    clockPin,                               // If soft SPI, clock pin #
    brightness,                             // Global brightness setting
   *pixels,                                 // LED RGB values (3 bytes ea.)
    rOffset,                                // Index of red in 3-byte pixel
    gOffset,                                // Index of green byte
    bOffset;                                // Index of blue byte
#ifdef __AVR__
  uint8_t
    dataPinMask,                            // If soft SPI, data pin bitmask
    clockPinMask;                           // If soft SPI, clock pin bitmask
  volatile uint8_t
   *dataPort,                               // If soft SPI, data PORT
   *clockPort;                              // If soft SPI, clock PORT
#endif
  void
    hw_spi_init(void),                      // Start hardware SPI
    hw_spi_end(void),                       // Stop hardware SPI
    sw_spi_init(void),                      // Start bitbang SPI
    sw_spi_out(uint8_t n),                  // Bitbang SPI write
    sw_spi_end(void);                       // Stop bitbang SPI

};

#endif // _ADAFRUIT_DOT_STAR_H_
