#ifndef ULTRA_LCD_IMPLEMENTATION_DOGM_H
#define ULTRA_LCD_IMPLEMENTATION_DOGM_H

/**
* Implementation of the LCD display routines for a DOGM128 graphic display. These are common LCD 128x64 pixel graphic displays.
**/


// CHANGE_DE begin ***
#include <U8glib.h>	// DE_U8glib
#include "start_bmp.h"


/* Russian language not supported yet, needs custom font

#if LANGUAGE_CHOICE == 6
#include "LiquidCrystalRus.h"
#define LCD_CLASS LiquidCrystalRus
#else
#include <LiquidCrystal.h>
#define LCD_CLASS LiquidCrystal
#endif
*/

// DOGM parameters (size in pixels)
#define DOG_CHAR_WIDTH			6
#define DOG_CHAR_HEIGHT			12
#define DOG_CHAR_WIDTH_LARGE	9
#define DOG_CHAR_HEIGHT_LARGE	18


#define START_ROW				0


/* Custom characters defined in font font_6x10_marlin.c */
#define LCD_STR_BEDTEMP     "\xFE"
#define LCD_STR_DEGREE      "\xB0"
#define LCD_STR_THERMOMETER "\xFF"
#define LCD_STR_UPLEVEL     "\xFB"
#define LCD_STR_REFRESH     "\xF8"
#define LCD_STR_FOLDER      "\xF9"
#define LCD_STR_FEEDRATE    "\xFD"
#define LCD_STR_CLOCK       "\xFC"
#define LCD_STR_ARROW_RIGHT "\xFA"

// LCD selection
// DE_U8glib
U8GLIB_DOGM128 u8g(29, 30);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

static void lcd_implementation_init()
{
	//  Uncomment this if you have the first generation (V1.10) of STBs board
	pinMode(17, OUTPUT);	// Enable LCD backlight
	digitalWrite(17, HIGH);
	
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_6x10_marlin);
		u8g.setColorIndex(1);
		u8g.drawBox (0, 0, u8g.getWidth(), u8g.getHeight());
		u8g.setColorIndex(1);
	   } while( u8g.nextPage() );
	  
	u8g.setRot180();	// Rotate screen by 180°
	   
	u8g.firstPage();
	do {
			// RepRap init bmp
			u8g.drawBitmapP(0,0,(unsigned int) 60/8+1,64,start_bmp);
			// Welcome message
			u8g.setFont(u8g_font_6x10_marlin);
			u8g.drawStr(62,10,"MARLIN"); 
			u8g.setFont(u8g_font_5x7);
			u8g.drawStr(62,18,"V1.0.0 RC2");
			u8g.setFont(u8g_font_6x10_marlin);
			u8g.drawStr(62,27,"by ErikZalm");
			u8g.drawStr(62,41,"DOGM128 LCD");
			u8g.setFont(u8g_font_5x8);
			u8g.drawStr(62,48,"enhancements");
			u8g.setFont(u8g_font_5x8);
			u8g.drawStr(62,55,"by STB");
			u8g.drawStr(62,61,"uses u8glib");
	   } while( u8g.nextPage() );



	delay(2000);		// wait 2sec. to show the welcome screen


}

static void lcd_implementation_clear()
{
	u8g.setRot180();
	u8g.firstPage();
	do {	
			u8g.setColorIndex(0);
			u8g.drawBox (0, 0, u8g.getWidth(), u8g.getHeight());
			u8g.setColorIndex(1);
		} while( u8g.nextPage() );
}

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
static void lcd_printPGM(const char* str)
{
    char c;
    while((c = pgm_read_byte(str++)) != '\0')
    {
			u8g.print(c);
    }
}


static void lcd_implementation_status_screen()
{
    int tHotend		= int(degHotend(0) + 0.5);
    int tTarget		= int(degTargetHotend(0) + 0.5);

#if EXTRUDERS > 1	
	int tHotend1 	= int(degHotend(1) + 0.5);
    int tTarget1	= int(degTargetHotend(1) + 0.5);
#endif
	
	int tBed		= int(degBed() + 0.5);
    int tBedTarget	= int(degTargetBed() + 0.5);

 
	u8g.setFont(u8g_font_6x10_marlin);
	
	u8g.setPrintPos(0 * DOG_CHAR_WIDTH, 10 + START_ROW ); // 0,0
	u8g.print(LCD_STR_THERMOMETER[0]);
	u8g.print(itostr3(tHotend));
	u8g.print('/');
	u8g.print(itostr3left(tTarget));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

# if EXTRUDERS > 1
	u8g.setPrintPos( 10 * DOG_CHAR_WIDTH, 10 + START_ROW );//(10, 0);
	u8g.print(LCD_STR_THERMOMETER[0]);
	u8g.print(itostr3(tHotend1));
	u8g.print('/');
	u8g.print(itostr3left(tTarget1));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
#endif
   
	u8g.setPrintPos( 0 * DOG_CHAR_WIDTH, 22 + START_ROW ); //(0, 1);
	u8g.print(LCD_STR_BEDTEMP[0]);
	u8g.print(itostr3(tBed));
	u8g.print('/');
	u8g.print(itostr3left(tBedTarget));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

	u8g.setPrintPos(0 * DOG_CHAR_WIDTH, 34 + START_ROW ); //(0,1);
	u8g.print('X');
	u8g.print(ftostr3(current_position[X_AXIS]));
	lcd_printPGM(PSTR(" Y"));
	u8g.print(ftostr3(current_position[Y_AXIS]));

	u8g.setPrintPos((LCD_WIDTH - 7)* DOG_CHAR_WIDTH, 34 + START_ROW );//(LCD_WIDTH - 7, 1);
	u8g.print('Z');
	u8g.print(ftostr31(current_position[Z_AXIS]));

	u8g.setPrintPos(0 * DOG_CHAR_WIDTH, 46 + START_ROW );//(0, 2);
	u8g.print(LCD_STR_FEEDRATE[0]);
	u8g.print(itostr3(feedmultiply));
	u8g.print('%');

#  ifdef SDSUPPORT

		u8g.setPrintPos(6 * DOG_CHAR_WIDTH, 46 + START_ROW );
		lcd_printPGM(PSTR("SD"));
	
    if (IS_SD_PRINTING) {

			u8g.print(itostr3(card.percentDone()));

		}
		
    else {
				lcd_printPGM(PSTR("---"));
		 }

		u8g.print('%');
	
#  endif//SDSUPPORT

	u8g.setPrintPos((LCD_WIDTH - 7) * DOG_CHAR_WIDTH, 46 + START_ROW );
	u8g.print(LCD_STR_CLOCK[0]);
	u8g.print(' ');

	if(starttime != 0)
    {
        uint16_t time = millis()/60000 - starttime/60000;

			u8g.print(itostr2(time/60));
			u8g.print(':');
			u8g.print(itostr2(time%60));
		
    }else{
				lcd_printPGM(PSTR("--:--"));
		 }

		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, 58 - START_ROW );
		u8g.print(lcd_status_message);
}

static void lcd_implementation_drawmenu_generic(uint8_t row, const char* pstr, char pre_char, char post_char)
{
    char c;
    
    uint8_t n = LCD_WIDTH - 1 - 2;
		
		if ((pre_char == '>') || (pre_char == LCD_STR_UPLEVEL[0] ))
		   {
			u8g.setColorIndex(1);		// black on white
			u8g.drawBox (0, row*DOG_CHAR_HEIGHT + 3, 128, DOG_CHAR_HEIGHT);
			u8g.setColorIndex(0);		// following text must be white on black
		   } else u8g.setColorIndex(1); // unmarked text is black on white
		
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);//(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		if (pre_char != '>') u8g.print(pre_char); else u8g.print(' ');	// Row selector is obsolete


    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
		u8g.print(c);
        pstr++;
        n--;
    }
    while(n--){
					u8g.print(' ');
		}
	   
		u8g.print(post_char);
		u8g.print(' ');
		u8g.setColorIndex(1);		// restore settings to black on white
}

static void lcd_implementation_drawmenu_setting_edit_generic(uint8_t row, const char* pstr, char pre_char, char* data)
{
    static unsigned int fkt_cnt = 0;
	char c;
    uint8_t n = LCD_WIDTH - 1 - 2 - strlen(data);
		
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT); //(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) + START_ROW );
		u8g.print(pre_char);
	
    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
			u8g.print(c);
		
        pstr++;
        n--;
    }
	
		u8g.print(':');

    while(n--){
					u8g.print(' ');
			  }

		u8g.print(data);
}

static void lcd_implementation_drawmenu_setting_edit_generic_P(uint8_t row, const char* pstr, char pre_char, const char* data)
{
    char c;
    uint8_t n= LCD_WIDTH - 1 - 2 - strlen_P(data);

		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);//(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		u8g.print(pre_char);
	
    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
			u8g.print(c);
		
        pstr++;
        n--;
    }

		u8g.print(':');
	
    while(n--){
					u8g.print(' ');
			  }

		lcd_printPGM(data);
}

#define lcd_implementation_drawmenu_setting_edit_int3_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_int3(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool_selected(row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_bool(row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

void lcd_implementation_drawedit(const char* pstr, char* value)
{
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH_LARGE, (u8g.getHeight() - 1 - DOG_CHAR_HEIGHT_LARGE) - (1 * DOG_CHAR_HEIGHT_LARGE) - START_ROW );
		u8g.setFont(u8g_font_9x18);
		lcd_printPGM(pstr);
		u8g.print(':');
		u8g.setPrintPos((14 - strlen(value)) * DOG_CHAR_WIDTH_LARGE, (u8g.getHeight() - 1 - DOG_CHAR_HEIGHT_LARGE) - (1 * DOG_CHAR_HEIGHT_LARGE) - START_ROW );
		u8g.print(value);
}

static void lcd_implementation_drawmenu_sdfile_selected(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    char c;
    uint8_t n = LCD_WIDTH - 1;

    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-1] = '\0';
    }

		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);//(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		u8g.print('>');
	   
    while((c = *filename) != '\0')
    {
		u8g.print(c);
        filename++;
        n--;
    }
    while(n--){
					u8g.print(' ');
			   }
}

static void lcd_implementation_drawmenu_sdfile(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    char c;
    uint8_t n = LCD_WIDTH - 1;

    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-1] = '\0';
    }

		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);//(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		u8g.print(' ');
		
while((c = *filename) != '\0')
    {
			u8g.print(c);
		
        filename++;
        n--;
    }
    while(n--){
					u8g.print(' ');
			   }

}

static void lcd_implementation_drawmenu_sddirectory_selected(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    char c;
    uint8_t n = LCD_WIDTH - 2;
		
    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-2] = '\0';
    }
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);//(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		u8g.print('>');
		u8g.print(LCD_STR_FOLDER[0]);
	   
    while((c = *filename) != '\0')
    {
			u8g.print(c);
		
        filename++;
        n--;
    }
    while(n--){
					u8g.print(' ');
			   }
}

static void lcd_implementation_drawmenu_sddirectory(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
    char c;
    uint8_t n = LCD_WIDTH - 2;

    if (longFilename[0] != '\0')
    {
        filename = longFilename;
        longFilename[LCD_WIDTH-2] = '\0';
    }

		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);//(u8g.getHeight() - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		u8g.print(' ');
		u8g.print(LCD_STR_FOLDER[0]);

    while((c = *filename) != '\0')
    {
			u8g.print(c);
		
        filename++;
        n--;
    }
    while(n--){
					u8g.print(' ');
			   }
}

#define lcd_implementation_drawmenu_back_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_back(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_submenu(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode_selected(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_gcode(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')
#define lcd_implementation_drawmenu_function_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')

static void lcd_implementation_quick_feedback()
{

#if BEEPER > -1
    SET_OUTPUT(BEEPER);
    for(int8_t i=0;i<10;i++)
    {
		WRITE(BEEPER,HIGH);
		delay(3);
		WRITE(BEEPER,LOW);
		delay(3);
    }
#endif
}
#endif//ULTRA_LCD_IMPLEMENTATION_DOGM_H
