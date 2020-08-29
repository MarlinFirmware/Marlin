// extui_TL_TFT_Display.h

#pragma once

//#include "src/core/millis_t.h"

// IGHMC, taken from ZYF Marlin.h and placed here because I didn't find anywhere better yet
// this is initial configuration elements for the Tenlog screen interface which is a Serial port connected
// 'intelligent' controller, it has its own menu display/processing which is not open source
//
//#define TL_TFT_DISPLAY
#ifdef TL_TFT_DISPLAY
//#pragma message(__FILE__)

#include "src/inc/MarlinConfig.h"
#include "src/MarlinCore.h"
//#include "src/lcd/lcdprint.h"
#if ENABLED(SDSUPPORT)
  #include "src/sd/cardreader.h"
#endif


// ******************* Externals **********************  // IGHMC: unclear where to define these
//int  dual_x_carriage_mode;
//bool wait_for_user, wait_for_heatup;

namespace ExtUI {

  bool MSerial2_available();
  char MSerial2_read();
  void Init_TenlogScreen();
  void TL_mainmenu_load();
  void TL_get_command_1();
  uint8_t TL_sdprinting();
  void TL_printdone(const char *message);

  bool TL_parameter_update();
  void TenlogScreen_begin(unsigned long int);
  void TenlogScreen_println(const char s[]);
  void TenlogScreen_print(const char s[]);
  void TL_status_timer();

  void TenlogScreen_printend();
  void TenlogScreen_printEmptyend();
  void Tenlog_status_screen();

  void abort_print();
  void onUserConfirmRequired(const char* const msg);
  void onPrintTimerStarted(); // IGHMC todo: implementation

  void sdcard_tlcontroller();

  void TenlogScreen_hack(uint8_t);

  //void TL_screen_commands();   // first attempt commented out
}

// IGHMC no buzzer, LCD menu, adc__buttons, buttons, newpanel, encoder, spi_lcd, reprapworlkeypad, i2c keypad, graphical lcd, charset

////////////////////////////////////////////
//////////// MarlinUI Singleton ////////////
////////////////////////////////////////////

class MarlinUI {
public:

  MarlinUI() {
    #if HAS_LCD_MENU
    currentScreen = status_screen;
    #endif
  }
  #if ENABLED(LCD_HAS_STATUS_INDICATORS)             // IGHMC don't have staus indicators but do need a regular status update
    static void update_indicators();
  #endif

  static void init();   // IGHMC  implementation in ui_api.cpp
  static void init_post();
  static void update(); // IGHMC  implementation in ui_api.cpp

  static bool detected() { return true; }
  static void refresh() {}
  static void return_to_status() {}
  static void set_alert_status_P(PGM_P const message);
  static void reset_status();
  // static inline void reset_alert_level() {}

  // LCD implementations
  static void finish_status(bool);
  static void clear_lcd(); // IGHMC probably not needed
  static void init_lcd();
  static void set_alert_status_P(const char*,const int8_t); // IGHMC todo: may need implementation of lcd_reset_alert_level
  static bool has_status();
  static inline void reset_alert_level() { alert_level = 0; }
  // IGHMC todo: may need implementation of lcd_reset_alert_level
  // IGHMC, no status message scrolling

  static void abort_print();    // IGHMC todo: implementation
  static void pause_print();    // IGHMC todo: implementation
  static void resume_print();   // IGHMC todo: implementation
  static void update_buttons() {}
  static constexpr uint8_t get_progress_percent() { return 0; }
  static void set_status(const char* message, const bool = false);
  static void set_status_P(PGM_P const message, const int8_t level = 0);
  static void status_printf_P(const uint8_t, PGM_P message, ...);
  static void kill_screen(PGM_P const, PGM_P const);
  static char status_message[];
  //bool has_status() { return (status_message[0] != '\0'); }
  static uint8_t alert_level; // Higher levels block lower levels // IGHMC
                              // todo: alert levels
  #if ENABLED(SDSUPPORT)
    static void media_changed(const uint8_t old_stat, const uint8_t stat);
  #endif
#if ENABLED(EEPROM_SETTINGS)
    static void init_eeprom();
    static void load_settings();
    static void store_settings();
#if DISABLED(EEPROM_AUTO_INIT)
    static void eeprom_alert(const uint8_t msgid);
    static inline void eeprom_alert_crc() { eeprom_alert(0); }
    static inline void eeprom_alert_index() { eeprom_alert(1); }
    static inline void eeprom_alert_version() { eeprom_alert(2); }
#endif
#endif
    // IGHMC, todo: these are definitely not needed but left in case of comaptibility
    // static constexpr bool wait_for_move = false;
    // static constexpr bool external_control = false;

private:

  // static void _synchronize();     - IGHMC, something to do with menus, unlikely to be needed
  // static constexpr bool defer_return_to_status = false;
  // static void draw_status_screen();

};

extern MarlinUI ui;

#define LCD_MESSAGEPGM_P(x)      ui.set_status_P(x)
#define LCD_ALERTMESSAGEPGM_P(x) ui.set_alert_status_P(x)

#define LCD_MESSAGEPGM(x)        LCD_MESSAGEPGM_P(GET_TEXT(x))
#define LCD_ALERTMESSAGEPGM(x)   LCD_ALERTMESSAGEPGM_P(GET_TEXT(x))

#endif // TL_TFT_DISPLAY
