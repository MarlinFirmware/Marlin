#ifndef __LCDH
#define __LCDH

#ifdef SIMPLE_LCD
        #define LCD_UPDATE_INTERVAL 400

	#include "Configuration.h"
	#include <LiquidCrystal.h>
	extern LiquidCrystal lcd;

	//lcd display size
	#define LCD_WIDTH 16
	#define LCD_HEIGHT 2

	void lcd_status();
	void lcd_status(const char* message);
	void lcd_init();
        void lcd_activate();
        void lcd_update();


	#define LCD_MESSAGE(x) lcd_status(x);
	#define LCD_STATUS lcd_status()
#else
	#define LCD_STATUS
	#define LCD_MESSAGE(x)
#endif
#endif

