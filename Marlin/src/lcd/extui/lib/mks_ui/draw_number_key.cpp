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
#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>
//#include "../../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../../lvgl/src/lv_objx/lv_img.h"
//#include "../../lvgl/src/lv_core/lv_disp.h"
//#include "../../lvgl/src/lv_core/lv_refr.h"

#include "../../../../gcode/gcode.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../module/temperature.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "../../../../module/stepper/indirection.h"
  #include "../../../../feature/tmc_util.h"
#endif

#if HAS_BED_PROBE
  #include "../../../../module/probe.h"
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
  #if HAS_TRINAMIC_CONFIG
    float milliamps;
  #endif

  switch (value) {
    case PrintAcceleration:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.acceleration);
      break;
    case RetractAcceleration:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.retract_acceleration);
      break;
    case TravelAcceleration:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.travel_acceleration);
      break;
    case XAcceleration:
      sprintf_P(public_buf_m, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
      break;
    case YAcceleration:
      sprintf_P(public_buf_m, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
      break;
    case ZAcceleration:
      sprintf_P(public_buf_m, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
      break;
    case E0Acceleration:
      sprintf_P(public_buf_m, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
      break;
    case E1Acceleration:
      sprintf_P(public_buf_m, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)]);
      break;
    case XMaxFeedRate:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.max_feedrate_mm_s[X_AXIS]);
      break;
    case YMaxFeedRate:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.max_feedrate_mm_s[Y_AXIS]);
      break;
    case ZMaxFeedRate:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.max_feedrate_mm_s[Z_AXIS]);
      break;
    case E0MaxFeedRate:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.max_feedrate_mm_s[E_AXIS]);
      break;
    case E1MaxFeedRate:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.max_feedrate_mm_s[E_AXIS_N(1)]);
      break;

    case XJerk:
      #if HAS_CLASSIC_JERK
        sprintf_P(public_buf_m, PSTR("%.1f"), planner.max_jerk[X_AXIS]);
      #endif
      break;
    case YJerk:
      #if HAS_CLASSIC_JERK
        sprintf_P(public_buf_m, PSTR("%.1f"), planner.max_jerk[Y_AXIS]);
      #endif
      break;
    case ZJerk:
      #if HAS_CLASSIC_JERK
        sprintf_P(public_buf_m, PSTR("%.1f"), planner.max_jerk[Z_AXIS]);
      #endif
      break;
    case EJerk:
      #if HAS_CLASSIC_JERK
        sprintf_P(public_buf_m, PSTR("%.1f"), planner.max_jerk[E_AXIS]);
      #endif
      break;

    case Xstep:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.axis_steps_per_mm[X_AXIS]);

      break;
    case Ystep:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.axis_steps_per_mm[Y_AXIS]);

      break;
    case Zstep:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.axis_steps_per_mm[Z_AXIS]);

      break;
    case E0step:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.axis_steps_per_mm[E_AXIS]);

      break;
    case E1step:
      sprintf_P(public_buf_m, PSTR("%.1f"), planner.settings.axis_steps_per_mm[E_AXIS_N(1)]);
      break;

    case Xcurrent:
      #if AXIS_IS_TMC(X)
        milliamps = stepperX.getMilliamps();
        sprintf_P(public_buf_m, PSTR("%.1f"), milliamps);
      #endif
      break;

    case Ycurrent:
      #if AXIS_IS_TMC(Y)
        milliamps = stepperY.getMilliamps();
        sprintf_P(public_buf_m, PSTR("%.1f"), milliamps);
      #endif
      break;

    case Zcurrent:
      #if AXIS_IS_TMC(Z)
        milliamps = stepperZ.getMilliamps();
        sprintf_P(public_buf_m, PSTR("%.1f"), milliamps);
      #endif
      break;

    case E0current:
      #if AXIS_IS_TMC(E0)
        milliamps = stepperE0.getMilliamps();
        sprintf_P(public_buf_m, PSTR("%.1f"), milliamps);
      #endif
      break;

    case E1current:
      #if AXIS_IS_TMC(E1)
        milliamps = stepperE1.getMilliamps();
        sprintf_P(public_buf_m, PSTR("%.1f"), milliamps);
      #endif
      break;

    case pause_pos_x:
      sprintf_P(public_buf_m, PSTR("%.1f"), gCfgItems.pausePosX);
      break;
    case pause_pos_y:
      sprintf_P(public_buf_m, PSTR("%.1f"), gCfgItems.pausePosY);
      break;
    case pause_pos_z:
      sprintf_P(public_buf_m, PSTR("%.1f"), gCfgItems.pausePosZ);
      break;
    case level_pos_x1:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[0][0]);
      break;
    case level_pos_y1:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[0][1]);
      break;
    case level_pos_x2:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[1][0]);
      break;
    case level_pos_y2:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[1][1]);
      break;
    case level_pos_x3:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[2][0]);
      break;
    case level_pos_y3:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[2][1]);
      break;
    case level_pos_x4:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[3][0]);
      break;
    case level_pos_y4:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[3][1]);
      break;
    case level_pos_x5:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[4][0]);
      break;
    case level_pos_y5:
      sprintf_P(public_buf_m, PSTR("%d"), (int)gCfgItems.levelingPos[4][1]);
      break;
    #if HAS_BED_PROBE
      case x_offset:
        #if HAS_PROBE_XY_OFFSET
        sprintf_P(public_buf_m, PSTR("%.1f"), probe.offset.x);
        #endif
        break;
      case y_offset:
        #if HAS_PROBE_XY_OFFSET
        sprintf_P(public_buf_m, PSTR("%.1f"), probe.offset.y);
        #endif
        break;
      case z_offset:
        sprintf_P(public_buf_m, PSTR("%.1f"), probe.offset.z);
        break;
    #endif
    case load_length:
      sprintf_P(public_buf_m, PSTR("%d"), gCfgItems.filamentchange_load_length);
      break;
    case load_speed:
      sprintf_P(public_buf_m, PSTR("%d"), gCfgItems.filamentchange_load_speed);
      break;
    case unload_length:
      sprintf_P(public_buf_m, PSTR("%d"), gCfgItems.filamentchange_unload_length);
      break;
    case unload_speed:
      sprintf_P(public_buf_m, PSTR("%d"), gCfgItems.filamentchange_unload_speed);
      break;
    case filament_temp:
      sprintf_P(public_buf_m, PSTR("%d"), gCfgItems.filament_limit_temper);
      break;
    case x_sensitivity:
      #if X_SENSORLESS
        sprintf_P(public_buf_m, PSTR("%d"), TERN(X_SENSORLESS, stepperX.homing_threshold(), 0));
      #endif
      break;
    case y_sensitivity:
      #if Y_SENSORLESS
        sprintf_P(public_buf_m, PSTR("%d"), TERN(Y_SENSORLESS, stepperY.homing_threshold(), 0));
      #endif
      break;
    case z_sensitivity:
      #if Z_SENSORLESS
        sprintf_P(public_buf_m, PSTR("%d"), TERN(Z_SENSORLESS, stepperZ.homing_threshold(), 0));
      #endif
      break;
    case z2_sensitivity:
      #if Z2_SENSORLESS
        sprintf_P(public_buf_m, PSTR("%d"), TERN(Z2_SENSORLESS, stepperZ2.homing_threshold(), 0));
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
  #if HAS_TRINAMIC_CONFIG
    uint16_t current_mA;
  #endif
  switch (value) {
    case PrintAcceleration:
      planner.settings.acceleration = atof(key_value);
      break;
    case RetractAcceleration:
      planner.settings.retract_acceleration = atof(key_value);
      break;
    case TravelAcceleration:
      planner.settings.travel_acceleration = atof(key_value);
      break;
    case XAcceleration:
      planner.settings.max_acceleration_mm_per_s2[X_AXIS] = atof(key_value);
      break;
    case YAcceleration:
      planner.settings.max_acceleration_mm_per_s2[Y_AXIS] = atof(key_value);
      break;
    case ZAcceleration:
      planner.settings.max_acceleration_mm_per_s2[Z_AXIS] = atof(key_value);
      break;
    case E0Acceleration:
      planner.settings.max_acceleration_mm_per_s2[E_AXIS] = atof(key_value);
      break;
    case E1Acceleration:
      planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)] = atof(key_value);
      break;
    case XMaxFeedRate:
      planner.settings.max_feedrate_mm_s[X_AXIS] = atof(key_value);
      break;
    case YMaxFeedRate:
      planner.settings.max_feedrate_mm_s[Y_AXIS] = atof(key_value);
      break;
    case ZMaxFeedRate:
      planner.settings.max_feedrate_mm_s[Z_AXIS] = atof(key_value);
      break;
    case E0MaxFeedRate:
      planner.settings.max_feedrate_mm_s[E_AXIS] = atof(key_value);
      break;
    case E1MaxFeedRate:
      planner.settings.max_feedrate_mm_s[E_AXIS_N(1)] = atof(key_value);
      break;
    case XJerk:
      #if HAS_CLASSIC_JERK
        planner.max_jerk[X_AXIS] = atof(key_value);
      #endif
      break;
    case YJerk:
      #if HAS_CLASSIC_JERK
        planner.max_jerk[Y_AXIS] = atof(key_value);
      #endif
      break;
    case ZJerk:
      #if HAS_CLASSIC_JERK
        planner.max_jerk[Z_AXIS] = atof(key_value);
      #endif
      break;
    case EJerk:
      #if HAS_CLASSIC_JERK
        planner.max_jerk[E_AXIS] = atof(key_value);
      #endif
      break;
    case Xstep:
      planner.settings.axis_steps_per_mm[X_AXIS] = atof(key_value);
      planner.refresh_positioning();
      break;
    case Ystep:
      planner.settings.axis_steps_per_mm[Y_AXIS] = atof(key_value);
      planner.refresh_positioning();
      break;
    case Zstep:
      planner.settings.axis_steps_per_mm[Z_AXIS] = atof(key_value);
      planner.refresh_positioning();
      break;
    case E0step:
      planner.settings.axis_steps_per_mm[E_AXIS] = atof(key_value);
      planner.refresh_positioning();
      break;
    case E1step:
      planner.settings.axis_steps_per_mm[E_AXIS_N(1)] = atof(key_value);
      planner.refresh_positioning();
      break;
    case Xcurrent:
      #if AXIS_IS_TMC(X)
        current_mA = atoi(key_value);
        stepperX.rms_current(current_mA);
      #endif
      break;
    case Ycurrent:
      #if AXIS_IS_TMC(Y)
        current_mA = atoi(key_value);
        stepperY.rms_current(current_mA);
      #endif
      break;
    case Zcurrent:
      #if AXIS_IS_TMC(Z)
        current_mA = atoi(key_value);
        stepperZ.rms_current(current_mA);
      #endif
      break;
    case E0current:
      #if AXIS_IS_TMC(E0)
        current_mA = atoi(key_value);
        stepperE0.rms_current(current_mA);
      #endif
      break;
    case E1current:
      #if AXIS_IS_TMC(E1)
        current_mA = atoi(key_value);
        stepperE1.rms_current(current_mA);
      #endif
      break;
    case pause_pos_x:
      gCfgItems.pausePosX = atof(key_value);
      update_spi_flash();
      break;
    case pause_pos_y:
      gCfgItems.pausePosY = atof(key_value);
      update_spi_flash();
      break;
    case pause_pos_z:
      gCfgItems.pausePosZ = atof(key_value);
      update_spi_flash();
      break;
    case level_pos_x1:
      gCfgItems.levelingPos[0][0] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_y1:
      gCfgItems.levelingPos[0][1] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_x2:
      gCfgItems.levelingPos[1][0] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_y2:
      gCfgItems.levelingPos[1][1] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_x3:
      gCfgItems.levelingPos[2][0] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_y3:
      gCfgItems.levelingPos[2][1] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_x4:
      gCfgItems.levelingPos[3][0] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_y4:
      gCfgItems.levelingPos[3][1] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_x5:
      gCfgItems.levelingPos[4][0] = atoi(key_value);
      update_spi_flash();
      break;
    case level_pos_y5:
      gCfgItems.levelingPos[4][1] = atoi(key_value);
      update_spi_flash();
      break;
    #if HAS_BED_PROBE
      case x_offset:
        #if HAS_PROBE_XY_OFFSET
          float x;
          x = atof(key_value);
          if (WITHIN(x, -(X_BED_SIZE), X_BED_SIZE))
            probe.offset.x = x;
        #endif
        break;
      case y_offset:
        #if HAS_PROBE_XY_OFFSET
          float y;
          y = atof(key_value);
          if (WITHIN(y, -(Y_BED_SIZE), Y_BED_SIZE))
            probe.offset.y = y;
        #endif
        break;
      case z_offset:
        float z;
        z = atof(key_value);
        if (WITHIN(z, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          probe.offset.z = z;
        break;
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
      gCfgItems.filament_limit_temper = atoi(key_value);
      update_spi_flash();
      break;
    case x_sensitivity:
      #if X_SENSORLESS
        stepperX.homing_threshold(atoi(key_value));
      #endif
      break;
    case y_sensitivity:
      #if Y_SENSORLESS
        stepperY.homing_threshold(atoi(key_value));
      #endif
      break;
    case z_sensitivity:
      #if Z_SENSORLESS
        stepperZ.homing_threshold(atoi(key_value));
      #endif
      break;
    case z2_sensitivity:
      #if Z2_SENSORLESS
        stepperZ2.homing_threshold(atoi(key_value));
      #endif
      break;
  }
  gcode.process_subcommands_now_P(PSTR("M500"));
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_NUM_KEY1:
      if (cnt <= 10) {
        key_value[cnt] = (char)'1';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY2:
      if (cnt <= 10) {
        key_value[cnt] = (char)'2';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY3:
      if (cnt <= 10) {
        key_value[cnt] = (char)'3';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY4:
      if (cnt <= 10) {
        key_value[cnt] = (char)'4';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY5:
      if (cnt <= 10) {
        key_value[cnt] = (char)'5';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY6:
      if (cnt <= 10) {
        key_value[cnt] = (char)'6';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY7:
      if (cnt <= 10) {
        key_value[cnt] = (char)'7';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY8:
      if (cnt <= 10) {
        key_value[cnt] = (char)'8';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY9:
      if (cnt <= 10) {
        key_value[cnt] = (char)'9';
        lv_label_set_text(labelValue, key_value);
        lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER, 0, 0);
        cnt++;
      }
      break;
    case ID_NUM_KEY0:
      if (cnt <= 10) {
        key_value[cnt] = (char)'0';
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

void lv_draw_number_key(void) {
  scr = lv_screen_create(NUMBER_KEY_UI, "");

  buttonValue = lv_btn_create(scr, 92, 40, 296, 40, event_handler, ID_NUM_KEY1, &style_num_text);
  labelValue = lv_label_create_empty(buttonValue);

  lv_obj_t *NumberKey_1 = lv_btn_create(scr, 92, 90, 68, 40, event_handler, ID_NUM_KEY1, &style_num_key_pre);
  lv_obj_t *labelKey_1 = lv_label_create_empty(NumberKey_1);
  lv_label_set_text(labelKey_1, machine_menu.key_1);
  lv_obj_align(labelKey_1, NumberKey_1, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_2 = lv_btn_create(scr, 168, 90, 68, 40, event_handler, ID_NUM_KEY2, &style_num_key_pre);
  lv_obj_t *labelKey_2 = lv_label_create_empty(NumberKey_2);
  lv_label_set_text(labelKey_2, machine_menu.key_2);
  lv_obj_align(labelKey_2, NumberKey_2, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_3 = lv_btn_create(scr, 244, 90, 68, 40, event_handler, ID_NUM_KEY3, &style_num_key_pre);
  lv_obj_t *labelKey_3 = lv_label_create_empty(NumberKey_3);
  lv_label_set_text(labelKey_3, machine_menu.key_3);
  lv_obj_align(labelKey_3, NumberKey_3, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_4 = lv_btn_create(scr, 92, 140, 68, 40, event_handler, ID_NUM_KEY4, &style_num_key_pre);
  lv_obj_t *labelKey_4 = lv_label_create_empty(NumberKey_4);
  lv_label_set_text(labelKey_4, machine_menu.key_4);
  lv_obj_align(labelKey_4, NumberKey_4, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_5 = lv_btn_create(scr, 168, 140, 68, 40, event_handler, ID_NUM_KEY5, &style_num_key_pre);
  lv_obj_t *labelKey_5 = lv_label_create_empty(NumberKey_5);
  lv_label_set_text(labelKey_5, machine_menu.key_5);
  lv_obj_align(labelKey_5, NumberKey_5, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_6 = lv_btn_create(scr, 244, 140, 68, 40, event_handler, ID_NUM_KEY6, &style_num_key_pre);
  lv_obj_t *labelKey_6 = lv_label_create_empty(NumberKey_6);
  lv_label_set_text(labelKey_6, machine_menu.key_6);
  lv_obj_align(labelKey_6, NumberKey_6, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_7 = lv_btn_create(scr, 92, 190, 68, 40, event_handler, ID_NUM_KEY7, &style_num_key_pre);
  lv_obj_t *labelKey_7 = lv_label_create_empty(NumberKey_7);
  lv_label_set_text(labelKey_7, machine_menu.key_7);
  lv_obj_align(labelKey_7, NumberKey_7, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_8 = lv_btn_create(scr, 168, 190, 68, 40, event_handler, ID_NUM_KEY8, &style_num_key_pre);
  lv_obj_t *labelKey_8 = lv_label_create_empty(NumberKey_8);
  lv_label_set_text(labelKey_8, machine_menu.key_8);
  lv_obj_align(labelKey_8, NumberKey_8, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_9 = lv_btn_create(scr, 244, 190, 68, 40, event_handler, ID_NUM_KEY9, &style_num_key_pre);
  lv_obj_t *labelKey_9 = lv_label_create_empty(NumberKey_9);
  lv_label_set_text(labelKey_9, machine_menu.key_9);
  lv_obj_align(labelKey_9, NumberKey_9, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *NumberKey_0 = lv_btn_create(scr, 92, 240, 68, 40, event_handler, ID_NUM_KEY0, &style_num_key_pre);
  lv_obj_t *labelKey_0 = lv_label_create_empty(NumberKey_0);
  lv_label_set_text(labelKey_0, machine_menu.key_0);
  lv_obj_align(labelKey_0, NumberKey_0, LV_ALIGN_CENTER, 0, 0);

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
