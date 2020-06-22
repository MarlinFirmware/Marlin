/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN)

#include "touch.h"
#include "pinconfig.h"

#include "../ultralcd.h"
#include "../menu/menu.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#include "tft.h"

int16_t Touch::x, Touch::y;
touchControl_t Touch::controls[];
touchControl_t *Touch::current_control;
uint16_t Touch::controls_count;
millis_t Touch::now = 0;
millis_t Touch::time_to_hold;
millis_t Touch::repeat_delay;
bool Touch::wait_for_unclick;
int32_t Touch::x_calibration = XPT2046_X_CALIBRATION, Touch::y_calibration = XPT2046_Y_CALIBRATION;
int16_t Touch::x_offset = XPT2046_X_OFFSET, Touch::y_offset = XPT2046_Y_OFFSET;

void Touch::init() {
  reset();
  io.Init();
}

void Touch::add_control(TouchControlType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int32_t data) {
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

  if (now == millis()) return;
  now = millis();

  if (get_point(&_x, &_y)) {

    #if LCD_TIMEOUT_TO_STATUS
      ui.return_to_status_ms = now + LCD_TIMEOUT_TO_STATUS;
    #endif

    if (wait_for_unclick) return;
    if (time_to_hold == 0) time_to_hold = now + MINIMUM_HOLD_TIME;
    if (PENDING(now, time_to_hold)) return;

    if (x != 0 && y != 0) {
      if (current_control) {
        if (WITHIN(x, current_control->x - FREE_MOVE_RANGE, current_control->x + current_control->width + FREE_MOVE_RANGE) && WITHIN(y, current_control->y - FREE_MOVE_RANGE, current_control->y + current_control->height + FREE_MOVE_RANGE)) {
          NOLESS(x, current_control->x);
          NOMORE(x, current_control->x + current_control->width);
          NOLESS(y, current_control->y);
          NOMORE(y, current_control->y + current_control->height);
          touch(current_control);
        }
        else {
          current_control = NULL;
        }
      }
      else {
        for (i = 0 ; i < controls_count ; i++) {
          if (WITHIN(x, controls[i].x, controls[i].x + controls[i].width) && WITHIN(y, controls[i].y, controls[i].y + controls[i].height)) {
            touch(&controls[i]);
            break;
          }
        }
      }

      if (current_control == NULL)
        wait_for_unclick = true;
    }
    x = _x;
    y = _y;
  }
  else {
    x = y = 0;
    current_control = NULL;
    wait_for_unclick = false;
    time_to_hold = 0;
    repeat_delay = TOUCH_REPEAT_DELAY;
  }
}

void Touch::touch(touchControl_t *control) {
  switch (control->type) {
    case MENU_SCREEN: ui.goto_screen((screenFunc_t)control->data); break;
    case BACK: ui.goto_previous_screen(); break;
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
      if (heater >= 0) { // HotEnd
        #if HOTENDS == 1
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_NOZZLE), &thermalManager.temp_hotend[0].target, 0, thermalManager.heater_maxtemp[0] - 15, []{ thermalManager.start_watching_hotend(0); });
        #else
          MenuItemBase::itemIndex = heater;
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_NOZZLE_N), &thermalManager.temp_hotend[heater].target, 0, thermalManager.heater_maxtemp[heater] - 15, []{ thermalManager.start_watching_hotend(MenuItemBase::itemIndex); });
        #endif
      }
      #if HAS_HEATED_BED
        else if (heater == H_BED) {
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_BED), &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 10, thermalManager.start_watching_bed);
        }
      #endif
      #if HAS_HEATED_CHAMBER
        else if (heater == H_CHAMBER) {
          MenuItem_int3::action((const char *)GET_TEXT_F(MSG_CHAMBER), &thermalManager.temp_chamber.target, 0, CHAMBER_MAXTEMP - 10, thermalManager.start_watching_chamber);
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
      case UBL_UP:    hold(control, UBL_REPEAT_DELAY); ui.encoderPosition = GRID_MAX_POINTS_X * ENCODER_STEPS_PER_MENU_ITEM; break;
      case UBL_DOWN:  hold(control, UBL_REPEAT_DELAY); ui.encoderPosition = - GRID_MAX_POINTS_X * ENCODER_STEPS_PER_MENU_ITEM; break;
      case UBL_LEFT:  hold(control, UBL_REPEAT_DELAY); ui.encoderPosition = - ENCODER_STEPS_PER_MENU_ITEM; break;
      case UBL_RIGHT: hold(control, UBL_REPEAT_DELAY); ui.encoderPosition = ENCODER_STEPS_PER_MENU_ITEM; break;
    #endif

    default: break;
  }
}

void Touch::hold(touchControl_t *control, millis_t delay) {
  current_control = control;
  if (delay) {
    repeat_delay = delay > MIN_REPEAT_DELAY ? delay : MIN_REPEAT_DELAY;
    time_to_hold = now + repeat_delay;
  }
  ui.refresh();
}

bool Touch::get_point(int16_t *x, int16_t *y) {
  bool is_touched = io.getRawPoint(x, y);
  if (is_touched) {
    *x = int16_t((int32_t(*x) * x_calibration) >> 16) + x_offset;
    *y = int16_t((int32_t(*y) * y_calibration) >> 16) + y_offset;
  }
  return is_touched;
}
Touch touch;

#endif // TOUCH_SCREEN
