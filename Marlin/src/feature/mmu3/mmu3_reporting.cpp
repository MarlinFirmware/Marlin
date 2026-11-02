/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * mmu3_reporting.cpp
 */

#include "../../inc/MarlinConfig.h"

#if HAS_PRUSA_MMU3

#include "mmu3.h"
#include "mmu3_log.h"
#include "mmu3_fsensor.h"
#include "mmu3_reporting.h"
#include "mmu3_error_converter.h"
#include "mmu3_marlin_macros.h"
#include "mmu3_progress_converter.h"
#include "mmu_hw/buttons.h"
#include "mmu_hw/error_codes.h"
#include "mmu_hw/errors_list.h"
#include "ultralcd.h"
#include "sound.h"

#include "../../core/language.h"
#include "../../gcode/gcode.h"
#include "../host_actions.h"
#include "../../lcd/marlinui.h"
#include "../../lcd/menu/menu.h"
#include "../../lcd/menu/menu_item.h"
#include "../../module/temperature.h"

namespace MMU3 {

  OperationStatistics operation_statistics;

  uint16_t OperationStatistics::fail_total_num;     // total failures
  uint8_t OperationStatistics::fail_num;            // fails during print
  uint16_t OperationStatistics::load_fail_total_num; // total load failures
  uint8_t OperationStatistics::load_fail_num;       // load failures during print
  uint16_t OperationStatistics::tool_change_counter; // number of tool changes per print
  uint32_t OperationStatistics::tool_change_total_counter; // number of total tool changes
  int OperationStatistics::fail_total_num_addr;     // total failures EEPROM addr
  int OperationStatistics::fail_num_addr;           // fails during print EEPROM addr
  int OperationStatistics::load_fail_total_num_addr; // total load failures EEPROM addr
  int OperationStatistics::load_fail_num_addr;      // load failures during print EEPROM addr
  int OperationStatistics::tool_change_counter_addr; // number of total tool changes EEPROM addr
  int OperationStatistics::tool_change_total_counter_addr; // number of total tool changes EEPROM addr

  /**
   * Increment both the total load fails and Per print job load fails.
  */
  void OperationStatistics::increment_load_fails() {
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
  void OperationStatistics::increment_mmu_fails() {
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
  void OperationStatistics::increment_tool_change_counter() {
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
   * @return true if everything went okay, false otherwise.
  */
  bool OperationStatistics::reset_per_print_stats() {
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
   * Reset fail statistics and update EEPROM.
   *
   * This will keep the tool change counter change counters and delete anything
   * else.
   *
   * @return true if everything went okay, false otherwise.
  */
  bool OperationStatistics::reset_fail_stats() {
    // Update data
    load_fail_num = 0;
    load_fail_total_num = 0;
    fail_num = 0;
    fail_total_num = 0;

    #if ENABLED(EEPROM_SETTINGS)
      // Update EEPROM
      persistentStore.access_start();
      persistentStore.write_data(load_fail_num_addr, load_fail_num);
      persistentStore.write_data(load_fail_total_num_addr, load_fail_total_num);
      persistentStore.write_data(fail_num_addr, fail_num);
      persistentStore.write_data(fail_total_num_addr, fail_total_num);
      persistentStore.access_finish();
      return settings.save();
    #else
      return true;
    #endif
  }


  /**
   * Reset all operation statistics and update EEPROM.
   *
   * @return true if everything went okay, false otherwise.
  */
  bool OperationStatistics::reset_stats() {
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
    if (!marlin.printJobOngoing()) ui.reset_status();
    //custom_message_type = CustomMsg::Status;
  }

  /**
   * @brief Renders any characters that will be updated live on the MMU error screen.
   *Currently, this is FINDA and Filament Sensor status and Extruder temperature.
   */
  extern void ReportErrorHookDynamicRender(void) {
    #if HAS_WIRED_LCD
      // beware - this optimization abuses the fact, that findaDetectsFilament returns 0 or 1 and '0' is followed by '1' in the ASCII table
      lcd_put_int(3, LCD_HEIGHT - 1, mmu3.findaDetectsFilament() + '0');
      lcd_put_int(8, LCD_HEIGHT - 1, FILAMENT_PRESENT() + '0');

      // print active/changing filament slot
      lcd_moveto(10, LCD_HEIGHT - 1);
      lcdui_print_extruder();

      // Print active extruder temperature
      lcd_put_int(16, LCD_HEIGHT - 1, (int)(thermalManager.degHotend(0) + 0.5));
    #endif
  }

  static FSTR_P msg_page = nullptr;  // Start of the page being shown (nullptr = start of the message)
  static FSTR_P msg_rest = nullptr;  // Text left over after the page being shown

  /**
   * Display more info about the error. If the error message doesn't fit into
   * the screen, clicking the LCD button will go to the next screen to display
   * the rest of the message, until no messages left to display and a final
   * click will return to the previous screen.
   *
   * This gets the message data from the "editable.uint8" which is set in the
   * action item.
   *
   * On a graphical LCD the screen handler is called once for every page of the
   * display, so drawing must not change any state. State only changes on a click.
   */
  void show_more_info_screen() {
    #if HAS_WIRED_LCD
      if (ui.use_click()) {
        if (!msg_rest || !pgm_read_byte(FTOP(msg_rest))) {
          // Nothing more to show, so return to the error screen.
          msg_page = msg_rest = nullptr;
          // Prevent this function being triggered again...
          SetButtonResponse(ButtonOperations::NoOperation);
          return ui.go_back();
        }
        msg_page = msg_rest;
      }
      msg_rest = lcd_display_message_fullscreen(msg_page ?: PrusaErrorDesc(editable.uint8));
      // Set the button response to MoreInfo so we keep coming back to this screen until all messages are consumed
      SetButtonResponse(ButtonOperations::MoreInfo);
    #else
      // no lcd, no error display... just break the loop...
      msg_page = msg_rest = nullptr;
      SetButtonResponse(ButtonOperations::NoOperation);
    #endif // HAS_WIRED_LCD
  }

  /**
   * @brief Renders any characters that are static on the MMU error screen i.e. they don't change.
   * @param[in] ei Error code index
   */
  static void ReportErrorHookStaticRender(uint8_t ei) {
    #if HAS_WIRED_LCD
      //! Show an error screen
      //! When an MMU error occurs, the LCD content will look like this:
      //! |01234567890123456789|
      //! |MMU FW update needed|     <- title/header of the error: max 20 characters
      //! |prusa.io/04504      |     <- URL max 20 characters
      //! |FI:1 FS:1  5>3 t201°|     <- status line, t is thermometer symbol
      //! |>Retry   >Done    >W|     <- buttons
      bool two_choices = false;

      // Read and determine what operations should be shown on the menu
      const uint8_t button_operation = PrusaErrorButtons(ei),
                    button_op_right  = BUTTON_OP_RIGHT(button_operation),
                    button_op_middle = BUTTON_OP_MIDDLE(button_operation);

      // Check if the menu should have three or two choices
      if (button_op_right == (uint8_t)ButtonOperations::NoOperation) {
        // Two operations not specified, the error menu should only show two choices
        two_choices = true;
      }

      START_MENU();
      #ifndef __AVR__
        // TODO: I couldn't make this work on AVR
        STATIC_ITEM_F(PrusaErrorTitle(ei), SS_DEFAULT | SS_INVERT);

        // Write the help page and error code
        MString<LCD_WIDTH> url("");
        url.appendf("prusa.io/04%hu", PrusaErrorCode(ei));
        STATIC_ITEM_F(nullptr, SS_DEFAULT, url.buffer());

        //ReportErrorHookSensorLineRender();

        editable.uint8 = button_op_middle;
        ACTION_ITEM_F(
          PrusaErrorButtonTitle(button_op_middle),
          []{ SetButtonResponse((ButtonOperations)editable.uint8); }
        );

        if (!two_choices) {
          editable.uint8 = button_op_right;
          ACTION_ITEM_F(
            PrusaErrorButtonTitle(button_op_right),
            []{ SetButtonResponse((ButtonOperations)editable.uint8); }
          );
        }

        // Add a More Info option
        editable.uint8 = ei;
        ACTION_ITEM_F(
          GET_TEXT_F(MSG_BTN_MORE),
          []{
            // only when the menu item is used push the current screen back
            ui.push_current_screen();
            msg_page = msg_rest = nullptr;
            SetButtonResponse(ButtonOperations::MoreInfo);
          }
        );

      #endif // !__AVR__

      // Render the choices
      //if (two_choices) {
      //  lcd_show_choices_prompt_P(
      //    LCD_LEFT_BUTTON_CHOICE,
      //    PrusaErrorButtonTitle(button_op_middle),
      //    GET_TEXT(MSG_BTN_MORE),
      //    18, nullptr
      //  );
      //}
      //else {
      //  lcd_show_choices_prompt_P(
      //    LCD_MIDDLE_BUTTON_CHOICE,
      //    PrusaErrorButtonTitle(button_op_middle),
      //    PrusaErrorButtonTitle(button_op_right),
      //    9, GET_TEXT(MSG_BTN_MORE)
      //  );
      //}

      END_MENU();
      //ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
    #endif // HAS_WIRED_LCD
  }

  void ReportErrorHookSensorLineRender() {
    #if HAS_WIRED_LCD
      // Render static characters in third line
      lcd_put_u8str(
        0,
        LCD_HEIGHT - 1,
        F("FI:  FS:    >  " LCD_STR_THERMOMETER "   " LCD_STR_DEGREE)
      );
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
    if (GetButtonResponse() == ButtonOperations::MoreInfo) {
      SetButtonResponse(ButtonOperations::NoOperation);
      ret = 1;
    }
    else if (GetButtonResponse() != ButtonOperations::NoOperation) {
      ret = 2;
    }
    // Next MMU error screen should reset the choice selection
    // reset_button_selection = 1;
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
      ReportErrorHook((CommandInProgress)mmu3.getCommandInProgress(), mmu3.getLastErrorCode(), mmu3.mmuLastErrorSource());
    }
  }

  bool TuneMenuEntered() {
    return putErrorScreenToSleep;
  }

  void ReportErrorHook(CommandInProgress /*cip*/, ErrorCode ec, uint8_t /*es*/) {
    if (putErrorScreenToSleep) return;

    if (mmu3.mmuCurrentErrorCode() == ErrorCode::OK && mmu3.mmuLastErrorSource() == MMU3::ErrorSourceMMU) {
      // If the error code suddenly changes to OK, that means
      // a button was pushed on the MMU and the LCD should
      // dismiss the error screen until MMU raises a new error
      ReportErrorHookState = ReportErrorHookStates::DISMISS_ERROR_SCREEN;
      msg_page = msg_rest = nullptr;
    }

    const uint8_t ei = PrusaErrorCodeIndex((ErrorCode)ec);

    // This should be the equivalent of the switch..case above...
    if ((uint8_t)ReportErrorHookState == (uint8_t)ReportErrorHookStates::RENDER_ERROR_SCREEN) {
      KEEPALIVE_STATE(PAUSED_FOR_USER);
      #if HAS_WIRED_LCD
        msg_page = msg_rest = nullptr;
        editable.uint8 = ei;
        ui.defer_status_screen();
        ui.goto_screen([]{ ReportErrorHookStaticRender(editable.uint8); });
      #endif
      ReportErrorHookState = ReportErrorHookStates::MONITOR_SELECTION;
    }

    if ((uint8_t)ReportErrorHookState == (uint8_t)ReportErrorHookStates::MONITOR_SELECTION) {
      is_mmu_error_monitor_active = true;
      // ReportErrorHookDynamicRender(); // Render dynamic characters
      sound_wait_for_user();
      uint8_t result = ReportErrorHookMonitor(ei);
      if (result == 0) {
        // No choice selected, return to loop()
      }
      else if (result == 1) {
        // More Info button selected, change state
        editable.uint8 = ei;
        //ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
        ui.goto_screen(show_more_info_screen);
        ReportErrorHookState = ReportErrorHookStates::MONITOR_SELECTION;
      }
      else if (result == 2) {
        // Exit error screen and enable lcd updates
        TERN_(HAS_WIRED_LCD, ui.return_to_status());
        sound_wait_for_user_reset();
        // Reset the state in case a new error is reported
        is_mmu_error_monitor_active = false;
        KEEPALIVE_STATE(IN_HANDLER);
        ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
      }
      return; // Always return to loop() to let MMU trigger a call to ReportErrorHook again
    }
    else if ((uint8_t)ReportErrorHookState == (uint8_t)ReportErrorHookStates::DISMISS_ERROR_SCREEN) {
      TERN_(HAS_WIRED_LCD, ui.return_to_status());
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
    : dpixel0(0), dpixel1(0), lcd_cursor_col(0)
    , pixel_per_mm(0.5F * float(LCD_WIDTH) / (delta_mm)
  ) {
    //lcd_clearstatus();
    ui.reset_status();
  }

  TryLoadUnloadReporter::~TryLoadUnloadReporter() {
    #if HAS_WIRED_LCD
      // Delay the next status message just so
      // the user can see the results clearly
      ui.set_status_no_expire(ui.status_message);
    #endif
  }

  void TryLoadUnloadReporter::Render(uint8_t col, bool sensorState) {
    #if HAS_WIRED_LCD
      // Set the cursor position each time in case some other
      // part of the firmware changes the cursor position
      lcd_replace_status_char(col, sensorState ? LCD_STR_SOLID_BLOCK[0] : '-');
      if (ui.lcdDrawUpdate == LCDViewAction::LCDVIEW_NONE)
        ui.draw_status_message(false);
    #endif
  }

  void TryLoadUnloadReporter::Progress(bool sensorState) {
    // Always round up, you can only have 'whole' pixels. (floor is also an option)
    dpixel1 = ceil((stepper_get_machine_position_E_mm() - planner_get_current_position_E()) * pixel_per_mm);
    if (dpixel1 != dpixel0) {
      dpixel0 = dpixel1;
      if (lcd_cursor_col > (LCD_WIDTH - 1)) lcd_cursor_col = LCD_WIDTH - 1;
      Render(lcd_cursor_col++, sensorState);
    }
  }

  void TryLoadUnloadReporter::DumpToSerial() {
    char buf[LCD_WIDTH + 1] = {};
    TERN_(HAS_WIRED_LCD, ui.status_message.copyto(buf, LCD_WIDTH));
    for (uint8_t i = 0; i < sizeof(buf); i++) {
      // 0xFF is -1 when converting from unsigned to signed char
      // If the number is negative, that means filament is present
      buf[i] = (buf[i] < 0) ? '1' : '0';
    }
    buf[LCD_WIDTH] = 0;
    TERN_(MMU_DEBUG, MMU2_ECHO_MSGLN(buf));
  }

  void IncrementLoadFails() {
    operation_statistics.increment_load_fails();
  }

  void IncrementMMUFails() {
    operation_statistics.increment_mmu_fails();
  }

  bool cutter_enabled() {
    return mmu3.cutter_mode > 0;
  }

  void MakeSound(SoundType s) {
    Sound_MakeSound((eSOUND_TYPE)s);
  }

  static void fullScreenMsg(FSTR_P const fstr, uint8_t slot) {
    #if HAS_WIRED_LCD
      ui.clear_lcd();
      #ifndef __AVR__
        SETCURSOR(0, 1);
        lcd_put_u8str(fstr);
        lcd_put_lchar(' ');
        lcd_put_int(slot + 1);
      #else
        UNUSED(fstr);
      #endif
      ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
      ui.screen_changed = true;
    #endif
  }

  void fullScreenMsgCut(uint8_t slot)   { fullScreenMsg(GET_TEXT_F(MSG_CUT_FILAMENT), slot); }
  void fullScreenMsgEject(uint8_t slot) { fullScreenMsg(GET_TEXT_F(MSG_EJECT_FROM_MMU), slot); }
  void fullScreenMsgTest(uint8_t slot)  { fullScreenMsg(GET_TEXT_F(MSG_TESTING_FILAMENT), slot); }
  void fullScreenMsgLoad(uint8_t slot)  { fullScreenMsg(GET_TEXT_F(MSG_LOADING_FILAMENT), slot); }

  void fullScreenMsgRestoringTemperature() {
    #if HAS_WIRED_LCD
      // Show the status screen (with live temperatures) so the error menu can't be clicked again
      ui.return_to_status();
      ui.set_status(F("MMU Retry: Heating..."), true);
    #endif
  }

  void ScreenUpdateEnable() {
    TERN_(HAS_WIRED_LCD, ui.refresh(LCDVIEW_CALL_REDRAW_NEXT));
  }

  void ScreenClear() { ui.clear_lcd(); }

  struct TuneItem {
    uint8_t address;
    uint8_t minValue;
    uint8_t maxValue;
  }
  __attribute__((packed));

  static const TuneItem TuneItems[] PROGMEM = {
    { (uint8_t)Register::Selector_sg_thrs_R, 1, 4 },
    { (uint8_t)Register::Idler_sg_thrs_R, 2, 10 },
  };

  static_assert(COUNT(TuneItems) == 2);

  // Stallguard sensitivity being edited by the Tune screen
  static TuneItem tune_item;
  static uint8_t tune_value;

  // Close the Tune screen and let the error screen come back
  static bool tune_finish_pending;
  static void tune_done() {
    putErrorScreenToSleep = false;
    tune_finish_pending = true; // manage_response() will apply the edit and raise the error screen again
    TERN_(HAS_WIRED_LCD, ui.return_to_status());
  }

  // The register write is deferred: the menu callback runs inside ui.update(), and the write
  // needs idle() to run the MMU protocol, which would re-enter this very menu.
  static bool tune_write_pending;
  static uint8_t tune_write_address, tune_write_value;

  void write_register_and_return_to_status_menu(uint8_t address, uint8_t value) {
    tune_write_address = address;
    tune_write_value = value;
    tune_write_pending = true;
    tune_done();
  }

  void TuneApplyPendingWrite() {
    if (!tune_finish_pending) return;
    tune_finish_pending = false;
    if (tune_write_pending) {
      tune_write_pending = false;
      mmu3.writeRegister(tune_write_address, (uint16_t)tune_write_value);
    }
    // The MMU is still in the same error, which it won't report as new, so bring the error screen back.
    // The pressed Tune button was consumed elsewhere, so the screen still thinks it is being monitored.
    ReportErrorHookState = ReportErrorHookStates::RENDER_ERROR_SCREEN;
    ReportErrorHook((CommandInProgress)mmu3.getCommandInProgress(), mmu3.getLastErrorCode(), mmu3.mmuLastErrorSource());
  }

  #if HAS_WIRED_LCD
    // Edit the Stallguard threshold of the axis that failed to home. The new value is
    // written to the MMU when the edit is confirmed. Back leaves it alone. Either way
    // the error screen returns so the user can press Retry.
    void tuneIdlerStallguardThresholdMenu() {
      START_MENU();
      ACTION_ITEM_F(GET_TEXT_F(MSG_BACK), tune_done);
      EDIT_ITEM(uint8, MSG_MMU_SENSITIVITY, &tune_value, tune_item.minValue, tune_item.maxValue, []{
        write_register_and_return_to_status_menu(tune_item.address, tune_value);
      });
      END_MENU();
    }
  #endif

  void tuneIdlerStallguardThreshold() {
    // The Selector and the Idler each have their own threshold
    const uint8_t offset = (mmu3.mmuCurrentErrorCode() == ErrorCode::HOMING_IDLER_FAILED) ? 1 : 0;
    memcpy_P(&tune_item, &TuneItems[offset], sizeof(TuneItem));

    // Fetch the value currently stored in the MMU
    if (!mmu3.readRegister(tune_item.address)) return;
    tune_value = _MIN(_MAX((uint8_t)mmu3.getLastReadRegisterValue(), tune_item.minValue), tune_item.maxValue);

    #if HAS_WIRED_LCD
      putErrorScreenToSleep = true;
      ui.goto_screen(tuneIdlerStallguardThresholdMenu);
    #endif
  }

} // MMU3

#endif // HAS_PRUSA_MMU3
