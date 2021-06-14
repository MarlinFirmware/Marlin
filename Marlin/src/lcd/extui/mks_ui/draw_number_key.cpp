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
#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../gcode/gcode.h"
#include "../../../gcode/queue.h"
#include "../../../module/planner.h"
#include "../../../inc/MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "../../../module/stepper/indirection.h"
  #include "../../../feature/tmc_util.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *buttonValue = nullptr;
static lv_obj_t *labelValue  = nullptr;

static char key_value[11] = { 0 };
static uint8_t cnt = 0;
static bool point_flag = true;

enum {
  ID_NUM_KEY1 = 1,
  ID_NUM_KEY2,
  ID_NUM_KEY3,
  ID_NUM_KEY4,
  ID_NUM_KEY5,
  ID_NUM_KEY6,
  ID_NUM_KEY7,
  ID_NUM_KEY8,
  ID_NUM_KEY9,
  ID_NUM_KEY0,
  ID_NUM_BACK,
  ID_NUM_RESET,
  ID_NUM_CONFIRM,
  ID_NUM_POINT,
  ID_NUM_NEGATIVE
};

static void disp_key_value() {
  char *temp;
  TERN_(HAS_TRINAMIC_CONFIG, float milliamps);

  switch (value) {
    case PrintAcceleration:
      dtostrf(planner.settings.acceleration, 1, 1, public_buf_m);
      break;
    case RetractAcceleration:
      dtostrf(planner.settings.retract_acceleration, 1, 1, public_buf_m);
      break;
    case TravelAcceleration:
      dtostrf(planner.settings.travel_acceleration, 1, 1, public_buf_m);
      break;
    case XAcceleration:
      itoa(planner.settings.max_acceleration_mm_per_s2[X_AXIS], public_buf_m, 10);
      break;
    case YAcceleration:
      itoa(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], public_buf_m, 10);
      break;
    case ZAcceleration:
      itoa(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], public_buf_m, 10);
      break;
    case E0Acceleration:
      itoa(planner.settings.max_acceleration_mm_per_s2[E_AXIS], public_buf_m, 10);
      break;
    case E1Acceleration:
      itoa(planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)], public_buf_m, 10);
      break;
    case XMaxFeedRate:
      dtostrf(planner.settings.max_feedrate_mm_s[X_AXIS], 1, 1, public_buf_m);
      break;
    case YMaxFeedRate:
      dtostrf(planner.settings.max_feedrate_mm_s[Y_AXIS], 1, 1, public_buf_m);
      break;
    case ZMaxFeedRate:
      dtostrf(planner.settings.max_feedrate_mm_s[Z_AXIS], 1, 1, public_buf_m);
      break;
    case E0MaxFeedRate:
      dtostrf(planner.settings.max_feedrate_mm_s[E_AXIS], 1, 1, public_buf_m);
      break;
    case E1MaxFeedRate:
      dtostrf(planner.settings.max_feedrate_mm_s[E_AXIS_N(1)], 1, 1, public_buf_m);
      break;

    case XJerk:
      #if HAS_CLASSIC_JERK
        dtostrf(planner.max_jerk[X_AXIS], 1, 1, public_buf_m);
      #endif
      break;
    case YJerk:
      #if HAS_CLASSIC_JERK
        dtostrf(planner.max_jerk[Y_AXIS], 1, 1, public_buf_m);
      #endif
      break;
    case ZJerk:
      #if HAS_CLASSIC_JERK
        dtostrf(planner.max_jerk[Z_AXIS], 1, 1, public_buf_m);
      #endif
      break;
    case EJerk:
      #if HAS_CLASSIC_JERK
        dtostrf(planner.max_jerk[E_AXIS], 1, 1, public_buf_m);
      #endif
      break;

    case Xstep:
      dtostrf(planner.settings.axis_steps_per_mm[X_AXIS], 1, 1, public_buf_m);
      break;
    case Ystep:
      dtostrf(planner.settings.axis_steps_per_mm[Y_AXIS], 1, 1, public_buf_m);

      break;
    case Zstep:
      dtostrf(planner.settings.axis_steps_per_mm[Z_AXIS], 1, 1, public_buf_m);

      break;
    case E0step:
      dtostrf(planner.settings.axis_steps_per_mm[E_AXIS], 1, 1, public_buf_m);

      break;
    case E1step:
      dtostrf(planner.settings.axis_steps_per_mm[E_AXIS_N(1)], 1, 1, public_buf_m);
      break;

    case Xcurrent:
      #if AXIS_IS_TMC(X)
        milliamps = stepperX.getMilliamps();
        dtostrf(milliamps, 1, 1, public_buf_m);
      #endif
      break;

    case Ycurrent:
      #if AXIS_IS_TMC(Y)
        milliamps = stepperY.getMilliamps();
        dtostrf(milliamps, 1, 1, public_buf_m);
      #endif
      break;

    case Zcurrent:
      #if AXIS_IS_TMC(Z)
        milliamps = stepperZ.getMilliamps();
        dtostrf(milliamps, 1, 1, public_buf_m);
      #endif
      break;

    case E0current:
      #if AXIS_IS_TMC(E0)
        milliamps = stepperE0.getMilliamps();
        dtostrf(milliamps, 1, 1, public_buf_m);
      #endif
      break;

    case E1current:
      #if AXIS_IS_TMC(E1)
        milliamps = stepperE1.getMilliamps();
        dtostrf(milliamps, 1, 1, public_buf_m);
      #endif
      break;

    case pause_pos_x:
      dtostrf(gCfgItems.pausePosX, 1, 1, public_buf_m);
      break;
    case pause_pos_y:
      dtostrf(gCfgItems.pausePosY, 1, 1, public_buf_m);
      break;
    case pause_pos_z:
      dtostrf(gCfgItems.pausePosZ, 1, 1, public_buf_m);
      break;
    case level_pos_x1:
      itoa(gCfgItems.trammingPos[0].x, public_buf_m, 10);
      break;
    case level_pos_y1:
      itoa(gCfgItems.trammingPos[0].y, public_buf_m, 10);
      break;
    case level_pos_x2:
      itoa(gCfgItems.trammingPos[1].x, public_buf_m, 10);
      break;
    case level_pos_y2:
      itoa(gCfgItems.trammingPos[1].y, public_buf_m, 10);
      break;
    case level_pos_x3:
      itoa(gCfgItems.trammingPos[2].x, public_buf_m, 10);
      break;
    case level_pos_y3:
      itoa(gCfgItems.trammingPos[2].y, public_buf_m, 10);
      break;
    case level_pos_x4:
      itoa(gCfgItems.trammingPos[3].x, public_buf_m, 10);
      break;
    case level_pos_y4:
      itoa(gCfgItems.trammingPos[3].y, public_buf_m, 10);
      break;
    case level_pos_x5:
      itoa(gCfgItems.trammingPos[4].x, public_buf_m, 10);
      break;
    case level_pos_y5:
      itoa(gCfgItems.trammingPos[4].y, public_buf_m, 10);
      break;
    #if HAS_BED_PROBE
      case x_offset:
        #if HAS_PROBE_XY_OFFSET
          dtostrf(probe.offset.x, 1, 3, public_buf_m);
        #endif
        break;
      case y_offset:
        #if HAS_PROBE_XY_OFFSET
          dtostrf(probe.offset.y, 1, 3, public_buf_m);
        #endif
        break;
      case z_offset:
        dtostrf(probe.offset.z, 1, 3, public_buf_m);
        break;
    #endif
    case load_length:
      itoa(gCfgItems.filamentchange_load_length, public_buf_m, 10);
      break;
    case load_speed:
      itoa(gCfgItems.filamentchange_load_speed, public_buf_m, 10);
      break;
    case unload_length:
      itoa(gCfgItems.filamentchange_unload_length, public_buf_m, 10);
      break;
    case unload_speed:
      itoa(gCfgItems.filamentchange_unload_speed, public_buf_m, 10);
      break;
    case filament_temp:
      itoa(gCfgItems.filament_limit_temp, public_buf_m, 10);
      break;
    case x_sensitivity:
      #if X_SENSORLESS
        itoa(TERN(X_SENSORLESS, stepperX.homing_threshold(), 0), public_buf_m, 10);
      #endif
      break;
    case y_sensitivity:
      #if Y_SENSORLESS
        itoa(TERN(Y_SENSORLESS, stepperY.homing_threshold(), 0), public_buf_m, 10);
      #endif
      break;
    case z_sensitivity:
      #if Z_SENSORLESS
        itoa(TERN(Z_SENSORLESS, stepperZ.homing_threshold(), 0), public_buf_m, 10);
      #endif
      break;
    case z2_sensitivity:
      #if Z2_SENSORLESS
        itoa(TERN(Z2_SENSORLESS, stepperZ2.homing_threshold(), 0), public_buf_m, 10);
      #endif
      break;
  }

  strcpy(key_value, public_buf_m);
  cnt  = strlen(key_value);
  temp = strchr(key_value, '.');
  point_flag = !temp;
  lv_label_set_text(labelValue, key_value);
  lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);

}

static void set_value_confirm() {
  switch (value) {
    case PrintAcceleration:   planner.settings.acceleration = atof(key_value); break;
    case RetractAcceleration: planner.settings.retract_acceleration = atof(key_value); break;
    case TravelAcceleration:  planner.settings.travel_acceleration = atof(key_value); break;
    case XAcceleration:  planner.settings.max_acceleration_mm_per_s2[X_AXIS] = atof(key_value); break;
    case YAcceleration:  planner.settings.max_acceleration_mm_per_s2[Y_AXIS] = atof(key_value); break;
    case ZAcceleration:  planner.settings.max_acceleration_mm_per_s2[Z_AXIS] = atof(key_value); break;
    case E0Acceleration: planner.settings.max_acceleration_mm_per_s2[E_AXIS] = atof(key_value); break;
    case E1Acceleration: planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)] = atof(key_value); break;
    case XMaxFeedRate:   planner.settings.max_feedrate_mm_s[X_AXIS] = atof(key_value); break;
    case YMaxFeedRate:   planner.settings.max_feedrate_mm_s[Y_AXIS] = atof(key_value); break;
    case ZMaxFeedRate:   planner.settings.max_feedrate_mm_s[Z_AXIS] = atof(key_value); break;
    case E0MaxFeedRate:  planner.settings.max_feedrate_mm_s[E_AXIS] = atof(key_value); break;
    case E1MaxFeedRate:  planner.settings.max_feedrate_mm_s[E_AXIS_N(1)] = atof(key_value); break;
    case XJerk: TERN_(HAS_CLASSIC_JERK, planner.max_jerk[X_AXIS] = atof(key_value)); break;
    case YJerk: TERN_(HAS_CLASSIC_JERK, planner.max_jerk[Y_AXIS] = atof(key_value)); break;
    case ZJerk: TERN_(HAS_CLASSIC_JERK, planner.max_jerk[Z_AXIS] = atof(key_value)); break;
    case EJerk: TERN_(HAS_CLASSIC_JERK, planner.max_jerk[E_AXIS] = atof(key_value)); break;
    case Xstep:  planner.settings.axis_steps_per_mm[X_AXIS] = atof(key_value); planner.refresh_positioning(); break;
    case Ystep:  planner.settings.axis_steps_per_mm[Y_AXIS] = atof(key_value); planner.refresh_positioning(); break;
    case Zstep:  planner.settings.axis_steps_per_mm[Z_AXIS] = atof(key_value); planner.refresh_positioning(); break;
    case E0step: planner.settings.axis_steps_per_mm[E_AXIS] = atof(key_value); planner.refresh_positioning(); break;
    case E1step: planner.settings.axis_steps_per_mm[E_AXIS_N(1)] = atof(key_value); planner.refresh_positioning(); break;
    case Xcurrent:
      #if AXIS_IS_TMC(X)
        stepperX.rms_current(atoi(key_value));
      #endif
      break;
    case Ycurrent:
      #if AXIS_IS_TMC(Y)
        stepperY.rms_current(atoi(key_value));
      #endif
      break;
    case Zcurrent:
      #if AXIS_IS_TMC(Z)
        stepperZ.rms_current(atoi(key_value));
      #endif
      break;
    case E0current:
      #if AXIS_IS_TMC(E0)
        stepperE0.rms_current(atoi(key_value));
      #endif
      break;
    case E1current:
      #if AXIS_IS_TMC(E1)
        stepperE1.rms_current(atoi(key_value));
      #endif
      break;
    case pause_pos_x: gCfgItems.pausePosX = atof(key_value); update_spi_flash(); break;
    case pause_pos_y: gCfgItems.pausePosY = atof(key_value); update_spi_flash(); break;
    case pause_pos_z: gCfgItems.pausePosZ = atof(key_value); update_spi_flash(); break;
    case level_pos_x1: gCfgItems.trammingPos[0].x = atoi(key_value); update_spi_flash(); break;
    case level_pos_y1: gCfgItems.trammingPos[0].y = atoi(key_value); update_spi_flash(); break;
    case level_pos_x2: gCfgItems.trammingPos[1].x = atoi(key_value); update_spi_flash(); break;
    case level_pos_y2: gCfgItems.trammingPos[1].y = atoi(key_value); update_spi_flash(); break;
    case level_pos_x3: gCfgItems.trammingPos[2].x = atoi(key_value); update_spi_flash(); break;
    case level_pos_y3: gCfgItems.trammingPos[2].y = atoi(key_value); update_spi_flash(); break;
    case level_pos_x4: gCfgItems.trammingPos[3].x = atoi(key_value); update_spi_flash(); break;
    case level_pos_y4: gCfgItems.trammingPos[3].y = atoi(key_value); update_spi_flash(); break;
    case level_pos_x5: gCfgItems.trammingPos[4].x = atoi(key_value); update_spi_flash(); break;
    case level_pos_y5: gCfgItems.trammingPos[4].y = atoi(key_value); update_spi_flash(); break;
    #if HAS_BED_PROBE
      case x_offset: {
        #if HAS_PROBE_XY_OFFSET
          const float x = atof(key_value);
          if (WITHIN(x, -(X_BED_SIZE), X_BED_SIZE))
            probe.offset.x = x;
        #endif
      } break;
      case y_offset: {
        #if HAS_PROBE_XY_OFFSET
          const float y = atof(key_value);
          if (WITHIN(y, -(Y_BED_SIZE), Y_BED_SIZE))
            probe.offset.y = y;
        #endif
      } break;
      case z_offset: {
        const float z = atof(key_value);
        if (WITHIN(z, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          probe.offset.z = z;
      } break;
    #endif
    case load_length:
      gCfgItems.filamentchange_load_length = atoi(key_value);
      uiCfg.filament_loading_time = (uint32_t)((gCfgItems.filamentchange_load_length*60.0/gCfgItems.filamentchange_load_speed)+0.5);
      update_spi_flash();
      break;
    case load_speed:
      gCfgItems.filamentchange_load_speed = atoi(key_value);
      uiCfg.filament_loading_time = (uint32_t)((gCfgItems.filamentchange_load_length*60.0/gCfgItems.filamentchange_load_speed)+0.5);
      update_spi_flash();
      break;
    case unload_length:
      gCfgItems.filamentchange_unload_length = atoi(key_value);
      uiCfg.filament_unloading_time = (uint32_t)((gCfgItems.filamentchange_unload_length*60.0/gCfgItems.filamentchange_unload_speed)+0.5);
      update_spi_flash();
      break;
    case unload_speed:
      gCfgItems.filamentchange_unload_speed = atoi(key_value);
      uiCfg.filament_unloading_time = (uint32_t)((gCfgItems.filamentchange_unload_length*60.0/gCfgItems.filamentchange_unload_speed)+0.5);
      update_spi_flash();
      break;
    case filament_temp:
      gCfgItems.filament_limit_temp = atoi(key_value);
      update_spi_flash();
      break;
    case x_sensitivity: TERN_(X_SENSORLESS, stepperX.homing_threshold(atoi(key_value))); break;
    case y_sensitivity: TERN_(Y_SENSORLESS, stepperY.homing_threshold(atoi(key_value))); break;
    case z_sensitivity: TERN_(Z_SENSORLESS, stepperZ.homing_threshold(atoi(key_value))); break;
    case z2_sensitivity: TERN_(Z2_SENSORLESS, stepperZ2.homing_threshold(atoi(key_value))); break;
  }
  gcode.process_subcommands_now_P(PSTR("M500"));
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_NUM_KEY1 ... ID_NUM_KEY0:
      if (cnt <= 10) {
        key_value[cnt] = (obj->mks_obj_id == ID_NUM_KEY0) ? (char)'0' : char('1' + obj->mks_obj_id - ID_NUM_KEY1);
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_BACK:
      if (cnt > 0) cnt--;
      if (key_value[cnt] == (char)'.') point_flag = true;
      key_value[cnt] = (char)'\0';
      lv_label_set_text(labelValue, key_value);
      lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
      break;
    case ID_NUM_RESET:
      ZERO(key_value);
      cnt = 0;
      key_value[cnt] = (char)'0';
      point_flag = true;
      lv_label_set_text(labelValue, key_value);
      lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
      break;
    case ID_NUM_POINT:
      if (cnt != 0 && point_flag) {
        point_flag = false;
        key_value[cnt] = (char)'.';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_NEGATIVE:
      if (cnt == 0) {
        key_value[cnt] = (char)'-';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_CONFIRM:
      last_disp_state = NUMBER_KEY_UI;
      if (strlen(key_value) != 0) set_value_confirm();
      lv_clear_number_key();
      draw_return_ui();
      break;
  }
}

void lv_draw_number_key() {
  scr = lv_screen_create(NUMBER_KEY_UI, "");

  buttonValue = lv_btn_create(scr, 92, 40, 296, 40, event_handler, ID_NUM_KEY1, &style_num_text);
  labelValue = lv_label_create_empty(buttonValue);

  #define DRAW_NUMBER_KEY(N,X,Y) \
    lv_obj_t *NumberKey_##N = lv_btn_create(scr, X, Y, 68, 40, event_handler, ID_NUM_KEY##N, &style_num_key_pre); \
    lv_obj_t *labelKey_##N = lv_label_create_empty(NumberKey_##N); \
    lv_label_set_text(labelKey_##N, machine_menu.key_##N); \
    lv_obj_align(labelKey_##N, NumberKey_##N, LV_ALIGN_CENTER, 0, 0)

  DRAW_NUMBER_KEY(1,  92,  90);
  DRAW_NUMBER_KEY(2, 168,  90);
  DRAW_NUMBER_KEY(3, 244,  90);
  DRAW_NUMBER_KEY(4,  92, 140);
  DRAW_NUMBER_KEY(5, 168, 140);
  DRAW_NUMBER_KEY(6, 244, 140);
  DRAW_NUMBER_KEY(7,  92, 190);
  DRAW_NUMBER_KEY(8, 168, 190);
  DRAW_NUMBER_KEY(9, 244, 190);
  DRAW_NUMBER_KEY(0,  92, 240);

  lv_obj_t *KeyBack = lv_btn_create(scr, 320, 90, 68, 40, event_handler, ID_NUM_BACK, &style_num_key_pre);
  lv_obj_t *labelKeyBack = lv_label_create_empty(KeyBack);
  lv_label_set_text(labelKeyBack, machine_menu.key_back);
  lv_obj_align(labelKeyBack, KeyBack, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *KeyReset = lv_btn_create(scr, 320, 140, 68, 40, event_handler, ID_NUM_RESET, &style_num_key_pre);
  lv_obj_t *labelKeyReset = lv_label_create_empty(KeyReset);
  lv_label_set_text(labelKeyReset, machine_menu.key_reset);
  lv_obj_align(labelKeyReset, KeyReset, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *KeyConfirm = lv_btn_create(scr, 320, 190, 68, 90, event_handler, ID_NUM_CONFIRM, &style_num_key_pre);
  lv_obj_t *labelKeyConfirm = lv_label_create_empty(KeyConfirm);
  lv_label_set_text(labelKeyConfirm, machine_menu.key_confirm);
  lv_obj_align(labelKeyConfirm, KeyConfirm, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *KeyPoint = lv_btn_create(scr, 244, 240, 68, 40, event_handler, ID_NUM_POINT, &style_num_key_pre);
  lv_obj_t *labelKeyPoint = lv_label_create_empty(KeyPoint);
  lv_label_set_text(labelKeyPoint, machine_menu.key_point);
  lv_obj_align(labelKeyPoint, KeyPoint, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *Minus = lv_btn_create(scr, 168, 240, 68, 40, event_handler, ID_NUM_NEGATIVE, &style_num_key_pre);
  lv_obj_t *labelMinus = lv_label_create_empty(Minus);
  lv_label_set_text(labelMinus, machine_menu.negative);
  lv_obj_align(labelMinus, Minus, LV_ALIGN_CENTER, 0, 0);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, NumberKey_1);
      lv_group_add_obj(g, NumberKey_2);
      lv_group_add_obj(g, NumberKey_3);
      lv_group_add_obj(g, KeyBack);
      lv_group_add_obj(g, NumberKey_4);
      lv_group_add_obj(g, NumberKey_5);
      lv_group_add_obj(g, NumberKey_6);
      lv_group_add_obj(g, KeyReset);
      lv_group_add_obj(g, NumberKey_7);
      lv_group_add_obj(g, NumberKey_8);
      lv_group_add_obj(g, NumberKey_9);
      lv_group_add_obj(g, NumberKey_0);
      lv_group_add_obj(g, Minus);
      lv_group_add_obj(g, KeyPoint);
      lv_group_add_obj(g, KeyConfirm);
    }
  #endif

  disp_key_value();
}

void lv_clear_number_key() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
