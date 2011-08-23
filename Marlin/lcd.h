#ifndef __LCDH
#define __LCDH

#ifdef FANCY_LCD

	#include "Configuration.h"

	#include <LiquidCrystal.h>
	extern LiquidCrystal lcd;

	//lcd display size
	#define LCD_WIDTH 20
	#define LCD_HEIGHT 4

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
