/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/dgus/creality/AxisSettingsHandler.cpp
 *
 * Edit steps/mm, max acceleration, jerk and max feedrate per axis, plus the
 * driver current, stealthChop and hybrid threshold for TMC drivers.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "AxisSettingsHandler.h"
#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../ui_api.h"
#include "../../../../module/settings.h"

#if ALL(HAS_EXTRUDERS, EDITABLE_STEPS_PER_UNIT)
  #include "EstepsHandler.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "../../../../feature/tmc_util.h"
  #include "../../../../module/stepper/indirection.h"
#endif

AxisEnum AxisSettingsHandler::current_axis = X_AXIS;
uint16_t AxisSettingsHandler::axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_X;

float AxisSettingsHandler::axis_steps_mm;
uint16_t AxisSettingsHandler::max_acceleration_mm_per_s2;
float AxisSettingsHandler::jerk;
feedRate_t AxisSettingsHandler::max_feedrate;

bool AxisSettingsHandler::has_tmc_settings = ENABLED(HAS_TRINAMIC_CONFIG);

#if HAS_TRINAMIC_CONFIG

  uint16_t AxisSettingsHandler::tmc_current;
  bool AxisSettingsHandler::stealthchop;
  uint32_t AxisSettingsHandler::hybrid_threshold;

  // ExtUI only provides the driver current, so stepping mode and hybrid threshold go to the driver
  template<typename TMC>
  static void loadSteppingMode(TMC &st) {
    AxisSettingsHandler::stealthchop = st.get_stored_stealthChop();
    TERN_(HYBRID_THRESHOLD, AxisSettingsHandler::hybrid_threshold = st.get_pwm_thrs());
  }

  template<typename TMC>
  static void saveSteppingMode(TMC &st) {
    st.set_stealthChop(AxisSettingsHandler::stealthchop);
    TERN_(HYBRID_THRESHOLD, st.set_pwm_thrs(AxisSettingsHandler::hybrid_threshold));
  }

  static float getAxisCurrent_mA(const AxisEnum axis) {
    return TERN0(HAS_EXTRUDERS, axis == E_AXIS) ? ExtUI::getAxisCurrent_mA(ExtUI::E0) : ExtUI::getAxisCurrent_mA(ExtUI::axis_to_axis_t(axis));
  }

#endif // HAS_TRINAMIC_CONFIG

void AxisSettingsHandler::handleNavigation(DGUS_VP_Variable &var, void *val_ptr) {
  switch (BE16_P(val_ptr)) {
    case AXIS_SETTINGS_NAV_BUTTON_VAL_X:
      current_axis = X_AXIS;
      axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_X;
      break;

    case AXIS_SETTINGS_NAV_BUTTON_VAL_Y:
      current_axis = Y_AXIS;
      axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_Y;
      break;

    case AXIS_SETTINGS_NAV_BUTTON_VAL_Z:
      current_axis = Z_AXIS;
      axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_Z;
      break;

    #if HAS_EXTRUDERS
      case AXIS_SETTINGS_NAV_BUTTON_VAL_E:
        current_axis = E_AXIS;
        axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_E;
        break;
    #endif
  }

  // Load settings for axis
  #if HAS_EXTRUDERS
    if (current_axis == E_AXIS) {
      axis_steps_mm = ExtUI::getAxisSteps_per_mm(ExtUI::E0);
      max_acceleration_mm_per_s2 = ExtUI::getAxisMaxAcceleration_mm_s2(ExtUI::E0);
      TERN_(CLASSIC_JERK, jerk = ExtUI::getAxisMaxJerk_mm_s(ExtUI::E0));
      max_feedrate = ExtUI::getAxisMaxFeedrate_mm_s(ExtUI::E0);
    }
    else
  #endif
    {
      const ExtUI::axis_t axis = ExtUI::axis_to_axis_t(current_axis);
      axis_steps_mm = ExtUI::getAxisSteps_per_mm(axis);
      max_acceleration_mm_per_s2 = ExtUI::getAxisMaxAcceleration_mm_s2(axis);
      TERN_(CLASSIC_JERK, jerk = ExtUI::getAxisMaxJerk_mm_s(axis));
      max_feedrate = ExtUI::getAxisMaxFeedrate_mm_s(axis);
    }

  #if HAS_TRINAMIC_CONFIG
    const float mA = getAxisCurrent_mA(current_axis);
    tmc_current = isnan(mA) ? 0 : uint16_t(mA);

    switch (current_axis) {
      default: break;
      #if AXIS_HAS_STEALTHCHOP(X)
        case X_AXIS: loadSteppingMode(stepperX); break;
      #endif
      #if AXIS_HAS_STEALTHCHOP(Y)
        case Y_AXIS: loadSteppingMode(stepperY); break;
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z)
        case Z_AXIS: loadSteppingMode(stepperZ); break;
      #endif
      #if AXIS_HAS_STEALTHCHOP(E0)
        case E_AXIS: loadSteppingMode(stepperE0); break;
      #endif
    }
  #endif

  screen.gotoScreen(DGUS_SCREEN_AXIS_SETTINGS_AXIS);
}

void AxisSettingsHandler::handleBackNavigation(DGUS_VP_Variable &var, void *val_ptr) {
  // Save settings for axis
  #if HAS_EXTRUDERS
    if (current_axis == E_AXIS) {
      TERN_(EDITABLE_STEPS_PER_UNIT, ExtUI::setAxisSteps_per_mm(axis_steps_mm, ExtUI::E0));
      ExtUI::setAxisMaxAcceleration_mm_s2(max_acceleration_mm_per_s2, ExtUI::E0);
      TERN_(CLASSIC_JERK, ExtUI::setAxisMaxJerk_mm_s(jerk, ExtUI::E0));
      ExtUI::setAxisMaxFeedrate_mm_s(max_feedrate, ExtUI::E0);

      #if ENABLED(EDITABLE_STEPS_PER_UNIT)
        // The back button may end on the E-steps screen. Show that we didn't forget the settings.
        EstepsHandler::set_esteps = axis_steps_mm;
        EstepsHandler::calculated_esteps = axis_steps_mm;
      #endif
    }
    else
  #endif
    {
      const ExtUI::axis_t axis = ExtUI::axis_to_axis_t(current_axis);
      TERN_(EDITABLE_STEPS_PER_UNIT, ExtUI::setAxisSteps_per_mm(axis_steps_mm, axis));
      ExtUI::setAxisMaxAcceleration_mm_s2(max_acceleration_mm_per_s2, axis);
      TERN_(CLASSIC_JERK, ExtUI::setAxisMaxJerk_mm_s(jerk, axis));
      ExtUI::setAxisMaxFeedrate_mm_s(max_feedrate, axis);
    }

  #if HAS_TRINAMIC_CONFIG
    // ExtUI limits the current to 400-1500mA, so only apply a changed value
    const float mA = getAxisCurrent_mA(current_axis);
    if (!isnan(mA) && tmc_current != uint16_t(mA)) {
      #if HAS_EXTRUDERS
        if (current_axis == E_AXIS)
          ExtUI::setAxisCurrent_mA(tmc_current, ExtUI::E0);
        else
      #endif
          ExtUI::setAxisCurrent_mA(tmc_current, ExtUI::axis_to_axis_t(current_axis));
    }

    switch (current_axis) {
      default: break;
      #if AXIS_HAS_STEALTHCHOP(X)
        case X_AXIS: saveSteppingMode(stepperX); break;
      #endif
      #if AXIS_HAS_STEALTHCHOP(Y)
        case Y_AXIS: saveSteppingMode(stepperY); break;
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z)
        case Z_AXIS: saveSteppingMode(stepperZ); break;
      #endif
      #if AXIS_HAS_STEALTHCHOP(E0)
        case E_AXIS: saveSteppingMode(stepperE0); break;
      #endif
    }
  #endif

  // Save and pop
  screen.popToOldScreen();

  settings.request_autosave();
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH
