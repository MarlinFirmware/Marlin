
#pragma once

#include <cstdint>

class FilamentLoadUnloadHandler {
    public:
        static void Init();

        static void HandleTemperature(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleLoadUnloadButton(DGUS_VP_Variable &var, void *val_ptr);

    public:
        static celsius_t nozzle_temperature;
        static float length;
        
    private:
        static void ValidateTemperatures();
        static void ChangeFilamentWithTemperature(PGM_P command);
        static void SetStatusMessage(PGM_P statusMessage);
};

