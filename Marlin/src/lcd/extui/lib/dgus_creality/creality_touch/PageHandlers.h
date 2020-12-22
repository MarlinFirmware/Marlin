#pragma once

// Mapping of handlers per page. This construction is necessary because the CR-6 touch screen re-uses the same button IDs all over the place.
typedef void (*DGUS_CREALITY_SCREEN_BUTTON_HANDLER)(DGUS_VP_Variable &var, unsigned short buttonValue);

struct PageHandler {
  DGUSLCD_Screens ScreenID;
  DGUS_CREALITY_SCREEN_BUTTON_HANDLER Handler;
};

void DGUSCrealityDisplay_HandleReturnKeyEvent(DGUS_VP_Variable &var, void *val_ptr);