
#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "EstepsHandler.h"
#include "AxisSettingsHandler.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"

#include "../../../../module/temperature.h"
#include "../../../../module/settings.h"
#include "../../../../module/planner.h"
#include "../../../../gcode/gcode.h"

#if HAS_TRINAMIC_CONFIG
#include "../../../../feature/tmc_util.h"
#include "../../../../module/stepper/indirection.h"
#endif

AxisEnum AxisSettingsHandler::current_axis;
uint16_t AxisSettingsHandler::axis_settings_title_icon = ICON_AXIS_SETTINGS_TITLE_X;

float AxisSettingsHandler::axis_steps_mm;
uint16_t AxisSettingsHandler::max_acceleration_mm_per_s2;
float AxisSettingsHandler::jerk;
feedRate_t AxisSettingsHandler::max_feedrate;

#if HAS_TRINAMIC_CONFIG
bool AxisSettingsHandler::has_tmc_settings = true;
#else 
bool AxisSettingsHandler::has_tmc_settings = false;
#endif

uint16_t AxisSettingsHandler::tmc_current;
bool AxisSettingsHandler::stealthchop;
uint32_t AxisSettingsHandler::hybrid_threshold;

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

    // Load settings for axis
    axis_steps_mm = planner.settings.axis_steps_per_mm[current_axis];
    max_acceleration_mm_per_s2 = static_cast<uint16_t>(planner.settings.max_acceleration_mm_per_s2[current_axis]);
    IF_ENABLED(CLASSIC_JERK, jerk = planner.max_jerk[current_axis]);
    max_feedrate = planner.settings.max_feedrate_mm_s[current_axis];

    #if HAS_TRINAMIC_CONFIG
    switch (current_axis){
        #if AXIS_IS_TMC(X)
        case X_AXIS:
            tmc_current = stepperX.getMilliamps();

            #if AXIS_HAS_STEALTHCHOP(X)
            stealthchop = stepperX.get_stored_stealthChop();

            #if ENABLED(HYBRID_THRESHOLD)
            hybrid_threshold = static_cast<uint32_t>(stepperX.get_pwm_thrs());
            #endif
            #endif
            break;
        #endif

        #if AXIS_IS_TMC(Y)
        case Y_AXIS:
            tmc_current = stepperY.getMilliamps();

            #if AXIS_HAS_STEALTHCHOP(Y)
            stealthchop = stepperY.get_stored_stealthChop();

            #if ENABLED(HYBRID_THRESHOLD)
            hybrid_threshold = static_cast<uint32_t>(stepperY.get_pwm_thrs());
            #endif
            #endif
            break;
        #endif

        #if AXIS_IS_TMC(Z)
        case Z_AXIS:
            tmc_current = stepperZ.getMilliamps();

            #if AXIS_HAS_STEALTHCHOP(Z)
            stealthchop = stepperZ.get_stored_stealthChop();

            #if ENABLED(HYBRID_THRESHOLD)
            hybrid_threshold = static_cast<uint32_t>(stepperZ.get_pwm_thrs());
            #endif
            #endif
            break;
        #endif

        #if AXIS_IS_TMC(E0)
        case E_AXIS:
            tmc_current = stepperE0.getMilliamps();

            #if AXIS_HAS_STEALTHCHOP(E0)
            stealthchop = stepperE0.get_stored_stealthChop();

            #if ENABLED(HYBRID_THRESHOLD)
            hybrid_threshold = static_cast<uint32_t>(stepperE0.get_pwm_thrs());
            #endif
            #endif
            break;
        #endif

        default:
        // Too bad
        break;
    }
    #endif

    // Nav
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_AXIS_SETTINGS_AXIS);
}

void AxisSettingsHandler::HandleBackNavigation(DGUS_VP_Variable &var, void *val_ptr) {
    // Save settings for axis
    planner.settings.axis_steps_per_mm[current_axis] = axis_steps_mm;
    planner.settings.max_acceleration_mm_per_s2[current_axis] = max_acceleration_mm_per_s2;
    IF_ENABLED(CLASSIC_JERK, planner.max_jerk[current_axis] = jerk);
    planner.settings.max_feedrate_mm_s[current_axis] = max_feedrate;

    // If we're handling the E-axis, the back button might end on that screen. Show that we didn't forget the settings.
    if (current_axis == E_AXIS) {
        EstepsHandler::set_esteps = axis_steps_mm;
        EstepsHandler::calculated_esteps = axis_steps_mm;
    }

    #if HAS_TRINAMIC_CONFIG
    switch (current_axis){
        #if AXIS_IS_TMC(X)
        case X_AXIS:
            stepperX.rms_current(tmc_current);

            #if AXIS_HAS_STEALTHCHOP(X)
            stepperX.set_stealthChop(stealthchop);

            #if ENABLED(HYBRID_THRESHOLD)
            stepperX.set_pwm_thrs(hybrid_threshold);
            #endif
            #endif
            break;
        #endif

        #if AXIS_IS_TMC(Y)
        case Y_AXIS:
            stepperY.rms_current(tmc_current);

            #if AXIS_HAS_STEALTHCHOP(Y)
            stepperY.set_stealthChop(stealthchop);

            #if ENABLED(HYBRID_THRESHOLD)
            stepperY.set_pwm_thrs(hybrid_threshold);
            #endif
            #endif
            break;
        #endif

        #if AXIS_IS_TMC(Z)
        case Z_AXIS:
            stepperZ.rms_current(tmc_current);

            #if AXIS_HAS_STEALTHCHOP(Z)
            stepperZ.set_stealthChop(stealthchop);

            #if ENABLED(HYBRID_THRESHOLD)
            stepperZ.set_pwm_thrs(hybrid_threshold);
            #endif
            #endif
            break;
        #endif

        #if AXIS_IS_TMC(E0)
            case E_AXIS:
            stepperE0.rms_current(tmc_current);

            #if AXIS_HAS_STEALTHCHOP(E0)
            stepperE0.set_stealthChop(stealthchop);

            #if ENABLED(HYBRID_THRESHOLD)
            stepperE0.set_pwm_thrs(hybrid_threshold);
            #endif
            #endif
            break;
        #endif

        default:
        // Too bad
        break;
    }
    #endif

    // Save and pop
    ScreenHandler.PopToOldScreen();

    settings.save();
}

void AxisSettingsHandler::HandleTMCNavigation(DGUS_VP_Variable &var, void *val_ptr) {
    #if HAS_TRINAMIC_CONFIG
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_AXIS_SETTINGS_TMC);
    #endif
}

#endif