
#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "AxisSettingsHandler.h"

#include "../../../ui_api.h"
#include "../../../../marlinui.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/planner.h"
#include "../../../../../gcode/gcode.h"

AxisEnum AxisSettingsHandler::current_axis;
uint16_t AxisSettingsHandler::axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_X;

float AxisSettingsHandler::axis_steps_mm;
uint32_t AxisSettingsHandler::max_acceleration_mm_per_s2;

void AxisSettingsHandler::HandleNavigation(DGUS_VP_Variable &var, void *val_ptr) {
    switch (uInt16Value(val_ptr)) {
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

        case AXIS_SETTINGS_NAV_BUTTON_VAL_E:
            current_axis = E_AXIS;
            axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_E;
            break;
    }

    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_AXIS_SETTINGS_AXIS);

    // Load settings for axis
    axis_steps_mm = planner.settings.axis_steps_per_mm[current_axis];
    max_acceleration_mm_per_s2 = planner.settings.max_acceleration_mm_per_s2[current_axis];
}

void AxisSettingsHandler::HandleBackNavigation(DGUS_VP_Variable &var, void *val_ptr) {
    // Save settings for axis
    planner.settings.axis_steps_per_mm[current_axis] = axis_steps_mm;
    planner.settings.max_acceleration_mm_per_s2[current_axis] = max_acceleration_mm_per_s2;

    // Save and pop
    ScreenHandler.PopToOldScreen();

    settings.save();
}

#endif