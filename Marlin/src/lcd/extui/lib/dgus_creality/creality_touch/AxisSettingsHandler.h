#include <cstdint>

class AxisSettingsHandler {
    private:
        static AxisEnum current_axis;

    public:
        static uint16_t axis_settings_title_icon;

        static float axis_steps_mm;
        static uint16_t max_acceleration_mm_per_s2;

        static float jerk;
        static feedRate_t max_feedrate;

        static bool has_tmc_settings;
        static uint16_t tmc_current;
        static bool stealthchop;
        static uint32_t hybrid_threshold;

    public:
        static void HandleNavigation(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleBackNavigation(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleTMCNavigation(DGUS_VP_Variable &var, void *val_ptr);
};

