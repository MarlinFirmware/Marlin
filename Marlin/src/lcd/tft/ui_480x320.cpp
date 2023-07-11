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

#include "../../inc/MarlinConfigPre.h"

#if HAS_UI_480x320 || HAS_UI_480x272

#include "ui_common.h"

#include "../marlinui.h"
#include "../menu/menu.h"
#include "../../libs/numtostr.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../../module/motion.h"

#if DISABLED(LCD_PROGRESS_BAR) && ALL(FILAMENT_LCD_DISPLAY, HAS_MEDIA)
  #include "../../feature/filwidth.h"
  #include "../../gcode/parser.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

void MarlinUI::tft_idle() {
  #if ENABLED(TOUCH_SCREEN)
    if (TERN0(HAS_TOUCH_SLEEP, lcd_sleep_task())) return;
    if (draw_menu_navigation) {
      add_control(TFT_WIDTH / 6 - 16, TFT_HEIGHT - 34, PAGE_UP, imgPageUp, encoderTopLine > 0);
      add_control(TFT_WIDTH / 2 - 16, TFT_HEIGHT - 34, PAGE_DOWN, imgPageDown, encoderTopLine + LCD_HEIGHT < screen_items);
      add_control(5 * TFT_WIDTH / 6 - 16, TFT_HEIGHT - 34, BACK, imgBack);
      draw_menu_navigation = false;
    }
  #endif

  tft.queue.async();

  TERN_(TOUCH_SCREEN, if (tft.queue.is_empty()) touch.idle()); // Touch driver is not DMA-aware, so only check for touch controls after screen drawing is completed
}

#if ENABLED(SHOW_BOOTSCREEN)

  void MarlinUI::show_bootscreen() {
    tft.queue.reset();

    tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
    #if ENABLED(BOOT_MARLIN_LOGO_SMALL)
      #define BOOT_LOGO_W 195   // MarlinLogo195x59x16
      #define BOOT_LOGO_H  59
      #define SITE_URL_Y (TFT_HEIGHT - 70)
      tft.set_background(COLOR_BACKGROUND);
    #else
      #define BOOT_LOGO_W TFT_WIDTH   // MarlinLogo480x320x16
      #define BOOT_LOGO_H TFT_HEIGHT
      #define SITE_URL_Y (TFT_HEIGHT - 90)
    #endif
    tft.add_image((TFT_WIDTH - BOOT_LOGO_W) / 2, (TFT_HEIGHT - BOOT_LOGO_H) / 2, imgBootScreen);
    #ifdef WEBSITE_URL
      tft_string.set(WEBSITE_URL);
      tft.add_text(tft_string.center(TFT_WIDTH), SITE_URL_Y, COLOR_WEBSITE_URL, tft_string);
    #endif

    tft.queue.sync();
  }

  void MarlinUI::bootscreen_completion(const millis_t sofar) {
    if ((BOOTSCREEN_TIMEOUT) > sofar) safe_delay((BOOTSCREEN_TIMEOUT) - sofar);
    clear_lcd();
  }

#endif

void MarlinUI::draw_kill_screen() {
  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_KILL_SCREEN_BG);

  uint16_t line = 2;

  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  line++;
  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_HALTED));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_PLEASE_RESET));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  tft.queue.sync();
}

void draw_heater_status(uint16_t x, uint16_t y, const int8_t heater) {
  MarlinImage image = imgHotEnd;
  celsius_t currentTemperature, targetTemperature;

  if (heater >= 0) { // HotEnd
    #if HAS_EXTRUDERS
      currentTemperature = thermalManager.wholeDegHotend(heater);
      targetTemperature = thermalManager.degTargetHotend(heater);
    #else
      return;
    #endif
  }
  #if HAS_HEATED_BED
    else if (heater == H_BED) {
      currentTemperature = thermalManager.wholeDegBed();
      targetTemperature = thermalManager.degTargetBed();
    }
  #endif
  #if HAS_TEMP_CHAMBER
    else if (heater == H_CHAMBER) {
      currentTemperature = thermalManager.wholeDegChamber();
      #if HAS_HEATED_CHAMBER
        targetTemperature = thermalManager.degTargetChamber();
      #else
        targetTemperature = ABSOLUTE_ZERO;
      #endif
    }
  #endif
  #if HAS_TEMP_COOLER
    else if (heater == H_COOLER) {
      currentTemperature = thermalManager.wholeDegCooler();
      targetTemperature = TERN(HAS_COOLER, thermalManager.degTargetCooler(), ABSOLUTE_ZERO);
    }
  #endif
  else return;

  TERN_(TOUCH_SCREEN, if (targetTemperature >= 0) touch.add_control(HEATER, x, y, 80, 120, heater));
  tft.canvas(x, y, 80, 120);
  tft.set_background(COLOR_BACKGROUND);

  uint16_t color = currentTemperature < 0 ? COLOR_INACTIVE : COLOR_COLD;

  if (heater >= 0) { // HotEnd
    if (currentTemperature >= 50) color = COLOR_HOTEND;
  }
  #if HAS_HEATED_BED
    else if (heater == H_BED) {
      if (currentTemperature >= 50) color = COLOR_HEATED_BED;
      image = targetTemperature > 0 ? imgBedHeated : imgBed;
    }
  #endif
  #if HAS_TEMP_CHAMBER
    else if (heater == H_CHAMBER) {
      if (currentTemperature >= 50) color = COLOR_CHAMBER;
      image = targetTemperature > 0 ? imgChamberHeated : imgChamber;
    }
  #endif
  #if HAS_TEMP_COOLER
    else if (heater == H_COOLER) {
      if (currentTemperature <= 26) color = COLOR_COLD;
      if (currentTemperature > 26) color = COLOR_RED;
      image = targetTemperature > 26 ? imgCoolerHot : imgCooler;
    }
  #endif

  tft.add_image(8, 28, image, color);

  tft_string.set(i16tostr3rj(currentTemperature));
  tft_string.add(LCD_STR_DEGREE);
  tft_string.trim();
  tft.add_text(tft_string.center(80) + 2, 80 + tft_string.vcenter(FONT_LINE_HEIGHT), color, tft_string);

  if (targetTemperature >= 0) {
    tft_string.set(i16tostr3rj(targetTemperature));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(80) + 2, 6 + tft_string.vcenter(FONT_LINE_HEIGHT), color, tft_string);
  }
}

void draw_fan_status(uint16_t x, uint16_t y, const bool blink) {
  TERN_(TOUCH_SCREEN, touch.add_control(FAN, x, y, 80, 120));
  tft.canvas(x, y, 80, 120);
  tft.set_background(COLOR_BACKGROUND);

  uint8_t fanSpeed = thermalManager.fan_speed[0];
  MarlinImage image;

  if (fanSpeed >= 127)
    image = blink ? imgFanFast1 : imgFanFast0;
  else if (fanSpeed > 0)
    image = blink ? imgFanSlow1 : imgFanSlow0;
  else
    image = imgFanIdle;

  tft.add_image(8, 20, image, COLOR_FAN);

  tft_string.set(ui8tostr4pctrj(thermalManager.fan_speed[0]));
  tft_string.trim();
  tft.add_text(tft_string.center(80) + 6, 80 + tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_FAN, tft_string);
}

void MarlinUI::draw_status_screen() {
  const bool blink = get_blink();

  TERN_(TOUCH_SCREEN, touch.clear());

  // Statuses of heaters and fans
  uint16_t y = STATUS_MARGIN_SIZE;
  for (uint16_t i = 0; i < ITEMS_COUNT; i++) {
    uint16_t x = (TFT_WIDTH / ITEMS_COUNT - 80) / 2  + (TFT_WIDTH * i / ITEMS_COUNT);
    switch (i) {
      #if HAS_EXTRUDERS
        case ITEM_E0: draw_heater_status(x, y, H_E0); break;
      #endif
      #if HAS_MULTI_HOTEND
        case ITEM_E1: draw_heater_status(x, y, H_E1); break;
      #endif
      #if HOTENDS > 2
        case ITEM_E2: draw_heater_status(x, y, H_E2); break;
      #endif
      #if HAS_HEATED_BED
        case ITEM_BED: draw_heater_status(x, y, H_BED); break;
      #endif
      #if HAS_TEMP_CHAMBER
        case ITEM_CHAMBER: draw_heater_status(x, y, H_CHAMBER); break;
      #endif
      #if HAS_TEMP_COOLER
        case ITEM_COOLER: draw_heater_status(x, y, H_COOLER); break;
      #endif
      #if HAS_FAN
        case ITEM_FAN: draw_fan_status(x, y, blink); break;
      #endif
    }
  }

  y += STATUS_MARGIN_SIZE + 114;

  // Coordinates
  constexpr uint16_t coords_width = TFT_WIDTH - 8;
  tft.canvas((TFT_WIDTH - coords_width) / 2, y, coords_width, FONT_LINE_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_rectangle(0, 0, coords_width, FONT_LINE_HEIGHT, COLOR_AXIS_HOMED);

  if (TERN0(LCD_SHOW_E_TOTAL, printingIsActive())) {
    #if ENABLED(LCD_SHOW_E_TOTAL)
      tft.add_text(16, tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_AXIS_HOMED, "E");
      const uint8_t escale = e_move_accumulator >= 100000.0f ? 10 : 1; // After 100m switch to cm
      tft_string.set(ftostr4sign(e_move_accumulator / escale));
      tft_string.add(escale == 10 ? 'c' : 'm');
      tft_string.add('m');
      tft.add_text(192 - tft_string.width(), tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_AXIS_HOMED, tft_string);
    #endif
  }
  else {
    // Coords in mask "X____Y____Z____"
    #if HAS_X_AXIS
      tft_string.set("X");
      tft.add_text(coords_width / 30 - tft_string.width() / 2, tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_AXIS_HOMED, tft_string);

      const bool nhx = axis_should_home(X_AXIS);
      tft_string.set(blink && nhx ? "?" : ftostr4sign(LOGICAL_X_POSITION(current_position.x)));
      tft_string.ltrim();
      tft.add_text(coords_width / 5 - tft_string.width() / 2, tft_string.vcenter(FONT_LINE_HEIGHT), nhx ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);
    #endif

    #if HAS_Y_AXIS
      tft_string.set("Y");
      tft.add_text(11 * coords_width / 30 - tft_string.width() / 2, tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_AXIS_HOMED, tft_string);

      const bool nhy = axis_should_home(Y_AXIS);
      tft_string.set(blink && nhy ? "?" : ftostr4sign(LOGICAL_Y_POSITION(current_position.y)));
      tft_string.ltrim();
      tft.add_text(8 * coords_width / 15 - tft_string.width() / 2, tft_string.vcenter(FONT_LINE_HEIGHT), nhy ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);
    #endif
  }

  #if HAS_Z_AXIS
    tft_string.set("Z");
    tft.add_text(7 * coords_width / 10 - tft_string.width() / 2, tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_AXIS_HOMED, tft_string);

    const bool nhz = axis_should_home(Z_AXIS);
    tft_string.set(blink && nhz ? "?" : ftostr52sp(LOGICAL_Z_POSITION(current_position.z)));
    tft_string.ltrim();
    tft_string.rtrim();
    tft.add_text(13 * coords_width / 15 - tft_string.width() / 2, tft_string.vcenter(FONT_LINE_HEIGHT), nhz ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);
  #endif

  TERN_(TOUCH_SCREEN, touch.add_control(MOVE_AXIS, 4, y, TFT_WIDTH - 8, FONT_LINE_HEIGHT));

  y += STATUS_MARGIN_SIZE + 34;

  // Feed rate (preparing)
  tft_string.set(i16tostr3rj(feedrate_percentage));
  tft_string.add("%");
  uint16_t component_width = 36 + tft_string.width(); // 32px icon size + 4px margin before text
  uint16_t color = feedrate_percentage == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;
  uint16_t x = FEEDRATE_X(component_width);

  // Feed rate (drawing)
  tft.canvas(x, y, component_width, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_image(0, 0, imgFeedRate, color);
  tft.add_text(36, tft_string.vcenter(30), color, tft_string);
  TERN_(TOUCH_SCREEN, touch.add_control(FEEDRATE, x, y, component_width, 32));

  #if HAS_EXTRUDERS
    // Flow rate (preparing)
    tft_string.set(i16tostr3rj(planner.flow_percentage[active_extruder]));
    tft_string.add('%');
    component_width = 36 + tft_string.width();
    color = planner.flow_percentage[0] == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;
    x = FLOWRATE_X(component_width);

    // Flow rate (drawing)
    tft.canvas(x, y, component_width, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgFlowRate, color);
    tft.add_text(36, tft_string.vcenter(30), color, tft_string);
    TERN_(TOUCH_SCREEN, touch.add_control(FLOWRATE, x, y, component_width, 32, active_extruder));
  #endif

  y += TERN(HAS_UI_480x272, 36, 44);

  const progress_t progress = TERN(HAS_PRINT_PROGRESS_PERMYRIAD, get_progress_permyriad, get_progress_percent)();
  #if ENABLED(SHOW_ELAPSED_TIME) && DISABLED(SHOW_REMAINING_TIME)
    // Print duration so far (time elapsed) - centered
    char elapsed_str[22];
    duration_t elapsed = print_job_timer.duration();
    elapsed.toString(elapsed_str);

    // Same width constraints as feedrate/flowrate controls
    constexpr uint16_t time_str_width = 288, image_width = 36;

    tft.canvas((TFT_WIDTH - time_str_width) / 2, y, time_str_width, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(elapsed_str);
    uint16_t text_pos_x = tft_string.center(time_str_width - image_width);
    tft.add_image(text_pos_x, 0, imgTimeElapsed, COLOR_PRINT_TIME);
    tft.add_text(text_pos_x + image_width, tft_string.vcenter(29), COLOR_PRINT_TIME, tft_string);

  #elif DISABLED(SHOW_ELAPSED_TIME) && ENABLED(SHOW_REMAINING_TIME)
    // Print time remaining estimation - centered
    char estimate_str[22];
    duration_t elapsed = print_job_timer.duration();

    // Get the estimate, first from M73
    uint32_t estimate_remaining = (0
      #if ALL(SET_PROGRESS_MANUALLY, SET_REMAINING_TIME)
        + get_remaining_time()
      #endif
    );
    // If no M73 estimate is available but we have progress data, calculate time remaining assuming time elapsed is linear with progress
    if (!estimate_remaining && progress > 0)
      estimate_remaining = elapsed.value * (100 * (PROGRESS_SCALE) - progress) / progress;

    // Generate estimate string
    if (!estimate_remaining)
      tft_string.set("-");
    else {
      duration_t estimation = estimate_remaining;
      estimation.toString(estimate_str);
      tft_string.set(estimate_str);
    }

    // Same width constraints as feedrate/flowrate controls
    constexpr uint16_t time_str_width = 288, image_width = 36;

    tft.canvas((TFT_WIDTH - time_str_width) / 2, y, time_str_width, 32);
    tft.set_background(COLOR_BACKGROUND);
    color = printingIsActive() ? COLOR_PRINT_TIME : COLOR_INACTIVE;
    uint16_t text_pos_x = tft_string.center(time_str_width - image_width);
    tft.add_image(text_pos_x, 0, imgTimeRemaining, color);
    tft.add_text(text_pos_x + image_width, tft_string.vcenter(29), color, tft_string);

  #elif ALL(SHOW_REMAINING_TIME, SHOW_ELAPSED_TIME)
    // Print duration so far (time elapsed) - aligned under feed rate
    char elapsed_str[18];
    duration_t elapsed = print_job_timer.duration();
    elapsed.toCompactString(elapsed_str);

    tft.canvas(96, y, 144, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgTimeElapsed, COLOR_PRINT_TIME);
    tft_string.set(elapsed_str);
    tft.add_text(36, tft_string.vcenter(29), COLOR_PRINT_TIME, tft_string);

    // Print time remaining estimation - aligned under flow rate
    char estimate_str[18];

    // Get the estimate, first from M73
    uint32_t estimate_remaining = (0
      #if ALL(SET_PROGRESS_MANUALLY, SET_REMAINING_TIME)
        + get_remaining_time()
      #endif
    );
    // If no M73 estimate is available but we have progress data, calculate time remaining assuming time elapsed is linear with progress
    if (!estimate_remaining && progress > 0)
      estimate_remaining = elapsed.value * (100 * (PROGRESS_SCALE) - progress) / progress;

    // Generate estimate string
    if (!estimate_remaining)
      tft_string.set("-");
    else {
      duration_t estimation = estimate_remaining;
      estimation.toCompactString(estimate_str);
      tft_string.set(estimate_str);
    }

    // Push out the estimate to the screen
    tft.canvas(256, y, 144, 32);
    tft.set_background(COLOR_BACKGROUND);
    color = printingIsActive() ? COLOR_PRINT_TIME : COLOR_INACTIVE;
    tft.add_image(0, 0, imgTimeRemaining, color);
    tft.add_text(36, tft_string.vcenter(29), color, tft_string);
  #endif

  y += TERN(HAS_UI_480x272, 36, 44);

  // Progress bar
  // TODO: print percentage text for SHOW_PROGRESS_PERCENT
  tft.canvas(4, y, TFT_WIDTH - 8, 9);
  tft.set_background(COLOR_PROGRESS_BG);
  tft.add_rectangle(0, 0, TFT_WIDTH - 8, 9, COLOR_PROGRESS_FRAME);
  if (progress)
    tft.add_bar(1, 1, ((TFT_WIDTH - 10) * progress / (PROGRESS_SCALE)) / 100, 7, COLOR_PROGRESS_BAR);

  y += 12;
  // Status message
  // Canvas height should be 40px on 480x320 and 28 on 480x272
  const uint16_t status_height = TFT_HEIGHT - y;
  tft.canvas(0, y, TFT_WIDTH, status_height);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), tft_string.vcenter(FONT_LINE_HEIGHT), COLOR_STATUS_MESSAGE, tft_string);
}

// Low-level draw_edit_screen can be used to draw an edit screen from anyplace
void MenuEditItemBase::draw_edit_screen(FSTR_P const fstr, const char * const value/*=nullptr*/) {
  ui.encoder_direction_normal();
  TERN_(TOUCH_SCREEN, touch.clear());

  uint16_t line = 1;

  menu_line(line++);
  tft_string.set(fstr, itemIndex, itemStringC, itemStringF);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);

  TERN_(AUTO_BED_LEVELING_UBL, if (ui.external_control) line++);  // ftostr52() will overwrite *value so *value has to be displayed first

  menu_line(line);
  tft_string.set(value);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    if (ui.external_control) {
      menu_line(line - 1);

      tft_string.set(X_LBL);
      tft.add_text((TFT_WIDTH / 2 - 120), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.x)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 - 16) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

      tft_string.set(Y_LBL);
      tft.add_text((TFT_WIDTH / 2 + 16), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.y)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 + 120) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
    }
  #endif

  if (ui.can_show_slider()) {

    #define SLIDER_LENGTH TFT_WIDTH * 0.7
    #define SLIDER_Y_POSITION 186

    tft.canvas((TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION, SLIDER_LENGTH, 16);
    tft.set_background(COLOR_BACKGROUND);

    int16_t position = (SLIDER_LENGTH - 2) * ui.encoderPosition / maxEditValue;
    tft.add_bar(0, 7, 1, 2, ui.encoderPosition == 0 ? COLOR_SLIDER_INACTIVE : COLOR_SLIDER);
    tft.add_bar(1, 6, position, 4, COLOR_SLIDER);
    tft.add_bar(position + 1, 6, SLIDER_LENGTH - 2 - position, 4, COLOR_SLIDER_INACTIVE);
    tft.add_bar(SLIDER_LENGTH - 1, 7, 1, 2, int32_t(ui.encoderPosition) == maxEditValue ? COLOR_SLIDER : COLOR_SLIDER_INACTIVE);

    #if ENABLED(TOUCH_SCREEN)
      tft.add_image((SLIDER_LENGTH - 8) * ui.encoderPosition / maxEditValue, 0, imgSlider, COLOR_SLIDER);
      touch.add_control(SLIDER, (TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION - 8, SLIDER_LENGTH, 32, maxEditValue);
    #endif
  }

  tft.draw_edit_screen_buttons();
}

void TFT::draw_edit_screen_buttons() {
  #if ENABLED(TOUCH_SCREEN)
    add_control((TFT_WIDTH - 3 * 64) / 6, TFT_HEIGHT - 64, DECREASE, imgDecrease);
    add_control((TFT_WIDTH - 64) / 2, TFT_HEIGHT - 64, INCREASE, imgIncrease);
    add_control((TFT_WIDTH * 5 - 3 * 64) / 6, TFT_HEIGHT - 64, CLICK, imgConfirm);
  #endif
}

// The Select Screen presents a prompt and two "buttons"
void MenuItem_confirm::draw_select_screen(FSTR_P const yes, FSTR_P const no, const bool yesno, FSTR_P const pref, const char * const string/*=nullptr*/, FSTR_P const suff/*=nullptr*/) {
  uint16_t line = 1;

  if (!string) line++;

  menu_line(line++);
  tft_string.set(pref);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  if (string) {
    menu_line(line++);
    tft_string.set(string);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

  if (suff) {
    menu_line(line);
    tft_string.set(suff);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }
  #if ENABLED(TOUCH_SCREEN)
    if (no)  add_control(TFT_WIDTH / 4 - 32, TFT_HEIGHT - 64, CANCEL,  imgCancel,  true, yesno ? HALF(COLOR_CONTROL_CANCEL) : COLOR_CONTROL_CANCEL);
    if (yes) add_control(TFT_WIDTH * 3 / 4 - 32, TFT_HEIGHT - 64, CONFIRM, imgConfirm, true, yesno ? COLOR_CONTROL_CONFIRM : HALF(COLOR_CONTROL_CONFIRM));
  #endif
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      touch.add_control(RESUME_CONTINUE, 0, 0, TFT_WIDTH, TFT_HEIGHT);
    #endif

    menu_line(row);
    tft_string.set(GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE));
    tft_string.add('E');
    tft_string.add((char)('1' + extruder));
    tft_string.add(' ');
    tft_string.add(i16tostr3rj(thermalManager.wholeDegHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.add(F(" / "));
    tft_string.add(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #define GRID_OFFSET_X   8
  #define GRID_OFFSET_Y   8
  #define GRID_WIDTH      192
  #define GRID_HEIGHT     192
  #define CONTROL_OFFSET  16

  void MarlinUI::ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {

    tft.canvas(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_WIDTH, GRID_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_rectangle(0, 0, GRID_WIDTH, GRID_HEIGHT, COLOR_WHITE);

    for (uint16_t x = 0; x < (GRID_MAX_POINTS_X); x++)
      for (uint16_t y = 0; y < (GRID_MAX_POINTS_Y); y++)
        if (position_is_reachable({ bedlevel.get_mesh_x(x), bedlevel.get_mesh_y(y) }))
          tft.add_bar(1 + (x * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2, GRID_HEIGHT - 3 - ((y * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 2, 2, COLOR_UBL);

    tft.add_rectangle((x_plot * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2 - 1, GRID_HEIGHT - 5 - ((y_plot * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 6, 6, COLOR_UBL);

    const xy_pos_t pos = { bedlevel.get_mesh_x(x_plot), bedlevel.get_mesh_y(y_plot) },
                   lpos = pos.asLogical();

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2 - MENU_ITEM_HEIGHT, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(X_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.x));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Y_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.y));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2 + MENU_ITEM_HEIGHT, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Z_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(isnan(bedlevel.z_values[x_plot][y_plot]) ? "-----" : ftostr43sign(bedlevel.z_values[x_plot][y_plot]));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    constexpr uint8_t w = (TFT_WIDTH) / 10;
    tft.canvas(GRID_OFFSET_X + (GRID_WIDTH - w) / 2, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET - 5, w, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(x_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(w), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET + 16 - 24, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2, w, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(y_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(w), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + CONTROL_OFFSET,                    UBL,  (ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgUp);
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + GRID_HEIGHT - CONTROL_OFFSET - 32, UBL, -(ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgDown);
      add_control(GRID_OFFSET_X + CONTROL_OFFSET,                   GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL, -(ENCODER_STEPS_PER_MENU_ITEM), imgLeft);
      add_control(GRID_OFFSET_X + GRID_WIDTH - CONTROL_OFFSET - 32, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL,   ENCODER_STEPS_PER_MENU_ITEM, imgRight);
      add_control(320, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET, CLICK, imgLeveling);
      add_control(224, TFT_HEIGHT - 34, BACK, imgBack);
    #endif
  }
#endif // AUTO_BED_LEVELING_UBL

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../feature/babystep.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#define Z_SELECTION_Z 1
#define Z_SELECTION_Z_PROBE -1

struct {
  #if HAS_X_AXIS
    xy_int_t xValuePos;
  #endif
  #if HAS_Y_AXIS
    xy_int_t yValuePos;
  #endif
  #if HAS_Z_AXIS
    xy_int_t zValuePos, zTypePos;
    int z_selection = Z_SELECTION_Z;
  #endif
  #if HAS_EXTRUDERS
    xy_int_t eValuePos, eNamePos;
    uint8_t e_selection = 0;
  #endif
  xy_int_t stepValuePos;
  float currentStepSize = 10.0;
  bool blocked = false;
  char message[32];
} motionAxisState;

#define BTN_WIDTH 64
#define BTN_HEIGHT 52
#define X_MARGIN 20
#define Y_MARGIN 15
#define Y_OFFSET TERN(HAS_UI_480x272, 28, 34)

static void quick_feedback() {
  #if HAS_CHIRP
    ui.chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if ALL(HAS_MARLINUI_MENU, HAS_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_MARLINUI_MENU
      delay(10);
    #endif
  #endif
}

#define CUR_STEP_VALUE_WIDTH 104
static void drawCurStepValue() {
  tft_string.set(ftostr52sp(motionAxisState.currentStepSize));
  tft_string.trim();
  tft_string.add(F("mm"));
  tft.canvas(motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_AXIS_HOMED, tft_string);
}

#if HAS_Z_AXIS
  static void drawCurZSelection() {
    tft_string.set('Z');
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, tft_string.width(), FONT_LINE_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_text(0, 0, Z_BTN_COLOR, tft_string);

    tft.queue.sync();

    tft_string.set(F("Offset"));
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y + Y_OFFSET, tft_string.width(), Y_OFFSET TERN_(HAS_UI_480x272, - 10));
    tft.set_background(COLOR_BACKGROUND);
    #if HAS_BED_PROBE
      if (motionAxisState.z_selection == Z_SELECTION_Z_PROBE)
        tft.add_text(0, 0, Z_BTN_COLOR, tft_string);
    #endif
  }
#endif

#if HAS_EXTRUDERS
  static void drawCurESelection() {
    tft.canvas(motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, BTN_WIDTH, BTN_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set('E');
    tft_string.add(utostr3(motionAxisState.e_selection));
    tft.add_text(tft_string.center(BTN_WIDTH), 0, E_BTN_COLOR, tft_string);
  }
#endif

static void drawMessage(PGM_P const msg) {
  #if ENABLED(TFT_COLOR_UI_PORTRAIT)
    tft.canvas(X_MARGIN, TFT_HEIGHT - 2 * MOVE_AXIS_MARGIN_SIZE - BTN_HEIGHT - FONT_LINE_HEIGHT, TFT_WIDTH - X_MARGIN * 2, FONT_LINE_HEIGHT);
  #else
    tft.canvas(X_MARGIN, TFT_HEIGHT - Y_MARGIN - Y_OFFSET, TFT_HEIGHT / 2, Y_OFFSET);
  #endif
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_YELLOW, msg);
}

static void drawMessage(FSTR_P const fmsg) { drawMessage(FTOP(fmsg)); }

static void drawAxisValue(const AxisEnum axis) {
  const float value = (
    TERN_(HAS_BED_PROBE, axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? probe.offset.z :)
    ui.manual_move.axis_value(axis)
  );
  xy_int_t pos;
  uint16_t color;
  switch (axis) {
    #if HAS_X_AXIS
      case X_AXIS: pos = motionAxisState.xValuePos; color = X_BTN_COLOR; break;
    #endif
    #if HAS_Y_AXIS
      case Y_AXIS: pos = motionAxisState.yValuePos; color = Y_BTN_COLOR; break;
    #endif
    #if HAS_Z_AXIS
      case Z_AXIS: pos = motionAxisState.zValuePos; color = Z_BTN_COLOR; break;
    #endif
    #if HAS_EXTRUDERS
      case E_AXIS: pos = motionAxisState.eValuePos; color = E_BTN_COLOR; break;
    #endif
    default: return;
  }
  tft.canvas(pos.x, pos.y, BTN_WIDTH + X_MARGIN, BTN_HEIGHT TERN_(HAS_UI_480x272, / 2));
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(ftostr52sp(value));
  tft_string.trim();
  tft.add_text(tft_string.center(BTN_WIDTH + X_MARGIN), 0, color, tft_string);
}

static void moveAxis(const AxisEnum axis, const int8_t direction) {
  quick_feedback();

  #if HAS_EXTRUDERS
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(motionAxisState.e_selection)) {
      drawMessage(F("Too cold"));
      return;
    }
  #endif

  const float diff = motionAxisState.currentStepSize * direction;

  #if HAS_BED_PROBE

    if (axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE) {
      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        const int16_t babystep_increment = direction * BABYSTEP_SIZE_Z;
        const bool do_probe = DISABLED(BABYSTEP_HOTEND_Z_OFFSET) || active_extruder == 0;
        const float bsDiff = planner.mm_per_step[Z_AXIS] * babystep_increment,
                    new_probe_offset = probe.offset.z + bsDiff,
                    new_offs = TERN(BABYSTEP_HOTEND_Z_OFFSET
                      , do_probe ? new_probe_offset : hotend_offset[active_extruder].z - bsDiff
                      , new_probe_offset
                    );
        if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
          babystep.add_steps(Z_AXIS, babystep_increment);
          if (do_probe)
            probe.offset.z = new_offs;
          else
            TERN(BABYSTEP_HOTEND_Z_OFFSET, hotend_offset[active_extruder].z = new_offs, NOOP);
          drawMessage(NUL_STR); // Clear the error
          drawAxisValue(axis);
        }
        else
          drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));

      #else
        // Only change probe.offset.z
        probe.offset.z += diff;
        if (direction < 0 && current_position.z < Z_PROBE_OFFSET_RANGE_MIN) {
          current_position.z = Z_PROBE_OFFSET_RANGE_MIN;
          drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
        }
        else if (direction > 0 && current_position.z > Z_PROBE_OFFSET_RANGE_MAX) {
          current_position.z = Z_PROBE_OFFSET_RANGE_MAX;
          drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
        }
        else
          drawMessage(NUL_STR); // Clear the error

        drawAxisValue(axis);
      #endif
    }

  #endif // HAS_BED_PROBE

  if (!ui.manual_move.processing) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits(axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS && TERN1(HAS_EXTRUDERS, axis != E_AXIS)) {
        max = SQRT(sq(float(PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const bool limited = ui.manual_move.apply_diff(axis, diff, min, max);
    #if IS_KINEMATIC
      UNUSED(limited);
    #else
      PGM_P const msg = limited ? GET_TEXT(MSG_LCD_SOFT_ENDSTOPS) : NUL_STR;
      drawMessage(msg);
    #endif

    ui.manual_move.soon(axis OPTARG(MULTI_E_MANUAL, motionAxisState.e_selection));
  }

  drawAxisValue(axis);
}

#if HAS_EXTRUDERS
  static void e_plus()  { moveAxis(E_AXIS, 1);  }
  static void e_minus() { moveAxis(E_AXIS, -1); }
#endif
#if HAS_X_AXIS
  static void x_minus() { moveAxis(X_AXIS, -1); }
  static void x_plus()  { moveAxis(X_AXIS, 1);  }
#endif
#if HAS_Y_AXIS
  static void y_plus()  { moveAxis(Y_AXIS, 1);  }
  static void y_minus() { moveAxis(Y_AXIS, -1); }
#endif
#if HAS_Z_AXIS
  static void z_plus()  { moveAxis(Z_AXIS, 1);  }
  static void z_minus() { moveAxis(Z_AXIS, -1); }
#endif

#if ENABLED(TOUCH_SCREEN)
  #if HAS_EXTRUDERS
    static void e_select() {
      if (++motionAxisState.e_selection >= EXTRUDERS)
        motionAxisState.e_selection = 0;
      quick_feedback();
      drawCurESelection();
      drawAxisValue(E_AXIS);
    }
  #endif

  static void do_home() {
    quick_feedback();
    drawMessage(GET_TEXT_F(MSG_LEVEL_BED_HOMING));
    queue.inject_P(G28_STR);
    // Disable touch until home is done
    touch.disable();
    TERN_(HAS_EXTRUDERS, drawAxisValue(E_AXIS));
    TERN_(HAS_X_AXIS, drawAxisValue(X_AXIS));
    TERN_(HAS_Y_AXIS, drawAxisValue(Y_AXIS));
    TERN_(HAS_Z_AXIS, drawAxisValue(Z_AXIS));
  }

  static void step_size() {
    motionAxisState.currentStepSize = motionAxisState.currentStepSize / 10.0;
    if (motionAxisState.currentStepSize < 0.0015) motionAxisState.currentStepSize = 10.0;
    quick_feedback();
    drawCurStepValue();
  }
#endif // TOUCH_SCREEN

#if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
  static void z_select() {
    motionAxisState.z_selection *= -1;
    quick_feedback();
    drawCurZSelection();
    drawAxisValue(Z_AXIS);
  }
#endif

static void disable_steppers() {
  quick_feedback();
  queue.inject(F("M84"));
}

static void drawBtn(const int x, const int y, const char *label, intptr_t data, const MarlinImage img, uint16_t bgColor, const bool enabled=true) {
  const uint16_t width = images[imgBtn52Rounded].width,
                height = images[imgBtn52Rounded].height;

  if (!enabled) bgColor = COLOR_CONTROL_DISABLED;

  tft.canvas(x, y, width, height);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_image(0, 0, imgBtn52Rounded, bgColor, COLOR_BACKGROUND, COLOR_DARKGREY);

  // TODO: Make an add_text() taking a font arg
  if (label) {
    tft_string.set(label);
    tft_string.trim();
    tft.add_text(tft_string.center(width), height / 2 - tft_string.font_height() / 2, bgColor, tft_string);
  }
  else
    tft.add_image(0, 0, img, bgColor, COLOR_BACKGROUND, COLOR_DARKGREY);

  TERN_(TOUCH_SCREEN, if (enabled) touch.add_control(BUTTON, x, y, width, height, data));
}

void MarlinUI::move_axis_screen() {
  // Reset
  defer_status_screen(true);
  motionAxisState.blocked = false;
  TERN_(TOUCH_SCREEN, touch.enable());

  ui.clear_lcd();

  TERN_(TOUCH_SCREEN, touch.clear());

  const bool busy = printingIsActive();

  // Babysteps during printing? Select babystep for Z probe offset
  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    if (busy) motionAxisState.z_selection = Z_SELECTION_Z_PROBE;
  #endif

  #if ENABLED(TFT_COLOR_UI_PORTRAIT)

    int y = MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 1st: | Button "E+" | Button "Y+" | Button "Z+" |
     *************************************************************************/

    TERN_(HAS_EXTRUDERS, drawBtn(TFT_WIDTH / 6     - BTN_WIDTH / 2, y, "E+", (intptr_t)e_plus, imgUp, E_BTN_COLOR, !busy));
    TERN_(HAS_Y_AXIS,    drawBtn(TFT_WIDTH / 2     - BTN_WIDTH / 2, y, "Y+", (intptr_t)y_plus, imgUp, Y_BTN_COLOR, !busy));
    TERN_(HAS_Z_AXIS,    drawBtn(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y, "Z+", (intptr_t)z_plus, imgUp, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)));

    y += BTN_HEIGHT + 4;

    /**************************************************************************
     * 2nd: | Label "Ex" (E0, E1, etc.) | Y value | Label "Z" |
     *************************************************************************/

    // Label "Ex" (E0, E1, etc.)
    #if HAS_EXTRUDERS
      motionAxisState.eNamePos.set(TFT_WIDTH / 6 - BTN_WIDTH / 2, y);
      drawCurESelection();
      #if ENABLED(TOUCH_SCREEN)
        if (!busy) touch.add_control(BUTTON, motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, BTN_WIDTH, BTN_HEIGHT, (intptr_t)e_select);
      #endif
    #endif

    // Y value
    #if HAS_Y_AXIS
      motionAxisState.yValuePos.set(TFT_WIDTH / 2 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(Y_AXIS);
    #endif

    // Label "Z"
    #ifdef HAS_Z_AXIS
      motionAxisState.zTypePos.set(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y);
      drawCurZSelection();
      #if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
        if (!busy) touch.add_control(BUTTON, motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, BTN_WIDTH, FONT_LINE_HEIGHT * 2, (intptr_t)z_select);
      #endif
    #endif

    y += BTN_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 3rd: | Button "X-" | Button "Homing" | Button "X+" |
     *************************************************************************/

    #if HAS_X_AXIS
      drawBtn(TFT_WIDTH / 6 - BTN_WIDTH / 2, y, "X-", (intptr_t)x_minus, imgLeft, X_BTN_COLOR, !busy);
      TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH / 2 - images[imgHome].width / 2, y - (images[imgHome].width - BTN_HEIGHT) / 2, BUTTON, (intptr_t)do_home, imgHome, !busy));
      drawBtn(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y, "X+", (intptr_t)x_plus, imgRight, X_BTN_COLOR, !busy);
    #endif

    y += BTN_HEIGHT + 4;

    /**************************************************************************
     * 4th: | X value |
     *************************************************************************/

    #ifdef HAS_X_AXIS
      motionAxisState.xValuePos.set(TFT_WIDTH / 6 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(X_AXIS);
    #endif

    y += BTN_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 5th: | Button "E-" | Button "Y-" | Button "Z-"
     *************************************************************************/

    TERN_(HAS_EXTRUDERS, drawBtn(TFT_WIDTH / 6 - BTN_WIDTH / 2, y, "E-", (intptr_t)e_minus, imgDown, E_BTN_COLOR, !busy));
    TERN_(HAS_Y_AXIS, drawBtn(TFT_WIDTH / 2 - BTN_WIDTH / 2, y, "Y-", (intptr_t)y_minus, imgDown, Y_BTN_COLOR, !busy));
    TERN_(HAS_Z_AXIS, drawBtn(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y, "Z-", (intptr_t)z_minus, imgDown, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)));

    y += BTN_HEIGHT + 4;

    /**************************************************************************
     * 6th: | Current Ex value | Z value |
     *************************************************************************/

    // Current Ex value
    #ifdef HAS_EXTRUDERS
      motionAxisState.eValuePos.set(TFT_WIDTH / 6 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(E_AXIS);
    #endif

    #ifdef HAS_Z_AXIS
      motionAxisState.zValuePos.set(TFT_WIDTH * 5 / 6 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(Z_AXIS);
    #endif

    y += BTN_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 7th: | Message - drawMessage(...) function |
     *************************************************************************/

    y += FONT_LINE_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 8th: | Step size | Button "Disable steppers" | Button "Back" |
     *************************************************************************/

    // Step size
    motionAxisState.stepValuePos.set(X_MARGIN, y + 12);
    if (!busy) {
      drawCurStepValue();
      TERN_(TOUCH_SCREEN, touch.add_control(BUTTON, motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y + 12, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT, (intptr_t)step_size));
    }

    // Button "Disable steppers"
    drawBtn(TFT_WIDTH / 2, y, "off", (intptr_t)disable_steppers, imgCancel, COLOR_WHITE, !busy);

    // Button "Back"
    TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH * 5 / 6 - images[imgBack].width / 2, y + 12, BACK, imgBack));

  #else // !TFT_COLOR_UI_PORTRAIT

    // ROW 1 -> E- Y- CurY Z+
    int x = X_MARGIN, y = Y_MARGIN, spacing = 0;

    TERN_(HAS_EXTRUDERS, drawBtn(x, y, "E+", (intptr_t)e_plus, imgUp, E_BTN_COLOR, !busy));

    spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;
    x += BTN_WIDTH + spacing;

    TERN_(HAS_Y_AXIS, drawBtn(x, y, "Y+", (intptr_t)y_plus, imgUp, Y_BTN_COLOR, !busy));

    x += BTN_WIDTH;

    // Cur Y
    #if HAS_Y_AXIS
      motionAxisState.yValuePos.set(x + 2, y);
      drawAxisValue(Y_AXIS);
    #endif

    x += spacing;
    #if HAS_Z_AXIS
      drawBtn(x, y, "Z+", (intptr_t)z_plus, imgUp, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); // Only enabled when not busy or have baby step
    #endif

    // ROW 2 -> "Ex"  X-  HOME X+  "Z"
    y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
    x = X_MARGIN;
    spacing = (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4;

    #if HAS_EXTRUDERS
      motionAxisState.eNamePos.set(x, y);
      drawCurESelection();
      TERN_(TOUCH_SCREEN, if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, BTN_HEIGHT, (intptr_t)e_select));
    #endif

    x += BTN_WIDTH + spacing;

    TERN_(HAS_X_AXIS, drawBtn(x, y, "X-", (intptr_t)x_minus, imgLeft, X_BTN_COLOR, !busy));

    x += BTN_WIDTH + spacing; //imgHome is 64x64

    #if ALL(HAS_X_AXIS, TOUCH_SCREEN)
      add_control(TFT_WIDTH / 2 - images[imgHome].width / 2, y - (images[imgHome].width - BTN_HEIGHT) / 2, BUTTON, (intptr_t)do_home, imgHome, !busy);
    #endif

    x += BTN_WIDTH + spacing;
    const uint16_t xplus_x = x;

    TERN_(HAS_X_AXIS, drawBtn(x, y, "X+", (intptr_t)x_plus, imgRight, X_BTN_COLOR, !busy));

    x += BTN_WIDTH + spacing;

    #if HAS_Z_AXIS
      motionAxisState.zTypePos.set(x, y);
      drawCurZSelection();
      #if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
        if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, Y_OFFSET * 2, (intptr_t)z_select);
      #endif
    #endif

    // ROW 3 -> E- CurX Y-  Z-
    y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
    x = X_MARGIN;
    spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;

    #if HAS_EXTRUDERS
      drawBtn(x, y, "E-", (intptr_t)e_minus, imgDown, E_BTN_COLOR, !busy);
      motionAxisState.eValuePos.set(x, y + BTN_HEIGHT + 2);
      drawAxisValue(E_AXIS);
    #endif

    // Cur X
    #if HAS_X_AXIS
      motionAxisState.xValuePos.set(BTN_WIDTH + (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4, y - 10);
      TERN_(HAS_X_AXIS, drawAxisValue(X_AXIS));
    #endif

    x += BTN_WIDTH + spacing;

    // Cur Y
    TERN_(HAS_Y_AXIS, drawBtn(x, y, "Y-", (intptr_t)y_minus, imgDown, Y_BTN_COLOR, !busy));

    x += BTN_WIDTH + spacing;

    // Cur Z
    #if HAS_Z_AXIS
      drawBtn(x, y, "Z-", (intptr_t)z_minus, imgDown, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); //only enabled when not busy or have baby step
      motionAxisState.zValuePos.set(x, y + BTN_HEIGHT + 2);
      drawAxisValue(Z_AXIS);
    #endif

    // ROW 4 -> step_size  disable steppers back
    y = TFT_HEIGHT - Y_MARGIN - TERN(HAS_UI_480x272, BTN_WIDTH / 2, 32);
    x = TFT_WIDTH / 2 - CUR_STEP_VALUE_WIDTH / 2;
    motionAxisState.stepValuePos.set(x, y);
    if (!busy) {
      drawCurStepValue();
      TERN_(TOUCH_SCREEN, touch.add_control(BUTTON, motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT, (intptr_t)step_size));
    }

    // Aligned with x+
    drawBtn(xplus_x, TFT_HEIGHT - Y_MARGIN - BTN_HEIGHT, "off", (intptr_t)disable_steppers, imgCancel, COLOR_WHITE, !busy);

    TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH - X_MARGIN - BTN_WIDTH, y, BACK, imgBack));

  #endif // !TFT_COLOR_UI_PORTRAIT
}

#endif // HAS_UI_480x320 || HAS_UI_480x272
