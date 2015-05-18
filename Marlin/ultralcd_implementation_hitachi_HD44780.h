#ifndef ULTRALCD_IMPLEMENTATION_HITACHI_HD44780_H
#define ULTRALCD_IMPLEMENTATION_HITACHI_HD44780_H

/**
* Implementation of the LCD display routines for a Hitachi HD44780 display. These are common LCD character displays.
* When selecting the Russian language, a slightly different LCD implementation is used to handle UTF8 characters.
**/

//#ifndef REPRAPWORLD_KEYPAD
//  extern volatile uint8_t buttons;  //the last checked buttons in a bit array.
//#else
  extern volatile uint8_t buttons;  //an extended version of the last checked buttons in a bit array.
//#endif

////////////////////////////////////
// Setup button and encode mappings for each panel (into 'buttons' variable
//
// This is just to map common functions (across different panels) onto the same 
// macro name. The mapping is independent of whether the button is directly connected or 
// via a shift/i2c register.

#ifdef ULTIPANEL
// All UltiPanels might have an encoder - so this is always be mapped onto first two bits
#define BLEN_B 1
#define BLEN_A 0

#define EN_B BIT(BLEN_B) // The two encoder pins are connected through BTN_EN1 and BTN_EN2
#define EN_A BIT(BLEN_A)

#if defined(BTN_ENC) && BTN_ENC > -1
  // encoder click is directly connected
  #define BLEN_C 2 
  #define EN_C BIT(BLEN_C) 
#endif 
  
//
// Setup other button mappings of each panel
//
#if defined(LCD_I2C_VIKI)
  #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)
  
  // button and encoder bit positions within 'buttons'
  #define B_LE (BUTTON_LEFT<<B_I2C_BTN_OFFSET)    // The remaining normalized buttons are all read via I2C
  #define B_UP (BUTTON_UP<<B_I2C_BTN_OFFSET)
  #define B_MI (BUTTON_SELECT<<B_I2C_BTN_OFFSET)
  #define B_DW (BUTTON_DOWN<<B_I2C_BTN_OFFSET)
  #define B_RI (BUTTON_RIGHT<<B_I2C_BTN_OFFSET)

  #if defined(BTN_ENC) && BTN_ENC > -1 
    // the pause/stop/restart button is connected to BTN_ENC when used
    #define B_ST (EN_C)                            // Map the pause/stop/resume button into its normalized functional name 
    #define LCD_CLICKED (buttons&(B_MI|B_RI|B_ST)) // pause/stop button also acts as click until we implement proper pause/stop.
  #else
    #define LCD_CLICKED (buttons&(B_MI|B_RI))
  #endif  

  // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
  #define LCD_HAS_SLOW_BUTTONS

#elif defined(LCD_I2C_PANELOLU2)
  // encoder click can be read through I2C if not directly connected
  #if BTN_ENC <= 0 
    #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)
  
    #define B_MI (PANELOLU2_ENCODER_C<<B_I2C_BTN_OFFSET) // requires LiquidTWI2 library v1.2.3 or later

    #define LCD_CLICKED (buttons&B_MI)

    // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
    #define LCD_HAS_SLOW_BUTTONS
  #else
    #define LCD_CLICKED (buttons&EN_C)  
  #endif

#elif defined(REPRAPWORLD_KEYPAD)
    // define register bit values, don't change it
    #define BLEN_REPRAPWORLD_KEYPAD_F3 0
    #define BLEN_REPRAPWORLD_KEYPAD_F2 1
    #define BLEN_REPRAPWORLD_KEYPAD_F1 2
    #define BLEN_REPRAPWORLD_KEYPAD_UP 6
    #define BLEN_REPRAPWORLD_KEYPAD_RIGHT 4
    #define BLEN_REPRAPWORLD_KEYPAD_MIDDLE 5
    #define BLEN_REPRAPWORLD_KEYPAD_DOWN 3
    #define BLEN_REPRAPWORLD_KEYPAD_LEFT 7
    
    #define REPRAPWORLD_BTN_OFFSET 0 // bit offset into buttons for shift register values

    #define EN_REPRAPWORLD_KEYPAD_F3 BIT((BLEN_REPRAPWORLD_KEYPAD_F3+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_F2 BIT((BLEN_REPRAPWORLD_KEYPAD_F2+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_F1 BIT((BLEN_REPRAPWORLD_KEYPAD_F1+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_UP BIT((BLEN_REPRAPWORLD_KEYPAD_UP+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_RIGHT BIT((BLEN_REPRAPWORLD_KEYPAD_RIGHT+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_MIDDLE BIT((BLEN_REPRAPWORLD_KEYPAD_MIDDLE+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_DOWN BIT((BLEN_REPRAPWORLD_KEYPAD_DOWN+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_LEFT BIT((BLEN_REPRAPWORLD_KEYPAD_LEFT+REPRAPWORLD_BTN_OFFSET))

    //#define LCD_CLICKED ((buttons&EN_C) || (buttons&EN_REPRAPWORLD_KEYPAD_F1))
    //#define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN (buttons&EN_REPRAPWORLD_KEYPAD_DOWN)
    //#define REPRAPWORLD_KEYPAD_MOVE_Y_UP (buttons&EN_REPRAPWORLD_KEYPAD_UP)
    //#define REPRAPWORLD_KEYPAD_MOVE_HOME (buttons&EN_REPRAPWORLD_KEYPAD_MIDDLE)

#elif defined(NEWPANEL)
  #define LCD_CLICKED (buttons&EN_C)
  
#else // old style ULTIPANEL
  //bits in the shift register that carry the buttons for:
  // left up center down right red(stop)
  #define BL_LE 7
  #define BL_UP 6
  #define BL_MI 5
  #define BL_DW 4
  #define BL_RI 3
  #define BL_ST 2

  //automatic, do not change
  #define B_LE BIT(BL_LE)
  #define B_UP BIT(BL_UP)
  #define B_MI BIT(BL_MI)
  #define B_DW BIT(BL_DW)
  #define B_RI BIT(BL_RI)
  #define B_ST BIT(BL_ST)
  
  #define LCD_CLICKED (buttons&(B_MI|B_ST))
#endif

#endif //ULTIPANEL

////////////////////////////////////
// Create LCD class instance and chipset-specific information
#if defined(LCD_I2C_TYPE_PCF8575)
  // note: these are register mapped pins on the PCF8575 controller not Arduino pins
  #define LCD_I2C_PIN_BL  3
  #define LCD_I2C_PIN_EN  2
  #define LCD_I2C_PIN_RW  1
  #define LCD_I2C_PIN_RS  0
  #define LCD_I2C_PIN_D4  4
  #define LCD_I2C_PIN_D5  5
  #define LCD_I2C_PIN_D6  6
  #define LCD_I2C_PIN_D7  7

  #include <Wire.h>
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  #define LCD_CLASS LiquidCrystal_I2C
  LCD_CLASS lcd(LCD_I2C_ADDRESS,LCD_I2C_PIN_EN,LCD_I2C_PIN_RW,LCD_I2C_PIN_RS,LCD_I2C_PIN_D4,LCD_I2C_PIN_D5,LCD_I2C_PIN_D6,LCD_I2C_PIN_D7);
  
#elif defined(LCD_I2C_TYPE_MCP23017)
  //for the LED indicators (which maybe mapped to different things in lcd_implementation_update_indicators())
  #define LED_A 0x04 //100
  #define LED_B 0x02 //010
  #define LED_C 0x01 //001

  #define LCD_HAS_STATUS_INDICATORS

  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2
  #if defined(DETECT_DEVICE)
     LCD_CLASS lcd(LCD_I2C_ADDRESS, 1);
  #else
     LCD_CLASS lcd(LCD_I2C_ADDRESS);
  #endif
  
#elif defined(LCD_I2C_TYPE_MCP23008)
  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2
  #if defined(DETECT_DEVICE)
     LCD_CLASS lcd(LCD_I2C_ADDRESS, 1);
  #else
     LCD_CLASS lcd(LCD_I2C_ADDRESS);
  #endif

#elif defined(LCD_I2C_TYPE_PCA8574)
    #include <LiquidCrystal_I2C.h>
    #define LCD_CLASS LiquidCrystal_I2C
    LCD_CLASS lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
    
// 2 wire Non-latching LCD SR from:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!shiftregister-connection 
#elif defined(SR_LCD_2W_NL)
  extern "C" void __cxa_pure_virtual() { while (1); }
  #include <LCD.h>
  #include <LiquidCrystal_SR.h>
  #define LCD_CLASS LiquidCrystal_SR
  LCD_CLASS lcd(SR_DATA_PIN, SR_CLK_PIN);
#else
  // Standard directly connected LCD implementations
  #include <LiquidCrystal.h>
  #define LCD_CLASS LiquidCrystal
  LCD_CLASS lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7
#endif

#include "utf_mapper.h"

#ifdef LCD_PROGRESS_BAR
  static millis_t progress_bar_ms = 0;
  #if PROGRESS_MSG_EXPIRE > 0
    static millis_t expire_status_ms = 0;
  #endif
  #define LCD_STR_PROGRESS  "\x03\x04\x05"
#endif

static void lcd_set_custom_characters(
  #ifdef LCD_PROGRESS_BAR
    bool progress_bar_set=true
  #endif
) {
  byte bedTemp[8] = {
    B00000,
    B11111,
    B10101,
    B10001,
    B10101,
    B11111,
    B00000,
    B00000
  }; //thanks Sonny Mounicou
  byte degree[8] = {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };
  byte thermometer[8] = {
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
  }; //thanks joris
  byte refresh[8] = {
    B00000,
    B00110,
    B11001,
    B11000,
    B00011,
    B10011,
    B01100,
    B00000,
  }; //thanks joris
  byte folder[8] = {
    B00000,
    B11100,
    B11111,
    B10001,
    B10001,
    B11111,
    B00000,
    B00000
  }; //thanks joris
  byte feedrate[8] = {
    B11100,
    B10000,
    B11000,
    B10111,
    B00101,
    B00110,
    B00101,
    B00000
  }; //thanks Sonny Mounicou
  byte clock[8] = {
    B00000,
    B01110,
    B10011,
    B10101,
    B10001,
    B01110,
    B00000,
    B00000
  }; //thanks Sonny Mounicou

  #ifdef LCD_PROGRESS_BAR
    static bool char_mode = false;
    byte progress[3][8] = { {
      B00000,
      B10000,
      B10000,
      B10000,
      B10000,
      B10000,
      B10000,
      B00000
    }, {
      B00000,
      B10100,
      B10100,
      B10100,
      B10100,
      B10100,
      B10100,
      B00000
    }, {
      B00000,
      B10101,
      B10101,
      B10101,
      B10101,
      B10101,
      B10101,
      B00000
    } };
    if (progress_bar_set != char_mode) {
      char_mode = progress_bar_set;
      lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
      lcd.createChar(LCD_STR_DEGREE[0], degree);
      lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
      lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
      lcd.createChar(LCD_STR_CLOCK[0], clock);
      if (progress_bar_set) {
        // Progress bar characters for info screen
        for (int i=3; i--;) lcd.createChar(LCD_STR_PROGRESS[i], progress[i]);
      }
      else {
        // Custom characters for submenus
        lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
        lcd.createChar(LCD_STR_REFRESH[0], refresh);
        lcd.createChar(LCD_STR_FOLDER[0], folder);
      }
    }
  #else
    lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
    lcd.createChar(LCD_STR_DEGREE[0], degree);
    lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
    lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
    lcd.createChar(LCD_STR_REFRESH[0], refresh);
    lcd.createChar(LCD_STR_FOLDER[0], folder);
    lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
    lcd.createChar(LCD_STR_CLOCK[0], clock);
  #endif
}

static void lcd_implementation_init(
  #ifdef LCD_PROGRESS_BAR
    bool progress_bar_set=true
  #endif
) {

#if defined(LCD_I2C_TYPE_PCF8575)
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  #ifdef LCD_I2C_PIN_BL
    lcd.setBacklightPin(LCD_I2C_PIN_BL,POSITIVE);
    lcd.setBacklight(HIGH);
  #endif
  
#elif defined(LCD_I2C_TYPE_MCP23017)
    lcd.setMCPType(LTI_TYPE_MCP23017);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    lcd.setBacklight(0); //set all the LEDs off to begin with
    
#elif defined(LCD_I2C_TYPE_MCP23008)
    lcd.setMCPType(LTI_TYPE_MCP23008);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);

#elif defined(LCD_I2C_TYPE_PCA8574)
      lcd.init();
      lcd.backlight();
    
#else
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
#endif

    lcd_set_custom_characters(
        #ifdef LCD_PROGRESS_BAR
            progress_bar_set
        #endif
    );

    lcd.clear();
}
static void lcd_implementation_clear()
{
    lcd.clear();
}

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
char lcd_printPGM(const char* str) {
  char c;
  char n = 0;
  while((c = pgm_read_byte(str++))) {
      n += charset_mapper(c);
  }
  return n;
}

char lcd_print(char* str) {
  char c, n = 0;;
  unsigned char i = 0;
  while((c = str[i++])) {
      n += charset_mapper(c);
  }
  return n;
}

unsigned lcd_print(char c) {
    return charset_mapper(c);
}

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
       |X000  Y000   Z000.00|
       |F100%  SD100% T--:--|
       |Status line.........|

20x4   |01234567890123456789|
       |T000/000D B000/000D |
       |T000/000D     Z000.0|
       |F100%  SD100% T--:--|
       |Status line.........|
*/
static void lcd_implementation_status_screen() {
  int tHotend = int(degHotend(0) + 0.5);
  int tTarget = int(degTargetHotend(0) + 0.5);

  #if LCD_WIDTH < 20

    lcd.setCursor(0, 0);
    lcd.print(itostr3(tHotend));
    lcd.print('/');
    lcd.print(itostr3left(tTarget));

    #if EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

      // If we have an 2nd extruder or heated bed, show that in the top right corner
      lcd.setCursor(8, 0);
      #if EXTRUDERS > 1
        tHotend = int(degHotend(1) + 0.5);
        tTarget = int(degTargetHotend(1) + 0.5);
        lcd.print(LCD_STR_THERMOMETER[0]);
      #else // Heated bed
        tHotend = int(degBed() + 0.5);
        tTarget = int(degTargetBed() + 0.5);
        lcd.print(LCD_STR_BEDTEMP[0]);
      #endif
      lcd.print(itostr3(tHotend));
      lcd.print('/');
      lcd.print(itostr3left(tTarget));

    #endif // EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

  #else // LCD_WIDTH > 19

    lcd.setCursor(0, 0);
    lcd.print(LCD_STR_THERMOMETER[0]);
    lcd.print(itostr3(tHotend));
    lcd.print('/');
    lcd.print(itostr3left(tTarget));
    lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
    if (tTarget < 10) lcd.print(' ');

    #if EXTRUDERS > 1 || TEMP_SENSOR_BED != 0
      // If we have an 2nd extruder or heated bed, show that in the top right corner
      lcd.setCursor(10, 0);
      #if EXTRUDERS > 1
        tHotend = int(degHotend(1) + 0.5);
        tTarget = int(degTargetHotend(1) + 0.5);
        lcd.print(LCD_STR_THERMOMETER[0]);
      #else // Heated bed
        tHotend = int(degBed() + 0.5);
        tTarget = int(degTargetBed() + 0.5);
        lcd.print(LCD_STR_BEDTEMP[0]);
      #endif
      lcd.print(itostr3(tHotend));
      lcd.print('/');
      lcd.print(itostr3left(tTarget));
      lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
      if (tTarget < 10) lcd.print(' ');

    #endif  // EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

  #endif // LCD_WIDTH > 19

  #if LCD_HEIGHT > 2
    // Lines 2 for 4 line LCD
    #if LCD_WIDTH < 20
      #ifdef SDSUPPORT
        lcd.setCursor(0, 2);
        lcd_printPGM(PSTR("SD"));
        if (IS_SD_PRINTING)
          lcd.print(itostr3(card.percentDone()));
        else
          lcd_printPGM(PSTR("---"));
          lcd.print('%');
      #endif // SDSUPPORT

    #else // LCD_WIDTH > 19

      #if EXTRUDERS > 1 && TEMP_SENSOR_BED != 0
        // If we both have a 2nd extruder and a heated bed, show the heated bed temp on the 2nd line on the left, as the first line is filled with extruder temps
        tHotend = int(degBed() + 0.5);
        tTarget = int(degTargetBed() + 0.5);

        lcd.setCursor(0, 1);
        lcd.print(LCD_STR_BEDTEMP[0]);
        lcd.print(itostr3(tHotend));
        lcd.print('/');
        lcd.print(itostr3left(tTarget));
        lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
        if (tTarget < 10) lcd.print(' ');
      #else
        lcd.setCursor(0,1);
        lcd.print('X');
        lcd.print(ftostr3(current_position[X_AXIS]));
        lcd_printPGM(PSTR("  Y"));
        lcd.print(ftostr3(current_position[Y_AXIS]));
      #endif // EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

    #endif // LCD_WIDTH > 19

    lcd.setCursor(LCD_WIDTH - 8, 1);
    lcd.print('Z');
    lcd.print(ftostr32sp(current_position[Z_AXIS] + 0.00001));

  #endif // LCD_HEIGHT > 2

  #if LCD_HEIGHT > 3

    lcd.setCursor(0, 2);
    lcd.print(LCD_STR_FEEDRATE[0]);
    lcd.print(itostr3(feedrate_multiplier));
    lcd.print('%');

    #if LCD_WIDTH > 19 && defined(SDSUPPORT)

      lcd.setCursor(7, 2);
      lcd_printPGM(PSTR("SD"));
      if (IS_SD_PRINTING)
        lcd.print(itostr3(card.percentDone()));
      else
        lcd_printPGM(PSTR("---"));
      lcd.print('%');

    #endif // LCD_WIDTH > 19 && SDSUPPORT

    lcd.setCursor(LCD_WIDTH - 6, 2);
    lcd.print(LCD_STR_CLOCK[0]);
    if (print_job_start_ms != 0) {
      uint16_t time = millis()/60000 - print_job_start_ms/60000;
      lcd.print(itostr2(time/60));
      lcd.print(':');
      lcd.print(itostr2(time%60));
    }
    else {
      lcd_printPGM(PSTR("--:--"));
    }

  #endif // LCD_HEIGHT > 3

  /**
   * Display Progress Bar, Filament display, and/or Status Message on the last line
   */

  lcd.setCursor(0, LCD_HEIGHT - 1);

  #ifdef LCD_PROGRESS_BAR

    if (card.isFileOpen()) {
      // Draw the progress bar if the message has shown long enough
      // or if there is no message set.
      if (millis() >= progress_bar_ms + PROGRESS_BAR_MSG_TIME || !lcd_status_message[0]) {
        int tix = (int)(card.percentDone() * LCD_WIDTH * 3) / 100,
          cel = tix / 3, rem = tix % 3, i = LCD_WIDTH;
        char msg[LCD_WIDTH+1], b = ' ';
        msg[i] = '\0';
        while (i--) {
          if (i == cel - 1)
            b = LCD_STR_PROGRESS[2];
          else if (i == cel && rem != 0)
            b = LCD_STR_PROGRESS[rem-1];
          msg[i] = b;
        }
        lcd.print(msg);
        return;
      }
    } //card.isFileOpen

  #elif defined(FILAMENT_LCD_DISPLAY)

    // Show Filament Diameter and Volumetric Multiplier %
    // After allowing lcd_status_message to show for 5 seconds
    if (millis() >= previous_lcd_status_ms + 5000) {
      lcd_printPGM(PSTR("Dia "));
      lcd.print(ftostr12ns(filament_width_meas));
      lcd_printPGM(PSTR(" V"));
      lcd.print(itostr3(100.0*volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]));
  	  lcd.print('%');
  	  return;
    }

  #endif // FILAMENT_LCD_DISPLAY

  lcd_print(lcd_status_message);
}

static void lcd_implementation_drawmenu_generic(bool sel, uint8_t row, const char* pstr, char pre_char, char post_char) {
  char c;
  uint8_t n = LCD_WIDTH - 2;
  lcd.setCursor(0, row);
  lcd.print(sel ? pre_char : ' ');
  while ((c = pgm_read_byte(pstr)) && n > 0) {
    n -= lcd_print(c);
    pstr++;
  }
  while(n--) lcd.print(' ');
  lcd.print(post_char);
}

static void lcd_implementation_drawmenu_setting_edit_generic(bool sel, uint8_t row, const char* pstr, char pre_char, char* data) {
  char c;
  uint8_t n = LCD_WIDTH - 2 - lcd_strlen(data);
  lcd.setCursor(0, row);
  lcd.print(sel ? pre_char : ' ');
  while ((c = pgm_read_byte(pstr)) && n > 0) {
    n -= lcd_print(c);
    pstr++;
  }
  lcd.print(':');
  while (n--) lcd.print(' ');
  lcd_print(data);
}
static void lcd_implementation_drawmenu_setting_edit_generic_P(bool sel, uint8_t row, const char* pstr, char pre_char, const char* data) {
  char c;
  uint8_t n = LCD_WIDTH - 2 - lcd_strlen_P(data);
  lcd.setCursor(0, row);
  lcd.print(sel ? pre_char : ' ');
  while ((c = pgm_read_byte(pstr)) && n > 0) {
    n -= lcd_print(c);
    pstr++;
  }
  lcd.print(':');
  while (n--) lcd.print(' ');
  lcd_printPGM(data);
}

#define lcd_implementation_drawmenu_setting_edit_int3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float43(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

//Add version for callback functions
#define lcd_implementation_drawmenu_setting_edit_callback_int3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float43(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

void lcd_implementation_drawedit(const char* pstr, char* value) {
  lcd.setCursor(1, 1);
  lcd_printPGM(pstr);
  lcd.print(':');
  lcd.setCursor(LCD_WIDTH - lcd_strlen(value), 1);
  lcd_print(value);
}

static void lcd_implementation_drawmenu_sd(bool sel, uint8_t row, const char* pstr, const char* filename, char* longFilename, uint8_t concat, char post_char) {
  char c;
  uint8_t n = LCD_WIDTH - concat;
  lcd.setCursor(0, row);
  lcd.print(sel ? '>' : ' ');
  if (longFilename[0]) {
    filename = longFilename;
    longFilename[n] = '\0';
  }
  while ((c = *filename) && n > 0) {
    n -= lcd_print(c);
    filename++;
  }
  while (n--) lcd.print(' ');
  lcd.print(post_char);
}

static void lcd_implementation_drawmenu_sdfile(bool sel, uint8_t row, const char* pstr, const char* filename, char* longFilename) {
  lcd_implementation_drawmenu_sd(sel, row, pstr, filename, longFilename, 2, ' ');
}

static void lcd_implementation_drawmenu_sddirectory(bool sel, uint8_t row, const char* pstr, const char* filename, char* longFilename) {
  lcd_implementation_drawmenu_sd(sel, row, pstr, filename, longFilename, 2, LCD_STR_FOLDER[0]);
}

#define lcd_implementation_drawmenu_back(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

#ifdef LCD_HAS_STATUS_INDICATORS

  static void lcd_implementation_update_indicators() {
    #if defined(LCD_I2C_PANELOLU2) || defined(LCD_I2C_VIKI)
      //set the LEDS - referred to as backlights by the LiquidTWI2 library 
      static uint8_t ledsprev = 0;
      uint8_t leds = 0;
      if (target_temperature_bed > 0) leds |= LED_A;
      if (target_temperature[0] > 0) leds |= LED_B;
      if (fanSpeed) leds |= LED_C;
      #if EXTRUDERS > 1  
        if (target_temperature[1] > 0) leds |= LED_C;
      #endif
      if (leds != ledsprev) {
        lcd.setBacklight(leds);
        ledsprev = leds;
      }
    #endif
  }

#endif // LCD_HAS_STATUS_INDICATORS

#ifdef LCD_HAS_SLOW_BUTTONS

  extern millis_t next_button_update_ms;

  static uint8_t lcd_implementation_read_slow_buttons() {
    #ifdef LCD_I2C_TYPE_MCP23017
      uint8_t slow_buttons;
      // Reading these buttons this is likely to be too slow to call inside interrupt context
      // so they are called during normal lcd_update
      slow_buttons = lcd.readButtons() << B_I2C_BTN_OFFSET; 
      #ifdef LCD_I2C_VIKI
        if ((slow_buttons & (B_MI|B_RI)) && millis() < next_button_update_ms) // LCD clicked
          slow_buttons &= ~(B_MI|B_RI); // Disable LCD clicked buttons if screen is updated
      #endif
      return slow_buttons;
    #endif
  }

#endif // LCD_HAS_SLOW_BUTTONS

#endif //__ULTRALCD_IMPLEMENTATION_HITACHI_HD44780_H
