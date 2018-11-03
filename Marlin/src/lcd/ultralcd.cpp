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

#include "../inc/MarlinConfigPre.h"

#if ENABLED(ULTRA_LCD)

#include <stdarg.h>

#include "ultralcd.h"
#include "lcdprint.h"

#include "../sd/cardreader.h"
#include "../module/temperature.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../module/motion.h"
#include "../module/probe.h"
#include "../module/printcounter.h"
#include "../gcode/gcode.h"
#include "../gcode/queue.h"
#include "../module/configuration_store.h"
#include "../module/tool_change.h"

#include "../Marlin.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../feature/pause.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/power_loss_recovery.h"
  #if HAS_LCD_MENU
    void menu_job_recovery();
  #endif
#endif

#if ENABLED(PRINTCOUNTER) && ENABLED(LCD_INFO_MENU)
  #include "../libs/duration_t.h"
#endif

#if ENABLED(FILAMENT_LCD_DISPLAY)
  #include "../feature/filwidth.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../module/endstops.h"
#endif

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if DISABLED(LCD_USE_I2C_BUZZER)
  #include "../libs/buzzer.h"
#endif

// Buttons
volatile uint8_t buttons;

#if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
  uint8_t lcd_sd_status;
#endif

#if ENABLED(STATUS_MESSAGE_SCROLLING)
  uint8_t status_scroll_offset = 0;
  #if LONG_FILENAME_LENGTH > CHARSIZE * 2 * (LCD_WIDTH)
    #define MAX_MESSAGE_LENGTH LONG_FILENAME_LENGTH
  #else
    #define MAX_MESSAGE_LENGTH CHARSIZE * 2 * (LCD_WIDTH)
  #endif
#else
  #define MAX_MESSAGE_LENGTH CHARSIZE * (LCD_WIDTH)
#endif

char lcd_status_message[MAX_MESSAGE_LENGTH + 1];
uint8_t lcd_status_update_delay = 1, // First update one loop delayed
        lcd_status_message_level;    // Higher level blocks lower level

#if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
  millis_t previous_lcd_status_ms = 0;
#endif

#if HAS_LCD_MENU && ENABLED(SDSUPPORT) && ENABLED(SCROLL_LONG_FILENAMES)
  uint8_t filename_scroll_pos, filename_scroll_max;
#endif

#if ENABLED(LCD_SET_PROGRESS_MANUALLY)
  uint8_t progress_bar_percent;
#endif

millis_t next_button_update_ms;

#if HAS_GRAPHICAL_LCD
  bool drawing_screen, first_page; // = false
#endif

#if ENABLED(ENCODER_RATE_MULTIPLIER)
  bool encoderRateMultiplierEnabled;
#endif

#if ENABLED(REVERSE_MENU_DIRECTION)
  int8_t encoderDirection = 1;
#endif

#if HAS_LCD_MENU
  #include "menu/menu.h"

  screenFunc_t currentScreen = lcd_status_screen;

  // Encoder Handling
  volatile int8_t encoderDiff; // Updated in lcd_buttons_update, added to encoderPosition every LCD update
  uint32_t encoderPosition;
  millis_t lastEncoderMovementMillis = 0;
  bool lcd_clicked, wait_for_unclick;
  float move_menu_scale;

  bool use_click() {
    const bool click = lcd_clicked;
    lcd_clicked = false;
    return click;
  }
#endif

void lcd_init() {

  lcd_implementation_init();

  #if ENABLED(NEWPANEL)
    #if BUTTON_EXISTS(EN1)
      SET_INPUT_PULLUP(BTN_EN1);
    #endif
    #if BUTTON_EXISTS(EN2)
      SET_INPUT_PULLUP(BTN_EN2);
    #endif
    #if BUTTON_EXISTS(ENC)
      SET_INPUT_PULLUP(BTN_ENC);
    #endif

    #if ENABLED(REPRAPWORLD_KEYPAD) && DISABLED(ADC_KEYPAD)
      SET_OUTPUT(SHIFT_CLK);
      OUT_WRITE(SHIFT_LD, HIGH);
      SET_INPUT_PULLUP(SHIFT_OUT);
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

  #else // !NEWPANEL

    #if ENABLED(SR_LCD_2W_NL) // Non latching 2 wire shift register
      SET_OUTPUT(SR_DATA_PIN);
      SET_OUTPUT(SR_CLK_PIN);
    #elif defined(SHIFT_CLK)
      SET_OUTPUT(SHIFT_CLK);
      OUT_WRITE(SHIFT_LD, HIGH);
      OUT_WRITE(SHIFT_EN, LOW);
      SET_INPUT_PULLUP(SHIFT_OUT);
    #endif // SR_LCD_2W_NL

  #endif // !NEWPANEL

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
    lcd_sd_status = 2; // UNKNOWN
  #endif

  #if ENABLED(LCD_HAS_SLOW_BUTTONS)
    slow_buttons = 0;
  #endif

  lcd_buttons_update();

  #if HAS_LCD_MENU
    encoderDiff = 0;
  #endif
}

bool lcd_blink() {
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

#if ENABLED(REPRAPWORLD_KEYPAD)
  volatile uint8_t buttons_reprapworld_keypad;
#endif

#if ENABLED(ADC_KEYPAD)

  inline bool handle_adc_keypad() {
    #define ADC_MIN_KEY_DELAY 100
    if (buttons_reprapworld_keypad) {
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
      if (encoderDirection == -1) { // side effect which signals we are inside a menu
        if      (RRK(EN_REPRAPWORLD_KEYPAD_DOWN))   encoderPosition -= ENCODER_STEPS_PER_MENU_ITEM;
        else if (RRK(EN_REPRAPWORLD_KEYPAD_UP))     encoderPosition += ENCODER_STEPS_PER_MENU_ITEM;
        else if (RRK(EN_REPRAPWORLD_KEYPAD_LEFT))   { menu_action_back();     lcd_quick_feedback(true); }
        else if (RRK(EN_REPRAPWORLD_KEYPAD_RIGHT))  { lcd_return_to_status(); lcd_quick_feedback(true); }
      }
      else if (RRK(EN_REPRAPWORLD_KEYPAD_DOWN))     encoderPosition += ENCODER_PULSES_PER_STEP;
      else if (RRK(EN_REPRAPWORLD_KEYPAD_UP))       encoderPosition -= ENCODER_PULSES_PER_STEP;
      else if (RRK(EN_REPRAPWORLD_KEYPAD_RIGHT))    encoderPosition = 0;

      next_button_update_ms = millis() + ADC_MIN_KEY_DELAY;
      return true;
    }

    return false;
  }

#elif ENABLED(REPRAPWORLD_KEYPAD)

  void lcd_move_x();
  void lcd_move_y();
  void lcd_move_z();

  void _reprapworld_keypad_move(const AxisEnum axis, const int16_t dir) {
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    encoderPosition = dir;
    switch (axis) {
      case X_AXIS: lcd_move_x(); break;
      case Y_AXIS: lcd_move_y(); break;
      case Z_AXIS: lcd_move_z();
      default: break;
    }
  }

  inline void handle_reprapworld_keypad() {

    static uint8_t keypad_debounce = 0;

    if (!RRK( EN_REPRAPWORLD_KEYPAD_F1    | EN_REPRAPWORLD_KEYPAD_F2
            | EN_REPRAPWORLD_KEYPAD_F3    | EN_REPRAPWORLD_KEYPAD_DOWN
            | EN_REPRAPWORLD_KEYPAD_RIGHT | EN_REPRAPWORLD_KEYPAD_MIDDLE
            | EN_REPRAPWORLD_KEYPAD_UP    | EN_REPRAPWORLD_KEYPAD_LEFT )
    ) {
      if (keypad_debounce > 0) keypad_debounce--;
    }
    else if (!keypad_debounce) {
      keypad_debounce = 2;

      if (RRK(EN_REPRAPWORLD_KEYPAD_MIDDLE))  lcd_goto_screen(menu_move);

      #if DISABLED(DELTA) && Z_HOME_DIR == -1
        if (RRK(EN_REPRAPWORLD_KEYPAD_F2))    _reprapworld_keypad_move(Z_AXIS,  1);
      #endif

      if (all_axes_homed()) {
        #if ENABLED(DELTA) || Z_HOME_DIR != -1
          if (RRK(EN_REPRAPWORLD_KEYPAD_F2))  _reprapworld_keypad_move(Z_AXIS,  1);
        #endif
        if (RRK(EN_REPRAPWORLD_KEYPAD_F3))    _reprapworld_keypad_move(Z_AXIS, -1);
        if (RRK(EN_REPRAPWORLD_KEYPAD_LEFT))  _reprapworld_keypad_move(X_AXIS, -1);
        if (RRK(EN_REPRAPWORLD_KEYPAD_RIGHT)) _reprapworld_keypad_move(X_AXIS,  1);
        if (RRK(EN_REPRAPWORLD_KEYPAD_DOWN))  _reprapworld_keypad_move(Y_AXIS,  1);
        if (RRK(EN_REPRAPWORLD_KEYPAD_UP))    _reprapworld_keypad_move(Y_AXIS, -1);
      }
      else if (RRK(EN_REPRAPWORLD_KEYPAD_F1)) enqueue_and_echo_commands_P(PSTR("G28"));
    }
  }

#endif // REPRAPWORLD_KEYPAD

/**
 * Status Screen
 *
 * This is very display-dependent, so the lcd implementation draws this.
 */

#if ENABLED(LCD_PROGRESS_BAR)
  millis_t progress_bar_ms = 0;     // Start millis of the current progress bar cycle
  #if PROGRESS_MSG_EXPIRE > 0
    static millis_t expire_status_ms = 0;
    void dontExpireStatus() { expire_status_ms = 0; }
  #endif
#endif

#if LCD_INFO_SCREEN_STYLE == 0
  void lcd_impl_status_screen_0();
#elif LCD_INFO_SCREEN_STYLE == 1
  void lcd_impl_status_screen_1();
#endif

void lcd_status_screen() {

  #if HAS_LCD_MENU
    ENCODER_DIRECTION_NORMAL();
    ENCODER_RATE_MULTIPLY(false);
  #endif

  #if ENABLED(LCD_SET_PROGRESS_MANUALLY) && ENABLED(SDSUPPORT) && (ENABLED(LCD_PROGRESS_BAR) || HAS_GRAPHICAL_LCD)
    // Progress bar % comes from SD when actively printing
    if (IS_SD_PRINTING())
      progress_bar_percent = card.percentDone();
  #endif

  #if ENABLED(LCD_PROGRESS_BAR)

    //
    // HD44780 implements the following message blinking and
    // message expiration because Status Line and Progress Bar
    // share the same line on the display.
    //

    millis_t ms = millis();

    // If the message will blink rather than expire...
    #if DISABLED(PROGRESS_MSG_ONCE)
      if (ELAPSED(ms, progress_bar_ms + PROGRESS_BAR_MSG_TIME + PROGRESS_BAR_BAR_TIME))
        progress_bar_ms = ms;
    #endif

    #if PROGRESS_MSG_EXPIRE > 0

      // Handle message expire
      if (expire_status_ms > 0) {

        #if DISABLED(LCD_SET_PROGRESS_MANUALLY)
          const uint8_t progress_bar_percent = card.percentDone();
        #endif

        // Expire the message if a job is active and the bar has ticks
        if (progress_bar_percent > 2 && !print_job_timer.isPaused()) {
          if (ELAPSED(ms, expire_status_ms)) {
            lcd_status_message[0] = '\0';
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
      #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
        previous_lcd_status_ms = millis();  // get status message to show up for a while
      #endif
      lcd_goto_screen(menu_main);
      lcd_implementation_init(); // May revive the LCD if static electricity killed it
      return;
    }

    #if ENABLED(ULTIPANEL_FEEDMULTIPLY)
      const int16_t new_frm = feedrate_percentage + (int32_t)encoderPosition;
      // Dead zone at 100% feedrate
      if ((feedrate_percentage < 100 && new_frm > 100) || (feedrate_percentage > 100 && new_frm < 100)) {
        feedrate_percentage = 100;
        encoderPosition = 0;
      }
      else if (feedrate_percentage == 100) {
        if ((int32_t)encoderPosition > ENCODER_FEEDRATE_DEADZONE) {
          feedrate_percentage += (int32_t)encoderPosition - (ENCODER_FEEDRATE_DEADZONE);
          encoderPosition = 0;
        }
        else if ((int32_t)encoderPosition < -(ENCODER_FEEDRATE_DEADZONE)) {
          feedrate_percentage += (int32_t)encoderPosition + ENCODER_FEEDRATE_DEADZONE;
          encoderPosition = 0;
        }
      }
      else {
        feedrate_percentage = new_frm;
        encoderPosition = 0;
      }
    #endif // ULTIPANEL_FEEDMULTIPLY

    feedrate_percentage = constrain(feedrate_percentage, 10, 999);

  #endif // HAS_LCD_MENU

  #if LCD_INFO_SCREEN_STYLE == 0
    lcd_impl_status_screen_0();
  #elif LCD_INFO_SCREEN_STYLE == 1
    lcd_impl_status_screen_1();
  #endif
}

/**
 * Reset the status message
 */
void lcd_reset_status() {
  static const char paused[] PROGMEM = MSG_PRINT_PAUSED;
  static const char printing[] PROGMEM = MSG_PRINTING;
  static const char welcome[] PROGMEM = WELCOME_MSG;
  PGM_P msg;
  if (print_job_timer.isPaused())
    msg = paused;
  #if ENABLED(SDSUPPORT)
    else if (card.sdprinting)
      return lcd_setstatus(card.longest_filename(), true);
  #endif
  else if (print_job_timer.isRunning())
    msg = printing;
  else
    msg = welcome;

  lcd_setstatusPGM(msg, -1);
}

void kill_screen(PGM_P lcd_msg) {
  lcd_init();
  lcd_setalertstatusPGM(lcd_msg);
  lcd_kill_screen();
}

#if HAS_BUZZER
  void lcd_buzz(const long duration, const uint16_t freq) {
    #if ENABLED(LCD_USE_I2C_BUZZER)
      lcd.buzz(duration, freq);
    #elif PIN_EXISTS(BEEPER)
      buzzer.tone(duration, freq);
    #endif
  }
#endif

void lcd_quick_feedback(const bool clear_buttons) {

  #if HAS_LCD_MENU
    lcd_refresh();
    if (clear_buttons) buttons = 0;
    next_button_update_ms = millis() + 500;
  #else
    UNUSED(clear_buttons);
  #endif

  // Buzz and wait. The delay is needed for buttons to settle!
  lcd_buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);

  #if HAS_LCD_MENU
    #if ENABLED(LCD_USE_I2C_BUZZER)
      delay(10);
    #elif PIN_EXISTS(BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #endif
  #endif
}

#if HAS_LCD_MENU

  extern bool no_reentry; // Flag to prevent recursion into menu handlers

  int8_t manual_move_axis = (int8_t)NO_AXIS;
  millis_t manual_move_start_time = 0;

  #if IS_KINEMATIC
    bool processing_manual_move = false;
    float manual_move_offset = 0;
  #endif

  #if !IS_KINEMATIC || (IS_KINEMATIC && EXTRUDERS > 1)
    int8_t manual_move_e_index = 0;
  #else
    constexpr int8_t manual_move_e_index = 0;
  #endif

  /**
   * If the most recent manual move hasn't been fed to the planner yet,
   * and the planner can accept one, send a move immediately.
   */
  void manage_manual_move() {

    if (processing_manual_move) return;

    if (manual_move_axis != (int8_t)NO_AXIS && ELAPSED(millis(), manual_move_start_time) && !planner.is_full()) {

      #if IS_KINEMATIC

        const float old_feedrate = feedrate_mm_s;
        feedrate_mm_s = MMM_TO_MMS(manual_feedrate_mm_m[manual_move_axis]);

        #if EXTRUDERS > 1
          const int8_t old_extruder = active_extruder;
          if (manual_move_axis == E_AXIS) active_extruder = manual_move_e_index;
        #endif

        // Set movement on a single axis
        set_destination_from_current();
        destination[manual_move_axis] += manual_move_offset;

        // Reset for the next move
        manual_move_offset = 0;
        manual_move_axis = (int8_t)NO_AXIS;

        // DELTA and SCARA machines use segmented moves, which could fill the planner during the call to
        // move_to_destination. This will cause idle() to be called, which can then call this function while the
        // previous invocation is being blocked. Modifications to manual_move_offset shouldn't be made while
        // processing_manual_move is true or the planner will get out of sync.
        processing_manual_move = true;
        prepare_move_to_destination(); // will call set_current_from_destination()
        processing_manual_move = false;

        feedrate_mm_s = old_feedrate;
        #if EXTRUDERS > 1
          active_extruder = old_extruder;
        #endif

      #else

        planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[manual_move_axis]), manual_move_axis == E_AXIS ? manual_move_e_index : active_extruder);
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
 *   - Set lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NOW on controller events
 *   - Reset the Info Screen timeout if there's any input
 *   - Update status indicators, if any
 *
 *   Run the current LCD menu handler callback function:
 *   - Call the handler only if lcdDrawUpdate != LCDVIEW_NONE
 *   - Before calling the handler, LCDVIEW_CALL_NO_REDRAW => LCDVIEW_NONE
 *   - Call the menu handler. Menu handlers should do the following:
 *     - If a value changes, set lcdDrawUpdate to LCDVIEW_REDRAW_NOW and draw the value
 *       (Encoder events automatically set lcdDrawUpdate for you.)
 *     - if (lcdDrawUpdate) { redraw }
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

LCDViewAction lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
  bool lcd_external_control; // = false
#endif

#if ENABLED(LCD_HAS_SLOW_BUTTONS)
  volatile uint8_t slow_buttons;
#endif

void lcd_update() {

  static uint16_t max_display_update_time = 0;
  static millis_t next_lcd_update_ms;

  #if HAS_LCD_MENU
    static millis_t return_to_status_ms = 0;

    // Handle any queued Move Axis motion
    manage_manual_move();

    // Update button states for LCD_CLICKED(), etc.
    // After state changes the next button update
    // may be delayed 300-500ms.
    lcd_buttons_update();

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      // Don't run the debouncer if UBL owns the display
      #define UBL_CONDITION !lcd_external_control
    #else
      #define UBL_CONDITION true
    #endif

    // If the action button is pressed...
    if (UBL_CONDITION && LCD_CLICKED()) {
      if (!wait_for_unclick) {           // If not waiting for a debounce release:
        wait_for_unclick = true;         //  Set debounce flag to ignore continous clicks
        lcd_clicked = !wait_for_user && !no_reentry; //  Keep the click if not waiting for a user-click
        wait_for_user = false;           //  Any click clears wait for user
        lcd_quick_feedback(true);        //  Always make a click sound
      }
    }
    else wait_for_unclick = false;

    #if BUTTON_EXISTS(BACK)
      if (LCD_BACK_CLICKED) {
        lcd_quick_feedback(true);
        lcd_goto_previous_menu();
      }
    #endif

  #endif // HAS_LCD_MENU

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)

    const uint8_t sd_status = (uint8_t)IS_SD_INSERTED();
    if (sd_status != lcd_sd_status && lcd_detected()) {

      uint8_t old_sd_status = lcd_sd_status; // prevent re-entry to this block!
      lcd_sd_status = sd_status;

      if (sd_status) {
        safe_delay(500); // Some boards need a delay to get settled
        card.initsd();
        if (old_sd_status == 2)
          card.beginautostart();  // Initial boot
        else
          LCD_MESSAGEPGM(MSG_SD_INSERTED);
      }
      else {
        card.release();
        if (old_sd_status != 2) LCD_MESSAGEPGM(MSG_SD_REMOVED);
      }

      lcd_refresh();
      lcd_implementation_init(); // May revive the LCD if static electricity killed it
    }

  #endif // SDSUPPORT && SD_DETECT_PIN

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (job_recovery_commands_count && job_recovery_phase == JOB_RECOVERY_IDLE) {
      lcd_goto_screen(menu_job_recovery);
      job_recovery_phase = JOB_RECOVERY_MAYBE; // Waiting for a response
    }
  #endif

  const millis_t ms = millis();
  if (ELAPSED(ms, next_lcd_update_ms)
    #if HAS_GRAPHICAL_LCD
      || drawing_screen
    #endif
  ) {

    next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;

    #if ENABLED(LCD_HAS_STATUS_INDICATORS)
      lcd_implementation_update_indicators();
    #endif

    #if HAS_LCD_MENU

      #if ENABLED(LCD_HAS_SLOW_BUTTONS)
        slow_buttons = lcd_implementation_read_slow_buttons(); // buttons which take too long to read in interrupt context
      #endif

      #if ENABLED(ADC_KEYPAD)

        if (handle_adc_keypad())
          return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;

      #elif ENABLED(REPRAPWORLD_KEYPAD)

        handle_reprapworld_keypad();

      #endif

      const bool encoderPastThreshold = (ABS(encoderDiff) >= ENCODER_PULSES_PER_STEP);
      if (encoderPastThreshold || lcd_clicked) {
        if (encoderPastThreshold) {
          int32_t encoderMultiplier = 1;

          #if ENABLED(ENCODER_RATE_MULTIPLIER)

            if (encoderRateMultiplierEnabled) {
              int32_t encoderMovementSteps = ABS(encoderDiff) / ENCODER_PULSES_PER_STEP;

              if (lastEncoderMovementMillis) {
                // Note that the rate is always calculated between two passes through the
                // loop and that the abs of the encoderDiff value is tracked.
                float encoderStepRate = float(encoderMovementSteps) / float(ms - lastEncoderMovementMillis) * 1000;

                if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC)     encoderMultiplier = 100;
                else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC) encoderMultiplier = 10;

                #if ENABLED(ENCODER_RATE_MULTIPLIER_DEBUG)
                  SERIAL_ECHO_START();
                  SERIAL_ECHOPAIR("Enc Step Rate: ", encoderStepRate);
                  SERIAL_ECHOPAIR("  Multiplier: ", encoderMultiplier);
                  SERIAL_ECHOPAIR("  ENCODER_10X_STEPS_PER_SEC: ", ENCODER_10X_STEPS_PER_SEC);
                  SERIAL_ECHOPAIR("  ENCODER_100X_STEPS_PER_SEC: ", ENCODER_100X_STEPS_PER_SEC);
                  SERIAL_EOL();
                #endif // ENCODER_RATE_MULTIPLIER_DEBUG
              }

              lastEncoderMovementMillis = ms;
            } // encoderRateMultiplierEnabled
          #endif // ENCODER_RATE_MULTIPLIER

          encoderPosition += (encoderDiff * encoderMultiplier) / ENCODER_PULSES_PER_STEP;
          encoderDiff = 0;
        }
        return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;
        lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
      }

    #endif // HAS_LCD_MENU

    // This runs every ~100ms when idling often enough.
    // Instead of tracking changes just redraw the Status Screen once per second.
    if (
      #if HAS_LCD_MENU
        currentScreen == lcd_status_screen &&
      #endif
      !lcd_status_update_delay--
    ) {
      lcd_status_update_delay = 9
        #if HAS_GRAPHICAL_LCD
          + 3
        #endif
      ;
      max_display_update_time--;
      lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
    }

    #if HAS_LCD_MENU && ENABLED(SCROLL_LONG_FILENAMES)
      // If scrolling of long file names is enabled and we are in the sd card menu,
      // cause a refresh to occur until all the text has scrolled into view.
      if (currentScreen == menu_sdcard && filename_scroll_pos < filename_scroll_max && !lcd_status_update_delay--) {
        lcd_status_update_delay = 6;
        lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
        filename_scroll_pos++;
        return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;
      }
    #endif

    // then we want to use 1/2 of the time only.
    uint16_t bbr2 = planner.block_buffer_runtime() >> 1;

    #if HAS_GRAPHICAL_LCD
      const bool &is_drawing = drawing_screen;
    #else
      constexpr bool is_drawing = false;
    #endif

    if ((lcdDrawUpdate || is_drawing) && (!bbr2 || bbr2 > max_display_update_time)) {

      // Change state of drawing flag between screen updates
      if (!is_drawing) switch (lcdDrawUpdate) {
        case LCDVIEW_CALL_NO_REDRAW:
          lcdDrawUpdate = LCDVIEW_NONE;
          break;
        case LCDVIEW_CLEAR_CALL_REDRAW:
        case LCDVIEW_CALL_REDRAW_NEXT:
          lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
        case LCDVIEW_REDRAW_NOW:        // set above, or by a handler through LCDVIEW_CALL_REDRAW_NEXT
        case LCDVIEW_NONE:
          break;
      } // switch

      #if ENABLED(ADC_KEYPAD)
        buttons_reprapworld_keypad = 0;
      #endif

      #if HAS_LCD_MENU
        #define CURRENTSCREEN() (*currentScreen)()
      #else
        #define CURRENTSCREEN() lcd_status_screen()
      #endif

      #if HAS_GRAPHICAL_LCD
        #if ENABLED(LIGHTWEIGHT_UI)
          #if HAS_LCD_MENU
            const bool in_status = currentScreen == lcd_status_screen;
          #else
            constexpr bool in_status = true;
          #endif
          const bool do_u8g_loop = !in_status;
          lcd_in_status(in_status);
          if (in_status) lcd_status_screen();
        #else
          constexpr bool do_u8g_loop = true;
        #endif
        if (do_u8g_loop) {
          if (!drawing_screen) {                        // If not already drawing pages
            u8g.firstPage();                            // Start the first page
            drawing_screen = first_page = true;         // Flag as drawing pages
          }
          lcd_setFont(FONT_MENU);                       // Setup font for every page draw
          u8g.setColorIndex(1);                         // And reset the color
          CURRENTSCREEN();                              // Draw and process the current screen
          first_page = false;

          // The screen handler can clear drawing_screen for an action that changes the screen.
          // If still drawing and there's another page, update max-time and return now.
          // The nextPage will already be set up on the next call.
          if (drawing_screen && (drawing_screen = u8g.nextPage())) {
            NOLESS(max_display_update_time, millis() - ms);
            return;
          }
        }
      #else
        CURRENTSCREEN();
      #endif

      #if HAS_LCD_MENU
        lcd_clicked = false;
      #endif

      // Keeping track of the longest time for an individual LCD update.
      // Used to do screen throttling when the planner starts to fill up.
      NOLESS(max_display_update_time, millis() - ms);
    }

    #if HAS_LCD_MENU

      // Return to Status Screen after a timeout
      if (currentScreen == lcd_status_screen || defer_return_to_status)
        return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;
      else if (ELAPSED(ms, return_to_status_ms))
        lcd_return_to_status();

    #endif // HAS_LCD_MENU

    // Change state of drawing flag between screen updates
    if (!is_drawing) switch (lcdDrawUpdate) {
      case LCDVIEW_CLEAR_CALL_REDRAW:
        lcd_implementation_clear(); break;
      case LCDVIEW_REDRAW_NOW:
        lcdDrawUpdate = LCDVIEW_NONE;
      case LCDVIEW_NONE:
      case LCDVIEW_CALL_REDRAW_NEXT:
      case LCDVIEW_CALL_NO_REDRAW:
      default: break;
    } // switch

  } // ELAPSED(ms, next_lcd_update_ms)
}

void lcd_finishstatus(const bool persist=false) {

  #if !(ENABLED(LCD_PROGRESS_BAR) && (PROGRESS_MSG_EXPIRE > 0))
    UNUSED(persist);
  #endif

  #if ENABLED(LCD_PROGRESS_BAR)
    progress_bar_ms = millis();
    #if PROGRESS_MSG_EXPIRE > 0
      expire_status_ms = persist ? 0 : progress_bar_ms + PROGRESS_MSG_EXPIRE;
    #endif
  #endif

  lcd_refresh();

  #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
    previous_lcd_status_ms = millis();  //get status message to show up for a while
  #endif

  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    status_scroll_offset = 0;
  #endif
}

bool lcd_hasstatus() { return (lcd_status_message[0] != '\0'); }

void lcd_setstatus(const char * const message, const bool persist) {
  if (lcd_status_message_level > 0) return;

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
  strncpy(lcd_status_message, message, maxLen);
  lcd_status_message[maxLen] = '\0';

  lcd_finishstatus(persist);
}

void lcd_setstatusPGM(PGM_P const message, int8_t level) {
  if (level < 0) level = lcd_status_message_level = 0;
  if (level < lcd_status_message_level) return;
  lcd_status_message_level = level;

  // Here we have a problem. The message is encoded in UTF8, so
  // arbitrarily cutting it will be a problem. We MUST be sure
  // that there is no cutting in the middle of a multibyte character!

  // Get a pointer to the null terminator
  PGM_P pend = message + strlen_P(message);

  //  If length of supplied UTF8 string is greater than
  // our buffer size, start cutting whole UTF8 chars
  while ((pend - message) > MAX_MESSAGE_LENGTH) {
    --pend;
    while (!START_OF_UTF8_CHAR(pgm_read_byte(pend))) --pend;
  };

  // At this point, we have the proper cut point. Use it
  uint8_t maxLen = pend - message;
  strncpy_P(lcd_status_message, message, maxLen);
  lcd_status_message[maxLen] = '\0';

  lcd_finishstatus(level > 0);
}

void lcd_status_printf_P(const uint8_t level, PGM_P const fmt, ...) {
  if (level < lcd_status_message_level) return;
  lcd_status_message_level = level;
  va_list args;
  va_start(args, fmt);
  vsnprintf_P(lcd_status_message, MAX_MESSAGE_LENGTH, fmt, args);
  va_end(args);
  lcd_finishstatus(level > 0);
}

void lcd_setalertstatusPGM(PGM_P const message) {
  lcd_setstatusPGM(message, 1);
  #if HAS_LCD_MENU
    lcd_return_to_status();
  #endif
}

void lcd_reset_alert_level() { lcd_status_message_level = 0; }

#if ENABLED(ADC_KEYPAD)

  typedef struct {
    uint16_t ADCKeyValueMin, ADCKeyValueMax;
    uint8_t  ADCKeyNo;
  } _stADCKeypadTable_;

  static const _stADCKeypadTable_ stADCKeyTable[] PROGMEM = {
    // VALUE_MIN, VALUE_MAX, KEY
    { 4000, 4096, 1 + BLEN_REPRAPWORLD_KEYPAD_F1     }, // F1
    { 4000, 4096, 1 + BLEN_REPRAPWORLD_KEYPAD_F2     }, // F2
    { 4000, 4096, 1 + BLEN_REPRAPWORLD_KEYPAD_F3     }, // F3
    {  300,  500, 1 + BLEN_REPRAPWORLD_KEYPAD_LEFT   }, // LEFT
    { 1900, 2200, 1 + BLEN_REPRAPWORLD_KEYPAD_RIGHT  }, // RIGHT
    {  570,  870, 1 + BLEN_REPRAPWORLD_KEYPAD_UP     }, // UP
    { 2670, 2870, 1 + BLEN_REPRAPWORLD_KEYPAD_DOWN   }, // DOWN
    { 1150, 1450, 1 + BLEN_REPRAPWORLD_KEYPAD_MIDDLE }, // ENTER
  };

  uint8_t get_ADC_keyValue(void) {
    if (thermalManager.ADCKey_count >= 16) {
      const uint16_t currentkpADCValue = thermalManager.current_ADCKey_raw >> 2;
      thermalManager.current_ADCKey_raw = 0;
      thermalManager.ADCKey_count = 0;
      if (currentkpADCValue < 4000)
        for (uint8_t i = 0; i < ADC_KEY_NUM; i++) {
          const uint16_t lo = pgm_read_word(&stADCKeyTable[i].ADCKeyValueMin),
                         hi = pgm_read_word(&stADCKeyTable[i].ADCKeyValueMax);
          if (WITHIN(currentkpADCValue, lo, hi)) return pgm_read_byte(&stADCKeyTable[i].ADCKeyNo);
        }
    }
    return 0;
  }
#endif

#if HAS_LCD_MENU

  /**
   * Setup Rotary Encoder Bit Values (for two pin encoders to indicate movement)
   * These values are independent of which pins are used for EN_A and EN_B indications
   * The rotary encoder part is also independent to the chipset used for the LCD
   */
  #if defined(EN_A) && defined(EN_B)
    #define encrot0 0
    #define encrot1 2
    #define encrot2 3
    #define encrot3 1
  #endif

  #define GET_SHIFT_BUTTON_STATES(DST) \
    uint8_t new_##DST = 0; \
    WRITE(SHIFT_LD, LOW); \
    WRITE(SHIFT_LD, HIGH); \
    for (int8_t i = 0; i < 8; i++) { \
      new_##DST >>= 1; \
      if (READ(SHIFT_OUT)) SBI(new_##DST, 7); \
      WRITE(SHIFT_CLK, HIGH); \
      WRITE(SHIFT_CLK, LOW); \
    } \
    DST = ~new_##DST; //invert it, because a pressed switch produces a logical 0

  #if (ENABLED(LCD_I2C_TYPE_MCP23017) || ENABLED(LCD_I2C_TYPE_MCP23008)) && ENABLED(DETECT_DEVICE)
    bool lcd_detected() { return lcd.LcdDetected() == 1; }
  #else
    bool lcd_detected() { return true; }
  #endif

  #if ENABLED(G26_MESH_VALIDATION)
    void lcd_chirp() {
      lcd_buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);
    }
  #endif

  #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
    bool is_lcd_clicked() { return LCD_CLICKED(); }
    void wait_for_release() {
      while (is_lcd_clicked()) safe_delay(50);
      safe_delay(50);
    }
  #endif

  /**
   * Read encoder buttons from the hardware registers
   * Warning: This function is called from interrupt context!
   */
  void lcd_buttons_update() {
    static uint8_t lastEncoderBits;
    const millis_t now = millis();
    if (ELAPSED(now, next_button_update_ms)) {

      #if ENABLED(NEWPANEL)
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

        //
        // Directional buttons
        //
        #if LCD_HAS_DIRECTIONAL_BUTTONS

          #if ENABLED(REVERSE_MENU_DIRECTION)
            #define _ENCODER_UD_STEPS (ENCODER_STEPS_PER_MENU_ITEM * encoderDirection)
          #else
            #define _ENCODER_UD_STEPS ENCODER_STEPS_PER_MENU_ITEM
          #endif
          #if ENABLED(REVERSE_ENCODER_DIRECTION)
            #define ENCODER_UD_STEPS _ENCODER_UD_STEPS
            #define ENCODER_LR_PULSES ENCODER_PULSES_PER_STEP
          #else
            #define ENCODER_UD_STEPS -(_ENCODER_UD_STEPS)
            #define ENCODER_LR_PULSES -(ENCODER_PULSES_PER_STEP)
          #endif

          if (false) {
            // for the else-ifs below
          }
          #if BUTTON_EXISTS(UP)
            else if (BUTTON_PRESSED(UP)) {
              encoderDiff = -(ENCODER_UD_STEPS);
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(DWN)
            else if (BUTTON_PRESSED(DWN)) {
              encoderDiff = ENCODER_UD_STEPS;
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(LFT)
            else if (BUTTON_PRESSED(LFT)) {
              encoderDiff = -(ENCODER_LR_PULSES);
              next_button_update_ms = now + 300;
            }
          #endif
          #if BUTTON_EXISTS(RT)
            else if (BUTTON_PRESSED(RT)) {
              encoderDiff = ENCODER_LR_PULSES;
              next_button_update_ms = now + 300;
            }
          #endif

        #endif // LCD_HAS_DIRECTIONAL_BUTTONS

        buttons = newbutton;
        #if ENABLED(LCD_HAS_SLOW_BUTTONS)
          buttons |= slow_buttons;
        #endif

        #if ENABLED(ADC_KEYPAD)

          uint8_t newbutton_reprapworld_keypad = 0;
          buttons = 0;
          if (buttons_reprapworld_keypad == 0) {
            newbutton_reprapworld_keypad = get_ADC_keyValue();
            if (WITHIN(newbutton_reprapworld_keypad, 1, 8))
              buttons_reprapworld_keypad = _BV(newbutton_reprapworld_keypad - 1);
          }

        #elif ENABLED(REPRAPWORLD_KEYPAD)

          GET_SHIFT_BUTTON_STATES(buttons_reprapworld_keypad);

        #endif

      #else // !NEWPANEL

        GET_SHIFT_BUTTON_STATES(buttons);

      #endif

    } // next_button_update_ms

    // Manage encoder rotation
    #if ENABLED(REVERSE_MENU_DIRECTION) && ENABLED(REVERSE_ENCODER_DIRECTION)
      #define ENCODER_DIFF_CW  (encoderDiff -= encoderDirection)
      #define ENCODER_DIFF_CCW (encoderDiff += encoderDirection)
    #elif ENABLED(REVERSE_MENU_DIRECTION)
      #define ENCODER_DIFF_CW  (encoderDiff += encoderDirection)
      #define ENCODER_DIFF_CCW (encoderDiff -= encoderDirection)
    #elif ENABLED(REVERSE_ENCODER_DIRECTION)
      #define ENCODER_DIFF_CW  (encoderDiff--)
      #define ENCODER_DIFF_CCW (encoderDiff++)
    #else
      #define ENCODER_DIFF_CW  (encoderDiff++)
      #define ENCODER_DIFF_CCW (encoderDiff--)
    #endif
    #define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: ENCODER_DIFF_CW; break; case _E2: ENCODER_DIFF_CCW; }

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
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (lcd_external_control) {
          ubl.encoder_diff = encoderDiff;   // Make encoder rotation available to UBL G29 mesh editing.
          encoderDiff = 0;                  // Hide the encoder event from the current screen handler.
        }
      #endif
      lastEncoderBits = enc;
    }
  }

#endif // HAS_LCD_MENU

#endif // ULTRA_LCD
