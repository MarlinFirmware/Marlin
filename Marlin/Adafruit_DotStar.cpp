/*------------------------------------------------------------------------
  Arduino library to control Adafruit Dot Star addressable RGB LEDs.

  Written by Limor Fried and Phil Burgess for Adafruit Industries.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  ------------------------------------------------------------------------
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

#include "Adafruit_DotStar.h"
#if !defined(__AVR_ATtiny85__)
 #include <SPI.h>
#endif

#define USE_HW_SPI 255 // Assign this to dataPin to indicate 'hard' SPI

// Constructor for hardware SPI -- must connect to MOSI, SCK pins
Adafruit_DotStar::Adafruit_DotStar(uint16_t n, uint8_t o) :
 numLEDs(n), dataPin(USE_HW_SPI), brightness(0), pixels(NULL),
 rOffset(o & 3), gOffset((o >> 2) & 3), bOffset((o >> 4) & 3)
{
  updateLength(n);
}

// Constructor for 'soft' (bitbang) SPI -- any two pins can be used
Adafruit_DotStar::Adafruit_DotStar(uint16_t n, uint8_t data, uint8_t clock,
  uint8_t o) :
 dataPin(data), clockPin(clock), brightness(0), pixels(NULL),
 rOffset(o & 3), gOffset((o >> 2) & 3), bOffset((o >> 4) & 3)
{
  updateLength(n);
}

Adafruit_DotStar::~Adafruit_DotStar(void) { // Destructor
  if(pixels)                free(pixels);
  if(dataPin == USE_HW_SPI) hw_spi_end();
  else                      sw_spi_end();
}

void Adafruit_DotStar::begin(void) { // Initialize SPI
  if(dataPin == USE_HW_SPI) hw_spi_init();
  else                      sw_spi_init();
}

// Pins may be reassigned post-begin(), so a sketch can store hardware
// config in flash, SD card, etc. rather than hardcoded.  Also permits
// "recycling" LED ram across multiple strips: set pins to first strip,
// render & write all data, reassign pins to next strip, render & write,
// etc.  They won't update simultaneously, but usually unnoticeable.

// Change to hardware SPI -- must connect to MOSI, SCK pins
void Adafruit_DotStar::updatePins(void) {
  sw_spi_end();
  dataPin = USE_HW_SPI;
  hw_spi_init();
}

// Change to 'soft' (bitbang) SPI -- any two pins can be used
void Adafruit_DotStar::updatePins(uint8_t data, uint8_t clock) {
  hw_spi_end();
  dataPin  = data;
  clockPin = clock;
  sw_spi_init();
}

// Length can be changed post-constructor for similar reasons (sketch
// config not hardcoded).  But DON'T use this for "recycling" strip RAM...
// all that reallocation is likely to fragment and eventually fail.
// Instead, set length once to longest strip.
void Adafruit_DotStar::updateLength(uint16_t n) {
  if(pixels) free(pixels);
  uint16_t bytes = (rOffset == gOffset) ?
    n + ((n + 3) / 4) : // MONO: 10 bits/pixel, round up to next byte
    n * 3;              // COLOR: 3 bytes/pixel
  if((pixels = (uint8_t *)malloc(bytes))) {
    numLEDs = n;
    clear();
  } else {
    numLEDs = 0;
  }
}

// SPI STUFF ---------------------------------------------------------------

void Adafruit_DotStar::hw_spi_init(void) { // Initialize hardware SPI
#ifdef __AVR_ATtiny85__
  PORTB &= ~(_BV(PORTB1) | _BV(PORTB2)); // Outputs
  DDRB  |=   _BV(PORTB1) | _BV(PORTB2);  // DO (NOT MOSI) + SCK
#elif (SPI_INTERFACES_COUNT > 0) || !defined(SPI_INTERFACES_COUNT)
  SPI.begin();
 #if defined(__AVR__) || defined(CORE_TEENSY) || defined(__ARDUINO_ARC__) || defined(__ARDUINO_X86__)
  SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (6 MHz on Pro Trinket 3V)
 #else
  #ifdef ESP8266
    SPI.setFrequency(8000000L);
  #elif defined(PIC32)
    // Use begin/end transaction to set SPI clock rate
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    SPI.endTransaction();
  #else
    SPI.setClockDivider((F_CPU + 4000000L) / 8000000L); // 8-ish MHz on Due
  #endif
 #endif
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
#endif
}

void Adafruit_DotStar::hw_spi_end(void) { // Stop hardware SPI
#ifdef __AVR_ATtiny85__
  DDRB &= ~(_BV(PORTB1) | _BV(PORTB2)); // Inputs
#elif (SPI_INTERFACES_COUNT > 0) || !defined(SPI_INTERFACES_COUNT)
  SPI.end();
#endif
}

void Adafruit_DotStar::sw_spi_init(void) { // Init 'soft' (bitbang) SPI
  pinMode(dataPin , OUTPUT);
  pinMode(clockPin, OUTPUT);
#ifdef __AVR__
  dataPort     =  portOutputRegister(digitalPinToPort(dataPin));
  clockPort    =  portOutputRegister(digitalPinToPort(clockPin));
  dataPinMask  =  digitalPinToBitMask(dataPin);
  clockPinMask =  digitalPinToBitMask(clockPin);
  *dataPort   &= ~dataPinMask;
  *clockPort  &= ~clockPinMask;
#else
  digitalWrite(dataPin , LOW);
  digitalWrite(clockPin, LOW);
#endif
}

void Adafruit_DotStar::sw_spi_end() { // Stop 'soft' SPI
  pinMode(dataPin , INPUT);
  pinMode(clockPin, INPUT);
}

#ifdef __AVR_ATtiny85__

// Teensy/Gemma-specific stuff for hardware-half-assisted SPI

#define SPIBIT                                  \
  USICR = ((1<<USIWM0)|(1<<USITC));             \
  USICR = ((1<<USIWM0)|(1<<USITC)|(1<<USICLK)); // Clock bit tick, tock

static void spi_out(uint8_t n) { // Clock out one byte
  USIDR = n;
  SPIBIT SPIBIT SPIBIT SPIBIT SPIBIT SPIBIT SPIBIT SPIBIT
}

#elif (SPI_INTERFACES_COUNT > 0) || !defined(SPI_INTERFACES_COUNT)

// All other boards have full-featured hardware support for SPI

#define spi_out(n) (void)SPI.transfer(n)
// Pipelining reads next byte while current byte is clocked out
#if (defined(__AVR__) && !defined(__AVR_ATtiny85__)) || defined(CORE_TEENSY)
 #define SPI_PIPELINE
#endif

#else // no hardware spi
#define spi_out(n) sw_spi_out(n)

#endif

void Adafruit_DotStar::sw_spi_out(uint8_t n) { // Bitbang SPI write
  for(uint8_t i=8; i--; n <<= 1) {
#ifdef __AVR__
    if(n & 0x80) *dataPort |=  dataPinMask;
    else         *dataPort &= ~dataPinMask;
    *clockPort |=  clockPinMask;
    *clockPort &= ~clockPinMask;
#else
    if(n & 0x80) digitalWrite(dataPin, HIGH);
    else         digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
#endif
  }
}

/* ISSUE DATA TO LED STRIP -------------------------------------------------

  Although the LED driver has an additional per-pixel 5-bit brightness
  setting, it is NOT used or supported here because it's a brain-dead
  misfeature that's counter to the whole point of Dot Stars, which is to
  have a much faster PWM rate than NeoPixels.  It gates the high-speed
  PWM output through a second, much slower PWM (about 400 Hz), rendering
  it useless for POV.  This brings NOTHING to the table that can't be
  already handled better in one's sketch code.  If you really can't live
  without this abomination, you can fork the library and add it for your
  own use, but any pull requests for this will NOT be merged, nuh uh!
*/

void Adafruit_DotStar::show(void) {

  if(!pixels) return;

  uint8_t *ptr = pixels, i;            // -> LED data
  uint16_t n   = numLEDs;              // Counter
  uint16_t b16 = (uint16_t)brightness; // Type-convert for fixed-point math

  if(dataPin == USE_HW_SPI) {

#ifdef SPI_PIPELINE
    uint8_t next;
    for(i=0; i<3; i++) spi_out(0x00);    // First 3 start-frame bytes
    SPDR = 0x00;                         // 4th is pipelined
    do {                                 // For each pixel...
      while(!(SPSR & _BV(SPIF)));        //  Wait for prior byte out
      SPDR = 0xFF;                       //  Pixel start
      for(i=0; i<3; i++) {               //  For R,G,B...
        next = brightness ? (*ptr++ * b16) >> 8 : *ptr++; // Read, scale
        while(!(SPSR & _BV(SPIF)));      //   Wait for prior byte out
        SPDR = next;                     //   Write scaled color
      }
    } while(--n);
    while(!(SPSR & _BV(SPIF)));          // Wait for last byte out
#else
    for(i=0; i<4; i++) spi_out(0x00);    // 4 byte start-frame marker
    if(brightness) {                     // Scale pixel brightness on output
      do {                               // For each pixel...
        spi_out(0xFF);                   //  Pixel start
        for(i=0; i<3; i++) spi_out((*ptr++ * b16) >> 8); // Scale, write RGB
      } while(--n);
    } else {                             // Full brightness (no scaling)
      do {                               // For each pixel...
        spi_out(0xFF);                   //  Pixel start
        for(i=0; i<3; i++) spi_out(*ptr++); // Write R,G,B
      } while(--n);
    }
#endif
    // Four end-frame bytes are seemingly indistinguishable from a white
    // pixel, and empirical testing suggests it can be left out...but it's
    // always a good idea to follow the datasheet, in case future hardware
    // revisions are more strict (e.g. might mandate use of end-frame
    // before start-frame marker).  i.e. let's not remove this. But after
    // testing a bit more the suggestion is to use at least (numLeds+1)/2
    // high values (1) or (numLeds+15)/16 full bytes as EndFrame. For details see also:
    // https://cpldcpu.wordpress.com/2014/11/30/understanding-the-apa102-superled/
    for(i=0; i<((numLEDs + 15) / 16); i++) spi_out(0xFF);

  } else {                               // Soft (bitbang) SPI

    for(i=0; i<4; i++) sw_spi_out(0);    // Start-frame marker
    if(brightness) {                     // Scale pixel brightness on output
      do {                               // For each pixel...
        sw_spi_out(0xFF);                //  Pixel start
        for(i=0; i<3; i++) sw_spi_out((*ptr++ * b16) >> 8); // Scale, write
      } while(--n);
    } else {                             // Full brightness (no scaling)
      do {                               // For each pixel...
        sw_spi_out(0xFF);                //  Pixel start
        for(i=0; i<3; i++) sw_spi_out(*ptr++); // R,G,B
      } while(--n);
    }
    for(i=0; i<((numLEDs + 15) / 16); i++) sw_spi_out(0xFF); // End-frame marker (see note above)
  }
}

void Adafruit_DotStar::clear() { // Write 0s (off) to full pixel buffer
  memset(pixels, 0, (rOffset == gOffset) ?
    numLEDs + ((numLEDs + 3) / 4) : // MONO: 10 bits/pixel
    numLEDs * 3);                   // COLOR: 3 bytes/pixel
}

// Set pixel color, separate R,G,B values (0-255 ea.)
void Adafruit_DotStar::setPixelColor(
 uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numLEDs) {
    uint8_t *p = &pixels[n * 3];
    p[rOffset] = r;
    p[gOffset] = g;
    p[bOffset] = b;
  }
}

// Set pixel color, 'packed' RGB value (0x000000 - 0xFFFFFF)
void Adafruit_DotStar::setPixelColor(uint16_t n, uint32_t c) {
  if(n < numLEDs) {
    uint8_t *p = &pixels[n * 3];
    p[rOffset] = (uint8_t)(c >> 16);
    p[gOffset] = (uint8_t)(c >>  8);
    p[bOffset] = (uint8_t)c;
  }
}

// Convert separate R,G,B to packed value
uint32_t Adafruit_DotStar::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

// Read color from previously-set pixel, returns packed RGB value.
uint32_t Adafruit_DotStar::getPixelColor(uint16_t n) const {
  if(n >= numLEDs) return 0;
  uint8_t *p = &pixels[n * 3];
  return ((uint32_t)p[rOffset] << 16) |
         ((uint32_t)p[gOffset] <<  8) |
          (uint32_t)p[bOffset];
}

uint16_t Adafruit_DotStar::numPixels(void) { // Ret. strip length
  return numLEDs;
}

// Set global strip brightness.  This does not have an immediate effect;
// must be followed by a call to show().  Not a fan of this...for various
// reasons I think it's better handled in one's sketch, but it's here for
// parity with the NeoPixel library.  Good news is that brightness setting
// in this library is 'non destructive' -- it's applied as color data is
// being issued to the strip, not during setPixel(), and also means that
// getPixelColor() returns the exact value originally stored.
void Adafruit_DotStar::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.  This
  // optimizes the actual scaling math later, allowing a fast 8x8-bit
  // multiply and taking the MSB.  'brightness' is a uint8_t, adding 1
  // here may (intentionally) roll over...so 0 = max brightness (color
  // values are interpreted literally; no scaling), 1 = min brightness
  // (off), 255 = just below max brightness.
  brightness = b + 1;
}

uint8_t Adafruit_DotStar::getBrightness(void) const {
  return brightness - 1; // Reverse above operation
}

// Return pointer to the library's pixel data buffer.  Use carefully,
// much opportunity for mayhem.  It's mostly for code that needs fast
// transfers, e.g. SD card to LEDs.  Color data is in BGR order.
uint8_t *Adafruit_DotStar::getPixels(void) const {
  return pixels;
}

/* A PROGMEM (flash mem) table containing 8-bit unsigned sine wave (0-255).
   Copy & paste this snippet into a Python REPL to regenerate:
import math
for x in range(256):
    print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
    if x&15 == 15: print
*/
static const uint8_t PROGMEM _sineTable[256] = {
  128,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,
  176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,
  218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,
  245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,
  255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,
  245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,
  218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,
  176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,
  128,124,121,118,115,112,109,106,103,100, 97, 93, 90, 88, 85, 82,
   79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40,
   37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11,
   10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,
    0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,
   10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
   37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
   79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124};

/* Similar to above, but for an 8-bit gamma-correction table.
   Copy & paste this snippet into a Python REPL to regenerate:
import math
gamma=2.6
for x in range(256):
    print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
    if x&15 == 15: print
*/
static const uint8_t PROGMEM _gammaTable[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
   13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
   20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
   30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
   42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
   58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
   76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
   97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
  122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
  150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
  182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
  218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255};

uint8_t Adafruit_DotStar::sine8(uint8_t x) const {
  return pgm_read_byte(&_sineTable[x]); // 0-255 in, 0-255 out
}

uint8_t Adafruit_DotStar::gamma8(uint8_t x) const {
  return pgm_read_byte(&_gammaTable[x]); // 0-255 in, 0-255 out
}

