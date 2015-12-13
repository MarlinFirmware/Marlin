#ifndef ULTRALCD_H
#define ULTRALCD_H

#include "Marlin.h"
#if ENABLED(ULTRA_LCD)
  #include "buzzer.h"

  int lcd_strlen(char* s);
  int lcd_strlen_P(const char* s);
  void lcd_update();
  void lcd_init();
  bool lcd_hasstatus();
  void lcd_setstatus(const char* message, const bool persist=false);
  void lcd_setstatuspgm(const char* message, const uint8_t level=0);
  void lcd_setalertstatuspgm(const char* message);
  void lcd_reset_alert_level();
  bool lcd_detected(void);

  #if ENABLED(LCD_USE_I2C_BUZZER)
    void lcd_buzz(long duration, uint16_t freq);
  #endif

  #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
    void dontExpireStatus();
  #endif

  #if ENABLED(DOGLCD)
    extern int lcd_contrast;
    void lcd_setcontrast(uint8_t value);
  #endif

  #define LCD_MESSAGEPGM(x) lcd_setstatuspgm(PSTR(x))
  #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))

  #define LCD_UPDATE_INTERVAL 100
  #define LCD_TIMEOUT_TO_STATUS 15000

  #if ENABLED(ULTIPANEL)
    void lcd_buttons_update();
    extern volatile uint8_t buttons;  //the last checked buttons in a bit array.
    #if ENABLED(REPRAPWORLD_KEYPAD)
      extern volatile uint8_t buttons_reprapworld_keypad; // to store the keypad shift register values
    #endif
  #else
    FORCE_INLINE void lcd_buttons_update() {}
  #endif

  extern int plaPreheatHotendTemp;
  extern int plaPreheatHPBTemp;
  extern int plaPreheatFanSpeed;
  extern int absPreheatHotendTemp;
  extern int absPreheatHPBTemp;
  extern int absPreheatFanSpeed;

  extern bool cancel_heatup;

  #if ENABLED(FILAMENT_LCD_DISPLAY)
    extern millis_t previous_lcd_status_ms;
  #endif
  void lcd_quick_feedback(); // Audible feedback for a button click - could also be visual
  bool lcd_clicked();

  void lcd_ignore_click(bool b=true);

  #if ENABLED(NEWPANEL)
    #define EN_C BIT(BLEN_C)
    #define EN_B BIT(BLEN_B)
    #define EN_A BIT(BLEN_A)

    #define LCD_CLICKED (buttons&EN_C)
    #if ENABLED(REPRAPWORLD_KEYPAD)
      #define EN_REPRAPWORLD_KEYPAD_F3 (BIT(BLEN_REPRAPWORLD_KEYPAD_F3))
      #define EN_REPRAPWORLD_KEYPAD_F2 (BIT(BLEN_REPRAPWORLD_KEYPAD_F2))
      #define EN_REPRAPWORLD_KEYPAD_F1 (BIT(BLEN_REPRAPWORLD_KEYPAD_F1))
      #define EN_REPRAPWORLD_KEYPAD_UP (BIT(BLEN_REPRAPWORLD_KEYPAD_UP))
      #define EN_REPRAPWORLD_KEYPAD_RIGHT (BIT(BLEN_REPRAPWORLD_KEYPAD_RIGHT))
      #define EN_REPRAPWORLD_KEYPAD_MIDDLE (BIT(BLEN_REPRAPWORLD_KEYPAD_MIDDLE))
      #define EN_REPRAPWORLD_KEYPAD_DOWN (BIT(BLEN_REPRAPWORLD_KEYPAD_DOWN))
      #define EN_REPRAPWORLD_KEYPAD_LEFT (BIT(BLEN_REPRAPWORLD_KEYPAD_LEFT))

      #define LCD_CLICKED ((buttons&EN_C) || (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_F1))
      #define REPRAPWORLD_KEYPAD_MOVE_Z_UP (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_F2)
      #define REPRAPWORLD_KEYPAD_MOVE_Z_DOWN (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_F3)
      #define REPRAPWORLD_KEYPAD_MOVE_X_LEFT (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_LEFT)
      #define REPRAPWORLD_KEYPAD_MOVE_X_RIGHT (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_RIGHT)
      #define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_DOWN)
      #define REPRAPWORLD_KEYPAD_MOVE_Y_UP (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_UP)
      #define REPRAPWORLD_KEYPAD_MOVE_HOME (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_MIDDLE)
    #endif //REPRAPWORLD_KEYPAD
  #else
    //atomic, do not change
    #define B_LE BIT(BL_LE)
    #define B_UP BIT(BL_UP)
    #define B_MI BIT(BL_MI)
    #define B_DW BIT(BL_DW)
    #define B_RI BIT(BL_RI)
    #define B_ST BIT(BL_ST)
    #define EN_B BIT(BLEN_B)
    #define EN_A BIT(BLEN_A)

    #define LCD_CLICKED ((buttons&B_MI)||(buttons&B_ST))
  #endif//NEWPANEL

#else //no LCD
  FORCE_INLINE void lcd_update() {}
  FORCE_INLINE void lcd_init() {}
  FORCE_INLINE bool lcd_hasstatus() { return false; }
  FORCE_INLINE void lcd_setstatus(const char* message, const bool persist=false) {UNUSED(message); UNUSED(persist);}
  FORCE_INLINE void lcd_setstatuspgm(const char* message, const uint8_t level=0) {UNUSED(message); UNUSED(level);}
  FORCE_INLINE void lcd_buttons_update() {}
  FORCE_INLINE void lcd_reset_alert_level() {}
  FORCE_INLINE bool lcd_detected(void) { return true; }

  #define LCD_MESSAGEPGM(x) do{}while(0)
  #define LCD_ALERTMESSAGEPGM(x) do{}while(0)

#endif //ULTRA_LCD

char* itostr2(const uint8_t& x);
char* itostr31(const int& xx);
char* itostr3(const int& xx);
char* itostr3left(const int& xx);
char* itostr4(const int& xx);
char* itostr4sign(const int& x);

char* ftostr3(const float& x);
char* ftostr4sign(const float& x);
char* ftostr31ns(const float& x); // float to string without sign character
char* ftostr31(const float& x);
char* ftostr32(const float& x);
char* ftostr43(const float& x);
char* ftostr12ns(const float& x);
char* ftostr32sp(const float& x); // remove zero-padding from ftostr32
char* ftostr5(const float& x);
char* ftostr51(const float& x);
char* ftostr52(const float& x);

#endif //ULTRALCD_H
