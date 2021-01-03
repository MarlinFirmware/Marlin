
#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "PIDHandler.h"

#include "../../../ui_api.h"
#include "../../../../marlinui.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/planner.h"
#include "../../../../../gcode/gcode.h"

// Storage init
uint16_t PIDHandler::cycles = 0;
uint16_t PIDHandler::calibration_temperature = 0;
PGM_P PIDHandler::result_message = nullptr;

void PIDHandler::Init() {
    // Reset
    cycles = 3;

    // Use configured PLA temps + 10 degrees
    calibration_temperature = ui.material_preset[0].hotend_temp + 15;

    // Welcome message
    SetStatusMessage(PSTR("Ready"));
}


void PIDHandler::HandleStartButton(DGUS_VP_Variable &var, void *val_ptr) {
    static_assert(ADVANCED_PAUSE_PURGE_LENGTH == 0, "Assuming PURGE_LENGTH is 0 so we can use M701");

    // Validate
    if (calibration_temperature < EXTRUDE_MINTEMP) {
        SetStatusMessage(PSTR("Invalid temperature set"));
        return;
    }

    if (cycles < 1) {
        SetStatusMessage(PSTR("Invalid number of cycles"));
        return;
    }

    // Set-up command
    SetStatusMessage(PSTR("PID tuning. Please wait..."));

    char cmd[64];
    sprintf_P(cmd, PSTR("M303 S%d C%d U1"), calibration_temperature, cycles);

    ExtUI::injectCommands(cmd);
    queue.advance();
    planner.synchronize();

    // Done
    ScreenHandler.Buzzer(0, 250);
    settings.save();

    SetStatusMessage(result_message);
}

void PIDHandler::SetStatusMessage(PGM_P statusMessage) {
    ScreenHandler.setstatusmessagePGM(statusMessage);
}

#endif