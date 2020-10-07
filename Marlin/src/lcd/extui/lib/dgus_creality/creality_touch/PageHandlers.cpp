#include "../../../../../inc/MarlinConfigPre.h"

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/motion.h"
#include "../../../../../module/planner.h"

#include "../../../../ultralcd.h"
#include "../../../ui_api.h"

#include "PageHandlers.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

// Definitions of page handlers

void MainMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (buttonValue) {
        case 1: // "Print"
            DGUSScreenHandler::GotoScreen(DGUSLCD_Screens::DGUSLCD_SCREEN_SDFILELIST_1);

            // TODO: update SD card info
            break;

        case 2: // "Prepare"
            DGUSScreenHandler::GotoScreen(DGUSLCD_Screens::DGUSLCD_SCREEN_PREPARE);
            break;

        case 3: // "Control"
            DGUSScreenHandler::GotoScreen(DGUSLCD_Screens::DGUSLCD_SCREEN_CONTROL);
            break;

        case 4: // "Level"
            DGUSScreenHandler::GotoScreen(DGUSLCD_Screens::DGUSLCD_SCREEN_ZOFFSET_LEVEL);
            break;
    }
}

void ControlMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_PREPAREENTERKEY: 
            switch(buttonValue) {
                case 5: // About
                    // TODO: update website 
                    // TODO: PageSwitch is set at DWIN side - should switch to about menu automatically
                    break;

                case 7: // Reset to factory settings
                    // TODO: Reset
                    break;

                case 9: // Back button
                    // TODO: should navigate automatically
                    break;
            }
            break;

        case VP_BUTTON_ADJUSTENTERKEY:
            // Switch LED ON/OFF
            if(LEDStatus == false)
            {
                digitalWrite(LED_CONTROL_PIN, LOW);
                LEDStatus = false;
            }
            else
            {
                // Turn off the LED
                digitalWrite(LED_CONTROL_PIN, LOW);
                LEDStatus = true;
            }
            break;
    }
}

void TempMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_ADJUSTENTERKEY:
            switch (buttonValue) {
                case 3:
                    if (thermalManager.fan_speed[0] == 0) {
                        thermalManager.fan_speed[0] = 255;
                    } else {
                        thermalManager.fan_speed[0] = 0;
                    }
                break;
            }

            break;

        case VP_BUTTON_TEMPCONTROL:
            switch (buttonValue) {
                case 3:
                    thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
                    thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
                    break;

                case 4:
                    thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
                    thermalManager.setTargetBed(ui.material_preset[1].bed_temp);
                    break;
            }
            break;
    }
}

// Register the page handlers
#define PAGE_HANDLER(SCRID, HDLRPTR) { .ScreenID=SCRID, .Handler=HDLRPTR },
const struct PageHandler PageHandlers[] PROGMEM = {
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_MAIN, MainMenuHandler)
    
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_CONTROL, ControlMenuHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_TEMP, TempMenuHandler)

    // Terminating
    PAGE_HANDLER(static_cast<DGUSLCD_Screens>(0) ,0)
};

void DGUSCrealityDisplay_HandleReturnKeyEvent(DGUS_VP_Variable &var, void *val_ptr) {
  const struct PageHandler *map = PageHandlers;
  const uint16_t *ret;
  const DGUSLCD_Screens current_screen = DGUSScreenHandler::getCurrentScreen();

  while ((ret = (uint16_t*) pgm_read_ptr(&(map->Handler)))) {
    if ((map->ScreenID) == current_screen) {
        map->Handler(var, *static_cast<unsigned short*>(val_ptr));
        return;
    }
  }
}

#endif