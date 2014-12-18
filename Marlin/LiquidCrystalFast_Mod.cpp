/*
 *  LiquidCrystalFast.cpp, version 1.1
 * 
 Copyright (c) 2010 John Raines

 Minor changes by Paul Stoffregen <paul@pjrc.com>, Copyright 2011
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General
 Public License along with this library; if not, write to the
 Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 Boston, MA  02111-1307  USA
 
 */

//#include "LiquidCrystalFast.h"
#include "LiquidCrystalFast_Mod.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
//#include <stdio.h>
//#include <string.h>


/******************************************/
/**  hardware initialization             **/
/******************************************/

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
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
// LiquidCrystalFast constructor is called).

void LiquidCrystalFast::init(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t en2,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	_rs_pin = rs;
	_rw_pin = 255;
	_enable_pin = enable;
	_en2 = en2;
	_chip = 0;
	_scroll_count = 0;  //to fix the bug if we scroll and then setCursor w/o home() or clear()
	_x = 0;
	_y = 0;
	
	_setCursFlag = 0;
	_direction = LCD_Right;

	_data_pins[0] = d0;
	_data_pins[1] = d1;
	_data_pins[2] = d2;
	_data_pins[3] = d3; 

	pinMode(d0, OUTPUT); //set data pin modes
	pinMode(d1, OUTPUT); 
	pinMode(d2, OUTPUT); 
	pinMode(d3, OUTPUT); 

	row_offsets[0] = 00;   // DDRAM addresses inside the HD44780 are strange: 0-nColumns-1 on line 0
	row_offsets[1] = 0x40; // 64-(63+nColumns) for line 1
	row_offsets[2] = 0x14; // 20- (19+nColumns) for line 2 --- NOTHING FROM 40-63 !
	row_offsets[3] = 0x54; // 84 - (83+nColumns) for line 3  -- so 80 characters tops out at #103 !
	
	pinMode(_rs_pin, OUTPUT);
	// we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
	if (rw != 255) { 
		pinMode(rw, OUTPUT);  //once in init does it
		digitalWrite(rw,LOW); //write data to LCD mode
	}
	pinMode(_enable_pin, OUTPUT);
	if( en2 != 255) pinMode(en2,OUTPUT);  //4X40 LCD

	begin(20, 1); 
	_rw_pin = rw;         //the game to initialize the 40x4 is over
}

void LiquidCrystalFast::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
	numcols=_numcols=cols;    //there is an implied lack of trust; the private version can't be munged up by the user.
	numlines=_numlines=lines;
	row_offsets[2] = cols + row_offsets[0];  //should autoadjust for 16/20 or whatever columns now
	row_offsets[3] = cols + row_offsets[1];
	begin2( cols,  lines,  dotsize, _enable_pin);
	
	if (_en2 != 255) {                      //if we were called with a 2nd enable line i.e. 4x40 LCD
		row_offsets[2] = 0;
		row_offsets[3] = 0x40;             //each line gets its own little 40 char section of DDRAM--would be fine if there were a 4x32, I suppose
		_chip = 2;
		begin2( cols,  lines,  dotsize,_en2);//initialize the second HD44780 chip
	}
}

void LiquidCrystalFast::begin2(uint8_t cols, uint8_t lines, uint8_t dotsize, uint8_t enable)
{  
	uint8_t	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (lines > 1) {
		displayfunction |= LCD_2LINE;
	}
	
	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
		displayfunction |= LCD_5x10DOTS;
	}
	
	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way before 4.5V.
	// is this delay long enough for all cases??
	for (uint8_t i=0;i<18;i++) {
		delayMicroseconds(7500);
	}

	// Now we pull both RS and R/W low to begin commands
	digitalWrite(_rs_pin, LOW);
	digitalWrite(enable, LOW);
	
	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
		
	// we start in 8bit mode, try to set 4 bit mode
	// at this point we are in 8 bit mode but of course in this
	// interface 4 pins are dangling unconnected and the values
	// on them don't matter for these instructions.
	digitalWrite(_rs_pin, LOW);
	write4bits(0x03);
	delayMicroseconds(5000); // I have one LCD for which 4500 here was not long enough.
	// second try
	write4bits(0x03);      
	delayMicroseconds(150); // wait 
	// third go!
	write4bits(0x03); 
	delayMicroseconds(150);
	// finally, set to 4-bit interface
	write4bits(0x02); 
	delayMicroseconds(150);

	
	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | displayfunction);  
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  display();
	
	// clear it off
	clear();
	
	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);	
}


/******************************************/
/**  high level commands, for the user!   */
/******************************************/

void LiquidCrystalFast::clear()
{
	if (_en2 != 255) {
		_chip=2;
		command(LCD_CLEARDISPLAY); 
		_chip=0;
		command(LCD_CLEARDISPLAY);
		delayPerHome();
		setCursor(0,0);
	} else {
		command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
		delayPerHome();
	}
	_scroll_count = 0;
}

void LiquidCrystalFast::home()
{
	commandBoth(LCD_RETURNHOME);  // set cursor position to zero both chips.
	delayPerHome();
	_scroll_count = 0;
	if (_en2 != 255) setCursor(0,0); 
}


// Turn the display on/off (quickly)
void LiquidCrystalFast::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	commandBoth(LCD_DISPLAYCONTROL | _displaycontrol);  //both chips
}
void LiquidCrystalFast::display() {
	_displaycontrol |= LCD_DISPLAYON;
	commandBoth(LCD_DISPLAYCONTROL | _displaycontrol);   //both chips
}

// Turns the underline cursor on/off
void LiquidCrystalFast::noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalFast::cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystalFast::noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalFast::blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystalFast::scrollDisplayLeft(void) {
	commandBoth(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);  //both chips
	_scroll_count++;
	if (_scroll_count >= 40) _scroll_count -= 40;   //  -39< scroll_count<39
}
void LiquidCrystalFast::scrollDisplayRight(void) {
	commandBoth(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);  //both chips
	_scroll_count--;
	if (_scroll_count <= -40) _scroll_count += 40;
}

// This is for text that flows Left to Right
void LiquidCrystalFast::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	_direction = LCD_Right;
	commandBoth(LCD_ENTRYMODESET | _displaymode);     //both chips
}

// This is for text that flows Right to Left
void LiquidCrystalFast::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	_direction = LCD_Left;
	commandBoth(LCD_ENTRYMODESET | _displaymode);    //both chips
}


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystalFast::createChar(uint8_t location, uint8_t charmap[]) {    
	location &= 0x7; // we only have 8 locations 0-7
	if (_en2 == 255) {
		command(LCD_SETCGRAMADDR | (location << 3));
		for (int i=0; i<8; i++) {
			send(charmap[i],HIGH);
		}
	}
	else {
		uint8_t chipSave = _chip;
		_chip = 0;
		command(LCD_SETCGRAMADDR | (location << 3));
		for (uint8_t i=0; i<8; i++) {
			send(charmap[i],HIGH);
		}
		_chip = 2;
		command(LCD_SETCGRAMADDR | (location << 3));
		for (uint8_t i=0; i<8; i++) {
			send(charmap[i],HIGH);
		}
		_chip = chipSave;
	}
}

void LiquidCrystalFast::setCursor(uint8_t col, uint8_t row)         // this can be called by the user but is also called before writing some characters.
{
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	_y = row;
	_x = col;
	_setCursFlag = 0;                                                 //user did a setCursor--clear the flag that may have been set in write()
	int8_t high_bit = row_offsets[row] & 0x40;                        // this keeps coordinates pegged to a spot on the LCD screen even if the user scrolls right or
	int8_t  offset = col + (row_offsets[row] &0x3f)  + _scroll_count; //left under program control. Previously setCursor was pegged to a location in DDRAM
	//the 3 quantities we add are each <40
	if (offset > 39) offset -= 40;                                    // if the display is autoscrolled this method does not work, however.
	if (offset < 0) offset += 40;
	offset |= high_bit;
	if (_chip != (row & 0b10)) noCursor();  //turn off cursor on chip we are leaving
	_chip = row & 0b10;                     //if it is row 0 or 1 this is 0; if it is row 2 or 3 this is 2
	command(LCD_SETDDRAMADDR | (byte) offset );
}

// This will 'right justify' text from the cursor 
void LiquidCrystalFast::autoscroll(void) {           //to count the number of times we scrolled; here we'd need to keep track of microseconds and divide. I'm not going there.
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	commandBoth(LCD_ENTRYMODESET | _displaymode);   //both chips
}

// This will 'left justify' text from the cursor
void LiquidCrystalFast::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;   //both chips
	commandBoth(LCD_ENTRYMODESET | _displaymode);
}


/*************************************************/
/**  mid level commands, for sending data/cmds   */
/*************************************************/

inline void LiquidCrystalFast::command(uint8_t value) {
	send(value, LOW);
}

//for many of the commands that need to be sent twice if 2 controller chips
void LiquidCrystalFast::commandBoth(uint8_t value)
{
	if (_en2 == 255) {
		send(value, LOW);  // normal LCD, single controller chip
	} else {
		uint8_t chipSave = _chip;
		_chip = 0;
		send(value,LOW);   //send command to first HD44780
		_chip = 2;
		send(value,LOW);   //send to 2nd HD44780
		_chip=chipSave;
	}
}

//print calls  this to send characters to the LCD
#if defined(ARDUINO) && ARDUINO >= 100
size_t LiquidCrystalFast::write(uint8_t value) {
#else
void LiquidCrystalFast::write(uint8_t value) {
#endif

	if ((_scroll_count != 0) || (_setCursFlag != 0) ) setCursor(_x,_y);   //first we call setCursor and send the character
	if ((value != '\r') && (value != '\n') ) send(value, HIGH);

	_setCursFlag = 0;
	if (_direction == LCD_Right) {                    // then we update the x & y location for the NEXT character
		_x++;
		if ((value == '\r') ||(_x >= _numcols) ) {      //romance languages go left to right
			
			_x = 0;
			_y++;
			_setCursFlag = 1;          //we'll need a setCursor() before the next char to move to begin of next line
		}
	} else {
		_x--;
		if ( (value == '\n') || (_x < 0)) {   //emulate right to left text mode which is built in but would be defeated by my code above
			_x = _numcols-1;
			_y++;
			_setCursFlag = 1;
		}
	}
	if (_y >= _numlines) _y = 0;   //wrap last line up to line 0
#if defined(ARDUINO) && ARDUINO >= 100
	return 1;
#endif
}


/****************************************/
/**  low level data pushing commands   **/
/****************************************/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalFast::send(uint8_t value, uint8_t mode) {
	uint8_t en = _enable_pin;
	if ((_en2 != 255) && (_chip)) en = _en2;
	if (_rw_pin == 255) {
		delayMicroseconds(DELAYPERCHAR);
	} else {
		pinMode(_data_pins[0], INPUT);
		pinMode(_data_pins[1], INPUT);
		pinMode(_data_pins[2], INPUT);
		pinMode(_data_pins[3], INPUT);
		digitalWrite(_rw_pin, HIGH);
		digitalWrite(_rs_pin, LOW);
		uint8_t busy;
		do {
			digitalWrite(en, HIGH);
			busy = digitalRead(_data_pins[3]);
			digitalWrite(en, LOW);
			digitalWrite(en, HIGH);
			digitalWrite(en, LOW);
		} while (busy == HIGH);
		pinMode(_data_pins[0], OUTPUT);
		pinMode(_data_pins[1], OUTPUT);
		pinMode(_data_pins[2], OUTPUT);
		pinMode(_data_pins[3], OUTPUT);
		digitalWrite(_rw_pin, LOW);
	}
	digitalWrite(_rs_pin, mode);

	digitalWrite(_data_pins[0], value & 0x10);
	digitalWrite(_data_pins[1], value & 0x20);
	digitalWrite(_data_pins[2], value & 0x40);
	digitalWrite(_data_pins[3], value & 0x80);
	digitalWrite(en, HIGH);   // enable pulse must be >450ns
	digitalWrite(en, LOW);

	digitalWrite(_data_pins[0], value & 0x01);
	digitalWrite(_data_pins[1], value & 0x02);
	digitalWrite(_data_pins[2], value & 0x04);
	digitalWrite(_data_pins[3], value & 0x08);
	digitalWrite(en, HIGH);   // enable pulse must be >450ns
	digitalWrite(en, LOW);
}

// used during init
void LiquidCrystalFast::write4bits(uint8_t value)
{
	uint8_t v=value;
	uint8_t *pinptr = _data_pins;
	digitalWrite(*pinptr++, v & 1 );
	digitalWrite(*pinptr++,( v >>= 1) & 1 );
	digitalWrite(*pinptr++,( v >>= 1) & 1 );
	digitalWrite(*pinptr++,( v >>= 1) & 1 );
	byte en = _enable_pin;
	if ((_en2 != 255) && (_chip)) en = _en2;    // 4x40 LCD with 2 controller chips with separate enable lines if we called w 2 enable pins and are on lines 2 or 3 enable chip 2  
	digitalWrite(en, HIGH);   // enable pulse must be >450ns
	digitalWrite(en, LOW);
}
 
