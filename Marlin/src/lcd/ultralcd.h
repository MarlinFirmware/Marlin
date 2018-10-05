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

#ifndef ULTRALCD_H
#define ULTRALCD_H

#include "../inc/MarlinConfig.h"

#if ENABLED(ULTRA_LCD) || ENABLED(MALYAN_LCD)
  void lcd_init();
  bool lcd_detected();
  void lcd_update();
  void lcd_setalertstatusPGM(PGM_P message);
#else
  inline void lcd_init() {}
  inline bool lcd_detected() { return true; }
  inline void lcd_update() {}
  inline void lcd_setalertstatusPGM(PGM_P message) { UNUSED(message); }
#endif

#if ENABLED(ULTRA_LCD)

  #include "../Marlin.h"

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #include "../feature/pause.h"
    #include "../module/motion.h" // for active_extruder
  #endif

  void lcd_return_to_status();
  bool lcd_hasstatus();
  void lcd_setstatus(const char* message, const bool persist=false);
  void lcd_setstatusPGM(PGM_P message, const int8_t level=0);
  void lcd_setalertstatusPGM(PGM_P message);
  void lcd_reset_alert_level();
  void lcd_reset_status();
  void lcd_status_printf_P(const uint8_t level, PGM_P const fmt, ...);
  void lcd_kill_screen();
  void kill_screen(PGM_P lcd_msg);

  enum LCDViewAction : uint8_t {
    LCDVIEW_NONE,
    LCDVIEW_REDRAW_NOW,
    LCDVIEW_CALL_REDRAW_NEXT,
    LCDVIEW_CLEAR_CALL_REDRAW,
    LCDVIEW_CALL_NO_REDRAW
  };

  extern LCDViewAction lcdDrawUpdate;
  inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

  #if HAS_BUZZER
    void lcd_buzz(const long duration, const uint16_t freq);
  #endif

  void lcd_quick_feedback(const bool clear_buttons); // Audible feedback for a button click - could also be visual

  #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
    void dontExpireStatus();
  #endif

  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    extern uint8_t progress_bar_percent;
  #endif

  #if ENABLED(ADC_KEYPAD)
    uint8_t get_ADC_keyValue();
  #endif

  #if ENABLED(DOGLCD)
    extern int16_t lcd_contrast;
    void set_lcd_contrast(const int16_t value);
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    void lcd_bootscreen();
  #endif

  #define LCD_UPDATE_INTERVAL 100
  #define BUTTON_EXISTS(BN) (defined(BTN_## BN) && BTN_## BN >= 0)
  #define BUTTON_PRESSED(BN) !READ(BTN_## BN)

  #if ENABLED(ULTIPANEL) // LCD with a click-wheel input

    extern bool defer_return_to_status;

    // Function pointer to menu functions.
    typedef void (*screenFunc_t)();
    typedef void (*menuAction_t)();

    extern int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];

    #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
      extern bool lcd_external_control;
    #else
      constexpr bool lcd_external_control = false;
    #endif

    #if ENABLED(LCD_BED_LEVELING)
      extern bool lcd_wait_for_move;
    #else
      constexpr bool lcd_wait_for_move = false;
    #endif

    void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder=0);

    void lcd_completion_feedback(const bool good=true);

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      void lcd_advanced_pause_show_message(const AdvancedPauseMessage message,
                                           const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT,
                                           const uint8_t extruder=active_extruder);
    #endif

    #if ENABLED(G26_MESH_VALIDATION)
      void lcd_chirp();
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      void lcd_mesh_edit_setup(const float &initial);
      float lcd_mesh_edit();
      void lcd_z_offset_edit_setup(const float &initial);
      float lcd_z_offset_edit();
    #endif

  #endif

  #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
    extern millis_t previous_lcd_status_ms;
  #endif

  bool lcd_blink();

  #if ENABLED(REPRAPWORLD_KEYPAD) // is also ULTIPANEL and NEWPANEL

    #define REPRAPWORLD_BTN_OFFSET 0 // bit offset into buttons for shift register values

    #define BLEN_REPRAPWORLD_KEYPAD_F3     0
    #define BLEN_REPRAPWORLD_KEYPAD_F2     1
    #define BLEN_REPRAPWORLD_KEYPAD_F1     2
    #define BLEN_REPRAPWORLD_KEYPAD_DOWN   3
    #define BLEN_REPRAPWORLD_KEYPAD_RIGHT  4
    #define BLEN_REPRAPWORLD_KEYPAD_MIDDLE 5
    #define BLEN_REPRAPWORLD_KEYPAD_UP     6
    #define BLEN_REPRAPWORLD_KEYPAD_LEFT   7

    #define EN_REPRAPWORLD_KEYPAD_F3      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F3))
    #define EN_REPRAPWORLD_KEYPAD_F2      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F2))
    #define EN_REPRAPWORLD_KEYPAD_F1      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F1))
    #define EN_REPRAPWORLD_KEYPAD_DOWN    (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_DOWN))
    #define EN_REPRAPWORLD_KEYPAD_RIGHT   (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_RIGHT))
    #define EN_REPRAPWORLD_KEYPAD_MIDDLE  (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_MIDDLE))
    #define EN_REPRAPWORLD_KEYPAD_UP      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_UP))
    #define EN_REPRAPWORLD_KEYPAD_LEFT    (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_LEFT))

    #define REPRAPWORLD_KEYPAD_MOVE_Z_DOWN  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_F3)
    #define REPRAPWORLD_KEYPAD_MOVE_Z_UP    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_F2)
    #define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_DOWN)
    #define REPRAPWORLD_KEYPAD_MOVE_X_RIGHT (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_RIGHT)
    #define REPRAPWORLD_KEYPAD_MOVE_Y_UP    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_UP)
    #define REPRAPWORLD_KEYPAD_MOVE_X_LEFT  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_LEFT)

    #if ENABLED(ADC_KEYPAD)
      #define KEYPAD_HOME EN_REPRAPWORLD_KEYPAD_F1
      #define KEYPAD_EN_C EN_REPRAPWORLD_KEYPAD_MIDDLE
    #else
      #define KEYPAD_HOME EN_REPRAPWORLD_KEYPAD_MIDDLE
      #define KEYPAD_EN_C EN_REPRAPWORLD_KEYPAD_F1
    #endif
    #define REPRAPWORLD_KEYPAD_MOVE_HOME    (buttons_reprapworld_keypad & KEYPAD_HOME)
    #define REPRAPWORLD_KEYPAD_MOVE_MENU    (buttons_reprapworld_keypad & KEYPAD_EN_C)

    #define REPRAPWORLD_KEYPAD_PRESSED      (buttons_reprapworld_keypad & ( \
                                              EN_REPRAPWORLD_KEYPAD_F3 | \
                                              EN_REPRAPWORLD_KEYPAD_F2 | \
                                              EN_REPRAPWORLD_KEYPAD_F1 | \
                                              EN_REPRAPWORLD_KEYPAD_DOWN | \
                                              EN_REPRAPWORLD_KEYPAD_RIGHT | \
                                              EN_REPRAPWORLD_KEYPAD_MIDDLE | \
                                              EN_REPRAPWORLD_KEYPAD_UP | \
                                              EN_REPRAPWORLD_KEYPAD_LEFT) \
                                            )

  #endif

  #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
    bool is_lcd_clicked();
    void wait_for_release();
  #endif

#else // MALYAN_LCD or no LCD

  constexpr bool lcd_wait_for_move = false;

  inline void lcd_refresh() {}
  inline bool lcd_hasstatus() { return false; }
  inline void lcd_setstatus(const char* const message, const bool persist=false) { UNUSED(message); UNUSED(persist); }
  inline void lcd_setstatusPGM(PGM_P const message, const int8_t level=0) { UNUSED(message); UNUSED(level); }
  inline void lcd_status_printf_P(const uint8_t level, PGM_P const fmt, ...) { UNUSED(level); UNUSED(fmt); }
  inline void lcd_reset_alert_level() {}
  inline void lcd_reset_status() {}

#endif // ULTRA_LCD

#if ENABLED(ULTIPANEL)

  #if ENABLED(NEWPANEL) // Uses digital switches, not a shift register

    // Wheel spin pins where BA is 00, 10, 11, 01 (1 bit always changes)
    #define BLEN_A 0
    #define BLEN_B 1

    #define EN_A _BV(BLEN_A)
    #define EN_B _BV(BLEN_B)

    #if BUTTON_EXISTS(ENC)
      #define BLEN_C 2
      #define EN_C _BV(BLEN_C)
    #endif

    #if BUTTON_EXISTS(BACK)
      #define BLEN_D 3
      #define EN_D _BV(BLEN_D)
      #define LCD_BACK_CLICKED (buttons & EN_D)
    #endif

  #endif // NEWPANEL

  extern volatile uint8_t buttons;  // The last-checked buttons in a bit array.
  void lcd_buttons_update();

#else

  inline void lcd_buttons_update() {}

#endif

#if ENABLED(REPRAPWORLD_KEYPAD)
  #ifdef EN_C
    #define LCD_CLICKED ((buttons & EN_C) || REPRAPWORLD_KEYPAD_MOVE_MENU)
  #else
    #define LCD_CLICKED REPRAPWORLD_KEYPAD_MOVE_MENU
  #endif
#elif defined(EN_C)
  #define LCD_CLICKED (buttons & EN_C)
#else
  #define LCD_CLICKED false
#endif

#define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

// For i2c define BUZZ to use lcd_buzz
#if ENABLED(LCD_USE_I2C_BUZZER)
  #define BUZZ(d,f) lcd_buzz(d, f)
#endif

#if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
  void lcd_reselect_last_file();
#endif

#if ENABLED(ULTIPANEL) && ENABLED(SDSUPPORT)
  extern bool abort_sd_printing;
#else
  constexpr bool abort_sd_printing = false;
#endif

#endif // ULTRALCD_H
