#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "FilamentLoadUnloadHandler.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"

#include "../../../../module/temperature.h"
#include "../../../../module/settings.h"
#include "../../../../module/planner.h"
#include "../../../../gcode/gcode.h"

celsius_t FilamentLoadUnloadHandler::nozzle_temperature = 0;
float FilamentLoadUnloadHandler::length = 0;

void FilamentLoadUnloadHandler::Init() {
    nozzle_temperature = ui.material_preset[0].hotend_temp;
    length = 150;

    if (ExtUI::isPrinting()) {
        nozzle_temperature = ExtUI::getTargetTemp_celsius(ExtUI::extruder_t::E0);
    }
}

void FilamentLoadUnloadHandler::HandleTemperature(DGUS_VP_Variable &var, void *val_ptr) {
    ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>(var, val_ptr);

    ValidateTemperatures();
}

void FilamentLoadUnloadHandler::HandleLoadUnloadButton(DGUS_VP_Variable &var, void *val_ptr) {
    // Common for load/unload -> determine minimum temperature
    if (length < 0.1) {
        SetStatusMessage("Invalid feed length");
        return;
    }

    if (ExtUI::isPrinting() && !ExtUI::isPrintingPaused()) {
        SetStatusMessage(PSTR("Please pause print first"));
        return;
    }
   
    DGUSSynchronousOperation syncOperation;
    uint16_t button_value = uInt16Value(val_ptr);
    switch (button_value) {
        case FILCHANGE_ACTION_LOAD_BUTTON:
            syncOperation.start();

            ChangeFilamentWithTemperature(PSTR("M701 L%s P0"));

            syncOperation.done();
        break;

        case FILCHANGE_ACTION_UNLOAD_BUTTON:
            syncOperation.start();

            ChangeFilamentWithTemperature(PSTR("M702 U%s"));

            syncOperation.done();
        break;
    }

}

void FilamentLoadUnloadHandler::ValidateTemperatures() {
    LIMIT(nozzle_temperature, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP - HOTEND_OVERSHOOT);
}

void FilamentLoadUnloadHandler::ChangeFilamentWithTemperature(PGM_P command) {
    // Heat if necessary
    if (ExtUI::getActualTemp_celsius(ExtUI::E0) < nozzle_temperature && abs(ExtUI::getActualTemp_celsius(ExtUI::E0) - nozzle_temperature) > THERMAL_PROTECTION_HYSTERESIS) {
        SetStatusMessage(PSTR("Heating up..."));

        uint16_t target_celsius = nozzle_temperature;
        NOMORE(target_celsius, thermalManager.hotend_max_target(0));

        thermalManager.setTargetHotend(target_celsius, ExtUI::H0);
        thermalManager.wait_for_hotend(ExtUI::H0, false);
    }

    // Inject load filament command
    SetStatusMessage(PSTR("Filament load/unload..."));

    char cmd[64];
    sprintf_P(cmd, command, length);
    
    // Handle commands
    SERIAL_ECHOPAIR("Injecting command: ", cmd);
    GcodeSuite::process_subcommands_now(cmd);
    SERIAL_ECHOPGM_P("- done");

    if (ScreenHandler.Settings.display_sound) ScreenHandler.Buzzer(500, 100);
    SetStatusMessage(PSTR("Filament load/unload complete"));
}

void FilamentLoadUnloadHandler::SetStatusMessage(PGM_P statusMessage) {
    ScreenHandler.setstatusmessagePGM(statusMessage);
}

#endif