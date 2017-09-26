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

#if ENABLED(ULTRA_LCD)

  #include "../Marlin.h"

  #define BUTTON_EXISTS(BN) (defined(BTN_## BN) && BTN_## BN >= 0)
  #define BUTTON_PRESSED(BN) !READ(BTN_## BN)

  extern int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];

  #if ENABLED(LCD_BED_LEVELING) && ENABLED(PROBE_MANUALLY)
    extern bool lcd_wait_for_move;
  #endif

  int16_t lcd_strlen(const char* s);
  int16_t lcd_strlen_P(const char* s);
  void lcd_update();
  void lcd_init();
  bool lcd_hasstatus();
  void lcd_setstatus(const char* message, const bool persist=false);
  void lcd_setstatusPGM(const char* message, const int8_t level=0);
  void lcd_setalertstatusPGM(const char* message);
  void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...);
  void lcd_reset_alert_level();
  void lcd_kill_screen();
  void kill_screen(const char* lcd_msg);
  bool lcd_detected(void);

  extern uint8_t lcdDrawUpdate;
  inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

  #if HAS_BUZZER
    void lcd_buzz(const long duration, const uint16_t freq);
  #endif

  #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
    void dontExpireStatus();
  #endif

  #if ENABLED(ADC_KEYPAD)
    uint8_t get_ADC_keyValue();
  #endif

  #if ENABLED(DOGLCD)
    extern uint16_t lcd_contrast;
    void set_lcd_contrast(const uint16_t value);
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    void lcd_bootscreen();
  #endif

  #define LCD_UPDATE_INTERVAL 100

  #if ENABLED(ULTIPANEL)

    extern bool defer_return_to_status;

    // Function pointer to menu functions.
    typedef void (*screenFunc_t)();

    void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder=0);

    #define BLEN_A 0
    #define BLEN_B 1
    // Encoder click is directly connected
    #if BUTTON_EXISTS(ENC)
      #define BLEN_C 2
    #endif
    #define EN_A (_BV(BLEN_A))
    #define EN_B (_BV(BLEN_B))
    #define EN_C (_BV(BLEN_C))

    extern volatile uint8_t buttons;  // The last-checked buttons in a bit array.
    void lcd_buttons_update();
    void lcd_quick_feedback();        // Audible feedback for a button click - could also be visual
    void lcd_completion_feedback(const bool good=true);

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      void lcd_advanced_pause_show_message(const AdvancedPauseMessage message);
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      void lcd_mesh_edit_setup(float initial);
      float lcd_mesh_edit();
      void lcd_z_offset_edit_setup(float);
      float lcd_z_offset_edit();
    #endif

    #if ENABLED(DELTA_CALIBRATION_MENU)
      float lcd_probe_pt(const float &lx, const float &ly);
    #endif

  #else

    inline void lcd_buttons_update() {}

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
    #define REPRAPWORLD_KEYPAD_MOVE_MENU    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_F1)
    #define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_DOWN)
    #define REPRAPWORLD_KEYPAD_MOVE_X_RIGHT (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_RIGHT)
    #define REPRAPWORLD_KEYPAD_MOVE_HOME    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_MIDDLE)
    #define REPRAPWORLD_KEYPAD_MOVE_Y_UP    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_UP)
    #define REPRAPWORLD_KEYPAD_MOVE_X_LEFT  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_LEFT)

    #if ENABLED(ADC_KEYPAD)
      #define REPRAPWORLD_KEYPAD_MOVE_HOME  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_F1)
      #define KEYPAD_EN_C                   EN_REPRAPWORLD_KEYPAD_MIDDLE
    #else
      #define REPRAPWORLD_KEYPAD_MOVE_HOME  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_MIDDLE)
      #define KEYPAD_EN_C                   EN_REPRAPWORLD_KEYPAD_F1
    #endif
    #define REPRAPWORLD_KEYPAD_MOVE_MENU    (buttons_reprapworld_keypad & KEYPAD_EN_C)

    #if BUTTON_EXISTS(ENC)
      #define LCD_CLICKED ((buttons & EN_C) || REPRAPWORLD_KEYPAD_MOVE_MENU)
    #else
      #define LCD_CLICKED REPRAPWORLD_KEYPAD_MOVE_MENU
    #endif

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

  #elif ENABLED(NEWPANEL)
    #define LCD_CLICKED (buttons & EN_C)
  #else
    #define LCD_CLICKED false
  #endif

#else // no LCD

  inline void lcd_update() {}
  inline void lcd_init() {}
  inline bool lcd_hasstatus() { return false; }
  inline void lcd_setstatus(const char* const message, const bool persist=false) { UNUSED(message); UNUSED(persist); }
  inline void lcd_setstatusPGM(const char* const message, const int8_t level=0) { UNUSED(message); UNUSED(level); }
  inline void lcd_setalertstatusPGM(const char* message) { UNUSED(message); }
  inline void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) { UNUSED(level); UNUSED(fmt); }
  inline void lcd_buttons_update() {}
  inline void lcd_reset_alert_level() {}
  inline bool lcd_detected() { return true; }
  inline void lcd_refresh() {}

#endif // ULTRA_LCD

#define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

void lcd_reset_status();

// For i2c define BUZZ to use lcd_buzz
#if ENABLED(LCD_USE_I2C_BUZZER)
  #define BUZZ(d,f) lcd_buzz(d, f)
#endif

#if ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY) || ENABLED(ENABLE_MESH_EDIT_GFX_OVERLAY)
    const unsigned char cw_bmp[] PROGMEM = { //AVR-GCC, WinAVR
      0x07,0xf8,0x00, // 000001111111100000000000
      0x0c,0x0c,0x00, // 000011000000110000000000
      0x10,0x02,0x00, // 000100000000001000000000
      0x20,0x01,0x00, // 001000000000000100000000
      0x60,0x01,0x80, // 011000000000000100000000
      0x40,0x00,0x80, // 010000000000000010000000
      0x40,0x03,0xe0, // 010000000000000011100000
      0x40,0x01,0xc0, // 010000000000000011000000
      0x40,0x00,0x80, // 010000000000000010000000
      0x40,0x00,0x00, // 010000000000000000000000
      0x40,0x00,0x00, // 010000000000000000000000
      0x60,0x00,0x00, // 011000000000000000000000
      0x20,0x00,0x00, // 001000000000000000000000
      0x10,0x00,0x00, // 000100000000000000000000
      0x0c,0x0c,0x00, // 000011000000110000000000
      0x07,0xf8,0x00  // 000001111111100000000000
    };

    const unsigned char ccw_bmp[] PROGMEM = { //AVR-GCC, WinAVR
      0x01,0xfe,0x00, // 000000011111111000000000
      0x03,0x03,0x00, // 000000110000001100000000
      0x04,0x00,0x80, // 000001000000000010000000
      0x08,0x00,0x40, // 000010000000000001000000
      0x18,0x00,0x60, // 000110000000000001100000
      0x10,0x00,0x20, // 000100000000000000100000
      0x7c,0x00,0x20, // 011111000000000000100000
      0x38,0x00,0x20, // 001110000000000000100000
      0x10,0x00,0x20, // 000100000000000000100000
      0x00,0x00,0x20, // 000000000000000000100000
      0x00,0x00,0x20, // 000000000000000000100000
      0x00,0x00,0x60, // 000000000000000001100000
      0x00,0x00,0x40, // 000000000000000001000000
      0x00,0x00,0x80, // 000000000000000010000000
      0x03,0x03,0x00, // 000000110000001100000000
      0x01,0xfe,0x00  // 000000011111111000000000
    };


    const unsigned char up_arrow_bmp[] PROGMEM = { //AVR-GCC, WinAVR
      0x06,0x00, // 000001100000
      0x0F,0x00, // 000011110000
      0x1F,0x80, // 000111111000
      0x3F,0xC0, // 001111111100
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00  // 000001100000
    };

    const unsigned char down_arrow_bmp[] PROGMEM = { //AVR-GCC, WinAVR
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x06,0x00, // 000001100000
      0x3F,0xC0, // 001111111100
      0x1F,0x80, // 000111111000
      0x0F,0x00, // 000011110000
      0x06,0x00  // 000001100000
    };

    const unsigned char offset_bedline_bmp[] PROGMEM = { //AVR-GCC, WinAVR
      0xFF,0xFF,0xFF // 111111111111111111111111
    };

    const unsigned char nozzle_bmp[] PROGMEM = { //AVR-GCC, WinAVR
      0x7F,0x80, // 0111111110000000
      0xFF,0xC0, // 1111111111000000
      0xFF,0xC0, // 1111111111000000
      0xFF,0xC0, // 1111111111000000
      0x7F,0x80, // 0111111110000000
      0x7F,0x80, // 0111111110000000
      0xFF,0xC0, // 1111111111000000
      0xFF,0xC0, // 1111111111000000
      0xFF,0xC0, // 1111111111000000
      0x3F,0x00, // 0011111100000000
      0x1E,0x00, // 0001111000000000
      0x0C,0x00  // 0000110000000000
    };
#endif // BABYSTEP_ZPROBE_GFX_OVERLAY || ENABLE_MESH_EDIT_GFX_OVERLAY

#endif // ULTRALCD_H
