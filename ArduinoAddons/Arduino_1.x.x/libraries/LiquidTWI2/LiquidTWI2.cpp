#include "LiquidTWI2.h"

/*
  LiquidTWI2 High Performance i2c LCD driver for MCP23008 & MCP23017
  hacked by Sam C. Lin / http://www.lincomatic.com
  from 
   LiquidTWI by Matt Falcon (FalconFour) / http://falconfour.com
   logic gleaned from Adafruit RGB LCD Shield library
   Panelolu2 support by Tony Lock / http://blog.think3dprint3d.com
   enhancements by Nick Sayer / https://github.com/nsayer

  Compatible with Adafruit I2C LCD backpack (MCP23008) and
  Adafruit RGB LCD Shield
*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if defined (__AVR_ATtiny84__) || defined(__AVR_ATtiny85__) || (__AVR_ATtiny2313__)
#include "TinyWireM.h"
#define Wire TinyWireM
#else
#include <Wire.h>
#endif
#if defined(ARDUINO) && (ARDUINO >= 100) //scl
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


//MCP23017 - Adafruit RGB LCD Shield
// bit pattern for the burstBits function is
//
//  B7 B6 B5 B4 B3 B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0 - MCP23017 
//  RS RW EN D4 D5 D6 D7 LB LG LR BZ B4 B3 B2 B1 B0 
//  15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0  
#define M17_BIT_RS 0x8000
#define M17_BIT_RW 0x4000
#define M17_BIT_EN 0x2000
#define M17_BIT_D4 0x1000
#define M17_BIT_D5 0x0800
#define M17_BIT_D6 0x0400
#define M17_BIT_D7 0x0200
#define M17_BIT_LB 0x0100
#define M17_BIT_LG 0x0080
#define M17_BIT_LR 0x0040
#define M17_BIT_BZ 0x0020 //Added a buzzer on this pin
#define M17_BIT_B4 0x0010
#define M17_BIT_B3 0x0008
#define M17_BIT_B2 0x0004
#define M17_BIT_B1 0x0002
#define M17_BIT_B0 0x0001

static inline void wiresend(uint8_t x) {
#if ARDUINO >= 100
  Wire.write((uint8_t)x);
#else
  Wire.send(x);
#endif
}

static inline uint8_t wirerecv(void) {
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}



// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 0; 4-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidTWI2 constructor is called). This is why we save the init commands
// for when the sketch calls begin(), except configuring the expander, which
// is required by any setup.

LiquidTWI2::LiquidTWI2(uint8_t i2cAddr,uint8_t detectDevice, uint8_t backlightInverted) {
  // if detectDevice != 0, set _deviceDetected to 2 to flag that we should
  // scan for it in begin()
#ifdef DETECT_DEVICE
  _deviceDetected = detectDevice ? 2 : 1;
#endif

  _backlightInverted = backlightInverted;

  //  if (i2cAddr > 7) i2cAddr = 7;
  _i2cAddr = i2cAddr; // transfer this function call's number into our internal class state
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS; // in case they forget to call begin() at least we have something
#if defined(MCP23017)&&defined(MCP23008)
  _mcpType = DEFAULT_TYPE; // default
#endif
}

void LiquidTWI2::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delay(50);

  Wire.begin();

  uint8_t result;
#if defined(MCP23017)&&defined(MCP23008)
  if (_mcpType == LTI_TYPE_MCP23017) {
#endif
#ifdef MCP23017
    /* don't think we need this
    // set defaults!
    Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
    wiresend(MCP23017_IODIRA);
    wiresend(0xFF);  // all inputs on port A
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
	  
    Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
    wiresend(MCP23017_IODIRB);
    wiresend(0xFF);  // all inputs on port B
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
    */

    // now set up input/output pins
    Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
    wiresend(MCP23017_IODIRA);
    wiresend(0x1F); // buttons input, all others output
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
    
    // set the button pullups
    Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
    wiresend(MCP23017_GPPUA);
    wiresend(0x1F);	
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
    
    Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
    wiresend(MCP23017_IODIRB);
    wiresend(0x00); // all pins output
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
#endif // MCP23017
#if defined(MCP23017)&&defined(MCP23008)
  }
  else { // MCP23008
#endif
#ifdef MCP23008
    // first thing we do is get the GPIO expander's head working straight, with a boatload of junk data.
    Wire.beginTransmission(MCP23008_ADDRESS | _i2cAddr);
    wiresend(MCP23008_IODIR);
    wiresend(0xFF);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);
    wiresend(0x00);	
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
	  
    // now we set the GPIO expander's I/O direction to output since it's soldered to an LCD output.
    Wire.beginTransmission(MCP23008_ADDRESS | _i2cAddr);
    wiresend(MCP23008_IODIR);
    wiresend(0x00); // all output: 00000000 for pins 1...8
    result = Wire.endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
#endif // MCP23008
#if defined(MCP23017)&&defined(MCP23008)
  }
#endif

#ifdef DETECT_DEVICE
  // If we haven't failed by now, then we pass
  if (_deviceDetected == 2) _deviceDetected = 1;
#endif

  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  //put the LCD into 4 bit mode
  // start with a non-standard command to make it realize we're speaking 4-bit here
  // per LCD datasheet, first command is a single 4-bit burst, 0011.
  //-----
  //  we cannot assume that the LCD panel is powered at the same time as
  //  the arduino, so we have to perform a software reset as per page 45
  //  of the HD44780 datasheet - (kch)
  //-----
#if defined(MCP23017)&&defined(MCP23008)
  if (_mcpType == LTI_TYPE_MCP23017) {
#endif // defined(MCP23017)&&defined(MCP23008)
#ifdef MCP23017
    _backlightBits = 0; // all backlight LED's on
	  
    // bit pattern for the burstBits function is
    //
    //  B7 B6 B5 B4 B3 B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0 - MCP23017 
    //  15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0  
    //  RS RW EN D4 D5 D6 D7 B  G  R     B4 B3 B2 B1 B0 
    for (uint8_t i=0;i < 3;i++) {
      burstBits8b((M17_BIT_EN|M17_BIT_D5|M17_BIT_D4) >> 8);
      burstBits8b((M17_BIT_D5|M17_BIT_D4) >> 8);
    }
    burstBits8b((M17_BIT_EN|M17_BIT_D5) >> 8);
    burstBits8b(M17_BIT_D5 >> 8);
#endif // MCP23017
#if defined(MCP23017)&&defined(MCP23008)
  }
  else {
#endif // defined(MCP23017)&&defined(MCP23008)
#ifdef MCP23008
    // bit pattern for the burstBits function is
    //
    //  7   6   5   4   3   2   1   0
    // LT  D7  D6  D5  D4  EN  RS  n/c
    //-----
    burstBits8(B10011100); // send LITE D4 D5 high with enable
    burstBits8(B10011000); // send LITE D4 D5 high with !enable
    burstBits8(B10011100); //
    burstBits8(B10011000); //
    burstBits8(B10011100); // repeat twice more
    burstBits8(B10011000); //
    burstBits8(B10010100); // send D4 low and LITE D5 high with enable
    burstBits8(B10010000); // send D4 low and LITE D5 high with !enable
#endif // MCP23008
#if defined(MCP23017)&&defined(MCP23008)
  }
#endif

  delay(5); // this shouldn't be necessary, but sometimes 16MHz is stupid-fast.

  command(LCD_FUNCTIONSET | _displayfunction); // then send 0010NF00 (N=lines, F=font)
  delay(5); // for safe keeping...
  command(LCD_FUNCTIONSET | _displayfunction); // ... twice.
  delay(5); // done!

  // turn on the LCD with our defaults. since these libs seem to use personal preference, I like a cursor.
  _displaycontrol = (LCD_DISPLAYON|LCD_BACKLIGHT);
  display();
  // clear it off
  clear();

  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
}

/********** high level commands, for the user! */
void LiquidTWI2::clear()
{
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidTWI2::home()
{
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidTWI2::setCursor(uint8_t col, uint8_t row)
{
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) row = _numlines - 1;    // we count rows starting w/0
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidTWI2::noDisplay() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidTWI2::display() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidTWI2::noCursor() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidTWI2::cursor() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidTWI2::noBlink() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidTWI2::blink() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidTWI2::scrollDisplayLeft(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidTWI2::scrollDisplayRight(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidTWI2::leftToRight(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidTWI2::rightToLeft(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidTWI2::autoscroll(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidTWI2::noAutoscroll(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidTWI2::createChar(uint8_t location, uint8_t charmap[]) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */
inline void LiquidTWI2::command(uint8_t value) {
  send(value, LOW);
}
#if defined(ARDUINO) && (ARDUINO >= 100) //scl
inline size_t LiquidTWI2::write(uint8_t value) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return 1;
#endif
  send(value, HIGH);
  return 1;
}
#else
inline void LiquidTWI2::write(uint8_t value) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  send(value, HIGH);
}
#endif

/************ low level data pushing commands **********/
#ifdef MCP23017
uint8_t LiquidTWI2::readButtons(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return 0;
#endif
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  wiresend(MCP23017_GPIOA);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | _i2cAddr, 1);
  return ~wirerecv() & ALL_BUTTON_BITS;
}
#endif // MCP23017

// Allows to set the backlight, if the LCD backpack is used
void LiquidTWI2::setBacklight(uint8_t status) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  if (_backlightInverted) status ^= 0x7;
#if defined(MCP23017)&&defined(MCP23008)
  if (_mcpType == LTI_TYPE_MCP23017) {
#endif
#ifdef MCP23017
    // LED turns on when bit is cleared
    _backlightBits = M17_BIT_LB|M17_BIT_LG|M17_BIT_LR; // all off
    if (status & RED) _backlightBits &= ~M17_BIT_LR; // red on
    if (status & GREEN) _backlightBits &= ~M17_BIT_LG; // green on
    if (status & BLUE) _backlightBits &= ~M17_BIT_LB; // blue on
    
    burstBits16(_backlightBits);
#endif // MCP23017
#if defined(MCP23017)&&defined(MCP23008)
  }
  else {
#endif
#ifdef MCP23008
    bitWrite(_displaycontrol,3,status); // flag that the backlight is enabled, for burst commands
    burstBits8((_displaycontrol & LCD_BACKLIGHT)?0x80:0x00);
#endif // MCP23008
#if defined(MCP23017)&&defined(MCP23008)
  }
#endif
}

// write either command or data, burst it to the expander over I2C.
void LiquidTWI2::send(uint8_t value, uint8_t mode) {
#if defined(MCP23017)&&defined(MCP23008)
  if (_mcpType == LTI_TYPE_MCP23017) {
#endif
#ifdef MCP23017
    // BURST SPEED, OH MY GOD
    // the (now High Speed!) I/O expander pinout
    //  B7 B6 B5 B4 B3 B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0 - MCP23017 
    //  15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0  
    //  RS RW EN D4 D5 D6 D7 B  G  R     B4 B3 B2 B1 B0 
    
    // n.b. RW bit stays LOW to write
    uint8_t buf = _backlightBits >> 8;
    // send high 4 bits
    if (value & 0x10) buf |= M17_BIT_D4 >> 8;
    if (value & 0x20) buf |= M17_BIT_D5 >> 8;
    if (value & 0x40) buf |= M17_BIT_D6 >> 8;
    if (value & 0x80) buf |= M17_BIT_D7 >> 8;
    
    if (mode) buf |= (M17_BIT_RS|M17_BIT_EN) >> 8; // RS+EN
    else buf |= M17_BIT_EN >> 8; // EN

    burstBits8b(buf);

    // resend w/ EN turned off
    buf &= ~(M17_BIT_EN >> 8);
    burstBits8b(buf);
    
    // send low 4 bits
    buf = _backlightBits >> 8;
    // send high 4 bits
    if (value & 0x01) buf |= M17_BIT_D4 >> 8;
    if (value & 0x02) buf |= M17_BIT_D5 >> 8;
    if (value & 0x04) buf |= M17_BIT_D6 >> 8;
    if (value & 0x08) buf |= M17_BIT_D7 >> 8;
    
    if (mode) buf |= (M17_BIT_RS|M17_BIT_EN) >> 8; // RS+EN
    else buf |= M17_BIT_EN >> 8; // EN
    
    burstBits8b(buf);
    
    // resend w/ EN turned off
    buf &= ~(M17_BIT_EN >> 8);
    burstBits8b(buf);
#endif // MCP23017
#if defined(MCP23017)&&defined(MCP23008)
  }
  else {
#endif
#ifdef MCP23008
    // BURST SPEED, OH MY GOD
    // the (now High Speed!) I/O expander pinout
    // RS pin = 1
    // Enable pin = 2
    // Data pin 4 = 3
    // Data pin 5 = 4
    // Data pin 6 = 5
    // Data pin 7 = 6
    byte buf;
    // crunch the high 4 bits
    buf = (value & B11110000) >> 1; // isolate high 4 bits, shift over to data pins (bits 6-3: x1111xxx)
    if (mode) buf |= 3 << 1; // here we can just enable enable, since the value is immediately written to the pins
    else buf |= 2 << 1; // if RS (mode), turn RS and enable on. otherwise, just enable. (bits 2-1: xxxxx11x)
    buf |= (_displaycontrol & LCD_BACKLIGHT)?0x80:0x00; // using DISPLAYCONTROL command to mask backlight bit in _displaycontrol
    burstBits8(buf); // bits are now present at LCD with enable active in the same write
    // no need to delay since these things take WAY, WAY longer than the time required for enable to settle (1us in LCD implementation?)
    buf &= ~(1<<2); // toggle enable low
    burstBits8(buf); // send out the same bits but with enable low now; LCD crunches these 4 bits.
    // crunch the low 4 bits
    buf = (value & B1111) << 3; // isolate low 4 bits, shift over to data pins (bits 6-3: x1111xxx)
    if (mode) buf |= 3 << 1; // here we can just enable enable, since the value is immediately written to the pins
    else buf |= 2 << 1; // if RS (mode), turn RS and enable on. otherwise, just enable. (bits 2-1: xxxxx11x)
    buf |= (_displaycontrol & LCD_BACKLIGHT)?0x80:0x00; // using DISPLAYCONTROL command to mask backlight bit in _displaycontrol
    burstBits8(buf);
    buf &= ~( 1 << 2 ); // toggle enable low (1<<2 = 00000100; NOT = 11111011; with "and", this turns off only that one bit)
    burstBits8(buf);
#endif // MCP23008
#if defined(MCP23017)&&defined(MCP23008)
  }
#endif
}

#ifdef MCP23017
// value byte order is BA
void LiquidTWI2::burstBits16(uint16_t value) {
  // we use this to burst bits to the GPIO chip whenever we need to. avoids repetitive code.
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  wiresend(MCP23017_GPIOA);
  wiresend(value & 0xFF); // send A bits
  wiresend(value >> 8);   // send B bits
  while(Wire.endTransmission());
}

/*
void LiquidTWI2::burstBits8a(uint8_t value) {
  // we use this to burst bits to the GPIO chip whenever we need to. avoids repetitive code.
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  wiresend(MCP23017_GPIOA);
  wiresend(value); // last bits are crunched, we're done.
  while(Wire.endTransmission());
}
*/
void LiquidTWI2::burstBits8b(uint8_t value) {
  // we use this to burst bits to the GPIO chip whenever we need to. avoids repetitive code.
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  wiresend(MCP23017_GPIOB);
  wiresend(value); // last bits are crunched, we're done.
  while(Wire.endTransmission());
}
#endif // MCP23017
#ifdef MCP23008
void LiquidTWI2::burstBits8(uint8_t value) {
  // we use this to burst bits to the GPIO chip whenever we need to. avoids repetitive code.
  Wire.beginTransmission(MCP23008_ADDRESS | _i2cAddr);
  wiresend(MCP23008_GPIO);
  wiresend(value); // last bits are crunched, we're done.
  while(Wire.endTransmission());
}
#endif // MCP23008

#ifdef MCP23017
//direct access to the registers for interrupt setting and reading, also the tone function using buzzer pin
uint8_t LiquidTWI2::readRegister(uint8_t reg) {
  // read a register
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  wiresend(reg);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | _i2cAddr, 1);
  return wirerecv();
}

//set registers
void LiquidTWI2::setRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
    wiresend(reg);
    wiresend(value);
    Wire.endTransmission();
}

//cycle the buzzer pin at a certain frequency (hz) for a certain duration (ms) 
//note: a 100Khz TWI/I2C bus on a 16Mhz Arduino will max out at around 1500Hz freq
void LiquidTWI2::buzz(long duration, uint16_t freq) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  int currentRegister = 0;

  // read gpio register
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  wiresend(MCP23017_GPIOA);	
  Wire.endTransmission();
  Wire.requestFrom(MCP23017_ADDRESS | _i2cAddr, 1);
  currentRegister = wirerecv();
  
  duration *=1000; //convert from ms to us
  unsigned long cycletime = 1000000UL / freq; // period in us
  unsigned long cycles = (unsigned long)duration / cycletime;
  unsigned long ontime;
  while (cycles-- > 0)
  {
    ontime = micros();
        Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
        wiresend(MCP23017_GPIOA);
        wiresend(currentRegister |= M17_BIT_BZ);
        while(Wire.endTransmission());
    while((long)(ontime + (cycletime/2) - micros()) > 0);
        Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
        wiresend(MCP23017_GPIOA);
        wiresend(currentRegister &= ~M17_BIT_BZ);
        while(Wire.endTransmission());
    while((long)(ontime + cycletime - micros()) > 0);
   }
}
#endif //MCP23017
