
#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "MeshValidationHandler.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"

#include "../../../../module/temperature.h"
#include "../../../../module/settings.h"
#include "../../../../module/planner.h"
#include "../../../../gcode/gcode.h"

celsius_t MeshValidationHandler::nozzle_temperature;
celsius_t MeshValidationHandler::bed_temperature;
bool MeshValidationHandler::is_running;
bool MeshValidationHandler::started_from_screen;
bool MeshValidationHandler::is_cancelling;
feedRate_t MeshValidationHandler::prev_feedrate;

void MeshValidationHandler::Init() {
    // Set to PLA pre-heat temps by default
    nozzle_temperature = ui.material_preset[0].hotend_temp;
    bed_temperature = ui.material_preset[0].bed_temp;

    ValidateTemperatures();
}

void MeshValidationHandler::HandleTemperature(DGUS_VP_Variable &var, void *val_ptr) {
    ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>(var, val_ptr);

    ValidateTemperatures();
}

void MeshValidationHandler::HandleStartOrCancelButton(DGUS_VP_Variable &var, void *val_ptr) {
    if (!is_running) {
        Start();
    } else {
        Cancel();
    }
}

void MeshValidationHandler::Start() {
    if (is_running) return;

    // Validate
    if (!ExtUI::getMeshValid()) {
        SetStatusMessage("Please level bed first");
        return;
    }

    // Block
    ScreenHandler.SetSynchronousOperationStart();
    is_running = true;
    started_from_screen = true;

    // Home if necessary - do this synchronously
    if (!all_axes_trusted()) {
        queue.inject_P("G28 U0");
        queue.advance();
    }

    // Home X and Y so we droop at the side of the bed.
    // G26 with temperature and set for full bed, full pattern, retract 4mm, prime 5mm
    char gcodeBuffer[128] = {0};
    sprintf_P(gcodeBuffer, PSTR("G90\nG0 X0\nG26 B%d H%d R Q%d P2 X%d Y%d"), bed_temperature, nozzle_temperature, int(G26_RETRACT_MULTIPLIER), X_BED_SIZE / 2, Y_BED_SIZE / 2);
    queue.inject(gcodeBuffer);
    queue.advance();

    // Set feedrate
    prev_feedrate = ExtUI::getFeedrate_mm_s();
    ExtUI::setFeedrate_mm_s(MESH_VALIDATION_PATTERN_FEEDRATE);

    SetStatusMessage("Starting...");
}

void MeshValidationHandler::Cancel() {
    if (is_cancelling) return;

    is_cancelling = true;
    ExtUI::setCancelState();

    SetStatusMessage("Cancelling...");
}

void MeshValidationHandler::OnMeshValidationStart() {
    // Note: can also be called when manually invoking G26
    if (ScreenHandler.getCurrentScreen() != DGUSLCD_SCREEN_MESH_VALIDATION) {
        ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MESH_VALIDATION);
    }

    // Set state
    ScreenHandler.SetSynchronousOperationStart();
    is_running = true;
}

void MeshValidationHandler::OnMeshValidationFinish() {
    // If invoked externally, pop back
    if (!started_from_screen) {
        ScreenHandler.PopToOldScreen();
    }

    if (started_from_screen) {
        ExtUI::setFeedrate_mm_s(prev_feedrate);

        char gcodeBuffer[128] = {0};
        if (!is_cancelling) {
            // Present
            // - Set absolute mode
            // - Present bed, high Z
            // - Disable stepper
            strcpy_P(gcodeBuffer, PSTR("M84"));

            SetStatusMessage("Mesh validation pattern printed");
        } else {
            // Park and disable steppers
            strcpy_P(gcodeBuffer, PSTR("G0 X5 F2000\nG27\nM84"));

            SetStatusMessage("Canceled mesh validation pattern");
        }

        // Enqueue
        gcode.process_subcommands_now(gcodeBuffer);
    }

    // Reset state
    is_running = false;
    started_from_screen = false;
    is_cancelling = false;

    ScreenHandler.SetSynchronousOperationFinish();
    ExtUI::resetCancelState();
}

void MeshValidationHandler::ValidateTemperatures() {
    LIMIT(nozzle_temperature, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP - HOTEND_OVERSHOOT);
    LIMIT(bed_temperature, 40 /*Hardcoded minimum for G26, apparently*/, BED_MAXTEMP - BED_OVERSHOOT);
}

void MeshValidationHandler::SetStatusMessage(PGM_P statusMessage) {
    ScreenHandler.setstatusmessagePGM(statusMessage);
}

#endif
