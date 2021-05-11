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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN)

#include "touch.h"

#include "../marlinui.h"  // for ui methods
#include "../menu/menu_item.h" // for touch_screen_calibration

#include "../../module/temperature.h"
#include "../../module/planner.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#include "tft.h"

bool Touch::enabled = true;
int16_t Touch::x, Touch::y;
touch_control_t Touch::controls[];
touch_control_t *Touch::current_control;
uint16_t Touch::controls_count;
millis_t Touch::last_touch_ms = 0,
         Touch::time_to_hold,
         Touch::repeat_delay,
         Touch::touch_time;
TouchControlType  Touch::touch_control_type = NONE;
#if HAS_RESUME_CONTINUE
  extern bool wait_for_user;
#endif

void Touch::init() {
  TERN_(TOUCH_SCREEN_CALIBRATION, touch_calibration.calibration_reset());
  reset();
  io.Init();
  enable();
}

void Touch::add_control(TouchControlType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, intptr_t data) {
  if (controls_count == MAX_CONTROLS) return;

  controls[controls_count].type = type;
  controls[controls_count].x = x;
  controls[controls_count].y = y;
  controls[controls_count].width = width;
  controls[controls_count].height = height;
  controls[controls_count].data = data;
  controls_count++;
}

void Touch::idle() {
  uint16_t i;
  int16_t _x, _y;

  if (!enabled) return;

  // Return if Touch::idle is called within the same millisecond
  const millis_t now = millis();
  if (last_touch_ms == now) return;
  last_touch_ms = now;

  if (get_point(&_x, &_y)) {
    #if HAS_RESUME_CONTINUE
      // UI is waiting for a click anywhere?
      if (wait_for_user) {
        touch_control_type = CLICK;
        ui.lcd_clicked = true;
        if (ui.external_control) wait_for_user = false;
        return;
      }
    #endif

    #if LCD_TIMEOUT_TO_STATUS
      ui.return_to_status_ms = last_touch_ms + LCD_TIMEOUT_TO_STATUS;
    #endif

    if (touch_time) {
      #if ENABLED(TOUCH_SCREEN_CALIBRATION)
        if (touch_control_type == NONE && ELAPSED(last_touch_ms, touch_time + TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS) && ui.on_status_screen())
          ui.goto_screen(touch_screen_calibration);
      #endif
      return;
    }

    if (time_to_hold == 0) time_to_hold = last_touch_ms + MINIMUM_HOLD_TIME;
    if (PENDING(last_touch_ms, time_to_hold)) return;

    if (x != 0 && y != 0) {
      if (current_control) {
        if (WITHIN(x, current_control->x - FREE_MOVE_RANGE, current_control->x + current_control->width + FREE_MOVE_RANGE) && WITHIN(y, current_control->y - FREE_MOVE_RANGE, current_control->y + current_control->height + FREE_MOVE_RANGE)) {
          NOLESS(x, current_control->x);
          NOMORE(x, current_control->x + current_control->width);
          NOLESS(y, current_control->y);
          NOMORE(y, current_control->y + current_control->height);
          touch(current_control);
        }
        else
          current_control = nullptr;
      }
      else {
        for (i = 0; i < controls_count; i++) {
          if ((WITHIN(x, controls[i].x, controls[i].x + controls[i].width) && WITHIN(y, controls[i].y, controls[i].y + controls[i].height)) || (TERN(TOUCH_SCREEN_CALIBRATION, controls[i].type == CALIBRATE, false))) {
            touch_control_type = controls[i].type;
            touch(&controls[i]);
            break;
          }
        }
      }

      if (!current_control)
        touch_time = last_touch_ms;
    }
    x = _x;
    y = _y;
  }
  else {
    x = y = 0;
    current_control = nullptr;
    touch_time = 0;
    touch_control_type = NONE;
    time_to_hold = 0;
    repeat_delay = TOUCH_REPEAT_DELAY;
  }
}

void Touch::touch(touch_control_t *control) {
  switch (control->type) {
    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      case CALIBRATE:
        if (touch_calibration.handleTouch(x, y)) ui.refresh();
        break;
    #endif // TOUCH_SCREEN_CALIBRATION

    case MENU_SCREEN: ui.goto_screen((screenFunc_t)control->data); break;
    case BACK: ui.goto_previous_screen(); break;
    case MENU_CLICK:
      TERN_(SINGLE_TOUCH_NAVIGATION, ui.encoderPosition = control->data);
      ui.lcd_clicked = true;
      break;
    case CLICK: ui.lcd_clicked = true; break;
    #if HAS_RESUME_CONTINUE
      case RESUME_CONTINUE: extern bool wait_for_user; wait_for_user = false; break;
    #endif
    case CANCEL:  ui.encoderPosition = 0; ui.selection = false; ui.lcd_clicked = true; break;
    case CONFIRM: ui.encoderPosition = 1; ui.selection = true; ui.lcd_clicked = true; break;
    case MENU_ITEM: ui.encoderPosition = control->data; ui.refresh(); break;
    case PAGE_UP:
      encoderTopLine = encoderTopLine > LCD_HEIGHT ? encoderTopLine - LCD_HEIGHT : 0;
      ui.encoderPosition = ui.encoderPosition > LCD_HEIGHT ? ui.encoderPosition - LCD_HEIGHT : 0;
      ui.refresh();
      break;
    case PAGE_DOWN:
      encoderTopLine = encoderTopLine + 2 * LCD_HEIGHT < screen_items ? encoderTopLine + LCD_HEIGHT : screen_items - LCD_HEIGHT;
      ui.encoderPosition = ui.encoderPosition + LCD_HEIGHT < (uint32_t)screen_items ? ui.encoderPosition + LCD_HEIGHT : screen_items;
      ui.refresh();
      break;
    case SLIDER:    hold(control); ui.encoderPosition = (x - control->x) * control->data / control->width; break;
    case INCREASE:  hold(control, repeat_delay - 5); TERN(AUTO_BED_LEVELING_UBL, ui.external_control ? ubl.encoder_diff++ : ui.encoderPosition++, ui.encoderPosition++); break;
    case DECREASE:  hold(control, repeat_delay - 5); TERN(AUTO_BED_LEVELING_UBL, ui.external_control ? ubl.encoder_diff-- : ui.encoderPosition--, ui.encoderPosition--); break;
    case HEATER:
      int8_t heater;
      heater = control->data;
      ui.clear_lcd();
      #if HAS_HOTEND
        if (heater >= 0) { // HotEnd
          #if HOTENDS == 1
            MenuItem_int3::action((const char *)GET_TEXT_F(MSG_NOZZLE), &thermalManager.temp_hotend[0].target, 0, thermalManager.hotend_max_target(0), []{ thermalManager.start_watching_hotend(0); });
          #else
            MenuItemBase::itemIndex = heater;
            MenuItem_int3::action((const char *)GET_TEXT_F(MSG_NOZZLE_N), &thermalManager.temp_hotend[heater].target, 0, thermalManager.hotend_max_target(heater), []{ thermalManager.start_watching_hotend(MenuItemBase::itemIndex); });
          #endif
        }
      #endif
      #if HAS_HEATED_BED
        else if (heater == H_BED) {
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_BED), &thermalManager.temp_bed.target, 0, BED_MAX_TARGET, thermalManager.start_watching_bed);
        }
      #endif
      #if HAS_HEATED_CHAMBER
        else if (heater == H_CHAMBER) {
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_CHAMBER), &thermalManager.temp_chamber.target, 0, CHAMBER_MAX_TARGET, thermalManager.start_watching_chamber);
        }
      #endif
      #if HAS_COOLER
        else if (heater == H_COOLER) {
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_COOLER), &thermalManager.temp_cooler.target, 0, COOLER_MAX_TARGET, thermalManager.start_watching_cooler);
        }
      #endif

      break;
    case FAN:
      ui.clear_lcd();
      static uint8_t fan, fan_speed;
      fan = 0;
      fan_speed = thermalManager.fan_speed[fan];
      MenuItem_percent::action((const char *)GET_TEXT_F(MSG_FIRST_FAN_SPEED), &fan_speed, 0, 255, []{ thermalManager.set_fan_speed(fan, fan_speed); });
      break;
    case FEEDRATE:
      ui.clear_lcd();
      MenuItem_int3::action((const char *)GET_TEXT_F(MSG_SPEED), &feedrate_percentage, 10, 999);
      break;
    case FLOWRATE:
      ui.clear_lcd();
      MenuItemBase::itemIndex = control->data;
      #if EXTRUDERS == 1
        MenuItem_int3::action((const char *)GET_TEXT_F(MSG_FLOW), &planner.flow_percentage[MenuItemBase::itemIndex], 10, 999, []{ planner.refresh_e_factor(MenuItemBase::itemIndex); });
      #else
        MenuItem_int3::action((const char *)GET_TEXT_F(MSG_FLOW_N), &planner.flow_percentage[MenuItemBase::itemIndex], 10, 999, []{ planner.refresh_e_factor(MenuItemBase::itemIndex); });
      #endif
      break;

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      case UBL: hold(control, UBL_REPEAT_DELAY); ui.encoderPosition += control->data; break;
    #endif

    case MOVE_AXIS:
      ui.goto_screen((screenFunc_t)ui.move_axis_screen);
      break;

    // TODO: TOUCH could receive data to pass to the callback
    case BUTTON: ((screenFunc_t)control->data)(); break;

    default: break;
  }
}

void Touch::hold(touch_control_t *control, millis_t delay) {
  current_control = control;
  if (delay) {
    repeat_delay = delay > MIN_REPEAT_DELAY ? delay : MIN_REPEAT_DELAY;
    time_to_hold = last_touch_ms + repeat_delay;
  }
  ui.refresh();
}

bool Touch::get_point(int16_t *x, int16_t *y) {
  #if ENABLED(TFT_TOUCH_DEVICE_XPT2046)
    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      bool is_touched = (touch_calibration.calibration.orientation == TOUCH_PORTRAIT ? io.getRawPoint(y, x) : io.getRawPoint(x, y));

      if (is_touched && touch_calibration.calibration.orientation != TOUCH_ORIENTATION_NONE) {
        *x = int16_t((int32_t(*x) * touch_calibration.calibration.x) >> 16) + touch_calibration.calibration.offset_x;
        *y = int16_t((int32_t(*y) * touch_calibration.calibration.y) >> 16) + touch_calibration.calibration.offset_y;
      }
    #else
      bool is_touched = (TOUCH_ORIENTATION == TOUCH_PORTRAIT ? io.getRawPoint(y, x) : io.getRawPoint(x, y));
      *x = uint16_t((uint32_t(*x) * TOUCH_CALIBRATION_X) >> 16) + TOUCH_OFFSET_X;
      *y = uint16_t((uint32_t(*y) * TOUCH_CALIBRATION_Y) >> 16) + TOUCH_OFFSET_Y;
    #endif
  #elif ENABLED(TFT_TOUCH_DEVICE_GT911)
    bool is_touched = (TOUCH_ORIENTATION == TOUCH_PORTRAIT ? io.getPoint(y, x) : io.getPoint(x, y));
  #endif

  return is_touched;
}
Touch touch;

bool MarlinUI::touch_pressed() {
  return touch.is_clicked();
}

void add_control(uint16_t x, uint16_t y, TouchControlType control_type, intptr_t data, MarlinImage image, bool is_enabled, uint16_t color_enabled, uint16_t color_disabled) {
  uint16_t width = Images[image].width;
  uint16_t height = Images[image].height;
  tft.canvas(x, y, width, height);
  tft.add_image(0, 0, image, is_enabled ? color_enabled : color_disabled);
  if (is_enabled)
    touch.add_control(control_type, x, y, width, height, data);
}

#endif // TOUCH_SCREEN
