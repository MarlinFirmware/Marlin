#pragma once

#include <cstdint>

class PIDHandler {
    public:
        static void Init();

        static void HandleStartButton(DGUS_VP_Variable &var, void *val_ptr);

    public:
        static uint16_t cycles;
        static celsius_t calibration_temperature;
        static bool fan_on;
        static PGM_P result_message;

    private:

        static void SetStatusMessage(PGM_P statusMessage);
};

