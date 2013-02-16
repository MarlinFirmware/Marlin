#ifndef ULTRA_LCD_IMPLEMENTATION_DOGM_H
#define ULTRA_LCD_IMPLEMENTATION_DOGM_H

/**
* Implementation of the LCD display routines for a DOGM128 graphic display. These are common LCD 128x64 pixel graphic displays.
**/


// CHANGE_DE begin ***
#include <Dogm.h>
#include "start_bmp.h"

extern const dog_pgm_uint8_t font_6x10_marlin[];	// Font with marlin specific symbols

int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module


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
#define LCD_STR_BEDTEMP     "\x27"
#define LCD_STR_DEGREE      "\x5E"
#define LCD_STR_THERMOMETER "\x26"
#define LCD_STR_UPLEVEL     "\x60"
#define LCD_STR_REFRESH     "\x23"
#define LCD_STR_FOLDER      "\x7D"
#define LCD_STR_FEEDRATE    "\x7C"
#define LCD_STR_CLOCK       "\x7B"
#define LCD_STR_ARROW_RIGHT "\x5C"

Dogm dogm(a0Pin);

static void lcd_implementation_init()
{
	dogm.start();
	do {
		//dogm.setFont(font_6x10_marlin);
		dogm.clrBox (0, 0, DOG_WIDTH, DOG_HEIGHT);
	   } while( dogm.next() );

	dogm.start();
	do {
			// RepRap init bmp
			dogm.setBitmapP(0,63,start_bmp,60,64);
			// Welcome message
			dogm.setFont(font_6x10_marlin);
			dogm.setXY(62,53);
			dogm.print("MARLIN");
			dogm.setFont(font_5x7);
			dogm.setXY(62,46);
			dogm.print("V1.0.0 RC2");
			dogm.setFont(font_6x10_marlin);
			dogm.setXY(62,35);
			dogm.print("by ErikZalm");
			dogm.setXY(62,20);
			dogm.print("DOGM128 LCD");
			dogm.setFont(font_5x8);
			dogm.setXY(62,13);
			dogm.print("enhancements");
			dogm.setFont(font_6x10_marlin);
			dogm.setXY(62, 2);
			dogm.print("by STB");
	   } while( dogm.next() );

//  Uncomment this if you have the first generation (V1.10) of STBs board
//	pinMode(17, OUTPUT);	// Enable LCD backlight
//	digitalWrite(17, HIGH);
	
	dog_Delay(2000);		// wait 2sec. to show the welcome screen
}

static void lcd_implementation_clear()
{
		dogm.clrBox (0, 0, DOG_WIDTH, DOG_HEIGHT);
}
/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
static void lcd_printPGM(const char* str)
{
    char c;
    while((c = pgm_read_byte(str++)) != '\0')
    {
			dogm.write(c);
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

 
	dogm.setFont(font_6x10_marlin);
	
	dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (0 * DOG_CHAR_HEIGHT) - START_ROW ); // 0,0
	dogm.print(LCD_STR_THERMOMETER[0]);
	dogm.print(itostr3(tHotend));
	dogm.print('/');
	dogm.print(itostr3left(tTarget));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

# if EXTRUDERS > 1
	dogm.setXY( 10 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (0 * DOG_CHAR_HEIGHT) - START_ROW );//(10, 0);
	dogm.print(LCD_STR_THERMOMETER[0]);
	dogm.print(itostr3(tHotend1));
	dogm.print('/');
	dogm.print(itostr3left(tTarget1));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
#endif
   
	dogm.setXY( 0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (1 * DOG_CHAR_HEIGHT) - START_ROW ); //(0, 1);
	dogm.print(LCD_STR_BEDTEMP[0]);
	dogm.print(itostr3(tBed));
	dogm.print('/');
	dogm.print(itostr3left(tBedTarget));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

	dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (2 * DOG_CHAR_HEIGHT ) - START_ROW ); //(0,1);
	dogm.print('X');
	dogm.print(ftostr3(current_position[X_AXIS]));
	lcd_printPGM(PSTR(" Y"));
	dogm.print(ftostr3(current_position[Y_AXIS]));

	dogm.setXY((LCD_WIDTH - 7)* DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (2 * DOG_CHAR_HEIGHT) - START_ROW );//(LCD_WIDTH - 7, 1);
	dogm.print('Z');
	dogm.print(ftostr31(current_position[Z_AXIS]));

	dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (3 * DOG_CHAR_HEIGHT) - START_ROW );//(0, 2);
	dogm.print(LCD_STR_FEEDRATE[0]);
	dogm.print(itostr3(feedmultiply));
	dogm.print('%');

#  ifdef SDSUPPORT

		dogm.setXY(6 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (3 * DOG_CHAR_HEIGHT) - START_ROW );
		lcd_printPGM(PSTR("SD"));
	
    if (IS_SD_PRINTING) {

			dogm.print(itostr3(card.percentDone()));

		}
		
    else {
				lcd_printPGM(PSTR("---"));
		 }

		dogm.print('%');
	
#  endif//SDSUPPORT

	dogm.setXY((LCD_WIDTH - 7) * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (3 * DOG_CHAR_HEIGHT) - START_ROW );
	dogm.print(LCD_STR_CLOCK[0]);
	dogm.print(' ');

	if(starttime != 0)
    {
        uint16_t time = millis()/60000 - starttime/60000;

			dogm.print(itostr2(time/60));
			dogm.print(':');
			dogm.print(itostr2(time%60));
		
    }else{
				lcd_printPGM(PSTR("--:--"));
		 }

		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - ((LCD_HEIGHT-1) * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print(lcd_status_message);
}

static void lcd_implementation_drawmenu_generic(uint8_t row, const char* pstr, char pre_char, char post_char)
{
    char c;
    
    uint8_t n = LCD_WIDTH - 1 - 2;
		
		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print(pre_char);


    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
		dogm.print(c);
        pstr++;
        n--;
    }
    while(n--){
					dogm.print(' ');
		}
	   
		dogm.print(post_char);
		dogm.print(' ');
}

static void lcd_implementation_drawmenu_setting_edit_generic(uint8_t row, const char* pstr, char pre_char, char* data)
{
    static unsigned int fkt_cnt = 0;
	char c;
    uint8_t n = LCD_WIDTH - 1 - 2 - strlen(data);
		
		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) + START_ROW );
		dogm.print(pre_char);
	
    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
			dogm.print(c);
		
        pstr++;
        n--;
    }
	
		dogm.print(':');

    while(n--){
					dogm.print(' ');
			  }

		dogm.print(data);
}

static void lcd_implementation_drawmenu_setting_edit_generic_P(uint8_t row, const char* pstr, char pre_char, const char* data)
{
    char c;
    uint8_t n= LCD_WIDTH - 1 - 2 - strlen_P(data);

		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print(pre_char);
	
    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
			dogm.print(c);
		
        pstr++;
        n--;
    }

		dogm.print(':');
	
    while(n--){
					dogm.print(' ');
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
		dogm.setXY(0 * DOG_CHAR_WIDTH_LARGE, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT_LARGE) - (1 * DOG_CHAR_HEIGHT_LARGE) - START_ROW );
		dogm.setFont(font_9x18);
		lcd_printPGM(pstr);
		dogm.print(':');
		dogm.setXY((14 - strlen(value)) * DOG_CHAR_WIDTH_LARGE, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT_LARGE) - (1 * DOG_CHAR_HEIGHT_LARGE) - START_ROW );
		dogm.print(value);
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

		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print('>');
	   
    while((c = *filename) != '\0')
    {
		dogm.print(c);
        filename++;
        n--;
    }
    while(n--){
					dogm.print(' ');
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

		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print(' ');
		
while((c = *filename) != '\0')
    {
			dogm.print(c);
		
        filename++;
        n--;
    }
    while(n--){
					dogm.print(' ');
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

		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print('>');
		dogm.print(LCD_STR_FOLDER[0]);
	   
    while((c = *filename) != '\0')
    {
			dogm.print(c);
		
        filename++;
        n--;
    }
    while(n--){
					dogm.print(' ');
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

		dogm.setXY(0 * DOG_CHAR_WIDTH, (DOG_HEIGHT - 1 - DOG_CHAR_HEIGHT) - (row * DOG_CHAR_HEIGHT) - START_ROW );
		dogm.print(' ');
		dogm.print(LCD_STR_FOLDER[0]);

    while((c = *filename) != '\0')
    {
			dogm.print(c);
		
        filename++;
        n--;
    }
    while(n--){
					dogm.print(' ');
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
