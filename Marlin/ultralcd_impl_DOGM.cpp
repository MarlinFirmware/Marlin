#include "Marlin.h"
#include "ultralcd.h"
#if ENABLED(DOGLCD)
#include "language.h"
#include "cardreader.h"
#include "temperature.h"
#include "planner.h"
#include "stepper.h"
#include "configuration_store.h"
#include "utility.h"

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "buzzer.h"
#endif

#if ENABLED(PRINTCOUNTER)
  #include "printcounter.h"
  #include "duration_t.h"
#endif

#if ENABLED(BLTOUCH)
  #include "endstops.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl.h"
  bool ubl_lcd_map_control = false;
#endif

  #include "ultralcd_impl_DOGM.h"
  #include <U8glib.h>

#if ENABLED(USE_SMALL_INFOFONT)
  #include "dogm_font_data_6x9_marlin.h"
#endif

#include "dogm_font_data_Marlin_symbols.h"   // The Marlin special symbols

#if DISABLED(SIMULATE_ROMFONT)
  #if ENABLED(DISPLAY_CHARSET_ISO10646_1)
    #include "dogm_font_data_ISO10646_1.h"
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_PL)
    #include "dogm_font_data_ISO10646_1_PL.h"
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_5)
    #include "dogm_font_data_ISO10646_5_Cyrillic.h"
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_KANA)
    #include "dogm_font_data_ISO10646_Kana.h"
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_GREEK)
    #include "dogm_font_data_ISO10646_Greek.h"
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_CN)
    #include "dogm_font_data_ISO10646_CN.h"
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_TR)
    #include "dogm_font_data_ISO10646_1_tr.h"
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
  #endif
#else // SIMULATE_ROMFONT
  #if DISPLAY_CHARSET_HD44780 == JAPANESE
    #include "dogm_font_data_HD44780_J.h"
  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    #include "dogm_font_data_HD44780_W.h"
  #elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    #include "dogm_font_data_HD44780_C.h"
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
  #endif
#endif // SIMULATE_ROMFONT

uint16_t lcd_contrast;
char currentfont = 0;

// The current graphical page being rendered
u8g_page_t &page = ((u8g_pb_t *)((u8g.getU8g())->dev->dev_mem))->p;

void lcd_setFont(const char font_nr) {
  switch (font_nr) {
    case FONT_STATUSMENU : {u8g.setFont(FONT_STATUSMENU_NAME); currentfont = FONT_STATUSMENU;}; break;
    case FONT_MENU       : {u8g.setFont(FONT_MENU_NAME); currentfont = FONT_MENU;}; break;
    case FONT_SPECIAL    : {u8g.setFont(FONT_SPECIAL_NAME); currentfont = FONT_SPECIAL;}; break;
    case FONT_MENU_EDIT  : {u8g.setFont(FONT_MENU_EDIT_NAME); currentfont = FONT_MENU_EDIT;}; break;
    break;
  }
}

void lcd_print(const char c) {
  if (WITHIN(c, 1, LCD_STR_SPECIAL_MAX)) {
    u8g.setFont(FONT_SPECIAL_NAME);
    u8g.print(c);
    lcd_setFont(currentfont);
  }
  else charset_mapper(c);
}

char lcd_print_and_count(const char c) {
  if (WITHIN(c, 1, LCD_STR_SPECIAL_MAX)) {
    u8g.setFont(FONT_SPECIAL_NAME);
    u8g.print(c);
    lcd_setFont(currentfont);
    return 1;
  }
  else return charset_mapper(c);
}

/**
 * Core LCD printing functions
 * On DOGM all strings go through a filter for utf
 * But only use lcd_print_utf and lcd_printPGM_utf for translated text
 */
void lcd_print(const char *str) { while (*str) lcd_print(*str++); }
void lcd_printPGM(const char *str) { while (const char c = pgm_read_byte(str)) lcd_print(c), ++str; }

void lcd_print_utf(const char *str, uint8_t n=LCD_WIDTH) {
  char c;
  while (n && (c = *str)) n -= charset_mapper(c), ++str;
}

void lcd_printPGM_utf(const char *str, uint8_t n=LCD_WIDTH) {
  char c;
  while (n && (c = pgm_read_byte(str))) n -= charset_mapper(c), ++str;
}

// Initialize or re-initialize the LCD
void lcd_implementation_init() {

  #if PIN_EXISTS(LCD_BACKLIGHT) // Enable LCD backlight
    OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
  #endif

  #if PIN_EXISTS(LCD_RESET)
    OUT_WRITE(LCD_RESET_PIN, LOW); // perform a clean hardware reset
    _delay_ms(5);
    OUT_WRITE(LCD_RESET_PIN, HIGH);
    _delay_ms(5); // delay to allow the display to initalize
    u8g.begin(); // re-initialize the display
  #endif

  #if DISABLED(MINIPANEL) // setContrast not working for Mini Panel
    u8g.setContrast(lcd_contrast);
  #endif

  #if ENABLED(LCD_SCREEN_ROT_90)
    u8g.setRot90();   // Rotate screen by 90°
  #elif ENABLED(LCD_SCREEN_ROT_180)
    u8g.setRot180();  // Rotate screen by 180°
  #elif ENABLED(LCD_SCREEN_ROT_270)
    u8g.setRot270();  // Rotate screen by 270°
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool show_bootscreen = true;

    #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
      if (show_bootscreen) {
        u8g.firstPage();
        do {
          u8g.drawBitmapP(
            (128 - (CUSTOM_BOOTSCREEN_BMPWIDTH))  /2,
            ( 64 - (CUSTOM_BOOTSCREEN_BMPHEIGHT)) /2,
            CEILING(CUSTOM_BOOTSCREEN_BMPWIDTH, 8), CUSTOM_BOOTSCREEN_BMPHEIGHT, custom_start_bmp);
        } while (u8g.nextPage());
        safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT);
      }
    #endif // SHOW_CUSTOM_BOOTSCREEN

    const uint8_t offx = (u8g.getWidth() - (START_BMPWIDTH)) / 2;

    #if ENABLED(START_BMPHIGH)
      constexpr uint8_t offy = 0;
    #else
      constexpr uint8_t offy = DOG_CHAR_HEIGHT;
    #endif

    const uint8_t txt1X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE1) - 1) * (DOG_CHAR_WIDTH)) / 2;

    if (show_bootscreen) {
      u8g.firstPage();
      do {
        u8g.drawBitmapP(offx, offy, START_BMPBYTEWIDTH, START_BMPHEIGHT, start_bmp);
        lcd_setFont(FONT_MENU);
        #ifndef STRING_SPLASH_LINE2
          u8g.drawStr(txt1X, u8g.getHeight() - (DOG_CHAR_HEIGHT), STRING_SPLASH_LINE1);
        #else
          const uint8_t txt2X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE2) - 1) * (DOG_CHAR_WIDTH)) / 2;
          u8g.drawStr(txt1X, u8g.getHeight() - (DOG_CHAR_HEIGHT) * 3 / 2, STRING_SPLASH_LINE1);
          u8g.drawStr(txt2X, u8g.getHeight() - (DOG_CHAR_HEIGHT) * 1 / 2, STRING_SPLASH_LINE2);
        #endif
      } while (u8g.nextPage());
    }

    show_bootscreen = false;

  #endif // SHOW_BOOTSCREEN
}


// The kill screen is displayed for unrecoverable conditions
void lcd_kill_screen() {
  lcd_setFont(FONT_MENU);
  u8g.setPrintPos(0, u8g.getHeight()/4*1);
  lcd_print_utf(lcd_status_message);
  u8g.setPrintPos(0, u8g.getHeight()/4*2);
  lcd_printPGM(PSTR(MSG_HALTED));
  u8g.setPrintPos(0, u8g.getHeight()/4*3);
  lcd_printPGM(PSTR(MSG_PLEASE_RESET));
}

void lcd_implementation_clear() { } // Automatically cleared by Picture Loop

void _draw_centered_temp(const int temp, const uint8_t x, const uint8_t y) {
  const uint8_t degsize = 6 * (temp >= 100 ? 3 : temp >= 10 ? 2 : 1); // number's pixel width
  u8g.setPrintPos(x - (18 - degsize) / 2, y); // move left if shorter
  lcd_print(itostr3(temp));
  lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
}

void _draw_heater_status(const uint8_t x, const int8_t heater, const bool blink) {
  #if HAS_TEMP_BED
    bool isBed = heater < 0;
  #else
    const bool isBed = false;
  #endif

  if (PAGE_UNDER(7)) {
    #if HEATER_IDLE_HANDLER
      const bool is_idle = (!isBed ? thermalManager.is_heater_idle(heater) :
      #if HAS_TEMP_BED
        thermalManager.is_bed_idle()
      #else
        false
      #endif
      );

      if (blink || !is_idle)
    #endif
    _draw_centered_temp((isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater)) + 0.5, x, 7); }

  if (PAGE_CONTAINS(21, 28))
    _draw_centered_temp((isBed ? thermalManager.degBed() : thermalManager.degHotend(heater)) + 0.5, x, 28);

  if (PAGE_CONTAINS(17, 20)) {
    const uint8_t h = isBed ? 7 : 8,
                  y = isBed ? 18 : 17;
    if (isBed ? thermalManager.isHeatingBed() : thermalManager.isHeatingHotend(heater)) {
      u8g.setColorIndex(0); // white on black
      u8g.drawBox(x + h, y, 2, 2);
      u8g.setColorIndex(1); // black on white
    }
    else {
      u8g.drawBox(x + h, y, 2, 2);
    }
  }
}

void _draw_axis_label(const AxisEnum axis, const char* const pstr, const bool blink) {
  if (blink)
    lcd_printPGM(pstr);
  else {
    if (!axis_homed[axis])
      u8g.print('?');
    else {
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[axis])
          u8g.print(' ');
        else
      #endif
      lcd_printPGM(pstr);
    }
  }
}

void lcd_implementation_status_message(const bool blink) {
  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    static bool last_blink = false;
    const uint8_t slen = lcd_strlen(lcd_status_message);
    const char *stat = lcd_status_message + status_scroll_pos;
    if (slen <= LCD_WIDTH)
      lcd_print_utf(stat);                                      // The string isn't scrolling
    else {
      if (status_scroll_pos <= slen - LCD_WIDTH)
        lcd_print_utf(stat);                                    // The string fills the screen
      else {
        uint8_t chars = LCD_WIDTH;
        if (status_scroll_pos < slen) {                         // First string still visible
          lcd_print_utf(stat);                                  // The string leaves space
          chars -= slen - status_scroll_pos;                    // Amount of space left
        }
        u8g.print('.');                                         // Always at 1+ spaces left, draw a dot
        if (--chars) {
          if (status_scroll_pos < slen + 1)                     // Draw a second dot if there's space
            --chars, u8g.print('.');
          if (chars) lcd_print_utf(lcd_status_message, chars);  // Print a second copy of the message
        }
      }
      if (last_blink != blink) {
        last_blink = blink;
        // Skip any non-printing bytes
        if (status_scroll_pos < slen) while (!PRINTABLE(lcd_status_message[status_scroll_pos])) status_scroll_pos++;
        if (++status_scroll_pos >= slen + 2) status_scroll_pos = 0;
      }
    }
  #else
    lcd_print_utf(lcd_status_message);
  #endif
}

#endif //ULTRA_LCD
