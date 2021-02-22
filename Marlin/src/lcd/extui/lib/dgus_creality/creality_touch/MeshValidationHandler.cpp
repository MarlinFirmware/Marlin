
#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "MeshValidationHandler.h"

#include "../../../ui_api.h"
#include "../../../../marlinui.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/planner.h"
#include "../../../../../gcode/gcode.h"

uint16_t MeshValidationHandler::nozzle_temperature;
uint16_t MeshValidationHandler::bed_temperature;
bool MeshValidationHandler::is_running;
bool MeshValidationHandler::was_running;
bool MeshValidationHandler::is_cancelling;

void MeshValidationHandler::Init() {
    // Set to PLA pre-heat temps by default
    nozzle_temperature = ui.material_preset[0].hotend_temp;
    bed_temperature = ui.material_preset[0].bed_temp;
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

    ScreenHandler.SetSynchronousOperationStart();
    is_running = true;
    was_running = true;

    // Home if necessary - do this synchronously
    queue.inject_P("G28 O U0");
    queue.advance();

    // Several commands being buffered here
    // - G26 with temperature and set for full bed
    // - Set absolute mode
    // - Present bed, high Z
    // - Disable stepper
    char gcodeBuffer[128] = {0};
    sprintf_P(gcodeBuffer, PSTR("G26 B%d H%d R\nG90\nG0 Y%d Z35 F3000\nM84"), bed_temperature, nozzle_temperature, (Y_BED_SIZE - 15));
    queue.inject(gcodeBuffer);

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
    if (!was_running) {
        ScreenHandler.PopToOldScreen();
    }

    // Reset state
    is_running = false;
    was_running = false;
    is_cancelling = false;

    ScreenHandler.SetSynchronousOperationFinish();
    ExtUI::resetCancelState();
}

void MeshValidationHandler::ValidateTemperatures() {
    LIMIT(nozzle_temperature, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP);
    LIMIT(bed_temperature, BED_MINTEMP, BED_MAXTEMP);
}

void MeshValidationHandler::SetStatusMessage(PGM_P statusMessage) {
    ScreenHandler.setstatusmessagePGM(statusMessage);
}

#endif