#include <cstdint>

class AxisSettingsHandler {
    private:
        static AxisEnum current_axis;

    public:
        static uint16_t axis_settings_title_icon;

    public:
        static void HandleNavigation(DGUS_VP_Variable &var, void *val_ptr);
        static void HandleBackNavigation(DGUS_VP_Variable &var, void *val_ptr);

};

