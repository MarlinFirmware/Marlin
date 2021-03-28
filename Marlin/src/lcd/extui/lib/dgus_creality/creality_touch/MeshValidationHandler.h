#include <cstdint>

class MeshValidationHandler {
    public:
        static void Init();

        static void HandleTemperature(DGUS_VP_Variable &var, void *val_ptr);

        static void HandleStartOrCancelButton(DGUS_VP_Variable &var, void *val_ptr);

        static void OnMeshValidationStart();
        static void OnMeshValidationFinish();

    public:
        static celsius_t nozzle_temperature;
        static celsius_t bed_temperature;

        static feedRate_t prev_feedrate;

        static bool is_cancelling;
        static bool is_running;
        static bool started_from_screen;

    private:
        static void Start();
        static void Cancel();

        static void ValidateTemperatures();
        static void SetStatusMessage(PGM_P statusMessage);
};

constexpr feedRate_t MESH_VALIDATION_PATTERN_FEEDRATE = G26_XY_FEEDRATE;
