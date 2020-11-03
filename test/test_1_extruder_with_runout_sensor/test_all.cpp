/* START_CONFIGURATION

opt_enable FILAMENT_RUNOUT_SENSOR;
opt_set FIL_RUNOUT_PIN 4;
opt_enable ADVANCED_PAUSE_FEATURE;
opt_enable EMERGENCY_PARSER;
opt_enable NOZZLE_PARK_FEATURE;

END_CONFIGURATION */
#include "tests/marlin_tests.cpp"
#include "tests/common_for/any_configuration.cpp"
#include "tests/runout/runout_1_extruder.cpp"

MAIN_FOR_MARLIN_TESTS();
