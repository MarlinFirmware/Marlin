#pragma once

#include <cstdint>

class EstepsHandler {
    public:
        static void Init();

        static void HandleStartButton(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleApplyButton(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleBackButton(DGUS_VP_Variable &var, void *val_ptr);

        static void HandleRemainingFilament(DGUS_VP_Variable &var, void *val_ptr);

    public:
        static float set_esteps;
        static float calculated_esteps;

        static float mark_filament_mm;
        static float remaining_filament;

        static float filament_to_extrude;
        static celsius_t calibration_temperature;

    private:
        static void SaveSettingsAndReturn(bool fullConfirm);
        static void SetStatusMessage(PGM_P statusMessage);
};

