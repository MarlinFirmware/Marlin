#include <cstdint>

class AxisSettingsHandler {
    private:
        static AxisEnum current_axis;

    public:
        static uint16_t axis_settings_title_icon;

        static float axis_steps_mm;
        static uint32_t max_acceleration_mm_per_s2;

        static float jerk;

    public:
        static void HandleNavigation(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleBackNavigation(DGUS_VP_Variable &var, void *val_ptr);

};

