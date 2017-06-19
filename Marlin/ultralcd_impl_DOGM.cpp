/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MarlinConfig.h"

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display.
 * These are common LCD 128x64 pixel graphic displays.
 */

#if ENABLED(DOGLCD)

  #include "ultralcd_impl_DOGM.h"
  #include "utf_mapper.h"

  extern u8g_page_t &page = ((u8g_pb_t *)((u8g.getU8g())->dev->dev_mem))->p;// LCD selection

  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_RS); // 2 stripes
    // U8GLIB_ST7920_128X64 u8g(LCD_PINS_RS); // 8 stripes
  #elif ENABLED(U8GLIB_ST7920)
    //U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS); // Original u8glib device. 2 stripes
                                                                              // No 4 stripe device available from u8glib.
    //U8GLIB_ST7920_128X64_1X u8g(LCD_PINS_D4, LCD_PINS_ENABLE, LCD_PINS_RS);    // Original u8glib device. 8 stripes
    U8GLIB_ST7920_128X64_RRD u8g(0); // Number of stripes can be adjusted in ultralcd_st7920_u8glib_rrd.h with PAGE_HEIGHT
  #elif ENABLED(CARTESIO_UI)
    // The CartesioUI display
    #if DOGLCD_MOSI != -1 && DOGLCD_SCK != -1
      // using SW-SPI
      //U8GLIB_DOGM128 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0);  // 8 stripes
      U8GLIB_DOGM128_2X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0); // 4 stripes
    #else
      //U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
      U8GLIB_DOGM128_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes
    #endif
  #elif ENABLED(U8GLIB_LM6059_AF)
    // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)
    //U8GLIB_LM6059 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    U8GLIB_LM6059_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes
  #elif ENABLED(MAKRPANEL) || ENABLED(VIKI2) || ENABLED(miniVIKI)
    // The MaKrPanel, Mini Viki, and Viki 2.0, ST7565 controller as well
    //U8GLIB_NHD_C12864 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    U8GLIB_NHD_C12864_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes
  #elif ENABLED(U8GLIB_SSD1306)
    // Generic support for SSD1306 OLED I2C LCDs
    //U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);  // 8 stripes
    U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
  #elif ENABLED(U8GLIB_SH1106)
    // Generic support for SH1106 OLED I2C LCDs
    //U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);  // 8 stripes
    U8GLIB_SH1106_128X64_2X u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST); // 4 stripes
  #elif ENABLED(MINIPANEL)
    // The MINIPanel display
    //U8GLIB_MINI12864 u8g(DOGLCD_CS, DOGLCD_A0);  // 8 stripes
    U8GLIB_MINI12864_2X u8g(DOGLCD_CS, DOGLCD_A0); // 4 stripes
  #else
    // for regular DOGM128 display with HW-SPI
    //U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0  // 8 stripes
    U8GLIB_DOGM128_2X u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0 // 4 stripes
  #endif

  uint8_t row_y1, row_y2;
  uint8_t row_height = DOG_CHAR_HEIGHT + 2 * (TALL_FONT_CORRECTION);

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

  extern char lcd_status_message[];

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

  //
  // Status Screen
  //

  FORCE_INLINE void _draw_centered_temp(const int temp, const uint8_t x, const uint8_t y) {
    const uint8_t degsize = 6 * (temp >= 100 ? 3 : temp >= 10 ? 2 : 1); // number's pixel width
    u8g.setPrintPos(x - (18 - degsize) / 2, y); // move left if shorter
    lcd_print(itostr3(temp));
    lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
  }

  FORCE_INLINE void _draw_heater_status(const uint8_t x, const int8_t heater, const bool blink) {
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

  FORCE_INLINE void _draw_axis_label(const AxisEnum axis, const char* const pstr, const bool blink) {
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

  inline void lcd_implementation_status_message() {
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
          lcd.print('.');                                         // Always at 1+ spaces left, draw a dot
          if (--chars) {
            if (status_scroll_pos < slen + 1)                     // Draw a second dot if there's space
              --chars, lcd.print('.');
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

  //#define DOGM_SD_PERCENT

  void lcd_implementation_status_screen() {

    const bool blink = lcd_blink();

    // Status Menu Font
    lcd_setFont(FONT_STATUSMENU);

    //
    // Fan Animation
    //

    if (PAGE_UNDER(STATUS_SCREENHEIGHT + 1)) {

      u8g.drawBitmapP(9, 1, STATUS_SCREENBYTEWIDTH, STATUS_SCREENHEIGHT,
        #if HAS_FAN0
          blink && fanSpeeds[0] ? status_screen0_bmp : status_screen1_bmp
        #else
          status_screen0_bmp
        #endif
      );

    }

    //
    // Temperature Graphics and Info
    //

    if (PAGE_UNDER(28)) {
      // Extruders
      HOTEND_LOOP() _draw_heater_status(5 + e * 25, e, blink);

      // Heated bed
      #if HOTENDS < 4 && HAS_TEMP_BED
        _draw_heater_status(81, -1, blink);
      #endif

      #if HAS_FAN0
        if (PAGE_CONTAINS(20, 27)) {
          // Fan
          const int per = ((fanSpeeds[0] + 1) * 100) / 256;
          if (per) {
            u8g.setPrintPos(104, 27);
            lcd_print(itostr3(per));
            u8g.print('%');
          }
        }
      #endif
    }

    #if ENABLED(SDSUPPORT)

      //
      // SD Card Symbol
      //

      if (PAGE_CONTAINS(42 - (TALL_FONT_CORRECTION), 51 - (TALL_FONT_CORRECTION))) {
        // Upper box
        u8g.drawBox(42, 42 - (TALL_FONT_CORRECTION), 8, 7);     // 42-48 (or 41-47)
        // Right edge
        u8g.drawBox(50, 44 - (TALL_FONT_CORRECTION), 2, 5);     // 44-48 (or 43-47)
        // Bottom hollow box
        u8g.drawFrame(42, 49 - (TALL_FONT_CORRECTION), 10, 4);  // 49-52 (or 48-51)
        // Corner pixel
        u8g.drawPixel(50, 43 - (TALL_FONT_CORRECTION));         // 43 (or 42)
      }

      //
      // Progress bar frame
      //

      #define PROGRESS_BAR_X 54
      #define PROGRESS_BAR_WIDTH (LCD_PIXEL_WIDTH - PROGRESS_BAR_X)

      if (PAGE_CONTAINS(49, 52 - (TALL_FONT_CORRECTION)))       // 49-52 (or 49-51)
        u8g.drawFrame(
          PROGRESS_BAR_X, 49,
          PROGRESS_BAR_WIDTH, 4 - (TALL_FONT_CORRECTION)
        );

      if (IS_SD_PRINTING) {

        //
        // Progress bar solid part
        //

        if (PAGE_CONTAINS(50, 51 - (TALL_FONT_CORRECTION)))     // 50-51 (or just 50)
          u8g.drawBox(
            PROGRESS_BAR_X + 1, 50,
            (unsigned int)((PROGRESS_BAR_WIDTH - 2) * card.percentDone() * 0.01), 2 - (TALL_FONT_CORRECTION)
          );

        //
        // SD Percent Complete
        //

        #if ENABLED(DOGM_SD_PERCENT)
          if (PAGE_CONTAINS(41, 48)) {
            // Percent complete
            u8g.setPrintPos(55, 48);
            u8g.print(itostr3(card.percentDone()));
            u8g.print('%');
          }
        #endif
      }

      //
      // Elapsed Time
      //

      #if DISABLED(DOGM_SD_PERCENT)
        #define SD_DURATION_X (PROGRESS_BAR_X + (PROGRESS_BAR_WIDTH / 2) - len * (DOG_CHAR_WIDTH / 2))
      #else
        #define SD_DURATION_X (LCD_PIXEL_WIDTH - len * DOG_CHAR_WIDTH)
      #endif

      if (PAGE_CONTAINS(41, 48)) {

        char buffer[10];
        duration_t elapsed = print_job_timer.duration();
        bool has_days = (elapsed.value > 60*60*24L);
        uint8_t len = elapsed.toDigital(buffer, has_days);
        u8g.setPrintPos(SD_DURATION_X, 48);
        lcd_print(buffer);
      }

    #endif

    // Before homing the axis letters are blinking 'X' <-> '?'.
    // When axis is homed but axis_known_position is false the axis letters are blinking 'X' <-> ' '.
    // When everything is ok you see a constant 'X'.

    static char xstring[5], ystring[5], zstring[7];
    #if ENABLED(FILAMENT_LCD_DISPLAY) && DISABLED(SDSUPPORT)
      static char wstring[5], mstring[4];
    #endif

    // At the first page, regenerate the XYZ strings
    if (page.page == 0) {
      strcpy(xstring, ftostr4sign(current_position[X_AXIS]));
      strcpy(ystring, ftostr4sign(current_position[Y_AXIS]));
      strcpy(zstring, ftostr52sp(FIXFLOAT(current_position[Z_AXIS])));
      #if ENABLED(FILAMENT_LCD_DISPLAY) && DISABLED(SDSUPPORT)
        strcpy(wstring, ftostr12ns(filament_width_meas));
        strcpy(mstring, itostr3(100.0 * volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]));
      #endif
    }

    if (PAGE_CONTAINS(XYZ_FRAME_TOP, XYZ_FRAME_TOP + XYZ_FRAME_HEIGHT - 1)) {

      #if ENABLED(XYZ_HOLLOW_FRAME)
        u8g.drawFrame(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT); // 8: 29-40  7: 29-39
      #else
        u8g.drawBox(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT);   // 8: 30-39  7: 30-37
      #endif

      if (PAGE_CONTAINS(XYZ_BASELINE - (INFO_FONT_HEIGHT - 1), XYZ_BASELINE)) {

        #if DISABLED(XYZ_HOLLOW_FRAME)
          u8g.setColorIndex(0); // white on black
        #endif

        u8g.setPrintPos(0 * XYZ_SPACING + X_LABEL_POS, XYZ_BASELINE);
        _draw_axis_label(X_AXIS, PSTR(MSG_X), blink);
        u8g.setPrintPos(0 * XYZ_SPACING + X_VALUE_POS, XYZ_BASELINE);
        lcd_print(xstring);

        u8g.setPrintPos(1 * XYZ_SPACING + X_LABEL_POS, XYZ_BASELINE);
        _draw_axis_label(Y_AXIS, PSTR(MSG_Y), blink);
        u8g.setPrintPos(1 * XYZ_SPACING + X_VALUE_POS, XYZ_BASELINE);
        lcd_print(ystring);

        u8g.setPrintPos(2 * XYZ_SPACING + X_LABEL_POS, XYZ_BASELINE);
        _draw_axis_label(Z_AXIS, PSTR(MSG_Z), blink);
        u8g.setPrintPos(2 * XYZ_SPACING + X_VALUE_POS, XYZ_BASELINE);
        lcd_print(zstring);

        #if DISABLED(XYZ_HOLLOW_FRAME)
          u8g.setColorIndex(1); // black on white
        #endif
      }
    }

    //
    // Feedrate
    //

    if (PAGE_CONTAINS(51 - INFO_FONT_HEIGHT, 49)) {
      lcd_setFont(FONT_MENU);
      u8g.setPrintPos(3, 50);
      lcd_print(LCD_STR_FEEDRATE[0]);

      lcd_setFont(FONT_STATUSMENU);
      u8g.setPrintPos(12, 50);
      lcd_print(itostr3(feedrate_percentage));
      u8g.print('%');

      //
      // Filament sensor display if SD is disabled
      //
      #if DISABLED(SDSUPPORT) && ENABLED(FILAMENT_LCD_DISPLAY)
        u8g.setPrintPos(56, 50);
        lcd_print(wstring);
        u8g.setPrintPos(102, 50);
        lcd_print(mstring);
        u8g.print('%');
        lcd_setFont(FONT_MENU);
        u8g.setPrintPos(47, 50);
        lcd_print(LCD_STR_FILAM_DIA);
        u8g.setPrintPos(93, 50);
        lcd_print(LCD_STR_FILAM_MUL);
      #endif
    }

    //
    // Status line
    //

    #define STATUS_BASELINE (55 + INFO_FONT_HEIGHT)

    if (PAGE_CONTAINS(STATUS_BASELINE + 1 - INFO_FONT_HEIGHT, STATUS_BASELINE)) {
      u8g.setPrintPos(0, STATUS_BASELINE);

      #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
        if (PENDING(millis(), previous_lcd_status_ms + 5000UL)) {  //Display both Status message line and Filament display on the last line
          lcd_implementation_status_message();
        }
        else {
          lcd_printPGM(PSTR(LCD_STR_FILAM_DIA));
          u8g.print(':');
          lcd_print(ftostr12ns(filament_width_meas));
          lcd_printPGM(PSTR("  " LCD_STR_FILAM_MUL));
          u8g.print(':');
          lcd_print(itostr3(100.0 * volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]));
          u8g.print('%');
        }
      #else
        lcd_implementation_status_message();
      #endif
    }
  }

  #if ENABLED(ULTIPANEL)

    #if ENABLED(ADVANCED_PAUSE_FEATURE)

      static void lcd_implementation_hotend_status(const uint8_t row) {
        row_y1 = row * row_height + 1;
        row_y2 = row_y1 + row_height - 1;

        if (!PAGE_CONTAINS(row_y1 + 1, row_y2 + 2)) return;

        u8g.setPrintPos(LCD_PIXEL_WIDTH - 11 * (DOG_CHAR_WIDTH), row_y2);
        lcd_print('E');
        lcd_print((char)('1' + active_extruder));
        lcd_print(' ');
        lcd_print(itostr3(thermalManager.degHotend(active_extruder)));
        lcd_print('/');

        if (lcd_blink() || !thermalManager.is_heater_idle(active_extruder))
          lcd_print(itostr3(thermalManager.degTargetHotend(active_extruder)));
      }

    #endif // ADVANCED_PAUSE_FEATURE

    // Set the colors for a menu item based on whether it is selected
    static void lcd_implementation_mark_as_selected(const uint8_t row, const bool isSelected) {
      row_y1 = row * row_height + 1;
      row_y2 = row_y1 + row_height - 1;

      if (!PAGE_CONTAINS(row_y1 + 1, row_y2 + 2)) return;

      if (isSelected) {
        #if ENABLED(MENU_HOLLOW_FRAME)
          u8g.drawHLine(0, row_y1 + 1, LCD_PIXEL_WIDTH);
          u8g.drawHLine(0, row_y2 + 2, LCD_PIXEL_WIDTH);
        #else
          u8g.setColorIndex(1); // black on white
          u8g.drawBox(0, row_y1 + 2, LCD_PIXEL_WIDTH, row_height - 1);
          u8g.setColorIndex(0); // white on black
        #endif
      }
      #if DISABLED(MENU_HOLLOW_FRAME)
        else {
          u8g.setColorIndex(1); // unmarked text is black on white
        }
      #endif
      u8g.setPrintPos((START_COL) * (DOG_CHAR_WIDTH), row_y2);
    }

    // Draw a static line of text in the same idiom as a menu item
    void lcd_implementation_drawmenu_static(const uint8_t row, const char* pstr, const bool center=true, const bool invert=false, const char* valstr=NULL) {

      lcd_implementation_mark_as_selected(row, invert);

      if (!PAGE_CONTAINS(row_y1, row_y2)) return;

      char c;
      int8_t n = LCD_WIDTH - (START_COL);

      if (center && !valstr) {
        int8_t pad = (LCD_WIDTH - lcd_strlen_P(pstr)) / 2;
        while (--pad >= 0) { u8g.print(' '); n--; }
      }
      while (n > 0 && (c = pgm_read_byte(pstr))) {
        n -= lcd_print_and_count(c);
        pstr++;
      }
      if (valstr) while (n > 0 && (c = *valstr)) {
        n -= lcd_print_and_count(c);
        valstr++;
      }
      while (n-- > 0) u8g.print(' ');
    }

    // Draw a generic menu item
    void lcd_implementation_drawmenu_generic(const bool isSelected, const uint8_t row, const char* pstr, const char pre_char, const char post_char) {
      UNUSED(pre_char);

      lcd_implementation_mark_as_selected(row, isSelected);

      if (!PAGE_CONTAINS(row_y1, row_y2)) return;

      uint8_t n = LCD_WIDTH - (START_COL) - 2;
      while (char c = pgm_read_byte(pstr)) {
        n -= lcd_print_and_count(c);
        pstr++;
      }
      while (n--) u8g.print(' ');
      u8g.setPrintPos(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH), row_y2);
      lcd_print(post_char);
      u8g.print(' ');
    }

    // Draw a menu item with an editable value
    void _drawmenu_setting_edit_generic(const bool isSelected, const uint8_t row, const char* pstr, const char* const data, const bool pgm) {

      lcd_implementation_mark_as_selected(row, isSelected);

      if (!PAGE_CONTAINS(row_y1, row_y2)) return;

      const uint8_t vallen = (pgm ? lcd_strlen_P(data) : (lcd_strlen((char*)data)));
      uint8_t n = LCD_WIDTH - (START_COL) - 2 - vallen;

      while (char c = pgm_read_byte(pstr)) {
        n -= lcd_print_and_count(c);
        pstr++;
      }
      u8g.print(':');
      while (n--) u8g.print(' ');
      u8g.setPrintPos(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH) * vallen, row_y2);
      if (pgm)  lcd_printPGM(data);  else  lcd_print((char*)data);
    }

    void lcd_implementation_drawedit(const char* const pstr, const char* const value=NULL) {
      const uint8_t labellen = lcd_strlen_P(pstr),
                    vallen = lcd_strlen(value);

      uint8_t rows = (labellen > LCD_WIDTH - 2 - vallen) ? 2 : 1;

      #if ENABLED(USE_BIG_EDIT_FONT)
        uint8_t lcd_width, char_width;
        if (labellen <= LCD_WIDTH_EDIT - 1) {
          if (labellen >= LCD_WIDTH_EDIT - vallen) rows = 2;
          lcd_width = LCD_WIDTH_EDIT + 1;
          char_width = DOG_CHAR_WIDTH_EDIT;
          lcd_setFont(FONT_MENU_EDIT);
        }
        else {
          lcd_width = LCD_WIDTH - (START_COL);
          char_width = DOG_CHAR_WIDTH;
          lcd_setFont(FONT_MENU);
        }
      #else
        constexpr uint8_t lcd_width = LCD_WIDTH - (START_COL),
                          char_width = DOG_CHAR_WIDTH;
      #endif

      // Center either one or two rows
      const uint8_t segmentHeight = u8g.getHeight() / (rows + 1); // 1 / (rows+1) = 1/2 or 1/3
      uint8_t baseline = segmentHeight + (DOG_CHAR_HEIGHT_EDIT + 1) / 2;

      if (PAGE_CONTAINS(baseline + 1 - (DOG_CHAR_HEIGHT_EDIT), baseline)) {
        u8g.setPrintPos(0, baseline);
        lcd_printPGM(pstr);
      }

      if (value != NULL) {
        baseline += (rows - 1) * segmentHeight;
        if (PAGE_CONTAINS(baseline + 1 - (DOG_CHAR_HEIGHT_EDIT), baseline)) {
          u8g.print(':');
          u8g.setPrintPos((lcd_width - 1 - vallen) * char_width, baseline);
          lcd_print(value);
        }
      }
    }

    #if ENABLED(SDSUPPORT)

      static void _drawmenu_sd(const bool isSelected, const uint8_t row, const char* const pstr, const char* filename, char* const longFilename, const bool isDir) {
        UNUSED(pstr);

        lcd_implementation_mark_as_selected(row, isSelected);

        if (!PAGE_CONTAINS(row_y1, row_y2)) return;

        uint8_t n = LCD_WIDTH - (START_COL) - 1;
        if (longFilename[0]) {
          filename = longFilename;
          longFilename[n] = '\0';
        }

        if (isDir) lcd_print(LCD_STR_FOLDER[0]);

        while (char c = *filename) {
          n -= lcd_print_and_count(c);
          filename++;
        }
        while (n--) u8g.print(' ');
      }

      #define lcd_implementation_drawmenu_sdfile(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, false)
      #define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, true)

    #endif // SDSUPPORT

  #endif // ULTIPANEL

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    /**
     * UBL LCD "radar" map data
     */
    #define MAP_UPPER_LEFT_CORNER_X 35  // These probably should be moved to the .h file  But for now,
    #define MAP_UPPER_LEFT_CORNER_Y 8  // it is easier to play with things having them here
    #define MAP_MAX_PIXELS_X        53
    #define MAP_MAX_PIXELS_Y        49

    void _lcd_ubl_plot_DOGLCD(uint8_t x_plot, uint8_t y_plot) {
      uint8_t i, j, x_offset, y_offset, x_map_pixels, y_map_pixels;
      uint8_t pixels_per_X_mesh_pnt, pixels_per_Y_mesh_pnt, inverted_y;

      /*********************************************************/
      /************ Scale the box pixels appropriately *********/
      /*********************************************************/
      x_map_pixels = ((MAP_MAX_PIXELS_X - 4) / GRID_MAX_POINTS_X) * GRID_MAX_POINTS_X;
      y_map_pixels = ((MAP_MAX_PIXELS_Y - 4) / GRID_MAX_POINTS_Y) * GRID_MAX_POINTS_Y;

      pixels_per_X_mesh_pnt = x_map_pixels / GRID_MAX_POINTS_X;
      pixels_per_Y_mesh_pnt = y_map_pixels / GRID_MAX_POINTS_Y;

      x_offset = MAP_UPPER_LEFT_CORNER_X + 1 + (MAP_MAX_PIXELS_X-x_map_pixels-2)/2;
      y_offset = MAP_UPPER_LEFT_CORNER_Y + 1 + (MAP_MAX_PIXELS_Y-y_map_pixels-2)/2;

      /*********************************************************/
      /************ Clear the Mesh Map Box**********************/
      /*********************************************************/

      u8g.setColorIndex(1);  // First draw the bigger box in White so we have a border around the mesh map box
      u8g.drawBox(x_offset-2, y_offset-2, x_map_pixels+4, y_map_pixels+4);

      u8g.setColorIndex(0);  // Now actually clear the mesh map box
      u8g.drawBox(x_offset, y_offset, x_map_pixels, y_map_pixels);

      /*********************************************************/
      /************ Display Mesh Point Locations ***************/
      /*********************************************************/

      u8g.setColorIndex(1);
      for (i = 0; i < GRID_MAX_POINTS_X; i++) {
        for (j = 0; j < GRID_MAX_POINTS_Y; j++) {
          u8g.drawBox(x_offset+i*pixels_per_X_mesh_pnt+pixels_per_X_mesh_pnt/2,
                      y_offset+j*pixels_per_Y_mesh_pnt+pixels_per_Y_mesh_pnt/2, 1, 1);
        }
      }

      /*********************************************************/
      /************ Fill in the Specified Mesh Point ***********/
      /*********************************************************/

      inverted_y = GRID_MAX_POINTS_Y - y_plot - 1;    // The origin is typically in the lower right corner.  We need to
                                                      // invert the Y to get it to plot in the right location.
      u8g.drawBox(x_offset+x_plot*pixels_per_X_mesh_pnt, y_offset+inverted_y*pixels_per_Y_mesh_pnt,
                    pixels_per_X_mesh_pnt, pixels_per_Y_mesh_pnt);

      /*********************************************************/
      /************** Put Relevent Text on Display *************/
      /*********************************************************/

      // Show X and Y positions at top of screen
      u8g.setColorIndex(1);
      u8g.setPrintPos(5, 7);
      lcd_print("X:");
      lcd_print(ftostr32(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x_plot]))));
      u8g.setPrintPos(74, 7);
      lcd_print("Y:");
      lcd_print(ftostr32(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[y_plot]))));

      // Print plot position
      u8g.setPrintPos(5, 64);
      lcd_print("(");
      u8g.print(x_plot);
      lcd_print(",");
      u8g.print(y_plot);
      lcd_print(")");

      // Show the location value
      u8g.setPrintPos(74, 64);
      lcd_print("Z:");
      if (!isnan(ubl.z_values[x_plot][y_plot])) {
        lcd_print(ftostr43sign(ubl.z_values[x_plot][y_plot]));
      }
      else {
        lcd_print(" -----");
      }
    }

  #endif // AUTO_BED_LEVELING_UBL

#endif // DOGLCD
