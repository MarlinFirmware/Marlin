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

#if HAS_UI_320x240

#include "ui_320x240.h"

#include "../marlinui.h"
#include "../menu/menu.h"
#include "../../libs/numtostr.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../../module/motion.h"

#if DISABLED(LCD_PROGRESS_BAR) && BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
  #include "../../feature/filwidth.h"
  #include "../../gcode/parser.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if !HAS_LCD_MENU
  #error "Seriously? High resolution TFT screen without menu?"
#endif

static bool draw_menu_navigation = false;

void MarlinUI::tft_idle() {
  #if ENABLED(TOUCH_SCREEN)
    if (draw_menu_navigation) {
      add_control(48, 206, PAGE_UP, imgPageUp, encoderTopLine > 0);
      add_control(240, 206, PAGE_DOWN, imgPageDown, encoderTopLine + LCD_HEIGHT < screen_items);
      add_control(144, 206, BACK, imgBack);
      draw_menu_navigation = false;
    }
  #endif

  tft.queue.async();
  TERN_(TOUCH_SCREEN, touch.idle());
}

void MarlinUI::init_lcd() {
  tft.init();
  tft.set_font(MENU_FONT_NAME);
  #ifdef SYMBOLS_FONT_NAME
    tft.add_glyphs(SYMBOLS_FONT_NAME);
  #endif
  TERN_(TOUCH_SCREEN, touch.init());
  clear_lcd();
}

bool MarlinUI::detected() { return true; }

void MarlinUI::clear_lcd() {
  #if ENABLED(TOUCH_SCREEN)
    touch.reset();
    draw_menu_navigation = false;
  #endif

  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_BACKGROUND);
}

#if ENABLED(SHOW_BOOTSCREEN)
  #ifndef BOOTSCREEN_TIMEOUT
    #define BOOTSCREEN_TIMEOUT 1500
  #endif

  void MarlinUI::show_bootscreen() {
    tft.queue.reset();

    tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
    tft.add_image(0, 0, imgBootScreen);  // MarlinLogo320x240x16

    #ifdef WEBSITE_URL
      tft.add_text(4, 188, COLOR_WEBSITE_URL, WEBSITE_URL);
    #endif

    tft.queue.sync();
    safe_delay(BOOTSCREEN_TIMEOUT);
    clear_lcd();
  }
#endif // SHOW_BOOTSCREEN

void MarlinUI::draw_kill_screen() {
  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_KILL_SCREEN_BG);

  tft.canvas(0, 60, TFT_WIDTH, 20);
  tft.set_background(COLOR_KILL_SCREEN_BG);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_KILL_SCREEN_TEXT, tft_string);

  tft.canvas(0, 120, TFT_WIDTH, 20);
  tft.set_background(COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_HALTED));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_KILL_SCREEN_TEXT, tft_string);

  tft.canvas(0, 160, TFT_WIDTH, 20);
  tft.set_background(COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_PLEASE_RESET));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_KILL_SCREEN_TEXT, tft_string);

  tft.queue.sync();
}

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater) {
  MarlinImage image = imgHotEnd;
  uint16_t Color;
  float currentTemperature, targetTemperature;

  if (Heater >= 0) { // HotEnd
    currentTemperature = thermalManager.degHotend(Heater);
    targetTemperature = thermalManager.degTargetHotend(Heater);
  }
#if HAS_HEATED_BED
  else if (Heater == H_BED) {
    currentTemperature = thermalManager.degBed();
    targetTemperature = thermalManager.degTargetBed();
  }
#endif // HAS_HEATED_BED
#if HAS_TEMP_CHAMBER
  else if (Heater == H_CHAMBER) {
    currentTemperature = thermalManager.degChamber();
    #if HAS_HEATED_CHAMBER
      targetTemperature = thermalManager.degTargetChamber();
    #else
      targetTemperature = ABSOLUTE_ZERO;
    #endif
  }
#endif // HAS_TEMP_CHAMBER
  else return;

  TERN_(TOUCH_SCREEN, if (targetTemperature >= 0) touch.add_control(HEATER, x, y, 64, 100, Heater));
  tft.canvas(x, y, 64, 100);
  tft.set_background(COLOR_BACKGROUND);

  Color = currentTemperature < 0 ? COLOR_INACTIVE : COLOR_COLD;

  if (Heater >= 0) { // HotEnd
    if (currentTemperature >= 50) Color = COLOR_HOTEND;
  }
  #if HAS_HEATED_BED
  else if (Heater == H_BED) {
    if (currentTemperature >= 50) Color = COLOR_HEATED_BED;
    image = targetTemperature > 0 ? imgBedHeated : imgBed;
  }
  #endif // HAS_HEATED_BED
  #if HAS_TEMP_CHAMBER
  else if (Heater == H_CHAMBER) {
    if (currentTemperature >= 50) Color = COLOR_CHAMBER;
    image = targetTemperature > 0 ? imgChamberHeated : imgChamber;
  }
  #endif // HAS_TEMP_CHAMBER

  tft.add_image(0, 18, image, Color);

  tft_string.set((uint8_t *)i16tostr3rj(currentTemperature + 0.5));
  tft_string.add(LCD_STR_DEGREE);
  tft_string.trim();
  tft.add_text(tft_string.center(64) + 2, 72, Color, tft_string);

  if (targetTemperature >= 0) {
    tft_string.set((uint8_t *)i16tostr3rj(targetTemperature + 0.5));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(64) + 2, 8, Color, tft_string);

  }
}

void draw_fan_status(uint16_t x, uint16_t y, const bool blink) {
  TERN_(TOUCH_SCREEN, touch.add_control(FAN, x, y, 64, 100));
  tft.canvas(x, y, 64, 100);
  tft.set_background(COLOR_BACKGROUND);

  uint8_t fanSpeed = thermalManager.fan_speed[0];
  MarlinImage image;

  if (fanSpeed >= 127)
    image = blink ? imgFanFast1 : imgFanFast0;
  else if (fanSpeed > 0)
    image = blink ? imgFanSlow1 : imgFanSlow0;
  else
    image = imgFanIdle;

  tft.add_image(0, 10, image, COLOR_FAN);

  tft_string.set((uint8_t *)ui8tostr4pctrj(thermalManager.fan_speed[0]));
  tft_string.trim();
  tft.add_text(tft_string.center(64) + 6, 72, COLOR_FAN, tft_string);
}

void MarlinUI::draw_status_screen() {
  const bool blink = get_blink();

  TERN_(TOUCH_SCREEN, touch.clear());

  // heaters and fan
  uint16_t i, x, y = POS_Y;

  for (i = 0 ; i < ITEMS_COUNT; i++) {
    x = (320 / ITEMS_COUNT - 64) / 2  + (320 * i / ITEMS_COUNT);
    switch (i) {
      #ifdef ITEM_E0
        case ITEM_E0: draw_heater_status(x, y, H_E0); break;
      #endif
      #ifdef ITEM_E1
        case ITEM_E1: draw_heater_status(x, y, H_E1); break;
      #endif
      #ifdef ITEM_E2
        case ITEM_E2: draw_heater_status(x, y, H_E2); break;
      #endif
      #ifdef ITEM_BED
        case ITEM_BED: draw_heater_status(x, y, H_BED); break;
      #endif
      #ifdef ITEM_CHAMBER
        case ITEM_CHAMBER: draw_heater_status(x, y, H_CHAMBER); break;
      #endif
      #ifdef ITEM_FAN
        case ITEM_FAN: draw_fan_status(x, y, blink); break;
      #endif
    }
  }

  // coordinates
  tft.canvas(4, 103, 312, 24);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_rectangle(0, 0, 312, 24, COLOR_AXIS_HOMED);

  uint16_t color;
  uint16_t offset;
  bool is_homed;

  tft.add_text( 10, 3, COLOR_AXIS_HOMED , "X");
  tft.add_text(127, 3, COLOR_AXIS_HOMED , "Y");
  tft.add_text(219, 3, COLOR_AXIS_HOMED , "Z");

  is_homed = TEST(axis_homed, X_AXIS);
  tft_string.set(blink & !is_homed ? "?" : ftostr4sign(LOGICAL_X_POSITION(current_position.x)));
  tft.add_text( 68 - tft_string.width(), 3, is_homed ? COLOR_AXIS_HOMED : COLOR_AXIS_NOT_HOMED, tft_string);

  is_homed = TEST(axis_homed, Y_AXIS);
  tft_string.set(blink & !is_homed ? "?" : ftostr4sign(LOGICAL_Y_POSITION(current_position.y)));
  tft.add_text(185 - tft_string.width(), 3, is_homed ? COLOR_AXIS_HOMED : COLOR_AXIS_NOT_HOMED, tft_string);

  is_homed = TEST(axis_homed, Z_AXIS);
  if (blink & !is_homed) {
    tft_string.set("?");
    offset = 25; // ".00"
  }
  else {
    const float z = LOGICAL_Z_POSITION(current_position.z);
    tft_string.set(ftostr52sp((int16_t)z));
    tft_string.rtrim();
    offset = tft_string.width();

    tft_string.set(ftostr52sp(z));
    offset += 25 - tft_string.width();
  }
  tft.add_text(301 - tft_string.width() - offset, 3, is_homed ? COLOR_AXIS_HOMED : COLOR_AXIS_NOT_HOMED, tft_string);

  // feed rate
  tft.canvas(70, 136, 80, 32);
  tft.set_background(COLOR_BACKGROUND);
  color = feedrate_percentage == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;
  tft.add_image(0, 0, imgFeedRate, color);
  tft_string.set(i16tostr3rj(feedrate_percentage));
  tft_string.add('%');
  tft.add_text(32, 6, color , tft_string);
  TERN_(TOUCH_SCREEN, touch.add_control(FEEDRATE, 70, 136, 80, 32));

  // flow rate
  tft.canvas(170, 136, 80, 32);
  tft.set_background(COLOR_BACKGROUND);
  color = planner.flow_percentage[0] == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;
  tft.add_image(0, 0, imgFlowRate, color);
  tft_string.set(i16tostr3rj(planner.flow_percentage[active_extruder]));
  tft_string.add('%');
  tft.add_text(32, 6, color , tft_string);
  TERN_(TOUCH_SCREEN, touch.add_control(FLOWRATE, 170, 136, 80, 32, active_extruder));

  // print duration
  char buffer[14];
  duration_t elapsed = print_job_timer.duration();
  elapsed.toDigital(buffer);

  tft.canvas(96, 176, 128, 20);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(buffer);
  tft.add_text(tft_string.center(128), 0, COLOR_PRINT_TIME, tft_string);

  // progress bar
  const uint8_t progress = ui.get_progress_percent();
  tft.canvas(4, 198, 312, 9);
  tft.set_background(COLOR_PROGRESS_BG);
  tft.add_rectangle(0, 0, 312, 9, COLOR_PROGRESS_FRAME);
  if (progress)
    tft.add_bar(1, 1, (310 * progress) / 100, 7, COLOR_PROGRESS_BAR);

  // status message
  tft.canvas(0, 216, 320, 20);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_STATUS_MESSAGE, tft_string);

  #if ENABLED(TOUCH_SCREEN)
    add_control(256, 130, menu_main, imgSettings);
    TERN_(SDSUPPORT, add_control(0, 130, menu_media, imgSD, card.isMounted() && !printingIsActive(), COLOR_CONTROL_ENABLED, card.isMounted() && printingIsActive() ? COLOR_BUSY : COLOR_CONTROL_DISABLED));
  #endif
}

// Draw a static item with no left-right margin required. Centered by default.
void MenuItem_static::draw(const uint8_t row, PGM_P const pstr, const uint8_t style/*=SS_DEFAULT*/, const char * const vstr/*=nullptr*/) {
  menu_item(row);
  tft_string.set(pstr, itemIndex, itemString);
  if (vstr)
    tft_string.add(vstr);
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_YELLOW, tft_string);
}

// Draw a generic menu item with pre_char (if selected) and post_char
void MenuItemBase::_draw(const bool sel, const uint8_t row, PGM_P const pstr, const char pre_char, const char post_char) {
  menu_item(row, sel);

  uint8_t *string = (uint8_t *)pstr;
  MarlinImage image = noImage;
  switch (*string) {
    case 0x01: image = imgRefresh; break;  // LCD_STR_REFRESH
    case 0x02: image = imgDirectory; break;  // LCD_STR_FOLDER
  }

  uint8_t offset = MENU_TEXT_X_OFFSET;
  if (image != noImage) {
    string++;
    offset = 32;
    tft.add_image(0, 0, image, COLOR_MENU_TEXT, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
  }

  tft_string.set(string, itemIndex, itemString);
  tft.add_text(offset, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
}

// Draw a menu item with a (potentially) editable value
void MenuEditItemBase::draw(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm) {
  menu_item(row, sel);

  tft_string.set(pstr, itemIndex, itemString);
  tft.add_text(MENU_TEXT_X_OFFSET, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  if (data) {
    tft_string.set(data);
    tft.add_text(TFT_WIDTH - MENU_TEXT_X_OFFSET - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
  }
}

// Low-level draw_edit_screen can be used to draw an edit screen from anyplace
void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char* const value/*=nullptr*/) {
  ui.encoder_direction_normal();
  TERN_(TOUCH_SCREEN, touch.clear());

  uint16_t line = 1;

  menu_line(line++);
  tft_string.set(pstr, itemIndex, itemString);
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
      tft.add_text(52, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.x)));
      tft_string.trim();
      tft.add_text(144 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

      tft_string.set(Y_LBL);
      tft.add_text(176, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.y)));
      tft_string.trim();
      tft.add_text(268 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
    }
  #endif

  extern screenFunc_t _manual_move_func_ptr;
  if (ui.currentScreen != _manual_move_func_ptr && !ui.external_control) {

    #define SLIDER_LENGHT 224
    #define SLIDER_Y_POSITION 140

    tft.canvas((TFT_WIDTH - SLIDER_LENGHT) / 2, SLIDER_Y_POSITION, SLIDER_LENGHT, 16);
    tft.set_background(COLOR_BACKGROUND);

    int16_t position = (SLIDER_LENGHT - 2) * ui.encoderPosition / maxEditValue;
    tft.add_bar(0, 7, 1, 2, ui.encoderPosition == 0 ? COLOR_SLIDER_INACTIVE : COLOR_SLIDER);
    tft.add_bar(1, 6, position, 4, COLOR_SLIDER);
    tft.add_bar(position + 1, 6, SLIDER_LENGHT - 2 - position, 4, COLOR_SLIDER_INACTIVE);
    tft.add_bar(SLIDER_LENGHT - 1, 7, 1, 2, int32_t(ui.encoderPosition) == maxEditValue ? COLOR_SLIDER : COLOR_SLIDER_INACTIVE);

    #if ENABLED(TOUCH_SCREEN)
      tft.add_image((SLIDER_LENGHT - 8) * ui.encoderPosition / maxEditValue, 0, imgSlider, COLOR_SLIDER);
      touch.add_control(SLIDER, (TFT_WIDTH - SLIDER_LENGHT) / 2, SLIDER_Y_POSITION - 8, SLIDER_LENGHT, 32, maxEditValue);
    #endif
  }

  #if ENABLED(TOUCH_SCREEN)
    add_control(32, 176, DECREASE, imgDecrease);
    add_control(224, 176, INCREASE, imgIncrease);
    add_control(128, 176, CLICK, imgConfirm);
  #endif
}

// The Select Screen presents a prompt and two "buttons"
void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
  uint16_t line = 1;

  if (!string) line++;

  menu_line(line++);
  tft_string.set(pref);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);

  if (string) {
    menu_line(line++);
    tft_string.set(string);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  }

  if (suff) {
    menu_line(line);
    tft_string.set(suff);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  }
  #if ENABLED(TOUCH_SCREEN)
    add_control(48, 176, CANCEL, imgCancel, true, yesno ? HALF(COLOR_CONTROL_CANCEL) : COLOR_CONTROL_CANCEL);
    add_control(208, 176, CONFIRM, imgConfirm, true, yesno ? COLOR_CONTROL_CONFIRM : HALF(COLOR_CONTROL_CONFIRM));
  #endif
}

#if ENABLED(SDSUPPORT)
  void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
    menu_item(row, sel);
    if (isDir)
      tft.add_image(0, 0, imgDirectory, COLOR_MENU_TEXT, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
    tft.add_text(32, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, theCard.longest_filename());
  }
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      touch.add_control(RESUME_CONTINUE , 0, 0, 320, 240);
    #endif

    menu_line(row);
    tft_string.set(GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE));
    tft_string.add('E');
    tft_string.add((char)('1' + extruder));
    tft_string.add(' ');
    tft_string.add(i16tostr3rj(thermalManager.degHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.add(" / ");
    tft_string.add(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  }
#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #define GRID_OFFSET_X   8
  #define GRID_OFFSET_Y   8
  #define GRID_WIDTH      144
  #define GRID_HEIGHT     144
  #define CONTROL_OFFSET  8

  void MarlinUI::ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {

    tft.canvas(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_WIDTH, GRID_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_rectangle(0, 0, GRID_WIDTH, GRID_HEIGHT, COLOR_WHITE);

    for (uint16_t x = 0; x < GRID_MAX_POINTS_X ; x++)
      for (uint16_t y = 0; y < GRID_MAX_POINTS_Y ; y++)
        if (position_is_reachable({ ubl.mesh_index_to_xpos(x), ubl.mesh_index_to_ypos(y) }))
          tft.add_bar(1 + (x * 2 + 1) * (GRID_WIDTH - 4) / GRID_MAX_POINTS_X / 2, GRID_HEIGHT - 3 - ((y * 2 + 1) * (GRID_HEIGHT - 4) / GRID_MAX_POINTS_Y / 2), 2, 2, COLOR_UBL);

    tft.add_rectangle((x_plot * 2 + 1) * (GRID_WIDTH - 4) / GRID_MAX_POINTS_X / 2 - 1, GRID_HEIGHT - 5 - ((y_plot * 2 + 1) * (GRID_HEIGHT - 4) / GRID_MAX_POINTS_Y / 2), 6, 6, COLOR_UBL);

    const xy_pos_t pos = { ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot) },
                   lpos = pos.asLogical();

    tft.canvas(216, GRID_OFFSET_Y + (GRID_HEIGHT - 32) / 2 - 32, 96, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(X_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.x));
    tft_string.trim();
    tft.add_text(96 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(216, GRID_OFFSET_Y + (GRID_HEIGHT - 32) / 2, 96, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Y_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.y));
    tft_string.trim();
    tft.add_text(96 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(216, GRID_OFFSET_Y + (GRID_HEIGHT - 32) / 2 + 32, 96, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Z_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(isnan(ubl.z_values[x_plot][y_plot]) ? "-----" : ftostr43sign(ubl.z_values[x_plot][y_plot]));
    tft_string.trim();
    tft.add_text(96 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);


    tft.canvas(GRID_OFFSET_X + (GRID_WIDTH - 32) / 2, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET - 1, 32, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(x_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(32), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET, GRID_OFFSET_Y + (GRID_HEIGHT - 27) / 2, 32, 32);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(y_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(32), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + CONTROL_OFFSET,                    UBL,   ENCODER_STEPS_PER_MENU_ITEM * GRID_MAX_POINTS_X, imgUp);
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + GRID_HEIGHT - CONTROL_OFFSET - 32, UBL, - ENCODER_STEPS_PER_MENU_ITEM * GRID_MAX_POINTS_X, imgDown);
      add_control(GRID_OFFSET_X + CONTROL_OFFSET,                   GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL, - ENCODER_STEPS_PER_MENU_ITEM, imgLeft);
      add_control(GRID_OFFSET_X + GRID_WIDTH - CONTROL_OFFSET - 32, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL,   ENCODER_STEPS_PER_MENU_ITEM, imgRight);
      add_control(224, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET, CLICK, imgLeveling);
      add_control(144, 206, BACK, imgBack);
    #endif
  }
#endif // AUTO_BED_LEVELING_UBL

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  void MarlinUI::touch_calibration() {
    static uint16_t x, y;

    calibrationState calibration_stage = touch.get_calibration_state();

    if (calibration_stage == CALIBRATION_NONE) {
      defer_status_screen(true);
      clear_lcd();
      calibration_stage = touch.calibration_start();
    }
    else {
      tft.canvas(x - 15, y - 15, 31, 31);
      tft.set_background(COLOR_BACKGROUND);
    }

    x = 20; y = 20;
    touch.clear();

    if (calibration_stage < CALIBRATION_SUCCESS) {
      switch (calibration_stage) {
        case CALIBRATION_POINT_1: tft_string.set("Top Left"); break;
        case CALIBRATION_POINT_2: y = TFT_HEIGHT - 21; tft_string.set("Bottom Left"); break;
        case CALIBRATION_POINT_3: x = TFT_WIDTH  - 21; tft_string.set("Top Right"); break;
        case CALIBRATION_POINT_4: x = TFT_WIDTH  - 21; y = TFT_HEIGHT - 21; tft_string.set("Bottom Right"); break;
        default: break;
      }

      tft.canvas(x - 15, y - 15, 31, 31);
      tft.set_background(COLOR_BACKGROUND);
      tft.add_bar(0, 15, 31, 1, COLOR_TOUCH_CALIBRATION);
      tft.add_bar(15, 0, 1, 31, COLOR_TOUCH_CALIBRATION);

      touch.add_control(CALIBRATE, 0, 0, TFT_WIDTH, TFT_HEIGHT, uint32_t(x) << 16 | uint32_t(y));
    }
    else {
      tft_string.set(calibration_stage == CALIBRATION_SUCCESS ? "Calibration Completed" : "Calibration Failed");
      defer_status_screen(false);
      touch.calibration_end();
      touch.add_control(BACK, 0, 0, TFT_WIDTH, TFT_HEIGHT);
    }

    tft.canvas(0, (TFT_HEIGHT - tft_string.font_height()) >> 1, TFT_WIDTH, tft_string.font_height());
    tft.set_background(COLOR_BACKGROUND);
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }
#endif // TOUCH_SCREEN_CALIBRATION

void menu_line(const uint8_t row, uint16_t color) {
  tft.canvas(0, 2 + 34 * row, TFT_WIDTH, 32);
  tft.set_background(color);
}

void menu_pause_option();

void menu_item(const uint8_t row, bool sel ) {
  #if ENABLED(TOUCH_SCREEN)
    if (row == 0) {
      touch.clear();
      draw_menu_navigation = TERN(ADVANCED_PAUSE_FEATURE, ui.currentScreen != menu_pause_option, true);
    }
  #endif

  menu_line(row, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
  TERN_(TOUCH_SCREEN, touch.add_control(sel ? CLICK : MENU_ITEM, 0, 2 + 34 * row, 320, 32, encoderTopLine + row));
}

void MarlinUI::move_axis_screen() {
}

#endif // HAS_UI_320x240
