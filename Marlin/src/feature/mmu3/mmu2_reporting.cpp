#include <avr/pgmspace.h>

#include "src/MarlinCore.h"
#if HAS_PRUSA_MMU3
#include "src/core/language.h"
#include "src/gcode/gcode.h"
#include "src/feature/host_actions.h"
#include "src/lcd/marlinui.h"
#include "src/lcd/menu/menu.h"
#include "src/lcd/menu/menu_item.h"
#include "src/module/temperature.h"
#include "mmu2.h"
#include "mmu2_log.h"
#include "mmu2_fsensor.h"
#include "mmu2_reporting.h"
#include "mmu2_error_converter.h"
#include "mmu2_marlin_macros.h"
#include "mmu2_progress_converter.h"
#include "mmu_hw/buttons.h"
#include "mmu_hw/error_codes.h"
#include "mmu_hw/errors_list.h"
#include "messages.h"
#include "ultralcd.h"
#include "sound.h"

#ifndef HAS_WIRED_LCD
#define LCD_WIDTH 21
#endif


namespace MMU2 {

OperationStatistics operation_statistics;

uint16_t OperationStatistics::fail_total_num;       // total failures
uint8_t  OperationStatistics::fail_num;             // fails during print
uint16_t OperationStatistics::load_fail_total_num;  // total load failures
uint8_t  OperationStatistics::load_fail_num;        // load failures during print
uint16_t OperationStatistics::tool_change_counter;  // number of tool changes per print
uint32_t OperationStatistics::tool_change_total_counter;  // number of total tool changes
int OperationStatistics::fail_total_num_addr;       // total failures EEPROM addr
int OperationStatistics::fail_num_addr;             // fails during print EEPROM addr
int OperationStatistics::load_fail_total_num_addr;  // total load failures EEPROM addr
int OperationStatistics::load_fail_num_addr;        // load failures during print EEPROM addr
int OperationStatistics::tool_change_counter_addr;  // number of total tool changes EEPROM addr
int OperationStatistics::tool_change_total_counter_addr;  // number of total tool changes EEPROM addr

/**
 * Increment both the total load fails and Per print job load fails.
*/
void OperationStatistics::increment_load_fails(){
  load_fail_num += 1;
  load_fail_total_num += 1;

  #if ENABLED(EEPROM_SETTINGS)
  // save load_fail_num to eeprom
  persistentStore.access_start();
  persistentStore.write_data(load_fail_num_addr, load_fail_num);

  // save load_fail_total_num to eeprom
  persistentStore.write_data(load_fail_total_num_addr, load_fail_total_num);
  persistentStore.access_finish();
  settings.save();
  #endif
}

/**
 * Increment both the total fails and the per print job fails.
*/
void OperationStatistics::increment_mmu_fails(){
  fail_num += 1;
  fail_total_num += 1;

  #if ENABLED(EEPROM_SETTINGS)
  // save fail_num to eeprom
  persistentStore.access_start();
  persistentStore.write_data(fail_num_addr, fail_num);
  // save fail_total_num to eeprom
  persistentStore.write_data(fail_total_num_addr, fail_total_num);
  persistentStore.access_finish();
  settings.save();
  #endif
}

/**
 * Increment tool change counter
*/
void OperationStatistics::increment_tool_change_counter(){
  tool_change_counter += 1;
  tool_change_total_counter += 1;

  #if ENABLED(EEPROM_SETTINGS)
  // save tool_change_total_counter to eeprom
  persistentStore.access_start();
  persistentStore.write_data(tool_change_total_counter_addr, tool_change_total_counter);
  persistentStore.access_finish();
  settings.save();
  #endif
}


/**
 * Reset only per print operation statistics and update EEPROM.
 * 
 * @returns true if everything went okay, false otherwise.
*/
bool OperationStatistics::reset_per_print_stats(){
  // Update data
  load_fail_num = 0;
  fail_num = 0;
  tool_change_counter = 0;

  #if ENABLED(EEPROM_SETTINGS)
  // Update EEPROM
  persistentStore.access_start();
  persistentStore.write_data(load_fail_num_addr, load_fail_num);
  persistentStore.write_data(fail_num_addr, fail_num);
  persistentStore.write_data(tool_change_counter_addr, tool_change_counter);
  persistentStore.access_finish();
  return settings.save();
  #else
  return true;
  #endif
}


/**
 * Reset all operation statistics and update EEPROM.
 * 
 * @returns true if everything went okay, false otherwise.
*/
bool OperationStatistics::reset_stats(){
  // Update data
  load_fail_num = 0;
  load_fail_total_num = 0;
  fail_num = 0;
  fail_total_num = 0;
  tool_change_counter = 0;
  tool_change_total_counter = 0;

  #if ENABLED(EEPROM_SETTINGS)
  // Update EEPROM
  persistentStore.access_start();
  persistentStore.write_data(load_fail_num_addr, load_fail_num);
  persistentStore.write_data(load_fail_total_num_addr, load_fail_total_num);
  persistentStore.write_data(fail_num_addr, fail_num);
  persistentStore.write_data(fail_total_num_addr, fail_total_num);
  persistentStore.write_data(tool_change_counter_addr, tool_change_counter);
  persistentStore.write_data(tool_change_total_counter_addr, tool_change_total_counter);
  persistentStore.access_finish();
  return settings.save();
  #else
  return true;
  #endif
}


void BeginReport(CommandInProgress /*cip*/, ProgressCode ec) {
  // custom_message_type = CustomMsg::MMUProgress;
  ui.set_status(ProgressCodeToText(ec));
}

void EndReport(CommandInProgress /*cip*/, ProgressCode /*ec*/) {
  // clear the status msg line - let the printed filename get visible again
  if (!printJobOngoing()) {
    ui.set_status(MSG_WELCOME);
  }
  // custom_message_type = CustomMsg::Status;
}

/**
 * @brief Renders any characters that will be updated live on the MMU error screen.
 *Currently, this is FINDA and Filament Sensor status and Extruder temperature.
 */
extern void ReportErrorHookDynamicRender(void){
  #ifdef HAS_WIRED_LCD
  // beware - this optimization abuses the fact, that FindaDetectsFilament returns 0 or 1 and '0' is followed by '1' in the ASCII table
  lcd_put_int(3, 2, mmu2.FindaDetectsFilament() + '0');
  lcd_put_int(8, 2, FILAMENT_PRESENT() + '0');

  // print active/changing filament slot
  lcd_moveto(10, 2);
  lcdui_print_extruder();

  // Print active extruder temperature
  lcd_put_int(16, 2, (int)(thermalManager.degHotend(0) + 0.5));
  #endif
}

/**
 * @brief Renders any characters that are static on the MMU error screen i.e. they don't change.
 * @param[in] ei Error code index
 */
static void ReportErrorHookStaticRender(uint8_t ei) {
  #ifdef HAS_WIRED_LCD
  //! Show an error screen
  //! When an MMU error occurs, the LCD content will look like this:
  //! |01234567890123456789|
  //! |MMU FW update needed|     <- title/header of the error: max 20 characters
  //! |prusa.io/04504      |     <- URL max 20 characters
  //! |FI:1 FS:1  5>3 t201°|     <- status line, t is thermometer symbol
  //! |>Retry   >Done    >W|     <- buttons
  bool two_choices = false;

  // Read and determine what operations should be shown on the menu
  const uint8_t button_operation = PrusaErrorButtons(ei);
  const uint8_t button_op_right  = BUTTON_OP_RIGHT(button_operation);
  const uint8_t button_op_middle = BUTTON_OP_MIDDLE(button_operation);

  // Check if the menu should have three or two choices
  if (button_op_right == (uint8_t)ButtonOperations::NoOperation){
    // Two operations not specified, the error menu should only show two choices
    two_choices = true;
  }

  ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_CALL_NO_REDRAW;
  ui.clear_lcd();

  START_SCREEN();
  #ifndef __AVR__
  // TODO: I couldn't make this work on AVR
  STATIC_ITEM_F(F(PrusaErrorTitle(ei)), SS_DEFAULT|SS_INVERT);

  MString<LCD_WIDTH> url("");
  url.append(
    "prusa.io/04%hu",
    PrusaErrorCode(ei)
  );
  STATIC_ITEM_F(F(url.buffer()));
  #endif

  ReportErrorHookSensorLineRender();

  // Render the choices
  lcd_show_choices_prompt_P(
    two_choices ? LCD_LEFT_BUTTON_CHOICE : LCD_MIDDLE_BUTTON_CHOICE,
    PrusaErrorButtonTitle(button_op_middle),
    two_choices ? PrusaErrorButtonMore() : PrusaErrorButtonTitle(button_op_right),
    two_choices ? 18 : 9,
    two_choices ? nullptr : PrusaErrorButtonMore()
  );

  END_SCREEN();
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  #endif
}

void ReportErrorHookSensorLineRender(){
  #ifdef HAS_WIRED_LCD
  // Render static characters in third line
  lcd_put_u8str(0, 2, F("FI:  FS:    >  " LCD_STR_THERMOMETER "   " LCD_STR_DEGREE));
  #endif
}

/**
 * @brief Monitors the LCD button selection without blocking MMU communication
 * @param[in] ei Error code index
 * @return 0 if there is no knob click --
 * 1 if user clicked 'More' and firmware should render
 * the error screen when ReportErrorHook is called next --
 * 2 if the user selects an operation and we would like
 * to exit the error screen. The MMU will raise the menu
 * again if the error is not solved.
 */
static uint8_t ReportErrorHookMonitor(uint8_t ei) {
  uint8_t ret = 0;
  bool two_choices = false;
  static uint8_t reset_button_selection;

  // Read and determine what operations should be shown on the menu
  const uint8_t button_operation   = PrusaErrorButtons(ei);
  const uint8_t button_op_right = BUTTON_OP_RIGHT(button_operation);
  const uint8_t button_op_middle  = BUTTON_OP_MIDDLE(button_operation);

  // Check if the menu should have three or two choices
  if (button_op_right == (uint8_t)ButtonOperations::NoOperation){
    // Two operations not specified, the error menu should only show two choices
    two_choices = true;
  }

  static int8_t current_selection = two_choices ? LCD_LEFT_BUTTON_CHOICE : LCD_MIDDLE_BUTTON_CHOICE;
  static int8_t choice_selected = -1;

  if (reset_button_selection) {
    // If a new error screen is shown, we must reset the button selection
    // Default selection is different depending on how many buttons are present
    current_selection = two_choices ? LCD_LEFT_BUTTON_CHOICE : LCD_MIDDLE_BUTTON_CHOICE;
    choice_selected = -1;
    reset_button_selection = 0;
  }

  #ifdef HAS_WIRED_LCD
  // Check if knob was rotated
  if (ui.encoderPosition != 0) {
    if (two_choices == false) { // third_choice is not nullptr, safe to dereference
      if (ui.encoderPosition < 0 && current_selection != LCD_LEFT_BUTTON_CHOICE) {
        // Rotating knob counter clockwise
        current_selection--;
      } else if (ui.encoderPosition > 0 && current_selection != LCD_RIGHT_BUTTON_CHOICE) {
        // Rotating knob clockwise
        current_selection++;
      }
    } else {
      if (ui.encoderPosition < 0 && current_selection != LCD_LEFT_BUTTON_CHOICE) {
        // Rotating knob counter clockwise
        current_selection = LCD_LEFT_BUTTON_CHOICE;
      } else if (ui.encoderPosition > 0 && current_selection != LCD_MIDDLE_BUTTON_CHOICE) {
        // Rotating knob clockwise
        current_selection = LCD_MIDDLE_BUTTON_CHOICE;
      }
    }

    // Update '>' render only
    //! @brief Button menu
    //!
    //! @code{.unparsed}
    //! |01234567890123456789|
    //! |                    |
    //! |                    |
    //! |                    |
    //! |>(left)             |
    //! ----------------------
    //! Three choices 
    //! |>(left)>(mid)>(righ)|
    //! ----------------------
    //! Two choices
    //! ----------------------
    //! |>(left)   >(mid)    |
    //! ----------------------
    //! @endcode
    //

    lcd_put_lchar(0, 3, current_selection == LCD_LEFT_BUTTON_CHOICE ? '>': ' ');
    if (two_choices == false)
    {
      lcd_put_lchar(9, 3, current_selection == LCD_MIDDLE_BUTTON_CHOICE ? '>': ' ');
      lcd_put_lchar(18, 3, current_selection == LCD_RIGHT_BUTTON_CHOICE ? '>': ' ');
    } else {
      // More button for two button screen
      lcd_put_lchar(18, 3, current_selection == LCD_MIDDLE_BUTTON_CHOICE ? '>': ' ');
    }
    // Consume rotation event
    ui.encoderPosition = 0;
  }
  #endif

  // Check if knob was clicked and consume the event
  if (ui.button_pressed()) {
    choice_selected = current_selection;
  } else {
    // continue monitoring
    return ret;
  }

  if ((two_choices && choice_selected == LCD_MIDDLE_BUTTON_CHOICE)      // Two choices and middle button selected
    || (!two_choices && choice_selected == LCD_RIGHT_BUTTON_CHOICE)) // Three choices and right most button selected
  {
    // 'More' show error description
    #ifdef HAS_WIRED_LCD
    lcd_show_fullscreen_message_and_wait_P(PrusaErrorDesc(ei));
    #ifndef __AVR__
    // TODO: I couldn't make this work on AVR
    LCD_ALERTMESSAGE_F(PrusaErrorDesc(ei));
    #endif
    #endif
    ret = 1;
  } else if(choice_selected == LCD_MIDDLE_BUTTON_CHOICE) {
    SetButtonResponse((ButtonOperations)button_op_right);
    ret = 2;
  } else {
    SetButtonResponse((ButtonOperations)button_op_middle);
    ret = 2;
  }

  // Next MMU error screen should reset the choice selection
  reset_button_selection = 1;
  return ret;
}

enum class ReportErrorHookStates : uint8_t {
  RENDER_ERROR_SCREEN  = 0,
  MONITOR_SELECTION    = 1,
  DISMISS_ERROR_SCREEN = 2,
};

enum ReportErrorHookStates ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;

// Helper variable to monitor knob in MMU error screen in blocking functions e.g. manage_response
static bool is_mmu_error_monitor_active;

// Helper variable to stop rendering the error screen when the firmware is rendering complementary
// UI to resolve the error screen, for example tuning Idler Stallguard Threshold
// Set to false to allow the error screen to render again.
static bool putErrorScreenToSleep;

void CheckErrorScreenUserInput() {
  if (is_mmu_error_monitor_active) {
    // Call this every iteration to keep the knob rotation responsive
    // This includes when mmu_loop is called within manage_response
    ReportErrorHook((CommandInProgress)mmu2.GetCommandInProgress(), mmu2.GetLastErrorCode(), mmu2.MMULastErrorSource());
  }
}

bool TuneMenuEntered() {
  return putErrorScreenToSleep;
}

void ReportErrorHook(CommandInProgress /*cip*/, ErrorCode ec, uint8_t /*es*/) {
  if (putErrorScreenToSleep) return;
  
  if (mmu2.MMUCurrentErrorCode() == ErrorCode::OK && mmu2.MMULastErrorSource() == MMU2::ErrorSourceMMU) {
    // If the error code suddenly changes to OK, that means
    // a button was pushed on the MMU and the LCD should
    // dismiss the error screen until MMU raises a new error
    ReportErrorHookState = ReportErrorHookStates::DISMISS_ERROR_SCREEN;
  }

  const uint8_t ei = PrusaErrorCodeIndex((ErrorCode)ec);

  // TODO: This part requires C++17 as "fallthrough" is not allowed in C++11,
  //       Converting this section to a if clause before the switch..case
  // switch ((uint8_t)ReportErrorHookState) {
  // case (uint8_t)ReportErrorHookStates::RENDER_ERROR_SCREEN:
  //     KEEPALIVE_STATE(PAUSED_FOR_USER);
  //     ReportErrorHookStaticRender(ei);
  //     ReportErrorHookState = ReportErrorHookStates::MONITOR_SELECTION;
  //     [[fallthrough]];
  // case (uint8_t)ReportErrorHookStates::MONITOR_SELECTION:
  //     is_mmu_error_monitor_active = true;
  //     ReportErrorHookDynamicRender(); // Render dynamic characters
  //     sound_wait_for_user();
  //     switch (ReportErrorHookMonitor(ei)) {
  //         case 0:
  //             // No choice selected, return to loop()
  //             break;
  //         case 1:
  //             // More button selected, change state
  //             ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
  //             break;
  //         case 2:
  //             // Exit error screen and enable lcd updates
  //             // lcd_update_enable(true);
  //             ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_NONE;
  //             // lcd_return_to_status();
  //             ui.return_to_status();
  //             sound_wait_for_user_reset();
  //             // Reset the state in case a new error is reported
  //             is_mmu_error_monitor_active = false;
  //             KEEPALIVE_STATE(IN_HANDLER);
  //             ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
  //             break;
  //         default:
  //             break;
  //     }
  //     return; // Always return to loop() to let MMU trigger a call to ReportErrorHook again
  //     break;
  // case (uint8_t)ReportErrorHookStates::DISMISS_ERROR_SCREEN:
  //     // lcd_update_enable(true);
  //     ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_NONE;
  //     // lcd_return_to_status();
  //     ui.return_to_status();
  //     sound_wait_for_user_reset();
  //     // Reset the state in case a new error is reported
  //     is_mmu_error_monitor_active = false;
  //     KEEPALIVE_STATE(IN_HANDLER);
  //     ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
  //     break;
  // default:
  //     break;
  // }

  // This should be the equivelent of the switch..case above...
  if( (uint8_t)ReportErrorHookState == (uint8_t)ReportErrorHookStates::RENDER_ERROR_SCREEN){
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    #if HAS_WIRED_LCD
    ReportErrorHookStaticRender(ei);
    #endif
    ReportErrorHookState = ReportErrorHookStates::MONITOR_SELECTION;
  }

  if((uint8_t)ReportErrorHookState == (uint8_t)ReportErrorHookStates::MONITOR_SELECTION){
    is_mmu_error_monitor_active = true;
    ReportErrorHookDynamicRender(); // Render dynamic characters
    sound_wait_for_user();
    uint8_t result = ReportErrorHookMonitor(ei);
    if( result == 0){
      // No choice selected, return to loop()
    } else if ( result == 1){
      // More button selected, change state
      ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
    } else if ( result == 2){
      // Exit error screen and enable lcd updates
      #ifdef HAS_WIRED_LCD
      ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_CALL_REDRAW_NEXT;
      ui.return_to_status();
      #endif
      sound_wait_for_user_reset();
      // Reset the state in case a new error is reported
      is_mmu_error_monitor_active = false;
      KEEPALIVE_STATE(IN_HANDLER);
      ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
    }
    return; // Always return to loop() to let MMU trigger a call to ReportErrorHook again
  } else if ((uint8_t)ReportErrorHookState == (uint8_t)ReportErrorHookStates::DISMISS_ERROR_SCREEN) {
    #ifdef HAS_WIRED_LCD
    ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_CALL_REDRAW_NEXT;
    ui.return_to_status();
    #endif
    sound_wait_for_user_reset();
    // Reset the state in case a new error is reported
    is_mmu_error_monitor_active = false;
    KEEPALIVE_STATE(IN_HANDLER);
    ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
  }
}

void ReportProgressHook(CommandInProgress cip, ProgressCode ec) {
  if (cip != CommandInProgress::NoCommand) {
    // custom_message_type = CustomMsg::MMUProgress;
    ui.set_status(ProgressCodeToText(ec));
  }
}

TryLoadUnloadReporter::TryLoadUnloadReporter(float delta_mm)
: dpixel0(0)
, dpixel1(0)
, lcd_cursor_col(0)
, pixel_per_mm(0.5F * float(LCD_WIDTH) / (delta_mm))
{
  // lcd_clearstatus();
  ui.reset_status();
}

TryLoadUnloadReporter::~TryLoadUnloadReporter() {
  #ifdef HAS_WIRED_LCD
  // Delay the next status message just so
  // the user can see the results clearly
  ui.set_status_no_expire(ui.status_message);
  #endif
}

void TryLoadUnloadReporter::Render(uint8_t col, bool sensorState) {
  #if HAS_WIRED_LCD
  // Set the cursor position each time in case some other
  // part of the firmware changes the cursor position
  lcd_insert_char_into_status(col, sensorState ? LCD_STR_SOLID_BLOCK[0] : '-');
  if (ui.lcdDrawUpdate == LCDViewAction::LCDVIEW_NONE)
    ui.draw_status_message(false);
  #endif
}

void TryLoadUnloadReporter::Progress(bool sensorState){
  // Always round up, you can only have 'whole' pixels. (floor is also an option)
  dpixel1 = ceil((stepper_get_machine_position_E_mm() - planner_get_current_position_E()) * pixel_per_mm);
  if (dpixel1 - dpixel0) {
    dpixel0 = dpixel1;
    if (lcd_cursor_col > (LCD_WIDTH - 1)) lcd_cursor_col = LCD_WIDTH - 1;
    Render(lcd_cursor_col++, sensorState);
  }
}

void TryLoadUnloadReporter::DumpToSerial(){
  char buf[LCD_WIDTH + 1];
  #ifdef HAS_WIRED_LCD
  ui.status_message.copyto(buf);
  #endif
  for (uint8_t i = 0; i < sizeof(buf); i++) {
    // 0xFF is -1 when converting from unsigned to signed char
    // If the number is negative, that means filament is present
    buf[i] = (buf[i] < 0) ? '1' : '0';
  }
  buf[LCD_WIDTH] = 0;
  MMU2_ECHO_MSGLN(buf);
}

/// Disables MMU in EEPROM
void DisableMMUInSettings() {
  mmu2.mmu_hw_enabled = false;

  #if ENABLED(EEPROM_SETTINGS)
  // save mmu_hw_enabled to eeprom
  persistentStore.access_start();
  persistentStore.write_data(mmu2.mmu_hw_enabled_addr, mmu2.mmu_hw_enabled);
  persistentStore.access_finish();
  settings.save();
  #endif

  mmu2.Status();
}

void IncrementLoadFails(){
  operation_statistics.increment_load_fails();
}

void IncrementMMUFails(){
  operation_statistics.increment_mmu_fails();
}

bool cutter_enabled(){
  return mmu2.cutter_mode > 0;
}

void MakeSound(SoundType s){
  Sound_MakeSound( (eSOUND_TYPE)s);
}

static void FullScreenMsg(const char *pgmS, uint8_t slot){
  #if HAS_WIRED_LCD
  ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_NONE;
  ui.clear_lcd();
  // START_SCREEN();
  // SETCURSOR(0, 1);
  lcd_moveto(0, 1);
  lcd_put_u8str_P(pgmS);
  // SETCURSOR(1, 0);
  lcd_moveto(1, 0);
  lcd_put_int(slot + 1);
  // END_SCREEN();
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  #endif
}

void FullScreenMsgCut(uint8_t slot){
  FullScreenMsg(MSG_CUT_FILAMENT, slot);
}

void FullScreenMsgEject(uint8_t slot){
  FullScreenMsg(MSG_EJECT_FROM_MMU, slot);
}

void FullScreenMsgTest(uint8_t slot){
  FullScreenMsg(MSG_TESTING_FILAMENT, slot);
}

void FullScreenMsgLoad(uint8_t slot){
  FullScreenMsg(MSG_LOADING_FILAMENT, slot);
}

void FullScreenMsgRestoringTemperature(){
  #ifdef HAS_WIRED_LCD
  lcd_display_message_fullscreen_P("MMU Retry: Restoring temperature..."); ////MSG_MMU_RESTORE_TEMP c=20 r=4
  #endif
}

void ScreenUpdateEnable(){
  #ifdef HAS_WIRED_LCD
  ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_CALL_REDRAW_NEXT;
  #endif
}

void ScreenClear(){
  ui.clear_lcd();
}

struct TuneItem {
  uint8_t address;
  uint8_t minValue;
  uint8_t maxValue;
} __attribute__((packed));

static const TuneItem TuneItems[] PROGMEM = {
  { (uint8_t)Register::Selector_sg_thrs_R, 1, 4},
  { (uint8_t)Register::Idler_sg_thrs_R, 2, 10},
};

static_assert(sizeof(TuneItems)/sizeof(TuneItem) == 2);


typedef struct
{
  //Variables used when editing values.
  const char* editLabel;
  uint8_t editValueBits; // 8 or 16
  void* editValuePtr;
  int16_t currentValue;
  int16_t minEditValue;
  int16_t maxEditValue;
  int16_t minJumpValue;
} menu_data_edit_t;

struct _menu_tune_data_t
{
  menu_data_edit_t reserved; //13 bytes reserved for number editing functions
  int8_t status;             // 1 byte
  uint8_t currentValue;      // 1 byte
  TuneItem item;             // 3 bytes
};

// static_assert(sizeof(_menu_tune_data_t) == 18);
// static_assert(sizeof(menu_data)>= sizeof(_menu_tune_data_t), "_menu_tune_data_t doesn't fit into menu_data");

void tuneIdlerStallguardThresholdMenu() {
  // const uint8_t menu_data[32] = "Set Stallguard Threshold";
  // // static constexpr _menu_tune_data_t * const _md = (_menu_tune_data_t*)&(menu_data[0]);
  // static constexpr _menu_tune_data_t * const _md = (_menu_tune_data_t*)&(menu_data[0]);

  // // Do not timeout the screen, otherwise there will be FW crash (menu recursion)
  // // lcd_timeoutToStatus.stop();
  // if (_md->status == 0)
  // {
  //     _md->status = 1; // Menu entered for the first time

  //     // Fetch the TuneItem from PROGMEM
  //     const uint8_t offset = (mmu2.MMUCurrentErrorCode() == ErrorCode::HOMING_IDLER_FAILED) ? 1 : 0;
  //     memcpy_P(&(_md->item), &TuneItems[offset], sizeof(TuneItem));

  //     // Fetch the value which is currently in MMU EEPROM
  //     mmu2.ReadRegister(_md->item.address);
  //     _md->currentValue = mmu2.GetLastReadRegisterValue();
  // }

  // // MENU_BEGIN();
  // // ON_MENU_LEAVE(
  // //     mmu2.WriteRegister(_md->item.address, (uint16_t)_md->currentValue);
  // //     putErrorScreenToSleep = false;
  // //     lcd_return_to_status();
  // //     return;
  // // );
  // // MENU_ITEM_BACK(MSG_DONE);
  // // MENU_ITEM_EDIT_int3_P(
  // //     _i("Sensitivity"), ////MSG_MMU_SENSITIVITY c=18
  // //     &_md->currentValue,
  // //     _md->item.minValue,
  // //     _md->item.maxValue
  // // );
  // // MENU_END();

  // START_MENU();
  // BACK_ITEM(MSG_BACK);
  // EDIT_ITEM(
  //     int8,
  //     MSG_MMU_SENSITIVITY,
  //     &_md->currentValue,
  //     _md->item.minValue,
  //     _md->item.maxValue,
  //     []{
  //         write_register_and_return_to_status_menu(_md->item.address, _md->currentValue);
  //     }
  //     );
  // END_MENU();
}


void write_register_and_return_to_status_menu(uint8_t address, uint8_t value){
  mmu2.WriteRegister(address, (uint16_t)value);
  putErrorScreenToSleep = false;
  ui.return_to_status();
  return;
}


void tuneIdlerStallguardThreshold() {
  putErrorScreenToSleep = true;
  // menu_submenu(tuneIdlerStallguardThresholdMenu);
}

} // namespace MMU2
#endif // HAS_PRUSA_MMU3
