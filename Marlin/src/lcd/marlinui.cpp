/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfig.h"

#include "../MarlinCore.h" // for printingIsPaused

#if LED_POWEROFF_TIMEOUT > 0 || BOTH(HAS_WIRED_LCD, PRINTER_EVENT_LEDS)
  #include "../feature/leds/leds.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../feature/host_actions.h"
#endif

#if BOTH(BROWSE_MEDIA_ON_INSERT, PASSWORD_ON_SD_PRINT_MENU)
  #include "../feature/password/password.h"
#endif

// All displays share the MarlinUI class
#include "marlinui.h"
MarlinUI ui;

#if HAS_DISPLAY
  #include "../gcode/queue.h"
  #include "fontutils.h"
#endif

#if ENABLED(DWIN_CREALITY_LCD)
  #include "e3v2/creality/dwin.h"
#elif ENABLED(DWIN_LCD_PROUI)
  #include "e3v2/proui/dwin.h"
#elif ENABLED(DWIN_CREALITY_LCD_JYERSUI)
  #include "e3v2/jyersui/dwin.h"
#endif

#if ENABLED(LCD_PROGRESS_BAR) && !IS_TFTGLCD_PANEL
  #define BASIC_PROGRESS_BAR 1
#endif

#if ANY(HAS_DISPLAY, HAS_STATUS_MESSAGE, BASIC_PROGRESS_BAR)
  #include "../module/printcounter.h"
#endif

#if LCD_HAS_WAIT_FOR_MOVE
  bool MarlinUI::wait_for_move; // = false
#endif

constexpr uint8_t epps = ENCODER_PULSES_PER_STEP;

#if HAS_STATUS_MESSAGE
  #if ENABLED(STATUS_MESSAGE_SCROLLING) && EITHER(HAS_WIRED_LCD, DWIN_LCD_PROUI)
    uint8_t MarlinUI::status_scroll_offset; // = 0
  #endif
  char MarlinUI::status_message[MAX_MESSAGE_LENGTH + 1];
  uint8_t MarlinUI::alert_level; // = 0
  #if HAS_STATUS_MESSAGE_TIMEOUT
    millis_t MarlinUI::status_message_expire_ms; // = 0
  #endif
  statusResetFunc_t MarlinUI::status_reset_callback; // = nullptr
#endif

#if ENABLED(LCD_SET_PROGRESS_MANUALLY)
  MarlinUI::progress_t MarlinUI::progress_override; // = 0
  #if ENABLED(USE_M73_REMAINING_TIME)
    uint32_t MarlinUI::remaining_time;
  #endif
#endif

#if HAS_MULTI_LANGUAGE
  uint8_t MarlinUI::language; // Initialized by settings.load()
  void MarlinUI::set_language(const uint8_t lang) {
    if (lang < NUM_LANGUAGES) {
      language = lang;
      TERN_(HAS_MARLINUI_U8GLIB, update_language_font());
      return_to_status();
      refresh();
    }
  }
#endif

#if HAS_LCD_CONTRAST
  uint8_t MarlinUI::contrast = LCD_CONTRAST_DEFAULT; // Initialized by settings.load()
  void MarlinUI::set_contrast(const uint8_t value) {
    contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
    _set_contrast();
  }
#endif

#if HAS_LCD_BRIGHTNESS
  uint8_t MarlinUI::brightness = LCD_BRIGHTNESS_DEFAULT;
  bool MarlinUI::backlight = true;

  void MarlinUI::set_brightness(const uint8_t value) {
    backlight = !!value;
    if (backlight) brightness = constrain(value, LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX);
    _set_brightness();
  }
#endif

#if ENABLED(SOUND_MENU_ITEM)
  bool MarlinUI::sound_on = ENABLED(SOUND_ON_DEFAULT);
#endif

#if EITHER(PCA9632_BUZZER, HAS_BEEPER)
  #if ENABLED(PCA9632_BUZZER)
    #include "../feature/leds/pca9632.h"
  #endif
  void MarlinUI::buzz(const long duration, const uint16_t freq) {
    if (!sound_on) return;
    #if ENABLED(PCA9632_BUZZER)
      PCA9632_buzz(duration, freq);
    #elif HAS_BEEPER
      buzzer.tone(duration, freq);
    #endif
  }
#endif

#if HAS_PREHEAT
  #include "../module/temperature.h"

  preheat_t MarlinUI::material_preset[PREHEAT_COUNT];  // Initialized by settings.load()

  PGM_P MarlinUI::get_preheat_label(const uint8_t m) {
    #define _PDEF(N) static PGMSTR(preheat_##N##_label, PREHEAT_##N##_LABEL);
    #define _PLBL(N) preheat_##N##_label,
    REPEAT_1(PREHEAT_COUNT, _PDEF);
    static PGM_P const preheat_labels[PREHEAT_COUNT] PROGMEM = { REPEAT_1(PREHEAT_COUNT, _PLBL) };
    return (PGM_P)pgm_read_ptr(&preheat_labels[m]);
  }

  void MarlinUI::apply_preheat(const uint8_t m, const uint8_t pmask, const uint8_t e/*=active_extruder*/) {
    const preheat_t &pre = material_preset[m];
    TERN_(HAS_HOTEND,           if (TEST(pmask, PT_HOTEND))  thermalManager.setTargetHotend(pre.hotend_temp, e));
    TERN_(HAS_HEATED_BED,       if (TEST(pmask, PT_BED))     thermalManager.setTargetBed(pre.bed_temp));
    //TERN_(HAS_HEATED_CHAMBER, if (TEST(pmask, PT_CHAMBER)) thermalManager.setTargetBed(pre.chamber_temp));
    TERN_(HAS_FAN,              if (TEST(pmask, PT_FAN))     thermalManager.set_fan_speed(0, pre.fan_speed));
  }
#endif

#if EITHER(HAS_MARLINUI_MENU, EXTENSIBLE_UI)
  bool MarlinUI::lcd_clicked;
#endif

#if EITHER(HAS_WIRED_LCD, DWIN_CREALITY_LCD_JYERSUI)

  bool MarlinUI::get_blink() {
    static uint8_t blink = 0;
    static millis_t next_blink_ms = 0;
    millis_t ms = millis();
    if (ELAPSED(ms, next_blink_ms)) {
      blink ^= 0xFF;
      next_blink_ms = ms + 1000 - (LCD_UPDATE_INTERVAL) / 2;
    }
    return blink != 0;
  }

#endif

// Encoder Handling
#if HAS_ENCODER_ACTION
  uint32_t MarlinUI::encoderPosition;
  volatile int8_t encoderDiff; // Updated in update_buttons, added to encoderPosition every LCD update
#endif

#if LCD_BACKLIGHT_TIMEOUT

  uint16_t MarlinUI::lcd_backlight_timeout; // Initialized by settings.load()
  millis_t MarlinUI::backlight_off_ms = 0;
  void MarlinUI::refresh_backlight_timeout() {
    backlight_off_ms = lcd_backlight_timeout ? millis() + lcd_backlight_timeout * 1000UL : 0;
    WRITE(LCD_BACKLIGHT_PIN, HIGH);
  }

#elif HAS_DISPLAY_SLEEP

  uint8_t MarlinUI::sleep_timeout_minutes; // Initialized by settings.load()
  millis_t MarlinUI::screen_timeout_millis = 0;
  void MarlinUI::refresh_screen_timeout() {
    screen_timeout_millis = sleep_timeout_minutes ? millis() + sleep_timeout_minutes * 60UL * 1000UL : 0;
    sleep_off();
  }

#endif

void MarlinUI::init() {

  init_lcd();

  #if HAS_DIGITAL_BUTTONS
    #if BUTTON_EXISTS(EN1)
      SET_INPUT_PULLUP(BTN_EN1);
    #endif
    #if BUTTON_EXISTS(EN2)
      SET_INPUT_PULLUP(BTN_EN2);
    #endif
    #if BUTTON_EXISTS(ENC)
      SET_INPUT_PULLUP(BTN_ENC);
    #endif
    #if BUTTON_EXISTS(ENC_EN)
      SET_INPUT_PULLUP(BTN_ENC_EN);
    #endif
    #if BUTTON_EXISTS(BACK)
      SET_INPUT_PULLUP(BTN_BACK);
    #endif
    #if BUTTON_EXISTS(UP)
      SET_INPUT(BTN_UP);
    #endif
    #if BUTTON_EXISTS(DWN)
      SET_INPUT(BTN_DWN);
    #endif
    #if BUTTON_EXISTS(LFT)
      SET_INPUT(BTN_LFT);
    #endif
    #if BUTTON_EXISTS(RT)
      SET_INPUT(BTN_RT);
    #endif
  #endif

  #if HAS_SHIFT_ENCODER

    #if ENABLED(SR_LCD_2W_NL) // Non latching 2 wire shift register

      SET_OUTPUT(SR_DATA_PIN);
      SET_OUTPUT(SR_CLK_PIN);

    #elif PIN_EXISTS(SHIFT_CLK)

      SET_OUTPUT(SHIFT_CLK_PIN);
      OUT_WRITE(SHIFT_LD_PIN, HIGH);
      #if PIN_EXISTS(SHIFT_EN)
        OUT_WRITE(SHIFT_EN_PIN, LOW);
      #endif
      SET_INPUT_PULLUP(SHIFT_OUT_PIN);

    #endif

  #endif // HAS_SHIFT_ENCODER

  #if BOTH(HAS_ENCODER_ACTION, HAS_SLOW_BUTTONS)
    slow_buttons = 0;
  #endif

  update_buttons();

  TERN_(HAS_ENCODER_ACTION, encoderDiff = 0);

  reset_status(); // Set welcome message
}

#if HAS_WIRED_LCD

  #if HAS_MARLINUI_U8GLIB
    #include "dogm/marlinui_DOGM.h"
  #endif

  #include "lcdprint.h"

  #include "../module/temperature.h"
  #include "../module/planner.h"
  #include "../module/motion.h"

  #if HAS_MARLINUI_MENU
    #include "../module/settings.h"
  #endif

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    #include "../feature/bedlevel/bedlevel.h"
  #endif

  #if HAS_TRINAMIC_CONFIG
    #include "../feature/tmc_util.h"
  #endif

  #if HAS_ADC_BUTTONS
    #include "../module/thermistor/thermistors.h"
  #endif

  #if HAS_POWER_MONITOR
    #include "../feature/power_monitor.h"
  #endif

  #if LED_POWEROFF_TIMEOUT > 0
    #include "../feature/power.h"
  #endif

  #if HAS_ENCODER_ACTION
    volatile uint8_t MarlinUI::buttons;
    #if HAS_SLOW_BUTTONS
      volatile uint8_t MarlinUI::slow_buttons;
    #endif
    #if HAS_TOUCH_BUTTONS
      #include "touch/touch_buttons.h"
      bool MarlinUI::on_edit_screen = false;
    #endif
  #endif

  #if SCREENS_CAN_TIME_OUT
    bool MarlinUI::defer_return_to_status;
    millis_t MarlinUI::return_to_status_ms = 0;
  #endif

  uint8_t MarlinUI::lcd_status_update_delay = 1; // First update one loop delayed

  #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
    millis_t MarlinUI::next_filament_display; // = 0
  #endif

  millis_t MarlinUI::next_button_update_ms; // = 0

  #if HAS_MARLINUI_U8GLIB
    bool MarlinUI::drawing_screen, MarlinUI::first_page; // = false
  #endif

  #if IS_DWIN_MARLINUI
    bool MarlinUI::did_first_redraw;
    bool MarlinUI::old_is_printing;
  #endif

  #if ENABLED(SDSUPPORT)

    #if MARLINUI_SCROLL_NAME
      uint8_t MarlinUI::filename_scroll_pos, MarlinUI::filename_scroll_max;
    #endif

    const char * MarlinUI::scrolled_filename(CardReader &theCard, const uint8_t maxlen, uint8_t hash, const bool doScroll) {
      const char *outstr = theCard.longest_filename();
      if (theCard.longFilename[0]) {
        #if MARLINUI_SCROLL_NAME
          if (doScroll) {
            for (uint8_t l = FILENAME_LENGTH; l--;)
              hash = ((hash << 1) | (hash >> 7)) ^ theCard.filename[l];      // rotate, xor
            static uint8_t filename_scroll_hash;
            if (filename_scroll_hash != hash) {                              // If the hash changed...
              filename_scroll_hash = hash;                                   // Save the new hash
              filename_scroll_max = _MAX(0, utf8_strlen(theCard.longFilename) - maxlen); // Update the scroll limit
              filename_scroll_pos = 0;                                       // Reset scroll to the start
              lcd_status_update_delay = 8;                                   // Don't scroll right away
            }
            // Advance byte position corresponding to filename_scroll_pos char position
            outstr += TERN(UTF_FILENAME_SUPPORT, utf8_byte_pos_by_char_num(outstr, filename_scroll_pos), filename_scroll_pos);
          }
        #else
          theCard.longFilename[
            TERN(UTF_FILENAME_SUPPORT, utf8_byte_pos_by_char_num(theCard.longFilename, maxlen), maxlen)
          ] = '\0'; // cutoff at screen edge
        #endif
      }
      return outstr;
    }

  #endif

  #if HAS_MARLINUI_MENU
    #include "menu/menu.h"

    screenFunc_t MarlinUI::currentScreen; // Initialized in CTOR
    bool MarlinUI::screen_changed;

    #if ENABLED(ENCODER_RATE_MULTIPLIER)
      bool MarlinUI::encoderRateMultiplierEnabled;
      millis_t MarlinUI::lastEncoderMovementMillis = 0;
      void MarlinUI::enable_encoder_multiplier(const bool onoff) {
        encoderRateMultiplierEnabled = onoff;
        lastEncoderMovementMillis = 0;
      }
    #endif

    #if EITHER(REVERSE_MENU_DIRECTION, REVERSE_SELECT_DIRECTION)
      int8_t MarlinUI::encoderDirection = ENCODERBASE;
    #endif

    #if HAS_TOUCH_BUTTONS
      uint8_t MarlinUI::touch_buttons;
      uint8_t MarlinUI::repeat_delay;
    #endif

    #if EITHER(AUTO_BED_LEVELING_UBL, G26_MESH_VALIDATION)

      bool MarlinUI::external_control; // = false

      void MarlinUI::wait_for_release() {
        while (button_pressed()) safe_delay(50);
        safe_delay(50);
      }

    #endif

    #if !HAS_GRAPHICAL_TFT

      void _wrap_string(uint8_t &col, uint8_t &row, const char * const string, read_byte_cb_t cb_read_byte, bool wordwrap/*=false*/) {
        SETCURSOR(col, row);
        if (!string) return;

        auto _newline = [&col, &row]{
          col = 0; row++;                 // Move col to string len (plus space)
          SETCURSOR(0, row);              // Simulate carriage return
        };

        uint8_t *p = (uint8_t*)string;
        wchar_t ch;
        if (wordwrap) {
          uint8_t *wrd = nullptr, c = 0;
          // find the end of the part
          for (;;) {
            if (!wrd) wrd = p;            // Get word start /before/ advancing
            p = get_utf8_value_cb(p, cb_read_byte, &ch);
            const bool eol = !ch;         // zero ends the string
            // End or a break between phrases?
            if (eol || ch == ' ' || ch == '-' || ch == '+' || ch == '.') {
              if (!c && ch == ' ') { if (wrd) wrd++; continue; } // collapse extra spaces
              // Past the right and the word is not too long?
              if (col + c > LCD_WIDTH && col >= (LCD_WIDTH) / 4) _newline(); // should it wrap?
              c += !eol;                  // +1 so the space will be printed
              col += c;                   // advance col to new position
              while (c) {                 // character countdown
                --c;                      // count down to zero
                wrd = get_utf8_value_cb(wrd, cb_read_byte, &ch); // get characters again
                lcd_put_wchar(ch);        // character to the LCD
              }
              if (eol) break;             // all done!
              wrd = nullptr;              // set up for next word
            }
            else c++;                     // count word characters
          }
        }
        else {
          for (;;) {
            p = get_utf8_value_cb(p, cb_read_byte, &ch);
            if (!ch) break;
            lcd_put_wchar(ch);
            col++;
            if (col >= LCD_WIDTH) _newline();
          }
        }
      }

      void MarlinUI::draw_select_screen_prompt(PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
        const uint8_t plen = utf8_strlen_P(pref), slen = suff ? utf8_strlen_P(suff) : 0;
        uint8_t col = 0, row = 0;
        if (!string && plen + slen <= LCD_WIDTH) {
          col = (LCD_WIDTH - plen - slen) / 2;
          row = LCD_HEIGHT > 3 ? 1 : 0;
        }
        if (LCD_HEIGHT >= 8) row = LCD_HEIGHT / 2 - 2;
        wrap_string_P(col, row, pref, true);
        if (string) {
          if (col) { col = 0; row++; } // Move to the start of the next line
          wrap_string(col, row, string);
        }
        if (suff) wrap_string_P(col, row, suff);
      }

    #endif // !HAS_GRAPHICAL_TFT

  #endif // HAS_MARLINUI_MENU

  ////////////////////////////////////////////
  ///////////// Keypad Handling //////////////
  ////////////////////////////////////////////

  #if IS_RRW_KEYPAD && HAS_ENCODER_ACTION

    volatile uint8_t MarlinUI::keypad_buttons;

    #if HAS_MARLINUI_MENU && !HAS_ADC_BUTTONS

      void lcd_move_x();
      void lcd_move_y();
      void lcd_move_z();

      void _reprapworld_keypad_move(const AxisEnum axis, const int16_t dir) {
        ui.manual_move.menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
        ui.encoderPosition = dir;
        switch (axis) {
          case X_AXIS: lcd_move_x(); break;
          case Y_AXIS: lcd_move_y(); break;
          case Z_AXIS: lcd_move_z();
          default: break;
        }
      }

    #endif

    bool MarlinUI::handle_keypad() {

      #if HAS_ADC_BUTTONS

        #define ADC_MIN_KEY_DELAY 100
        if (keypad_buttons) {
          #if HAS_ENCODER_ACTION
            refresh(LCDVIEW_REDRAW_NOW);
            #if HAS_MARLINUI_MENU
              if (encoderDirection == -(ENCODERBASE)) { // HAS_ADC_BUTTONS forces REVERSE_MENU_DIRECTION, so this indicates menu navigation
                     if (RRK(EN_KEYPAD_UP))     encoderPosition += ENCODER_STEPS_PER_MENU_ITEM;
                else if (RRK(EN_KEYPAD_DOWN))   encoderPosition -= ENCODER_STEPS_PER_MENU_ITEM;
                else if (RRK(EN_KEYPAD_LEFT))   { MenuItem_back::action(); quick_feedback(); }
                else if (RRK(EN_KEYPAD_RIGHT))  { return_to_status(); quick_feedback(); }
              }
              else
            #endif
            {
              #if HAS_MARLINUI_MENU
                     if (RRK(EN_KEYPAD_UP))     encoderPosition -= epps;
                else if (RRK(EN_KEYPAD_DOWN))   encoderPosition += epps;
                else if (RRK(EN_KEYPAD_LEFT))   { MenuItem_back::action(); quick_feedback(); }
                else if (RRK(EN_KEYPAD_RIGHT))  encoderPosition = 0;
              #else
                     if (RRK(EN_KEYPAD_UP)   || RRK(EN_KEYPAD_LEFT))  encoderPosition -= epps;
                else if (RRK(EN_KEYPAD_DOWN) || RRK(EN_KEYPAD_RIGHT)) encoderPosition += epps;
              #endif
            }
          #endif
          next_button_update_ms = millis() + ADC_MIN_KEY_DELAY;
          return true;
        }

      #else // !HAS_ADC_BUTTONS

        static uint8_t keypad_debounce = 0;

        if (!RRK( EN_KEYPAD_F1    | EN_KEYPAD_F2
                | EN_KEYPAD_F3    | EN_KEYPAD_DOWN
                | EN_KEYPAD_RIGHT | EN_KEYPAD_MIDDLE
                | EN_KEYPAD_UP    | EN_KEYPAD_LEFT )
        ) {
          if (keypad_debounce > 0) keypad_debounce--;
        }
        else if (!keypad_debounce) {
          keypad_debounce = 2;

          const bool homed = all_axes_homed();

          #if HAS_MARLINUI_MENU

            if (RRK(EN_KEYPAD_MIDDLE))  goto_screen(menu_move);

            #if NONE(DELTA, Z_HOME_TO_MAX)
              if (RRK(EN_KEYPAD_F2))    _reprapworld_keypad_move(Z_AXIS,  1);
            #endif

            if (homed) {
              #if EITHER(DELTA, Z_HOME_TO_MAX)
                if (RRK(EN_KEYPAD_F2))  _reprapworld_keypad_move(Z_AXIS,  1);
              #endif
              if (RRK(EN_KEYPAD_F3))    _reprapworld_keypad_move(Z_AXIS, -1);
              if (RRK(EN_KEYPAD_LEFT))  _reprapworld_keypad_move(X_AXIS, -1);
              if (RRK(EN_KEYPAD_RIGHT)) _reprapworld_keypad_move(X_AXIS,  1);
              if (RRK(EN_KEYPAD_DOWN))  _reprapworld_keypad_move(Y_AXIS,  1);
              if (RRK(EN_KEYPAD_UP))    _reprapworld_keypad_move(Y_AXIS, -1);
            }

          #endif // HAS_MARLINUI_MENU

          if (!homed && RRK(EN_KEYPAD_F1)) queue.inject_P(G28_STR);
          return true;
        }

      #endif // !HAS_ADC_BUTTONS

      return false;
    }

  #endif // IS_RRW_KEYPAD && HAS_ENCODER_ACTION

  /**
   * Status Screen
   *
   * This is very display-dependent, so the lcd implementation draws this.
   */

  #if BASIC_PROGRESS_BAR
    millis_t MarlinUI::progress_bar_ms; // = 0
    #if PROGRESS_MSG_EXPIRE > 0
      millis_t MarlinUI::expire_status_ms; // = 0
    #endif
  #endif

  void MarlinUI::status_screen() {

    TERN_(HAS_MARLINUI_MENU, ENCODER_RATE_MULTIPLY(false));

    #if BASIC_PROGRESS_BAR

      //
      // HD44780 implements the following message blinking and
      // message expiration because Status Line and Progress Bar
      // share the same line on the display.
      //

      #if DISABLED(PROGRESS_MSG_ONCE) || PROGRESS_MSG_EXPIRE > 0
        #define GOT_MS
        const millis_t ms = millis();
      #endif

      // If the message will blink rather than expire...
      #if DISABLED(PROGRESS_MSG_ONCE)
        if (ELAPSED(ms, progress_bar_ms + PROGRESS_BAR_MSG_TIME + PROGRESS_BAR_BAR_TIME))
          progress_bar_ms = ms;
      #endif

      #if PROGRESS_MSG_EXPIRE > 0

        // Handle message expire
        if (expire_status_ms) {

          // Expire the message if a job is active and the bar has ticks
          if (get_progress_percent() > 2 && !print_job_timer.isPaused()) {
            if (ELAPSED(ms, expire_status_ms)) {
              status_message[0] = '\0';
              expire_status_ms = 0;
            }
          }
          else {
            // Defer message expiration before bar appears
            // and during any pause (not just SD)
            expire_status_ms += LCD_UPDATE_INTERVAL;
          }
        }

      #endif // PROGRESS_MSG_EXPIRE

    #endif // BASIC_PROGRESS_BAR

    bool did_expire = status_reset_callback && (*status_reset_callback)();

    #if HAS_STATUS_MESSAGE_TIMEOUT
      #ifndef GOT_MS
        #define GOT_MS
        const millis_t ms = millis();
      #endif
      did_expire |= status_message_expire_ms && ELAPSED(ms, status_message_expire_ms);
    #endif

    if (did_expire) reset_status();

    #if HAS_MARLINUI_MENU
      if (use_click()) {
        #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
          next_filament_display = millis() + 5000UL;  // Show status message for 5s
        #endif
        goto_screen(menu_main);
        reinit_lcd(); // Revive a noisy shared SPI LCD
        return;
      }

    #endif

    #if ENABLED(ULTIPANEL_FEEDMULTIPLY)

      const int16_t old_frm = feedrate_percentage;
            int16_t new_frm = old_frm + int16_t(encoderPosition);

      // Dead zone at 100% feedrate
      if (old_frm == 100) {
        if (int16_t(encoderPosition) > ENCODER_FEEDRATE_DEADZONE)
          new_frm -= ENCODER_FEEDRATE_DEADZONE;
        else if (int16_t(encoderPosition) < -(ENCODER_FEEDRATE_DEADZONE))
          new_frm += ENCODER_FEEDRATE_DEADZONE;
        else
          new_frm = old_frm;
      }
      else if ((old_frm < 100 && new_frm > 100) || (old_frm > 100 && new_frm < 100))
        new_frm = 100;

      LIMIT(new_frm, 10, 999);

      if (old_frm != new_frm) {
        feedrate_percentage = new_frm;
        encoderPosition = 0;
        #if BOTH(HAS_BUZZER, BEEP_ON_FEEDRATE_CHANGE)
          static millis_t next_beep;
          #ifndef GOT_MS
            const millis_t ms = millis();
          #endif
          if (ELAPSED(ms, next_beep)) {
            BUZZ(FEEDRATE_CHANGE_BEEP_DURATION, FEEDRATE_CHANGE_BEEP_FREQUENCY);
            next_beep = ms + 500UL;
          }
        #endif
      }

    #endif // ULTIPANEL_FEEDMULTIPLY

    draw_status_screen();
  }

  void MarlinUI::kill_screen(FSTR_P const lcd_error, FSTR_P const lcd_component) {
    init();
    status_printf(1, F(S_FMT ": " S_FMT), FTOP(lcd_error), FTOP(lcd_component));
    TERN_(HAS_MARLINUI_MENU, return_to_status());

    // RED ALERT. RED ALERT.
    #if ENABLED(PRINTER_EVENT_LEDS)
      leds.set_color(LEDColorRed());
      #ifdef NEOPIXEL_BKGD_INDEX_FIRST
        neo.set_background_color(255, 0, 0, 0);
        neo.show();
      #endif
    #endif

    draw_kill_screen();
  }

  #if HAS_TOUCH_SLEEP
    #if HAS_TOUCH_BUTTONS
      #include "touch/touch_buttons.h"
    #else
      #include "tft/touch.h"
    #endif
    // Wake up a sleeping TFT
    void MarlinUI::wakeup_screen() {
      TERN(HAS_TOUCH_BUTTONS, touchBt.wakeUp(), touch.wakeUp());
    }
  #endif

  void MarlinUI::quick_feedback(const bool clear_buttons/*=true*/) {
    TERN_(HAS_TOUCH_SLEEP, wakeup_screen()); // Wake up the TFT with most buttons
    TERN_(HAS_MARLINUI_MENU, refresh());

    #if HAS_ENCODER_ACTION
      if (clear_buttons)
        TERN_(HAS_ADC_BUTTONS, keypad_buttons =) buttons = 0;
      next_button_update_ms = millis() + 500;
    #else
      UNUSED(clear_buttons);
    #endif

    #if HAS_CHIRP
      chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
      #if BOTH(HAS_MARLINUI_MENU, HAS_BEEPER)
        for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
      #elif HAS_MARLINUI_MENU
        delay(10);
      #endif
    #endif
  }

  ////////////////////////////////////////////
  /////////////// Manual Move ////////////////
  ////////////////////////////////////////////

  #if HAS_MARLINUI_MENU

    ManualMove MarlinUI::manual_move{};

    millis_t ManualMove::start_time = 0;
    float ManualMove::menu_scale = 1;
    #if IS_KINEMATIC
      float ManualMove::offset = 0;
      xyze_pos_t ManualMove::all_axes_destination = { 0 };
      bool ManualMove::processing = false;
    #endif
    #if MULTI_E_MANUAL
      int8_t ManualMove::e_index = 0;
    #endif
    AxisEnum ManualMove::axis = NO_AXIS_ENUM;

    /**
     * If a manual move has been posted and its time has arrived, and if the planner
     * has a space for it, then add a linear move to current_position the planner.
     *
     * If any manual move needs to be interrupted, make sure to force a manual move
     * by setting manual_move.start_time to millis() after updating current_position.
     *
     * To post a manual move:
     *   - Update current_position to the new place you want to go.
     *   - Set manual_move.axis to an axis like X_AXIS. Use ALL_AXES_ENUM for diagonal moves.
     *   - Set manual_move.start_time to a point in the future (in ms) when the move should be done.
     *
     * For kinematic machines:
     *   - Set manual_move.offset to modify one axis and post the move.
     *     This is used to achieve more rapid stepping on kinematic machines.
     *
     * Currently used by the _lcd_move_xyz function in menu_motion.cpp
     * and the ubl_map_move_to_xy function in menu_ubl.cpp.
     */
    void ManualMove::task() {

      if (processing) return;   // Prevent re-entry from idle() calls

      // Add a manual move to the queue?
      if (axis != NO_AXIS_ENUM && ELAPSED(millis(), start_time) && !planner.is_full()) {

        const feedRate_t fr_mm_s = (axis < LOGICAL_AXES) ? manual_feedrate_mm_s[axis] : XY_PROBE_FEEDRATE_MM_S;

        #if IS_KINEMATIC

          #if HAS_MULTI_EXTRUDER
            REMEMBER(ae, active_extruder);
            #if MULTI_E_MANUAL
              if (axis == E_AXIS) active_extruder = e_index;
            #endif
          #endif

          // Apply a linear offset to a single axis
          if (axis == ALL_AXES_ENUM)
            destination = all_axes_destination;
          else if (axis <= XYZE) {
            destination = current_position;
            destination[axis] += offset;
          }

          // Reset for the next move
          offset = 0;
          axis = NO_AXIS_ENUM;

          // DELTA and SCARA machines use segmented moves, which could fill the planner during the call to
          // move_to_destination. This will cause idle() to be called, which can then call this function while the
          // previous invocation is being blocked. Modifications to offset shouldn't be made while
          // processing is true or the planner will get out of sync.
          processing = true;
          prepare_internal_move_to_destination(fr_mm_s);  // will set current_position from destination
          processing = false;

        #else

          // For Cartesian / Core motion simply move to the current_position
          planner.buffer_line(current_position, fr_mm_s,
            TERN_(MULTI_E_MANUAL, axis == E_AXIS ? e_index :) active_extruder
          );

          //SERIAL_ECHOLNPGM("Add planner.move with Axis ", AS_CHAR(AXIS_CHAR(axis)), " at FR ", fr_mm_s);

          axis = NO_AXIS_ENUM;

        #endif
      }
    }

    //
    // Tell ui.update() to start a move to current_position after a short delay.
    //
    void ManualMove::soon(const AxisEnum move_axis
      OPTARG(MULTI_E_MANUAL, const int8_t eindex/*=active_extruder*/)
    ) {
      TERN_(MULTI_E_MANUAL, if (move_axis == E_AXIS) e_index = eindex);
      start_time = millis() + (menu_scale < 0.99f ? 0UL : 250UL); // delay for bigger moves
      axis = move_axis;
      //SERIAL_ECHOLNPGM("Post Move with Axis ", AS_CHAR(AXIS_CHAR(axis)), " soon.");
    }

    #if ENABLED(AUTO_BED_LEVELING_UBL)

      void MarlinUI::external_encoder() {
        if (external_control && encoderDiff) {
          ubl.encoder_diff += encoderDiff;  // Encoder for UBL G29 mesh editing
          encoderDiff = 0;                  // Hide encoder events from the screen handler
          refresh(LCDVIEW_REDRAW_NOW);      // ...but keep the refresh.
        }
      }

    #endif

  #endif // HAS_MARLINUI_MENU

  /**
   * Update the LCD, read encoder buttons, etc.
   *   - Read button states
   *   - Check the SD Card slot state
   *   - Act on RepRap World keypad input
   *   - Update the encoder position
   *   - Apply acceleration to the encoder position
   *   - Do refresh(LCDVIEW_CALL_REDRAW_NOW) on controller events
   *   - Reset the Info Screen timeout if there's any input
   *   - Update status indicators, if any
   *
   *   Run the current LCD menu handler callback function:
   *   - Call the handler only if lcdDrawUpdate != LCDVIEW_NONE
   *   - Before calling the handler, LCDVIEW_CALL_NO_REDRAW => LCDVIEW_NONE
   *   - Call the menu handler. Menu handlers should do the following:
   *     - If a value changes, set lcdDrawUpdate to LCDVIEW_REDRAW_NOW and draw the value
   *       (Encoder events automatically set lcdDrawUpdate for you.)
   *     - if (should_draw()) { redraw }
   *     - Before exiting the handler set lcdDrawUpdate to:
   *       - LCDVIEW_CLEAR_CALL_REDRAW to clear screen and set LCDVIEW_CALL_REDRAW_NEXT.
   *       - LCDVIEW_REDRAW_NOW to draw now (including remaining stripes).
   *       - LCDVIEW_CALL_REDRAW_NEXT to draw now and get LCDVIEW_REDRAW_NOW on the next loop.
   *       - LCDVIEW_CALL_NO_REDRAW to draw now and get LCDVIEW_NONE on the next loop.
   *     - NOTE: For graphical displays menu handlers may be called 2 or more times per loop,
   *             so don't change lcdDrawUpdate without considering this.
   *
   *   After the menu handler callback runs (or not):
   *   - Clear the LCD if lcdDrawUpdate == LCDVIEW_CLEAR_CALL_REDRAW
   *   - Update lcdDrawUpdate for the next loop (i.e., move one state down, usually)
   *
   * This function is only called from the main thread.
   */

  LCDViewAction MarlinUI::lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;
  millis_t next_lcd_update_ms;

  inline bool can_encode() {
    return !BUTTON_PRESSED(ENC_EN); // Update encoder only when ENC_EN is not LOW (pressed)
  }

  void MarlinUI::update() {

    static uint16_t max_display_update_time = 0;
    millis_t ms = millis();

    #if LED_POWEROFF_TIMEOUT > 0
      leds.update_timeout(powerManager.psu_on);
    #endif

    #if HAS_MARLINUI_MENU

      // Handle any queued Move Axis motion
      manual_move.task();

      // Update button states for button_pressed(), etc.
      // If the state changes the next update may be delayed 300-500ms.
      update_buttons();

      // If the action button is pressed...
      static bool wait_for_unclick; // = false

      auto do_click = [&]{
        wait_for_unclick = true;                        //  - Set debounce flag to ignore continuous clicks
        lcd_clicked = !wait_for_user;                   //  - Keep the click if not waiting for a user-click
        wait_for_user = false;                          //  - Any click clears wait for user
        quick_feedback();                               //  - Always make a click sound
      };

      #if HAS_TOUCH_BUTTONS
        if (touch_buttons) {
          reset_status_timeout(ms);
          if (touch_buttons & (EN_A | EN_B)) {              // Menu arrows, in priority
            if (ELAPSED(ms, next_button_update_ms)) {
              encoderDiff = (ENCODER_STEPS_PER_MENU_ITEM) * epps * encoderDirection;
              if (touch_buttons & EN_A) encoderDiff *= -1;
              TERN_(AUTO_BED_LEVELING_UBL, external_encoder());
              next_button_update_ms = ms + repeat_delay;    // Assume the repeat delay
              if (!wait_for_unclick) {
                next_button_update_ms += 250;               // Longer delay on first press
                wait_for_unclick = true;                    // Avoid Back/Select click while repeating
                chirp();
              }
            }
          }
          else if (!wait_for_unclick && (buttons & EN_C))   // OK button, if not waiting for a debounce release:
            do_click();
        }
        // keep wait_for_unclick value
      #endif

      if (!touch_buttons) {
        // Integrated LCD click handling via button_pressed
        if (!external_control && button_pressed()) {
          if (!wait_for_unclick) do_click();              // Handle the click
        }
        else
          wait_for_unclick = false;
      }

      if (LCD_BACK_CLICKED()) {
        quick_feedback();
        goto_previous_screen();
      }

    #endif // HAS_MARLINUI_MENU

    if (ELAPSED(ms, next_lcd_update_ms) || TERN0(HAS_MARLINUI_U8GLIB, drawing_screen)) {

      next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;

      #if HAS_TOUCH_BUTTONS

        if (on_status_screen()) next_lcd_update_ms += (LCD_UPDATE_INTERVAL) * 2;

        TERN_(HAS_ENCODER_ACTION, touch_buttons = touchBt.read_buttons());

      #endif

      TERN_(LCD_HAS_STATUS_INDICATORS, update_indicators());

      #if HAS_ENCODER_ACTION

        TERN_(HAS_SLOW_BUTTONS, slow_buttons = read_slow_buttons()); // Buttons that take too long to read in interrupt context

        if (TERN0(IS_RRW_KEYPAD, handle_keypad()))
          reset_status_timeout(ms);

        uint8_t abs_diff = ABS(encoderDiff);

        #if ENCODER_PULSES_PER_STEP > 1
          // When reversing the encoder direction, a movement step can be missed because
          // encoderDiff has a non-zero residual value, making the controller unresponsive.
          // The fix clears the residual value when the encoder is idle.
          // Also check if past half the threshold to compensate for missed single steps.
          static int8_t lastEncoderDiff;

          // Timeout? No decoder change since last check. 10 or 20 times per second.
          if (encoderDiff == lastEncoderDiff && abs_diff <= epps / 2)   // Same direction & size but not over a half-step?
            encoderDiff = 0;                                            // Clear residual pulses.
          else if (WITHIN(abs_diff, epps / 2 + 1, epps - 1)) {          // Past half of threshold?
            abs_diff = epps;                                            // Treat as a full step size
            encoderDiff = (encoderDiff < 0 ? -1 : 1) * abs_diff;        // ...in the spin direction.
          }
          TERN_(HAS_TOUCH_SLEEP, if (lastEncoderDiff != encoderDiff) wakeup_screen());
          lastEncoderDiff = encoderDiff;
        #endif

        const bool encoderPastThreshold = (abs_diff >= epps);
        if (encoderPastThreshold || lcd_clicked) {
          if (encoderPastThreshold && TERN1(IS_TFTGLCD_PANEL, !external_control)) {

            #if BOTH(HAS_MARLINUI_MENU, ENCODER_RATE_MULTIPLIER)

              int32_t encoderMultiplier = 1;

              if (encoderRateMultiplierEnabled) {
                const float encoderMovementSteps = float(abs_diff) / epps;

                if (lastEncoderMovementMillis) {
                  // Note that the rate is always calculated between two passes through the
                  // loop and that the abs of the encoderDiff value is tracked.
                  const float encoderStepRate = encoderMovementSteps / float(ms - lastEncoderMovementMillis) * 1000;

                  if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC)     encoderMultiplier = 100;
                  else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC) encoderMultiplier = 10;

                  // Enable to output the encoder steps per second value
                  //#define ENCODER_RATE_MULTIPLIER_DEBUG
                  #if ENABLED(ENCODER_RATE_MULTIPLIER_DEBUG)
                    SERIAL_ECHO_START();
                    SERIAL_ECHOPGM("Enc Step Rate: ", encoderStepRate);
                    SERIAL_ECHOPGM("  Multiplier: ", encoderMultiplier);
                    SERIAL_ECHOPGM("  ENCODER_10X_STEPS_PER_SEC: ", ENCODER_10X_STEPS_PER_SEC);
                    SERIAL_ECHOPGM("  ENCODER_100X_STEPS_PER_SEC: ", ENCODER_100X_STEPS_PER_SEC);
                    SERIAL_EOL();
                  #endif
                }

                lastEncoderMovementMillis = ms;
              } // encoderRateMultiplierEnabled

            #else

              constexpr int32_t encoderMultiplier = 1;

            #endif // ENCODER_RATE_MULTIPLIER

            if (can_encode()) encoderPosition += (encoderDiff * encoderMultiplier) / epps;

            encoderDiff = 0;
          }

          reset_status_timeout(ms);

          #if LCD_BACKLIGHT_TIMEOUT
            refresh_backlight_timeout();
          #elif HAS_DISPLAY_SLEEP
            refresh_screen_timeout();
          #endif

          refresh(LCDVIEW_REDRAW_NOW);

          #if LED_POWEROFF_TIMEOUT > 0
            if (!powerManager.psu_on) leds.reset_timeout(ms);
          #endif
        } // encoder activity

      #endif // HAS_ENCODER_ACTION

      // This runs every ~100ms when idling often enough.
      // Instead of tracking changes just redraw the Status Screen once per second.
      if (on_status_screen() && !lcd_status_update_delay--) {
        lcd_status_update_delay = TERN(HAS_MARLINUI_U8GLIB, 12, 9);
        if (max_display_update_time) max_display_update_time--;  // Be sure never go to a very big number
        refresh(LCDVIEW_REDRAW_NOW);
      }

      #if BOTH(HAS_MARLINUI_MENU, SCROLL_LONG_FILENAMES)
        // If scrolling of long file names is enabled and we are in the sd card menu,
        // cause a refresh to occur until all the text has scrolled into view.
        if (currentScreen == menu_media && !lcd_status_update_delay--) {
          lcd_status_update_delay = ++filename_scroll_pos >= filename_scroll_max ? 12 : 4; // Long delay at end and start
          if (filename_scroll_pos > filename_scroll_max) filename_scroll_pos = 0;
          refresh(LCDVIEW_REDRAW_NOW);
          reset_status_timeout(ms);
        }
      #endif

      // Then we want to use only 50% of the time
      const uint16_t bbr2 = planner.block_buffer_runtime() >> 1;

      if ((should_draw() || drawing_screen) && (!bbr2 || bbr2 > max_display_update_time)) {

        // Change state of drawing flag between screen updates
        if (!drawing_screen) switch (lcdDrawUpdate) {
          case LCDVIEW_CALL_NO_REDRAW:
            refresh(LCDVIEW_NONE);
            break;
          case LCDVIEW_CLEAR_CALL_REDRAW:
          case LCDVIEW_CALL_REDRAW_NEXT:
            refresh(LCDVIEW_REDRAW_NOW);
          case LCDVIEW_REDRAW_NOW:        // set above, or by a handler through LCDVIEW_CALL_REDRAW_NEXT
          case LCDVIEW_NONE:
            break;
        } // switch

        TERN_(HAS_ADC_BUTTONS, keypad_buttons = 0);

        #if HAS_MARLINUI_U8GLIB

          #if ENABLED(LIGHTWEIGHT_UI)
            const bool in_status = on_status_screen(),
                       do_u8g_loop = !in_status;
            lcd_in_status(in_status);
            if (in_status) status_screen();
          #else
            constexpr bool do_u8g_loop = true;
          #endif

          if (do_u8g_loop) {
            if (!drawing_screen) {                // If not already drawing pages
              u8g.firstPage();                    // Start the first page
              drawing_screen = first_page = true; // Flag as drawing pages
            }
            set_font(FONT_MENU);                  // Setup font for every page draw
            u8g.setColorIndex(1);                 // And reset the color
            run_current_screen();                 // Draw and process the current screen
            first_page = false;

            // The screen handler can clear drawing_screen for an action that changes the screen.
            // If still drawing and there's another page, update max-time and return now.
            // The nextPage will already be set up on the next call.
            if (drawing_screen && (drawing_screen = u8g.nextPage())) {
              if (on_status_screen())
                NOLESS(max_display_update_time, millis() - ms);
              return;
            }
          }

        #else

          run_current_screen();

          // Apply all DWIN drawing after processing
          TERN_(IS_DWIN_MARLINUI, DWIN_UpdateLCD());

        #endif

        TERN_(HAS_MARLINUI_MENU, lcd_clicked = false);

        // Keeping track of the longest time for an individual LCD update.
        // Used to do screen throttling when the planner starts to fill up.
        if (on_status_screen())
          NOLESS(max_display_update_time, millis() - ms);
      }

      #if SCREENS_CAN_TIME_OUT
        // Return to Status Screen after a timeout
        if (on_status_screen() || defer_return_to_status)
          reset_status_timeout(ms);
        else if (ELAPSED(ms, return_to_status_ms))
          return_to_status();
      #endif

      #if LCD_BACKLIGHT_TIMEOUT
        if (backlight_off_ms && ELAPSED(ms, backlight_off_ms)) {
          WRITE(LCD_BACKLIGHT_PIN, LOW); // Backlight off
          backlight_off_ms = 0;
        }
      #elif HAS_DISPLAY_SLEEP
        if (screen_timeout_millis && ELAPSED(ms, screen_timeout_millis))
          sleep_on();
      #endif

      // Change state of drawing flag between screen updates
      if (!drawing_screen) switch (lcdDrawUpdate) {
        case LCDVIEW_CLEAR_CALL_REDRAW:
          clear_lcd(); break;
        case LCDVIEW_REDRAW_NOW:
          refresh(LCDVIEW_NONE);
        case LCDVIEW_NONE:
        case LCDVIEW_CALL_REDRAW_NEXT:
        case LCDVIEW_CALL_NO_REDRAW:
        default: break;
      } // switch

    } // ELAPSED(ms, next_lcd_update_ms)

    TERN_(HAS_GRAPHICAL_TFT, tft_idle());
  }

  #if HAS_ADC_BUTTONS

    typedef struct {
      raw_adc_t ADCKeyValueMin, ADCKeyValueMax;
      uint8_t  ADCKeyNo;
    } _stADCKeypadTable_;

    #ifndef ADC_BUTTONS_VALUE_SCALE
      #define ADC_BUTTONS_VALUE_SCALE       1.0  // for the power voltage equal to the reference voltage
    #endif
    #ifndef ADC_BUTTONS_R_PULLUP
      #define ADC_BUTTONS_R_PULLUP          4.7  // common pull-up resistor in the voltage divider
    #endif
    #ifndef ADC_BUTTONS_LEFT_R_PULLDOWN
      #define ADC_BUTTONS_LEFT_R_PULLDOWN   0.47 // pull-down resistor for LEFT button voltage divider
    #endif
    #ifndef ADC_BUTTONS_RIGHT_R_PULLDOWN
      #define ADC_BUTTONS_RIGHT_R_PULLDOWN  4.7  // pull-down resistor for RIGHT button voltage divider
    #endif
    #ifndef ADC_BUTTONS_UP_R_PULLDOWN
      #define ADC_BUTTONS_UP_R_PULLDOWN     1.0  // pull-down resistor for UP button voltage divider
    #endif
    #ifndef ADC_BUTTONS_DOWN_R_PULLDOWN
      #define ADC_BUTTONS_DOWN_R_PULLDOWN   10.0 // pull-down resistor for DOWN button voltage divider
    #endif
    #ifndef ADC_BUTTONS_MIDDLE_R_PULLDOWN
      #define ADC_BUTTONS_MIDDLE_R_PULLDOWN 2.2  // pull-down resistor for MIDDLE button voltage divider
    #endif

    // Calculate the ADC value for the voltage divider with specified pull-down resistor value
    #define ADC_BUTTON_VALUE(r)  raw_adc_t(HAL_ADC_RANGE * (ADC_BUTTONS_VALUE_SCALE) * r / (r + ADC_BUTTONS_R_PULLUP))

    static constexpr raw_adc_t adc_button_tolerance = HAL_ADC_RANGE *   25 / 1024,
                                   adc_other_button = raw_adc_t(uint32_t(HAL_ADC_RANGE * 1000UL) / 1024UL);
    static const _stADCKeypadTable_ stADCKeyTable[] PROGMEM = {
      // VALUE_MIN, VALUE_MAX, KEY
      { adc_other_button, HAL_ADC_RANGE, 1 + BLEN_KEYPAD_F1     }, // F1
      { adc_other_button, HAL_ADC_RANGE, 1 + BLEN_KEYPAD_F2     }, // F2
      { adc_other_button, HAL_ADC_RANGE, 1 + BLEN_KEYPAD_F3     }, // F3
      {  ADC_BUTTON_VALUE(ADC_BUTTONS_LEFT_R_PULLDOWN)   - adc_button_tolerance,
         ADC_BUTTON_VALUE(ADC_BUTTONS_LEFT_R_PULLDOWN)   + adc_button_tolerance, 1 + BLEN_KEYPAD_LEFT   }, // LEFT  ( 272 ...  472)
      {  ADC_BUTTON_VALUE(ADC_BUTTONS_RIGHT_R_PULLDOWN)  - adc_button_tolerance,
         ADC_BUTTON_VALUE(ADC_BUTTONS_RIGHT_R_PULLDOWN)  + adc_button_tolerance, 1 + BLEN_KEYPAD_RIGHT  }, // RIGHT (1948 ... 2148)
      {  ADC_BUTTON_VALUE(ADC_BUTTONS_UP_R_PULLDOWN)     - adc_button_tolerance,
         ADC_BUTTON_VALUE(ADC_BUTTONS_UP_R_PULLDOWN)     + adc_button_tolerance, 1 + BLEN_KEYPAD_UP     }, // UP    ( 618 ...  818)
      {  ADC_BUTTON_VALUE(ADC_BUTTONS_DOWN_R_PULLDOWN)   - adc_button_tolerance,
         ADC_BUTTON_VALUE(ADC_BUTTONS_DOWN_R_PULLDOWN)   + adc_button_tolerance, 1 + BLEN_KEYPAD_DOWN   }, // DOWN  (2686 ... 2886)
      {  ADC_BUTTON_VALUE(ADC_BUTTONS_MIDDLE_R_PULLDOWN) - adc_button_tolerance,
         ADC_BUTTON_VALUE(ADC_BUTTONS_MIDDLE_R_PULLDOWN) + adc_button_tolerance, 1 + BLEN_KEYPAD_MIDDLE }, // ENTER (1205 ... 1405)
    };

    uint8_t get_ADC_keyValue() {
      if (thermalManager.ADCKey_count >= 16) {
        const raw_adc_t currentkpADCValue = thermalManager.current_ADCKey_raw;
        thermalManager.current_ADCKey_raw = HAL_ADC_RANGE;
        thermalManager.ADCKey_count = 0;
        if (currentkpADCValue < adc_other_button)
          LOOP_L_N(i, ADC_KEY_NUM) {
            const raw_adc_t lo = pgm_read_word(&stADCKeyTable[i].ADCKeyValueMin),
                            hi = pgm_read_word(&stADCKeyTable[i].ADCKeyValueMax);
            if (WITHIN(currentkpADCValue, lo, hi)) return pgm_read_byte(&stADCKeyTable[i].ADCKeyNo);
          }
      }
      return 0;
    }

  #endif // HAS_ADC_BUTTONS

  #if HAS_ENCODER_ACTION

    /**
     * Read encoder buttons from the hardware registers
     * Warning: This function is called from interrupt context!
     */
    void MarlinUI::update_buttons() {
      const millis_t now = millis();
      if (ELAPSED(now, next_button_update_ms)) {

        #if HAS_DIGITAL_BUTTONS

          #if ANY_BUTTON(EN1, EN2, ENC, BACK)

            uint8_t newbutton = 0;
            if (BUTTON_PRESSED(EN1))                 newbutton |= EN_A;
            if (BUTTON_PRESSED(EN2))                 newbutton |= EN_B;
            if (can_encode() && BUTTON_PRESSED(ENC)) newbutton |= EN_C;
            if (BUTTON_PRESSED(BACK))                newbutton |= EN_D;

          #else

            constexpr uint8_t newbutton = 0;

          #endif

          //
          // Directional buttons
          //
          #if ANY_BUTTON(UP, DWN, LFT, RT)

            const int8_t pulses = epps * encoderDirection;

            if (BUTTON_PRESSED(UP)) {
              encoderDiff = (ENCODER_STEPS_PER_MENU_ITEM) * pulses;
              next_button_update_ms = now + 300;
            }
            else if (BUTTON_PRESSED(DWN)) {
              encoderDiff = -(ENCODER_STEPS_PER_MENU_ITEM) * pulses;
              next_button_update_ms = now + 300;
            }
            else if (BUTTON_PRESSED(LFT)) {
              encoderDiff = -pulses;
              next_button_update_ms = now + 300;
            }
            else if (BUTTON_PRESSED(RT)) {
              encoderDiff = pulses;
              next_button_update_ms = now + 300;
            }

          #endif // UP || DWN || LFT || RT

          buttons = (newbutton | TERN0(HAS_SLOW_BUTTONS, slow_buttons)
            #if BOTH(HAS_TOUCH_BUTTONS, HAS_ENCODER_ACTION)
              | (touch_buttons & TERN(HAS_ENCODER_WHEEL, ~(EN_A | EN_B), 0xFF))
            #endif
          );

        #elif HAS_ADC_BUTTONS

          buttons = 0;

        #endif

        #if HAS_ADC_BUTTONS
          if (keypad_buttons == 0) {
            const uint8_t b = get_ADC_keyValue();
            if (WITHIN(b, 1, 8)) keypad_buttons = _BV(b - 1);
          }
        #endif

        #if HAS_SHIFT_ENCODER
          /**
           * Set up Rotary Encoder bit values (for two pin encoders to indicate movement).
           * These values are independent of which pins are used for EN_A / EN_B indications.
           * The rotary encoder part is also independent of the LCD chipset.
           */
          uint8_t val = 0;
          WRITE(SHIFT_LD_PIN, LOW);
          WRITE(SHIFT_LD_PIN, HIGH);
          LOOP_L_N(i, 8) {
            val >>= 1;
            if (READ(SHIFT_OUT_PIN)) SBI(val, 7);
            WRITE(SHIFT_CLK_PIN, HIGH);
            WRITE(SHIFT_CLK_PIN, LOW);
          }
          TERN(REPRAPWORLD_KEYPAD, keypad_buttons, buttons) = ~val;
        #endif

        #if IS_TFTGLCD_PANEL
          next_button_update_ms = now + (LCD_UPDATE_INTERVAL / 2);
          buttons = slow_buttons;
          TERN_(AUTO_BED_LEVELING_UBL, external_encoder());
        #endif

      } // next_button_update_ms

      #if HAS_ENCODER_WHEEL
        static uint8_t lastEncoderBits;

        // Manage encoder rotation
        #define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: encoderDiff += encoderDirection; break; case _E2: encoderDiff -= encoderDirection; }

        uint8_t enc = 0;
        if (buttons & EN_A) enc |= B01;
        if (buttons & EN_B) enc |= B10;
        if (enc != lastEncoderBits) {
          switch (enc) {
            case ENCODER_PHASE_0: ENCODER_SPIN(ENCODER_PHASE_3, ENCODER_PHASE_1); break;
            case ENCODER_PHASE_1: ENCODER_SPIN(ENCODER_PHASE_0, ENCODER_PHASE_2); break;
            case ENCODER_PHASE_2: ENCODER_SPIN(ENCODER_PHASE_1, ENCODER_PHASE_3); break;
            case ENCODER_PHASE_3: ENCODER_SPIN(ENCODER_PHASE_2, ENCODER_PHASE_0); break;
          }
          #if BOTH(HAS_MARLINUI_MENU, AUTO_BED_LEVELING_UBL)
            external_encoder();
          #endif
          lastEncoderBits = enc;
        }

      #endif // HAS_ENCODER_WHEEL
    }

  #endif // HAS_ENCODER_ACTION

#endif // HAS_WIRED_LCD

#if HAS_STATUS_MESSAGE

  ////////////////////////////////////////////
  ////////////// Status Message //////////////
  ////////////////////////////////////////////

  #if ENABLED(EXTENSIBLE_UI)
    #include "extui/ui_api.h"
  #endif

  bool MarlinUI::has_status() { return (status_message[0] != '\0'); }

  void MarlinUI::set_status(const char * const cstr, const bool persist) {
    if (alert_level) return;

    TERN_(HOST_STATUS_NOTIFICATIONS, hostui.notify(cstr));

    // Here we have a problem. The message is encoded in UTF8, so
    // arbitrarily cutting it will be a problem. We MUST be sure
    // that there is no cutting in the middle of a multibyte character!

    // Get a pointer to the null terminator
    const char* pend = cstr + strlen(cstr);

    //  If length of supplied UTF8 string is greater than
    // our buffer size, start cutting whole UTF8 chars
    while ((pend - cstr) > MAX_MESSAGE_LENGTH) {
      --pend;
      while (!START_OF_UTF8_CHAR(*pend)) --pend;
    };

    // At this point, we have the proper cut point. Use it
    uint8_t maxLen = pend - cstr;
    strncpy(status_message, cstr, maxLen);
    status_message[maxLen] = '\0';

    finish_status(persist);
  }

  /**
   * Reset the status message
   */

  void MarlinUI::reset_status(const bool no_welcome) {
    #if SERVICE_INTERVAL_1 > 0
      static PGMSTR(service1, "> " SERVICE_NAME_1 "!");
    #endif
    #if SERVICE_INTERVAL_2 > 0
      static PGMSTR(service2, "> " SERVICE_NAME_2 "!");
    #endif
    #if SERVICE_INTERVAL_3 > 0
      static PGMSTR(service3, "> " SERVICE_NAME_3 "!");
    #endif

    FSTR_P msg;
    if (printingIsPaused())
      msg = GET_TEXT_F(MSG_PRINT_PAUSED);
    #if ENABLED(SDSUPPORT)
      else if (IS_SD_PRINTING())
        return set_status(card.longest_filename(), true);
    #endif
    else if (print_job_timer.isRunning())
      msg = GET_TEXT_F(MSG_PRINTING);

    #if SERVICE_INTERVAL_1 > 0
      else if (print_job_timer.needsService(1)) msg = FPSTR(service1);
    #endif
    #if SERVICE_INTERVAL_2 > 0
      else if (print_job_timer.needsService(2)) msg = FPSTR(service2);
    #endif
    #if SERVICE_INTERVAL_3 > 0
      else if (print_job_timer.needsService(3)) msg = FPSTR(service3);
    #endif

    else if (!no_welcome) msg = GET_TEXT_F(WELCOME_MSG);

    else if (ENABLED(DWIN_LCD_PROUI))
        msg = F("");
    else
      return;

    set_status(msg, -1);
  }

  /**
   * Set Status with a fixed string and alert level.
   * @param fstr  A constant F-string to set as the status.
   * @param level Alert level. Negative to ignore and reset the level. Non-zero never expires.
   */
  void MarlinUI::set_status(FSTR_P const fstr, int8_t level) {
    // Alerts block lower priority messages
    if (level < 0) level = alert_level = 0;
    if (level < alert_level) return;
    alert_level = level;

    PGM_P const pstr = FTOP(fstr);

    // Since the message is encoded in UTF8 it must
    // only be cut on a character boundary.

    // Get a pointer to the null terminator
    PGM_P pend = pstr + strlen_P(pstr);

    // If length of supplied UTF8 string is greater than
    // the buffer size, start cutting whole UTF8 chars
    while ((pend - pstr) > MAX_MESSAGE_LENGTH) {
      --pend;
      while (!START_OF_UTF8_CHAR(pgm_read_byte(pend))) --pend;
    };

    // At this point, we have the proper cut point. Use it
    uint8_t maxLen = pend - pstr;
    strncpy_P(status_message, pstr, maxLen);
    status_message[maxLen] = '\0';

    TERN_(HOST_STATUS_NOTIFICATIONS, hostui.notify(fstr));

    finish_status(level > 0);
  }

  void MarlinUI::set_alert_status(FSTR_P const fstr) {
    set_status(fstr, 1);
    TERN_(HAS_TOUCH_SLEEP, wakeup_screen());
    TERN_(HAS_MARLINUI_MENU, return_to_status());
  }

  #include <stdarg.h>

  void MarlinUI::status_printf(int8_t level, FSTR_P const fmt, ...) {
    // Alerts block lower priority messages
    if (level < 0) level = alert_level = 0;
    if (level < alert_level) return;
    alert_level = level;

    va_list args;
    va_start(args, FTOP(fmt));
    vsnprintf_P(status_message, MAX_MESSAGE_LENGTH, FTOP(fmt), args);
    va_end(args);

    TERN_(HOST_STATUS_NOTIFICATIONS, hostui.notify(status_message));

    finish_status(level > 0);
  }

  void MarlinUI::finish_status(const bool persist) {

    UNUSED(persist);

    set_status_reset_fn();

    TERN_(HAS_STATUS_MESSAGE_TIMEOUT, status_message_expire_ms = persist ? 0 : millis() + (STATUS_MESSAGE_TIMEOUT_SEC) * 1000UL);

    #if HAS_WIRED_LCD

      #if BASIC_PROGRESS_BAR || BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
        const millis_t ms = millis();
      #endif

      #if BASIC_PROGRESS_BAR
        progress_bar_ms = ms;
        #if PROGRESS_MSG_EXPIRE > 0
          expire_status_ms = persist ? 0 : ms + PROGRESS_MSG_EXPIRE;
        #endif
      #endif

      #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
        next_filament_display = ms + 5000UL; // Show status message for 5s
      #endif

    #endif

    #if ENABLED(STATUS_MESSAGE_SCROLLING) && EITHER(HAS_WIRED_LCD, DWIN_LCD_PROUI)
      status_scroll_offset = 0;
    #endif

    TERN_(EXTENSIBLE_UI, ExtUI::onStatusChanged(status_message));
    TERN_(DWIN_CREALITY_LCD, DWIN_StatusChanged(status_message));
    TERN_(DWIN_LCD_PROUI, DWIN_CheckStatusMessage());
    TERN_(DWIN_CREALITY_LCD_JYERSUI, CrealityDWIN.Update_Status(status_message));
  }

  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    void MarlinUI::advance_status_scroll() {
      // Advance by one UTF8 code-word
      if (status_scroll_offset < utf8_strlen(status_message))
        while (!START_OF_UTF8_CHAR(status_message[++status_scroll_offset]));
      else
        status_scroll_offset = 0;
    }

    char* MarlinUI::status_and_len(uint8_t &len) {
      char *out = status_message + status_scroll_offset;
      len = utf8_strlen(out);
      return out;
    }

  #endif

#else // !HAS_STATUS_MESSAGE

  //
  // Send the status line as a host notification
  //
  void MarlinUI::set_status(const char * const cstr, const bool) {
    TERN(HOST_PROMPT_SUPPORT, hostui.notify(cstr), UNUSED(cstr));
  }
  void MarlinUI::set_status(FSTR_P const fstr, const int8_t) {
    TERN(HOST_PROMPT_SUPPORT, hostui.notify(fstr), UNUSED(fstr));
  }
  void MarlinUI::status_printf(int8_t, FSTR_P const fstr, ...) {
    TERN(HOST_PROMPT_SUPPORT, hostui.notify(fstr), UNUSED(fstr));
  }

#endif // !HAS_STATUS_MESSAGE

#if HAS_DISPLAY

  #if ENABLED(SDSUPPORT)
    extern bool wait_for_user, wait_for_heatup;
  #endif

  void MarlinUI::abort_print() {
    #if ENABLED(SDSUPPORT)
      wait_for_heatup = wait_for_user = false;
      card.abortFilePrintSoon();
    #endif
    #ifdef ACTION_ON_CANCEL
      hostui.cancel();
    #endif
    IF_DISABLED(SDSUPPORT, print_job_timer.stop());
    TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_INFO, F("UI Aborted"), FPSTR(DISMISS_STR)));
    LCD_MESSAGE(MSG_PRINT_ABORTED);
    TERN_(HAS_MARLINUI_MENU, return_to_status());
  }

  #if BOTH(HAS_MARLINUI_MENU, PSU_CONTROL)

    void MarlinUI::poweroff() {
      queue.inject(F("M81" TERN_(POWER_OFF_WAIT_FOR_COOLDOWN, "S")));
      return_to_status();
    }

  #endif

  void MarlinUI::flow_fault() {
    LCD_ALERTMESSAGE(MSG_FLOWMETER_FAULT);
    BUZZ(1000, 440);
    TERN_(HAS_MARLINUI_MENU, return_to_status());
  }

  void MarlinUI::pause_print() {
    #if HAS_MARLINUI_MENU
      synchronize(GET_TEXT(MSG_PAUSING));
      defer_status_screen();
    #endif

    TERN_(HAS_TOUCH_SLEEP, wakeup_screen());
    TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_PAUSE_RESUME, F("UI Pause"), F("Resume")));

    LCD_MESSAGE(MSG_PRINT_PAUSED);

    #if ENABLED(PARK_HEAD_ON_PAUSE)
      pause_show_message(PAUSE_MESSAGE_PARKING, PAUSE_MODE_PAUSE_PRINT); // Show message immediately to let user know about pause in progress
      queue.inject(F("M25 P\nM24"));
    #elif ENABLED(SDSUPPORT)
      queue.inject(F("M25"));
    #elif defined(ACTION_ON_PAUSE)
      hostui.pause();
    #endif
  }

  void MarlinUI::resume_print() {
    reset_status();
    TERN_(PARK_HEAD_ON_PAUSE, wait_for_heatup = wait_for_user = false);
    TERN_(SDSUPPORT, if (IS_SD_PAUSED()) queue.inject_P(M24_STR));
    #ifdef ACTION_ON_RESUME
      hostui.resume();
    #endif
    print_job_timer.start(); // Also called by M24
  }

  #if HAS_PRINT_PROGRESS

    MarlinUI::progress_t MarlinUI::_get_progress() {
      return (
        TERN0(LCD_SET_PROGRESS_MANUALLY, (progress_override & PROGRESS_MASK))
        #if ENABLED(SDSUPPORT)
          ?: TERN(HAS_PRINT_PROGRESS_PERMYRIAD, card.permyriadDone(), card.percentDone())
        #endif
      );
    }

  #endif

  #if HAS_TOUCH_BUTTONS

    //
    // Screen Click
    //  - On menu screens move directly to the touched item
    //  - On menu screens, right side (last 3 cols) acts like a scroll - half up => prev page, half down = next page
    //  - On select screens (and others) touch the Right Half for +, Left Half for -
    //  - On edit screens, touch Up Half for -,  Bottom Half to +
    //
    void MarlinUI::screen_click(const uint8_t row, const uint8_t col, const uint8_t, const uint8_t) {
      const millis_t now = millis();
      if (PENDING(now, next_button_update_ms)) return;
      next_button_update_ms = now + repeat_delay;    // Assume the repeat delay
      const int8_t xdir = col < (LCD_WIDTH ) / 2 ? -1 : 1,
                   ydir = row < (LCD_HEIGHT) / 2 ? -1 : 1;
      if (on_edit_screen)
        encoderDiff = epps * ydir;
      else if (screen_items > 0) {
        // Last 5 cols act as a scroll :-)
        if (col > (LCD_WIDTH) - 5)
          // 2 * LCD_HEIGHT to scroll to bottom of next page. (LCD_HEIGHT would only go 1 item down.)
          encoderDiff = epps * (encoderLine - encoderTopLine + 2 * (LCD_HEIGHT)) * ydir;
        else
          encoderDiff = epps * (row - encoderPosition + encoderTopLine);
      }
      else if (!on_status_screen())
        encoderDiff = epps * xdir;
    }

  #endif

#endif // HAS_DISPLAY

#if ENABLED(SDSUPPORT)

  #if ENABLED(EXTENSIBLE_UI)
    #include "extui/ui_api.h"
  #endif

  void MarlinUI::media_changed(const uint8_t old_status, const uint8_t status) {
    if (old_status == status) {
      TERN_(EXTENSIBLE_UI, ExtUI::onMediaError()); // Failed to mount/unmount
      return;
    }

    if (status) {
      if (old_status < 2) {
        #if ENABLED(EXTENSIBLE_UI)
          ExtUI::onMediaInserted();
        #elif ENABLED(BROWSE_MEDIA_ON_INSERT)
          clear_menu_history();
          quick_feedback();
          goto_screen(MEDIA_MENU_GATEWAY);
        #else
          LCD_MESSAGE(MSG_MEDIA_INSERTED);
        #endif
      }
    }
    else {
      if (old_status < 2) {
        #if ENABLED(EXTENSIBLE_UI)
          ExtUI::onMediaRemoved();
        #elif PIN_EXISTS(SD_DETECT)
          LCD_MESSAGE(MSG_MEDIA_REMOVED);
          #if HAS_MARLINUI_MENU
            if (!defer_return_to_status) return_to_status();
          #endif
        #endif
      }
    }

    reinit_lcd(); // Revive a noisy shared SPI LCD

    refresh();

    #if HAS_WIRED_LCD || LED_POWEROFF_TIMEOUT > 0
      const millis_t ms = millis();
    #endif

    TERN_(HAS_WIRED_LCD, next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL); // Delay LCD update for SD activity

    #if LED_POWEROFF_TIMEOUT > 0
      leds.reset_timeout(ms);
    #endif
  }

#endif // SDSUPPORT

#if HAS_MARLINUI_MENU
  void MarlinUI::reset_settings() {
    settings.reset();
    completion_feedback();
    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      if (touch_calibration.need_calibration()) ui.goto_screen(touch_screen_calibration);
    #endif
  }

  #if EITHER(BABYSTEP_ZPROBE_GFX_OVERLAY, MESH_EDIT_GFX_OVERLAY)
    void MarlinUI::zoffset_overlay(const_float_t zvalue) {
      // Determine whether the user is raising or lowering the nozzle.
      static int8_t dir;
      static float old_zvalue;
      if (zvalue != old_zvalue) {
        dir = zvalue ? zvalue < old_zvalue ? -1 : 1 : 0;
        old_zvalue = zvalue;
      }
      zoffset_overlay(dir);
    }
  #endif

#endif

#if BOTH(EXTENSIBLE_UI, ADVANCED_PAUSE_FEATURE)

  void MarlinUI::pause_show_message(
    const PauseMessage message,
    const PauseMode mode/*=PAUSE_MODE_SAME*/,
    const uint8_t extruder/*=active_extruder*/
  ) {
    pause_mode = mode;
    ExtUI::pauseModeStatus = message;
    switch (message) {
      case PAUSE_MESSAGE_PARKING:  ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_PAUSE_PRINT_PARKING)); break;
      case PAUSE_MESSAGE_CHANGING: ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_INIT)); break;
      case PAUSE_MESSAGE_UNLOAD:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_UNLOAD)); break;
      case PAUSE_MESSAGE_WAITING:  ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING)); break;
      case PAUSE_MESSAGE_INSERT:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_INSERT)); break;
      case PAUSE_MESSAGE_LOAD:     ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_LOAD)); break;
      case PAUSE_MESSAGE_PURGE:
        ExtUI::onUserConfirmRequired(GET_TEXT_F(TERN(ADVANCED_PAUSE_CONTINUOUS_PURGE, MSG_FILAMENT_CHANGE_CONT_PURGE, MSG_FILAMENT_CHANGE_PURGE)));
        break;
      case PAUSE_MESSAGE_RESUME:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT)); break;
      case PAUSE_MESSAGE_HEATING:  ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_OPTION:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_OPTION_HEADER)); break;
      case PAUSE_MESSAGE_STATUS:   break;
      default: break;
    }
  }

#endif

#if ENABLED(EEPROM_SETTINGS)

  #if HAS_MARLINUI_MENU
    void MarlinUI::init_eeprom() {
      const bool good = settings.init_eeprom();
      completion_feedback(good);
      return_to_status();
    }
    void MarlinUI::load_settings() {
      const bool good = settings.load();
      completion_feedback(good);
    }
    void MarlinUI::store_settings() {
      const bool good = settings.save();
      completion_feedback(good);
    }
  #endif

  #if DISABLED(EEPROM_AUTO_INIT)

    static inline PGM_P eeprom_err(const uint8_t msgid) {
      switch (msgid) {
        default:
        case 0: return GET_TEXT(MSG_ERR_EEPROM_CRC);
        case 1: return GET_TEXT(MSG_ERR_EEPROM_INDEX);
        case 2: return GET_TEXT(MSG_ERR_EEPROM_VERSION);
      }
    }

    void MarlinUI::eeprom_alert(const uint8_t msgid) {
      #if HAS_MARLINUI_MENU
        editable.uint8 = msgid;
        goto_screen([]{
          PGM_P const restore_msg = GET_TEXT(MSG_INIT_EEPROM);
          char msg[utf8_strlen_P(restore_msg) + 1];
          strcpy_P(msg, restore_msg);
          MenuItem_confirm::select_screen(
            GET_TEXT(MSG_BUTTON_RESET), GET_TEXT(MSG_BUTTON_IGNORE),
            init_eeprom, return_to_status,
            eeprom_err(editable.uint8), msg, PSTR("?")
          );
        });
      #else
        set_status(FPSTR(eeprom_err(msgid)));
      #endif
    }

  #endif // EEPROM_AUTO_INIT

#endif // EEPROM_SETTINGS
