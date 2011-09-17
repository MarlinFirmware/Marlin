#ifndef __LCDH
#define __LCDH

#if defined FANCY_LCD || defined SIMPLE_LCD
  #define LCD_UPDATE_INTERVAL 400

	#include "Configuration.h"

	#include <LiquidCrystal.h>
	extern LiquidCrystal lcd;

	//lcd display size
	#ifdef FANCY_LCD
		#define LCD_WIDTH 20
		#define LCD_HEIGHT 4
	#else
		#define LCD_WIDTH 16
		#define LCD_HEIGHT 2
	#endif

	//arduino pin witch triggers an piezzo beeper
	#define BEEPER 18

	void lcd_status();
	void lcd_init();
	void lcd_status(const char* message);
	void beep();



	#define LCD_MESSAGE(x) lcd_status(x);
	#define LCD_STATUS lcd_status()
#else
	#define LCD_STATUS
	#define LCD_MESSAGE(x)
#endif

#endif
