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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN)

#include "touch.h"

#include "../ultralcd.h"
#include "../menu/menu.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#include "tft.h"

int16_t Touch::x, Touch::y;
touch_control_t Touch::controls[];
touch_control_t *Touch::current_control;
uint16_t Touch::controls_count;
millis_t Touch::now = 0;
millis_t Touch::time_to_hold;
millis_t Touch::repeat_delay;
millis_t Touch::touch_time;
TouchControlType  Touch::touch_control_type = NONE;
touch_calibration_t Touch::calibration;
#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  calibrationState Touch::calibration_state = CALIBRATION_NONE;
  touch_calibration_point_t Touch::calibration_points[4];
#endif

void Touch::init() {
  calibration_reset();
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

    if (touch_time) {
      #if ENABLED(TOUCH_SCREEN_CALIBRATION)
        if (touch_control_type == NONE && ELAPSED(now, touch_time + TOUCH_SCREEN_HOLD_TO_CALIBRATE_MS) && ui.on_status_screen())
          ui.goto_screen(touch_screen_calibration);
      #endif
      return;
    }

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
        for (i = 0; i < controls_count; i++) {
          if ((WITHIN(x, controls[i].x, controls[i].x + controls[i].width) && WITHIN(y, controls[i].y, controls[i].y + controls[i].height)) || (TERN(TOUCH_SCREEN_CALIBRATION, controls[i].type == CALIBRATE, false))) {
            touch_control_type = controls[i].type;
            touch(&controls[i]);
            break;
          }
        }
      }

      if (current_control == NULL)
        touch_time = now;
    }
    x = _x;
    y = _y;
  }
  else {
    x = y = 0;
    current_control = NULL;
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
        ui.refresh();

        if (calibration_state < CALIBRATION_SUCCESS) {
          calibration_points[calibration_state].x = int16_t(control->data >> 16);
          calibration_points[calibration_state].y = int16_t(control->data & 0xFFFF);
          calibration_points[calibration_state].raw_x = x;
          calibration_points[calibration_state].raw_y = y;
        }

        switch (calibration_state) {
          case CALIBRATION_POINT_1: calibration_state = CALIBRATION_POINT_2; break;
          case CALIBRATION_POINT_2: calibration_state = CALIBRATION_POINT_3; break;
          case CALIBRATION_POINT_3: calibration_state = CALIBRATION_POINT_4; break;
          case CALIBRATION_POINT_4:
            if (validate_precision_x(0, 1) && validate_precision_x(2, 3) && validate_precision_y(0, 2) && validate_precision_y(1, 3)) {
              calibration_state = CALIBRATION_SUCCESS;
              calibration.x = ((calibration_points[2].x - calibration_points[0].x) << 17) / (calibration_points[3].raw_x + calibration_points[2].raw_x - calibration_points[1].raw_x - calibration_points[0].raw_x);
              calibration.y = ((calibration_points[1].y - calibration_points[0].y) << 17) / (calibration_points[3].raw_y - calibration_points[2].raw_y + calibration_points[1].raw_y - calibration_points[0].raw_y);
              calibration.offset_x = calibration_points[0].x - int16_t(((calibration_points[0].raw_x + calibration_points[1].raw_x) * calibration.x) >> 17);
              calibration.offset_y = calibration_points[0].y - int16_t(((calibration_points[0].raw_y + calibration_points[2].raw_y) * calibration.y) >> 17);
              calibration.orientation = TOUCH_LANDSCAPE;
            }
            else if (validate_precision_y(0, 1) && validate_precision_y(2, 3) && validate_precision_x(0, 2) && validate_precision_x(1, 3)) {
              calibration_state = CALIBRATION_SUCCESS;
              calibration.x = ((calibration_points[2].x - calibration_points[0].x) << 17) / (calibration_points[3].raw_y + calibration_points[2].raw_y - calibration_points[1].raw_y - calibration_points[0].raw_y);
              calibration.y = ((calibration_points[1].y - calibration_points[0].y) << 17) / (calibration_points[3].raw_x - calibration_points[2].raw_x + calibration_points[1].raw_x - calibration_points[0].raw_x);
              calibration.offset_x = calibration_points[0].x - int16_t(((calibration_points[0].raw_y + calibration_points[1].raw_y) * calibration.x) >> 17);
              calibration.offset_y = calibration_points[0].y - int16_t(((calibration_points[0].raw_x + calibration_points[2].raw_x) * calibration.y) >> 17);
              calibration.orientation = TOUCH_PORTRAIT;
            }
            else {
              calibration_state = CALIBRATION_FAIL;
              calibration_reset();
            }

            if (calibration_state == CALIBRATION_SUCCESS) {
              SERIAL_ECHOLN("Touch screen calibration completed");
              SERIAL_ECHOLNPAIR("TOUCH_CALIBRATION_X ", calibration.x);
              SERIAL_ECHOLNPAIR("TOUCH_CALIBRATION_Y ", calibration.y);
              SERIAL_ECHOLNPAIR("TOUCH_OFFSET_X ", calibration.offset_x);
              SERIAL_ECHOLNPAIR("TOUCH_OFFSET_Y ", calibration.offset_y);
              SERIAL_ECHO("TOUCH_ORIENTATION "); if (calibration.orientation == TOUCH_LANDSCAPE) SERIAL_ECHOLN("TOUCH_LANDSCAPE"); else SERIAL_ECHOLN("TOUCH_PORTRAIT");
            }
            break;
          default: break;
        }
        break;
    #endif // TOUCH_SCREEN_CALIBRATION

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
      case UBL: hold(control, UBL_REPEAT_DELAY); ui.encoderPosition += control->data; break;
    #endif

    default: break;
  }
}

void Touch::hold(touch_control_t *control, millis_t delay) {
  current_control = control;
  if (delay) {
    repeat_delay = delay > MIN_REPEAT_DELAY ? delay : MIN_REPEAT_DELAY;
    time_to_hold = now + repeat_delay;
  }
  ui.refresh();
}

bool Touch::get_point(int16_t *x, int16_t *y) {
  bool is_touched = (calibration.orientation == TOUCH_PORTRAIT ? io.getRawPoint(y, x) : io.getRawPoint(x, y));

  if (is_touched && calibration.orientation != TOUCH_ORIENTATION_NONE) {
    *x = int16_t((int32_t(*x) * calibration.x) >> 16) + calibration.offset_x;
    *y = int16_t((int32_t(*y) * calibration.y) >> 16) + calibration.offset_y;
  }
  return is_touched;
}
Touch touch;

bool MarlinUI::touch_pressed() {
  return touch.is_clicked();
}

void add_control(uint16_t x, uint16_t y, TouchControlType control_type, int32_t data, MarlinImage image, bool is_enabled, uint16_t color_enabled, uint16_t color_disabled) {
  uint16_t width = Images[image].width;
  uint16_t height = Images[image].height;
  tft.canvas(x, y, width, height);
  tft.add_image(0, 0, image, is_enabled ? color_enabled : color_disabled);
  if (is_enabled)
    touch.add_control(control_type, x, y, width, height, data);
}

#endif // TOUCH_SCREEN
