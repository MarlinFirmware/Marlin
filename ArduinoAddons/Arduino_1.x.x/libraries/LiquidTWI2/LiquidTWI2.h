#ifndef LiquidTWI2_h
#define LiquidTWI2_h

#include <inttypes.h>
#include "Print.h"

// for memory-constrained projects, comment out the MCP230xx that doesn't apply
#define MCP23017 // Adafruit RGB LCD (PANELOLU2 is now supported without additional define)
#define MCP23008 // Adafruit I2C Backpack

// if DETECT_DEVICE is enabled, then when constructor's detectDevice != 0
// device will be detected in the begin() function...
// if the device isn't detected in begin() then we won't try to talk to the
// device in any of the other functions... this allows you to compile the
// code w/o an LCD installed, and not get hung in the write functions
#define DETECT_DEVICE // enable device detection code

// for setBacklight() with MCP23017
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7 

// Standard directional button bits
#define BUTTON_UP 0x08
#define BUTTON_DOWN 0x04
#define BUTTON_LEFT 0x10
#define BUTTON_RIGHT 0x02
#define BUTTON_SELECT 0x01

// Panelolu2 encoder button bits (which has only rotary encoder and encoder button)
#define PANELOLU2_ENCODER_C 0x04 // == encoder button
#define PANELOLU2_ENCODER_B 0x02
#define PANELOLU2_ENCODER_A 0x01

// readButtons() will only return these bit values 
// (the Panelolu2 encoder bits are subset of these bits)
#define ALL_BUTTON_BITS (BUTTON_UP|BUTTON_DOWN|BUTTON_LEFT|BUTTON_RIGHT|BUTTON_SELECT)

#define MCP23008_ADDRESS 0x20

// registers
#define MCP23008_IODIR 0x00
#define MCP23008_IPOL 0x01
#define MCP23008_GPINTEN 0x02
#define MCP23008_DEFVAL 0x03
#define MCP23008_INTCON 0x04
#define MCP23008_IOCON 0x05
#define MCP23008_GPPU 0x06
#define MCP23008_INTF 0x07
#define MCP23008_INTCAP 0x08
#define MCP23008_GPIO 0x09
#define MCP23008_OLAT 0x0A

#define MCP23017_ADDRESS 0x20

// registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14


#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

// commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_BACKLIGHT  0x08 // used to pick out the backlight flag since _displaycontrol will never set itself

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
//we only support 4-bit mode #define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// for setMCPType()
#define LTI_TYPE_MCP23008 0
#define LTI_TYPE_MCP23017 1
#define DEFAULT_TYPE LTI_TYPE_MCP23008

class LiquidTWI2 : public Print {
public:
	LiquidTWI2(uint8_t i2cAddr,uint8_t detectDevice=0,uint8_t backlightInverted=0);

	void begin(uint8_t cols, uint8_t rows,uint8_t charsize = LCD_5x8DOTS);

#ifdef DETECT_DEVICE
	uint8_t LcdDetected() { return _deviceDetected; }
#endif // DETECT_DEVICE
	void clear();
	void home();

	void noDisplay();
	void display();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void leftToRight();
	void rightToLeft();
	void autoscroll();
	void noAutoscroll();

	void setBacklight(uint8_t status); 

	void createChar(uint8_t, uint8_t[]);
	void setCursor(uint8_t, uint8_t); 
#if defined(ARDUINO) && (ARDUINO >= 100) // scl
	virtual size_t write(uint8_t);
#else
	virtual void write(uint8_t);
#endif
	void command(uint8_t);
#ifdef MCP23017
	uint8_t readButtons();
  //check registers
  uint8_t readRegister(uint8_t);
  //set registers
  void setRegister(uint8_t, uint8_t);
  //make some noise
  void buzz(long,uint16_t);
#endif
	void setMCPType(uint8_t mcptype) {
#if defined(MCP23017)&&defined(MCP23008)
	  _mcpType = mcptype;
#endif //defined(MCP23017)&&defined(MCP23008)
	}


private:
	void send(uint8_t, uint8_t);
#ifdef MCP23017
	void burstBits16(uint16_t);
	void burstBits8b(uint8_t);
	//void burstBits8a(uint8_t);
#endif
#ifdef MCP23008
	void burstBits8(uint8_t);
#endif

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines,_currline;
	uint8_t _i2cAddr;
	uint8_t _backlightInverted;
#ifdef DETECT_DEVICE
	uint8_t _deviceDetected;
#endif // DETECT_DEVICE
#ifdef MCP23017
	uint16_t _backlightBits; // only for MCP23017
#endif
#if defined(MCP23017)&&defined(MCP23008)
	uint8_t _mcpType; // LTI_MODE_xx
#endif

};

#endif
