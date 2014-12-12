#ifndef ULTRA_LCD_IMPLEMENTATION_HITACHI_HD44780_H
#define ULTRA_LCD_IMPLEMENTATION_HITACHI_HD44780_H

/**
* Implementation of the LCD display routines for a Hitachi HD44780 display. These are common LCD character displays.
* When selecting the Russian language, a slightly different LCD implementation is used to handle UTF8 characters.
**/

////////////////////////////////////
// Setup button and encode mappings for each panel (into 'buttons' variable
//
// This is just to map common functions (across different panels) onto the same 
// macro name. The mapping is independent of whether the button is directly connected or 
// via a shift/i2c register.

#  ifdef WITBOX

#define BLEN_C 2 
#define BLEN_B 1
#define BLEN_A 0

#define EN_C (1<<BLEN_C)
#define EN_B (1<<BLEN_B)
#define EN_A (1<<BLEN_A)

#  endif // WITBOX

////////////////////////
// Setup Rotary Encoder Bit Values (for two pin encoders to indicate movement)
// These values are independent of which pins are used for EN_A and EN_B indications
// The rotary encoder part is also independent to the chipset used for the LCD
#  if ( defined(EN_A) && defined(EN_B) )
#define encrot0 0
#define encrot1 2
#define encrot2 3
#define encrot3 1
#  endif // ( defined(EN_A) && defined(EN_B) )


#    if (LANGUAGE_CHOICE == 6)

#include "LiquidCrystalRus.h"
#define LCD_CLASS LiquidCrystalRus

#    else // (LANGUAGE_CHOICE == 6)

#include <LiquidCrystal.h>
#define LCD_CLASS LiquidCrystal

#    endif  // (LANGUAGE_CHOICE == 6)

LCD_CLASS lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7


/* Custom characters defined in the first 8 characters of the LCD */
#define LCD_STR_BEDTEMP     "\x00"
#define LCD_STR_DEGREE      "\x01"
#define LCD_STR_THERMOMETER "\x02"
#define LCD_STR_UPLEVEL     "\x03"
#define LCD_STR_REFRESH     "\x04"
#define LCD_STR_FOLDER      "\x05"
#define LCD_STR_FEEDRATE    "\x06"
#define LCD_STR_CLOCK       "\x07"
#define LCD_STR_ARROW_RIGHT "\x7E"  /* from the default character set */


// API

static void lcd_implementation_init()
{
  byte bedTemp[8] =
  {
    B00000,
    B11111,
    B10101,
    B10001,
    B10101,
    B11111,
    B00000,
    B00000
  };

  byte degree[8] =
  {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };

  byte thermometer[8] =
  {
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B10001,
    B01110
  };
  byte uplevel[8] = {
    B00100,
    B01110,
    B11111,
    B00100,
    B11100,
    B00000,
    B00000,
    B00000
  };

  byte refresh[8] = {
    B00000,
    B00110,
    B11001,
    B11000,
    B00011,
    B10011,
    B01100,
    B00000,
  };

  byte folder[8] = {
    B00000,
    B11100,
    B11111,
    B10001,
    B10001,
    B11111,
    B00000,
    B00000
  };

  byte feedrate[8] = {
    B11100,
    B10000,
    B11000,
    B10111,
    B00101,
    B00110,
    B00101,
    B00000
  };

  byte clock[8] = {
    B00000,
    B01110,
    B10011,
    B10101,
    B10001,
    B01110,
    B00000,
    B00000
  };

#  ifdef WITBOX
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
#  endif // WITBOX

  lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
  lcd.createChar(LCD_STR_DEGREE[0], degree);
  lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
  lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
  lcd.createChar(LCD_STR_REFRESH[0], refresh);
  lcd.createChar(LCD_STR_FOLDER[0], folder);
  lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
  lcd.createChar(LCD_STR_CLOCK[0], clock);

  lcd.noAutoscroll();

  lcd.clear();
}

static void lcd_implementation_clear()
{
  lcd.clear();
}

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
static void lcd_implementation_print(const char* str)
{
  lcd.print(str);
}

static void lcd_implementation_print(const char str)
{
  lcd.print(str);
}

static void lcd_implementation_print_P(const char* str)
{
  char c;
  while((c = pgm_read_byte(str++)) != '\0') {
    lcd.write(c);
  }
}

static void lcd_implementation_quick_feedback()
{
#  if ( defined(BEEPER) && (BEEPER > 0) )
  SET_OUTPUT(BEEPER);
  beeper_duration = 19;
  beeper_level = false;
#  endif // ( defined(BEEPER) && (BEEPER > 0) )
}

uint8_t lcd_implementation_update_buttons()
{
  uint8_t buttons_vector = 0x00;
  if ( READ(BTN_EN1) == 0 )  buttons_vector |= EN_A;
  if ( READ(BTN_EN2) == 0 )  buttons_vector |= EN_B;
  if ( READ(BTN_ENC) == 0 )  buttons_vector |= EN_C;

  return buttons_vector;
}

void lcd_implementation_set_cursor(uint8_t row, uint8_t column)
{
  lcd.setCursor(column, row);
}


// Graphical API

/*
Possible status screens:
16x2   |0123456789012345|
       |000/000 B000/000|
       |Status line.....|

16x4   |0123456789012345|
       |000/000 B000/000|
       |SD100%    Z000.0|
       |F100%     T--:--|
       |Status line.....|

20x2   |01234567890123456789|
       |T000/000D B000/000D |
       |Status line.........|

20x4   |01234567890123456789|
       |T000/000D B000/000D |
       |X+000.0 Y+000.0 Z+000.0|
       |F100%  SD100% T--:--|
       |Status line.........|

20x4   |01234567890123456789|
       |T000/000D B000/000D |
       |T000/000D     Z000.0|
       |F100%  SD100% T--:--|
       |Status line.........|
*/
static void lcd_implementation_status_screen()
{
  int tHotend=int(degHotend(0) + 0.5);
  int tTarget=int(degTargetHotend(0) + 0.5);

#  if LCD_WIDTH < 20
  lcd.setCursor(0, 0);
  lcd.print(itostr3(tHotend));
  lcd.print('/');
  lcd.print(itostr3left(tTarget));

#    if ((EXTRUDERS > 1) || (TEMP_SENSOR_BED != 0))
  //If we have an 2nd extruder or heated bed, show that in the top right corner
  lcd.setCursor(8, 0);

#      if (EXTRUDERS > 1)
  tHotend = int(degHotend(1) + 0.5);
  tTarget = int(degTargetHotend(1) + 0.5);
  lcd.print(LCD_STR_THERMOMETER[0]);
#      else // (EXTRUDERS > 1)
  tHotend=int(degBed() + 0.5);
  tTarget=int(degTargetBed() + 0.5);
  lcd.print(LCD_STR_BEDTEMP[0]);
#      endif // EXTRUDERS > 1
  
  lcd.print(itostr3(tHotend));
  lcd.print('/');
  lcd.print(itostr3left(tTarget));
#    endif // ((EXTRUDERS > 1) || (TEMP_SENSOR_BED != 0))

#  else // LCD_WIDTH < 20
  lcd.setCursor(0, 0);
  lcd.print(LCD_STR_THERMOMETER[0]);
  lcd.print(itostr3(tHotend));
  lcd.print('/');
  lcd.print(itostr3left(tTarget));
  lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
  if (tTarget < 10)
      lcd.print(' ');

#    if ((EXTRUDERS > 1) || (TEMP_SENSOR_BED != 0)) //If we have an 2nd extruder or heated bed, show that in the top right corner
  lcd.setCursor(10, 0);

#      if (EXTRUDERS > 1)
  tHotend = int(degHotend(1) + 0.5);
  tTarget = int(degTargetHotend(1) + 0.5);
  lcd.print(LCD_STR_THERMOMETER[0]);
#      else // (EXTRUDERS > 1)
  tHotend=int(degBed() + 0.5);
  tTarget=int(degTargetBed() + 0.5);
  lcd.print(LCD_STR_BEDTEMP[0]);
#      endif // (EXTRUDERS > 1)

  lcd.print(itostr3(tHotend));
  lcd.print('/');
  lcd.print(itostr3left(tTarget));
  lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
  if (tTarget < 10)
      lcd.print(' ');
#    endif // ((EXTRUDERS > 1) || (TEMP_SENSOR_BED != 0))
#  endif // LCD_WIDTH < 20

#  if LCD_HEIGHT > 2 // Lines 2 for 4 line LCD
#    if LCD_WIDTH < 20
#      ifdef SDSUPPORT
  lcd.setCursor(0, 2);
  lcd_implementation_print_P(PSTR("SD"));
  if (IS_SD_PRINTING)
    lcd.print(itostr3(card.percentDone()));
  else
    lcd_implementation_print_P(PSTR("---"));
  lcd.print('%');
#      endif // SDSUPPORT
#    else//LCD_WIDTH > 19

#      if (EXTRUDERS > 1 && TEMP_SENSOR_BED != 0)
  //If we both have a 2nd extruder and a heated bed, show the heated bed temp on the 2nd line on the left, as the first line is filled with extruder temps
  tHotend=int(degBed() + 0.5);
  tTarget=int(degTargetBed() + 0.5);

  lcd.setCursor(0, 1);
  lcd.print(LCD_STR_BEDTEMP[0]);
  lcd.print(itostr3(tHotend));
  lcd.print('/');
  lcd.print(itostr3left(tTarget));
  lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
  if (tTarget < 10)
    lcd.print(' ');
#      else // (EXTRUDERS > 1 && TEMP_SENSOR_BED != 0)
  lcd.setCursor(0,1);
  lcd.print('X');
  lcd.print(ftostr3(current_position[X_AXIS]));
  lcd_implementation_print_P(PSTR(" Y"));
  lcd.print(ftostr3(current_position[Y_AXIS]));
#      endif // (EXTRUDERS > 1 && TEMP_SENSOR_BED != 0)
#    endif // LCD_WIDTH < 20
  lcd.setCursor(LCD_WIDTH - 8, 1);
  lcd.print('Z');
  lcd.print(ftostr32(current_position[Z_AXIS]));
#  endif // LCD_HEIGHT > 2

#  if LCD_HEIGHT > 3
  lcd.setCursor(0, 2);
  lcd.print(LCD_STR_FEEDRATE[0]);
  lcd.print(itostr3(feedmultiply));
  lcd.print('%');

#    if LCD_WIDTH > 19
#      ifdef SDSUPPORT
  lcd.setCursor(7, 2);
  lcd_implementation_print_P(PSTR("SD"));
  if (IS_SD_PRINTING)
    lcd.print(itostr3(card.percentDone()));
  else
    lcd_implementation_print_P(PSTR("---"));
  lcd.print('%');
#      endif // SDSUPPORT
#    endif // LCD_WIDTH > 19
  lcd.setCursor(LCD_WIDTH - 6, 2);
  lcd.print(LCD_STR_CLOCK[0]);
  if (starttime != 0) {
    uint16_t time = millis()/60000 - starttime/60000;
    lcd.print(itostr2(time/60));
    lcd.print(':');
    lcd.print(itostr2(time%60));
  } else {
    lcd_implementation_print_P(PSTR("--:--"));
  }
#  endif // LCD_HEIGHT > 3

  //Status message line on the last line
  lcd.setCursor(0, LCD_HEIGHT - 1);
  lcd.print(lcd_status_message);
}

static void lcd_implementation_drawmenu_generic(uint8_t row, const char* pstr, char pre_char, char post_char)
{
  char c;
//Use all characters in narrow LCDs
#  if LCD_WIDTH < 20
  uint8_t n = LCD_WIDTH - 1 - 1;
#  else // LCD_WITH < 20
  uint8_t n = LCD_WIDTH - 1 - 2;
#  endif // LCD_WIDTH < 20
  lcd.setCursor(0, row);
  lcd.print(pre_char);
  while (((c = pgm_read_byte(pstr)) != '\0') && (n>0)) {
    lcd.print(c);
    pstr++;
    n--;
  }

  while(n--)
    lcd.print(' ');
  lcd.print(post_char);
  lcd.print(' ');
}

static void lcd_implementation_drawmenu_setting_edit_generic(uint8_t row, const char* pstr, char pre_char, char* data)
{
  char c;
  //Use all characters in narrow LCDs
#  if LCD_WIDTH < 20
  uint8_t n = LCD_WIDTH - 1 - 1 - strlen(data);
#  else // LCD_WIDTH < 20
  uint8_t n = LCD_WIDTH - 1 - 2 - strlen(data);
#  endif // LCD_WIDTH < 20
  lcd.setCursor(0, row);
  lcd.print(pre_char);
  while (((c = pgm_read_byte(pstr)) != '\0') && (n>0)) {
    lcd.print(c);
    pstr++;
    n--;
  }
  lcd.print(':');
  while(n--)
    lcd.print(' ');
  lcd.print(data);
}

static void lcd_implementation_drawmenu_setting_edit_generic_P(uint8_t row, const char* pstr, char pre_char, const char* data)
{
  char c;
  //Use all characters in narrow LCDs
#  if LCD_WIDTH < 20
  uint8_t n = LCD_WIDTH - 1 - 1 - strlen_P(data);
#  else // LCD_WIDTH < 20
  uint8_t n = LCD_WIDTH - 1 - 2 - strlen_P(data);
#  endif // LCD_WIDTH < 20
  lcd.setCursor(0, row);
  lcd.print(pre_char);
  while (((c = pgm_read_byte(pstr)) != '\0') && (n>0)) {
    lcd.print(c);
    pstr++;
    n--;
  }
  lcd.print(':');

  while(n--)
    lcd.print(' ');
  lcd_implementation_print_P(data);
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
  lcd.setCursor(1, 1);
  lcd_implementation_print_P(pstr);
  lcd.print(':');
#  if LCD_WIDTH < 20
  lcd.setCursor(LCD_WIDTH - strlen(value), 1);
#  else // LCD_WIDTH < 20
  lcd.setCursor(LCD_WIDTH -1 - strlen(value), 1);
#  endif // LCD_WIDTH < 20
  lcd.print(value);
}

static void lcd_implementation_drawmenu_sdfile_selected(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
  char c;
  uint8_t n = LCD_WIDTH - 1;
  lcd.setCursor(0, row);
  lcd.print('>');
  if (longFilename[0] != '\0') {
      filename = longFilename;
      longFilename[LCD_WIDTH-1] = '\0';
  }
  while (((c = *filename) != '\0') && (n>0)) {
    lcd.print(c);
    filename++;
    n--;
  }
  while(n--)
    lcd.print(' ');
}

static void lcd_implementation_drawmenu_sdfile(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
  char c;
  uint8_t n = LCD_WIDTH - 1;
  lcd.setCursor(0, row);
  lcd.print(' ');
  if (longFilename[0] != '\0') {
      filename = longFilename;
      longFilename[LCD_WIDTH-1] = '\0';
  }
  while (((c = *filename) != '\0') && (n>0)) {
    lcd.print(c);
    filename++;
    n--;
  }
  while(n--)
    lcd.print(' ');
}

static void lcd_implementation_drawmenu_sddirectory_selected(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
  char c;
  uint8_t n = LCD_WIDTH - 2;
  lcd.setCursor(0, row);
  lcd.print('>');
  lcd.print(LCD_STR_FOLDER[0]);
   
  if (longFilename[0] != '\0') {
    filename = longFilename;
    longFilename[LCD_WIDTH-2] = '\0';
  }

  while(((c = *filename) != '\0') && (n>0)) {
    lcd.print(c);
    filename++;
    n--;
  }
  while(n--)
    lcd.print(' ');
}

static void lcd_implementation_drawmenu_sddirectory(uint8_t row, const char* pstr, const char* filename, char* longFilename)
{
  char c;
  uint8_t n = LCD_WIDTH - 2;
  lcd.setCursor(0, row);
  lcd.print(' ');
  lcd.print(LCD_STR_FOLDER[0]);
  if (longFilename[0] != '\0') {
    filename = longFilename;
    longFilename[LCD_WIDTH-2] = '\0';
  }

  while(((c = *filename) != '\0') && (n>0)) {
    lcd.print(c);
    filename++;
    n--;
  }
  while(n--)
    lcd.print(' ');
}

#define lcd_implementation_drawmenu_back_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_back(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_submenu(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode_selected(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_gcode(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')
#define lcd_implementation_drawmenu_function_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')
#define lcd_implementation_drawmenu_wizard_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_wizard(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')

void lcd_implementation_drawline(uint8_t row, const char* pstr)
{
  char c;
  uint8_t n = LCD_WIDTH - 1;
  lcd.setCursor(0, row);

  while ( ((c = *pstr) != '\0') && (n > 0) ) {
    lcd.print(c);
    n--;
  }

  while (n-- != 0)
    lcd.print(' ');
}

#endif // ULTRA_LCD_IMPLEMENTATION_HITACHI_HD44780_H
