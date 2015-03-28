#include "ultralcd.h"
#ifdef ULTRA_LCD
#include "Marlin.h"
#include "language.h"
#include "cardreader.h"
#include "temperature.h"
#include "stepper.h"
#include "ConfigurationStore.h"

int8_t encoderDiff; /* encoderDiff is updated from interrupt context and added to encoderPosition every LCD update */

bool encoderRateMultiplierEnabled;
int32_t lastEncoderMovementMillis;

/* Configuration settings */
int plaPreheatHotendTemp;
int plaPreheatHPBTemp;
int plaPreheatFanSpeed;

int absPreheatHotendTemp;
int absPreheatHPBTemp;
int absPreheatFanSpeed;

#ifdef FILAMENT_LCD_DISPLAY
  unsigned long message_millis = 0;
#endif

/* !Configuration settings */

//Function pointer to menu functions.
typedef void (*menuFunc_t)();

uint8_t lcd_status_message_level;
char lcd_status_message[LCD_WIDTH+1] = WELCOME_MSG;

#ifdef DOGLCD
  #include "dogm_lcd_implementation.h"
#else
  #include "ultralcd_implementation_hitachi_HD44780.h"
#endif

// The main status screen
static void lcd_status_screen();

#ifdef ULTIPANEL

  extern bool powersupply;
  static float manual_feedrate[] = MANUAL_FEEDRATE;
  static void lcd_main_menu();
  static void lcd_tune_menu();
  static void lcd_prepare_menu();
  static void lcd_move_menu();
  static void lcd_control_menu();
  static void lcd_control_temperature_menu();
  static void lcd_control_temperature_preheat_pla_settings_menu();
  static void lcd_control_temperature_preheat_abs_settings_menu();
  static void lcd_control_motion_menu();
  static void lcd_control_volumetric_menu();
  #ifdef DOGLCD
    static void lcd_set_contrast();
  #endif
  #ifdef FWRETRACT
    static void lcd_control_retract_menu();
  #endif
  static void lcd_sdcard_menu();

  #ifdef DELTA_CALIBRATION_MENU
    static void lcd_delta_calibrate_menu();
  #endif

  #if defined(MANUAL_BED_LEVELING)
    #include "mesh_bed_leveling.h"
    static void _lcd_level_bed();
    static void _lcd_level_bed_homing();
    static void lcd_level_bed();
  #endif

  static void lcd_quick_feedback();//Cause an LCD refresh, and give the user visual or audible feedback that something has happened

  /* Different types of actions that can be used in menu items. */
  static void menu_action_back(menuFunc_t data);
  static void menu_action_submenu(menuFunc_t data);
  static void menu_action_gcode(const char* pgcode);
  static void menu_action_function(menuFunc_t data);
  static void menu_action_sdfile(const char* filename, char* longFilename);
  static void menu_action_sddirectory(const char* filename, char* longFilename);
  static void menu_action_setting_edit_bool(const char* pstr, bool* ptr);
  static void menu_action_setting_edit_int3(const char* pstr, int* ptr, int minValue, int maxValue);
  static void menu_action_setting_edit_float3(const char* pstr, float* ptr, float minValue, float maxValue);
  static void menu_action_setting_edit_float32(const char* pstr, float* ptr, float minValue, float maxValue);
  static void menu_action_setting_edit_float43(const char* pstr, float* ptr, float minValue, float maxValue);
  static void menu_action_setting_edit_float5(const char* pstr, float* ptr, float minValue, float maxValue);
  static void menu_action_setting_edit_float51(const char* pstr, float* ptr, float minValue, float maxValue);
  static void menu_action_setting_edit_float52(const char* pstr, float* ptr, float minValue, float maxValue);
  static void menu_action_setting_edit_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue);
  static void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_int3(const char* pstr, int* ptr, int minValue, int maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_float3(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_float32(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_float43(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_float5(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_float51(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_float52(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
  static void menu_action_setting_edit_callback_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue, menuFunc_t callbackFunc);

  #define ENCODER_FEEDRATE_DEADZONE 10

  #if !defined(LCD_I2C_VIKI)
    #ifndef ENCODER_STEPS_PER_MENU_ITEM
      #define ENCODER_STEPS_PER_MENU_ITEM 5
    #endif
    #ifndef ENCODER_PULSES_PER_STEP
      #define ENCODER_PULSES_PER_STEP 1
    #endif
  #else
    #ifndef ENCODER_STEPS_PER_MENU_ITEM
      #define ENCODER_STEPS_PER_MENU_ITEM 2 // VIKI LCD rotary encoder uses a different number of steps per rotation
    #endif
    #ifndef ENCODER_PULSES_PER_STEP
      #define ENCODER_PULSES_PER_STEP 1
    #endif
  #endif


  /* Helper macros for menus */

  /**
   * START_MENU generates the init code for a menu function
   */
  #define START_MENU() do { \
    encoderRateMultiplierEnabled = false; \
    if (encoderPosition > 0x8000) encoderPosition = 0; \
    uint8_t encoderLine = encoderPosition / ENCODER_STEPS_PER_MENU_ITEM; \
    if (encoderLine < currentMenuViewOffset) currentMenuViewOffset = encoderLine; \
    uint8_t _lineNr = currentMenuViewOffset, _menuItemNr; \
    bool wasClicked = LCD_CLICKED, itemSelected; \
    if (wasClicked) lcd_quick_feedback(); \
    for (uint8_t _drawLineNr = 0; _drawLineNr < LCD_HEIGHT; _drawLineNr++, _lineNr++) { \
      _menuItemNr = 0;

  /**
   * MENU_ITEM generates draw & handler code for a menu item, potentially calling:
   *
   *   lcd_implementation_drawmenu_[type](sel, row, label, arg3...)
   *   menu_action_[type](arg3...)
   *
   * Examples:
   *   MENU_ITEM(back, MSG_WATCH, lcd_status_screen)
   *     lcd_implementation_drawmenu_back(sel, row, PSTR(MSG_WATCH), lcd_status_screen)
   *     menu_action_back(lcd_status_screen)
   *
   *   MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause)
   *     lcd_implementation_drawmenu_function(sel, row, PSTR(MSG_PAUSE_PRINT), lcd_sdcard_pause)
   *     menu_action_function(lcd_sdcard_pause)
   *
   *   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedmultiply, 10, 999)
   *   MENU_ITEM(setting_edit_int3, MSG_SPEED, PSTR(MSG_SPEED), &feedmultiply, 10, 999)
   *     lcd_implementation_drawmenu_setting_edit_int3(sel, row, PSTR(MSG_SPEED), PSTR(MSG_SPEED), &feedmultiply, 10, 999)
   *     menu_action_setting_edit_int3(PSTR(MSG_SPEED), &feedmultiply, 10, 999)
   *
   */
  #define MENU_ITEM(type, label, args...) do { \
    if (_menuItemNr == _lineNr) { \
      itemSelected = encoderLine == _menuItemNr; \
      if (lcdDrawUpdate) \
        lcd_implementation_drawmenu_ ## type(itemSelected, _drawLineNr, PSTR(label), ## args); \
      if (wasClicked && itemSelected) { \
        menu_action_ ## type(args); \
        return; \
      } \
    } \
    _menuItemNr++; \
  } while(0)

  #ifdef ENCODER_RATE_MULTIPLIER
    /**
     * MENU_MULTIPLIER_ITEM generates drawing and handling code for a multiplier menu item
     */
    #define MENU_MULTIPLIER_ITEM(type, label, args...) do { \
      if (_menuItemNr == _lineNr) { \
        itemSelected = encoderLine == _menuItemNr; \
        if (lcdDrawUpdate) \
          lcd_implementation_drawmenu_ ## type(itemSelected, _drawLineNr, PSTR(label), ## args); \
        if (wasClicked && itemSelected) { \
          encoderRateMultiplierEnabled = true; \
          lastEncoderMovementMillis = 0; \
          menu_action_ ## type(args); \
          return; \
        } \
      } \
      _menuItemNr++; \
    } while(0)
  #endif //ENCODER_RATE_MULTIPLIER

  #define MENU_ITEM_DUMMY() do { _menuItemNr++; } while(0)
  #define MENU_ITEM_EDIT(type, label, args...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label), ## args)
  #define MENU_ITEM_EDIT_CALLBACK(type, label, args...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## args)
  #ifdef ENCODER_RATE_MULTIPLIER
    #define MENU_MULTIPLIER_ITEM_EDIT(type, label, args...) MENU_MULTIPLIER_ITEM(setting_edit_ ## type, label, PSTR(label), ## args)
    #define MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(type, label, args...) MENU_MULTIPLIER_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## args)
  #else //!ENCODER_RATE_MULTIPLIER
    #define MENU_MULTIPLIER_ITEM_EDIT(type, label, args...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label), ## args)
    #define MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(type, label, args...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## args)
  #endif //!ENCODER_RATE_MULTIPLIER
  #define END_MENU() \
      if (encoderLine >= _menuItemNr) { encoderPosition = _menuItemNr * ENCODER_STEPS_PER_MENU_ITEM - 1; encoderLine = encoderPosition / ENCODER_STEPS_PER_MENU_ITEM; }\
      if (encoderLine >= currentMenuViewOffset + LCD_HEIGHT) { currentMenuViewOffset = encoderLine - LCD_HEIGHT + 1; lcdDrawUpdate = 1; _lineNr = currentMenuViewOffset - 1; _drawLineNr = -1; } \
      } } while(0)

  /** Used variables to keep track of the menu */
  #ifndef REPRAPWORLD_KEYPAD
    volatile uint8_t buttons; // Bits of the pressed buttons.
  #else
    volatile uint8_t buttons_reprapworld_keypad; // The reprapworld_keypad shift register values
  #endif
  #ifdef LCD_HAS_SLOW_BUTTONS
    volatile uint8_t slow_buttons; // Bits of the pressed buttons.
  #endif
  uint8_t currentMenuViewOffset;              /* scroll offset in the current menu */
  uint32_t blocking_enc;
  uint8_t lastEncoderBits;
  uint32_t encoderPosition;
  #if (SDCARDDETECT > 0)
    bool lcd_oldcardstatus;
  #endif

#endif // ULTIPANEL

menuFunc_t currentMenu = lcd_status_screen; /* function pointer to the currently active menu */
uint32_t lcd_next_update_millis;
uint8_t lcd_status_update_delay;
bool ignore_click = false;
bool wait_for_unclick;
uint8_t lcdDrawUpdate = 2;                  /* Set to none-zero when the LCD needs to draw, decreased after every draw. Set to 2 in LCD routines so the LCD gets at least 1 full redraw (first redraw is partial) */

//prevMenu and prevEncoderPosition are used to store the previous menu location when editing settings.
menuFunc_t prevMenu = NULL;
uint16_t prevEncoderPosition;
//Variables used when editing values.
const char* editLabel;
void* editValue;
int32_t minEditValue, maxEditValue;
menuFunc_t callbackFunc;

// place-holders for Ki and Kd edits
float raw_Ki, raw_Kd;

static void lcd_goto_menu(menuFunc_t menu, const uint32_t encoder=0, const bool feedback=true) {
  if (currentMenu != menu) {
    currentMenu = menu;
    encoderPosition = encoder;
    if (feedback) lcd_quick_feedback();

    // For LCD_PROGRESS_BAR re-initialize the custom characters
    #if defined(LCD_PROGRESS_BAR) && defined(SDSUPPORT) && !defined(DOGLCD)
      lcd_set_custom_characters(menu == lcd_status_screen);
    #endif
  }
}

/* Main status screen. It's up to the implementation specific part to show what is needed. As this is very display dependent */
static void lcd_status_screen()
{
	encoderRateMultiplierEnabled = false;
  #if defined(LCD_PROGRESS_BAR) && defined(SDSUPPORT) && !defined(DOGLCD)
    uint16_t mil = millis();
    #ifndef PROGRESS_MSG_ONCE
      if (mil > progressBarTick + PROGRESS_BAR_MSG_TIME + PROGRESS_BAR_BAR_TIME) {
        progressBarTick = mil;
      }
    #endif
    #if PROGRESS_MSG_EXPIRE > 0
      // keep the message alive if paused, count down otherwise
      if (messageTick > 0) {
        if (card.isFileOpen()) {
          if (IS_SD_PRINTING) {
            if ((mil-messageTick) >= PROGRESS_MSG_EXPIRE) {
              lcd_status_message[0] = '\0';
              messageTick = 0;
            }
          }
          else {
            messageTick += LCD_UPDATE_INTERVAL;
          }
        }
        else {
          messageTick = 0;
        }
      }
    #endif
  #endif //LCD_PROGRESS_BAR

  if (lcd_status_update_delay)
    lcd_status_update_delay--;
  else
    lcdDrawUpdate = 1;

  if (lcdDrawUpdate) {
    lcd_implementation_status_screen();
    lcd_status_update_delay = 10;   /* redraw the main screen every second. This is easier then trying keep track of all things that change on the screen */
  }

#ifdef ULTIPANEL

    bool current_click = LCD_CLICKED;

    if (ignore_click) {
        if (wait_for_unclick) {
          if (!current_click) {
              ignore_click = wait_for_unclick = false;
          }
          else {
              current_click = false;
          }
        }
        else if (current_click) {
            lcd_quick_feedback();
            wait_for_unclick = true;
            current_click = false;
        }
    }

    if (current_click)
    {
        lcd_goto_menu(lcd_main_menu);
        lcd_implementation_init( // to maybe revive the LCD if static electricity killed it.
          #if defined(LCD_PROGRESS_BAR) && defined(SDSUPPORT) && !defined(DOGLCD)
            currentMenu == lcd_status_screen
          #endif
        );
        #ifdef FILAMENT_LCD_DISPLAY
          message_millis = millis();  // get status message to show up for a while
        #endif
    }

#ifdef ULTIPANEL_FEEDMULTIPLY
    // Dead zone at 100% feedrate
    if ((feedmultiply < 100 && (feedmultiply + int(encoderPosition)) > 100) ||
            (feedmultiply > 100 && (feedmultiply + int(encoderPosition)) < 100))
    {
        encoderPosition = 0;
        feedmultiply = 100;
    }

    if (feedmultiply == 100 && int(encoderPosition) > ENCODER_FEEDRATE_DEADZONE)
    {
        feedmultiply += int(encoderPosition) - ENCODER_FEEDRATE_DEADZONE;
        encoderPosition = 0;
    }
    else if (feedmultiply == 100 && int(encoderPosition) < -ENCODER_FEEDRATE_DEADZONE)
    {
        feedmultiply += int(encoderPosition) + ENCODER_FEEDRATE_DEADZONE;
        encoderPosition = 0;
    }
    else if (feedmultiply != 100)
    {
        feedmultiply += int(encoderPosition);
        encoderPosition = 0;
    }
#endif //ULTIPANEL_FEEDMULTIPLY

    if (feedmultiply < 10)
        feedmultiply = 10;
    else if (feedmultiply > 999)
        feedmultiply = 999;
#endif //ULTIPANEL
}

#ifdef ULTIPANEL

static void lcd_return_to_status() { lcd_goto_menu(lcd_status_screen, 0, false); }

static void lcd_sdcard_pause() { card.pauseSDPrint(); }

static void lcd_sdcard_resume() { card.startFileprint(); }

static void lcd_sdcard_stop() {
  quickStop();
  card.sdprinting = false;
  card.closefile();
  autotempShutdown();
  cancel_heatup = true;
  lcd_setstatus(MSG_PRINT_ABORTED);
}

/* Menu implementation */
static void lcd_main_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_WATCH, lcd_status_screen);
  if (movesplanned() || IS_SD_PRINTING) {
    MENU_ITEM(submenu, MSG_TUNE, lcd_tune_menu);
  }
  else {
    MENU_ITEM(submenu, MSG_PREPARE, lcd_prepare_menu);
    #ifdef DELTA_CALIBRATION_MENU
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE, lcd_delta_calibrate_menu);
    #endif
  }
  MENU_ITEM(submenu, MSG_CONTROL, lcd_control_menu);

  #ifdef SDSUPPORT
    if (card.cardOK) {
      if (card.isFileOpen()) {
        if (card.sdprinting)
          MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause);
        else
          MENU_ITEM(function, MSG_RESUME_PRINT, lcd_sdcard_resume);
        MENU_ITEM(function, MSG_STOP_PRINT, lcd_sdcard_stop);
      }
      else {
        MENU_ITEM(submenu, MSG_CARD_MENU, lcd_sdcard_menu);
        #if SDCARDDETECT < 1
          MENU_ITEM(gcode, MSG_CNG_SDCARD, PSTR("M21"));  // SD-card changed by user
        #endif
      }
    }
    else {
      MENU_ITEM(submenu, MSG_NO_CARD, lcd_sdcard_menu);
      #if SDCARDDETECT < 1
        MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually initialize the SD-card via user interface
      #endif
    }
  #endif //SDSUPPORT

  END_MENU();
}

#if defined( SDSUPPORT ) && defined( MENU_ADDAUTOSTART )
  static void lcd_autostart_sd() {
    card.autostart_index = 0;
    card.setroot();
    card.checkautostart(true);
  }
#endif

void lcd_set_home_offsets() {
  for(int8_t i=0; i < NUM_AXIS; i++) {
    if (i != E_AXIS) {
      home_offset[i] -= current_position[i];
      current_position[i] = 0.0;
    }
  }
  plan_set_position(0.0, 0.0, 0.0, current_position[E_AXIS]);

  // Audio feedback
  enquecommands_P(PSTR("M300 S659 P200\nM300 S698 P200"));
  lcd_return_to_status();
}


#ifdef BABYSTEPPING

  static void _lcd_babystep(int axis, const char *msg) {
    if (encoderPosition != 0) {
      babystepsTodo[axis] += (int)encoderPosition;
      encoderPosition = 0;
      lcdDrawUpdate = 1;
    }
    if (lcdDrawUpdate) lcd_implementation_drawedit(msg, "");
    if (LCD_CLICKED) lcd_goto_menu(lcd_tune_menu);
  }
  static void lcd_babystep_x() { _lcd_babystep(X_AXIS, PSTR(MSG_BABYSTEPPING_X)); }
  static void lcd_babystep_y() { _lcd_babystep(Y_AXIS, PSTR(MSG_BABYSTEPPING_Y)); }
  static void lcd_babystep_z() { _lcd_babystep(Z_AXIS, PSTR(MSG_BABYSTEPPING_Z)); }

#endif //BABYSTEPPING

static void lcd_tune_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
  MENU_ITEM_EDIT(int3, MSG_SPEED, &feedmultiply, 10, 999);
  #if TEMP_SENSOR_0 != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE, &target_temperature[0], 0, HEATER_0_MAXTEMP - 15);
  #endif
  #if TEMP_SENSOR_1 != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE MSG_N2, &target_temperature[1], 0, HEATER_1_MAXTEMP - 15);
  #endif
  #if TEMP_SENSOR_2 != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE MSG_N3, &target_temperature[2], 0, HEATER_2_MAXTEMP - 15);
  #endif
  #if TEMP_SENSOR_3 != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE MSG_N4, &target_temperature[3], 0, HEATER_3_MAXTEMP - 15);
  #endif
  #if TEMP_SENSOR_BED != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_BED, &target_temperature_bed, 0, BED_MAXTEMP - 15);
  #endif
  MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_FAN_SPEED, &fanSpeed, 0, 255);
  MENU_ITEM_EDIT(int3, MSG_FLOW, &extrudemultiply, 10, 999);
  MENU_ITEM_EDIT(int3, MSG_FLOW MSG_F0, &extruder_multiply[0], 10, 999);
  #if TEMP_SENSOR_1 != 0
    MENU_ITEM_EDIT(int3, MSG_FLOW MSG_F1, &extruder_multiply[1], 10, 999);
  #endif
  #if TEMP_SENSOR_2 != 0
    MENU_ITEM_EDIT(int3, MSG_FLOW MSG_F2, &extruder_multiply[2], 10, 999);
  #endif
  #if TEMP_SENSOR_3 != 0
    MENU_ITEM_EDIT(int3, MSG_FLOW MSG_F3, &extruder_multiply[3], 10, 999);
  #endif

  #ifdef BABYSTEPPING
    #ifdef BABYSTEP_XY
      MENU_ITEM(submenu, MSG_BABYSTEP_X, lcd_babystep_x);
      MENU_ITEM(submenu, MSG_BABYSTEP_Y, lcd_babystep_y);
    #endif //BABYSTEP_XY
    MENU_ITEM(submenu, MSG_BABYSTEP_Z, lcd_babystep_z);
  #endif
  #ifdef FILAMENTCHANGEENABLE
     MENU_ITEM(gcode, MSG_FILAMENTCHANGE, PSTR("M600"));
  #endif
  END_MENU();
}

void _lcd_preheat(int endnum, const float temph, const float tempb, const int fan) {
  if (temph > 0) setTargetHotend(temph, endnum);
  setTargetBed(tempb);
  fanSpeed = fan;
  lcd_return_to_status();
  setWatch(); // heater sanity check timer
}
void lcd_preheat_pla0() { _lcd_preheat(0, plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed); }
void lcd_preheat_abs0() { _lcd_preheat(0, absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed); }

#if TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_BED != 0 //more than one extruder present

  #if TEMP_SENSOR_1 != 0
    void lcd_preheat_pla1() { _lcd_preheat(1, plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed); }
    void lcd_preheat_abs1() { _lcd_preheat(1, absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed); }
  #endif
  #if TEMP_SENSOR_2 != 0
    void lcd_preheat_pla2() { _lcd_preheat(2, plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed); }
    void lcd_preheat_abs2() { _lcd_preheat(2, absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed); }
  #endif
  #if TEMP_SENSOR_3 != 0
    void lcd_preheat_pla3() { _lcd_preheat(3, plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed); }
    void lcd_preheat_abs3() { _lcd_preheat(3, absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed); }
  #endif

  void lcd_preheat_pla0123() {
    setTargetHotend0(plaPreheatHotendTemp);
    setTargetHotend1(plaPreheatHotendTemp);
    setTargetHotend2(plaPreheatHotendTemp);
    _lcd_preheat(3, plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed);
  }
  void lcd_preheat_abs0123() {
    setTargetHotend0(absPreheatHotendTemp);
    setTargetHotend1(absPreheatHotendTemp);
    setTargetHotend2(absPreheatHotendTemp);
    _lcd_preheat(3, absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed);
  }

  #if TEMP_SENSOR_0 != 0

    void lcd_preheat_pla_bedonly() { _lcd_preheat(0, 0, plaPreheatHPBTemp, plaPreheatFanSpeed); }
    void lcd_preheat_abs_bedonly() { _lcd_preheat(0, 0, absPreheatHPBTemp, absPreheatFanSpeed); }

    static void lcd_preheat_pla_menu() {
      START_MENU();
      MENU_ITEM(back, MSG_PREPARE, lcd_prepare_menu);
      MENU_ITEM(function, MSG_PREHEAT_PLA_N MSG_H1, lcd_preheat_pla0);
      #if TEMP_SENSOR_1 != 0
        MENU_ITEM(function, MSG_PREHEAT_PLA_N MSG_H2, lcd_preheat_pla1);
      #endif
      #if TEMP_SENSOR_2 != 0
        MENU_ITEM(function, MSG_PREHEAT_PLA_N MSG_H3, lcd_preheat_pla2);
      #endif
      #if TEMP_SENSOR_3 != 0
        MENU_ITEM(function, MSG_PREHEAT_PLA_N MSG_H4, lcd_preheat_pla3);
      #endif
      MENU_ITEM(function, MSG_PREHEAT_PLA_ALL, lcd_preheat_pla0123);
      #if TEMP_SENSOR_BED != 0
        MENU_ITEM(function, MSG_PREHEAT_PLA_BEDONLY, lcd_preheat_pla_bedonly);
      #endif
      END_MENU();
    }

    static void lcd_preheat_abs_menu() {
      START_MENU();
      MENU_ITEM(back, MSG_PREPARE, lcd_prepare_menu);
      MENU_ITEM(function, MSG_PREHEAT_ABS_N MSG_H1, lcd_preheat_abs0);
      #if TEMP_SENSOR_1 != 0
        MENU_ITEM(function, MSG_PREHEAT_ABS_N MSG_H2, lcd_preheat_abs1);
      #endif
      #if TEMP_SENSOR_2 != 0
        MENU_ITEM(function, MSG_PREHEAT_ABS_N MSG_H3, lcd_preheat_abs2);
      #endif
      #if TEMP_SENSOR_3 != 0
        MENU_ITEM(function, MSG_PREHEAT_ABS_N MSG_H4, lcd_preheat_abs3);
      #endif
      MENU_ITEM(function, MSG_PREHEAT_ABS_ALL, lcd_preheat_abs0123);
      #if TEMP_SENSOR_BED != 0
        MENU_ITEM(function, MSG_PREHEAT_ABS_BEDONLY, lcd_preheat_abs_bedonly);
      #endif
      END_MENU();
    }
  #endif

#endif // more than one temperature sensor present

void lcd_cooldown() {
  setTargetHotend0(0);
  setTargetHotend1(0);
  setTargetHotend2(0);
  setTargetHotend3(0);
  setTargetBed(0);
  fanSpeed = 0;
  lcd_return_to_status();
}

static void lcd_prepare_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
  #if defined( SDSUPPORT ) && defined( MENU_ADDAUTOSTART )
    MENU_ITEM(function, MSG_AUTOSTART, lcd_autostart_sd);
  #endif
  MENU_ITEM(gcode, MSG_DISABLE_STEPPERS, PSTR("M84"));
  MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
  MENU_ITEM(function, MSG_SET_HOME_OFFSETS, lcd_set_home_offsets);
  //MENU_ITEM(gcode, MSG_SET_ORIGIN, PSTR("G92 X0 Y0 Z0"));
  #if TEMP_SENSOR_0 != 0
    #if TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_BED != 0
      MENU_ITEM(submenu, MSG_PREHEAT_PLA, lcd_preheat_pla_menu);
      MENU_ITEM(submenu, MSG_PREHEAT_ABS, lcd_preheat_abs_menu);
    #else
      MENU_ITEM(function, MSG_PREHEAT_PLA, lcd_preheat_pla0);
      MENU_ITEM(function, MSG_PREHEAT_ABS, lcd_preheat_abs0);
    #endif
  #endif
  MENU_ITEM(function, MSG_COOLDOWN, lcd_cooldown);
  #if defined(POWER_SUPPLY) && POWER_SUPPLY > 0 && defined(PS_ON_PIN) && PS_ON_PIN > -1
    if (powersupply) {
      MENU_ITEM(gcode, MSG_SWITCH_PS_OFF, PSTR("M81"));
    }
    else {
      MENU_ITEM(gcode, MSG_SWITCH_PS_ON, PSTR("M80"));
    }
  #endif
  MENU_ITEM(submenu, MSG_MOVE_AXIS, lcd_move_menu);

  #if defined(MANUAL_BED_LEVELING)
    MENU_ITEM(submenu, MSG_LEVEL_BED, lcd_level_bed);
  #endif
	
  END_MENU();
}

#ifdef DELTA_CALIBRATION_MENU
static void lcd_delta_calibrate_menu()
{
    START_MENU();
    MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
    MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
    MENU_ITEM(gcode, MSG_DELTA_CALIBRATE_X, PSTR("G0 F8000 X-77.94 Y-45 Z0"));
    MENU_ITEM(gcode, MSG_DELTA_CALIBRATE_Y, PSTR("G0 F8000 X77.94 Y-45 Z0"));
    MENU_ITEM(gcode, MSG_DELTA_CALIBRATE_Z, PSTR("G0 F8000 X0 Y90 Z0"));
    MENU_ITEM(gcode, MSG_DELTA_CALIBRATE_CENTER, PSTR("G0 F8000 X0 Y0 Z0"));
    END_MENU();
}
#endif // DELTA_CALIBRATION_MENU

float move_menu_scale;
static void lcd_move_menu_axis();

static void _lcd_move(const char *name, int axis, int min, int max) {
  if (encoderPosition != 0) {
    refresh_cmd_timeout();
    current_position[axis] += float((int)encoderPosition) * move_menu_scale;
    if (min_software_endstops && current_position[axis] < min) current_position[axis] = min;
    if (max_software_endstops && current_position[axis] > max) current_position[axis] = max;
    encoderPosition = 0;
    #ifdef DELTA
      calculate_delta(current_position);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS], manual_feedrate[axis]/60, active_extruder);
    #else
      plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[axis]/60, active_extruder);
    #endif
    lcdDrawUpdate = 1;
  }
  if (lcdDrawUpdate) lcd_implementation_drawedit(name, ftostr31(current_position[axis]));
  if (LCD_CLICKED) lcd_goto_menu(lcd_move_menu_axis);
}
static void lcd_move_x() { _lcd_move(PSTR("X"), X_AXIS, X_MIN_POS, X_MAX_POS); }
static void lcd_move_y() { _lcd_move(PSTR("Y"), Y_AXIS, Y_MIN_POS, Y_MAX_POS); }
static void lcd_move_z() { _lcd_move(PSTR("Z"), Z_AXIS, Z_MIN_POS, Z_MAX_POS); }

static void lcd_move_e() {
  if (encoderPosition != 0) {
    current_position[E_AXIS] += float((int)encoderPosition) * move_menu_scale;
    encoderPosition = 0;
    #ifdef DELTA
      calculate_delta(current_position);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS], manual_feedrate[E_AXIS]/60, active_extruder);
    #else
      plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[E_AXIS]/60, active_extruder);
    #endif
    lcdDrawUpdate = 1;
  }
  if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR("Extruder"), ftostr31(current_position[E_AXIS]));
  if (LCD_CLICKED) lcd_goto_menu(lcd_move_menu_axis);
}

static void lcd_move_menu_axis() {
  START_MENU();
  MENU_ITEM(back, MSG_MOVE_AXIS, lcd_move_menu);
  MENU_ITEM(submenu, MSG_MOVE_X, lcd_move_x);
  MENU_ITEM(submenu, MSG_MOVE_Y, lcd_move_y);
  if (move_menu_scale < 10.0) {
    MENU_ITEM(submenu, MSG_MOVE_Z, lcd_move_z);
    MENU_ITEM(submenu, MSG_MOVE_E, lcd_move_e);
  }
  END_MENU();
}

static void lcd_move_menu_10mm() {
  move_menu_scale = 10.0;
  lcd_move_menu_axis();
}
static void lcd_move_menu_1mm() {
  move_menu_scale = 1.0;
  lcd_move_menu_axis();
}
static void lcd_move_menu_01mm() {
  move_menu_scale = 0.1;
  lcd_move_menu_axis();
}

static void lcd_move_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_PREPARE, lcd_prepare_menu);
  MENU_ITEM(submenu, MSG_MOVE_10MM, lcd_move_menu_10mm);
  MENU_ITEM(submenu, MSG_MOVE_1MM, lcd_move_menu_1mm);
  MENU_ITEM(submenu, MSG_MOVE_01MM, lcd_move_menu_01mm);
  //TODO:X,Y,Z,E
  END_MENU();
}

static void lcd_control_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
  MENU_ITEM(submenu, MSG_TEMPERATURE, lcd_control_temperature_menu);
  MENU_ITEM(submenu, MSG_MOTION, lcd_control_motion_menu);
  MENU_ITEM(submenu, MSG_VOLUMETRIC, lcd_control_volumetric_menu);

  #ifdef DOGLCD
    //MENU_ITEM_EDIT(int3, MSG_CONTRAST, &lcd_contrast, 0, 63);
    MENU_ITEM(submenu, MSG_CONTRAST, lcd_set_contrast);
  #endif
  #ifdef FWRETRACT
    MENU_ITEM(submenu, MSG_RETRACT, lcd_control_retract_menu);
  #endif
  #ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
    MENU_ITEM(function, MSG_LOAD_EPROM, Config_RetrieveSettings);
  #endif
  MENU_ITEM(function, MSG_RESTORE_FAILSAFE, Config_ResetDefault);
  END_MENU();
}

#ifdef PIDTEMP

  // Helpers for editing PID Ki & Kd values
  // grab the PID value out of the temp variable; scale it; then update the PID driver
  void copy_and_scalePID_i(int e) {
    PID_PARAM(Ki, e) = scalePID_i(raw_Ki);
    updatePID();
  }
  void copy_and_scalePID_d(int e) {
    PID_PARAM(Kd, e) = scalePID_d(raw_Kd);
    updatePID();
  }
  void copy_and_scalePID_i_E1() { copy_and_scalePID_i(0); }
  void copy_and_scalePID_d_E1() { copy_and_scalePID_d(0); }
  #ifdef PID_PARAMS_PER_EXTRUDER
    #if EXTRUDERS > 1
      void copy_and_scalePID_i_E2() { copy_and_scalePID_i(1); }
      void copy_and_scalePID_d_E2() { copy_and_scalePID_d(1); }
      #if EXTRUDERS > 2
        void copy_and_scalePID_i_E3() { copy_and_scalePID_i(2); }
        void copy_and_scalePID_d_E3() { copy_and_scalePID_d(2); }
        #if EXTRUDERS > 3
          void copy_and_scalePID_i_E4() { copy_and_scalePID_i(3); }
          void copy_and_scalePID_d_E4() { copy_and_scalePID_d(3); }
        #endif //EXTRUDERS > 3
      #endif //EXTRUDERS > 2
    #endif //EXTRUDERS > 1
  #endif //PID_PARAMS_PER_EXTRUDER

#endif //PIDTEMP

static void lcd_control_temperature_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_CONTROL, lcd_control_menu);
  #if TEMP_SENSOR_0 != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE, &target_temperature[0], 0, HEATER_0_MAXTEMP - 15);
  #endif
  #if EXTRUDERS > 1
    #if TEMP_SENSOR_1 != 0
      MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE MSG_N2, &target_temperature[1], 0, HEATER_1_MAXTEMP - 15);
    #endif
    #if EXTRUDERS > 2
      #if TEMP_SENSOR_2 != 0
        MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE MSG_N3, &target_temperature[2], 0, HEATER_2_MAXTEMP - 15);
      #endif
      #if EXTRUDERS > 3
        #if TEMP_SENSOR_3 != 0
          MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_NOZZLE MSG_N4, &target_temperature[3], 0, HEATER_3_MAXTEMP - 15);
        #endif
      #endif // EXTRUDERS > 3
    #endif // EXTRUDERS > 2
  #endif // EXTRUDERS > 1
  #if TEMP_SENSOR_BED != 0
    MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_BED, &target_temperature_bed, 0, BED_MAXTEMP - 15);
  #endif
  MENU_MULTIPLIER_ITEM_EDIT(int3, MSG_FAN_SPEED, &fanSpeed, 0, 255);
  #if defined(AUTOTEMP) && (TEMP_SENSOR_0 != 0)
    MENU_ITEM_EDIT(bool, MSG_AUTOTEMP, &autotemp_enabled);
    MENU_ITEM_EDIT(float3, MSG_MIN, &autotemp_min, 0, HEATER_0_MAXTEMP - 15);
    MENU_ITEM_EDIT(float3, MSG_MAX, &autotemp_max, 0, HEATER_0_MAXTEMP - 15);
    MENU_ITEM_EDIT(float32, MSG_FACTOR, &autotemp_factor, 0.0, 1.0);
  #endif
  #ifdef PIDTEMP
    // set up temp variables - undo the default scaling
    raw_Ki = unscalePID_i(PID_PARAM(Ki,0));
    raw_Kd = unscalePID_d(PID_PARAM(Kd,0));
    MENU_ITEM_EDIT(float52, MSG_PID_P, &PID_PARAM(Kp,0), 1, 9990);
    // i is typically a small value so allows values below 1
    MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_I, &raw_Ki, 0.01, 9990, copy_and_scalePID_i_E1);
    MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_D, &raw_Kd, 1, 9990, copy_and_scalePID_d_E1);
    #ifdef PID_ADD_EXTRUSION_RATE
      MENU_ITEM_EDIT(float3, MSG_PID_C, &PID_PARAM(Kc,0), 1, 9990);
    #endif//PID_ADD_EXTRUSION_RATE
    #ifdef PID_PARAMS_PER_EXTRUDER
      #if EXTRUDERS > 1
        // set up temp variables - undo the default scaling
        raw_Ki = unscalePID_i(PID_PARAM(Ki,1));
        raw_Kd = unscalePID_d(PID_PARAM(Kd,1));
        MENU_ITEM_EDIT(float52, MSG_PID_P MSG_E2, &PID_PARAM(Kp,1), 1, 9990);
        // i is typically a small value so allows values below 1
        MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_I MSG_E2, &raw_Ki, 0.01, 9990, copy_and_scalePID_i_E2);
        MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_D MSG_E2, &raw_Kd, 1, 9990, copy_and_scalePID_d_E2);
        #ifdef PID_ADD_EXTRUSION_RATE
          MENU_ITEM_EDIT(float3, MSG_PID_C MSG_E2, &PID_PARAM(Kc,1), 1, 9990);
        #endif//PID_ADD_EXTRUSION_RATE

        #if EXTRUDERS > 2
          // set up temp variables - undo the default scaling
          raw_Ki = unscalePID_i(PID_PARAM(Ki,2));
          raw_Kd = unscalePID_d(PID_PARAM(Kd,2));
          MENU_ITEM_EDIT(float52, MSG_PID_P MSG_E3, &PID_PARAM(Kp,2), 1, 9990);
          // i is typically a small value so allows values below 1
          MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_I MSG_E3, &raw_Ki, 0.01, 9990, copy_and_scalePID_i_E3);
          MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_D MSG_E3, &raw_Kd, 1, 9990, copy_and_scalePID_d_E3);
          #ifdef PID_ADD_EXTRUSION_RATE
            MENU_ITEM_EDIT(float3, MSG_PID_C MSG_E3, &PID_PARAM(Kc,2), 1, 9990);
          #endif//PID_ADD_EXTRUSION_RATE

          #if EXTRUDERS > 3
            // set up temp variables - undo the default scaling
            raw_Ki = unscalePID_i(PID_PARAM(Ki,3));
            raw_Kd = unscalePID_d(PID_PARAM(Kd,3));
            MENU_ITEM_EDIT(float52, MSG_PID_P MSG_E4, &PID_PARAM(Kp,3), 1, 9990);
            // i is typically a small value so allows values below 1
            MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_I MSG_E4, &raw_Ki, 0.01, 9990, copy_and_scalePID_i_E4);
            MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_D MSG_E4, &raw_Kd, 1, 9990, copy_and_scalePID_d_E4);
            #ifdef PID_ADD_EXTRUSION_RATE
              MENU_ITEM_EDIT(float3, MSG_PID_C MSG_E4, &PID_PARAM(Kc,3), 1, 9990);
            #endif//PID_ADD_EXTRUSION_RATE
          #endif//EXTRUDERS > 3
        #endif//EXTRUDERS > 2
      #endif//EXTRUDERS > 1
    #endif //PID_PARAMS_PER_EXTRUDER
  #endif//PIDTEMP
  MENU_ITEM(submenu, MSG_PREHEAT_PLA_SETTINGS, lcd_control_temperature_preheat_pla_settings_menu);
  MENU_ITEM(submenu, MSG_PREHEAT_ABS_SETTINGS, lcd_control_temperature_preheat_abs_settings_menu);
  END_MENU();
}

static void lcd_control_temperature_preheat_pla_settings_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_TEMPERATURE, lcd_control_temperature_menu);
  MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &plaPreheatFanSpeed, 0, 255);
  #if TEMP_SENSOR_0 != 0
    MENU_ITEM_EDIT(int3, MSG_NOZZLE, &plaPreheatHotendTemp, 0, HEATER_0_MAXTEMP - 15);
  #endif
  #if TEMP_SENSOR_BED != 0
    MENU_ITEM_EDIT(int3, MSG_BED, &plaPreheatHPBTemp, 0, BED_MAXTEMP - 15);
  #endif
  #ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
  #endif
  END_MENU();
}

static void lcd_control_temperature_preheat_abs_settings_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_TEMPERATURE, lcd_control_temperature_menu);
  MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &absPreheatFanSpeed, 0, 255);
  #if TEMP_SENSOR_0 != 0
    MENU_ITEM_EDIT(int3, MSG_NOZZLE, &absPreheatHotendTemp, 0, HEATER_0_MAXTEMP - 15);
  #endif
  #if TEMP_SENSOR_BED != 0
    MENU_ITEM_EDIT(int3, MSG_BED, &absPreheatHPBTemp, 0, BED_MAXTEMP - 15);
  #endif
  #ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
  #endif
  END_MENU();
}

static void lcd_control_motion_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_CONTROL, lcd_control_menu);
  #ifdef ENABLE_AUTO_BED_LEVELING
    MENU_ITEM_EDIT(float32, MSG_ZPROBE_ZOFFSET, &zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
  #endif
  MENU_ITEM_EDIT(float5, MSG_ACC, &acceleration, 10, 99000);
  MENU_ITEM_EDIT(float3, MSG_VXY_JERK, &max_xy_jerk, 1, 990);
  MENU_ITEM_EDIT(float52, MSG_VZ_JERK, &max_z_jerk, 0.1, 990);
  MENU_ITEM_EDIT(float3, MSG_VE_JERK, &max_e_jerk, 1, 990);
  MENU_ITEM_EDIT(float3, MSG_VMAX MSG_X, &max_feedrate[X_AXIS], 1, 999);
  MENU_ITEM_EDIT(float3, MSG_VMAX MSG_Y, &max_feedrate[Y_AXIS], 1, 999);
  MENU_ITEM_EDIT(float3, MSG_VMAX MSG_Z, &max_feedrate[Z_AXIS], 1, 999);
  MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E, &max_feedrate[E_AXIS], 1, 999);
  MENU_ITEM_EDIT(float3, MSG_VMIN, &minimumfeedrate, 0, 999);
  MENU_ITEM_EDIT(float3, MSG_VTRAV_MIN, &mintravelfeedrate, 0, 999);
  MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_X, &max_acceleration_units_per_sq_second[X_AXIS], 100, 99000, reset_acceleration_rates);
  MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_Y, &max_acceleration_units_per_sq_second[Y_AXIS], 100, 99000, reset_acceleration_rates);
  MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_Z, &max_acceleration_units_per_sq_second[Z_AXIS], 10, 99000, reset_acceleration_rates);
  MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &max_acceleration_units_per_sq_second[E_AXIS], 100, 99000, reset_acceleration_rates);
  MENU_ITEM_EDIT(float5, MSG_A_RETRACT, &retract_acceleration, 100, 99000);
  MENU_ITEM_EDIT(float5, MSG_A_TRAVEL, &travel_acceleration, 100, 99000);
  MENU_ITEM_EDIT(float52, MSG_XSTEPS, &axis_steps_per_unit[X_AXIS], 5, 9999);
  MENU_ITEM_EDIT(float52, MSG_YSTEPS, &axis_steps_per_unit[Y_AXIS], 5, 9999);
  MENU_ITEM_EDIT(float51, MSG_ZSTEPS, &axis_steps_per_unit[Z_AXIS], 5, 9999);
  MENU_ITEM_EDIT(float51, MSG_ESTEPS, &axis_steps_per_unit[E_AXIS], 5, 9999);
  #ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED
    MENU_ITEM_EDIT(bool, MSG_ENDSTOP_ABORT, &abort_on_endstop_hit);
  #endif
  #ifdef SCARA
    MENU_ITEM_EDIT(float74, MSG_XSCALE, &axis_scaling[X_AXIS],0.5,2);
    MENU_ITEM_EDIT(float74, MSG_YSCALE, &axis_scaling[Y_AXIS],0.5,2);
  #endif
  END_MENU();
}

static void lcd_control_volumetric_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_CONTROL, lcd_control_menu);

  MENU_ITEM_EDIT_CALLBACK(bool, MSG_VOLUMETRIC_ENABLED, &volumetric_enabled, calculate_volumetric_multipliers);

  if (volumetric_enabled) {
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_SIZE_EXTRUDER_0, &filament_size[0], 1.5, 3.25, calculate_volumetric_multipliers);
    #if EXTRUDERS > 1
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_SIZE_EXTRUDER_1, &filament_size[1], 1.5, 3.25, calculate_volumetric_multipliers);
      #if EXTRUDERS > 2
        MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_SIZE_EXTRUDER_2, &filament_size[2], 1.5, 3.25, calculate_volumetric_multipliers);
        #if EXTRUDERS > 3
          MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_FILAMENT_SIZE_EXTRUDER_3, &filament_size[3], 1.5, 3.25, calculate_volumetric_multipliers);
        #endif //EXTRUDERS > 3
      #endif //EXTRUDERS > 2
    #endif //EXTRUDERS > 1
  }

  END_MENU();
}

#ifdef DOGLCD

static void lcd_set_contrast() {
  if (encoderPosition != 0) {
    lcd_contrast -= encoderPosition;
    if (lcd_contrast < 0) lcd_contrast = 0;
    else if (lcd_contrast > 63) lcd_contrast = 63;
    encoderPosition = 0;
    lcdDrawUpdate = 1;
    u8g.setContrast(lcd_contrast);
  }
  if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR(MSG_CONTRAST), itostr2(lcd_contrast));
  if (LCD_CLICKED) lcd_goto_menu(lcd_control_menu);
}

#endif //DOGLCD

#ifdef FWRETRACT

static void lcd_control_retract_menu() {
  START_MENU();
  MENU_ITEM(back, MSG_CONTROL, lcd_control_menu);
  MENU_ITEM_EDIT(bool, MSG_AUTORETRACT, &autoretract_enabled);
  MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT, &retract_length, 0, 100);
  #if EXTRUDERS > 1
    MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_SWAP, &retract_length_swap, 0, 100);
  #endif
  MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACTF, &retract_feedrate, 1, 999);
  MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_ZLIFT, &retract_zlift, 0, 999);
  MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_RECOVER, &retract_recover_length, 0, 100);
  #if EXTRUDERS > 1
    MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_RECOVER_SWAP, &retract_recover_length_swap, 0, 100);
  #endif
  MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVERF, &retract_recover_feedrate, 1, 999);
  END_MENU();
}

#endif //FWRETRACT

#if SDCARDDETECT == -1
  static void lcd_sd_refresh() {
    card.initsd();
    currentMenuViewOffset = 0;
  }
#endif

static void lcd_sd_updir() {
  card.updir();
  currentMenuViewOffset = 0;
}

void lcd_sdcard_menu() {
  if (lcdDrawUpdate == 0 && LCD_CLICKED == 0) return;	// nothing to do (so don't thrash the SD card)
  uint16_t fileCnt = card.getnrfilenames();
  START_MENU();
  MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
  card.getWorkDirName();
  if (card.filename[0] == '/') {
    #if SDCARDDETECT == -1
      MENU_ITEM(function, LCD_STR_REFRESH MSG_REFRESH, lcd_sd_refresh);
    #endif
  }
  else {
    MENU_ITEM(function, LCD_STR_FOLDER "..", lcd_sd_updir);
  }

  for(uint16_t i = 0; i < fileCnt; i++) {
    if (_menuItemNr == _lineNr) {
      #ifndef SDCARD_RATHERRECENTFIRST
        card.getfilename(i);
      #else
        card.getfilename(fileCnt-1-i);
      #endif
      if (card.filenameIsDir)
        MENU_ITEM(sddirectory, MSG_CARD_MENU, card.filename, card.longFilename);
      else
        MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename, card.longFilename);
    }
    else {
      MENU_ITEM_DUMMY();
    }
  }
  END_MENU();
}

#define menu_edit_type(_type, _name, _strFunc, scale) \
  bool _menu_edit_ ## _name () { \
    bool isClicked = LCD_CLICKED; \
    if ((int32_t)encoderPosition < 0) encoderPosition = 0; \
    if ((int32_t)encoderPosition > maxEditValue) encoderPosition = maxEditValue; \
    if (lcdDrawUpdate) \
        lcd_implementation_drawedit(editLabel, _strFunc(((_type)((int32_t)encoderPosition + minEditValue)) / scale)); \
    if (isClicked) { \
      *((_type*)editValue) = ((_type)((int32_t)encoderPosition + minEditValue)) / scale; \
      lcd_goto_menu(prevMenu, prevEncoderPosition); \
    } \
    return isClicked; \
  } \
  void menu_edit_ ## _name () { _menu_edit_ ## _name(); } \
  void menu_edit_callback_ ## _name () { if (_menu_edit_ ## _name ()) (*callbackFunc)(); } \
  static void _menu_action_setting_edit_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue) { \
    prevMenu = currentMenu; \
    prevEncoderPosition = encoderPosition; \
     \
    lcdDrawUpdate = 2; \
    currentMenu = menu_edit_ ## _name; \
     \
    editLabel = pstr; \
    editValue = ptr; \
    minEditValue = minValue * scale; \
    maxEditValue = maxValue * scale - minEditValue; \
    encoderPosition = (*ptr) * scale - minEditValue; \
  } \
  static void menu_action_setting_edit_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue) { \
    _menu_action_setting_edit_ ## _name(pstr, ptr, minValue, maxValue); \
    currentMenu = menu_edit_ ## _name; \
  }\
  static void menu_action_setting_edit_callback_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue, menuFunc_t callback) { \
    _menu_action_setting_edit_ ## _name(pstr, ptr, minValue, maxValue); \
    currentMenu = menu_edit_callback_ ## _name; \
    callbackFunc = callback; \
  }
menu_edit_type(int, int3, itostr3, 1)
menu_edit_type(float, float3, ftostr3, 1)
menu_edit_type(float, float32, ftostr32, 100)
menu_edit_type(float, float43, ftostr43, 1000)
menu_edit_type(float, float5, ftostr5, 0.01)
menu_edit_type(float, float51, ftostr51, 10)
menu_edit_type(float, float52, ftostr52, 100)
menu_edit_type(unsigned long, long5, ftostr5, 0.01)

#ifdef REPRAPWORLD_KEYPAD
  static void reprapworld_keypad_move_z_up() {
    encoderPosition = 1;
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    lcd_move_z();
  }
  static void reprapworld_keypad_move_z_down() {
    encoderPosition = -1;
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    lcd_move_z();
  }
  static void reprapworld_keypad_move_x_left() {
    encoderPosition = -1;
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    lcd_move_x();
  }
  static void reprapworld_keypad_move_x_right() {
    encoderPosition = 1;
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    lcd_move_x();
  }
  static void reprapworld_keypad_move_y_down() {
    encoderPosition = 1;
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    lcd_move_y();
  }
  static void reprapworld_keypad_move_y_up() {
    encoderPosition = -1;
    move_menu_scale = REPRAPWORLD_KEYPAD_MOVE_STEP;
    lcd_move_y();
  }
  static void reprapworld_keypad_move_home() {
    enquecommands_P((PSTR("G28"))); // move all axis home
  }
#endif //REPRAPWORLD_KEYPAD

/** End of menus **/

static void lcd_quick_feedback() {
  lcdDrawUpdate = 2;
  blocking_enc = millis() + 500;
    
  #ifdef LCD_USE_I2C_BUZZER
    #ifndef LCD_FEEDBACK_FREQUENCY_HZ
      #define LCD_FEEDBACK_FREQUENCY_HZ 100
    #endif
    #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS (1000/6)
    #endif    
    lcd_buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);
  #elif defined(BEEPER) && BEEPER > -1
    SET_OUTPUT(BEEPER);
    #ifndef LCD_FEEDBACK_FREQUENCY_HZ
      #define LCD_FEEDBACK_FREQUENCY_HZ 500
    #endif
    #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 50
    #endif
    const unsigned int delay = 1000000 / LCD_FEEDBACK_FREQUENCY_HZ / 2;
    int i = LCD_FEEDBACK_FREQUENCY_DURATION_MS * LCD_FEEDBACK_FREQUENCY_HZ / 1000;
    while (i--) {
      WRITE(BEEPER,HIGH);
      delayMicroseconds(delay);
      WRITE(BEEPER,LOW);
      delayMicroseconds(delay);
     }
  #endif
}

/** Menu action functions **/
static void menu_action_back(menuFunc_t data) { lcd_goto_menu(data); }
static void menu_action_submenu(menuFunc_t data) { lcd_goto_menu(data); }
static void menu_action_gcode(const char* pgcode) { enquecommands_P(pgcode); }
static void menu_action_function(menuFunc_t data) { (*data)(); }
static void menu_action_sdfile(const char* filename, char* longFilename) {
  char cmd[30];
  char* c;
  sprintf_P(cmd, PSTR("M23 %s"), filename);
  for(c = &cmd[4]; *c; c++) *c = tolower(*c);
  enquecommand(cmd);
  enquecommands_P(PSTR("M24"));
  lcd_return_to_status();
}
static void menu_action_sddirectory(const char* filename, char* longFilename) {
  card.chdir(filename);
  encoderPosition = 0;
}
static void menu_action_setting_edit_bool(const char* pstr, bool* ptr) { *ptr = !(*ptr); }
static void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, menuFunc_t callback) {
  menu_action_setting_edit_bool(pstr, ptr);
  (*callback)();
}

#endif //ULTIPANEL

/** LCD API **/
void lcd_init() {
  lcd_implementation_init();

  #ifdef NEWPANEL

    SET_INPUT(BTN_EN1);
    SET_INPUT(BTN_EN2);
    WRITE(BTN_EN1,HIGH);
    WRITE(BTN_EN2,HIGH);
  #if BTN_ENC > 0
    SET_INPUT(BTN_ENC);
    WRITE(BTN_ENC,HIGH);
  #endif
  #ifdef REPRAPWORLD_KEYPAD
    pinMode(SHIFT_CLK,OUTPUT);
    pinMode(SHIFT_LD,OUTPUT);
    pinMode(SHIFT_OUT,INPUT);
    WRITE(SHIFT_OUT,HIGH);
    WRITE(SHIFT_LD,HIGH);
  #endif
#else  // Not NEWPANEL
  #ifdef SR_LCD_2W_NL // Non latching 2 wire shift register
     pinMode (SR_DATA_PIN, OUTPUT);
     pinMode (SR_CLK_PIN, OUTPUT);
  #elif defined(SHIFT_CLK)
     pinMode(SHIFT_CLK,OUTPUT);
     pinMode(SHIFT_LD,OUTPUT);
     pinMode(SHIFT_EN,OUTPUT);
     pinMode(SHIFT_OUT,INPUT);
     WRITE(SHIFT_OUT,HIGH);
     WRITE(SHIFT_LD,HIGH);
     WRITE(SHIFT_EN,LOW);
  #endif // SR_LCD_2W_NL
#endif//!NEWPANEL

  #if defined(SDSUPPORT) && defined(SDCARDDETECT) && (SDCARDDETECT > 0)
    pinMode(SDCARDDETECT, INPUT);
    WRITE(SDCARDDETECT, HIGH);
    lcd_oldcardstatus = IS_SD_INSERTED;
  #endif //(SDCARDDETECT > 0)

  #ifdef LCD_HAS_SLOW_BUTTONS
    slow_buttons = 0;
  #endif

  lcd_buttons_update();

  #ifdef ULTIPANEL
    encoderDiff = 0;
  #endif
}

int lcd_strlen(char *s) {
  int i = 0, j = 0;
  while (s[i]) {
    if ((s[i] & 0xc0) != 0x80) j++;
    i++;
  }
  return j;
}

int lcd_strlen_P(const char *s) {
  int j = 0;
  while (pgm_read_byte(s)) {
    if ((pgm_read_byte(s) & 0xc0) != 0x80) j++;
    s++;
  }
  return j;
}

void lcd_update() {
  static unsigned long timeoutToStatus = 0;

  #ifdef LCD_HAS_SLOW_BUTTONS
    slow_buttons = lcd_implementation_read_slow_buttons(); // buttons which take too long to read in interrupt context
  #endif

  lcd_buttons_update();

  #if (SDCARDDETECT > 0)
    if (IS_SD_INSERTED != lcd_oldcardstatus && lcd_detected()) {
      lcdDrawUpdate = 2;
      lcd_oldcardstatus = IS_SD_INSERTED;
      lcd_implementation_init( // to maybe revive the LCD if static electricity killed it.
        #if defined(LCD_PROGRESS_BAR) && defined(SDSUPPORT) && !defined(DOGLCD)
          currentMenu == lcd_status_screen
        #endif
      );

      if (lcd_oldcardstatus) {
        card.initsd();
        LCD_MESSAGEPGM(MSG_SD_INSERTED);
      }
      else {
        card.release();
        LCD_MESSAGEPGM(MSG_SD_REMOVED);
      }
    }
  #endif//CARDINSERTED

  uint32_t ms = millis();
  if (ms > lcd_next_update_millis) {

    #ifdef ULTIPANEL

      #ifdef REPRAPWORLD_KEYPAD
        if (REPRAPWORLD_KEYPAD_MOVE_Z_UP)     reprapworld_keypad_move_z_up();
        if (REPRAPWORLD_KEYPAD_MOVE_Z_DOWN)   reprapworld_keypad_move_z_down();
        if (REPRAPWORLD_KEYPAD_MOVE_X_LEFT)   reprapworld_keypad_move_x_left();
        if (REPRAPWORLD_KEYPAD_MOVE_X_RIGHT)  reprapworld_keypad_move_x_right();
        if (REPRAPWORLD_KEYPAD_MOVE_Y_DOWN)   reprapworld_keypad_move_y_down();
        if (REPRAPWORLD_KEYPAD_MOVE_Y_UP)     reprapworld_keypad_move_y_up();
        if (REPRAPWORLD_KEYPAD_MOVE_HOME)     reprapworld_keypad_move_home();
      #endif

      bool encoderPastThreshold = (abs(encoderDiff) >= ENCODER_PULSES_PER_STEP);
      if (encoderPastThreshold || LCD_CLICKED) {
        if (encoderPastThreshold) {
          int32_t encoderMultiplier = 1;

          #ifdef ENCODER_RATE_MULTIPLIER

            if (encoderRateMultiplierEnabled) {
              int32_t encoderMovementSteps = abs(encoderDiff) / ENCODER_PULSES_PER_STEP;

              if (lastEncoderMovementMillis != 0) {
                // Note that the rate is always calculated between to passes through the 
                // loop and that the abs of the encoderDiff value is tracked.
                float encoderStepRate = (float)(encoderMovementSteps) / ((float)(ms - lastEncoderMovementMillis)) * 1000.0;

                if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC)     encoderMultiplier = 100;
                else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC) encoderMultiplier = 10;

                #ifdef ENCODER_RATE_MULTIPLIER_DEBUG
                  SERIAL_ECHO_START;
                  SERIAL_ECHO("Enc Step Rate: ");
                  SERIAL_ECHO(encoderStepRate);
                  SERIAL_ECHO("  Multiplier: ");
                  SERIAL_ECHO(encoderMultiplier);
                  SERIAL_ECHO("  ENCODER_10X_STEPS_PER_SEC: ");
                  SERIAL_ECHO(ENCODER_10X_STEPS_PER_SEC);
                  SERIAL_ECHO("  ENCODER_100X_STEPS_PER_SEC: ");
                  SERIAL_ECHOLN(ENCODER_100X_STEPS_PER_SEC);
                #endif //ENCODER_RATE_MULTIPLIER_DEBUG
              }

              lastEncoderMovementMillis = ms;
            }
          #endif //ENCODER_RATE_MULTIPLIER

          lcdDrawUpdate = 1;
          encoderPosition += (encoderDiff * encoderMultiplier) / ENCODER_PULSES_PER_STEP;
          encoderDiff = 0;
        }
        timeoutToStatus = ms + LCD_TIMEOUT_TO_STATUS;
      }

    #endif //ULTIPANEL

    #ifdef DOGLCD  // Changes due to different driver architecture of the DOGM display
      blink++;     // Variable for fan animation and alive dot
      u8g.firstPage();
      do {
        lcd_setFont(FONT_MENU);
        u8g.setPrintPos(125, 0);
        if (blink % 2) u8g.setColorIndex(1); else u8g.setColorIndex(0); // Set color for the alive dot
        u8g.drawPixel(127, 63); // draw alive dot
        u8g.setColorIndex(1); // black on white
        (*currentMenu)();
        if (!lcdDrawUpdate) break; // Terminate display update, when nothing new to draw. This must be done before the last dogm.next()
      } while( u8g.nextPage() );
    #else
      (*currentMenu)();
    #endif

    #ifdef LCD_HAS_STATUS_INDICATORS
      lcd_implementation_update_indicators();
    #endif

    #ifdef ULTIPANEL
      if (currentMenu != lcd_status_screen &&
        #if defined(MANUAL_BED_LEVELING)
          currentMenu != _lcd_level_bed && 
          currentMenu != _lcd_level_bed_homing && 
        #endif  // MANUAL_BED_LEVELING
          millis() > timeoutToStatus) {
        lcd_return_to_status();
        lcdDrawUpdate = 2;
      }
    #endif //ULTIPANEL

    if (lcdDrawUpdate == 2) lcd_implementation_clear();
    if (lcdDrawUpdate) lcdDrawUpdate--;
    lcd_next_update_millis = millis() + LCD_UPDATE_INTERVAL;
  }
}

void lcd_ignore_click(bool b) {
  ignore_click = b;
  wait_for_unclick = false;
}

void lcd_finishstatus() {
  int len = lcd_strlen(lcd_status_message);
  if (len > 0) {
    while (len < LCD_WIDTH) {
      lcd_status_message[len++] = ' ';
    }
  }
  lcd_status_message[LCD_WIDTH] = '\0';
  #if defined(LCD_PROGRESS_BAR) && defined(SDSUPPORT) && !defined(DOGLCD)
    #if PROGRESS_MSG_EXPIRE > 0
      messageTick =
    #endif
    progressBarTick = millis();
  #endif
  lcdDrawUpdate = 2;

  #ifdef FILAMENT_LCD_DISPLAY
    message_millis = millis();  //get status message to show up for a while
  #endif
}

void lcd_setstatus(const char* message) {
  if (lcd_status_message_level > 0) return;
  strncpy(lcd_status_message, message, LCD_WIDTH);
  lcd_finishstatus();
}

void lcd_setstatuspgm(const char* message) {
  if (lcd_status_message_level > 0) return;
  strncpy_P(lcd_status_message, message, LCD_WIDTH);
  lcd_finishstatus();
}

void lcd_setalertstatuspgm(const char* message) {
  lcd_setstatuspgm(message);
  lcd_status_message_level = 1;
  #ifdef ULTIPANEL
    lcd_return_to_status();
  #endif
}

void lcd_reset_alert_level() { lcd_status_message_level = 0; }

#ifdef DOGLCD
  void lcd_setcontrast(uint8_t value) {
    lcd_contrast = value & 63;
    u8g.setContrast(lcd_contrast);
  }
#endif

#ifdef ULTIPANEL

////////////////////////
// Setup Rotary Encoder Bit Values (for two pin encoders to indicate movement)
// These values are independent of which pins are used for EN_A and EN_B indications
// The rotary encoder part is also independent to the chipset used for the LCD
#if defined(EN_A) && defined(EN_B)
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1
#endif 

/* Warning: This function is called from interrupt context */
void lcd_buttons_update() {
  #ifdef NEWPANEL
    uint8_t newbutton = 0;
    if (READ(BTN_EN1) == 0) newbutton |= EN_A;
    if (READ(BTN_EN2) == 0) newbutton |= EN_B;
    #if BTN_ENC > 0
      if (millis() > blocking_enc && READ(BTN_ENC) == 0) newbutton |= EN_C;
    #endif
    buttons = newbutton;
    #ifdef LCD_HAS_SLOW_BUTTONS
      buttons |= slow_buttons;
    #endif
    #ifdef REPRAPWORLD_KEYPAD
      // for the reprapworld_keypad
      uint8_t newbutton_reprapworld_keypad=0;
      WRITE(SHIFT_LD, LOW);
      WRITE(SHIFT_LD, HIGH);
      for(int8_t i = 0; i < 8; i++) {
        newbutton_reprapworld_keypad >>= 1;
        if (READ(SHIFT_OUT)) newbutton_reprapworld_keypad |= BIT(7);
        WRITE(SHIFT_CLK, HIGH);
        WRITE(SHIFT_CLK, LOW);
      }
      buttons_reprapworld_keypad=~newbutton_reprapworld_keypad; //invert it, because a pressed switch produces a logical 0
    #endif
  #else   //read it from the shift register
    uint8_t newbutton = 0;
    WRITE(SHIFT_LD, LOW);
    WRITE(SHIFT_LD, HIGH);
    unsigned char tmp_buttons = 0;
    for(int8_t i=0; i<8; i++) {
      newbutton >>= 1;
      if (READ(SHIFT_OUT)) newbutton |= BIT(7);
      WRITE(SHIFT_CLK, HIGH);
      WRITE(SHIFT_CLK, LOW);
    }
    buttons = ~newbutton; //invert it, because a pressed switch produces a logical 0
  #endif //!NEWPANEL

  //manage encoder rotation
  uint8_t enc=0;
  if (buttons & EN_A) enc |= B01;
  if (buttons & EN_B) enc |= B10;
  if (enc != lastEncoderBits) {
    switch(enc) {
      case encrot0:
        if (lastEncoderBits==encrot3) encoderDiff++;
        else if (lastEncoderBits==encrot1) encoderDiff--;
        break;
      case encrot1:
        if (lastEncoderBits==encrot0) encoderDiff++;
        else if (lastEncoderBits==encrot2) encoderDiff--;
        break;
      case encrot2:
        if (lastEncoderBits==encrot1) encoderDiff++;
        else if (lastEncoderBits==encrot3) encoderDiff--;
        break;
      case encrot3:
        if (lastEncoderBits==encrot2) encoderDiff++;
        else if (lastEncoderBits==encrot0) encoderDiff--;
        break;
    }
  }
  lastEncoderBits = enc;
}

bool lcd_detected(void) {
  #if (defined(LCD_I2C_TYPE_MCP23017) || defined(LCD_I2C_TYPE_MCP23008)) && defined(DETECT_DEVICE)
    return lcd.LcdDetected() == 1;
  #else
    return true;
  #endif
}

void lcd_buzz(long duration, uint16_t freq) {
  #ifdef LCD_USE_I2C_BUZZER
    lcd.buzz(duration,freq);
  #endif
}

bool lcd_clicked() { return LCD_CLICKED; }

#endif //ULTIPANEL

/********************************/
/** Float conversion utilities **/
/********************************/
//  convert float to string with +123.4 format
char conv[8];
char *ftostr3(const float &x)
{
  return itostr3((int)x);
}

char *itostr2(const uint8_t &x)
{
  //sprintf(conv,"%5.1f",x);
  int xx=x;
  conv[0]=(xx/10)%10+'0';
  conv[1]=(xx)%10+'0';
  conv[2]=0;
  return conv;
}

// Convert float to string with 123.4 format, dropping sign
char *ftostr31(const float &x)
{
  int xx=x*10;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]=(xx/10)%10+'0';
  conv[4]='.';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

// Convert float to string with 123.4 format
char *ftostr31ns(const float &x)
{
  int xx=x*10;
  //conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[0]=(xx/1000)%10+'0';
  conv[1]=(xx/100)%10+'0';
  conv[2]=(xx/10)%10+'0';
  conv[3]='.';
  conv[4]=(xx)%10+'0';
  conv[5]=0;
  return conv;
}

char *ftostr32(const float &x)
{
  long xx=x*100;
  if (xx >= 0)
    conv[0]=(xx/10000)%10+'0';
  else
    conv[0]='-';
  xx=abs(xx);
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]='.';
  conv[4]=(xx/10)%10+'0';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

// Convert float to string with 1.234 format
char *ftostr43(const float &x)
{
	long xx = x * 1000;
    if (xx >= 0)
		conv[0] = (xx / 1000) % 10 + '0';
	else
		conv[0] = '-';
	xx = abs(xx);
	conv[1] = '.';
	conv[2] = (xx / 100) % 10 + '0';
	conv[3] = (xx / 10) % 10 + '0';
	conv[4] = (xx) % 10 + '0';
	conv[5] = 0;
	return conv;
}

//Float to string with 1.23 format
char *ftostr12ns(const float &x)
{
  long xx=x*100;
  
  xx=abs(xx);
  conv[0]=(xx/100)%10+'0';
  conv[1]='.';
  conv[2]=(xx/10)%10+'0';
  conv[3]=(xx)%10+'0';
  conv[4]=0;
  return conv;
}

//  convert float to space-padded string with -_23.4_ format
char *ftostr32sp(const float &x) {
  long xx = abs(x * 100);
  uint8_t dig;

  if (x < 0) { // negative val = -_0
    conv[0] = '-';
    dig = (xx / 1000) % 10;
    conv[1] = dig ? '0' + dig : ' ';
  }
  else { // positive val = __0
    dig = (xx / 10000) % 10;
    if (dig) {
      conv[0] = '0' + dig;
      conv[1] = '0' + (xx / 1000) % 10;
    }
    else {
      conv[0] = ' ';
      dig = (xx / 1000) % 10;
      conv[1] = dig ? '0' + dig : ' ';
    }
  }

  conv[2] = '0' + (xx / 100) % 10; // lsd always

  dig = xx % 10;
  if (dig) { // 2 decimal places
    conv[5] = '0' + dig;
    conv[4] = '0' + (xx / 10) % 10;
    conv[3] = '.';
  }
  else { // 1 or 0 decimal place
    dig = (xx / 10) % 10;
    if (dig) {
      conv[4] = '0' + dig;
      conv[3] = '.';
    }
    else {
      conv[3] = conv[4] = ' ';
    }
    conv[5] = ' ';
  }
  conv[6] = '\0';
  return conv;
}

char *itostr31(const int &xx)
{
  conv[0]=(xx>=0)?'+':'-';
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]=(xx/10)%10+'0';
  conv[4]='.';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

// Convert int to rj string with 123 or -12 format
char *itostr3(const int &x)
{
  int xx = x;
  if (xx < 0) {
     conv[0]='-';
     xx = -xx;
  } else if (xx >= 100)
    conv[0]=(xx/100)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 10)
    conv[1]=(xx/10)%10+'0';
  else
    conv[1]=' ';
  conv[2]=(xx)%10+'0';
  conv[3]=0;
  return conv;
}

// Convert int to lj string with 123 format
char *itostr3left(const int &xx)
{
  if (xx >= 100)
  {
    conv[0]=(xx/100)%10+'0';
    conv[1]=(xx/10)%10+'0';
    conv[2]=(xx)%10+'0';
    conv[3]=0;
  }
  else if (xx >= 10)
  {
    conv[0]=(xx/10)%10+'0';
    conv[1]=(xx)%10+'0';
    conv[2]=0;
  }
  else
  {
    conv[0]=(xx)%10+'0';
    conv[1]=0;
  }
  return conv;
}

// Convert int to rj string with 1234 format
char *itostr4(const int &xx) {
  conv[0] = xx >= 1000 ? (xx / 1000) % 10 + '0' : ' ';
  conv[1] = xx >= 100 ? (xx / 100) % 10 + '0' : ' ';
  conv[2] = xx >= 10 ? (xx / 10) % 10 + '0' : ' ';
  conv[3] = xx % 10 + '0';
  conv[4] = 0;
  return conv;
}

// Convert float to rj string with 12345 format
char *ftostr5(const float &x) {
  long xx = abs(x);
  conv[0] = xx >= 10000 ? (xx / 10000) % 10 + '0' : ' ';
  conv[1] = xx >= 1000 ? (xx / 1000) % 10 + '0' : ' ';
  conv[2] = xx >= 100 ? (xx / 100) % 10 + '0' : ' ';
  conv[3] = xx >= 10 ? (xx / 10) % 10 + '0' : ' ';
  conv[4] = xx % 10 + '0';
  conv[5] = 0;
  return conv;
}

// Convert float to string with +1234.5 format
char *ftostr51(const float &x)
{
  long xx=x*10;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/10000)%10+'0';
  conv[2]=(xx/1000)%10+'0';
  conv[3]=(xx/100)%10+'0';
  conv[4]=(xx/10)%10+'0';
  conv[5]='.';
  conv[6]=(xx)%10+'0';
  conv[7]=0;
  return conv;
}

// Convert float to string with +123.45 format
char *ftostr52(const float &x)
{
  long xx=x*100;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/10000)%10+'0';
  conv[2]=(xx/1000)%10+'0';
  conv[3]=(xx/100)%10+'0';
  conv[4]='.';
  conv[5]=(xx/10)%10+'0';
  conv[6]=(xx)%10+'0';
  conv[7]=0;
  return conv;
}

#if defined(MANUAL_BED_LEVELING)
static int _lcd_level_bed_position;
static void _lcd_level_bed()
{
  if (encoderPosition != 0) {
    refresh_cmd_timeout();
    current_position[Z_AXIS] += float((int)encoderPosition) * 0.05;
    if (min_software_endstops && current_position[Z_AXIS] < Z_MIN_POS) current_position[Z_AXIS] = Z_MIN_POS;
    if (max_software_endstops && current_position[Z_AXIS] > Z_MAX_POS) current_position[Z_AXIS] = Z_MAX_POS;
    encoderPosition = 0;
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Z_AXIS]/60, active_extruder);
    lcdDrawUpdate = 1;
  }
  if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR("Z"), ftostr32(current_position[Z_AXIS]));
  static bool debounce_click = false;
  if (LCD_CLICKED) {
    if (!debounce_click) {
      debounce_click = true;
      int ix = _lcd_level_bed_position % MESH_NUM_X_POINTS;
      int iy = _lcd_level_bed_position / MESH_NUM_X_POINTS;
      mbl.set_z(ix, iy, current_position[Z_AXIS]);
      _lcd_level_bed_position++;
      if (_lcd_level_bed_position == MESH_NUM_X_POINTS*MESH_NUM_Y_POINTS) {
        current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
        mbl.active = 1;
        enquecommands_P(PSTR("G28"));
        lcd_return_to_status();
      } else {
        current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
        ix = _lcd_level_bed_position % MESH_NUM_X_POINTS;
        iy = _lcd_level_bed_position / MESH_NUM_X_POINTS;
        if (iy&1) { // Zig zag
          ix = (MESH_NUM_X_POINTS - 1) - ix;
        }
        current_position[X_AXIS] = mbl.get_x(ix);
        current_position[Y_AXIS] = mbl.get_y(iy);
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
        lcdDrawUpdate = 1;
      }
    }
  } else {
    debounce_click = false;
  }
}
static void _lcd_level_bed_homing()
{
  if (axis_known_position[X_AXIS] &&
      axis_known_position[Y_AXIS] &&
      axis_known_position[Z_AXIS]) {
    current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    current_position[X_AXIS] = MESH_MIN_X;
    current_position[Y_AXIS] = MESH_MIN_Y;
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
    _lcd_level_bed_position = 0;
    lcd_goto_menu(_lcd_level_bed);
  }
}
static void lcd_level_bed()
{
  axis_known_position[X_AXIS] = false;
  axis_known_position[Y_AXIS] = false;
  axis_known_position[Z_AXIS] = false;
  mbl.reset();
  enquecommands_P(PSTR("G28"));
  lcd_goto_menu(_lcd_level_bed_homing);
}
#endif  // MANUAL_BED_LEVELING

#endif //ULTRA_LCD
