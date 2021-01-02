
#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "EstepsHandler.h"

#include "../../../ui_api.h"
#include "../../../../marlinui.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/planner.h"
#include "../../../../../gcode/gcode.h"

// Storage init
float EstepsHandler::set_esteps = 0;
float EstepsHandler::calculated_esteps = 0;
float EstepsHandler::remaining_filament = 0;
float EstepsHandler::filament_to_extrude = 0;
uint16_t EstepsHandler::calibration_temperature = 0;

void EstepsHandler::Init() {
    // Use steps
    set_esteps = ExtUI::getAxisSteps_per_mm(ExtUI::E0);
    calculated_esteps = 0;

    // Reset
    filament_to_extrude = 120;
    remaining_filament = 0;

    // Use configured PLA temps + 10 degrees
    calibration_temperature = ui.material_preset[0].hotend_temp + 10;

    // Welcome message
    SetStatusMessage(PSTR("Ready"));
}


void EstepsHandler::HandleStartButton(DGUS_VP_Variable &var, void *val_ptr) {
    static_assert(ADVANCED_PAUSE_PURGE_LENGTH == 0, "Assuming PURGE_LENGTH is 0 so we can use M701");

    // Validate
    if (calibration_temperature < EXTRUDE_MINTEMP) {
        SetStatusMessage(PSTR("Invalid temperature set"));
        return;
    }

    if (filament_to_extrude < 10) {
        SetStatusMessage(PSTR("Invalid extrusion length set"));
        return;
    }

    // Prepare
    bool zAxisWasRelative = GcodeSuite::axis_is_relative(Z_AXIS);
    bool eAxisWasRelative = GcodeSuite::axis_is_relative(E_AXIS);

    ExtUI::injectCommands_P("G0 Z5 F150");
    queue.advance();

    // Heat up if necessary
    if (ExtUI::getActualTemp_celsius(ExtUI::E0) < calibration_temperature && abs(ExtUI::getActualTemp_celsius(ExtUI::E0) - calibration_temperature) > 2) {
        thermalManager.setTargetHotend(calibration_temperature, ExtUI::H0);

        SetStatusMessage(PSTR("Heating up..."));
        thermalManager.wait_for_hotend(ExtUI::H0, false);
    }

    planner.synchronize();
   
    // Set-up command
    SetStatusMessage(PSTR("Extruding..."));

    char cmd[64];
    sprintf_P(cmd, PSTR("G1 E%f F50"), filament_to_extrude);

    ExtUI::injectCommands(cmd);
    queue.advance();
    planner.synchronize();

    // Restore position
    ExtUI::injectCommands_P("G0 Z-5 F150");
    queue.advance();
    planner.synchronize();

    // Restore defaults
    if (!zAxisWasRelative) GcodeSuite::set_relative_mode(false);
    if (!eAxisWasRelative) GcodeSuite::set_e_absolute();

    // Done
    ScreenHandler.Buzzer(0, 250);
    SetStatusMessage(PSTR("Please measure remaining filament"));
}

void EstepsHandler::HandleApplyButton(DGUS_VP_Variable &var, void *val_ptr) {
    if (abs(calculated_esteps) < 1) {
        // User intented to set e-steps directly
        ExtUI::setAxisSteps_per_mm(set_esteps, ExtUI::E0);
    } else {
        ExtUI::setAxisSteps_per_mm(calculated_esteps, ExtUI::E0);
    }

    // Save & reset
    settings.save();
    Init();

    ScreenHandler.Buzzer(0, 250);
    SetStatusMessage(PSTR("New e-steps value saved"));
}

void EstepsHandler::HandleRemainingFilament(DGUS_VP_Variable &var, void *val_ptr) {
    ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>(var, val_ptr);

    // Calculate
    float actualExtrusion = filament_to_extrude - remaining_filament;
    float current_steps = ExtUI::getAxisSteps_per_mm(ExtUI::E0);
    float new_steps = actualExtrusion = 0 ? current_steps : (current_steps / actualExtrusion) * 100;

    calculated_esteps = new_steps;

    // Status update
    SetStatusMessage(PSTR("Calculated new e-steps"));
}

void EstepsHandler::SetStatusMessage(PGM_P statusMessage) {
    ScreenHandler.setstatusmessagePGM(statusMessage);
}

#endif