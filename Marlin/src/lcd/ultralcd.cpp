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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfigPre.h"

#ifdef LED_BACKLIGHT_TIMEOUT
  #include "../feature/leds/leds.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../feature/host_actions.h"
#endif

// All displays share the MarlinUI class
#include "ultralcd.h"
MarlinUI ui;

#if HAS_DISPLAY
  #include "../module/printcounter.h"
  #include "../MarlinCore.h"
  #include "../gcode/queue.h"
  #include "fontutils.h"
  #include "../sd/cardreader.h"
  #if EITHER(EXTENSIBLE_UI, DWIN_CREALITY_LCD)
    #define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80u)
  #endif
#endif

#if LCD_HAS_WAIT_FOR_MOVE
  bool MarlinUI::wait_for_move; // = false
#endif

#if HAS_SPI_LCD
  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    uint8_t MarlinUI::status_scroll_offset; // = 0
    constexpr uint8_t MAX_MESSAGE_LENGTH = _MAX(LONG_FILENAME_LENGTH, MAX_LANG_CHARSIZE * 2 * (LCD_WIDTH));
  #else
    constexpr uint8_t MAX_MESSAGE_LENGTH = MAX_LANG_CHARSIZE * (LCD_WIDTH);
  #endif
#elif EITHER(EXTENSIBLE_UI, DWIN_CREALITY_LCD)
  constexpr uint8_t MAX_MESSAGE_LENGTH = 63;
#endif

#if EITHER(HAS_SPI_LCD, EXTENSIBLE_UI)
  uint8_t MarlinUI::alert_level; // = 0
  char MarlinUI::status_message[MAX_MESSAGE_LENGTH + 1];
#endif

#if ENABLED(LCD_SET_PROGRESS_MANUALLY)
  MarlinUI::progress_t MarlinUI::progress_override; // = 0
  #if BOTH(LCD_SET_PROGRESS_MANUALLY, USE_M73_REMAINING_TIME)
    uint32_t MarlinUI::remaining_time;
  #endif
#endif

#if ENABLED(PCA9632_BUZZER) || USE_BEEPER
  #include "../libs/buzzer.h" // for BUZZ() macro
  #if ENABLED(PCA9632_BUZZER)
    #include "../feature/leds/pca9632.h"
  #endif
  void MarlinUI::buzz(const long duration, const uint16_t freq) {
    #if ENABLED(PCA9632_BUZZER)
      pca9632_buzz(duration, freq);
    #elif USE_BEEPER
      buzzer.tone(duration, freq);
    #endif
  }
#endif

#if PREHEAT_COUNT
  preheat_t MarlinUI::material_preset[PREHEAT_COUNT];  // Initialized by settings.load()
#endif

#if HAS_SPI_LCD

#if HAS_GRAPHICAL_LCD
  #include "dogm/ultralcd_DOGM.h"
#endif

#include "lcdprint.h"

#include "../sd/cardreader.h"
#include "../module/configuration_store.h"
#include "../module/temperature.h"
#include "../module/planner.h"
#include "../module/motion.h"

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

#if HAS_ENCODER_ACTION
  volatile uint8_t MarlinUI::buttons;
  #if HAS_SLOW_BUTTONS
    volatile uint8_t MarlinUI::slow_buttons;
  #endif
  #if ENABLED(TOUCH_BUTTONS)
    #include "../feature/touch/xpt2046.h"
    bool MarlinUI::on_edit_screen = false;
  #endif
#endif

#if HAS_LCD_MENU && LCD_TIMEOUT_TO_STATUS > 0
  bool MarlinUI::defer_return_to_status;
#endif

uint8_t MarlinUI::lcd_status_update_delay = 1; // First update one loop delayed

#if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
  millis_t MarlinUI::next_filament_display; // = 0
#endif

millis_t MarlinUI::next_button_update_ms; // = 0

#if HAS_GRAPHICAL_LCD
  bool MarlinUI::drawing_screen, MarlinUI::first_page; // = false
#endif

// Encoder Handling
#if HAS_ENCODER_ACTION
  uint32_t MarlinUI::encoderPosition;
  volatile int8_t encoderDiff; // Updated in update_buttons, added to encoderPosition every LCD update
#endif

#if ENABLED(SDSUPPORT)

  #include "../sd/cardreader.h"

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
          outstr += filename_scroll_pos;
        }
      #else
        theCard.longFilename[maxlen] = '\0'; // cutoff at screen edge
      #endif
    }
    return outstr;
  }

#endif

#if HAS_LCD_MENU
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

  #if ENABLED(TOUCH_BUTTONS)
    uint8_t MarlinUI::touch_buttons;
    uint8_t MarlinUI::repeat_delay;
  #endif

  bool MarlinUI::lcd_clicked;
  float move_menu_scale;

  bool MarlinUI::use_click() {
    const bool click = lcd_clicked;
    lcd_clicked = false;
    return click;
  }

  #if EITHER(AUTO_BED_LEVELING_UBL, G26_MESH_VALIDATION)

    bool MarlinUI::external_control; // = false

    void MarlinUI::wait_for_release() {
      while (button_pressed()) safe_delay(50);
      safe_delay(50);
    }

  #endif

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
    wrap_string_P(col, row, pref, true);
    if (string) {
      if (col) { col = 0; row++; } // Move to the start of the next line
      wrap_string(col, row, string);
    }
    if (suff) wrap_string_P(col, row, suff);
  }

#endif // HAS_LCD_MENU

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

  #endif // !HAS_DIGITAL_BUTTONS

  #if HAS_SHIFT_ENCODER

    #if ENABLED(SR_LCD_2W_NL) // Non latching 2 wire shift register

      SET_OUTPUT(SR_DATA_PIN);
      SET_OUTPUT(SR_CLK_PIN);

    #elif defined(SHIFT_CLK)

      SET_OUTPUT(SHIFT_CLK);
      OUT_WRITE(SHIFT_LD, HIGH);
      #if defined(SHIFT_EN) && SHIFT_EN >= 0
        OUT_WRITE(SHIFT_EN, LOW);
      #endif
      SET_INPUT_PULLUP(SHIFT_OUT);

    #endif

  #endif // HAS_SHIFT_ENCODER

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
  #endif

  #if HAS_ENCODER_ACTION && HAS_SLOW_BUTTONS
    slow_buttons = 0;
  #endif

  update_buttons();

  TERN_(HAS_ENCODER_ACTION, encoderDiff = 0);
}

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

////////////////////////////////////////////
///////////// Keypad Handling //////////////
////////////////////////////////////////////

#if BOTH(REPRAPWORLD_KEYPAD, HAS_ENCODER_ACTION)

  volatile uint8_t MarlinUI::keypad_buttons;

  #if HAS_LCD_MENU && !HAS_ADC_BUTTONS

    void lcd_move_x();
    void lcd_move_y();
    void lcd_move_z();

    void _reprapworld_keypad_move(const AxisEnum axis, const int16_t dir) {
      move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
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
          #if HAS_LCD_MENU
            if (encoderDirection == -(ENCODERBASE)) { // ADC_KEYPAD forces REVERSE_MENU_DIRECTION, so this indicates menu navigation
                   if (RRK(EN_KEYPAD_UP))     encoderPosition += ENCODER_STEPS_PER_MENU_ITEM;
              else if (RRK(EN_KEYPAD_DOWN))   encoderPosition -= ENCODER_STEPS_PER_MENU_ITEM;
              else if (RRK(EN_KEYPAD_LEFT))   { MenuItem_back::action(); quick_feedback(); }
              else if (RRK(EN_KEYPAD_RIGHT))  { return_to_status(); quick_feedback(); }
            }
            else
          #endif
          {
            #if HAS_LCD_MENU
                   if (RRK(EN_KEYPAD_UP))     encoderPosition -= ENCODER_PULSES_PER_STEP;
              else if (RRK(EN_KEYPAD_DOWN))   encoderPosition += ENCODER_PULSES_PER_STEP;
              else if (RRK(EN_KEYPAD_LEFT))   { MenuItem_back::action(); quick_feedback(); }
              else if (RRK(EN_KEYPAD_RIGHT))  encoderPosition = 0;
            #else
                   if (RRK(EN_KEYPAD_UP)   || RRK(EN_KEYPAD_LEFT))  encoderPosition -= ENCODER_PULSES_PER_STEP;
              else if (RRK(EN_KEYPAD_DOWN) || RRK(EN_KEYPAD_RIGHT)) encoderPosition += ENCODER_PULSES_PER_STEP;
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

        #if HAS_LCD_MENU

          if (RRK(EN_KEYPAD_MIDDLE))  goto_screen(menu_move);

          #if DISABLED(DELTA) && Z_HOME_DIR < 0
            if (RRK(EN_KEYPAD_F2))    _reprapworld_keypad_move(Z_AXIS,  1);
          #endif

          if (homed) {
            #if ENABLED(DELTA) || Z_HOME_DIR != -1
              if (RRK(EN_KEYPAD_F2))  _reprapworld_keypad_move(Z_AXIS,  1);
            #endif
            if (RRK(EN_KEYPAD_F3))    _reprapworld_keypad_move(Z_AXIS, -1);
            if (RRK(EN_KEYPAD_LEFT))  _reprapworld_keypad_move(X_AXIS, -1);
            if (RRK(EN_KEYPAD_RIGHT)) _reprapworld_keypad_move(X_AXIS,  1);
            if (RRK(EN_KEYPAD_DOWN))  _reprapworld_keypad_move(Y_AXIS,  1);
            if (RRK(EN_KEYPAD_UP))    _reprapworld_keypad_move(Y_AXIS, -1);
          }

        #endif // HAS_LCD_MENU

        if (!homed && RRK(EN_KEYPAD_F1)) queue.inject_P(G28_STR);
        return true;
      }

    #endif // !ADC_KEYPAD

    return false;
  }

#endif // REPRAPWORLD_KEYPAD

/**
 * Status Screen
 *
 * This is very display-dependent, so the lcd implementation draws this.
 */

#if ENABLED(LCD_PROGRESS_BAR)
  millis_t MarlinUI::progress_bar_ms; // = 0
  #if PROGRESS_MSG_EXPIRE > 0
    millis_t MarlinUI::expire_status_ms; // = 0
  #endif
#endif

void MarlinUI::status_screen() {

  TERN_(HAS_LCD_MENU, ENCODER_RATE_MULTIPLY(false));

  #if ENABLED(LCD_PROGRESS_BAR)

    //
    // HD44780 implements the following message blinking and
    // message expiration because Status Line and Progress Bar
    // share the same line on the display.
    //

    #if DISABLED(PROGRESS_MSG_ONCE) || (PROGRESS_MSG_EXPIRE > 0)
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

  #endif // LCD_PROGRESS_BAR

  #if HAS_LCD_MENU
    if (use_click()) {
      #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
        next_filament_display = millis() + 5000UL;  // Show status message for 5s
      #endif
      goto_screen(menu_main);
      #if DISABLED(NO_LCD_REINIT)
        init_lcd(); // May revive the LCD if static electricity killed it
      #endif
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
          buzz(FEEDRATE_CHANGE_BEEP_DURATION, FEEDRATE_CHANGE_BEEP_FREQUENCY);
          next_beep = ms + 500UL;
        }
      #endif
    }

  #endif // ULTIPANEL_FEEDMULTIPLY

  draw_status_screen();
}

void MarlinUI::kill_screen(PGM_P lcd_error, PGM_P lcd_component) {
  init();
  status_printf_P(1, PSTR(S_FMT ": " S_FMT), lcd_error, lcd_component);
  TERN_(HAS_LCD_MENU, return_to_status());

  // RED ALERT. RED ALERT.
  #ifdef LED_BACKLIGHT_TIMEOUT
    leds.set_color(LEDColorRed());
    #ifdef NEOPIXEL_BKGD_LED_INDEX
      neo.set_pixel_color(NEOPIXEL_BKGD_LED_INDEX, 255, 0, 0, 0);
      neo.show();
    #endif
  #endif

  draw_kill_screen();
}

void MarlinUI::quick_feedback(const bool clear_buttons/*=true*/) {

  TERN_(HAS_LCD_MENU, refresh());

  #if HAS_ENCODER_ACTION
    if (clear_buttons) buttons = 0;
    next_button_update_ms = millis() + 500;
  #else
    UNUSED(clear_buttons);
  #endif

  #if HAS_CHIRP
    chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if BOTH(HAS_LCD_MENU, USE_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_LCD_MENU
      delay(10);
    #endif
  #endif
}

////////////////////////////////////////////
/////////////// Manual Move ////////////////
////////////////////////////////////////////

#if HAS_LCD_MENU

  int8_t MarlinUI::manual_move_axis = (int8_t)NO_AXIS;
  millis_t MarlinUI::manual_move_start_time = 0;

  #if IS_KINEMATIC
    bool MarlinUI::processing_manual_move = false;
    float MarlinUI::manual_move_offset = 0;
  #endif

  #if MULTI_MANUAL
    int8_t MarlinUI::manual_move_e_index = 0;
  #endif

  /**
   * If the most recent manual move hasn't been fed to the planner yet,
   * and the planner can accept one, send a move immediately.
   */
  void MarlinUI::manage_manual_move() {

    if (processing_manual_move) return;

    if (manual_move_axis != (int8_t)NO_AXIS && ELAPSED(millis(), manual_move_start_time) && !planner.is_full()) {

      const feedRate_t fr_mm_s = manual_feedrate_mm_s[manual_move_axis];
      #if IS_KINEMATIC

        #if EXTRUDERS > 1
          const int8_t old_extruder = active_extruder;
          if (manual_move_axis == E_AXIS) active_extruder = manual_move_e_index;
        #endif

        // Set movement on a single axis
        destination = current_position;
        destination[manual_move_axis] += manual_move_offset;

        // Reset for the next move
        manual_move_offset = 0;
        manual_move_axis = (int8_t)NO_AXIS;

        // DELTA and SCARA machines use segmented moves, which could fill the planner during the call to
        // move_to_destination. This will cause idle() to be called, which can then call this function while the
        // previous invocation is being blocked. Modifications to manual_move_offset shouldn't be made while
        // processing_manual_move is true or the planner will get out of sync.
        processing_manual_move = true;
        prepare_internal_move_to_destination(fr_mm_s);  // will set current_position from destination
        processing_manual_move = false;

        #if EXTRUDERS > 1
          active_extruder = old_extruder;
        #endif

      #else

        planner.buffer_line(current_position, fr_mm_s, manual_move_axis == E_AXIS ? manual_move_e_index : active_extruder);
        manual_move_axis = (int8_t)NO_AXIS;

      #endif
    }
  }

#endif // HAS_LCD_MENU

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

void MarlinUI::update() {

  static uint16_t max_display_update_time = 0;
  millis_t ms = millis();

  #if HAS_LCD_MENU && LCD_TIMEOUT_TO_STATUS > 0
    static millis_t return_to_status_ms = 0;
    #define RESET_STATUS_TIMEOUT() (return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS)
  #else
    #define RESET_STATUS_TIMEOUT() NOOP
  #endif

  #ifdef LED_BACKLIGHT_TIMEOUT
    leds.update_timeout(powersupply_on);
  #endif

  #if HAS_LCD_MENU

    // Handle any queued Move Axis motion
    manage_manual_move();

    // Update button states for button_pressed(), etc.
    // If the state changes the next update may be delayed 300-500ms.
    update_buttons();

    // If the action button is pressed...
    static bool wait_for_unclick; // = false

    auto do_click = [&]{
      wait_for_unclick = true;                        //  - Set debounce flag to ignore continous clicks
      lcd_clicked = !wait_for_user;                   //  - Keep the click if not waiting for a user-click
      wait_for_user = false;                          //  - Any click clears wait for user
      quick_feedback();                               //  - Always make a click sound
    };

    #if ENABLED(TOUCH_BUTTONS)
      if (touch_buttons) {
        RESET_STATUS_TIMEOUT();
        if (touch_buttons & (EN_A | EN_B)) {              // Menu arrows, in priority
          if (ELAPSED(ms, next_button_update_ms)) {
            encoderDiff = (ENCODER_STEPS_PER_MENU_ITEM) * (ENCODER_PULSES_PER_STEP) * encoderDirection;
            if (touch_buttons & EN_A) encoderDiff *= -1;
            TERN_(AUTO_BED_LEVELING_UBL, if (external_control) ubl.encoder_diff = encoderDiff);
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
      else // keep wait_for_unclick value

    #endif // TOUCH_BUTTONS

      {
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

  #endif // HAS_LCD_MENU

  if (ELAPSED(ms, next_lcd_update_ms) || TERN0(HAS_GRAPHICAL_LCD, drawing_screen)) {

    next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;

    #if ENABLED(TOUCH_BUTTONS)

      if (on_status_screen()) next_lcd_update_ms += (LCD_UPDATE_INTERVAL) * 2;

      TERN_(HAS_ENCODER_ACTION, touch_buttons = touch.read_buttons());

    #endif

    TERN_(LCD_HAS_STATUS_INDICATORS, update_indicators());

    #if HAS_ENCODER_ACTION

      TERN_(HAS_SLOW_BUTTONS, slow_buttons = read_slow_buttons()); // Buttons that take too long to read in interrupt context

      if (TERN0(REPRAPWORLD_KEYPAD, handle_keypad()))
        RESET_STATUS_TIMEOUT();

      const float abs_diff = ABS(encoderDiff);
      const bool encoderPastThreshold = (abs_diff >= (ENCODER_PULSES_PER_STEP));
      if (encoderPastThreshold || lcd_clicked) {
        if (encoderPastThreshold) {

          #if BOTH(HAS_LCD_MENU, ENCODER_RATE_MULTIPLIER)

            int32_t encoderMultiplier = 1;

            if (encoderRateMultiplierEnabled) {
              const float encoderMovementSteps = abs_diff / (ENCODER_PULSES_PER_STEP);

              if (lastEncoderMovementMillis) {
                // Note that the rate is always calculated between two passes through the
                // loop and that the abs of the encoderDiff value is tracked.
                const float encoderStepRate = encoderMovementSteps / float(ms - lastEncoderMovementMillis) * 1000;

                if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC)     encoderMultiplier = 100;
                else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC) encoderMultiplier = 10;

                #if ENABLED(ENCODER_RATE_MULTIPLIER_DEBUG)
                  SERIAL_ECHO_START();
                  SERIAL_ECHOPAIR("Enc Step Rate: ", encoderStepRate);
                  SERIAL_ECHOPAIR("  Multiplier: ", encoderMultiplier);
                  SERIAL_ECHOPAIR("  ENCODER_10X_STEPS_PER_SEC: ", ENCODER_10X_STEPS_PER_SEC);
                  SERIAL_ECHOPAIR("  ENCODER_100X_STEPS_PER_SEC: ", ENCODER_100X_STEPS_PER_SEC);
                  SERIAL_EOL();
                #endif
              }

              lastEncoderMovementMillis = ms;
            } // encoderRateMultiplierEnabled

          #else

            constexpr int32_t encoderMultiplier = 1;

          #endif // ENCODER_RATE_MULTIPLIER

          encoderPosition += (encoderDiff * encoderMultiplier) / (ENCODER_PULSES_PER_STEP);
          encoderDiff = 0;
        }

        RESET_STATUS_TIMEOUT();

        refresh(LCDVIEW_REDRAW_NOW);

        #ifdef LED_BACKLIGHT_TIMEOUT
          leds.reset_timeout(ms);
        #endif
      }

    #endif

    // This runs every ~100ms when idling often enough.
    // Instead of tracking changes just redraw the Status Screen once per second.
    if (on_status_screen() && !lcd_status_update_delay--) {
      lcd_status_update_delay = TERN(HAS_GRAPHICAL_LCD, 12, 9);
      if (max_display_update_time) max_display_update_time--;  // Be sure never go to a very big number
      refresh(LCDVIEW_REDRAW_NOW);
    }

    #if BOTH(HAS_LCD_MENU, SCROLL_LONG_FILENAMES)
      // If scrolling of long file names is enabled and we are in the sd card menu,
      // cause a refresh to occur until all the text has scrolled into view.
      if (currentScreen == menu_media && !lcd_status_update_delay--) {
        lcd_status_update_delay = 4;
        if (++filename_scroll_pos > filename_scroll_max) {
          filename_scroll_pos = 0;
          lcd_status_update_delay = 12;
        }
        refresh(LCDVIEW_REDRAW_NOW);
        RESET_STATUS_TIMEOUT();
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

      #if HAS_GRAPHICAL_LCD

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

      #endif

      TERN_(HAS_LCD_MENU, lcd_clicked = false);

      // Keeping track of the longest time for an individual LCD update.
      // Used to do screen throttling when the planner starts to fill up.
      if (on_status_screen())
        NOLESS(max_display_update_time, millis() - ms);
    }

    #if HAS_LCD_MENU && LCD_TIMEOUT_TO_STATUS > 0
      // Return to Status Screen after a timeout
      if (on_status_screen() || defer_return_to_status)
        RESET_STATUS_TIMEOUT();
      else if (ELAPSED(ms, return_to_status_ms))
        return_to_status();
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
}

#if HAS_ADC_BUTTONS

  typedef struct {
    uint16_t ADCKeyValueMin, ADCKeyValueMax;
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
  #define ADC_BUTTON_VALUE(r)  int(HAL_ADC_RANGE * (ADC_BUTTONS_VALUE_SCALE) * r / (r + ADC_BUTTONS_R_PULLUP))

  static constexpr uint16_t adc_button_tolerance = HAL_ADC_RANGE *   25 / 1024,
                                adc_other_button = HAL_ADC_RANGE * 1000 / 1024;
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
      const uint16_t currentkpADCValue = thermalManager.current_ADCKey_raw;
      thermalManager.current_ADCKey_raw = HAL_ADC_RANGE;
      thermalManager.ADCKey_count = 0;
      if (currentkpADCValue < adc_other_button)
        LOOP_L_N(i, ADC_KEY_NUM) {
          const uint16_t lo = pgm_read_word(&stADCKeyTable[i].ADCKeyValueMin),
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

          #if BUTTON_EXISTS(EN1)
            if (BUTTON_PRESSED(EN1)) newbutton |= EN_A;
          #endif
          #if BUTTON_EXISTS(EN2)
            if (BUTTON_PRESSED(EN2)) newbutton |= EN_B;
          #endif
          #if BUTTON_EXISTS(ENC)
            if (BUTTON_PRESSED(ENC)) newbutton |= EN_C;
          #endif
          #if BUTTON_EXISTS(BACK)
            if (BUTTON_PRESSED(BACK)) newbutton |= EN_D;
          #endif

        #else

          constexpr uint8_t newbutton = 0;

        #endif

        //
        // Directional buttons
        //
        #if ANY_BUTTON(UP, DWN, LFT, RT)

          const int8_t pulses = (ENCODER_PULSES_PER_STEP) * encoderDirection;

          if (false) {
            // for the else-ifs below
          }
          #if BUTTON_EXISTS(UP)
            else if (BUTTON_PRESSED(UP)) {
              encoderDiff = (ENCODER_STEPS_PER_MENU_ITEM) * pulses;
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(DWN)
            else if (BUTTON_PRESSED(DWN)) {
              encoderDiff = -(ENCODER_STEPS_PER_MENU_ITEM) * pulses;
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(LFT)
            else if (BUTTON_PRESSED(LFT)) {
              encoderDiff = -pulses;
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(RT)
            else if (BUTTON_PRESSED(RT)) {
              encoderDiff = pulses;
              next_button_update_ms = now + 300;
            }
          #endif

        #endif // UP || DWN || LFT || RT

        buttons = (newbutton
          #if HAS_SLOW_BUTTONS
            | slow_buttons
          #endif
          #if BOTH(TOUCH_BUTTONS, HAS_ENCODER_ACTION)
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
        WRITE(SHIFT_LD, LOW);
        WRITE(SHIFT_LD, HIGH);
        LOOP_L_N(i, 8) {
          val >>= 1;
          if (READ(SHIFT_OUT)) SBI(val, 7);
          WRITE(SHIFT_CLK, HIGH);
          WRITE(SHIFT_CLK, LOW);
        }
        TERN(REPRAPWORLD_KEYPAD, keypad_buttons, buttons) = ~val;
      #endif

    } // next_button_update_ms

    #if HAS_ENCODER_WHEEL
      static uint8_t lastEncoderBits;

      #define encrot0 0
      #define encrot1 2
      #define encrot2 3
      #define encrot3 1

      // Manage encoder rotation
      #define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: encoderDiff += encoderDirection; break; case _E2: encoderDiff -= encoderDirection; }

      uint8_t enc = 0;
      if (buttons & EN_A) enc |= B01;
      if (buttons & EN_B) enc |= B10;
      if (enc != lastEncoderBits) {
        switch (enc) {
          case encrot0: ENCODER_SPIN(encrot3, encrot1); break;
          case encrot1: ENCODER_SPIN(encrot0, encrot2); break;
          case encrot2: ENCODER_SPIN(encrot1, encrot3); break;
          case encrot3: ENCODER_SPIN(encrot2, encrot0); break;
        }
        if (external_control) {
          TERN_(AUTO_BED_LEVELING_UBL, ubl.encoder_diff = encoderDiff); // Make encoder rotation available to UBL G29 mesh editing.
          encoderDiff = 0;                    // Hide the encoder event from the current screen handler.
        }
        lastEncoderBits = enc;
      }

    #endif // HAS_ENCODER_WHEEL
  }

#endif // HAS_ENCODER_ACTION

#endif // HAS_SPI_LCD

#if HAS_DISPLAY

  #if ENABLED(EXTENSIBLE_UI)
    #include "extui/ui_api.h"
  #endif

  ////////////////////////////////////////////
  /////////////// Status Line ////////////////
  ////////////////////////////////////////////

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

  void MarlinUI::finish_status(const bool persist) {

    #if !(ENABLED(LCD_PROGRESS_BAR) && (PROGRESS_MSG_EXPIRE) > 0)
      UNUSED(persist);
    #endif

    #if ENABLED(LCD_PROGRESS_BAR) || BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
      const millis_t ms = millis();
    #endif

    #if ENABLED(LCD_PROGRESS_BAR)
      progress_bar_ms = ms;
      #if PROGRESS_MSG_EXPIRE > 0
        expire_status_ms = persist ? 0 : ms + PROGRESS_MSG_EXPIRE;
      #endif
    #endif

    #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
      next_filament_display = ms + 5000UL; // Show status message for 5s
    #endif

    #if BOTH(HAS_SPI_LCD, STATUS_MESSAGE_SCROLLING)
      status_scroll_offset = 0;
    #endif

    TERN_(EXTENSIBLE_UI, ExtUI::onStatusChanged(status_message));
  }

  bool MarlinUI::has_status() { return (status_message[0] != '\0'); }

  void MarlinUI::set_status(const char * const message, const bool persist) {
    if (alert_level) return;

    TERN_(HOST_PROMPT_SUPPORT, host_action_notify(message));

    // Here we have a problem. The message is encoded in UTF8, so
    // arbitrarily cutting it will be a problem. We MUST be sure
    // that there is no cutting in the middle of a multibyte character!

    // Get a pointer to the null terminator
    const char* pend = message + strlen(message);

    //  If length of supplied UTF8 string is greater than
    // our buffer size, start cutting whole UTF8 chars
    while ((pend - message) > MAX_MESSAGE_LENGTH) {
      --pend;
      while (!START_OF_UTF8_CHAR(*pend)) --pend;
    };

    // At this point, we have the proper cut point. Use it
    uint8_t maxLen = pend - message;
    strncpy(status_message, message, maxLen);
    status_message[maxLen] = '\0';

    finish_status(persist);
  }

  #include <stdarg.h>

  void MarlinUI::status_printf_P(const uint8_t level, PGM_P const fmt, ...) {
    if (level < alert_level) return;
    alert_level = level;
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(status_message, MAX_MESSAGE_LENGTH, fmt, args);
    va_end(args);
    finish_status(level > 0);
  }

  void MarlinUI::set_status_P(PGM_P const message, int8_t level) {
    if (level < 0) level = alert_level = 0;
    if (level < alert_level) return;
    alert_level = level;

    TERN_(HOST_PROMPT_SUPPORT, host_action_notify_P(message));

    // Since the message is encoded in UTF8 it must
    // only be cut on a character boundary.

    // Get a pointer to the null terminator
    PGM_P pend = message + strlen_P(message);

    // If length of supplied UTF8 string is greater than
    // the buffer size, start cutting whole UTF8 chars
    while ((pend - message) > MAX_MESSAGE_LENGTH) {
      --pend;
      while (!START_OF_UTF8_CHAR(pgm_read_byte(pend))) --pend;
    };

    // At this point, we have the proper cut point. Use it
    uint8_t maxLen = pend - message;
    strncpy_P(status_message, message, maxLen);
    status_message[maxLen] = '\0';

    finish_status(level > 0);
  }

  void MarlinUI::set_alert_status_P(PGM_P const message) {
    set_status_P(message, 1);
    TERN_(HAS_LCD_MENU, return_to_status());
  }

  PGM_P print_paused = GET_TEXT(MSG_PRINT_PAUSED);

  /**
   * Reset the status message
   */
  void MarlinUI::reset_status(const bool no_welcome) {
    PGM_P printing = GET_TEXT(MSG_PRINTING);
    PGM_P welcome  = GET_TEXT(WELCOME_MSG);
    #if SERVICE_INTERVAL_1 > 0
      static PGMSTR(service1, "> " SERVICE_NAME_1 "!");
    #endif
    #if SERVICE_INTERVAL_2 > 0
      static PGMSTR(service2, "> " SERVICE_NAME_2 "!");
    #endif
    #if SERVICE_INTERVAL_3 > 0
      static PGMSTR(service3, "> " SERVICE_NAME_3 "!");
    #endif
    PGM_P msg;
    if (printingIsPaused())
      msg = print_paused;
    #if ENABLED(SDSUPPORT)
      else if (IS_SD_PRINTING())
        return set_status(card.longest_filename(), true);
    #endif
    else if (print_job_timer.isRunning())
      msg = printing;

    #if SERVICE_INTERVAL_1 > 0
      else if (print_job_timer.needsService(1)) msg = service1;
    #endif
    #if SERVICE_INTERVAL_2 > 0
      else if (print_job_timer.needsService(2)) msg = service2;
    #endif
    #if SERVICE_INTERVAL_3 > 0
      else if (print_job_timer.needsService(3)) msg = service3;
    #endif

    else if (!no_welcome)
      msg = welcome;
    else
      return;

    set_status_P(msg, -1);
  }

  #if ENABLED(SDSUPPORT)
    extern bool wait_for_user, wait_for_heatup;
  #endif

  void MarlinUI::abort_print() {
    #if ENABLED(SDSUPPORT)
      wait_for_heatup = wait_for_user = false;
      card.flag.abort_sd_printing = true;
    #endif
    #ifdef ACTION_ON_CANCEL
      host_action_cancel();
    #endif
    TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_INFO, PSTR("UI Aborted"), DISMISS_STR));
    print_job_timer.stop();
    set_status_P(GET_TEXT(MSG_PRINT_ABORTED));
    TERN_(HAS_LCD_MENU, return_to_status());
  }

  #if ANY(PARK_HEAD_ON_PAUSE, SDSUPPORT)
    #include "../gcode/queue.h"
  #endif

  void MarlinUI::pause_print() {
    #if HAS_LCD_MENU
      synchronize(GET_TEXT(MSG_PAUSING));
      defer_status_screen();
    #endif

    TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_PAUSE_RESUME, PSTR("UI Pause"), PSTR("Resume")));

    set_status_P(print_paused);

    #if ENABLED(PARK_HEAD_ON_PAUSE)
      TERN_(HAS_SPI_LCD, lcd_pause_show_message(PAUSE_MESSAGE_PARKING, PAUSE_MODE_PAUSE_PRINT)); // Show message immediately to let user know about pause in progress
      queue.inject_P(PSTR("M25 P\nM24"));
    #elif ENABLED(SDSUPPORT)
      queue.inject_P(PSTR("M25"));
    #elif defined(ACTION_ON_PAUSE)
      host_action_pause();
    #endif
  }

  void MarlinUI::resume_print() {
    reset_status();
    TERN_(PARK_HEAD_ON_PAUSE, wait_for_heatup = wait_for_user = false);
    if (IS_SD_PAUSED()) queue.inject_P(M24_STR);
    #ifdef ACTION_ON_RESUME
      host_action_resume();
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

  #if ENABLED(TOUCH_BUTTONS)

    //
    // Screen Click
    //  - On menu screens move directly to the touched item
    //  - On menu screens, right side (last 3 cols) acts like a scroll - half up => prev page, half down = next page
    //  - On select screens (and others) touch the Right Half for +, Left Half for -
    //  - On edit screens, touch Up Half for -,  Bottom Half to +
    //
    void MarlinUI::screen_click(const uint8_t row, const uint8_t col, const uint8_t, const uint8_t) {
      const int8_t xdir = col < (LCD_WIDTH ) / 2 ? -1 : 1,
                   ydir = row < (LCD_HEIGHT) / 2 ? -1 : 1;
      if (on_edit_screen)
        encoderDiff = ENCODER_PULSES_PER_STEP * ydir;
      else if (screen_items > 0) {
        // Last 3 cols act as a scroll :-)
        if (col > (LCD_WIDTH) - 5)
          // 2 * LCD_HEIGHT to scroll to bottom of next page. (LCD_HEIGHT would only go 1 item down.)
          encoderDiff = ENCODER_PULSES_PER_STEP * (encoderLine - encoderTopLine + 2 * (LCD_HEIGHT)) * ydir;
        else
          encoderDiff = ENCODER_PULSES_PER_STEP * (row - encoderPosition + encoderTopLine);
      }
      else if (!on_status_screen())
        encoderDiff = ENCODER_PULSES_PER_STEP * xdir;
    }

  #endif

#else // !HAS_DISPLAY

  //
  // Send the status line as a host notification
  //
  void MarlinUI::set_status(const char * const message, const bool) {
    TERN(HOST_PROMPT_SUPPORT, host_action_notify(message), UNUSED(message));
  }
  void MarlinUI::set_status_P(PGM_P message, const int8_t) {
    TERN(HOST_PROMPT_SUPPORT, host_action_notify_P(message), UNUSED(message));
  }
  void MarlinUI::status_printf_P(const uint8_t, PGM_P const message, ...) {
    TERN(HOST_PROMPT_SUPPORT, host_action_notify_P(message), UNUSED(message));
  }

#endif // !HAS_DISPLAY

#if ENABLED(SDSUPPORT)

  void MarlinUI::media_changed(const uint8_t old_status, const uint8_t status) {
    if (old_status == status) {
      TERN_(EXTENSIBLE_UI, ExtUI::onMediaError()); // Failed to mount/unmount
      return;
    }

    if (status) {
      if (old_status < 2) {
        TERN_(EXTENSIBLE_UI, ExtUI::onMediaInserted()); // ExtUI response
        set_status_P(GET_TEXT(MSG_MEDIA_INSERTED));
      }
    }
    else {
      if (old_status < 2) {
        TERN_(EXTENSIBLE_UI, ExtUI::onMediaRemoved()); // ExtUI response
        #if PIN_EXISTS(SD_DETECT)
          set_status_P(GET_TEXT(MSG_MEDIA_REMOVED));
          #if HAS_LCD_MENU
            if (!defer_return_to_status) return_to_status();
          #endif
        #endif
      }
    }

    #if PIN_EXISTS(SD_DETECT) && DISABLED(NO_LCD_REINIT)
      init_lcd(); // Revive a noisy shared SPI LCD
    #endif

    refresh();

    #if HAS_SPI_LCD || defined(LED_BACKLIGHT_TIMEOUT)
      const millis_t ms = millis();
    #endif

    TERN_(HAS_SPI_LCD, next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL); // Delay LCD update for SD activity

    #ifdef LED_BACKLIGHT_TIMEOUT
      leds.reset_timeout(ms);
    #endif
  }

#endif // SDSUPPORT

#if HAS_LCD_MENU
  void MarlinUI::reset_settings() { settings.reset(); completion_feedback(); }
#endif

#if ENABLED(EEPROM_SETTINGS)

  #if HAS_LCD_MENU
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
      #if HAS_LCD_MENU
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
        set_status_P(eeprom_err(msgid));
      #endif
    }

  #endif // EEPROM_AUTO_INIT

#endif // EEPROM_SETTINGS
